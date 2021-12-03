#include "tuhaojinconfig.hpp"
#include "obj/character/role.h"
#include "item/itemextern.hpp"
#include "world.h"
#include <servercommon/configcommon.h>

TuHaoJinConfig::TuHaoJinConfig() : max_tuhaojin_level(0)
{

}

TuHaoJinConfig::~TuHaoJinConfig()
{

}

bool TuHaoJinConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "TuHaoJinConfig", *err);
	
	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode root_element = RootElement.child("level_cfg");
		if (root_element.empty())
		{
			*err = configname + ": no [level_cfg].";
			return false;
		}

		iRet = this->InitLevelConfig(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitLevelConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

const TuhaojinLevelConfig * TuHaoJinConfig::GetTuhaojinLevelCfg(short level)
{
	if (level < 0 || level > max_tuhaojin_level || level > TUHAOJIN_MAX_LEVEL)
	{
		return NULL;
	}

	if (m_tuhaojin_level_cfg[level].tuhaojin_level != level)
	{
		return NULL;
	}

	return  &m_tuhaojin_level_cfg[level];
}

int TuHaoJinConfig::InitLevelConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_tuhaojin_color = 0;
	while (!data_element.empty())
	{
		if (max_tuhaojin_level >= TUHAOJIN_MAX_LEVEL)
		{
			return -1000;
		}

		short tuhaojin_level = 0;
		if (!PugiGetSubNodeValue(data_element, "tuhaojin_level", tuhaojin_level) || tuhaojin_level < 0 || tuhaojin_level > TUHAOJIN_MAX_LEVEL)
		{
			return -1;
		}

		max_tuhaojin_level = tuhaojin_level;
		TuhaojinLevelConfig &cfg = m_tuhaojin_level_cfg[tuhaojin_level];
		cfg.tuhaojin_level = tuhaojin_level;

		if (!PugiGetSubNodeValue(data_element, "tuhaojin_color_id", cfg.tuhaojin_color_id) || cfg.tuhaojin_color_id < 0 || cfg.tuhaojin_color_id < last_tuhaojin_color)
		{
			return -2;
		}
		last_tuhaojin_color = cfg.tuhaojin_color_id;

		if (!PugiGetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", cfg.shanbi) || cfg.shanbi < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", cfg.baoji) || cfg.baoji < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", cfg.baoji) || cfg.jianren < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "is_notice", cfg.is_notice) || cfg.is_notice < 0)
		{
			return -10;
		}

		PugiXmlNode Itemelement = data_element.child("common_item");
		if (!cfg.common_item.ReadConfig(Itemelement) && cfg.common_item.item_id != 0)
		{
			return -100;
		}
		
		Itemelement = data_element.child("prof_one_item");
		if (!cfg.prof_one_item.ReadConfig(Itemelement) && cfg.prof_one_item.item_id != 0)
		{
			return -101;
		}
		
		Itemelement = data_element.child("prof_two_item");
		if (!cfg.prof_two_item.ReadConfig(Itemelement) && cfg.prof_two_item.item_id != 0)
		{
			return -102;
		}

		Itemelement = data_element.child("prof_three_item");
		if (!cfg.prof_three_item.ReadConfig(Itemelement) && cfg.prof_three_item.item_id != 0)
		{
			return -103;
		}

		Itemelement = data_element.child("prof_four_item");
		if (!cfg.prof_four_item.ReadConfig(Itemelement) && cfg.prof_four_item.item_id != 0)
		{
			return -103;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}


