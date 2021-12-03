#ifndef __RANDACTIVITY_PROMOTING_POSITION__
#define __RANDACTIVITY_PROMOTING_POSITION__

#include "global/randactivity/randactivity.hpp"

class RandActivityPromotingPosition : public RandActivity
{
public:
	RandActivityPromotingPosition(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityPromotingPosition();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	void OnSpecialActivityStatusChange(int from_status, int to_status);

	int GetRewardRecordsList(RAPromotingPositionRewardRecord *records_list);
	void AddRewardRecord(Role * role, RAPromotingPositionRewardInfo &reward_info);

	int GetRewardBeginIndex() { return m_data_param.reward_begin_index; }
	int GetRewardEndIndex() { return m_data_param.reward_end_index; }

	int GetOutsideBeginIndex() { return m_data_param.outside_begin_index; }
	int GetOutsideEndIndex() { return m_data_param.outside_end_index; }

	int GetInsideBeginIndex() { return m_data_param.inside_begin_index; }
	int GetInsideEndIndex() { return m_data_param.inside_end_index; }

	const int GetServerChouTimes() { return m_data_param.server_chou_times; }
	void AddServerChouTimes(const int add_times) { m_data_param.server_chou_times += add_times; }
	void ClearServerChouTimes() { m_data_param.server_chou_times = 0; }

protected:
	int m_records_count;
	int m_records_last_index;
	RAPromotingPositionRewardRecord m_records_list[RA_PROMOTION_POSITION_REWARD_RECORDS_MAX_COUNT];

private:
	struct ActivityDataParam
	{
		ActivityDataParam() : reward_begin_index(0), reward_end_index(0), outside_begin_index(0), outside_end_index(0), inside_begin_index(0), inside_end_index(0),
			server_chou_times(0)
		{}

		short reward_begin_index;				// 奖励起始下标
		short reward_end_index;					// 奖励结束下标
		short outside_begin_index;				// 刷新起始下标
		short outside_end_index;				// 刷新结束下标
		short inside_begin_index;
		short inside_end_index;
		int server_chou_times;
	};

	ActivityDataParam m_data_param;
};

#endif
