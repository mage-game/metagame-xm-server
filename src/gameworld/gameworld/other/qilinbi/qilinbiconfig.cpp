#include "qilinbiconfig.hpp"
#include "servercommon/servercommon.h"
#include "globalconfig/equipconfig.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "gameworld/checkresourcecenter.hpp"
#include <string>

QiLinBiConfig::QiLinBiConfig() : m_use_jinjie_card_reward_itemid(0), m_chengzhangdan_percent(0), m_temporary_image_time(0),
m_active_equip_grade(0), m_active_equip_skill_level(0), m_is_open_special_cap_add(false), m_max_star_level(0)
{
	memset(m_equip_skill_level_cfg, 0, sizeof(m_equip_skill_level_cfg));
}

QiLinBiConfig::~QiLinBiConfig()
{
}

bool QiLinBiConfig::Init(const std::string &configname, std::string *err)
{
	PRE_LOAD_CONFIG;

	LOAD_CONFIG("other", InitOtherCfg);

	LOAD_CONFIG("level", InitLevelCfg);

	LOAD_CONFIG("grade", InitGradeCfg);

	LOAD_CONFIG("upgrade_prob", InitUpGradeProbCfg);

	LOAD_CONFIG("image_list", InitImageCfg);

	LOAD_CONFIG("special_img", InitSpecialImageCfg);

	LOAD_CONFIG("special_image_upgrade", InitSpecialImageUpgradeCfg);

	LOAD_CONFIG("up_start_stuff", InitUpStarStuffCfg);

	LOAD_CONFIG("up_start_exp", InitUpStarExpCfg);

	LOAD_CONFIG("qilinbi_equip_info", InitEquipCfg);

	LOAD_CONFIG("qilinbi_skill", InitSkillCfg);

	//LOAD_CONFIG("equip_skill_level", InitEquipSkillLevel);

	LOAD_CONFIG("huanhua_special_cap_add", InitHuanHuaSpecialCapAddCfg);

	return true;
}

int QiLinBiConfig::InitOtherCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");

	if (nullptr == data_element)
	{
		return -100000;
	}

	if (nullptr != data_element)
	{
		if (!GetSubNodeValue(data_element, "qilinbi_grade_reward_id", m_use_jinjie_card_reward_itemid) || nullptr == ITEMPOOL->GetItem(m_use_jinjie_card_reward_itemid))
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "chengzhangdan_percent", m_chengzhangdan_percent) || m_chengzhangdan_percent <= 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "temporary_image_time", m_temporary_image_time) || m_temporary_image_time < 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "active_equip_grade", m_active_equip_grade) || !IsValidGrade(m_active_equip_grade))
		{
			return -4;
		}

		if (!GetSubNodeValue(data_element, "active_equip_skill_level", m_active_equip_skill_level) || !IsValidLevel(m_active_equip_skill_level))
		{
			return -5;
		}

		if (!GetSubNodeValue(data_element, "extra_attrs_per_grade", m_other_cfg.extra_attrs_per_grade) || m_other_cfg.extra_attrs_per_grade <= 0)
		{
			return -6;
		}

		if (!GetSubNodeValue(data_element, "extra_attrs_per", m_other_cfg.extra_attrs_per) || m_other_cfg.extra_attrs_per < 0)
		{
			return -7;
		}

		if (!GetSubNodeValue(data_element, "is_open_special_cap_add", m_is_open_special_cap_add))
		{
			return -8;
		}
	}

	return 0;
}

int QiLinBiConfig::InitLevelCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");

	if (nullptr == data_element)
	{
		return -100000;
	}

	int last_level = 0;
	while (nullptr != data_element)
	{
		QiLinBiLevelConfig cfg;

		if (!GetSubNodeValue(data_element, "qilinbi_level", cfg.qilinbi_level) || !IsValidLevel(cfg.qilinbi_level) || last_level + 1 != cfg.qilinbi_level)
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0) return -101;

		if (!GetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0) return -102;

		if (!GetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0) return -103;

		if (!GetSubNodeValue(data_element, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) return -104;

		if (!GetSubNodeValue(data_element, "shanbi", cfg.shanbi) || cfg.shanbi < 0) return -105;

		if (!GetSubNodeValue(data_element, "baoji", cfg.baoji) || cfg.baoji < 0) return -106;

		if (!GetSubNodeValue(data_element, "jianren", cfg.jianren) || cfg.jianren < 0) return -107;

		m_level_list[cfg.qilinbi_level] = cfg;

		last_level = cfg.qilinbi_level;

		data_element = data_element->NextSiblingElement();
	}

	if (last_level != QiLinBiParam::MAX_LEVEL)
	{
		return -2;
	}

	return 0;
}

int QiLinBiConfig::InitGradeCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (nullptr == data_element)
	{
		return -100000;
	}

	while (nullptr != data_element)
	{
		short grade = 0;
		if (!GetSubNodeValue(data_element, "grade", grade) || !IsValidGrade(grade))
		{
			return -1;
		}

		QiLinBiGradeConfig &cfg = m_grade_cfg_list[grade];
		cfg.grade = grade;

		if (!GetSubNodeValue(data_element, "client_grade", cfg.client_grade) || !IsValidClientGrade(cfg.client_grade))
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "upgrade_stuff_id", cfg.upgrade_stuff_id) || nullptr == ITEMPOOL->GetItem(cfg.upgrade_stuff_id))
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "upgrade_stuff2_id", cfg.upgrade_stuff2_id) || nullptr == ITEMPOOL->GetItem(cfg.upgrade_stuff2_id))
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "upgrade_stuff_count", cfg.upgrade_stuff_count) || cfg.upgrade_stuff_count <= 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(data_element, "coin", cfg.coin) || cfg.coin < 0)
		{
			return -5;
		}

		if (!GetSubNodeValue(data_element, "image_id", cfg.active_image_id) || cfg.active_image_id <= 0)
		{
			return -6;
		}

		if (!GetSubNodeValue(data_element, "bless_val_limit", cfg.bless_val_limit) || cfg.bless_val_limit <= 0)
		{
			return -7;
		}

		if (!GetSubNodeValue(data_element, "broadcast", cfg.is_broadcast))
		{
			return -8;
		}

		if (!GetSubNodeValue(data_element, "shuxingdan_limit", cfg.shuxingdan_limit) || cfg.shuxingdan_limit < 0)
		{
			return -9;
		}

		if (!GetSubNodeValue(data_element, "chengzhangdan_limit", cfg.chengzhangdan_limit) || cfg.chengzhangdan_limit < 0)
		{
			return -10;
		}

		if (!GetSubNodeValue(data_element, "is_clear_bless", cfg.is_clear_bless) || cfg.is_clear_bless < 0)
		{
			return -11;
		}

		if (!GetSubNodeValue(data_element, "bless_addition", cfg.bless_addition) || cfg.bless_addition < 0)
		{
			return -12;
		}

		if (!GetSubNodeValue(data_element, "equip_level_limit", cfg.equip_level_limit) || cfg.equip_level_limit < 0)
		{
			return -13;
		}

		if (!GetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0) return -101;

		if (!GetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0) return -102;

		if (!GetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0) return -103;

		if (!GetSubNodeValue(data_element, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) return -104;

		if (!GetSubNodeValue(data_element, "shanbi", cfg.shanbi) || cfg.shanbi < 0) return -105;

		if (!GetSubNodeValue(data_element, "baoji", cfg.baoji) || cfg.baoji < 0) return -106;

		if (!GetSubNodeValue(data_element, "jianren", cfg.jianren) || cfg.jianren < 0) return -107;

		if (!GetSubNodeValue(data_element, "movespeed", cfg.movespeed) || cfg.movespeed < 0) return -108;

		if (!GetSubNodeValue(data_element, "pack_num", cfg.pack_num) || cfg.pack_num <= 0) return -109;

		if (!GetSubNodeValue(data_element, "extra_zengshang", cfg.extra_zengshang) || cfg.extra_zengshang < 0) return -110;

		if (!GetSubNodeValue(data_element, "extra_mianshang", cfg.extra_mianshang) || cfg.extra_mianshang < 0) return -111;

		if (!GetSubNodeValue(data_element, "per_jingzhun", cfg.per_jingzhun) || cfg.per_jingzhun < 0) return -111;

		if (!GetSubNodeValue(data_element, "per_baoji", cfg.per_baoji) || cfg.per_baoji < 0) return -112;

		if (!GetSubNodeValue(data_element, "jianshang_per", cfg.per_mianshang) || cfg.per_mianshang < 0) return -113;

		if (!GetSubNodeValue(data_element, "zengshang_per", cfg.per_pofang) || cfg.per_pofang < 0) return -114;

		if (!GetSubNodeValue(data_element, "pvp_zengshang_per", cfg.pvp_zengshang_per) || cfg.pve_zengshang_per < 0) return -115;

		if (!GetSubNodeValue(data_element, "pve_zengshang_per", cfg.pve_zengshang_per) || cfg.pve_zengshang_per < 0) return -116;

		if (!GetSubNodeValue(data_element, "pvp_jianshang_per", cfg.pvp_jianshang_per) || cfg.pvp_jianshang_per < 0) return -117;

		if (!GetSubNodeValue(data_element, "pve_jianshang_per", cfg.pve_jianshang_per) || cfg.pve_jianshang_per < 0) return -118;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int QiLinBiConfig::InitUpGradeProbCfg(TiXmlElement *RootElement)
{
	short last_high = -1;
	short last_grade = 0;

	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	while (nullptr != data_element)
	{
		int grade = 0;
		if (!GetSubNodeValue(data_element, "grade", grade) || !IsValidGrade(grade))
		{
			return -1;
		}

		if (grade != last_grade) last_high = -1;

		QiLinBiUpGradeProbConfig cfg;

		if (!GetSubNodeValue(data_element, "low_bless", cfg.low_bless_val) || cfg.low_bless_val < 0)
		{
			return -3;
		}

		if (cfg.low_bless_val != last_high + 1)
		{
			return -20;
		}

		if (!GetSubNodeValue(data_element, "high_bless", cfg.high_bless_val) || cfg.high_bless_val < cfg.low_bless_val)
		{
			return -4;
		}

		last_high = cfg.high_bless_val;

		if (!GetSubNodeValue(data_element, "succ_rate", cfg.succ_rate) || cfg.succ_rate < 0 || cfg.succ_rate > 10000)
		{
			return -5;
		}

		if (!GetSubNodeValue(data_element, "fail_add_1_rate", cfg.fail_add_1_bless_rate) || cfg.fail_add_1_bless_rate < 0 || cfg.fail_add_1_bless_rate > 100)
		{
			return -6;
		}

		if (!GetSubNodeValue(data_element, "fail_add_2_rate", cfg.fail_add_2_bless_rate) || cfg.fail_add_2_bless_rate < 0 || cfg.fail_add_2_bless_rate > 100)
		{
			return -7;
		}

		if (!GetSubNodeValue(data_element, "fail_add_3_rate", cfg.fail_add_3_bless_rate) || cfg.fail_add_3_bless_rate < 0 || cfg.fail_add_3_bless_rate > 100)
		{
			return -8;
		}

		if (cfg.fail_add_1_bless_rate + cfg.fail_add_2_bless_rate + cfg.fail_add_3_bless_rate != 100)
		{
			return -9;
		}

		cfg.fail_add_2_bless_rate += cfg.fail_add_1_bless_rate;
		cfg.fail_add_3_bless_rate += cfg.fail_add_2_bless_rate;

		std::map<int, QiLinBiUpGradeProbList>::iterator iter = m_upgrade_prob_map.find(grade);
		if (m_upgrade_prob_map.end() != iter)
		{
			QiLinBiUpGradeProbList &cfg_list = iter->second;
			if (!cfg_list.Add(cfg))
			{
				return -50;
			}
		}
		else
		{
			QiLinBiUpGradeProbList cfg_list;
			cfg_list.grade = grade;
			cfg_list.Add(cfg);
			m_upgrade_prob_map[grade] = cfg_list;
		}

		last_grade = grade;

		data_element = data_element->NextSiblingElement();
	}

	for (std::map<int, QiLinBiUpGradeProbList>::const_iterator iter = m_upgrade_prob_map.begin(); m_upgrade_prob_map.end() != iter; ++iter)
	{
		const QiLinBiUpGradeProbList &cfg_list = iter->second;

		if (cfg_list.prob_count <= 0 || cfg_list.prob_count > QiLinBiUpGradeProbList::MAX_PROB_COUNT_PER_GRADE)
		{
			return -60;
		}
		const QiLinBiGradeConfig *grade_cfg = this->GetGradeConfig(cfg_list.grade);
		if (nullptr == grade_cfg)
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

int QiLinBiConfig::InitImageCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	while (nullptr != data_element)
	{
		int image_id = 0;
		if (!GetSubNodeValue(data_element, "image_id", image_id) || image_id < 0)
		{
			return -1;
		}

		QiLinBiImageConfig cfg;

		cfg.image_id = image_id;

		m_image_list[cfg.image_id] = cfg;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int QiLinBiConfig::InitSpecialImageCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (nullptr == data_element)
	{
		return -10000;
	}

	while (nullptr != data_element)
	{
		int image_id = 0;
		if (!GetSubNodeValue(data_element, "image_id", image_id) || !IsValidSpecialImageID(image_id))
		{
			return -1;
		}

		QiLinBiSpecialImgCfg &cfg_item = m_special_img_cfglist[image_id];
		if (cfg_item.IsValid())
		{
			return -2;
		}

		cfg_item.image_id = image_id;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int QiLinBiConfig::InitSpecialImageUpgradeCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (nullptr == data_element)
	{
		return -10000;
	}

	int last_image_id = 0;
	int last_grade = -1;

	while (nullptr != data_element)
	{
		int image_id = 0;
		if (!GetSubNodeValue(data_element, "special_img_id", image_id) || !IsValidSpecialImageID(image_id))
		{
			return -1;
		}

		if (image_id != last_image_id)
		{
			last_image_id = image_id;
			last_grade = -1;
		}

		QiLinBiSpecialImgUpgradeCfgList &cfg_list = m_special_img_upgrade_map[image_id];

		cfg_list.max_grade = 0;
		if (!GetSubNodeValue(data_element, "grade", cfg_list.max_grade) || !IsValidSpecialImageGrade(cfg_list.max_grade) || cfg_list.max_grade != last_grade + 1)
		{
			return -2;
		}

		last_grade = cfg_list.max_grade;

		QiLinBiSpecialImgUpgradeCfg &cfg_item = cfg_list.upgrade_list[cfg_list.max_grade];
		cfg_item.grade = cfg_list.max_grade;

		if (!GetSubNodeValue(data_element, "stuff_id", cfg_item.stuff_id) || nullptr == ITEMPOOL->GetItem(cfg_item.stuff_id))
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "stuff_num", cfg_item.stuff_num) || cfg_item.stuff_num <= 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "shuxingdan_count", cfg_item.shuxingdan_count) || cfg_item.shuxingdan_count < 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "chengzhangdan_count", cfg_item.chengzhangdan_count) || cfg_item.chengzhangdan_count < 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(data_element, "equip_level", cfg_item.equip_level) || cfg_item.equip_level < 0)
		{
			return -5;
		}

		if (!GetSubNodeValue(data_element, "maxhp", cfg_item.maxhp) || cfg_item.maxhp < 0) return -101;

		if (!GetSubNodeValue(data_element, "gongji", cfg_item.gongji) || cfg_item.gongji < 0) return -102;

		if (!GetSubNodeValue(data_element, "fangyu", cfg_item.fangyu) || cfg_item.fangyu < 0) return -103;

		if (!GetSubNodeValue(data_element, "mingzhong", cfg_item.mingzhong) || cfg_item.mingzhong < 0) return -104;

		if (!GetSubNodeValue(data_element, "shanbi", cfg_item.shanbi) || cfg_item.shanbi < 0) return -105;

		if (!GetSubNodeValue(data_element, "baoji", cfg_item.baoji) || cfg_item.baoji < 0) return -106;

		if (!GetSubNodeValue(data_element, "jianren", cfg_item.jianren) || cfg_item.jianren < 0) return -107;

		if (!GetSubNodeValue(data_element, "extra_zengshang", cfg_item.extra_zengshang) || cfg_item.extra_zengshang < 0) return -108;

		if (!GetSubNodeValue(data_element, "extra_mianshang", cfg_item.extra_mianshang) || cfg_item.extra_mianshang < 0) return -109;

		if (!GetSubNodeValue(data_element, "per_jingzhun", cfg_item.per_jingzhun) || cfg_item.per_jingzhun < 0) return -110;

		if (!GetSubNodeValue(data_element, "per_baoji", cfg_item.per_baoji) || cfg_item.per_baoji < 0) return -111;

		if (!GetSubNodeValue(data_element, "jianshang_per", cfg_item.per_mianshang) || cfg_item.per_mianshang < 0) return -112;

		if (!GetSubNodeValue(data_element, "zengshang_per", cfg_item.per_pofang) || cfg_item.per_pofang < 0) return -113;

		if (!GetSubNodeValue(data_element, "pvp_zengshang_per", cfg_item.pvp_zengshang_per) || cfg_item.pve_zengshang_per < 0) return -114;

		if (!GetSubNodeValue(data_element, "pve_zengshang_per", cfg_item.pve_zengshang_per) || cfg_item.pve_zengshang_per < 0) return -115;

		if (!GetSubNodeValue(data_element, "pvp_jianshang_per", cfg_item.pvp_jianshang_per) || cfg_item.pvp_jianshang_per < 0) return -116;

		if (!GetSubNodeValue(data_element, "pve_jianshang_per", cfg_item.pve_jianshang_per) || cfg_item.pve_jianshang_per < 0) return -117;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int QiLinBiConfig::InitEquipCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (nullptr == data_element)
	{
		return -10000;
	}

	while (nullptr != data_element)
	{
		int equip_idx = 0;
		if (!GetSubNodeValue(data_element, "equip_idx", equip_idx) || !IsValidEquipIdx(equip_idx))
		{
			return -1;
		}

		QiLinBiEquipConifg &equip_cfg = m_equip_cfg_list[equip_idx];

		int level = 0;

		if (!GetSubNodeValue(data_element, "equip_level", level) || !IsValidEquipLevel(level))
		{
			return -2;
		}

		TiXmlElement *item_element = data_element->FirstChildElement("item");
		if (nullptr != item_element)
		{
			if (!equip_cfg.level_cfg[level].item.ReadConfig(item_element))
			{
				return -3;
			}
		}

		if (!GetSubNodeValue(data_element, "maxhp", equip_cfg.level_cfg[level].maxhp) || equip_cfg.level_cfg[level].maxhp < 0) return -101;

		if (!GetSubNodeValue(data_element, "gongji", equip_cfg.level_cfg[level].gongji) || equip_cfg.level_cfg[level].gongji < 0) return -102;

		if (!GetSubNodeValue(data_element, "fangyu", equip_cfg.level_cfg[level].fangyu) || equip_cfg.level_cfg[level].fangyu < 0) return -103;

		if (!GetSubNodeValue(data_element, "mingzhong", equip_cfg.level_cfg[level].mingzhong) || equip_cfg.level_cfg[level].mingzhong < 0) return -104;

		if (!GetSubNodeValue(data_element, "shanbi", equip_cfg.level_cfg[level].shanbi) || equip_cfg.level_cfg[level].shanbi < 0) return -105;

		if (!GetSubNodeValue(data_element, "baoji", equip_cfg.level_cfg[level].baoji) || equip_cfg.level_cfg[level].baoji < 0) return -106;

		if (!GetSubNodeValue(data_element, "jianren", equip_cfg.level_cfg[level].jianren) || equip_cfg.level_cfg[level].jianren < 0) return -107;

		if (!GetSubNodeValue(data_element, "add_percent", equip_cfg.level_cfg[level].add_percent) || equip_cfg.level_cfg[level].add_percent < 0) return -108;

		equip_cfg.max_level++;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int QiLinBiConfig::InitSkillCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (nullptr == data_element)
	{
		return -10000;
	}

	while (nullptr != data_element)
	{
		int skill_idx = 0;
		if (!GetSubNodeValue(data_element, "skill_idx", skill_idx) || !IsValidSkillIdx(skill_idx))
		{
			return -1;
		}

		int skill_level = 0;
		if (!GetSubNodeValue(data_element, "skill_level", skill_level) || !IsValidSkillLevel(skill_level))
		{
			return -2;
		}

		QiLinBiSkillConifg &cfg = m_skill_cfg_list[skill_idx][skill_level];
		cfg.index = skill_idx;
		cfg.skill_level = skill_level;

		if (!GetSubNodeValue(data_element, "skill_type", cfg.skill_type) || cfg.skill_type < 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "grade", cfg.qilinbi_grade) || cfg.qilinbi_grade < 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(data_element, "uplevel_stuff_id", cfg.uplevel_stuff_id) || (cfg.skill_type > 0 && nullptr == ITEMPOOL->GetItem(cfg.uplevel_stuff_id)))
		{
			return -5;
		}

		if (!GetSubNodeValue(data_element, "uplevel_stuff_num", cfg.uplevel_stuff_num) || cfg.uplevel_stuff_num < 0)
		{
			return -6;
		}

		if (!GetSubNodeValue(data_element, "param_a", cfg.param_a) || cfg.param_a < 0)
		{
			return -7;
		}

		if (!GetSubNodeValue(data_element, "param_b", cfg.param_b) || cfg.param_b < 0)
		{
			return -8;
		}

		if (!GetSubNodeValue(data_element, "param_c", cfg.param_c) || cfg.param_c < 0)
		{
			return -9;
		}

		if (!GetSubNodeValue(data_element, "param_d", cfg.param_d) || cfg.param_d < 0)
		{
			return -10;
		}

		if (!GetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0) return -101;

		if (!GetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0) return -102;

		if (!GetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0) return -103;

		if (!GetSubNodeValue(data_element, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) return -104;

		if (!GetSubNodeValue(data_element, "shanbi", cfg.shanbi) || cfg.shanbi < 0) return -105;

		if (!GetSubNodeValue(data_element, "baoji", cfg.baoji) || cfg.baoji < 0) return -106;

		if (!GetSubNodeValue(data_element, "jianren", cfg.jianren) || cfg.jianren < 0) return -107;

		if (!GetSubNodeValue(data_element, "capability", cfg.capability) || cfg.capability < 0) return -108;

		if (!GetSubNodeValue(data_element, "skill_cap_per", cfg.skill_cap_per) || cfg.skill_cap_per < 0) return -109;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int QiLinBiConfig::InitUpStarExpCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (nullptr == data_element)
	{
		return -10000;
	}

	int last_star_level = -1;
	while (nullptr != data_element)
	{
		int star_level = 0;
		if (!GetSubNodeValue(data_element, "star_level", star_level) || !IsValidStartLevel(star_level) || last_star_level + 1 != star_level)
		{
			return -1;
		}

		QiLinBiUpStarExpCfg &cfg = m_upstar_exp_list[star_level];
		last_star_level = star_level;

		if (star_level > m_max_star_level)
		{
			m_max_star_level = star_level;
		}

		if (!GetSubNodeValue(data_element, "up_star_level_exp", cfg.up_star_level_exp) || cfg.up_star_level_exp <= 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "up_start_total_exp", cfg.up_start_total_exp) || cfg.up_start_total_exp < 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -5;
		}

		if (!GetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -6;
		}

		if (!GetSubNodeValue(data_element, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0)
		{
			return -7;
		}

		if (!GetSubNodeValue(data_element, "shanbi", cfg.shanbi) || cfg.shanbi < 0)
		{
			return -8;
		}

		if (!GetSubNodeValue(data_element, "baoji", cfg.baoji) || cfg.baoji < 0)
		{
			return -9;
		}

		if (!GetSubNodeValue(data_element, "jianren", cfg.jianren) || cfg.jianren < 0)
		{
			return -10;
		}

		if (!GetSubNodeValue(data_element, "movespeed", cfg.movespeed) || cfg.movespeed < 0)
		{
			return -11;
		}

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int QiLinBiConfig::InitUpStarStuffCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (nullptr == data_element)
	{
		return -10000;
	}

	int last_up_star_item_index = -1;
	while (nullptr != data_element)
	{
		int up_star_item_index = 0;
		if (!GetSubNodeValue(data_element, "up_star_item_index", up_star_item_index) || !IsValidStuffIdx(up_star_item_index) || up_star_item_index <= last_up_star_item_index)
		{
			return -1;
		}

		QiLinBiUpStarStuffCfg &cfg = m_upstar_item_list[up_star_item_index];
		cfg.up_star_item_index = up_star_item_index;

		last_up_star_item_index = up_star_item_index;

		if (!GetSubNodeValue(data_element, "up_star_item_id", cfg.up_star_item_id)|| nullptr == ITEMPOOL->GetItem(cfg.up_star_item_id))
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "star_exp", cfg.star_exp) || cfg.star_exp < 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "baoji_per", cfg.baoji_per) || cfg.baoji_per < 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(data_element, "baoji_mul", cfg.baoji_mul) || cfg.baoji_mul <= 0)
		{
			return -5;
		}

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int QiLinBiConfig::InitEquipSkillLevel(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (nullptr == data_element)
	{
		return -10000;
	}

	while (nullptr != data_element)
	{
		int equip_min_level = 0;
		if (!GetSubNodeValue(data_element, "equip_min_level", equip_min_level) || !IsValidSkillLevel(equip_min_level))
		{
			return -1;
		}

		if (!GetSubNodeValue(data_element, "equip_skill_level", m_equip_skill_level_cfg[equip_min_level]) || m_equip_skill_level_cfg[equip_min_level] < 0)
		{
			return -2;
		}

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int QiLinBiConfig::InitHuanHuaSpecialCapAddCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (nullptr == dataElement)
	{
		return -10000;
	}

	while (nullptr != dataElement)
	{
		QiLinBiHuanHuaSpecialCapAddCfgItem cfg_item;
		if (!GetSubNodeValue(dataElement, "huanhua_id", cfg_item.huanhua_id) || cfg_item.huanhua_id <= 0 || cfg_item.huanhua_id >= QiLinBiParam::MAX_SPECIAL_IMAGE_COUNT)
		{
			return -1;
		}

		if (!GetSubNodeValue(dataElement, "huanhua_level", cfg_item.huanhua_level) || cfg_item.huanhua_level <= 0 || cfg_item.huanhua_level > QiLinBiParam::MAX_UPGRADE_LIMIT)
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "maxhp", cfg_item.maxhp) || cfg_item.maxhp < 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "gongji", cfg_item.gongji) || cfg_item.gongji < 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "fangyu", cfg_item.fangyu) || cfg_item.fangyu < 0)
		{
			return -5;
		}

		// ÅÐ¶ÏÖØ¸´
		if (m_huanhua_special_cap_cfg.find(cfg_item.huanhua_id) != m_huanhua_special_cap_cfg.end())
		{
			return -6;
		}
		m_huanhua_special_cap_cfg[cfg_item.huanhua_id] = cfg_item;

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

const QiLinBiLevelConfig * QiLinBiConfig::GetLevelConfig(int qilinbi_level)
{
	if (!this->IsValidLevel(qilinbi_level))
	{
		return nullptr;
	}

	if (m_level_list[qilinbi_level].qilinbi_level != qilinbi_level)
	{
		return nullptr;
	}

	return &m_level_list[qilinbi_level];
}

const QiLinBiGradeConfig * QiLinBiConfig::GetGradeConfig(int grade)
{
	if (!IsValidGrade(grade))
	{
		return nullptr;
	}

	return &m_grade_cfg_list[grade];
}

const QiLinBiUpGradeProbConfig * QiLinBiConfig::GetUpGradeProbConfig(int grade, int bless_val)
{
	if (!IsValidGrade(grade))
	{
		return nullptr;
	}

	std::map<int, QiLinBiUpGradeProbList>::const_iterator iter = m_upgrade_prob_map.find(grade);
	if (m_upgrade_prob_map.end() != iter)
	{
		const QiLinBiUpGradeProbList &cfg_list = iter->second;

		for (int i = 0; i < cfg_list.prob_count && i < QiLinBiUpGradeProbList::MAX_PROB_COUNT_PER_GRADE; ++i)
		{
			if (cfg_list.prob_list[i].low_bless_val <= bless_val && bless_val <= cfg_list.prob_list[i].high_bless_val)
			{
				return &cfg_list.prob_list[i];
			}
		}
	}

	return nullptr;
}

const QiLinBiImageConfig * QiLinBiConfig::GetQiLinBiImage(int qilinbi_image_id)
{
	if (!IsValidImageID(qilinbi_image_id))
	{
		return nullptr;
	}

	if (m_image_list[qilinbi_image_id].image_id != qilinbi_image_id)
	{
		return nullptr;
	}

	return &m_image_list[qilinbi_image_id];
}

const QiLinBiSpecialImgCfg * QiLinBiConfig::GetSpecialImgCfg(int special_img_id)
{
	if (IsValidSpecialImageID(special_img_id))
	{
		if (m_special_img_cfglist[special_img_id].IsValid())
		{
			return &m_special_img_cfglist[special_img_id];
		}
	}

	return nullptr;
}


const QiLinBiSpecialImgUpgradeCfg * QiLinBiConfig::GetSpecialImgUpgradeCfg(int special_img_id, int grade)
{
	if (!IsValidSpecialImageID(special_img_id) || !IsValidSpecialImageGrade(grade))
	{
		return nullptr;
	}

	QiLinBiSpecialImgUpgradeCfgMap::iterator iter = m_special_img_upgrade_map.find(special_img_id);
	if (iter != m_special_img_upgrade_map.end())
	{
		QiLinBiSpecialImgUpgradeCfgList &cfg_list = iter->second;

		if (grade > iter->second.max_grade)
		{
			return nullptr;
		}

		if (cfg_list.upgrade_list[grade].grade == grade)
		{
			return &cfg_list.upgrade_list[grade];
		}
	}

	return nullptr;
}

const QiLinBiEquipConifg * QiLinBiConfig::GetEquipCfg(int equip_idx)
{
	if (IsValidEquipIdx(equip_idx))
	{
		return &m_equip_cfg_list[equip_idx];
	}

	return nullptr;
}

const QiLinBiSkillConifg * QiLinBiConfig::GetSkillCfg(int skill_idx, int level)
{
	if (IsValidSkillIdx(skill_idx) && IsValidSkillLevel(level))
	{
		if (m_skill_cfg_list[skill_idx][level].skill_level > 0)
		{
			return &m_skill_cfg_list[skill_idx][level];
		}
	}

	return nullptr;
}

const QiLinBiUpStarExpCfg * QiLinBiConfig::GetUpStarExpCfg(int star_level)
{
	if (!IsValidStartLevel(star_level) || m_upstar_exp_list[star_level].up_star_level_exp <= 0)
		return nullptr;

	return &m_upstar_exp_list[star_level];
}

const QiLinBiUpStarStuffCfg * QiLinBiConfig::GetUpStarStuffCfg(int stuff_index)
{
	if (!IsValidStuffIdx(stuff_index))
		return nullptr;

	return &m_upstar_item_list[stuff_index];
}

int QiLinBiConfig::GetEquipSkillLevel(int min_equip_level)
{
	if (min_equip_level >= m_active_equip_skill_level && IsValidEquipLevel(min_equip_level))
	{
		return m_equip_skill_level_cfg[min_equip_level];
	}

	return -1;
}

bool QiLinBiConfig::IsValidGrade(int grade)
{
	return grade > 0 && grade < QiLinBiParam::MAX_GRADE;
}

bool QiLinBiConfig::IsValidClientGrade(int client_grade)
{
	return client_grade >= 0 && client_grade < QiLinBiParam::MAX_GRADE;
}

bool QiLinBiConfig::IsValidLevel(int level)
{
	return level >= 0 && level <= QiLinBiParam::MAX_LEVEL;
}

bool QiLinBiConfig::IsValidImageID(int image_id)
{
	return image_id > 0 && image_id < QiLinBiParam::MAX_IMAGE_COUNT;
}

bool QiLinBiConfig::IsValidSpecialImageID(int image_id)
{
	return image_id > 0 && image_id < QiLinBiParam::MAX_SPECIAL_IMAGE_COUNT;
}

bool QiLinBiConfig::IsValidSpecialImageGrade(int grade)
{
	return grade >= 0 && grade <= QiLinBiParam::MAX_UPGRADE_LIMIT;
}

bool QiLinBiConfig::IsValidEquipIdx(int equip_idx)
{
	return equip_idx >= 0 && equip_idx < QiLinBiParam::EQUIP_COUNT;
}

bool QiLinBiConfig::IsValidEquipLevel(int level)
{
	return level >= 0 && level <= QiLinBiParam::EQUIP_MAX_LEVEL;
}

bool QiLinBiConfig::IsValidSkillIdx(int skill_idx)
{
	return skill_idx >= 0 && skill_idx < QiLinBiParam::SKILL_COUNT;
}

bool QiLinBiConfig::IsValidSkillLevel(int level)
{
	return level >= 0 && level <= QiLinBiParam::SKILL_MAX_LEVEL;
}

bool QiLinBiConfig::IsValidStartLevel(int level)
{
	return level >= 0 && level < QiLinBiParam::MAX_STAR_LEVEL;
}

bool QiLinBiConfig::IsValidStuffIdx(int stuff_idx)
{
	return stuff_idx >= 0 && stuff_idx < QiLinBiParam::MAX_UP_STAR_ITEM_COUNT;
}

const QiLinBiHuanHuaSpecialCapAddCfgItem* QiLinBiConfig::GetHuanHuaSpecialCapAddCfg(int huanhua_id)const
{
	if (huanhua_id <= 0 || huanhua_id >= QiLinBiParam::MAX_SPECIAL_IMAGE_COUNT)
	{
		return nullptr;
	}

	auto iter = m_huanhua_special_cap_cfg.find(huanhua_id);
	if (iter != m_huanhua_special_cap_cfg.end())
	{
		return &(iter->second);
	}

	return nullptr;
}

bool QiLinBiConfig::IsAddSpecialCap()
{
	return m_is_open_special_cap_add;
}