#ifndef __ACTIVITY_SHADOW_HPP__
#define __ACTIVITY_SHADOW_HPP__

#include <map>
#include <vector>
#include <set>

#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "servercommon/activitydef.hpp"
#include "protocal/msgactivity.hpp"
#include "obj/otherobj/bigcoinobj.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "global/rank/personrank.hpp"

class Role;
class Monster;

class ActivityShadow
{
public:
	static const int UPGRADE_RANK_GET_MAX_ROLE_NUM = 100;

	static ActivityShadow & Instance();

	int GetFBKeyByRoomIndex(int room_index) { return room_index + 1; }

	bool SetActivityStatus(ActivityStatus *as);
	void OnSyncActivityStatus(ActivityStatus *as);
	void OnSyncCrossActivityStatus(int cross_type, crossgameprotocal::CrossGameSyncActivityStatus *cgsas);  // 同步跨服活动状态
	
	bool IsActivtyOpen(int activity_type);
	bool IsActivtyStandy(int activity_type);
	bool IsActivtyClose(int activity_type);
	bool IsCrossRandActivityOpen(int cross_rand_activity_type);
	unsigned int GetActivityBeginTime(int activity_type);
	unsigned int GetActivityEndTime(int activity_type);
	unsigned int GetRandActivityBeginTime(int activity_type);
	unsigned int GetRandActivityEndTime(int activity_type);

	unsigned int GetSpecialActivityBeginTime(int special_activity_type);
	unsigned int GetSpecialActivityEndTime(int special_activity_type);
	bool IsSpecialActivityOpen(int special_activity_type);
	bool IsInSpecialActivityOpenTime(int special_activity_type, unsigned int timestamp);
	bool IsInGameOpenActivityTime(unsigned int timestamp);
	bool IsInCombineActivityTime(unsigned int timestamp);
	bool IsInCurrentCrossRandActivityOpenTime(int cross_rand_activity_type, unsigned int timestamp);

	bool IsRandActivityOpen(int rand_activity_type);
	bool IsInRandActivityOpenTime(int rand_activity_type, unsigned int timestamp);
	bool HadOpenActivityBefore(int rand_activity_type);
	
	void Update(unsigned long interval);
	void OnDayChange(unsigned int old_dayid, unsigned int now_day_id);

	void OnRolePickBigCoin(Role *role, BigCoinObj *bigcoin);
	SpecialZhuXieUser * GetZhuXieUser(const UserID &user_id, int world_level);
	
	void OnUserLogin(Role *role);
	void SendMonsterInvadeNoticeNextTime(Role *role);
	void SendMillionaireBoxNoticeNextTime(Role *role);

	void OnGather(Role *role, int gather_id, ObjID obj_id);

	int GetMoShenJiangLinBossID() { return m_moshenjianglin_boss_id; }
	void OnRoleHurtMonster(Role *role, Monster *monster, Attribute hurt);
	void OnKillMonster(Role *role, Monster *monster);

	bool IsGuaJiActivityExtraReward(Role *role, int *extra_reward_times);

	void OnCSASubActivitySyncState(gglobalprotocal::CombineServerActivitySyncState *csass);
	bool IsCSASubActivityNoStart(int sub_type);
	bool IsCSASubActivityOpen(int sub_type);
	bool IsCSASubActivityFinish(int sub_type);

	bool IsCrossActivityStandby(int cross_activity_type);
	bool IsCrossActivityOpen(int cross_activity_type);
	bool IsCrossActivityClose(int cross_activity_type); 
	unsigned int GetCrossActivityBeginTime(int cross_activity_type);
	unsigned int GetCrossActivityEndTime(int cross_activity_type);

	void OnCrossXiuluoTowerStatusChange(int from_status, int to_status);
	CrossXiuluoTowerUser * GetCrossXiuluoTowerUser(long long uuid);
	CrossXiuluoTowerUser * RandGetCrossXiuluoTowerUser(long long uuid);
	void CheckCrossXiuluoTowerRank(long long uuid);
	void SetCrossXiuluoTowerRankTitle(long long uuid);
	int GetCrossXiuluoTowerRankList(Protocol::SCCrossXiuluoTowerRankInfo::RankInfo rank_list[CROSS_ACTIVITY_XIULUO_TOWER_RANK_NUM]);
	int GetCrossXiuluoTowerRankTitleList(Protocol::SCCossXiuluoTowerRankTitleInfo::RankInfo titl_rank[CROSS_ACTIVITY_XIULUO_TOWER_RANK_NUM]);
	int GetCrossXiuluoTowerMaxLayer() { return m_cross_xiuluo_tower_max_layer; }
	void SetCrossXiuluoTowerMaxLayer(int max_layer) { m_cross_xiuluo_tower_max_layer = max_layer; }
	void IncCrossXiuluoTowerServerDropTitleNum() { ++m_xiuluotower_server_daily_drop_title_num; }
	void IncCrossXiuluoTowerServerDropMountNum() { ++m_xiuluotower_server_daily_drop_mount_num; }
	void IncCrossXiuluoTowerServerGoldBoxNum() { ++m_xiuluotower_server_daily_gold_box_num; }

	const int GetCrossXiuluoTowerServerDropTitleNum() { return m_xiuluotower_server_daily_drop_title_num; }
	const int GetCrossXiuluoTowerServerDropMountNum() { return m_xiuluotower_server_daily_drop_mount_num; }
	const int GetCrossXiuluoTowerServerGoldBoxNum() { return m_xiuluotower_server_daily_gold_box_num; }

	void IncreaseCrossGuildBattleDropWingNum() { ++m_guild_battle_daily_drop_wing_num; }
	const int GetCrossGuildBattleDropWingNum() { return m_guild_battle_daily_drop_wing_num; }

	//跨服1v1-----------------------------------------------------------------------------------------------------
	CrossActivity1v1FightUnit* GetCross1v1FightMatchUnit(long long uuid);					// 获取匹配信息
	void ClearRoleMatchInfo(long long uuid);												// 清除匹配信息
	void OnCross1v1MatchInfo(crossgameprotocal::CrossGameSyncCross1v1MatchInfo *cgscmi);	// 同步匹配信息
//	unsigned int GetCross1v1FightStartTime() { return m_cross_1v1_fight_start_time; }
	unsigned int GetCross1v1LastMatchTime() { return m_cross_1v1_last_match_time; }
	unsigned int GetCross1v1NextMatchTime() { return m_cross_1v1_next_match_time; }

	//-----------------------------------------------------------------------------------------------------
	bool CrossActivityForceNextState(int activity_type);

	SyncUserInfoOnBack * GetCrossUserInfoOnBack(long long uuid);
	void DeleteCrossUserInfoOnBack(long long uuid);

	const CrossTeamFBUserInfo * GetCrossTeamFBUserInfo(long long uuid);
	void SyncCrossTeamFBRoomInfo(CrossTeamFBUserInfo info);

	time_t GetXiuLuoTowerBossRefreshTime() { return m_xiuluotower_boss_refresh_time; }
	void SetXiuLuoTowerBossRefreshTime(time_t boss_refresh_time) { m_xiuluotower_boss_refresh_time = boss_refresh_time; }
	unsigned int GetXiuLuoTowerBossCount() { return m_xiuluotower_boss_count; }
	void SetXiuLuoTowerBossCount(int boss_count) { m_xiuluotower_boss_count = boss_count; }
	void SendXiuLuoTowerAllRoleInfo();

	void ChangeShenzhouGatherNum(int num);
	int GetShenzhouGatherNum(){return m_shenzhou_weapon_gather_num;}
	unsigned int GetShenzhouGatherNextRefreshTime(){return static_cast<unsigned int>(m_shenzhou_weapon_next_refresh_gather_time);}
	void SynceShenzhouGatherInfoToAll();
	void SetMillionairBossNextFreshTime(unsigned int next_refresh_time){m_next_millionaire_refresh_boss_time = next_refresh_time;}
	unsigned int GetMillionairBossNextFreshTime(){ return m_next_millionaire_refresh_boss_time;}

	//跨服帮派战-----------------------------------------------------------------------------------------------------
	CrossGuildBattleUserInfo * GetCrossGuildBattleUser(Role * role);
	void OnAddGuildBattleRoleScore(Role *role, int score);
	void OnAddRoleScore(Role *role, int score);				// 添加玩家个人积分信息
	void SortRoleScoreRank();								// 积分排序
	void SyncRoleTopToGame();								// 同步回原服
	int GetGuildBattleRoleScore(Role *role);
	int GetGuildBattleRoleSosTimes(Role *role);
	void OnCrossGuildBattleSpecialTimeNotice(Role *role = nullptr);
	bool CheckCrossGuildBattleSpecialTime();
	UniqueUserID GetGuildUniqueUserId(Role *role);
	void SyncCrossGuildBattleSceneRoleNum(UniqueUserID guild_id);
	void OnCrossGuildBattlSosCost(Role * role);
	void SendCrossGuildBattleStatistic(UniqueUserID guild_id, int notify, Role *role = NULL);
	void SendCrossGuildBattleInfoStatisticAll();
	void SyncCrossGuildBattleMvpInfo(UniqueUserID guild_id, Role* role = NULL);
	void SendResultInGuildChat(); // 发送胜利结果到仙盟聊天
	

	// 活动-帮派答题-------------------------------------------------------------------------------------------------
	void OnGuildQuestionEnterSceneReq(Role *role);
	void OnJoinGuildQuestionActivity(Role *role);
	bool IsRoleInGuildQuestionActivityState(Role *role);
	bool IsCanGatherGuildQuestion(Role *role);
	GuildQuestionPlayerInfo * GetGuildQuestionPlayerInfo(const UserID &user_id, const GameName user_name);
	bool OnGuildQuestionAnswerQuestion(Role *role, const char* buff, int total_len);	// 答题请求
	void SendGuildQuestionQuestionInfo(GuildID guild_id = 0, Role *role = NULL);		// 发送帮派的题目信息
	void GuildQuestionQuickSetQuestion(GuildID guild_id);								// 快速出题(对于已开始答题，再创建帮派时)
	void AddGuildQuestionInfo(Role *role);												// 添加帮派答题信息
	// 跨服猎鲲地带
	CrossLieKunFBUser * GetCrossLieKunFBUser(long long uuid);
	void ClearCrossLieKunFBUserGatherTimes(int zone, int boss_type);

	// 随机活动--聚宝盆---------------------------------------------------------------------------------
	void AddRACollectTreasureRecord(CollectTreasureRollRecord record_item);
	void GetRACollectTreasureRecord(CollectTreasureRollRecord record_list[RAND_ACTIVITY_COLLECT_TREASURE_MAX_RECORD_COUNT], short &count);

	//形象比拼
	bool GetImageCompetitionRankInfo(Role *role, int *rank_value, long long *flexible_ll);

	// 通用的跨服中仙盟SOS请求
	void OnCrossGuildSOSReq(Role * role, Protocol::CSSendGuildSosReq *sgsr);

	// 活动击杀-增加仙盟仇人
	void OnCrossAddGuildEnemy(Role *be_killer, Role *killer);
	void OnGetGuildEnemyListFromOriginal(Role *role);

private:
	void OnZhuxieClose();	
	void OnMoShenJiangLinOpen();																// 魔神降临
	void OnMoShenJiangLinClose();

	void OnRaUpgradeMountRankClose();
	void OnRaUpgradeHaloRankClose();
	void OnRaUpgradeWingRankClose();
	void OnRaUpgradeShengongRankClose();
	void OnRaUpgradeShenyiRankClose();
	void OnRaUpgradeQilinbiRankClose();
	void OnRaUpgradeToushiRankClose();
	void OnRaUpgradeYaoshiRankClose();
	void OnRaUpgradeLingTongRankClose();
	void OnRaUpgradeLingGongRankClose();
	void OnRaUpgradeLingQiRankClose();
	void OnRaEuqipStrenghtLevelRankClose();
	void OnRaStoneTotalLevelRankClose();
	void OnRAXianNvTotalRankClose();
	void OnRAJingLingTotalRankClose();
	void OnRAFightMountTotalRankClose();
	void OnRATotalCapRankClose();
	void OnRaUpgradeRankClose(int rank_type, int ra_type);
	int OnRaUpgradeRankFiltrate(int ra_type, int rank_type, int count, PersonRankItem *item_list);

//新增时装、神兵、法宝、足迹
	void OnRaUpgradeShizhuangRankClose();
	void OnRaUpgradeShenbinRankClose();
	void OnRaUpgradeFabaoRankClose();
	void OnRaUpgradeFootprintRankClose();
	void OnRaUpgradeFlypetRankClose();
	void OnRaUpgradeWeiyanRankClose();

	void OnGuaJiActivityOpen();																	// 幸运挂机
	void OnGuaJiActivityClose();
	void CheckGuaJiActivityReward(time_t now_second);

	void SendCSASubActivityState(Role *role);
	void OnCrossActivityStatusChange(int cross_activity_type, int from_status, int to_status, unsigned int end_time = 0);
	bool CheckSyncCrossActivityStatusBefore(int cross_activity_type, int to_status);			// 检查是否可以同步跨服的活动状态

	void ShenzhouWeaponCalcNextFleshGatherTime(time_t now);
	void ShenzhouWeaponReFreshGathers();

	// 活动-帮派答题-------------------------------------------------------------------------------------------------
	void GuildQuestionRefreshGuildScore(GuildID guild_id, unsigned int now_second);				// 单个帮派积分改变
	int GetGuildQuestionGuildScore(GuildID guild_id);											// 得到单个帮派的积分
	int GetGuildQuestionGuildRank(GuildID guild_id);											// 得到单个帮派排名
	void SendGuildQuestionGuildScoreRankInfo(Role *role = NULL);								// 发送积分排行
	void SendGuildQuestionPlayerInfo(Role *role, bool is_answer = false);
	void RewardAllGuildQuestionGuildOnEnd();
	void BroadcastGuildquestionEnd(const GuildQuestionQuestion &question_info);					// 发群聊系统公告

	void OnGuildQuestionActivityStandy();
	void OnGuildQuestionActivityStart();
	void OnGuildQuestionActivityClose();
	void OnGuildQuestionActivityUpdate();

	// 跨服BOSS
	void CheckCrossBossRefreshNotice(time_t now);												// 检查跨服类BOSS刷新提醒

	//形象比拼结算
	void OnImageCompetitionAccount();
	void SendImageCompetitionInfo(Role *role = nullptr);
	void SyncCrossActivityStatusToUser(int cross_activity_type, Role *user = NULL);

	ActivityShadow();
	~ActivityShadow();

	ActivityStatus m_activity_status[ACTIVITY_TYPE_MAX];										// 活动状态
	ActivityStatus m_special_activity_status[SPECIAL_ACTIVITY_TYPE_MAX];						// 特殊活动状态
	ActivityStatus m_rand_activity_status[RAND_ACTIVITY_TYPE_MAX];								// 随机活动状态
	ActivityStatus m_cross_activity_status[CROSS_ACTIVITY_TYPE_MAX];							// 随机活动状态
	ActivityStatus m_cross_rand_activity_status[CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT];			// 跨服随机活动状态

	SpecialZhuXieUserMap m_zhuxie_usermap;														// 诛邪战场的用户列表 

	time_t m_next_monster_invade_time;															// 下一波怪物入侵时间

	unsigned int m_next_millionaire_refresh_box_time;											// 大富豪下一波宝箱刷新时间
	unsigned int m_next_millionaire_refresh_boss_time;											// 大富豪boss刷新时间

	int m_moshenjianglin_boss_id;																// 魔神降临bossid

	time_t m_guaji_reward_next_timetamp;														// 挂机活动

	char m_csa_sub_activity_state_list[CSA_SUB_TYPE_MAX];										// 合服活动子活动状态

	typedef std::map<long long, CrossXiuluoTowerUser> CrossXiuluoTowerUserMap;
	typedef std::map<long long, CrossXiuluoTowerUser>::iterator CrossXiuluoTowerUserMapIt;

	CrossXiuluoTowerUserMap m_cross_xiuluo_tower_user_map;

	int m_cross_xiuluo_tower_rank_count;
	CrossXiuluoTowerUserRankItem m_cross_xiuluo_tower_rank_list[CROSS_ACTIVITY_XIULUO_TOWER_RANK_NUM];

	int m_cross_xiuluo_tower_rank_title_count;
	CrossXiuluoTowerUserRankItem m_cross_xiuluo_tower_rank_title_list[CROSS_ACTIVITY_XIULUO_TOWER_RANK_NUM];

	int m_cross_xiuluo_tower_max_layer;

	std::map<long long, CrossActivity1v1FightUnit> m_cross_1v1_match_map;
	unsigned int m_cross_1v1_fight_start_time;
	unsigned int m_cross_1v1_last_match_time;
	unsigned int m_cross_1v1_next_match_time;

	typedef std::map<long long, SyncUserInfoOnBack> SyncUserInfoOnBackMap;
	typedef std::map<long long, SyncUserInfoOnBack>::iterator SyncUserInfoOnBackMapIt;
	SyncUserInfoOnBackMap m_syncuserinfo_onback_map;

	short m_shenzhou_weapon_gather_num;
	time_t m_shenzhou_weapon_next_refresh_gather_time;

	typedef std::map<long long, CrossTeamFBUserInfo> CrossTeamFBUserInfoMap;
	typedef std::map<long long, CrossTeamFBUserInfo>::iterator CrossTeamFBUserInfoMapIt;
	CrossTeamFBUserInfoMap m_cross_team_fb_user_info;

	time_t m_xiuluotower_boss_refresh_time;
	int m_xiuluotower_boss_count;

	typedef std::map<UniqueUserID, CrossGuildBattleUserInfo> CrossGuildBattleMap;
	CrossGuildBattleMap m_cross_guildbattle_map;

	//std::map< GuildID, std::vector<CrossGuildBattleUserScoreRankInfo> >  m_guildbattle_user_score_rank_map_old;

	std::map< UniqueUserID, std::vector<CrossGuildBattleUserScoreRankInfo> >  m_guildbattle_user_score_rank_map;
	std::map< UniqueUserID, std::vector<CrossGuildBattleUserInfo*> >  m_guildbattle_user_rank_map;

	//int m_xiuluotower_server_daily_drop_title_num_limit;		// 修罗塔每日掉落称号数量上限
	//int m_xiuluotower_server_daily_drop_mount_num_limit;		// 修罗塔每日掉落坐骑数量上限
	//int m_xiuluotower_server_daily_gold_box_num_limit;			// 修罗塔每日金箱子奖励数量上限

	int m_xiuluotower_server_daily_drop_title_num;				// 修罗塔每日掉落称号数量
	int m_xiuluotower_server_daily_drop_mount_num;				// 修罗塔每日掉落坐骑数量
	int m_xiuluotower_server_daily_gold_box_num;				// 修罗塔每日金箱子奖励数量
	int m_guild_battle_daily_drop_wing_num;						// 跨服帮派战羽翼掉落数量
	
	// 活动-帮派答题-------------------------------------------------------------------------------------------------
	int m_question_state;																		// 答题活动当前状态
	unsigned int m_next_question_state_time;													// 答题活动下次状态时间
	int m_question_index;																		// 当前题目下标
	int m_question_id;																			// 题目ID
	int m_question_answer;																		// 题目答案
	int m_time_count;                                                                           // 每隔10秒矫正一次时间
	int m_guild_question_state;																	// 帮派答题 帮派答题状态
	time_t m_next_give_reward_timestamp;														// 帮派答题 下次奖励时间戳
	time_t m_guild_question_state_change_timestamp;												// 帮派答题 答题状态切换的时间戳

	QuestionUserMap m_question_usermap;															// 答题活动 用户列表 
	QuestionRankUserVec m_question_rankvec;														// 答题活动 排行列表
	std::set<int> m_question_id_set;															// 本次答题题目列表

	GuildQuestionPlayerInfoMap m_guild_question_player_info_map;								// 帮派答题 用户列表
	GuildQuestionGuildRankVec m_guild_question_guild_rankvec;									// 帮派答题 帮派积分排行
	GuilldQuestionPersonRankVec m_guild_question_person_rankvec;								// 帮派答题 个人正确答题数量排行
	GuildQuestionQuestionMap m_guild_question_question_map;										// 帮派答题 题目记录

	typedef std::map<long long, CrossLieKunFBUser> CrossLieKunFBUserMap;
	typedef std::map<long long, CrossLieKunFBUser>::iterator CrossLieKunFBUserMapIt;
	CrossLieKunFBUserMap m_cross_liekunfb_user_map;

	std::deque<CollectTreasureRollRecord> m_collect_treasure_record_deq;                        // 随机活动--聚宝盆（新） 记录

	time_t m_next_refresh_crossboss_time;														// 下一次跨服BOSS刷新的时间戳
	time_t m_next_refresh_cross_mizang_boss_time;												// 下一次跨服秘藏BOSS刷新的时间戳
	time_t m_next_refresh_cross_youming_boss_time;												// 下一次跨服幽冥BOSS刷新的时间戳
};

#endif // __ACTIVITY_SHADOW_HPP__



