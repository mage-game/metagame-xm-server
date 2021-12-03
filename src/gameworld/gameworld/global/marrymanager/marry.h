#ifndef __MARRY_H__
#define __MARRY_H__

#include "servercommon/marrydef.hpp"

class Role;

class Marry
{
public:
	Marry();
	~Marry();

	void OnMarryStart(const MarryReserve &marry_reserve, int total_marry_count, unsigned int marry_start_time);
	void OnUpdate(unsigned long interval, time_t now_second);

	void OnUserLogin(Role *role);
	void OnHunyanEnterReq(Role *role);
	void OnMarryobjSyncPos(short pos_x, short pos_y);
	void OnHunyanInviteUser(int invite_uid, int be_invited_uid, bool is_free_invite);

	void OnGmForceToNextStatus();

	bool IsMarryClose() { return (MARRY_STATUS_INVALID == m_status || MARRY_STATUS_FINISH == m_status); }

private:
	void OnStatusChange(int to_status);

	void OnMarryStatusToZhunBei();									// ªÈ¿Ò ◊º±∏
	void OnMarryStatusToBaiTianDi();								// ªÈ¿Ò ∞›ÃÏµÿ
	void OnMarryStatusToXunYou();									// ªÈ¿Ò —≤”Œ
	void OnMarryStatusToHunYanZhunBei();							// ªÈ¿Ò ªÈ—Á◊º±∏
	void OnMarryStatusToHunYan();									// ªÈ¿Ò ªÈ—Á
	void OnMarryStatusToFinish();									// ªÈ¿Ò Ω· ¯

	void SyncMarryInfoToGameWorld();
	void NotifyMarryStatusAllUser(Role *role = NULL);
	void NotifyHunyanInviteList(Role *role);

	SyncMarryInfo m_marry_info;

	int m_status;
	unsigned int m_next_status_time;

	MarryHunyanInviteList m_hunyan_invite_list[MARRY_COUPLE_USER_LIMIT_NUM];
};

#endif // __MARRY_H__

