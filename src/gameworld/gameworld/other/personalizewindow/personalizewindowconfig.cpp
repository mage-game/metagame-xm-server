#include "personalizewindowconfig.hpp"
#include "servercommon/configcommon.h"
#include "config/logicconfigmanager.hpp"
#include "item/itempool.h"
#include <set>

PersonalizeWindowConfig::PersonalizeWindowConfig()
{
	memset(max_bubble_level, 0, sizeof(max_bubble_level));
	memset(max_avatar_level, 0, sizeof(max_avatar_level));
}

PersonalizeWindowConfig::~PersonalizeWindowConfig()
{

}


bool PersonalizeWindowConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "PersonalizeWindowConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 气泡框配置信息
		PugiXmlNode root_element = RootElement.child("bubble_level_cfg");
		if (root_element.empty())
		{
			*err = configname + ": no [bubble_level_cfg].";
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

	{
		// 头像框配置信息
		PugiXmlNode root_element = RootElement.child("avatar_rim_level");
		if (root_element.empty())
		{
			*err = configname + ": no [avatar_rim_level].";
			return false;
		}

		iRet = this->InitAvatarLevelConfig(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitAvatarLevelConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;	
}

int PersonalizeWindowConfig::InitLevelConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		short bubble_type = -1;
		if (!PugiGetSubNodeValue(data_element, "bubble_type", bubble_type) || bubble_type < 0 || bubble_type >= BUBBLE_WINDOW_MAX_TYPE)
		{
			return -1;
		}

		short bubble_level = 0;
		if (!PugiGetSubNodeValue(data_element, "bubble_level", bubble_level) || bubble_level < 0 || bubble_level > BUBBLE_WINDOW_MAX_LEVEL)
		{
			return -2;
		}

		BubbleLevelConfig &cfg = m_bubble_level_cfg[bubble_type][bubble_level];
		cfg.bubble_type = bubble_type;
		cfg.bubble_level = bubble_level;

		max_bubble_level[bubble_type] = bubble_level;

		if (!PugiGetSubNodeValue(data_element, "is_active", cfg.is_active) || (cfg.is_active != 0 && cfg.is_active != 1))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "is_notice", cfg.is_notice) || (cfg.is_notice != 0 && cfg.is_notice != 1))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", cfg.shanbi) || cfg.shanbi < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", cfg.baoji) || cfg.baoji < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", cfg.jianren) || cfg.jianren < 0)
		{
			return -11;
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

int PersonalizeWindowConfig::InitAvatarLevelConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		short avatar_type = -1;
		if (!PugiGetSubNodeValue(data_element, "avatar_type", avatar_type) || avatar_type < 0 || avatar_type >= BUBBLE_WINDOW_MAX_TYPE)
		{
			return -1;
		}

		short avatar_level = 0;
		if (!PugiGetSubNodeValue(data_element, "avatar_level", avatar_level) || avatar_level < 0 || avatar_level > BUBBLE_WINDOW_MAX_LEVEL)
		{
			return -2;
		}

		AvatarLevelConfig &cfg = m_avatar_level_cfg[avatar_type][avatar_level];
		cfg.avatar_type = avatar_type;
		cfg.avatar_level = avatar_level;

		max_avatar_level[avatar_type] = avatar_level;

		if (!PugiGetSubNodeValue(data_element, "is_active", cfg.is_active) || (cfg.is_active != 0 && cfg.is_active != 1))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "is_notice", cfg.is_notice) || (cfg.is_notice != 0 && cfg.is_notice != 1))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", cfg.shanbi) || cfg.shanbi < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", cfg.baoji) || cfg.baoji < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", cfg.jianren) || cfg.jianren < 0)
		{
			return -11;
		}

		PugiXmlNode Itemelement = data_element.child("common_item");
		if (!cfg.common_item.ReadConfig(Itemelement) && cfg.common_item.item_id != 0)
		{
			return -100;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const BubbleLevelConfig * PersonalizeWindowConfig::GetBubbleWindowLevelCfg(short bubble_type, short bubble_level)
{
	if (bubble_type < 0 || bubble_type >= BUBBLE_WINDOW_MAX_TYPE)
	{
		return NULL;
	}

	if (bubble_level < 0 || bubble_level > max_bubble_level[bubble_type] || bubble_level > BUBBLE_WINDOW_MAX_LEVEL)
	{
		return NULL;
	}

	if (m_bubble_level_cfg[bubble_type][bubble_level].bubble_type != bubble_type || m_bubble_level_cfg[bubble_type][bubble_level].bubble_level != bubble_level)
	{
		return NULL;
	}

	return  &m_bubble_level_cfg[bubble_type][bubble_level];
}

const int PersonalizeWindowConfig::GetTuhaojinMaxLevel(short bubble_type)
{
	if (bubble_type < 0 || bubble_type >= BUBBLE_WINDOW_MAX_TYPE)
	{
		return 0;
	}

	return max_bubble_level[bubble_type];
}

const AvatarLevelConfig * PersonalizeWindowConfig::GetAvatarWindowLevelCfg(short avatar_type, short avatar_level)
{
	if (avatar_type < 0 || avatar_type >= AVATAR_WINDOW_MAX_TYPE)
	{
		return nullptr;
	}

	if (avatar_level < 0 || avatar_level > max_avatar_level[avatar_type] || avatar_level > AVATAR_WINDOW_MAX_LEVEL)
	{
		return nullptr;
	}

	if (m_avatar_level_cfg[avatar_type][avatar_level].avatar_type != avatar_type || m_avatar_level_cfg[avatar_type][avatar_level].avatar_level != avatar_level)
	{
		return nullptr;
	}

	return &m_avatar_level_cfg[avatar_type][avatar_level];
}

const int PersonalizeWindowConfig::GetAvatarMaxLevel(short avatar_type)
{
	if (avatar_type < 0 || avatar_type >= AVATAR_WINDOW_MAX_TYPE)
	{
		return 0;
	}

	return max_avatar_level[avatar_type];
}


