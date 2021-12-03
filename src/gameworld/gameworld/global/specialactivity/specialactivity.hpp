#ifndef __SPECIAL_ACTIVITY_HPP__
#define __SPECIAL_ACTIVITY_HPP__

#include "iclockmodule.h"

#include "servercommon/activitydef.hpp"
#include "servercommon/serverdef.h"

#include "engineadapter.h"

class SpecialActivityManager;
class Role;

class SpecialActivity
{
public:
	SpecialActivity(SpecialActivityManager *special_activity_manager, int activity_type);
	virtual ~SpecialActivity();

	void InitActivityTime();

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid) {}
	
	virtual void OnUserLogin(Role *role);
	virtual void OnUserLogout(Role *role) {}

	void SyncActivityStatusToGameworld();

	virtual bool ForceSetSpecialActivityTime(unsigned int begin_time, unsigned int end_time);

	int GetActivityType() { return m_special_activity_type; }
	int GetStatus() { return m_special_activity_status; }

	unsigned int GetBeginTime() const { return m_begin_time; }
	unsigned int GetEndTime() const { return m_end_time; }

protected:
	void NotfiySpecialActivityStatus(bool is_broadcast = false, Role *role = NULL);

	SpecialActivityManager *m_special_activity_manager;
	int m_special_activity_type;

	int m_special_activity_status;
	unsigned int m_begin_time;
	unsigned int m_end_time;
};

#endif // __SPECIAL_ACTIVITY_HPP__
