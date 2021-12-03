#ifndef __LIFE_SKILL_HPP__
#define __LIFE_SKILL_HPP__

#include "servercommon/lifeskilldef.hpp"

class Role;

class LifeSkillMgr
{
public:
	LifeSkillMgr();
	~LifeSkillMgr();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const LifeSkillParam &p);
	void GetInitParam(LifeSkillParam *p);

	void UpLevelSkill(int skill_type);
	void Make(int index);

	void AddShuliandu(int skill_type, LifeSkill *life_skill, int add_shuliandu);
	void SyncLifeSkillInfo();

private:
	Role *m_role;
	
	LifeSkillParam m_skill_p;
};

#endif // __LIFE_SKILL_HPP__


