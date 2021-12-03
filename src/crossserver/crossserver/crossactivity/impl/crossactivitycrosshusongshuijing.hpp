#ifndef __CROSS_ACTIVITY_CROSS_HUSONG_SHUIJING_HPP__
#define __CROSS_ACTIVITY_CROSS_HUSONG_SHUIJING_HPP__

#include "crossactivity/crossactivity.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include <map>

class CrossActivityCrossHusongShuijing : public CrossActivity
{
public:
	CrossActivityCrossHusongShuijing(CrossActivityManager *cross_activity_manager);
	~CrossActivityCrossHusongShuijing();

	virtual void Init(const CrossActivityData &data);
	virtual void OnCrossUserLogin(CrossUser *cross_user);
	virtual bool CheckCanStartCross(const UniqueUserID &unique_user_id, int merge_server_id = 0) { return true; }

private:
	time_t CalcActivityNextOpenTime();
	virtual void OnChangeToNextStatus();

	int m_open_cfg_idx;
	
};

#endif // __CROSS_ACTIVITY_CROSS_BIANJINGZHIDI_HPP__


