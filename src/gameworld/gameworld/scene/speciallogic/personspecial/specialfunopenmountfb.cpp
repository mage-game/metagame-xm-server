#include "specialfunopenmountfb.hpp"
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "obj/otherobj/gatherobj.h"
#include "engineadapter.h"
#include "config/logicconfigmanager.hpp"
#include "other/fb/funopenfbconfig.hpp"
#include "other/event/eventhandler.hpp"
#include "monster/monsterpool.h"

SpecialFunOpenMountFb::SpecialFunOpenMountFb(Scene *scene)
: SpecialLogic(scene), m_owner_uid(0), m_is_finish(0), m_can_operate_next_step(false), m_cur_step_monster_total_num(0), m_cur_step_monster_kill_num(0), 
	m_cur_step_gather_total_num(0), m_cur_step_gather_num(0), m_cur_step(0), m_mount_gather_id(0)
{

}

SpecialFunOpenMountFb::~SpecialFunOpenMountFb()
{
}

void SpecialFunOpenMountFb::RefreshStepMonsterClienReq(Role *role, Protocol::CSFunOpenStoryStep *cmd)
{
	if (m_is_finish || NULL == role || NULL == cmd) return;

	if (cmd->create_monster_step <= INVALID_STEP || cmd->create_monster_step >= CREATE_MONSTER_STEP_MAX) return;

	if (cmd->create_monster_step != m_cur_step + 1) return;

	if (m_can_operate_next_step)
	{
		m_can_operate_next_step = false;
		m_monster_set.clear();
		m_cur_step = cmd->create_monster_step;
		m_cur_step_monster_kill_num = 0;
		m_cur_step_monster_total_num = 0;

		m_cur_step_gather_num = 0;
		m_cur_step_gather_total_num = 0;

		std::map<int, FunOpenMonsterCfg>::iterator iter = m_monster_cfg_map.begin();
		for(;iter != m_monster_cfg_map.end(); iter++)
		{
			if (FUN_OPEN_FB_MOUNT == iter->second.fb_type && cmd->create_monster_step == iter->second.step)
			{
				Monster *monster = MONSTERPOOL->CreateMonster(iter->second.monster_id, m_scene, iter->second.bron_pos);
				if (NULL != monster)
				{
					m_monster_set.insert(monster->GetMonsterId());
					m_cur_step_monster_total_num++;
				}
			}
		}

		std::map<int, FunOpenGatherCfg>::iterator iter_gather = m_gather_cfg_map.begin();
		for(;iter_gather != m_gather_cfg_map.end(); iter_gather++)
		{
			if (FUN_OPEN_FB_MOUNT == iter_gather->second.fb_type && cmd->create_monster_step == iter_gather->second.step)
			{
				GatherObj *gather_obj = new GatherObj();
				gather_obj->Init(NULL, iter_gather->second.gather_id, iter_gather->second.gather_time * 1000, 0, true);
				gather_obj->SetPos(iter_gather->second.bron_pos);
				m_scene->AddObj(gather_obj);

				m_cur_step_gather_total_num++;
			}
		}

		this->SyncFBInfo();
	}
}

void SpecialFunOpenMountFb::OnCreateGather(Role *role, int gather_x, int gather_y, int gather_id, int gather_time)
{
	const FunOpenGatherCfg *gather_cfg = LOGIC_CONFIG->GetFunOpenFBCfg().GetFunOpenGatherCfg(FUN_OPEN_FB_MOUNT, gather_id);
	if (NULL == gather_cfg)
	{
		return;
	}

	GatherObj *gather_obj = new GatherObj();
	gather_obj->Init(NULL, gather_id, gather_time * 1000, 0, true);
	Posi posi(gather_x, gather_y);
	gather_obj->SetPos(posi);
	m_scene->AddObj(gather_obj);
}

void SpecialFunOpenMountFb::SetObjPos(Role * role, Protocol::CSFunOpenSetObjToPos *cmd)
{
	if (m_is_finish || NULL == role || NULL == cmd) return;

	Obj *obj = m_scene->GetObj(cmd->obj_id);
	if (NULL == obj) return;

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *temp_role	= (Role*)obj;
		if (NULL != temp_role)
		{
			temp_role->ResetPos(cmd->pos_x, cmd->pos_y);
		}
	}
	else if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *temp_monster = (Monster *)obj;
		if (NULL != temp_monster)
		{
			temp_monster->ResetPos(cmd->pos_x, cmd->pos_y);
		}
	}
}

void SpecialFunOpenMountFb::OnRoleEnterScene(Role *role)
{
	if (m_is_finish)
	{
		this->DelayKickOutAllRole();
	}

	if (0 == m_owner_uid)
	{
		m_owner_uid = role->GetUID();

		std::map<int, FunOpenMonsterCfg> &monster_map = LOGIC_CONFIG->GetFunOpenFBCfg().GetFunOpenMonsterCfg();
		m_monster_cfg_map = monster_map;

		std::map<int, FunOpenGatherCfg> &gahter_map = LOGIC_CONFIG->GetFunOpenFBCfg().GetFunOpenGatherCfgMap();
		m_gather_cfg_map = gahter_map;

		m_is_finish = false;
		m_cur_step_monster_total_num = 0;
		m_cur_step_monster_kill_num = 0;
		m_can_operate_next_step = true;

		const FunOpenOtherCfg &other_cfg = LOGIC_CONFIG->GetFunOpenFBCfg().GetOtherCfg();
		m_mount_gather_id = other_cfg.mount_gather_id;

		this->SyncFBInfo();
		this->SyncFBStepEndInfo();
	}
	else
	{
		this->DelayKickOutRole(role);
	}
}

void SpecialFunOpenMountFb::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead)
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE == dead->GetObjType())
	{
		m_is_finish = false;
		this->SyncFBInfo();
		this->DelayKickOutAllRole();
	}
	else if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster *death_monster = (Monster*)dead;

		if (m_monster_set.find(death_monster->GetMonsterId()) != m_monster_set.end())
		{
			m_cur_step_monster_kill_num++;

			if (m_cur_step_monster_kill_num >= m_cur_step_monster_total_num)
			{
				m_can_operate_next_step = true;
				this->SyncFBStepEndInfo();
			}
		}
	}

	this->SyncFBInfo();
}

void SpecialFunOpenMountFb::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);
}

void SpecialFunOpenMountFb::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	if(m_mount_gather_id ==  gather_id)
	{
		m_is_finish = true;
		m_cur_step_gather_num++;
	}
	
	this->SyncFBInfo();
}

void SpecialFunOpenMountFb::SyncFBInfo()
{
	Role *role = m_scene->GetRoleByUID(m_owner_uid);
	if (NULL != role)
	{
		static Protocol::SCFunOpenMountInfo cmd;

		cmd.is_finish = m_is_finish ? 1 : 0;
		cmd.reserve_1 = 0;
		cmd.reserve_2 = 0;

		cmd.cur_step_monster_total_num = m_cur_step_monster_total_num;
		cmd.cur_step_monster_kill_num = m_cur_step_monster_kill_num;

		cmd.cur_step_gather_total_num = m_cur_step_gather_total_num;
		cmd.cur_step_gather_num = m_cur_step_gather_num;

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
	}
}

void SpecialFunOpenMountFb::SyncFBStepEndInfo()
{
	Role *role = m_scene->GetRoleByUID(m_owner_uid);
	if (NULL != role)
	{
		static Protocol::SCFunOpenStoryStepEnd cmd;

		cmd.cur_step = m_cur_step;

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
	}
}

