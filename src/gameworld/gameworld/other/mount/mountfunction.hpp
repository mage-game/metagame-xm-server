#ifndef MOUNTFUNCTION_HPP
#define MOUNTFUNCTION_HPP

#include "protocal/msgmount.hpp"

class Role;
class Knapsack;

class MountFunction
{
public:
	// ����
	static bool Upgrade(Role *role, bool is_auto_buy, int repeat_times = 1, int *old_level = 0, int *coin_cost = 0, int *gold_cost = 0);

	// ֱ��һ��
	static bool UpOneGrade(Role *role, int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num);

	// ���߽���
	static bool UpgradeByItem(Role *role, int target_grade, int reward_item_count);

	// GM���ý���
	static bool GMResetGrade(Role *role);

	// ����
	static bool ActiveMount(Role *role, bool is_recal = true);

	// �����������
	static bool MountSpecialImgUpgrade(Role *role, short special_img_id);

	// ����װ��
	static bool UplevelEquip(Role *role, int equip_idx);

	// ��������
	static bool UplevelSkill(Role *role, int skill_idx, bool is_auto_buy);

	// ���Ǽ�
	static bool UpStarLevel(Role *role,Protocol::CSMountUpStarLevel *msg);

	// ������������һ����ף��ֵ
	static int UplevelAndSaveBlessValue(Role *role, int add_star);

	//����Ƿ���������
	static bool CheckCanUpGrade(Role* role);

private:
	static bool CheckConsumeAndUpgrade(Role * role, int repeat_times, bool is_auto_buy);
};

#endif
