#ifndef __RANDACTIVITY_ZHENBAOGE_HPP__
#define __RANDACTIVITY_ZHENBAOGE_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityZhenBaoGe : public RandActivity
{
public:
	RandActivityZhenBaoGe(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityZhenBaoGe();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	void AddServerReflushTimes();
	int GetServerReflushTimes() { return m_server_reflush_times; }

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

private:
	int m_server_reflush_times;

};

#endif


