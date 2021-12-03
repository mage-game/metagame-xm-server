#ifndef __UPGRADE_CARD_BUY_HPP__
#define __UPGRADE_CARD_BUY_HPP__

#include "engineadapter.h"
#include "obj/character/attribute.hpp"
#include "servercommon/struct/upgradecardparam.hpp"
#include <map>
#include <functional>

#define UPGRADE_CARD_BUY(role) role->GetUpgradeCardBuy()

// 设置、判断、清除标记位，bit取值范围：[0, 64)
#define SET_BIT(flag, bit) ((flag) |= (UInt64)1 << (bit))
#define IS_BIT_SET(flag, bit) (((flag) & ((UInt64)1 << (bit))) != 0)
#define CLEAR_BIT(flag, bit) ((flag) &= ~((UInt64)1 << (bit)))

class Role;

enum ACTIVITY_RELATED_INDEX
{
	ACTIVITY_RELATED_INDEX_BEGIN = 0,

	ACTIVITY_RELATED_INDEX_0 = ACTIVITY_RELATED_INDEX_BEGIN,
	ACTIVITY_RELATED_INDEX_1,
	ACTIVITY_RELATED_INDEX_2,
	ACTIVITY_RELATED_INDEX_3,
	ACTIVITY_RELATED_INDEX_4,
	ACTIVITY_RELATED_INDEX_5,
	ACTIVITY_RELATED_INDEX_6,
	ACTIVITY_RELATED_INDEX_7,
	ACTIVITY_RELATED_INDEX_8,
	ACTIVITY_RELATED_INDEX_9,
	ACTIVITY_RELATED_INDEX_10,
	ACTIVITY_RELATED_INDEX_11,
	ACTIVITY_RELATED_INDEX_12,
	ACTIVITY_RELATED_INDEX_13,
	ACTIVITY_RELATED_INDEX_14,
	ACTIVITY_RELATED_INDEX_15,
	ACTIVITY_RELATED_INDEX_16,
	ACTIVITY_RELATED_INDEX_17,
	ACTIVITY_RELATED_INDEX_18,
	ACTIVITY_RELATED_INDEX_19,

	ACTIVITY_RELATED_INDEX_END,
};

UNSTD_STATIC_CHECK(ACTIVITY_RELATED_INDEX_END <= UPGRADE_CARD_BUY_MAX_SYSTEM_COUNT);

struct ActivityRelatedInfo
{
	using RelateFunc = std::function<int(Role *role)>;

	ActivityRelatedInfo(int _bit_index, RelateFunc _func) : index(_bit_index), get_grade_func(_func) {};

	int index = 0;
	RelateFunc get_grade_func;
};

class UpgradeCardBuy {
public:
	UpgradeCardBuy() : m_role(nullptr) {}
	~UpgradeCardBuy() {}

	void SetRole(Role *role) { m_role = role; }

	void InitParam(const UpgradeCardBuyParam &param) { m_param = param; }
	void GetInitParam(UpgradeCardBuyParam *param) { *param = m_param; }

	void OnRoleLogin();
	void OnDayChange(UInt32 old_dayid, UInt32 now_dayid);

	void SendUpgradeCardBuyInfo(int activity_id);

	void OnUpgradeCardBuyReq(int activity_id, ItemID item_id);

private:

	void SendAllInfo();

	void UpdateTodayGrade();

	int GetGradeByActivityID(int activity_id);
	int GetIndexByActivityID(int activity_id);

	int GetGradeByThemeID(int theme);
	int GetIndexByThemeID(int theme);

private:

	Role *m_role;

	UpgradeCardBuyParam m_param;

	static std::map<int, ActivityRelatedInfo> m_activity_bitflag_related_map;
};

#endif // __UPGRADE_CARD_BUY_HPP__