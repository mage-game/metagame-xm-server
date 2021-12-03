#ifndef __RAND_ACTIVITY_MAGIC_SHOP_HPP__
#define __RAND_ACTIVITY_MAGIC_SHOP_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityMagicShop : public RandActivity
{
public:
	RandActivityMagicShop(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityMagicShop();

	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
};

#endif

