#ifndef __RAND_ACTIVITY_TOTAL_CHARGE_4_HPP__
#define __RAND_ACTIVITY_TOTAL_CHARGE_4_HPP__

#include "global/randactivity/randactivity.hpp"
#include "global/randactivity/randactivitymanager.hpp"


const static int MAX_REWARD = 500;                                     //全服奖励份数
class RandActivityTotalCharge4 : public RandActivity
{
public:
	RandActivityTotalCharge4(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityTotalCharge4();


	virtual void Init(const RandActivityData &data) override;
	virtual void GetInitParam(RandActivityData *data) override;
	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid) override;
	void OnUseReward();
	short GetRemainReward();

	//static RandActivity *GetCharge4();


private:
	struct ChargeRandActivityDataParam
	{
		ChargeRandActivityDataParam() :activity_reward_remain(0)
		{

		}
		short activity_reward_remain ;                                 //活动奖励剩余
	};
	                                         
	ChargeRandActivityDataParam m_charge4_data_param;
};

#endif

