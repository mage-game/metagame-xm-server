#ifndef __EQUIP_SUIT_CONFIG_H__
#define __EQUIP_SUIT_CONFIG_H__

#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gameworld/item/itembase.h"
#include <map>

struct SuitEquipUplevelCfg
{
	SuitEquipUplevelCfg() : equip_id(0), suit_id(0), need_stuff_id_ss(0), need_stuff_count_ss(0), need_stuff_id_cq1(0), need_stuff_count_cq1(0), need_stuff_id_cq2(0), need_stuff_count_cq2(0), total_equip_cout(0) {}

	ItemID equip_id;
	short suit_id;

	ItemID need_stuff_id_ss;			//史诗套装
	short need_stuff_count_ss;

	ItemID need_stuff_id_cq1;			//传奇套装
	short need_stuff_count_cq1;
	ItemID need_stuff_id_cq2;
	short need_stuff_count_cq2;

	int total_equip_cout;
};

struct EquipSuitAttrCfg
{
	EquipSuitAttrCfg() : suit_id(0), equip_count(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), maxhp_attr(0), gongji_attr(0), 
		fangyu_attr(0), mingzhong_attr(0), shanbi_attr(0), baoji_attr(0), jianren_attr(0), fujiashanghai(0), dikangshanghai(0), is_broacast(0) {}
	
	short suit_id;
	short equip_count;

	Attribute maxhp;
	Attribute gongji;													
	Attribute fangyu;																										
	Attribute mingzhong;												
	Attribute shanbi;													
	Attribute baoji;													
	Attribute jianren;
	Attribute maxhp_attr;	
	Attribute gongji_attr;													
	Attribute fangyu_attr;																									
	Attribute mingzhong_attr;												
	Attribute shanbi_attr;													
	Attribute baoji_attr;													
	Attribute jianren_attr;
	Attribute fujiashanghai;													
	Attribute dikangshanghai;

	int is_broacast;
};

class EquipSuitConfig : public ConfigBase
{
public:
	EquipSuitConfig();
	~EquipSuitConfig();

	bool Init(const std::string &configname, std::string *err);

	const SuitEquipUplevelCfg * GetSuitUpLevelCfg(ItemID equip_id) const;
	const EquipSuitAttrCfg * GetEquipShiShiSuitCfg(int suit_id, int equip_count) const;
	const EquipSuitAttrCfg * GetEquipChuanQiSuitCfg(int suit_id, int equip_count) const;

private:
	int InitEquipSuitUpLevelCfg(PugiXmlNode RootElement);
	int InitEquipShiShiSuitCfg(PugiXmlNode RootElement);
	int InitEquipChuanQiSuitCfg(PugiXmlNode RootElement);
	
	std::map<ItemID, SuitEquipUplevelCfg> m_suit_uplevel_map;

	std::map<long long, EquipSuitAttrCfg> m_suit_attr_ss_map;					// 史诗套装属性列表
	std::map<long long, EquipSuitAttrCfg> m_suit_attr_cq_map;					// 传奇套装属性列表
};

#endif
