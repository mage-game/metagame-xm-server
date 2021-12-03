#ifndef __CROSS_ACTIVITY_CROSS_MULTIUSER_CHALLENGE_HPP__
#define __CROSS_ACTIVITY_CROSS_MULTIUSER_CHALLENGE_HPP__

#include <map>

#include "engineadapter.h"

#include "servercommon/activitydef.hpp"
#include "servercommon/serverdef.h"
#include "servercommon/crossdef.hpp"

#include "crossactivity/crossactivity.hpp"

#include "servercommon/internalprotocal/crossglobalprotocal.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"

#include "crossactivity/impl/crossmultiuserchallengematchregister.hpp"


#include "protocal/msgcross.h"

struct CrossMultiuserChallengeUser
{
	CrossMultiuserChallengeUser() { this->Reset(); }

	void Reset()
	{
		unique_user_id = INVALID_UNIQUE_USER_ID;
		memset(user_name, 0, sizeof(user_name));
		camp = 0;
		sex = 0;
		prof = 0;
		level = 0;
		capability = 0;
		challenge_score = 0;
		challenge_exp = 0;
		win_percent = 0;
		challenge_level = 0;
		match_total_count = 0;
		match_win_count = 0;
		dur_win_count = 0;
		mvp_count = 0;
	}

	UniqueUserID unique_user_id;
	GameName user_name;
	char camp;
	char sex;
	char prof;
	short level;
	long long capability;
	int challenge_score;
	int challenge_exp;
	int win_percent;
	int challenge_level;
	int match_total_count;
	int match_win_count;
	int dur_win_count;
	int mvp_count;
};

struct CrossMultiuserChallengeUserRank
{
	CrossMultiuserChallengeUserRank() : count(0) {}

	int count;
	CrossMultiuserChallengeUserRankItem user_rank_list[CROSS_MULTIUSER_CHALLENGE_USER_RANK_MAX];

	typedef std::map<UniqueUserID, int> UserRankIndexMap;
	typedef std::map<UniqueUserID, int>::iterator UserRankIndexMapIt;
	UserRankIndexMap user_rank_map;

	CrossMultiuserChallengeUserRankItem * GetUserRankItem(const UniqueUserID &unique_user_id)
	{
		UserRankIndexMapIt it = user_rank_map.find(unique_user_id);
		if (it != user_rank_map.end() && it->second >= 0 && it->second < CROSS_MULTIUSER_CHALLENGE_USER_RANK_MAX)
		{
			return &user_rank_list[it->second];
		}

		return NULL;
	}
};

struct CrossMultiuserChallengeSideUserList
{
	CrossMultiuserChallengeSideUserList() : user_count(0), start_match_time(0), max_division_range(0), average_score(0), average_division(0),
	                                        average_capability(0), is_matched(false), matching_count(0) {}

	void Reset()
	{
		user_count = 0;
		start_match_time = 0;
		max_division_range = 0;
		average_score = 0;
		average_division = 0;
		average_capability = 0;
		is_matched = false;
		matching_count = 0;
		for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT; ++ i) user_list[i].Reset();
	}

	void CalcAverageScore();

	void CalcAverageCapability()
	{
		average_capability = 0;
		for (int i = 0; i < user_count && i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT; ++ i) average_capability += user_list[i].capability;
		average_capability /= (user_count > 0 ? user_count : 1);
	}

	bool AddUser(const CrossMultiuserChallengeUser &user)
	{
		if (user_count >= 0 && user_count < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT)
		{
			user_list[user_count ++] = user;
			this->CalcAverageScore();
			this->CalcAverageCapability();
			return true;
		}

		return false;
	}

	bool RemoveUser(const UniqueUserID &unique_user_id)
	{
		if (user_count <= 0 || user_count > CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT) return false;

		for (int i = 0; i < user_count && i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT; ++ i)
		{
			if (user_list[i].unique_user_id == unique_user_id)
			{
				user_list[i] = user_list[user_count - 1];
				user_list[user_count - 1].Reset();
				-- user_count;

				this->CalcAverageScore();
				this->CalcAverageCapability();

				return true;
			}
		}

		return false;
	}

	bool RemoveUserByUniqueServerID(const UniqueServerID &unique_server_id)
	{
		if (user_count <= 0 || user_count > CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT) return false;

		bool del_user = false;
		for (int i = 0; i < user_count && i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT; ++i)
		{
			UniqueServerID tmp_server_id(user_list[i].unique_user_id.plat_type, user_list[i].unique_user_id.user_id.db_index);
			if (unique_server_id == tmp_server_id)
			{
				user_list[i] = user_list[user_count - 1];
				user_list[user_count - 1].Reset();
				--user_count;
				del_user = true;
			}
		}

		this->CalcAverageScore();
		this->CalcAverageCapability();

		return del_user;
	}

	void AddUserList(const CrossMultiuserChallengeSideUserList &r)
	{
		for (int i = 0; i < r.user_count && i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT; i++)
		{
			this->AddUser(r.user_list[i]);
		}
	}

	bool HasUser(const UniqueUserID &unique_user_id)
	{
		if (INVALID_UNIQUE_USER_ID == unique_user_id) return false;

		for (int i = 0; i < user_count && i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT; ++ i)
		{
			if (user_list[i].unique_user_id == unique_user_id) return true;
		}

		return false;
	}

	CrossMultiuserChallengeUser * GetUser(const UniqueUserID &unique_user_id)
	{
		if (INVALID_UNIQUE_USER_ID == unique_user_id) return NULL;

		for (int i = 0; i < user_count && i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT; ++ i)
		{
			if (user_list[i].unique_user_id == unique_user_id) return &user_list[i];
		}

		return NULL;
	}

	int user_count;
	CrossMultiuserChallengeUser user_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT];
	time_t start_match_time;			// 开始匹配的时间
	int max_division_range;				// 允许匹配的最大段位差距（只在匹配的函数里临时赋值跟使用）

	int average_score;
	int average_division;				// 平均段位，由平均积分对应得到，不需要额外计算
	long long average_capability;
	bool is_matched;
	int matching_count;
};

class CrossActivityCrossMultiuserChallenge : public CrossActivity
{
public:
	CrossActivityCrossMultiuserChallenge(CrossActivityManager *cross_activity_manager);
	virtual ~CrossActivityCrossMultiuserChallenge();

	void Init(const CrossActivityData &data);
	void GetInitParam(CrossActivityData *data);

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnRegisterGameServer(NetID global_netid);
	virtual void OnGSDisconnect(NetID global_netid, const UniqueServerID &unique_server_id);
	virtual void OnDayChange();

	void OnWeekChange();
	void OnSeasonChange();

	void OnMatchingReq(NetID global_netid, crossgameprotocal::GameCrossCrossMuliuserChallengeMatchingReq *gccmcmr);
	void OnMatchingCancel(NetID global_netid, UniqueUserID unique_user_id);
	void OnGetMatchingState(NetID global_netid, UniqueUserID unique_user_id);

	void OnMatchStart(CrossMultiuserChallengeMatchInfo *match_info);
	void OnMatchTimeoutFinish(CrossMultiuserChallengeMatchInfo *match_info);
	void OnMatchResult(crossgameprotocal::GameCrossCrossMultiuserChallengeMatchResult *gccmcmr);
	void OnGlobalSyncRankList(crossgameprotocal::GlobalCrossMultiuserChallengeRankList *gcmcrl);
	void OnGetRankList(NetID global_netid, const UniqueUserID &unique_user_id, int rank_type);
	void OnCheckUserHasMatch(NetID global_netid, const UniqueUserID &unique_user_id);

	virtual bool CheckCanStartCross(const UniqueUserID &unique_user_id, int merge_server_id = 0);

	virtual void OnCrossUserLogin(CrossUser *cross_user);

private:
	time_t CalcActivityNextOpenTime();
	virtual void OnChangeToNextStatus();
	void OnActivityOpen();
	void OnActivityClose();
	void ClearAllData();

	bool IsUserInMatching(const UniqueUserID &unique_user_id);

	void CalcDivisionRange(std::map<long long, CrossMultiuserChallengeSideUserList> &side_user_map);
	void TeammateMatching(bool is_matching_singel_user);
	void OpponentMatching();

	// 同步最低积分到原服
	void SyncDataToGameworld();
	void UpdateUserRank(int rank_type, const CrossMultiuserChallengeUserRankItem &rank_item);

	void SendMatchingStateToUser(int matching_state, CrossMultiuserChallengeSideUserList *side_user_list, CrossMultiuserChallengeSideUserList *other_side_user_list = NULL, short notice_match = 0);
	
	void ClearMatchInfoByUniqueServerID(const UniqueServerID &unique_server_id);
	//////////////////////////////////////////////////////////////////////////

	CrossMultiuserChallengeMatchRegister m_match_register;

	typedef std::map<long long, CrossMultiuserChallengeSideUserList> UserMatchingMap;
	typedef std::map<long long, CrossMultiuserChallengeSideUserList>::iterator UserMatchingMapIt;

	UserMatchingMap m_teammate_matching_map;			// 正在匹配队友的组
	UserMatchingMap m_opponent_matching_map;			// 正在匹配对手的组

	int m_inc_num;										// 自增数
	unsigned int m_last_matching_time;					// 上一次匹配时间
	unsigned int m_last_sync_data_time;					// 上一次同步时间

	typedef std::map<UniqueUserID, int> UserSkillIDMap;
	typedef std::map<UniqueUserID, int>::iterator UserSkillIDMapIt;

	UserSkillIDMap m_user_skill_map;					// 玩家上一场选择的技能表

	CrossMultiuserChallengeUserRank m_user_rank_list[CROSS_MULTIUSER_CHALLENGE_USER_RANK_TYPE_MAX];

	int m_open_cfg_idx;

	ActivityDataParam m_data_param;
};

#endif // __CROSS_ACTIVITY_CROSS_MULTIUSER_CHALLENGE_HPP__

