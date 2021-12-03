#include "obj/character/role.h"

#include "scene/scene.h"
#include "scene/scenemanager.h"

#include "global/team/teammanager.hpp"
#include "teamfbroom.hpp"
#include "protocal/msgfb.h"
#include "servercommon/servercommon.h"
#include "npc/transmit/fbmanager.hpp"
#include "servercommon/errornum.h"
#include "migongxianfumgr.hpp"
#include "config/logicconfigmanager.hpp"
#include "other/fb/migongxianfuconfig.hpp"
#include "world.h"
#include "other/event/eventhandler.hpp"
#include "item/knapsack.h"
#include "other/daycounter/daycounter.hpp"

MiGongXianFuMgr::MiGongXianFuMgr()
{

}

MiGongXianFuMgr::~MiGongXianFuMgr()
{

}

MiGongXianFuMgr & MiGongXianFuMgr::Instance()
{
	static MiGongXianFuMgr key; 
	return key; 
}

void MiGongXianFuMgr::OnDestroyTeam(int team_index)
{
	m_data_map.erase(team_index);
}

void MiGongXianFuMgr::OnAddTeam(int team_index)
{
	LogicDataMap::iterator iter = m_data_map.find(team_index);

	if (m_data_map.end() == iter)
	{
		MgxfTeamFbLogicData logic_data;
		logic_data.team_index = team_index;

		m_data_map[logic_data.team_index] = logic_data;
	}
}

MgxfTeamFbLogicData *MiGongXianFuMgr::GetLogicData(int team_index)
{
	LogicDataMap::iterator iter = m_data_map.find(team_index);
	if (m_data_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

MgxfTeamFbLogicData::LayerItem *MiGongXianFuMgr::GetLayerItem(int team_index, int layer)
{
	MgxfTeamFbLogicData *logic_data = this->GetLogicData(team_index);
	if (NULL == logic_data) return NULL;

	if (layer < 0 || layer >= MIGONGXIANFU_LAYER_NUM)
	{
		return NULL;
	}

	return &logic_data->layer_list[layer];
}

int MiGongXianFuMgr::GetChangeSceneKey(int team_index, int layer)
{
	return team_index * 1000000 + layer;
}

int MiGongXianFuMgr::OnTouchDoor(int team_index, int layer, int door_index)
{
	MgxfTeamFbLogicData *logic_data = this->GetLogicData(team_index);
	if (NULL == logic_data)
	{
		return MGXF_DOOR_STATUS_NONE;
	}

	if (layer < 0 || layer >= MIGONGXIANFU_LAYER_NUM)
	{
		return MGXF_DOOR_STATUS_NONE;
	}

	if (door_index < 0 || door_index >= MIGONGXIANFU_DOOR_NUM)
	{
		return MGXF_DOOR_STATUS_NONE;
	}
	
	const MiGongXianFuLayerItemCfg *layer_cfg = LOGIC_CONFIG->GetMiGongXianFuTeamFbCfg().GetLayerItemCfg(logic_data->mode, layer);
	if (NULL == layer_cfg) 
	{
		return MGXF_DOOR_STATUS_NONE;
	}

	MgxfTeamFbLogicData::LayerItem *layer_item = &logic_data->layer_list[layer];
	int *door_status = &layer_item->door_status_list[door_index];

	if (MGXF_DOOR_STATUS_NONE != *door_status)
	{
		return *door_status;
	}

	if (MGXF_LAYER_TYPE_HIDE ==  layer_cfg->layer_type || MGXF_LAYER_TYPE_BOSS == layer_cfg->layer_type) //隐藏层,boss层的传送点：返回第一层
	{
		*door_status = MGXF_DOOR_STATUS_TO_FIRST;
		return *door_status;
	}
	
	if (!logic_data->is_entered_hide && this->CheckMustToHide(team_index))				//如果还未去过则，确保所有在倒数第一个之前可以去到隐藏层
	{
		*door_status = MGXF_DOOR_STATUS_TO_HIDE;
		return *door_status;
	}

	//本层所有传送点只剩最后一个时，必须去下一层
	{
		int door_num = 0;
		for (int i = 0; i < MIGONGXIANFU_DOOR_NUM; ++ i)
		{
			int check_door_status = layer_item->door_status_list[i];
			if (MGXF_DOOR_STATUS_NONE != check_door_status &&
				MGXF_DOOR_STATUS_TO_NEXT != check_door_status)
			{
				++ door_num;
			}
		}

		if (MIGONGXIANFU_DOOR_NUM - 1 == door_num) 
		{
			*door_status = MGXF_DOOR_STATUS_TO_NEXT;
			return *door_status;
		}
	}

	// 判断本层是否已探索到下一层的传送点
	bool is_has_next_status_door = false; 
	{
		for (int i = 0; i < MIGONGXIANFU_DOOR_NUM; ++ i)
		{
			int check_door_status = layer_item->door_status_list[i];
			if (MGXF_DOOR_STATUS_TO_NEXT == check_door_status)
			{
				is_has_next_status_door = true;
				break;
			}
		}
	}

	//按权重随机
	{
		int total_weight =  layer_cfg->to_prve_weight + layer_cfg->to_here_weight;
		if (!logic_data->is_entered_hide)  //去过隐藏层则不再参与
		{
			total_weight += layer_cfg->to_hide_weight;
		}
		if (!is_has_next_status_door)	   //当前层已有探索过去下一层的传送点则不再参与
		{
			total_weight += layer_cfg->to_next_weight;
		}

		if (total_weight <= 0)
		{
			return MGXF_DOOR_STATUS_NONE;
		}

		int to_rand = RandomNum(total_weight);

		{
			if (layer_cfg->to_prve_weight > 0 && to_rand <= layer_cfg->to_prve_weight)
			{
				*door_status = MGXF_DOOR_STATUS_TO_PRVE;
				return *door_status;
			}
			to_rand -= layer_cfg->to_prve_weight;
		}

		{
			if (layer_cfg->to_here_weight > 0 && to_rand <= layer_cfg->to_here_weight)
			{
				*door_status = MGXF_DOOR_STATUS_TO_HERE;
				return *door_status;
			}
			to_rand -= layer_cfg->to_here_weight;
		}

		{
			if (layer_cfg->to_next_weight > 0 && to_rand <= layer_cfg->to_next_weight && !is_has_next_status_door)
			{
				*door_status = MGXF_DOOR_STATUS_TO_NEXT;
				return *door_status;
			}
			to_rand -= layer_cfg->to_next_weight;
		}
		
		{
			if (layer_cfg->to_hide_weight > 0 && to_rand <= layer_cfg->to_hide_weight && !logic_data->is_entered_hide)
			{
				*door_status = MGXF_DOOR_STATUS_TO_HIDE;
				return *door_status;
			}
			to_rand -= layer_cfg->to_hide_weight;
		}
	}

	return MGXF_DOOR_STATUS_NONE;
}

bool MiGongXianFuMgr::CheckMustToHide(int team_index)
{
	MgxfTeamFbLogicData *logic_data = this->GetLogicData(team_index);
	if (NULL == logic_data) return false;

	int door_num = 0;

	for (int i = 0; i < MIGONGXIANFU_NORMAL_LAYER_NUM; ++ i)
	{
		for (int k = 0; k < MIGONGXIANFU_DOOR_NUM; ++ k)
		{
			int status = logic_data->layer_list[i].door_status_list[k];
			if (MGXF_DOOR_STATUS_NONE != status && MGXF_DOOR_STATUS_TO_HIDE != status)
			{
				++ door_num;
			}
		}
	}
	
	// 若之前没出现隐藏boss,则倒数第二个必定是去隐藏层，确保还有层可以去下一层（简单处理）
	if (door_num == MIGONGXIANFU_NORMAL_LAYER_NUM * MIGONGXIANFU_DOOR_NUM - 2) 
	{
		return true;
	}

	return false;
}


void MiGongXianFuMgr::PutPassReward(int team_index)
{
	MgxfTeamFbLogicData *logic_data = this->GetLogicData(team_index);
	if (NULL == logic_data) return;

	const MiGongXianFuModeItemCfg *mode_cfg = LOGIC_CONFIG->GetMiGongXianFuTeamFbCfg().GetModeItemCfg(logic_data->mode);
	if (NULL == mode_cfg) return;

	if (!logic_data->is_finish || !logic_data->is_pass)
	{
		return;
	}

	Team *team = TeamManager::Instance().GetTeam(logic_data->team_index);
	if (NULL == team)
	{
		return;
	}

	// 不在副本里的队员不拿奖励也不扣次数，先打完隐藏boss，双倍奖励

	ItemConfigData item1 = mode_cfg->reward_item1;
	ItemConfigData item2 = mode_cfg->reward_item2;
	ItemConfigData item3 = mode_cfg->reward_item3;
	if (logic_data->is_entered_hide && logic_data->kill_hide_boss_num > 0)
	{
		item1.num *= 2;
		item2.num *= 2;
		item3.num *= 2;
	}

	ItemConfigData item_configdata_list[MAX_ATTACHMENT_ITEM_NUM] = {item1, item2, item3};

	int join_user_list[MAX_TEAM_MEMBER_NUM]; memset(join_user_list, 0, sizeof(join_user_list));
	int join_user_count = team->GetMemberUidList(join_user_list, MAX_TEAM_MEMBER_NUM);
	for (int i = 0; i < join_user_count && i < MAX_TEAM_MEMBER_NUM; ++ i)
	{
		UserID user_id = IntToUid(join_user_list[i]);

		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
		if (NULL == role)
		{
			continue;
		}

		if (NULL == role->GetScene() || Scene::SCENE_TYPE_MIGONGXIANFU_TEAM != role->GetScene()->GetSceneType())
		{
			continue;
		}

		if (role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_MIGOGNXIANFU_JOIN_TIMES) >= mode_cfg->free_join_times)
		{
			continue;
		}

		role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_MIGOGNXIANFU_JOIN_TIMES);
		role->GetKnapsack()->PutListOrMail(item_configdata_list, PUT_REASON_MIGONGXIANFU_REWARD);

		EventHandler::Instance().OnPassMgxfTeamFb(role);
	}
}

void MiGongXianFuMgr::SyncSceneLogicInfo(int team_index)
{
	MgxfTeamFbLogicData *logic_data = this->GetLogicData(team_index);
	if (NULL == logic_data)
	{
		return;
	}

	const MiGongXianFuModeItemCfg *mode_cfg = LOGIC_CONFIG->GetMiGongXianFuTeamFbCfg().GetModeItemCfg(logic_data->mode);
	if (NULL == mode_cfg)
	{
		return;
	}

	Team *team = TeamManager::Instance().GetTeam(logic_data->team_index);
	if (NULL == team)
	{
		return;
	}
	
	int pass_time_s = 0;
	{
		pass_time_s = static_cast<unsigned int>(EngineAdapter::Instance().Time() - logic_data->start_time);
		if (pass_time_s < 0)
		{
			pass_time_s = 0;
		}

		if (pass_time_s > mode_cfg->time)
		{
			pass_time_s = mode_cfg->time;
		}
	}

	static Protocol::SCMgxfTeamFbSceneLogicInfo cmd;
	cmd.time_out_stamp = logic_data->time_out_stamp;
	cmd.is_pass = logic_data->is_pass ? 1 : 0;
	cmd.is_finish = logic_data->is_finish? 1 : 0;
	cmd.pass_time_s = pass_time_s;
	cmd.mode = logic_data->mode;
	cmd.kill_hide_boos_num = logic_data->kill_hide_boss_num;
	cmd.kill_end_boss_num = logic_data->kill_end_boos_num;
	
	for (int i = 0; i < MIGONGXIANFU_LAYER_NUM; ++ i)
	{
		memcpy(cmd.layer_list[i].door_status_list,  logic_data->layer_list[i].door_status_list, sizeof(cmd.layer_list[i].door_status_list));
	}

	// 发给队伍里且还在副本的所有人
	{
		int join_user_list[MAX_TEAM_MEMBER_NUM]; memset(join_user_list, 0, sizeof(join_user_list));
		int join_user_count = team->GetMemberUidList(join_user_list, MAX_TEAM_MEMBER_NUM);
		for (int i = 0; i < join_user_count && i < MAX_TEAM_MEMBER_NUM; ++ i)
		{
			UserID user_id = IntToUid(join_user_list[i]);

			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
			if (NULL == role)
			{
				continue;
			}

			if (NULL == role->GetScene() || Scene::SCENE_TYPE_MIGONGXIANFU_TEAM != role->GetScene()->GetSceneType())
			{
				continue;
			}
			
			const MiGongXianFuLayerItemCfg *item_cfg =  LOGIC_CONFIG->GetMiGongXianFuTeamFbCfg().GetLayerItemCfgBySceneId(role->GetScene()->GetSceneID());
			if (NULL == item_cfg)
			{
				continue;
			}
			
			cmd.layer = item_cfg->layer;
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
		}
	}
}

