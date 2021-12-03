#ifndef __IMP_GUARD_HPP__
#define __IMP_GUARD_HPP__

#include "item/itemgriddata.h"
#include "impguardconfig.hpp"
#include "obj/character/attribute.hpp"

class Role;

class ImpGuard
{
public:
	enum IMP_GUARD_TYPE         // 小鬼类型  配表imp_type字段    1、3为经验小鬼 2、4为守护
	{
		IMP_GUARD_TYPE_INVALID = 0,
		IMP_GUARD_TYPE_01,
		IMP_GUARD_TYPE_02,
		IMP_GUARD_TYPE_03,
		IMP_GUARD_TYPE_04,
	};

	enum IMP_GUARD_GRID_INDEX      // 可装备两个小鬼 格子索引0为经验小鬼， 1为守护小鬼
	{
		IMP_GUARD_GRID_INDEX_0 = 0,
		IMP_GUARD_GRID_INDEX_1,
		IMP_GUARD_GRID_INDEX_MAX,
	};

	struct ImpGridData
	{
		ImpGridData()
		{
			Reset();
		}

		void Reset()
		{
			invalid_notice = false;
			item_data.Reset();
		}

		bool invalid_notice;
		ItemGridData item_data;
	};

	ImpGuard();
	~ImpGuard();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role);

	void Update(unsigned long interval, unsigned int now_second);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void OnRoleLogin();
	void OnImpGuardOperaReq(int opera_type, int param1, int param2);
	void SendImpGuardAllInfo();
	//void SendImpGuardExpireTime(int index);
	int PutOn(const ItemDataWrapper &item_wrapper);
	void OnRoleLogout();
	void AutoPutOn();
	int TakeOff(int grid_index);
	int GetAddPerExp();
	void GetImpUseAppeId(short imp_appe_id[IMP_GUARD_GRID_INDEX_MAX]);
	void ResetImpMailStautsByType(int imp_type);

	Attribute GetPerMianshangVal() { return m_attr_per_mianshang; }

private:
	void OnImpGuardRenewPutOn(int grid_index, bool is_bind, bool from_puton);
	void OnImpGuardRenewKnapsack(int index, bool is_bind);

	void OnCheckImpGuardBuff();
	void CheckImpGuardMail();
	void CheckImpGuardExpireTime();

	void OnImpGuardChangeNotice(int grid_index, int imp_id);

	int GetGridIndexByImpType(short imp_type);    // 根据小鬼类型来获取应该装备再身上哪个格子

	Role *m_role;
	ImpGridData m_grid_data[IMP_GUARD_GRID_INDEX_MAX];
	CharIntAttrs m_attrs_add;

	bool m_invalid_update;

	Attribute m_attr_per_mianshang = 0;							// 守护小鬼伤害减免属性值
};
#endif