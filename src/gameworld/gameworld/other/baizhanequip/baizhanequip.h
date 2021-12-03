#ifndef __BAIZHAN_EQUIP_H__
#define __BAIZHAN_EQUIP_H__

#include "item/itemgriddata.h"
#include "equipment/equipment.h"
#include "servercommon/struct/itemlistparam.h"
#include "obj/character/attribute.hpp"
#include "gamedef.h"
#include "servercommon/baizhanequipdef.hpp"
#include "baizhanequipconfig.hpp"

#include <set>

class Role;

class BaizhanEquip
{
public:
	BaizhanEquip();
	~BaizhanEquip();

	void SetRole(Role *role);

	void Init(Role *role, const BaizhanEquipParam &param);
	void GetInitParam(BaizhanEquipParam *param);
	void OnInitFinish();																	// 暂未调用

	void Update(time_t now_second);															// 暂未调用

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	int GetEquipIndexByType(int equip_type);												// 获取装备索引

	void OnOperaReq(int opera_type, int param1, int param2);								// 操作请求

	int PutOn(const ItemDataWrapper &item_wrapper);											// 穿上
	int TakeOff(short equip_index, bool is_from_puton = false);								// 脱下

	bool CheckKnapsack();																	// 检查背包是否锁
	bool IsCanPuton(int equip_part, int equip_order, ItemID item_id);

	const Equipment *GetBaizhanEquip(ItemID item_id);										// 获取装备

	const ItemDataWrapper *GetEquipmentGridData(short equip_index);

	//套装相关---------------------------------------------------------------------------------------------
	int ReCalcSameOrderCount(int order = 0);												// 计算相同阶数数量

	void GetSuitTotalAttr(CharIntAttrs &tmp_attr);

	//升级相关---------------------------------------------------------------------------------------------
	void OnPutonOrUplevel(int part_index, int item_id);									    // 穿戴或升级

	//----------------------------------------------------------------------------------------------------------
	void SyncBaizhanEquipAllInfo();															// 下发信息

	void PrintLog(const char *reason, int part_index, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0);

	int HaveEquipNum();

private:

	Role			*m_role = nullptr;
	BaizhanEquipParam m_param;
	CharIntAttrs	m_attrs_add;								// 各属性加成数值
};

#endif

