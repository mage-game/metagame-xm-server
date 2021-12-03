#ifndef __SPECIAL_NEQ_PERSONAL_HPP__
#define __SPECIAL_NEQ_PERSONAL_HPP__

#include "scene/speciallogic/speciallogic.hpp"

class SpecialNewEquip : public SpecialLogic
{
public:
	SpecialNewEquip(Scene *scene);
	virtual ~SpecialNewEquip();

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnRoleEnterScene(Role *role);

	virtual void OnLeaveFB(Role *role);

	virtual bool SpecialRelive(Role *role, int realive_type);

	virtual bool OnTimeout();

	virtual void OnPickFallingItem(Role *role, FallingItem *falling_item);
	
	virtual bool SpecialCanMountOn(Role *role) { return false; }

	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual void OnCharacterDie(Character *dead, ObjID killer_id);

	virtual bool CanDestroy() { return m_is_finish; }

protected:
	Role * GetOwnerRole();
	virtual void OnAllMonsterDead();
	void CheckFBStar();
	void SyncNewEQLogicInfo(bool is_active_leave_fb = false);

	ObjID m_owner_objid;										// 副本拥有者对象ID
	UserID m_owner_user_id;										// 副本拥有者用户ID

	bool m_is_finish;											// 是否结束 
	bool m_is_passed;											// 是否通关
	bool m_is_boss_flush;		
	bool m_is_monster_flush;

	static const int REWARD_ITEM_MAX_TYPE = 32;								// 奖励物品最多种类 

	int m_get_coin;
	int m_get_item_count;
	ItemConfigData m_reward_item_list[REWARD_ITEM_MAX_TYPE];

	unsigned short m_boss_id;
	unsigned int m_sync_logic_info_time;

	int m_kill_monster_num;
	int m_kill_boss_num;

	int m_sec_star;				//副本当前星级
};

#endif // __SPECIAL_EQUIP_HPP__


