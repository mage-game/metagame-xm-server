#include <memory.h>
#include <stdlib.h>

#include "equipconfig.h"
#include "servercommon/servercommon.h"
#include "globalconfig/equipconfig.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "obj/character/attribute.hpp"
#include "gameworld/checkresourcecenter.hpp"

#include "gameworld/skill/skillpool.hpp"
#include "item/itemgriddata.h"
#include "equipment/equipment.h"
#include "monster/monsterpool.h"
#include <string>
#include <set>
#include "config/logicconfigmanager.hpp"
#include "other/feixian/reallyequipconfig.hpp"
#include <algorithm>

EquipConfig::EquipConfig() : m_strength_totallvl_cfg_count(0), m_stone_totallvl_cfg_count(0), m_xianpin_cfg_total_num(0)
{
	memset(m_gongji_xianpin_total_weight, 0 , sizeof(m_gongji_xianpin_total_weight));
	memset(m_fangyu_xianpin_taotal_weight, 0, sizeof(m_fangyu_xianpin_taotal_weight));
}

EquipConfig::~EquipConfig()
{
}

bool EquipConfig::Init(const std::string& configname, std::string* err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	TiXmlDocument document;

	if (configname == "" || !document.LoadFile(configname))
	{
		sprintf(errinfo,"%s: Load Equipment Global Config Error,\n %s.", configname.c_str(), document.ErrorDesc());
		*err = errinfo;
		return false;
	}

	TiXmlElement *RootElement = document.RootElement();

	if (NULL == RootElement)
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 强化基本配置
		TiXmlElement *strength_base_element = RootElement->FirstChildElement("strength_base");
		if (NULL == strength_base_element)
		{
			*err = configname + ": no [strength_base].";
			return false;
		}

		iRet = InitStrengthBaseCfg(strength_base_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitStrengthBaseCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 强化最小等级加成配置
		TiXmlElement *strength_minlevel_element = RootElement->FirstChildElement("strength_minlevel_reward");
		if (NULL == strength_minlevel_element)
		{
			*err = configname + ": no [strength_minlevel_reward].";
			return false;
		}

		iRet = InitStrengthTotallvlCfg(strength_minlevel_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitStrengthMinlvlCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 升星配置
	{
		// 强化最小等级加成配置
		TiXmlElement *up_star_element = RootElement->FirstChildElement("up_star");
		if (NULL == up_star_element)
		{
			*err = configname + ": no [up_star].";
			return false;
		}
	
		iRet = InitUpStarCfg(up_star_element);
		if(iRet)
		{
			sprintf(errinfo,"%s: InitUpStarCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 全身升星配置
		TiXmlElement *all_up_star_element = RootElement->FirstChildElement("total_upstar");
		if (NULL == all_up_star_element)
		{
			*err = configname + ": no [total_upstar].";
			return false;
		}

		iRet = InitUpStarTotalCfg(all_up_star_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitUpStarTotalCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

 	{
 		// 提升品质配置
 		TiXmlElement *up_quality_element = RootElement->FirstChildElement("up_quality");
 		if (NULL == up_quality_element)
 		{
 			*err = configname + ": no [up_quality].";
 			return false;
 		}
 
 		iRet = InitUpQualityCfg(up_quality_element);
 		if (iRet)
 		{
 			sprintf(errinfo,"%s: InitUpQualityCfg failed %d", configname.c_str(), iRet);
 			*err = errinfo;
 			return false;
 		}
 	}

	{
		// 神铸配置
		TiXmlElement *shen_op_element = RootElement->FirstChildElement("shen_op");
		if (NULL == shen_op_element)
		{
			*err = configname + ": no [shen_op].";
			return false;
		}

		iRet = InitShenOpCfg(shen_op_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitShenopCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 全身神铸配置
		TiXmlElement *all_shen_op_element = RootElement->FirstChildElement("all_shen_op");
		if (NULL == all_shen_op_element)
		{
			*err = configname + ": no [all_shen_op].";
			return false;
		}

		iRet = InitShenOpTotalCfg(all_shen_op_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitShenOpTotalCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}
	
 	{
 		// 升级配置
 		TiXmlElement *uplevel_element = RootElement->FirstChildElement("uplevel");
 		if (NULL == uplevel_element)
 		{
 			*err = configname + ": no [uplevel].";
 			return false;
 		}
 
 		iRet = InitUpLevelCfg(uplevel_element);
 		if (iRet)
 		{
 			sprintf(errinfo,"%s: InitUpLevelCfg failed %d", configname.c_str(), iRet);
 			*err = errinfo;
 			return false;
 		}
 	}

	//传承配置
// 	{
// 		TiXmlElement *inherit_element = RootElement->FirstChildElement("inherit");
// 		if (NULL == inherit_element)
// 		{
// 			*err = configname + ": no [inherit].";
// 			return false;
// 		}
// 
// 		iRet = InitInheritCfg(inherit_element);
// 		if (iRet)
// 		{
// 			sprintf(errinfo,"%s: InitInherit failed %d", configname.c_str(), iRet);
// 			*err = errinfo;
// 			return false;
// 		}
// 	}

	{
		// 宝石
		TiXmlElement *stone_element = RootElement->FirstChildElement("stone");
		if (NULL == stone_element)
		{
			*err = configname + ": no [stone].";
			return false;
		}

		iRet = InitStoneBaseCfg(stone_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitStoneBaseCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		TiXmlElement *elem = RootElement->FirstChildElement("stone_open_limit");
		if (NULL == elem)
		{
			*err = configname + ": no [stone_open_limit].";
			return false;
		}

		//iRet = InitStoneGridOpenLimit(elem);
		iRet = InitStoneGridOpenLimitByStrengthLevel(elem);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitStoneOpenLimit failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 宝石额外
		TiXmlElement *stone_ex_element = RootElement->FirstChildElement("stone_ex_add");
		if (NULL == stone_ex_element)
		{
			*err = configname + ": no [stone_ex_add].";
			return false;
		}

		iRet = InitStoneExAddCfg(stone_ex_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitStoneExAddCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

// 	{
// 		// 装备附灵
// 		TiXmlElement *equip_fuling_element = RootElement->FirstChildElement("equip_fuling");
// 		if (NULL == equip_fuling_element)
// 		{
// 			*err = configname + ": no [equip_fuling].";
// 			return false;
// 		}
// 
// 		iRet = InitFulingCfg(equip_fuling_element);
// 		if (iRet)
// 		{
// 			sprintf(errinfo,"%s: InitFulingCfg failed %d", configname.c_str(), iRet);
// 			*err = errinfo;
// 			return false;
// 		}
// 	}

// 	{
// 		// 装备附魔
// 		TiXmlElement *equip_fumo_element = RootElement->FirstChildElement("equip_fumo");
// 		if (NULL == equip_fumo_element)
// 		{
// 			*err = configname + ": no [equip_fumo].";
// 			return false;
// 		}
// 
// 		iRet = InitFumoCfg(equip_fumo_element);
// 		if (iRet)
// 		{
// 			sprintf(errinfo,"%s: InitFumoCfg failed %d", configname.c_str(), iRet);
// 			*err = errinfo;
// 			return false;
// 		}
// 	}

// 	{
// 		// 跨服装备
// 		TiXmlElement *cross_equip_element = RootElement->FirstChildElement("cross_equip");
// 		if (NULL == cross_equip_element)
// 		{
// 			*err = configname + ": no [cross_equip].";
// 			return false;
// 		}
// 
// 		iRet = InitCrossEquip(cross_equip_element);
// 		if (iRet)
// 		{
// 			sprintf(errinfo,"%s: InitCrossEquip failed %d", configname.c_str(), iRet);
// 			*err = errinfo;
// 			return false;
// 		}
// 	}

	{
		// 仙品属性数量
		TiXmlElement *xianpin_element = RootElement->FirstChildElement("xianpin_count");
		if (NULL == xianpin_element)
		{
			*err = configname + ": no [xianpin_count].";
			return false;
		}

		iRet = InitXianPinCountCfg(xianpin_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitXianPinCountCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 仙品属性, 仙品属性列表初始化必须在固定仙品前，顺序不能乱
		TiXmlElement *xianpin_element = RootElement->FirstChildElement("xianpin");
		if (NULL == xianpin_element)
		{
			*err = configname + ": no [xianpin].";
			return false;
		}

		iRet = InitXianPinCfg(xianpin_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitXianPin failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 固定仙品配置
		TiXmlElement *xianpin_element = RootElement->FirstChildElement("xianpin_fix");
		if (NULL == xianpin_element)
		{
			*err = configname + ": no [xianpin_fix].";
			return false;
		}

		iRet = InitXianPinFixCfg(xianpin_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitXianPinFixCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 掉落仙品配置
		TiXmlElement *xianpin_element = RootElement->FirstChildElement("xianpin_drop");
		if (NULL == xianpin_element)
		{
			*err = configname + ": no [xianpin_drop].";
			return false;
		}

		iRet = InitXianPinDropCfg(xianpin_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitXianPinDropCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 装备合成配置
		TiXmlElement *element = RootElement->FirstChildElement("equiment_compound_cfg");
		if (NULL == element)
		{
			*err = configname + ": no [equiment_compound_cfg].";
			return false;
		}

		iRet = InitEquipmentCompoundCfg(element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitEquipmentCompound failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 永恒装备
		TiXmlElement *element = RootElement->FirstChildElement("eternity_equip");
		if (NULL == element)
		{
			*err = configname + ": no [eternity_equip].";
			return false;
		}

		iRet = InitEternityCfg(element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitEternityCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 永恒套装
		TiXmlElement *element = RootElement->FirstChildElement("eternity_suit");
		if (NULL == element)
		{
			*err = configname + ": no [eternity_suit].";
			return false;
		}

		iRet = InitEternitySuitCfg(element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitEternitySuitCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

// 	{
// 		// 装备技能
// 		TiXmlElement *equip_skill_element = RootElement->FirstChildElement("equip_skill");
// 		if (NULL == equip_skill_element)
// 		{
// 			*err = configname + ": no [equip_skill].";
// 			return false;
// 		}
// 
// 		iRet = InitEquipSkillCfg(equip_skill_element);
// 		if (iRet)
// 		{
// 			sprintf(errinfo,"%s: InitEquipSkillCfg failed %d", configname.c_str(), iRet);
// 			*err = errinfo;
// 			return false;
// 		}
// 	}

// 	{
// 		// 装备成长
// 		TiXmlElement *equip_grow_up_element = RootElement->FirstChildElement("equip_grow_up");
// 		if (NULL == equip_grow_up_element)
// 		{
// 			*err = configname + ": no [equip_grow_up].";
// 			return false;
// 		}
// 
// 		iRet = InitEquipGrowUpConfig(equip_grow_up_element);
// 		if (iRet)
// 		{
// 			sprintf(errinfo,"%s: InitEquipGrowUpConfig failed %d", configname.c_str(), iRet);
// 			*err = errinfo;
// 			return false;
// 		}
// 	}

	{
		// 普通装备分解
		TiXmlElement *element = RootElement->FirstChildElement("decompose");
		if (NULL == element)
		{
			*err = configname + ": no [decompose].";
			return false;
		}

		iRet = InitDecomposeCfg(element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitEternitySuitCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 普通装备等级套装
		TiXmlElement *element = RootElement->FirstChildElement("uplevel_suit");
		if (NULL == element)
		{
			*err = configname + ": no [uplevel_suit].";
			return false;
		}

		iRet = InitEquipUpLevelSuitCfg(element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitEquipUpLevelSuitCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

long long EquipConfig::ConvertParamToLongLong(short param1, short param2, short param3, short param4) const
{
	ParamToLongLong ll;

	ll.m_param1 = param1;
	ll.m_param2 = param2;
	ll.m_param3 = param3;
	ll.m_param4 = param4;

	return ll.val;
}

// 强化相关
int EquipConfig::InitStrengthBaseCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		int equip_index = 0;
		if (!GetSubNodeValue(dataElement, "equip_index", equip_index) || equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX)
		{
			return -1001;
		}

		int strength_level = 0;
		if (!GetSubNodeValue(dataElement, "strength_level", strength_level) || strength_level <= 0 || strength_level > MAX_STRENGTH_LEVEL)
		{
			return -1002;
		}
		
		StrengthBaseConfig *cfg = &m_strength_base_cfg_list[equip_index][strength_level];
		
		if (0 != cfg->equip_index || 0 != cfg->strength_level) return -100;

		cfg->equip_index = equip_index;
		cfg->strength_level = strength_level;

		if (!GetSubNodeValue(dataElement, "need_order", cfg->need_order) || cfg->need_order < 0)// 改成用阶数来限制强化
		{
			return -208;
		}

		if (!GetSubNodeValue(dataElement, "stone_hole_num", cfg->stone_hole_num) || cfg->stone_hole_num < 0)
		{
			return -28;
		}

		if (!GetSubNodeValue(dataElement, "equip_level", cfg->equip_level) || cfg->equip_level <= 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "coin", cfg->coin) || cfg->coin < 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "stuff_id", cfg->stuff_id) || (NULL == ITEMPOOL->GetItem(cfg->stuff_id)))
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "stuff_count", cfg->stuff_count) || cfg->stuff_count <= 0)
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "base_succ_rate", cfg->base_succ_rate) || cfg->base_succ_rate <= 0 || cfg->base_succ_rate > 100)
		{
			return -8;
		}

		if (!GetSubNodeValue(dataElement, "is_downlevel_on_fail", cfg->is_downlevel_on_fail) || (cfg->is_downlevel_on_fail != 0 && cfg->is_downlevel_on_fail != 1) || (cfg->is_downlevel_on_fail == 1 && cfg->strength_level == 1))
		{
			return -9;
		}

		if (!GetSubNodeValue(dataElement, "lucky_stuff_id", cfg->protect_stuff_id) || (cfg->is_downlevel_on_fail == 1 && (0 == cfg->protect_stuff_id || NULL == ITEMPOOL->GetItem(cfg->protect_stuff_id))))
		{
			return -10;
		}

		if (!GetSubNodeValue(dataElement, "lucky_stuff_count", cfg->protect_stuff_count) || (cfg->is_downlevel_on_fail == 1 && 0 != cfg->protect_stuff_id && cfg->protect_stuff_count <= 0))
		{
			return -11;
		}

		if (!GetSubNodeValue(dataElement, "max_hp", cfg->add_maxhp) || cfg->add_maxhp < 0)
		{
			return -21;
		}

		if (!GetSubNodeValue(dataElement, "gongji", cfg->add_gongji) || cfg->add_gongji < 0)
		{
			return -22;
		}

		if (!GetSubNodeValue(dataElement, "fangyu", cfg->add_fangyu) || cfg->add_fangyu < 0)
		{
			return -23;
		}

		if (!GetSubNodeValue(dataElement, "mingzhong", cfg->add_mingzhong) || cfg->add_mingzhong < 0)
		{
			return -24;
		}

		if (!GetSubNodeValue(dataElement, "shanbi", cfg->add_shanbi) || cfg->add_shanbi < 0)
		{
			return -25;
		}

		if (!GetSubNodeValue(dataElement, "baoji", cfg->add_baoji) || cfg->add_baoji < 0)
		{
			return -26;
		}

		if (!GetSubNodeValue(dataElement, "jianren", cfg->add_jianren) || cfg->add_jianren < 0)
		{
			return -27;
		}

 		if (!GetSubNodeValue(dataElement, "is_broadcast", cfg->is_broadcast) || cfg->is_broadcast < 0)
 		{
 			return -28;
 		}

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int EquipConfig::InitStrengthTotallvlCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");

	m_strength_totallvl_cfg_count = 0;

	int last_strength_total_lvl = 0;

	while (NULL != dataElement)
	{
		if (m_strength_totallvl_cfg_count >= TOTAL_STRENGTH_LEVEL_CFG_MAX_COUNT)
		{
			return -10000;
		}

		StrengthTotalLvlAddConfig *cfg = &m_strength_totallvl_cfg_list[m_strength_totallvl_cfg_count];

		if (!GetSubNodeValue(dataElement, "seq", cfg->seq) || cfg->seq < 0 || cfg->seq >= TOTAL_STRENGTH_LEVEL_CFG_MAX_COUNT)
		{
			return -100;
		}

		if (!GetSubNodeValue(dataElement, "total_strength_level", cfg->total_strength_level) || cfg->total_strength_level <= last_strength_total_lvl)
		{
			return -1;
		}
		last_strength_total_lvl = cfg->total_strength_level;
	
		if (!GetSubNodeValue(dataElement, "maxhp", cfg->add_maxhp) || cfg->add_maxhp < 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "gongji", cfg->add_gongji) || cfg->add_gongji < 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "fangyu", cfg->add_fangyu) || cfg->add_fangyu < 0)
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "mingzhong", cfg->add_mingzhong) || cfg->add_mingzhong < 0)
		{
			return -6;
		}

		if (!GetSubNodeValue(dataElement, "shanbi", cfg->add_shanbi) || cfg->add_shanbi < 0)
		{
			return -7;
		}

		if (!GetSubNodeValue(dataElement, "baoji", cfg->add_baoji) || cfg->add_baoji < 0)
		{
			return -8;
		}

		if (!GetSubNodeValue(dataElement, "jianren", cfg->add_jianren) || cfg->add_jianren < 0)
		{
			return -9;
		}

		if (!GetSubNodeValue(dataElement, "is_broadcast", cfg->is_broadcast) || cfg->is_broadcast < 0 || cfg->is_broadcast >1)
		{
			return -10;
		}

		++ m_strength_totallvl_cfg_count;
		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int EquipConfig::InitUpStarCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		int equip_index = 0;
		if (!GetSubNodeValue(dataElement, "equip_index", equip_index) || equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX)
		{
			return -1;
		}

		int star_level = 0;
		if (!GetSubNodeValue(dataElement, "star_level", star_level) || star_level < 0 || star_level > MAX_STAR_LEVEL)
		{
			return -2;
		}

		UpStarConfig &cfg = m_up_star_list[equip_index][star_level];
		cfg.equip_index = equip_index;
		cfg.star_level = star_level;

		if (!GetSubNodeValue(dataElement, "equip_level", cfg.equip_level) || cfg.equip_level < 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "need_shengwang", cfg.need_mojing) || cfg.need_mojing < 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "maxhp", cfg.add_maxhp) || cfg.add_maxhp < 0)
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "gongji", cfg.add_gongji) || cfg.add_gongji < 0)
		{
			return -6;
		}

		if (!GetSubNodeValue(dataElement, "fangyu", cfg.add_fangyu) || cfg.add_fangyu < 0)
		{
			return -7;
		}

		if (!GetSubNodeValue(dataElement, "is_broadcast", cfg.is_broadcast) || cfg.is_broadcast < 0 || cfg.is_broadcast > 1)
		{
			return -8;
		}

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int EquipConfig::InitUpStarTotalCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");

	m_up_star_total_cfg_count = 0;
	int last_up_star_total_level = 0;

	while (NULL != dataElement)
	{
		if (m_up_star_total_cfg_count >= TOTAL_STAR_CFG_MAX_COUNT)
		{
			return -10000;
		}

		UpStarTotalConfig *cfg = &m_up_star_total_cfg_list[m_up_star_total_cfg_count];


		if (!GetSubNodeValue(dataElement, "seq", cfg->seq) || cfg->seq < 0 || cfg->seq >= TOTAL_STAR_CFG_MAX_COUNT)
		{
			return -1;
		}

		if (!GetSubNodeValue(dataElement, "total_star", cfg->total_star) || cfg->total_star <= last_up_star_total_level)
		{
			return -2;
		}
		last_up_star_total_level = cfg->total_star;

		if (!GetSubNodeValue(dataElement, "maxhp", cfg->add_maxhp) || cfg->add_maxhp < 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "gongji", cfg->add_gongji) || cfg->add_gongji < 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "fangyu", cfg->add_fangyu) || cfg->add_fangyu < 0)
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "is_broadcast", cfg->is_broadcast) || cfg->is_broadcast < 0 || cfg->is_broadcast >1)
		{
			return -6;
		}

		++ m_up_star_total_cfg_count;
		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

// 提升品质相关
 int EquipConfig::InitUpQualityCfg(TiXmlElement *RootElement)
 {
 	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
 	while (NULL != dataElement)
 	{
 		int equip_index = 0;
 		if (!GetSubNodeValue(dataElement, "equip_index", equip_index) || equip_index < 0 || equip_index >= Equipment::EQUIPMENT_TYPE_COUNT)
 		{
 			return -1;
 		}
 
 		int quality = 0;
 		if (!GetSubNodeValue(dataElement, "quality", quality) || quality < ItemBase::I_QUALITY_MIN || quality >= ItemBase::I_QUALITY_MAX * ItemBase::QUALITY_PER_COLOR)
 		{
 			return -2;
 		}
 
 		UpQualityConfig &cfg = m_up_quality_list[equip_index][quality];
 		if (0 != cfg.equip_index || 0 != cfg.quality)
 		{
 			return -100;
 		}
 
 		cfg.equip_index = equip_index;
 		cfg.quality = quality;
 
 		if (!GetSubNodeValue(dataElement, "equip_level", cfg.equip_level) || cfg.equip_level < 0)
 		{
 			return -3;
 		}
 
 		if (!GetSubNodeValue(dataElement, "coin", cfg.coin) || cfg.coin < 0)
 		{
 			return -4;
 		}
 
 		if (!GetSubNodeValue(dataElement, "stuff_id", cfg.stuff_id) || (NULL == ITEMPOOL->GetItem(cfg.stuff_id)))
 		{
 			return -5;
 		}
 
 		if (!GetSubNodeValue(dataElement, "stuff_count", cfg.stuff_count) || cfg.stuff_count <= 0)
 		{
 			return -6;
 		}
 
 		if (!GetSubNodeValue(dataElement, "attr_percent", cfg.attr_percent) || cfg.attr_percent < 0)
 		{
 			return -7;
 		}

		if (!GetSubNodeValue(dataElement, "color", cfg.color) || cfg.color < 0)
		{
			return -8;
		}

		if (!GetSubNodeValue(dataElement, "is_notice", cfg.is_notice))
		{
			return -9;
		}
 
 		dataElement = dataElement->NextSiblingElement();
 	}
 
 	return 0;
 }

// 神铸相关
int EquipConfig::InitShenOpCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		int equip_index = 0;
		if (!GetSubNodeValue(dataElement, "equip_index", equip_index) || equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX)
		{
			return -1;
		}

		int shen_level = 0;
		if (!GetSubNodeValue(dataElement, "shen_level", shen_level) || shen_level < 0 || shen_level > MAX_EQUIPMENT_SHEN_LEVEL)
		{
			return -2;
		}

		ShenOPConfig &cfg = m_shen_op_list[equip_index][shen_level];
		cfg.equip_index = equip_index;
		cfg.shen_level = shen_level;

		if (!GetSubNodeValue(dataElement, "equip_level", cfg.equip_level) || cfg.equip_level < 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "coin", cfg.coin) || cfg.coin < 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "stuff_id", cfg.stuff_id) || (NULL == ITEMPOOL->GetItem(cfg.stuff_id)))
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "stuff_count", cfg.stuff_count) || cfg.stuff_count <= 0)
		{
			return -6;
		}

		if (!GetSubNodeValue(dataElement, "attr_percent", cfg.attr_percent) || cfg.attr_percent < 0)
		{
			return -8;
		}

		if (!GetSubNodeValue(dataElement, "maxhp", cfg.add_maxhp) || cfg.add_maxhp < 0)
		{
			return -9;
		}

		if (!GetSubNodeValue(dataElement, "gongji", cfg.add_gongji) || cfg.add_gongji < 0)
		{
			return -10;
		}

		if (!GetSubNodeValue(dataElement, "fangyu", cfg.add_fangyu) || cfg.add_fangyu < 0)
		{
			return -11;
		}

		if (!GetSubNodeValue(dataElement, "mingzhong", cfg.add_mingzhong) || cfg.add_mingzhong < 0)
		{
			return -12;
		}

		if (!GetSubNodeValue(dataElement, "shanbi", cfg.add_shanbi) || cfg.add_shanbi < 0)
		{
			return -13;
		}

		if (!GetSubNodeValue(dataElement, "baoji", cfg.add_baoji) || cfg.add_baoji < 0)
		{
			return -14;
		}

		if (!GetSubNodeValue(dataElement, "jianren", cfg.add_jianren) || cfg.add_jianren < 0)
		{
			return -15;
		}

		if (!GetSubNodeValue(dataElement, "is_broadcast", cfg.is_broadcast) || cfg.is_broadcast < 0)
		{
			return -16;
		}

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int EquipConfig::InitShenOpTotalCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");

	m_shen_op_total_cfg_count = 0;
	int last_shen_op_total_level = 0;

	while (NULL != dataElement)
	{
		if (m_shen_op_total_cfg_count >= TOTAL_SHEN_OP_CFG_MAX_COUNT)
		{
			return -10000;
		}

		ShenOPTotalConfig *cfg = &m_shen_op_total_cfg_list[m_shen_op_total_cfg_count];

		if (!GetSubNodeValue(dataElement, "shen_level", cfg->shen_level) || cfg->shen_level <= last_shen_op_total_level)
		{
			return -1;
		}
		last_shen_op_total_level = cfg->shen_level;

		if (!GetSubNodeValue(dataElement, "maxhp", cfg->add_maxhp) || cfg->add_maxhp < 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "gongji", cfg->add_gongji) || cfg->add_gongji < 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "fangyu", cfg->add_fangyu) || cfg->add_fangyu < 0)
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "mingzhong", cfg->add_mingzhong) || cfg->add_mingzhong < 0)
		{
			return -6;
		}

		if (!GetSubNodeValue(dataElement, "shanbi", cfg->add_shanbi) || cfg->add_shanbi < 0)
		{
			return -7;
		}

		if (!GetSubNodeValue(dataElement, "baoji", cfg->add_baoji) || cfg->add_baoji < 0)
		{
			return -8;
		}

		if (!GetSubNodeValue(dataElement, "jianren", cfg->add_jianren) || cfg->add_jianren < 0)
		{
			return -9;
		}

		if (!GetSubNodeValue(dataElement, "is_broadcast", cfg->is_broadcast) || cfg->is_broadcast < 0 || cfg->is_broadcast >1)
		{
			return -10;
		}

		++ m_shen_op_total_cfg_count;
		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

//// 升级相关
//int EquipConfig::InitUpLevelCfg(TiXmlElement *RootElement) {
//	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
//	m_uplevel_map.clear();
//	while (NULL != dataElement)
//	{
//		UpLevelConfig cfg;
//
//		if (!GetSubNodeValue(dataElement, "old_equip_id", cfg.old_itemid) || (NULL == ITEMPOOL->GetItem(cfg.old_itemid)))
//		{
//			return -1;
//		}
//
//		if (!GetSubNodeValue(dataElement, "new_equip_id", cfg.new_itemid))
//		{
//			return -2;
//		}
//
//		if (!GetSubNodeValue(dataElement, "coin", cfg.coin) || cfg.coin < 0)
//		{
//			return -3;
//		}
//
//		if (!GetSubNodeValue(dataElement, "stuff_id", cfg.stuff_id) || (NULL == ITEMPOOL->GetItem(cfg.stuff_id)))
//		{
//			return -4;
//		}
//
//		if (!GetSubNodeValue(dataElement, "stuff_count", cfg.stuff_count) || cfg.stuff_count <= 0)
//		{
//			return -5;
//		}
//
//		if (!GetSubNodeValue(dataElement, "base_succ_rate", cfg.base_succ_rate) || cfg.base_succ_rate <= 0 || cfg.base_succ_rate > 100)
//		{
//			return -8;
//		}
//
//		if (!GetSubNodeValue(dataElement, "is_downlevel_on_fail", cfg.is_downlevel_on_fail))
//		{
//			return -9;
//		}
//
//		if (!GetSubNodeValue(dataElement, "lucky_stuff_id", cfg.protect_stuff_id) || cfg.protect_stuff_id < 0)
//		{
//			return -10;
//		}
//
//		if (!GetSubNodeValue(dataElement, "lucky_add_rate", cfg.protect_add_rate) || cfg.protect_add_rate < 0)
//		{
//			return -11;
//		}
//
//		if (!GetSubNodeValue(dataElement, "drop_equip_id", cfg.previous_itemid))
//		{
//			return -12;
//		}
//		if (cfg.previous_itemid !=0 && !ITEMPOOL->GetItem(cfg.previous_itemid))
//		{
//			return -13;
//		}
//
//		if (cfg.old_itemid == cfg.new_itemid)
//		{
//			return -14;
//		}
//
//		int key = cfg.old_itemid;
//		m_uplevel_map.insert(std::make_pair(key, cfg));
//
//		dataElement = dataElement->NextSiblingElement();
//	}
//
//	return 0;
//}
//int EquipConfig::InitUpLevelCfg( TiXmlElement *RootElement )
//{
//	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
//	while (NULL != dataElement)
//	{
//		int old_equip_id = 0;
//		int new_equip_id = 0;
//
//		const ItemBase *old_item_base = NULL;
//		const ItemBase *new_item_base = NULL;
//
//		if (!GetSubNodeValue(dataElement, "old_equip_id", old_equip_id))
//		{
//			return -1;
//		}
//
//		old_item_base = ITEMPOOL->GetItem(old_equip_id);
//		if (NULL == old_item_base || ItemBase::I_TYPE_EQUIPMENT != old_item_base->GetItemType())
//		{
//			return -11;
//		}
//
//		if (!GetSubNodeValue(dataElement, "new_equip_id", new_equip_id) || (NULL == ITEMPOOL->GetItem(new_equip_id)))
//		{
//			return -2;
//		}
//
//		new_item_base = ITEMPOOL->GetItem(new_equip_id);
//		if (NULL == new_item_base || ItemBase::I_TYPE_EQUIPMENT != new_item_base->GetItemType())
//		{
//			return -21;
//		}
//
//		if (new_item_base->GetColor() != old_item_base->GetColor())
//		{
//			return -22;
//		}
//
//		if (new_item_base->GetLimitLevel() <= old_item_base->GetLimitLevel())
//		{
//			return -23;
//		}
//
//		UpLevelConfig cfg;
//		cfg.m_old_equip_id = old_equip_id;
//		cfg.m_new_equip_id = new_equip_id;
//
//		if (!GetSubNodeValue(dataElement, "nv_wa_shi", cfg.m_nv_wa_shi) || cfg.m_nv_wa_shi <= 0)
//		{
//			return -4;
//		}
//
//		std::map<ItemID, UpLevelConfig>::const_iterator iter = m_uplevel_map.find(old_equip_id);
//
//		if (m_uplevel_map.end() != iter) return -100;
//
//		m_uplevel_map[old_equip_id] = cfg;
//
//		dataElement = dataElement->NextSiblingElement();
//	}
//
//	return 0;
//}

// 升级相关(合xxqy)
int EquipConfig::InitUpLevelCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		int old_equip_id = 0;
		int new_equip_id = 0;

		const ItemBase *old_item_base = NULL;
		const ItemBase *new_item_base = NULL;

		if (!GetSubNodeValue(dataElement, "old_equip_id", old_equip_id))
		{
			return -1;
		}

		old_item_base = ITEMPOOL->GetItem(old_equip_id);
		if (NULL == old_item_base || ItemBase::I_TYPE_EQUIPMENT != old_item_base->GetItemType())
		{
			return -11;
		}

		//if (old_item_base->GetColor() < ItemBase::I_COLOR_PURPLE)
		//{
		//	return -12;
		//}

		//if (old_item_base->GetLimitLevel() < 10)
		//{
		//	return -13;
		//}

		if (!GetSubNodeValue(dataElement, "new_equip_id", new_equip_id) || (NULL == ITEMPOOL->GetItem(new_equip_id)))
		{
			return -2;
		}

		new_item_base = ITEMPOOL->GetItem(new_equip_id);
		if (NULL == new_item_base || ItemBase::I_TYPE_EQUIPMENT != new_item_base->GetItemType())
		{
			return -21;
		}

		//if (new_item_base->GetColor() != old_item_base->GetColor())
		//{
		//	return -22;
		//}

		//if (new_item_base->GetLimitLevel() <= old_item_base->GetLimitLevel())
		//{
		//	return -23;
		//}

		UpLevelConfig cfg;
		cfg.m_old_equip_id = old_equip_id;
		cfg.m_new_equip_id = new_equip_id;

		//if (!GetSubNodeValue(dataElement, "coin", cfg.m_coin) || cfg.m_coin <= 0)
		//{
		//	return -4;
		//}

		if (!GetSubNodeValue(dataElement, "stuff_id_1", cfg.m_stuff_id_1) || (cfg.m_stuff_id_1 > 0 && NULL == ITEMPOOL->GetItem(cfg.m_stuff_id_1)))
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "stuff_count_1", cfg.m_stuff_count_1) || (cfg.m_stuff_id_1 > 0 && cfg.m_stuff_count_1 < 0))
		{
			return -6;
		}
		if (0 == cfg.m_stuff_count_1)
		{
			cfg.m_stuff_id_1 = 0;
		}

		if (!GetSubNodeValue(dataElement, "stuff_id_2", cfg.m_stuff_id_2) || (cfg.m_stuff_id_2 > 0 && NULL == ITEMPOOL->GetItem(cfg.m_stuff_id_2)))
		{
			return -7;
		}

		if (!GetSubNodeValue(dataElement, "stuff_count_2", cfg.m_stuff_count_2) || (cfg.m_stuff_id_2 > 0 && cfg.m_stuff_count_2 < 0))
		{
			return -8;
		}
		if (0 == cfg.m_stuff_count_2)
		{
			cfg.m_stuff_id_2 = 0;
		}

		if (!GetSubNodeValue(dataElement, "stuff_id_3", cfg.m_stuff_id_3) || (cfg.m_stuff_id_3 > 0 && NULL == ITEMPOOL->GetItem(cfg.m_stuff_id_3)))
		{
			return -9;
		}

		if (!GetSubNodeValue(dataElement, "stuff_count_3", cfg.m_stuff_count_3) || (cfg.m_stuff_id_3 > 0 && cfg.m_stuff_count_3 < 0))
		{
			return -10;
		}
		if (0 == cfg.m_stuff_count_3)
		{
			cfg.m_stuff_id_3 = 0;
		}

		if ((0 == cfg.m_stuff_id_1 && 0 == cfg.m_stuff_id_2 && 0 == cfg.m_stuff_id_3) ||
			(cfg.m_stuff_count_1 <= 0 && cfg.m_stuff_count_2 <= 0 && cfg.m_stuff_count_3 <= 0))
		{
			return -11;
		}

		//if (!GetSubNodeValue(dataElement, "protect_stuff_id", cfg.m_protect_id) ||
		//	(ItemBase::INVALID_ITEM_ID != cfg.m_protect_id && (NULL == ITEMPOOL->GetItem(cfg.m_protect_id))))
		//{
		//	return -12;
		//}

		//if (!GetSubNodeValue(dataElement, "strength_0_prob", cfg.m_strength_0_prob) || cfg.m_strength_0_prob < 0)
		//{
		//	return -13;
		//}

		//if (ItemBase::INVALID_ITEM_ID == cfg.m_protect_id && 100 != cfg.m_strength_0_prob)
		//{
		//	return -50;
		//}

		//if (!GetSubNodeValue(dataElement, "strength_1_prob", cfg.m_strength_1_prob) || cfg.m_strength_1_prob < 0)
		//{
		//	return -14;
		//}

		//if (!GetSubNodeValue(dataElement, "strength_2_prob", cfg.m_strength_2_prob) || cfg.m_strength_2_prob < 0)
		//{
		//	return -15;
		//}

		//if (100 != cfg.m_strength_0_prob + cfg.m_strength_1_prob + cfg.m_strength_2_prob)
		//{
		//	return -16;
		//}

		//cfg.m_strength_1_prob += cfg.m_strength_0_prob;
		//cfg.m_strength_2_prob += cfg.m_strength_1_prob;


		std::map<ItemID, UpLevelConfig>::const_iterator iter = m_uplevel_map.find(old_equip_id);

		if (m_uplevel_map.end() != iter) return -100;

		m_uplevel_map[old_equip_id] = cfg;

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int EquipConfig::InitInheritCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if(NULL != dataElement)
	{
		if (!GetSubNodeValue(dataElement, "coin", m_inherit_cfg.coin) || m_inherit_cfg.coin <= 0)
		{
			return -1;
		}

		if (!GetSubNodeValue(dataElement, "gold", m_inherit_cfg.gold) || m_inherit_cfg.gold <= 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "xygold", m_inherit_cfg.xy_gold) || m_inherit_cfg.xy_gold <= 0)
		{
			return -3;
		}
	}
	return 0;
}

int EquipConfig::InitStoneGridOpenLimit(TiXmlElement *RootElement)
{	
	TiXmlElement *element = RootElement->FirstChildElement("data");
	if (NULL == element)
	{
		return -10000;
	}

	int last_index = -1;
	while (NULL != element)
	{
		int index = -1;
		if (!GetSubNodeValue(element, "stone_index", index) || index < 0 || index != last_index + 1)
		{
			return -1;
		}
		StoneGridOpenLimit stone_limit;
		stone_limit.stone_index = index;
		last_index = index;

		if (!GetSubNodeValue(element, "limit", stone_limit.limit) || stone_limit.limit < 0 || stone_limit.limit >= STONE_LIMIT_TYPE_MAX)
		{
			return -2;
		}

		if (!GetSubNodeValue(element, "param1", stone_limit.param1) || stone_limit.param1 < 0)
		{
			return -3;
		}

		m_stone_grid_open_limit_vct.push_back(stone_limit);
		element = element->NextSiblingElement();
	}

	return 0;
}

int EquipConfig::InitStoneGridOpenLimitByStrengthLevel(TiXmlElement *RootElement)
{
	TiXmlElement *element = RootElement->FirstChildElement("data");
	if (NULL == element)
	{
		return -10000;
	}

	//int last_index = -1;
	while (NULL != element)
	{
		int index = -1;
		if (!GetSubNodeValue(element, "stone_index", index) || index < 0 /*|| index != last_index + 1*/)
		{
			return -1;
		}
		StoneGridOpenLimitByStrengthLevel stone_limit;
		stone_limit.stone_index = index;
		//last_index = index;

		if (!GetSubNodeValue(element, "limit", stone_limit.limit) || stone_limit.limit < 0 || stone_limit.limit >= STONE_LIMIT_TYPE_MAX)
		{
			return -2;
		}

		if (!GetSubNodeValue(element, "equip_index", stone_limit.equip_index) || stone_limit.equip_index < 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(element, "param1", stone_limit.param1) || stone_limit.param1 < 0)
		{
			return -4;
		}

		m_stone_grid_open_limit_by_strength_level_vct.push_back(stone_limit);
		element = element->NextSiblingElement();
	}

	return 0;
}

// 宝石相关
int EquipConfig::InitStoneBaseCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		StoneCfg cfg;
		int last_level_need_num = -1;
		if (!GetSubNodeValue(dataElement, "item_id", cfg.item_id) || (cfg.item_id <= 0 || NULL == ITEMPOOL->GetItem(cfg.item_id)))
		{
			return -1;
		}

		// 加成的属性1
		std::string temp_attr_type_str;
		if (!GetSubNodeValue(dataElement, "attr_type1", temp_attr_type_str))
		{
			return -2;
		}

		if (!CharIntAttrs::GetAttrTypeFromAttrString(temp_attr_type_str, &cfg.attr_type1) || !CharIntAttrs::IsRoleBaseAttr(cfg.attr_type1))
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "attr_val1", cfg.att_value1) || cfg.att_value1 <= 0)
		{
			return -4;
		}

		// 加成的属性2
		if (!GetSubNodeValue(dataElement, "attr_type2", temp_attr_type_str))
		{
			return -5;
		}

		if ("0" != temp_attr_type_str)
		{
			if (!CharIntAttrs::GetAttrTypeFromAttrString(temp_attr_type_str, &cfg.attr_type2) || !CharIntAttrs::IsRoleBaseAttr(cfg.attr_type2))
			{
				return -6;
			}

			if (!GetSubNodeValue(dataElement, "attr_val2", cfg.att_value2) || cfg.att_value2 <= 0)
			{
				return -7;
			}
		}

		if (!GetSubNodeValue(dataElement, "level", cfg.level) || cfg.level <= 0 || cfg.level > MAX_STONE_LEVEL)
		{
			return -8;
		}

		if (!GetSubNodeValue(dataElement, "stone_type", cfg.stone_type) || cfg.stone_type < 0 || cfg.stone_type >= MAX_STONE_COUNT)
		{
			return -9;
		}
		
		if (!GetSubNodeValue(dataElement, "level_need_num", cfg.level_need_num) || cfg.level_need_num < 0 || (last_level_need_num != -1 && last_level_need_num != cfg.level_need_num))
		{
			return -10;
		}

		last_level_need_num = cfg.level_need_num;

		std::map<int, StoneCfg>::const_iterator iter = m_stone_map.find(cfg.item_id);
		if (m_stone_map.end() != iter) 
		{
			return -1000;
		}

		m_stone_map[cfg.item_id] = cfg;
		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int EquipConfig::InitStoneExAddCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");

	m_stone_totallvl_cfg_count = 0;

	int last_seq = -1;
	int last_total_level = -1;

	while (NULL != dataElement)
	{
		if (m_stone_totallvl_cfg_count > TOTAL_STONE_LEVEL_CFG_MAX_COUNT)
		{
			return -1;	
		}

		StoneExAdd cfg;
		if (!GetSubNodeValue(dataElement, "seq", cfg.seq) || cfg.seq < 0 ||cfg.seq <= last_seq)
		{
			return -2;
		}
		last_seq = cfg.seq;

		if (!GetSubNodeValue(dataElement, "total_level", cfg.total_level) || cfg.total_level < 0 || cfg.total_level <= last_total_level) 
		{
			return -3;
		}
		last_total_level = cfg.total_level;

		if (!GetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0) 
		{	
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0) 
		{	
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0) 
		{	
			return -6;
		}

		if (!GetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) 
		{	
			return -7;
		}

		if (!GetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0) 
		{	
			return -8;
		}

		if (!GetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0) 
		{	
			return -9;
		}

		if (!GetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0) 
		{	
			return -10;
		}

		if (!GetSubNodeValue(dataElement, "is_broadcast", cfg.is_broadcast) || cfg.is_broadcast < 0 || cfg.is_broadcast > 1)
		{
			return -11;
		}

		m_stone_ex_add_list[m_stone_totallvl_cfg_count] = cfg;

		++ m_stone_totallvl_cfg_count;
		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int EquipConfig::InitCrossEquip(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");

	while (NULL != dataElement)
	{
		EquipCrossEquipConfig cfg;

		if (!GetSubNodeValue(dataElement, "item_id", cfg.item_id))
		{
			return -1;
		}

		const ItemBase *item_base = ITEMPOOL->GetItem(cfg.item_id);
		if (NULL == item_base)
		{
			return -2;
		}

		if (1 != item_base->GetPileLimit())
		{
			return -3;
		}

		if (!item_base->HasInvalidTime())
		{
			return -4;
		}

		if (m_cross_equip_map.end() != m_cross_equip_map.find(cfg.item_id))
		{
			return -5;
		}

		int tmp_val = 0;

		if (!GetSubNodeValue(dataElement, "maxhp", tmp_val) || tmp_val < 0) return -20;
		cfg.maxhp = tmp_val;

		if (!GetSubNodeValue(dataElement, "gongji", tmp_val) || tmp_val < 0) return -40;
		cfg.gongji = tmp_val;

		if (!GetSubNodeValue(dataElement, "fangyu", tmp_val) || tmp_val < 0) return -50;
		cfg.fangyu = tmp_val;

		if (!GetSubNodeValue(dataElement, "mingzhong", tmp_val) || tmp_val < 0) return -60;
		cfg.mingzhong = tmp_val;

		if (!GetSubNodeValue(dataElement, "shanbi", tmp_val) || tmp_val < 0) return -70;
		cfg.shanbi = tmp_val;

		if (!GetSubNodeValue(dataElement, "fujiashanghai", tmp_val) || tmp_val < 0) return -80;
		cfg.fujiashanghai = tmp_val;

		if (!GetSubNodeValue(dataElement, "dikangshanghai", tmp_val) || tmp_val < 0) return -90;
		cfg.dikangshanghai = tmp_val;

		m_cross_equip_map[cfg.item_id] = cfg;
		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int EquipConfig::InitFulingCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	
	int fuling_level = 1;
	while (NULL != dataElement)
	{
		if (fuling_level > MAX_FULIN_MAX_LEVEL)
		{
			return -1;
		}

		EquipFulingConfig *cfg = &m_fuling_list[fuling_level];

		if (!GetSubNodeValue(dataElement, "fuling_level", cfg->fuling_level) || cfg->fuling_level != fuling_level) 
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "equip_level_limit", cfg->equip_level_limit) || cfg->equip_level_limit < 0) 
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "equip_color_limit", cfg->equip_color_limit) || cfg->equip_color_limit < Equipment::I_COLOR_PURPLE) 
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "wuqi_stuff_id", cfg->wuqi_stuff_id) || NULL == ITEMPOOL->GetItem(cfg->wuqi_stuff_id)) 
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "wuqi_stuff_count", cfg->wuqi_stuff_count) || cfg->wuqi_stuff_count <= 0) 
		{
			return -6;
		}

		if (!GetSubNodeValue(dataElement, "shipin_stuff_id", cfg->shipin_stuff_id) || NULL == ITEMPOOL->GetItem(cfg->shipin_stuff_id)) 
		{
			return -7;
		}

		if (!GetSubNodeValue(dataElement, "shipin_stuff_count", cfg->shipin_stuff_count) || cfg->shipin_stuff_count <= 0) 
		{
			return -8;
		}

		if (!GetSubNodeValue(dataElement, "hujia_stuff_id", cfg->hujia_stuff_id) || NULL == ITEMPOOL->GetItem(cfg->hujia_stuff_id)) 
		{
			return -9;
		}

		if (!GetSubNodeValue(dataElement, "hujia_stuff_count", cfg->hujia_stuff_count) || cfg->hujia_stuff_count <= 0) 
		{
			return -10;
		}

		if (!GetSubNodeValue(dataElement, "fangju_stuff_id", cfg->fangju_stuff_id) || NULL == ITEMPOOL->GetItem(cfg->fangju_stuff_id)) 
		{
			return -11;
		}

		if (!GetSubNodeValue(dataElement, "fangju_stuff_count", cfg->fangju_stuff_count) || cfg->fangju_stuff_count <= 0) 
		{
			return -12;
		}

		if (!GetSubNodeValue(dataElement, "coin", cfg->coin) || cfg->coin <= 0) 
		{
			return -13;
		}

		if (!GetSubNodeValue(dataElement, "gongji_per", cfg->gongji_per) || cfg->gongji_per <= 0) 
		{
			return -14;
		}

		if (!GetSubNodeValue(dataElement, "gongji", cfg->gongji) || cfg->gongji <= 0) 
		{
			return -15;
		}

		if (!GetSubNodeValue(dataElement, "fangyu_per", cfg->fangyu_per) || cfg->fangyu_per <= 0) 
		{
			return -16;
		}

		if (!GetSubNodeValue(dataElement, "fangyu", cfg->fangyu) || cfg->fangyu <= 0) 
		{
			return -17;
		}

		if (!GetSubNodeValue(dataElement, "is_broadcast", cfg->is_broadcast) || cfg->is_broadcast < 0) 
		{
			return -17;
		}
		
		++ fuling_level;
		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int EquipConfig::InitFumoCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");

	while (NULL != dataElement)
	{
		int id = 0;
		if (!GetSubNodeValue(dataElement, "id", id) || id <= 0) 
		{
			return -1;
		}

		EquipFumoConfig &cfg = m_fumo_map[id];
		cfg.id = id;

		if (!GetSubNodeValue(dataElement, "equip_sub_type", cfg.equip_sub_type) || cfg.equip_sub_type < Equipment::E_TYPE_MIN || cfg.equip_sub_type >= Equipment::E_TYPE_MAX) 
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "time", cfg.time) || cfg.time <= 0) 
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0) 
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "max_hp", cfg.max_hp) || cfg.max_hp < 0) 
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0) 
		{
			return -6;
		}

		if (!GetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) 
		{
			return -7;
		}

		if (!GetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0) 
		{
			return -8;
		}

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int EquipConfig::InitXianPinCountCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -10000;
	}

	while (NULL != dataElement)
	{
		int equip_color = 0;
		int xianpin_num = 0;
		if (!GetSubNodeValue(dataElement, "equip_color", equip_color) || equip_color <= ItemBase::I_COLOR_INVALID || equip_color >= ItemBase::I_COLOR_MAX) 
		{
			return -1;
		}

		if (!GetSubNodeValue(dataElement, "xianpin_num", xianpin_num) || xianpin_num < 0 || 
			xianpin_num > ItemNamespace::MAX_XIANPIN_NUM) 
		{
			return -2;
		}

//		if (!GetSubNodeValue(dataElement, "hight_quality_num", hight_quality_num) || hight_quality_num < 0 || 
//			hight_quality_num > ItemNamespace::MAX_XIANPIN_NUM || xianpin_num < hight_quality_num) 
//		{
//			return -3;
//		}

		m_xianpin_count_cfg[equip_color].xianpin_count = xianpin_num;
//		m_xianpin_count_cfg[equip_color].hight_quality_count = hight_quality_num;

		dataElement = dataElement->NextSiblingElement();
	}
	return 0;
}

int EquipConfig::InitXianPinCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -10000;
	}
	
	while (NULL != dataElement)
	{
		if ( MAX_XIANPING_CFG_NUM <= m_xianpin_cfg_total_num)
		{
			return -1;
		}
		
		int xianpin_type = 0;
		if (!GetSubNodeValue(dataElement, "xianpin_type", xianpin_type) || xianpin_type <= 0 || xianpin_type >= MAX_XIANPING_CFG_NUM || xianpin_type != m_xianpin_cfg_total_num + 1) 
		{
			return -2;
		}

		EquipXianPinConfig *xianpin_cfg = &m_xianpin_cfg_list[xianpin_type];
		xianpin_cfg->xianpin_type = xianpin_type;

		if (!GetSubNodeValue(dataElement, "big_type", xianpin_cfg->big_type) || BIG_TYPE_GONGJI > xianpin_cfg->big_type || BIG_TYPE_ALL < xianpin_cfg->big_type) 
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "add_value", xianpin_cfg->add_value) || xianpin_cfg->add_value <= 0) 
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "weight", xianpin_cfg->weight) || xianpin_cfg->weight < 0) 
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "shuxing_type", xianpin_cfg->shuxing_type) || xianpin_cfg->shuxing_type <= Equipment::E_XIANPIN_INVALID || xianpin_cfg->shuxing_type >= Equipment::E_XIANPIN_MAX) 
		{
			return -6;
		}

		if (!GetSubNodeValue(dataElement, "equip_color", xianpin_cfg->equip_color) || xianpin_cfg->equip_color <= ItemBase::I_COLOR_INVALID || xianpin_cfg->equip_color >= ItemBase::I_COLOR_MAX) 
		{
			return -7;
		}

		if (!GetSubNodeValue(dataElement, "color", xianpin_cfg->color) || xianpin_cfg->color < QUALITY_COLOR_TYPE_INVALID || xianpin_cfg->color > QUALITY_COLOR_TYPE_MAX)
		{
			return -8;
		}

		if (!GetSubNodeValue(dataElement, "score", xianpin_cfg->score) || (xianpin_cfg->score < 0))
		{
			return -9;
		}

		if (xianpin_cfg->big_type == BIG_TYPE_GONGJI) 
		{
			m_gongji_xianpin_total_weight[xianpin_cfg->equip_color] += xianpin_cfg->weight;
		}
		else if (xianpin_cfg->big_type == BIG_TYPE_FANGYU) 
		{
			m_fangyu_xianpin_taotal_weight[xianpin_cfg->equip_color] += xianpin_cfg->weight;
		}
		// 攻击、防御类型的装备都可以有这种仙品属性
		else if (xianpin_cfg->big_type == BIG_TYPE_ALL)
		{
			m_gongji_xianpin_total_weight[xianpin_cfg->equip_color] += xianpin_cfg->weight;
			m_fangyu_xianpin_taotal_weight[xianpin_cfg->equip_color] += xianpin_cfg->weight;
		}
			
		++m_xianpin_cfg_total_num;
		dataElement = dataElement->NextSiblingElement();
	}
	return 0;
}

const StrengthBaseConfig * EquipConfig::GetStrengthBaseCfg(int equip_index, short strength_level) const
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return NULL;

	if (strength_level < 0 || strength_level > MAX_STRENGTH_LEVEL) return NULL;

	if (strength_level != m_strength_base_cfg_list[equip_index][strength_level].strength_level) return NULL;	// 检查当前的需要强化的等级与强化配置表里面对应的等级是否相等

	return &m_strength_base_cfg_list[equip_index][strength_level];
}

const int EquipConfig::GetStrengthMaxValue(int equip_type, short order) const
{
	if (equip_type < 0 || equip_type >= Equipment::E_INDEX_MAX) return -1;

	for(int max_strength_level = MAX_STRENGTH_LEVEL; max_strength_level > 0; max_strength_level--)
	{
		int need_order = m_strength_base_cfg_list[equip_type][max_strength_level].need_order;
		if (0 == need_order || order < need_order)
		{
			continue;
		}
		else
		{
			return max_strength_level;	
		}
	}

	return -1;
}

const StrengthTotalLvlAddConfig * EquipConfig::GetStrengthTotallvlAddCfg(short total_strength_level) const
{
	for (int i = m_strength_totallvl_cfg_count - 1; i >= 0; -- i)
	{
		if (total_strength_level >= m_strength_totallvl_cfg_list[i].total_strength_level)
		{
			return &m_strength_totallvl_cfg_list[i];
		}
	}

	return NULL;
}

const UpStarConfig * EquipConfig::GetUpStarCfg(int equip_index, int star_level) const
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return NULL;

	if (star_level < 0 || star_level > MAX_STAR_LEVEL) return NULL;

	if (star_level != m_up_star_list[equip_index][star_level].star_level) return NULL;

	return &m_up_star_list[equip_index][star_level];
}

 const UpQualityConfig * EquipConfig::GetUpQualityCfg(int equip_type, int quality) const
 {
 	if (equip_type < Equipment::E_TYPE_MIN || equip_type >= Equipment::E_TYPE_MAX) return NULL;
 
 	if (quality < ItemBase::I_QUALITY_MIN || quality >= ItemBase::I_QUALITY_MAX * ItemBase::QUALITY_PER_COLOR) return NULL;
 
 	if (quality != m_up_quality_list[equip_type - Equipment::E_TYPE_MIN][quality].quality) return NULL;
 
 	return &m_up_quality_list[equip_type - Equipment::E_TYPE_MIN][quality];
 }

const ShenOPConfig * EquipConfig::GetShenOPCfg(int equip_index, int shen_level) const
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return NULL;

	if (shen_level < 0 || shen_level > MAX_EQUIPMENT_SHEN_LEVEL) return NULL;

	if (shen_level != m_shen_op_list[equip_index][shen_level].shen_level) return NULL;

	return &m_shen_op_list[equip_index][shen_level];
}

const ShenOPTotalConfig * EquipConfig::GetShenOPTotalCfg(int shen_level, bool is_system_msg) const
{
	for (int i = m_shen_op_total_cfg_count - 1; i >= 0; -- i)
	{
		if (is_system_msg)
		{
			if (shen_level == m_shen_op_total_cfg_list[i].shen_level)
			{
				return &m_shen_op_total_cfg_list[i];
			}
		}
		else
		{
			if (shen_level >= m_shen_op_total_cfg_list[i].shen_level)
			{
				return &m_shen_op_total_cfg_list[i];
			}
		}
	}

	return NULL;
}

const UpStarTotalConfig * EquipConfig::GetUpStarTotalCfg(int star_level, bool is_system_msg) const
{
	for (int i = m_up_star_total_cfg_count - 1; i >= 0; -- i)
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
/*
const UpLevelConfig * EquipConfig::GetUpLevelCfg(int equip_index, short up_level) const
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return NULL;

	if (up_level < 0 || up_level > MAX_STRENGTH_LEVEL) return NULL;

	if (up_level != m_up_level_cfg_list[equip_index][up_level].up_level) return NULL;	// 检查当前的需要强化的等级与强化配置表里面对应的等级是否相等

	return &m_up_level_cfg_list[equip_index][up_level];
}
*/
const UpLevelConfig * EquipConfig::GetUpLevelCfg(ItemID old_equip_id) const
{
	std::map<ItemID, UpLevelConfig>::const_iterator iter = m_uplevel_map.find(old_equip_id);

	if (m_uplevel_map.end() == iter) return NULL;

	return &iter->second;
}

const StoneCfg * EquipConfig::GetStoneCfg(int item_id) const
{
	std::map<int, StoneCfg>::const_iterator iter = m_stone_map.find(item_id);

	if (m_stone_map.end() == iter) 
	{
		return NULL;
	}

	return &iter->second;
}

const StoneExAdd * EquipConfig::GetStoneExAdd(int stone_level) const
{
	for (int i = m_stone_totallvl_cfg_count - 1; i >= 0; --i)
	{
		if (stone_level >= m_stone_ex_add_list[i].total_level)
		{
			return &m_stone_ex_add_list[i];
		}
	}

	return NULL;
}

const StoneGridOpenLimit * EquipConfig::GetStoneOpenLimit(int index) const
{
	if (index < 0 || index >= MAX_STONE_COUNT)
	{
		return NULL;
	}

	for (int i = 0; i < static_cast<int>(m_stone_grid_open_limit_vct.size()); i ++)
	{
		if (m_stone_grid_open_limit_vct[i].stone_index == index)
		{
			return &m_stone_grid_open_limit_vct[i];
		}
	}

	return NULL;
}

const StoneGridOpenLimitByStrengthLevel * EquipConfig::GetStoneOpenLimitByEquipIndex(int stone_index, int equip_index) const
{
	if (stone_index < 0 || stone_index >= MAX_STONE_COUNT)
	{
		return NULL;
	}

	for (int i = 0; i < static_cast<int>(m_stone_grid_open_limit_by_strength_level_vct.size()); i++)
	{
		if (m_stone_grid_open_limit_by_strength_level_vct[i].stone_index == stone_index && m_stone_grid_open_limit_by_strength_level_vct[i].equip_index == equip_index)
		{
			return &m_stone_grid_open_limit_by_strength_level_vct[i];
		}
	}

	return NULL;
}

const EquipFulingConfig *EquipConfig::GetFulingCfg(int fuling_level) const
{
	if (fuling_level <= 0 || fuling_level > MAX_FULIN_MAX_LEVEL)
	{
		return NULL;
	}

	return &m_fuling_list[fuling_level];
}

const EquipCrossEquipConfig * EquipConfig::GetCrossEquipCfg(ItemID item_id) const
{
	CrossEquipMapIt it = m_cross_equip_map.find(item_id);
	if (m_cross_equip_map.end() != it)
	{
		return &it->second;
	}

	return NULL;
}

const EquipXianPinCountConfig * EquipConfig::GetXianPinCountCfg(int color) const
{
	if (color <= ItemBase::I_COLOR_INVALID || color >= ItemBase::I_COLOR_MAX) return NULL;

	return &m_xianpin_count_cfg[color];
}

const EquipXianPinConfig * EquipConfig::GetXianPinEquipCfg(int xianpin_type) const
{
	if (xianpin_type <= 0 || xianpin_type > m_xianpin_cfg_total_num ||  xianpin_type >= MAX_XIANPING_CFG_NUM)
	{
		return NULL;
	}

	return &m_xianpin_cfg_list[xianpin_type];
}

const EquipFumoConfig * EquipConfig::GetFumoEquipCfg(int id) const
{
	std::map<int, EquipFumoConfig>::const_iterator iter = m_fumo_map.find(id);
	if (m_fumo_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

const bool EquipConfig::GetRandomXianpin(ItemGridData *item_grid_data, int getway, int getway_param1, int fix_best_attr_num) const
{
	if (NULL == item_grid_data)
	{
		return false;
	}

	const Equipment* equip_item = (const Equipment *)ITEMPOOL->GetItem(item_grid_data->item_id);
	if (NULL == equip_item || ItemBase::I_TYPE_EQUIPMENT != equip_item->GetItemType())
	{
		return false;
	}

	short equip_type = equip_item->GetEquipType();
	if (equip_type < Equipment::E_TYPE_ZHUANZHI_MIN || equip_type >= Equipment::E_TYPE_ZHUANZHI_MAX)
	{
		return false;
	}

	// 固定获得方式
	if (getway > Equipment::EQUIPMENT_FIX_GETWAY_MIN && getway < Equipment::EQUIPMENT_FIX_GETWAY_MAX)
	{
		if (this->GetFixedXianPinType(item_grid_data, getway, getway_param1))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	// 其他获得方式
// 	const int big_type = (Equipment::IsWuqiType(equip_item->IsGongjiType(equip_type))) ? 0: 1;
// 	int total_weight = big_type == 0? m_gongji_xianpin_total_weight[equip_item->GetColor()] : m_fangyu_xianpin_taotal_weight[equip_item->GetColor()];
// 	if (total_weight <= 0) 
// 	{
// 		return false;
// 	}

	// 目前不分大类型，直接获取一个总权重
	int total_weight = m_gongji_xianpin_total_weight[equip_item->GetColor()];
	if (total_weight <= 0)
	{
		return false;
	}

	int rand_value = 0;
	short xianpin_num = 0;
	EquipmentParam *equip_param = (EquipmentParam *)item_grid_data->param;
	memset(equip_param->xianpin_type_list, 0, sizeof(equip_param->xianpin_type_list));
	
	// 仙品条数
	const EquipXianPinCountConfig * xianpin_count_cfg = this->GetXianPinCountCfg(equip_item->GetColor());
	if (NULL == xianpin_count_cfg) return false;

	if (fix_best_attr_num > 0)
	{
		xianpin_num = fix_best_attr_num;
	}
	else
	{
		xianpin_num = xianpin_count_cfg->xianpin_count;
	}
	
	if (xianpin_num <= 0)
	{
		return false;
	}
	
	// 随机仙品
	{
		unsigned short star_num = 0;   // 星星数
		unsigned short score = 0;      // 评分
		std::set<int> randomed;        // 记录已随机到的，保证不重复
		short created_num = 0;
		for(int rand_count = 0; total_weight > 0 && created_num < xianpin_num && created_num >= 0 && created_num < ItemNamespace::MAX_XIANPIN_NUM && rand_count < 2000; rand_count++)
		{
			rand_value = RandomNum(total_weight);

			for (int i = 1; i <= m_xianpin_cfg_total_num && i < MAX_XIANPING_CFG_NUM; ++i) 
			{
				const EquipXianPinConfig &xianpin_cfg = m_xianpin_cfg_list[i];

				//相同颜色中随机
				if (xianpin_cfg.equip_color != equip_item->GetColor())
					continue;

				// 不能存在两条相同仙品属性
				if (randomed.find(xianpin_cfg.xianpin_type) != randomed.end())
					continue;

				// 有些仙品属性只能存在于攻击或者防御类型的装备中，而 BIG_TYPE_ALL 这种类型的仙品可以存在于任何类型的装备中
// 				if(big_type != xianpin_cfg.big_type && BIG_TYPE_ALL != xianpin_cfg.big_type)
// 					continue;

				if (rand_value <= xianpin_cfg.weight)
				{
					score += xianpin_cfg.score;
					equip_param->xianpin_type_list[created_num] = xianpin_cfg.xianpin_type;
					++created_num;
					randomed.insert(xianpin_cfg.xianpin_type);
					break;
				}

				rand_value -= xianpin_cfg.weight;
			}
		}

		// 星数
		star_num = GetXianpinStarNumByScore(score);
		equip_param->xianpin_star_num = star_num;

		equip_param->xianpin_equip = equip_item->GetEquipType();
		randomed.clear();
	}
	
	return true;
}

const bool EquipConfig::GetLucky(ItemGridData *item_grid_data) const
{
	if (NULL == item_grid_data)
	{
		return false;
	}

	const Equipment* equip_item = (const Equipment *)ITEMPOOL->GetItem(item_grid_data->item_id);
	if (NULL == equip_item || ItemBase::I_TYPE_EQUIPMENT != equip_item->GetItemType())
	{
		return false;
	}

	short equip_type = equip_item->GetEquipType();
	if (equip_type < Equipment::E_TYPE_MIN || equip_type >= Equipment::E_TYPE_MAX)
	{
		return false;
	}

	EquipmentParam *equip_param = (EquipmentParam *)item_grid_data->param;
	equip_param->has_lucky = 1;

	return true;
}

int EquipConfig::InitEquipSkillCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");

	while (NULL != dataElement)
	{
		ItemID item_id;
		if (!GetSubNodeValue(dataElement, "item_id", item_id) || NULL == ITEMPOOL->GetItem(item_id)) 
		{
			return -1;
		}

		if (m_equip_skill_map.find(item_id) != m_equip_skill_map.end())
		{
			return -100;
		}

		EquipSkillConfig &cfg = m_equip_skill_map[item_id];
		cfg.item_id = item_id;

		if (!GetSubNodeValue(dataElement, "skill_id", cfg.skill_id) || NULL == SKILLPOOL->GetSkill(cfg.skill_id)) 
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "skill_level", cfg.skill_level) || cfg.skill_level < 1) 
		{
			return -3;
		}

		dataElement = dataElement->NextSiblingElement();
	}

	return	0;
}

const EquipSkillConfig * EquipConfig::GetSkillCfg(ItemID item_id) const
{
	std::map<ItemID, EquipSkillConfig>::const_iterator iter;
	for (iter = m_equip_skill_map.begin(); iter != m_equip_skill_map.end(); ++ iter)
	{
		if (iter->second.item_id == item_id)
		{
			return &iter->second;
		}
	}

	return NULL;
}

int EquipConfig::InitEquipGrowUpConfig(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		int equip_index = 0;
		if (!GetSubNodeValue(dataElement, "equip_index", equip_index) || equip_index < 0 || equip_index >= Equipment::EQUIPMENT_TYPE_COUNT)
		{
			return -1;
		}

		EquipGrowUpConfig &cfg = m_equip_grow_up_list[equip_index];
		if (0 != cfg.equip_index)
		{
			return -100;
		}

		cfg.equip_index = equip_index;

		if (!GetSubNodeValue(dataElement, "maxhp", cfg.add_maxhp) || cfg.add_maxhp < 0)
		{
			return -9;
		}

		if (!GetSubNodeValue(dataElement, "gongji", cfg.add_gongji) || cfg.add_gongji < 0)
		{
			return -10;
		}

		if (!GetSubNodeValue(dataElement, "fangyu", cfg.add_fangyu) || cfg.add_fangyu < 0)
		{
			return -11;
		}

		if (!GetSubNodeValue(dataElement, "mingzhong", cfg.add_mingzhong) || cfg.add_mingzhong < 0)
		{
			return -12;
		}

		if (!GetSubNodeValue(dataElement, "shanbi", cfg.add_shanbi) || cfg.add_shanbi < 0)
		{
			return -13;
		}

		if (!GetSubNodeValue(dataElement, "baoji", cfg.add_baoji) || cfg.add_baoji < 0)
		{
			return -14;
		}

		if (!GetSubNodeValue(dataElement, "jianren", cfg.add_jianren) || cfg.add_jianren < 0)
		{
			return -15;
		}

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

const EquipGrowUpConfig * EquipConfig::GetEquipGrowUpConfig(int equip_index) const
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) return NULL;

	return &m_equip_grow_up_list[equip_index];
}

int EquipConfig::InitXianPinFixCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");

	while (NULL != dataElement)
	{

		EquipXianPinFixConfig cfg;

		if (!GetSubNodeValue(dataElement, "get_way", cfg.get_way) || cfg.get_way <= Equipment::EQUIPMENT_FIX_GETWAY_MIN || cfg.get_way >= Equipment::EQUIPMENT_FIX_GETWAY_MAX) 
		{
			return -1;
		}

		if (!GetSubNodeValue(dataElement, "equip_id", cfg.equip_id))
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "param_1", cfg.param_1))
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "param_2", cfg.param_2))
		{
			return -4;
		}

		if (Equipment::EQUIPMENT_FIX_GETWAY_MONSTER_ID_DROP == cfg.get_way || Equipment::EQUIPMENT_FIX_GETWAY_SCENE_ID_DROP == cfg.get_way 
			|| Equipment::EQUIPMENT_FIX_GETWAY_COLOR_DROP == cfg.get_way)
		{
			if (Equipment::EQUIPMENT_FIX_GETWAY_MONSTER_ID_DROP == cfg.get_way && !MONSTERPOOL->IsMonsterExist(cfg.param_1))
			{
				return -110;
			}

			if (Equipment::EQUIPMENT_FIX_GETWAY_MONSTER_ID_DROP == cfg.get_way && (cfg.param_2 <= ItemBase::I_COLOR_INVALID || cfg.param_2 >= ItemBase::I_COLOR_MAX))
			{
				return -111;
			}

			if (Equipment::EQUIPMENT_FIX_GETWAY_SCENE_ID_DROP == cfg.get_way)
			{
				CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.param_1);
			}


			if (Equipment::EQUIPMENT_FIX_GETWAY_COLOR_DROP == cfg.get_way && (cfg.param_1 <= 0 || cfg.param_1 >= ItemBase::I_COLOR_MAX))
			{
				return -112;
			}
		}
		else
		{
			if (cfg.equip_id > 0 && NULL == ITEMPOOL->GetItem(cfg.equip_id))
			{
				return -101;
			}

			if (Equipment::EQUIPMENT_FIX_GETWAY_DEFAULT_GIFT == cfg.get_way && NULL == ITEMPOOL->GetItem(cfg.param_1))
			{
				return -102;
			}
		}

		//short xianpin_type_count = 0;
		//short temp_xianpin_type = 0;

		char tem_str[32];
		for (int i = 0; i < EquipXianPinFixConfig::GETWAY_XIANPIN_MAX_COUNT; i++)
		{
			sprintf(tem_str, "rate_%d", i);
			if (!GetSubNodeValue(dataElement, tem_str, cfg.xianpin_rate_list[i]) || cfg.xianpin_rate_list[i] < 0)
			{
				return -(20 + i);
			}
			cfg.total_weight += cfg.xianpin_rate_list[i];

			if (i > 0)
			{
				sprintf(tem_str, "xianpin_type_%d", i);
				if (!GetSubNodeValue(dataElement, tem_str, cfg.xianpin_type_list[i]) || cfg.xianpin_type_list[i] <= Equipment::E_XIANPIN_INVALID ||
					cfg.xianpin_type_list[i] >= Equipment::E_XIANPIN_MAX)
				{
					return -(30 + i);
				}
			}
		}

		if (cfg.equip_id > 0)
		{
			const Equipment* equip_item = (const Equipment *)ITEMPOOL->GetItem(cfg.equip_id);
			if (NULL == equip_item || ItemBase::I_TYPE_EQUIPMENT != equip_item->GetItemType())
			{
				return -10;
			}
		}

		long long key = ConvertParamToLongLong(cfg.get_way, cfg.equip_id, cfg.param_1);

 		EquipXianPinFixConfigMapIt it = m_xianpin_fix_cfg_map.find(key);
 		if (m_xianpin_fix_cfg_map.end() != it)
 		{
 			return -100;
 		}

		m_xianpin_fix_cfg_map[key] = cfg;

		dataElement = dataElement->NextSiblingElement();
	}
	return 0;
}

int EquipConfig::InitXianPinDropCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");

	m_xianpin_drop_cfg_map.clear();

	while (NULL != dataElement)
	{
		int equip_id = 0;
		if (!GetSubNodeValue(dataElement, "equip_id", equip_id) || equip_id < 0 || NULL == ITEMPOOL->GetItem(equip_id))
		{
			return -1;
		}

		EquipXianPinDropConfig cfg;
		cfg.equip_id = equip_id;

		if (m_xianpin_drop_cfg_map.find(cfg.equip_id) != m_xianpin_drop_cfg_map.end())
		{
			return -2;
		}

		const Equipment* equip_item = (const Equipment *)ITEMPOOL->GetItem(cfg.equip_id);
		if (NULL == equip_item || ItemBase::I_TYPE_EQUIPMENT != equip_item->GetItemType())
		{
			return 10;
		}

		char tem_str[32];
		int total_rate = 0;
		for (int i = 0; i < EquipXianPinDropConfig::DROP_XIANPIN_MAX_COUNT; i++)
		{
			sprintf(tem_str, "drop_rate_%d", i + 1);
			if (!GetSubNodeValue(dataElement, tem_str, cfg.drop_rate_list[i]) || cfg.drop_rate_list[i] < 0)
			{
				return -20;
			}
			total_rate += cfg.drop_rate_list[i];

			sprintf(tem_str, "xianpin_type_%d", i + 1);
			if (!GetSubNodeValue(dataElement, tem_str, cfg.drop_xianpin_type_list[i]))
			{
				return -21;
			}
		}

		if (total_rate > 10000)
		{
			return -22;
		}

		m_xianpin_drop_cfg_map[cfg.equip_id] = cfg;
		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

const bool EquipConfig::GetFixedXianPinType(ItemGridData *item_grid_data, int get_way, int param_1) const
{
	if (NULL == item_grid_data)
	{
		return false;
	}

	ItemID item_id = item_grid_data->item_id;
	if (Equipment::EQUIPMENT_FIX_GETWAY_MONSTER_ID_DROP == get_way 
		|| Equipment::EQUIPMENT_FIX_GETWAY_SCENE_ID_DROP == get_way
		|| Equipment::EQUIPMENT_FIX_GETWAY_COLOR_DROP == get_way
		|| Equipment::EQUIPMENT_FIX_GETWAY_DEFAULT_GIFT == get_way)
	{
		item_id = 0;
	}

	long long key = ConvertParamToLongLong(get_way, item_id, param_1);
	EquipXianPinFixConfigMapIt iter = m_xianpin_fix_cfg_map.find(key);
	if (m_xianpin_fix_cfg_map.end() == iter)
	{
		return false;
	}

	const EquipXianPinFixConfig *xianpin_fix_cfg = &iter->second;
	if (Equipment::EQUIPMENT_FIX_GETWAY_MONSTER_ID_DROP == get_way)
	{
		const ItemBase *itembase = ITEMPOOL->GetItem(item_grid_data->item_id);
		if (itembase == nullptr || itembase->GetColor() < xianpin_fix_cfg->param_2)
		{
			return true;               // 根据怪物id和最低颜色，返回true，掉落那边不再继续判断颜色 
		}
	}

	// 仙品条数
	int rand_value = RandomNum(xianpin_fix_cfg->total_weight);
	int xianping_count = 0;
	for (int i = 0; i < EquipXianPinFixConfig::GETWAY_XIANPIN_MAX_COUNT; i++)
	{
		if (xianpin_fix_cfg->xianpin_rate_list[i] == 0)
		{
			continue;
		}

		if (rand_value >= xianpin_fix_cfg->xianpin_rate_list[i])
		{
			rand_value -= iter->second.xianpin_rate_list[i];
		}
		else
		{
			xianping_count = i;
			break;
		}
	}

	if (xianping_count == 0)
	{
		return true;
	}

	if (xianping_count < 0 || xianping_count >= EquipXianPinFixConfig::GETWAY_XIANPIN_MAX_COUNT)
	{
		return false;
	}

	// 给予仙品属性
	EquipmentParam *equip_param = (EquipmentParam *)item_grid_data->param;
	if (NULL == equip_param) return false;

	std::vector<int> rand_index_list;        // 记录随机集合

	for (int xianping_index = 1; xianping_index < EquipXianPinFixConfig::GETWAY_XIANPIN_MAX_COUNT; xianping_index++)
	{	
		rand_index_list.push_back(xianping_index);
	}

	//策划需求，固定顺序
//	std::random_shuffle(rand_index_list.begin(), rand_index_list.end());

	for (int i = 0; i < xianping_count && i < EquipXianPinFixConfig::GETWAY_XIANPIN_MAX_COUNT && i < ItemNamespace::MAX_XIANPIN_NUM; ++i)
	{
		const EquipXianPinConfig *xianpin_cfg = this->GetXianPinEquipCfg(xianpin_fix_cfg->xianpin_type_list[rand_index_list[i]]);
		if (NULL != xianpin_cfg)
		{
			equip_param->xianpin_type_list[i] = static_cast<unsigned short>(xianpin_fix_cfg->xianpin_type_list[rand_index_list[i]]);
		}
	}

	return true;
}

int EquipConfig::GetXianPinStarLevel(int xianpin_capability) const
{
	for (int i = MAX_XIANPING_STAR_LEVEL - 1; i >= 0; --i)
	{
		if (xianpin_capability >= m_xianpinstar_cfg_list[i].need_capability)
		{
			return m_xianpinstar_cfg_list[i].star_level;
		}
	}

	return 0;
}

const EquipXianPinDropConfig * EquipConfig::GetXianPinDropCfg(int equip_id) const
{
	EquipXianPinDropConfigMapIt iter = m_xianpin_drop_cfg_map.find(equip_id);
	if (iter == m_xianpin_drop_cfg_map.end()) return NULL;

	return &iter->second;
}

const bool EquipConfig::GetDropXianPinType(ItemGridData *item_grid_data) const
{
	if (NULL == item_grid_data)
	{
		return false;
	}

	EquipXianPinDropConfigMapIt iter = m_xianpin_drop_cfg_map.find(item_grid_data->item_id);
	if (iter == m_xianpin_drop_cfg_map.end()) return false;

	int rand_value = RandomNum(10000);
	int xianping_count = 0;
	for (int i = 0; i < EquipXianPinDropConfig::DROP_XIANPIN_MAX_COUNT; i++)
	{
		if (rand_value >= iter->second.drop_rate_list[i])
		{
			rand_value -= iter->second.drop_rate_list[i];
		}
		else
		{
			xianping_count = i + 1;
			break;
		}
	}

	if (xianping_count <= 0 || xianping_count > EquipXianPinDropConfig::DROP_XIANPIN_MAX_COUNT)
	{
		return false;
	}

	int give_xianpin_count = 0;
	EquipmentParam *equip_param = (EquipmentParam *)item_grid_data->param;
	if (NULL == equip_param) return false;
	
	for (int i = 0; give_xianpin_count < xianping_count && give_xianpin_count < EquipXianPinDropConfig::DROP_XIANPIN_MAX_COUNT && i < ItemNamespace::MAX_XIANPIN_NUM; ++i)
	{
		++give_xianpin_count;
		const EquipXianPinConfig *xianpin_cfg = this->GetXianPinEquipCfg(iter->second.drop_xianpin_type_list[give_xianpin_count]);
		if (NULL != xianpin_cfg)
		{
			equip_param->xianpin_type_list[i] = static_cast<unsigned short>(iter->second.drop_xianpin_type_list[give_xianpin_count]);
		}	
	}

	return true;
}

int EquipConfig::InitEquipmentCompoundCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -10000;
	}

	while(NULL != dataElement)
	{
		ItemID item_id = 0;
		short xianpin_count = 0;
		if (!GetSubNodeValue(dataElement, "item_id", item_id) || item_id < 0 || NULL == ITEMPOOL->GetItem(item_id))
		{
			return -1;
		}

		if (!GetSubNodeValue(dataElement, "xianpin_count", xianpin_count) || xianpin_count < 0 || xianpin_count >= ItemNamespace::MAX_XIANPIN_NUM)
		{
			return -2;
		}

		int item_index = 0;
		item_index |= (xianpin_count << 16);
		item_index |= item_id;

		if (m_equipment_compound_cfg.end() != m_equipment_compound_cfg.find(item_index))
		{
			return -3;
		}

		EquipmentCompoundCfg compound_cfg;
		compound_cfg.item_id = item_id;
		compound_cfg.xianpin_count = xianpin_count;

		if (!GetSubNodeValue(dataElement, "gain_item", compound_cfg.gain_item) || compound_cfg.gain_item < 0 ||
			(0 != compound_cfg.gain_item && NULL == ITEMPOOL->GetItem(compound_cfg.gain_item)))
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "compound_xianpin", compound_cfg.compound_xianpin) || compound_cfg.compound_xianpin < 0)
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "is_notice", compound_cfg.is_notice) || compound_cfg.is_notice < 0)
		{
			return -6;
		}

		TiXmlElement *Itemelement = dataElement->FirstChildElement("stuff_item");
		if (!compound_cfg.stuff_item.ReadConfig(Itemelement))
		{
			return -200;
		}

		{
			TiXmlElement *element = dataElement->FirstChildElement("discard_return_list");
			if (NULL == element)
			{
				return -200;
			}

			int i = 0;
			TiXmlElement *item_element = element->FirstChildElement("discard_return");
			while (NULL != item_element)
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -201 - i;
				}

				if (!compound_cfg.discard_return[i].ReadConfig(item_element) && 0 != compound_cfg.discard_return[i].item_id)
				{
					return -251 - i;
				}

				i++;
				compound_cfg.discard_return_count = i;
				item_element = item_element->NextSiblingElement();
			}
		}

		m_equipment_compound_cfg[item_index] = compound_cfg;

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

const EquipmentCompoundCfg * EquipConfig::GetEquipCompoundConfig(ItemID item_id, short xianpin_count) const
{
	if (item_id <= 0 || NULL == ITEMPOOL->GetItem(item_id) || xianpin_count < 0 || xianpin_count >= ItemNamespace::MAX_XIANPIN_NUM)
	{
		return NULL;
	}

	int seq = 0;
	seq |= (xianpin_count << 16);
	seq |= item_id;

	std::map<int, EquipmentCompoundCfg>::const_iterator iter = m_equipment_compound_cfg.find(seq);
	if (iter != m_equipment_compound_cfg.end() && iter->second.item_id == item_id && iter->second.xianpin_count == xianpin_count)
	{
		return &iter->second;
	}

	return NULL;
}

int EquipConfig::InitEternityCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -10000;
	}

	while (NULL != dataElement)
	{
		int equip_index = 0;
		if (!GetSubNodeValue(dataElement, "equip_index", equip_index) || equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX)
		{
			return -1;
		}

		int eternity_level = 0;
		if (!GetSubNodeValue(dataElement, "eternity_level", eternity_level) || eternity_level < 0 || eternity_level > EquipConfig::MAX_ETERNITY_LEVEL)
		{
			return -2;
		}

		EquipEternityCfg &cfg = m_eternity_cfg_list[equip_index][eternity_level];
		cfg.equip_index = equip_index;
		cfg.eternity_level = eternity_level;

		if (!GetSubNodeValue(dataElement, "show_level", cfg.show_level) || cfg.show_level < 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "stuff_id", cfg.stuff_id) || NULL == ITEMPOOL->GetItem(cfg.stuff_id))
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "stuff_count", cfg.stuff_count) || cfg.stuff_count <= 0)
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -6;
		}

		if (!GetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -7;
		}

		if (!GetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -8;
		}

		if (!GetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0)
		{
			return -9;
		}

		if (!GetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0)
		{
			return -10;
		}

		if (!GetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0)
		{
			return -11;
		}

		if (!GetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0)
		{
			return -12;
		}

		if (!GetSubNodeValue(dataElement, "is_broadcast", cfg.is_broadcast))
		{
			return -13;
		}

		if (!GetSubNodeValue(dataElement, "stuff_2_id", cfg.stuff_2_id))
		{
			if (cfg.stuff_2_id > 0 && NULL == ITEMPOOL->GetItem(cfg.stuff_2_id))
			{
				return -14;
			}
		}

		if (!GetSubNodeValue(dataElement, "stuff_2_num", cfg.stuff_2_num) || cfg.stuff_2_num < 0)
		{
			return -15;
		}

		if (cfg.stuff_2_id > 0 && cfg.stuff_2_num <= 0)
		{
			return -16;
		}

		if (!GetSubNodeValue(dataElement, "jingjie_level", cfg.jingjie_level) || cfg.jingjie_level < 0)
		{
			return -17;
		}

		std::string name;
		if (!GetSubNodeValue(dataElement, "name", name))
		{
			return -18;
		}
		if (name.length() >= sizeof(cfg.name))
		{
			return -19;
		}
		memcpy(cfg.name, name.c_str(), (int)name.length());

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

const EquipEternityCfg * EquipConfig::GetEquipEternityCfg(int equip_index, int eternity_level) const
{
	if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX
		|| eternity_level < 0 || eternity_level > MAX_ETERNITY_LEVEL
		|| equip_index != m_eternity_cfg_list[equip_index][eternity_level].equip_index
		|| eternity_level != m_eternity_cfg_list[equip_index][eternity_level].eternity_level)
	{
		return NULL;
	}

	return &m_eternity_cfg_list[equip_index][eternity_level];
}

int EquipConfig::InitEternitySuitCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -10000;
	}

	while (NULL != dataElement)
	{
		int suit_level = 0;
		if (!GetSubNodeValue(dataElement, "suit_level", suit_level) || suit_level < 0 || suit_level > MAX_ETERNITY_LEVEL)
		{
			return -1;
		}

		EquipEternitySuitCfg &cfg = m_eternity_suit_cfg_list[suit_level];
		cfg.suit_level = suit_level;

		if (!GetSubNodeValue(dataElement, "show_level", cfg.show_level) || cfg.show_level < 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0)
		{
			return -6;
		}

		if (!GetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0)
		{
			return -7;
		}

		if (!GetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0)
		{
			return -8;
		}

		if (!GetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0)
		{
			return -9;
		}

		if (!GetSubNodeValue(dataElement, "per_attr", cfg.per_attr) || cfg.per_attr < 0)
		{
			return -10;
		}

		if (!GetSubNodeValue(dataElement, "is_broadcast", cfg.is_broadcast))
		{
			return -11;
		}

		if (!GetSubNodeValue(dataElement, "hxyj", cfg.hxyj) || cfg.hxyj < 0)
		{
			return -12;
		}

		if (!GetSubNodeValue(dataElement, "hxyj_hurt_per", cfg.hxyj_hurt_per) || cfg.hxyj_hurt_per < 0)
		{
			return -13;
		}

		std::string name;
		if (!GetSubNodeValue(dataElement, "name", name))
		{
			return -14;
		}
		if (name.length() >= sizeof(cfg.name))
		{
			return -15;
		}
		memcpy(cfg.name, name.c_str(), (int)name.length());

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

const EquipEternitySuitCfg * EquipConfig::GetEquipEternitySuitCfg(int eternity_suit_level) const
{
	if (eternity_suit_level < 0 || eternity_suit_level > MAX_ETERNITY_LEVEL
		|| eternity_suit_level != m_eternity_suit_cfg_list[eternity_suit_level].suit_level)
	{
		return NULL;
	}
	return &m_eternity_suit_cfg_list[eternity_suit_level];
}

// 通过评分获取仙品星星数量
unsigned short EquipConfig::GetXianpinStarNumByScore(unsigned short score) const
{
	const FeiXianOtherConfig* cfg = LOGIC_CONFIG->Instance()->GetReallyequipComposeCfg().GetOtherCfg(score);
	if (NULL != cfg)
	{
		unsigned short star_num = cfg->star_num;
		return star_num;
	}
	return 0;
}

int EquipConfig::InitDecomposeCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -10000;
	}


	while (NULL != dataElement)
	{
		int type;
		if (!GetSubNodeValue(dataElement, "type", type) || type < 0)
		{
			return -1;
		}

		if (m_decompose_map.find(type) != m_decompose_map.end())
		{
			return -100;
		}

		DecomposeCfg &cfg = m_decompose_map[type];
		cfg.type = type;

		{
			TiXmlElement *item_element = dataElement->FirstChildElement("return_item");
			if (NULL == item_element)
			{
				return -2;
			}

			if (!cfg.return_item.ReadConfig(item_element))
			{
				return -101;
			}
		}

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

const DecomposeCfg * EquipConfig::GetEquipDecomposeCfg(int type) const   
{
	if (type < 0) return NULL;

	for (auto & decompose_it : m_decompose_map)
	{
		if (decompose_it.first == type)
		{
			return &decompose_it.second;
		}
	}

	return NULL;
}

int EquipConfig::InitEquipUpLevelSuitCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -10000;
	}

	while (NULL != dataElement)
	{
		int order = 0;
		if (!GetSubNodeValue(dataElement, "order", order) || order < 0 || order > UPLEVEL_SUIT_COUNT)
		{
			return -97;
		}

		EquipUpLevelSuitCfg &suit_cfg = m_equip_uplevel_suit_map[order];

		int need_count = 0;
		if (!GetSubNodeValue(dataElement, "need_count", need_count) || need_count < 0)
		{
			return -98;
		}

		if (suit_cfg.suit_count < 0 || suit_cfg.suit_count >= Equipment::E_INDEX_MAX)
		{
			return -99;
		}

		EquipUpLevelSuitAttrCfg &attr_cfg = suit_cfg.attr[suit_cfg.suit_count];

		attr_cfg.need_count = need_count;

		if (!GetSubNodeValue(dataElement, "maxhp", attr_cfg.maxhp) || attr_cfg.maxhp < 0)
		{
			return -100;
		}

		if (!GetSubNodeValue(dataElement, "gongji", attr_cfg.gongji) || attr_cfg.gongji < 0)
		{
			return -101;
		}

		if (!GetSubNodeValue(dataElement, "fangyu", attr_cfg.fangyu) || attr_cfg.fangyu < 0)
		{
			return -102;
		}

		if (!GetSubNodeValue(dataElement, "mingzhong", attr_cfg.mingzhong) || attr_cfg.mingzhong < 0)
		{
			return -103;
		}

		if (!GetSubNodeValue(dataElement, "shanbi", attr_cfg.shanbi) || attr_cfg.shanbi < 0)
		{
			return -104;
		}

		if (!GetSubNodeValue(dataElement, "baoji", attr_cfg.baoji) || attr_cfg.baoji < 0)
		{
			return -105;
		}

		if (!GetSubNodeValue(dataElement, "jianren", attr_cfg.jianren) || attr_cfg.jianren < 0)
		{
			return -106;
		}

		if (!GetSubNodeValue(dataElement, "per_zhufuyiji", attr_cfg.per_zhufuyiji) || attr_cfg.per_zhufuyiji < 0)
		{
			return -108;
		}

		++suit_cfg.suit_count;

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

const EquipUpLevelSuitAttrCfg * EquipConfig::GetEquipUplevelSuitCfg(int order, int count) const
{
	if (order < 0 || order > UPLEVEL_SUIT_COUNT)
	{
		return NULL;
	}

	auto it = m_equip_uplevel_suit_map.find(order);
	if (it != m_equip_uplevel_suit_map.end() && it->second.suit_count > 0)
	{
		for (int i = it->second.suit_count - 1; i >= 0 && i < Equipment::E_INDEX_MAX; i--)
		{
			if (count >= it->second.attr[i].need_count)
			{
				return &(it->second.attr[i]);
			}
		}
	}

	return NULL;
}

