#ifndef JINGLINGGUANGHUAN_HPP
#define JINGLINGGUANGHUAN_HPP

#include "protocal/msgjinglingguanghuan.hpp"

class Role;
class Knapsack;

class JinglingGuanghuanFunction
{
public:

	// GM���ý���
	static bool GMResetGrade(Role *role);

	// ����
	static bool ActiveJinglingGuanghuan(Role *role, bool is_recal = true);

	// ���Ǽ�
	static bool UpStarLevel(Role *role,Protocol::CSJinglingGuanghuanUpStarLevel *msg);

	// �����������
	static bool JinglingGuanghuanSpecialImgUpgrade(Role *role, short special_img_id);

};

#endif