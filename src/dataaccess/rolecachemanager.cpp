#include "rolecachemanager.hpp"

#include "engineadapter.h"

#include "rmiobj/rmirole.h"

RoleCacheManager::RoleCacheManager()
	: m_is_remove_role_cache(false), m_remove_cache_time(24 * 60 * 60 * 7), m_last_check_time(0)
{

}

RoleCacheManager::~RoleCacheManager()
{

}

RoleCacheManager & RoleCacheManager::Instance()
{
	static RoleCacheManager cache_manager;
	return cache_manager;
}

void RoleCacheManager::OnRoleLogin(int role_id)
{
	if (!m_is_remove_role_cache) return;

	RoleCacheInfoMapIt it = m_role_cache_info_map.find(role_id);
	if (it != m_role_cache_info_map.end())
	{
		m_role_cache_info_map.erase(it);
	}
}

void RoleCacheManager::OnRoleLogout(int role_id, int allow_to_cross_flag)
{
	if (!m_is_remove_role_cache) return;

	if (0 == allow_to_cross_flag)
	{
		RoleCacheInfoMapIt it = m_role_cache_info_map.find(role_id);
		if (it != m_role_cache_info_map.end())
		{
			it->second.logout_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		}
		else 
		{
			RoleCacheInfo cache_info;
			cache_info.logout_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

			m_role_cache_info_map[role_id] = cache_info;
		}
	}
}

void RoleCacheManager::OnUpdate(unsigned int now_second)
{
	if (!m_is_remove_role_cache) return;

	if (now_second >= m_last_check_time + 60)
	{
		for (RoleCacheInfoMapIt it = m_role_cache_info_map.begin(); it != m_role_cache_info_map.end();)
		{
			if (0 != it->second.logout_time && now_second >= it->second.logout_time + m_remove_cache_time)  
			{
				this->RemoveRoleCacheFromFastDB(it->first);
				m_role_cache_info_map.erase(it++);
			}
			else 
			{
				++ it;
			}
		}

		m_last_check_time = now_second;
	}
}

void RoleCacheManager::RemoveRoleCacheFromFastDB(int role_id)
{
	RMIRoleObject::RemoveRoleFromCache(role_id);
}

