#ifndef __RAND_ACTIVITY_MAP_HUNT_H__
#define __RAND_ACTIVITY_MAP_HUNT_H__
#include "global/randactivity/randactivitymanager.hpp"

class RandActivityMapHunt : public RandActivity
{
public:
	RandActivityMapHunt(RandActivityManager *act_mgr);
	virtual ~RandActivityMapHunt();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);
	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	void AddFlushTimes(const int times) { m_server_total_flush_times += times; }
	const int GetServerTotalFlushTimes() { return m_server_total_flush_times; }

private:
	
	int m_server_total_flush_times;
};
#endif