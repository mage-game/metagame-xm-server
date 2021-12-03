#include "guildbonfireconfig.hpp"
#include "gamecommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "world.h"
#include "scene/scene.h"
#include "scene/map.h"
#include <string>

#include "item/itempool.h"

GuildBonfireConfig::GuildBonfireConfig()
	: m_guildexp_cost(0), m_timereward_cd_s(0), m_timereward_range(0), m_timereward_gongxian(0),
	m_gathar_id(0), m_gathar_time_s(0), m_gathar_exist_time_s(0),
	m_gathar_max(0), m_gathar_self_max(0), m_mucai_itemid(0), mucai_add_banggong(0), mucai_add_count_limit(0)
{
	memset(m_openfire_scene_list, 0, sizeof(m_openfire_scene_list));
}

GuildBonfireConfig::~GuildBonfireConfig()
{

}

bool GuildBonfireConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "GuildBonfireConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode other_element = RootElement.child("other_cfg");
		if (other_element.empty())
		{
			*err = configname + ": no [other_cfg].";
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
		PugiXmlNode atl_element = RootElement.child("timereward_cfg_list");
		if (atl_element.empty())
		{
			*err = configname + ": no [activity_time_list].";
			return false;
		}

		iRet = this->InitTimeRewardCfg(atl_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTimeRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}
	
	{
		PugiXmlNode ll_element = RootElement.child("gathar_cfg_list");
		if (ll_element.empty())
		{
			*err = configname + ": no [gathar_cfg_list].";
			return false;
		}

		iRet = this->InitGatharRewardCfg(ll_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitGatharRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode ll_element = RootElement.child("open_fire_scene");
		if (ll_element.empty())
		{
			*err = configname + ": no [open_fire_scene].";
			return false;
		}

		iRet = this->InitOpenFireSceneCfg(ll_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOpenFireSceneCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int GuildBonfireConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "guildexp_cost", m_guildexp_cost) || m_guildexp_cost < 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(dataElement, "timereward_cd_s", m_timereward_cd_s) || m_timereward_cd_s <= 0)
	{
		return -11;
	}
	if (!PugiGetSubNodeValue(dataElement, "timereward_range", m_timereward_range) || m_timereward_range <= 0)
	{
		return -12;
	}
	if (!PugiGetSubNodeValue(dataElement, "timereward_gongxian", m_timereward_gongxian) || m_timereward_gongxian <= 0)
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(dataElement, "gathar_id", m_gathar_id) || m_gathar_id <= 0)
	{
		return -145;
	}
	if (!PugiGetSubNodeValue(dataElement, "gathar_time_s", m_gathar_time_s) || m_gathar_time_s <= 0)
	{
		return -15;
	}
	if (!PugiGetSubNodeValue(dataElement, "gathar_exist_time_s", m_gathar_exist_time_s) || m_gathar_exist_time_s <= 0)
	{
		return -16;
	}

	if (!PugiGetSubNodeValue(dataElement, "gathar_max", m_gathar_max) || m_gathar_max <= 0)
	{
		return -17;
	}
	if (!PugiGetSubNodeValue(dataElement, "gathar_self_max", m_gathar_self_max) || m_gathar_self_max <= 0)
	{
		return -18;
	}

	if (!PugiGetSubNodeValue(dataElement, "mucai_itemid", m_mucai_itemid) || m_mucai_itemid <= 0)
	{
		return -19;
	}
	if (NULL == ITEMPOOL->GetItem(m_mucai_itemid))
	{
		return -199;
	}

	{
		PugiXmlNode element = dataElement.child("mucai_add_reward_list");
		if (element.empty())
		{
			return -100;
		}

		int i = 0;
		PugiXmlNode item_element = element.child("mucai_add_reward");
		while (!item_element.empty())
		{
			if (i >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -101 - i;
			}

			if (!m_mucai_add_reward[i].ReadConfig(item_element))
			{
				return - 151 - i;
			}

			i++;
			item_element = item_element.next_sibling();
		}
	}

	if (!PugiGetSubNodeValue(dataElement, "mucai_add_banggong", mucai_add_banggong) || mucai_add_banggong < 0)
	{
		return -20;
	}

	if (!PugiGetSubNodeValue(dataElement, "mucai_add_count_limit", mucai_add_count_limit) || mucai_add_count_limit < 0)
	{
		return -21;
	}

	return 0;
}

int GuildBonfireConfig::InitTimeRewardCfg(PugiXmlNode RootElement)
{
	int count = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		if (count >= GUILD_BONFIRE_LV_CFG_MAX)
		{
			return -1;
		}

		GuildBonfireTimesCfg *cfg = &m_timereward_cfg_list[count];

		if (!PugiGetSubNodeValue(dataElement, "min_lv", cfg->min_lv) || cfg->min_lv <= 0)
		{
			return -2;
		}
		if (!PugiGetSubNodeValue(dataElement, "max_lv", cfg->max_lv) || cfg->max_lv <= 0)
		{
			return -3;
		}
		if (!PugiGetSubNodeValue(dataElement, "base_exp", cfg->base_exp) || cfg->base_exp < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "base_bangong", cfg->base_bangong) || cfg->base_bangong < 0)
		{
			return -5;
		}

		++ count;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildBonfireConfig::InitGatharRewardCfg(PugiXmlNode RootElement)
{
	int count = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		if (count >= GUILD_BONFIRE_LV_CFG_MAX)
		{
			return -1;
		}

		GuildBonfireLvCfg *cfg = &m_gatharreward_cfg_list[count];

		if (!PugiGetSubNodeValue(dataElement, "min_lv", cfg->min_lv) || cfg->min_lv <= 0)
		{
			return -2;
		}
		if (!PugiGetSubNodeValue(dataElement, "max_lv", cfg->max_lv) || cfg->max_lv <= 0)
		{
			return -3;
		}
		if (!PugiGetSubNodeValue(dataElement, "base_exp", cfg->base_exp) || cfg->base_exp < 0)
		{
			return -4;
		}
		
		{
			PugiXmlNode element = dataElement.child("gather_reward_list");
			if (element.empty())
			{
				return -100;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("gather_reward");
			while (!item_element.empty())
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -101 - i;
				}

				if (!cfg->gather_reward[i].ReadConfig(item_element))
				{
					return - 151 - i;
				}

				i++;
				item_element = item_element.next_sibling();
			}
		}

		++ count;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildBonfireConfig::InitOpenFireSceneCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1;
	}

	int count = 0;
	while (!dataElement.empty())
	{
		if (count >= GUILD_BONFIRE_SCENE_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "guild_bonfire_limit", m_openfire_scene_list[count]) || m_openfire_scene_list[count] < 0)
		{
			return -3;
		}

		++ count;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const GuildBonfireTimesCfg * GuildBonfireConfig::GetTimeRewardByLv(int role_lv)
{
	for (int i = 0; i < GUILD_BONFIRE_LV_CFG_MAX; i++)
	{
		if (role_lv >= m_timereward_cfg_list[i].min_lv && role_lv <= m_timereward_cfg_list[i].max_lv)
		{
			return &m_timereward_cfg_list[i];
		}
	}
	
	return NULL;
}

const GuildBonfireLvCfg *  GuildBonfireConfig::GetGatharRewardByLv(int role_lv)
{
	for (int i = 0; i < GUILD_BONFIRE_LV_CFG_MAX; i++)
	{
		if (role_lv >= m_gatharreward_cfg_list[i].min_lv && role_lv <= m_gatharreward_cfg_list[i].max_lv)
		{
			return &m_gatharreward_cfg_list[i];
		}
	}

	return NULL;
}

const void GuildBonfireConfig::GetAddMucaiReward(ItemConfigData *mucai_add_reward)
{
	if (NULL == mucai_add_reward) return;

	for(int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; i++)
	{
		mucai_add_reward[i].item_id = m_mucai_add_reward[i].item_id;
		mucai_add_reward[i].is_bind = m_mucai_add_reward[i].is_bind;
		mucai_add_reward[i].num = m_mucai_add_reward[i].num;
	}
}

bool GuildBonfireConfig::IsOpenFireScene(int scene_id)
{
	for (int i = 0; i < GUILD_BONFIRE_SCENE_MAX; i++)
	{
		if (m_openfire_scene_list[i] == scene_id)
		{
			return true;
		}
	}

	return false;
}








