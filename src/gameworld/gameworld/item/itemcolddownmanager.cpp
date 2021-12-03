#include "itemcolddownmanager.hpp"

#include "engineadapter.h"

#include "obj/character/role.h"

#include "gameworld/protocal/msgitem.h"

ItemColddownManager::ItemColddownManager() : m_role(NULL), m_colddown_num(0)
{

}

ItemColddownManager::~ItemColddownManager()
{

}

void ItemColddownManager::Init(Role *role, const ItemColddownParam &param)
{
	m_role = role;

	m_colddown_num = param.count;
	if (m_colddown_num < 0) m_colddown_num = 0;
	else if (m_colddown_num > MAX_COLDDOWN_NUM) m_colddown_num = MAX_COLDDOWN_NUM;

	memcpy(m_colddown_info_list, param.colddown_info_list, sizeof(m_colddown_info_list));
}

void ItemColddownManager::GetItemColddownParam(ItemColddownParam *param)
{
	param->count = m_colddown_num;
	memcpy(param->colddown_info_list, m_colddown_info_list, sizeof(param->colddown_info_list));
}

bool ItemColddownManager::AddColddown(UInt16 colddown_id, int colddown_second)
{
	this->RefreshCD();

	if (m_colddown_num < 0 || m_colddown_num >= MAX_COLDDOWN_NUM) return false;

	UInt32 end_time = (UInt32)(EngineAdapter::Instance().Time() + colddown_second);

	m_colddown_info_list[m_colddown_num].colddown_id = colddown_id;
	m_colddown_info_list[m_colddown_num].end_time = end_time;

	++m_colddown_num;

	return true;
}

bool ItemColddownManager::HasColddown(UInt16 colddown_id)
{
	this->RefreshCD();

	if (m_colddown_num > 0 && m_colddown_num <= MAX_COLDDOWN_NUM)
	{
		for (int i = 0; i < m_colddown_num; ++i)
		{
			if (colddown_id == m_colddown_info_list[i].colddown_id) return true;
		}
	}

	return false;
}

void ItemColddownManager::RefreshCD()
{
	time_t now_second = EngineAdapter::Instance().Time();

	short temp_colddown_num = 0;
	for(int i = 0; i < m_colddown_num && i < MAX_COLDDOWN_NUM; i++)
	{
		if (now_second > m_colddown_info_list[i].end_time) 
		{
			continue;
		}

		if (temp_colddown_num != i)
		{
			m_colddown_info_list[temp_colddown_num].colddown_id = m_colddown_info_list[i].colddown_id;
			m_colddown_info_list[temp_colddown_num].end_time = m_colddown_info_list[i].end_time;
		}

		++temp_colddown_num;
	}

	m_colddown_num = temp_colddown_num;
}

void ItemColddownManager::SendItemColddown()
{
	static Protocol::SCItemColddownInfo item_cd_info;
	item_cd_info.count = m_colddown_num;

	if (m_colddown_num > 0 && m_colddown_num <= MAX_COLDDOWN_NUM)
	{
		memcpy(item_cd_info.cd_info_list, m_colddown_info_list, sizeof(m_colddown_info_list[0]) * m_colddown_num);
	}

	int length = sizeof(item_cd_info) - sizeof(m_colddown_info_list[0]) * (MAX_COLDDOWN_NUM - m_colddown_num);

	if (length > 0)
	{
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&item_cd_info, length);
	}
}

