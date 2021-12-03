#ifndef SPECIAL_HOLIDAY_GUARD_H
#define SPECIAL_HOLIDAY_GUARD_H

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgfb.h"
#include "other/fb/holidayguardconfig.hpp"

class SpecialHolidayGuard :
	public SpecialLogic
{
public:
	SpecialHolidayGuard(Scene *scene);
	~SpecialHolidayGuard();

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual void OnLeaveFB(Role *role);
	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
	virtual bool OnTimeout();
	virtual bool CanDestroy();
	virtual void OnRemoveObj(Obj *obj);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);

	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialIsFriend(Role *role, Obj *obj, bool check_alive = true);

	virtual bool SpecialCanReAlive(Role *role, int realive_type);
	virtual bool SpecialCanRest(Role *role) { return false; }
	virtual void OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper);

	void OnNextWaveReq(Role *role);

private:
	Role * GetOwnerRole();
	void RefreshOne(time_t now_second);							// �Ӹ�������ˢһֻ
	void SyncToClient(int reason = Protocol::SCTowerDefendInfo::NOTIFY_REASON_DEFAULT);
	void OnFinish(bool is_pass);
	void CheckAttackMonsterStatus(unsigned long interval, time_t now_second);
	void OnAllAttackMonsterDead();
	void NotifyWarning(short warning_type, short percent);

	bool m_is_inited;											// ��ʼ��
	bool m_is_finish;											// �Ƿ����
	bool m_is_pass;												// �Ƿ�ɹ�
	bool m_is_last_wave;										// �Ƿ����һ��
	bool m_msg_need_sync;										// ��Ϣ�Ƿ���Ҫͬ�����ͻ���

	int m_curr_wave_index;										// ��ǰ��
	unsigned int m_next_wave_start_time;						// �²�ˢ��ʱ��
	unsigned int m_next_refresh_one_time;						// ��ֻˢ��ʱ��
	int m_refresh_left_count[HOLIDAYGUARD_PATH_MAX];			// ��ˢ�ֵ㱾��ʣ������

	unsigned int m_last_check_monster_status_time;				// �ϴμ�鳡��״̬ʱ��
	unsigned int m_last_warning_attack_time;					// �ϴ�����������ı���ʱ��
	unsigned int m_last_sync_to_allclient_time;					// �ϴ�ͬ����Ϣ���ͻ��˵�ʱ��

	long long m_life_tower_left_hp;									// ������ʣ��HP
	long long m_life_tower_left_maxhp;								// ������ʣ�����HP

	UserID m_owner_user_id;
	ObjID m_owner_objid;

	int m_clear_wave_count;										// ɱ�Ⲩ��
	int m_kill_count[PERSONAL_HOLIDAYGUARD_MAX_WAVE_PER_LEVEL];	// ÿ��ɱ������

	static const int REWARD_ITEM_MAX_TYPE = 32;					// ������Ʒ������� 

	int m_get_coin;
	int m_get_item_count;
	ItemConfigData m_reward_item_list[REWARD_ITEM_MAX_TYPE];

	time_t m_send_drop_info_timestamp;

	unsigned short m_kill_monster_count;						// ��ɱ��������
};

#endif //SPECIAL_HOLIDAY_GUARD_H