#ifndef __CROSS_ACTIVITY_MESS_BATTLE_HPP__
#define __CROSS_ACTIVITY_MESS_BATTLE_HPP__

#include "servercommon/crossdef.hpp"
#include "crossactivity/crossactivity.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"

class CrossActivityMessBattle : public CrossActivity
{
public:
	CrossActivityMessBattle(CrossActivityManager *cross_activity_manager);
	~CrossActivityMessBattle();

	virtual void Init(const CrossActivityData &data);
	virtual void OnCrossUserLogin(CrossUser *cross_user);
	virtual void OnCrossUserLogout(CrossUser *cross_user);
	virtual bool CheckCanStartCross(const UniqueUserID &unique_user_id, int merge_server_id = 0) { return true; }

	void OnRecvMessBattleRankTop(const crossgameprotocal::GameCrossMessBattleInfoSync *gcmbi);
private:
	time_t CalcActivityNextOpenTime();
	virtual void OnChangeToNextStatus();
	int m_open_cfg_idx;
};

#endif // __CROSS_ACTIVITY_MESS_BATTLE_HPP__

