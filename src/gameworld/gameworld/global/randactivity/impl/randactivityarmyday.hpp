#ifndef __RAND_ACTIVITY_ARM_DAY_HPP_
#define __RAND_ACTIVITY_ARM_DAY_HPP_

#include "global/randactivity/randactivity.hpp"
#include "servercommon/activitydef.hpp"

class RandActivityArmyDay : public RandActivity
{
public:
	RandActivityArmyDay(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityArmyDay();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);
	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	void OnAddArmyScore(int side, const int flag_count);
	void GetAllArmyScoreInfoList(int army_info_list[RA_ARMY_DAY_ARMY_SIDE_NUM]);

	void CalWinAmrySide();
	int GetWinAmrySide() { return m_win_army_side; };

private:

	int m_win_army_side;
	int m_all_army_score_list[RA_ARMY_DAY_ARMY_SIDE_NUM];
};

#endif 