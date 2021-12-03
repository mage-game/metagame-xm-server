#ifndef __SPECIAL_WANGLING_EXPLORE_HPP__
#define __SPECIAL_WANGLING_EXPLORE_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"
#include <vector>

class SpecialWangLingExplore : public SpecialLogic
{
public:
	SpecialWangLingExplore(Scene *scene);
	virtual ~SpecialWangLingExplore();

	virtual void Update(unsigned long interval, time_t now_second);
	void CheckBornBoss(time_t now_second);
	void CheckBornMonster(time_t now_second);

	virtual bool CanDestroy() { return m_is_finish; }

	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialIsFriend(Role *role, Obj *obj, bool check_alive = true);

	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);

	virtual void OnCharacterDie(Character *dead_cha, ObjID killer_objid);
	virtual void OnHurtOther(Character *cha, Character *target);

	void OnKillBoss(Role *role);
	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual bool SpecialCanMountOn(Role *role){return false;}
	
	virtual int GetSpecialParam(Role *role);
	virtual bool SpecialStopGahterOnAttack(Role *role);

	void OnActivityStatusChange(int status);

	void OnFetchTaskExtraReward(Role *role);

	WangLingExploreUser * GetWangLingExploreUser(const UserID &user_id);

	void OnBuyGatherBuff(Role * role);

protected:
	void SendWangLingExploreInfoToAllUser();
	void SendWangLingExploreUserInfo(Role *role);
	void CheckUserTaskInfo(Role *role, short task_type, int param_id);
	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);

	void OnActivityStandby();
	void OnActivityStart();
	void OnActivityClose();

	void OnWangLingExploreClose();

	void CheckLianZhanNotice(Role *kill_role, Role *dead_role, bool is_assist = false);

	void SendBossInfo(Role *role = nullptr);
	void CheckSendBossInfo();

	void CheckGatherBuffInfo();
	void NotifyGatherBuffInfo(Role *role, short is_has_buff);

	bool m_is_finish;
	int m_boss_num;
	time_t m_next_reflush_boss_time;
	time_t m_next_reflush_monster_time;
	time_t m_reflush_notice_time;
	bool m_is_notice;
	std::map<int, time_t> m_role_task_time_map;
	UserID m_boss_owner;
	ObjID m_boss_objid;
	int m_boos_id;
	long long m_boss_max_hp;
	long long m_boss_cur_hp;

	unsigned m_send_boss_info_time;

	int m_monster_die_num;
	unsigned int m_next_check_buff_time;
	WangLingExploreUserMap m_wanglingexplore_usermap;											// 王陵探险用户列表
	
};

#endif // __SPECIAL_WANGLING_EXPLORE_HPP__
