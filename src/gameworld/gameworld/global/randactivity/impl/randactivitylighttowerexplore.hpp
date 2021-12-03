#ifndef __RAND_ACTIVITY_LIGHT_TOWER_EXPLORE_HPP__
#define __RAND_ACTIVITY_LIGHT_TOWER_EXPLORE_HPP__

#include "global/randactivity/randactivitymanager.hpp"

class RandActivityLightTowerExplore : public RandActivity
{
public:
	struct ActivityDataParam
	{
		ActivityDataParam() { this->Reset();}

		void Reset()
		{
			cost_total_gold = 0;
			join_uer_count = 0;
			chou_one_time_count = 0;
			chou_ten_time_count = 0;
		}

		int cost_total_gold;
		short join_uer_count;
		short chou_one_time_count;
		short chou_ten_time_count;
		short reserve_sh;
	};


	RandActivityLightTowerExplore(RandActivityManager *act_manager);
	virtual ~RandActivityLightTowerExplore();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	void AddServerDrawTimes(const int times);
	const int GetServerDrawTimes() { return m_server_draw_times; }

	ActivityDataParam *  GetActivityParam() { return &m_data_param; }

private:

	int m_server_draw_times;
	ActivityDataParam m_data_param;
};
#endif
	