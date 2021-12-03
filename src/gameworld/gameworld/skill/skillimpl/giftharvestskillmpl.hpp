#ifndef __GIFT_HARVEST_SKILL_IMPL_HPP__
#define __GIFT_HARVEST_SKILL_IMPL_HPP__

#include "skill/skillbase.hpp"

/////////////////////////////////  礼物收割技能技能  ////////////////////////////////////////////////////////////////////////////////

// 击退
class GiftHarvestSkill701 : public SkillBase
{
public:
	GiftHarvestSkill701() : SkillBase(SKILL_TYPE_ACTIVE, true, true) {}
	virtual ~GiftHarvestSkill701() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s701"; }
};

// 眩晕
class GiftHarvestSkill702 : public SkillBase
{
public:
	GiftHarvestSkill702() : SkillBase(SKILL_TYPE_ACTIVE, false, false) {}
	virtual ~GiftHarvestSkill702() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s702"; }
};

// 嘲讽
class GiftHarvestSkill703 : public SkillBase
{
public:
	GiftHarvestSkill703() : SkillBase(SKILL_TYPE_ACTIVE, true, true) {}
	virtual ~GiftHarvestSkill703() {}

	virtual bool LoadConfig(const char *dir, std::string *err) { return true; }
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return "s703"; }
};

#endif // __GIFT_HARVEST_SKILL_IMPL_HPP__


