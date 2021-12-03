#include "systemmonitor.hpp"

#include "gameworld/gamelog.h"
#include "obj/character/role.h"

#include "world.h"
#include "engineadapter.h"

SystemMonitor::SystemMonitor()
	: m_monitor_time(0), m_next_day_interval(0), m_plat_online_print_time(0)
{
	memset(m_monitor_num_list, 0, sizeof(m_monitor_num_list));
}

SystemMonitor::~SystemMonitor()
{

}

SystemMonitor & SystemMonitor::Instance()
{
	static SystemMonitor obj;
	return obj;
}

void SystemMonitor::Update(unsigned long interval)
{
	{
		m_monitor_time += interval;

		if (m_monitor_time > MONITOR_LOG_INTERVAL * 1000)
		{
			gamelog::g_log_monitor.printf(LL_INFO, "[SystemMonitor] gold_get:%d gold_consum:%d nobind_item_num:%d p2ptrade_num:%d"
				" publicsale_num:%d sendmail_num:%d fetch_attachment_num:%d guild_store_oper_num:%d shop_buy_num:%d chest_shop_buy_num:%d"
				" coin_get:%d coin_consum:%d mysteryshop_flush_num:%d compose_num:%d",
				m_monitor_num_list[MONITOR_TYPE_DAY_GOLD_GET], m_monitor_num_list[MONITOR_TYPE_DAY_GOLD_CONSUME], 
				m_monitor_num_list[MONITOR_TYPE_DAY_NOBIND_ITEM_NUM], m_monitor_num_list[MONITOR_TYPE_P2PTRADE_NUM], 
				m_monitor_num_list[MONITOR_TYPE_PUBLICSALE_NUM], m_monitor_num_list[MONITOR_TYPE_SENDMAIL_NUM], 
				m_monitor_num_list[MONITOR_TYPE_FETCH_ATTACHMENT_NUM], m_monitor_num_list[MONITOR_TYPE_GUILD_STORE_OPER_NUM], 
				m_monitor_num_list[MONITOR_TYPE_SHOP_BUY_NUM], m_monitor_num_list[MONITOR_TYPE_CHEST_SHOP_BUY_NUM],
				m_monitor_num_list[MONITOR_TYPE_DAY_NOBIND_COIN_GET], m_monitor_num_list[MONITOR_TYPE_DAY_NOBIND_COIN_CONSUME],
				m_monitor_num_list[MONITOR_TYPE_DAY_MYSTERYSHOP_FLUSH_NUM], m_monitor_num_list[MONITOR_TYPE_DAY_COMPOSE_NUM]);

			m_monitor_time = 0;
		}
	}

	{
		m_next_day_interval -= interval;

		if (m_next_day_interval <= 0)
		{
			memset(m_monitor_num_list, 0, sizeof(m_monitor_num_list));

			m_next_day_interval = EngineAdapter::Instance().NextDayInterval(0, 0, 0) * 1000;
		}
	}

	{
		this->PrintPlatOnline(interval);
	}
}

void SystemMonitor::AddMonitorNum(short monitor_type, int add_num)
{
	if (monitor_type < 0 || monitor_type >= MONITOR_TYPE_MAX || add_num <= 0) return;

	m_monitor_num_list[monitor_type] += add_num;
}

void SystemMonitor::OnRoleLogin(Role *role)
{
	int index = role->GetPlatSpid();
	if (0 == index) return;

	PlatOnlineMapIt it = m_plat_online_map.find(index);
	if (m_plat_online_map.end() != it) 
	{
		it->second += 1;
	} 
	else if ((int)m_plat_online_map.size() < PLAT_ONLINE_MAX) 
	{
		m_plat_online_map[index] = 1;
	}
}

void SystemMonitor::OnRoleLogout(Role *role)
{
	int index = role->GetPlatSpid();
	if (0 == index) return;

	PlatOnlineMapIt it = m_plat_online_map.find(index);
	if (m_plat_online_map.end() != it) 
	{
		it->second -= 1;
	} 
}

void SystemMonitor::PrintPlatOnline(unsigned long interval)
{
	m_plat_online_print_time += interval;

	if (m_plat_online_print_time > 60 * 1000)  // 1分钟打印一次
	{
		m_plat_online_print_time = 0;

		int online_role_num = World::GetInstWorld()->GetSceneManager()->GetOnlineRoleNum();
		int max_role_num = World::GetInstWorld()->GetSceneManager()->GetMaxRoleNum();

		gamelog::g_log_role_num.buff_printf("[%d/%d] ", online_role_num, max_role_num); // 子后台用
		gamelog::g_log_role_num.commit_buff(LL_INFO);

		LOG_QUICK8(gamelog::g_log_quick, LOG_TYPE_ONLINE_ROLE_NUM, online_role_num, NULL, NULL, 0, 0, NULL, NULL);  // 总在线

		for (PlatOnlineMapIt it = m_plat_online_map.begin(); m_plat_online_map.end() != it; ++it)  // 平台在线
		{
			int index = it->first;
			char *plat = (char*)&index;
			plat[sizeof(index) - 1] = '\0';
			LOG_QUICK8(gamelog::g_log_quick, LOG_TYPE_PLAT_ONLINE, it->second, plat, NULL, 0, 0, NULL, NULL);
		}
	}
}
