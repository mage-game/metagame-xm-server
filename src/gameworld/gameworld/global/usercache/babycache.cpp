#include "world.h"
#include "babycache.hpp"
#include "gamelog.h"

BabyCacheManager::BabyCacheManager() : m_cur_sign(0)
{

}

BabyCacheManager::~BabyCacheManager()
{

}

BabyCacheManager & BabyCacheManager::Instance()
{
	static BabyCacheManager instance;
	return instance;
}

bool BabyCacheManager::SyncBabyCache(BabyInfo &info)
{
	bool is_cache_been_sync = false; // 缓存是否被修改过，作为返回值

	BabyCacheNode &cache = m_all_baby_map[info.unique_id];

	if (cache.last_modity_time > info.last_modity_time)
	{
		F_STRNCPY(info.baby_name, cache.baby_name, sizeof(GameName));
		info.last_modity_time = cache.last_modity_time;
	}
	else
	{
		F_STRNCPY(cache.baby_name, info.baby_name, sizeof(GameName));
		cache.last_modity_time = info.last_modity_time;
		is_cache_been_sync = true;
	}

	if (cache.grade > info.grade || (cache.grade == info.grade && cache.bless > info.bless))
	{
		info.grade = cache.grade;
		info.bless = cache.bless;
	}
	else
	{
		cache.grade = info.grade;
		cache.bless = info.bless;
		is_cache_been_sync = true;
	}

	if (cache.level > info.level)
	{
		info.level = cache.level;
	}
	else
	{
		cache.level = info.level;
		is_cache_been_sync = true;
	}

	if (cache.rename_times > info.rename_times)
	{
		info.rename_times = cache.rename_times;
	}
	else
	{
		cache.rename_times = info.rename_times;
		is_cache_been_sync = true;
	}

	for (int sp_index = 0; sp_index < BABY_SPIRIT_COUNT; ++ sp_index)
	{
		BabySpiritInfo &cache_sp = cache.baby_spirit_list[sp_index];
		BabySpiritInfo &info_sp = info.baby_spirit_list[sp_index];

		if (cache_sp.spirit_level > info_sp.spirit_level || (cache_sp.spirit_level == info_sp.spirit_level && cache_sp.spirit_train > info_sp.spirit_train))
		{
			info_sp = cache_sp;
		}
		else
		{
			cache_sp = info_sp;
			is_cache_been_sync = true;
		}
	}

	return is_cache_been_sync;
}

long long BabyCacheManager::GetNewUniqueId(int db_index)
{
	long long nowtime = EngineAdapter::Instance().Time();

	long long unique_id = (nowtime << 32) + static_cast<int>(db_index << DB_INDEX_MARK_BIT) + m_cur_sign;

	++ m_cur_sign;
	if (m_cur_sign >= (1 << DB_INDEX_MARK_BIT))
	{
		m_cur_sign = 0;
	}

	return unique_id;
}

