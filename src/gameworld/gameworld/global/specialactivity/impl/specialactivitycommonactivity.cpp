#include "specialactivitycommonactivity.hpp"

SpecialActivityCommonActivity::SpecialActivityCommonActivity(SpecialActivityManager *special_activity_manager)
	: SpecialActivity(special_activity_manager, SPECIAL_ACTIVITY_TYPE_COMMON_ACTIVITY)
{

}

SpecialActivityCommonActivity::~SpecialActivityCommonActivity()
{

}

void SpecialActivityCommonActivity::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	SpecialActivity::OnSpecialActivityStatusChange(from_status, to_status);
}

void SpecialActivityCommonActivity::Update(unsigned long interval, time_t now_second)
{
	SpecialActivity::Update(interval, now_second);
}

void SpecialActivityCommonActivity::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{

	}
}


