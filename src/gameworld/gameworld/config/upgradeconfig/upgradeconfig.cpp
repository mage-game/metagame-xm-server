#include "upgradeconfig.hpp"
#include "upgradeconfigmanager.hpp"
#include "globalconfig/equipconfig.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "gameworld/checkresourcecenter.hpp"
#include "config/logicconfigmanager.hpp"


#include <string>


UpgradeConfig::UpgradeConfig(UpgradeconfigManager *upgradeconfig_manager, int upgrade_type) 
	: m_upgradeconfig_manager(upgradeconfig_manager), m_upgrade_type(upgrade_type),
	m_up_one_grade_reward_id(0), m_temporary_image_time(0), m_active_equip_grade(0), 
	m_active_equip_skill_level(0), m_is_open_special_cap_add(false), m_medal_skill_index(0)
{
	memset(m_equip_skill_level_cfg, 0, sizeof(m_equip_skill_level_cfg));
}

UpgradeConfig::~UpgradeConfig()
{

}

bool UpgradeConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	// 其他
	PUGI_XML_LOAD_CONFIG("other", InitOtherCfg);

	// 等级
// 	if (IsModuleOpen(UPGRADE_MODULE_TYPE_LEVEL))
// 	{
// 		PUGI_XML_LOAD_CONFIG("level", InitLevelCfg);
// 	}

	// 进阶
	if (IsModuleOpen(UPGRADE_MODULE_TYPE_UPGRADE))
	{
		PUGI_XML_LOAD_CONFIG("grade", InitGradeCfg);
		PUGI_XML_LOAD_CONFIG("upgrade_prob", InitUpGradeProbCfg);
	}

	// 形象
	if (IsModuleOpen(UPGRADE_MODULE_TYPE_IMAGE))
	{
		PUGI_XML_LOAD_CONFIG("image_list", InitImageCfg);
		PUGI_XML_LOAD_CONFIG("image_upgrade", InitImageUpgradeCfg);
	}

	// 装备
	if (IsModuleOpen(UPGRADE_MODULE_TYPE_EQUIP))
	{
		PUGI_XML_LOAD_CONFIG("equip_info", InitEquipCfg);
	}

	// 技能
	if (IsModuleOpen(UPGRADE_MODULE_TYPE_SKILL))
	{
		PUGI_XML_LOAD_CONFIG("skill", InitSkillCfg);
	}

	// 超强战力加成
	if (IsModuleOpen(UPGRADE_MODULE_TYPE_BIG_GOAL))
	{
		PUGI_XML_LOAD_CONFIG("big_goal_cap_add", InitSuperSpecialCapAddCfg);
	}

	return true;
}

bool UpgradeConfig::IsModuleOpen(int module_type)
{
	bool is_open = LOGIC_CONFIG->GetUpgradeCfgManager().IsUpgradeModuleOpen(m_upgrade_type, module_type);

	return is_open;
}

//其他相关---------------------------------------------------------------
int UpgradeConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "up_one_grade_reward_id", m_up_one_grade_reward_id) || NULL == ITEMPOOL->GetItem(m_up_one_grade_reward_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "temporary_image_time", m_temporary_image_time) || m_temporary_image_time < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "active_equip_grade", m_active_equip_grade) || m_active_equip_grade < 0 || m_active_equip_grade > UPGRADE_MAX_GRADE)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "active_equip_skill_level", m_active_equip_skill_level) || m_active_equip_skill_level < 0 || m_active_equip_skill_level > UPGRADE_EQUIP_MAX_LEVEL)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "extra_attrs_per_grade", m_other_cfg.extra_attrs_per_grade) || m_other_cfg.extra_attrs_per_grade <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "extra_attrs_per", m_other_cfg.extra_attrs_per) || m_other_cfg.extra_attrs_per < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_open_special_cap_add", m_is_open_special_cap_add))
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "medal_skill_index", m_medal_skill_index) || m_medal_skill_index < 0 || m_medal_skill_index >= UPGRADE_SKILL_COUNT)
		{
			return -9;
		}
	}

	return 0;
}

//进阶相关---------------------------------------------------------------
int UpgradeConfig::InitLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");

	while (!dataElement.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level <= 0 || level > UPGRADE_MAX_LEVEL)
		{
			return -1;
		}

		LevelConfig cfg;
		cfg.level = level;

		int iRet = cfg.attr_cfg.ReadConfig(dataElement);
		if (iRet < 0)
		{
			return iRet;
		}

		m_level_list[level] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int UpgradeConfig::InitGradeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		short grade = 0;
		if (!PugiGetSubNodeValue(dataElement, "grade", grade) || grade < 0 || grade >= UPGRADE_MAX_GRADE)
		{
			return -1;
		}

		GradeConfig &cfg = m_grade_cfg_list[grade];
		cfg.grade = grade;

		if (!PugiGetSubNodeValue(dataElement, "client_grade", cfg.client_grade) || cfg.client_grade < 0 || cfg.client_grade >= UPGRADE_MAX_GRADE - 1)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "upgrade_stuff_id", cfg.upgrade_stuff_id) || NULL == ITEMPOOL->GetItem(cfg.upgrade_stuff_id))
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(dataElement, "upgrade_stuff2_id", cfg.upgrade_stuff2_id) || NULL == ITEMPOOL->GetItem(cfg.upgrade_stuff2_id))
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(dataElement, "upgrade_stuff_count", cfg.upgrade_stuff_count) || cfg.upgrade_stuff_count <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "image_id", cfg.active_image_id) || cfg.active_image_id <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "bless_val_limit", cfg.bless_val_limit) || cfg.bless_val_limit <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "broadcast", cfg.is_broadcast))
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "shuxingdan_limit", cfg.shuxingdan_limit) || cfg.shuxingdan_limit < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "chengzhangdan_limit", cfg.chengzhangdan_limit) || cfg.chengzhangdan_limit < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_clear_bless", cfg.is_clear_bless) || cfg.is_clear_bless < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "bless_addition", cfg.bless_addition) || cfg.bless_addition < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "equip_level_limit", cfg.equip_level_limit) || cfg.equip_level_limit < 0)
		{
			return -13;
		}

		int iRet = cfg.attr_cfg.ReadConfig(dataElement);
		if (iRet < 0)
		{
			return iRet;
		}

		if (!PugiGetSubNodeValue(dataElement, "movespeed", cfg.movespeed) || cfg.movespeed < 0) return -110;

		if (!PugiGetSubNodeValue(dataElement, "extra_zengshang", cfg.extra_zengshang) || cfg.extra_zengshang < 0) return -111;

		if (!PugiGetSubNodeValue(dataElement, "extra_mianshang", cfg.extra_mianshang) || cfg.extra_mianshang < 0) return -112;

		if (!PugiGetSubNodeValue(dataElement, "per_jingzhun", cfg.per_jingzhun) || cfg.per_jingzhun < 0) return -113;

		if (!PugiGetSubNodeValue(dataElement, "per_baoji", cfg.per_baoji) || cfg.per_baoji < 0) return -114;

		if (!PugiGetSubNodeValue(dataElement, "jianshang_per", cfg.jianshang_per) || cfg.jianshang_per < 0) return -115;

		if (!PugiGetSubNodeValue(dataElement, "zengshang_per", cfg.zengshang_per) || cfg.zengshang_per < 0) return -116;

		if (!PugiGetSubNodeValue(dataElement, "pvp_zengshang_per", cfg.pvp_zengshang_per) || cfg.pvp_zengshang_per < 0) return -117;

		if (!PugiGetSubNodeValue(dataElement, "pve_zengshang_per", cfg.pve_zengshang_per) || cfg.pve_zengshang_per < 0) return -118;

		if (!PugiGetSubNodeValue(dataElement, "pvp_jianshang_per", cfg.pvp_jianshang_per) || cfg.pvp_jianshang_per < 0) return -119;

		if (!PugiGetSubNodeValue(dataElement, "pve_jianshang_per", cfg.pve_jianshang_per) || cfg.pve_jianshang_per < 0) return -120;

		if (!PugiGetSubNodeValue(dataElement, "equip_level_toplimit", cfg.equip_level_toplimit) || cfg.equip_level_toplimit < 0) return -121;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int UpgradeConfig::InitUpGradeProbCfg(PugiXmlNode RootElement)
{
	short last_high = -1;
	short last_grade = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int grade = 0;
		if (!PugiGetSubNodeValue(dataElement, "grade", grade) || grade < 0)
		{
			return -1;
		}

		if (grade != last_grade) last_high = -1;

		UpGradeProbConfig cfg;

		if (!PugiGetSubNodeValue(dataElement, "low_bless", cfg.low_bless_val) || cfg.low_bless_val < 0)
		{
			return -3;
		}

		if (cfg.low_bless_val != last_high + 1) 
		{
			return -20;
		}

		if (!PugiGetSubNodeValue(dataElement, "high_bless", cfg.high_bless_val) || cfg.high_bless_val < cfg.low_bless_val)
		{
			return -4;
		}

		last_high = cfg.high_bless_val;

		if (!PugiGetSubNodeValue(dataElement, "succ_rate", cfg.succ_rate) || cfg.succ_rate < 0 || cfg.succ_rate > 10000)
		{
			return -5;
		}

		if (!cfg.upgrade_lucky_cfg.ReadConfig(dataElement, "upgrade_lucky_vals", "upgrade_lucky_rates"))
		{
			return -6;
		}

		std::map<int, UpGradeProbList>::iterator iter = m_upgrade_prob_map.find(grade);
		if (m_upgrade_prob_map.end() != iter)
		{
			UpGradeProbList &cfg_list = iter->second;
			if (!cfg_list.Add(cfg))
			{
				return -50;
			}
		}
		else
		{
			UpGradeProbList cfg_list;
			cfg_list.grade = grade;
			cfg_list.Add(cfg);
			m_upgrade_prob_map[grade] = cfg_list;
		}

		last_grade = grade;

		dataElement = dataElement.next_sibling();
	}

	for (std::map<int, UpGradeProbList>::const_iterator iter = m_upgrade_prob_map.begin(); m_upgrade_prob_map.end() != iter; ++iter)
	{
		const UpGradeProbList &cfg_list = iter->second;

		if (cfg_list.prob_count <= 0 || cfg_list.prob_count > UpGradeProbList::MAX_PROB_COUNT_PER_GRADE)
		{
			return -60;
		}
		const GradeConfig *grade_cfg = this->GetGradeConfig(cfg_list.grade);
		if (NULL == grade_cfg)
		{
			return -100;
		}
		if (grade_cfg->bless_val_limit != cfg_list.prob_list[cfg_list.prob_count - 1].high_bless_val)
		{
			return -101;
		}
	}

	return 0;
}

const LevelConfig * UpgradeConfig::GetLevelConfig(int level) const
{
	if (level < 0 || level > UPGRADE_MAX_LEVEL)
	{
		return NULL;
	}

	if (m_level_list[level].level != level)
	{
		return NULL;
	}

	return &m_level_list[level];
}

const GradeConfig * UpgradeConfig::GetGradeConfig(int grade) const
{
	if (grade < 0 || grade >= UPGRADE_MAX_GRADE || m_grade_cfg_list[grade].grade <= 0)
	{
		return NULL;
	}

	return &m_grade_cfg_list[grade];
}

const UpGradeProbConfig * UpgradeConfig::GetUpGradeProbConfig(int grade, int bless_val) const
{
	std::map<int, UpGradeProbList>::const_iterator iter = m_upgrade_prob_map.find(grade);
	if (m_upgrade_prob_map.end() != iter)
	{
		const UpGradeProbList &cfg_list = iter->second;

		for (int i = 0; i < cfg_list.prob_count && i < UpGradeProbList::MAX_PROB_COUNT_PER_GRADE; ++i)
		{
			if (cfg_list.prob_list[i].low_bless_val <= bless_val && bless_val <= cfg_list.prob_list[i].high_bless_val)
			{
				return &cfg_list.prob_list[i];
			}
		}
	}

	return NULL;
}

//形象相关---------------------------------------------------------------
int UpgradeConfig::InitImageCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int image_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "image_id", image_id) || image_id <= 0 || image_id >= UPGRADE_IMAGE_MAX_COUNT)
		{
			return -1;
		}

		ImageConfig cfg;

		cfg.image_id = image_id;

		m_image_list[cfg.image_id] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int UpgradeConfig::InitImageUpgradeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_image_id = 0;
	int last_grade = -1;

	while (!dataElement.empty())
	{
		int image_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "image_id", image_id) || image_id <= 0 || image_id >= UPGRADE_IMAGE_MAX_COUNT)
		{
			return -1;
		}

		if (image_id != last_image_id)
		{
			last_image_id = image_id;
			last_grade = -1;
		}

		ImgUpgradeCfgList &cfg_list = m_special_img_upgrade_map[image_id];

		cfg_list.max_grade = 0;
		if (!PugiGetSubNodeValue(dataElement, "grade", cfg_list.max_grade) || cfg_list.max_grade < 0 || cfg_list.max_grade > UPGRADE_MAX_IMAGE_GRADE
			|| cfg_list.max_grade != last_grade + 1)
		{
			return -2;
		}

		last_grade = cfg_list.max_grade;

		ImgUpgradeCfg &cfg_item = cfg_list.upgrade_list[cfg_list.max_grade];
		cfg_item.grade = cfg_list.max_grade;

		if (!PugiGetSubNodeValue(dataElement, "stuff_id", cfg_item.stuff_id) || NULL == ITEMPOOL->GetItem(cfg_item.stuff_id))
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_num", cfg_item.stuff_num) || cfg_item.stuff_num <= 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(dataElement, "shuxingdan_count", cfg_item.shuxingdan_count) || cfg_item.shuxingdan_count < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "chengzhangdan_count", cfg_item.chengzhangdan_count) || cfg_item.chengzhangdan_count < 0)
		{
			return -4;
		}

// 		if (!PugiGetSubNodeValue(dataElement, "equip_level", cfg_item.equip_level) || cfg_item.equip_level < 0)
// 		{
// 			return -5;
// 		}

		if (!PugiGetSubNodeValue(dataElement, "add_sys_per", cfg_item.add_per) || cfg_item.add_per < 0)
		{
			return -6;
		}

		int iRet = cfg_item.attr_cfg.ReadConfig(dataElement);
		if (iRet < 0)
		{
			return iRet;
		}

		if (!PugiGetSubNodeValue(dataElement, "extra_zengshang", cfg_item.extra_zengshang) || cfg_item.extra_zengshang < 0) return -111;

		if (!PugiGetSubNodeValue(dataElement, "extra_mianshang", cfg_item.extra_mianshang) || cfg_item.extra_mianshang < 0) return -112;

		if (!PugiGetSubNodeValue(dataElement, "per_jingzhun", cfg_item.per_jingzhun) || cfg_item.per_jingzhun < 0) return -113;

		if (!PugiGetSubNodeValue(dataElement, "per_baoji", cfg_item.per_baoji) || cfg_item.per_baoji < 0) return -114;

		if (!PugiGetSubNodeValue(dataElement, "jianshang_per", cfg_item.jianshang_per) || cfg_item.jianshang_per < 0) return -115;

		if (!PugiGetSubNodeValue(dataElement, "zengshang_per", cfg_item.zengshang_per) || cfg_item.zengshang_per < 0) return -116;

		if (!PugiGetSubNodeValue(dataElement, "pvp_zengshang_per", cfg_item.pvp_zengshang_per) || cfg_item.pvp_zengshang_per < 0) return -117;

		if (!PugiGetSubNodeValue(dataElement, "pve_zengshang_per", cfg_item.pve_zengshang_per) || cfg_item.pve_zengshang_per < 0) return -118;

		if (!PugiGetSubNodeValue(dataElement, "pvp_jianshang_per", cfg_item.pvp_jianshang_per) || cfg_item.pvp_jianshang_per < 0) return -119;

		if (!PugiGetSubNodeValue(dataElement, "pve_jianshang_per", cfg_item.pve_jianshang_per) || cfg_item.pve_jianshang_per < 0) return -120;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const ImageConfig * UpgradeConfig::GetImageCfg(int image_id) const
{
	if (image_id <= 0 || image_id >= UPGRADE_IMAGE_MAX_COUNT)
	{
		return NULL;
	}

	if (m_image_list[image_id].image_id != image_id)
	{
		return NULL;
	}

	return &m_image_list[image_id];
}

const ImgUpgradeCfg * UpgradeConfig::GetImgUpgradeCfg(int special_img_id, int grade)
{
	ImgUpgradeCfgMap::iterator iter = m_special_img_upgrade_map.find(special_img_id);
	if (iter != m_special_img_upgrade_map.end())
	{
		ImgUpgradeCfgList &cfg_list = iter->second;

		if (grade < 0 || grade > UPGRADE_MAX_IMAGE_GRADE || grade > iter->second.max_grade)
		{
			return NULL;
		}

		if (cfg_list.upgrade_list[grade].grade == grade)
		{
			return &cfg_list.upgrade_list[grade];
		}
	}

	return NULL;
}


//装备相关---------------------------------------------------------------
int UpgradeConfig::InitEquipCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int equip_idx = 0;
		if (!PugiGetSubNodeValue(dataElement, "equip_idx", equip_idx) || equip_idx < 0 || equip_idx >= UPGRADE_EQUIP_COUNT)
		{
			return -1;
		}

		EquipConifg &equip_cfg = m_equip_cfg_list[equip_idx];

		int level = 0;

		if (!PugiGetSubNodeValue(dataElement, "equip_level", level) || level < 0 || level > UPGRADE_EQUIP_MAX_LEVEL) return -2;

		PugiXmlNode item_element = dataElement.child("item");
		if (NULL != item_element)
		{
			if (!equip_cfg.level_cfg[level].item.ReadConfig(item_element))
			{
				return -3;
			}
		}

		int iRet = equip_cfg.level_cfg[level].attr_cfg.ReadConfig(dataElement);
		if (iRet < 0)
		{
			return iRet;
		}

		equip_cfg.max_level++;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const EquipConifg * UpgradeConfig::GetEquipCfg(int equip_idx)
{
	if (equip_idx >= 0 && equip_idx < UPGRADE_EQUIP_COUNT)
	{
		return &m_equip_cfg_list[equip_idx];
	}

	return NULL;
}


//技能相关---------------------------------------------------------------
int UpgradeConfig::InitSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int skill_idx = 0;
		if (!PugiGetSubNodeValue(dataElement, "skill_idx", skill_idx) || skill_idx < 0 || skill_idx >= UPGRADE_SKILL_COUNT)
		{
			return -1;
		}

		int skill_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "skill_level", skill_level) || skill_level < 0 || skill_level > UPGRADE_SKILL_MAX_LEVEL)
		{
			return -2;
		}

		SkillConifg &cfg = m_skill_cfg_list[skill_idx][skill_level];
		cfg.index = skill_idx;
		cfg.skill_level = skill_level;

		if (!PugiGetSubNodeValue(dataElement, "skill_type", cfg.skill_type) || cfg.skill_type < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "grade", cfg.grade) || cfg.grade < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "uplevel_stuff_id", cfg.uplevel_stuff_id) || (cfg.skill_type > 0 && NULL == ITEMPOOL->GetItem(cfg.uplevel_stuff_id)))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "uplevel_stuff_num", cfg.uplevel_stuff_num) || cfg.uplevel_stuff_num < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_a", cfg.param_a) || cfg.param_a < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_b", cfg.param_b) || cfg.param_b < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_c", cfg.param_c) || cfg.param_c < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_d", cfg.param_d) || cfg.param_d < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "capability", cfg.capability) || cfg.capability < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "skill_cap_per", cfg.skill_cap_per) || cfg.skill_cap_per < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "base_attr_add_per", cfg.base_attr_add_per) || cfg.base_attr_add_per < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "end_day_of_up_skill3", cfg.end_day_of_up_skill) || cfg.end_day_of_up_skill < 0)
		{
			return -14;
		}

		int iRet = cfg.attr_cfg.ReadConfig(dataElement);
		if (iRet < 0)
		{
			return iRet;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int UpgradeConfig::InitSuperSpecialCapAddCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		SuperSpecialCapAddCfgItem cfg_item;
		if (!PugiGetSubNodeValue(dataElement, "image_id", cfg_item.image_id) || cfg_item.image_id <= 0 || cfg_item.image_id > UPGRADE_IMAGE_MAX_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "grade", cfg_item.grade) || cfg_item.grade <= 0 || cfg_item.grade > UPGRADE_MAX_IMAGE_GRADE)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg_item.maxhp) || cfg_item.maxhp < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg_item.gongji) || cfg_item.gongji < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg_item.fangyu) || cfg_item.fangyu < 0)
		{
			return -5;
		}

		// 判断重复
		if (m_super_special_cap_cfg.find(cfg_item.image_id) != m_super_special_cap_cfg.end())
		{
			return -6;
		}
		m_super_special_cap_cfg[cfg_item.image_id] = cfg_item;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const SkillConifg * UpgradeConfig::GetSkillCfg(int skill_idx, int level)
{
	if (skill_idx >= 0 && skill_idx < UPGRADE_SKILL_COUNT
		&& level > 0 && level <= UPGRADE_SKILL_MAX_LEVEL
		&& m_skill_cfg_list[skill_idx][level].skill_level > 0)
	{
		return &m_skill_cfg_list[skill_idx][level];
	}

	return nullptr;
}

const SuperSpecialCapAddCfgItem * UpgradeConfig::GetSuperSpecialCapAddCfg(int image_id) const
{
	if (image_id <= 0 || image_id > UPGRADE_IMAGE_MAX_COUNT)
	{
		return nullptr;
	}

	auto iter = m_super_special_cap_cfg.find(image_id);
	if (iter != m_super_special_cap_cfg.end())
	{
		return &(iter->second);
	}

	return nullptr;
}

bool UpgradeConfig::IsAddSpecialCap()
{
	return m_is_open_special_cap_add;
}

