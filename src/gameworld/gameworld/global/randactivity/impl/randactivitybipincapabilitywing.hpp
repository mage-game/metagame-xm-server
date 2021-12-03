#ifndef __RAND_ACTIVITY_BIPIN_CAPABILITY_WING_HPP__
#define __RAND_ACTIVITY_BIPIN_CAPABILITY_WING_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityBipinCapabilityWing : public RandActivity
{
public:
	RandActivityBipinCapabilityWing(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityBipinCapabilityWing();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);
	virtual void OnPersonRankSnapshot(int old_top_uid_list[PERSON_RANK_TYPE_MAX], int new_top_uid_list[PERSON_RANK_TYPE_MAX]);
	virtual bool IsSyncRankData() { return 0 != m_data_param.need_give_reward; }
	virtual void RankBipinHistory(PersonRankItem top_user_list[Protocol::TOP_COUNT]);

private:
	void CheckGiveRankReward();

	struct ActivityDataParam
	{
		ActivityDataParam() : need_give_reward(0), reserve_ch(0), reserve_sh(0)
		{
			memset(role_id_list, 0, sizeof(role_id_list));
			memset(rank_value_list, 0, sizeof(rank_value_list));
		}

		char need_give_reward;
		char reserve_ch;
		short reserve_sh;
		int role_id_list[Protocol::TOP_COUNT];
		long long rank_value_list[Protocol::TOP_COUNT];
	};

	ActivityDataParam m_data_param;

	time_t m_check_rank_reward_timestamp;
};

#endif

