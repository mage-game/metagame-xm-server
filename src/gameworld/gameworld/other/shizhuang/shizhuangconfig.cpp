#include "shizhuangconfig.h"
#include "item/itempool.h"

ShizhuangConfig::ShizhuangConfig():m_active_equip_skill_level(0), m_active_equip_grade(0), m_is_open_special_cap_add(0)
{
	memset(m_equip_skill_level_cfg,0,sizeof(m_equip_skill_level_cfg));
}

ShizhuangConfig::~ShizhuangConfig()
{

}

bool ShizhuangConfig::Init(std::string configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;;

	// 其他
	PUGI_XML_LOAD_CONFIG("other", InitOther);
	// 时装进阶
	PUGI_XML_LOAD_CONFIG("shizhuang_upgrade", InitShizhuangUpGradeCfg);
	// 时装进阶成功率
	PUGI_XML_LOAD_CONFIG("shizhuang_upgrade_prob", InitShizhuangUpGradeProbCfg);
	// 时装特殊形象
	PUGI_XML_LOAD_CONFIG("shizhuang_special_img", InitShizhuangSpecialImageCfg);
	// 时装特殊形象进阶
	PUGI_XML_LOAD_CONFIG("shizhuang_special_img_upgrade", InitShizhuangSpecialImageUpgradeCfg);
	// 时装装备信息
	PUGI_XML_LOAD_CONFIG("shizhuang_equip_info", InitShizhuangEquipInfoCfg);
	// 时装技能
	PUGI_XML_LOAD_CONFIG("shizhuang_skill", InitShizhuangSkillCfg);

	// 武器进阶成功率
	PUGI_XML_LOAD_CONFIG("weapon_upgrade", InitWeaponUpGradeCfg);
	// 武器进阶成功率
	PUGI_XML_LOAD_CONFIG("weapon_upgrade_prob", InitWeaponUpGradeProbCfg);
	// 武器特殊形象
	PUGI_XML_LOAD_CONFIG("weapon_special_img", InitWeaponSpecialImageCfg);
	// 武器特殊形象
	PUGI_XML_LOAD_CONFIG("weapon_special_img_upgrade", InitWeaponSpecialImageUpgradeCfg);
	// 武器装备信息
	PUGI_XML_LOAD_CONFIG("weapon_equip_info", InitWeaponEquipInfoCfg);
	// 武器技能
	PUGI_XML_LOAD_CONFIG("weapon_skill", InitWeaponSkillCfg);

	// 装备技能等级
	PUGI_XML_LOAD_CONFIG("equip_skill_level", InitEquipSkillLevelCfg);

	//神兵特殊战力加成
	PUGI_XML_LOAD_CONFIG("huanhua_special_cap_add", InitShizhuangWuqiHuanHuaSpecialCapAddCfg);

	//时装特殊战力加成
	PUGI_XML_LOAD_CONFIG("fashion_huanhua_special_cap_add", InitShizhuangHuanHuaSpecialCapAddCfg);

	return true;
}

const ShizhuangGradeConfig * ShizhuangConfig::GetShizhuangGradeConfig(int grade) const
{
	if (grade < 0 || grade >= SHIZHUANG_MAX_GRADE || m_shizhuang_grade_cfg_list[grade].grade <= 0)
	{
		return NULL;
	}

	return &m_shizhuang_grade_cfg_list[grade];
}

const ShizhuangUpGradeProbConfig * ShizhuangConfig::GetShizhuangUpGradeProbConfig(int grade, int bless_val) const
{
	std::map<int, ShizhuangUpGradeProbList>::const_iterator iter = m_shizhuang_upgrade_prob_map.find(grade);
	if (m_shizhuang_upgrade_prob_map.end() != iter)
	{
		const ShizhuangUpGradeProbList &cfg_list = iter->second;

		for (int i = 0; i < cfg_list.prob_count && i < ShizhuangUpGradeProbList::MAX_PROB_COUNT_PER_GRADE; ++i)
		{
			if (cfg_list.prob_list[i].low_bless_val <= bless_val && bless_val <= cfg_list.prob_list[i].high_bless_val)
			{
				return &cfg_list.prob_list[i];
			}
		}
	}

	return NULL;
}

const ShizhuangSpecialImgCfg * ShizhuangConfig::GetShizhuangSpecialImgCfg(int special_img_id)
{
	if (special_img_id > 0 && special_img_id < SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT)
	{
		if (m_shizhuang_special_img_cfglist[special_img_id].IsValid())
		{
			return &m_shizhuang_special_img_cfglist[special_img_id];
		}
	}

	return NULL;
}

const ShizhuangSpecialImgUpgradeCfg * ShizhuangConfig::GetShizhuangSpecialImgUpgradeCfg(int special_img_id, int grade)
{
	ShizhuangSpecialImgUpgradeCfgMap::iterator iter = m_shizhuang_special_img_upgrade_map.find(special_img_id);
	if (iter != m_shizhuang_special_img_upgrade_map.end())
	{
		ShizhuangSpecialImgUpgradeCfgList &cfg_list = iter->second;

		if (grade < 0 || grade > SHIZHUANG_SPECIAL_IMG_MAX_GRADE || grade > iter->second.max_grade)
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

const ShizhuangGradeConfig * ShizhuangConfig::GetWeaponGradeConfig(int grade) const
{
	if (grade < 0 || grade >= SHIZHUANG_MAX_GRADE || m_weapon_grade_cfg_list[grade].grade <= 0)
	{
		return NULL;
	}

	return &m_weapon_grade_cfg_list[grade];
}

const ShizhuangUpGradeProbConfig * ShizhuangConfig::GetWeaponUpGradeProbConfig(int grade, int bless_val) const
{
	std::map<int, ShizhuangUpGradeProbList>::const_iterator iter = m_weapon_upgrade_prob_map.find(grade);
	if (m_weapon_upgrade_prob_map.end() != iter)
	{
		const ShizhuangUpGradeProbList &cfg_list = iter->second;

		for (int i = 0; i < cfg_list.prob_count && i < ShizhuangUpGradeProbList::MAX_PROB_COUNT_PER_GRADE; ++i)
		{
			if (cfg_list.prob_list[i].low_bless_val <= bless_val && bless_val <= cfg_list.prob_list[i].high_bless_val)
			{
				return &cfg_list.prob_list[i];
			}
		}
	}

	return NULL;
}

const ShizhuangSpecialImgCfg * ShizhuangConfig::GetWeaponSpecialImgCfg(int special_img_id)
{
	if (special_img_id > 0 && special_img_id < SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT)
	{
		if (m_weapon_special_img_cfglist[special_img_id].IsValid())
		{
			return &m_weapon_special_img_cfglist[special_img_id];
		}
	}

	return NULL;
}

const ShizhuangSpecialImgUpgradeCfg * ShizhuangConfig::GetWeaponSpecialImgUpgradeCfg(int special_img_id, int grade)
{
	ShizhuangSpecialImgUpgradeCfgMap::iterator iter = m_weapon_special_img_upgrade_map.find(special_img_id);
	if (iter != m_weapon_special_img_upgrade_map.end())
	{
		ShizhuangSpecialImgUpgradeCfgList &cfg_list = iter->second;

		if (grade < 0 || grade > SHIZHUANG_SPECIAL_IMG_MAX_GRADE || grade > iter->second.max_grade)
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

const ShizhuangEquipInfoCfg * ShizhuangConfig::GetShizhuangEquipInfoCfg(int equip_idx)
{
	if (equip_idx >= 0 && equip_idx < EQUIP_COUNT)
	{
		return &m_shizhuang_equip_info_list[equip_idx];
	}
	return NULL;
}

const ShizhuangSkillCfg * ShizhuangConfig::GetShizhuangSkillCfg(int index, int skill_level)
{
	if (index >= 0 && index < MAX_SKILL_COUNT && skill_level >= 0 && skill_level <= SKILL_MAX_LEVEL && m_shizhuang_skill_cfg_list[index][skill_level].skill_level > 0)
	{
		return &m_shizhuang_skill_cfg_list[index][skill_level];
	}

	return NULL;
}

const ShizhuangEquipInfoCfg * ShizhuangConfig::GetWeaponEquipInfoCfg(int equip_idx)
{
	if (equip_idx >= 0 && equip_idx < EQUIP_COUNT)
	{
		return &m_weapon_equip_info_list[equip_idx];
	}

	return NULL;
}

const ShizhuangSkillCfg * ShizhuangConfig::GetWeaponSkillCfg(int index, int skill_level)
{
	if (index >= 0 && index < MAX_SKILL_COUNT && skill_level > 0 && skill_level <= SKILL_MAX_LEVEL && m_weapon_skill_cfg_list[index][skill_level].skill_level > 0)
	{
		return &m_weapon_skill_cfg_list[index][skill_level];
	}

	return NULL;
}

const ShizhuangGradeConfig * ShizhuangConfig::GetGradeConfigByType(int grade, int type) const
{
	switch (type)
	{
	case SHIZHUANG_TYPE_WUQI:
	{
		return this->GetWeaponGradeConfig(grade);
	}
	break;

	case SHIZHUANG_TYPE_BODY:
	{
		return this->GetShizhuangGradeConfig(grade);
	}
	break;

	default: return nullptr;
	}
}

int ShizhuangConfig::GetEquipSkillLevelCfg(int min_equip_level)
{
	if (min_equip_level >= m_active_equip_skill_level && min_equip_level < EQUIP_MAX_LEVEL)
	{
		return m_equip_skill_level_cfg[min_equip_level];
	}

	return -1;
}

const ShizhuangHuanHuaSpecialCapAddCfgItem* ShizhuangConfig::GetShizhuangHuanHuaSpecialCapAddCfg(int huanhua_id)const
{
	if (huanhua_id <= 0 || huanhua_id >= SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT)
	{
		return nullptr;
	}

	auto iter = m_shizhuang_huanhua_special_cap_cfg.find(huanhua_id);
	if (iter != m_shizhuang_huanhua_special_cap_cfg.end())
	{
		return &(iter->second);
	}

	return nullptr;
}

const ShizhuangHuanHuaSpecialCapAddCfgItem* ShizhuangConfig::GetShizhuangWuqiHuanHuaSpecialCapAddCfg(int huanhua_id)const
{
	if (huanhua_id <= 0 || huanhua_id >= SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT)
	{
		return nullptr;
	}

	auto iter = m_shizhuang_wuqi_huanhua_special_cap_cfg.find(huanhua_id);
	if (iter != m_shizhuang_wuqi_huanhua_special_cap_cfg.end())
	{
		return &(iter->second);
	}

	return nullptr;
}

bool ShizhuangConfig::IsAddSpecialCap()
{
	return m_is_open_special_cap_add;
}

//ShizhuangItemConfig * ShizhuangConfig::GetItemCfg(int part, int index)
//{
//	if (part >= 0 && part < SHIZHUANG_TYPE_MAX && index > 0 && index <= SHIZHUANG_MAX_IMG_ID)
//	{
//		if (m_item_cfglist[part][index].IsValid())
//		{
//			return &m_item_cfglist[part][index];
//		}
//	}
//
//	return NULL;
//}
//
//const ShizhaungUpgradeConfig * ShizhuangConfig::GetUpgradeCfg(int part_type, int index, int grade)
//{
//	UpGradeVec::const_iterator iter = m_upgrade_cfg_vec.begin();
//	while(iter != m_upgrade_cfg_vec.end())
//	{
//		if (iter->part_type == part_type && iter->index == index && iter->grade == grade)
//		{
//			return &(*iter);
//		}
//
//		++ iter;
//	}
//
//	return NULL;
//}

int ShizhuangConfig::InitOther(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (!dataElement.empty())
	{

		if (!PugiGetSubNodeValue(dataElement, "active_equip_grade", m_active_equip_grade) || m_active_equip_grade < 0 || m_active_equip_grade > ShizhuangParam::MAX_GRADE)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "active_equip_skill_level", m_active_equip_skill_level) || m_active_equip_skill_level < 0 || m_active_equip_skill_level > ShizhuangParam::EQUIP_MAX_LEVEL)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_open_special_cap_add", m_is_open_special_cap_add) || (m_is_open_special_cap_add != 0 && m_is_open_special_cap_add != 1))
		{
			return -6;
		}
	}

	return 0;
}

int ShizhuangConfig::InitShizhuangUpGradeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		short grade = 0;
		if (!PugiGetSubNodeValue(dataElement, "grade", grade) || grade < 0 || grade >= SHIZHUANG_MAX_GRADE)
		{
			return -1;
		}

		ShizhuangGradeConfig &cfg = m_shizhuang_grade_cfg_list[grade];
		cfg.grade = grade;

		if (!PugiGetSubNodeValue(dataElement, "client_grade", cfg.client_grade) || cfg.client_grade < 0 || cfg.client_grade >= SHIZHUANG_MAX_GRADE)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "upgrade_stuff_id", cfg.upgrade_stuff_id) || NULL == ITEMPOOL->GetItem(cfg.upgrade_stuff_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "upgrade_stuff2_id", cfg.upgrade_stuff2_id) || nullptr == ITEMPOOL->GetItem(cfg.upgrade_stuff2_id))
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "upgrade_stuff_count", cfg.upgrade_stuff_count) || cfg.upgrade_stuff_count <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "coin", cfg.coin) || cfg.coin < 0)
		{
			return -5;
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

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0) return -101;

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0) return -102;

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0) return -103;

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) return -104;

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0) return -105;

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0) return -106;

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0) return -107;

		if (!PugiGetSubNodeValue(dataElement, "movespeed", cfg.movespeed) || cfg.movespeed < 0) return -108;

		if (!PugiGetSubNodeValue(dataElement, "pack_num", cfg.pack_num) || cfg.pack_num <= 0) return -109;

		if (!PugiGetSubNodeValue(dataElement, "equip_level_toplimit", cfg.equip_level_toplimit) || cfg.equip_level_toplimit < 0) return -110;

		if (!PugiGetSubNodeValue(dataElement, "extra_zengshang", cfg.extra_zengshang) || cfg.extra_zengshang < 0) return -111;

		if (!PugiGetSubNodeValue(dataElement, "extra_mianshang", cfg.extra_mianshang) || cfg.extra_mianshang < 0) return -112;

		if (!PugiGetSubNodeValue(dataElement, "per_jingzhun", cfg.per_jingzhun) || cfg.per_jingzhun < 0) return -113;

		if (!PugiGetSubNodeValue(dataElement, "per_baoji", cfg.per_baoji) || cfg.per_baoji < 0) return -114;

		if (!PugiGetSubNodeValue(dataElement, "jianshang_per", cfg.per_mianshang) || cfg.per_mianshang < 0) return -115;

		if (!PugiGetSubNodeValue(dataElement, "zengshang_per", cfg.per_pofang) || cfg.per_pofang < 0) return -116;

		if (!PugiGetSubNodeValue(dataElement, "pvp_zengshang_per", cfg.pvp_zengshang_per) || cfg.pve_zengshang_per < 0) return -117;

		if (!PugiGetSubNodeValue(dataElement, "pve_zengshang_per", cfg.pve_zengshang_per) || cfg.pve_zengshang_per < 0) return -118;

		if (!PugiGetSubNodeValue(dataElement, "pvp_jianshang_per", cfg.pvp_jianshang_per) || cfg.pvp_jianshang_per < 0) return -119;

		if (!PugiGetSubNodeValue(dataElement, "pve_jianshang_per", cfg.pve_jianshang_per) || cfg.pve_jianshang_per < 0) return -120;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ShizhuangConfig::InitShizhuangUpGradeProbCfg(PugiXmlNode RootElement)
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

		ShizhuangUpGradeProbConfig cfg;

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

		if (!PugiGetSubNodeValue(dataElement, "fail_add_1_rate", cfg.fail_add_1_bless_rate) || cfg.fail_add_1_bless_rate < 0 || cfg.fail_add_1_bless_rate > 100)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "fail_add_2_rate", cfg.fail_add_2_bless_rate) || cfg.fail_add_2_bless_rate < 0 || cfg.fail_add_2_bless_rate > 100)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "fail_add_3_rate", cfg.fail_add_3_bless_rate) || cfg.fail_add_3_bless_rate < 0 || cfg.fail_add_3_bless_rate > 100)
		{
			return -8;
		}

		if (cfg.fail_add_1_bless_rate + cfg.fail_add_2_bless_rate + cfg.fail_add_3_bless_rate != 100)
		{
			return -9;
		}

		cfg.fail_add_2_bless_rate += cfg.fail_add_1_bless_rate;
		cfg.fail_add_3_bless_rate += cfg.fail_add_2_bless_rate;

		std::map<int, ShizhuangUpGradeProbList>::iterator iter = m_shizhuang_upgrade_prob_map.find(grade);
		if (m_shizhuang_upgrade_prob_map.end() != iter)
		{
			ShizhuangUpGradeProbList &cfg_list = iter->second;
			if (!cfg_list.Add(cfg))
			{
				return -50;
			}
		}
		else
		{
			ShizhuangUpGradeProbList cfg_list;
			cfg_list.grade = grade;
			cfg_list.Add(cfg);
			m_shizhuang_upgrade_prob_map[grade] = cfg_list;
		}

		last_grade = grade;

		dataElement = dataElement.next_sibling();
	}

	for (std::map<int, ShizhuangUpGradeProbList>::const_iterator iter = m_shizhuang_upgrade_prob_map.begin(); m_shizhuang_upgrade_prob_map.end() != iter; ++iter)
	{
		const ShizhuangUpGradeProbList &cfg_list = iter->second;

		if (cfg_list.prob_count <= 0 || cfg_list.prob_count > ShizhuangUpGradeProbList::MAX_PROB_COUNT_PER_GRADE)
		{
			return -60;
		}
		const ShizhuangGradeConfig *grade_cfg = this->GetShizhuangGradeConfig(cfg_list.grade);
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

int ShizhuangConfig::InitShizhuangSpecialImageCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int image_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "image_id", image_id) || image_id <= 0 || image_id >= SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT)
		{
			return -1;
		}

		ShizhuangSpecialImgCfg &cfg_item = m_shizhuang_special_img_cfglist[image_id];
		if (cfg_item.IsValid())
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "valid_time_s", cfg_item.valid_time_s) || cfg_item.valid_time_s < 0)
		{
			return -3;
		}
		
		if (!PugiGetSubNodeValue(dataElement, "item_id", cfg_item.active_item_id) || cfg_item.active_item_id < 0 || NULL == ITEMPOOL->GetItem(cfg_item.active_item_id))
		{
			return -4;
		}

		cfg_item.image_id = image_id;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ShizhuangConfig::InitShizhuangSpecialImageUpgradeCfg(PugiXmlNode RootElement)
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
		if (!PugiGetSubNodeValue(dataElement, "special_img_id", image_id) || image_id <= 0 || image_id >= SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT)
		{
			return -1;
		}

		if (image_id != last_image_id)
		{
			last_image_id = image_id;
			last_grade = -1;
		}

		ShizhuangSpecialImgUpgradeCfgList &cfg_list = m_shizhuang_special_img_upgrade_map[image_id];

		cfg_list.max_grade = 0;
		if (!PugiGetSubNodeValue(dataElement, "grade", cfg_list.max_grade) || cfg_list.max_grade < 0 || cfg_list.max_grade > SHIZHUANG_SPECIAL_IMG_MAX_GRADE || cfg_list.max_grade != last_grade + 1)
		{
			return -2;
		}

		last_grade = cfg_list.max_grade;

		ShizhuangSpecialImgUpgradeCfg &cfg_item = cfg_list.upgrade_list[cfg_list.max_grade];
		cfg_item.grade = cfg_list.max_grade;

		if (!PugiGetSubNodeValue(dataElement, "stuff_id", cfg_item.stuff_id) || NULL == ITEMPOOL->GetItem(cfg_item.stuff_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_num", cfg_item.stuff_num) || cfg_item.stuff_num <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "shuxingdan_count", cfg_item.shuxingdan_count) || cfg_item.shuxingdan_count < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "chengzhangdan_count", cfg_item.chengzhangdan_count) || cfg_item.chengzhangdan_count < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "equip_level", cfg_item.equip_level) || cfg_item.equip_level < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg_item.maxhp) || cfg_item.maxhp < 0) return -101;

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg_item.gongji) || cfg_item.gongji < 0) return -102;

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg_item.fangyu) || cfg_item.fangyu < 0) return -103;

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg_item.mingzhong) || cfg_item.mingzhong < 0) return -104;

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg_item.shanbi) || cfg_item.shanbi < 0) return -105;

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg_item.baoji) || cfg_item.baoji < 0) return -106;

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg_item.jianren) || cfg_item.jianren < 0) return -107;

		if (!PugiGetSubNodeValue(dataElement, "extra_zengshang", cfg_item.extra_zengshang) || cfg_item.extra_zengshang < 0) return -108;

		if (!PugiGetSubNodeValue(dataElement, "extra_mianshang", cfg_item.extra_mianshang) || cfg_item.extra_mianshang < 0) return -109;

		if (!PugiGetSubNodeValue(dataElement, "per_jingzhun", cfg_item.per_jingzhun) || cfg_item.per_jingzhun < 0) return -110;

		if (!PugiGetSubNodeValue(dataElement, "per_baoji", cfg_item.per_baoji) || cfg_item.per_baoji < 0) return -111;

		if (!PugiGetSubNodeValue(dataElement, "jianshang_per", cfg_item.per_mianshang) || cfg_item.per_mianshang < 0) return -112;

		if (!PugiGetSubNodeValue(dataElement, "zengshang_per", cfg_item.per_pofang) || cfg_item.per_pofang < 0) return -113;

		if (!PugiGetSubNodeValue(dataElement, "pvp_zengshang_per", cfg_item.pvp_zengshang_per) || cfg_item.pve_zengshang_per < 0) return -114;

		if (!PugiGetSubNodeValue(dataElement, "pve_zengshang_per", cfg_item.pve_zengshang_per) || cfg_item.pve_zengshang_per < 0) return -115;

		if (!PugiGetSubNodeValue(dataElement, "pvp_jianshang_per", cfg_item.pvp_jianshang_per) || cfg_item.pvp_jianshang_per < 0) return -116;

		if (!PugiGetSubNodeValue(dataElement, "pve_jianshang_per", cfg_item.pve_jianshang_per) || cfg_item.pve_jianshang_per < 0) return -117;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ShizhuangConfig::InitShizhuangEquipInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int equip_idx = 0;
		if (!PugiGetSubNodeValue(dataElement, "equip_idx", equip_idx) || equip_idx < 0 || equip_idx > EQUIP_COUNT)
		{
			return -1;
		}

		ShizhuangEquipInfoCfg &equip_cfg = m_shizhuang_equip_info_list[equip_idx];

		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "equip_level", level) || level < 0 || level > EQUIP_MAX_LEVEL)
		{
			return -2;
		}

		PugiXmlNode item_element = dataElement.child("item");
		if (NULL != item_element)
		{
			if (!equip_cfg.level_cfg[level].item.ReadConfig(item_element))
			{
				return -3;
			}
		}
		if (!PugiGetSubNodeValue(dataElement, "maxhp", equip_cfg.level_cfg[level].maxhp) || equip_cfg.level_cfg[level].maxhp < 0) return -4;

		if (!PugiGetSubNodeValue(dataElement, "gongji", equip_cfg.level_cfg[level].gongji) || equip_cfg.level_cfg[level].gongji < 0) return -5;

		if (!PugiGetSubNodeValue(dataElement, "fangyu", equip_cfg.level_cfg[level].fangyu) || equip_cfg.level_cfg[level].fangyu < 0) return -6;

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", equip_cfg.level_cfg[level].mingzhong) || equip_cfg.level_cfg[level].mingzhong < 0) return -7;

		if (!PugiGetSubNodeValue(dataElement, "shanbi", equip_cfg.level_cfg[level].shanbi) || equip_cfg.level_cfg[level].shanbi < 0) return -8;

		if (!PugiGetSubNodeValue(dataElement, "baoji", equip_cfg.level_cfg[level].baoji) || equip_cfg.level_cfg[level].baoji < 0) return -9;

		if (!PugiGetSubNodeValue(dataElement, "jianren", equip_cfg.level_cfg[level].jianren) || equip_cfg.level_cfg[level].jianren < 0) return -10;

		if (!PugiGetSubNodeValue(dataElement, "add_percent", equip_cfg.level_cfg[level].add_percent) || equip_cfg.level_cfg[level].add_percent < 0) return -11;

		equip_cfg.max_level++;

		dataElement = dataElement.next_sibling();
	}
	return 0;

}

int ShizhuangConfig::InitShizhuangSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int skill_idx = 0;
		if (!PugiGetSubNodeValue(dataElement, "skill_idx", skill_idx) || skill_idx < 0 || skill_idx >= MAX_SKILL_COUNT)
		{
			return -1;
		}

		int skill_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "skill_level", skill_level) || skill_level < 0)
		{
			return -2;
		}

		ShizhuangSkillCfg &cfg = m_shizhuang_skill_cfg_list[skill_idx][skill_level];
		cfg.index = skill_idx;
		cfg.skill_level = skill_level;

		if (!PugiGetSubNodeValue(dataElement, "skill_type", cfg.skill_type) || cfg.skill_type < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "grade", cfg.shizhuang_grade) || cfg.shizhuang_grade < 0)
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

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0) return -101;

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0) return -102;

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0) return -103;

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) return -104;

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0) return -105;

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0) return -106;

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0) return -107;

		if (!PugiGetSubNodeValue(dataElement, "capability", cfg.capability) || cfg.capability < 0) return -108;

		if (!PugiGetSubNodeValue(dataElement, "skill_cap_per", cfg.skill_cap_per) || cfg.skill_cap_per < 0) return -109;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}


int ShizhuangConfig::InitWeaponUpGradeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		short grade = 0;
		if (!PugiGetSubNodeValue(dataElement, "grade", grade) || grade < 0 || grade >= SHIZHUANG_MAX_GRADE)
		{
			return -1;
		}

		ShizhuangGradeConfig &cfg = m_weapon_grade_cfg_list[grade];
		cfg.grade = grade;

		if (!PugiGetSubNodeValue(dataElement, "client_grade", cfg.client_grade) || cfg.client_grade < 0 || cfg.client_grade >= SHIZHUANG_MAX_GRADE)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "upgrade_stuff_id", cfg.upgrade_stuff_id) || NULL == ITEMPOOL->GetItem(cfg.upgrade_stuff_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "upgrade_stuff2_id", cfg.upgrade_stuff2_id) || nullptr == ITEMPOOL->GetItem(cfg.upgrade_stuff2_id))
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "upgrade_stuff_count", cfg.upgrade_stuff_count) || cfg.upgrade_stuff_count <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "coin", cfg.coin) || cfg.coin < 0)
		{
			return -5;
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

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0) return -101;

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0) return -102;

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0) return -103;

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) return -104;

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0) return -105;

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0) return -106;

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0) return -107;

		if (!PugiGetSubNodeValue(dataElement, "movespeed", cfg.movespeed) || cfg.movespeed < 0) return -108;

		if (!PugiGetSubNodeValue(dataElement, "pack_num", cfg.pack_num) || cfg.pack_num <= 0) return -109;

		if (!PugiGetSubNodeValue(dataElement, "equip_level_toplimit", cfg.equip_level_toplimit) || cfg.equip_level_toplimit < 0) return -110;

		if (!PugiGetSubNodeValue(dataElement, "extra_zengshang", cfg.extra_zengshang) || cfg.extra_zengshang < 0) return -112;

		if (!PugiGetSubNodeValue(dataElement, "extra_mianshang", cfg.extra_mianshang) || cfg.extra_mianshang < 0) return -113;

		if (!PugiGetSubNodeValue(dataElement, "per_jingzhun", cfg.per_jingzhun) || cfg.per_jingzhun < 0) return -114;

		if (!PugiGetSubNodeValue(dataElement, "per_baoji", cfg.per_baoji) || cfg.per_baoji < 0) return -115;

		if (!PugiGetSubNodeValue(dataElement, "jianshang_per", cfg.per_mianshang) || cfg.per_mianshang < 0) return -116;

		if (!PugiGetSubNodeValue(dataElement, "zengshang_per", cfg.per_pofang) || cfg.per_pofang < 0) return -117;

		if (!PugiGetSubNodeValue(dataElement, "pvp_zengshang_per", cfg.pvp_zengshang_per) || cfg.pve_zengshang_per < 0) return -118;

		if (!PugiGetSubNodeValue(dataElement, "pve_zengshang_per", cfg.pve_zengshang_per) || cfg.pve_zengshang_per < 0) return -119;

		if (!PugiGetSubNodeValue(dataElement, "pvp_jianshang_per", cfg.pvp_jianshang_per) || cfg.pvp_jianshang_per < 0) return -120;

		if (!PugiGetSubNodeValue(dataElement, "pve_jianshang_per", cfg.pve_jianshang_per) || cfg.pve_jianshang_per < 0) return -121;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ShizhuangConfig::InitWeaponUpGradeProbCfg(PugiXmlNode RootElement)
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

		ShizhuangUpGradeProbConfig cfg;

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

		if (!PugiGetSubNodeValue(dataElement, "fail_add_1_rate", cfg.fail_add_1_bless_rate) || cfg.fail_add_1_bless_rate < 0 || cfg.fail_add_1_bless_rate > 100)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "fail_add_2_rate", cfg.fail_add_2_bless_rate) || cfg.fail_add_2_bless_rate < 0 || cfg.fail_add_2_bless_rate > 100)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "fail_add_3_rate", cfg.fail_add_3_bless_rate) || cfg.fail_add_3_bless_rate < 0 || cfg.fail_add_3_bless_rate > 100)
		{
			return -8;
		}

		if (cfg.fail_add_1_bless_rate + cfg.fail_add_2_bless_rate + cfg.fail_add_3_bless_rate != 100)
		{
			return -9;
		}

		cfg.fail_add_2_bless_rate += cfg.fail_add_1_bless_rate;
		cfg.fail_add_3_bless_rate += cfg.fail_add_2_bless_rate;

		std::map<int, ShizhuangUpGradeProbList>::iterator iter = m_weapon_upgrade_prob_map.find(grade);
		if (m_weapon_upgrade_prob_map.end() != iter)
		{
			ShizhuangUpGradeProbList &cfg_list = iter->second;
			if (!cfg_list.Add(cfg))
			{
				return -50;
			}
		}
		else
		{
			ShizhuangUpGradeProbList cfg_list;
			cfg_list.grade = grade;
			cfg_list.Add(cfg);
			m_weapon_upgrade_prob_map[grade] = cfg_list;
		}

		last_grade = grade;

		dataElement = dataElement.next_sibling();
	}

	for (std::map<int, ShizhuangUpGradeProbList>::const_iterator iter = m_weapon_upgrade_prob_map.begin(); m_weapon_upgrade_prob_map.end() != iter; ++iter)
	{
		const ShizhuangUpGradeProbList &cfg_list = iter->second;

		if (cfg_list.prob_count <= 0 || cfg_list.prob_count > ShizhuangUpGradeProbList::MAX_PROB_COUNT_PER_GRADE)
		{
			return -60;
		}
		const ShizhuangGradeConfig *grade_cfg = this->GetWeaponGradeConfig(cfg_list.grade);
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

int ShizhuangConfig::InitWeaponSpecialImageCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int image_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "image_id", image_id) || image_id <= 0 || image_id >= SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT)
		{
			return -1;
		}

		ShizhuangSpecialImgCfg &cfg_item = m_weapon_special_img_cfglist[image_id];
		if (cfg_item.IsValid())
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "valid_time_s", cfg_item.valid_time_s) || cfg_item.valid_time_s < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "item_id", cfg_item.active_item_id) || cfg_item.active_item_id < 0 || NULL == ITEMPOOL->GetItem(cfg_item.active_item_id))
		{
			return -4;
		}

		cfg_item.image_id = image_id;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ShizhuangConfig::InitWeaponSpecialImageUpgradeCfg(PugiXmlNode RootElement)
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
		if (!PugiGetSubNodeValue(dataElement, "special_img_id", image_id) || image_id <= 0 || image_id >= SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT)
		{
			return -1;
		}

		if (image_id != last_image_id)
		{
			last_image_id = image_id;
			last_grade = -1;
		}

		ShizhuangSpecialImgUpgradeCfgList &cfg_list = m_weapon_special_img_upgrade_map[image_id];

		cfg_list.max_grade = 0;
		if (!PugiGetSubNodeValue(dataElement, "grade", cfg_list.max_grade) || cfg_list.max_grade < 0 || cfg_list.max_grade > SHIZHUANG_SPECIAL_IMG_MAX_GRADE || cfg_list.max_grade != last_grade + 1)
		{
			return -2;
		}

		last_grade = cfg_list.max_grade;

		ShizhuangSpecialImgUpgradeCfg &cfg_item = cfg_list.upgrade_list[cfg_list.max_grade];
		cfg_item.grade = cfg_list.max_grade;

		if (!PugiGetSubNodeValue(dataElement, "stuff_id", cfg_item.stuff_id) || NULL == ITEMPOOL->GetItem(cfg_item.stuff_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_num", cfg_item.stuff_num) || cfg_item.stuff_num <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "shuxingdan_count", cfg_item.shuxingdan_count) || cfg_item.shuxingdan_count < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "chengzhangdan_count", cfg_item.chengzhangdan_count) || cfg_item.chengzhangdan_count < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "equip_level", cfg_item.equip_level) || cfg_item.equip_level < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg_item.maxhp) || cfg_item.maxhp < 0) return -101;

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg_item.gongji) || cfg_item.gongji < 0) return -102;

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg_item.fangyu) || cfg_item.fangyu < 0) return -103;

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg_item.mingzhong) || cfg_item.mingzhong < 0) return -104;

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg_item.shanbi) || cfg_item.shanbi < 0) return -105;

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg_item.baoji) || cfg_item.baoji < 0) return -106;

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg_item.jianren) || cfg_item.jianren < 0) return -107;

		if (!PugiGetSubNodeValue(dataElement, "extra_zengshang", cfg_item.extra_zengshang) || cfg_item.extra_zengshang < 0) return -108;

		if (!PugiGetSubNodeValue(dataElement, "extra_mianshang", cfg_item.extra_mianshang) || cfg_item.extra_mianshang < 0) return -109;

		if (!PugiGetSubNodeValue(dataElement, "per_jingzhun", cfg_item.per_jingzhun) || cfg_item.per_jingzhun < 0) return -110;

		if (!PugiGetSubNodeValue(dataElement, "per_baoji", cfg_item.per_baoji) || cfg_item.per_baoji < 0) return -111;

		if (!PugiGetSubNodeValue(dataElement, "jianshang_per", cfg_item.per_mianshang) || cfg_item.per_mianshang < 0) return -112;

		if (!PugiGetSubNodeValue(dataElement, "zengshang_per", cfg_item.per_pofang) || cfg_item.per_pofang < 0) return -113;

		if (!PugiGetSubNodeValue(dataElement, "pvp_zengshang_per", cfg_item.pvp_zengshang_per) || cfg_item.pve_zengshang_per < 0) return -114;

		if (!PugiGetSubNodeValue(dataElement, "pve_zengshang_per", cfg_item.pve_zengshang_per) || cfg_item.pve_zengshang_per < 0) return -115;

		if (!PugiGetSubNodeValue(dataElement, "pvp_jianshang_per", cfg_item.pvp_jianshang_per) || cfg_item.pvp_jianshang_per < 0) return -116;

		if (!PugiGetSubNodeValue(dataElement, "pve_jianshang_per", cfg_item.pve_jianshang_per) || cfg_item.pve_jianshang_per < 0) return -117;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ShizhuangConfig::InitWeaponEquipInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int equip_idx = 0;
		if (!PugiGetSubNodeValue(dataElement, "equip_idx", equip_idx) || equip_idx < 0 || equip_idx > EQUIP_COUNT)
		{
			return -1;
		}

		ShizhuangEquipInfoCfg &equip_cfg = m_weapon_equip_info_list[equip_idx];

		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "equip_level", level) || level < 0 || level > EQUIP_MAX_LEVEL)
		{
			return -2;
		}

		PugiXmlNode item_element = dataElement.child("item");
		if (NULL != item_element)
		{
			if (!equip_cfg.level_cfg[level].item.ReadConfig(item_element))
			{
				return -3;
			}
		}
		if (!PugiGetSubNodeValue(dataElement, "maxhp", equip_cfg.level_cfg[level].maxhp) || equip_cfg.level_cfg[level].maxhp < 0) return -4;

		if (!PugiGetSubNodeValue(dataElement, "gongji", equip_cfg.level_cfg[level].gongji) || equip_cfg.level_cfg[level].gongji < 0) return -5;

		if (!PugiGetSubNodeValue(dataElement, "fangyu", equip_cfg.level_cfg[level].fangyu) || equip_cfg.level_cfg[level].fangyu < 0) return -6;

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", equip_cfg.level_cfg[level].mingzhong) || equip_cfg.level_cfg[level].mingzhong < 0) return -7;

		if (!PugiGetSubNodeValue(dataElement, "shanbi", equip_cfg.level_cfg[level].shanbi) || equip_cfg.level_cfg[level].shanbi < 0) return -8;

		if (!PugiGetSubNodeValue(dataElement, "baoji", equip_cfg.level_cfg[level].baoji) || equip_cfg.level_cfg[level].baoji < 0) return -9;

		if (!PugiGetSubNodeValue(dataElement, "jianren", equip_cfg.level_cfg[level].jianren) || equip_cfg.level_cfg[level].jianren < 0) return -10;

		if (!PugiGetSubNodeValue(dataElement, "add_percent", equip_cfg.level_cfg[level].add_percent) || equip_cfg.level_cfg[level].add_percent < 0) return -11;

		equip_cfg.max_level++;

		dataElement = dataElement.next_sibling();
	}
	return 0;

}

int ShizhuangConfig::InitWeaponSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int skill_idx = 0;
		if (!PugiGetSubNodeValue(dataElement, "skill_idx", skill_idx) || skill_idx < 0 || skill_idx >= MAX_SKILL_COUNT)
		{
			return -1;
		}

		int skill_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "skill_level", skill_level) || skill_level < 0)
		{
			return -2;
		}

		ShizhuangSkillCfg &cfg = m_weapon_skill_cfg_list[skill_idx][skill_level];
		cfg.index = skill_idx;
		cfg.skill_level = skill_level;

		if (!PugiGetSubNodeValue(dataElement, "skill_type", cfg.skill_type) || cfg.skill_type < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "grade", cfg.shizhuang_grade) || cfg.shizhuang_grade < 0)
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

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0) return -101;

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0) return -102;

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0) return -103;

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) return -104;

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0) return -105;

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0) return -106;

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0) return -107;

		if (!PugiGetSubNodeValue(dataElement, "capability", cfg.capability) || cfg.capability < 0) return -108;

		if (!PugiGetSubNodeValue(dataElement, "skill_cap_per", cfg.skill_cap_per) || cfg.skill_cap_per < 0) return -109;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ShizhuangConfig::InitEquipSkillLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int equip_min_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "equip_min_level", equip_min_level) || equip_min_level < 0 || equip_min_level >= EQUIP_MAX_LEVEL)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "equip_skill_level", m_equip_skill_level_cfg[equip_min_level]) || m_equip_skill_level_cfg[equip_min_level] < 0)
		{
			return -2;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ShizhuangConfig::InitShizhuangHuanHuaSpecialCapAddCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		ShizhuangHuanHuaSpecialCapAddCfgItem cfg_item;
		if (!PugiGetSubNodeValue(dataElement, "huanhua_id", cfg_item.huanhua_id) || cfg_item.huanhua_id <= 0 || cfg_item.huanhua_id >= SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "huanhua_level", cfg_item.huanhua_level) || cfg_item.huanhua_level <= 0 || cfg_item.huanhua_level >= SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT)
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
		if (m_shizhuang_huanhua_special_cap_cfg.find(cfg_item.huanhua_id) != m_shizhuang_huanhua_special_cap_cfg.end())
		{
			return -6;
		}
		m_shizhuang_huanhua_special_cap_cfg[cfg_item.huanhua_id] = cfg_item;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ShizhuangConfig::InitShizhuangWuqiHuanHuaSpecialCapAddCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		ShizhuangHuanHuaSpecialCapAddCfgItem cfg_item;
		if (!PugiGetSubNodeValue(dataElement, "huanhua_id", cfg_item.huanhua_id) || cfg_item.huanhua_id <= 0 || cfg_item.huanhua_id >= SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "huanhua_level", cfg_item.huanhua_level) || cfg_item.huanhua_level <= 0 || cfg_item.huanhua_level >= SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT)
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
		if (m_shizhuang_wuqi_huanhua_special_cap_cfg.find(cfg_item.huanhua_id) != m_shizhuang_wuqi_huanhua_special_cap_cfg.end())
		{
			return -6;
		}
		m_shizhuang_wuqi_huanhua_special_cap_cfg[cfg_item.huanhua_id] = cfg_item;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}