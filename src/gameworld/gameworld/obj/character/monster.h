#ifndef __MONSTER_H__
#define __MONSTER_H__

#include <map>

#include "character.h"
#include "monster/monsterinitparam.h"
#include "servercommon/crossdef.hpp"

class Role;
class Scene;
class MonsterGenerater;
class MonsterAIBase;


class Monster : public Character
{
public:
	struct RoleHurtInfo
	{
		bool operator < (const RoleHurtInfo &r) const
		{
			return (hurt < r.hurt) || (hurt == r.hurt && uid < r.uid);
		}

		int uid;
		ObjID objid;
		long long hurt;
		time_t logout_time;			// ��鵽������ʱ��
	};

	Monster(const GameName _name);
	virtual ~Monster();

	void Init(const MonsterInitParam &param, int monster_key);

	int GetMonsterType() { return m_monster_type; }
	unsigned short GetMonsterId() { return m_monster_id; }
	virtual int GetLevel() { return m_monster_level; }
	int GetCapability() { return m_monster_capability; }
	bool IsForbidRecover() { return m_is_forbid_recover; }
	time_t GetCreateTime() { return m_create_time; }
	int GetMonsterKey() { return m_monster_key; }
	long long GetRewardExp() { return m_exp_reward; }
	int GetAutoRecoverPer() { return m_auto_reover_per; }
	int GetAutoRecoverTimeGap() { return m_auto_reover_time_gap; }

	MonsterAIBase * GetMonsterAI() { return m_ai; }
	int GetAIType();

	virtual void OnEnterScene();
	virtual void OnLeaveScene();
	virtual void OnAOIEnter(ObjID obj_id);
	virtual void OnAOILeave(ObjID obj_id);

	void OnNearRoleRealive(ObjID obj_id, const Posi &pos);
	void OnNearRoleDie(ObjID obj_id, const Posi &pos);

	virtual	void Update(unsigned long interval, time_t now_second, unsigned int now_dayid);

	bool HasGenerater() { return NULL != m_generater; }
	void SetGenerater(MonsterGenerater *g) { m_generater = g;}

	bool SetOwnerUserId(const UserID &user_id, bool chk_boss = true);
	const UserID & GetOwnerUserId() { return m_owner_user_id; } 
	bool HasOwner() { return (INVALID_USER_ID != m_owner_user_id); }

	void SetOwnerMonster(ObjID owner_obj_id, int owner_key) { m_owner_monster_objid = owner_obj_id; m_owner_monster_key = owner_key; }

	void *operator new(size_t c);
	void operator delete(void *m);

	void ForceSetEnemy(ObjID obj_id);

	virtual bool IsEnemy(Obj *obj);
	virtual bool IsFriend(Obj *obj, bool check_alive = true);

	virtual bool CanBeCaptureBySkill() { return (!m_forbid_capture_by_skill && !IsMianyiCapture()); }
	virtual bool IsFixHurtOnAttack() { return m_is_fix_hurt_on_attack; }
	virtual int GetFixHurtOnAttack();

	virtual void OnAttackCalcHurt(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill, int fight_type);
	virtual void OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp, bool from_skill);
	virtual void OnDie(ObjID killer);

	bool IsBossMonster() { return MonsterInitParam::MONSTER_TYPE_BOSS == m_monster_type ; }
	bool IsWorldBossMonster();
	bool IsCrossBossMonster();

	void ForceSetDead(Role *role = NULL);
	void ReAlive();
	bool ResetPos(Axis x, Axis y);
	virtual bool MoveBySkill(Axis _x, Axis _y, Dir _dir, Scalar distance, bool check_special_state = true);

	void SetBossMonsterBuff();
	void SetIsFixHurtOnAttack(bool is_fix_hurt_on_attack) { m_is_fix_hurt_on_attack = is_fix_hurt_on_attack; }

	int GetSpecialParam() { return m_special_param; }
	void SetSpecialParam(int special_param) { m_special_param = special_param; }

	void SetMonsterInitDir(Dir dir, Scalar dis);

	void GuaJiActivityExtraDrop(Role *drop_role);
	void GuildBossLuckyExtraDrop(Role *drop_role);
	void GuildFbLuckyExtraDrop(Role *drop_role, int dropid);

	int GetDropOwnerUid() { return m_drop_owner_uid; }
	int GetDropOwnerTeamIndex() { return m_drop_owner_team_index; }

	void NotifyDropOwner(Role *role = NULL, bool dead_notify = false); // ֪ͨ��ҵ�ǰ����Ĺ���
	void BroadcastFirstHitInfo();

protected:
	const static int OWNER_KEEP_TIME_S = 60;	// ������߹�������ʱ��
	const static int RECALC_DROP_OWNER_INTERVAL_S = 2;	// �������˺��������

	const Posi & GetBirthPosi();

	void RewardExpOnDead(Role *killer_role, int member_count, Role *member_list[MAX_TEAM_MEMBER_NUM]);
	void RewardYuanliOnDead(Role *killer_role, int member_count, Role *member_list[MAX_TEAM_MEMBER_NUM]);
	void RewardIntimacyOnDead(Role *killer_role, int member_count, Role *member_list[MAX_TEAM_MEMBER_NUM]);
	void DropOnDead(Role *killer_role, int member_count, Role *member_list[MAX_TEAM_MEMBER_NUM]);
	void DropMoneyOnDead(Role *killer_role, int member_count, Role *member_list[MAX_TEAM_MEMBER_NUM]);
	void EventOnDead(Role *killer_role, int member_count, Role *member_list[MAX_TEAM_MEMBER_NUM]);
	void NoticeOnDead(Role *killer_role, int member_count, Role *member_list[MAX_TEAM_MEMBER_NUM]);

	int DropToRandRole(UInt16 dropid, int member_count, Role *member_list[MAX_TEAM_MEMBER_NUM]);
	void DropNoKiller();
	void DropExtra(Role *role);

	bool IsDropSpecial();
	bool IsLogDrop() { return (this->IsWorldBossMonster() || this->IsCrossBossMonster()); }

	void SyncDeadInfo();

	void CheckOwnerAlive();

	bool RecalcDropOwner(bool boss_die);
	void DropToFirstRole(UniqueUserID killer_role_uuid);
	void DropToFirstRoleTeam(Role *first_hit_role);			// �׵�������ڶ��飬�����˷�һ��
	void DropToFirstAllTeam(UniqueUserID killer_role_uuid); // �׵�������ڶ��飬ÿ��һ��(BOSS_DROP_OWN_TYPE_FIRST_ALL_TEAM)
	void DropToRole(UserID user_id);
	bool CanDropToRole(Role &role);
	void DropToTeam(int team_index);                    // �����������������˺���
	void DropToTeam(Role *killer_role, int member_count, Role *member_list[MAX_TEAM_MEMBER_NUM]);                    // ȫ��ÿ��һ��
	bool CanGetDropLevel(int level);                    // ��Щ�������Ƶ���ȼ�
	void AddAngryOrTireToRole(Role *role);              // ��õ���֮���һЩ����
	void NoticeHurtRoleWhenDie(Role * killer);

	void OnBossDie();									// boss����

	char				m_monster_type;					// ��������
	char				m_boss_type;					// boss����
	unsigned short		m_monster_id;					// ����ID 
	int					m_monster_level;				// ����ȼ�
	int					m_monster_capability;			// ����ս����
	
	MonsterGenerater	*m_generater;					// ���ֵ�

	UserID				m_owner_user_id;				// ������ɫID
	ObjID				m_owner_monster_objid;			// �ٻ���objid
	int					m_owner_monster_key;			// �ٻ��߹���key

	int                 m_drop_type;                    // ��������
	int					m_drop_index;
	std::vector<UInt16>	m_dropid_list;					// ��ɱ����ID�б�
	std::vector<UInt16> m_firsthit_dropid_list;			// ��һ������ID�б�
	std::vector<UInt16> m_dps_dropid_list;				// DPS����ID�б�
	std::vector<UInt16> m_rand_dropid_list;				// �����������ID�б�
	std::vector<UInt16> m_task_dropid_list;             // ���������
	int					m_drop_money_count;				// ��Ǯ����
	int					m_drop_money_prob;				// ��Ǯ����

	long long			m_exp_reward;					// ɱ�־��齱��
	int					m_yuanli_reward;				// Ԫ������
	time_t				m_create_time;					// ��������ʱ��
	time_t				m_disappear_time;				// �Զ���ʧʱ��
	bool				m_die_notice;					// �Ƿ���������
	bool				m_forbid_capture_by_skill;		// ���ܱ��������������
	bool				m_is_fix_hurt_on_attack;		// ����ʱ�Ƿ�̶��˺�
	bool				m_is_forbid_recover;			// ��ֹ�Զ���Ѫ
	bool				m_is_forbid_recover_backup;		// ��ֹ�Զ���Ѫ����

	int					m_auto_reover_per;				// һֱ�Զ���Ѫ��ֱ�
	int					m_auto_reover_time_gap;			//�Զ���Ѫ���ʱ�䣨���룩

	MonsterAIBase		*m_ai;							// ����AI

	typedef std::map<ObjID, UserID> MonsterHurterMap;
	typedef std::map<ObjID, UserID>::iterator MonsterHurterMapIt;
	MonsterHurterMap m_monster_hurter_map;				// �˺�������û��б�

	int					m_monster_key;					// ����KEY

	int					m_special_param;				// �������

	bool                 m_need_notice_to_hurt_role_when_die; // �����ɱ����ҵ�����ɱ���

	// ���������� =================================================================================
	int					OWNER_MAX_DISTANCE;				// ��ҹ�����Χ������Ҿ��볬����Χ��ʧ����������Ϊ׷�ٷ�Χ�����Բ���const��
	int					m_drop_owner_uid;				// ���������UID
	int					m_drop_owner_team_index;		// ������������ڶ���index
	time_t				m_drop_owner_team_next_check_time;	// ��ӹ������´μ��ʱ��
	time_t				m_drop_owner_team_last_force_check_time; // ��ӹ�������һ��ǿ�Ƽ���ʱ��

	long long			m_drop_owner_hurt;				// �����߻���鵱ǰ�˺���

	typedef std::map<int, RoleHurtInfo> RoleHurtInfoMap;
	typedef std::map<int, RoleHurtInfo>::iterator RoleHurtInfoMapIt;
	RoleHurtInfoMap		m_role_hurt_info_map;			// �˺������б�
};

#endif

