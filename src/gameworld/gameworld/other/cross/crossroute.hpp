#ifndef __CROSS_ROUTE__
#define __CROSS_ROUTE__

#include <map>

#include "engineadapter.h"

#include "servercommon/servercommon.h"

#include "servercommon/crossdef.hpp"

#include "servercommon/internalprotocal/crossglobalprotocal.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"

class GlobalUser;

class CrossRoute
{
public:
	static CrossRoute & Instance();

	void OnCrossMultiuserChallengeScoreRankReward(crossgameprotocal::CrossGameMultiuserChallengeScoreRankReward *cgmcsrr);
	void OnCross1V1ScoreRankReward(crossgameprotocal::CrossGameCross1V1ScoreRankReward *cgcsrr);

private:
	CrossRoute();
	~CrossRoute();

};

#endif // __CROSS_ROUTE__

