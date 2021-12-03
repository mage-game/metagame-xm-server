#include "relationcache.hpp"
#include "gamelog.h"

RelationCacheManager::RelationCacheManager()
{

}

RelationCacheManager::~RelationCacheManager()
{

}

RelationCacheManager & RelationCacheManager::Instance()
{
	static RelationCacheManager ws;
	return ws;
}

bool RelationCacheManager::LoadAllRelation(TMysqlStatement &stMysqlStatement)
{
	bool ret = false;
	
	ret = this->LoadFriend(stMysqlStatement);
	if (!ret)
	{
		return false;
	}

	return true;
}

void RelationCacheManager::RemoveFriend(int owner_uid, int remove_uid)
{
	unsigned long long pair_val = this->MakeFriendPair(owner_uid, remove_uid);
	m_friend_set.erase(pair_val);
}

void RelationCacheManager::AddFriend(int owner_uid, int add_uid)
{
	unsigned long long pair_val = this->MakeFriendPair(owner_uid, add_uid);
	m_friend_set.insert(pair_val);
}

bool RelationCacheManager::HasFriend(int owner_uid, int friend_uid)
{
	unsigned long long pair_val = this->MakeFriendPair(owner_uid, friend_uid);

	std::set<unsigned long long>::iterator friend_iter = m_friend_set.find(pair_val);
	if (friend_iter != m_friend_set.end())
	{
		return true;
	}

	return false;
}

bool RelationCacheManager::LoadFriend(TMysqlStatement &stMysqlStatement)
{
	char szErrMsg[512];
	int iRet = 0;

	int iErrno;
	stMysqlStatement.iQueryType = SQL_TYPE_SELECT;
	strncpy(stMysqlStatement.szSQLString, 
		"SELECT `role_id`, `friend_uid` FROM friends WHERE `role_id` > 0",
		sizeof(stMysqlStatement.szSQLString) - 1);

	iRet = CMysqlUtility::ExecSelect(&stMysqlStatement, &iErrno, szErrMsg);
	if(0 != iRet)
	{
		CMysqlUtility::FreeResult(&stMysqlStatement);

		gamelog::g_log_world.printf(LL_MAINTANCE, "RelationCacheManager::LoadFriend select failed with :%d errmsg:%s",
			iErrno, szErrMsg);

		return false;
	}

	int iFetchRows = stMysqlStatement.iResNum;

	enum field
	{
		field_uid = 0,
		field_friend_uid,
	};

	for (int i = 0; i < iFetchRows; ++i)
	{
		iRet = CMysqlUtility::FetchRow(&stMysqlStatement, szErrMsg);
		if(0 != iRet)
		{
			gamelog::g_log_world.printf(LL_MAINTANCE, "RelationCacheManager::LoadFriend when fetched data has error :%s,index:%d",
				szErrMsg, i);

			break;
		}

		int owner_uid = 0;
		if(stMysqlStatement.stRow[field_uid])
		{
			owner_uid = atoi(stMysqlStatement.stRow[field_uid]);
		}

		int friend_uid = 0;
		if(stMysqlStatement.stRow[field_friend_uid])
		{
			friend_uid = atoi(stMysqlStatement.stRow[field_friend_uid]);
		}

		this->AddFriend(owner_uid, friend_uid);
	}

	CMysqlUtility::FreeResult(&stMysqlStatement);

	if (0 != iRet)
	{
		return false;
	}

	gamelog::g_log_world.printf(LL_MAINTANCE, "RelationCacheManager::LoadFriend total pair:%d", (int)m_friend_set.size());

	return true;
}

unsigned long long RelationCacheManager::MakeFriendPair(int uid1, int uid2)
{
	unsigned long long bigger_uid = uid1;
	unsigned long long smaller_uid = uid2;

	unsigned long long tmp_ret = (bigger_uid << 32 | smaller_uid);
	return tmp_ret;
}
