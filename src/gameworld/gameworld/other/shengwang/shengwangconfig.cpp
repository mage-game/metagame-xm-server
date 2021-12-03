#include "item/itempool.h"
#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "obj/character/attribute.hpp"
#include "servercommon/servercommon.h"
#include "shengwangconfig.hpp"
#include "gamedef.h"
#include "skill/skillpool.hpp"

ShengWangConfig::ShengWangConfig() : m_max_xiandan_level(0)
{

}

ShengWangConfig::~ShengWangConfig()
{

}

bool ShengWangConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "ShengWangConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode root_element = RootElement.child("xianjie");
		if (root_element.empty())
		{
			*err = configname + ": no [xianjie].";
			return false;
		}

		iRet = this->InitXianJieLevelConfig(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitXianJieLevelConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("xiandan");
		if (root_element.empty())
		{
			*err = configname + ": no [xiandan].";
			return false;
		}

		iRet = this->InitXianDanLevelConfig(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitXianDanLevelConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("xiandan_gold");
		if (root_element.empty())
		{
			*err = configname + ": no [xiandan_gold].";
			return false;
		}

		iRet = this->InitXianDanGoldConfig(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitXianDanGoldConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int ShengWangConfig::InitXianJieLevelConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	short last_level = 0;
	short last_shengwang = 0;

	while(!dataElement.empty())
	{
		short level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level - last_level != 1)
		{
			return -1;
		}

		XianJieLevelCfg &xianjie_level_cfg = m_xianjie_map[level];
		xianjie_level_cfg.level = level;
		last_level = level;

		if (!PugiGetSubNodeValue(dataElement, "shengwang", xianjie_level_cfg.shengwang) || xianjie_level_cfg.shengwang <= 0 || xianjie_level_cfg.shengwang <= last_shengwang)
		{
			return -2;
		}
		last_shengwang = xianjie_level_cfg.shengwang;
		
		if (!PugiGetSubNodeValue(dataElement, "gongji", xianjie_level_cfg.gongji) || xianjie_level_cfg.gongji < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", xianjie_level_cfg.fangyu) || xianjie_level_cfg.fangyu < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", xianjie_level_cfg.maxhp) || xianjie_level_cfg.maxhp < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", xianjie_level_cfg.mingzhong) || xianjie_level_cfg.mingzhong < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_broadcast", xianjie_level_cfg.is_broadcast) || xianjie_level_cfg.is_broadcast < 0)
		{
			return -7;
		}
		

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ShengWangConfig::InitXianDanLevelConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	short last_level = 0;
	short last_shengwang = 0;

	while(!dataElement.empty())
	{
		short level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level <= 0 || level - last_level != 1)
		{
			return -1;
		}

		XianDanLevelCfg &xiandan_level_cfg = m_xiandan_map[level];
		xiandan_level_cfg.level = level;
		last_level = level;
		m_max_xiandan_level = level;

		if (!PugiGetSubNodeValue(dataElement, "xianjie_limit", xiandan_level_cfg.xianjie_limit) || xiandan_level_cfg.xianjie_limit <= 0 || NULL == this->GetXianJieLevelCfg(xiandan_level_cfg.xianjie_limit))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "once_cost_shengwang", xiandan_level_cfg.once_cost_shengwang) || xiandan_level_cfg.once_cost_shengwang <= 0 || xiandan_level_cfg.once_cost_shengwang < last_shengwang)
		{
			return -3;
		}
		last_shengwang = xiandan_level_cfg.once_cost_shengwang;

		if (!PugiGetSubNodeValue(dataElement, "max_times", xiandan_level_cfg.max_times) || xiandan_level_cfg.max_times <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "once_add_gongji", xiandan_level_cfg.once_add_gongji) || xiandan_level_cfg.once_add_gongji < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "once_add_fangyu", xiandan_level_cfg.once_add_fangyu) || xiandan_level_cfg.once_add_fangyu < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "once_add_maxhp", xiandan_level_cfg.once_add_maxhp) || xiandan_level_cfg.once_add_maxhp < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "once_add_mingzhong", xiandan_level_cfg.once_add_mingzhong) || xiandan_level_cfg.once_add_mingzhong < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_baoji_on_act", xiandan_level_cfg.add_baoji_on_act) || xiandan_level_cfg.add_baoji_on_act < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_kangbao_on_act", xiandan_level_cfg.add_kangbao_on_act) || xiandan_level_cfg.add_kangbao_on_act < 0)
		{
			return -10;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ShengWangConfig::InitXianDanGoldConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_max_times = 0;
	int last_gold = 0;

	while(!dataElement.empty())
	{
		int times_min = 0;
		int times_max = 0;

		if (!PugiGetSubNodeValue(dataElement, "times_min", times_min) ||  (last_max_times != 0 && times_min - last_max_times != 1))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "times_max", times_max) ||  (times_max < times_min))
		{
			return -1;
		}
		last_max_times = times_max;

		XianDanGoldConfig &gold_cfg = m_xiandan_gold_map[times_min];
		gold_cfg.times_min = times_min;
		gold_cfg.times_max = times_max;

		if (!PugiGetSubNodeValue(dataElement, "gold", gold_cfg.cost_gold) || gold_cfg.cost_gold < last_gold)
		{
			return -1;
		}

		last_gold = gold_cfg.cost_gold;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const XianJieLevelCfg *ShengWangConfig::GetXianJieLevelCfg(short level)
{
	std::map<short, XianJieLevelCfg>::const_iterator iter = m_xianjie_map.find(level);
	if (m_xianjie_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

const XianDanLevelCfg *ShengWangConfig::GetXianDanLevelCfg(short level)
{
	std::map<short, XianDanLevelCfg>::const_iterator iter = m_xiandan_map.find(level);
	if (m_xiandan_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

void ShengWangConfig::GetXianDanUpLevelCostGold(int times, int *cost_gold)
{
	if (times < 0 || NULL == cost_gold)
	{
		return;
	}

	std::map<int, XianDanGoldConfig>::const_iterator iter;
	for (iter = m_xiandan_gold_map.begin(); iter != m_xiandan_gold_map.end(); ++ iter)
	{
		if (times >= iter->second.times_min && times <= iter->second.times_max)
		{
			*cost_gold = iter->second.cost_gold;
			break;
		}
	}
}
