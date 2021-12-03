#include <memory.h>
#include <stdlib.h>

#include "zhuanzhiequipconfig.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "servercommon/zhuanzhiequipdef.hpp"
#include "item/itempool.h"
#include "obj/character/attribute.hpp"
#include "gameworld/checkresourcecenter.hpp"


#include <string>
#include<algorithm>

ZhuanzhiEquipConfig::ZhuanzhiEquipConfig()
{
	memset(m_suit_group_cfg_list, 0, sizeof(m_suit_group_cfg_list));
	memset(m_suit_forge_limit_list, 0, sizeof(m_suit_forge_limit_list));
}

ZhuanzhiEquipConfig::~ZhuanzhiEquipConfig()
{

}

bool ZhuanzhiEquipConfig::Init(const std::string &configname, std::string *err)
{

	PUGI_XML_CONFIG_PRE_LOAD;

	// 其他
	PUGI_XML_LOAD_CONFIG("other", InitOtherConfig);

	// 转职装备配置
	PUGI_XML_LOAD_CONFIG("zhuanzhi_equip_info", InitZhuanzhiEquipCfg);
	// 装备特殊效果
	PUGI_XML_LOAD_CONFIG("equip_special_effect", InitEquipSpecialEffectCfg);

	// 升星
	PUGI_XML_LOAD_CONFIG("up_star", InitUpStarCfg);
	// 全身升星
	PUGI_XML_LOAD_CONFIG("total_upstar", InitUpStarTotalCfg);
	// 附灵
	PUGI_XML_LOAD_CONFIG("equip_fuling", InitFulingCfg);
	// 附灵材料
	PUGI_XML_LOAD_CONFIG("fuling_stuff", InitFulingStuffCfg);

	// 玉石
	PUGI_XML_LOAD_CONFIG("stone", InitZhuanzhiStoneCfg);
	// 全身玉石
	PUGI_XML_LOAD_CONFIG("total_stone", InitZhuanzhiStoneTotalCfg);
	// 玉石升级配置
	PUGI_XML_LOAD_CONFIG("stone_level_up", InitZhuanzhiStoneLevelCfg);
	// 玉石孔配置
	PUGI_XML_LOAD_CONFIG("stone_slot_cfg", InitZhuanzhiStoneSlotCfg);
	// 玉石精炼配置
	PUGI_XML_LOAD_CONFIG("stone_refine", InitZhuanzhiStoneRefineCfg);
	// 精炼材料配置
	PUGI_XML_LOAD_CONFIG("stone_refine_stuff", InitRefineStuffCfg);
	// 玉石分解配置
	PUGI_XML_LOAD_CONFIG("stone_resolve", InitStoneResolveCfg);
	// 玉石兑换配置
	PUGI_XML_LOAD_CONFIG("stone_convert", InitStoneConvertCfg);

	// 套装组合类型
	PUGI_XML_LOAD_CONFIG("suit_group_type", InitSuitGroupTypeCfg);
	// 套装锻造限制
	PUGI_XML_LOAD_CONFIG("suit_forge_limit", InitSuitForgeLimitCfg);
	//套装部位激活
	PUGI_XML_LOAD_CONFIG("suit_part_active", InitOneEquipActConfig);
	// 防具组合套装属性
	PUGI_XML_LOAD_CONFIG("suit_attr_fang", InitFangGroupAttrCfg);
	// 攻击组合套装属性
	PUGI_XML_LOAD_CONFIG("suit_attr_gong", InitGongGroupAttrCfg);
	// 至尊组合套装属性
	PUGI_XML_LOAD_CONFIG("suit_attr_zhizun", InitZhizunGroupAttrCfg);

	// 至尊装备
	PUGI_XML_LOAD_CONFIG("zhizun_cfg", InitZhiZunEquipCfg);
	// 至尊装备合成
	PUGI_XML_LOAD_CONFIG("zhizun_compose_cfg", InitEquipZhiZunComposeCfg);

	// 特殊装备
	PUGI_XML_LOAD_CONFIG("special_equip_cfg", InitSpecialEquipCfg);

	//觉醒等级限制
	PUGI_XML_LOAD_CONFIG("wake_level_limit_cfg", InitWakeLevelLimitCfg);
	//觉醒属性
	PUGI_XML_LOAD_CONFIG("wake_attribute", InitWakeAttributeCfg);
	//觉醒类型权重
	PUGI_XML_LOAD_CONFIG("wake_type_weight", InitWakeTypeWeightCfg);
	//觉醒等级权重
	PUGI_XML_LOAD_CONFIG("wake_level_weight", InitWakeLevelWeightCfg);
	//觉醒锁定消耗配置
	PUGI_XML_LOAD_CONFIG("lock_consume", InitWakeLockConsumeCfg);
	//合成配置
	PUGI_XML_LOAD_CONFIG("zhuanzhi_equip_compose", InitEquipComposeCfg);
	//条件属性配置
	PUGI_XML_LOAD_CONFIG("condition_attr", InitConditionAttrCfg);
	return true;
}

// 其它配置
int ZhuanzhiEquipConfig::InitOtherConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}


	if (!PugiGetSubNodeValue(dataElement, "refine_min_order", m_other_cfg.refine_min_order) || m_other_cfg.refine_min_order <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "zhizun_add_max_level", m_other_cfg.zhizun_add_max_level) || m_other_cfg.zhizun_add_max_level <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "wake_min_order", m_other_cfg.wake_min_order) || m_other_cfg.wake_min_order < 0 || m_other_cfg.wake_min_order >= ItemPool::MAX_EQUIPMENT_ORDER)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "wake_min_color", m_other_cfg.wake_min_color) || m_other_cfg.wake_min_color <= ItemBase::I_COLOR_INVALID || m_other_cfg.wake_min_order >= ItemBase::I_COLOR_MAX)
	{
		return -4;
	}
	return 0;
}


int ZhuanzhiEquipConfig::InitZhuanzhiEquipCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		ZhuanzhiEquipCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "equip_part", cfg.equip_part) || cfg.equip_part < 0 || cfg.equip_part >= E_INDEX_ZHUANZHI::E_INDEX_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "equip_order", cfg.equip_order) || cfg.equip_order <= 0 || cfg.equip_order > ItemPool::MAX_EQUIPMENT_ORDER)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "role_need_min_level", cfg.role_need_min_level) || cfg.role_need_min_level <= 0 || cfg.role_need_min_level > MAX_ROLE_LEVEL)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "role_need_min_prof_level", cfg.role_need_min_prof_level) || cfg.role_need_min_prof_level < 0 || cfg.role_need_min_prof_level > MAX_PROF_LEVEL)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "special_attr_color", cfg.special_attr_color) || cfg.special_attr_color <= 0 || cfg.special_attr_color >= ItemBase::I_COLOR_MAX)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "add_cap", cfg.add_cap) || cfg.add_cap < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "pvp_zengshang_per", cfg.pvp_zengshang_per) || cfg.pvp_zengshang_per < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "pve_zengshang_per", cfg.pve_zengshang_per) || cfg.pve_zengshang_per < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "pvp_jianshang_per", cfg.pvp_jianshang_per) || cfg.pvp_jianshang_per < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "pve_jianshang_per", cfg.pve_jianshang_per) || cfg.pve_jianshang_per < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "sys_baoji_per", cfg.sys_baoji_per) || cfg.sys_baoji_per < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "sys_kangbao_per", cfg.sys_kangbao_per) || cfg.sys_kangbao_per < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "effect_type", cfg.effect_type) || cfg.effect_type < 0)
		{
			return -13;
		}

		long long key = ConvertParamToLongLong(cfg.equip_part, cfg.equip_order);
		EquipPutOnLimitConfigMapIt it = m_equip_puton_limit_cfg_map.find(key);
		if (it != m_equip_puton_limit_cfg_map.end())
		{
			return -100;
		}

		m_equip_puton_limit_cfg_map[key] = cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitEquipSpecialEffectCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		EquipSpecialEffectCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "effect_type", cfg.effect_type) || cfg.effect_type <= 0 || cfg.effect_type >= EQUIP_SPECIAL_EFFECT_TYPE_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "cd_time", cfg.cd_time) || cfg.cd_time < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "rate", cfg.rate) || cfg.rate <= 0 || cfg.rate >= 10000)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_1", cfg.param_1) || cfg.param_1 < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_2", cfg.param_2) || cfg.param_2 < 0)
		{
			return -5;
		}

		m_special_effect_vec.push_back(cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitUpStarCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int equip_index = 0;
		if (!PugiGetSubNodeValue(dataElement, "equip_index", equip_index) || equip_index < 0 || equip_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX)
		{
			return -1;
		}

		int star_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "star_level", star_level) || star_level < 0 || star_level > MAX_STAR_LEVEL)
		{
			return -2;
		}

		ZhuanzhiUpStarConfig &cfg = m_up_star_list[equip_index][star_level];
		cfg.equip_index = equip_index;
		cfg.star_level = star_level;

		if (!PugiGetSubNodeValue(dataElement, "equip_level", cfg.equip_level) || cfg.equip_level < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_mojing", cfg.need_mojing) || cfg.need_mojing < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.add_maxhp) || cfg.add_maxhp < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.add_gongji) || cfg.add_gongji < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.add_fangyu) || cfg.add_fangyu < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_broadcast", cfg.is_broadcast) || cfg.is_broadcast < 0 || cfg.is_broadcast > 1)
		{
			return -8;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitUpStarTotalCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_up_star_total_cfg_count = 0;
	int last_up_star_total_level = 0;

	while (!dataElement.empty())
	{
		if (m_up_star_total_cfg_count >= TOTAL_STAR_CFG_MAX_COUNT)
		{
			return -10000;
		}

		ZhuanzhiUpStarTotalConfig *cfg = &m_up_star_total_cfg_list[m_up_star_total_cfg_count];


		if (!PugiGetSubNodeValue(dataElement, "seq", cfg->seq) || cfg->seq < 0 || cfg->seq >= TOTAL_STAR_CFG_MAX_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "total_star", cfg->total_star) || cfg->total_star <= last_up_star_total_level)
		{
			return -2;
		}
		last_up_star_total_level = cfg->total_star;

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg->add_maxhp) || cfg->add_maxhp < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg->add_gongji) || cfg->add_gongji < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg->add_fangyu) || cfg->add_fangyu < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_broadcast", cfg->is_broadcast) || cfg->is_broadcast < 0 || cfg->is_broadcast >1)
		{
			return -6;
		}

		++m_up_star_total_cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}


const ZhuanzhiEquipCfg * ZhuanzhiEquipConfig::GetZhuanzhiEquipCfg(int equip_part, int equip_order)
{
	if (equip_part < 0 || equip_part >= Equipment::E_INDEX_MAX || equip_order <= 0 || equip_order > ItemPool::MAX_EQUIPMENT_ORDER)
	{
		return NULL;
	}

	long long key = ConvertParamToLongLong(equip_part, equip_order);
	EquipPutOnLimitConfigMapIt it = m_equip_puton_limit_cfg_map.find(key);
	if (it != m_equip_puton_limit_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const EquipSpecialEffectCfg * ZhuanzhiEquipConfig::GetEquipEffectCfg(int effect_type)
{
	auto it = std::find_if(m_special_effect_vec.begin(), m_special_effect_vec.end(), [effect_type](const EquipSpecialEffectCfg &cmp_item) {
		if (effect_type == cmp_item.effect_type)
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	if (it != m_special_effect_vec.end())
	{
		return &(*it);
	}

	return NULL;
}

const ZhuanzhiUpStarConfig * ZhuanzhiEquipConfig::GetUpStarCfg(int equip_index, int star_level)
{
	if (equip_index < 0 || equip_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX) return NULL;

	if (star_level < 0 || star_level > MAX_STAR_LEVEL) return NULL;

	if (star_level != m_up_star_list[equip_index][star_level].star_level) return NULL;

	return &m_up_star_list[equip_index][star_level];
}

const ZhuanzhiUpStarTotalConfig * ZhuanzhiEquipConfig::GetUpStarTotalCfg(int star_level, bool is_system_msg) 
{
	for (int i = m_up_star_total_cfg_count - 1; i >= 0; --i)
	{
		if (is_system_msg)
		{
			if (star_level == m_up_star_total_cfg_list[i].total_star)
			{
				return &m_up_star_total_cfg_list[i];
			}
		}
		else
		{
			if (star_level >= m_up_star_total_cfg_list[i].total_star)
			{
				return &m_up_star_total_cfg_list[i];
			}
		}
	}

	return NULL;
}

//附灵相关-----------------------------------------------------------------------------
int ZhuanzhiEquipConfig::InitFulingCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int equip_order = 0;
		if (!PugiGetSubNodeValue(dataElement, "equip_order", equip_order) || equip_order <= 0 || equip_order >= ItemPool::MAX_EQUIPMENT_ORDER)
		{
			return -1;
		}

		ZhuanzhiEquipFulingCfg &cfg = m_zhuanzhi_fuling_cfg_list[equip_order];
		cfg.equip_order = equip_order;

		if (!PugiGetSubNodeValue(dataElement, "fuling_max_count", cfg.fuling_max_count) || cfg.fuling_max_count <= 0)
		{
			return -2;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitFulingStuffCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{

		int seq = 0;
		if (!PugiGetSubNodeValue(dataElement, "seq", seq) || seq < 0 || seq >= MAX_ZHUANZHI_FULING_ATTR_COUNT)
		{
			return -1;
		}

		ZhuanzhiFulingStuffCfg &cfg = m_fuling_stuff_cfg_list[seq];
		cfg.seq = seq;

		if (!PugiGetSubNodeValue(dataElement, "stuff_id", cfg.stuff_id) || nullptr == ITEMPOOL->GetItem(cfg.stuff_id))
		{
			return -2;
		}

		// 加成的属性
		std::string temp_attr_type_str;
		if (!PugiGetSubNodeValue(dataElement, "add_attr_type", temp_attr_type_str))
		{
			return -3;
		}

		if (!CharIntAttrs::GetAttrTypeFromAttrString(temp_attr_type_str, &cfg.add_attr_type) || !CharIntAttrs::IsRoleBaseAttr(cfg.add_attr_type))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_attr_val", cfg.add_attr_val) || cfg.add_attr_val <= 0)
		{
			return -5;
		}


		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const ZhuanzhiEquipFulingCfg * ZhuanzhiEquipConfig::GetZhuanzhiFulingCfg(int equip_order)
{
	if (equip_order < 0 || equip_order >= ItemPool::MAX_EQUIPMENT_ORDER)
	{
		return NULL;
	}

	return &m_zhuanzhi_fuling_cfg_list[equip_order];
}

const ZhuanzhiFulingStuffCfg * ZhuanzhiEquipConfig::GetFulingStuffCfg(int seq)
{
	if (seq < 0 || seq >= MAX_ZHUANZHI_FULING_ATTR_COUNT)
	{
		return NULL;
	}

	return &m_fuling_stuff_cfg_list[seq];
}

//玉石相关-----------------------------------------------------------------------------
int ZhuanzhiEquipConfig::InitZhuanzhiStoneCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{

		ZhuanzhiStoneCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "item_id", cfg.item_id) || nullptr == ITEMPOOL->GetItem(cfg.item_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "stone_type", cfg.stone_type) || cfg.stone_type < ZHUANZHI_STONE_TYPE_RED || cfg.stone_type >= ZHUANZHI_STONE_TYPE_MAX)
		{
			return -2;
		}

		// 加成的属性1
		std::string temp_attr_type_str;
		if (!PugiGetSubNodeValue(dataElement, "attr_type1", temp_attr_type_str))
		{
			return -3;
		}

		if (!CharIntAttrs::GetAttrTypeFromAttrString(temp_attr_type_str, &cfg.attr_type1) || !CharIntAttrs::IsRoleBaseAttr(cfg.attr_type1))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "attr_val1", cfg.attr_val1) || cfg.attr_val1 <= 0)
		{
			return -5;
		}

		// 加成的属性2
		if (!PugiGetSubNodeValue(dataElement, "attr_type2", temp_attr_type_str))
		{
			return -6;
		}

		if ("0" != temp_attr_type_str)
		{
			if (!CharIntAttrs::GetAttrTypeFromAttrString(temp_attr_type_str, &cfg.attr_type2) || !CharIntAttrs::IsRoleBaseAttr(cfg.attr_type2))
			{
				return -7;
			}

			if (!PugiGetSubNodeValue(dataElement, "attr_val2", cfg.attr_val2) || cfg.attr_val2 <= 0)
			{
				return -8;
			}
		}

		if (!PugiGetSubNodeValue(dataElement, "level", cfg.level) || cfg.level <= 0 || cfg.level > MAX_ZHUANZHI_STONE_LEVEL)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "role_level_limit", cfg.role_level_limit) || cfg.role_level_limit < 0)
		{
			return -10;
		}

		std::map<int, ZhuanzhiStoneCfg>::const_iterator iter = m_zhuanzhi_stone_map.find(cfg.item_id);
		if (m_zhuanzhi_stone_map.end() != iter)
		{
			return -1000;
		}

		m_zhuanzhi_stone_map[cfg.item_id] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitZhuanzhiStoneTotalCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{

		ZhuanzhiStoneTotalCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "total_stone_level", cfg.total_stone_level) || cfg.total_stone_level < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -4;
		}

		m_zhuanzhi_stone_total_vec.push_back(cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitZhuanzhiStoneLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{

		ZhuanzhiStoneLevelCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "old_stone_item_id", cfg.old_stone_item_id) || nullptr == ITEMPOOL->GetItem(cfg.old_stone_item_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "new_stone_item_id", cfg.new_stone_item_id) || nullptr == ITEMPOOL->GetItem(cfg.new_stone_item_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_num", cfg.need_num) || cfg.need_num <= 0)
		{
			return -3;
		}

		m_zhuanzhi_stone_level_vec.push_back(cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitZhuanzhiStoneSlotCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{

		ZhuanzhiStoneSlotCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "part_index", cfg.part_index) || cfg.part_index < E_INDEX_ZHUANZHI::E_INDEX_WUQI || cfg.part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "slot_index", cfg.slot_index) || cfg.slot_index < 0 || cfg.slot_index >= MAX_ZHUANZHI_STONE_SLOT)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "stone_type_limit", cfg.stone_type_limit) || cfg.stone_type_limit < ZHUANZHI_STONE_TYPE_RED || cfg.stone_type_limit >= ZHUANZHI_STONE_TYPE_MAX)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "order_limit", cfg.order_limit) || cfg.order_limit < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "vip_level_limit", cfg.vip_level_limit) || cfg.vip_level_limit < 0)
		{
			return -5;
		}

		m_zhuanzhi_stone_slot_vec.push_back(cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitZhuanzhiStoneRefineCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{

		ZhuanzhiStoneRefineCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "refine_level", cfg.refine_level) || cfg.refine_level < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "consume_refine_val", cfg.consume_refine_val) || cfg.consume_refine_val <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "per_attr_add", cfg.per_attr_add) || cfg.per_attr_add < 0)
		{
			return -3;
		}

		m_zhuanzhi_stone_refine_vec.push_back(cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitRefineStuffCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{

		RefineStuffCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "part_index", cfg.part_index) || cfg.part_index < E_INDEX_ZHUANZHI::E_INDEX_WUQI || cfg.part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "seq", cfg.seq) || cfg.seq < 0 || cfg.seq > MAX_REFINE_STUFF_COUNT)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_id", cfg.stuff_id) || nullptr == ITEMPOOL->GetItem(cfg.stuff_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_refine", cfg.add_refine) || cfg.add_refine <= 0)
		{
			return -4;
		}

		m_refine_stuff_vec.push_back(cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitStoneResolveCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{

		StoneResolveCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "stone_id", cfg.stone_id) || nullptr == ITEMPOOL->GetItem(cfg.stone_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "resolve_get_score", cfg.resolve_get_score) || cfg.resolve_get_score <= 0)
		{
			return -2;
		}

		if (m_stone_resolve_map.find(cfg.stone_id) != m_stone_resolve_map.end())
		{
			return -100;
		}

		m_stone_resolve_map[cfg.stone_id] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitStoneConvertCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_seq = -1;
	while (!dataElement.empty())
	{

		StoneConvertCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "seq", cfg.seq) || cfg.seq < 0 || cfg.seq != last_seq + 1)
		{
			return -1;
		}

		last_seq = cfg.seq;

		if (!PugiGetSubNodeValue(dataElement, "convert_need_score", cfg.convert_need_score) || cfg.convert_need_score <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "convert_stone_id", cfg.convert_stone_id) || nullptr == ITEMPOOL->GetItem(cfg.convert_stone_id))
		{
			return -2;
		}

		if (m_stone_resolve_map.find(cfg.seq) != m_stone_resolve_map.end())
		{
			return -100;
		}

		m_stone_convert_map[cfg.seq] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const ZhuanzhiStoneCfg * ZhuanzhiEquipConfig::GetZhuanzhiStoneCfg(int stone_id)
{
	std::map<int, ZhuanzhiStoneCfg>::iterator iter = m_zhuanzhi_stone_map.find(stone_id);
	if (iter != m_zhuanzhi_stone_map.end())
	{
		return &iter->second;
	}

	return NULL;
}

const ZhuanzhiStoneTotalCfg *ZhuanzhiEquipConfig::GetZhuanzhiStoneTotalCfg(int total_stone_level)
{
	for (int i = (int)m_zhuanzhi_stone_total_vec.size() - 1; i >= 0 ; -- i)
	{
		if (total_stone_level >= m_zhuanzhi_stone_total_vec[i].total_stone_level)
		{
			return &m_zhuanzhi_stone_total_vec[i];
		}
	}
	
	return NULL;
}

const ZhuanzhiStoneLevelCfg * ZhuanzhiEquipConfig::GetZhuanzhiStoneLevelCfg(int old_stone_id)
{

	auto it = std::find_if(m_zhuanzhi_stone_level_vec.begin(), m_zhuanzhi_stone_level_vec.end(), [old_stone_id](const ZhuanzhiStoneLevelCfg &cmp_item) {
		if (old_stone_id == cmp_item.old_stone_item_id)
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	if (it != m_zhuanzhi_stone_level_vec.end())
	{
		return &(*it);
	}

	return NULL;
}

const ZhuanzhiStoneLevelCfg * ZhuanzhiEquipConfig::GetZhuanzhiStoneLevelReverseCfg(int new_stone_id)
{
	auto it = std::find_if(m_zhuanzhi_stone_level_vec.begin(), m_zhuanzhi_stone_level_vec.end(), [new_stone_id](const ZhuanzhiStoneLevelCfg &cmp_item) {
		if (new_stone_id == cmp_item.new_stone_item_id)
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	if (it != m_zhuanzhi_stone_level_vec.end())
	{
		return &(*it);
	}

	return NULL;
}

const ZhuanzhiStoneSlotCfg * ZhuanzhiEquipConfig::GetZhuanzhiStoneSlotCfg(int part_index, int slot_index)
{
	auto it = std::find_if(m_zhuanzhi_stone_slot_vec.begin(), m_zhuanzhi_stone_slot_vec.end(), [part_index, slot_index](const ZhuanzhiStoneSlotCfg &cmp_item) {
		if (part_index == cmp_item.part_index && slot_index == cmp_item.slot_index)
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	if (it != m_zhuanzhi_stone_slot_vec.end())
	{
		return &(*it);
	}

	return NULL;
}

const ZhuanzhiStoneRefineCfg * ZhuanzhiEquipConfig::GetZhuanzhiStoneRefineCfg(int refine_level)
{
	auto it = std::find_if(m_zhuanzhi_stone_refine_vec.begin(), m_zhuanzhi_stone_refine_vec.end(), [refine_level](const ZhuanzhiStoneRefineCfg &cmp_item) {
		if (refine_level == cmp_item.refine_level)
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	if (it != m_zhuanzhi_stone_refine_vec.end())
	{
		return &(*it);
	}

	return NULL;
}

const RefineStuffCfg * ZhuanzhiEquipConfig::GetRefineStuffCfg(int part_index, int seq)
{
	auto it = std::find_if(m_refine_stuff_vec.begin(), m_refine_stuff_vec.end(), [part_index, seq](const RefineStuffCfg &cmp_item) {
		if (part_index == cmp_item.part_index && seq == cmp_item.seq)
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	if (it != m_refine_stuff_vec.end())
	{
		return &(*it);
	}

	return NULL;
}

const StoneConvertCfg * ZhuanzhiEquipConfig::GetStoneConvertCfg(int seq)
{
	auto it = m_stone_convert_map.find(seq);
	if (it == m_stone_convert_map.end())
	{
		return nullptr;
	}

	return &it->second;
}

int ZhuanzhiEquipConfig::GetStoneResolveScore(int stone_id)
{
	auto it = m_stone_resolve_map.find(stone_id);
	if (it == m_stone_resolve_map.end())
	{
		return 0;
	}

	return it->second.resolve_get_score;
}

int ZhuanzhiEquipConfig::GetZhuanzhiStoneType(int stone_id)
{
	std::map<int, ZhuanzhiStoneCfg>::iterator iter = m_zhuanzhi_stone_map.find(stone_id);
	if (iter != m_zhuanzhi_stone_map.end())
	{
		return iter->second.stone_type;
	}

	return -1;
}

bool ZhuanzhiEquipConfig::IsVipLimitSlot(int part_index, int slot_index)
{
	const ZhuanzhiStoneSlotCfg *slot_cfg = this->GetZhuanzhiStoneSlotCfg(part_index, slot_index);
	if (nullptr == slot_cfg)
	{
		return false;
	}

	if (slot_cfg->vip_level_limit <= 0)
	{
		return false;
	}

	return true;
}

//套装相关-----------------------------------------------------------------------------

int ZhuanzhiEquipConfig::InitSuitGroupTypeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int equip_index = 0;
		if (!PugiGetSubNodeValue(dataElement, "equip_index", equip_index) || equip_index < E_INDEX_ZHUANZHI::E_INDEX_WUQI || equip_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX)
		{
			return -1;
		}

		SuitGroupTypeCfg &cfg = m_suit_group_cfg_list[equip_index];

		if (!PugiGetSubNodeValue(dataElement, "group_type", cfg.group_type) || cfg.group_type < ZHUANZHI_SUIT_GROUP_TYPE_FANG || cfg.group_type >= ZHUANZHI_SUIT_GROUP_TYPE_MAX)
		{
			return -2;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitSuitForgeLimitCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int suit_index = -1;
		if (!PugiGetSubNodeValue(data_element, "suit_index", suit_index) || suit_index <= ZHUANZHI_SUIT_TYPE_INVALID || suit_index >= ZHUANZHI_SUIT_TYPE_MAX)
		{
			return -1;
		}

		SuitForgeLimitCfg &cfg = m_suit_forge_limit_list[suit_index];

		if (!PugiGetSubNodeValue(data_element, "suit_min_color", cfg.suit_min_color) || cfg.suit_min_color <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "suit_min_order", cfg.suit_min_order) || cfg.suit_min_order <= 0)
		{
			return -3;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitOneEquipActConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		SuitOneEquipActiveCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "role_prof", cfg.role_prof) || cfg.role_prof < PROF_TYPE_PROF_1 || cfg.role_prof > PROF_TYPE_PROF_4)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "suit_index", cfg.suit_index) || cfg.suit_index <= ZHUANZHI_SUIT_TYPE_INVALID || cfg.suit_index >= ZHUANZHI_SUIT_TYPE_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "equip_part", cfg.equip_part) || cfg.equip_part < 0 || cfg.equip_part >= E_INDEX_MAX)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "equip_order", cfg.equip_order) || cfg.equip_order <= 0 || cfg.equip_order >= ItemPool::MAX_EQUIPMENT_ORDER)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff_1_id", cfg.stuff_1_id) || cfg.stuff_1_id < 0 || (cfg.stuff_1_id > 0 && NULL == ITEMPOOL->GetItem(cfg.stuff_1_id)))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff_1_num", cfg.stuff_1_num) || cfg.stuff_1_num < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff_2_id", cfg.stuff_2_id) || cfg.stuff_2_id < 0 || (cfg.stuff_2_id > 0 && NULL == ITEMPOOL->GetItem(cfg.stuff_2_id)))
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff_2_num", cfg.stuff_2_num) || cfg.stuff_2_num < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff_3_id", cfg.stuff_3_id) || cfg.stuff_3_id < 0 || (cfg.stuff_3_id > 0 && NULL == ITEMPOOL->GetItem(cfg.stuff_3_id)))
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff_3_num", cfg.stuff_3_num) || cfg.stuff_3_num < 0)
		{
			return -10;
		}

		m_suit_active_cfg_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitFangGroupAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		FangGroupAttrCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "role_prof", cfg.role_prof) || cfg.role_prof < PROF_TYPE_PROF_1 || cfg.role_prof > PROF_TYPE_PROF_4)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "suit_index", cfg.suit_index) || cfg.suit_index <= ZHUANZHI_SUIT_TYPE_INVALID || cfg.suit_index >= ZHUANZHI_SUIT_TYPE_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "equip_order", cfg.equip_order) || cfg.equip_order <= 0 || cfg.equip_order >= ItemPool::MAX_EQUIPMENT_ORDER)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "same_order_num", cfg.same_order_num) || cfg.same_order_num <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "per_maxhp", cfg.per_maxhp) || cfg.per_maxhp < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "per_gongji", cfg.per_gongji) || cfg.per_gongji < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "per_fangyu", cfg.per_fangyu) || cfg.per_fangyu < 0)
		{
			return -10;
		}

		m_suit_fang_group_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitGongGroupAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		GongGroupAttrCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "suit_index", cfg.suit_index) || cfg.suit_index <= ZHUANZHI_SUIT_TYPE_INVALID || cfg.suit_index >= ZHUANZHI_SUIT_TYPE_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "same_order_num", cfg.same_order_num) || cfg.same_order_num <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "per_maxhp", cfg.per_maxhp) || cfg.per_maxhp < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "per_fangyu", cfg.per_fangyu) || cfg.per_fangyu < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "per_gongji", cfg.per_gongji) || cfg.per_gongji < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "skill_zengshang_per", cfg.skill_zengshang_per) || cfg.skill_zengshang_per < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(data_element, "per_mianshang", cfg.per_mianshang) || cfg.per_mianshang < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(data_element, "per_pofang", cfg.per_pofang) || cfg.per_pofang < 0)
		{
			return -15;
		}

		m_suit_gong_group_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitZhizunGroupAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		ZhizunGroupAttrCfg cfg;
		if (!PugiGetSubNodeValue(data_element, "equip_order", cfg.equip_order) || cfg.equip_order <= 0 || cfg.equip_order >= ItemPool::MAX_EQUIPMENT_ORDER)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "pvp_zengshang_per", cfg.pvp_zengshang_per) || cfg.pvp_zengshang_per < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "pve_zengshang_per", cfg.pve_zengshang_per) || cfg.pve_zengshang_per < 0)
		{
			return -3;
		}

		m_suit_zhizun_group_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

const SuitOneEquipActiveCfg * ZhuanzhiEquipConfig::GetSuitOneEquipActCfg(int role_prof, int suit_index, int equip_part, int equip_order)
{
	auto it = std::find_if(m_suit_active_cfg_vec.begin(), m_suit_active_cfg_vec.end(), [role_prof, suit_index, equip_part, equip_order](const SuitOneEquipActiveCfg &cmp_item) {
		if (role_prof == cmp_item.role_prof && suit_index == cmp_item.suit_index && 
			equip_part == cmp_item.equip_part && equip_order == cmp_item.equip_order)
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	if (it != m_suit_active_cfg_vec.end())
	{
		return &(*it);
	}

	return NULL;
}

const SuitForgeLimitCfg * ZhuanzhiEquipConfig::GetSuitForgeLimitCfg(int suit_index)
{
	if (suit_index <= ZHUANZHI_SUIT_TYPE_INVALID || suit_index >= ZHUANZHI_SUIT_TYPE_MAX)
	{
		return nullptr;
	}

	return &m_suit_forge_limit_list[suit_index];
}

const FangGroupAttrCfg * ZhuanzhiEquipConfig::GetFangGroupAttrCfg(int role_prof, int suit_index, int equip_order, int same_order_num)
{
	for (int i = (int)m_suit_fang_group_vec.size() - 1; i >= 0; -- i)
	{
		if (role_prof == m_suit_fang_group_vec[i].role_prof &&
			suit_index == m_suit_fang_group_vec[i].suit_index &&
			equip_order == m_suit_fang_group_vec[i].equip_order &&
			same_order_num >= m_suit_fang_group_vec[i].same_order_num
			)
		{
			return &m_suit_fang_group_vec[i];
		}
	}

	return NULL;
}

const GongGroupAttrCfg * ZhuanzhiEquipConfig::GetGongGroupAttrCfg(int suit_index, int same_order_num)
{
	for (int i = (int)m_suit_gong_group_vec.size() - 1; i >= 0; -- i)
	{
		if (suit_index == m_suit_gong_group_vec[i].suit_index &&
			same_order_num >= m_suit_gong_group_vec[i].same_order_num
			)
		{
			return &m_suit_gong_group_vec[i];
		}
	}

	return NULL;
}

const ZhizunGroupAttrCfg * ZhuanzhiEquipConfig::GetZhizunGroupAttrCfg(int equip_order)
{
	for (int i = 0; i < (int)m_suit_zhizun_group_vec.size(); i ++)
	{
		if (equip_order == m_suit_zhizun_group_vec[i].equip_order)
		{
			return &m_suit_zhizun_group_vec[i];
		}
	}

	return NULL;
}

int ZhuanzhiEquipConfig::GetGroupType(int equip_index)
{
	if (equip_index < E_INDEX_ZHUANZHI::E_INDEX_WUQI || equip_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX)
	{
		return -1;
	}

	return m_suit_group_cfg_list[equip_index].group_type;
}

bool ZhuanzhiEquipConfig::IsActiveFangSuit(int role_prof, int suit_index, int equip_order, int same_order_num)
{
	for (int i = (int)m_suit_fang_group_vec.size() - 1; i >= 0; -- i)
	{
		if (role_prof == m_suit_fang_group_vec[i].role_prof &&
			suit_index == m_suit_fang_group_vec[i].suit_index &&
			equip_order == m_suit_fang_group_vec[i].equip_order &&
			same_order_num == m_suit_fang_group_vec[i].same_order_num
			)
		{
			return true;
		}
	}

	return false;
}


//至尊装备相关-----------------------------------------------------------------------------

int ZhuanzhiEquipConfig::InitZhiZunEquipCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		ZhiZunEquipCfg cfg;
		if (!PugiGetSubNodeValue(data_element, "equip_id", cfg.equip_id) || cfg.equip_id < 0 || (cfg.equip_id > 0 && NULL == ITEMPOOL->GetItem(cfg.equip_id)))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "equip_order", cfg.equip_order) || cfg.equip_order <= 0 || cfg.equip_order >= ItemPool::MAX_EQUIPMENT_ORDER)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "param1", cfg.param1) || cfg.param1 < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "param2", cfg.param2) || cfg.param2 < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "role_need_min_level", cfg.role_need_min_level) || cfg.role_need_min_level < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "role_need_min_prof_level", cfg.role_need_min_prof_level) || cfg.role_need_min_prof_level < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "add_cap", cfg.add_cap) || cfg.add_cap < 0)
		{
			return -8;
		}

		if (m_zhizun_cfg_map.end() != m_zhizun_cfg_map.find(cfg.equip_id))
		{
			return -100;
		}

		m_zhizun_cfg_map[cfg.equip_id] = cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitEquipZhiZunComposeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		EquipZhiZunComposeCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "compose_equip_id", cfg.compose_equip_id) || NULL == ITEMPOOL->GetItem(cfg.compose_equip_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "compose_equip_best_attr_num", cfg.compose_equip_best_attr_num) || cfg.compose_equip_best_attr_num <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff1_id", cfg.stuff1_id) || cfg.stuff1_id < 0 || (cfg.stuff1_id > 0 && NULL == ITEMPOOL->GetItem(cfg.stuff1_id)))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff1_num", cfg.stuff1_num) || cfg.stuff1_num < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "equip1_id", cfg.equip1_id) || cfg.equip1_id < 0 || (cfg.equip1_id > 0 && NULL == ITEMPOOL->GetItem(cfg.equip1_id)))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "equip1_num", cfg.equip1_num) || cfg.equip1_num < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "equip2_id", cfg.equip2_id) || cfg.equip2_id < 0 || (cfg.equip2_id > 0 && NULL == ITEMPOOL->GetItem(cfg.equip2_id)))
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "equip2_num", cfg.equip2_num) || cfg.equip2_num < 0)
		{
			return -8;
		}

		m_zhizun_compose_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitSpecialEquipCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		SpecialEquipCfg cfg;
		if (!PugiGetSubNodeValue(data_element, "equip_id", cfg.equip_id) || cfg.equip_id < 0 || (cfg.equip_id > 0 && NULL == ITEMPOOL->GetItem(cfg.equip_id)))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "role_need_min_level", cfg.role_need_min_level) || cfg.role_need_min_level < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "role_need_min_prof_level", cfg.role_need_min_prof_level) || cfg.role_need_min_prof_level < 0)
		{
			return -7;
		}

		// 不能与至尊装备id重复
		if (m_zhizun_cfg_map.end() != m_zhizun_cfg_map.find(cfg.equip_id))
		{
			return -100;
		}

		if (m_special_equip_cfg_map.end() != m_special_equip_cfg_map.find(cfg.equip_id))
		{
			return -101;
		}

		m_special_equip_cfg_map[cfg.equip_id] = cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitWakeLevelLimitCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_equip_grade = -1;
	int equip_grade = 0;
	wake_level_t max_level = 0;
	m_awakening_level_limit_vec.clear();
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "equip_grade", equip_grade) || equip_grade < 0 || equip_grade >= ItemPool::MAX_EQUIPMENT_ORDER)
		{
			return -1;
		}

		if (last_equip_grade + 1 != equip_grade)
		{
			return -2;
		}
		last_equip_grade = equip_grade;

		if (!PugiGetSubNodeValue(data_element, "max_level", max_level) || max_level < 0)
		{
			return -3;
		}
		m_awakening_level_limit_vec.push_back(max_level);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitWakeAttributeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	wake_type_t type = 0;
	wake_level_t level = 0;
	m_awakening_attribute_map.clear();
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "type", type) || type < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "level", level) || level < 0)
		{
			return -2;
		}
		wake_key_t key = this->MakeWakeTypeLevelKey(type, level);
		if (m_awakening_attribute_map.find(key) != m_awakening_attribute_map.end())
		{
			return -3;
		}
		AwakeningAttributeCfg cfg;
		if (!PugiGetSubNodeValue(data_element, "attribute_type", cfg.attribute_type) || !CharIntAttrs::IsRoleBaseAttr(cfg.attribute_type))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "attribute", cfg.attribute_value) || cfg.attribute_value < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "max_gongji", cfg.max_gongji_per) || cfg.max_gongji_per < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "max_fangyu", cfg.max_fangyu_per) || cfg.max_fangyu_per < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "max_maxhp", cfg.max_hp_per) || cfg.max_hp_per < 0)
		{
			return -8;
		}
		m_awakening_attribute_map[key] = cfg;

		data_element = data_element.next_sibling();
	}
	return 0;
}

int ZhuanzhiEquipConfig::InitWakeTypeWeightCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	const wake_level_t init_level = 0;
	int last_part_index = -1;
	int part_index = 0;
	wake_type_t type = 0;
	wake_weight_t weight = 0;
	int is_precious = 0;
	m_awakening_type_weight_vec.clear();
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "part", part_index) || part_index < 0 || part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX)
		{
			return -1;
		}
		
		if (!PugiGetSubNodeValue(data_element, "type", type) || type < 0)
		{
			return -2;
		}
		wake_key_t key = this->MakeWakeTypeLevelKey(type, init_level);
		if (m_awakening_attribute_map.find(key) == m_awakening_attribute_map.end())
		{
			return -20000;
		}
		if (!PugiGetSubNodeValue(data_element, "weight", weight) || weight < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "is_zhenxi", is_precious))
		{
			return -4;
		}

		if (last_part_index == part_index)
		{
			if (part_index < 0 || part_index >= (int)m_awakening_type_weight_vec.size())
			{
				return -5;
			}
			if (m_awakening_type_weight_vec[part_index].type_cfg_map.find(type) != m_awakening_type_weight_vec[part_index].type_cfg_map.end())
			{
				return -6;
			}
			m_awakening_type_weight_vec[part_index].type_cfg_map[type].weight = weight;
			m_awakening_type_weight_vec[part_index].type_cfg_map[type].is_precious = is_precious;
			m_awakening_type_weight_vec[part_index].total_weight += weight;
		}
		else
		{
			if (last_part_index + 1 != part_index)
			{
				return -7;
			}
			AwakeningTypeWeightCfg cfg;
			cfg.type_cfg_map[type].weight = weight;
			cfg.type_cfg_map[type].is_precious = is_precious;
			cfg.total_weight += weight;
			m_awakening_type_weight_vec.push_back(cfg);
		}
		last_part_index = part_index;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitWakeLevelWeightCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_level = -1;
	int level = 0;
	m_awakening_level_weight_cfg.level_weight_cfg_vec.clear();
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "level", level) || level < 0)
		{
			return -1;
		}

		if (last_level + 1 != level)
		{
			return -2;
		}
		last_level = level;
		AwakeningLevelWeightCfg::LevelRangeWeightCfg cfg;
		if (!PugiGetSubNodeValue(data_element, "level_left", cfg.level_left))
		{
			return -3;
		}
		if (!PugiGetSubNodeValue(data_element, "level_right", cfg.level_right))
		{
			return -4;
		}
		if (cfg.level_left > cfg.level_right)
		{
			return -5;
		}
		//不能随出0级
		if (level + cfg.level_left < 1)
		{
			return -6;
		}
		int ret = this->ReadList(data_element, "weight", cfg.weight, ",");
		if (ret < 0)
		{
			return -6 + ret;
		}
		size_t level_size = cfg.level_right - cfg.level_left + 1;
		if (level_size != cfg.weight.size())
		{
			return -10;
		}
		for (size_t i = 0; i < cfg.weight.size(); ++i)
		{
			if (cfg.weight[i] < 0)
			{
				return -20 - i;
			}
			cfg.total_weight += cfg.weight[i];
		}
		m_awakening_level_weight_cfg.level_weight_cfg_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitWakeLockConsumeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_lock_num = -1;
	int lock_num = 0;
	m_awakening_lock_consume_vec.clear();
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "lock_num", lock_num) || lock_num < 0 || lock_num > MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT)
		{
			return -1;
		}

		if (last_lock_num + 1 != lock_num)
		{
			return -2;
		}
		last_lock_num = lock_num;
		AwakeningLockConsumeCfg cfg;
		if (!PugiGetSubNodeValue(data_element, "consume_stuff_id", cfg.consume_stuff_id) || nullptr == ITEMPOOL->GetItem(cfg.consume_stuff_id))
		{
			return -3;
		}
		if (!PugiGetSubNodeValue(data_element, "consume_stuff_num", cfg.consume_stuff_num) || cfg.consume_stuff_num < 0)
		{
			return -4;
		}
		if (!PugiGetSubNodeValue(data_element, "lock_stuff_id", cfg.lock_stuff_id) || nullptr == ITEMPOOL->GetItem(cfg.lock_stuff_id))
		{
			return -5;
		}
		if (!PugiGetSubNodeValue(data_element, "lock_stuff_num", cfg.lock_stuff_num) || cfg.lock_stuff_num < 0)
		{
			return -6;
		}
		if (!PugiGetSubNodeValue(data_element, "consume_stuff_num_lock", cfg.consume_stuff_num_1) || cfg.consume_stuff_num_1 < 0)
		{
			return -7;
		}
		if (!PugiGetSubNodeValue(data_element, "gold_num", cfg.gold_num) || cfg.lock_stuff_num < 0)
		{
			return -8;
		}
		m_awakening_lock_consume_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitEquipComposeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	ItemID compose_equip_id = 0;
	int xianpin_num = 0;
	m_zhuanzhi_equip_compose_map.clear();
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "compose_equip_id", compose_equip_id) || nullptr == ITEMPOOL->GetItem(compose_equip_id))
		{
			return -1;
		}
		
		if (!PugiGetSubNodeValue(data_element, "compose_equip_best_attr_num", xianpin_num) || xianpin_num < 0 || xianpin_num >= ItemNamespace::MAX_XIANPIN_NUM)
		{
			return -2;
		}
		int key = this->MakeComposeKey(compose_equip_id, xianpin_num);
		if (m_zhuanzhi_equip_compose_map.find(key) != m_zhuanzhi_equip_compose_map.end())
		{
			return -3;
		}

		ZhuanzhiEquipComposeCfg cfg;
		if (!PugiGetSubNodeValue(data_element, "best_attr_count", cfg.best_attr_count) || cfg.best_attr_count < 0 || cfg.best_attr_count >= ItemNamespace::MAX_XIANPIN_NUM)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "color", cfg.color) || cfg.color <= ItemBase::I_COLOR_INVALID || cfg.color >= ItemBase::I_COLOR_MAX)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "is_broadcast", cfg.is_broadcast))
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "need_equip_num_min", cfg.need_equip_num_min) || cfg.need_equip_num_min < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "need_equip_num_max", cfg.need_equip_num_max) || cfg.need_equip_num_max < 0)
		{
			return -8;
		}
		
		if (cfg.need_equip_num_min > cfg.need_equip_num_max)
		{
			return -9;
		}
		size_t size = cfg.need_equip_num_max - cfg.need_equip_num_min + 1;
		int ret = this->ReadColumnList(data_element, "success_rate_", cfg.success_rate, size);
		if (ret < 0)
		{
			return -10 + ret;
		}
		size_t vec_size = cfg.success_rate.size();
		if (vec_size != size)
		{
			return -13;
		}
		for (size_t i = 0; i < vec_size; ++i)
		{
			if (cfg.success_rate[i] < 0)
			{
				return -14;
			}
		}
		m_zhuanzhi_equip_compose_map[key] = cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::InitConditionAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	const int max_column = 100;
	int ret = 0;
	int part_index = 0;
	int order = 0;
	m_condition_attr_cfg_map.clear();

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "part_index", part_index) || part_index < 0 || part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX)
		{
			return -1;
		}
		if (!PugiGetSubNodeValue(data_element, "order", order) || order < 0 || order >= ItemPool::MAX_EQUIPMENT_ORDER)
		{
			return -2;
		}

		int key = this->MakeConditionAttrKey(part_index, order);
		if (m_condition_attr_cfg_map.find(key) != m_condition_attr_cfg_map.end())
		{
			return -3;
		}

		ZhuanzhiEquipConditionAttrCfg cfg;
		if (!PugiGetSubNodeValue(data_element, "xianpin_num", cfg.xianpin_num) || cfg.xianpin_num < 0 || cfg.xianpin_num >= ItemNamespace::MAX_XIANPIN_NUM)
		{
			return -4;
		}

		ret = this->ReadColumnList(data_element, "attribute_type_", cfg.attribute_type, max_column);
		if (ret < 0)
		{
			return -4 + ret;
		}
		
		ret = this->ReadColumnList(data_element, "attribute_value_", cfg.attribute_value, max_column);
		if (ret < 0)
		{
			return -6 + ret;
		}
		size_t size = cfg.attribute_type.size();
		if (size == 0 || size != cfg.attribute_value.size())
		{
			return -9;
		}
		for (size_t i = 0; i < size; ++i)
		{
			if (!CharIntAttrs::IsRoleBaseAttr(cfg.attribute_type[i]))
			{
				return -10;
			}
			if (cfg.attribute_value[i] < 0)
			{
				return -11;
			}
		}
		m_condition_attr_cfg_map[key] = cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ZhuanzhiEquipConfig::MakeConditionAttrKey(int part_index, int order)
{
	return part_index * 10000 + order;
}

const ZhuanzhiEquipConditionAttrCfg * ZhuanzhiEquipConfig::GetConditionAttrCfg(int part_index, int order)
{
	int key = this->MakeConditionAttrKey(part_index, order);
	if (m_condition_attr_cfg_map.find(key) == m_condition_attr_cfg_map.end())
	{
		return nullptr;
	}
	return &m_condition_attr_cfg_map[key];
}

const ZhiZunEquipCfg * ZhuanzhiEquipConfig::GetZhiZunEquipCfg(ItemID equip_id)
{
	std::map<ItemID, ZhiZunEquipCfg>::iterator iter = m_zhizun_cfg_map.find(equip_id);
	if (iter != m_zhizun_cfg_map.end())
	{
		return &iter->second;
	}

	return NULL;
}

const EquipZhiZunComposeCfg * ZhuanzhiEquipConfig::GetEquipZhiZunComposeCfg(ItemID equip_id, int best_attr_num)
{
	auto it = std::find_if(m_zhizun_compose_vec.begin(), m_zhizun_compose_vec.end(), [equip_id, best_attr_num](const EquipZhiZunComposeCfg &cmp_item) {
		if (equip_id == cmp_item.compose_equip_id && best_attr_num == cmp_item.compose_equip_best_attr_num)
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	if (it != m_zhizun_compose_vec.end())
	{
		return &(*it);
	}

	return NULL;
}

bool ZhuanzhiEquipConfig::IsZhiZunEquip(ItemID equip_id)
{
	std::map<ItemID, ZhiZunEquipCfg>::iterator iter = m_zhizun_cfg_map.find(equip_id);
	if (iter != m_zhizun_cfg_map.end())
	{
		return true;
	}

	return false;
}

int ZhuanzhiEquipConfig::IsZhizunEquipSuit(ItemID equip_id1, ItemID equip_id2)
{
	int equip_order = -1;
	if (equip_id1 > 0 && equip_id1 > 0)
	{
		ZhiZunEquipCfg &active1_cfg = m_zhizun_cfg_map[equip_id1];
		ZhiZunEquipCfg &active2_cfg = m_zhizun_cfg_map[equip_id2];
		if (active1_cfg.equip_order == active2_cfg.equip_order)
		{
			equip_order = active1_cfg.equip_order;
		}
	}
	return equip_order;
}

const SpecialEquipCfg * ZhuanzhiEquipConfig::GetSpecialEquipCfg(ItemID equip_id)
{
	std::map<ItemID, SpecialEquipCfg>::iterator iter = m_special_equip_cfg_map.find(equip_id);
	if (iter != m_special_equip_cfg_map.end())
	{
		return &iter->second;
	}

	return NULL;
}

wake_level_t ZhuanzhiEquipConfig::GetWakeLevelLimitCfg(size_t equip_grade)
{
	if (equip_grade < 0 || equip_grade >= m_awakening_level_limit_vec.size())
	{
		return 0;
	}
	return m_awakening_level_limit_vec[equip_grade];
}

wake_key_t ZhuanzhiEquipConfig::MakeWakeTypeLevelKey(wake_type_t type, wake_level_t level)
{
	wake_key_t key = static_cast<wake_key_t>(type * 1000 + level);
	return key;
}

const AwakeningAttributeCfg *ZhuanzhiEquipConfig::GetWakeAttributeCfg(wake_type_t type, wake_level_t level)
{
	wake_key_t key = this->MakeWakeTypeLevelKey(type, level);
	if (m_awakening_attribute_map.find(key) == m_awakening_attribute_map.end())
	{
		return nullptr;
	}
	return &m_awakening_attribute_map[key];
}

AwakeningTypeWeightCfg ZhuanzhiEquipConfig::GetWakeTypeWeightCfg(size_t part_index)
{
	if (part_index < 0 || part_index >= m_awakening_type_weight_vec.size())
	{
		return AwakeningTypeWeightCfg();
	}
	return m_awakening_type_weight_vec[part_index];
}

int ZhuanzhiEquipConfig::WakeTypeIsPrecious(size_t part_index, wake_type_t type)
{
	if (part_index < 0 || part_index >= m_awakening_type_weight_vec.size() || m_awakening_type_weight_vec[part_index].type_cfg_map.find(type) == m_awakening_type_weight_vec[part_index].type_cfg_map.end())
	{
		return 0;
	}
	return m_awakening_type_weight_vec[part_index].type_cfg_map[type].is_precious;
}

const AwakeningLevelWeightCfg::LevelRangeWeightCfg * ZhuanzhiEquipConfig::GetWakeLevelWeightCfg(wake_level_t level)
{
	if (level < 0 || level >= m_awakening_level_weight_cfg.level_weight_cfg_vec.size())
	{
		return nullptr;
	}
	return &m_awakening_level_weight_cfg.level_weight_cfg_vec[level];
}

const AwakeningLockConsumeCfg * ZhuanzhiEquipConfig::GetWakeLockConsumeCfg(size_t lock_num)
{
	if (lock_num < 0 || lock_num >= m_awakening_lock_consume_vec.size())
	{
		return nullptr;
	}
	return &m_awakening_lock_consume_vec[lock_num];
}

int ZhuanzhiEquipConfig::MakeComposeKey(ItemID item_id, short xianpin_num)
{
	return xianpin_num * 100000 + item_id;
}

const ZhuanzhiEquipComposeCfg * ZhuanzhiEquipConfig::GetEquipComposeCfg(ItemID item_id, short xianpin_num)
{
	int key = this->MakeComposeKey(item_id, xianpin_num);
	if (m_zhuanzhi_equip_compose_map.find(key) == m_zhuanzhi_equip_compose_map.end())
	{
		return nullptr;
	}
	return &m_zhuanzhi_equip_compose_map[key];
}

