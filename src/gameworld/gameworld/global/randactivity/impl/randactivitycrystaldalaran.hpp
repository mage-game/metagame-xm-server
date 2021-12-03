#ifndef __RANDACTIVITY_CRYSTAL_DALARAN_HPP__
#define __RANDACTIVITY_CRYSTAL_DALARAN_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityCrytalDalaran : public RandActivity
{
public:
	RandActivityCrytalDalaran(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityCrytalDalaran();

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);	
};

#endif //__RANDACTIVITY_CRYSTAL_DALARAN_HPP__