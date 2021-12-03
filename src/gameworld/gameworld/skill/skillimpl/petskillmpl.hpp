#ifndef __PET_SKILL_IMPL_HPP__
#define __PET_SKILL_IMPL_HPP__

#include "skill/skillbase.hpp"

static const int PRCENT_OF_PET_SKILL = 10000;
UNSTD_STATIC_CHECK(PRCENT_OF_PET_SKILL > 0);
//////////////////////////////////////////////////////////////////////////
// ³èÎï¼¼ÄÜ
class PetSkill146 : public SkillBase
{
public:
	PetSkill146() : SkillBase(SKILL_TYPE_PASSIVE, false, false) {}
	virtual ~PetSkill146() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const {}
	virtual void ReCalcAttr(Character *chara, int skill_level, CharIntAttrs &base_add) const;
	virtual const char * GetSkillName() { return "s146"; }
};

class PetSkill147 : public SkillBase
{
public:
	PetSkill147() : SkillBase(SKILL_TYPE_PASSIVE, true, true) {}
	virtual ~PetSkill147() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const {}
	virtual void ReCalcAttr(Character *chara, int skill_level, CharIntAttrs &base_add) const;
	virtual const char * GetSkillName() { return "s147"; }
};

class PetSkill148 : public SkillBase
{
public:
	PetSkill148() : SkillBase(SKILL_TYPE_PASSIVE, false, false) {}
	virtual ~PetSkill148() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const {}
	virtual void ReCalcAttr(Character *chara, int skill_level, CharIntAttrs &base_add) const;
	virtual const char * GetSkillName() { return "s148"; }
};

class PetSkill149 : public SkillBase
{
public:
	PetSkill149() : SkillBase(SKILL_TYPE_PASSIVE, false, false) {}
	virtual ~PetSkill149() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const {}
	virtual void ReCalcAttr(Character *chara, int skill_level, CharIntAttrs &base_add) const;
	virtual const char * GetSkillName() { return "s149"; }
};

class PetSkill150 : public SkillBase
{
public:
	PetSkill150() : SkillBase(SKILL_TYPE_PASSIVE, false, false) {}
	virtual ~PetSkill150() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const {}
	virtual void ReCalcAttr(Character *chara, int skill_level, CharIntAttrs &base_add) const;
	virtual const char * GetSkillName() { return "s150"; }
};

#endif // __PET_SKILL_IMPL_HPP__


