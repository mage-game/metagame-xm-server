/******************************************************************************************
FileName: rolecachemanager.hpp
Author: dreamwang
Description: 角色缓存管理 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2012/03/10/ 17:04:05     1.0           创建
*****************************************************************************************/

#ifndef __ROLE_CACHE_MANAGER_HPP__
#define __ROLE_CACHE_MANAGER_HPP__

#include <map>

class RoleCacheManager
{
public:
	static RoleCacheManager & Instance();

	void SetRemoveRoleCache(bool is_remove_role_cache, int remove_cache_time) 
	{ 
		m_is_remove_role_cache = is_remove_role_cache; 
		m_remove_cache_time = remove_cache_time;
	}

	void OnRoleLogin(int role_id);
	void OnRoleLogout(int role_id, int allow_to_cross_flag);

	void OnUpdate(unsigned int now_second);

private:
	RoleCacheManager();
	~RoleCacheManager();

	void RemoveRoleCacheFromFastDB(int role_id);
	
	bool m_is_remove_role_cache;
	int m_remove_cache_time;

	struct RoleCacheInfo
	{
		RoleCacheInfo() : logout_time(0) {}

		unsigned int logout_time;				// 角色登出的时间
	};

	typedef std::map<int, RoleCacheInfo> RoleCacheInfoMap;
	typedef std::map<int, RoleCacheInfo>::iterator RoleCacheInfoMapIt;

	RoleCacheInfoMap m_role_cache_info_map;
	unsigned int m_last_check_time;
};

#endif // __ROLE_CACHE_MANAGER_HPP__
