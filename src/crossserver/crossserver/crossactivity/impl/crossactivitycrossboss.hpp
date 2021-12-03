#ifndef __CROSS_ACTIVITY_CROSS_BOSS_HPP__
#define __CROSS_ACTIVITY_CROSS_BOSS_HPP__

#include "crossactivity/crossactivity.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include <map>

class CrossActivityCrossBoss : public CrossActivity
{
public:
	CrossActivityCrossBoss(CrossActivityManager *cross_activity_manager);
	~CrossActivityCrossBoss();

	virtual void Init(const CrossActivityData &data);
	virtual void OnCrossUserLogin(CrossUser *cross_user);
	virtual bool CheckCanStartCross(const UniqueUserID &unique_user_id, int merge_server_id = 0) { return true; }

	void OnPlayerInfoResult(crossgameprotocal::GameCrossBossSyncPlayerInfo *tzsfr);
	
private:
	virtual void OnChangeToNextStatus();
	
	typedef std::map<long long, int> CrossBossSceneMap;
	typedef std::map<long long, int>::iterator CrossBossSceneMapIt;
};

#endif // __CROSS_ACTIVITY_1V1_HPP__


