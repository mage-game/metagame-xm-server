#ifndef __ROLE_SKILL_CONDITION_HPP__
#define __ROLE_SKILL_CONDITION_HPP__

#include "skilldef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include <string>
#include <map>

class RoleSkillCondition : public ConfigBase
{
public:
	struct CondInfo
	{
		CondInfo() : type(0), global_cd(0) {}
		int type;
		int global_cd;
	};

	RoleSkillCondition();
	~RoleSkillCondition();

	bool Init(const std::string &path, std::string *err);

	int GetSkillCondType(unsigned short skill_id);
	int GetGlobalCD(unsigned short skill_id);

private:
	typedef std::map<unsigned short, CondInfo> SkillCondMarkMap;
	typedef SkillCondMarkMap::iterator SkillCondMarkMapIt;
	SkillCondMarkMap m_roleskill_cond_map;
};

#endif	// __ROLE_SKILL_CONDITION_HPP__


