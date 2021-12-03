#ifndef JINGLINGFAZHEN_HPP
#define JINGLINGFAZHEN_HPP

#include "protocal/msgjinglingfazhen.hpp"

class Role;
class Knapsack;

class JinglingFazhenFunction
{
public:

	// GM���ý���
	static bool GMResetGrade(Role *role);

	// ����
	static bool ActiveJinglingFazhen(Role *role, bool is_recal = true);

	// ���Ǽ�
	static bool UpStarLevel(Role *role,Protocol::CSJinglingFazhenUpStarLevel *msg);

	// �����������
	static bool JinglingFazhenSpecialImgUpgrade(Role *role, short special_img_id);

};

#endif