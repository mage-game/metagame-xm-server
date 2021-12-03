#ifndef _SPECIAL_ARMORDEFEND_HPP_
#define _SPECIAL_ARMORDEFEND_HPP_

#include "scene/speciallogic/speciallogic.hpp"
#include "other/fb/armordefendconfig.hpp"
#include "protocal/msgfb.h"

class SpecialArmorDefend : public SpecialLogic 
{
public:
	SpecialArmorDefend(Scene *scene);
	virtual ~SpecialArmorDefend();

	struct MovingMonsterInfo
	{
		MovingMonsterInfo() : obj_id(0), cur_pos_index(0) {}

		ObjID obj_id;
		int cur_pos_index;
	};

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnLeaveFB(Role *role);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);

	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);

	virtual bool SpecialCanRest(Role *role) { return false; }
	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual void OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper);

	void OnNextWaveReq(Role *role);

	virtual bool PerformSkill(Role *role, int skill_index, const Posi &pos, ObjID target_id);

	void SetRefreshWhenClean(short parm1);
	void SetIsPlayerGuider(bool is_guider) { m_is_new_player = is_guider; }
	void GmAddEnergy(int value);
	
private:
	Role * GetOwnerRole();
	void RefreshOne(time_t now_second);							// �Ӹ�������ˢһֻ
	void SyncToClient(int reason = Protocol::SCTowerDefendInfo::NOTIFY_REASON_DEFAULT);
	void OnFinish(bool is_pass);
	void NotifyWarning();
	void SyncPerformSkill(int skill_index);
	void AddToMoveList(ObjID obj_id);
	void RemoveFromMoveList(ObjID obj_id);
	void CheckMonsterEscape();
	void CheckFbFinish();
	void SendArmorDefendDropInfo();
	bool IsCurrWaveRefreshAll();

	unsigned int m_init_time;									// ��ʼ��ʱ��
	bool m_is_finish;											// �Ƿ����
	bool m_is_pass;												// �Ƿ�ɹ�
	bool m_is_last_wave;										// �Ƿ����һ��
	bool m_msg_need_sync;										// ��Ϣ�Ƿ���Ҫͬ�����ͻ���
	bool m_is_new_player;										// �Ƿ���������

	int m_curr_wave_index;										// ��ǰ��
	unsigned int m_next_wave_start_time;						// �²�ˢ��ʱ��
	unsigned int m_next_refresh_one_time;						// ��ֻˢ��ʱ��
	int m_refresh_left_count[ARMORDEFEND_PATH_MAX];				// ��ˢ�ֵ㱾��ʣ������

	UserID m_owner_user_id;
	ObjID m_owner_objid;
	int m_energy;

	int m_clear_wave_count;										// ɱ�Ⲩ��
	int m_kill_count[PERSONAL_TOWERDEFEND_MAX_WAVE_PER_LEVEL];	// ÿ��ɱ������

	static const int ENERGY_MAX_NUM = 100;

	unsigned int m_next_perform_time_list[PERSONAL_ARMORDEFEND_SKILL_MAX_COUNT];// �����ϴ��ͷ�ʱ��

	int m_get_coin;
	int m_get_item_count;
	ItemConfigData m_reward_item_list[Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM];

	int m_escape_monster_count;
	int m_escape_boss_count;
	typedef std::map<ObjID, MovingMonsterInfo> MovingMonsterInfoMap;
	typedef std::map<ObjID, MovingMonsterInfo>::iterator MovingMonsterInfoMapIt;
	MovingMonsterInfoMap m_moving_monster_step_map;
	short m_refresh_when_clean;				//���������ˢ��
	unsigned int m_pass_kick_out_time;		//�߳����
};

#endif



