#ifndef __RAND_ACTIVITY_APPEARANCE_RANK_HPP__
#define __RAND_ACTIVITY_APPEARANCE_RANK_HPP__

#include "global/randactivity/randactivity.hpp"
#include "servercommon/activitydef.hpp"
#include <set>

class RandActivitySpecialAppearanceRank : public RandActivity
{

public:
	RandActivitySpecialAppearanceRank(RandActivityManager *rand_activity_manager);
	virtual ~RandActivitySpecialAppearanceRank();

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	void UpdateRankUser(Role *role, int change_num);
	int GetRankList(RASpecialAppearacePlayerRankInfo rank_list[RA_SPECIAL_APPEARANCE_RANK_MAX_COUNT]);
	void CheckGiveRankReward();

private:
	int m_player_rank_count;												// ���а������������ٵ�����±�
	RASpecialAppearacePlayerRankInfo m_player_rank_list[RA_SPECIAL_APPEARANCE_RANK_MAX_COUNT];// ���а�����б�
};

#endif

