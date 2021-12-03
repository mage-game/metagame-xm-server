#ifndef __MOJIE_SKILL_IMPL_HPP__
#define __MOJIE_SKILL_IMPL_HPP__

#include "skill/skillbase.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MojieSkill70 : public SkillBase
{
public:
	MojieSkill70() : SkillBase(SKILL_TYPE_ACTIVE, true, true) {}
	virtual ~MojieSkill70() {}

	virtual bool LoadConfig(const char *dir, std::string *err){return true;};
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s70"; }
};

class MojieSkill71 : public SkillBase
{
public:
	MojieSkill71() : SkillBase(SKILL_TYPE_PASSIVE, true, true) {}
	virtual ~MojieSkill71() {}

	virtual bool LoadConfig(const char *dir, std::string *err){return true;};
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s71"; }
};

class MojieSkill72 : public SkillBase
{
public:
	MojieSkill72() : SkillBase(SKILL_TYPE_PASSIVE, true, false) {}
	virtual ~MojieSkill72() {}

	virtual bool LoadConfig(const char *dir, std::string *err){return true;};
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s72"; }
};

class MojieSkill73 : public SkillBase
{
public:
	MojieSkill73() : SkillBase(SKILL_TYPE_PASSIVE, true, false) {}
	virtual ~MojieSkill73() {}

	virtual bool LoadConfig(const char *dir, std::string *err){return true;};
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s73"; }
};


#endif // __MOJIE_SKILL_IMPL_HPP__


