#ifndef __SYSTEMMONITOR_HPP__
#define __SYSTEMMONITOR_HPP__

#include "servercommon/monitordef.hpp"
#include <map>

class Role;

class SystemMonitor
{
public:
	static SystemMonitor & Instance();

	void Update(unsigned long interval);

	void AddMonitorNum(short monitor_type, int add_num);

	void OnRoleLogin(Role *role);
	void OnRoleLogout(Role *role);

private:
	SystemMonitor();
	~SystemMonitor();

	unsigned int m_monitor_time;
	int m_next_day_interval;
	int m_monitor_num_list[MONITOR_TYPE_MAX];

	static const int PLAT_ONLINE_MAX = 64;
	typedef std::map<int, int> PlatOnlineMap;
	typedef PlatOnlineMap::iterator PlatOnlineMapIt;
	PlatOnlineMap m_plat_online_map;
	unsigned int m_plat_online_print_time;

	void PrintPlatOnline(unsigned long interval);
};

#endif // __SYSTEMMONITOR_HPP__

