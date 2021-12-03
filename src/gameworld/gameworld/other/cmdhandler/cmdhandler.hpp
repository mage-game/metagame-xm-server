#ifndef __CMDHANDLER_H__
#define __CMDHANDLER_H__

class Role;

class CmdHandler
{
public:
	static CmdHandler & Instance();

	bool OnCmdToRole(Role *role, int effect_type, int effect_param, int effect_param1);
	bool OnCmdToRoleCount(Role *role, int count_type, int count, bool only_reduce);
	bool OnCmdToRoleFbCount(Role *role, int day_count_id, int count, bool only_reduce);

	bool OnAddMountWithData(Role *role, int mount_id, int grade_level, int strengthen_level);
	bool OnAddPetWithData(Role *role, int pet_id, int level, int wuxing, int grow, int zizhi);

private:
	CmdHandler();
	~CmdHandler();
};

#endif
