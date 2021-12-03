#ifndef __BOSS_SKILL_CONDITION_CONFIG_HPP__
#define __BOSS_SKILL_CONDITION_CONFIG_HPP__

#include <string>

#include "skilldef.hpp"
#include "servercommon/basedef.h"
#include "servercommon/struct/skilllistparam.h"

class BossSkillConditionConfig
{
public:
	BossSkillConditionConfig();
	~BossSkillConditionConfig();

	bool LoadConfig(const char *path, std::string *err);

	UInt16 GetBossSkillCondID() const { return m_bossskill_cond_id; }

public:
	bool AddBossSkillID(UInt16 skill_id, int *skill_index = NULL);

	UInt16 m_bossskill_cond_id;

	int m_skill_count;
	UInt16 m_skill_list[SkillListParam::MAX_SKILL_NUM];

	bool m_goback;
	bool m_hate_drive;
	BossSkillTypeCond m_typecond_list[BOSS_SKILL_COND_MAX];
};

#endif // __BOSS_SKILL_CONDITION_CONFIG_HPP__

