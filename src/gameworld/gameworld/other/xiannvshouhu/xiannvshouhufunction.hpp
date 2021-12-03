#ifndef XIANNVSHOUHUFUNCTION_HPP
#define XIANNVSHOUHUFUNCTION_HPP

#include "protocal/msgxiannvshouhu.hpp"

class Role;
class Knapsack;

class XiannvShouhuFunction
{
public:

	// GM重置阶数
	static bool GMResetGrade(Role *role);

	// 激活
	static bool ActiveXiannvShouhu(Role *role, bool is_recal = true);

	// 升星级
	static bool UpStarLevel(Role *role,Protocol::CSXiannvShouhuUpStarLevel *msg);

};

#endif