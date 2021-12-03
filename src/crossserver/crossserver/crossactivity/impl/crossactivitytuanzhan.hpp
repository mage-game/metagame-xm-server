#ifndef __CROSS_ACTIVITY_TUANZHAN_HPP__
#define __CROSS_ACTIVITY_TUANZHAN_HPP__

#include "crossactivity/crossactivity.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"

class CrossActivityTuanzhan : public CrossActivity
{
public:
	CrossActivityTuanzhan(CrossActivityManager *cross_activity_manager);
	~CrossActivityTuanzhan();

	virtual void Init(const CrossActivityData &data);
	virtual void OnCrossUserLogin(CrossUser *cross_user);
	virtual bool CheckCanStartCross(const UniqueUserID &unique_user_id, int merge_server_id = 0) { return true; }

	void OnFightResult(crossgameprotocal::GameCrossTuanzhanSyncFightResult *tzsfr);

private:
	time_t CalcActivityNextOpenTime();
	virtual void OnChangeToNextStatus();

	int m_open_cfg_idx;
};

#endif // __CROSS_ACTIVITY_1V1_HPP__


