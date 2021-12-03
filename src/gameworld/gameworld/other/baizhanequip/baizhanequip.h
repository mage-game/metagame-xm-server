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
	void OnInitFinish();																	// ��δ����

	void Update(time_t now_second);															// ��δ����

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	int GetEquipIndexByType(int equip_type);												// ��ȡװ������

	void OnOperaReq(int opera_type, int param1, int param2);								// ��������

	int PutOn(const ItemDataWrapper &item_wrapper);											// ����
	int TakeOff(short equip_index, bool is_from_puton = false);								// ����

	bool CheckKnapsack();																	// ��鱳���Ƿ���
	bool IsCanPuton(int equip_part, int equip_order, ItemID item_id);

	const Equipment *GetBaizhanEquip(ItemID item_id);										// ��ȡװ��

	const ItemDataWrapper *GetEquipmentGridData(short equip_index);

	//��װ���---------------------------------------------------------------------------------------------
	int ReCalcSameOrderCount(int order = 0);												// ������ͬ��������

	void GetSuitTotalAttr(CharIntAttrs &tmp_attr);

	//�������---------------------------------------------------------------------------------------------
	void OnPutonOrUplevel(int part_index, int item_id);									    // ����������

	//----------------------------------------------------------------------------------------------------------
	void SyncBaizhanEquipAllInfo();															// �·���Ϣ

	void PrintLog(const char *reason, int part_index, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0);

	int HaveEquipNum();

private:

	Role			*m_role = nullptr;
	BaizhanEquipParam m_param;
	CharIntAttrs	m_attrs_add;								// �����Լӳ���ֵ
};

#endif

