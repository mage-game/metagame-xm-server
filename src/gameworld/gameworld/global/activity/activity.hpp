#ifndef __ACTIVITY_HPP__
#define __ACTIVITY_HPP__

#include "iclockmodule.h"

#include "servercommon/activitydef.hpp"
#include "servercommon/serverdef.h"

class ActivityManager;

class Activity
{
public:
	Activity(ActivityManager *activity_manager, int m_activity_type);
	virtual ~Activity();

	virtual void OnActivityStatusChange(int from_status, int to_status);

	virtual void OnLeaveActivity(Role *role) {}

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnUserLogin(Role *role);
	virtual void OnUserLogout(Role *role);
	virtual void OnUserEnterScene(Role *role);
	virtual void OnUserLeaveScene(Role *role);

	virtual int GetRoomMaxUserCount() { return ACTIVITY_ROOM_DEFAULT_USER_MAX; }

	virtual bool ForceToNextState();
	int GetActivityStatus() { return m_activity_status; }

	void OnQueryRoomStatusReq(Role *role);

	int GetActivityType() { return m_activity_type; }

protected:
	void NotfiyActivityStatus(bool is_broadcast = false, Role *role = NULL);

	virtual bool IsRecordRoomStatus(int *activity_sceneid = NULL) { return false; }

	ActivityManager *m_activity_manager;
	
	int m_activity_type;

	int m_activity_status;
	unsigned int m_next_status_switch_time;
	unsigned int m_first_next_status_switch_time;
	int m_data;

	int m_next_open_wday;
	StatusTimeCfgList m_statustime_list_cfg;

	ActivityRoomStatus m_room_status_list[ACTIVITY_ROOM_MAX];			// 房间状态列表 若有的话 
};

#endif // __ACTIVITY_HPP__

