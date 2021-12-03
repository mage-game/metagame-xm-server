#ifndef __SPECIAL_ACTIVITY_COMMON_ACTIVITY_HPP__
#define __SPECIAL_ACTIVITY_COMMON_ACTIVITY_HPP__

#include <map>

#include "global/specialactivity/specialactivity.hpp"
#include "servercommon/activitydef.hpp"

class SpecialActivityCommonActivity : public SpecialActivity
{
public:
	SpecialActivityCommonActivity(SpecialActivityManager *special_activity_manager);
	virtual ~SpecialActivityCommonActivity();

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

private:
};

#endif // __SPECIAL_ACTIVITY_COMMON_ACTIVITY_HPP__

