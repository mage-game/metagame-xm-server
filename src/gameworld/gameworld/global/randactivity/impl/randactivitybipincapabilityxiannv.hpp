#ifndef __RAND_ACTIVITY_BIPIN_CAPABILITY_XIANNV_HPP__
#define __RAND_ACTIVITY_BIPIN_CAPABILITY_XIANNV_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityBipinCapabilityXiannv : public RandActivity
{
public:
	RandActivityBipinCapabilityXiannv(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityBipinCapabilityXiannv();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);
	virtual void OnPersonRankSnapshot(int old_top_uid_list[PERSON_RANK_TYPE_MAX], int new_top_uid_list[PERSON_RANK_TYPE_MAX]);
	virtual bool IsSyncRankData() { return 0 != m_data_param.need_give_reward; }

private:
	void CheckGiveRankReward();

	struct ActivityDataParam
	{
		ActivityDataParam() : need_give_reward(0), reserve_ch(0), reserve_sh(0) {}

		char need_give_reward;
		char reserve_ch;
		short reserve_sh;
	};

	ActivityDataParam m_data_param;

	time_t m_check_rank_reward_timestamp;
};

#endif

