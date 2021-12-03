#ifndef _SPECIAL_EQUIP_TEAM_HPP_
#define _SPECIAL_EQUIP_TEAM_HPP_

#include "scene/speciallogic/speciallogic.hpp"

#include <set>

/*
	��Ӣ���װ����
*/


class SpecialEquipTeam : public SpecialLogic
{
public:
	SpecialEquipTeam(Scene *scene);
	virtual ~SpecialEquipTeam();

	virtual void Update(unsigned long interval, time_t now_second);

	virtual bool CanDestroy() { return false; }

	virtual void OnAddObj(Obj *obj);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialCanTransport(Role *role);
	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual void OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper);

	virtual bool OnTimeout();

	void OnJumpLayerReq(Role *role);
	void GMJumpLayerReq(Role *role, int to_layer);

	void SendSceneLogicInfo(Role *role);

protected:
	virtual void OnAllMonsterDead();
	void SyncSceneLogicInfo(Role *role = nullptr);

	void OnOver();
	void OnPassNoticeWord();          //ͨ�ش���

	bool m_is_passed;											// �Ƿ�ͨ��
	bool m_is_over;												// �Ƿ����

	bool m_is_first_enter;										// ��һ�����˽���
	bool m_can_jump;											// �Ƿ�������
	int m_total_monster_num;									// ��������
	int m_totall_boss_num;										// boss����
	int m_kill_boss_num;										// �ѻ�ɱboss����
	int m_kill_monster_num;										// �ѻ�ɱ��������
	unsigned int m_jumplayer_limit_time;						// ���㳬ʱʱ��
	int m_monster_hp_percent;									// ����Ѫ���ٷֱ�

	int m_team_index;											// ����±�
	int m_add_monster_maxhp_per;								// ���ӹ���Ѫ���ٷֱ�
	int m_add_monster_gongji_per;								// ���ӹ��﹥���ٷֱ�
	int m_add_monster_fangyu_per;								// ���ӹ�������ٷֱ�

	std::set<UserID> m_rewardexp_user_id_set;					// ������ؾ�������
};

#endif
