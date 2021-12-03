#ifndef __RAND_ACTIVITY_MINE_HPP__
#define __RAND_ACTIVITY_MINE_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityMine : public RandActivity
{
public:
	RandActivityMine(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityMine();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);

	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	int GetTotalRefreshTimes() { return m_data_param.cur_total_refresh_times; }
	int GetRewardBeginIndex() { return m_data_param.reward_begin_index; }
	int GetRewardEndIndex() { return m_data_param.reward_end_index; }
	int GetRefreshBeginIndex() { return m_data_param.refresh_begin_index; }
	int GetRefreshEndIndex() { return m_data_param.refresh_end_index; }
	void AddTotalRefreshTimes();
	bool IsPreDayClearRefreshTimes() { return (0 != m_data_param.is_pre_day_clear_refresh_times); }

private:
	struct ActivityDataParam
	{
		ActivityDataParam() : cur_total_refresh_times(0), is_pre_day_clear_refresh_times(0), reward_begin_index(0), reward_end_index(0), 
			refresh_begin_index(0), refresh_end_index(0)
		{}

		int cur_total_refresh_times;			// ��ǰȫ���������
		int is_pre_day_clear_refresh_times;		// ǰһ���Ƿ������ȫ���������
		short reward_begin_index;				// ������ʼ�±�
		short reward_end_index;					// ���������±�
		short refresh_begin_index;				// ˢ����ʼ�±�
		short refresh_end_index;				// ˢ�½����±�
	};

	ActivityDataParam m_data_param;
};

#endif

