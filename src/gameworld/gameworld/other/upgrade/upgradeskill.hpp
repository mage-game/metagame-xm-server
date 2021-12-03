#ifndef __UPGRADE_SKILL_HPP__
#define __UPGRADE_SKILL_HPP__

#include "obj/character/attribute.hpp"
#include "obj/character/character.h"

#include <map>

class Role;
class UpgradeManager;

class UpgradeSkill
{

	struct VirSkillItem
	{
		short upgrade_type = 0;										// 进阶类型
		short vir_skill_index = 0;									// 虚拟技能索引
		short level = 0;											// 层数
		short param_a = 0;
		short param_b = 0;
		short param_c = 0;
		short param_d = 0;
		unsigned int last_perform_skill_timestamp = 0;				// 技能上次触发时间戳
	};

public:
	UpgradeSkill();
	~UpgradeSkill();

	void Init(Role * role, UpgradeManager *upgrade_manager);

	void OnInitFinish();												// 需要初始化计算的操作放这里

	void RecalcActivedSkill();											// 重算一下已激活技能列表

	void OnHurtOther(Character *target, int skill_id, long long injure, int fight_type);					// 攻击
	void OnRoleBeenAttack(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill);			// 被攻击
	void OnAfterHurtOther(Character *target, int skill_id, long long injure, bool from_skill, int fight_type);

	// 攻击触发---------------------------------------------------------------------
	void OnHurtOtherUpgrade2(Character * target, long long injure, bool from_skill);

	// 受击触发---------------------------------------------------------------------


	// 自动触发---------------------------------------------------------------------

	// 主动触发---------------------------------------------------------------------


	// ------------------------------------------------------------------------------
	

private:
	Role *m_role;
	UpgradeManager *m_upgrade_manager;

	std::map<int, VirSkillItem> m_cur_active_skill_map;				// 所有当前激活的技能
};

#endif