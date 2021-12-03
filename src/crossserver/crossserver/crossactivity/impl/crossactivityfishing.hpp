#ifndef __CROSS_ACTIVITY_FISHING_HPP__
#define __CROSS_ACTIVITY_FISHING_HPP__

#include "servercommon/crossdef.hpp"
#include "crossactivity/crossactivity.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"

class CrossActivityFishing : public CrossActivity
{
public:
	CrossActivityFishing(CrossActivityManager *cross_activity_manager);
	~CrossActivityFishing();

	virtual void Init(const CrossActivityData &data);
	virtual void OnCrossUserLogin(CrossUser *cross_user);
	virtual bool CheckCanStartCross(const UniqueUserID &unique_user_id, int merge_server_id = 0) { return true; }

	void OnReceiveFishingInfo(crossgameprotocal::GameCrossFishingInfoSync *gcfis);

private:
	time_t CalcActivityNextOpenTime();
	virtual void OnChangeToNextStatus();
	void ClearRankTitle();

	int m_open_cfg_idx;
};

#endif // __CROSS_ACTIVITY_FISHING_HPP__

