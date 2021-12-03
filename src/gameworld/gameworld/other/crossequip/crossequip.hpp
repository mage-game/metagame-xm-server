#ifndef __CROSS_EQUIP_HPP__
#define __CROSS_EQUIP_HPP__

#include "servercommon/crossequipdef.hpp"
#include "equipment/equipment.h"

struct CrossEquipInfo
{
	bool operator < (const CrossEquipInfo &other) const 
	{
		if (color == other.color)
		{
			return order > other.order;
		}

		return color > other.color;
	}

	int color = 0;
	int order = 0;
};

class CrossEquip
{
public:
	CrossEquip();
	~CrossEquip();

	void SetRole(Role *role) { m_role = role; }
	void InitParam(const CrossEquipParam &param) { m_param = param; }
	void GetInitParam(CrossEquipParam *param) { *param = m_param; }
	void OnRoleLogin() {};
	void OnRoleLogout() {};
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void Update(time_t now_second, unsigned long interval);
	void OnDayChange(UInt32 old_dayid, UInt32 now_dayid);

	void HandleMsg(short req_type, short param_1, short param_2, short param_3);

	void DouqiUpgrade();												// 斗气升阶
	void DouqiXiulian();												// 斗气修炼
	void Roll(short roll_type, short roll_times_type);					// 抽奖
	bool UseDouqidan(short douqidan_type, short &use_num);				// 使用斗气丹
	void Lianzhi(int equip_index, int douqi_grade);						// 炼制

	bool AddChuanshiFragment(int score);								// 增加传世碎片

	//套装相关---------------------------------------------------------------------------------------------
	void ReCalcSuitCount();												// 计算套装数量
	void GetSuitTotalAttr(CharIntAttrs &tmp_attr);

	void SendAllInfo();
	void SendDouqiExpChange(int add_exp);
	void SendAllEquipInfo();
	void SendOneEquipInfo(int equip_index);
	void SendChuanshiFragChange(int change_type, int change_frag);

	int PutOn(const ItemDataWrapper & item_wrapper);
	int TakeOff(short equip_index, bool is_from_puton = false);
	int GetEquipIndexByType(int equip_type);
	ItemID GetCrossEquipID(int equip_index);							// 获取装备id

	bool IsFuncOpen();													// 系统是否开启

	short GetDouqiGrade() { return m_param.douqi_grade; }

	bool IsValidEquipType(int equip_type)
	{
		return equip_type > Equipment::E_TYPE_CHUANSHI_MIN && equip_type < Equipment::E_TYPE_CHUANSHI_MAX;
	}

	bool IsValidEquipIndex(int equip_index)
	{
		return equip_index >= CROSS_EQUIP_INDEX_MIN && equip_index < CROSS_EQUIP_INDEX_MAX;
	}

private:

	Role *m_role;
	CrossEquipParam m_param;
	CharIntAttrs m_attrs_add;

	static std::map<int, int> m_equip_type2inde_map;
	int m_suit_num_list[CROSS_EQUIP_MAX_ORDER + 1][Equipment::I_COLOR_MAX];

	CrossEquipInfo m_equip_info_list[CROSS_EQUIP_INDEX_MAX];
};

#endif // __CROSS_EQUIP_CONFIG_HPP__