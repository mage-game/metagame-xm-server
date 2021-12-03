#ifndef __SPECIAL_CROSS_MULTIUSER_CHALLENGE_HPP__
#define __SPECIAL_CROSS_MULTIUSER_CHALLENGE_HPP__

#include <map>
#include <stdio.h>

#include "iclockmodule.h"

#include "scene/speciallogic/speciallogic.hpp"

#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "servercommon/serverconfig/crossmultiuserchallengeconfig.hpp"

enum CROSS_MULTIUSER_CHALLENGE_MATCH_STATE
{
	CROSS_MULTIUSER_CHALLENGE_MATCH_STATE_READY,									// 倒计时
	CROSS_MULTIUSER_CHALLENGE_MATCH_STATE_FIGHTING,									// 对战
	CROSS_MULTIUSER_CHALLENGE_MATCH_STATE_FINISH,									// 结束
};

struct ChallengeHurtInfo
{
	ChallengeHurtInfo() : obj_id(INVALID_OBJ_ID), last_hurt_time(0) {}
	ChallengeHurtInfo(ObjID _obj_id, unsigned int _last_hurt_time) : obj_id(_obj_id), last_hurt_time(_last_hurt_time) {}

	ObjID obj_id;
	unsigned int last_hurt_time;
};

struct CrossMultiUserChallengeUserInfo
{
	CrossMultiUserChallengeUserInfo() : obj_id(INVALID_OBJ_ID), prof(0), sex(0), level(0), kills(0), assist(0), 
		dead(0), occupy(0), challenge_score(0), challenge_exp(0), challenge_level(0), add_challenge_score(0), 
		add_challenge_exp(0), match_total_count(0), match_win_count(0), dur_win_count(0), mvp_count(0), add_honor(0), add_gongxun(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	bool Invaild() { return INVALID_OBJ_ID == obj_id; }

	ObjID obj_id;
	UniqueUserID unique_user_id;
	GameName user_name;
	char prof;
	char sex;
	int level;
	int kills;
	int assist;
	int dead;
	int occupy;
	int challenge_score;
	int challenge_exp;
	int challenge_level;
	int add_challenge_score;
	int add_challenge_exp;
	int match_total_count;
	int match_win_count;
	int dur_win_count;
	int mvp_count;
	int add_honor;
	int add_gongxun;

	typedef std::map<UniqueUserID, ChallengeHurtInfo> HurtMap;
	typedef std::map<UniqueUserID, ChallengeHurtInfo>::iterator HurtMapIt;

	HurtMap be_hurt_user_map;
};

class SpecialCrossMultiuserChallenge : public SpecialLogic
{
public:
	SpecialCrossMultiuserChallenge(Scene *scene);
	virtual ~SpecialCrossMultiuserChallenge();

	void OnMatchStart(crossgameprotocal::CrossGameCrossMultiuserChallengeStartMatch *cgcmcsm);

	virtual bool CanDestroy();

	virtual bool SpecialCanReAlive(Role *role, int realive_type);
	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialIsFriend(Role *role, Obj *obj, bool check_alive = true);
	virtual int GetSpecialParam(Role *role);

	bool IsMatchTwoSides(Obj *obj1, Obj *obj2, bool is_check_alive = true);

	virtual bool SpecialCanMountOn(Role *role);

	virtual long long GetSpecialParam1(Role *role);

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);

	int GetObjSide(ObjID obj_id);
	int GetRoleSide(const UniqueUserID &uuid);

	void ForceNextState();
	void SendSideMemberPosToRole(Role * role);
	virtual bool SpecialIsCostMp(Role *role) { return false; }

protected:
	bool IsReady() { return CROSS_MULTIUSER_CHALLENGE_MATCH_STATE_READY == m_state; }
	bool IsFighting() { return CROSS_MULTIUSER_CHALLENGE_MATCH_STATE_FIGHTING == m_state; }
	bool IsFinish() { return CROSS_MULTIUSER_CHALLENGE_MATCH_STATE_FINISH == m_state; }

	void OnChangeToNextState();
	void OnFinish(int win_side);

	void RefreshMultiuserChallengeSelfInfo(Role *role, CrossMultiUserChallengeUserInfo *user_info = NULL);
	void RefreshMultiuserChallengeMatchInfo(Role *role = NULL);
	void RefreshMultiuserChallengeMatchState(Role *role = NULL);

	void CheckStrongholdRateInfo();

	CrossMultiUserChallengeUserInfo * GetUserInfo(UniqueUserID original_unique_user_id);
	CrossMultiUserChallengeUserInfo * GetUserInfo(Role *role);

	CrossMultiUserChallengeUserInfo m_side_user_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX];

	void AddSideScore(int side, int add_score);
	int CalSideAverageScore(int side);
	int GetOpponentSide(int side);

// 	struct StrongHoldInfo
// 	{
// 		StrongHoldInfo() { this->Reset(); }
// 		void Reset() { gather_objid = INVALID_OBJ_ID; owner_side = -1; luolei_next_perform_time = 0; luolei_is_real_perform = false; luolei_real_pos = Posi(0, 0); cur_gather_side = -1; cur_gather_end_time = 0; }
// 
// 		ObjID gather_objid;
// 		int owner_side;
// 		unsigned int luolei_next_perform_time;
// 		bool luolei_is_real_perform;
// 		Posi luolei_real_pos;
// 		int cur_gather_side;
// 		unsigned int cur_gather_end_time;
// 	};
// 	StrongHoldInfo m_stronghold_info_list[CROSS_MULTIUSER_CHALLENGE_STRONGHOLD_NUM];// 各据点信息

	int m_side_score_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MAX];			// 各方分数
	
	
	int m_state;														// 当前比赛状态
	time_t m_next_state_time;											// 切换到下一状态的时间
	time_t m_delay_kick_time;											// 延迟踢人时间
	time_t m_return_origin_server_time;									// 通知回原服的时间
	int m_win_side;														// 结果胜利方
	UniqueUserID m_mvp_uuid;											// MVP获得者ID

	int m_stronghold_owner_side;										// 据点占领方
	int m_stronghold_rate_info;											// 据点进度条信息(初始各占50%，100%为0方占领，0%为1方占领)

	time_t m_last_stronghold_add_score_time;							// 上一次据点定时加分时间

	int m_first_enter;													// 第一个进入场景的标志
	time_t m_wait_ready_timeout;										// 等待准备超时时间戳
};

#endif // __SPECIAL_CROSS_MULTIUSER_CHALLENGE_HPP__

