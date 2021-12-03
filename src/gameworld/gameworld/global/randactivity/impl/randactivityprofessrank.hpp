#ifndef __RAND_ACTIVITY_PROFESS_RANK_HPP__
#define __RAND_ACTIVITY_PROFESS_RANK_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityProfessRank : public RandActivity
{
public:
	RandActivityProfessRank(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityProfessRank();

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);
};

#endif//__RAND_ACTIVITY_PROFESS_RANK_HPP__