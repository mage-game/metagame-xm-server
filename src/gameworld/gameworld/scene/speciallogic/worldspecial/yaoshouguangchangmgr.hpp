#ifndef _YAOSHOUGUANGCHANG_MGR_HPP_
#define _YAOSHOUGUANGCHANG_MGR_HPP_

#include "servercommon/servercommon.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "inetworkmodule.h"
#include <map>

#include "config/activityconfig/yaoshouguangchangconfig.hpp"

class Role;

class YaoShouGuangChangMgr
{
public:
	static YaoShouGuangChangMgr & Instance();

	void OnServerStart();

	void Update(unsigned long interval, time_t now_second);
	void OnRoleLogin(Role *role) { this->NotifyYSGCReward(role); }

	void OnGetYSGCState(Role *role = NULL);
	void OnGetYSGCReward(Role *role, int times);
	void NotifyYSGCReward(Role *role, bool force = false);

	void OnJoinActivity(Role *role, bool is_buy_times);

	int GetUserRoomKey(const UserID & user_id);

	void GmForceNextStatus();

private:
	YaoShouGuangChangMgr();
	~YaoShouGuangChangMgr();

	int GetLvIntervalRoomMinKey(int lv_interval_idx);
	int GetLvIntervalRoomMaxKey(int lv_interval_idx);
	int GetLvIntervalRoomKeyHelper(int lv_interval_idx, int room_idx);

	void OnActivityStop();
	void OnActivityStandBy();
	void OnActivityStart();

	int m_status;
	unsigned int m_next_status_time;

	int m_lv_interval_player_count_list[YAOSHOUGUANGCHANG_LEVEL_INTERVAL_MAX];

	typedef std::map<UserID, int> UserRoomMap;
	typedef std::map<UserID, int>::iterator UserRoomMapIt;
	UserRoomMap m_user_room_key_map;
};

#endif




