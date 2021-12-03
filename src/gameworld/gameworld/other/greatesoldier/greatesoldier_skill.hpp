#ifndef __GREATE_SOLDIER_SKILL_H__
#define __GREATE_SOLDIER_SKILL_H__

#include "servercommon/basedef.h"

class Role;
class GreateSoldier;

class GreateSoldierSkill
{
public:
	void Init(Role *role, GreateSoldier *greate_soldier);

	void CheckActiveSkill();					// 检查激活技能

	void Update(time_t now_second, unsigned long interval);

	void OnRoleBeenAttack(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill);

	void SetBaoji(bool is_baoji) { is_greatesoldier_baoji_skill = is_baoji; }
	bool IsBaoji() { return is_greatesoldier_baoji_skill; }

private:
	void OnChangeSpecialSkill();				// 当改变特殊技能（变身or结束变身时触发）

private:
	Role *m_role = nullptr;
	GreateSoldier *m_greate_soldier = nullptr;

	UInt32 m_special_skill_flag = 0;			// 特殊技能激活标记
	UInt32 m_old_special_skill_flag = 0;		// 旧的特殊技能激活标记

	time_t m_skill_perform_time_11 = 0;			// 上次技能11释放时间
	time_t m_skill_perform_time_13 = 0;			// 上次技能13释放时间

	bool is_greatesoldier_baoji_skill = false;	// 是否暴击技能，百分百产生暴击
};

#endif // __GREATE_SOLDIER_SKILL_H__