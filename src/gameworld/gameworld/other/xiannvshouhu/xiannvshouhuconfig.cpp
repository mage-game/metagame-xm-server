#include <memory.h>
#include <stdlib.h>

#include "xiannvshouhuconfig.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "obj/character/attribute.hpp"
#include "gameworld/checkresourcecenter.hpp"

#include <string>

XiannvShouhuConfig::XiannvShouhuConfig()
{
}

XiannvShouhuConfig::~XiannvShouhuConfig()
{

}

bool XiannvShouhuConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "XiannvShouhuConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
// 		// 其他
// 		PugiXmlNode other_element = RootElement.child("other");
// 		if (other_element.empty())
// 		{
// 			*err = configname + ": no [other].";
// 			return false;
// 		}
// 
// 		iRet = InitOther(other_element);
// 		if (iRet)
// 		{
// 			sprintf(errinfo,"%s: InitOther failed %d", configname.c_str(), iRet);
// 			*err = errinfo;
// 			return false;
// 		}
	}

	{
		// 进阶配置
		PugiXmlNode grade_element = RootElement.child("grade");
		if (grade_element.empty())
		{
			*err = configname + ": no [grade].";
			return false;
		}

		iRet = InitGradeCfg(grade_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitGradeCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 进阶形象
		PugiXmlNode image_element = RootElement.child("image_list");
		if (image_element.empty())
		{
			*err = configname + ": no [image_list].";
			return false;
		}

		iRet = InitImageCfg(image_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitImageCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 升星物品配置
		PugiXmlNode up_start_stuff = RootElement.child("up_start_stuff");
		if (NULL == up_start_stuff)
		{
			*err = configname + ": no [up_start_stuff].";
			return false;
		}

		iRet = InitUpStarStuffCfg(up_start_stuff);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitUpStarStuffCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 升星经验值配置
		PugiXmlNode up_start_exp = RootElement.child("up_start_exp");
		if (NULL == up_start_exp)
		{
			*err = configname + ": no [up_start_exp].";
			return false;
		}

		iRet = InitUpStarExpCfg(up_start_exp);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitUpStarExpCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int XiannvShouhuConfig::InitOther(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (!dataElement.empty())
	{

	}

	return 0;
}

int XiannvShouhuConfig::InitGradeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		short grade = 0;
		if (!PugiGetSubNodeValue(dataElement, "grade", grade) || grade < 0 || grade >= XiannvShouhuParam::MAX_GRADE)
		{
			return -1;
		}

		XiannvShouhuGradeConfig &cfg = m_grade_cfg_list[grade];
		cfg.grade = grade;

		if (!PugiGetSubNodeValue(dataElement, "client_grade", cfg.client_grade) || cfg.client_grade < 0 || cfg.client_grade >=XiannvShouhuParam::MAX_GRADE)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "image_id", cfg.active_image_id) || cfg.active_image_id <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "bless_val_limit", cfg.bless_val_limit) || cfg.bless_val_limit <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "broadcast", cfg.is_broadcast))
		{
			return -5;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int XiannvShouhuConfig::InitImageCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int image_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "image_id", image_id) || image_id < 0)
		{
			return -1;
		}

		XiannvShouhuImageConfig cfg;

		cfg.image_id = image_id;

		m_image_list[cfg.image_id] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int XiannvShouhuConfig::InitUpStarExpCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_star_level = -1;
	while (!dataElement.empty())
	{
		int star_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "star_level", star_level) || star_level < 0 || star_level > XiannvShouhuParam::MAX_STAR_LEVEL || star_level <= last_star_level)
		{
			return -1;
		}

		XiannvShouhuUpStarExpCfg &cfg = m_upstar_exp_list[star_level];
		last_star_level = star_level;

		if (!PugiGetSubNodeValue(dataElement, "up_star_level_exp", cfg.up_star_level_exp) || cfg.up_star_level_exp <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "up_start_total_exp", cfg.up_start_total_exp) || cfg.up_start_total_exp < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren <= 0)
		{
			return -10;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int XiannvShouhuConfig::InitUpStarStuffCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_up_star_item_index = -1;
	while (!dataElement.empty())
	{
		int up_star_item_index = 0;
		if (!PugiGetSubNodeValue(dataElement, "up_star_item_index", up_star_item_index) || up_star_item_index < 0 || up_star_item_index > XiannvShouhuParam::MAX_UP_STAR_ITEM_COUNT || up_star_item_index <= last_up_star_item_index)
		{
			return -1;
		}

		XiannvShouhuUpStarStuffCfg &cfg = m_upstar_item_list[up_star_item_index];
		cfg.up_star_item_index = up_star_item_index;

		last_up_star_item_index = up_star_item_index;

		if (!PugiGetSubNodeValue(dataElement, "up_star_item_id", cfg.up_star_item_id) || cfg.up_star_item_id < 0 || NULL == ITEMPOOL->GetItem(cfg.up_star_item_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "star_exp", cfg.star_exp) || cfg.star_exp < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji_per", cfg.baoji_per) || cfg.baoji_per < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji_mul", cfg.baoji_mul) || cfg.baoji_mul <= 0)
		{
			return -5;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const XiannvShouhuGradeConfig * XiannvShouhuConfig::GetGradeConfig(int grade) const
{
	if (grade < 0 || grade >= XiannvShouhuParam::MAX_GRADE || m_grade_cfg_list[grade].grade <= 0)
	{
		return NULL;
	}

	return &m_grade_cfg_list[grade];	
}

const XiannvShouhuImageConfig * XiannvShouhuConfig::GetImageCfg(int attr_id) const
{
	if (attr_id < 0 || attr_id > XiannvShouhuParam::MAX_IMAGE_ID)
	{
		return NULL;
	}

	if (m_image_list[attr_id].image_id != attr_id)
	{
		return NULL;
	}

	return &m_image_list[attr_id];
}

const XiannvShouhuUpStarExpCfg * XiannvShouhuConfig::GetUpStarExpCfg(int star_level)
{
	if (star_level < 0 || star_level >= XiannvShouhuParam::MAX_STAR_LEVEL || m_upstar_exp_list[star_level].up_star_level_exp <= 0)
		return NULL;

	return &m_upstar_exp_list[star_level];
}

const XiannvShouhuUpStarStuffCfg * XiannvShouhuConfig::GetUpStarStuffCfg(int stuff_index)
{
	if (stuff_index < 0 || stuff_index >= XiannvShouhuParam::MAX_UP_STAR_ITEM_COUNT)
		return NULL;

	return &m_upstar_item_list[stuff_index];
}
