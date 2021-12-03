#ifndef QILINBIFUNCTION_HPP
#define QILINBIFUNCTION_HPP

#include "protocal/msgqilinbi.hpp"

class Role;
class Knapsack;

class QiLinBiFunction
{
public:
	// ����
	static bool Upgrade(Role *role, bool is_auto_buy, int repeat_times = 1);

	// ֱ��һ��
	static bool UpOneGrade(Role *role, int valid_min_grade, int valid_max_grade, int return_item_id, int return_num);

	// GM���ý���
	static bool GMResetGrade(Role *role);

	// ����
	static bool ActiveQiLinBi(Role *role, bool is_recal = true);

	// �����������
	static bool QiLinBiSpecialImgUpgrade(Role *role, short special_img_id);

	// ����װ��
	static bool UplevelEquip(Role *role, int equip_idx);

	// ��������
	static bool UplevelSkill(Role *role, int skill_idx, bool is_auto_buy);

};

#endif
