#ifndef __RAND_ACTIVITY_DAY_ONLINE_GIFT_HPP__
#define __RAND_ACTIVITY_DAY_ONLINE_GIFT_HPP__

#include "global/randactivity/randactivity.hpp"
#include "scene/scenemanager.h"
#include "protocal/msgother.h"

class RandActivityDayOnlineGift : public RandActivity
{
public:
	RandActivityDayOnlineGift(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityDayOnlineGift();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

private:
	void CalcNextSendRewardTime();

	time_t m_next_flush_gift_time;														// 下一次发放奖励时间
};
#endif

