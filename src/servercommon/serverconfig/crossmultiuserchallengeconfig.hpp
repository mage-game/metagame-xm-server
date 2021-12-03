#ifndef __CROSS_MUTIUSER_CHALLENGE_CONFIG_H__
#define __CROSS_MUTIUSER_CHALLENGE_CONFIG_H__

#include "servercommon/crossdef.hpp"
#include "servercommon/activitydef.hpp"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct CrossMultiUserChallengeActivityTimeInterval
{
	CrossMultiUserChallengeActivityTimeInterval() : open_day_second(0), dur_s(0) {}

	int open_day_second;		// �����ʱ�� ��0�������
	int dur_s;					// ����ʱ�� ����
};

struct CrossMultiUserChallengeActivityTimeCfg
{
	CrossMultiUserChallengeActivityTimeCfg() : is_open(false), open_day_second(0), standby_dur_s(0), open_dur_s(0) 
	{
		memset(open_day_list, 0, sizeof(open_day_list));
	}

	bool is_open;
	bool open_day_list[CROSS_ACTIVITY_1V1_MAX_WEEK_DAYS];
	int open_day_second;			// �����ʱ�� ��0�������
	int standby_dur_s;				// ׼���׶γ���ʱ�� ����
	int open_dur_s;					// ���������ʱ�� ����
};

struct CrossMultiuserChallengeScoreRankReward
{
	CrossMultiuserChallengeScoreRankReward() : rank_pos_min(-1), rank_pos_max(-1), reward_honor(0) {}

	bool Invalid() const { return (rank_pos_min < 0 || rank_pos_max < 0); }

	int rank_pos_min;
	int rank_pos_max;
	int reward_honor;
	ItemConfigData item;
};

class CrossMultiUserChallengeConfig
{
public:
	static CrossMultiUserChallengeConfig & Instance();

	bool Init(const std::string &path, std::string *err);

	CrossMultiUserChallengeActivityTimeCfg * GetActivityTimeCfg() { return &m_activity_time_cfg; }
	int GetActivityStandbyTime() { return m_activity_time_cfg.standby_dur_s; }
	int GetActivityOpenTime() { return m_activity_time_cfg.open_dur_s; }

	int GetJoinMinLevel() const { return m_join_min_level; }
	int GetJoinMinCapability() const { return m_join_min_capability; }
	int GetJoinDayCountLimit() const { return m_join_day_count_limit; }

	int GetMatchSceneID() const { return m_match_scene_id; }
	Posi GetRelivePos(int side) const;

	int GetFinishMatchScore() { return m_finish_match_score; }

	int GetKillAddScore() { return m_kill_add_score; }
	int GetOccupyAddScore() { return m_occupy_add_score; }
	int GetStrongholdAddScore() { return m_stronghold_add_score; }
	int GetStrongholdGatherId() { return m_stronghold_gather_id; }
	Posi GetStrongholdPos(int stronghold_index);

	int GetWinAddHonor() { return m_win_add_honor; }
	int GetLoseAddHonor() { return m_lose_add_honor; }
	int GetDrawAddHonor() { return m_draw_add_honor; }
	int GetMvpAddHonor() { return m_mvp_add_honor; }
	int GetDayRewardHonor() { return m_day_reward_honor; }

	int GetRewardDayCount() { return m_reward_daycount; }
	bool GetDaycountReward(ItemConfigData *reward_item, int *reward_shengwang);
	bool GetChallengeScoreRankReward(int rank_pos, int *reward_honor);

private:
	CrossMultiUserChallengeConfig();
	~CrossMultiUserChallengeConfig();

	CrossMultiUserChallengeActivityTimeCfg m_activity_time_cfg;		// �ʱ������

	int m_join_min_level;											// ������С�ȼ�
	int m_join_min_capability;										// ������Сս��
	int m_join_day_count_limit;										// ����ÿ�����ƴ���

	int m_match_scene_id;											// ��������ID
	Posi m_relive_pos_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MAX];		// ������б�

	int m_finish_match_score;										// ��ǰ������������

	int m_kill_add_score;											// ɱ�˼ӷ�
	int m_occupy_add_score;											// ռ��ӷ�
	int m_stronghold_add_score;										// �ݵ�ӷ�
	int m_stronghold_gather_id;										// �ݵ�ɼ�ID

	int m_win_add_honor;											// ��ʤ����
	int m_lose_add_honor;											// ʧ������
	int m_draw_add_honor;											// ��ƽ����
	int m_mvp_add_honor;											// MVP����
	int m_day_reward_honor;											// ÿ����������

	Posi m_stronghold_pos_list[CROSS_MULTIUSER_CHALLENGE_STRONGHOLD_NUM];	// �ݵ�����

	CrossMultiuserChallengeScoreRankReward m_challenge_score_rank_reward_list[CROSS_MULTIUSER_CHALLENGE_SCORE_RANK_REWARD_MAX_COUNT];	// �������н����б�

	int m_reward_daycount;											// ����Ŀ�곡��
	int m_daycount_reward_shengwang;								// ��������
	ItemConfigData m_daycount_reward_item;							// ������Ʒ
};

#endif	// __CROSS_MUTIUSER_CHALLENGE_CONFIG_H__

