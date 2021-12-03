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
	// װ��ǿ��
	static bool EquipStrengthen(Role *role, bool is_puton, bool select_bind_first, bool is_auto_buy, short equip_index, bool use_lucky_item, short flag, int *old_level = NULL, int *coin_cost = NULL, int *gold_cost = NULL);

	// ����װ��ǿ���ȼ�
	static bool GMResetStrengthenLevel(Role *role, bool is_puton, short equip_index, int lvl = 0, bool dirty = false);

	//װ����������
	static bool EquipUpStar(Role *role, short equip_index);

	// װ������
	//static bool EquipUpLevel(Role *role, short equip_index1, short equip_index2,  short equip_index3);
	//static bool EquipUpLevel(Role *role, bool is_puton, bool select_bind_first, bool is_auto_buy, short equip_index, bool use_protect_item, int use_protect_num, bool isGM = false, int *coin_cost = NULL, int *gold_cost = NULL);
	//static bool EquipUpLevel(Role *role, bool is_puton, bool select_bind_first, bool is_auto_buy, short equip_index, char use_protect_item, bool isGM = false);
	static bool EquipUpLevel(Role *role, bool is_puton, bool select_bind_first, short equip_index /*bool use_protect_stuff*/);

	// װ���̳�
	static bool EquipInherit(Role *role, short equip_index1, short equip_index2, bool is_puton1, bool is_puton2, short inherit_type, short cost_type);

	// װ����Ʒ��
	static bool EquipUpQuality(Role *role, bool is_puton, bool select_bind_first, short equip_index);

	// ����
	static bool EquipShenOP(Role *role, bool is_puton, bool select_bind_first, short equip_index);

	// װ������
	static bool EquipFuling(Role *role, bool is_puton, bool select_bind_first, short equip_index);
	
	// װ������ƴ�ӳ��ַ���
	static const char * EquipParamDataClientString(int role_id, ItemID equip_id, bool is_bind, UInt32 invalid_time, const EquipmentParam *equip_param, const EquipBaptizePartInfo *baptize_info);

	// װ���ϳ�
	static bool EquipCompound(Role *role, int equip_index);

	// װ����������ȼ�
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
