#ifndef _SPECIALGUILDMONSTER_HPP_
#define _SPECIALGUILDMONSTER_HPP_

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgfb.h"

class SpecialGuildMonster : public SpecialLogic
{
public:
	SpecialGuildMonster(Scene *scene);
	virtual ~SpecialGuildMonster();

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual void OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper);
	virtual bool OnTimeout();
	virtual void OnLeaveFB(Role *role);
	
	void OnReqNextLevel();

private:
	Role * GetOwnerRole();
	void OnFinish(bool is_pass, bool is_active_leave_fb = false);
	void CheckBuffStatus();

	UserID m_owner_user_id;
	ObjID m_owner_objid;

	bool m_is_inited;											// 初始化
	bool m_is_finish;											// 是否结束
	bool m_is_pass;												// 是否通关

	int m_monster_id;
	int m_target_seq;

	static const int REWARD_ITEM_MAX_TYPE = 32;								// 奖励物品最多种类 

	int m_get_coin;
	int m_get_item_count;
	ItemConfigData m_reward_item_list[REWARD_ITEM_MAX_TYPE];

	time_t m_send_drop_info_timestamp;
};

#endif
