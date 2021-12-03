

#include "greatesoldierconfig.hpp"
#include "config/logicconfigmanager.hpp"

bool GreateSoldierConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	PUGI_XML_LOAD_CONFIG("other", InitOtherCfg);
	PUGI_XML_LOAD_CONFIG("level", InitLevelCfg);
	PUGI_XML_LOAD_CONFIG("level_attr_per", InitLevelAttrPerCfg);
	PUGI_XML_LOAD_CONFIG("potential", InitUpgradeCfg);
	PUGI_XML_LOAD_CONFIG("passive_skill", InitPassiveSkillCfg);
	PUGI_XML_LOAD_CONFIG("specialskill_tips", InitSpecialSkillCfg);
	PUGI_XML_LOAD_CONFIG("slot", InitSlotCfg);
	PUGI_XML_LOAD_CONFIG("slot_name", InitSlotOtherCfg);
	PUGI_XML_LOAD_CONFIG("combine", InitCombineCfg);
	PUGI_XML_LOAD_CONFIG("combine_attr", InitCombineAttrCfg);
	PUGI_XML_LOAD_CONFIG("draw", InitDrawCfg);
	PUGI_XML_LOAD_CONFIG("general_limit_draw", InitLimitDrawCfg);
	PUGI_XML_LOAD_CONFIG("equip_level", InitEquipLevelCfg);

	//LOAD_CONFIG_LIST2("goal", m_goal_cfg);
	PUGI_XML_LOAD_CONFIG("huanhua", InitHuanHuaCfg);

	return true;
}

const GreateSoldierLevelConfig::CfgItem * GreateSoldierConfig::GetLevelCfg(int seq)
{
	if (!this->IsValidSeq(seq)) return nullptr;

	return &m_level_cfg.cfg_item_list[seq];
}

const int GreateSoldierConfig::GetGreateSoldierLevelAttrPer(int level)
{
	auto it = m_level_attr_per_cfg.find(level);
	if (it != m_level_attr_per_cfg.end())
	{
		return it->second;
	}

	return 0;
}

const GreateSoldierPotentialConfig::CfgItem * GreateSoldierConfig::GetPotentialCfg(int seq, int level)
{
	if (!this->IsValidSeq(seq)) return nullptr;

	const auto &cfg_vec = m_upgrade_cfg.cfg_item_vec_list[seq];
	for (auto it = cfg_vec.cbegin(); it != cfg_vec.cend(); ++it)
	{
		if (level < it->level)
		{
			return &(*it);
		}
	}

	return nullptr;
}

const GreateSoldierPassiveSkillConfig::CfgItem * GreateSoldierConfig::GetPassiveSkillCfg(int seq)
{
	if (!this->IsValidPassiveSkillSeq(seq)) return nullptr;

	return &m_passive_skill_cfg.cfg_item_list[seq];
}

const GreateSoldierSpecialSkillConfig::CfgItem * GreateSoldierConfig::GetSpecialSkillCfg(int special_skill_type, int skill_level)
{
	if (special_skill_type <= GREATE_SOLDIER_SPECIAL_SKILL_TYPE_INVALID || special_skill_type >= GREATE_SOLDIER_SPECIAL_SKILL_TYPE_MAX)
	{
		return nullptr;
	}

	if (skill_level <= 0 || skill_level >= GREATE_SOLDIER_SPECIAL_SKILL_MAX_LEVEL)
	{
		return nullptr;
	}

	return &m_special_skill_cfg.cfg_item_list[special_skill_type][skill_level];
}

const GreateSoldierSlotConfig::CfgItem * GreateSoldierConfig::GetSlotCfg(int seq, int level)
{
	if (!this->IsValidSlotSeqLevel(seq, level)) return nullptr;

	return &m_slot_cfg.cfg_item_list[seq * GREATE_SOLDIER_SLOT_LEVEL_MAX_COUNT + level];
}

const GreateSoldierSlotOtherConfig::CfgItem * GreateSoldierConfig::GetSlotOtherCfg(int seq)
{
	if (!this->IsValidSlotSeq(seq))
	{
		return nullptr;
	}

	return &m_slot_other_cfg.cfg_item_list[seq];
}

const GreateSoldierCombineConfig::CfgItem * GreateSoldierConfig::GetCombineCfg(int seq)
{
	if (!this->IsValidCombineSeq(seq)) return nullptr;

	return &m_combine_cfg.cfg_item_list[seq];
}

const GreateSoldierCombineAttrConfig::CfgItem * GreateSoldierConfig::GetCombineAttrCfg(int seq, int level)
{
	if (!this->IsValidCombineSeq(seq)) return nullptr;

	const auto &cfg_vec = m_combine_attr_cfg.cfg_item_vec_list[seq];
	for (auto it = cfg_vec.crbegin(); it != cfg_vec.crend(); ++it)
	{
		if (level >= it->need_min_strength_level)
		{
			return &(*it);
		}
	}

	return nullptr;
}

const GreateSoldierDrawConfig::CfgItem * GreateSoldierConfig::GetRandomDrawReward(int draw_type)
{
	RandItemConfig *draw_cfg = nullptr;

	if (GREATE_SOLDIER_DRAW_TYPE_1_DRAW == draw_type)
	{
		draw_cfg = &m_draw_cfg.draw_1_weight;
	}
	else if (GREATE_SOLDIER_DRAW_TYPE_10_DRAW == draw_type)
	{
		draw_cfg = &m_draw_cfg.draw_10_weight;
	}
	else if (GREATE_SOLDIER_DRAW_TYPE_50_DRAW == draw_type)
	{
		draw_cfg = &m_draw_cfg.draw_50_weight;
	}
	else if (GREATE_SOLDIER_DRAW_TYPE_SPECIAL_10_DRAW == draw_type)
	{
		draw_cfg = &m_draw_cfg.special_draw_10_weight;
	}
	else if (GREATE_SOLDIER_DRAW_TYPE_SPECIAL_50_DRAW == draw_type)
	{
		draw_cfg = &m_draw_cfg.special_draw_50_weight;
	}

	if (!draw_cfg) return nullptr;

	auto rand_res = draw_cfg->Rand();
	if (!rand_res || !this->IsValidDrawSeq(rand_res->seq))
	{
		return nullptr;
	}

	return &m_draw_cfg.cfg_item_list[rand_res->seq];
}

const GreateSoldierEquipLevelCfg::CfgItem* GreateSoldierConfig::GetGreateSoldierEquipLevelConfig(int slot_index, int quality, int strength_level)
{
	if (slot_index < 0 || slot_index >= GREATE_SOLDIER_EQUIPMENT_SLOT_MAX_COUNT)
	{
		return nullptr;
	}

	if (quality <= ItemBase::I_COLOR_INVALID || quality >= ItemBase::I_COLOR_MAX)
	{
		return nullptr;
	}

	if (strength_level < 0 || strength_level > GREATE_SOLDIER_MAX_EQUIP_STRENGTH_LEVEL)
	{
		return nullptr;
	}

	return &m_equip_level_cfg.item_cfg_list[slot_index][quality][strength_level];
}

int GreateSoldierConfig::GetEquipShulianduMax(int equip_slot_index, int quality)
{
	if (equip_slot_index < 0 || equip_slot_index >= GREATE_SOLDIER_EQUIPMENT_SLOT_MAX_COUNT)
	{
		return -1;
	}

	if (quality <= ItemBase::I_COLOR_INVALID || quality >= ItemBase::I_COLOR_MAX)
	{
		return -2;
	}

	int MAX_LEVEL = m_equip_level_cfg.max_strength_level;
	auto tmp_shuliandu_cfg = GetGreateSoldierEquipLevelConfig(equip_slot_index, quality, MAX_LEVEL);
	if (nullptr == tmp_shuliandu_cfg)
	{
		return -3;
	}

	return tmp_shuliandu_cfg->contain_shulian;
}

const GreateSoldierHuanhuaConfig* GreateSoldierConfig::GetGreateSoldierHuanhuaConfig(int id)
{
	std::map<int, GreateSoldierHuanhuaConfig>::const_iterator it = m_huanhua_cfg.find(id);
	if (it != m_huanhua_cfg.end())
	{
		return &it->second;
	}

	return nullptr;
}

int GreateSoldierConfig::InitOtherCfg(PugiXmlNode root_element)
{
	PugiXmlNode data_element = root_element.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (m_other_cfg.has_read) return -1000;
	m_other_cfg.has_read = 1;

	if (!PugiGetSubNodeValue(data_element, "max_level", m_other_cfg.max_level) || m_other_cfg.max_level <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "bianshen_duration_s", m_other_cfg.bianshen_duration_s) || m_other_cfg.bianshen_duration_s <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "bianshen_cd_s", m_other_cfg.bianshen_cd_s) || m_other_cfg.bianshen_cd_s <= 0)
	{
		return -3;
	}

	if (!m_other_cfg.draw_1_consume_item.ReadConfig(data_element, "draw_1_item_id", "draw_1_item_num"))
	{
		return -4;
	}

	if (!m_other_cfg.dailyfirst_draw_10_consume_item.ReadConfig(data_element, "daily_first_draw_10_item_id", "daily_first_draw_10_item_num"))
	{
		return -5;
	}

	if (!m_other_cfg.draw_10_consume_item.ReadConfig(data_element, "draw_10_item_id", "draw_10_item_num"))
	{
		return -6;
	}

	if (!m_other_cfg.draw_50_consume_item.ReadConfig(data_element, "draw_50_item_id", "draw_50_item_num"))
	{
		return -7;
	}
	
	if (!PugiGetSubNodeValue(data_element, "bianyi_need_potential_per", m_other_cfg.bianyi_need_potential_per) || m_other_cfg.bianyi_need_potential_per <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(data_element, "bianyi_add_passive_skill_per", m_other_cfg.bianyi_add_passive_skill_per) || m_other_cfg.bianyi_add_passive_skill_per <= 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(data_element, "bianshen_trail_s", m_other_cfg.bianshen_trail_s))
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(data_element, "bianshen_trail_capability", m_other_cfg.bianshen_trail_capability))
	{
		return -12;
	}

	return 0;
}

int GreateSoldierConfig::InitLevelCfg(PugiXmlNode root_element)
{
	PugiXmlNode data_element = root_element.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || !this->IsValidSeq(seq, true))
		{
			return -100;
		}

		int ret = m_level_cfg.cfg_item_list[seq].ReadConfig(data_element, seq);
		if (ret != 0)
		{
			return ret;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int GreateSoldierConfig::InitLevelAttrPerCfg(PugiXmlNode root_element)
{
	PugiXmlNode data_element = root_element.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_level = 0;

	while (!data_element.empty())
	{
		int level = 0, per_attr = 0;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level <= 0)
		{
			return -1;
		}

		if (level != last_level + 1)
		{
			return -100;
		}

		last_level = level;

		if (!PugiGetSubNodeValue(data_element, "per_attr", per_attr) || per_attr <= 0)
		{
			return -2;
		}

		m_level_attr_per_cfg[level] = per_attr;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int GreateSoldierConfig::InitUpgradeCfg(PugiXmlNode root_element)
{
	PugiXmlNode data_element = root_element.child("data");
	if (data_element.empty())
	{
		return -10000;
	}
	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || !this->IsValidSeq(seq, true))
		{
			return -100;
		}

		GreateSoldierPotentialConfig::CfgItem cfg;
		int ret = cfg.ReadConfig(data_element, seq);
		if (ret != 0)
		{
			return ret;
		}

		m_upgrade_cfg.cfg_item_vec_list[seq].emplace_back(cfg);

		data_element = data_element.next_sibling();
	}
	return 0;
}

int GreateSoldierConfig::InitPassiveSkillCfg(PugiXmlNode root_element)
{
	PugiXmlNode data_element = root_element.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || !this->IsValidPassiveSkillSeq(seq, true))
		{
			return -100;
		}

		int ret = m_passive_skill_cfg.cfg_item_list[seq].ReadConfig(data_element, seq);
		if (ret != 0)
		{
			return ret;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int GreateSoldierConfig::InitSpecialSkillCfg(PugiXmlNode root_element)
{
	PugiXmlNode data_element = root_element.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int special_skill_type = 0;
		int skill_level = 0;
		if (!PugiGetSubNodeValue(data_element, "active_skill_type", special_skill_type) || 
			special_skill_type <= GREATE_SOLDIER_SPECIAL_SKILL_TYPE_INVALID || special_skill_type >= GREATE_SOLDIER_SPECIAL_SKILL_TYPE_MAX)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "skill_level", skill_level) ||
			skill_level <= 0 || skill_level >= GREATE_SOLDIER_SPECIAL_SKILL_MAX_LEVEL)
		{
			return -200;
		}

		GreateSoldierSpecialSkillConfig::CfgItem &cfg_item = m_special_skill_cfg.cfg_item_list[special_skill_type][skill_level];
		int ret = cfg_item.ReadConfig(data_element, special_skill_type, skill_level);
		if (ret != 0)
		{
			return ret;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int GreateSoldierConfig::InitSlotCfg(PugiXmlNode root_element)
{
	PugiXmlNode data_element = root_element.child("data");
	while (!data_element.empty())
	{
		int seq = 0, level = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0) return -100;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level < 0) return -101;

		if (!this->IsValidSlotSeqLevel(seq, level, true))
		{
			return -102;
		}

		int ret = m_slot_cfg.cfg_item_list[seq * GREATE_SOLDIER_SLOT_LEVEL_MAX_COUNT + level].ReadConfig(data_element, seq, level);
		if (ret != 0)
		{
			return ret;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int GreateSoldierConfig::InitSlotOtherCfg(PugiXmlNode root_element)
{
	auto data_element = root_element.child("data");
	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || !this->IsValidSlotSeq(seq)) return -1000;

		int ret = m_slot_other_cfg.cfg_item_list[seq].ReadConfig(data_element, seq);
		if (ret != 0)
		{
			return ret;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int GreateSoldierConfig::InitCombineCfg(PugiXmlNode root_element)
{
	PugiXmlNode data_element = root_element.child("data");
	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || !this->IsValidCombineSeq(seq, true))
		{
			return -100;
		}

		int ret = m_combine_cfg.cfg_item_list[seq].ReadConfig(data_element, seq);
		if (ret != 0)
		{
			return ret;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int GreateSoldierConfig::InitCombineAttrCfg(PugiXmlNode root_element)
{
	PugiXmlNode data_element = root_element.child("data");
	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || !this->IsValidCombineSeq(seq, true))
		{
			return -100;
		}

		GreateSoldierCombineAttrConfig::CfgItem cfg;

		int ret = cfg.ReadConfig(data_element, seq);
		if (ret != 0)
		{
			return ret;
		}

		m_combine_attr_cfg.cfg_item_vec_list[seq].emplace_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int GreateSoldierConfig::InitDrawCfg(PugiXmlNode root_element)
{
	PugiXmlNode data_element = root_element.child("data");
	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || !this->IsValidDrawSeq(seq, true))
		{
			return -10000;
		}

		auto cfg_item = &m_draw_cfg.cfg_item_list[seq];
		int ret = cfg_item->ReadConfig(data_element, seq);
		if (ret != 0)
		{
			return ret;
		}
		
		if (!m_draw_cfg.draw_1_weight.ReadConfig(data_element, "draw_1_weight")) return -100;
		if (!m_draw_cfg.draw_10_weight.ReadConfig(data_element, "draw_10_weight")) return -101;
		if (!m_draw_cfg.draw_50_weight.ReadConfig(data_element, "draw_50_weight")) return -102;

		if (cfg_item->is_special_10_reward)
		{
			if (!m_draw_cfg.special_draw_10_weight.ReadConfig(data_element, "draw_10_weight")) return -103;
		}
		else if (cfg_item->is_special_50_reward)
		{
			if (!m_draw_cfg.special_draw_50_weight.ReadConfig(data_element, "draw_50_weight")) return -104;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}


int GreateSoldierConfig::InitLimitDrawCfg(PugiXmlNode root_element)
{
	PugiXmlNode data_element = root_element.child("data");
	if (data_element.empty())
	{
		return -6666;
	}

	while (!data_element.empty())
	{
		GreateSoldierLimitDrawConfig::CfgItem cfg;
		if (!PugiGetSubNodeValue(data_element, "draw_number", cfg.draw_num) || cfg.draw_num < 0)
		{
			return -1000;
		}

		{
			int count = ItemConfigData::ReadConfigList(data_element, std::string("reward_item"),
				cfg.reward_list, MAX_ATTACHMENT_ITEM_NUM);
			if (count < 0)
			{
				return -200 + count;
			}
		}

		m_limit_draw_reward_cfg.reward_vec.push_back(cfg);
		data_element = data_element.next_sibling();
	}

	return 0;
}

int GreateSoldierConfig::InitEquipLevelCfg(PugiXmlNode root_element)
{
	PugiXmlNode data_element = root_element.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_equip_level_cfg.max_strength_level = 0;

	while (!data_element.empty())
	{
		int slot_index = 0, quality = 0, strength_level = 0;
		if (!PugiGetSubNodeValue(data_element, "slot_index", slot_index) || slot_index < 0 || slot_index >= GREATE_SOLDIER_EQUIPMENT_SLOT_MAX_COUNT)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "quality", quality) || quality <= ItemBase::I_COLOR_INVALID || quality >= ItemBase::I_COLOR_MAX)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "strength_level", strength_level) || strength_level < 0 || strength_level > GREATE_SOLDIER_MAX_EQUIP_STRENGTH_LEVEL)
		{
			return -102;
		}

		GreateSoldierEquipLevelCfg::CfgItem &cfg = m_equip_level_cfg.item_cfg_list[slot_index][quality][strength_level];
		int ret = cfg.ReadConfig(data_element, slot_index, quality, strength_level);
		if (ret != 0)
		{
			return ret;
		}

		if (strength_level > m_equip_level_cfg.max_strength_level)
		{
			m_equip_level_cfg.max_strength_level = strength_level;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int GreateSoldierConfig::InitHuanHuaCfg(PugiXmlNode root_element)
{
	PugiXmlNode data_element = root_element.child("data");
	if (data_element.empty())
	{
		return -6666;
	}

	while (!data_element.empty())
	{
		GreateSoldierHuanhuaConfig cfg;

		if (!PugiGetSubNodeValue(data_element, "id", cfg.id) || cfg.id <= 0) return -1;
		if (!PugiGetSubNodeValue(data_element, "res_id", cfg.res_id) || cfg.res_id <= 0) return -2;
		if (cfg.attr_cfg.ReadConfig(data_element) < 0) return -3;
		if (!PugiGetSubNodeValue(data_element, "active_skill_level", cfg.active_skill_level) || cfg.active_skill_level <= 0) return -4;

		if (m_huanhua_cfg.find(cfg.id) != m_huanhua_cfg.end())  // id²»ÄÜÖØ¸´
		{
			return -100;
		}

		m_huanhua_cfg[cfg.id] = cfg;
		
		data_element = data_element.next_sibling();
	}

	return 0;
}
