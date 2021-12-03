#ifndef _ZHUANSHENGEQUIP_HPP_
#define _ZHUANSHENGEQUIP_HPP_

#include "servercommon/zhuanshengequipdef.hpp"
#include "obj/character/attribute.hpp"

class Role;

class ZhuanShengEquip
{
public:
	ZhuanShengEquip();
	~ZhuanShengEquip();
	
	void SetRole(Role *role) { m_role = role; }

	void InitParam(Role *role, const ZhuanShengEquipParam &param);
	void GetInitParam(ZhuanShengEquipParam *param);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void CalcInjureOnAttack(long long &injure);
	void CalcInjureOnBeAttacked(long long &injure);
	int GetAddExpPerOnKillMonster();
	int GetAddCoinPerOnKillMonster();

	bool OnUseXiuweiDan(int xiuwei_count, int num);
	void AddXiuwei(int add_xiuwei);

	void OnUpZhuanShengLevel();
	void OnChangeXiuwei();

	void TakeOutEquip(int backpack_index);													// ȡװ��
	int PutOnEquip(const ItemDataWrapper &item_wrapper);									// ��װ��
	int TakeOffEquip(short equip_index);													// ��װ��
	int GetEquipIndexByType(int equip_type);
	void CombineEquip(int index1, int index2, int index3, short is_auto_combine);			// ��װ��
	void AutoCombineEquip();																// �Զ���װ��

	void Chou(int type);

	void OnChangeProf();
	
	ItemID GetZhuanshengEquipID(int equip_index);

	int GetZhuanshengLevel() { return m_param.zhuansheng_level; }

	void SendAllInfo();
	void SendBackpackInfo();
	void SendOtherInfo(int notice_reason = 0);
	void SendXiuWeiNotice(int xiuwei, int notice_reason = 0);

	void GMClearBackpack();													// ��ձ���
	void GMSetZhuanShengLevel(int value);

private:
	Role *m_role;
	ZhuanShengEquipParam m_param;
	CharIntAttrs m_attrs_add;												// �����Լӳ���ֵ

	unsigned int m_next_allow_autoconbine_time;
	
	int m_add_exp_per;
	int m_add_coin_per;
	int m_per_to_role_shang;
	int m_per_to_role_mianshang;
};

#endif