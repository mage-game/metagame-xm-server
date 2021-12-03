#ifndef __RANDACTIVITY_HUANLEYAOJIANG2_HPP__
#define __RANDACTIVITY_HUANLEYAOJIANG2_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityHuanLeYaoJiang2 : public RandActivity
{
	struct ActivityData
	{
		ActivityData() : server_chou_times(0) {}

		int server_chou_times;			// ȫ���齱����
	};

public:
	RandActivityHuanLeYaoJiang2(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityHuanLeYaoJiang2();

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);
	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	const int GetServerChouTimes() { return m_activity_data.server_chou_times; }
	void AddServerChouTimes(const int add_times) { m_activity_data.server_chou_times += add_times; }
	void ClearServerChouTimes() { m_activity_data.server_chou_times = 0; }

protected:
	ActivityData m_activity_data;
};

#endif