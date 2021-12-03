#ifndef __SKILLPOOL_HPP__
#define __SKILLPOOL_HPP__

#include "servercommon/basedef.h"
#include "servercommon/servercommon.h"

#include <limits.h>
#include <string>

class SkillBase;
class TiXmlElement;

#define SKILLPOOL SkillPool::Instance()

const static int ROLE_PROF_SKILL_COUNT = 4;							// 每个职业专属技能的个数

// 角色主动技能id分配规则：职业 = 技能id / 100; 第几个主动技能 = 技能id % 100 / 10

class SkillPool
{
public:
	static const int MAX_SKILL_ID = (int)USHRT_MAX + 1;
	
	SkillPool();
	~SkillPool();

	static SkillPool * Instance();

	bool Init(const char *config_dir, std::string *err);
	static bool Reload(const char *config_dir, std::string *err);

	const SkillBase * GetSkill(UInt16 skillid) const;
	UInt16 GetSkillId(const char &prof, int index) const;

private:
	bool CheckSkillInit(std::string *err) const;
	bool LoadMonsterSkill(const std::string &skill_dir, const std::string &name, std::string *err);
	bool LoadRoleSkill(const std::string &skill_dir, const std::string &name, std::string *err);

	int LoadNormalSkill(TiXmlElement *noraml_element, std::string *err);

	void Release();

	SkillBase *m_skill_list[MAX_SKILL_ID];

	int m_prof_skill_count_list[PROF_BASE_TYPE_MAX + 1];
	UInt16 m_prof_skill_list[PROF_BASE_TYPE_MAX + 1][ROLE_PROF_SKILL_COUNT];
};

#endif


