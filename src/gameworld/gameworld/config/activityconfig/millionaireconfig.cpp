#include "millionaireconfig.hpp"
#include "gamecommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "obj/character/monster.h"
#include "obj/otherobj/gatherobj.h"
#include "world.h"
#include "scene/scene.h"
#include "scene/map.h"
#include <string>
#include "scene/worldshadow/worldshadow.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "servercommon/errornum.h"
#include "config/logicconfigmanager.hpp"
#include "servercommon/string/gameworldstr.h"
#include "item/knapsack.h"

MillionaireConfig::MillionaireConfig(): m_total_box_count(0)
{

}

MillionaireConfig::~MillionaireConfig()
{

}

bool MillionaireConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "MillionaireConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode other_element = RootElement.child("other");
		if (other_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(other_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode extra_reward_element = RootElement.child("extra_reaward");
		if (extra_reward_element.empty())
		{
			*err = configname + ": no [extra_reward].";
			return false;
		}

		iRet = this->InitExtraReward(extra_reward_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitExtraReward failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode reward_pool_element = RootElement.child("box_reward");
		if (reward_pool_element.empty())
		{
			*err = configname + ": no [box_reward].";
			return false;
		}

		iRet = this->InitRewardPool(reward_pool_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitRewardPool failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode gather_box_element = RootElement.child("gather_box_cfg");
		if (gather_box_element.empty())
		{
			*err = configname + ": no [gather_box_cfg].";
			return false;
		}

		iRet = this->InitGatherCfg(gather_box_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitGatherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode pos_element = RootElement.child("position");
		if (pos_element.empty())
		{
			*err = configname + ": no [position].";
			return false;
		}

		iRet = this->InitPosCfg(pos_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitPosCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode gather_box_element = RootElement.child("exp_gather_position");
		if (gather_box_element.empty())
		{
			*err = configname + ": no [exp_gather_position].";
			return false;
		}

		iRet = this->InitExpGatherPosCfg(gather_box_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitExpGatherPosCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

bool MillionaireConfig::IsCanGather(Role *role)
{
	if (!ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_MILLIONAIRE))
	{
		role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return false;
	}

	CommonDataParam * common = role->GetCommonDataParam();

	if (common->millionaire_last_join_time != ActivityShadow::Instance().GetActivityBeginTime(ACTIVITY_TYPE_MILLIONAIRE))
	{
		common->millionaire_last_join_time = ActivityShadow::Instance().GetActivityBeginTime(ACTIVITY_TYPE_MILLIONAIRE);
		common->millionaire_gather_count = 0; 
		common->is_turn_millionaire = 0;
		common->is_turn = 0;
		common->millionaire_reward_index = -1;
		common->millionaire_use_skill_times = 0;
		common->millionaire_valid_perform_skill_time = 0;
	}

	if (common->millionaire_gather_count >= LOGIC_CONFIG->GetMillionaireConfig().GetOhterCfg()->role_can_gather_max_times)
	{
		role->NoticeNum(errornum::EN_MILLIONAIRE_GATHER_LIMIT);
		return false;
	}
	
	int has_gathar = common->millionaire_gather_count;

	// 5、15、20获得特殊奖励，所以要判断背包空格是否大于四格，额外奖励3格，随机奖励1格
	if (has_gathar == 4 || has_gathar == 14 || has_gathar == 19)
	{
		if (!role->GetKnapsack()->CheckEmptyGridNoLessThan(4))
		{
			role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return false;
		}
	}
	else if(has_gathar == 9)
	{
		if (!role->GetKnapsack()->CheckEmptyGridNoLessThan(2))
		{
			role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return false;
		}
	}
	else
	{
		if (!role->GetKnapsack()->HaveSpace())
		{
			role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return false;
		}
	}

	return true;
}

void MillionaireConfig::CalcGatherReward(Role *role, int gather_id)
{
	const OtherCfg *other_cfg = LOGIC_CONFIG->GetMillionaireConfig().GetOhterCfg();
	if (NULL == other_cfg) return;

	if (role->GetCommonDataParam()->millionaire_gather_count < other_cfg->role_can_gather_max_times)
	{
		CommonDataParam *common_p = role->GetCommonDataParam();
		++common_p->millionaire_gather_count;
		{
			if (10 == common_p->millionaire_gather_count || 20 == common_p->millionaire_gather_count)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_millionaire_gather_times,
					role->GetUID(), role->GetName(), (int)role->GetCamp(), common_p->millionaire_gather_count);
				if (length > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}

		// 额外奖励，直接放背包
		const ExtraRewardCfg *extra_reward = this->GetExtraReward(common_p->millionaire_gather_count);
		if (NULL != extra_reward)
		{
			role->GetKnapsack()->Put(extra_reward->reward_item1,PUT_REASON_MIllIONAIRE);
			role->GetKnapsack()->Put(extra_reward->reward_item2,PUT_REASON_MIllIONAIRE);
			role->GetKnapsack()->Put(extra_reward->reward_item3,PUT_REASON_MIllIONAIRE);
		}

		const RewardPoolCfg *gather_reward = this->GetRewardPoolReward(gather_id);
		if (NULL != gather_reward)
		{
			if (common_p->is_turn_millionaire)
			{
				role->GetKnapsack()->Put(gather_reward->reward_item,PUT_REASON_MIllIONAIRE);
				role->GetKnapsack()->Put(gather_reward->reward_item,PUT_REASON_MIllIONAIRE);
			}
			else
			{
				role->GetKnapsack()->Put(gather_reward->reward_item,PUT_REASON_MIllIONAIRE);
			}
		}
	}
	else
	{
		role->NoticeNum(errornum::EN_MILLIONAIRE_GATHER_LIMIT);
		return;
	}
}

int MillionaireConfig::GetMillionaireEnterInfo(Posi *pos)
{
	if (NULL == pos) return 0;
	*pos = m_other_cfg.enter_pos;
	return m_other_cfg.scene_id;
}

int MillionaireConfig::InitPosCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		PosCfg positem;

		if (!PugiGetSubNodeValue(dataElement, "scene_id", positem.scene_id))
		{
			return -1;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(positem.scene_id);

		if (!PugiGetSubNodeValue(dataElement, "pos_x", positem.born_pos.x))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", positem.born_pos.y))
		{
			return -3;
		}

		m_pos_vec.push_back(positem);

		dataElement = dataElement.next_sibling();
	}

	if (static_cast<int>(m_pos_vec.size()) < m_total_box_count)
	{
		return -4;
	}

	return 0;
}

int MillionaireConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "refresh_interval_s", m_other_cfg.refresh_interval_s) || m_other_cfg.refresh_interval_s < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "role_gather_max_time", m_other_cfg.role_can_gather_max_times) || m_other_cfg.role_can_gather_max_times <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "level_limit", m_other_cfg.level_limit) || m_other_cfg.level_limit <= 0)
		{
			return -5;
		}

		if(!PugiGetSubNodeValue(dataElement,"scene_id",m_other_cfg.scene_id) || m_other_cfg.scene_id <= 0)
		{
			return -6;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(m_other_cfg.scene_id);

		if (!PugiGetSubNodeValue(dataElement,"fly_pos_x",m_other_cfg.enter_pos.x) || m_other_cfg.enter_pos.x <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement,"fly_pos_y",m_other_cfg.enter_pos.y) || m_other_cfg.enter_pos.y <= 0)
		{
			return -8;
		}

		
		if(!PugiGetSubNodeValue(dataElement,"boss_id", m_other_cfg.exp_boss_id) || m_other_cfg.exp_boss_id < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement,"boss_born_pos_x",m_other_cfg.boss_born_pos.x) || m_other_cfg.boss_born_pos.x <= 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement,"boss_born_pos_y",m_other_cfg.boss_born_pos.y) || m_other_cfg.boss_born_pos.y <= 0)
		{
			return -11;
		}

		if(!PugiGetSubNodeValue(dataElement,"boss_first_refresh_time", m_other_cfg.boss_first_refresh_time) || m_other_cfg.boss_first_refresh_time < 0)
		{
			return -12;
		}

		if(!PugiGetSubNodeValue(dataElement,"boss_refresh_interval", m_other_cfg.boss_refresh_interval) || m_other_cfg.boss_refresh_interval < 0)
		{
			return -13;
		}

		if(!PugiGetSubNodeValue(dataElement,"exp_gather_id", m_other_cfg.exp_gather_id) || m_other_cfg.exp_gather_id < 0)
		{
			return -14;
		}

		if(!PugiGetSubNodeValue(dataElement,"exp_gather_exit_time", m_other_cfg.exp_gather_exit_time) || m_other_cfg.exp_gather_exit_time < 0)
		{
			return -15;
		}

		if(!PugiGetSubNodeValue(dataElement,"exp_gather_gather_time", m_other_cfg.exp_gather_gather_time) || m_other_cfg.exp_gather_gather_time < 0)
		{
			return -16;
		}

		if(!PugiGetSubNodeValue(dataElement,"exp_reward", m_other_cfg.exp_reward) || m_other_cfg.exp_reward < 0)
		{
			return -17;
		}

		if(!PugiGetSubNodeValue(dataElement,"frozen_time", m_other_cfg.frozen_can_use_times) || m_other_cfg.frozen_can_use_times < 0)
		{
			return -18;
		}

		if(!PugiGetSubNodeValue(dataElement,"frozen_cloddown_time", m_other_cfg.frozen_cloddown_time) || m_other_cfg.frozen_cloddown_time < 0)
		{
			return -19;
		}

		if(!PugiGetSubNodeValue(dataElement,"frozen_continue_time", m_other_cfg.frozen_continue_time) || m_other_cfg.frozen_continue_time < 0)
		{
			return -20;
		}

		if(!PugiGetSubNodeValue(dataElement,"frozen_exp_reward", m_other_cfg.frozen_exp_reward) || m_other_cfg.frozen_exp_reward < 0)
		{
			return -21;
		}

		if(!PugiGetSubNodeValue(dataElement,"frozen_distance", m_other_cfg.frozen_distance) || m_other_cfg.frozen_distance <= 0)
		{
			return -22;
		}

		{
			PugiXmlNode element = dataElement.child("frozen_item_reward_list");
			if (element.empty())
			{
				return -23;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("frozen_item_reward");
			while (!item_element.empty())
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -24;
				}

				if (!m_other_cfg.frozen_reward_item[i].ReadConfig(item_element))
				{
					if (m_other_cfg.frozen_reward_item[i].item_id == 0) break;

					return -25;
				}

				i++;
				item_element = item_element.next_sibling();
			}
		}
	}

	return 0;
}

int MillionaireConfig::InitExtraReward(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");

	int last_extra_index = 0; 
	while (!dataElement.empty())
	{
		int extra_index = 0;
		if (!PugiGetSubNodeValue(dataElement, "extra_index", extra_index) || extra_index <= 0 || extra_index <= last_extra_index || extra_index > m_other_cfg.role_can_gather_max_times)
		{
			return -1;
		}

		ExtraRewardCfg extra_reward_cfg;
		extra_reward_cfg.extra_reward_index = extra_index;
		last_extra_index = extra_index;


		PugiXmlNode Itemelement = dataElement.child("item1");
		ItemID itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
		{
			if (!extra_reward_cfg.reward_item1.ReadConfig(Itemelement))
			{
				return -2;
			}
		}

		Itemelement = dataElement.child("item2");
		itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement,"item_id",itemid) && NULL != ITEMPOOL->GetItem(itemid))
		{
			if (!extra_reward_cfg.reward_item2.ReadConfig(Itemelement))
			{
				return -3;
			}
		}

		Itemelement = dataElement.child("item3");
		itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement,"item_id",itemid) && NULL != ITEMPOOL->GetItem(itemid))
		{
			if (!extra_reward_cfg.reward_item3.ReadConfig(Itemelement))
			{
				return -4;
			}
		}
		
		m_extra_reward[extra_index] = extra_reward_cfg;

		dataElement = dataElement.next_sibling();
	}
	return 0;
}

int MillionaireConfig::InitRewardPool(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");

	int last_reward_index = 0; 
	int last_gather_id = 0;

	while (!dataElement.empty())
	{
		int gather_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "gather_id", gather_id) || gather_id <= 0)
		{
			return -1;
		}

		if (last_gather_id != gather_id)
		{
			last_reward_index = 0;
		}
		last_gather_id = gather_id;

		int reward_index = 0;
		if (!PugiGetSubNodeValue(dataElement, "index", reward_index) || reward_index <= 0 || reward_index <= last_reward_index)
		{
			return -1;
		}

		RewardPoolCfg reward_pool_cfg;
		reward_pool_cfg.reward_index = reward_index;
		last_reward_index = reward_index;

		if (!PugiGetSubNodeValue(dataElement, "weight", reward_pool_cfg.weight) || reward_pool_cfg.weight < 0)
		{
			return -2;
		}

		PugiXmlNode Itemelement = dataElement.child("item");
		ItemID itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
		{
			if (!reward_pool_cfg.reward_item.ReadConfig(Itemelement))
			{
				return -4;
			}
		}

		std::map<int,RewardPoolCfg> &reward_pool = m_reward_pool_map[gather_id];
		
		reward_pool[reward_index] = reward_pool_cfg;

		dataElement = dataElement.next_sibling();
	}
	return 0;
}

int MillionaireConfig::InitGatherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return - 10000;
	}

	m_total_box_count = 0;

	while (!dataElement.empty())
	{
		GatherCfg gather_cfg;
		if (!PugiGetSubNodeValue(dataElement, "gather_id", gather_cfg.gather_id) || gather_cfg.gather_id <= 0)
		{
			return -1;
		}

		if (m_gather_map.find(gather_cfg.gather_id) != m_gather_map.end())
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_time", gather_cfg.gather_time) || gather_cfg.gather_time <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_exist_time", gather_cfg.gather_exist_time) || gather_cfg.gather_exist_time <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_num", gather_cfg.gather_total_count) || gather_cfg.gather_total_count < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_notice", gather_cfg.is_notice) || gather_cfg.is_notice < 0)
		{
			return -6;
		}

		m_gather_map[gather_cfg.gather_id] = gather_cfg;
		m_total_box_count += gather_cfg.gather_total_count;

		dataElement = dataElement.next_sibling();
	}
	return 0;
}

int MillionaireConfig::InitExpGatherPosCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		PosCfg positem;

		if (!PugiGetSubNodeValue(dataElement, "scene_id", positem.scene_id))
		{
			return -1;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(positem.scene_id);

		if (!PugiGetSubNodeValue(dataElement, "pos_x", positem.born_pos.x))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", positem.born_pos.y))
		{
			return -3;
		}

		m_exp_gather_pos.push_back(positem);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

void MillionaireConfig::RefreshAll()
{
	int pos_list_size = static_cast<int>(m_pos_vec.size());
	if (pos_list_size < m_total_box_count) return;

	int start_index = 0;
	if (m_total_box_count < pos_list_size)
	{
		start_index = RandomNum(pos_list_size - m_total_box_count + 1);
	}

	std::map<int, GatherCfg>::iterator iter = m_gather_map.begin();
	for (; iter != m_gather_map.end(); iter++)
	{
		GatherCfg & gather_cfg = iter->second;
		for(int i = gather_cfg.gather_total_count; i > 0 && start_index < pos_list_size; i--)
		{
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(m_pos_vec[start_index].scene_id, 1);
			if (NULL == scene) continue;

			if (!scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, m_pos_vec[start_index].born_pos.x, m_pos_vec[start_index].born_pos.y))
			{
				continue;
			}
		
			GatherObj *gather_obj = new GatherObj();
			gather_obj->Init(NULL, gather_cfg.gather_id, gather_cfg.gather_time * 1000, 0, true, gather_cfg.gather_exist_time);
			gather_obj->SetPos(m_pos_vec[start_index].born_pos);
			scene->AddObj(gather_obj);
			start_index++;
		}
	}
}

void MillionaireConfig::RefreshExpGather()
{
	for(std::vector<PosCfg>::iterator iter = m_exp_gather_pos.begin(); iter != m_exp_gather_pos.end(); iter++)
	{
		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(iter->scene_id, 1);
		if (NULL == scene)
		{
			continue;
		}

		if (!scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, iter->born_pos.x, iter->born_pos.y))
		{
			continue;
		}
		
		GatherObj *gather_obj = new GatherObj();
		gather_obj->Init(NULL, m_other_cfg.exp_gather_id, m_other_cfg.exp_gather_gather_time * 1000, 0, true, m_other_cfg.exp_gather_exit_time);
		gather_obj->SetPos(iter->born_pos);
		scene->AddObj(gather_obj);
	}
}

const ExtraRewardCfg* MillionaireConfig::GetExtraReward(int extra_index)
{
	if (extra_index <= 0)
	{
		return NULL;
	}

	std::map<int,ExtraRewardCfg>::iterator it = m_extra_reward.find(extra_index);
	if (it == m_extra_reward.end())
	{
		return NULL;
	}

	return &it->second;
}

const RewardPoolCfg* MillionaireConfig::GetRewardPool(int gather_id, int index)
{
	if (gather_id <= 0)
	{
		return NULL;
	}
	
	std::map<int ,std::map<int ,RewardPoolCfg> >::iterator iter = m_reward_pool_map.find(gather_id);
	if (iter == m_reward_pool_map.end())
	{
		return NULL;
	}

	if (index <= 0)
	{
		return NULL;
	}

	std::map<int ,RewardPoolCfg> &it = iter->second;
	if (it.find(index) == it.end())
	{
		return NULL;
	}

	std::map<int ,RewardPoolCfg>::iterator itt = it.find(index);

	return &itt->second;
}

const RewardPoolCfg* MillionaireConfig::GetRewardPoolReward(int gather_id)
{
	if (m_reward_pool_map.find(gather_id) == m_reward_pool_map.end())
	{
		return NULL;
	}
	
	std::map<int,RewardPoolCfg> &reward_cfg = m_reward_pool_map[gather_id];

	int total_rate = 0;

	std::map<int,RewardPoolCfg>::iterator iter = reward_cfg.begin();
	for(;iter != reward_cfg.end();iter++)
	{
		total_rate += iter->second.weight;
	}
	
	if (total_rate <= 0)
	{
		return NULL;
	}

	int cur_rate = RandomNum(total_rate);
	
	for(iter = reward_cfg.begin();iter != reward_cfg.end();iter++)
	{
		if (cur_rate < iter->second.weight)
		{
			return &iter->second;
		}

		cur_rate -= iter->second.weight;
	}

	return NULL;

}

const GatherCfg * MillionaireConfig::GetGatherCfg(int gather_id)
{
	if (gather_id <= 0) return NULL;

	std::map<int, GatherCfg>::iterator it = m_gather_map.find(gather_id);
	if (it != m_gather_map.end())
	{
		return &it->second;
	}

	return NULL;
}


