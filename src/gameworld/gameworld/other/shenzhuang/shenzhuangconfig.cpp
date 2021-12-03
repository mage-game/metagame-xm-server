#include "item/itempool.h"
#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "obj/character/attribute.hpp"
#include "servercommon/servercommon.h"
#include "shenzhuangconfig.hpp"
#include "gamedef.h"
#include "skill/skillpool.hpp"

ShenZhuangConfig::ShenZhuangConfig() : m_max_level(0), m_reward_level_max(0)
{

}

ShenZhuangConfig::~ShenZhuangConfig()
{

}

bool ShenZhuangConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	PUGI_XML_LOAD_CONFIG("other", InitOtherCfg);
	PUGI_XML_LOAD_CONFIG("uplevel", InitUplevelConfig);
	PUGI_XML_LOAD_CONFIG("down_scene", InitDownSceneConfig);
	PUGI_XML_LOAD_CONFIG("reward_percent", InitRewardPercentConfig);

	return true;
}

int ShenZhuangConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElem, "drop_level_max_times", m_other_cfg.drop_level_max_times) || m_other_cfg.drop_level_max_times < 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElem, "gain_max_times", m_other_cfg.gain_max_times) || m_other_cfg.gain_max_times < 0)
	{
		return -2;
	}

	return 0;
}

int ShenZhuangConfig::InitUplevelConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	int last_part_index = -1;
	int last_level = 0;
	
	while (!dataElem.empty())
	{
		int part_index = 0;
		if (!PugiGetSubNodeValue(dataElem, "index", part_index) || part_index >= SHENZHUANG_MAX_PART)
		{
			return -1;
		}

		if (part_index != last_part_index)
		{
			if (1 != part_index - last_part_index)
			{
				return -200;
			}

			last_part_index = part_index;
			last_level = 0;
		}

		SZPartItemCfg &part_cfg = m_part_list[part_index];
		part_cfg.part_index = part_index;

		int level = 0;
		if (!PugiGetSubNodeValue(dataElem, "level", level) || 1 != level - last_level || level > SHENZHUANG_MAX_LEVEL)
		{
			return -2;
		}
		last_level = level;

		if (level > m_max_level)
		{
			m_max_level = level;
		}
		
		SZUplevelItemCfg &item_cfg = part_cfg.uplevel_list[level];
		item_cfg.index = part_index;
		item_cfg.level = level;

		if (!PugiGetSubNodeValue(dataElem, "stuff_id", item_cfg.stuff_id) || NULL == ITEMPOOL->GetItem(item_cfg.stuff_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElem, "stuff_num", item_cfg.stuff_num) || item_cfg.stuff_num <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElem, "gongji", item_cfg.gongji) || item_cfg.gongji < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElem, "fangyu", item_cfg.fangyu) || item_cfg.fangyu < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElem, "maxhp", item_cfg.maxhp) || item_cfg.maxhp < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElem, "mingzhong", item_cfg.mingzhong) || item_cfg.mingzhong < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElem, "shanbi", item_cfg.shanbi) || item_cfg.shanbi < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElem, "baoji", item_cfg.baoji) || item_cfg.baoji < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElem, "jianren", item_cfg.jianren) || item_cfg.jianren < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElem, "red_ratio_1", item_cfg.red_ratio_1) || item_cfg.red_ratio_1 < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElem, "red_ratio_2", item_cfg.red_ratio_2) || item_cfg.red_ratio_2 < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(dataElem, "red_ratio_3", item_cfg.red_ratio_3) || item_cfg.red_ratio_3 < 0)
		{
			return -16;
		}

		if (!PugiGetSubNodeValue(dataElem, "pink_ratio", item_cfg.pink_ratio) || item_cfg.pink_ratio < 0)
		{
			return -17;
		}
		
		if (!PugiGetSubNodeValue(dataElem, "downlevel_rate", item_cfg.downlevel_rate) || item_cfg.downlevel_rate < 0)
		{
			return -20;
		}

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int ShenZhuangConfig::InitDownSceneConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while (!dataElem.empty())
	{
		int down_scene_id = 0;
		if (!PugiGetSubNodeValue(dataElem, "down_scene_id", down_scene_id) || down_scene_id <= 0)
		{
			return -1;
		}

		m_scene_id_set.insert(down_scene_id);

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

int ShenZhuangConfig::InitRewardPercentConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElem = RootElement.child("data");
	if (dataElem.empty())
	{
		return -10000;
	}

	while (!dataElem.empty())
	{
		int exceed_level = 0;
		if (!PugiGetSubNodeValue(dataElem, "exceed_level", exceed_level) || exceed_level != m_reward_level_max || exceed_level > REWARD_LEVEL_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElem, "reward_percent", m_reward_percent_list[exceed_level]))
		{
			return -2;
		}

		m_reward_level_max++;

		dataElem = dataElem.next_sibling();
	}

	return 0;
}

const SZUplevelItemCfg *ShenZhuangConfig::GetUplevelItemCfg(int part_index, int level)
{
	if (part_index < 0 || part_index >= SHENZHUANG_MAX_PART || level <= 0 || level > SHENZHUANG_MAX_LEVEL)
	{
		return NULL;
	}

	return &m_part_list[part_index].uplevel_list[level];
}

bool ShenZhuangConfig::IsDownScene(int scene_id)
{
	return m_scene_id_set.end() != m_scene_id_set.find(scene_id);
}

int ShenZhuangConfig::GetRewardPercent(int exceed_level)
{
	if (exceed_level < 0)
	{
		exceed_level = 0;
	}

	if (exceed_level >= 0 && exceed_level <= m_reward_level_max && exceed_level <= REWARD_LEVEL_MAX)
	{
		return m_reward_percent_list[exceed_level];
	}

	return 0;
}