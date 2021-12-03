#ifndef __CROSSUSER_H__
#define __CROSSUSER_H__

#include "servercommon/serverdef.h"
#include "servercommon/crossdef.hpp"

class CrossServer;
class CrossUserManager;

class CrossUser
{
public:
	CrossUser(const UniqueUserID &orignal_unique_user_id, GameName original_user_name, const UserID &cross_user_id);
	~CrossUser();

	const UniqueUserID & GetOriginalUniqueUserID() { return m_original_unique_user_id; }
	const UserID & GetCrossUserID() { return m_cross_user_id; }
	int GetCrossUid() { return UidToInt(m_cross_user_id); }

	UniqueServerID GetOriginalUniqueServerID() { return UniqueServerID(m_original_unique_user_id.plat_type, m_original_unique_user_id.user_id.db_index); }
	int GetOriginalPlatType() { return m_original_unique_user_id.plat_type; }
	int GetOriginalServerID() { return m_original_unique_user_id.user_id.db_index; }
	const UserID & GetOriginalUserID() { return m_original_unique_user_id.user_id; }
	const char * GetOriginalName() { return m_original_user_name; }

	int GetCrossActivityType() { return GetCrossUserCrossActivityType(m_cross_user_id); }
	
	void * operator new(size_t c);
	void operator delete(void *m);

private:
	UniqueUserID		m_original_unique_user_id;			// 在原服的全局角色ID
	GameName			m_original_user_name;				// 在原服的角色名称
	UserID				m_cross_user_id;					// 在隐藏服的角色ID
};

#endif

