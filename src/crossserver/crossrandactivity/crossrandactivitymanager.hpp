#ifndef __CROSS_RAND_ACTIVITY_MANAGER_HPP__
#define __CROSS_RAND_ACTIVITY_MANAGER_HPP__

#include "engineadapter.h"

#include "servercommon/activitydef.hpp"
#include "servercommon/crossdef.hpp"
#include "servercommon/crossrandactivitydef.hpp"

class CrossServer;
class CrossRandActivity;
class CrossActivityXiuluoTower;

namespace crossgameprotocal {
	struct GameCrossSyncCrossRandActivityData;
}

class CrossRandActivityManager
{
public:
	CrossRandActivityManager();
	~CrossRandActivityManager();

	void SetCrossServer(CrossServer *cross_server) { m_cross_server = cross_server; }
	CrossServer * GetCrossServer() { return m_cross_server; }

	void OnServerStart() {}
	void OnServerStop() {}

	void InitActivity();
	void Update(unsigned long interval, time_t now_second);
	void OnDayChange();
	void OnWeekChange();

	bool IsLoadFinish();

	void OnRegisterGameServer(NetID global_netid);

	CrossRandActivity * GetActivity(int activity_type);
	void OnForceToNextState(int cross_activity_type);

	void OnSyncActivityData(NetID global_netid, crossgameprotocal::GameCrossSyncCrossRandActivityData * srad, int length);

private:
	CrossServer *m_cross_server;

	CrossRandActivity *m_activity_list[CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT];		// 跨服随机活动列表
};

#endif // __CROSS_ACTIVITY_MANAGER_HPP__

