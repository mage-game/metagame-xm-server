#ifndef __CROSS_ACTIVITY_GUILD_BATTLE_HPP__
#define __CROSS_ACTIVITY_GUILD_BATTLE_HPP__

#include "crossactivity/crossactivity.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include <map>

class CrossActivityGuildBattle : public CrossActivity
{
public:
	CrossActivityGuildBattle(CrossActivityManager *cross_activity_manager);
	~CrossActivityGuildBattle();

	virtual void Init(const CrossActivityData &data);
	virtual void OnCrossUserLogin(CrossUser *cross_user);
	virtual bool CheckCanStartCross(const UniqueUserID &unique_user_id, int merge_server_id = 0) { return true; }

	virtual void Update(unsigned long interval, time_t now_second);
	
	void OnRegisterGameServer(NetID global_netid);

	void OnRecvGuildBattleInfo(crossgameprotocal::GameCrossGuildBattleInfoSync *cmd);
/*	void OnPlayerInfoResult(crossgameprotocal::GameCrossBossSyncPlayerInfo *tzsfr);*/
	void OnRecvGuildBattleRankLogInfo(crossgameprotocal::GameCrossGuildBattleRankLogSync *log_cmd);
	void OnRecvGuildBattleGetRankLogInfoReq(NetID global_netid, crossgameprotocal::GameCrossGetGuildBattleRankLogReq *req_cmd);

	void OnGetDropLog(const UniqueUserID &unique_user_id);
	void OnSyncDropLog(const UniqueUserID unique_user_id, crossgameprotocal::GameCrossSyncGuildBattleDropInfo *gcdi);

private:
	time_t CalcActivityNextOpenTime();
	virtual void OnChangeToNextStatus();
	void SendSpecialTimeNotice();
	
	int m_open_cfg_idx;

	typedef std::map<long long, int> CrossBossSceneMap;
	typedef std::map<long long, int>::iterator CrossBossSceneMapIt;
	
	CrossGuildBattleSceneLogInfo m_cross_guildbattle_scene_rank_log_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];

	int m_drop_log_count;				// 当前掉落日志数
	CrossGuildBattleDropLog m_drop_log_list[CROSS_ACTIVITY_XIULUO_TOWER_DROP_LOG_COUNT_MAX];		//掉落日志列表
};

#endif // __CROSS_ACTIVITY_1V1_HPP__


