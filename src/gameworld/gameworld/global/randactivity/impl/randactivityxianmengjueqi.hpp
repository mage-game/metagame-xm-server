#ifndef __RANDACTIVITY_XIANMENG_JUEQI_HPP__
#define __RANDACTIVITY_XIANMENG_JUEQI_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityXianmengJueQi : public RandActivity
{
public:
	RandActivityXianmengJueQi(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityXianmengJueQi();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);
	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

private:
	struct ActivityDataParam
	{
		ActivityDataParam() : need_send_reward_flag(0), reserve_ch(0), reserve_sh(0) {}
		
		char need_send_reward_flag;
		char reserve_ch;
		short reserve_sh;
	};

	ActivityDataParam m_data_param;
};

#endif


