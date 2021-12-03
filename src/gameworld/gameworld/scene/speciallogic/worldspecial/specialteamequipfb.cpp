#include "specialteamequipfb.hpp"

#include "config/logicconfigmanager.hpp"
#include "other/teamequipfb/teamequipfbconfig.hpp"
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "engineadapter.h"
#include "gamecommon.h"
#include "gamelog.h"
#include "protocal/msgfight.h"
#include "protocal/msgfb.h"
#include "obj/otherobj/bigcoinobj.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "other/event/eventhandler.hpp"
#include "globalconfig/globalconfig.h"
#include "servercommon/errornum.h"
#include "servercommon/userprotocal/msgcrosscommon.h"

#include "scene/activityshadow/activityshadow.hpp"
#include "item/itempool.h"
#include "obj/otherobj/fallingitem.h"

#include "servercommon/crossdef.hpp"
#include "internalcomm.h"
#include "monster/drop/droppool.hpp"
#include "global/usercache/usercache.hpp"
#include "npc/transmit/fbmanager.hpp"
#include "global/team/team.hpp"
#include "item/knapsack.h"
#include "task/newtaskmanager.h"
#include "friend/teaminfo.hpp"
#include "other/shop/roleshop.hpp"

SpecialTeamEquipFB::SpecialTeamEquipFB(Scene *scene)
	: SpecialLogic(scene), m_is_finish(false), m_has_fetch_finish_reward(false), m_user_count(0),
	m_normal_monster_kill_num(0), m_boss_monster_kill_num(0),
	m_auto_get_reward_timestamp(0), m_fb_state(NEW_FB_STATE_TYPE_DEFAULT), m_next_send_fb_info_timestamp(0), m_next_update_time(0),
	m_team_member_count(0), m_is_create_boss(false)
{

}

SpecialTeamEquipFB::~SpecialTeamEquipFB()
{
	
}

void SpecialTeamEquipFB::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (m_is_finish)
	{
		if (0 != m_auto_get_reward_timestamp && now_second >= m_auto_get_reward_timestamp)
		{
			this->FBAutoGetReward();
		}
	}
	else
	{
		if (now_second >= m_next_update_time)
		{
			this->CheckSceneMonsterStatus(interval, now_second);
			m_next_update_time = now_second + 2;
		}

		if (now_second >= m_next_send_fb_info_timestamp)
		{
			this->SendFBInfo();
		}
	}
};

void SpecialTeamEquipFB::OnRoleEnterScene(Role *role)
{
	if (m_is_finish)
	{
		this->DelayKickOutRole(role);
		return;
	}

	for (int i = 0; i < TEAM_EQUIP_FB_ROLE_MAX; i++)
	{
		if (0 == m_user_info[i].user_id.role_id)
		{
			m_user_info[i].user_id = role->GetUserId();
			m_user_info[i].obj_id = role->GetId();
			F_STRNCPY(m_user_info[i].user_name, role->GetName(), sizeof(m_user_info[i].user_name));
			++ m_user_count;
			break;
		}
	}

	if (!m_is_create_boss)
	{
		if (0 == m_team_member_count)
		{
			TeamInfo *team_info = role->GetTeamInfo();
			if (NULL != team_info)
			{
				Team *team = team_info->GetMyTeam();
				if (NULL != team)
				{
					if (team->IsLeader(role->GetUserId()))
					{
						m_team_member_count = team->GetMemberNum();
					}
				}
			}
		}

		if (m_team_member_count > 0 && m_team_member_count <= MAX_TEAM_MEMBER_NUM)
		{
			int boss_hp_add_per = LOGIC_CONFIG->GetTeamEquipFBConfig().GetBossHpAddPer(m_team_member_count);
			int fb_layer = LOGIC_CONFIG->GetTeamEquipFBConfig().GetFBLayerBySceneID(m_scene->GetSceneID());
			const TeamEquipFBLayerCfg *fb_cfg = LOGIC_CONFIG->GetTeamEquipFBConfig().GetTeamEquipFBLayerCfg(fb_layer);
			if (NULL != fb_cfg)
			{
				const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(fb_cfg->boss_id);
				if (NULL != monster_param)
				{
					int add_hp = static_cast<int>(monster_param->hp * (boss_hp_add_per * 0.0001f));
					if (add_hp <= 0)
					{
						add_hp = 0;
					}

					if (NULL != MONSTERPOOL->CreateMonster(fb_cfg->boss_id, m_scene, fb_cfg->born_pos, add_hp))
					{
						m_is_create_boss = true;
					}
					else
					{
						m_is_finish = true;
						this->DelayKickOutAllRole();
					}
				}
			}
		}
	}

	// 进入自动满血
	role->Recover(true, true, true);
	
	this->ReCalculateBuff();
	this->SendFBInfo();

	EventHandler::Instance().OnEnterTeamFb(role);
}

void SpecialTeamEquipFB::OnRoleLeaveScene(Role *role, bool is_logout)
{
	this->AutoPickFallingItem(role);

	if (m_is_finish && !m_has_fetch_finish_reward)
	{
		this->OnFetchFinishReward();
	}

	for (int i = 0; i < TEAM_EQUIP_FB_ROLE_MAX; i++)
	{
		if (role->GetUserId() == m_user_info[i].user_id)
		{

			if (role->IsAlive())
			{
				role->Recover();
			}
			else
			{
				role->ReAlive(100, 100);
			}

			{
				static Protocol::SCFbPickItemInfo pro;
				pro.item_count = 0;

				for (std::vector<ItemDataWrapper>::iterator it = m_user_info[i].pick_item_vec.begin(); it != m_user_info[i].pick_item_vec.end(); it++)
				{
					if (pro.item_count >= Protocol::SCFbPickItemInfo::PICK_ITEM_COUNT_MAX)
					{
						break;
					}

					pro.item_list[pro.item_count++] = *it;
				}

				if (pro.item_count > 0)
				{
					int length = sizeof(pro) - sizeof(pro.item_list[0]) * (Protocol::SCFbPickItemInfo::PICK_ITEM_COUNT_MAX - pro.item_count);
					EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&pro, length);
				}
			}

			m_user_info[i].Reset();
			-- m_user_count;
			break;
		}
	}

	if (m_user_count <= 0)
	{
		// 全部离线，副本就删除了
		m_is_finish = true;
		m_has_fetch_finish_reward = true;
	}
	else
	{
		this->ReCalculateBuff();
	}
}

void SpecialTeamEquipFB::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster *monster = (Monster*)dead;

		if (monster->IsBossMonster())
		{
			++ m_boss_monster_kill_num;

			int layer = LOGIC_CONFIG->GetTeamEquipFBConfig().GetFBLayerBySceneID(m_scene->GetSceneID());
			const TeamEquipFBLayerCfg *cfg = LOGIC_CONFIG->GetTeamEquipFBConfig().GetTeamEquipFBLayerCfg(layer);
			if (NULL != cfg)
			{
				int dorp_index = 0;
				int max_count = LOGIC_CONFIG->GetTeamEquipFBConfig().GetOtherCfg().max_count;

				int index = m_scene->RoleNum() - 1;
				if (index < 0) index = 0;
				if (index >= TEAM_EQUIP_FB_ROLE_MAX) index = TEAM_EQUIP_FB_ROLE_MAX - 1;

				for(unsigned int i = 0; i < m_scene->RoleNum(); i++)
				{
					Role *role = m_scene->GetRoleByIndex(i);

					CommonDataParam *common_data = role->GetCommonDataParam();
					if (NULL == common_data)
					{
						continue;
					}

					if (NULL != role && common_data->team_equip_fb_day_count < max_count + common_data->team_equip_fb_day_buy_count)
					{
						for (std::vector<UInt16>::const_iterator it = cfg->drop_id_vec[index].begin(); it != cfg->drop_id_vec[index].end(); it++)
						{
							DROPPOOL->DropHelper(*it, role->GetScene(), monster->GetPos(), role->GetUserId(), role->GetProf(), monster->GetMonsterId(), false, false, &dorp_index);
						}

						common_data->team_equip_fb_day_count++;
						EventHandler::Instance().OnPassTeamFb(role);

						Protocol::SCTeamEquipFbDropCountInfo cmd;
						cmd.team_equip_fb_day_count = common_data->team_equip_fb_day_count;
						cmd.team_equip_fb_day_buy_count = common_data->team_equip_fb_day_buy_count;
						cmd.team_equip_fb_pass_flag = common_data->team_equip_fb_pass_flag;

						EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
					}
				}
			}
		}
		else
		{
			++ m_normal_monster_kill_num;
		}

		m_next_update_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 2;

		this->SendFBInfo();
	}
}

bool SpecialTeamEquipFB::OnTimeout()
{
	this->OnFBFinish(false);
	m_has_fetch_finish_reward = true;

	return false;
}

void SpecialTeamEquipFB::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
	if (delta_hp > 0) return;

	Role *attacker_role = (Role*)m_scene->GetObj(enemy_id);
	if (NULL != cha && Obj::OBJ_TYPE_MONSTER == cha->GetObjType() && 
		NULL != attacker_role && Obj::OBJ_TYPE_ROLE == attacker_role->GetObjType())
	{
		for (int i = 0; i < TEAM_EQUIP_FB_ROLE_MAX; i++)
		{
			if (attacker_role->GetUserId() == m_user_info[i].user_id)
			{
				m_user_info[i].DPS += (-delta_hp);
				break;
			}
		}
	}
}

bool SpecialTeamEquipFB::SpecialRelive(Role *role, int realive_type)
{
	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		Posi realive_pos;
		int target_scene_id = m_scene->GetTownPoint(role, &realive_pos);
		if (target_scene_id == m_scene->GetSceneID())
		{
			if (role->ReAlive(10, 10, realive_pos)) return true;
		}
	}

	return false;
}

bool SpecialTeamEquipFB::SpecialIsEnemy(Role *role, Obj *obj)
{
	switch (obj->GetObjType())	
	{
	case Obj::OBJ_TYPE_MONSTER:
		{
			Monster *monster = (Monster*)obj;
			return monster->IsAlive();
		}
		break;

	case Obj::OBJ_TYPE_ROLE:
		{
			return false;
		}
		break;

	default:
		break;
	}

	return false;
}

void SpecialTeamEquipFB::OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper)
{
	if (NULL == item_wrapper || 0 == item_wrapper->item_id)
	{
		return;
	}

	for (int i = 0; i < TEAM_EQUIP_FB_ROLE_MAX; i++)
	{
		if (role->GetUserId() == m_user_info[i].user_id)
		{
			m_user_info[i].pick_item_vec.push_back(*item_wrapper);
			break;
		}
	}
}

Role* SpecialTeamEquipFB::GetRole(ObjID obj_id)
{
	if (m_user_count <= 0) return NULL;

	Obj *obj = m_scene->GetObj(obj_id);
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;
		for (int i = 0; i < TEAM_EQUIP_FB_ROLE_MAX; i++)
		{
			if (m_user_info[i].user_id == role->GetUserId())
			{
				return role;
			}
		}
	}

	return NULL;
}

void SpecialTeamEquipFB::OnAllMonsterDead()
{
	if (m_is_finish) return;

	{
		this->OnFBFinish(true);
	}
}

void SpecialTeamEquipFB::OnFBFinish(bool is_pass)
{
	if (m_is_finish) return;

	m_is_finish = true;

	if (is_pass)
	{
		m_auto_get_reward_timestamp = EngineAdapter::Instance().Time() + TEAM_EQUIP_FB_DELAY_SEND_TIME;
		m_fb_state = NEW_FB_STATE_TYPE_SUCCEED;

		for(int i = 0; i < static_cast<int>(m_scene->RoleNum()); i++)
		{
			Role *temp_role = m_scene->GetRoleByIndex(i);
			if (NULL != temp_role)
			{
				temp_role->GetTaskManager()->OnPassFB(FBManager::FBCT_TEAM_EQUIP_FB);
			}
		}
	}
	else
	{
		m_fb_state = NEW_FB_STATE_TYPE_FAIL;
	}

	for (int i = 0; i < TEAM_EQUIP_FB_ROLE_MAX; i++)
	{
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(m_user_info[i].user_id);
		if (NULL != user_node)
		{
			LOG_QUICK8(gamelog::g_log_quick, LOG_TYPE_TEAM_FB, user_node->uid, user_node->user_name, NULL, user_node->level, m_fb_state, NULL, NULL);
		}
	}

	this->SendFBInfo();
	this->FBAutoGetReward();
}

void SpecialTeamEquipFB::SendFBInfo()
{
	if (m_user_count <= 0) return;

	for (int i = 0; i < TEAM_EQUIP_FB_ROLE_MAX; i++)
	{
		if (INVALID_OBJ_ID != m_user_info[i].obj_id)
		{
			Role *role = this->GetRole(m_user_info[i].obj_id);
			if (NULL != role)
			{
				this->SyncFBSceneLogicInfo(role, m_user_info[i].DPS);
			}
		}
	}

	Protocol::SCTeamEquipFbInfo ctfi;
	ctfi.user_count = m_user_count;
	for (int i = 0; i < TEAM_EQUIP_FB_ROLE_MAX; i++)
	{
		F_STRNCPY(ctfi.user_info[i].user_name, m_user_info[i].user_name, sizeof(m_user_info[i].user_name));
		ctfi.user_info[i].dps = m_user_info[i].DPS;
	}

	if (NULL != m_scene)
	{
		m_scene->SendToRole((const char*)&ctfi, sizeof(ctfi));
	}

	// 3秒后自动发送一次
	m_next_send_fb_info_timestamp = EngineAdapter::Instance().Time() + 3;
}

void SpecialTeamEquipFB::OnFetchFinishReward()
{
	if (!m_is_finish || m_has_fetch_finish_reward || NEW_FB_STATE_TYPE_SUCCEED != m_fb_state) return;

	if (m_user_count > 0)
	{
		int layer = LOGIC_CONFIG->GetTeamEquipFBConfig().GetFBLayerBySceneID(m_scene->GetSceneID());
		const TeamEquipFBLayerCfg *cfg = LOGIC_CONFIG->GetTeamEquipFBConfig().GetTeamEquipFBLayerCfg(layer);
		if (NULL != cfg)
		{
			for (int i = 0; i < TEAM_EQUIP_FB_ROLE_MAX; i++)
			{
				Role *role = this->GetRole(m_user_info[i].obj_id);
				if (NULL != role)
				{
					CommonDataParam *common_data = role->GetCommonDataParam();
					if (NULL == common_data)
					{
						continue;
					}

					if (common_data->team_equip_fb_day_count >= LOGIC_CONFIG->GetTeamEquipFBConfig().GetOtherCfg().max_count + common_data->team_equip_fb_day_buy_count)
					{
						if (cfg->assist_mojing > 0)
						{
							role->GetRoleShop()->AddChestShopMojing(cfg->assist_mojing, "TeamEquipFB");
						}
					}
					else
					{
						role->AddExp(cfg->day_exp, "TeamEquipFB", Role::EXP_ADD_REASON_DEFAULT);
						if (cfg->day_shengwang > 0)
						{
							role->GetRoleShop()->AddChestShopShengwang(cfg->day_shengwang, "TeamEquipFB");
						}
					}

					if (role->IsTeamEquipFBPass(layer))
					{
						if (common_data->team_equip_fb_day_count <= LOGIC_CONFIG->GetTeamEquipFBConfig().GetOtherCfg().max_count + common_data->team_equip_fb_day_buy_count)
						{
							if (cfg->pass_item_count > 0)
							{
								role->GetKnapsack()->PutListOrMail(cfg->pass_item_list, PUT_REASON_TEAM_EQUIP_FB);
							}
						}
					}
					else
					{
						role->OnTeamEquipFBPassLayer(layer);

						if (cfg->mojing > 0)
						{
							role->GetRoleShop()->AddChestShopMojing(cfg->mojing, "TeamEquipFB");
						}

						if (cfg->first_item_count > 0)
						{
							role->GetKnapsack()->PutListOrMail(cfg->first_item_list, PUT_REASON_TEAM_EQUIP_FB);
						}
					}
				}
			}
		}
	}

	m_has_fetch_finish_reward = true;
	m_auto_get_reward_timestamp = 0;
}

void SpecialTeamEquipFB::FBAutoGetReward()
{
	if (m_has_fetch_finish_reward) return;

	this->OnFetchFinishReward();
}

void SpecialTeamEquipFB::ReCalculateBuff()
{
	if (m_user_count <= 0) return;

	for (int i = 0; i < TEAM_EQUIP_FB_ROLE_MAX; i++)
	{
		if (INVALID_OBJ_ID != m_user_info[i].obj_id)
		{
			Role *role = this->GetRole(m_user_info[i].obj_id);
			if (NULL != role)
			{
				role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_ATTRBUFF, EBT_INVALID, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_SCENE_SKILL, 1);

				if (m_user_count >= TEAM_EQUIP_FB_ROLE_MAX)
				{
					Attribute add_gongji = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * (TEAM_EQUIP_FB_GONGJI_BUFF_ADDON / 100.0));
					EffectBuff *effect_buff = new EffectBuff(INVALID_OBJ_ID, 1, 3600 * 48 * 1000, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
					effect_buff->AddGongJiEffect(add_gongji, TEAM_EQUIP_FB_GONGJI_BUFF_ADDON);
					effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
					role->AddEffect(effect_buff);
				}
			}
		}
	}
}

void SpecialTeamEquipFB::SyncFBSceneLogicInfo(Role *role, long long dps)
{
	static Protocol::SCFBSceneLogicInfo cmd; 
	cmd.reset();

	cmd.time_out_stamp = m_scene->GetSceneTimeOutStamp();
	cmd.scene_type = this->GetSceneType();

	cmd.is_finish = m_is_finish ? 1 : 0;
	cmd.is_pass = NEW_FB_STATE_TYPE_SUCCEED == m_fb_state ? 1 : 0;
	cmd.is_active_leave_fb = 1;

	cmd.kill_boss_num = m_boss_monster_kill_num;
	cmd.kill_allmonster_num = m_normal_monster_kill_num + m_boss_monster_kill_num;

	cmd.pass_time_s = this->GetPassTime();

	cmd.param1 = dps;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}
