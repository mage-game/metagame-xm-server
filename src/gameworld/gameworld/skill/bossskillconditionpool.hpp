#ifndef __BOSS_SKILL_CONDITION_POOL_HPP__
#define __BOSS_SKILL_CONDITION_POOL_HPP__

#include <string>

#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

class BossSkillConditionConfig;

class BossSkillConditionPool : public ConfigBase
{
public:
	BossSkillConditionPool();
	~BossSkillConditionPool();

	bool Init(const std::string &configname, std::string *err);

	const BossSkillConditionConfig * GetBossSkillConditionConfig(UInt16 bossskill_cond_id) const;

private:
	void Release();

	static const int MAX_BOSS_SKILL_CONDITION_ID = (int)USHRT_MAX + 1;
	BossSkillConditionConfig *m_boss_skill_cond_list[MAX_BOSS_SKILL_CONDITION_ID];
};

#endif // __BOSS_SKILL_CONDITION_POOL_HPP__

