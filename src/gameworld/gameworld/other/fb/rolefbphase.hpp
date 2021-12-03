#ifndef __ROLE_PHASE_FB_HPP__
#define __ROLE_PHASE_FB_HPP__

#include "servercommon/fbdef.hpp"

class Role;

class RolePhaseFB
{
public:
	RolePhaseFB();
	~RolePhaseFB();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const PhaseFbParam &phase_fb_param);
	void GetInitParam(PhaseFbParam *phase_fb_param);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	bool CanEnter(int fb_index);
	void OnEnterFB(int fb_index);
	void AddFBTodayTimes(int fb_index);
	void DecFBTodayTimes(int fb_index);
	void BuyFBTimes(int fb_index);

	void AutoFBReq(int fb_index, int fb_level);
	void SendInfo();

	int GetCurrFbIndex() { return m_param.curr_fb_index; }
	int GetCurrFbLevel() { return m_param.curr_fb_level; }
	void OnPassLevel(int fb_index, int fb_level);
	bool IsPassLevel(int fb_index);
	char GetPassLevel(int fb_index);  // 获取已经通关了第几层

private:
	void OnJoinFb(int fb_type);

	Role *m_role;

	PhaseFbParam m_param;
};

#endif // __ROLE_PHASE_FB_HPP__


