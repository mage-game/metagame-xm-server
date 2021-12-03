#include "specialnewequip.hpp"
#include "engineadapter.h"
#include "gamecommon.h"
#include "servercommon/errornum.h"
#include "obj/character/role.h"
#include "protocal/msgfight.h"
#include "config/logicconfigmanager.hpp"
#include "internalcomm.h"
#include "other/evaluate/roleevaluate.hpp"
#include "world.h"
#include "item/itempool.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "other/event/eventhandler.hpp"
#include "obj/otherobj/fallingitem.h"
#include "scene/guildshadow/guildshadow.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "gameworld/other/fb/rolefbequip.hpp"
#include "gameworld/item/itempool.h"
#include "obj/otherobj/fallingitem.h"
#include "scene/roleequipfbmanager/roleequipfbmanager.hpp"

SpecialNewEquip::SpecialNewEquip(Scene *scene)
	: SpecialLogic(scene), m_owner_objid(INVALID_OBJ_ID), m_owner_user_id(INVALID_USER_ID), m_is_finish(false), m_is_passed(false), 
	m_is_boss_flush(false), m_is_monster_flush(false), m_get_coin(0), m_get_item_count(0), m_boss_id(0), m_sync_logic_info_time(0),
	m_kill_monster_num(0),m_kill_boss_num(0),m_sec_star(3)
{

}

SpecialNewEquip::~SpecialNewEquip()
{

}

void SpecialNewEquip::Update(unsigned long interval, time_t now_second)
{
	if (m_is_finish) 
	{
		return;
	}

	SpecialLogic::Update(interval, now_second);

	if (!m_is_monster_flush)
	{
		m_is_monster_flush = true;

		const NeqFBLevelCfg *lvl_cfg = LOGIC_CONFIG->GetNeqFBConfig().GetLevelByScene(m_scene->GetSceneID());
		if (NULL != lvl_cfg)
		{
			for (int i = 0; i < NeqFBLevelCfg::MAX_MONSTER_TO_FLUSH; ++i)
			{
				if(lvl_cfg->monster_pos_list[i].x > 0 && lvl_cfg->monster_pos_list[i].y > 0)
				{
					MONSTERPOOL->CreateMonster(lvl_cfg->monster_id, m_scene, lvl_cfg->monster_pos_list[i]);
				}
			}

			m_boss_id = lvl_cfg->boss_id;
			MONSTERPOOL->CreateMonster(lvl_cfg->boss_id, m_scene, Posi(lvl_cfg->boss_x, lvl_cfg->boss_y));
		}

		return;
	}
	
	this->CheckSceneMonsterStatus(interval, now_second);

	this->CheckFBStar();

	if (now_second > m_sync_logic_info_time + 5)
	{
		this->SyncNewEQLogicInfo();
	}
}

void SpecialNewEquip::OnRoleEnterScene(Role *role)
{
	if (nullptr == role)
	{
		return;
	}
	if (m_is_finish)
	{
		this->DelayKickOutRole(role, CHANGE_SCENE_REASON_FB_FINISH);
	}
	else
	{
		bool is_first_enter = (INVALID_USER_ID == m_owner_user_id);

		m_owner_user_id = role->GetUserId();
		m_owner_objid = role->GetId();

		EventHandler::Instance().OnEnterFBEquip(role, is_first_enter, false);
	}
	//role->GetFBEquip()->AddTodayFightAllTimes();	//进入的时候不消耗次数
	this->SyncNewEQLogicInfo();
}

void SpecialNewEquip::OnLeaveFB(Role *role)
{
	if (NULL == role) return;

	if (!m_is_finish)
	{
		m_is_finish = true;
		m_is_passed = false;

		this->KillAllMonster();

		role->NotifyFBFinish();
		role->GetFBEquip()->OnPassNeqFB(false, 0, 0, 0);
		
	}
}

Role * SpecialNewEquip::GetOwnerRole()
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

void SpecialNewEquip::OnAllMonsterDead()
{
	if (m_is_finish) return;

	m_is_finish = true;
	m_is_passed = true; 

	this->NotifyBossHPPercent(m_boss_id);

	Role *role = this->GetOwnerRole();
	if (NULL == role) return;

	role->NotifyFBFinish();
	role->Recover();

	const NeqFBLevelCfg *lvl_cfg = LOGIC_CONFIG->GetNeqFBConfig().GetLevelByScene(m_scene->GetSceneID());
	if (NULL != lvl_cfg)
	{
		int seconds = (int)(EngineAdapter::Instance().Time() - m_scene->GetCreateTime());
		if (seconds < 0)
		{
			seconds = 0;
		}

		role->GetFBEquip()->OnPassNeqFB(true, lvl_cfg->chapter, lvl_cfg->level, seconds);

		Protocol::SCFBDropInfo fbdi;
		fbdi.get_coin = m_get_coin;
		fbdi.get_item_count = 0;

		memset(fbdi.drop_items, 0, sizeof(Protocol::SCFBDropInfo::DropItem) * Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM);
		for (int i = 0;i < m_get_item_count && i < Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM; ++ i)
		{
			fbdi.drop_items[i].item_id = m_reward_item_list[i].item_id;
			fbdi.drop_items[i].num = static_cast<short>(m_reward_item_list[i].num);

			++ fbdi.get_item_count;
		}

		int len = sizeof(fbdi) - sizeof(fbdi.drop_items[0]) * (Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM - fbdi.get_item_count);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&fbdi, len);
	}
}

bool SpecialNewEquip::OnTimeout()
{
	if (!m_is_finish)
	{
		m_is_finish = true;
		m_is_passed = false;

		this->KillAllMonster();

		Role *role = this->GetOwnerRole();
		if (NULL != role)
		{
			role->NotifyFBFinish();
			role->GetFBEquip()->OnPassNeqFB(false, 0, 0, 0);
			this->DelayKickOutRole(role, CHANGE_SCENE_REASON_FB_FINISH);
		}
	}

	return m_is_passed;
}

void SpecialNewEquip::OnPickFallingItem(Role *role, FallingItem *falling_item)
{
	if (NULL != role && NULL != falling_item)
	{
		const ItemBase *item = ITEMPOOL->GetItem(falling_item->GetItemId());
		if (NULL != item)
		{
			bool is_find = false;

			for (int i = 0; i < m_get_item_count && i < REWARD_ITEM_MAX_TYPE; ++ i)
			{
				if (m_reward_item_list[i].item_id == falling_item->GetItemId())
				{
					m_reward_item_list[i].num += falling_item->GetItemNum();
					is_find = true;

					break;
				}	
			}

			if (!is_find && m_get_item_count < REWARD_ITEM_MAX_TYPE)
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
	}
}

void SpecialNewEquip::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL != role)
	{
		RoleEquipFBManager::Instance().CheckRollState(role);
	}
}

void SpecialNewEquip::OnCharacterDie(Character *dead, ObjID killer_id)
{
	Monster * monster = dynamic_cast<Monster*>(dead);
	if (nullptr == monster)
	{
		return;
	}
	if (monster->IsBossMonster())
	{
		m_kill_boss_num++;
	}
	else
	{
		m_kill_monster_num++;
	}
	this->SyncNewEQLogicInfo();
}

bool SpecialNewEquip::SpecialRelive(Role *role, int realive_type)
{
	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		//Posi realive_pos;
		//int target_scene_id = m_scene->GetTownPoint(role, &realive_pos);
		const NeqFBLevelCfg *lvl_cfg = LOGIC_CONFIG->GetNeqFBConfig().GetLevelByScene(m_scene->GetSceneID());
		if (nullptr != lvl_cfg)
		{
			if (role->ReAlive(10, 10, Posi(lvl_cfg->pos_x,lvl_cfg->pos_y)))
			{
				return true;
			}
		}
	}	
	
	return false;
}

void SpecialNewEquip::CheckFBStar()
{
	const NeqFBLevelCfg *lvl_cfg = LOGIC_CONFIG->GetNeqFBConfig().GetLevelByScene(m_scene->GetSceneID());
	if (NULL != lvl_cfg)
	{
		int pass_time_s = this->GetPassTime();
		int old_star = m_sec_star;
		if (pass_time_s < lvl_cfg->sec_3_star)
		{
			m_sec_star = 3;
		}
		else if (pass_time_s < lvl_cfg->sec_2_star)
		{
			m_sec_star = 2;
		}
		else
		{
			m_sec_star = 1;
		}

		if (old_star != m_sec_star)
		{
			this->SyncNewEQLogicInfo();
		}
	}
}

void SpecialNewEquip::SyncNewEQLogicInfo(bool is_active_leave_fb)
{
	m_sync_logic_info_time = static_cast<int>(EngineAdapter::Instance().Time());

	Role *role = this->GetOwnerRole();
	if (NULL != role)
	{
		static Protocol::SCFBSceneLogicInfo cmd;
		cmd.reset();

		cmd.time_out_stamp = m_scene->GetSceneTimeOutStamp();
		cmd.scene_type = this->GetSceneType();

		cmd.is_finish = m_is_finish ? 1 : 0;
		cmd.is_pass = m_is_passed ? 1 : 0;
		cmd.is_active_leave_fb = is_active_leave_fb ? 1 : 0;

		cmd.kill_allmonster_num = m_kill_monster_num + m_kill_boss_num;
		cmd.kill_boss_num = m_kill_boss_num;

		cmd.pass_time_s = this->GetPassTime();
		cmd.coin = m_get_coin;
		cmd.exp = 0;

		const NeqFBLevelCfg *lvl_cfg = LOGIC_CONFIG->GetNeqFBConfig().GetLevelByScene(m_scene->GetSceneID());
		if (NULL != lvl_cfg)
		{
			if (cmd.pass_time_s < lvl_cfg->sec_3_star)
			{
				cmd.param1 = lvl_cfg->sec_3_star + m_scene->GetCreateTime() ;
				cmd.param2 = 3;
			}
			else if (cmd.pass_time_s < lvl_cfg->sec_2_star)
			{
				cmd.param1 = lvl_cfg->sec_2_star + m_scene->GetCreateTime();
				cmd.param2 = 2;
			}
			else
			{
				cmd.param2 = 1;
			}
		}
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
	}
}


