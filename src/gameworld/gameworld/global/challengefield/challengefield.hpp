#ifndef __CHALLENGE_FIELD_HPP__
#define __CHALLENGE_FIELD_HPP__

#include <map>
#include "servercommon/struct/global/challengeuserparam.hpp"
#include "protocal/msgactivity.hpp"

class GlobalUser;
class ChallengeUser;

class ChallengeUserListParam;

enum CHALLENGE_FIELD_DATA_STATE
{	
	CHALLENGE_FIELD_DATA_STATE_INVALID = 0,						// 初始状态
	CHALLENGE_FIELD_DATA_STATE_LOADING,							// 数据加载中 
	CHALLENGE_FIELD_DATA_STATE_LOAD_FINISH,						// 数据加载完成
	CHALLENGE_FIELD_DATA_STATE_MAX,
};

enum RANDOM_CAPABILITY											// 匹配战力限制倍数
{
	CAPABILITY_MULTIPE_1 = 2,								
	CAPABILITY_MULTIPE_2 = 3,
	CAPABILITY_MULTIPE_3 = 4,
	CAPABILITY_MULTIPE_4 = 5,

	CAPABILITY_NUM = CAPABILITY_MULTIPE_4 - CAPABILITY_MULTIPE_1 + 1,
};


class ChallengeField
{
	friend class RMIInitChallengeUserObjectImpl;

public:
	static ChallengeField & Instance();

	bool OnServerStart();
	void OnServerStop();

	void Init(const ChallengeFieldDataParam &data_param);
	void GetInitParam(ChallengeFieldDataParam *data_param);

	bool IsLoadFinish() const { return (CHALLENGE_FIELD_DATA_STATE_LOAD_FINISH == m_data_state); }

	void Update(unsigned long interval, time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void SetDirty(int rankpos);						// 排名变化

	bool IsChallengeOpen();							// 竞技场是否开启

	int GetEndRankRewardOpenday();					// 获取竞技场结束时的开服天数

	ChallengeUser * GetChallengeUser(const UniqueUserID &user_id, int *rankpos = NULL);					// 获取竞技场玩家信息
	ChallengeUser * GetChallengeUserByPos(int pos);													// 获取竞技场玩家信息
	void ConstructChallengeParam(Role *role,ChallengeParam * param, RoleAppearance &appearance);    // 获取竞技场玩家信息

	void OnUserLevelChange(Role *role);
	void OnUserLogin(Role *role);
	void OnUserLogout(Role *role);

	void OnSyncChallengeParam(Role *role);															// 同步竞技场玩家信息
	void OnChallengeResult(UniqueUserID sponsor_userid, UniqueUserID opponent_userid, bool is_sponsor_win);		// 挑战结果
	void OnGetChallengeUserInfo(Role *role);														// 下发自己竞技场信息

	void OnResetOpponentReq(Role *role);																	// 刷新对手请求
	void OnFightReq(Role *role, int opponent_idx, bool ignore_rank_pos, int rank_pos, bool buy_join_times); // 战斗请求
	void OnBuyJoinTimes(Role *role);																		// 购买挑战次数
	void OnFetchJifenReq(Role *role, int seq);																// 领取积分请求
	void OnGetRankInfo(Role *role);																			// 获取前100名信息
	void OnGetLuckyInfo(Role *role);																		// 暂屏蔽
	void OnFetchGuangHui(Role *role);																		// 领取奖励
	void OnGetOpponentDetailInfo(Role *role, bool is_get_top);												// 获取对手信息
	void OnChallengeFieldBuyBuff(Role *role);																// 购买buff请求
	void RandOpponentHelper(UniqueUserID user_id);															// 设置竞技场对手

	int GetRoleRewardRankPos(int uid);																		// 获取玩家排名
	int GetChallengeUserRankPos(Role *role);																// 获取玩家排名
	void GMSetEmemySelf(Role *role, int percent);															// 设置敌人

	///////////////////////////////////// 跨服竞技场 ////////////////////////////////////////////
	bool IsCanSyncCrossChallengeUserInfo();																	// 能否进行数据同步
	void OnSyncChallengeUserInitDataToGame(NetID net_id);													// 下发初始数据给游戏服
	void OnSyncOrignalChallengefieldInfo(crossgameprotocal::SyncCrossChallengefieldDataInfo *sccbi);		// 从游戏服更新数据
	void OnSyncChallengefieldRankInfo(crossgameprotocal::SyncCrossChallengefieldDataInfo *sccbi);			// 游戏服更新排名变更数据
	void OnUpdateChallengefieldInfoToGame();																// 把更新数据同步到游戏服
	void CheckGiveCrossRankReward();																		// 结算排行奖励

	void CrossRandOpponentHelper(UniqueUserID user_id);


private: 
	ChallengeField();
	~ChallengeField();

	bool LoadChallengeUser(int rank_from);
	void LoadChallengeUserSucc();
	bool InitChallengeUser(const ChallengeUserListParam::ChallengeUserAttr &user_attr);

	void ResizeUserList(int size);
	void GetChangeList(ChallengeUserListParam *p, int rank_begin, int *rank_end) const;
	void ClearDirtyMark(int rank_begin, int rank_end);
	
	void CheckSave(time_t now_second);

	int GetFreeRankPosSlot();					// 获取空缺排名
	
	ChallengeUser * CreateChallengeUser(const UniqueUserID &user_id, const ChallengeParam &challenge_p, const RoleAppearance &challenge_appearance);
	void SetChallengeUser(int rankpos, ChallengeUser *challenge_user, bool is_init = false, bool need_reset_opponen = false);
	bool GenrateChallengeUserParamInfo(ChallengeParam &param, RoleAppearance &appearance, int rank_index, int robot_num);  // 根据排名计算生成机器人信息

	void OnRankPosChange(UniqueUserID user_id, int rank_pos);
	void CheckGiveLuckyReward(time_t now_second);
	void CheckGiveRankReward(time_t now_second);
	void GenrateLuckyRankPos();
	bool randLimitCap(int rand_interval, int begin_pos, int challenge_cap, int& get_pos);

	///////////////////////////////////// 跨服竞技场 ////////////////////////////////////////////
	bool GenrateCrossChallengeUserParamInfo(ChallengeParam &param, RoleAppearance &appearance, int rank_index, int robot_num);  // 根据排名计算生成机器人信息
	void ResetCrossChallengeUserRank();																							// 结算后重置排名
	void SwapCrossChallengeUserRank();																							// 重新生成排名
	void SwapChallengeUserRank(ChallengeUser *challenge_user_1, ChallengeUser *challenge_user_2);								// 互换排名

	int m_data_state;

	bool m_is_first;
	
	int m_rankuser_num;
	ChallengeUser **m_rankuser_list;
	bool *m_dirty_mark;
	bool *m_old_state;
	bool m_is_change;

	time_t m_last_save_time;
	
	typedef std::map<UniqueUserID, int> RankPosMap;
	typedef std::map<UniqueUserID, int>::iterator RankPosMapIt;
	RankPosMap m_rankpos_map;

	ChallengeFieldDataParam m_challenge_field_data;
	time_t m_last_check_give_lucky_reward_timestamp;
	time_t m_last_check_give_rank_reward_timestamp;

	Protocol::SCChallengeFieldRankInfo m_msg_rank_info;
};

#endif // __CHALLENGE_FIELD_HPP__

