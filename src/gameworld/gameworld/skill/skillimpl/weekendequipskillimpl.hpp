//-------------------------------------------------------------------------------------------

// 周末装备主动技能

//-------------------------------------------------------------------------------------------

#ifndef __WEEKEND_EQUIP_SKILL_IMPL_HPP__
#define __WEEKEND_EQUIP_SKILL_IMPL_HPP__

#include "skill/skillbase.hpp"

class WeekendEquipSkill610 : public SkillBase
{
public:
	WeekendEquipSkill610() : SkillBase(SKILL_TYPE_ACTIVE, false, false) {}
	virtual ~WeekendEquipSkill610() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s610"; }
};

class WeekendEquipSkill611 : public SkillBase
{
public:
	WeekendEquipSkill611() : SkillBase(SKILL_TYPE_ACTIVE, false, false) {}
	virtual ~WeekendEquipSkill611() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s611"; }
};

class WeekendEquipSkill612 : public SkillBase
{
public:
	WeekendEquipSkill612() : SkillBase(SKILL_TYPE_ACTIVE, false, false) {}
	virtual ~WeekendEquipSkill612() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s612"; }
};

#endif // __WEEKEND_EQUIP_SKILL_IMPL_HPP__