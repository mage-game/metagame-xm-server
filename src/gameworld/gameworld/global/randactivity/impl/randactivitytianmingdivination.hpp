#ifndef __RAND_ACTIVITY_TIAN_MING_DIVINATION_HPP_
#define __RAND_ACTIVITY_TIAN_MING_DIVINATION_HPP_

#include "global/randactivity/randactivity.hpp"

class RandActivityTianMingDivination : public RandActivity
{
public:
	RandActivityTianMingDivination(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityTianMingDivination();	

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	int GetRefreshBeginIndex() { return m_data_param.refresh_begin_index; }
	int GetRefreshEndIndex() { return m_data_param.refresh_end_index; }

	const int GetServerChouTimes() { return m_data_param.server_chou_times; }
	void  AddServerChouTimes(const int add_times) { m_data_param.server_chou_times += add_times; }
	void ClearServerChouTimes() { m_data_param.server_chou_times = 0; }

private:
	struct ActivityDataParam
	{
		ActivityDataParam() : refresh_begin_index(0), refresh_end_index(0), server_chou_times(0)
		{}

		short refresh_begin_index;				// 刷新起始下标
		short refresh_end_index;				// 刷新结束下标
		int server_chou_times;
	};

	ActivityDataParam m_data_param;
};

#endif 