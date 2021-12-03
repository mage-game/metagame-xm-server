#ifndef __CROSS_ACTIVITY_XIULUO_TOWER_HPP__
#define __CROSS_ACTIVITY_XIULUO_TOWER_HPP__

#include <map>
#include <set>
#include <vector>
#include <algorithm>

#include "engineadapter.h"

#include "servercommon/activitydef.hpp"
#include "servercommon/serverdef.h"
#include "servercommon/crossdef.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "servercommon/internalprotocal/crossglobalprotocal.h"

#include "crossactivity/crossactivity.hpp"

#include "protocal/msgcross.h"

struct CrossXiuluoTowerRoomInfo
{
	CrossXiuluoTowerRoomInfo() : join_user_count(0) 
	{
		memset(join_uuid_list, 0, sizeof(join_uuid_list));
	}

	int join_user_count;
	long long join_uuid_list[CROSS_ACTIVITY_XIULUO_TOWER_ROOM_USER_MAX_NUM];
};

class CrossActivityXiuluoTower : public CrossActivity
{
public:
	CrossActivityXiuluoTower(CrossActivityManager *cross_activity_manager);
	virtual ~CrossActivityXiuluoTower();

	virtual void Init(const CrossActivityData &data);

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnDayChange();

	virtual bool CheckCanStartCross(const UniqueUserID &unique_user_id, int merge_server_id = 0);

	virtual void OnCrossUserLogin(CrossUser *cross_user);

	void OnSyncUserResult(crossgameprotocal::GameCrossSyncXiuluoTowerResult *gcsxltr);

	void OnGetDropLog(const UniqueUserID &unique_user_id);

	void OnSyncDropLog(const UniqueUserID unique_user_id, crossgameprotocal::GameCrossSyncXiuluoTowerDropInfo *gcdi);

	void OnRecvSyncRankTile(crossgameprotocal::GameCrossSyncXiuluoTowerRankTitle * gcrt);

private:
	time_t CalcActivityNextOpenTime();
	virtual void OnChangeToNextStatus();

	int m_open_room_count;
	CrossXiuluoTowerRoomInfo m_room_info_list[CROSS_ACTIVITY_XIULUO_TOWER_ROOM_MAX];

	int m_drop_log_count;				// 当前掉落日志数
	CrossXiuluoTowerDropLog m_drop_log_list[CROSS_ACTIVITY_XIULUO_TOWER_DROP_LOG_COUNT_MAX];		//掉落日志列表
};

#endif // __CROSS_ACTIVITY_XIULUO_TOWER_HPP__

