#ifndef __ROLE_GOLD_HPP__
#define __ROLE_GOLD_HPP__

class Role;

#include "servercommon/struct/rolegoalparam.hpp"
#include "obj/character/character.h"

class RoleGoal
{
public:
	RoleGoal();
	~RoleGoal();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const RoleGoalParam &param);
	void GetInitParam(RoleGoalParam *param);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnRoleLevelChange(int role_level);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void RoleGoalAddRecordTimes(int cond_type, long long param1, int param2);
	void CheckCond(int cond_type, long long param1, int param2);
	void FetchPersonalGoalReward();
	void OnUplevelSkill(int skill_type);

	void CheckBattleFieldCond(int field_type, int param1);
	void FetchBattleFieldGoalReward(int field_type);

	int GetGoalExpAdd(){return m_kill_monster_exp_add;}
	int GetHurtMonsterAdd(){return m_hurt_monster_add;}

	void OnAbsordBlood(long long injure);

	void SendInfoToRole();

	bool IsFinishGoal(int cond_type);

	RoleGoalParam::RoleGoalRecord & GetRoleGoldRecord(){return m_param.role_goal_record;}

private:

	void CalcGoalExpAdd();
	void CalcGoalHurtAdd();
	void CalcGoalAbsord();

	Role *m_role;
	RoleGoalParam m_param;

	int m_kill_monster_exp_add;
	int m_hurt_monster_add;
	unsigned int m_absord_blood_colddown_time;
	int m_absord_blood_per;
	int m_absord_blood_trigger_per;

	int m_other_capability;
};

#endif

