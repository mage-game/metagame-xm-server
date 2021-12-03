#include "specialtowerdefend.hpp"
#include "engineadapter.h"
#include "gamecommon.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"

#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "obj/otherobj/gatherobj.h"
#include "monster/monsterpool.h"

#include "scene/scenemanager.h"
#include "world.h"
#include "servercommon/errornum.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "protocal/msgfight.h"
#include "other/event/eventhandler.hpp"
#include "internalcomm.h"
#include "servercommon/string/gameworldstr.h"

#include "ai/monsterai/specialai/monsteraitowerdefendteam.hpp"

#include "config/logicconfigmanager.hpp"

#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/sceneeffect/sceneeffect.hpp"
#include "effect/effectbase.hpp"

#include "other/fb/towerdefendteamskill.hpp"
#include "other/fb/towerdefendteamfunc.hpp"
#include "obj/otherobj/fallingitem.h"
#include "gameworld/item/itempool.h"
#include "npc/transmit/fbmanager.hpp"
#include "effect/skilleffect/effecthpstore.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/fb/rolefbtowerdefend.hpp"
#include "other/capability/capability.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "gameworld/item/knapsack.h"

SpecialTowerDefend::SpecialTowerDefend(Scene *scene)
	: SpecialLogic(scene), m_is_inited(false), m_is_finish(false), m_is_pass(false), m_is_last_wave(false), m_msg_need_sync(false),
	m_curr_wave_index(-1), m_next_wave_start_time(0), m_next_refresh_one_time(0), m_last_check_monster_status_time(0), 
	m_last_warning_attack_time(0), m_last_sync_to_allclient_time(0), m_life_tower_left_hp(0), m_life_tower_left_maxhp(0), m_owner_objid(0),
	m_clear_wave_count(0), m_get_coin(0), m_get_item_count(0), m_send_drop_info_timestamp(0), m_death_count(0), m_pass_kick_out_time(0),
	m_new_player_leave(false), m_scene_begin_time(0)
{
	memset(m_refresh_left_count, 0, sizeof(m_refresh_left_count));
	memset(m_kill_count, 0, sizeof(m_kill_count));
	memset(m_reward_item_list, 0, sizeof(m_reward_item_list));
}

SpecialTowerDefend::~SpecialTowerDefend()
{

}

void SpecialTowerDefend::Update(unsigned long interval, time_t now_second)
{
	//副本最大存在时间. 超过必然可销毁
	if (m_scene_begin_time != 0 && now_second > m_scene_begin_time + 60*20)
	{
		if (!m_is_finish)
		{
			OnFinish(false);
		}
		m_is_finish = true;
	}

	if (m_new_player_leave)return;

	SpecialLogic::Update(interval, now_second);

	if (m_is_finish)
	{
		if (m_pass_kick_out_time > 0 && now_second >= m_pass_kick_out_time)
		{
			this->DelayKickOutAllRole(CHANGE_SCENE_REASON_FB_FINISH);
		}
		return;
	}

	{
		// 刚启动
		if (!m_is_inited)
		{
			m_is_inited = true;

			m_scene_begin_time = static_cast<unsigned int>(now_second);
			m_next_wave_start_time = static_cast<unsigned int>(now_second) + LOGIC_CONFIG->GetTowerDefendConfig().GetRefreshStartDelay();
			UInt32 monster_num = m_scene->MonsterNum();
			for (int i = 0; i < (int)monster_num; ++i)
			{
				Monster *monster = m_scene->GetMonsterByIndex(i);
				if (NULL != monster && monster->IsAlive() &&
					monster->GetMonsterId() == LOGIC_CONFIG->GetTowerDefendConfig().GetLifeTowerMonsterID())
				{
					m_life_tower_left_maxhp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
					m_life_tower_left_hp = m_life_tower_left_maxhp;

					break;
				}
			}

			this->SyncToClient(Protocol::SCTowerDefendInfo::NOTIFY_REASON_INIT); 
		}
	}

	{
		// 新一波开始
		if (m_next_wave_start_time > 0 && now_second >= m_next_wave_start_time)
		{
			++m_curr_wave_index;
			const TowerDefendWaveCfg * wave_cfg = LOGIC_CONFIG->GetTowerDefendConfig().GetWaveCfg(m_scene->GetSceneID(), m_curr_wave_index);
			if (NULL != wave_cfg) 
			{
				m_next_wave_start_time = static_cast<unsigned int>(now_second) + wave_cfg->next_wave_s;

				for (int i = 0; i < TOWERDEFEND_PATH_MAX; ++i)
				{
					m_refresh_left_count[i] = wave_cfg->monster_count_list[i];
				}

				this->RefreshOne(now_second);
			}

			if (m_curr_wave_index + 1 >= LOGIC_CONFIG->GetTowerDefendConfig().GetWaveCountBySceneID(m_scene->GetSceneID()))	// 最后一波了
			{
				m_is_last_wave = true;
				m_next_wave_start_time = 0;
			}

			this->SyncToClient(Protocol::SCTowerDefendInfo::NOTIFY_REASON_NEW_WAVE_START); 
		}
	}

	{
		// 按频率在各路线出怪
		if (m_next_refresh_one_time > 0 && now_second >= m_next_refresh_one_time)
		{
			this->RefreshOne(now_second);
		}
	}

	if (!m_is_finish)
	{
		// 检查是否杀光
		this->CheckAttackMonsterStatus(interval, now_second);
	}

	{
		if (m_curr_wave_index >= 0 && m_msg_need_sync)
		{
			this->SyncToClient();
		}
	}
}

void SpecialTowerDefend::OnRoleEnterScene(Role *role)
{
	if (role == NULL) return;

	if (m_is_finish)
	{
		this->DelayKickOutRole(role, CHANGE_SCENE_REASON_FB_FINISH);
	}
	else
	{
		bool is_first_enter = (INVALID_USER_ID == m_owner_user_id);

		m_owner_user_id = role->GetUserId(); 
		m_owner_objid = role->GetId();

		EventHandler::Instance().OnEnterFBTowerDefendPerson(role, is_first_enter);

		m_msg_need_sync = true;
		
	}

	// 战力达到时,加buff
	const TowerDefendLevelCfg::ConfigItem *lvl_cfg = LOGIC_CONFIG->GetTowerDefendConfig().GetLevelCfgBySceneID(m_scene->GetSceneID());
	if (nullptr != lvl_cfg && role->GetCapability()->GetTotalCapability() >= lvl_cfg->capability)
	{
		static const int BUFF_TIME_MS = 3600 * 1000;
		int add_gongji_per = LOGIC_CONFIG->GetTowerDefendConfig().GetOtherCfg().add_role_gongji_per;
		Attribute value = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (add_gongji_per * ROLE_ATTR_PER));

		EffectBuff *add_gongji = new EffectBuff(role->GetId(), 1, BUFF_TIME_MS, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		add_gongji->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
		add_gongji->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, value, add_gongji_per);
		role->AddEffect(add_gongji);
	}

	if (m_new_player_leave)
	{
		m_new_player_leave = false;
		int monster_num = m_scene->MonsterNum();
		for (int i = 0; i < monster_num; ++i)
		{
			Monster * monster = m_scene->GetMonsterByIndex(i);
			if (monster != NULL)
			{
				MonsterAIBase * ai = monster->GetMonsterAI();
				if (NULL != ai && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == monster->GetAIType())
				{
					ai->SetIsStatic(false);
				}
			}
		}
	}

	this->SyncToClient();
}

void SpecialTowerDefend::OnRoleLeaveScene(Role *role, bool is_logout)
{
	this->AutoPickFallingItem(role);

	role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_ATTRBUFF, EBT_ADD_GONGJI, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_SCENE_SKILL, 1);

	const TowerDefendLevelCfg::ConfigItem *lvl_cfg = LOGIC_CONFIG->GetTowerDefendConfig().GetLevelCfgBySceneID(m_scene->GetSceneID());
	if (lvl_cfg != NULL && lvl_cfg->level == TOWERDEFEND_NEW_PLAYER_FIRST_LEVEL && !m_is_finish)
	{
		m_new_player_leave = true;

		int star_level = LOGIC_CONFIG->GetTowerDefendConfig().GetStarByDeathCount(m_death_count);
		role->GetFBTowerDefend()->OnPersonalFBFinish(true, m_scene->GetSceneID(), star_level);

		int monster_num = m_scene->MonsterNum();
		unsigned int  now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		for (int i = 0; i < monster_num; ++i)
		{
			Monster * monster = m_scene->GetMonsterByIndex(i);
			if (monster != NULL )
			{
				MonsterAIBase * ai = monster->GetMonsterAI();
				if (NULL != ai && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == monster->GetAIType())
				{
					ai->SetIsStatic(true);
					ai->SetStaticEndTime(now_sec + 600);
				}
			}
		}
	}
}

bool SpecialTowerDefend::CanDestroy()
{
	return m_is_finish;
}

void SpecialTowerDefend::OnRemoveObj(Obj *obj)
{
}

void SpecialTowerDefend::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead) return;

	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster * monster = (Monster *)dead;

		TowerDefendBuffDropWeightCfg *buff_weight_cfg = LOGIC_CONFIG->GetTowerDefendConfig().GetBuffDropCfg(monster->GetMonsterId());
		if (NULL != buff_weight_cfg)
		{
			if (buff_weight_cfg->total_weight > 0)
			{
				int buff_type = DEFEND_BUff_TYPE_INVALID;
				int rand_value = RandomNum(buff_weight_cfg->total_weight);

				for (int i = 0; i < (TOWERDEFEND_BUFF_DROP_WEIGHT_COUNT - 1); i++)
				{
					if (rand_value < buff_weight_cfg->weight[i])
					{
						buff_type = i + 1;
						break;
					}
					else
					{
						rand_value -= buff_weight_cfg->weight[i];
					}
				}

				if (buff_type > DEFEND_BUff_TYPE_INVALID && buff_type < DEFEND_BUff_TYPE_MAX)
				{
					FallingItem *fallingitem = new FallingItem();
					fallingitem->Init(0, 0, false, 0, INVALID_USER_ID, m_scene->GetSceneID(), LOGIC_CONFIG->GetTowerDefendConfig().GetBuffDropExitTime(), 0, FALLING_ITEM_BROADCAST_TYPE_NO_BROADCAST, false);
					fallingitem->SetPos(monster->GetPos());
					fallingitem->SetObjPosi(monster->GetPos());
					fallingitem->SetBuffFalling(true);
					fallingitem->SetBuffAppearan(buff_type);
					m_scene->AddObj(fallingitem);
				}
			}
		}

		if (monster->GetMonsterId() == LOGIC_CONFIG->GetTowerDefendConfig().GetLifeTowerMonsterID())
		{
			if (!m_is_finish)
			{
				m_life_tower_left_hp = 0;
				this->SyncToClient();

				m_is_finish = true;

				this->OnFinish(false);
			}
		}
		else
		{
			if (MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == monster->GetAIType())
			{
				const TowerDefendWaveCfg * wave_cfg = LOGIC_CONFIG->GetTowerDefendConfig().GetWaveCfgByMonsterID(m_scene->GetSceneID(), monster->GetMonsterId());
				if (NULL != wave_cfg)
				{
					if (wave_cfg->wave >= 0 && wave_cfg->wave < PERSONAL_TOWERDEFEND_MAX_WAVE_PER_LEVEL)
					{
						++m_kill_count[wave_cfg->wave];
						if (m_kill_count[wave_cfg->wave] >= wave_cfg->total_monster_count)
						{
							++m_clear_wave_count;

							m_msg_need_sync = true;
						}
					}
				}
			}
		}
	}
	if (Obj::OBJ_TYPE_ROLE == dead->GetObjType())
	{
		Role *death_role = (Role*)dead;

		if (death_role->GetUserId() == m_owner_user_id)
		{
			m_death_count++;
			this->SyncToClient();
		}
	}
}

void SpecialTowerDefend::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
	if (NULL == cha) return;

	if (Obj::OBJ_TYPE_MONSTER == cha->GetObjType())
	{
		Monster *monster = (Monster*)cha;

		if (monster->GetMonsterId() == LOGIC_CONFIG->GetTowerDefendConfig().GetLifeTowerMonsterID() && monster->IsAlive())
		{
			unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

			{
				if (now_second >= m_last_warning_attack_time + LOGIC_CONFIG->GetTowerDefendConfig().GetWarningInterval())
				{
					m_last_warning_attack_time = now_second;

					this->NotifyWarning(Protocol::SCTowerDefendWarning::WARNING_TYPE_NORMAL, 0);
				}
			}

			{
				long long now_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
				long long max_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
				long long old_hp = now_hp + (-delta_hp);

				int old_percent = static_cast<int>(max_hp > 0 ? old_hp * 100 / max_hp : 0); //血量百分比
				int now_percent = static_cast<int>(max_hp > 0 ? now_hp * 100 / max_hp : 0);

				if (old_percent > now_percent)
				{
					int warning_percent = 0;
					if (LOGIC_CONFIG->GetTowerDefendConfig().CheckWarning(old_percent, now_percent, &warning_percent))
					{
						this->NotifyWarning(Protocol::SCTowerDefendWarning::WARNING_TYPE_LOW_PERCENT, warning_percent);
					}
				}

				m_life_tower_left_hp = static_cast<int>(now_hp);
				m_msg_need_sync = true;
			}
		}
	}
}

bool SpecialTowerDefend::OnTimeout()
{
	if (!m_is_finish)
	{
		m_is_finish = true;

		this->OnFinish(false);
	}

	return m_is_pass;
}

void SpecialTowerDefend::OnLeaveFB(Role *role)
{
	if (!m_is_finish)
	{
		this->OnFinish(false);
	}

	// role->GetSpecialAppearance()->SetFbAppearance(SPECIAL_APPEARANCE_TYPE_NORMAL, 0, false);
	role->GetMountManager()->GoonMount(true, false, true);
}

bool SpecialTowerDefend::SpecialRelive(Role *role, int realive_type)
{
	const TowerDefendOtherCfg & other_cfg = LOGIC_CONFIG->GetTowerDefendConfig().GetOtherCfg();
	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		if (role->GetDeadTime() < other_cfg.relive_back_home_interval)
		{
			return true;
		}
		Posi realive_pos;
		int target_scene_id = m_scene->GetTownPoint(role, &realive_pos);
		if (target_scene_id == m_scene->GetSceneID())
		{
			if (role->ReAlive(50, 100, realive_pos)) return true;
		}
	}
	else if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_HERE_GOLD == realive_type)
	{
		if (role->GetDeadTime() < other_cfg.relive_gold_interval)
		{
			return true;
		}
	}
	
	return false;
}

bool SpecialTowerDefend::SpecialIsEnemy(Role *role, Obj *obj)
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
			Monster *monster = (Monster*)obj;
			return monster->IsAlive() && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == monster->GetAIType();
		}
		break;
	}

	return SpecialLogic::SpecialIsEnemy(role, obj);
}

bool SpecialTowerDefend::SpecialIsFriend(Role *role, Obj *obj, bool check_alive)
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
			Monster *monster = (Monster*)obj;
			return (!check_alive || monster->IsAlive()) && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == monster->GetAIType();
		}
		break;
	}

	return SpecialLogic::SpecialIsFriend(role, obj, check_alive);
}

bool SpecialTowerDefend::SpecialCanReAlive(Role *role, int realive_type)
{
	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		return role->GetDeadTime() >= TOWERDEFEND_HOME_REALIVE_LIMIT_INTERVAL;
	}

	return role->GetDeadTime() >= TOWERDEFEND_GOLD_REALIVE_LIMIT_INTERVAL;
}

void SpecialTowerDefend::OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper)
{
	if (NULL != role && NULL != falling_item)
	{
		const ItemBase *item = ITEMPOOL->GetItem(falling_item->GetItemId());
		if (NULL != item)
		{
			bool is_find = false;

			for (int i = 0; i < m_get_item_count && i < Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM; ++ i)
			{
				if (m_reward_item_list[i].item_id == falling_item->GetItemId())
				{
					m_reward_item_list[i].num += falling_item->GetItemNum();
					is_find = true;

					break;
				}	
			}

			if (!is_find && m_get_item_count < Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM)
			{
				m_reward_item_list[m_get_item_count].item_id = falling_item->GetItemId();
				m_reward_item_list[m_get_item_count].num = falling_item->GetItemNum();

				++ m_get_item_count;
			}
		}
		else
		{
			m_get_coin += falling_item->GetCoinOrGold();
		}

		if (falling_item->IsBuffFalling())
		{
			TowerDefendBuffCfg *buff_cfg = LOGIC_CONFIG->GetTowerDefendConfig().GetBufffCfg(falling_item->GetBuffAppearan());
			if (NULL != buff_cfg)
			{
				switch (buff_cfg->buff_type)
				{
				case DEFEND_BUff_TYPE_RECOVER:
				{
					UInt32 monster_num = m_scene->MonsterNum();
					for (int i = 0; i < (int)monster_num; ++i)
					{
						Monster *monster = m_scene->GetMonsterByIndex(i);
						if (NULL != monster && monster->IsAlive() &&
							monster->GetMonsterId() == LOGIC_CONFIG->GetTowerDefendConfig().GetLifeTowerMonsterID())
						{
							Attribute add_hp = static_cast<Attribute>(monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (buff_cfg->param[0] * ROLE_ATTR_PER));
							monster->ChangeHp(&add_hp, true);
							m_life_tower_left_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
							this->SyncToClient();
							break;
						}
					}
				}
				break;

				case DEFEND_BUff_TYPE_ADD_GONGJI:
				{
					EffectBuff *add_gongji = new EffectBuff(role->GetId(), 1, buff_cfg->param[1], 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
					add_gongji->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
					Attribute value = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * (buff_cfg->param[0] * ROLE_ATTR_PER));
					add_gongji->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, value, buff_cfg->param[0]);
					add_gongji->SetClientEffectType(EffectBase::ECT_NSTF_ADD_GONGJI);
					role->AddEffect(add_gongji);
				}
				break;

				case DEFEND_BUff_TYPE_KILL_MONSTER:
				{
					UInt32 monster_num = m_scene->MonsterNum();
					for (int i = 0; i < (int)monster_num; ++i)
					{
						Monster *monster = m_scene->GetMonsterByIndex(i);
						if (NULL != monster && monster->IsAlive() &&
							monster->GetMonsterId() != LOGIC_CONFIG->GetTowerDefendConfig().GetLifeTowerMonsterID())
						{
							monster->ForceSetDead(role);
						}
					}
				}
				break;

				case DEFEND_BUff_TYPE_ADD_HUDUN:
				{
					UInt32 monster_num = m_scene->MonsterNum();
					for (int i = 0; i < (int)monster_num; ++i)
					{
						Monster *monster = m_scene->GetMonsterByIndex(i);
						if (NULL != monster && monster->IsAlive() &&
							monster->GetMonsterId() == LOGIC_CONFIG->GetTowerDefendConfig().GetLifeTowerMonsterID())
						{
							EffectHpStore *effect_hp_store = new EffectHpStore(role->GetId(), 1, buff_cfg->param[1], buff_cfg->param[0], 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
							effect_hp_store->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
							monster->AddEffect(effect_hp_store);
							break;
						}
					}
				}
				break;

				default:
					break;
				}
			}
		}
	}

	//用于客户端掉落统计
	{
		Protocol::SCFBDropInfo fbdi;
		fbdi.get_coin = m_get_coin;
		fbdi.get_item_count = 0;

		memset(fbdi.drop_items, 0, sizeof(Protocol::SCFBDropInfo::DropItem) * Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM);
		for (int i = 0; i < m_get_item_count && i < Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM; ++i)
		{
			fbdi.drop_items[i].item_id = m_reward_item_list[i].item_id;
			fbdi.drop_items[i].num = static_cast<short>(m_reward_item_list[i].num);

			++fbdi.get_item_count;
		}

		int len = sizeof(fbdi) - sizeof(fbdi.drop_items[0]) * (Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM - fbdi.get_item_count);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&fbdi, len);
	}
}

void SpecialTowerDefend::OnNextWaveReq(Role *role)
{
	if (NULL == role) return;

	const TowerDefendWaveCfg * wave_cfg = LOGIC_CONFIG->GetTowerDefendConfig().GetWaveCfg(m_scene->GetSceneID(), m_curr_wave_index + 1);
	if (NULL == wave_cfg) return;

	for (int i = 0; i < TOWERDEFEND_PATH_MAX; ++i)
	{
		if (m_refresh_left_count[i] > 0)
		{
			role->NoticeNum(errornum::EN_TOWER_DEFEND_CURR_WAVE_NOT_DONE);
			return;
		}
	}

	int monster_num = m_scene->MonsterNum();
	for (int i = 0; i < monster_num; ++i)
	{
		Monster *monster = m_scene->GetMonsterByIndex(i);
		if (monster != NULL && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == monster->GetAIType() && monster->IsAlive())
		{
			role->NoticeNum(errornum::EN_FB_REST_MONSTER_NUM_LIMIT);
			return;
		}
	}

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (m_next_wave_start_time > 0 && now_second < m_next_wave_start_time)
	{
		m_next_wave_start_time = now_second;

		this->SyncToClient();

		role->NoticeNum(noticenum::NT_TOWERDEFEND_NEXT_WAVE_SUCC);
	}
}

Role* SpecialTowerDefend::GetOwnerRole()
{
	Obj *obj = m_scene->GetObj(m_owner_objid);
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;
		if (m_owner_user_id == role->GetUserId()) 
		{
			return role;
		}
	}

	return NULL;
}

void SpecialTowerDefend::RefreshOne(time_t now_second)
{
	const TowerDefendWaveCfg * wave_cfg = LOGIC_CONFIG->GetTowerDefendConfig().GetWaveCfg(m_scene->GetSceneID(), m_curr_wave_index);
	if (NULL == wave_cfg) return;

	bool is_left = false;

	for (int path = TOWERDEFEND_PATH_LEFT; path < TOWERDEFEND_PATH_MAX; ++path)
	{
		if (m_refresh_left_count[path] > 0)
		{
			Posi birth_pos = LOGIC_CONFIG->GetTowerDefendConfig().GetBirthPos(path);

			// 未达到推荐战力时
			Attribute add_hp = 0, add_gongji = 0;
			Role *role = this->GetOwnerRole();
			const TowerDefendLevelCfg::ConfigItem *lvl_cfg = LOGIC_CONFIG->GetTowerDefendConfig().GetLevelCfgBySceneID(m_scene->GetSceneID());
			if (nullptr != lvl_cfg)
			{
				if (nullptr != role && role->GetCapability()->GetTotalCapability() < lvl_cfg->capability)
				{
					const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(wave_cfg->monster_id_list[path]);
					if (NULL != monster_param)
					{
						const TowerDefendOtherCfg &other_cfg = LOGIC_CONFIG->GetTowerDefendConfig().GetOtherCfg();
						add_hp = (Attribute)(monster_param->hp * (other_cfg.add_hp_per * SKILL_ATTR_PER));
						add_gongji = (Attribute)(monster_param->gongji * (other_cfg.add_gongji_per * SKILL_ATTR_PER));
					}
				}
			}
			
			Monster *monster = MONSTERPOOL->CreateMonster(wave_cfg->monster_id_list[path], m_scene, gamecommon::GetDisRandPos(birth_pos, 4), add_hp, add_gongji);
			if (NULL != monster)
			{
				MonsterAIBase *aibase = monster->GetMonsterAI();
				if (NULL != aibase && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == aibase->GetAIType())
				{
					MonsterAITowerDefendTeam *aiattack = (MonsterAITowerDefendTeam *)aibase;
					aiattack->SetBirthPath(path);
				}

				-- m_refresh_left_count[path];
			}
			//printf("------refresh wave:%d ---path:%d,left:%d\n",m_curr_wave_index, path, m_refresh_left_count[path]);
			if (m_refresh_left_count[path] > 0) is_left = true;
		}
	}

	if (is_left)
	{
		m_next_refresh_one_time = static_cast<unsigned int>(now_second) + LOGIC_CONFIG->GetTowerDefendConfig().GetRefreshInterval();
	}
	else
	{
		m_next_refresh_one_time = 0;
	}
}

void SpecialTowerDefend::CheckAttackMonsterStatus(unsigned long interval, time_t now_second)
{
	if (now_second >= m_last_check_monster_status_time + 1)
	{
		m_last_check_monster_status_time = static_cast<unsigned int>(now_second);

		for (int i = 0; i < TOWERDEFEND_PATH_MAX; ++i)
		{
			if (m_refresh_left_count[i] > 0) return; // 怪都没出完呢
		}

		UInt32 monster_num = m_scene->MonsterNum();
		for (int i = 0; i < (int)monster_num; ++i)
		{
			Monster *monster = m_scene->GetMonsterByIndex(i);
			if (NULL != monster && monster->IsAlive() && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == monster->GetAIType())
			{
				return;
			}
		}

		this->OnAllAttackMonsterDead();
	}
}

void SpecialTowerDefend::OnAllAttackMonsterDead()
{
	if (!m_is_finish && m_is_last_wave)
	{
		m_is_finish = true;

		this->OnFinish(true);
	}
}

void SpecialTowerDefend::OnFinish(bool is_pass)
{
	if (OnNewPlayerLevelFinish())
	{
		return;
	}

	m_is_pass = is_pass;
	m_next_wave_start_time = 0;
	m_is_finish = true;

	Role *role = this->GetOwnerRole();
	if (NULL != role)
	{
		short star_level = 0;
		if(is_pass)
		{
			this->AutoPickFallingItem(role);
			role->Recover();
			star_level = LOGIC_CONFIG->GetTowerDefendConfig().GetStarByDeathCount(m_death_count);
		}

		this->SyncToClient(Protocol::SCTowerDefendInfo::NOTIFY_REASON_FINISHED);
		role->GetFBTowerDefend()->OnPersonalFBFinish(is_pass, m_scene->GetSceneID(), star_level);
		role->NotifyFBFinish();

		Protocol::SCTowerDefendResult ttdr;
		ttdr.is_passed = m_is_pass;
		ttdr.clear_wave_count = m_clear_wave_count;
		ttdr.use_time = 0;
		ttdr.have_pass_reward = 0;
		ttdr.reserve_sh = 0;

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&ttdr, sizeof(ttdr));

		if (is_pass)
		{
			m_pass_kick_out_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 9;
		}
		else
		{
			this->DelayKickOutRole(role, CHANGE_SCENE_REASON_FB_FINISH);
		}
	}
	UInt32 monster_num = m_scene->MonsterNum();
	for (int i = 0; i < static_cast<int>(monster_num); i++)
	{
		Monster *monster = m_scene->GetMonsterByIndex(i);
		if (NULL != monster && monster->GetMonsterId() != LOGIC_CONFIG->GetTowerDefendConfig().GetLifeTowerMonsterID())
		{
			monster->ForceSetDead();
		}
	}
}

bool SpecialTowerDefend::OnNewPlayerLevelFinish()
{
	const TowerDefendLevelCfg::ConfigItem *lvl_cfg = LOGIC_CONFIG->GetTowerDefendConfig().GetLevelCfgBySceneID(m_scene->GetSceneID());
	if (lvl_cfg == NULL || lvl_cfg->level != TOWERDEFEND_NEW_PLAYER_FIRST_LEVEL)
	{
		return false;
	}

	ItemConfigData first_reward[MAX_ATTACHMENT_ITEM_NUM];
	LOGIC_CONFIG->GetTowerDefendConfig().GetFirstReward(first_reward, MAX_ATTACHMENT_ITEM_NUM);

	m_is_pass = true;
	m_next_wave_start_time = 0;
	m_is_finish = true;

	Role *role = this->GetOwnerRole();
	if (NULL != role)
	{
		short star_level = 0;
		this->AutoPickFallingItem(role);
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			if(first_reward[i].item_id == 0 || first_reward[i].num == 0) continue;

			bool is_find = false;
			for (int j = 0;j < m_get_item_count && j < Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM; ++j)
			{
				if (first_reward[i].item_id == m_reward_item_list[j].item_id)
				{
					if (first_reward[i].num > m_reward_item_list[j].num)
					{
						first_reward[i].num -= m_reward_item_list[j].num;
						m_reward_item_list[j].num += first_reward[i].num;
					}
					else
					{
						first_reward[i].num = 0;
					}
					is_find = true;
					break;
				}
			}

			if (!is_find && m_get_item_count < Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM)
			{
				m_reward_item_list[m_get_item_count].item_id = first_reward[i].item_id;
				m_reward_item_list[m_get_item_count].num = first_reward[i].num;
				m_reward_item_list[m_get_item_count].is_bind = first_reward[i].is_bind;
				++m_get_item_count;
			}
		}

		role->GetKnapsack()->PutListOrMail(first_reward, PUT_REASON_TOWERDEFEND_FIRST_REWARD);

		this->SyncToClient(Protocol::SCTowerDefendInfo::NOTIFY_REASON_FINISHED);
		role->Recover();
		star_level = LOGIC_CONFIG->GetTowerDefendConfig().GetStarByDeathCount(m_death_count);
		
		role->GetFBTowerDefend()->OnPersonalFBFinish(true, m_scene->GetSceneID(), star_level);
		role->NotifyFBFinish();

		Protocol::SCTowerDefendResult ttdr;
		ttdr.is_passed = m_is_pass;
		ttdr.clear_wave_count = m_clear_wave_count;
		ttdr.use_time = 0;
		ttdr.have_pass_reward = 0;
		ttdr.reserve_sh = 0;

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&ttdr, sizeof(ttdr));

		m_pass_kick_out_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 9;

	}
	else
	{
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			if (first_reward[i].item_id == 0 || first_reward[i].num == 0) continue;

			bool is_find = false;
			for (int j = 0; j < m_get_item_count && j < Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM; ++j)
			{
				if (first_reward[i].item_id == m_reward_item_list[j].item_id)
				{
					if (first_reward[i].num > m_reward_item_list[j].num)
					{
						first_reward[i].num -= m_reward_item_list[j].num;
						m_reward_item_list[j].num += first_reward[i].num;
					}
					else
					{
						first_reward[i].num = 0;
					}
					is_find = true;
					break;
				}
			}

			if (!is_find && m_get_item_count < Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM)
			{
				m_reward_item_list[m_get_item_count].item_id = first_reward[i].item_id;
				m_reward_item_list[m_get_item_count].num = first_reward[i].num;
				m_reward_item_list[m_get_item_count].is_bind = first_reward[i].is_bind;
				++m_get_item_count;
			}
		}

		MailContentParam contentparam; contentparam.Reset();

		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			if (0 != first_reward[i].item_id && first_reward[i].num > 0)
			{
				contentparam.AddItem(first_reward[i]);
			}
		}

		int length1 = gstr::MailContent(contentparam.contenttxt, sizeof(contentparam.contenttxt), "towerdefend_new_player");
		//int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_knapsack_put_content);
		if (length1 > 0)
		{
			MailRoute::MailToUser(m_owner_user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);
		}
	}

	this->KillAllMonster();
	return true;
}

void SpecialTowerDefend::SyncToClient(int reason)
{
	Role *role = this->GetOwnerRole();
	if (NULL != role)
	{
		Protocol::SCTowerDefendInfo ttdi;
		ttdi.reason = static_cast<short>(reason);
		ttdi.time_out_stamp = 0;
		ttdi.is_finish = m_is_finish ? 1 : 0;
		ttdi.is_pass = m_is_pass ? 1: 0;
		ttdi.pass_time_s = 0;
		ttdi.life_tower_left_hp = m_life_tower_left_hp;
		ttdi.life_tower_left_maxhp = m_life_tower_left_maxhp;
		ttdi.curr_wave = m_curr_wave_index;
		ttdi.curr_level = LOGIC_CONFIG->GetTowerDefendConfig().GetLevelBySceneID(m_scene->GetSceneID());
		ttdi.next_wave_refresh_time = m_next_wave_start_time;
		ttdi.clear_wave_count = m_clear_wave_count;
		ttdi.death_count = m_death_count;
		if (m_is_pass)
		{
			ttdi.get_coin = m_get_coin;
			ttdi.get_item_count = 0;
			memset(ttdi.drop_items, 0, sizeof(Protocol::SCTowerDefendInfo::DropItem) * FB_RECORD_DROP_ITEM_MAX_NUM);
			for (unsigned int i = 0; i < Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM && ttdi.get_item_count< FB_RECORD_DROP_ITEM_MAX_NUM; i++)
			{
				if (m_reward_item_list[i].item_id > 0 && m_reward_item_list[i].num > 0 )
				{
					ttdi.drop_items[ttdi.get_item_count].item_id = m_reward_item_list[i].item_id;
					ttdi.drop_items[ttdi.get_item_count].num = m_reward_item_list[i].num;
					++ttdi.get_item_count;
				}
			}
		}
		else
		{
			ttdi.get_coin = 0;
			ttdi.get_item_count = 0;
		}

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&ttdi, sizeof(ttdi));

		m_last_sync_to_allclient_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	}

	m_msg_need_sync = false;
}

void SpecialTowerDefend::NotifyWarning(short warning_type, short percent)
{
	Protocol::SCTowerDefendWarning ttdw;
	ttdw.warning_type = warning_type;
	ttdw.percent = percent;

	m_scene->SendToRole((const char *)&ttdw, sizeof(ttdw));
}




