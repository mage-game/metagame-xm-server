#ifndef __CROSS_USER_REGISTER__
#define __CROSS_USER_REGISTER__

#include "engineadapter.h"
#include "servercommon/servercommon.h"
#include "servercommon/internalprotocal/crossglobalprotocal.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"

class Role;

class CrossUserRegister
{
public:
	static CrossUserRegister & Instance();

	void OnSaveCrossRoleInfo(crossgameprotocal::CrossGameSaveCrossRoleInfo *cgscri);
	void OnSaveCrossRoleInfoSucc(PlatName pname, const UserID &user_id, int original_plat_type, const UserID &original_user_id, GameName original_user_name);

	void OnUserLogin(Role *user);
	void OnUserLogout(Role *user);

	void OnConnectCrossServerSucc();

	void SendToAllCrossUser(const char *msg, int len, int cross_activity_type = CROSS_ACTIVITY_TYPE_RESERVED);
	void KickAllCrossUser(int cross_activity_type = CROSS_ACTIVITY_TYPE_RESERVED);

private:
	CrossUserRegister();
	~CrossUserRegister();

	bool CheckCanSaveCrossRoleInfo(PlatName pname);

	void SendCrossUserLoginToCross(const UserID &user_id);

	struct CrossUserInfo
	{
		CrossUserInfo() : create_time(0), original_plat_type(-1)
		{
			memset(original_user_name, 0, sizeof(original_user_name));
		}

		time_t					create_time;								// 跨服角色创建时间

		int						original_plat_type;							// 原服平台类型
		UserID					original_user_id;							// 在原服的角色ID
		GameName				original_user_name;							// 在原服的角色名
	};

	typedef std::map<std::string, UserID> PnameUserIDmap;
	typedef std::map<std::string, UserID>::iterator PnameUserIDmapIt;
	PnameUserIDmap m_pname_uid_map;											// 跨服角色pname user_id对应表

	typedef std::map<UserID, CrossUserInfo> CrossUserInfoMap;
	typedef std::map<UserID, CrossUserInfo>::iterator CrossUserInfoMapIt;
	CrossUserInfoMap m_cross_user_info_map;									// 跨服角色user_id和原服角色信息对应表
};

#endif // __CROSS_USER_REGISTER__

