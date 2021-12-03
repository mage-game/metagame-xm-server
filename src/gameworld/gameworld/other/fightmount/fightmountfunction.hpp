#ifndef FIGHTMOUNTFUNCTION_HPP
#define FIGHTMOUNTFUNCTION_HPP

#include "protocal/msgfightmount.hpp"

class Role;
class Knapsack;

class FightMountFunction
{
public:
	// 进阶
	static bool Upgrade(Role *role, bool is_auto_buy, int repeat_times = 1, int *old_level = 0, int *coin_cost = 0, int *gold_cost = 0);

	// 直升一阶
	static bool UpOneGrade(Role *role, int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num);

	// 道具进阶
	static bool UpgradeByItem(Role *role, int target_grade, int reward_item_count);

	// GM重置阶数
	static bool GMResetGrade(Role *role);

	// 激活
	static bool ActiveFightMount(Role *role, bool is_recal = true);

	// 特殊形象进阶
	static bool FightMountSpecialImgUpgrade(Role *role, short special_img_id);

	// 升级装备
	static bool UplevelEquip(Role *role, int equip_idx);

// 	// 升级技能
	static bool UplevelSkill(Role *role, int skill_idx, bool is_auto_buy);

	// 升星级
	static bool UpStarLevel(Role *role, Protocol::CSFightMountUpStarLevel *msg);

	// 升星助力
	static int UplevelAndSaveBless(Role *role, int add_star);

	// 检查是否激活
	static bool CheckCanUpGrade(Role* role);

};

#endif
