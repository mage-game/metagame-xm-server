#ifndef __ROLE_CROSS_H__
#define __ROLE_CROSS_H__

#include "protocal/msgactivity.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "servercommon/rolecrossdef.hpp"

class Role;

namespace Protocol
{
	class CSEnterBossFamily;
}

enum CROSS_RING_CARD_OPER_TYPE
{
	CROSS_RING_CARD_OPER_WEAR = 1,     // 佩戴
	CROSS_RING_CARD_OPER_OFF,          // 脱下
};

enum CROSS1V1_GM_OPER_TYPE
{
	CROSS1V1_GM_OPER_TYPE_SCORE = 0,
	CROSS1V1_GM_OPER_TYPE_GONGXUN,
	CROSS1V1_GM_OPER_TYPE_JOIN_TIMES,
};

class RoleCross
{
public:
	RoleCross();
	~RoleCross();

	void SetRole(Role *role) { m_role = role; }

	void InitParam(Role *role, const RoleCrossParam &param);
	void GetInitParam(RoleCrossParam *param);

	void ReCalcAttr(CharIntAttrs & base_add, bool is_recalc);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnWeekChange();
	void OnCross1v1SeasonChange();
	void OnCross3v3SeasonChange();
	void OnLogin();
	void OnLogout();
	void Update(unsigned long interval, time_t now_second);
	void OnRoleLeaveTeam();
	RoleCrossParam & GetRoleCrossParam() { return m_param; }

	void OnStartCrossReq(int activity_type, bool is_force = false, int param_1 = 0, int param_2 = 0, int param_3 = 0, short move_to_pos_x = 0, short move_to_pos_y = 0);
	void OnStartCrossAck(crossgameprotocal::CrossGameStartCrossAck *cgsca);

	const UniqueUserID & GetCrossOriginalUniqueUserID();		// 获取原服玩家的唯一ID
	int GetOriginMergeServerID();								// 获取原服的合服服务器ID
	int GetOriginPlatType();									// 获取原服的平台类型
	UniqueServerID GetOriginalUniqueServerID();					// 获取原服的unique serverid

	static void CrossNameToOriginName(GameName name);			// 跨服名转原服名 name_s2 -> name
	static void CrossPlatNameToOrigin(PlatName name);			// 跨服平台名转原服平台名 |dev_name_2_2046 -> dev_name_3

	bool IsFromSameGameServer(Role *role);						// 是否来自同一个服务器

	bool IsEnterCrossMoveToPos() { return ((m_param.enter_cross_move_to_pos_x > 0) && (m_param.enter_cross_move_to_pos_y > 0));}
	void ClearEnterCrossMovePos() { m_param.enter_cross_move_to_pos_x = 0; m_param.enter_cross_move_to_pos_y = 0; };
	Posi GetEnterCrossMovePos() { return Posi(m_param.enter_cross_move_to_pos_x, m_param.enter_cross_move_to_pos_y); }

	void OnCrossXiuluoTowerBuyBuff(bool is_buy_realive);
	void SendCrossXiuluoTowerInfo();
	short GetCrossXiuluoTowerBuyRealiveCount() { return m_param.cross_xiuluo_tower_buy_realive_count; }
	char GetCrossXiuluoTowerAddGongjiPer() { return m_param.cross_xiuluo_tower_add_gongji_per; }
	char GetCrossXiuluoTowerAddHpPer() { return m_param.cross_xiuluo_tower_add_hp_per; }

	void IncCrossXiuluoTowerDailyDropTitleNum() { ++m_param.cross_xiuluotower_daily_drop_title_get_num; }
	void IncCrossXiuluoTowerDailyDropMountNum() { ++m_param.cross_xiuluotower_daily_drop_mount_get_num; }
	void IncCrossXiuluoTowerDailyGoldBoxTitleNum() { ++m_param.cross_xiuluotower_daily_gold_box_title_get_num; }
	char GetCrossXiuluoTowerDailyDropTitleNum() { return m_param.cross_xiuluotower_daily_drop_title_get_num; }
	char GetCrossXiuluoTowerDailyDropMountNum() { return m_param.cross_xiuluotower_daily_drop_mount_get_num; }
	char GetCrossXiuluoTowerDailyGoldBoxNum() { return m_param.cross_xiuluotower_daily_gold_box_title_get_num; }


	unsigned int GetCrossXiuluoTowerGatherBuffEndTimestamp() { return m_param.cross_xiuluo_tower_gather_buff_end_timestamp; }
	void SetCrossXiuluoTowerGatherBuffEndTimestamp(unsigned int end_timestamp) { m_param.cross_xiuluo_tower_gather_buff_end_timestamp = end_timestamp; }

	void IncreaseCrossGuildBattleDailyDropWingNum() { ++m_param.cross_guild_battle_daily_drop_wing_get_num; }
	char GetCrossGuildBattleDailyDropWingNum() { return m_param.cross_guild_battle_daily_drop_wing_get_num; }
	// 跨服1v1 --------------------------------------------------------------------------------------------------------
	void OnCrossMatch1v1Req();
	void OnSyncCross1V1Info();
	void OnSyncCross1V1Info(crossgameprotocal::CrossGame1V1Info *sci);
	void SendCross1V1SelfInfo();
	bool IsInCross1v1Matching();
	void OnCross1v1FecthRewardReq(short fetch_type, int seq);
	void Cross1v1FetchJoinTimesReward(int seq);
	void Cross1v1GiveJoinTimesReward();
	void Cross1v1FetchScoreReward(int seq);
	void Cross1v1GiveScoreReward();
	void Cross1v1GiveJoinReward();
	void Cross1v1BuyTime();
	void SetCross1v1Score(int score, int reason);
	int GetCross1v1Score();
	void OnCross1v1RingOper(int oper_type, int seq);
	void OnCross1v1GM(int type, int score);
	void OnCross1v1MatchResultReq(int type);
	bool OnActivePVPRewardItem(int item_type, int item_id, int item_seq);       //1v1 3v3 激活赛季戒指
	void OnClearCross1v1Matching(int cross_type);  // 清除1v1匹配

	// 跨服3v3 --------------------------------------------------------------------------------------------------------
	void OnSyncCrossMultiuserChallengeMatchingState(crossgameprotocal::CrossGameMultiuserChallengeMatchingStateNotify *scmcms);

	void OnCrossMultiuserChallengeOpera(bool is_matching_req);
	//void OnCrossMultiuserChallengeFetchDaycountReward();
	void CancelCrossMultiuserChallengeMatching(bool can_cancel_self_in_team = false);
	void AddCrossMultiuserChallengeScore(int add_score, const char *reason);
	
	void SendCrossMultiuserChallengeSelfActivityInfo(); // 下发跨服3v3个人信息
	int GetCrossMultiuserChallengeScore() { return m_param.cross_multiuser_challenge_score; }
	int GetCrossMultiuserChallengeMatchTotalCount() { return m_param.cross_multiuser_challenge_total_match_count; }
	int GetCrossMultiuserChallengeMatchCount() { return m_param.cross_multiuser_challenge_month_match_count; }
	int GetCrossMultiuserChallengeMatchWinCount() { return m_param.cross_multiuser_challenge_win_match_count; }
	int GetCrossMultiuserChallengeMvpCount() { return m_param.cross_multiuser_challenge_mvp_count; }
	int GetCrossMultiuserChallengeWinPercent();  // 当赛季胜率
	int GetCrossMultiuserChallengeMatchingState() { return m_param.cross_multiuser_challenge_matching_state; }
	void ResetCrossMultiuserChallengeMatchingState() { m_param.cross_multiuser_challenge_matching_state = -1; }
	void SyncCrossMultiuserChallengeRecordInfo();  // 从cross记录同步3v3信息到角色
	void SyncCrossTuanzhanRecordInfo();
	int GetCrossMultiuserChallengeDurWinCount() const { return m_param.cross_multiuser_challenge_dur_win_count; }
	void SetCrossMultiuserChallengeDurWinCount(int dur_win_count) { m_param.cross_multiuser_challenge_dur_win_count = dur_win_count; }

	void OnCrossMultiuserChallengeFetchJoinTimesReward(int seq = -1, bool need_notify = true);
	void OnCrossMultiuserChallengeFetchGongxunReward(int seq = -1, bool need_notify = true);
	bool Add3v3Gongxun(int add_value, const char *reason);
	void Cross3v3AddMatchTimes(int add_times, const char* reason);
	void OnCrossMultiuserChallengeCardOper(int oper_type, int seq);
	void Cross3v3GiveDivisionReward();
	
	bool IsInCross3v3Matching();
	void CheckCrossServerCross3v3MatchingState();   // 向crossserver查询3v3匹配状态。该函数调用必须在OnSendAllInfo之后，不可随便调用
	void SetCross3v3LogoutFlag(char flag) { m_param.cross_3v3_logout_flag = flag; }
	void OnClearCross3v3Matching(int cross_type);  // 清除3v3匹配

	// 跨服牧场 --------------------------------------------------------------------------------------------------------
	void AddPastureScore(int add_score, const char *reason);
	bool ConsumePastureScore(unsigned int score, const char *reason);
	unsigned int GetPastureScore() { return m_param.cross_pasture_score; }
	bool AddPastureGetScoreTimes(int add_score_times, const char *reason);
	int GetPastureGetScoreTimes() { return m_param.cross_pasture_today_get_score_times; }
	void SyncCrossPastureRecordInfo();

	// 跨服BOSS --------------------------------------------------------------------------------------------------------
	int GetCrossBossTodayGatherOrdinaryCrystalNum() { return m_param.today_gather_ordinary_crystal_num; }
	int GetCrossBossTodayGatherTreasureCrystalNum() { return m_param.today_gather_treasure_crystal_num; }
	int GetCrossBossTodayKillBossNum() { return m_param.today_kill_boss_num; }
	short GetCrossBossSceneLevel() { return m_param.cross_boss_scene_level; }
	void OnCrossBossReq(int opera_type, int layer, int param2);
	void OnEnterCrossBossScene(int scene_level, short move_to_pos_x = 0, short move_to_pos_y = 0);
	void SendCrossBossSelfInfo();
	void SyncCrossBossRecordInfo();
	bool AddCrossBossReliveTimes(int add_times, const char *reason, bool send_info = true);
	int GetCrossBossReliveTimes() { return m_param.cross_boss_relive_times; }
	void GetCrossBossConcernFlag(unsigned int *flag);
	void RegesiterCrossBossConcernInfo();
	void SetCrossBossConcernFlag(unsigned int flag, int index);
	bool OnCrossBossDecTireValue(int tire_value);

	void OnCrossBossBuyReliveTimes(int buy_times);

	// 跨服消耗元宝----------------------------------------------------------------------------------------------------
	void SyncCrossCommonRecordInfo();
	int GetAllowToCrossFlag() { return m_allow_to_cross_flag; }

	void SyncCrossCommonAttendRecordInfo();

	// 跨服组队本----------------------------------------------------------------------------------------------------
	void OnCrossTeamFBOption(int option_type, int layer, int room, int param, int param2 = 0, int param3 = 0);
	void OnCrossTeamFBEnter();
	bool IsCrossTeamFBPass(int layer);
	int GetCrossTeamFBDayCount();
	void OnCrossTeamFBPassLayer(int layer);
	void SendCrossTeamRBInfo();

	// 跨服帮派战----------------------------------------------------------------------------------------------------
	void CheckCrossGuildBattleClacTime();
	void SyncCrossGuildBattleInfo();
	void OnFetchCrossGuildBattleReward(int reward_type, int index);
	void OnGetCrossGuildBattleDailyReward(int index);
	void ReqCrossGuildBattleGetRankLog();
	void RespCrossGuildBattleGetRankLog(CrossGuildBattleSceneLogInfo scene_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM]);
	int GetCrossGuildBattleSceneId() { return m_param.cross_guildbattle_scene_id; }
	void GetCrossGuildBattleTuanzhangName(GameName name) { F_STRNCPY(name, m_param.cross_guild_tuanzhang_name, sizeof(GameName)); }
	void SetCrossGuildBattleTuanzhangProf(char prof) { m_param.cross_guildbattle_tuanzhang_prof = prof; }
	void SetCrossGuildBattleTuanzhangSex(char sex) { m_param.cross_guildbattle_tuanzhang_sex = sex; }
	char GetCrossGuildBattleTuanzhangProf() { return m_param.cross_guildbattle_tuanzhang_prof; }
	char GetCrossGuildBattleTuanzhangSex() { return m_param.cross_guildbattle_tuanzhang_sex; }
	void SetCrossGuildBattleTuanzhangUuid(long long uuid) { m_param.cross_guildbattle_tuanzhang_uuid = uuid;}
	long long GetCrossGuildBattleTuanzhangUuid() { return m_param.cross_guildbattle_tuanzhang_uuid; }
	void SendGuildBattleTaskInfo();
	void SkipCrossGuildBattleTask();

	//猎鲲地带
	bool CheckCanUpdateLieKunRank();
	bool OnEnterCrossLieKunFb(short zone, bool is_force); //初步检测副本能否进入
	short GetCrossLieKunSceneZone() { return m_param.cross_liekun_scene_zone; };

	// 跨服vipboss
	bool OnEnterCrossVipBoss(int enter_type, int scene_id, int boss_id);
	bool GetCrossVipBossEnterInfo(Protocol::CSEnterBossFamily *ebf);

	// 跨服秘藏boss
	short GetCrossMizangBossSceneLevel() { return m_param.cross_mizang_boss_scene_level; }
	int GetCrossMizangBossTodayGatherOrdinaryCrystalNum() { return m_param.cross_mizang_boss_today_gather_ordinary_crystal_num; }
	int GetCrossMizangBossTodayGatherTreasureCrystalNum() { return m_param.cross_mizang_boss_today_gather_treasure_crystal_num; }
	int GetCrossMizangBossTodayKillBossNum() { return m_param.cross_mizang_boss_today_kill_boss_num; }
	void OnCrossMizangBossReq(int opera_type, int layer, int param2);
	void OnEnterCrossMizangBossScene(int scene_level, short move_to_pos_x = 0, short move_to_pos_y = 0);
	bool AddCrossMizangBossReliveTimes(int add_times, const char *reason, bool send_info);
	bool OnCrossMizangBossDecTireValue(int tire_value);
	void SetCrossMizangBossConcernFlag(unsigned int flag, int index);
	void GetCrossMizangBossConcernFlag(unsigned int *flag);
	void SendCrossMizangBossSelfInfo();
	void SyncCrossMizangBossRecordInfo();
	void RegesiterCrossMizangBossConcernInfo();
	

	// 跨服幽冥boss
	short GetCrossYoumingBossSceneLevel() { return m_param.cross_youming_boss_scene_level; }
	int GetCrossYoumingBossTodayGatherOrdinaryCrystalNum() { return m_param.cross_youming_boss_today_gather_ordinary_crystal_num; }
	int GetCrossYoumingBossTodayGatherTreasureCrystalNum() { return m_param.cross_youming_boss_today_gather_treasure_crystal_num; }
	int GetCrossYoumingBossTodayKillBossNum() { return m_param.cross_youming_boss_today_kill_boss_num; }
	void OnCrossYoumingBossReq(int opera_type, int layer, int param2);
	void OnEnterCrossYoumingBossScene(int scene_level, short move_to_pos_x = 0, short move_to_pos_y = 0);
	bool AddCrossYoumingBossReliveTimes(int add_times, const char *reason, bool send_info);
	bool OnCrossYoumingBossDecTireValue(int tire_value);
	void SetCrossYoumingBossConcernFlag(unsigned int flag, int index);
	void GetCrossYoumingBossConcernFlag(unsigned int *flag);
	void SendCrossYoumingBossSelfInfo();
	void SyncCrossYoumingBossRecordInfo();
	void RegesiterCrossYoumingBossConcernInfo();
	void OnCrossConsumeGold(int consume_gold_num);

	// 跨服边境之地
	bool OnEnterCrossBianJingZhiDi(bool is_force);

	char GetCrossHusongShuijingEnterType() { return m_param.cross_husong_shuijing_enter_type; }

public:
	void CheckCanRetransferToCrossOnRoleLogin();  // 检查是否可以重连进跨服
	void SetCrossLogoutRecord(const RoleCrossLogoutRecord &record) { m_param.role_cross_logout_record = record; }
	void ClearCrossLogoutRecord() { m_param.role_cross_logout_record.Reset(); }
public:
	bool IsTransferToCrossIng();
private:
	void CheckCreateCrossRecord();

	Role *m_role;

	time_t m_last_cross_req_time;
	RoleCrossParam m_param;
	CharIntAttrs m_attrs_add;

	UniqueUserID m_cross_original_unique_user_id;
	int m_allow_to_cross_flag;

};

//-------------------------------------------------------------------------------------------

inline const UniqueUserID & RoleCross::GetCrossOriginalUniqueUserID()
{
	return m_cross_original_unique_user_id;
}

inline int RoleCross::GetOriginMergeServerID()
{
	return m_param.merge_server_id;
}

inline int RoleCross::GetOriginPlatType()
{
	return m_cross_original_unique_user_id.plat_type;
}

inline UniqueServerID RoleCross::GetOriginalUniqueServerID()
{
	return UniqueServerID(m_cross_original_unique_user_id.plat_type, m_cross_original_unique_user_id.user_id.db_index);
}

//-------------------------------------------------------------------------------------------

#endif

