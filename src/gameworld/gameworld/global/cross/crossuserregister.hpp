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

		time_t					create_time;								// �����ɫ����ʱ��

		int						original_plat_type;							// ԭ��ƽ̨����
		UserID					original_user_id;							// ��ԭ���Ľ�ɫID
		GameName				original_user_name;							// ��ԭ���Ľ�ɫ��
	};

	typedef std::map<std::string, UserID> PnameUserIDmap;
	typedef std::map<std::string, UserID>::iterator PnameUserIDmapIt;
	PnameUserIDmap m_pname_uid_map;											// �����ɫpname user_id��Ӧ��

	typedef std::map<UserID, CrossUserInfo> CrossUserInfoMap;
	typedef std::map<UserID, CrossUserInfo>::iterator CrossUserInfoMapIt;
	CrossUserInfoMap m_cross_user_info_map;									// �����ɫuser_id��ԭ����ɫ��Ϣ��Ӧ��
};

#endif // __CROSS_USER_REGISTER__

