#ifndef __RAND_ACTIVITY_CHARGE_REPAYMENT_HPP__
#define __RAND_ACTIVITY_CHARGE_REPAYMENT_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityChargeRepayment : public RandActivity
{
public:
	RandActivityChargeRepayment(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityChargeRepayment();
};

#endif

