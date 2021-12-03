#ifndef _SPECIAL_CROSS_1V1_HPP_
#define _SPECIAL_CROSS_1V1_HPP_

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"
#include <set>

class Role;

class SpecialCross1v1 : public SpecialLogic
{
public:
	SpecialCross1v1(Scene *scene);
	virtual ~SpecialCross1v1();

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
	virtual void OnCharacterDie(Character *dead_cha, ObjID killer_objid);

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialCanMountOn(Role *role);
	virtual bool SpecialCanPerformSkill(Role *role);

	virtual long long GetSpecialParam1(Role *role);

	virtual bool CanDestroy();
	
	void OnReady(Role *role);															// 设置已准备状态
	void OnSetFighterInfo(long long uuid1, long long uuid2);							// 设置战斗信息
	virtual bool SpecialCanSupplyEffect(Role *role, int supply_type);
	virtual bool SpecialIsCostMp(Role *role) { return false; }

protected:
	Role * GetRole(int index);

	void OnFightStart();
	void OnFightTimeout();
	void OnFightResult(const char *reason, Role *role, bool is_win, Role *op_role);		// 战斗结束

	void SyncResultToCross(int winner_index);											// 同步战斗结果到跨服

	bool m_inited;
	bool m_finish;

	static const int FIGHTER_COUNT = 2;

	struct FighterInfo
	{
		FighterInfo() : uuid(0), role_obj_id(INVALID_OBJ_ID), role_userid(INVALID_USER_ID), hurt(0), start_hp(0), ready(false), is_robot(false)
		{
		}

		long long uuid;
		ObjID role_obj_id;
		UserID role_userid;
		long long hurt;				// 伤害值
		long long start_hp;			// 初始气血值
		bool ready;					// 是否已准备
		bool is_robot;				// 是否是机器人
	};

	FighterInfo m_fighter_list[FIGHTER_COUNT];
	
	bool m_fight_start;							// 战斗是否开始
	unsigned int m_fight_start_time;			// 战斗开始时间
	unsigned int m_fight_end_time;				// 战斗结束时间
	unsigned int m_delay_kickout_time;			// 延迟踢人时间
};


#endif // _SPECIAL_CROSS_1V1_HPP_

