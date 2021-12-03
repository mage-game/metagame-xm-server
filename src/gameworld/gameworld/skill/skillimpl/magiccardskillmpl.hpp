#ifndef __MAGIC_CARD_SKILL_IMPL_HPP__
#define __MAGIC_CARD_SKILL_IMPL_HPP__

#include "skill/skillbase.hpp"

/////////////////////////////////  Ä§¿¨¼¼ÄÜ  ////////////////////////////////////////////////////////////////////////////////

class MagicCardSkill200 : public SkillBase
{
public:
	MagicCardSkill200() : SkillBase(SKILL_TYPE_ACTIVE, true, true) {}
	virtual ~MagicCardSkill200() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s200"; }
};

class MagicCardSkill201 : public SkillBase
{
public:
	MagicCardSkill201() : SkillBase(SKILL_TYPE_ACTIVE, false, false) {}
	virtual ~MagicCardSkill201() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s1201"; }
};

class MagicCardSkill202 : public SkillBase
{
public:
	MagicCardSkill202() : SkillBase(SKILL_TYPE_ACTIVE, true, true) {}
	virtual ~MagicCardSkill202() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s202"; }
};

class MagicCardSkill203 : public SkillBase
{
public:
	MagicCardSkill203() : SkillBase(SKILL_TYPE_ACTIVE, true, false) {}
	virtual ~MagicCardSkill203() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s203"; }
};

#endif // __XIANNV_SKILL_IMPL_HPP__


