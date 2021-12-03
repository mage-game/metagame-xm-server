#ifndef MASKFUNCTION_HPP
#define MASKFUNCTION_HPP

#include "protocal/msgmask.hpp"

class Role;
class Knapsack;

class MaskFunction
{
public:
	// 进阶
	static bool Upgrade(Role *role, bool is_auto_buy, int repeat_times = 1);

	// 直升一阶
	static bool UpOneGrade(Role *role, int valid_min_grade, int valid_max_grade, int return_item_id, int return_num);

	// GM重置阶数
	static bool GMResetGrade(Role *role);

	// 激活
	static bool ActiveMask(Role *role, bool is_recal = true);

	// 特殊形象进阶
	static bool MaskSpecialImgUpgrade(Role *role, short special_img_id);

	// 升级装备
	static bool UplevelEquip(Role *role, int equip_idx);

	// 升级技能
	static bool UplevelSkill(Role *role, int skill_idx, bool is_auto_buy);

};

#endif
