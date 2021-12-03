#ifndef __SPECIALACTIVITY_COMBINE_SERVER_HPP__
#define __SPECIALACTIVITY_COMBINE_SERVER_HPP__

#include "global/specialactivity/specialactivity.hpp"

class SpecialActivityCombineServer : public SpecialActivity
{
public:
	SpecialActivityCombineServer(SpecialActivityManager *special_activity_manager);
	virtual ~SpecialActivityCombineServer();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual bool ForceSetSpecialActivityTime(unsigned int begin_time, unsigned int end_time);
	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void GetSubActivitState(int list_count, char *state_list);
	bool SubActivitIsOpening(int sub_type);

private:
	void InitSubActivityTime();
	void UpdateSubActivity(time_t now_second);
	void SubActivityFinish(int sub_type);
	void SubActivityOpen(int sub_type);

	bool m_is_init_sub_activity_time;
	time_t m_last_update_time;
	time_t m_sub_activity_begin_time_list[CSA_SUB_TYPE_MAX];
	time_t m_sub_activity_end_time_list[CSA_SUB_TYPE_MAX];

	int m_activity_day;
};

#endif // __SPECIALACTIVITY_COMBINE_SERVER_HPP__



