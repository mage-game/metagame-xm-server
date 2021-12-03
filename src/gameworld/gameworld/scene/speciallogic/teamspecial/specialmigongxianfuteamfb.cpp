#include "specialmigongxianfuteamfb.hpp"
#include "config/logicconfigmanager.hpp"
#include "other/fb/migongxianfuconfig.hpp"
#include "monster/monsterpool.h"
#include "engineadapter.h"
#include "global/team/team.hpp"
#include "obj/character/role.h"
#include "protocal/msgfb.h"
#include "item/itempool.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "scene/worldshadow/worldshadow.hpp"
#include "obj/character/monster.h"
#include "other/event/eventhandler.hpp"
#include "world.h"
#include "global/team/teammanager.hpp"
#include "global/team/team.hpp"
#include "servercommon/errornum.h"
#include "friend/teaminfo.hpp"

SpecialMiGongXianFuTeamFb::SpecialMiGongXianFuTeamFb(Scene *scene)
	: SpecialLogic(scene), m_is_init(false), m_layer(0), m_team_index(-1)
{

}

SpecialMiGongXianFuTeamFb::~SpecialMiGongXianFuTeamFb()
{
	
}

void SpecialMiGongXianFuTeamFb::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	MgxfTeamFbLogicData *m_logic_data = this->GetLogicData();

	if (NULL == m_logic_data)
	{
		return;
	}
	
	if (!m_is_init)
	{
		m_is_init = true;
		const MiGongXianFuLayerItemCfg *layer_cfg = LOGIC_CONFIG->GetMiGongXianFuTeamFbCfg().GetLayerItemCfgBySceneId(m_scene->GetSceneID());
		if (NULL == layer_cfg)
		{
			return;
		}
		
		{
			m_logic_data->mode = layer_cfg->mode;
			m_layer = layer_cfg->layer;

			if ((MGXF_LAYER_TYPE_HIDE == layer_cfg->layer_type && 0 == m_logic_data->kill_hide_boss_num) || 
				(MGXF_LAYER_TYPE_BOSS == layer_cfg->layer_type && 0 == m_logic_data->kill_end_boos_num))
			{
				MONSTERPOOL->CreateMonster(layer_cfg->boss_id, m_scene, layer_cfg->boss_pos);
			}
		}

		{
			const MiGongXianFuModeItemCfg * mode_cfg = LOGIC_CONFIG->GetMiGongXianFuTeamFbCfg().GetModeItemCfg(m_logic_data->mode);
			if (NULL != mode_cfg && 0 == m_logic_data->start_time)  //确保只会设置一次
			{
				m_logic_data->start_time = static_cast<int>(EngineAdapter::Instance().Time());
				m_logic_data->time_out_stamp = m_logic_data->start_time + mode_cfg->time;
			}
		}

		MiGongXianFuMgr::Instance().SyncSceneLogicInfo(m_logic_data->team_index);
	}

	if (m_logic_data->time_out_stamp > 0 && now_second >= m_logic_data->time_out_stamp)  // 整个副本玩法超时
	{
		this->OnFinish(false);
		this->DelayKickOutAllRole();
	}
	
	if (m_logic_data->is_finish)
	{
		return;
	}
}

void SpecialMiGongXianFuTeamFb::OnRoleEnterScene(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	m_team_index = role->GetTeamInfo()->GetTeamIndex();

	if (NULL == this->GetLogicData())
	{
		MiGongXianFuMgr::Instance().OnAddTeam(m_team_index);
	}

	MiGongXianFuMgr::Instance().SyncSceneLogicInfo(m_team_index);
}

bool SpecialMiGongXianFuTeamFb::SpecialIsEnemy(Role *role, Obj *obj)
{
	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		return false;
	}

	return SpecialLogic::SpecialIsEnemy(role, obj);
}

void SpecialMiGongXianFuTeamFb::OnTouchDoor(Role *role, int layer, int door_index)
{
	MgxfTeamFbLogicData *m_logic_data = this->GetLogicData();

	if (NULL == role || NULL == m_logic_data)
	{
		return;
	}

	const MiGongXianFuLayerItemCfg *item_cfg = LOGIC_CONFIG->GetMiGongXianFuTeamFbCfg().GetLayerItemCfgBySceneId(m_scene->GetSceneID());
	if (NULL == item_cfg)
	{
		return;
	}

	if (item_cfg->layer != layer)  //确认是在本层
	{
		return;
	}

	if (door_index < 0 || door_index >= MIGONGXIANFU_DOOR_NUM)
	{
		return;
	}
	
 	Posi door_pos = item_cfg->door_list[door_index];
 	if ((door_pos - role->GetPos()).IsLongerThan(20))  //确定在传送点附近，测试时先屏掉
 	{
		role->NoticeNum(errornum::EN_MGXF_NOT_NEAR_DOOR);
 		return;
 	}
	
	int door_status = MiGongXianFuMgr::Instance().OnTouchDoor(m_logic_data->team_index, layer, door_index);
	if (MGXF_DOOR_STATUS_NONE == door_status)
	{
		role->NoticeNum(errornum::EN_MGXF_DOOR_NOT_EXIST);
		return;
	}

	int to_layer = 0;
	// 计算要去的层
	{
		if (MGXF_DOOR_STATUS_TO_PRVE == door_status)
		{
			to_layer = m_layer - 1;
		}

		if (MGXF_DOOR_STATUS_TO_HERE == door_status)
		{
			to_layer = m_layer;
		}

		if (MGXF_DOOR_STATUS_TO_NEXT == door_status)
		{
			to_layer = m_layer + 1;
		}

		if (MGXF_DOOR_STATUS_TO_HIDE == door_status)
		{
			m_logic_data->is_entered_hide = true;  // 记录进过隐藏传送点，减少了每次遍历检查有无进过
			to_layer = MIGONGXIANFU_HIDE_LAYER_INDEX;
		}

		if (MGXF_DOOR_STATUS_TO_BOSS == door_status)
		{
			to_layer = MIGONGXIANFU_BOSS_LAYER_INDEX;
		}

		if (MGXF_DOOR_STATUS_TO_FIRST == door_status)
		{
			to_layer = MIGONGXIANFU_LAYER_FIRST_INDEX;
		}
	}

	{
		const MiGongXianFuLayerItemCfg *item_cfg = LOGIC_CONFIG->GetMiGongXianFuTeamFbCfg().GetLayerItemCfg(m_logic_data->mode, to_layer);
		if (NULL == item_cfg)
		{
			role->NoticeNum(errornum::EN_MGXF_LAYER_NOT_EXIST);
			return;
		}

		int scene_key =MiGongXianFuMgr::Instance().GetChangeSceneKey(m_logic_data->team_index, to_layer);  //不同的层有不同的scene_key, 做个层的偏移
		bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(role, item_cfg->scene_id, scene_key, item_cfg->enter_pos);
		if (!ret)
		{
			role->NoticeNum(errornum::EN_FB_ENTER_FB_ERROR);
		}
	}
	
	MiGongXianFuMgr::Instance().SyncSceneLogicInfo(m_logic_data->team_index);
}

void SpecialMiGongXianFuTeamFb::OnRemoveObj(Obj *obj)
{
	MgxfTeamFbLogicData *m_logic_data = this->GetLogicData();

	if (NULL == m_logic_data || NULL == obj)
	{
		return;
	}

	const MiGongXianFuLayerItemCfg *item_cfg = LOGIC_CONFIG->GetMiGongXianFuTeamFbCfg().GetLayerItemCfgBySceneId(m_scene->GetSceneID());
	if (NULL == item_cfg)
	{
		return;
	}

	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *monster = (Monster *)obj;

		if (MGXF_LAYER_TYPE_HIDE == item_cfg->layer_type && monster->GetMonsterId() == item_cfg->boss_id)
		{
			++ m_logic_data->kill_hide_boss_num;
		}

		if (MGXF_LAYER_TYPE_BOSS == item_cfg->layer_type && monster->GetMonsterId() == item_cfg->boss_id)
		{
			++ m_logic_data->kill_end_boos_num;
			this->OnFinish(true);
		}
	}

	MiGongXianFuMgr::Instance().SyncSceneLogicInfo(m_logic_data->team_index);
}

void SpecialMiGongXianFuTeamFb::OnLeaveFB(Role *role)
{
	MgxfTeamFbLogicData *m_logic_data = this->GetLogicData();

	if (NULL == m_logic_data || NULL == role)
	{
		return;
	}

	if (m_logic_data->team_index == role->GetTeamInfo()->GetTeamIndex())
	{
		Team *team = TeamManager::Instance().GetTeam(m_logic_data->team_index);
		if (NULL != team)
		{
			TeamManager::Instance().RemoveTeamMember(role, m_logic_data->team_index, role->GetUserId());
		}
	}
}

void SpecialMiGongXianFuTeamFb::OnFinish(bool is_pass)
{
	MgxfTeamFbLogicData *m_logic_data = this->GetLogicData();

	if (NULL == m_logic_data) return;

	if (m_logic_data->is_finish)
	{
		return;
	}

	m_logic_data->is_finish = true;
	m_logic_data->is_pass = is_pass;

	if (m_logic_data->is_finish && m_logic_data->is_pass && m_logic_data->kill_end_boos_num > 0)
	{
		MiGongXianFuMgr::Instance().PutPassReward(m_logic_data->team_index);
	}

	MiGongXianFuMgr::Instance().SyncSceneLogicInfo(m_logic_data->team_index);
}

MgxfTeamFbLogicData *SpecialMiGongXianFuTeamFb::GetLogicData()
{
	return MiGongXianFuMgr::Instance().GetLogicData(m_team_index);
}