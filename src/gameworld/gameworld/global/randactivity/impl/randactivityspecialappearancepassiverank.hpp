#ifndef __RAND_ACTIVITY_APPEARANCE_PASSIVE_RANK_HPP__
#define __RAND_ACTIVITY_APPEARANCE_PASSIVE_RANK_HPP__

#include "global/randactivity/randactivity.hpp"
#include "servercommon/activitydef.hpp"
#include <set>

class RandActivitySpecialAppearancePassiveRank : public RandActivity
{

public:
	RandActivitySpecialAppearancePassiveRank(RandActivityManager *rand_activity_manager);
	virtual ~RandActivitySpecialAppearancePassiveRank();

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	void UpdateRankUser(Role *role, int change_num);
	int GetRankList(RASpecialAppearacePassivePlayerRankInfo rank_list[RA_SPECIAL_APPEARANCE_PASSIVE_RANK_MAX_COUNT]);
	void CheckGiveRankReward();

private:
	int m_player_rank_count;																  // 排行榜里玩家
	RASpecialAppearacePassivePlayerRankInfo m_player_rank_list[RA_SPECIAL_APPEARANCE_PASSIVE_RANK_MAX_COUNT];// 排行榜玩家列表
};

#endif

