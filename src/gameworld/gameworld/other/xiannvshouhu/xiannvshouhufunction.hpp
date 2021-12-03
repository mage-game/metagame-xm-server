#ifndef XIANNVSHOUHUFUNCTION_HPP
#define XIANNVSHOUHUFUNCTION_HPP

#include "protocal/msgxiannvshouhu.hpp"

class Role;
class Knapsack;

class XiannvShouhuFunction
{
public:

	// GM���ý���
	static bool GMResetGrade(Role *role);

	// ����
	static bool ActiveXiannvShouhu(Role *role, bool is_recal = true);

	// ���Ǽ�
	static bool UpStarLevel(Role *role,Protocol::CSXiannvShouhuUpStarLevel *msg);

};

#endif