#include "roleconvertshop.hpp"
#include "convertshop.hpp"
#include "obj/character/role.h"
#include "engineadapter.h"
#include "config/logicconfigmanager.hpp"

RoleConvertRecord::RoleConvertRecord() : m_role(NULL)
{

}

RoleConvertRecord::~RoleConvertRecord()
{

}

void RoleConvertRecord::SetRole(Role *role)
{
	m_role = role;
}

void RoleConvertRecord::Init(Role *role, const ConvertRecordParam &record_param)
{
	m_role = role;
	m_record_param = record_param;
}

void RoleConvertRecord::GetParam(ConvertRecordParam *record_param)
{
	if (NULL != record_param)
	{
		*record_param = m_record_param;
	}
}

void RoleConvertRecord::OnRoleLogin()
{
	//LOGIC_CONFIG->GetConvertShop().CheckLimitTimeConvertItem(m_role, m_role->GetLevel());
	this->SendInfo();
}

void RoleConvertRecord::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		m_record_param.OnResetData();
		this->SendInfo();
	}
}

int RoleConvertRecord::GetRecordCount(int type, short seq)
{
	for (int i =0; i < m_record_param.record_count && i < MAX_CONVERT_RECORD_ITEM_COUNT; ++ i)
	{
		const ConvertRecordItem &record_item = m_record_param.record_list[i];

		if (type == record_item.convert_type && seq == record_item.seq)
		{
			return record_item.convert_count;
		}
	}

	return 0;
}

int RoleConvertRecord::GetLifetimeRecordCount(int type, short seq)
{
	for (int i =0; i < m_record_param.lifetime_record_count && i < MAX_CONVERT_RECORD_ITEM_COUNT; ++ i)
	{
		const ConvertRecordItem &record_item = m_record_param.lifetime_record_list[i];

		if (type == record_item.convert_type && seq == record_item.seq)
		{
			return record_item.convert_count;
		}
	}

	return 0;
}

bool RoleConvertRecord::AddRecordCount(int convert_type, short seq, short convert_count, bool is_lifetime_limit_item)
{
	if (convert_type <= ConvertShop::SCORE_TO_ITEM_TYPE_INVALID || convert_type >= ConvertShop::SCORE_TO_ITEM_TYPE_MAX)
	{
		return false;
	}

	if (seq < 0 || seq >= Protocol::MAX_CONVERT_SCORE_TO_ITEM_NUM)
	{
		return false;
	}

	if (is_lifetime_limit_item)
	{
		int lifetime_record_index = -1;

		for (int i = 0; i < m_record_param.lifetime_record_count && i < MAX_CONVERT_RECORD_ITEM_COUNT; ++ i)
		{
			const ConvertRecordItem &record_item = m_record_param.lifetime_record_list[i];

			if (convert_type == record_item.convert_type && seq == record_item.seq)
			{
				lifetime_record_index = i;
				break;
			}
		}

		if (-1 == lifetime_record_index)
		{
			if (m_record_param.lifetime_record_count < MAX_CONVERT_RECORD_ITEM_COUNT)
			{
				lifetime_record_index = m_record_param.lifetime_record_count;
				++ m_record_param.lifetime_record_count;
			}
		}

		if (lifetime_record_index >= 0 && lifetime_record_index < MAX_CONVERT_RECORD_ITEM_COUNT && lifetime_record_index < m_record_param.lifetime_record_count)
		{
			m_record_param.lifetime_record_list[lifetime_record_index].convert_type = static_cast<char>(convert_type);
			m_record_param.lifetime_record_list[lifetime_record_index].seq = seq;
			m_record_param.lifetime_record_list[lifetime_record_index].convert_count += convert_count;
		}
	}

	int record_index = -1;

	for (int i = 0; i < m_record_param.record_count && i < MAX_CONVERT_RECORD_ITEM_COUNT; ++ i)
	{
		const ConvertRecordItem &record_item = m_record_param.record_list[i];

		if (convert_type == record_item.convert_type && seq == record_item.seq)
		{
			record_index = i;
			break;
		}
	}

	if (-1 == record_index)
	{
		if (m_record_param.record_count >= MAX_CONVERT_RECORD_ITEM_COUNT)
		{
			return false;
		}
		else
		{
			record_index = m_record_param.record_count;
			++ m_record_param.record_count;
		}
	}

	if (record_index < 0 || record_index >= MAX_CONVERT_RECORD_ITEM_COUNT || record_index >= m_record_param.record_count)
	{
		return false;
	}

	m_record_param.record_list[record_index].convert_type = static_cast<char>(convert_type);
	m_record_param.record_list[record_index].seq = seq;
	m_record_param.record_list[record_index].convert_count += convert_count;

	this->SendInfo();

	return true;
}

void RoleConvertRecord::SendInfo()
{
	static Protocol::SCConvertRecordInfo cri;
	cri.record_count = 0;
	cri.lifetime_record_count = 0;
	cri.limittime_record_count = 0;

	for (int i = 0; i < m_record_param.record_count && i < MAX_CONVERT_RECORD_ITEM_COUNT; ++ i)
	{
		//cri.record_list[i] = m_record_param.record_list[i];

		cri.record_list[i].Reset();
		cri.record_list[i].convert_type = m_record_param.record_list[i].convert_type;
		cri.record_list[i].convert_count = m_record_param.record_list[i].convert_count;
		cri.record_list[i].seq = m_record_param.record_list[i].seq;

		++ cri.record_count;
	}

	for (int j = 0; j < m_record_param.lifetime_record_count && j < MAX_CONVERT_RECORD_ITEM_COUNT; ++ j)
	{
		//cri.record_list[cri.record_count + j] = m_record_param.lifetime_record_list[j];
		cri.record_list[cri.record_count + j].Reset();
		cri.record_list[cri.record_count + j].convert_type = m_record_param.lifetime_record_list[j].convert_type;
		cri.record_list[cri.record_count + j].convert_count = m_record_param.lifetime_record_list[j].convert_count;
		cri.record_list[cri.record_count + j].seq = m_record_param.lifetime_record_list[j].seq;
		++ cri.lifetime_record_count;
	}

	int index_offset = cri.record_count + cri.lifetime_record_count;
	if (index_offset < 0 || index_offset >= MAX_CONVERT_RECORD_ITEM_COUNT + MAX_CONVERT_RECORD_ITEM_COUNT)
	{
		return;
	}
	for (int k = 0; k < m_record_param.limittime_record_count && k < MAX_TIME_CONVERT_RECORD_ITEM_COUNT; ++k)
	{
		//cri.limit_time_record_list[k] = m_record_param.limit_time_record_list[k];
		cri.record_list[index_offset + k].Reset();
		cri.record_list[index_offset + k].convert_type = (char)m_record_param.limit_time_record_list[k].convert_type;
		cri.record_list[index_offset + k].seq = m_record_param.limit_time_record_list[k].seq;
		cri.record_list[index_offset + k].timestamp = m_record_param.limit_time_record_list[k].timestamp;
		++cri.limittime_record_count;
	}

	int count = MAX_CONVERT_RECORD_ITEM_COUNT + MAX_CONVERT_RECORD_ITEM_COUNT + MAX_TIME_CONVERT_RECORD_ITEM_COUNT - (cri.record_count + cri.lifetime_record_count + cri.limittime_record_count);
	int send_len = sizeof(cri) - sizeof(cri.record_list[0]) * count;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cri, send_len);
}

void RoleConvertRecord::OnRoleLevelChange(int level)
{
	LOGIC_CONFIG->GetConvertShop().CheckLimitTimeConvertItem(m_role, level);
}

unsigned int RoleConvertRecord::GetRecordLimitBeginTimeStamp(int convert_type, int seq)
{
	for (int i = 0; i < m_record_param.limittime_record_count && i < MAX_TIME_CONVERT_RECORD_ITEM_COUNT; ++i)
	{
		const ConvertRecordItemLimitTime &record_item = m_record_param.limit_time_record_list[i];

		if ((short)convert_type == record_item.convert_type && (short)seq == record_item.seq && record_item.timestamp > 0)
		{
			return record_item.timestamp;
		}
	}

	return 0;
}

bool RoleConvertRecord::AddRecordLimitBeginTimeStamp(int convert_type, int seq)
{
	if (convert_type <= ConvertShop::SCORE_TO_ITEM_TYPE_INVALID || convert_type >= ConvertShop::SCORE_TO_ITEM_TYPE_MAX || 
		seq < 0 || seq >= Protocol::MAX_CONVERT_SCORE_TO_ITEM_NUM)
	{
		return false;
	}

	// 只存这么多
	if (m_record_param.limittime_record_count >= MAX_TIME_CONVERT_RECORD_ITEM_COUNT)
	{
		return false;
	}

	if (m_record_param.limittime_record_count < 0)
	{
		m_record_param.limittime_record_count = 0;
	}

	for (int i = 0; i < MAX_TIME_CONVERT_RECORD_ITEM_COUNT; ++i)
	{
		ConvertRecordItemLimitTime &record_item = m_record_param.limit_time_record_list[i];
		
		// 已经存过的就不再存了
		if ((short)convert_type == record_item.convert_type && (short)seq == record_item.seq) return false;

		if (record_item.timestamp > 0) continue;

		record_item.convert_type = (short)convert_type;
		record_item.seq = (short)seq;
		record_item.timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		m_record_param.limittime_record_count++;

		return true;
	}

	return false;
}
