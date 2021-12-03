#include "teamequipfbconfig.hpp"
#include "servercommon/configcommon.h"
#include "monster/drop/droppool.hpp"

#include "checkresourcecenter.hpp"

TeamEquipFBConfig::TeamEquipFBConfig()
{
	memset(m_boss_hp_add_per, 0, sizeof(m_boss_hp_add_per));
}

TeamEquipFBConfig::~TeamEquipFBConfig()
{
}

bool TeamEquipFBConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "TeamEquipFBConfig", *err);

	int iRet = 0;

	PugiXmlNode root_element = document.document_element();
	if (root_element.empty())
	{
		*err = path + ": xml node error in root";
		return false;
	}

	{
		PugiXmlNode element = root_element.child("other");
		if (element.empty())
		{
			*err = path + ": xml no other node";
			return false;
		}

		iRet = this->InitOtherCfg(element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode element = root_element.child("fb_config");
		if (element.empty())
		{
			*err = path + ": xml no fb_config node";
			return false;
		}

		iRet = this->InitFbCfg(element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitFbCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode element = root_element.child("buy_times_cost");
		if (element.empty())
		{
			*err = path + ": xml no buy_times_cost node";
			return false;
		}

		iRet = this->InitBuyTimesCostCfg(element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBuyTimesCostCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode element = root_element.child("boss_hp_add");
		if (element.empty())
		{
			*err = path + ": xml no boss_hp_add node";
			return false;
		}

		iRet = this->InitBossHpAddPer(element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBossHpAddPer failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int TeamEquipFBConfig::InitOtherCfg(PugiXmlNode root_element)
{
	PugiXmlNode data_element = root_element.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "max_count", m_other_cfg.max_count) || m_other_cfg.max_count <= 0)
	{
		return -1;
	}

	return 0;
}

int TeamEquipFBConfig::InitFbCfg(PugiXmlNode root_element)
{
	PugiXmlNode data_element = root_element.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int layer = 0;
	m_fb_layer_count = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "layer", layer) || layer != m_fb_layer_count || layer >= MAX_TEAM_EQUIP_FB_LAYER)
		{
			return -1;
		}

		TeamEquipFBLayerCfg &cfg = m_fb_layer_list[layer];
		cfg.layer = layer;

		if (!PugiGetSubNodeValue(data_element, "level_limit", cfg.level_limit) || cfg.level_limit <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "scene_id", cfg.scene_id) || cfg.scene_id <= 0)
		{
			return -3;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.scene_id);

		if (!PugiGetSubNodeValue(data_element, "enter_x", cfg.enter_pos.x) || cfg.enter_pos.x <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "enter_y", cfg.enter_pos.y) || cfg.enter_pos.y <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "day_exp", cfg.day_exp) || cfg.day_exp < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "day_shengwang", cfg.day_shengwang) || cfg.day_shengwang < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "mojing", cfg.mojing) || cfg.mojing < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "assist_mojing", cfg.assist_mojing) || cfg.assist_mojing < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_id", cfg.boss_id) || cfg.boss_id < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "born_x", cfg.born_pos.x) || cfg.born_pos.x <= 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "born_y", cfg.born_pos.y) || cfg.born_pos.y <= 0)
		{
			return -12;
		}

		{
			int item_count = ItemConfigData::ReadConfigListNoCheck(data_element, std::string("first_item"), cfg.first_item_list, MAX_ATTACHMENT_ITEM_NUM);
			if (item_count < 0)
			{
				return -100 + item_count;
			}

			cfg.first_item_count = item_count;
		}

		{
			int item_count = ItemConfigData::ReadConfigListNoCheck(data_element, std::string("pass_item"), cfg.pass_item_list, MAX_ATTACHMENT_ITEM_NUM);
			if (item_count < 0)
			{
				return -200 + item_count;
			}

			cfg.pass_item_count = item_count;
		}

		{
			std::string name_list[TEAM_EQUIP_FB_ROLE_MAX] = {"drop_id_list", "drop_id_list2", "drop_id_list3", "drop_id_list4"};
			for (int i = 0; i < TEAM_EQUIP_FB_ROLE_MAX; i++)
			{
				cfg.drop_id_vec[i].clear();

				PugiXmlNode dropid_list_element = data_element.child(name_list[i].c_str());
				if (dropid_list_element.empty())
				{
					return -300 - i;
				}

				PugiXmlNode dropid_element = dropid_list_element.child("dropid");
				while (!dropid_element.empty())
				{
					UInt16 dropid = 0;
					if (!PugiGetNodeValue(dropid_element, dropid)) return false;

					if (0 != dropid && NULL != DROPPOOL->GetDropConfig(dropid))
					{
						cfg.drop_id_vec[i].push_back(dropid);
					}

					dropid_element = dropid_element.next_sibling();
				}
			}
		}

		m_fb_layer_count++;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int TeamEquipFBConfig::InitBuyTimesCostCfg(PugiXmlNode root_element)
{
	PugiXmlNode data_element = root_element.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int max_times = -1;
	int max_cost = 0;
	m_buy_times_cost_count = 0;

	while (!data_element.empty())
	{
		if (m_buy_times_cost_count < 0 || m_buy_times_cost_count >= MAX_BUY_TIME_CFG_COUNT)
		{
			return -1;
		}

		TeamEquipBuyTimesCostCfg &cfg = m_buy_times_cost_cfg[m_buy_times_cost_count];
		if (!PugiGetSubNodeValue(data_element, "buytimes", cfg.buytimes) || cfg.buytimes < 0 || cfg.buytimes <= max_times)
		{
			return -2;
		}

		max_times = cfg.buytimes;

		if (!PugiGetSubNodeValue(data_element, "gold_cost", cfg.gold_cost) || cfg.gold_cost <= 0 || cfg.gold_cost <= max_cost)
		{
			return -3;
		}

		max_cost = cfg.gold_cost;
		
		m_buy_times_cost_count++;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int TeamEquipFBConfig::InitBossHpAddPer(PugiXmlNode root_element)
{
	PugiXmlNode data_element = root_element.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	memset(m_boss_hp_add_per, 0, sizeof(m_boss_hp_add_per));

	while (!data_element.empty())
	{
		int enter_num = 0;
		if (!PugiGetSubNodeValue(data_element, "enter_num", enter_num) || enter_num <= 0 || enter_num > MAX_TEAM_MEMBER_NUM)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "hp_add_per", m_boss_hp_add_per[enter_num]) || m_boss_hp_add_per[enter_num] < 0)
		{
			return -2;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const TeamEquipFBOtherCfg & TeamEquipFBConfig::GetOtherCfg()
{
	return m_other_cfg;
}

int TeamEquipFBConfig::GetFBLayerBySceneID(int scene_id)
{
	for (int i = 0; i < m_fb_layer_count && i < MAX_TEAM_EQUIP_FB_LAYER; i++)
	{
		if (m_fb_layer_list[i].scene_id == scene_id)
		{
			return m_fb_layer_list[i].layer;
		}
	}

	return -1;
}

const TeamEquipFBLayerCfg * TeamEquipFBConfig::GetTeamEquipFBLayerCfg(int layer)
{
	if (layer < 0 || layer >= m_fb_layer_count || layer >= MAX_TEAM_EQUIP_FB_LAYER)
	{
		return NULL;
	}

	return &m_fb_layer_list[layer];
}

const int TeamEquipFBConfig::GetTeamEquipFbBuyTimesCost(int times)
{
	for (int i = m_buy_times_cost_count - 1; i >= 0; i--)
	{
		if (m_buy_times_cost_count >= 0 && m_buy_times_cost_count <MAX_BUY_TIME_CFG_COUNT)
		{
			if (m_buy_times_cost_cfg[i].buytimes <= times)
			{
				return m_buy_times_cost_cfg[i].gold_cost;
			}
		}
	}

	return 0;
}

int TeamEquipFBConfig::GetBossHpAddPer(int enter_num)
{
	if (enter_num <= 0 || enter_num > MAX_TEAM_MEMBER_NUM) return 0;

	return m_boss_hp_add_per[enter_num];
}
