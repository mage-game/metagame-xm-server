#ifndef __RAND_ACTIVITY_MARRY_ME_HPP__
#define __RAND_ACTIVITY_MARRY_ME_HPP__

#include "global/randactivity/randactivity.hpp"

class Role;

class RandActivityMarryMe : public RandActivity
{
public:
	RandActivityMarryMe(RandActivityManager *rand_activity_manager);

	virtual ~RandActivityMarryMe();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	bool OnRoleAcceptProposal(Role *role, Role *propose_role);
	bool IsRewardCountLimit();

	int GetCoupleInfoList(CoupleInfo couple_list[RA_MARRY_SHOW_COUPLE_COUNT_MAX]);

protected:
	struct ActivityDataParam
	{	
		ActivityDataParam()
		{
			couple_count = 0;
			memset(oga_show_couple_list, 0, sizeof(oga_show_couple_list));
		}

		int couple_count;
		CoupleInfo oga_show_couple_list[RA_MARRY_SHOW_COUPLE_COUNT_MAX];
	};

	ActivityDataParam m_data_param;
};

#endif	//__RAND_ACTIVITY_MARRY_ME_HPP__
