#ifndef __CROSS_ACTIVITY_PASTURE_HPP__
#define __CROSS_ACTIVITY_PASTURE_HPP__

#include "crossactivity/crossactivity.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"

class CrossActivityPasture : public CrossActivity
{
public:
	CrossActivityPasture(CrossActivityManager *cross_activity_manager);
	~CrossActivityPasture();

	virtual void Init(const CrossActivityData &data);
	virtual void OnCrossUserLogin(CrossUser *cross_user);
	virtual bool CheckCanStartCross(const UniqueUserID &unique_user_id, int merge_server_id = 0) { return true; }

	void OnReceiveInfoSync(crossgameprotocal::GameCrossPastureInfoSync * pis);

private:
	time_t CalcActivityNextOpenTime();
	virtual void OnChangeToNextStatus();
	int m_open_cfg_idx;
};

#endif // __CROSS_ACTIVITY_PASTURE_HPP__

