#ifndef __ROLE_SKILL_IMPL_HPP__
#define __ROLE_SKILL_IMPL_HPP__

#include "skill/skillbase.hpp"

#define ROLE_ACTIVE_SKILL_CLASS(SKILL_ID, IS_ATTACK_CHARACTER, IS_TARGET_ENEMY) \
class RoleSkill##SKILL_ID : public SkillBase\
{\
public:\
	RoleSkill##SKILL_ID() : SkillBase(SKILL_TYPE_ACTIVE, IS_ATTACK_CHARACTER, IS_TARGET_ENEMY) { m_fight_status_policy = FIGHT_STATUS_POLICY_PERFORM; }\
	virtual ~RoleSkill##SKILL_ID() {}\
	virtual bool LoadConfig(const char *dir, std::string *err) { return true; };\
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;\
	virtual long long PerformSpecial(Character *chara, int level, Character *target, bool is_main_target,int &fight_type, bool is_baoji, double add_hurt_percent) const;\
	virtual const char* GetSkillName() { return "s"#SKILL_ID; }\
};

#define ROLE_PASSIVE_SKILL_CLASS(SKILL_ID) \
class RoleSkill##SKILL_ID : public SkillBase\
{\
public:\
	RoleSkill##SKILL_ID() : SkillBase(SKILL_TYPE_PASSIVE, false, false) {}\
	virtual ~RoleSkill##SKILL_ID() {}\
	virtual bool LoadConfig(const char *dir, std::string *err) { return true; };\
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const {};\
	virtual void ReCalcAttr(Character *chara, int skill_level, CharIntAttrs &base_add) const;\
	virtual const char* GetSkillName() { return "s"#SKILL_ID; }\
};

ROLE_ACTIVE_SKILL_CLASS(111, true, true)
ROLE_ACTIVE_SKILL_CLASS(121, true, true)
ROLE_ACTIVE_SKILL_CLASS(131, true, true)
ROLE_ACTIVE_SKILL_CLASS(141, true, true)

ROLE_ACTIVE_SKILL_CLASS(211, true, true)
ROLE_ACTIVE_SKILL_CLASS(221, true, true)
ROLE_ACTIVE_SKILL_CLASS(231, true, true)
ROLE_ACTIVE_SKILL_CLASS(241, true, true)

ROLE_ACTIVE_SKILL_CLASS(311, true, true)
ROLE_ACTIVE_SKILL_CLASS(321, true, true)
ROLE_ACTIVE_SKILL_CLASS(331, true, true)
ROLE_ACTIVE_SKILL_CLASS(341, true, true)

ROLE_ACTIVE_SKILL_CLASS(411, true, true)
ROLE_ACTIVE_SKILL_CLASS(421, true, true)
ROLE_ACTIVE_SKILL_CLASS(431, true, true)
ROLE_ACTIVE_SKILL_CLASS(441, true, true)

//////////////////////////////////////////////////////////////////////////
// 攻城战城主技能
ROLE_ACTIVE_SKILL_CLASS(6, true, true)

//////////////////////////////////////////////////////////////////////////
// 跨服BOSS排行榜技能
ROLE_ACTIVE_SKILL_CLASS(7, false, false)

//////////////////////////////////////////////////////////////////////////
// 心法
ROLE_PASSIVE_SKILL_CLASS(41)
ROLE_PASSIVE_SKILL_CLASS(42)
ROLE_PASSIVE_SKILL_CLASS(43)
ROLE_PASSIVE_SKILL_CLASS(44)
ROLE_PASSIVE_SKILL_CLASS(45)
ROLE_PASSIVE_SKILL_CLASS(46)
ROLE_PASSIVE_SKILL_CLASS(47)

//////////////////////////////////////////////////////////////////////////
//武器被动技能
//眩晕
class RoleSkill200 : public SkillBase
{
public:
	RoleSkill200() : SkillBase(SKILL_TYPE_PASSIVE, false, false) {}
	virtual ~RoleSkill200() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; };
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const {};
	virtual const char * GetSkillName() { return "s200"; }
};

//减伤
class RoleSkill201 : public SkillBase
{
public:
	RoleSkill201() : SkillBase(SKILL_TYPE_PASSIVE, false, false) {}
	virtual ~RoleSkill201() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; };
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const {};
	virtual const char * GetSkillName() { return "s201"; }
};

#endif