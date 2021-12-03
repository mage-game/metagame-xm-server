#ifndef _SPECIAL_CROSS_HUSONG_SHUIJING_HPP_
#define _SPECIAL_CROSS_HUSONG_SHUIJING_HPP_

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"

class Role;

enum HUSONG_SJ_STATUS
{
	HUSONG_SJ_STATUS_NONE = 0,	// 没护送
	HUSONG_SJ_STATUS_HS_FULL,   // 满载护送中
	HUSONG_SJ_STATUS_HS_LOST,	// 损失护送中
	HUSONG_SJ_STATUS_MAX,
};

struct HusongShuijingControlParam
{
	HusongShuijingControlParam() : last_refresh_gather_times(0), cur_remain_gather_times(0), next_try_refresh_time(0), gather_objid(0),gather_id(0)
	{}

	int last_refresh_gather_times;
	int cur_remain_gather_times;
	UInt32 next_try_refresh_time;

	ObjID gather_objid;
	int gather_id;
};

struct HusongShuijingUser
{
	HusongShuijingUser():user_id(INVALID_UNIQUE_USER_ID),hssj_vip_buy_times(0),hssj_rob_count(0), hssj_gather_times(0),hssj_commit_count(0),
		hssj_husong_type(-1),hssj_husong_status(HUSONG_SJ_STATUS_NONE),hssj_invalid_time(0){}

	UniqueUserID user_id;
	short hssj_vip_buy_times;				// 购买次数
	short hssj_rob_count;					// 截取成功次数
	short hssj_gather_times;				// 采集次数
	unsigned short hssj_commit_count;		// 护送提交次数
	short hssj_husong_type;					// 护送类型
	short hssj_husong_status;				// 护送状态
	UInt32 hssj_invalid_time;			   // 护送的失效时间
	
};

class SpecialCrossHusongShuijing : public SpecialLogic
{
public:
	SpecialCrossHusongShuijing(Scene *scene);
	virtual ~SpecialCrossHusongShuijing();

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
	virtual void OnCharacterDie(Character *dead_cha, ObjID killer_objid);

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialCanMountOn(Role *role);

	virtual void OnAddObj(Obj *obj);

	virtual bool SpecialStopGahterOnAttack(Role *role) { return false; }
	virtual bool SpecialCanGather(Role *role, GatherObj *gather);
	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);

	virtual bool CanDestroy() { return m_finish; }
	virtual int GetSpecialParam(Role *role);

	int GetRemainGatherTimes(ObjID obj_id);

	bool IsHusongStatus(Role *role);
	void OnActivityStatusChange(int to_status);

	void OnCommit(Role *role);
	void SendRoleInfo(Role * role);

	void SyncShuijingInfo(Role * role = NULL, int reason = Protocol::CS_CROSS_HUSONG_SHUIJING_GATHER_DEFAULT);
	void SyncShuijingInfo(long long uid);
	void GmReq(int parm1, int parm2);
protected:
	HusongShuijingUser * GetHusongShuiJingUser(Role * role);
	void OnRoleHusongStatusChange(Role *role, int status, int type);
	void CheckBuffStatus(Role *role, bool is_add);
	void OnHusongTimeout(Role *role);
	void OnActivityStandy();
	void OnActivityOpen();
	void OnActivityClose();

	void CheckJingHuaHusongTime(Role *role);
	void NotifyHusongStatusInfo(Role *role);

	void StartGenerater();
	void StopGenerater();
	void RefreshGatherInfo( int gather_id);
	
private:

	bool m_isopen;
	bool m_finish;
	typedef std::map<UniqueUserID, HusongShuijingUser> HusongShuijingUserMap;
	HusongShuijingUserMap m_user_map;

	std::vector<HusongShuijingControlParam> m_refresh_control_vec;
};


#endif // _SPECIAL_CROSS_1V1_HPP_

