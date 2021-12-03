#ifndef __RANDACTIVITY_ZHENBAOGE2_HPP__
#define __RANDACTIVITY_ZHENBAOGE2_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityZhenBaoGe2 : public RandActivity
{
public:
	RandActivityZhenBaoGe2(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityZhenBaoGe2();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	void AddServerReflushTimes();
	int GetServerReflushTimes() { return m_server_reflush_times; }

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

private:
	int m_server_reflush_times;

};

#endif


