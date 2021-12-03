#include <memory.h>
#include <stdlib.h>

#include "fabaoconfig.hpp"
#include "globalconfig/equipconfig.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "gameworld/checkresourcecenter.hpp"

FabaoConfig::FabaoConfig()
{
	m_active_equip_grade = 0;
	m_active_equip_skill_level = 0;
	m_is_open_special_cap_add = 0;
	m_medal_skill_index = 0;
	memset(m_equip_skill_level_cfg, 0, sizeof(m_equip_skill_level_cfg));
}

FabaoConfig::~FabaoConfig()
{
	
}

bool FabaoConfig::Init(const std::string& configname, std::string* err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	//其他
	PUGI_XML_LOAD_CONFIG("other",InitOther);
	//进阶
	PUGI_XML_LOAD_CONFIG("grade", InitFabaoGradeCfg);
	//进阶成功率
	PUGI_XML_LOAD_CONFIG("upgrade_prob", InitFabaoUpGradeProbCfg);
	//形象
	PUGI_XML_LOAD_CONFIG("image_list", InitFabaoImageCfg);
	//特殊形象
	PUGI_XML_LOAD_CONFIG("special_img", InitFabaoSpecialImageCfg);
	//特殊形象进阶
	PUGI_XML_LOAD_CONFIG("special_image_upgrade", InitFabaoSpecialImageUpgradeCfg);
	//装备
	PUGI_XML_LOAD_CONFIG("fabao_equip_info", InitEquipCfg);
	//装备等级
	PUGI_XML_LOAD_CONFIG("equip_skill_level", InitEquipSkillLevel);
	//技能
	PUGI_XML_LOAD_CONFIG("fabao_skill", InitFabaoSkillCfg);
	//法宝特殊战力加成
	PUGI_XML_LOAD_CONFIG("huanhua_special_cap_add", InitHuanHuaSpecialCapAddCfg);

	return true;
}

const FabaoGradeConfig* FabaoConfig::GetGradeConfig(int grade) const
{
	if (grade < 0 || grade >= FabaoParam::MAX_GRADE || m_fabao_grade_cfg_list[grade].grade <= 0)
	{
		return NULL;
	}

	return &m_fabao_grade_cfg_list[grade];
}

const FabaoUpGradeProbConfig* FabaoConfig::GetUpGradeProbConfig(int grade, int bless_val) const
{
	std::map<int, FabaoUpGradeProbConfigList>::const_iterator iter = m_upgrade_prob_map.find(grade);
	if (iter != m_upgrade_prob_map.end())
	{
		const FabaoUpGradeProbConfigList &cfg_list = iter->second;
		for (int i = 0;i < cfg_list.prob_count && i < FabaoUpGradeProbConfigList::MAX_PROB_COUNT_PER_GRADE;i++)
		{
			if (cfg_list.prob_list[i].low_bless_val <= bless_val && bless_val <= cfg_list.prob_list[i].high_bless_val)
			{
				return &cfg_list.prob_list[i];
			}
		}
	}

	return NULL;
}

const FabaoImageConfig* FabaoConfig::GetImageConfig(int image_index_id) const
{
	if (image_index_id < 0 || image_index_id >= FabaoParam::MAX_IMAGE_COUNT)
	{
		return NULL;
	}

	if (m_image_config_list[image_index_id].image_id != image_index_id)
	{
		return NULL;
	}

	return &m_image_config_list[image_index_id];
}

const FabaoSpecialImgCfg* FabaoConfig::GetSpecialImgCfg(int special_img_id)
{
	if (special_img_id > 0 || special_img_id < FabaoParam::MAX_SPECIAL_IMAGE_COUNT)
	{
		if (m_special_img_list[special_img_id].IsValid())
		{
			return &m_special_img_list[special_img_id];
		}
	}

	return NULL;
}

const FabaoSpecialImgUpGradeCfg* FabaoConfig::GetSpecialImgUpGradeCfg(int special_img_id, int grade)
{
	FabaoSpecialImgUpgradeCfgMap::iterator iter = m_special_img_upgrade_map.find(special_img_id);
	if (iter != m_special_img_upgrade_map.end())
	{
		FabaoSpecialImgUpGradeCfgList &cfg_list = iter->second;
		if (grade < 0 || grade > FabaoParam::MAX_UPGRADE_LIMIT || grade > iter->second.max_grade)
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

const FabaoSkillConifg* FabaoConfig::GetSkillCfg(int skill_idx, int level)
{
	if (skill_idx >= 0 && skill_idx < FabaoParam::SKILL_COUNT
		&& level > 0 && level <= FabaoParam::SKILL_MAX_LEVEL
		&& m_skill_cfg_list[skill_idx][level].skill_level > 0)
	{
		return &m_skill_cfg_list[skill_idx][level];
	}

	return NULL;
}

const FabaoEquipConifg* FabaoConfig::GetEquipCfg(int equip_idx)
{
	if (equip_idx >= 0 && equip_idx < FabaoParam::EQUIP_COUNT)
	{
		return &m_equip_cfg_list[equip_idx];
	}

	return NULL;
}

int FabaoConfig::GetEquipSkillLevel(int min_equip_level)
{
	if (min_equip_level >= m_active_equip_skill_level && min_equip_level < FabaoParam::EQUIP_MAX_LEVEL)
	{
		return m_equip_skill_level_cfg[min_equip_level];
	}

	return -1;
}

const FabaoHuanHuaSpecialCapAddCfgItem* FabaoConfig::GetHuanHuaSpecialCapAddCfg(int huanhua_id)const
{
	if (huanhua_id <= 0 || huanhua_id >= FabaoParam::MAX_SPECIAL_IMAGE_COUNT)
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

bool FabaoConfig::IsAddSpecialCap()
{
	return m_is_open_special_cap_add;
}

int FabaoConfig::InitOther(PugiXmlNode RootElement)
{
	PugiXmlNode  dataElement = RootElement.child("data");
	if (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement,"fabao_grade_reward_id", m_other_cfg.m_reward_itemid) || NULL == ITEMPOOL->GetItem(m_other_cfg.m_reward_itemid))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "active_equip_grade", m_active_equip_grade) || m_active_equip_grade <= 0 || m_active_equip_grade > FabaoParam::MAX_GRADE)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "active_equip_skill_level", m_active_equip_skill_level) || m_active_equip_skill_level <= 0 || m_active_equip_skill_level > FabaoParam::EQUIP_MAX_LEVEL)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_open_special_cap_add", m_is_open_special_cap_add))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "medal_skill_index", m_medal_skill_index) || m_medal_skill_index < 0 || m_medal_skill_index >= FabaoParam::SKILL_COUNT)
		{
			return -5;
		}
	}

	return 0;
}

int FabaoConfig::InitFabaoGradeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		short grade = 0;
		
		if (!PugiGetSubNodeValue(dataElement,"grade", grade) || grade < 0 || grade >= FabaoParam::MAX_GRADE)
		{
			return -1;
		}

		FabaoGradeConfig &cfg = m_fabao_grade_cfg_list[grade];
		cfg.grade = grade;

		if (!PugiGetSubNodeValue(dataElement, "client_grade", cfg.client_grade) || cfg.client_grade < 0 || cfg.client_grade >= FabaoParam::MAX_GRADE)
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

		if (!PugiGetSubNodeValue(dataElement, "pack_num", cfg.pack_num) || cfg.pack_num < 0) return -109;

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

int FabaoConfig::InitFabaoUpGradeProbCfg(PugiXmlNode RootElement)
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

		FabaoUpGradeProbConfig cfg;

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

		std::map<int, FabaoUpGradeProbConfigList>::iterator iter = m_upgrade_prob_map.find(grade);

		if (iter != m_upgrade_prob_map.end())
		{
			FabaoUpGradeProbConfigList &cfg_list = iter->second;
			if (!cfg_list.Add(cfg))
			{
				return -50;
			}
		}
		else
		{
			FabaoUpGradeProbConfigList cfg_list;
			cfg_list.grade = grade;
			cfg_list.Add(cfg);
			m_upgrade_prob_map[grade] = cfg_list;
		}

		last_grade = grade;
		
		dataElement = dataElement.next_sibling();
	}

	for (std::map<int,FabaoUpGradeProbConfigList>::const_iterator iter = m_upgrade_prob_map.begin();iter != m_upgrade_prob_map.end();++iter)
	{
		const FabaoUpGradeProbConfigList &cfg_list = iter->second;

		if(cfg_list.prob_count <= 0 || cfg_list.prob_count >= FabaoUpGradeProbConfigList::MAX_PROB_COUNT_PER_GRADE)
		{
			return -60;
		}

		const FabaoGradeConfig* grade_cfg = GetGradeConfig(cfg_list.grade);
		if (grade_cfg == NULL)
		{
			return -100;
		}

		if (grade_cfg->bless_val_limit != cfg_list.prob_list[cfg_list.prob_count-1].high_bless_val)
		{
			return -101;
		}
	}
	return 0;
}

int FabaoConfig::InitFabaoImageCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int image_id = 0;

		if (!PugiGetSubNodeValue(dataElement,"image_id",image_id) || image_id < 0)
		{
			return -1;
		}

		FabaoImageConfig cfg;
		cfg.image_id = image_id;
		m_image_config_list[cfg.image_id] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int FabaoConfig::InitFabaoSpecialImageCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int image_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "image_id", image_id) || image_id <= 0 || image_id >= FabaoParam::MAX_SPECIAL_IMAGE_COUNT)
		{
			return -1;
		}

		FabaoSpecialImgCfg &cfg_item = m_special_img_list[image_id];
		if (cfg_item.IsValid())
		{
			return -2;
		}

		cfg_item.special_img_id = image_id;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int FabaoConfig::InitFabaoSpecialImageUpgradeCfg(PugiXmlNode RootElement)
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
		if (!PugiGetSubNodeValue(dataElement, "special_img_id", image_id) || image_id <= 0 || image_id >= FabaoParam::MAX_SPECIAL_IMAGE_COUNT)
		{
			return -1;
		}

		if (image_id != last_image_id)
		{
			last_image_id = image_id;
			last_grade = -1;
		}

		FabaoSpecialImgUpGradeCfgList &cfg_list = m_special_img_upgrade_map[image_id];

		cfg_list.max_grade = 0;
		if (!PugiGetSubNodeValue(dataElement, "grade", cfg_list.max_grade) || cfg_list.max_grade < 0 || cfg_list.max_grade > FabaoParam::MAX_UPGRADE_LIMIT
			|| cfg_list.max_grade != last_grade + 1)
		{
			return -2;
		}

		last_grade = cfg_list.max_grade;

		FabaoSpecialImgUpGradeCfg &cfg_item = cfg_list.upgrade_list[cfg_list.max_grade];
		cfg_item.special_img_id = image_id;
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

int FabaoConfig::InitFabaoSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int skill_idx = 0;
		if (!PugiGetSubNodeValue(dataElement, "skill_idx", skill_idx) || skill_idx < 0 || skill_idx >= FabaoParam::SKILL_COUNT)
		{
			return -1;
		}

		int skill_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "skill_level", skill_level) || skill_level <= 0)
		{
			return -2;
		}

		FabaoSkillConifg &cfg = m_skill_cfg_list[skill_idx][skill_level];
		cfg.index = skill_idx;
		cfg.skill_level = skill_level;

		if (!PugiGetSubNodeValue(dataElement, "skill_type", cfg.skill_type) || cfg.skill_type < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "grade", cfg.fabao_grade) || cfg.fabao_grade < 0)
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

		if (!PugiGetSubNodeValue(dataElement, "base_attr_add_per", cfg.base_attr_add_per) || cfg.base_attr_add_per < 0) return -110;

		if (!PugiGetSubNodeValue(dataElement, "end_day_of_up_skill3", cfg.end_day_of_up_skill) || cfg.end_day_of_up_skill < 0) return -111;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int FabaoConfig::InitEquipCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int equip_idx = 0;
		if (!PugiGetSubNodeValue(dataElement, "equip_idx", equip_idx) || equip_idx < 0 || equip_idx >= FabaoParam::EQUIP_COUNT)
		{
			return -1;
		}

		FabaoEquipConifg &equip_cfg = m_equip_cfg_list[equip_idx];

		int level = 0;

		if (!PugiGetSubNodeValue(dataElement, "equip_level", level) || level < 0 || level >= FabaoParam::EQUIP_MAX_LEVEL) return -2;

		PugiXmlNode item_element = dataElement.child("item");
		if (NULL != item_element)
		{
			if (!equip_cfg.level_cfg[level].item.ReadConfig(item_element))
			{
				return -3;
			}
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", equip_cfg.level_cfg[level].maxhp) || equip_cfg.level_cfg[level].maxhp < 0) return -101;

		if (!PugiGetSubNodeValue(dataElement, "gongji", equip_cfg.level_cfg[level].gongji) || equip_cfg.level_cfg[level].gongji < 0) return -102;

		if (!PugiGetSubNodeValue(dataElement, "fangyu", equip_cfg.level_cfg[level].fangyu) || equip_cfg.level_cfg[level].fangyu < 0) return -103;

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", equip_cfg.level_cfg[level].mingzhong) || equip_cfg.level_cfg[level].mingzhong < 0) return -104;

		if (!PugiGetSubNodeValue(dataElement, "shanbi", equip_cfg.level_cfg[level].shanbi) || equip_cfg.level_cfg[level].shanbi < 0) return -105;

		if (!PugiGetSubNodeValue(dataElement, "baoji", equip_cfg.level_cfg[level].baoji) || equip_cfg.level_cfg[level].baoji < 0) return -106;

		if (!PugiGetSubNodeValue(dataElement, "jianren", equip_cfg.level_cfg[level].jianren) || equip_cfg.level_cfg[level].jianren < 0) return -107;

		if (!PugiGetSubNodeValue(dataElement, "add_percent", equip_cfg.level_cfg[level].add_percent) || equip_cfg.level_cfg[level].add_percent < 0) return -108;

		equip_cfg.max_level++;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int FabaoConfig::InitEquipSkillLevel(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int equip_min_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "equip_min_level", equip_min_level) || equip_min_level < 0 || equip_min_level >= FabaoParam::EQUIP_MAX_LEVEL)
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

int FabaoConfig::InitHuanHuaSpecialCapAddCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		FabaoHuanHuaSpecialCapAddCfgItem cfg_item;
		if (!PugiGetSubNodeValue(dataElement, "huanhua_id", cfg_item.huanhua_id) || cfg_item.huanhua_id <= 0 || cfg_item.huanhua_id >= FabaoParam::MAX_SPECIAL_IMAGE_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "huanhua_level", cfg_item.huanhua_level) || cfg_item.huanhua_level <= 0 || cfg_item.huanhua_level > FabaoParam::MAX_UPGRADE_LIMIT)
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
		if (m_huanhua_special_cap_cfg.find(cfg_item.huanhua_id) != m_huanhua_special_cap_cfg.end())
		{
			return -6;
		}
		m_huanhua_special_cap_cfg[cfg_item.huanhua_id] = cfg_item;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}
