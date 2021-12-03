#ifndef __ACTIVITY_MANAGER_HPP__
#define __ACTIVITY_MANAGER_HPP__

#include "iclockmodule.h"
#include "inetworkmodule.h"

#include "servercommon/activitydef.hpp"

#include "servercommon/internalprotocal/gglobalprotocal.h"

class Activity;
class Role;

class ActivityManager
{
public:
	static ActivityManager & Instance();

	void Update(unsigned long interval, time_t now_second);

	bool IsActivityOpen(int activity_type);
	bool IsActivityClose(int activity_type);

	void OnUserLogin(Role *role);
	void OnUserLogout(Role *role);

	void OnUserEnterScene(Role *role);
	void OnUserLeaveScene(Role *role);

	bool IsActivityLimitGuildOper(int guild_oper_type, Role *role);

	void OnQueryRoomStatusReq(Role *role, int activity_type);

	bool OnForceToNextState(int activity_type);

	Activity * ForceGetActivity(int activity_type);

private:
	ActivityManager();
	~ActivityManager();

	Activity *m_activity_list[ACTIVITY_TYPE_MAX];
};

#endif // __ACTIVITY_MANAGER_HPP__

