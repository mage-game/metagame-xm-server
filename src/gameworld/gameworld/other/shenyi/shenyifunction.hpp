#ifndef SHENYIFUNCTION_HPP
#define SHENYIFUNCTION_HPP

#include "protocal/msgshenyi.hpp"

class Role;
class Knapsack;

class ShenyiFunction
{
public:
	// ����
	static bool Upgrade(Role *role, bool is_auto_buy, int repeat_times = 1, int *old_level = 0, int *coin_cost = 0, int *gold_cost = 0);

	// ֱ��һ��
	static bool UpOneGrade(Role *role, int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num);

	// GM���ý���
	static bool GMResetGrade(Role *role);

	// ����
	static bool ActiveShenyi(Role *role, bool is_recal = true);

	// �����������
	static bool ShenyiSpecialImgUpgrade(Role *role, short special_img_id);

	// ����װ��
	static bool UplevelEquip(Role *role, int equip_idx);

	// ��������
	static bool UplevelSkill(Role *role, int skill_idx, bool is_auto_buy);

	// ���Ǽ�
	static bool UpStarLevel(Role *role,Protocol::CSShenyiUpStarLevel *msg);

	// ������������һ����ף��ֵ
	static int UplevelAndSaveBless(Role *role, int add_star);

	// ����Ƿ񼤻�
	static bool CheckCanUpGrade(Role* role);
};

#endif