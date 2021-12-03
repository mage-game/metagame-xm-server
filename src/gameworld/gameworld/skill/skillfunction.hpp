#ifndef SKILLFUNCTION_HPP
#define SKILLFUNCTION_HPP

class Role;

class SkillFunction
{
public:
	static bool LearnSkill(Role *role, short skill_id);

	static bool LearnSkillActiveAll(Role *role);

	static bool LearnSkillPassiveAll(Role *role);

private:

};

#endif
