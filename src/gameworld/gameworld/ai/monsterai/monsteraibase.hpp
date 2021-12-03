/*************************************************************************************************
FileName: monsteraibase.hpp
Author: dreamwang
Description: ����AI 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/11/10/ 13:37:05     1.0           ����
************************************************************************************************/

#ifndef __MONSTER_AIBASE_HPP__
#define __MONSTER_AIBASE_HPP__

#include <map>

#include "ai/aibase.hpp"
#include "ai/monsterai/monsteraiparam.hpp"
#include "monster/monsterinitparam.h"
#include "obj/character/character.h"

#include "servercommon/servercommon.h"

static const int BOSS_RAND_REWARD_RAND_RANGE = 50;			// BOSS���������Χ
static const int BOSS_RAND_REWARD_RAND_COUNT = 20;			// BOSS�����������

struct RewarderUnit
{
	RewarderUnit() { this->Clear(); }

	void Clear()
	{
		for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i) objid_list[i] = INVALID_OBJ_ID;
	}

	void AddObj(ObjID obj_id)
	{
		for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
		{
			if (objid_list[i] == obj_id) return;

			if (INVALID_OBJ_ID == objid_list[i])
			{
				objid_list[i] = obj_id; return;
			}
		}
	}

	ObjID objid_list[MAX_TEAM_MEMBER_NUM];
};

class Role;

class MonsterAIBase : public AIBase
{
	friend class Monster;

public:
	MonsterAIBase(Character *me, int aitype);
	virtual ~MonsterAIBase();

	virtual void Init(const MonsterAIParam &mp);

	virtual void OnEnterScene() {}
	virtual void OnLeaveScene() {}
	virtual void OnAOIEnter(ObjID obj_id) {}
	virtual void OnAOILeave(ObjID obj_id) {}
	
	virtual void OnRoleDie(ObjID obj_id);
	virtual void OnDie(ObjID killer);
	virtual void OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp);
	virtual void Update(unsigned long interval, unsigned int now_second);

	virtual int GetAIType() { return m_aitype; }

	ObjID GetEnemy() { return m_enemy; }
	virtual void ForceSetEnemy(ObjID obj_id);

	void OnChaofeng(Role *enemy_role, long long hate);

	UniqueUserID GetFirstHitUser();										// ��ȡ�׵����
	void SetFirstHitUser(const UniqueUserID &user_uuid);				// �����׵����
	void GetTopHurtRewarder(RewarderUnit *top_reward_unit, RewarderUnit rand_reward_unit_list[BOSS_RAND_REWARD_RAND_COUNT]);
	Role * RandGetHurtRole();
	Role *GetTopHurtRole();
	bool GetRewarderByHurtMoreThen(int hurt, int max_list_count, int *list_count, UserID *userid_list);

	const Posi & GetBirthPosi() { return m_birth_pos; }

	bool IsContinuePerforming() { return SPECIAL_STATUS_TYPE_CONTINUE_PERFORM == m_special_status.status_type; }

	bool IsFighting() { return 0 != m_bossskill_cond_id && 0 != m_fight_time; }

	void CancleSpcialStaus();
	short GetStatusType() { return m_special_status.status_type; }

	void SetHasDpsDrop(bool has_dps_drop) { m_has_dps_drop = has_dps_drop; };
	void SetHasFirstDrop(bool has_first_hit_drop) { m_has_first_hit_drop = has_first_hit_drop; }

	void SetIsStatic(bool is_static) { m_is_static = is_static; }
	void SetStaticEndTime(unsigned int end_static_time) { m_static_end_time = end_static_time; }
	bool IsSatic() { return m_is_static;}

protected:
	virtual bool IsEnemy(Obj *obj);
	virtual bool IsFriend(Obj *obj, bool check_alive = true);

	virtual bool IsHateDrive() { return true; }

	void StartAttack(ObjID enemy, bool perform_skill = false);
	void SetNextSkill(ObjID enemy);

	bool PerformNextSkillFromList(int count, const int *skill_index_list);
	bool CheckHateDrive(bool is_update = false);
	void ClearBossInfo();
	void OnEnemyLost();

	bool CanStartAttack();
	bool IsOutRange();
	void Follow(ObjID obj_id, unsigned long interval);

	int SearchNearEnemys(unsigned long interval, const Posi &pos, int range, ObjID *target_list, int max_num);

	void AttachWudiState(int dual_ms = 0);

	virtual bool IsAttackWhenOnAttack(ObjID attacker) { return INVALID_OBJ_ID == m_enemy; }

	virtual bool PerformSkillByIndex(int skill_index, int skill_phase);
	bool GetSkillTarget(bool is_target_enemy, Posi *target_pos, ObjID *target_id);
	virtual bool CheckSpecialStatus();
	virtual void OnSpecialStatus();

	long long CalcHate(Role *attacker_role, UInt16 skill_id, long long deltahp);
	void ClearHate(ObjID obj_id);
	
	int m_aitype;										// AI����

	unsigned int m_current_runtime;						// ��ǰ����ʱ����
	unsigned int m_follow_interval;						// �ɸ���ʱ����
	unsigned int m_run_range;							// ��������
	unsigned int m_aoi_range;							// ������̽�ⷶΧ
	ObjID m_enemy;										// ��¼�����ĵ���
	bool  m_is_out_range;								// �Ƿ񳬳���������
	Posi  m_birth_pos;									// ������

	int m_bossskill_cond_id;							// boss�������� ID

	bool m_release_skillcond_hplow_per;					// �ͷ�Ѫ���ٷֱȼ��ܱ��
	int m_release_skillcond_hplow_per_idx;				// �ͷ�Ѫ���ٷֱȼ��� �����±�

	int m_fight_time;									// ս���˶��ٺ���
	int m_born_time;									// �����˶��ٺ���

	struct HateInfo
	{
		HateInfo() : user_id(INVALID_USER_ID), hate(0) {}
		HateInfo(const UserID _user_id, long long _hate) : user_id(_user_id), hate(_hate) {}

		UserID user_id;
		long long hate;
	};

	typedef std::map<ObjID, HateInfo> HateMap;
	typedef std::map<ObjID, HateInfo>::iterator HateMapIt;
	HateMap m_hate_drive_map;							// ��������б�

	typedef std::map<UserID, long long> HurtMap;
	typedef std::map<UserID, long long>::iterator HurtMapIt;
	HurtMap m_hurt_map;									// �˺��б�

	static const int CHANGE_DPS_TIME = 3;				// �л�dpsʱ��
	const static int DIS_CHECK_INTERVAL_TIME = 2;

	bool m_has_dps_drop;
	UserID m_top_hurt_user_id;
	long long m_top_hurt;
	unsigned int m_dps_end_time;
	
	bool m_has_first_hit_drop;							// �Ƿ��ǵ�һ������
	UniqueUserID m_first_hit_user_uuid;					// ��һ���û�ID

	unsigned int m_last_check_hate_drive_time;			// �ϴμ�����б�ʱ��
	unsigned int m_last_auto_recover_time;				// �ϴ��Զ���Ѫʱ��

	bool m_need_search;									// �Ƿ���Ҫ������ͼ�ϵ���
	unsigned long m_search_enemy_interval;				// ������ͼ�ϵ��˵ļ��

	ObjID m_chaofenger_objid;							// ������id
	unsigned int m_chaofeng_invalid_time;				// ����ʧЧ

	enum SPECIAL_STATUS_TYPE
	{
		SPECIAL_STATUS_TYPE_NONE = 0,
		SPECIAL_STATUS_TYPE_READING,					// ����
		SPECIAL_STATUS_TYPE_CONTINUE_PERFORM,			// ����ʩ��
		SPECIAL_STATUS_TYPE_PERFORM_SKILL_ANYWAY,		// �ض��ͷż���
	};

	struct SpecialStatus
	{
		SpecialStatus() { this->Clear(); }

		void Clear()
		{ 
			status_type = SPECIAL_STATUS_TYPE_NONE;
			skill_index = -1;
			end_time = 0;
			status_param = 0;
			start_pos.x = start_pos.y = 0;
			target_pos.x = target_pos.y = 0;
		}

		short status_type;								// ״̬����
		short skill_index;
		unsigned int end_time;							// ״̬����ʱ��
		int status_param;
		Posi start_pos;									// ״̬��ʼʱ����
		Posi target_pos;								// Ŀ������
	};

	SpecialStatus m_special_status;
	unsigned int m_last_fight_time;
	time_t	m_dis_next_check_time;						// �´μ�������boss�ľ���

	bool m_is_static;									// �Ƿ�ֹ
	unsigned int m_static_end_time;						// ��ֹ����ʱ��
};

#endif // __MONSTER_AIBASE_HPP__

