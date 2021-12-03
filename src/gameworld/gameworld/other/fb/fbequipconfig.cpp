#include "fbequipconfig.hpp"
#include <stdlib.h>
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "task/taskpool.h"
#include "monster/monsterinitparam.h"
#include "monster/drop/droppool.hpp"
#include "checkresourcecenter.hpp"
#include <string>
#include <set>


FBEquipConfig::FBEquipConfig() : m_team_max_layer(0)
{

}

FBEquipConfig::~FBEquipConfig()
{

}

bool FBEquipConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	TiXmlDocument document;

	if (configname == "" || !document.LoadFile(configname))
	{
		sprintf(errinfo,"%s: Load FBEquipConfig Error,\n %s.", configname.c_str(), document.ErrorDesc());
		*err = errinfo;
		return false;
	}

	TiXmlElement *RootElement = document.RootElement();

	if (NULL == RootElement)
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// ÆäËûÅäÖÃ
		TiXmlElement *other_element = RootElement->FirstChildElement("other");
		if (NULL == other_element)
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
		// ×é¶Ó²ãÅäÖÃ
		TiXmlElement *team_layer_element = RootElement->FirstChildElement("team_layer_list");
		if (NULL == team_layer_element)
		{
			*err = configname + ": no [team_layer_list].";
			return false;
		}

		iRet = this->InitTeamLayerCfg(team_layer_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTeamLayerCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ×é¶ÓÉñÃØ²ãÉÌµê
		TiXmlElement *team_my_shop_element = RootElement->FirstChildElement("team_mystery_shop");
		if (NULL == team_my_shop_element)
		{
			*err = configname + ": no [team_mystery_shop].";
			return false;
		}

		iRet = this->InitTeamMysteryShopCfg(team_my_shop_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTeamMysteryShopCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

const FBEquipLayerCfg * FBEquipConfig::GetTeamLayerCfgByLayer(int layer) const
{
	if (layer <= 0 || layer > FB_EQUIP_MAX_LAYER_ID) return NULL;

	return &m_team_layer_cfg[layer];
}

const FBEquipLayerCfg * FBEquipConfig::GetTeamLayerCfgBySceneID(int scene_id) const
{
	std::map<int, FBEquipLayerCfg>::const_iterator iter = m_team_scene_to_layer_map.find(scene_id);
	if (m_team_scene_to_layer_map.end() == iter) return NULL;

	return &iter->second;
}

const FBEquipJumpLayerCfg * FBEquipConfig::GetTeamJumpCfgByLayer(int layer) const
{
	if (layer < 0 || layer > FB_EQUIP_MAX_LAYER_ID) return NULL;

	return &m_team_layer_cfg[layer].jump_layer_cfg;
}

const FBEquipJumpLayerCfg * FBEquipConfig::GetTeamJumpCfgBySceneID(int scene_id) const
{
	std::map<int, FBEquipLayerCfg>::const_iterator iter = m_team_scene_to_layer_map.find(scene_id);
	if (m_team_scene_to_layer_map.end() == iter) return NULL;

	return &iter->second.jump_layer_cfg;
}

const FBEquipMysteryShopItem * FBEquipConfig::GetTeamMysteryShopCfg(int shop_item_seq) const
{
	if (shop_item_seq < 0 || shop_item_seq >= FB_EQUIP_MAX_GOODS_SEQ) return NULL;
	if (0 == m_team_mystery_shop_list[shop_item_seq].item_cfg_data.item_id) return NULL;

	return &m_team_mystery_shop_list[shop_item_seq];
}

int FBEquipConfig::GetTeamSceneIDByLayer(int layer)
{
	if (layer < 0 || layer > FB_EQUIP_MAX_LAYER_ID) return 0;

	return m_team_layer_cfg[layer].scene_id;
}

int FBEquipConfig::GetTeamLayerBySceneID(int scene_id)
{
	std::map<int, FBEquipLayerCfg>::const_iterator iter = m_team_scene_to_layer_map.find(scene_id);
	if (m_team_scene_to_layer_map.end() == iter) return 0;

	return iter->second.layer;
}

int FBEquipConfig::InitOtherCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL != dataElement)
	{
		if (!GetSubNodeValue(dataElement, "team_double_cost_gold", m_other_cfg.team_double_cost_gold) || 
			m_other_cfg.team_double_cost_gold <= 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "trial_task_id",  m_other_cfg.trial_task_id) || NULL == TASKPOOL->GetCfgByTaskID(m_other_cfg.trial_task_id))
		{
			return -9;
		}

		if (!GetSubNodeValue(dataElement, "guild_add_gongji_percent",  m_other_cfg.guild_add_gongji_percent) || m_other_cfg.guild_add_gongji_percent <= 0)
		{
			return -10;
		}

		if (!GetSubNodeValue(dataElement, "join_times", m_other_cfg.join_times) || m_other_cfg.join_times <= 0)
		{
			return -11;
		}

		if (!GetSubNodeValue(dataElement, "kick_time_s", m_other_cfg.kick_time_s) || m_other_cfg.kick_time_s < 0)
		{
			return -12;
		}
		
		if (!GetSubNodeValue(dataElement, "buy_double_reward_gold", m_other_cfg.buy_double_reward_need_gold) || m_other_cfg.buy_double_reward_need_gold < 0)
		{
			return -13;
		}

		if (!GetSubNodeValue(dataElement, "buy_triple_reward_gold", m_other_cfg.buy_triple_reward_need_gold) || m_other_cfg.buy_triple_reward_need_gold < 0)
		{
			return -14;
		}

		if (!GetSubNodeValue(dataElement, "double_reward_need_vip_level", m_other_cfg.double_reward_need_vip_level) || m_other_cfg.double_reward_need_vip_level < 0)
		{
			return -15;
		}

		if (!GetSubNodeValue(dataElement, "triple_reward_need_vip_level", m_other_cfg.triple_reward_need_vip_level) || m_other_cfg.triple_reward_need_vip_level < 0)
		{
			return -16;
		}

		//¶îÍâ½±Àø
		{
			TiXmlElement *dropidListElement = dataElement->FirstChildElement("dropid_list");
			if (NULL == dropidListElement)
			{
				return -100;
			}
			TiXmlElement *dropidElement = dropidListElement->FirstChildElement("dropid");
			if (NULL == dropidElement) return -101;

			while (NULL != dropidElement)
			{
				UInt16 dropid = 0;
				if (!GetNodeValue(dropidElement, dropid))
				{
					return -102;
				}

				if (0 != dropid)
				{
					if (NULL == DROPPOOL->GetDropConfig(dropid))
					{
						return -103;
					}

					m_other_cfg.dropid_list.push_back(dropid);
					if ((int)m_other_cfg.dropid_list.size() >= MonsterInitParam::DROP_LIST_MAX_COUNT)
					{
						return -104;
					}
				}

				dropidElement = dropidElement->NextSiblingElement();
			}
		}
	}

	return 0;
}

int FBEquipConfig::InitTeamLayerCfg(TiXmlElement *RootElement)
{
	m_team_max_layer = 0;
	int count = 0;

	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		FBEquipLayerCfg cfg;

		if (!GetSubNodeValue(dataElement, "layer", cfg.layer) || cfg.layer <= 0 || cfg.layer > FB_EQUIP_MAX_LAYER_ID)
		{
			return -1;
		}

		if (!GetSubNodeValue(dataElement, "scene_id", cfg.scene_id))
		{
			return -2;
		}

		if (0 != m_team_layer_cfg[cfg.layer].layer)
		{
			return -3;
		}

		int is_mystery = 0;
		if (!GetSubNodeValue(dataElement, "is_mystery_layer", is_mystery) || (0 != is_mystery && 1 != is_mystery))
		{
			return -4;
		}
		cfg.is_mystery = (0 != is_mystery);

		if (!GetSubNodeValue(dataElement, "show_layer", cfg.show_layer) || cfg.show_layer < 0 || (!cfg.is_mystery && 0 == cfg.show_layer))
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "pass_exp", cfg.pass_exp))
		{
			return -6;
		}

		int is_pass_notice = 0;
		if (!GetSubNodeValue(dataElement, "pass_notice", is_pass_notice) || (0 != is_pass_notice && 1 != is_pass_notice))
		{
			return -7;
		}
		cfg.is_pass_notice = (0 != is_pass_notice);

		{	// ¶ÁÈ¡Ìø²ãÅäÖÃ
			if (!GetSubNodeValue(dataElement, "jump_target_layer", cfg.jump_layer_cfg.target_layer) || cfg.jump_layer_cfg.target_layer > FB_EQUIP_MAX_LAYER_ID || 
				(cfg.jump_layer_cfg.target_layer > 0 && cfg.jump_layer_cfg.target_layer <= cfg.layer + 1))
			{
				return -12;
			}
			if (!GetSubNodeValue(dataElement, "jump_limit_time", cfg.jump_layer_cfg.limit_time) || cfg.jump_layer_cfg.limit_time < 0)
			{
				return -13;
			}

			TiXmlElement *jumpGiftItem = dataElement->FirstChildElement("jump_gift_item");
			if (NULL == jumpGiftItem)
			{
				return -14;
			}

			if (!cfg.jump_layer_cfg.gift_item.ReadConfig(jumpGiftItem))
			{
				return -15; 
			}

			if (!GetSubNodeValue(dataElement, "enter_pos_x", cfg.jump_layer_cfg.enter_pos.x) || cfg.jump_layer_cfg.enter_pos.x < 0)
			{
				return -16;
			}
			if (!GetSubNodeValue(dataElement, "enter_pos_y", cfg.jump_layer_cfg.enter_pos.y) || cfg.jump_layer_cfg.enter_pos.y < 0)
			{
				return -17;
			}
		}

		{
			// ¶ÁÈ¡Ìø²ãµÄµôÂä°üÅäÖÃ
			TiXmlElement *dropidListElement = dataElement->FirstChildElement("dropid_list");
			if (NULL == dropidListElement)
			{
				return -100;
			}
			TiXmlElement *dropidElement = dropidListElement->FirstChildElement("dropid");
			if (NULL == dropidElement) return -101;

			while (NULL != dropidElement)
			{
				UInt16 dropid = 0;
				if (!GetNodeValue(dropidElement, dropid))
				{
					return -102;
				}

				if (0 != dropid)
				{
					if (NULL == DROPPOOL->GetDropConfig(dropid))
					{
						return -103;
					}

					cfg.dropid_list.push_back(dropid);
					if ((int)cfg.dropid_list.size() >= MonsterInitParam::DROP_LIST_MAX_COUNT)
					{
						return -104;
					}
				}

				dropidElement = dropidElement->NextSiblingElement();
			}
		}

		if (m_team_scene_to_layer_map.end() != m_team_scene_to_layer_map.find(cfg.scene_id))
		{
			return -20;
		}

		if (m_team_max_layer < cfg.layer)
		{
			m_team_max_layer = cfg.layer;
		}

		m_team_layer_cfg[cfg.layer].layer = cfg.layer;
		m_team_layer_cfg[cfg.layer].scene_id = cfg.scene_id;
		m_team_layer_cfg[cfg.layer].is_mystery = cfg.is_mystery;
		m_team_layer_cfg[cfg.layer].show_layer = cfg.show_layer;
		m_team_layer_cfg[cfg.layer].pass_exp = cfg.pass_exp;
		m_team_layer_cfg[cfg.layer].jump_layer_cfg = cfg.jump_layer_cfg;
		m_team_layer_cfg[cfg.layer].dropid_list = cfg.dropid_list;

		m_team_scene_to_layer_map[cfg.scene_id] = cfg;

		++ count;

		//CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.scene_id);

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int FBEquipConfig::InitTeamMysteryShopCfg(TiXmlElement *RootElement)
{
	int counter[FB_EQUIP_MAX_LAYER_ID + 1];
	memset(counter, 0, sizeof(counter));

	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		int shop_item_seq = 0;
		if (!GetSubNodeValue(dataElement, "shop_item_seq", shop_item_seq) || shop_item_seq < 0 || shop_item_seq >= FB_EQUIP_MAX_GOODS_SEQ)
		{
			return -1;
		}

		int layer = 0;
		if (!GetSubNodeValue(dataElement, "layer", layer) || layer <= 0 || layer > FB_EQUIP_MAX_LAYER_ID)
		{
			return -2;
		}

		TiXmlElement *item_element = dataElement->FirstChildElement("item");
		if (NULL == item_element)
		{
			return -3;
		}

		ItemConfigData item_cfg_data;
		if (!item_cfg_data.ReadConfig(item_element))
		{
			return -4;
		}

		if (false == item_cfg_data.is_bind)
		{
			return -50;
		}

		int gold_cost = 0;
		if (!GetSubNodeValue(dataElement, "gold_cost", gold_cost) || gold_cost <= 0)
		{
			return -5;
		}

		int buy_limit = 0;
		if (!GetSubNodeValue(dataElement, "buy_limit", buy_limit) || buy_limit <= 0 || buy_limit >= 120)
		{
			return -6;
		}

		if (0 != m_team_mystery_shop_list[shop_item_seq].item_cfg_data.item_id)
		{
			return -10;
		}

		m_team_mystery_shop_list[shop_item_seq].mystery_layer = layer;
		m_team_mystery_shop_list[shop_item_seq].item_cfg_data = item_cfg_data;
		m_team_mystery_shop_list[shop_item_seq].gold_price = gold_cost;
		m_team_mystery_shop_list[shop_item_seq].buy_limit = buy_limit;

		++ counter[layer];

		dataElement = dataElement->NextSiblingElement();
	}

	int mystery_layer_count = 0;

	int length = static_cast<int>(sizeof(counter) / sizeof(counter[0]));
	for (int i = 0; i < length; ++i)
	{
		if (counter[i] > 0)
		{
			++ mystery_layer_count;

			if (FB_EQUIP_MAX_GOODS_NUM_PER_MYSTERYLAYER != counter[i])
			{
				return -100;
			}
		}
	}

	if (mystery_layer_count > FB_EQUIP_MAX_MYSTERYLAYER_NUM)
	{
		return -200;
	}

	return 0;
}



