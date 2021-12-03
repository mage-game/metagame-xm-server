#ifndef __CROSS_USER_SHADOW__
#define __CROSS_USER_SHADOW__

#include <map>

#include "engineadapter.h"

#include "servercommon/servercommon.h"

#include "servercommon/internalprotocal/crossgameprotocal.h"

class Role;

class CrossUserShadow
{
public:
	static CrossUserShadow & Instance();

	void OnSyncCrossUser(crossgameprotocal::CrossGameSyncCrossUser *cgscu);
	void OnDisconnectFromCrossServer(const int cross_type);
	void OnUserLogin(Role *user);
	void ClearCrossUserMap(const int cross_type);

private:
	CrossUserShadow();
	~CrossUserShadow();

	typedef std::map<UserID, int> InCrossUserMap;
	typedef std::map<UserID, int>::iterator InCrossUserMapIt;
	InCrossUserMap m_in_cross_user_map;			// 处于跨服状态的角色表 <UserID, cross_activity_type>
};

#endif // __CROSS_USER_SHADOW__

