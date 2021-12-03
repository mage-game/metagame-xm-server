#include "multimountconfig.hpp"
#include "obj/character/role.h"
#include "item/itemextern.hpp"
#include "world.h"
#include "item/itempool.h"
#include <servercommon/configcommon.h>

MultiMountConfig::MultiMountConfig() : m_mount_max_id(-1), m_max_special_img_id(0) 
{

}

MultiMountConfig::~MultiMountConfig()
{

}

int MultiMountInfoConfig::GradeCfg::GetRandFailBless() const
{
	int range = fail_add_bless_max - fail_add_bless_min + 1; // 闭区间 [fail_add_bless_max, fail_add_bless_min]
	if (range <= 0)
	{
		return 0;
	}
	return RandomNum(range) + fail_add_bless_min;
}

bool MultiMountConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "MultiMountConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 其他配置信息
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 双人坐骑信息
		PugiXmlNode root_element = RootElement.child("mount_info");
		if (root_element.empty())
		{
			*err = configname + ": no [mount_info].";
			return false;
		}

		iRet = this->InitMultiMountInfoCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitMultiMountInfoCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 双人坐骑进阶配置
		PugiXmlNode root_element = RootElement.child("grade");
		if (root_element.empty())
		{
			*err = configname + ": no [grade].";
			return false;
		}

		iRet = this->InitMultiMountUpgradeCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitMultiMountUpgradeCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 双人坐骑幻化配置
		PugiXmlNode root_element = RootElement.child("special_img_cfg");
		if (root_element.empty())
		{
			*err = configname + ": no [special_img_cfg].";
			return false;
		}

		iRet = this->InitMultiMountSpecialImgCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitMultiMountSpecialImgCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 双人坐骑装备配置
		PugiXmlNode root_element = RootElement.child("equip");
		if (root_element.empty())
		{
			*err = configname + ": no [equip].";
			return false;
		}

		iRet = this->InitMultiMountEquipCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitMultiMountEquipCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 双人特殊形象进阶配置
		PugiXmlNode root_element = RootElement.child("special_img_uplevel");
		if (root_element.empty())
		{
			*err = configname + ": no [special_img_uplevel].";
			return false;
		}

		iRet = this->InitMultiMountSpecialImgUplevelCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitMultiMountSpecialImgUplevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	//{
	//	// 双人特殊形象超强战力配置
	//	PugiXmlNode root_element = RootElement.child("huanhua_special_cap_add");
	//	if (root_element.empty())
	//	{
	//		*err = configname + ": no [huanhua_special_cap_add].";
	//		return false;
	//	}

	//	iRet = this->InitHuanHuaSpecialCapAddCfg(root_element);
	//	if (iRet)
	//	{
	//		sprintf(errinfo, "%s: InitHuanHuaSpecialCapAddCfg failed %d", configname.c_str(), iRet);
	//		*err = errinfo;
	//		return false;
	//	}
	//}

	return true;
}

const MultiMountInfoConfig * MultiMountConfig::GetMultiMountCfg(int mount_id)
{
	if (mount_id <= 0 || mount_id > m_mount_max_id || mount_id > COUPLEMOUNT_MAX_ID)
	{
		return NULL;
	}

	return &m_mount_cfg_list[mount_id];
}

const MultiMountInfoConfig::GradeCfg * MultiMountConfig::GetMultiMountGradeCfg(int mount_id, int grade) const
{
	if (mount_id <= 0 || mount_id > m_mount_max_id || mount_id > COUPLEMOUNT_MAX_ID)
	{
		return NULL;
	}

	const MultiMountInfoConfig &mount_cfg = m_mount_cfg_list[mount_id];
	if (grade < 0 || grade > mount_cfg.max_grade || grade > COUPLEMOUNT_MAX_GRADE)
	{
		return NULL;
	}

	return &mount_cfg.upgrade_cfglist[grade];
}

const MultiMountInfoConfig::GradeCfg * MultiMountConfig::GetMultiMountActiveImgCfg(int mount_id) const
{
	if (mount_id <= 0 || mount_id > m_mount_max_id || mount_id > COUPLEMOUNT_MAX_ID)
	{
		return NULL;
	}

	const MultiMountInfoConfig &mount_cfg = m_mount_cfg_list[mount_id];
	if (mount_cfg.first_active_img_grade >= 0 && mount_cfg.first_active_img_grade <= mount_cfg.max_grade)
	{
		return &mount_cfg.upgrade_cfglist[mount_cfg.first_active_img_grade];
	}

	return NULL;
}

const MultiMountSpecialConfig * MultiMountConfig::GetMultiMountSpecialImgCfg(int image_id) const
{
	if (image_id <= 0 || image_id > m_max_special_img_id || image_id > COUPLEMOUNT_SPECIAL_IMG_MAX_ID_LIMIT)
	{
		return NULL;
	}

	return &m_special_img_cfg_list[image_id];
}

const MultimountEquipConfig * MultiMountConfig::GetEquipCfg(int equip_type) const
{
	if (equip_type < 0 || equip_type >= MULTIMOUNT_EQUIP_TYPE_NUM)
	{
		return NULL;
	}

	return &m_equip_cfg_list[equip_type];
}

const MultimountSpecialImgUplevelCfg * MultiMountConfig::GetSpecialImgUplevelCfg(int special_img_id, int grade)
{
	if (special_img_id < 0 || grade < 0) return NULL;

	for (int i = 0; i < (int)m_multi_mount_special_img_uplevel_cfg.size(); ++i)
	{
		if (special_img_id == m_multi_mount_special_img_uplevel_cfg[i].special_img_id && grade == m_multi_mount_special_img_uplevel_cfg[i].grade)
		{
			return &m_multi_mount_special_img_uplevel_cfg[i];
		}
	}

	return NULL;
}

int MultiMountConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

// 	if (!PugiGetSubNodeValue(data_element, "choucard_need_coin", m_other_cfg.choucard_need_coin) || m_other_cfg.choucard_need_coin <= 0)
// 	{
// 		return -1;
// 	}

	/*if (!PugiGetSubNodeValue(data_element, "is_open_special_cap_add", m_is_open_special_cap_add))
	{
		return -2;
	}*/

	return 0;
}

int MultiMountConfig::InitMultiMountInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_mount_max_id = 0;

	while (!data_element.empty())
	{
		if (m_mount_max_id >= COUPLEMOUNT_MAX_ID)
		{
			return -1;
		}

		int mount_id = 0;
		if (!PugiGetSubNodeValue(data_element, "mount_id", mount_id) || mount_id != m_mount_max_id + 1)
		{
			return -2;
		}

		MultiMountInfoConfig &mount_cfg = m_mount_cfg_list[mount_id];
		mount_cfg.mount_id = mount_id;
		m_mount_max_id = mount_id;

		if (!PugiGetSubNodeValue(data_element, "res_id", mount_cfg.res_id) || mount_cfg.res_id <= 0)
		{
			return -3;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int MultiMountConfig::InitMultiMountUpgradeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int mount_id = 0;
		if (!PugiGetSubNodeValue(data_element, "mount_id", mount_id) || mount_id <= 0 || mount_id > m_mount_max_id || mount_id > COUPLEMOUNT_MAX_ID)
		{
			return -1;
		}

		MultiMountInfoConfig &mount_cfg = m_mount_cfg_list[mount_id];

		if (mount_cfg.max_grade >= COUPLEMOUNT_MAX_GRADE)
		{
			return -2;
		}

		int grade = 0;
		if (!PugiGetSubNodeValue(data_element, "grade", grade) || grade != mount_cfg.max_grade + 1)
		{
			return -3;
		}

		MultiMountInfoConfig::GradeCfg &grade_cfg = mount_cfg.upgrade_cfglist[grade];
		grade_cfg.star_level = grade;
		mount_cfg.max_grade = grade;

		if (!PugiGetSubNodeValue(data_element, "upgrade_stuff_id", grade_cfg.upgrade_stuff_id) || NULL == ITEMPOOL->GetItem(grade_cfg.upgrade_stuff_id))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "upgrade_stuff_num", grade_cfg.upgrade_stuff_num) || grade_cfg.upgrade_stuff_num <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "max_bless", grade_cfg.max_bless) || grade_cfg.max_bless <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "min_need_bless", grade_cfg.min_need_bless) || grade_cfg.min_need_bless <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "success_rate", grade_cfg.success_rate) || grade_cfg.success_rate <= 0 || grade_cfg.success_rate >= 10000)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "fail_add_bless_min", grade_cfg.fail_add_bless_min) || grade_cfg.fail_add_bless_min <= 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "fail_add_bless_max", grade_cfg.fail_add_bless_max) || grade_cfg.fail_add_bless_max < grade_cfg.fail_add_bless_min)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "active_mount_id", grade_cfg.active_mount_id) || grade_cfg.active_mount_id < 0)
		{
			return -11;
		}

		int is_active_image = 0;
		if (!PugiGetSubNodeValue(data_element, "is_active_image", is_active_image) || is_active_image < 0 || is_active_image > 1)
		{
			return -12;
		}
		grade_cfg.is_active_image = (0 != is_active_image);

		if (grade_cfg.is_active_image && -1 == mount_cfg.first_active_img_grade)
		{
			mount_cfg.first_active_img_grade = grade_cfg.star_level;
		}

		int is_broadcast = 0;
		if (!PugiGetSubNodeValue(data_element, "is_broadcast", is_broadcast) || is_broadcast < 0 || is_broadcast > 1)
		{
			return -13;
		}
		grade_cfg.is_broadcast = (0 != is_broadcast);

		if (!PugiGetSubNodeValue(data_element, "move_speed", grade_cfg.move_speed) || grade_cfg.move_speed < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", grade_cfg.maxhp) || grade_cfg.maxhp < 0)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", grade_cfg.gongji) || grade_cfg.gongji < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", grade_cfg.fangyu) || grade_cfg.fangyu < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", grade_cfg.mingzhong) || grade_cfg.mingzhong < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", grade_cfg.shanbi) || grade_cfg.shanbi < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", grade_cfg.baoji) || grade_cfg.baoji < 0)
		{
			return -105;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", grade_cfg.jianren) || grade_cfg.jianren < 0)
		{
			return -106;
		}

		data_element = data_element.next_sibling();
	}

	// 测试一下，从激活形象的阶数到满阶（不包含满阶），有无可能激活坐骑id（预防使用物品卡直接达到某个阶数后，无法再激活下一阶的坐骑了）
	for (int mount_id = 1; mount_id < m_mount_max_id; ++mount_id)
	{
		bool can_active_next_mount = false;

		MultiMountInfoConfig &mount_cfg = m_mount_cfg_list[mount_id];
		for (int grade = mount_cfg.first_active_img_grade + 1; grade <= mount_cfg.max_grade; ++grade)
		{
			if (mount_cfg.upgrade_cfglist[grade].active_mount_id > 0)
			{
				can_active_next_mount = true;
			}
		}

		if (!can_active_next_mount)
		{
			return -20000;
		}
	}

	return 0;
}

int MultiMountConfig::InitMultiMountSpecialImgCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_max_special_img_id = 0;

	while (!data_element.empty())
	{
		if (m_max_special_img_id >= COUPLEMOUNT_SPECIAL_IMG_MAX_ID_LIMIT)
		{
			return -1;
		}

		int image_id = 0;
		if (!PugiGetSubNodeValue(data_element, "image_id", image_id) || image_id != m_max_special_img_id + 1)
		{
			return -2;
		}

		MultiMountSpecialConfig &special_img_cfg = m_special_img_cfg_list[image_id];
		special_img_cfg.image_id = image_id;
		m_max_special_img_id = image_id;

		if (!PugiGetSubNodeValue(data_element, "res_id", special_img_cfg.res_id) || special_img_cfg.res_id <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "move_speed", special_img_cfg.move_speed) || special_img_cfg.move_speed < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", special_img_cfg.maxhp) || special_img_cfg.maxhp < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", special_img_cfg.gongji) || special_img_cfg.gongji < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", special_img_cfg.fangyu) || special_img_cfg.fangyu < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", special_img_cfg.mingzhong) || special_img_cfg.mingzhong < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", special_img_cfg.shanbi) || special_img_cfg.shanbi < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", special_img_cfg.baoji) || special_img_cfg.baoji < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", special_img_cfg.jianren) || special_img_cfg.jianren < 0)
		{
			return -11;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int MultiMountConfig::InitMultiMountEquipCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int next_equip_type = 0;

	while (!data_element.empty())
	{
		if (next_equip_type >= MULTIMOUNT_EQUIP_TYPE_NUM)
		{
			return -1;
		}

		int equip_type = 0;
		if (!PugiGetSubNodeValue(data_element, "equip_type", equip_type) || equip_type != next_equip_type)
		{
			return -2;
		}
		++ next_equip_type;

		MultimountEquipConfig &equip_cfg = m_equip_cfg_list[equip_type];
		equip_cfg.equip_type = equip_type;

		if (!PugiGetSubNodeValue(data_element, "max_level", equip_cfg.max_level) || equip_cfg.max_level <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "upgrade_need_stuff", equip_cfg.upgrade_need_stuff) || NULL == ITEMPOOL->GetItem(equip_cfg.upgrade_need_stuff))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "upgrade_stuff_count", equip_cfg.upgrade_stuff_count) || equip_cfg.upgrade_stuff_count <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", equip_cfg.maxhp) || equip_cfg.maxhp < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", equip_cfg.gongji) || equip_cfg.gongji < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", equip_cfg.fangyu) || equip_cfg.fangyu < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", equip_cfg.mingzhong) || equip_cfg.mingzhong < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", equip_cfg.shanbi) || equip_cfg.shanbi < 0)
		{
			return -105;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int MultiMountConfig::InitMultiMountSpecialImgUplevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		MultimountSpecialImgUplevelCfg cfg;
		if (!PugiGetSubNodeValue(dataElement, "special_img_id", cfg.special_img_id) || cfg.special_img_id <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "grade", cfg.grade) || cfg.grade < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_id", cfg.stuff_id) || NULL == ITEMPOOL->GetItem(cfg.stuff_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_num", cfg.stuff_num) || cfg.stuff_num <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0) return -101;

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0) return -102;

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0) return -103;

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) return -104;

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0) return -105;

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0) return -106;

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0) return -107;

		m_multi_mount_special_img_uplevel_cfg.push_back(cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}


//const MultimountHuanHuaSpecialCapAddCfgItem* MultiMountConfig::GetHuanHuaSpecialCapAddCfg(int huanhua_id)const
//{
//	if (huanhua_id <= 0 || huanhua_id > MULTIMOUNT_SPECIAL_IMG_MAX_ID_LIMIT)
//	{
//		return nullptr;
//	}
//
//	auto iter = m_huanhua_special_cap_cfg.find(huanhua_id);
//	if (iter != m_huanhua_special_cap_cfg.end())
//	{
//		return &(iter->second);
//	}
//
//	return nullptr;
//}

//bool MultiMountConfig::IsAddSpecialCap()
//{
//	return m_is_open_special_cap_add;
//}

//int MultiMountConfig::InitHuanHuaSpecialCapAddCfg(PugiXmlNode RootElement)
//{
//	PugiXmlNode dataElement = RootElement.child("data");
//	if (dataElement.empty())
//	{
//		return -10000;
//	}
//
//	while (!dataElement.empty())
//	{
//		MultimountHuanHuaSpecialCapAddCfgItem cfg_item;
//		if (!PugiGetSubNodeValue(dataElement, "huanhua_id", cfg_item.huanhua_id) || cfg_item.huanhua_id <= 0 || cfg_item.huanhua_id > MULTIMOUNT_SPECIAL_IMG_MAX_ID_LIMIT)
//		{
//			return -1;
//		}
//
//		if (!PugiGetSubNodeValue(dataElement, "huanhua_level", cfg_item.huanhua_level) || cfg_item.huanhua_level <= 0 || cfg_item.huanhua_level > MULTIMOUNT_SPECIAL_IMG_MAX_ID_LIMIT)
//		{
//			return -2;
//		}
//
//		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg_item.maxhp) || cfg_item.maxhp < 0)
//		{
//			return -3;
//		}
//
//		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg_item.gongji) || cfg_item.gongji < 0)
//		{
//			return -4;
//		}
//
//		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg_item.fangyu) || cfg_item.fangyu < 0)
//		{
//			return -5;
//		}
//
//		// 判断重复
//		if (m_huanhua_special_cap_cfg.find(cfg_item.huanhua_id) != m_huanhua_special_cap_cfg.end())
//		{
//			return -6;
//		}
//		m_huanhua_special_cap_cfg[cfg_item.huanhua_id] = cfg_item;
//
//		dataElement = dataElement.next_sibling();
//	}
//
//	return 0;
//}
