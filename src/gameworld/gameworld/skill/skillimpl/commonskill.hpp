#ifndef __COMMONSKILL_HPP__
#define __COMMONSKILL_HPP__

#include "skill/skillbase.hpp"

#define ROLE_NORMAL_SKILL_CLASS(SKILL_ID) \
class NormalSkill##SKILL_ID : public SkillBase\
{\
public:\
	NormalSkill##SKILL_ID() : SkillBase(SKILL_TYPE_ACTIVE, true, true) { m_fight_status_policy = FIGHT_STATUS_POLICY_PERFORM; }\
	virtual ~NormalSkill##SKILL_ID() {}\
	virtual bool LoadConfig(const char *dir, std::string *err) { return true; };\
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;\
	virtual const char* GetSkillName() { return "s"#SKILL_ID; }\
};

ROLE_NORMAL_SKILL_CLASS(1)
ROLE_NORMAL_SKILL_CLASS(2)
ROLE_NORMAL_SKILL_CLASS(3)
ROLE_NORMAL_SKILL_CLASS(4)


//转职技能---------------------------------------------------------------------------------------------------------------------
// 对目标造成500%的伤害并使对方进入禁锢状态，持续5秒
ROLE_NORMAL_SKILL_CLASS(180)

// 恢复10%的血量，并在5秒内提高5000点防御
class NormalSkill181 : public SkillBase
{
public:
	NormalSkill181() : SkillBase(SKILL_TYPE_ACTIVE, false, false) {}
	virtual ~NormalSkill181() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char *GetSkillName() { return "s181"; }
};

// 使周围X个目标眩晕param_a毫秒
ROLE_NORMAL_SKILL_CLASS(182)

// 自身获得param_a万分比的反弹效果，持续param_b毫秒
class NormalSkill183 : public SkillBase
{
public:
	NormalSkill183() : SkillBase(SKILL_TYPE_ACTIVE, false, false) {}
	virtual ~NormalSkill183() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char *GetSkillName() { return "s183"; }
};

// 使周围目标防御降低param_a万分比，持续param_b毫秒
ROLE_NORMAL_SKILL_CLASS(184)

// 吸血,目标伤害的param_a万分比，持续param_b毫秒
class NormalSkill185 : public SkillBase
{
public:
	NormalSkill185() : SkillBase(SKILL_TYPE_ACTIVE, false, false) {}
	virtual ~NormalSkill185() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char *GetSkillName() { return "s185"; }
};

// 范围目标中毒，持续param_a毫秒，每param_b毫秒受到最大血量的param_c万分比伤害，总伤害不超过自身param_d万分比最大血量
ROLE_NORMAL_SKILL_CLASS(186)

// 提高自身parma_a万分比闪避几率，持续param_b毫秒
class NormalSkill187 : public SkillBase
{
public:
	NormalSkill187() : SkillBase(SKILL_TYPE_ACTIVE, false, false) {}
	virtual ~NormalSkill187() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char *GetSkillName() { return "s187"; }
};

// 使范围内目标沉默param_a毫秒
ROLE_NORMAL_SKILL_CLASS(188)

// 自身伤害提高param_a万分比,持续parma_b毫秒
class NormalSkill189 : public SkillBase
{
public:
	NormalSkill189() : SkillBase(SKILL_TYPE_ACTIVE, false, false) {}
	virtual ~NormalSkill189() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char *GetSkillName() { return "s189"; }
};


// 必杀技 （区分职业）----------------------------------------------------------------------------------------------------------
class NuqiSkill101 : public SkillBase
{
public:
	NuqiSkill101() : SkillBase(SKILL_TYPE_NUQI, true, true) {}
	virtual ~NuqiSkill101() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual long long PerformSpecial(Character *chara, int level, Character *target, bool is_main_target,int &fight_type, bool is_baoji, double add_hurt_percent) const;
	virtual const char *GetSkillName() { return "s101"; }
};

class NuqiSkill201 : public SkillBase
{
public:
	NuqiSkill201() : SkillBase(SKILL_TYPE_NUQI, true, true) {}
	virtual ~NuqiSkill201() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual long long PerformSpecial(Character *chara, int level, Character *target, bool is_main_target, int &fight_type, bool is_baoji, double add_hurt_percent) const;
	virtual const char *GetSkillName() { return "s201"; }
};

class NuqiSkill301 : public SkillBase
{
public:
	NuqiSkill301() : SkillBase(SKILL_TYPE_NUQI, true, true) {}
	virtual ~NuqiSkill301() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual long long PerformSpecial(Character *chara, int level, Character *target, bool is_main_target, int &fight_type, bool is_baoji, double add_hurt_percent) const;
	virtual const char *GetSkillName() { return "s301"; }
};

class NuqiSkill401 : public SkillBase
{
public:
	NuqiSkill401() : SkillBase(SKILL_TYPE_NUQI, true, true) {}
	virtual ~NuqiSkill401() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual long long PerformSpecial(Character *chara, int level, Character *target, bool is_main_target, int &fight_type, bool is_baoji, double add_hurt_percent) const;
	virtual const char *GetSkillName() { return "s401"; }
};

#endif

