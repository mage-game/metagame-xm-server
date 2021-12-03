#ifndef __CROSS_PASTURE_SKILL_IMPL_HPP__
#define __CROSS_PASTURE_SKILL_IMPL_HPP__

#include "skill/skillbase.hpp"

/////////////////////////////////  跨服牧场技能  ////////////////////////////////////////////////////////////////////////////////

class CrossPastureSkill250 : public SkillBase
{
public:
	CrossPastureSkill250() : SkillBase(SKILL_TYPE_ACTIVE, true, true) {}
	virtual ~CrossPastureSkill250() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s250"; }
};

class CrossPastureSkill251 : public SkillBase
{
public:
	CrossPastureSkill251() : SkillBase(SKILL_TYPE_ACTIVE, false, false) {}
	virtual ~CrossPastureSkill251() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s251"; }
};

class CrossPastureSkill252 : public SkillBase
{
public:
	CrossPastureSkill252() : SkillBase(SKILL_TYPE_ACTIVE, false, false) {}
	virtual ~CrossPastureSkill252() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s252"; }
};

class CrossPastureSkill253 : public SkillBase
{
public:
	CrossPastureSkill253() : SkillBase(SKILL_TYPE_ACTIVE, true, true) {}
	virtual ~CrossPastureSkill253() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s253"; }
};

#endif // __CROSS_PASTURE_SKILL_IMPL_HPP__


