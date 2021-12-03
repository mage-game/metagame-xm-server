#ifndef __GUILD_HPP__
#define __GUILD_HPP__

#include "servercommon/guilddef.hpp"
#include "global/guild/impl/guildapplyformanager.hpp"
#include "global/guild/impl/guildmembermanager.hpp"
#include "global/guild/impl/guildregister.hpp"
#include "global/guild/impl/guildmember.hpp"
#include "global/guild/impl/guildstorage.hpp"
#include "engineadapter.h"

#include "global/guild/other/guildeventmanager.hpp"
#include "config/guildconfig.hpp"
#include "gameworld/gameworld/protocal/msgfb.h"
#include <stdlib.h>
#include <set>

class GuildManager;

class Guild
{
public:
	struct MazeRankInfo
	{
		MazeRankInfo() : uid(0), maze_complete_time(0) {}
		MazeRankInfo(int _uid, time_t _maze_complete_time) : uid(_uid), maze_complete_time(_maze_complete_time) {}

		int uid;
		time_t maze_complete_time;
	};

	Guild(GuildManager *guild_manager);
	~Guild();

	GuildManager * GetGuildManager() { return m_guild_manager; }

	GuildApplyForManager & GetGuildApplyForManager() { return m_apply_for_manager; }
	GuildMemberManager & GetGuildMemberManager() { return m_member_manager; }
	GuildRegister & GetGuildRegister() { return m_register; }
	GuildEventManager & GetGuildEventManager() { return m_event_manager; }

	void Init(const AllGuildParam::GuildParam &guild_param);
	void Save();

	void OnServerStart();
	void OnServerStop();

	void OnDayChange();
	void OnWeekChange();
	void OnMonthChange();
	void Update(unsigned long interval, time_t now_second);

	GuildID GetGuildID() { return m_guild_id; }
	void GetGuildName(GuildName name) { F_STRNCPY(name, m_guild_name, sizeof(GuildName)); }
	char * GetGuildName() { return m_guild_name; }
	time_t GetGuildCreateTime() { return m_create_time; }
	int GetGuildLevel() { return m_guild_level; }
	int GetGuildExp(int *guild_max_exp = NULL);
	void GetTuanzhuangName(GameName name);
	char GetTuanzhangProf();
	char GetTuanzhangSex();
	int GetTuanzhuangRoleId();
	long long GetTuanzhuangUuid();

	int GetTotemLevel() { return m_common_data.totem_level; }
	int GetTotemExp() { return m_common_data.totem_exp; }
	int GetTotemExpToday() { return m_common_data.totem_exp_today; }
	void AddTotemExpToday(int value);

	void ClearWorldBossCurrentKillNum() { m_common_data.curr_week_worldboss_kill_num = 0; }
	void AddGuildWorldBossCurrentKillNum() { m_common_data.curr_week_worldboss_kill_num += 1; }
	void SetWorldBossLastWeekKillNum() { m_common_data.last_week_worldboss_kill_num = m_common_data.curr_week_worldboss_kill_num; }
	int GetLastWeekWorldBossKillNum() { return m_common_data.last_week_worldboss_kill_num; }
	void ChangeWorldBossCurrentKillBossTime() { m_common_data.last_kill_world_boss_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()); }
	unsigned int GetWorldBossLastKillBossTime() { return m_common_data.last_kill_world_boss_time; }

	int GetGuildMaxMemberCount();
	char GetCamp() { return m_camp; }
	void GetNotice(GuildNotice content) { STRNCPY(content, m_guild_notice, sizeof(GuildNotice)); }

	int GetApplyforSetup() { return m_applyfor_setup; }
	int GetApplyforNeedCapability() { return m_applyfor_need_capability; }
	int GetApplyforNeedLevel() { return m_applyfor_need_level; }
	bool SetApplyforSetup(int applyfor_setup, int need_capability, int need_level);
	int GetActiveDegree() { return m_active_degree; }
	short GetIsAutoClear() { return m_common_data.is_auto_clear; }
	unsigned long long GetGuildAvatar() { return m_common_data.avatar_timestamp; }
	char GetGuildAvatarChangeInfo() { return m_common_data.avatar_changed; }

	void SetAutoKickoutSetup(int aotu_kickout_setup);
	int GetAutoKickoutSetup() { return m_auto_kickout_setup; }

	void AddCallinTimes(int add_val);
	int GetCallinTimes() { return m_callin_times; }

	int GetCapability();

	bool IsGuildFull();
	void ChangeNotice(GuildNotice notice);

	void NotifyMsgToGuild(int notice_type, GuildMember *member = NULL, int notify_param = 0, int notify_param1 = 0);
	void SendMessageToGuild(int notice_type, const char *data, int len, GuildMember *member = NULL);
	void SendMessageToGuildAll(int notice_type, const char *data, int len, GuildMember *member = NULL);


	void GuildSystemMsg(int length, const char *msg, int filter_member_count = 0, const UserID *filter_member_list = NULL, int msg_type = SYS_MSG_ONLY_CHAT_GUILD);

	void OnMemberLogin(GuildMember *member, Role *role);
	void OnMemberLogout(GuildMember *member);

	void OnNewApplyFor(Role *role);

	void OnBaseInfoChange();

	int GetExp() { return m_guild_exp; }
	void AddGuildExp(int add_exp);
	bool DecGuildExp(int dec_exp);

	void AddTotemExp(int totem_exp);
	void GuildUpTotemLevel(Role *role);

	void OnMemberSos(Role *member, gglobalprotocal::GuildMemberSos *gg_gms);

	void OnResetName(GuildName new_name);

	int GetGuildMaxCapability();
	int GetRank() { return m_rank; }
	void SetRank(int rank) { m_rank = rank; }

	void * operator new(size_t c);
	void operator delete(void *m);

	void GetMonsterFinishUsername(GameName username);
	short GetMonsterFinishSeq() { return m_monster_finish_seq; }
	void OnMonsterFinish(short seq, const GameName user_name);

	void OnMemberKillWorldBoss();
	int GetKillWorldBossCount();
	void ClearKillWorldBossCount();

	int GetXianmengBipinKillWorldBoss();
	void ClearXianmengBipinKillWorldBossCount();
	int GetDayIncreaseCapability();
	void RecordYesterdayCapability();

	void SyncGuildRankDataToCross(time_t now);			// 同步数据到跨服榜

	// 仙盟工资----------------------------------------------------------------------------------
	void AddGuildGongzi(int gongzi);
	void AddMemberGongzi(int member_uid , int gongzi);

	int GetGuildGongzi() { return m_common_data.guild_total_gongzi; }
	int GetMemberGongzi(int member_uid);

	void OnGiveGongzi(Role *role);						// 发工资

	int SortMemberGongziRank();							// 排序
	void SendMemberGongziList(Role *role);				// 发送工资列表

	//仙盟试炼----------------------------------------------------------------------------------
	void StartGuildFb(Role *role);													// 开启仙盟副本
	bool IsGuildFbAlreadyOpen() { return m_common_data.guild_fb_open_times > 0; }
	bool IsGuildFbOpen(unsigned int check_second);
	void SyncGuildFbStatus(const GSNetID net_id = INVALID_GSNETID);
	void UpdateFbStatus(unsigned long interval, time_t now_second);
	void OnGuildFbFinish();
	void SetGuildFbFinishTime(unsigned int sec) { m_guild_fb_finish_timestamp = sec;}

	void SetGuildFbCurIndex(int fb_index) { m_guild_fb_cur_index = fb_index; }
	int GetGuildFbCurIndex() { return m_guild_fb_cur_index; }

	void SetGuildFbGuardHp(long long cur_hp) { m_guild_fb_guard_boss_hp = cur_hp; }
	long long GetGuldfbGuardCurhp() { return m_guild_fb_guard_boss_hp; }

	void CalcRoleHurtMonsterVal(Role *role, Attribute delta_hp);				   // 计算伤害值
	void SortHurtRank();														   // 伤害排序
	void GetRoleRankInfo(int role_id , int &rank_pos, long long &rank_val);		   // 获取玩家伤害排名
	void GetHurtRank(Protocol::SCGuildFBInfo &msg);								   // 获取全部排名
	void CheckPutHurtRankInfo(Role *role);										   // 插入玩家信息

	void AddHistoryRewardRecord(Role *role, const Protocol::SCGuildFBRewardRecordInfo::ItemInfo &reward_item);  // 添加历史奖励记录
	void GetHistoryRewardRecord(Role *role, Protocol::SCGuildFBRewardRecordInfo &msg);

	//仙盟篝火----------------------------------------------------------------------------------
	void StartGuildBonfire(Role *role);
	bool IsGuildBonfireOpen(unsigned int check_second);
	bool IsGuildBonfireAlreadyOpen() { return m_common_data.guild_bonfire_open_times > 0; }
	void SyncGuildBonfireStatus(const GSNetID net_id = INVALID_GSNETID);
	void UpdateBonfireStatus(unsigned long interval, time_t now_second);

	int GetMemberLuckyColor(const UserID &user_id);
	bool CanDismiss(Role *role);
	bool CanQuit(Role *role);

	void SyncGulidReliveTimesInfo(Role *role = NULL);
	void SyncGuildBeastStatus(const GSNetID net_id = INVALID_GSNETID);
	void StartGuildBeast(Role *role);
	void ResetBeastFinishTime() { m_guild_beast_finish_timestamp = 0; }
	void SetXianmengzhanRankValue(long long rank_value);
	void ClearXianmengzhanRankValue();
	long long GetXianmengzhanRankValue() { return m_common_data.xianmengzhan_rank_value; }

	void SetGuildBattleRank(int rank);
	void ClearGuildBattleRank();
	int GetGuildBattleRank() { return m_common_data.guildbattle_rank; }

	void SetTerritoryWarRank(int rank);
	void ClearTerritoryWarRank();
	int GetTerritoryWarRank() { return m_common_data.territorywar_rank; }

	void OnUplevelBoss(Role *role);
	void CallGuildBoss(Role *role, bool is_super_boss);
	void SyncGuildBossInfo(const GSNetID net_id = INVALID_GSNETID);

	GuildStorage *GetStorge() { return &m_guild_storge; }

	bool IsGuildStationHasActivity();

	GuildCommonDataParam * GetGuildCommonDataParam(){ return &m_common_data;}

	void OnMemberKillMonter(Role *role, int monster_id);
	void OnMemberFetchRedbag(Role *role, int gold_num);
	void SendBossRedbagInfo(Role *role, int index);

	bool IsRoleSyncRedPaperInfo(int uid) { return m_guild_red_paper_set.find(uid) != m_guild_red_paper_set.end();}
	void AddSyncRedPaperMember(int uid) { m_guild_red_paper_set.insert(uid);}
	bool CanCreateGuildPaper(Role *role, int red_paper_index);
	void ClearRedPaperByIndex(Role *role ,int red_paper_index);
	void RemoveMemberSyncPaperInfo(int uid);
	void AddGuildRedPaper(Role *role, int red_paper_seq, int red_paper_state, int red_paper_id, unsigned int invalid_time);
	void SignalChatGuildPaperRole(Role *role, int red_paper_index);
	void GetGuildRedPaperList(Role *role, Protocol::SCGuildRedPaperListInfo *cmd);

	struct QuestionRankItem
	{
		QuestionRankItem() : uid(0), right_answer_num(0), last_num_change_time(0)
		{
		}

		QuestionRankItem(int _uid, int _right_answer_num, unsigned int _last_num_change_time) :
			uid(_uid), right_answer_num(_right_answer_num), last_num_change_time(_last_num_change_time)
		{
		}

		int uid;
		int right_answer_num;
		unsigned int last_num_change_time;
	};

	void CalcNextQuestionTime();
	void BrocastGuildQuestion(Role *role);
	void OnAnswerGuildQuestion(Role *role, int choose);
	void AddQuestionRankItem(Role *role, int right_answer_num, unsigned int change_timshamp);
	void CalcQuestionRankReward();
	void SendGuildQuestionRankInfo(Role *role);
	void RemoveMemberSyncQuestionRankInfo(int uid);

	void OnGetMazeInfo(Role *role);
	void OnMazeSelectDoor(Role *role, int door_id);

	void SetAutoClear(Role *role, short is_auto_clear);
	void AutoClearGuildMember(unsigned int now_sec);

	void OnMemberEnterGuildBattle(Role *role);
	void ClearGuildBattleJoinFlag();

	void SetJoinGuildBattleFlag(int flag) { m_common_data.join_guild_battle_flag = flag; }
	int GetJoinGuildBattleFlag() { return m_common_data.join_guild_battle_flag; }

	//new guild battle
	void SetJoinNewGuildBattleFlag(int flag) { m_common_data.join_new_guild_battle_flag = flag; }
	int GetJoinNewGuildBattleFlag() { return m_common_data.join_new_guild_battle_flag; }

	void SendBattleWinReward(int scene_index, int scene_id);
	void SendBattleJoinReward();

	// 镖车---------------------------------------------------------
	bool IsBiaoCheHadStart() { return 0 != m_today_biaoche_isstart; } // 是否开启过
	bool IsHadBiaoChe();		// 是否开启中
	bool IsBiaoCheHadEnd();		// 是否开启过或已结束
	void SetOpenBiaoCheUID(int role_id) { m_open_biaoche_role_id = role_id; }
	void SetBiaoCheStart() { m_today_biaoche_isstart = 1; }
	void AddRobBiaoCheCount() { ++m_today_rob_count; }
	int GetRobBiaoCheCount() { return m_today_rob_count; }

	void OnYunBiaoStatus(gglobalprotocal::GuildYunBiaoStatus *gybs);
	void OnYunBiaoComplete(gglobalprotocal::GuildYunBiaoStatus *gybs);
	void OnYunBiaoFail(gglobalprotocal::GuildYunBiaoStatus *gybs);
	void OnYunBiaoActivityEndFail();										// 活动结束强制失败
	void OnTransToBiaoCheReq(Role *role);
	void OnGetBiaoChePos(Role *role);

	void OnRewardYunBiaoFail(GuildName rob_guild_name);									// 运镖失败奖励
	void SendYunBiaoRewardMail(const MailContentParam &contentparam, Guild *guild);		// 发送邮件给在运镖场景的同仙盟id玩家

	// 签到---------------------------------------------------------
	void OnSignin(Role *role);
	void FethSinginReward(Role *role, int index);
	void SendSigninAllInfo(Role *role);

	// 天赐铜币---------------------------------------------------------
	bool IsTianCiTongBiOpen(Role *role);
	int  OnTianCiTongBiReq(GuildMember *member);
	void SetTianCiTongBiOpentatus(bool parameter);
	int GetTianCiTongBiDayCount() { return m_guild_tianci_tongbi_day_count; }
	void OnGuildTianCiTongBiOpenReq(const UserID & user_id, Protocol::CSGuildTianCiTongBiReq * gtctbq);

	void AddMemberKillCrossBoss(UserID uid,int value);
	int GetKillCrossBossNum() { return m_today_kill_cross_boss_num; }
	void ClearKillCrossBossNum() { m_today_kill_cross_boss_num = 0; }

	//跨服目标仙盟目标
	void AddCrossGoalkillCrossBoss(UserID uid, int value);
	int GetCrossGoalKillCrossBoss() { return m_cross_goal_kill_cross_boss; }

	void AddCrossGoalkillBaiZhanBoss(UserID uid, int value);
	int GetCrossGoalKillBaiZhanBoss() { return m_cross_goal_kill_baizhan_boss; }

	int GetGuildCrossGoalFlag() { return m_cross_goal_guild_flag;}
	void CheckGuildCrossGoalFinish(int cond_type);
	void CheckCrossGoalGuildNotify(unsigned int now_second);

	// 击杀BOSS仙盟榜---------------------------------------------------------
	void AddGuildKillBossNum(int num);
	int GetGuildKillBossNum() { return m_common_data.guild_kill_boss_num; }
	void ResetGuildKillBossNum() { m_common_data.guild_kill_boss_num = 0; }

	// 仙盟仇人 ------------------------------------------------------------------------------------
	void InitGuildEnemy(const GuildBaseParam *param);
	void SaveGuildEnemy(GuildBaseParam *param);
	void AddGuildEnemy(const GuildEnemyInfo &enemy_info);
	void RemoveGuildEnemy(const UniqueUserID &unqiue_user_id);  // 删除仇人，如果玩家进入该仙盟则需要删除他，如果进盟时不在线则尝试登录时删除
	void SortRankGuildEnemy();								// 排序
	void SendGuildEnemy(Role *role, long long uuid = 0);
	// ------------------------------------------------------------------------------------

private:
	void SetLastActiveTime(time_t last_active_time);
	void OnLevelChange(bool is_upgrade);
	void ResetMaze();
	void RecalcMazeRank();
	void CheckMazeRankReward();
	void SendMazeInfo(Role *role, int reason = 0);
	void SendMazeRankInfo(Role *role);

	GuildManager *m_guild_manager;
	
	GuildID m_guild_id;													// 军团ID
	char m_camp;														// 所属阵营
	GuildName m_guild_name;												// 军团名字
	GuildNotice m_guild_notice;											// 军团公告
	unsigned int m_create_time;											// 军团创建时间

	int m_guild_level;													// 军团等级
	int m_guild_exp;													// 军团经验 建设度
	unsigned int m_guild_last_uplevel_timestamp;						// 最后仙盟升级的时间戳

	unsigned int m_last_update_dayid;									// 最后update的日期
	time_t m_last_active_time;											// 最后活跃时间
	time_t m_next_get_cabablity_time;									// 下一次刷新战力时间
	time_t m_next_clear_member_time;									// 下一次清理公会成员时间
	time_t m_next_sync_to_cross_time;									// 下一次上传仙盟击杀数量时间
	int m_warning_dismiss_days;											// 解散警告天数
	int m_applyfor_setup;												// 申请设置
	int m_applyfor_need_capability;										// 申请战力限制
	int m_applyfor_need_level;											// 申请等级限制
	int m_auto_kickout_setup;											// 自动踢人设置
	int m_callin_times;													// 仙盟召唤次数

	bool m_is_change;													// 数据是否改变	
	unsigned int m_next_save_time;										// 下次保存时间

	unsigned int m_next_check_auto_kickout_time;						// 下次踢人时间

	int m_guild_max_capability;											// 仙盟历史最高战斗力
	int m_rank;															// 仙盟排名

	short m_monster_finish_seq;											// 仙盟今天完成的神兽最高序号
	GameName m_monster_finish_username;									// 仙盟神兽今天完成的最高序号的角色名

	int m_active_degree;												// 仙盟活跃度

	unsigned int m_guild_fb_finish_timestamp;							// 仙盟副本完成时间 不存档
	unsigned int m_guild_beast_finish_timestamp;						// 仙盟凶兽完成时间 不存档
	unsigned int m_guild_bonfire_finish_timestamp;						// 仙盟篝火完成时间 不存档

	GuildCommonDataParam m_common_data;									// 通用数据

	GuildApplyForManager m_apply_for_manager;							// 申请管理
	GuildMemberManager m_member_manager;								// 成员管理
	GuildRegister m_register;											// 临时寄存器
	GuildEventManager m_event_manager;									// 日志管理
	GuildStorage m_guild_storge;										// 仙盟仓库

	std::vector<Protocol::FetchUserInfo> m_daily_fetch_user_info;		// 当天成员领取红包的记录

	unsigned int m_next_send_saizi_rank_msg;							// 下次发送骰子积分排名信息的时间

	int m_question_id;													// 公会答题-题目id
	int m_question_answer;												// 公会答题-题目答案
	unsigned int m_question_cur_begin;									// 公会答题-当前题开始时间
	unsigned int m_question_cur_end;									// 公会答题-当前题结束时间
	unsigned int m_question_next_begin;									// 公会答题-下一题开始时间
	int m_question_cur_right_num;										// 公会答题-当前回答正确人数
	int m_question_fast_uid[3];											// 公会答题-前3名玩家

	int m_open_biaoche_role_id;											// 今天开启镖车的玩家id
	int m_today_biaoche_isstart;										// 今天的镖车是否运送
	int m_today_rob_count;												// 今天劫镖的次数
	int m_biaoche_status;												// 镖车的状态
	Posi m_biaoche_pos;													// 镖车的坐标

	std::vector<QuestionRankItem> m_question_rank_vec;

	struct GuildRedPaperItem
	{
		GuildRedPaperItem():role_uid(0), red_paper_state(RED_PAPER_INVALID), red_paper_seq(0), 
			red_paper_id(0), create_time(0), invalid_time(0), red_paper_index(-1), avator_timestamp(0),
			sex(0), prof(0), reserve(0)
		{
			memset(role_name, 0, sizeof(role_name));
		}

		int				role_uid;
		GameName		role_name;
		short			red_paper_state;
		short			red_paper_seq;
		int				red_paper_id;
		unsigned int	create_time;
		unsigned int	invalid_time;
		int				red_paper_index;
		long long		avator_timestamp;
		char			sex;
		char			prof;
		short			reserve;
	};
	GuildRedPaperItem m_guild_red_paper_list[MAX_GUILD_RED_PAPER];		// 公会红包列表
	std::set<int> m_guild_red_paper_set;

	std::vector<MazeRankInfo> m_maze_rank_vec;

	std::vector<MemberGongziInfo> m_gongzi_rank_vec;						// 仙盟工资列表

	// 天赐铜币---------------------------------------------------------
	int m_guild_tianci_tongbi_day_count;									// 仙盟天赐铜币次数
	bool m_guild_tianci_tongbi_is_open;										// 天赐铜币是否开启

	// 仙盟试炼---------------------------------------------------------
	int m_guild_fb_cur_index = -1;											// 仙盟试炼当前关卡
	long long m_guild_fb_guard_boss_hp = 0;									// 仙盟试炼守卫当前血量

	int m_today_kill_cross_boss_num;											//神域boss榜仙盟击杀神域boss数

	std::vector<HurtMonsterInfo> m_hurt_monster_info_vec;					// 对怪伤害排名信息
	std::map<int, std::vector<Protocol::SCGuildFBRewardRecordInfo::ItemInfo> > m_history_reward_record_map;	 // 历史奖励记录表 uid - reward_vec

	int m_cross_goal_kill_cross_boss;										//跨服目标仙盟击杀神域boss数
	int m_cross_goal_kill_baizhan_boss;										//跨服目标仙盟击杀百战boss数
	int m_cross_goal_guild_flag;											//跨服目标完成目标标志

	bool m_enemy_list_dirty;
	std::vector<GuildEnemyInfo> m_enemy_list;								// 仙盟仇人信息

	bool m_today_cross_goal_guild_notify;									//跨服目标仙盟目标提示
};

#endif // __GUILD_HPP__

