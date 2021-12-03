#ifndef _SPECIALTOWERDEFENDTEAM_HPP_
#define _SPECIALTOWERDEFENDTEAM_HPP_

#include "scene/speciallogic/speciallogic.hpp"
#include "other/fb/towerdefendteamconfig.hpp"
#include "protocal/msgfb.h"

class SpecialTeamTowerDefend : public SpecialLogic
{
public:
	SpecialTeamTowerDefend(Scene *scene);
	virtual ~SpecialTeamTowerDefend();

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual void OnLeaveFB(Role *role);
	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
	virtual bool OnTimeout();
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);

	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialIsFriend(Role *role, Obj *obj, bool check_alive = true);

	virtual bool SpecialCanReAlive(Role *role, int realive_type);
	virtual bool SpecialCanMountOn(Role *role) { return false; }

	virtual void OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper);

	void OnNextWaveReq(Role *role);

	virtual bool PerformSkill(Role *role, int skill_index, const Posi &pos, ObjID target_id);

	virtual long long SpecialRecalcHate(Role *role, int skill_id, long long hate);
	virtual bool CanDestroy() { return m_has_role_leave; }

	void SyncFBLogicInfo(Role *role = nullptr);

	void GmTowave(int wave);

private:
	struct SkillInfo
	{
		SkillInfo() { this->Clear(); }
		void Clear() { skill_id = 0; next_can_perform_timestamp = 0;}
		bool Invalid() { return 0 == skill_id; }

		int skill_id;
		unsigned int next_can_perform_timestamp;
	};

	const static int DROP_ITEM_TYPE_MAX = 32;

	struct FBPersonalInfo
	{
		static const int EXTRA_START_INDEX = TEAM_TOWERDEFEND_NORMAL_SKILL_COUNT;

		FBPersonalInfo() : uid(0), camp(0), attr_type(0), hp_when_leave(0),angry_value(0)
		{
			memset(role_name, 0, sizeof(role_name));
			memset(reward_list, 0, sizeof(reward_list));
		}

		SkillInfo * GetNormalSkillInfo(int skill_id)
		{
			for (int i = 0; i < TEAM_TOWERDEFEND_NORMAL_SKILL_COUNT; ++i)
			{
				if (skill_id == skill_list[i].skill_id) return &skill_list[i];
			}

			return NULL;
		}	

		int uid;
		GameName role_name;
		char camp;
		char attr_type;											// ��������

		long long hp_when_leave;								// �뿪ʱ��hp
		int angry_value;										// ����ֵ
		SkillInfo skill_list[TEAM_TOWERDEFEND_NORMAL_SKILL_COUNT + TEAM_TOWERDEFEND_EXTRA_SKILL_COUNT];
		ItemConfigData reward_list[DROP_ITEM_TYPE_MAX];
	};

	void RefreshOne(time_t now_second);							// �Ӹ�������ˢһֻ
	void BroadcastToClient(int reason = Protocol::SCTeamTowerDefendInfo::NOTIFY_REASON_DEFAULT);
	
	void OnFinish(bool is_pass, Role *role = NULL, short notify_reason = 0);
	void CheckAttackMonsterStatus(unsigned long interval, time_t now_second);
	void OnAllAttackMonsterDead();
	void NotifyWarning(short warning_type, short percent);
	void FinishBroadcast();
	void SyncSkillInfo(Role *role, SkillInfo *skill_info);

	void InitSkill(FBPersonalInfo &person_info, int attr_type);
	void RecalcAttrAdd(Role *role, bool first_enter);
	void AddGongjiBySkill(Role *role, FBPersonalInfo &person_info);
	void AddMaxhpBySkill(Role *role, FBPersonalInfo &person_info, bool recover_hp, bool add_hp);
	void SyncAllRoleInfo(Role * role = nullptr);

	unsigned int m_is_inited;									// ��ʼ��ʱ��
	bool m_is_finish;											// �Ƿ����
	bool m_is_pass;												// �Ƿ�ɹ�
	bool m_is_last_wave;										// �Ƿ����һ��
	bool m_has_role_leave;
	int m_team_index;											// ����index
	int m_pass_level;											// ͨ������
	int m_passtime_s;											// ͨ��ʱ��

	int m_curr_wave_index;										// ��ǰ��
	unsigned int m_next_wave_start_time;						// �²�ˢ��ʱ��
	unsigned int m_next_refresh_one_time;						// ��ֻˢ��ʱ��
	int m_refresh_left_count[TEAM_TOWERDEFEND_PATH_MAX];		// ��ˢ�ֵ㱾��ʣ������

	int m_clear_wave_count;										// ɱ�Ⲩ��
	int m_kill_count[TEAM_TOWERDEFEND_WAVE_COUNT];				// ÿ��ɱ������

	unsigned int m_last_check_monster_status_time;				// �ϴμ�鳡��״̬ʱ��
	unsigned int m_last_warning_attack_time;					// �ϴ�����������ı���ʱ��
	unsigned int m_last_sync_to_allclient_time;					// �ϴ�ͬ����Ϣ���ͻ��˵�ʱ��

	int m_gongji_uid;											// �����ӳ���
	int m_fangyu_uid;											// �����ӳ���
	int m_assist_uid;											// �����ӳ���

	long long m_life_tower_left_hp;								// ������ʣ��HP
	long long m_life_tower_left_maxhp;							// ���������HP

	int m_add_monster_maxhp_per;								// ���ӹ���Ѫ���ٷֱ�
	int m_add_monster_gongji_per;								// ���ӹ��﹥���ٷֱ�
	int m_add_monster_fangyu_per;								// ���ӹ�������ٷֱ�

	FBPersonalInfo m_personal_info_list[MAX_TEAM_MEMBER_NUM];
	unsigned int m_last_sync_role_info_time;					//�ϴ�ͬ��������Ϣʱ��
	bool m_have_role_enter;										// �Ƿ�����ҽ���
	unsigned int m_pass_kick_out_time;
};

#endif
