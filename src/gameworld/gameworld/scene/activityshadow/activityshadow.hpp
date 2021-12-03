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
	void OnSyncCrossActivityStatus(int cross_type, crossgameprotocal::CrossGameSyncActivityStatus *cgsas);  // ͬ������״̬
	
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

	//���1v1-----------------------------------------------------------------------------------------------------
	CrossActivity1v1FightUnit* GetCross1v1FightMatchUnit(long long uuid);					// ��ȡƥ����Ϣ
	void ClearRoleMatchInfo(long long uuid);												// ���ƥ����Ϣ
	void OnCross1v1MatchInfo(crossgameprotocal::CrossGameSyncCross1v1MatchInfo *cgscmi);	// ͬ��ƥ����Ϣ
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

	//�������ս-----------------------------------------------------------------------------------------------------
	CrossGuildBattleUserInfo * GetCrossGuildBattleUser(Role * role);
	void OnAddGuildBattleRoleScore(Role *role, int score);
	void OnAddRoleScore(Role *role, int score);				// �����Ҹ��˻�����Ϣ
	void SortRoleScoreRank();								// ��������
	void SyncRoleTopToGame();								// ͬ����ԭ��
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
	void SendResultInGuildChat(); // ����ʤ���������������
	

	// �-���ɴ���-------------------------------------------------------------------------------------------------
	void OnGuildQuestionEnterSceneReq(Role *role);
	void OnJoinGuildQuestionActivity(Role *role);
	bool IsRoleInGuildQuestionActivityState(Role *role);
	bool IsCanGatherGuildQuestion(Role *role);
	GuildQuestionPlayerInfo * GetGuildQuestionPlayerInfo(const UserID &user_id, const GameName user_name);
	bool OnGuildQuestionAnswerQuestion(Role *role, const char* buff, int total_len);	// ��������
	void SendGuildQuestionQuestionInfo(GuildID guild_id = 0, Role *role = NULL);		// ���Ͱ��ɵ���Ŀ��Ϣ
	void GuildQuestionQuickSetQuestion(GuildID guild_id);								// ���ٳ���(�����ѿ�ʼ���⣬�ٴ�������ʱ)
	void AddGuildQuestionInfo(Role *role);												// ��Ӱ��ɴ�����Ϣ
	// �������ش�
	CrossLieKunFBUser * GetCrossLieKunFBUser(long long uuid);
	void ClearCrossLieKunFBUserGatherTimes(int zone, int boss_type);

	// ����--�۱���---------------------------------------------------------------------------------
	void AddRACollectTreasureRecord(CollectTreasureRollRecord record_item);
	void GetRACollectTreasureRecord(CollectTreasureRollRecord record_list[RAND_ACTIVITY_COLLECT_TREASURE_MAX_RECORD_COUNT], short &count);

	//�����ƴ
	bool GetImageCompetitionRankInfo(Role *role, int *rank_value, long long *flexible_ll);

	// ͨ�õĿ��������SOS����
	void OnCrossGuildSOSReq(Role * role, Protocol::CSSendGuildSosReq *sgsr);

	// ���ɱ-�������˳���
	void OnCrossAddGuildEnemy(Role *be_killer, Role *killer);
	void OnGetGuildEnemyListFromOriginal(Role *role);

private:
	void OnZhuxieClose();	
	void OnMoShenJiangLinOpen();																// ħ����
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

//����ʱװ��������������㼣
	void OnRaUpgradeShizhuangRankClose();
	void OnRaUpgradeShenbinRankClose();
	void OnRaUpgradeFabaoRankClose();
	void OnRaUpgradeFootprintRankClose();
	void OnRaUpgradeFlypetRankClose();
	void OnRaUpgradeWeiyanRankClose();

	void OnGuaJiActivityOpen();																	// ���˹һ�
	void OnGuaJiActivityClose();
	void CheckGuaJiActivityReward(time_t now_second);

	void SendCSASubActivityState(Role *role);
	void OnCrossActivityStatusChange(int cross_activity_type, int from_status, int to_status, unsigned int end_time = 0);
	bool CheckSyncCrossActivityStatusBefore(int cross_activity_type, int to_status);			// ����Ƿ����ͬ������Ļ״̬

	void ShenzhouWeaponCalcNextFleshGatherTime(time_t now);
	void ShenzhouWeaponReFreshGathers();

	// �-���ɴ���-------------------------------------------------------------------------------------------------
	void GuildQuestionRefreshGuildScore(GuildID guild_id, unsigned int now_second);				// �������ɻ��ָı�
	int GetGuildQuestionGuildScore(GuildID guild_id);											// �õ��������ɵĻ���
	int GetGuildQuestionGuildRank(GuildID guild_id);											// �õ�������������
	void SendGuildQuestionGuildScoreRankInfo(Role *role = NULL);								// ���ͻ�������
	void SendGuildQuestionPlayerInfo(Role *role, bool is_answer = false);
	void RewardAllGuildQuestionGuildOnEnd();
	void BroadcastGuildquestionEnd(const GuildQuestionQuestion &question_info);					// ��Ⱥ��ϵͳ����

	void OnGuildQuestionActivityStandy();
	void OnGuildQuestionActivityStart();
	void OnGuildQuestionActivityClose();
	void OnGuildQuestionActivityUpdate();

	// ���BOSS
	void CheckCrossBossRefreshNotice(time_t now);												// �������BOSSˢ������

	//�����ƴ����
	void OnImageCompetitionAccount();
	void SendImageCompetitionInfo(Role *role = nullptr);
	void SyncCrossActivityStatusToUser(int cross_activity_type, Role *user = NULL);

	ActivityShadow();
	~ActivityShadow();

	ActivityStatus m_activity_status[ACTIVITY_TYPE_MAX];										// �״̬
	ActivityStatus m_special_activity_status[SPECIAL_ACTIVITY_TYPE_MAX];						// ����״̬
	ActivityStatus m_rand_activity_status[RAND_ACTIVITY_TYPE_MAX];								// ����״̬
	ActivityStatus m_cross_activity_status[CROSS_ACTIVITY_TYPE_MAX];							// ����״̬
	ActivityStatus m_cross_rand_activity_status[CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT];			// �������״̬

	SpecialZhuXieUserMap m_zhuxie_usermap;														// ��аս�����û��б� 

	time_t m_next_monster_invade_time;															// ��һ����������ʱ��

	unsigned int m_next_millionaire_refresh_box_time;											// �󸻺���һ������ˢ��ʱ��
	unsigned int m_next_millionaire_refresh_boss_time;											// �󸻺�bossˢ��ʱ��

	int m_moshenjianglin_boss_id;																// ħ����bossid

	time_t m_guaji_reward_next_timetamp;														// �һ��

	char m_csa_sub_activity_state_list[CSA_SUB_TYPE_MAX];										// �Ϸ���ӻ״̬

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

	//int m_xiuluotower_server_daily_drop_title_num_limit;		// ������ÿ�յ���ƺ���������
	//int m_xiuluotower_server_daily_drop_mount_num_limit;		// ������ÿ�յ���������������
	//int m_xiuluotower_server_daily_gold_box_num_limit;			// ������ÿ�ս����ӽ�����������

	int m_xiuluotower_server_daily_drop_title_num;				// ������ÿ�յ���ƺ�����
	int m_xiuluotower_server_daily_drop_mount_num;				// ������ÿ�յ�����������
	int m_xiuluotower_server_daily_gold_box_num;				// ������ÿ�ս����ӽ�������
	int m_guild_battle_daily_drop_wing_num;						// �������ս�����������
	
	// �-���ɴ���-------------------------------------------------------------------------------------------------
	int m_question_state;																		// ������ǰ״̬
	unsigned int m_next_question_state_time;													// �����´�״̬ʱ��
	int m_question_index;																		// ��ǰ��Ŀ�±�
	int m_question_id;																			// ��ĿID
	int m_question_answer;																		// ��Ŀ��
	int m_time_count;                                                                           // ÿ��10�����һ��ʱ��
	int m_guild_question_state;																	// ���ɴ��� ���ɴ���״̬
	time_t m_next_give_reward_timestamp;														// ���ɴ��� �´ν���ʱ���
	time_t m_guild_question_state_change_timestamp;												// ���ɴ��� ����״̬�л���ʱ���

	QuestionUserMap m_question_usermap;															// ���� �û��б� 
	QuestionRankUserVec m_question_rankvec;														// ���� �����б�
	std::set<int> m_question_id_set;															// ���δ�����Ŀ�б�

	GuildQuestionPlayerInfoMap m_guild_question_player_info_map;								// ���ɴ��� �û��б�
	GuildQuestionGuildRankVec m_guild_question_guild_rankvec;									// ���ɴ��� ���ɻ�������
	GuilldQuestionPersonRankVec m_guild_question_person_rankvec;								// ���ɴ��� ������ȷ������������
	GuildQuestionQuestionMap m_guild_question_question_map;										// ���ɴ��� ��Ŀ��¼

	typedef std::map<long long, CrossLieKunFBUser> CrossLieKunFBUserMap;
	typedef std::map<long long, CrossLieKunFBUser>::iterator CrossLieKunFBUserMapIt;
	CrossLieKunFBUserMap m_cross_liekunfb_user_map;

	std::deque<CollectTreasureRollRecord> m_collect_treasure_record_deq;                        // ����--�۱��裨�£� ��¼

	time_t m_next_refresh_crossboss_time;														// ��һ�ο��BOSSˢ�µ�ʱ���
	time_t m_next_refresh_cross_mizang_boss_time;												// ��һ�ο���ز�BOSSˢ�µ�ʱ���
	time_t m_next_refresh_cross_youming_boss_time;												// ��һ�ο����ڤBOSSˢ�µ�ʱ���
};

#endif // __ACTIVITY_SHADOW_HPP__



