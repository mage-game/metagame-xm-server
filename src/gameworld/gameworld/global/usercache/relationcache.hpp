#ifndef _RELATIONCACHE_HPP_
#define _RELATIONCACHE_HPP_

#include "servercommon/serverdef.h"
#include "servercommon/serverconfig/configstruct.h"
#include "servercommon/mysqlutility.hpp"

#include <map>
#include <set>


class RelationCacheManager
{
public:
	static RelationCacheManager & Instance();

	bool LoadAllRelation(TMysqlStatement &stMysqlStatement);

	void RemoveFriend(int owner_uid, int remove_uid);
	void AddFriend(int owner_uid, int add_uid);
	bool HasFriend(int owner_uid, int friend_uid);

private:
	bool LoadFriend(TMysqlStatement &stMysqlStatement);

	unsigned long long MakeFriendPair(int uid1, int uid2);

	RelationCacheManager();
	~RelationCacheManager();

	std::set<unsigned long long> m_friend_set;
};

#endif

