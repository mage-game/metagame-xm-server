#ifndef __SPECIAL_CROSS_LOCAL_CHALLENGE_FIELD_HPP__
#define __SPECIAL_CROSS_LOCAL_CHALLENGE_FIELD_HPP__

#include <stdio.h>

#include "iclockmodule.h"

#include "scene/speciallogic/speciallogic.hpp"

#include "servercommon/challengefielddef.hpp"

class SpecialCrossLocalChallengeField : public SpecialLogic
{
public:
	SpecialCrossLocalChallengeField(Scene *scene);
	virtual ~SpecialCrossLocalChallengeField();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual bool SpecialCanRcoverHpByMedicine() { return false; }

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual void OnCharacterDie(Character *dead, ObjID killer_id);

	virtual bool SpecialBroadcastMP(Character *cha) { return true; }

	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialIsFriend(Role *role, Obj *obj, bool check_alive = true);
	virtual bool SpecialIsCostMp(Role *role) { return false; }
	virtual bool SpecialCanSupplyEffect(Role *role, int supply_type);
	virtual bool SpecialCanRecoverHp(Role *role) { return false; }
	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual bool SpecialCanCampWeekBuffEffect(Role *role) { return false; }

	void StartReadyFightCountdownReq(Role *role);  // 客户端请求开始准备

protected:
	bool IsWaiting() { return CHALLENGE_FIELD_STATE_WAITING == m_status; }
	bool IsReady() { return CHALLENGE_FIELD_STATE_READY == m_status; } 
	bool IsFighting() { return CHALLENGE_FIELD_STATE_FIGHTING == m_status; }
	bool IsFinish() { return CHALLENGE_FIELD_STATE_FINISH == m_status; }

	void BroadcastChallengeStatus();		// 广播场景玩家信息

	void ForceToNextStatus();
	void OnChangeToNextStatus();

	void OnChallengeFinish(int win_side);

	struct ChallengeSideInfo
	{
		ChallengeSideInfo() : obj_id(INVALID_OBJ_ID), user_id(INVALID_UNIQUE_USER_ID) {}

		bool Invalid() { return INVALID_OBJ_ID == obj_id; }
		
		ObjID obj_id;
		UniqueUserID user_id;
	};

	int m_status;
	time_t m_next_status_time;

	ChallengeSideInfo m_side_list[CHALLENGE_FIELD_SIDE_MAX];

	time_t m_delay_kick_time;

	time_t m_role_waiting_timeout;  // 角色进入后等待状态的超时时间

	int m_role_old_nuqi;
};

#endif // __SPECIAL_CHALLENGE_FIELD_HPP__

