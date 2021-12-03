#ifndef __RAND_ACTIVITY_CORNUCOPIA_HPP__
#define __RAND_ACTIVITY_CORNUCOPIA_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityCornucopia : public RandActivity
{
public:
	RandActivityCornucopia(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityCornucopia();

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	void AddRewardRecord(const RACornucopiaRewardRecord &record);
	int GetRewardRecordList(RACornucopiaRewardRecord reward_record_list[RAND_ACTIVITY_CORNUCOPIA_RECORD_MAX_COUNT]);

protected:
	int m_reward_record_count;
	int m_reward_record_cursor;
	RACornucopiaRewardRecord m_reward_record_list[RAND_ACTIVITY_CORNUCOPIA_RECORD_MAX_COUNT];
};

#endif

