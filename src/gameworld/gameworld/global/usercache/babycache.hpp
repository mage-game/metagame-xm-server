#ifndef _BABY_CACHE_HPP_
#define _BABY_CACHE_HPP_

#include "servercommon/servercommon.h"
#include "servercommon/serverdef.h"
#include "servercommon/serverconfig/configstruct.h"
#include <map>
#include "servercommon/babydef.hpp"

class Role;

struct BabyCacheNode
{
	BabyCacheNode() : unique_id(0), baby_id(0), grade(0), level(0), bless(0), rename_times(0), reserve_sh(0), last_modity_time(0)
	{
		memset(baby_name, 0, sizeof(baby_name));
	}

	void GetName(GameName name) const
	{
		if (NULL != name)
		{
			F_STRNCPY(name, baby_name, sizeof(GameName));
		}
	}

	long long unique_id;
	GameName baby_name;

	short baby_id;
	short grade;
	short level;
	short bless;

	short rename_times;
	short reserve_sh;

	BabySpiritInfo baby_spirit_list[BABY_SPIRIT_COUNT];

	unsigned int last_modity_time;
};

class BabyCacheManager
{
public:
	static BabyCacheManager & Instance();

	bool SyncBabyCache(BabyInfo &info);
	long long GetNewUniqueId(int db_index);
	
private:
	BabyCacheManager();
	~BabyCacheManager();

	typedef std::map<long long, BabyCacheNode>::iterator CacheNodeIt;
	std::map<long long, BabyCacheNode> m_all_baby_map;

	int m_cur_sign;
};

#endif

