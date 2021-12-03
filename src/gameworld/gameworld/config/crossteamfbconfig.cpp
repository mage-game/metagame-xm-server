#include "crossteamfbconfig.hpp"
#include "servercommon/configcommon.h"
#include "monster/drop/droppool.hpp"

#include "checkresourcecenter.hpp"

CrossTeamFBConfig::CrossTeamFBConfig()
{
}

CrossTeamFBConfig::~CrossTeamFBConfig()
{
}

bool CrossTeamFBConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "CrossTeamFBConfig", *err);
	
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

	return true;
}

int CrossTeamFBConfig::InitOtherCfg(PugiXmlNode root_element)
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

int CrossTeamFBConfig::InitFbCfg(PugiXmlNode root_element)
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
		if (!PugiGetSubNodeValue(data_element, "layer", layer) || layer != m_fb_layer_count || layer >= MAX_CROSS_TEAM_LAYER)
		{
			return -1;
		}

		CrossTeamFBLayerCfg &cfg = m_fb_layer_list[layer];
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
			std::string name_list[CROSS_TEAM_FB_ROLE_MAX] = {"drop_id_list", "drop_id_list2", "drop_id_list3", "drop_id_list4"};
			for (int i = 0; i < CROSS_TEAM_FB_ROLE_MAX; i++)
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

const CrossTeamFBOtherCfg & CrossTeamFBConfig::GetOtherCfg()
{
	return m_other_cfg;
}

int CrossTeamFBConfig::GetFBLayerBySceneID(int scene_id)
{
	for (int i = 0; i < m_fb_layer_count && i < MAX_CROSS_TEAM_LAYER; i++)
	{
		if (m_fb_layer_list[i].scene_id == scene_id)
		{
			return m_fb_layer_list[i].layer;
		}
	}

	return -1;
}

const CrossTeamFBLayerCfg * CrossTeamFBConfig::GetCrossTeamFBLayerCfg(int layer)
{
	if (layer < 0 || layer >= m_fb_layer_count || layer >= MAX_CROSS_TEAM_LAYER)
	{
		return NULL;
	}

	return &m_fb_layer_list[layer];
}
