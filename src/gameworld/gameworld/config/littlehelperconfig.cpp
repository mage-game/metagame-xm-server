#include "littlehelperconfig.h"

#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "servercommon/configbase.h"
#include "item/itempool.h"
#include "config/logicconfigmanager.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "servercommon/serverconfig/openserverconfig.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "monster/drop/droppool.hpp"


LittleHelperConfig::LittleHelperConfig()
{
}

LittleHelperConfig::~LittleHelperConfig()
{
}

bool LittleHelperConfig::Init(const std::string & configname, std::string * err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	// ÷˙ ÷
	PUGI_XML_LOAD_CONFIG("helper", InitHelperCfg);
	return true;
}

const LittleHelperCfg * LittleHelperConfig::GetHelperCfg(lhns::complete_type_t type, lhns::seq_t seq)
{
	if (type < 0 || type >= Protocol::LITTLE_HELPER_COMPLETE_TYPE_MAX || m_little_helper_cfg_vec_in_map.find(type) == m_little_helper_cfg_vec_in_map.end())
	{
		return nullptr;
	}
	lhns::seq_t max_seq_count = static_cast<lhns::seq_t>(m_little_helper_cfg_vec_in_map[type].size());
	if (seq < 0 || seq >= max_seq_count)
	{
		return nullptr;
	}
	return &m_little_helper_cfg_vec_in_map[type][seq];
}

const LittleHelperCfg * LittleHelperConfig::GetHelperCfgByLevel(lhns::complete_type_t type, lhns::level_t level)
{
	if (type < 0 || type >= Protocol::LITTLE_HELPER_COMPLETE_TYPE_MAX || m_little_helper_cfg_vec_in_map.find(type) == m_little_helper_cfg_vec_in_map.end())
	{
		return nullptr;
	}
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return nullptr;
	}
	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	size_t size = this->GetLittleHelperCfgVecSize(type);
	for (size_t i = 0; i < size; ++i)
	{
		const LittleHelperCfg *cfg = this->GetHelperCfg(type, (lhns::seq_t)i);
		if (nullptr == cfg)
		{
			continue;
		}
		if (opengame_day < cfg->opengame_day)
		{
			continue;
		}
		if (level < cfg->level_left || level > cfg->level_right)
		{
			continue;
		}
		return cfg;
	}
	return nullptr;
}

size_t LittleHelperConfig::GetLittleHelperCfgVecSize(lhns::complete_type_t type)
{
	if (type < 0 || type >= Protocol::LITTLE_HELPER_COMPLETE_TYPE_MAX || m_little_helper_cfg_vec_in_map.find(type) == m_little_helper_cfg_vec_in_map.end())
	{
		return 0;
	}

	return m_little_helper_cfg_vec_in_map[type].size();
}

int LittleHelperConfig::InitHelperCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");

	lhns::complete_type_t type = 0;
	lhns::seq_t last_seq = -1;
	lhns::seq_t seq = 0;
	int ret = 0;
	while (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "complete_type", type) || type < 0 || type >= Protocol::LITTLE_HELPER_COMPLETE_TYPE_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "seq", seq))
		{
			return -2;
		}
		LittleHelperCfg cfg;
		if (!PugiGetSubNodeValue(dataElement, "opengame_day", cfg.opengame_day) || cfg.opengame_day < 0)	return -3;
		if (!PugiGetSubNodeValue(dataElement, "level_left", cfg.level_left) || cfg.level_left < 0)	return -4;
		if (!PugiGetSubNodeValue(dataElement, "level_right", cfg.level_right) || cfg.level_right < 0)	return -5;
		if (!PugiGetSubNodeValue(dataElement, "task_id", cfg.task_id) || cfg.task_id < 0)	return -51;
		if (!PugiGetSubNodeValue(dataElement, "money_type", cfg.money_type) || cfg.money_type < 0 || cfg.money_type >= lhns::CONSUME_TYPE_MAX)	return -6;
		if (!PugiGetSubNodeValue(dataElement, "gold", cfg.gold) || cfg.gold < 0)	return -7;
		if (!PugiGetSubNodeValue(dataElement, "exp", cfg.exp) || cfg.exp < 0)	return -8;
		if (!PugiGetSubNodeValue(dataElement, "prestige", cfg.prestige) || cfg.prestige < 0)	return -9;
		if (!PugiGetSubNodeValue(dataElement, "cross_honor", cfg.cross_honor) || cfg.cross_honor < 0)	return -10;
		if (!PugiGetSubNodeValue(dataElement, "monster_id", cfg.monster_id) || cfg.monster_id < 0)	return -11;
		ret = ItemConfigData::ReadConfigList(dataElement, "reward", cfg.reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
		if (ret < 0)
		{
			return -20 + ret;
		}
		
		PugiXmlNode dropidVecElement = dataElement.child("drop_id_vec");
		if (dropidVecElement.empty())
		{
			return -28;
		}
		PugiXmlNode dropidElement = dropidVecElement.child("dropid");
		if (dropidElement.empty())
		{
			return -29;
		}
		while (!dropidElement.empty())
		{
			UInt16 dropid = 0;
			if (!PugiGetNodeValue(dropidElement, dropid))
			{
				return -30;
			}
			if (0 != dropid)
			{
				if (NULL == DROPPOOL->GetDropConfig(dropid))
				{
					return -31;
				}
				cfg.drop_id_vec.push_back(dropid);
				if ((int)cfg.drop_id_vec.size() >= lhns::DROP_LIST_MAX_COUNT)
				{
					return -32;
				}
			}
			dropidElement = dropidElement.next_sibling();
		}
		
		if (m_little_helper_cfg_vec_in_map.find(type) == m_little_helper_cfg_vec_in_map.end())
		{
			if (seq != 0)
			{
				return -100;
			}
			LittleHelperCfgVec vec;
			vec.push_back(cfg);
			m_little_helper_cfg_vec_in_map[type] = vec;
		}
		else
		{
			if (last_seq + 1 != seq)
			{
				return -200;
			}
			m_little_helper_cfg_vec_in_map[type].push_back(cfg);
		}
		last_seq = seq;
		
		dataElement = dataElement.next_sibling();
	}

	return 0;
}
