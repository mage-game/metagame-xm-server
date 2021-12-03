#include "daycounter.hpp"

#include "engineadapter.h"
#include "obj/character/role.h"
#include "protocal/msgfunction.h"
#include "protocal/msgdaycounter.hpp"

#include "servercommon/errornum.h"

#include "config/logicconfigmanager.hpp"
#include "other/event/eventhandler.hpp"

#define RECORD_COUNT_BEG 'A'
#define RECORD_COUNT_EBD 'z'

DayCounter::DayCounter() 
	: m_role(NULL)
{
	memset(m_daycount_data, 0, sizeof(m_daycount_data));
}

DayCounter::~DayCounter()
{

}

void DayCounter::Init(Role *role, const DayCountData daycount_data)
{
	m_role = role;
	F_STRNCPY(m_daycount_data, daycount_data, sizeof(DayCountData));
}

void DayCounter::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		memset(m_daycount_data, 0, sizeof(m_daycount_data));

		this->SendDayCounterInfo();
	}
}

bool DayCounter::DayCountCheck(int day_count_id, int max_count)
{
	if (day_count_id < 0 || day_count_id >= MAX_DAYCOUNT_COUNT_ID) return false;
	if (0 == m_daycount_data[day_count_id]) return true;

	return (m_daycount_data[day_count_id] - RECORD_COUNT_BEG) < max_count;
}

void DayCounter::DayCountIncrease(int day_count_id)
{
	if (day_count_id < 0 || day_count_id >= MAX_DAYCOUNT_COUNT_ID) return;

	if (0 == m_daycount_data[day_count_id])
	{
		m_daycount_data[day_count_id + 1] = '\0';
		for (int i = day_count_id; i >= 0; --i)
		{
			if (0 != m_daycount_data[i]) break;
			m_daycount_data[i] = RECORD_COUNT_BEG;
		}
	}

	if (m_daycount_data[day_count_id] < RECORD_COUNT_EBD) ++ m_daycount_data[day_count_id];
	
	this->SendDayCounterItemInfo(day_count_id);
}

void DayCounter::DayCountReduce(int day_count_id)
{
	if (day_count_id < 0 || day_count_id >= MAX_DAYCOUNT_COUNT_ID) return;
	if (0 == m_daycount_data[day_count_id]) return;

	if (m_daycount_data[day_count_id] > RECORD_COUNT_BEG) -- m_daycount_data[day_count_id];
	
	this->SendDayCounterItemInfo(day_count_id);
}

void DayCounter::DayCountReset(int day_count_id)
{
	if (day_count_id < 0 || day_count_id >= MAX_DAYCOUNT_COUNT_ID || m_daycount_data[day_count_id] <= 0) return;

	m_daycount_data[day_count_id] = RECORD_COUNT_BEG;

	this->SendDayCounterItemInfo(day_count_id);
}

void DayCounter::DayCountSet(int day_count_id, int count)
{
	if (day_count_id < 0 || day_count_id >= MAX_DAYCOUNT_COUNT_ID) return;
	if (count < 0 || count > RECORD_COUNT_EBD - RECORD_COUNT_BEG) return;

	if (0 == m_daycount_data[day_count_id])
	{
		m_daycount_data[day_count_id + 1] = '\0';
		for (int i = day_count_id; i >= 0; --i)
		{
			if (0 != m_daycount_data[i]) break;
			m_daycount_data[i] = RECORD_COUNT_BEG;
		}
	}

	m_daycount_data[day_count_id] = RECORD_COUNT_BEG + count;
	
	this->SendDayCounterItemInfo(day_count_id);
}

int DayCounter::GetDayCount(int day_count_id)
{
	if (day_count_id < 0 || day_count_id >= MAX_DAYCOUNT_COUNT_ID || m_daycount_data[day_count_id] <= 0) return 0;

	return m_daycount_data[day_count_id] - RECORD_COUNT_BEG;
}

void DayCounter::GetCountData(DayCountData day_count_data)
{
	F_STRNCPY(day_count_data, m_daycount_data, sizeof(DayCountData));
}

void DayCounter::SendDayCounterInfo()
{
	UNSTD_STATIC_CHECK(MAX_DAYCOUNT_COUNT_ID < (int)sizeof(m_daycount_data) / (int)sizeof(m_daycount_data[0]));

	static Protocol::SCDayCounterInfo dci;
	for (int i = 0; i < MAX_DAYCOUNT_COUNT_ID; ++i)
	{
		dci.day_counter_list[i] = (m_daycount_data[i] >= RECORD_COUNT_BEG ? m_daycount_data[i] - RECORD_COUNT_BEG : 0);
	}
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&dci, sizeof(dci));
}

void DayCounter::SendDayCounterItemInfo(int day_count_id)
{
	if (day_count_id < 0 || day_count_id >= MAX_DAYCOUNT_COUNT_ID) return;

	Protocol::SCDayCounterItemInfo dcii;
	dcii.day_counter_id = day_count_id;
	dcii.day_counter_value = (m_daycount_data[day_count_id] >= RECORD_COUNT_BEG ? m_daycount_data[day_count_id] - RECORD_COUNT_BEG : 0);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&dcii, sizeof(dcii));
}

void DayCounter::GMResetDayCount()
{
	memset(m_daycount_data, 0, sizeof(m_daycount_data));

	this->SendDayCounterInfo();	
}


