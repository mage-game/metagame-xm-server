
#ifndef WORLD_EVENT_OBJ_H
#define WORLD_EVENT_OBJ_H

class Role;

#include "obj/obj.h"

class WorldEventObj : public Obj
{
public:
	WorldEventObj();
	virtual ~WorldEventObj();

	void Init(int world_event_id, int event_type, int event_param, int max_hp, int touch_distance, time_t disappear_time);
	virtual void Update(unsigned long interval, time_t now_second, unsigned int now_dayid);

	void Touch(Role *role);

	int GetWorldEventID() { return m_world_event_id; }
	int GetWorldEventType() { return m_event_type; }
	int GetEventParam() { return m_event_param; }
	long long GetHp() { return m_hp; }
	long long GetMaxHp() { return m_max_hp; }

	void DecHp(long long dec_hp);

	void *			operator new(size_t c);
	void			operator delete(void *m);

protected:
	int m_world_event_id;										// �����¼�ID
	int m_event_type;											// �¼�����

	int m_event_param;											// �¼�����
	time_t m_disappear_time;									// ��ʧʱ��

	long long m_max_hp;											// ���Ѫ��
	long long m_hp;												// Ѫ�� ��������
	int m_touch_distance;										// �Ӵ�����
};

#endif // WORLD_EVENT_H



