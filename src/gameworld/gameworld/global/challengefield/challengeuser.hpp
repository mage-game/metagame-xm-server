#ifndef __CHALLENGE_USER_HPP__
#define __CHALLENGE_USER_HPP__

#include <stdlib.h>
#include "servercommon/serverdef.h"
#include "servercommon/challengefielddef.hpp"
#include "servercommon/struct/global/challengeuserparam.hpp"

#include "servercommon/internalprotocal/gglobalprotocal.h"

#include "protocal/msgother.h"

class ChallengeField;

struct ChallengeFieldOpponentInfo
{
	ChallengeFieldOpponentInfo() : user_id(INVALID_UNIQUE_USER_ID), rank_pos(0) {}

	UniqueUserID user_id;
	int rank_pos;
};

class ChallengeUser
{
public:
	ChallengeUser(ChallengeField *challenge_field);
	~ChallengeUser();

	void Init(const ChallengeUserListParam::ChallengeUserAttr &attr);
	void GetAttr(ChallengeUserListParam::ChallengeUserAttr *attr);

	void SetRankPos(int rankpos, bool is_init, bool need_reset_opponen = false);
	int GetRankPos() const { return m_rankpos; }
	
	const UserID & GetUserID() const { return m_user_id; }
	const UniqueUserID & GetUniqueUserID() const { return m_unique_user_id; }
	const ChallengeParam & GetChallengeParam() const { return m_challenge_p; }
	const RoleAppearance & GetChallengeAppearance() const { return m_role_appearance; }
	ChallengeParam & GetParam()  { return m_challenge_p; }
	RoleAppearance & GetAppearance()  { return m_role_appearance; }
	const char * GetName() const { return m_challenge_p.role_name; }
	void GetName(GameName name) const { F_STRNCPY(name, m_challenge_p.role_name, sizeof(GameName)); }
	char GetCamp() const { return m_challenge_p.camp; }
	char GetAvatar() const { return m_challenge_p.avatar; }
	char GetSex() const { return m_challenge_p.sex; }
	char GetProf() const { return m_challenge_p.prof; }
	int GetLevel() const { return m_challenge_p.level; }
	int GetCapability() const { return m_challenge_p.capability; }
	int GetContinueWins() const { return m_common_data.continue_wins; }
	int GetBestRankPos() const { return m_common_data.best_rankpos; }
	int GetBuyJoinTimes() const { return m_common_data.buy_join_times; }
	time_t GetLastOnlineTime() const { return static_cast<time_t>(m_common_data.last_online_time); }
	void SetLastOnlineTime();

	int GetJoinTimes() { return m_common_data.join_times; }
	int GetBuyJoinTimes() { return m_common_data.buy_join_times; }
	void OnJoinChallengeField();
	void AddBuyJoinTimes(int add_value);

	void AddJifen(int add_value);
	void OnFetchJifenReward(int seq);

	void AddGuangHui(int add_value, const char *reason);
	int GetGuangHui() { return m_common_data.reward_guanghui; }

	void AddBindGold(int add_value, const char *reason);
	int GetBindGold() { return m_common_data.reward_bind_gold; }

	void OnWin(bool is_broadcast = false);
	void OnFail();

	void OnCrossWin(bool is_broadcast = false);
	void OnCrossFail();

	void AddBuyBuffTimes(int add_value);
	int GetBuyBuffTimes() { return m_common_data.buy_buff_times; }

	void AddRewradDay(int add_value);
	int GetRewardDay() { return m_common_data.reward_day; }

	void AddRewardItem(ItemID item_id, short item_num, bool is_bind);
	const ItemConfigData *GetRewardItem(int *count);
	void ClearRewardItem();

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnSyncChallengeParam(const ChallengeParam &challenge_p, const RoleAppearance &apparance);

	void ResetOpponentUserList(ChallengeFieldOpponentInfo user_list[CHALLENGE_FIELD_OPPONENT_MAX]);
	bool SetOpponentUser(int index);
	UniqueUserID GetCurrOpponentUserID();

	void SendChallengeReportInfo();
	void SendChallengeUserInfo();
	void OnGetOpponentDetailInfo();

	void AddReport(const ChallengeUserReport::ReportItem &report_item);

	bool IsRobot() { return CHALLENGE_FIELD_ROBOT_SERVER_ID == m_user_id.db_index; }

	void * operator new(size_t c);
	void operator delete(void *m);

private:
	ChallengeField *m_challenge_field;

	int m_rankpos;
	UserID m_user_id;
	UniqueUserID m_unique_user_id;

	int m_curr_opponent_idx;														// 当前对手索引(不存储)
	ChallengeFieldOpponentInfo m_opponent_info_list[CHALLENGE_FIELD_OPPONENT_MAX];	// 对手userid列表(不存储)

	ChallengeParam m_challenge_p;													// 战斗数据
	ChallengeUserCommonData m_common_data;											// 通用数据
	ChallengeUserReport m_report_info;												// 战报
	RoleAppearance m_role_appearance;												// 外观数据
};

#endif // __CHALLENGE_USER_HPP__

