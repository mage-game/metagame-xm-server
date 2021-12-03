#ifndef __ROLE_FB_CHALLENGE_HPP__
#define __ROLE_FB_CHALLENGE_HPP__

class Role;

#include "servercommon/fbdef.hpp"

class RoleFBChallenge
{
public:
	RoleFBChallenge();
	~RoleFBChallenge();

	enum CHALLENGE_FB_STATE
	{
		NOT_FIGHT = 0,
		CAN_RESET,
		HAD_RESET,
		END_TODAY,
	};

	enum CHALLENGE_FB_REWARD
	{
		CHALLENGE_FB_REWARD_NOT = 0,
		CHALLENGE_FB_REWARD_ONE_STAR,
		CHALLENGE_FB_REWARD_TWO_STAR,
		CHALLENGE_FB_REWARD_THREE_STAR,
	};

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const ChallengeFBParam &param);
	void GetInitParam(ChallengeFBParam *param);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void OnFBFinish(bool is_passed, int scene_id, bool is_active_leave_fb, unsigned int start_time);
	
	void OnOperateReq(int type, int param);
	void SendInfo(short is_auto = 0);
	void AutoFBReq(int level);
	void ResetLevel(int level);
	void BuyFBTimes();
	bool CanEnter(int level);
	int GetFightLayer(int level);
	int GetPassLevel();									// 获取已通关关卡
	void OnEnterFB(int level);
	void SyncLevelInfo(int scene_id);
	int GetEnterCount();
	

private:
	Role *m_role;
	ChallengeFBParam m_param;
};

#endif // __ROLE_FB_CHALLENGE_HPP__

