#ifndef WINGFUNCTION_HPP
#define WINGFUNCTION_HPP

#include "protocal/msgwing.hpp"

class Role;
class Knapsack;

class WingFunction
{
public:
	// 进阶
	static bool Upgrade(Role *role, bool is_auto_buy, int repeat_times = 1, int *old_level = 0, int *coin_cost = 0, int *gold_cost = 0);

	// 直升一阶
	static bool UpOneGrade(Role *role, int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num);

	// GM重置阶数
	static bool GMResetGrade(Role *role);

	// 激活
	static bool ActiveWing(Role *role, bool is_recal = true);

	// 特殊形象进阶
	static bool WingSpecialImgUpgrade(Role *role, short special_img_id);

	// 升级装备
	static bool UplevelEquip(Role *role, int equip_idx);

	// 升级技能
	static bool UplevelSkill(Role *role, int skill_idx, bool is_auto_buy);

	// 升星级
	static bool UpStarLevel(Role *role,Protocol::CSWingUpStarLevel *msg);

	// 升级并保留祝福值
	static int UplevelAndSaveBless(Role *role, int add_star);

	// 检查是否能够升级
	static bool CheckCanUpGrade(Role* role);
};

#endif