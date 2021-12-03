#ifndef __SPECIAL_CHALLENGE_HPP__
#define __SPECIAL_CHALLENGE_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgfb.h"

class SpecialChallenge : public SpecialLogic
{
public:
	SpecialChallenge(Scene *scene);
	virtual ~SpecialChallenge();

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnLeaveFB(Role *role);
	virtual bool OnTimeout();
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual void OnRemoveObj(Obj *obj);
	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual bool SpecialCanTransport(Role *role);
	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual void OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper);

protected:
	virtual void OnAllMonsterDead();

private:
	void OnLayerFinish(bool is_passed, bool is_active_leave_fb = false);
	Role * GetOwnerRole();

	void SendLayerInfo();

	bool is_first;															
	bool m_is_finish;														// 是否结束
	bool m_is_passed;														// 是否通过

	ObjID m_owner_objid; 
	UserID m_owner_user_id;
	
	unsigned short m_boss_id;
	unsigned int m_die_timestamp;

	int m_get_coin;
	int m_get_item_count;
	ItemDataWrapper m_reward_item_list[Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM];
	unsigned int m_pass_kick_out_time;
};

#endif



