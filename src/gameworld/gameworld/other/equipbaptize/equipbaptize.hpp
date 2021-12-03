#ifndef __EQUIP_BAPTIZE_HPP__
#define __EQUIP_BAPTIZE_HPP__

#include "obj/character/attribute.hpp"
#include "servercommon/equipbaptizedef.hpp"

class Role;

class EquipBaptize
{
public:
	EquipBaptize();
	~EquipBaptize();

	void SetRole(Role *role) { m_role = role; }
	void InitParam(Role *role, const EquipBaptizeParam &param);
	void GetInitParam(EquipBaptizeParam *param);
	void OnDayChange(int old_dayid, int now_dayid);
	void OnRoleLogin();

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void CheckOpenPartAndSlot();									// 检查开启的部位、装备开启的槽

	void OnOpenBaptizeSlot(int equip_part, int index);				// 开启洗炼槽
	void OnLockOrUnLockSlot(int equip_part, int slot_index);		// 加锁 or 去锁
	void BeginBaptize(int equip_part, bool is_auto_buy, int specila_type);		// 开始洗炼(equip_part:洗练位置，is_auto_buy:是否使用元宝,specila_type:特殊洗练石)

	void SetOpenSlotAttr(int equip_part, int slot_index);			// 给予开启槽属性

	int GetRandAttrSeq(int equip_part);								// 随机得到属性seq

	int GetColorAndAttrValBySeq(int seq, int special_type = EQUIP_BAPTIZE_SPECIAL_TYPE_NONE);
	EquipBaptizePartInfo *GetEquipBaptizePartInfo(int equip_part);

	void SendEquipBaptizeAllInfo();

	int GetColorAttrCount(int color);								// 获取颜色color及以上的属性条数

	void PrintOperationLog();

private:
	Attribute GetBaptizeSuitAttrValue(int equip_index, int &baptize_color);

	Role *m_role;

	EquipBaptizeParam m_param;
	CharIntAttrs m_attrs_add;
};

#endif
