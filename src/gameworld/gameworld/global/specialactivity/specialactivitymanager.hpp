#ifndef __SPECIAL_ACTIVITY_MANAGER_HPP__
#define __SPECIAL_ACTIVITY_MANAGER_HPP__

#include "servercommon/activitydef.hpp"

#include "specialactivity.hpp"
#include "servercommon/internalprotocal/gglobalprotocal.h"

class SpecialActivityCombineServer;
class Role;

class SpecialActivityManager
{
public:
	static SpecialActivityManager & Instance();

	void ReInitActivityTime();

	void Update(unsigned long interval, time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void OnUserLogin(Role *role);
	void OnUserLogout(Role *role);

	bool OnForceSetSpecialActivityTime(int special_activity_type, unsigned int begin_time, unsigned int end_time);
	bool IsSpecialActivityOpening(int special_activity_type);

	SpecialActivityCombineServer * GetSpecialActivityCombineServer();

private:
	SpecialActivityManager();
	~SpecialActivityManager();

	SpecialActivity *m_special_activity_list[SPECIAL_ACTIVITY_TYPE_MAX];
};

#endif // __SPECIAL_ACTIVITY_MANAGER_HPP__

