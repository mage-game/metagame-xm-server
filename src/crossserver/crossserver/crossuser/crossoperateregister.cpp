#include "crossoperateregister.hpp"

#include "engineadapter.h"

CrossOperateRegister::CrossOperateRegister()
	: m_cross_server(NULL), m_last_check_expire_time(0)
{
	
}

CrossOperateRegister::~CrossOperateRegister()
{
}

void CrossOperateRegister::Update(unsigned long interval, time_t now_second)
{
	if (now_second >= m_last_check_expire_time + CROSS_OPERATE_CHECK_TIME_INTERVAL_S)
	{
		for (UserNextTimeInfoMapIt it = m_user_time_info_map.begin(); it != m_user_time_info_map.end(); )
		{
			bool is_expire = true;
			for (int i = 0; i < CROSS_OPERATE_MAX; ++ i)
			{
				if (now_second < it->second.next_time_list[i])
				{
					is_expire = false;
					break;
				}
			}

			if (is_expire)
			{
				m_user_time_info_map.erase(it++);
			}
			else
			{
				++ it;
			}
		}

		m_last_check_expire_time = now_second + CROSS_OPERATE_CHECK_TIME_INTERVAL_S;
	}
}

bool CrossOperateRegister::CheckCanCrossOperate(const UniqueUserID &unique_user_id, int cross_operate_type, int operate_interval_s)
{
	if (cross_operate_type < 0 || cross_operate_type >= CROSS_OPERATE_MAX) return false;

	time_t now = EngineAdapter::Instance().Time();

	UserNextTimeInfoMapIt it = m_user_time_info_map.find(unique_user_id);
	if (it == m_user_time_info_map.end())
	{
		UserNextTimeInfo user_next_time_info;
		m_user_time_info_map[unique_user_id] = user_next_time_info;

		it = m_user_time_info_map.find(unique_user_id);
	}
	if (it != m_user_time_info_map.end())
	{
		if (now >= it->second.next_time_list[cross_operate_type])
		{
			it->second.next_time_list[cross_operate_type] = now + operate_interval_s;

			return true;
		}
	}
	
	return false;
}

bool CrossOperateRegister::CheckCanGetCrossChallengeSessionInfo(const UniqueUserID &unique_user_id)
{
	return this->CheckCanCrossOperate(unique_user_id, CROSS_OPERATE_GET_CROSS_CHALLENGE_SESSION_INFO, 2);
}

bool CrossOperateRegister::CheckCanGetCrossChallengeSessionRecord(const UniqueUserID &unique_user_id)
{
	return this->CheckCanCrossOperate(unique_user_id, CROSS_OPERATE_GET_CROSS_CHALLENGE_SESSION_RECORD, 3);
}

