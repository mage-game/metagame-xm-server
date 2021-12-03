#ifndef __SPECIAL_FIGHTING__
#define __SPECIAL_FIGHTING__

#include "gameworld/gameworld/scene/speciallogic/speciallogic.hpp"

class SpecialFighting : public SpecialLogic
{
	static const int READY_TIME_S = 3;								// ��ɫ������N���������๥��

public:
	SpecialFighting(Scene *scene);
	virtual ~SpecialFighting();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	bool OnTimeout();
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual bool CanDestroy() { return m_can_destroy; }
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialCanRecoverHp(Role *role) { return false; }
	virtual bool SpecialCanSupplyEffect(Role *role, int supply_type) { return false; }
	
private:
	bool m_can_destroy;

	int m_challenger_uid;											// ��ս��UID
	ObjID m_challenger_obj_id;										// ��ս��ObjID
	ObjID m_opponent_obj_id;										// ����ObjID

	unsigned int m_start_fighting_time;								// ��ʼ����ս��ʱ��
	bool m_can_fight;												// �ܷ�ʼ����

	int m_role_old_nuqi;
};

#endif