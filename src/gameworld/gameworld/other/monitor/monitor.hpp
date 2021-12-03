#ifndef __MONITOR_HPP__
#define __MONITOR_HPP__

#include "servercommon/monitordef.hpp"

class Role;

class Monitor
{
public:
	Monitor();
	~Monitor();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const MonitorParam &monitor_param);
	void GetMonitorParam(MonitorParam *monitor_param);

	void Update(time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void AddMonitorNum(short monitor_type, int add_num);

private:
	void MonitorLog();
	bool CanMonitorLog();

	Role *m_role;

	unsigned int m_next_monitor_time;
	int m_monitor_num_list[MONITOR_TYPE_MAX];
};

#endif // __MONITOR_HPP__

