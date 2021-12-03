#ifndef _TOWERDEFENDTEAMSKILL_HPP_
#define _TOWERDEFENDTEAMSKILL_HPP_

#include "skill/skillbase.hpp"

class Role;

class TeamTowerDefendSkill : public SkillBase
{
public:
	static const int ADD_MAXHP_FANGYU_SKILL_ID = 0;
	static const int ADD_GONGJI_SKILL_ID = 4;

	TeamTowerDefendSkill() : SkillBase(SKILL_TYPE_ACTIVE, false, false), m_cfg_skill_id(0) {}
	virtual ~TeamTowerDefendSkill() {}

	void SetSkillCfgID(int skill_cfg_id) { m_cfg_skill_id = skill_cfg_id; }

	void SetParamA(int param_a, int level)
	{
		if (level >= 1 && level <= MAX_SKILL_LEVEL)
		{
			m_param_a[level] = param_a;
		}
	}

	void SetParamB(int param_b, int level)
	{
		if (level >= 1 && level <= MAX_SKILL_LEVEL)
		{
			m_param_b[level] = param_b;
		}
	}

	void SetParamC(int param_c, int level)
	{
		if (level >= 1 && level <= MAX_SKILL_LEVEL)
		{
			m_param_c[level] = param_c;
		}
	}

	void SetParamD(int param_d, int level)
	{
		if (level >= 1 && level <= MAX_SKILL_LEVEL)
		{
			m_param_d[level] = param_d;
		}
	}

	void SetAttackDis(int dis) { m_attack_dis = dis; }

	void SetCD(int cd_s, int level)
	{
		if (level >= 1 && level <= MAX_SKILL_LEVEL)
		{
			m_cooldown_time_list[level] = cd_s;

			if (m_cooldown_time_list[level] < 0) m_cooldown_time_list[level] = 0;
		}
	}

	void SetHurtPercent(int percent, int level)
	{
		if (level >= 1 && level <= MAX_SKILL_LEVEL)
		{
			m_hurt_percent_list[level] = percent;
		}
	}

	void SetFixHurt(int fix_hurt, int level)
	{
		if (level >= 1 && level <= MAX_SKILL_LEVEL)
		{
			m_fix_hurt_list[level] = fix_hurt;
		}
	}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "TowerDefendSkill"; }

private:
	int m_cfg_skill_id;

	static const int AOE_ATK_ENEMY_COUNT = 20;

	bool CheckDis(const Posi &src_pos, const Posi &dst_pos) const;

	void DoSkill1(Role *role, int level, Character *target, const Posi &pos) const;
	void DoSkill2(Role *role, int level, Character *target, const Posi &pos) const;
	void DoSkill3(Role *role, int level, Character *target, const Posi &pos) const;
	void DoSkill6(Role *role, int level, Character *target, const Posi &pos) const;
	void DoSkill7(Role *role, int level, Character *target, const Posi &pos) const;
	void DoSkill8(Role *role, int level, Character *target, const Posi &pos) const;
	void DoSkill9(Role *role, int level, Character *target, const Posi &pos) const;
	void DoSkill10(Role *role, int level, Character *target, const Posi &pos) const;
	void DoSkill11(Role *role, int level, Character *target, const Posi &pos) const;
	void DoSkill12(Role *role, int level, Character *target, const Posi &pos) const;
};

#endif
