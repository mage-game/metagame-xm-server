#ifndef __CROSS_ACTIVITY_HOTSPRING_HPP__
#define __CROSS_ACTIVITY_HOTSPRING_HPP__

#include "servercommon/crossdef.hpp"
#include "crossactivity/crossactivity.hpp"

class CrossActivityHS : public CrossActivity
{
public:
	CrossActivityHS(CrossActivityManager *cross_activity_manager);
	virtual ~CrossActivityHS();

	virtual void Init(const CrossActivityData &data);

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnDayChange();
	virtual void OnWeekChange();

	virtual bool CheckCanStartCross(const UniqueUserID &unique_user_id, int merge_server_id = 0);

	virtual void OnCrossUserLogin(CrossUser *cross_user);

private:
	time_t CalcActivityNextOpenTime();
	virtual void OnChangeToNextStatus();
};

#endif // __CROSS_ACTIVITY_HOTSPRING_HPP__


