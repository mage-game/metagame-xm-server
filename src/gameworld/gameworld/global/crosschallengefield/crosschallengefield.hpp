#ifndef __CROSS_CHALLENGE_FIELD_HPP__
#define __CROSS_CHALLENGE_FIELD_HPP__

/************************************************************************
* 跨服竞技场
* 数据从隐藏服获取
* 数据变更同步回隐藏服
* 
* modify 2019-04-18
************************************************************************/

#include <map>
#include "servercommon/struct/global/challengeuserparam.hpp"
#include "protocal/msgactivity.hpp"

class GlobalUser;
class CrossChallengeUser;

class ChallengeUserListParam;

enum CROSS_CHALLENGE_FIELD_DATA_STATE
{	
	CROSS_CHALLENGE_FIELD_DATA_STATE_INVALID = 0,						// 初始状态
	CROSS_CHALLENGE_FIELD_DATA_STATE_LOADING,							// 数据加载中 
	CROSS_CHALLENGE_FIELD_DATA_STATE_LOAD_FINISH,						// 数据加载完成
	CROSS_CHALLENGE_FIELD_DATA_STATE_MAX,
};


class CrossChallengeField
{
public:
	static CrossChallengeField & Instance();

	bool OnServerStart();
	void OnServerStop();

	void Init();
	void GetInitParam(ChallengeFieldDataParam *data_param);

	bool IsLoadFinish() const { return (CROSS_CHALLENGE_FIELD_DATA_STATE_LOAD_FINISH == m_data_state); }

	void Update(unsigned long interval, time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void SetDirty(int rankpos);						// 排名变化

	bool IsChallengeOpen();							// 竞技场是否开启

	CrossChallengeUser * GetChallengeUser(const UniqueUserID &user_id, int *rankpos = NULL);					// 获取竞技场玩家信息
	CrossChallengeUser * GetChallengeUserByPos(int pos);														// 获取竞技场玩家信息
	void ConstructChallengeParam(Role *role,ChallengeParam * param, RoleAppearance &appearance);				// 打包竞技场玩家信息

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

	int GetChallengeUserRankPos(Role *role);																// 获取玩家排名
	void GMSetEmemySelf(Role *role, int percent);															// 设置敌人

	void OnSyncInitCrossChallengefield(crossgameprotocal::SyncCrossChallengefieldDataInfo *sccbi);			// 从跨服同步初始化数据
	void OnSyncUpdateCrossChallengefield(crossgameprotocal::SyncCrossChallengefieldDataInfo *sccbi);		// 从跨服同步更新数据

private: 
	CrossChallengeField();
	~CrossChallengeField();

	bool LoadChallengeUser();																		// 开始载入竞技场玩家数据
	void LoadChallengeUserSucc();																	// 数据加载完成

	bool InitChallengeUser(const ChallengeUserListParam::ChallengeUserAttr &user_attr);				// 初始化单个竞技场玩家

	void CheckOnlineCreateAllCrossChallengeUser();													// 创建所有竞技场角色信息
	CrossChallengeUser * CheckCreateCrossChallengeUser(Role *role);									// 检查创建竞技场角色信息

	void ResizeUserList(int size);
	void GetChangeList(ChallengeUserListParam *p);													// 获取改变列表数据
	void ClearDirtyMark(int rank_begin, int rank_end);
	
	void CheckSave(time_t now_second);																// 定时检查更新数据到隐藏服
	void AddDirtyDeque(long long unique_uid);														// 添加脏数据标记
	bool CheckIsAddDirtyDeque(long long unique_uid);												// 检查脏数据标记
	void DeleteDirtyDeque(long long unique_uid);													// 删除脏数据标记

	void OnInsertCrossChallengefieldUser(Role *role);												// 插入新玩家数据到隐藏服(暂无用)
	void OnChallengeResultToHidden(const ChallengeUserListParam::ChallengeUserAttr &user_attr, const ChallengeUserListParam::ChallengeUserAttr &target_user_attr);

	int GetFreeRankPosSlot();																		// 获取空缺排名
	
	CrossChallengeUser * CreateChallengeUser(const UniqueUserID &user_id, const ChallengeParam &challenge_p, const RoleAppearance &challenge_appearance);
	void SetChallengeUser(int rankpos, CrossChallengeUser *challenge_user, bool is_init = false, bool need_reset_opponen = false);
	bool GenrateChallengeUserParamInfo(ChallengeParam &param, RoleAppearance &appearance, int rank_index, int robot_num);  // 根据排名计算生成机器人信息

	void OnRankPosChange(UniqueUserID user_id, int rank_pos);										// 排名变化
	
	void CheckGiveLuckyReward(time_t now_second);
	void CheckGiveRankReward(time_t now_second);
	void GenrateLuckyRankPos();

	int m_data_state;

	bool m_is_first;

	int m_had_init_count = 0;						// 初始化数量
	
	int m_rankuser_num;
	CrossChallengeUser **m_rankuser_list;			// rankpos - CrossChallengeUser* 列表
	bool *m_dirty_mark;
	bool *m_old_state;
	bool m_is_change;

	time_t m_last_save_time;
	
	typedef std::map<UniqueUserID, int> RankPosMap;
	typedef std::map<UniqueUserID, int>::iterator RankPosMapIt;
	RankPosMap m_rankpos_map;						// uuid - rankpos列表

	ChallengeFieldDataParam m_challenge_field_data;
	time_t m_last_check_give_lucky_reward_timestamp;
	time_t m_last_check_give_rank_reward_timestamp;

	Protocol::SCCrossChallengeFieldRankInfo m_msg_rank_info;

	// 记录待同步到跨服的脏数据
	std::deque<long long> m_dirty_deque;
	std::set<long long> m_dirty_set;
};

#endif // __CHALLENGE_FIELD_HPP__

