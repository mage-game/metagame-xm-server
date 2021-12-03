#ifndef __WORLD_STATUS_HPP__
#define __WORLD_STATUS_HPP__

class Role;

#include "iclockmodule.h"
#include "inetworkmodule.h"
#include "servercommon/struct/global/worldstatusparam.hpp"

enum WORLDSTATE_DATA_STATUS
{			
	WORLDSTATE_DATA_STATUS_INVALID = 0,						// 初始状态
	WORLDSTATE_DATA_STATUS_LOADING,							// 数据加载中 
	WORLDSTATE_DATA_STATUS_LOAD_FINISH,						// 数据加载完成 
	WORLDSTATE_DATA_STATUS_MAX,
};

static const int WORLDSTATUS_SAVE_INTERVAL = 30;			// 半分钟回写一次

static const unsigned int WORLDSTATUS_FAKE_CHONGZHI_MSG_MIN_INTERVAL = 30;	// 假首冲传闻最小时间间隔
static const int WORLDSTATUS_FAKE_CHONGZHI_MSG_ROLE_NUM = 15;				// 假首冲传闻每隔多少个新增发送

class WorldStatus
{
	friend class RMIInitWorldStatusBackObjectImpl;

public:
	static WorldStatus & Instance();

	void Init(const WorldStatusParam &worldstatus_param);

	void OnServerStart();
	void OnServerStop();

	void OnEnterScene(Role *role);
	void Update(unsigned long interval, time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	bool IsLoadFinish() const;

	unsigned int GetLastUpdateDayId();
	void SetLastUpdateDayId(unsigned int dayid);

	unsigned int GetLastResetRoleModuleDataDayId() { return m_common_data.last_reset_role_module_dayid; }
	void SetLastResetRoleModuleDataDayId(unsigned int dayid) { m_common_data.last_reset_role_module_dayid = dayid; }
	unsigned int GetLastResetRoleModuleDataTimestamp() { return m_common_data.last_reset_role_modlue_timestamp; }
	void SetLastResetRoleModuleDataTimestamp(unsigned int timestamp) { m_common_data.last_reset_role_modlue_timestamp = timestamp; }

	unsigned int GetStartTime() { return m_start_time; }

	void OnRecordActivityClose(int activity_type);
	void OnActivityFinish(int activity_find_type);
	ActivityOpenRecords & GetActivityOpenRecords() { return m_activity_open_record; }

	void SetChallengeFieldKingUserId(long long king_userid);
	long long GetChallengeFieldKingUserId() { return m_challengefield_king_userid; }

	void SetXianmengzhanMengzhu(int mengzhu_id);
	int GetXianmengzhanMengZhu() { return m_xianmengzhan_mengzhu_id; }

	void SetYiZhanDaoDiKingUserId(int user_id,int rank);
	void ClearYiZhanDaoUserTitle();
	int GetYiZhanDaoDiKingUserId() { return m_common_data.yizhandaodi_win_user_id; }
	void SendYizhanDaoDiKingUserId(Role *role);

	void SetQunxianlundouSideFirstUser(int side_first_user_list[], int list_len);
	void SendQuanxianLuandouSideFirstUser(Role *role);

	WorldStatusCommonDataParam & GetCommonData() { return m_common_data; }
	void OnXunbao(int xunbao_times);
	void OnUpgradeVipLevel(int vip_level);
	void OnBuyMonthCard();

	void OnSyncGCZInfo(int wing_guild, int chengzhu_id);
	void OnSyncGBMenzhuInfo(GuildID guild_id);

	void OnSyncQingyuanCapTop(int male_uid, int female_uid);
	void OnSyncBabyCapTop(int male_uid, int female_uid);
	void OnSyncLittePetCapTop(int male_uid, int female_uid);

	int GetGCZWinGuild() { return m_common_data.gcz_win_guild; }
	int GetGCZChengzhuID() { return m_common_data.gcz_chengzhu_id; }
	char GetGCZChengzhuProf();
	GuildID GetGBMengzhuGuildID() { return m_common_data.guildbattle_mengzhu_guild_id; }
	int GetGCZSeriesWinTimes() { return m_common_data.gcz_series_win; }

	void SendGongChengZhangOwnerInfo(Role *role = NULL);
	void SynActivityTitleInfo(Role *role, int reason);
	void SynOtherTitleInfo(Role *role, int reason);

	int GetCampExistsDiaoXiangNum(int camp);
	int GetCampDiaoXiangStatus(int index);
	void SetCampDiaoXiangStatus(int index, int result);
	void ResetCampDiaoXiangStatus(int camp);

	int GetOpenServerQunxianID() { return m_common_data.open_server_qunxian_uid; }
	int GetOpenServerGuildBattelID() { return m_common_data.open_server_guildbatte_uid; }
	int GetOpenServerGCZID() { return m_common_data.open_server_gongchengzhan_uid; }
	int GetOpenServerTerritoryWarID() { return m_common_data.open_server_territorywar_uid; }

	void SetOpenServerQunxianID(int uid) { m_common_data.open_server_qunxian_uid = uid; }
	void SetOpenServerGuildBattelID(int uid) { m_common_data.open_server_guildbatte_uid = uid; }
	void SetOpenServerGCZID(int uid) { m_common_data.open_server_gongchengzhan_uid = uid; }
	void SetOpenServerTerritoryWarID(int uid) { m_common_data.open_server_territorywar_uid = uid; }

	int GetFamousManID(int famous_type);
	void SetFamousManID(int famous_type, int uid);

	int GetFreeCreateGuildTimes(){return m_common_data.free_create_guild_times;}
	void OnFakeFirstChongzhiSystemMsg();
	
	void AddChestShopRecord(Role *role , ItemID item_id, int chest_shop_type);
	void SendChestShopRecord(Role *role, int chest_shop_type);

	void  GmSetNextQuestionTime();
	void CalcNextQuestionTime();
	void BrocastWorldQuestion(Role *role);
	void OnAnswerWorldQuestion(Role *role, int choose);
	long long GetGoldTurntablePool() { return m_common_data.gold_turntable_pool; }
	void SetGoldTurntablePool(long long gold_turntable_pool) { m_common_data.gold_turntable_pool = gold_turntable_pool; }
	void AddGoldTurntablePool(long long gold_turntable_pool) { m_common_data.gold_turntable_pool += gold_turntable_pool; }

	void GetCrossGuildBattleInfo(CrossGuildBattleGuildItem cross_guildbattle_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM]);
	void GetCrossGuildBattleTuanzhangInfo(CrossGuildBattleTuanzhangInfo cross_guildbattle_tuanzhang_info_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM]);
	void GetCrossGuildBattleApperanceInfo(CrossGuildBattleApperanceInfo cross_guildbattle_apperance_info_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM]);
	int GetCrossGuildBattleAverageLevel();
	void SetCrossGuildBattleAverageLevel(int level);
	int GetCrossGuildBattleRewardCalctime();
	void ResetCrossGuildBattleInfo();
	void SetCrossGuildBattleInfo(CrossGuildBattleGuildItem cross_guildbattle_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM]);
	void SetCrossGuildBattleTuanzhangInfo(CrossGuildBattleTuanzhangInfo cross_guildbattle_tuanzhang_info_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM]);
	void SetCrossGuildBattleApperanceInfo(CrossGuildBattleApperanceInfo cross_guildbattle_apperance_info_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM]);
	void OnRoleJoinDarkNight(int role_level);
	int GetDarkNightAverageLevel() { return m_common_data.total_join_darknight_average_level; }
	void SetCrossPastureTopRankUid(Int32 rank, Int32 uid);

	void SetCrossFishingTopRankUid(Int32 rank, Int32 uid);

	int GetTotalUpgradeMountRecordList(int grade);		//获取坐骑进阶人数
	int GetTotalUpgradeHaloRecordList(int grade);		//获取光环进阶人数
	int GetTotalUpgradeWingRecordList(int grade);		//获取羽翼进阶人数
	int GetTotalUpgradeShengongRecordList(int grade);	//获取仙女光环进阶人数
	int GetTotalUpgradeShenyiRecordList(int grade);		//获取仙女法阵进阶人数

	void IncTotalUpgradeMountRecordList(int grade);		//坐骑进阶人数+1
	void IncTotalUpgradeHaloRecordList(int grade);		//光环进阶人数+1
	void IncTotalUpgradeWingRecordList(int grade);		//羽翼进阶人数+1
	void IncTotalUpgradeShengongRecordList(int grade);	//仙女光环进阶人数+1
	void IncTotalUpgradeShenyiRecordList(int grade);	//仙女法阵进阶人数+1

	// 聊天限制--------------------------------------------------------
	const int GetChatLimitConditionType(const int chat_type);
	const int GetChatLimitConditionTypeFlag();
	void CmdSetChatLimitConditionType(const int chat_type, const int condition_type);

	const int GetChannelLevelLimit(short channel_type);
	bool GetChannelLevelLimit(int *level_list, int list_len);
	bool CmdSetChannelLevelLimit(short channel_type, short level);

	const short GetChannelVipLevelLimit(short channel_type);
	bool GetChannelVipLevelLimit(short *vip_level_list, int list_len);
	bool CmdSetChannelVipLevelLimit(short channel_type, short vip_level);

	bool CheckSatisfyLevel(Role *role, int channel_type);	// 检查是否满足等级限制

	int GetAudioChatForbidFlag() { return m_common_data.is_forbid_audio_chat; }
	void CmdSetAudioChatForbidFlag(int audio_chat_forbit_flag) { m_common_data.is_forbid_audio_chat = audio_chat_forbit_flag; }

	// 后台玩家数调整--------------------------------------------------------
	void CmdSetServerRoleNum(const int server_role_num);
	int GetRefreshTimeInterval(const int refresh_time_interval);

	int GetCurAdjustServerRoleNum() { return m_common_data.adjust_server_role_num; }

	// ----------------------------------------------------------------

	// 仙盟工资
	void AddGuildGongziByMonster(Role *role, int monster_id);

	// 跨服仙盟击杀榜
	void AddGuildKillBossNum(UniqueUserID unique_uid, int scene_type);

	// 随机活动全局数据
	int GetTodayFirstChongzhiCount() { return m_common_data.today_chongzhi_role_count; }
	void AddTodayFirstChongzhiCount(int num);

	CrossCloudPurchaseParam & GetCrossCloudPurchaseParam() { return m_cross_cloudpurchase_param; }
	void SyncKillCrossBossRankGuild(int mengzhu, GuildID guild_id);
	int GetKillCrossBossRankMengzhu();
	GuildID GetKillCrossBossRankGuild();

	// 获取首冲团购假人数
	int GetTodayFirstChongzhiFakeCount() { return m_first_charge_fake_num; }
private:
	WorldStatus();
	~WorldStatus();

	bool LoadWorldStatus(long long id_fram);
	void LoadWorldStatusSucc();

	bool SaveWorldStatus(); 

	int m_data_state; 

	unsigned int m_start_time;							
	int m_world_time;									
	unsigned int m_last_update_dayid;	

	ActivityOpenRecords m_activity_open_record;
	WorldStatusCommonDataParam m_common_data;

	int m_xianmengzhan_mengzhu_id;
	long long m_challengefield_king_userid;					// 不存档

	int m_question_id;									// 世界答题-题目id
	int m_question_answer;								// 世界答题-题目答案
	unsigned int m_question_cur_begin;					// 世界答题-当前题开始时间
	unsigned int m_question_cur_end;					// 世界答题-当前题结束时间
	unsigned int m_question_next_begin;					// 世界答题-下一题开始时间
	int m_question_cur_right_num;						// 世界答题-当前回答正确人数
	int m_question_fast_uid[3];							// 世界答题-前3名玩家

	CrossGuildBattleParam m_cross_guildbattle_param;

	CrossCloudPurchaseParam m_cross_cloudpurchase_param;

	unsigned int m_chongzhi_count_next_refresh_time;		//用于强制设置首冲团购（2136）充值人数
	bool m_can_refresh_chongzhi_count;						//用于强制设置首冲团购（2136）充值人数

	int m_first_charge_fake_num;							//首冲团购假充值人数(不会累加，仅用于显示)
};

#endif // __WORLD_STATUS_HPP__

