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

	ObjID m_owner_objid;										// ����ӵ���߶���ID
	UserID m_owner_user_id;										// ����ӵ�����û�ID

	bool m_is_finish;											// �Ƿ���� 
	bool m_is_passed;											// �Ƿ�ͨ��
	bool m_is_boss_flush;		
	bool m_is_monster_flush;

	static const int REWARD_ITEM_MAX_TYPE = 32;								// ������Ʒ������� 

	int m_get_coin;
	int m_get_item_count;
	ItemConfigData m_reward_item_list[REWARD_ITEM_MAX_TYPE];

	unsigned short m_boss_id;
	unsigned int m_sync_logic_info_time;

	int m_kill_monster_num;
	int m_kill_boss_num;

	int m_sec_star;				//������ǰ�Ǽ�
};

#endif // __SPECIAL_EQUIP_HPP__


