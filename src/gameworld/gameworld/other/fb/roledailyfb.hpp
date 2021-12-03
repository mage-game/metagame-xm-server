#ifndef __DAILY_FB_HPP__
#define __DAILY_FB_HPP__

class Role;

#include "servercommon/fbdef.hpp"
#include "dailyfbconfig.hpp"

class RoleDailyFb
{
public:
	RoleDailyFb();
	~RoleDailyFb();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const DailyFbParam &param);
	void GetInitParam(DailyFbParam *param);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	bool CheckEenterCD();
	bool CanEnter(int dailyfb_type);
	void OnDailyFbFinish(int dailyfb_type, int pass_time, bool is_pass);
	bool AutoFBReq(int dailyfb_type);
	void EnterDailyFb(int dailyfb_type);		//Ä£Äâ½øÈë
	void AutoFBReward();

	void AddExpFbTodayPayTimes() { ++m_fb_param.expfb_today_pay_times; }
	short GetExpFbHistoryEnterTimes() { return m_fb_param.expfb_history_enter_times; }

	long long GetExpFbLastExp() { return m_fb_param.expfb_last_exp; }
	void SetExpFbLastExp(long long exp);

	long long GetExpFbRecordMaxExp() { return m_fb_param.expfb_record_max_exp; }
	void SetExpFbRecordMaxExp(long long exp);

	void SetLastQuitFbTime(int quit_time) { m_fb_param.last_quit_fb_time = quit_time; }

	void OnEnterExpFb();
	void OnPayFbTimes();
	void SendAllInfo();

private:
	Role *m_role;
	DailyFbParam m_fb_param;
};

#endif
