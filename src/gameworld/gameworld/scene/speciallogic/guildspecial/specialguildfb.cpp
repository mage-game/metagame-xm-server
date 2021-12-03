#include "specialguildfb.hpp"

#include "engineadapter.h"
#include "world.h"

#include "globalconfig/globalconfig.h"
#include "obj/character/monster.h"
#include "obj/otherobj/bigcoinobj.hpp"
#include "obj/otherobj/gatherobj.h"
#include "monster/monsterpool.h"
#include "internalcomm.h"
#include "obj/character/role.h"
#include "monster/drop/droppool.hpp"
#include "gamecommon.h"
#include "other/route/guildroute.hpp"
#include "scene/guildshadow/guildshadow.hpp"
#include "servercommon/noticenum.h"

#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/guildfbconfig.hpp"
#include "config/monsterreplaceconfig.hpp"
#include "servercommon/string/gameworldstr.h"
#include "item/itempool.h"
#include "other/route/mailroute.hpp"

#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectwudiprotect.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "monster/monsterpool.h"
#include "servercommon/errornum.h"

#include "protocal/msgfight.h"
#include "protocal/msgfb.h"

#include "servercommon/guilddef.hpp"
#include "servercommon/activitydef.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "other/event/eventhandler.hpp"
#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"

#include "ai/monsterai/specialai/monsteraiguildfbguard.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "gameworld/protocal/msgchatmsg.h"

#include <algorithm>

SpecialGuildFb::SpecialGuildFb(Scene *scene)
	: SpecialLogic(scene), m_is_open(false), m_is_pass(false), m_is_finish(false), m_guard_boss_objid(INVALID_OBJ_ID), m_fb_index(0),
	m_fb_index_enemy_count(0), m_fb_index_enemy_max(0), m_next_fb_index_time(0), m_wave_index(0), m_wave_enemy_count(0), m_guard_boss_pos_idx(0), m_last_sync_time(0), m_kick_role_time(0)
{

}

SpecialGuildFb::~SpecialGuildFb()
{

}

bool SpecialGuildFb::CanDestroy()
{
	int fb_index_count = LOGIC_CONFIG->GetGuildFbConfig().GetGuardWaveCount();

	if (m_fb_index < fb_index_count && m_is_finish)
	{
		return true;
	}

	return ActivityShadow::Instance().IsActivtyClose(ACTIVITY_TYPE_GUILD_FB);
}

void SpecialGuildFb::OnRoleEnterScene(Role *role)
{
	{
		role->GetRoleActivity()->SetFirstEnterGuildFb(m_scene->GetSceneKey());
	}

	this->SyncInfoToRole(role, Protocol::SCGuildFBInfo::NOTIFY_REASON_ENTER);

	EventHandler::Instance().OnEnterGuildFb(role);
}

void SpecialGuildFb::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (m_is_open && !m_is_finish)
	{
		if (m_next_fb_index_time > 0 && now_second >= m_next_fb_index_time) // 进入下一关
		{
			m_next_fb_index_time = 0;
			this->OnCreateNextFbIndex();
		}

		// 检测守卫bossAI移动
		this->UpdateGuardBossAI();

		// 定时同步玩家信息
		if (now_second >= m_last_sync_time + 3) 
		{
			m_last_sync_time = now_second;
			this->SyncInfoToRole(NULL, Protocol::SCGuildFBInfo::NOTIFY_REASON_UPDATE);
		}
	}

	// 踢人
	if (m_kick_role_time != 0 && (unsigned int)now_second > m_kick_role_time)
	{
		this->DelayKickOutAllRole();
	}
}

void SpecialGuildFb::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
	Obj *obj = m_scene->GetObj(enemy_id);
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role *)obj;

		// 计算玩家伤害值
		Guild *guild = this->GetOwnerGuild();
		if (nullptr != guild)
		{
			guild->CalcRoleHurtMonsterVal(role, delta_hp);
		}	
	}
}

void SpecialGuildFb::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (m_is_finish) return;

	if (NULL == dead) return;

	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster *monster = (Monster *)dead;

		if (monster->GetId() == m_guard_boss_objid)  // 守卫挂了
		{
			this->OnGuardBossDead();
		}
		else  // 敌人挂了一个
		{
			this->OnEnemyDead();
		}
	}
}

bool SpecialGuildFb::OnTimeout()
{
	// 副本时间结束，但是通过guild来控制的OnGuildFbClose也会调用到
	// 所以这里不返回false给scene::update踢人

	return true;
}

bool SpecialGuildFb::SpecialRelive(Role *role, int realive_type)
{
	// 因为复活点不是使用场景的默认复活点，所以重载了这个函数，仅仅对复活点复活进行处理
	// 复活到这一关卡的第一个点

	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)		
	{
		Posi birth_pos(0, 0);
		if (this->GetCurFbIndexFirstPos(&birth_pos))
		{
			if (role->ReAliveTargetScene(m_scene->GetSceneID(), birth_pos))
			{
				return true;
			}
		}
	}

	return false;
}

void SpecialGuildFb::OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper)
{
	if (NULL == role || NULL == falling_item || NULL == item_wrapper)
	{
		return;
	}

	Protocol::SCGuildFBRewardRecordInfo::ItemInfo item;
	item.reward_item_id = item_wrapper->item_id;
	item.reward_item_num = item_wrapper->num;
	item.item_color = 0;

	const ItemBase *item_base = ITEMPOOL->GetItem(item_wrapper->item_id);
	if (nullptr != item_base && ItemBase::I_TYPE_EQUIPMENT == item_base->GetItemType())
	{
		item.item_color = item_base->GetColor();
	}

	this->AddCurIndexRewardRecord(role, item);

	Guild *guild = this->GetOwnerGuild();
	if (nullptr != guild)
	{
		guild->AddHistoryRewardRecord(role, item);
	}
}

bool SpecialGuildFb::SpecialIsEnemy(Role *role, Obj *obj)
{
	switch (obj->GetObjType())	
	{
	case Obj::OBJ_TYPE_ROLE:
		{
			return false;
		}
		break;

	case Obj::OBJ_TYPE_MONSTER:
		{
			if (obj->GetId() == m_guard_boss_objid)
			{
				return false;
			}

			Monster *monster = (Monster*)obj;
			return monster->IsAlive();
		}
		break;
	}

	return SpecialLogic::SpecialIsEnemy(role, obj);
}

bool SpecialGuildFb::SpecialIsFriend(Role *role, Obj *obj, bool check_alive)
{
	switch (obj->GetObjType())	
	{
	case Obj::OBJ_TYPE_ROLE:
		{
			Role *friend_role = (Role*)obj;
			return (!check_alive || friend_role->IsAlive());
		}
		break;

	case Obj::OBJ_TYPE_MONSTER:
		{
			return false;
		}
		break;
	}

	return SpecialLogic::SpecialIsFriend(role, obj, check_alive);
}

void SpecialGuildFb::OnGuildFbOpen()
{
	m_is_open = true;

	m_is_pass = false;
	m_is_finish = false;

	m_guard_boss_objid = INVALID_OBJ_ID;
	
	m_fb_index = 0;
	m_fb_index_enemy_count = 0;
	m_fb_index_enemy_max = 0;
	m_next_fb_index_time = EngineAdapter::Instance().Time();

	m_guard_boss_pos_idx = 0;

	m_last_sync_time = 0;
}

void SpecialGuildFb::OnGuildFbClose()
{
	this->OnFbFinish(false);
}

bool SpecialGuildFb::GetCurFbIndexFirstPos(Posi *get_pos)
{
	Posi birth_pos(0, 0);
	if (LOGIC_CONFIG->GetGuildFbConfig().GetGuardPathPos(m_fb_index, 0, &birth_pos))
	{
		if (NULL != get_pos) *get_pos = birth_pos;
			
		return true;
	}
	
	return false;
}

bool SpecialGuildFb::GetGuardBossPos(Posi *get_pos)
{
	Obj *obj = m_scene->GetObj(m_guard_boss_objid);
	if (NULL != obj && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		if (NULL != get_pos) *get_pos = obj->GetPos();

		return true;
	}

	return false;
}

void SpecialGuildFb::AddCurIndexRewardRecord(Role *role, const Protocol::SCGuildFBRewardRecordInfo::ItemInfo &reward_item)
{
	if (nullptr == role)
	{
		return;
	}

	auto it = m_cur_wave_reward_record_map.find(role->GetUID());
	if (it != m_cur_wave_reward_record_map.end())
	{
		// 装备不叠加
		if (reward_item.item_color > 0)
		{
			it->second.emplace_back(reward_item);
		}
		else
		{
			auto vec_it = std::find_if(it->second.begin(), it->second.end(), [reward_item](const Protocol::SCGuildFBRewardRecordInfo::ItemInfo &cmp_item) {
				if (cmp_item.reward_item_id == reward_item.reward_item_id)
				{
					return true;
				}
				else
				{
					return false;
				}
			});

			if (vec_it != it->second.end())
			{
				(*vec_it).reward_item_num += reward_item.reward_item_num;
			}
			else
			{
				it->second.emplace_back(reward_item);
			}
		}

	}
	else
	{
		std::vector<Protocol::SCGuildFBRewardRecordInfo::ItemInfo> cur_vec;
		cur_vec.emplace_back(reward_item);
		m_cur_wave_reward_record_map[role->GetUID()] = cur_vec;
	}
}

void SpecialGuildFb::GiveFbIndexRewardToSceneRole()
{
	GuildFbWaveCfg *guildfb_wavecfg = LOGIC_CONFIG->GetGuildFbConfig().GetGuildFbWaveCfg(m_fb_index);
	if (NULL == guildfb_wavecfg)
	{
		return;
	}

	for (unsigned int i = 0; i < m_scene->RoleNum(); i ++)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			role->GetKnapsack()->PutList(guildfb_wavecfg->reward_count, guildfb_wavecfg->reward_item_list, PUT_REASON_GUILD_FB_WAVE_REWARD);
			//加帮派贡献
			role->AddGuildGongxian(guildfb_wavecfg->reward_gongxian, "OnGuildGuildFBWaveReward");

			int max_reward_count = static_array_size(guildfb_wavecfg->reward_item_list);
			for (int count = 0; count < guildfb_wavecfg->reward_count && count < max_reward_count; count ++)
			{
				Protocol::SCGuildFBRewardRecordInfo::ItemInfo item;
				item.reward_item_id = guildfb_wavecfg->reward_item_list[count].item_id;
				item.reward_item_num = guildfb_wavecfg->reward_item_list[count].num;
				item.item_color = 0;

				//添加奖励记录
				this->AddCurIndexRewardRecord(role, item);

				Guild *guild = this->GetOwnerGuild();
				if (nullptr != guild)
				{
					guild->AddHistoryRewardRecord(role, item);
				}
			}	
	
			this->SendCurWaveRewardInfo(role);
		}
	}
}

void SpecialGuildFb::OnGuardBossDead()
{
	m_guard_boss_objid = INVALID_OBJ_ID;

	this->OnFbFinish(false);
	GuildRoute::Instance().OnGuildFbFinish(m_scene->GetSceneKey(), false);
}

void SpecialGuildFb::OnEnemyDead()
{
	// 1、当前关卡的怪被干完了，还有新关卡就继续++，并通知客户端等待下一关卡的计算
	// 2、当前关卡的怪被干完了，没有关卡了，那么就结束副本

	--m_wave_enemy_count;
	-- m_fb_index_enemy_count;
	
	if (m_fb_index_enemy_count <= 0) 
	{
		// 发放每关卡奖励
		this->GiveFbIndexRewardToSceneRole();


		++ m_fb_index;
		m_wave_index = 0;
		if (m_fb_index >= LOGIC_CONFIG->GetGuildFbConfig().GetGuardWaveCount())
		{
			this->OnFbFinish(true);
			GuildRoute::Instance().OnGuildFbFinish(m_scene->GetSceneKey(), true);
		}
		else 
		{	
			m_next_fb_index_time = EngineAdapter::Instance().Time() + LOGIC_CONFIG->GetGuildFbConfig().GetGuardWaitWaveTime();

			this->SyncInfoToRole(NULL, Protocol::SCGuildFBInfo::NOTIFY_REASON_WAIT);
		}
	}
	else if(m_wave_enemy_count <= 0)
	{
		// 刷新波的怪
		++ m_wave_index;
		this->FlushWaveEnemyMonster();
	}
	
	this->SyncInfoToRole(NULL, Protocol::SCGuildFBInfo::NOTIFY_REASON_UPDATE);
}

void SpecialGuildFb::UpdateGuardBossAI()
{
	if (INVALID_OBJ_ID != m_guard_boss_objid)
	{
		Obj *obj = m_scene->GetObj(m_guard_boss_objid);
		if (NULL != obj && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
		{
			GuildFbWaveCfg *guildfb_wavecfg = LOGIC_CONFIG->GetGuildFbConfig().GetGuildFbWaveCfg(m_fb_index);
			if (NULL != guildfb_wavecfg)
			{
				Monster *monster = (Monster*)obj;
				MonsterAIBase *ai = monster->GetMonsterAI();

				if (monster->GetDirDistance() <= 0 && NULL != ai && INVALID_OBJ_ID == ai->GetEnemy()) // 怪物没在走动，并且没敌人
				{
					if (m_guard_boss_pos_idx >= 0 && m_guard_boss_pos_idx < guildfb_wavecfg->guard_pt_count)
					{
						Posi last_target_pos = guildfb_wavecfg->guard_path_ptlist[m_guard_boss_pos_idx];

						Posi dist_pt = last_target_pos - monster->GetPos();
						if (!dist_pt.IsLongerThan(5)) // 已经到达目标点
						{
							++ m_guard_boss_pos_idx;
						}

						if (m_guard_boss_pos_idx < guildfb_wavecfg->guard_pt_count)
						{
							Posi target_pos = guildfb_wavecfg->guard_path_ptlist[m_guard_boss_pos_idx];
							if (MonsterInitParam::AITYPE_GUILDFB_GUARD == ai->GetAIType())
							{
								MonsterAIGuildFbGuard *guard_ai = (MonsterAIGuildFbGuard*)ai;
								guard_ai->SetGuardBossTargetPos(target_pos);
							}
						}
					}
				}
			}
		}
	}
}

void SpecialGuildFb::FlushWaveEnemyMonster()
{
	m_wave_enemy_count = 0;

	GuildFbWaveCfg *guildfb_wavecfg = LOGIC_CONFIG->GetGuildFbConfig().GetGuildFbWaveCfg(m_fb_index);
	if (NULL != guildfb_wavecfg)
	{
		for (int i = 0; i < guildfb_wavecfg->enemy_count; i++)
		{
			GuildFbEnemyCfg *enemy_cfg = &guildfb_wavecfg->enemy_list[i];
			if (NULL != enemy_cfg && enemy_cfg->sub_wave == m_wave_index)
			{
				int replace_monster_id = LOGIC_CONFIG->GetReMonsterCfg().GetReMonsterIdByWorldLevel(enemy_cfg->enemy_monster_id);

				Monster *monster = MONSTERPOOL->CreateMonster(replace_monster_id, m_scene, enemy_cfg->birth_pos);
				if (nullptr != monster)
				{
					++m_wave_enemy_count;
				}
			}
		}
	}
}

void SpecialGuildFb::SyncInfoToRole(Role *role, int reason)
{
	static Protocol::SCGuildFBInfo gfi;
	gfi.notify_reason = reason;
	gfi.curr_wave = m_fb_index;
	gfi.next_wave_time = static_cast<unsigned int>(m_next_fb_index_time);
	gfi.wave_enemy_count = static_cast<short>(m_fb_index_enemy_count);
	gfi.wave_enemy_max = static_cast<short>(m_fb_index_enemy_max);
	gfi.is_pass = (true == m_is_pass);
	gfi.is_finish = (true == m_is_finish);
	gfi.kick_role_time = m_kick_role_time;

	gfi.hp = 0;
	gfi.maxhp = 0;
	gfi.rank_count = 0;
	memset(gfi.rank_info_list, 0, sizeof(gfi.rank_info_list));

	Guild *guild = this->GetOwnerGuild();
	if (nullptr == guild)
	{
		return;
	}

	//江湖传闻
	if (m_is_finish)
	{
		if (role != NULL)
		{
			GuildID guild_id = guild->GetGuildID();
			if (INVALID_GUILD_ID != guild_id)
			{
				if (reason == Protocol::SCGuildFBInfo::NOTIFY_REASON_FINISH)
				{
					if (guild->GetTuanzhuangRoleId() == role->GetUID())
					{
						EventHandler::Instance().OnAddGuildRumorEvent(guild_id, role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_GUILD_FB_RESULT, m_fb_index);
					}
				}
			}
		}
	}

	guild->GetHurtRank(gfi);

	if (INVALID_OBJ_ID != m_guard_boss_objid)
	{
		Obj *obj = m_scene->GetObj(m_guard_boss_objid);
		if (NULL != obj && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
		{
			Monster *monster = (Monster*)obj;
			
			gfi.hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
			gfi.maxhp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
		}
	}

	
	if (NULL == role)
	{
		for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
		{
			Role *t_role = m_scene->GetRoleByIndex(i);
			if (t_role != nullptr)
			{
				guild->GetRoleRankInfo(t_role->GetUID(), gfi.self_rank_pos, gfi.self_hurt_val);

				EngineAdapter::Instance().NetSend(t_role->GetNetId(), (const char*)&gfi, sizeof(gfi));
			}
		}
	}
	else
	{
		gfi.self_rank_pos = 0; gfi.self_hurt_val = 0;
		guild->GetRoleRankInfo(role->GetUID(), gfi.self_rank_pos, gfi.self_hurt_val);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gfi, sizeof(gfi));
	} 
}

void SpecialGuildFb::OnCreateNextFbIndex()
{
	// 到下一关，设置相关数据
	Guild *guild = this->GetOwnerGuild();
	if (nullptr != guild)
	{
		guild->SetGuildFbCurIndex(m_fb_index);

		if (m_fb_index > 0)
		{
			Obj *obj = m_scene->GetObj(m_guard_boss_objid);
			if (NULL != obj && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
			{
				Monster *monster = (Monster*)obj;

				long long cur_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
				guild->SetGuildFbGuardHp(cur_hp);
			}
		}
	}

	if (m_fb_index <= 0)
	{
		this->OnNextFbIndexStart();
		return;
	}

	GuildFbWaveCfg *guildfb_wavecfg = LOGIC_CONFIG->GetGuildFbConfig().GetGuildFbWaveCfg(m_fb_index);
	if (nullptr == guildfb_wavecfg)
	{
		return;
	}

	//创建下一关副本
	SpecialGuildFb *guild_fb = World::GetInstWorld()->GetSceneManager()->GetSpecialGuildFb(guildfb_wavecfg->sceneid, m_scene->GetSceneKey(), true);
	if (nullptr != guild_fb)
	{
		guild_fb->SetFbIndex(m_fb_index);
		guild_fb->OnNextFbIndexStart();

		// 传送玩家到下一关
		Posi birth_pos(0, 0);
		if (LOGIC_CONFIG->GetGuildFbConfig().GetGuardPathPos(m_fb_index, 0, &birth_pos))
		{
			for (int i = 0; i < static_cast<int>(m_scene->RoleNum()); i++)
			{
				Role *role = m_scene->GetRoleByIndex(i);
				if (NULL != role)
				{
					birth_pos = gamecommon::GetDisRandPos(birth_pos, 5);

					World::GetInstWorld()->GetSceneManager()->GoTo(role, guildfb_wavecfg->sceneid, m_scene->GetSceneKey(), birth_pos);
				}
			}
		}
	}

	m_is_finish = true;
}

void SpecialGuildFb::OnNextFbIndexStart()
{
	// 初始化相关数据
	if(m_fb_index > 0)
	{
		m_is_open = true;

		m_is_pass = false;
		m_is_finish = false;

		m_guard_boss_objid = INVALID_OBJ_ID;

		m_fb_index_enemy_count = 0;
		m_fb_index_enemy_max = 0;
		m_next_fb_index_time = 0;

		m_guard_boss_pos_idx = 0;

		m_wave_index = 0;
		m_last_sync_time = 0;
	}

	int guard_boss_recover_per = 0;
	GuildFbWaveCfg *guildfb_wavecfg = LOGIC_CONFIG->GetGuildFbConfig().GetGuildFbWaveCfg(m_fb_index);
	if (NULL != guildfb_wavecfg)
	{
		m_fb_index_enemy_max = guildfb_wavecfg->enemy_count;
		m_fb_index_enemy_count = guildfb_wavecfg->enemy_count;
		guard_boss_recover_per = guildfb_wavecfg->guard_boss_recover_per;
	}
	
	// 刷新守卫boss
	const int guard_bossid = LOGIC_CONFIG->GetGuildFbConfig().GetGuardBossID();
	Posi birth_pos(0, 0);
	if (LOGIC_CONFIG->GetGuildFbConfig().GetGuardPathPos(m_fb_index, 0, &birth_pos))
	{
		// 下一关时保留守卫血量
		const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(guard_bossid);
		if (NULL != monster_param)
		{
			Attribute dec_hp = 0;
			Guild *guild = this->GetOwnerGuild();
			if (nullptr != guild && m_fb_index > 0)
			{
				Attribute cur_hp = guild->GetGuldfbGuardCurhp();
				Attribute recover_hp = static_cast<Attribute>(monster_param->hp * (guard_boss_recover_per / 100));

				dec_hp = cur_hp - monster_param->hp + recover_hp;
			}

			Monster *monster = MONSTERPOOL->CreateMonster(guard_bossid, m_scene, birth_pos);
			if (NULL != monster)
			{
				if (dec_hp < 0)
				{
					monster->ChangeHp(&dec_hp);
				}				
				m_guard_boss_objid = monster->GetId();
			}
		}
	}

	{
		// 刷新怪物
		this->FlushWaveEnemyMonster();
	}
}

Guild * SpecialGuildFb::GetOwnerGuild()
{
	return GuildManager::Instance().GetGuild(m_scene->GetSceneKey());
}

void SpecialGuildFb::SyncRewardRecordToSceneRole()
{
	for (unsigned int i = 0; i < m_scene->RoleNum(); i ++)
	{
		Role *scene_role = m_scene->GetRoleByIndex(i);
		if (nullptr != scene_role)
		{
			this->SendHistoryRewardRecordInfo(scene_role);
		}
	}
}

void SpecialGuildFb::SendHistoryRewardRecordInfo(Role *role)
{
	if (nullptr == role)
	{
		return;
	}

	Protocol::SCGuildFBRewardRecordInfo msg;
	msg.info_type = Protocol::REWARD_RECORDE_INFO_TYPE_HISTORY;
	msg.item_count = 0;
	memset(msg.item_list, 0, sizeof(msg.item_list));

	Guild *guild = this->GetOwnerGuild();
	if (nullptr == guild)
	{
		return;
	}

	guild->GetHistoryRewardRecord(role, msg);

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&msg, sizeof(msg));
}

void SpecialGuildFb::SendCurWaveRewardInfo(Role *role)
{
	if (nullptr == role)
	{
		return;
	}

	Protocol::SCGuildFBRewardRecordInfo msg;
	msg.info_type = Protocol::REWARD_RECORDE_INFO_TYPE_CUR_WAVE;
	msg.item_count = 0;
	memset(msg.item_list, 0, sizeof(msg.item_list));

	auto it = m_cur_wave_reward_record_map.find(role->GetUID());
	if (it == m_cur_wave_reward_record_map.end() || it->second.empty())
	{
		return;
	}

	for (auto &item : it->second)
	{
		if (msg.item_count >= Protocol::SCGuildFBRewardRecordInfo::MAX_ITEM_COUNT)
		{
			break;
		}

		Protocol::SCGuildFBRewardRecordInfo::ItemInfo &item_info = msg.item_list[msg.item_count];
		item_info.reward_item_id = item.reward_item_id;
		item_info.reward_item_num = item.reward_item_num;
		item_info.item_color = item.item_color;

		msg.item_count++;
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&msg, sizeof(msg));
}

void SpecialGuildFb::OnFbFinish(bool is_pass)
{
	m_is_pass = is_pass;
	m_is_finish = true;

	m_is_open = false;

	m_kick_role_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetGuildFbConfig().GetDelayKickTime();
	UInt32 monster_num = m_scene->MonsterNum();
	const int guard_bossid = LOGIC_CONFIG->GetGuildFbConfig().GetGuardBossID();
	for (int i = 0; i < static_cast<int>(monster_num); i++)
	{
		Monster *monster = m_scene->GetMonsterByIndex(i);
		if (NULL != monster && guard_bossid != monster->GetMonsterId())
		{
			monster->ForceSetDead();
		}
	}

	this->SyncInfoToRole(NULL, Protocol::SCGuildFBInfo::NOTIFY_REASON_FINISH);
	this->SyncRewardRecordToSceneRole();

	// 发送聊天
	Guild *guild = this->GetOwnerGuild();
	if (nullptr != guild)
	{
		static char buff[1024]; memset(buff, 0, sizeof(buff));

		Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)buff;
		cc->channel_type = chatdef::CHANNEL_TYPE_GUILD;
		cc->msg_length = gstr::SysMsgContent(buff + sizeof(Protocol::CSChannelChat), sizeof(buff) - sizeof(Protocol::CSChannelChat), "guild_fb_end_chat");

		guild->GetGuildMemberManager().GuildleaderGuildChat(cc, cc->msg_length + sizeof(Protocol::CSChannelChat));
	}

	// 清除记录
	m_cur_wave_reward_record_map.clear();
}


