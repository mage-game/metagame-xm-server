#ifndef __CROSS_ACTIVITY_CROSS_BIANJINGZHIDI_HPP__
#define __CROSS_ACTIVITY_CROSS_BIANJINGZHIDI_HPP__

#include "crossactivity/crossactivity.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include <map>

class CrossActivityCrossBianJingZhiDi : public CrossActivity
{
public:
	CrossActivityCrossBianJingZhiDi(CrossActivityManager *cross_activity_manager);
	~CrossActivityCrossBianJingZhiDi();

	virtual void Init(const CrossActivityData &data);
	virtual void OnCrossUserLogin(CrossUser *cross_user);
	virtual bool CheckCanStartCross(const UniqueUserID &unique_user_id, int merge_server_id = 0) { return true; }

	void SendCrossBianJingZhiDiBossInfo(NetID global_netid, const UniqueUserID & unique_user_id);
	void OnSyncCrossBianJingZhiDiBossInfo(crossgameprotocal::GameCrossSyncCrossBianJingZhiBossInfo *scrad);

	struct BossInfo
	{
		int boss_id;
		int live_flag;
	};

private:
	time_t CalcActivityNextOpenTime();
	virtual void OnChangeToNextStatus();

	int m_open_cfg_idx;
	
	unsigned int m_next_boss_refresh_time;
	BossInfo m_boss_list[CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE];
};

#endif // __CROSS_ACTIVITY_CROSS_BIANJINGZHIDI_HPP__


