#ifndef JINGLINGGUANGHUAN_HPP
#define JINGLINGGUANGHUAN_HPP

#include "protocal/msgjinglingguanghuan.hpp"

class Role;
class Knapsack;

class JinglingGuanghuanFunction
{
public:

	// GM重置阶数
	static bool GMResetGrade(Role *role);

	// 激活
	static bool ActiveJinglingGuanghuan(Role *role, bool is_recal = true);

	// 升星级
	static bool UpStarLevel(Role *role,Protocol::CSJinglingGuanghuanUpStarLevel *msg);

	// 特殊形象进阶
	static bool JinglingGuanghuanSpecialImgUpgrade(Role *role, short special_img_id);

};

#endif