#ifndef __RAND_ACTIVITY_CHONGZHI_NIU_EGG_HPP__
#define __RAND_ACTIVITY_CHONGZHI_NIU_EGG_HPP__

#include "global/randactivity/randactivity.hpp"
#include "servercommon/activitydef.hpp"

class RandActivityChongzhiNiuEgg : public RandActivity
{
public:
	RandActivityChongzhiNiuEgg(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityChongzhiNiuEgg();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);
	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	int GetServerTotalNiuEggTimes() { return m_server_total_niu_egg_times; }
	int GetNiueggChouHistoryList(RAChongzhiNiuEggHistoryItem *history_list);

	void ClearNiuEggTimes() { m_server_total_niu_egg_times = 0; }
	void AddTotalNiuEggTimes(int times);
	void AddNiuEggChouHistoryItem(Role *role, int reward_req);

private:
	int m_server_total_niu_egg_times;

	int m_history_count;													// 历史纪录数量
	int m_history_last_index;												// 历史纪录最新插入的记录下标
	RAChongzhiNiuEggHistoryItem m_history_list[RA_NIU_EGG_CHOU_HISTORY_MAX_COUNT];
};

#endif

