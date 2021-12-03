#ifndef __CROSS_ACTIVITY_CROSS_LIEHUNFB_HPP__
#define __CROSS_ACTIVITY_CROSS_LIEHUNFB_HPP__

#include "crossactivity/crossactivity.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include <map>

class CrossActivityCrossLieKunFb : public CrossActivity
{
public:
	CrossActivityCrossLieKunFb(CrossActivityManager *cross_activity_manager);
	~CrossActivityCrossLieKunFb();

	virtual void Init(const CrossActivityData &data);
	virtual void OnCrossUserLogin(CrossUser *cross_user);
	virtual bool CheckCanStartCross(const UniqueUserID &unique_user_id, int merge_server_id = 0) { return true; }

	void OnRecvCrossLieKunFBInfo(crossgameprotocal::GameCrossLieKunFBInfoSync *gclkfbis);
	void OnRecvCrossLieKunKillZoneBossGuildInfo(int zone, GuildID guild_id);
	void SendCrossLieKunFBPlayerInfo(NetID global_netid, const UniqueUserID & unique_user_id, GuildID guild_id);
	
private:
	time_t CalcActivityNextOpenTime();
	virtual void OnChangeToNextStatus();
	void SendCrossLieKunFBTopReward();
	void ResetCrossLieKunFBInfo();

	int m_open_cfg_idx;

	CrossLieKunFBSceneInfo m_scene_list[LIEKUN_ZONE_TYPE_COUNT];
	
};

#endif // __CROSS_ACTIVITY_CROSS_LIEHUNFB_HPP__


