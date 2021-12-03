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

	void CheckOpenPartAndSlot();									// ��鿪���Ĳ�λ��װ�������Ĳ�

	void OnOpenBaptizeSlot(int equip_part, int index);				// ����ϴ����
	void OnLockOrUnLockSlot(int equip_part, int slot_index);		// ���� or ȥ��
	void BeginBaptize(int equip_part, bool is_auto_buy, int specila_type);		// ��ʼϴ��(equip_part:ϴ��λ�ã�is_auto_buy:�Ƿ�ʹ��Ԫ��,specila_type:����ϴ��ʯ)

	void SetOpenSlotAttr(int equip_part, int slot_index);			// ���迪��������

	int GetRandAttrSeq(int equip_part);								// ����õ�����seq

	int GetColorAndAttrValBySeq(int seq, int special_type = EQUIP_BAPTIZE_SPECIAL_TYPE_NONE);
	EquipBaptizePartInfo *GetEquipBaptizePartInfo(int equip_part);

	void SendEquipBaptizeAllInfo();

	int GetColorAttrCount(int color);								// ��ȡ��ɫcolor�����ϵ���������

	void PrintOperationLog();

private:
	Attribute GetBaptizeSuitAttrValue(int equip_index, int &baptize_color);

	Role *m_role;

	EquipBaptizeParam m_param;
	CharIntAttrs m_attrs_add;
};

#endif
