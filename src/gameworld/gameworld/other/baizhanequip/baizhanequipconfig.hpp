#ifndef __BAIZHAN_EQUIP_CONFIG_HPP__
#define __BAIZHAN_EQUIP_CONFIG_HPP__

#include "gamedef.h"
#include "servercommon/basedef.h"
#include "servercommon/baizhanequipdef.hpp"
#include "servercommon/struct/itemlistparam.h"

#include "equipment/equipment.h"
#include "item/itembase.h"
#include "item/itempool.h"

#include "obj/character/attribute.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include <map>
#include <vector>

// 其它配置
struct BaizhanOtherConfig
{
	BaizhanOtherConfig() {}

	int is_open = 0;
};

// 装备配置
struct BaizhanEquipCfg
{
	BaizhanEquipCfg() {}

	int equip_part = 0;
	int equip_order = 0;
	int role_need_min_level = 0;
};

//套装相关-------------------------------------------------------------------

// 套装属性
struct SuitAttrCfg
{
	SuitAttrCfg() {}

	int equip_order = 0;
	int same_order_num = 0;
	
	AttrConfig attr_cfg;
	Attribute per_pvp_zengshenang = 0;
	Attribute per_pvp_mianshang = 0;
	Attribute huixinyiji = 0;
	Attribute per_jingzhun = 0;
	Attribute per_zhufuyiji = 0;
	Attribute skill_zengshang_per = 0;
	Attribute skill_jianshang_per = 0;
	Attribute per_pofang = 0;
	Attribute per_maxhp = 0;
	Attribute per_gongji = 0;
};

//合成升级------------------------------------------
// 装备升级配置
struct BaizhanEquipLevelCfg
{
	BaizhanEquipLevelCfg() {}

	int old_equip_item_id = 0;
	int new_equip_item_id = 0;
	int need_num = 0;
	int is_broadcast = 0;
	int part = 0;
};

class BaizhanEquipConfig : public ConfigBase
{
public:

	BaizhanEquipConfig();
	~BaizhanEquipConfig();

	bool Init(const std::string &configname, std::string *err);

	const BaizhanOtherConfig &GetOtherCfg() { return m_other_cfg; }

	// 装备相关-----------------------------------------------------------------------------
	const BaizhanEquipCfg * GetBaizhanEquipCfg(int equip_part, int equip_order);

	//套装相关-----------------------------------------------------------------------------
	const SuitAttrCfg * GetSuitAttrCfg(int equip_order, int same_order_num);

	//装备升级相关-----------------------------------------------------------------------------
	const BaizhanEquipLevelCfg * GetBaizhanEquipFirstCfg();
	const BaizhanEquipLevelCfg * GetBaizhanEquipLevelCfg(int old_equip_id, int part);
	const BaizhanEquipLevelCfg * GetBaizhanEquipLevelReverseCfg(int new_equip_id, int part);

private:

	// 其他-----------------------------------------------------------------------------
	int InitOtherConfig(PugiXmlNode RootElement);         
	BaizhanOtherConfig m_other_cfg;            

	// 装备相关-----------------------------------------------------------------------------
	int InitBaizhanEquipCfg(PugiXmlNode RootElement);

	typedef std::map<long long, BaizhanEquipCfg> EquipPutOnLimitConfigMap;
	typedef std::map<long long, BaizhanEquipCfg>::const_iterator EquipPutOnLimitConfigMapIt;
	EquipPutOnLimitConfigMap m_equip_puton_limit_cfg_map;

	//套装相关-----------------------------------------------------------------------------
	int InitSuitAttrCfg(PugiXmlNode RootElement);
	std::vector<SuitAttrCfg> m_suit_attr_vec;

	//装备升级相关-----------------------------------------------------------------------------
	int InitBaizhanEquipLevelCfg(PugiXmlNode RootElement);
	std::vector<BaizhanEquipLevelCfg> m_baizhan_equip_level_vec;
};

#endif




