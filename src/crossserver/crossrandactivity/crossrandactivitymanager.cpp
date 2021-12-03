#include "crossrandactivitymanager.hpp"
#include "crossrandactivity.hpp"
#include "impl/crossrandactivitychongzhirank.hpp"
#include "impl/crossrandactivityconsumerank.hpp"
#include "impl/crossrandactivitycloudpurchase.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "servercommon/internalprotocal/crossgamerandactivitymsg.h"

CrossRandActivityManager::CrossRandActivityManager() : m_cross_server(NULL)
{
	UNSTD_STATIC_CHECK(3 == CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT);

	memset(m_activity_list, 0, sizeof(m_activity_list));

	m_activity_list[CROSS_RAND_ACTIVITY_TYPE_CHONGZHI_RANK - CROSS_RAND_ACTIVITY_TYPE_BEGIN] = new CrossRandActivityChongzhiRank();
	m_activity_list[CROSS_RAND_ACTIVITY_TYPE_CLOUD_PURCHASE - CROSS_RAND_ACTIVITY_TYPE_BEGIN] = new CrossRandActivityCloudPurchase();
	m_activity_list[CROSS_RAND_ACTIVITY_TYPE_CONSUME_RANK - CROSS_RAND_ACTIVITY_TYPE_BEGIN] = new CrossRandActivityConsumeRank();
}

CrossRandActivityManager::~CrossRandActivityManager()
{
	for (int i = 0; i < CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT; i++)
	{
		if (NULL != m_activity_list[i])
		{
			delete m_activity_list[i];
		}
	}

	memset(m_activity_list, 0, sizeof(m_activity_list));
}

void CrossRandActivityManager::InitActivity()
{
	for (int i = 0; i < CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT; i++)
	{
		if (NULL != m_activity_list[i])
		{
			m_activity_list[i]->Init();
		}
	}
}

void CrossRandActivityManager::Update(unsigned long interval, time_t now_second)
{
	for (int i = 0; i < CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT; i++)
	{
		if (NULL != m_activity_list[i])
		{
			m_activity_list[i]->Update((unsigned int)now_second);
		}
	}
}

void CrossRandActivityManager::OnDayChange()
{
	for (int i = 0; i < CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT; i++)
	{
		if (NULL != m_activity_list[i])
		{
			m_activity_list[i]->OnDayChange();
		}
	}
}


void CrossRandActivityManager::OnWeekChange()
{
	for (int i = 0; i < CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT; i++)
	{
		if (NULL != m_activity_list[i])
		{
			m_activity_list[i]->OnWeekChange();
		}
	}
}

bool CrossRandActivityManager::IsLoadFinish()
{
	for (int i = 0; i < CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT; ++ i)
	{
		if (NULL != m_activity_list[i])
		{
			if (!m_activity_list[i]->IsInitFinish())
			{
				return false;
			}
		}
	}

	return true;
}

void CrossRandActivityManager::OnRegisterGameServer(NetID global_netid)
{
	for (int i = 0; i < CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT; ++ i)
	{
		if (NULL != m_activity_list[i])
		{
			m_activity_list[i]->OnRegisterGameServer(global_netid);
		}
	}
}

CrossRandActivity * CrossRandActivityManager::GetActivity(int activity_type)
{
	int cross_rand_activity_type_index = activity_type - CROSS_RAND_ACTIVITY_TYPE_BEGIN;
	if (cross_rand_activity_type_index < 0 || cross_rand_activity_type_index >= CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT) return NULL;

	return m_activity_list[cross_rand_activity_type_index];
}

void CrossRandActivityManager::OnForceToNextState(int cross_activity_type)
{
	CrossRandActivity *cross_activity = this->GetActivity(cross_activity_type);
	if (NULL != cross_activity)
	{
		cross_activity->ForceToNextState();
	}
}

void CrossRandActivityManager::OnSyncActivityData(NetID global_netid, crossgameprotocal::GameCrossSyncCrossRandActivityData * srad, int length)
{
	CrossRandActivity * act = this->GetActivity(srad->activity_type);
	if (NULL == act)
	{
		return;
	}

	int header_len = sizeof(crossgameprotocal::GameCrossSyncCrossRandActivityData);
	int msg_len = length - header_len;

	if (msg_len <= 0)
	{
		return;
	}

	crossgameramsg::MessageHeader *header = (crossgameramsg::MessageHeader *)((char*)srad + header_len);

	if (header->msg_type > crossgameramsg::MSG_COMMON_BEGIN && header->msg_type <= crossgameramsg::MSG_COMMON_END)
	{
		act->OnCommonMessageFromGameworld(global_netid, header->msg_type, (char*)header);
	}
	else if (header->msg_type > crossgameramsg::MSG_ACT_BEGIN && header->msg_type <= crossgameramsg::MSG_ACT_END)
	{
		act->OnMessageFromGameworld(global_netid, header->msg_type, (char*)header);
	}
}

