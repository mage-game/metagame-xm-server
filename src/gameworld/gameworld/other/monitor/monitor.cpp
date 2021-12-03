#include "monitor.hpp"

#include "systemmonitor.hpp"
#include "obj/character/role.h"
#include "gameworld/gamelog.h"
#include "engineadapter.h"
#include "item/knapsack.h"

Monitor::Monitor()
	: m_role(NULL), m_next_monitor_time(0)
{
	memset(m_monitor_num_list, 0, sizeof(m_monitor_num_list));
}

Monitor::~Monitor()
{

}

void Monitor::Init(Role *role, const MonitorParam &monitor_param)
{
	m_role = role;

	m_next_monitor_time = monitor_param.next_monitor_time;
	memcpy(m_monitor_num_list, monitor_param.monitor_num_list, sizeof(m_monitor_num_list));
}

void Monitor::GetMonitorParam(MonitorParam *monitor_param)
{
	monitor_param->next_monitor_time = m_next_monitor_time;
	memcpy(monitor_param->monitor_num_list, m_monitor_num_list, sizeof(monitor_param->monitor_num_list));
}

void Monitor::Update(time_t now_second)
{
	if (now_second > m_next_monitor_time)
	{
		this->MonitorLog();
	}
}

void Monitor::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		memset(m_monitor_num_list, 0, sizeof(m_monitor_num_list));
	}
}

void Monitor::AddMonitorNum(short monitor_type, int add_num)
{
	if (monitor_type < 0 || monitor_type >= MONITOR_TYPE_MAX || add_num <= 0 || !this->CanMonitorLog()) return;

	m_monitor_num_list[monitor_type] += add_num;

	SystemMonitor::Instance().AddMonitorNum(monitor_type, add_num);
}

void Monitor::MonitorLog()
{
	if (this->CanMonitorLog())
	{
		Money *money = m_role->GetKnapsack()->GetMoney();

		gamelog::g_log_monitor.printf(LL_INFO, "[UserMonitor] [user_id:%d user_name:%s] [level:%d] [gold:%lld bind_gold:%lld coin_bind:%lld]"
			" gold_get:%d gold_consum:%d nobind_item_num:%d p2ptrade_num:%d publicsale_num:%d sendmail_num:%d"
			" fetch_attachment_num:%d guild_store_oper_num:%d shop_buy_num:%d chest_shop_buy_num:%d"
			" coin_get:%d coin_consum:%d mysteryshop_flush_num:%d compose_num:%d",
			UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetLevel(), money->GetGold(), money->GetGoldBind(), money->GetCoinBind(),
			m_monitor_num_list[MONITOR_TYPE_DAY_GOLD_GET], m_monitor_num_list[MONITOR_TYPE_DAY_GOLD_CONSUME], 
			m_monitor_num_list[MONITOR_TYPE_DAY_NOBIND_ITEM_NUM], m_monitor_num_list[MONITOR_TYPE_P2PTRADE_NUM], 
			m_monitor_num_list[MONITOR_TYPE_PUBLICSALE_NUM], m_monitor_num_list[MONITOR_TYPE_SENDMAIL_NUM], 
			m_monitor_num_list[MONITOR_TYPE_FETCH_ATTACHMENT_NUM], m_monitor_num_list[MONITOR_TYPE_GUILD_STORE_OPER_NUM], 
			m_monitor_num_list[MONITOR_TYPE_SHOP_BUY_NUM], m_monitor_num_list[MONITOR_TYPE_CHEST_SHOP_BUY_NUM],
			m_monitor_num_list[MONITOR_TYPE_DAY_NOBIND_COIN_GET], m_monitor_num_list[MONITOR_TYPE_DAY_NOBIND_COIN_CONSUME],
			m_monitor_num_list[MONITOR_TYPE_DAY_MYSTERYSHOP_FLUSH_NUM], m_monitor_num_list[MONITOR_TYPE_DAY_COMPOSE_NUM]);
	}

	m_next_monitor_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + MONITOR_LOG_INTERVAL;
}

bool Monitor::CanMonitorLog()
{
	return (Role::AUTHORITY_TYPE_GM != m_role->GetAuthority() && Role::AUTHORITY_TYPE_TEST != m_role->GetAuthority());
}
