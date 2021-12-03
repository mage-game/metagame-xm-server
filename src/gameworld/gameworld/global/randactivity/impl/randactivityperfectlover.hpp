#ifndef __RANDACTIVITY_PERFECT_LOVER__
#define __RANDACTIVITY_PERFECT_LOVER__

#include "global/randactivity/randactivity.hpp"

class RandActivityPerfectLover : public RandActivity
{
public:
	RandActivityPerfectLover(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityPerfectLover();

	void OnCoupleMarryRAPerfectLover(int role_id1, int role_id2);

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);
	int GetPerfectLoverCoupleNum();
	int GetPerfectLoverIdByIndex(int index);
private:

};

#endif


