#ifndef __EQUIPMENT_FUNCTION_H__
#define __EQUIPMENT_FUNCTION_H__

#include "servercommon/basedef.h"
#include "servercommon/serverdef.h"
#include "obj/character/attribute.hpp"
#include "servercommon/stonedef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/equipbaptizedef.hpp"

class Role;
class Equipment;
class EquipmentParam;

class EquipFunction
{
public:
	// 装备强化
	static bool EquipStrengthen(Role *role, bool is_puton, bool select_bind_first, bool is_auto_buy, short equip_index, bool use_lucky_item, short flag, int *old_level = NULL, int *coin_cost = NULL, int *gold_cost = NULL);

	// 重置装备强化等级
	static bool GMResetStrengthenLevel(Role *role, bool is_puton, short equip_index, int lvl = 0, bool dirty = false);

	//装备升级星星
	static bool EquipUpStar(Role *role, short equip_index);

	// 装备升级
	//static bool EquipUpLevel(Role *role, short equip_index1, short equip_index2,  short equip_index3);
	//static bool EquipUpLevel(Role *role, bool is_puton, bool select_bind_first, bool is_auto_buy, short equip_index, bool use_protect_item, int use_protect_num, bool isGM = false, int *coin_cost = NULL, int *gold_cost = NULL);
	//static bool EquipUpLevel(Role *role, bool is_puton, bool select_bind_first, bool is_auto_buy, short equip_index, char use_protect_item, bool isGM = false);
	static bool EquipUpLevel(Role *role, bool is_puton, bool select_bind_first, short equip_index /*bool use_protect_stuff*/);

	// 装备继承
	static bool EquipInherit(Role *role, short equip_index1, short equip_index2, bool is_puton1, bool is_puton2, short inherit_type, short cost_type);

	// 装备升品质
	static bool EquipUpQuality(Role *role, bool is_puton, bool select_bind_first, short equip_index);

	// 神铸
	static bool EquipShenOP(Role *role, bool is_puton, bool select_bind_first, short equip_index);

	// 装备附灵
	static bool EquipFuling(Role *role, bool is_puton, bool select_bind_first, short equip_index);
	
	// 装备数据拼接成字符串
	static const char * EquipParamDataClientString(int role_id, ItemID equip_id, bool is_bind, UInt32 invalid_time, const EquipmentParam *equip_param, const EquipBaptizePartInfo *baptize_info);

	// 装备合成
	static bool EquipCompound(Role *role, int equip_index);

	// 装备提升永恒等级
	static bool EquipUpEternity(Role *role, int equip_index, bool is_auto_buy);
	static bool UseEternityLevel(Role *role, short eternity_level);

	static bool CanStrength(int equiptype);
	static bool CanUpQuality(int equiptype);
	static bool CanRefine(int equiptype);
};

#define EQUIP_FUNCTION_GET_EQUIP(is_putting_on, equip_index, out_item_data, out_equip_item) \
{\
	if (!is_putting_on)\
	{\
		if (!knapsack->GetItemGridData(equip_index, &out_item_data))\
		{\
			role->NoticeNum(errornum::EN_FUNC_ITEM_LOAD);\
			return false;\
		}\
	}\
	else\
	{\
		if (!equip_mgr->GetEquipmentGridData(equip_index, &out_item_data))\
		{\
			role->NoticeNum(errornum::EN_FUNC_ITEM_LOAD);\
			return false;\
		}\
	}\
	if (NULL == out_item_data.param)\
	{\
		role->NoticeNum(errornum::EN_FUNC_ITEM_LOAD);\
		return false;\
	}\
	out_equip_item = (const Equipment *)ITEMPOOL->GetItem(out_item_data.item_id);\
	if (NULL == out_equip_item || out_equip_item->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)\
	{\
		role->NoticeNum(errornum::EN_FUNC_ITEM_LOAD);\
		return false;\
	}\
}

#endif
