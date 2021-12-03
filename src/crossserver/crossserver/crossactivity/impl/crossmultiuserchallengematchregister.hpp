#ifndef __CROSS_MULTIUSER_CHALLENGE_MATCH_REGISTER_HPP__
#define __CROSS_MULTIUSER_CHALLENGE_MATCH_REGISTER_HPP__

#include <map>

#include "servercommon/activitydef.hpp"
#include "servercommon/serverdef.h"
#include "servercommon/crossdef.hpp"

enum CROSS_MULTIUSER_CHALLENGE_MATCH_STATUS
{
	CROSS_MULTIUSER_CHALLENGE_MATCH_STATUS_INVALID,
	CROSS_MULTIUSER_CHALLENGE_MATCH_STATUS_READY,					// 3v3比赛准备开始
	CROSS_MULTIUSER_CHALLENGE_MATCH_STATUS_GOING,					// 3v3比赛进行中
	CROSS_MULTIUSER_CHALLENGE_MATCH_STATUS_FINISH,					// 3v3比赛完成
	CROSS_MULTIUSER_CHALLENGE_MATCH_STATUS_MAX
};

// 对战双方比赛信息
struct CrossMultiuserChallengeMatchInfo
{
	CrossMultiuserChallengeMatchInfo() : key(0), create_time(0), status(CROSS_MULTIUSER_CHALLENGE_MATCH_STATUS_INVALID), next_status_time(0), win_side(-1) 
	{
		memset(capability_list, 0, sizeof(capability_list));
	}

	int GetSide(const UniqueUserID &unique_user_id)
	{
		for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; ++ i)
		{
			if (unique_user_id_list[i] == unique_user_id)
			{
				UNSTD_STATIC_CHECK(CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT > 0);
				return i / CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT;
			}
		}

		return -1;
	}

	int key;																				// key (场景key，是一个自增值)
	time_t create_time;																		// 创建时间
	int status;																				// 当前状态
	time_t next_status_time;																// 下一状态时间
	UniqueUserID unique_user_id_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX];		// 双方ID列表
	long long capability_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX];	// 战斗力列表
	int win_side;																			// 胜利方
};

typedef std::map<int, CrossMultiuserChallengeMatchInfo> CrossMultiuserChallengeMatchMap;
typedef std::map<int, CrossMultiuserChallengeMatchInfo>::iterator CrossMultiuserChallengeMatchMapIt;

struct CrossMultiuserChallengeSideUserList;
class CrossActivityCrossMultiuserChallenge;

class CrossMultiuserChallengeMatchRegister
{
public:
	CrossMultiuserChallengeMatchRegister();
	~CrossMultiuserChallengeMatchRegister();

	void SetCrossMultiuserChallengeActivity(CrossActivityCrossMultiuserChallenge *cross_activity_multiuser_challenge) { m_cross_activity_multiuser_challenge = cross_activity_multiuser_challenge; }

	void Clear();

	void Update(unsigned long interval, time_t now_second);

	CrossMultiuserChallengeMatchInfo * CreateMatch(CrossMultiuserChallengeSideUserList *side_user_list, CrossMultiuserChallengeSideUserList *other_side_user_list);
	CrossMultiuserChallengeMatchInfo * GetMatch(int key);
	CrossMultiuserChallengeMatchInfo * GetMatch(const UniqueUserID &unique_user_id);
	void RemoveMatch(int key);

	CrossMultiuserChallengeMatchMap & GetCrossMultiuserChallengeMatchMap() { return m_match_map; }

private:
	void OnMatchStatusChange(CrossMultiuserChallengeMatchInfo *match_info);

	CrossActivityCrossMultiuserChallenge *m_cross_activity_multiuser_challenge;

	int m_inc_key;

	CrossMultiuserChallengeMatchMap m_match_map;	// <场景key, 对战双方比赛信息>

	typedef std::map<UniqueUserID, int> UserMatchMap;
	typedef std::map<UniqueUserID, int>::iterator UserMatchMapIt;
	UserMatchMap m_user_match_map;
};

#endif // __CROSS_MULTIUSER_CHALLENGE_MATCH_REGISTER_HPP__

