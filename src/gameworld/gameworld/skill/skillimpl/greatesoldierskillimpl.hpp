//-------------------------------------------------------------------------------------------

// ������������

//-------------------------------------------------------------------------------------------

#ifndef __GREATE_SOLDIER_SKILL_IMPL_HPP__
#define __GREATE_SOLDIER_SKILL_IMPL_HPP__

#include "skill/skillbase.hpp"

// �����չ�
class GreateSoldierSkill600 : public SkillBase
{
public:
	GreateSoldierSkill600() : SkillBase(SKILL_TYPE_ACTIVE, true, true) {}
	virtual ~GreateSoldierSkill600() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s600"; }
};

// ����Ⱥ��1��100%��Ѫ
class GreateSoldierSkill601 : public SkillBase
{
public:
	GreateSoldierSkill601() : SkillBase(SKILL_TYPE_ACTIVE, false, false) {}
	virtual ~GreateSoldierSkill601() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s601"; }
};

// ����Ⱥ��2,100%����
class GreateSoldierSkill602 : public SkillBase
{
public:
	GreateSoldierSkill602() : SkillBase(SKILL_TYPE_ACTIVE, false, false) {}
	virtual ~GreateSoldierSkill602() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s602"; }
};

// �������⼼��11
class GreateSoldierSkill603 : public SkillBase
{
public:
	GreateSoldierSkill603() : SkillBase(SKILL_TYPE_ACTIVE, true, true) {}
	virtual ~GreateSoldierSkill603() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s603"; }
};

// �������⼼��14
class GreateSoldierSkill604 : public SkillBase
{
public:
	GreateSoldierSkill604() : SkillBase(SKILL_TYPE_ACTIVE, true, true) {}
	virtual ~GreateSoldierSkill604() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s604"; }
};

// �������⼼��15
class GreateSoldierSkill605 : public SkillBase
{
public:
	GreateSoldierSkill605() : SkillBase(SKILL_TYPE_ACTIVE, true, true) {}
	virtual ~GreateSoldierSkill605() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s605"; }
};

// �������⼼��16
class GreateSoldierSkill606 : public SkillBase
{
public:
	GreateSoldierSkill606() : SkillBase(SKILL_TYPE_ACTIVE, true, true) {}
	virtual ~GreateSoldierSkill606() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s606"; }
};

#endif // __GREATE_SOLDIER_SKILL_IMPL_HPP__