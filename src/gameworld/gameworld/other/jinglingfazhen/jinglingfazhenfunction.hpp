#ifndef JINGLINGFAZHEN_HPP
#define JINGLINGFAZHEN_HPP

#include "protocal/msgjinglingfazhen.hpp"

class Role;
class Knapsack;

class JinglingFazhenFunction
{
public:

	// GM重置阶数
	static bool GMResetGrade(Role *role);

	// 激活
	static bool ActiveJinglingFazhen(Role *role, bool is_recal = true);

	// 升星级
	static bool UpStarLevel(Role *role,Protocol::CSJinglingFazhenUpStarLevel *msg);

	// 特殊形象进阶
	static bool JinglingFazhenSpecialImgUpgrade(Role *role, short special_img_id);

};

#endif