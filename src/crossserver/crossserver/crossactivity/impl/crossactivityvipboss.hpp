#ifndef __CROSS_ACTIVITY_CROSS_VIP_BOSS_HPP__
#define __CROSS_ACTIVITY_CROSS_VIP_BOSS_HPP__

#include "crossactivity/crossactivity.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include <map>

class CrossActivityCrossVipBoss : public CrossActivity
{
public:
	CrossActivityCrossVipBoss(CrossActivityManager *cross_activity_manager);
	~CrossActivityCrossVipBoss();

	virtual void Init(const CrossActivityData &data);
	virtual void OnRegisterGameServer(NetID global_netid);
	virtual void Update(unsigned long interval, time_t now_second) override;
	virtual void OnCrossUserLogin(CrossUser *cross_user);
	virtual bool CheckCanStartCross(const UniqueUserID &unique_user_id, int merge_server_id = 0) { return true; }

private:
	virtual void OnChangeToNextStatus();
	
};

#endif // __CROSS_ACTIVITY_CROSS_VIP_BOSS_HPP__


