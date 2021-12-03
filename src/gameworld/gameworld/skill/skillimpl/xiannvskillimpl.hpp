#ifndef __XIANNV_SKILL_IMPL_HPP__
#define __XIANNV_SKILL_IMPL_HPP__

#include "skill/skillbase.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class XiannvSkill80 : public SkillBase
{
public:
	XiannvSkill80() : SkillBase(SKILL_TYPE_ACTIVE, true, true) {}
	virtual ~XiannvSkill80() {}

	virtual bool LoadConfig(const char *dir, std::string *err){return true;}
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s80"; }
};

class XiannvSkill81 : public SkillBase
{
public:
	XiannvSkill81() : SkillBase(SKILL_TYPE_ACTIVE, false, false) {}
	virtual ~XiannvSkill81() {}

	virtual bool LoadConfig(const char *dir, std::string *err){return true;}
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s81"; }
};

class XiannvSkill82 : public SkillBase
{
public:
	XiannvSkill82() : SkillBase(SKILL_TYPE_ACTIVE, true, false) {}
	virtual ~XiannvSkill82() {}

	virtual bool LoadConfig(const char *dir, std::string *err){return true;}
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s82"; }
};

class XiannvSkill83 : public SkillBase
{
public:
	XiannvSkill83() : SkillBase(SKILL_TYPE_ACTIVE, true, false) {}
	virtual ~XiannvSkill83() {}

	virtual bool LoadConfig(const char *dir, std::string *err){return true;}
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s83"; }
};

class XiannvSkill84 : public SkillBase
{
public:
	XiannvSkill84() : SkillBase(SKILL_TYPE_ACTIVE, true, true) {}
	virtual ~XiannvSkill84() {}

	virtual bool LoadConfig(const char *dir, std::string *err){return true;}
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s84"; }
};

class XiannvSkill85 : public SkillBase
{
public:
	XiannvSkill85() : SkillBase(SKILL_TYPE_ACTIVE, true, true) {}
	virtual ~XiannvSkill85() {}

	virtual bool LoadConfig(const char *dir, std::string *err){return true;};
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s85"; }
};

class XiannvSkill86 : public SkillBase
{
public:
	XiannvSkill86() : SkillBase(SKILL_TYPE_ACTIVE, true, true) {}
	virtual ~XiannvSkill86() {}

	virtual bool LoadConfig(const char *dir, std::string *err){return true;}
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s86"; }
};

#endif // __XIANNV_SKILL_IMPL_HPP__


