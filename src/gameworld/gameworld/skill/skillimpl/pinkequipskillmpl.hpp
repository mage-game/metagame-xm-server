#ifndef __PINK_EQUPI_SKILL_IMPL_HPP__
#define __PINK_EQUPI_SKILL_IMPL_HPP__

#include "skill/skillbase.hpp"

/////////////////////////////////  粉色装备技能  ////////////////////////////////////////////////////////////////////////////////

class PinkEquipSkill170 : public SkillBase
{
public:
	PinkEquipSkill170() : SkillBase(SKILL_TYPE_ACTIVE, false, false) {}
	virtual ~PinkEquipSkill170() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s1201"; }
};

class PinkEquipSkill171 : public SkillBase
{
public:
	PinkEquipSkill171() : SkillBase(SKILL_TYPE_ACTIVE, true, true) {}
	virtual ~PinkEquipSkill171() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s202"; }
};

class PinkEquipSkill172 : public SkillBase
{
public:
	PinkEquipSkill172() : SkillBase(SKILL_TYPE_ACTIVE, false, false) {}
	virtual ~PinkEquipSkill172() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s203"; }
};

#endif // __PINK_EQUPI_SKILL_IMPL_HPP__


