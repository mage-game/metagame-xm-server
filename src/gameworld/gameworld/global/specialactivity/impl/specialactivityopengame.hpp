#ifndef __SPECIALACTIVITY_OPENGAME_HPP__
#define __SPECIALACTIVITY_OPENGAME_HPP__

#include "global/specialactivity/specialactivity.hpp"

class SpecialActivityOpenGame : public SpecialActivity
{
public:
	SpecialActivityOpenGame(SpecialActivityManager *special_activity_manager);
	virtual ~SpecialActivityOpenGame();

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

private:
};

#endif
