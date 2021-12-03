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
		time_t logout_time;			// 检查到的下线时间
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

	void NotifyDropOwner(Role *role = NULL, bool dead_notify = false); // 通知玩家当前怪物的归属
	void BroadcastFirstHitInfo();

protected:
	const static int OWNER_KEEP_TIME_S = 60;	// 玩家下线归属保留时间
	const static int RECALC_DROP_OWNER_INTERVAL_S = 2;	// 检测掉落伤害排名间隔

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
	void DropToFirstRoleTeam(Role *first_hit_role);			// 首刀玩家所在队伍，所有人分一份
	void DropToFirstAllTeam(UniqueUserID killer_role_uuid); // 首刀玩家所在队伍，每人一份(BOSS_DROP_OWN_TYPE_FIRST_ALL_TEAM)
	void DropToRole(UserID user_id);
	bool CanDropToRole(Role &role);
	void DropToTeam(int team_index);                    // 队伍随机（需有输出伤害）
	void DropToTeam(Role *killer_role, int member_count, Role *member_list[MAX_TEAM_MEMBER_NUM]);                    // 全队每人一份
	bool CanGetDropLevel(int level);                    // 有些场景限制掉落等级
	void AddAngryOrTireToRole(Role *role);              // 获得掉落之后的一些操作
	void NoticeHurtRoleWhenDie(Role * killer);

	void OnBossDie();									// boss死亡

	char				m_monster_type;					// 怪物类型
	char				m_boss_type;					// boss类型
	unsigned short		m_monster_id;					// 怪物ID 
	int					m_monster_level;				// 怪物等级
	int					m_monster_capability;			// 怪物战斗力
	
	MonsterGenerater	*m_generater;					// 出怪点

	UserID				m_owner_user_id;				// 所属角色ID
	ObjID				m_owner_monster_objid;			// 召唤者objid
	int					m_owner_monster_key;			// 召唤者怪物key

	int                 m_drop_type;                    // 掉落类型
	int					m_drop_index;
	std::vector<UInt16>	m_dropid_list;					// 击杀掉落ID列表
	std::vector<UInt16> m_firsthit_dropid_list;			// 第一刀掉落ID列表
	std::vector<UInt16> m_dps_dropid_list;				// DPS掉落ID列表
	std::vector<UInt16> m_rand_dropid_list;				// 随机奖励掉落ID列表
	std::vector<UInt16> m_task_dropid_list;             // 任务定向掉落
	int					m_drop_money_count;				// 掉钱数量
	int					m_drop_money_prob;				// 掉钱概率

	long long			m_exp_reward;					// 杀怪经验奖励
	int					m_yuanli_reward;				// 元力奖励
	time_t				m_create_time;					// 创建怪物时间
	time_t				m_disappear_time;				// 自动消失时间
	bool				m_die_notice;					// 是否死亡公告
	bool				m_forbid_capture_by_skill;		// 不能被技能拉扯或击退
	bool				m_is_fix_hurt_on_attack;		// 被打时是否固定伤害
	bool				m_is_forbid_recover;			// 禁止自动回血
	bool				m_is_forbid_recover_backup;		// 禁止自动回血备份

	int					m_auto_reover_per;				// 一直自动回血万分比
	int					m_auto_reover_time_gap;			//自动回血间隔时间（毫秒）

	MonsterAIBase		*m_ai;							// 怪物AI

	typedef std::map<ObjID, UserID> MonsterHurterMap;
	typedef std::map<ObjID, UserID>::iterator MonsterHurterMapIt;
	MonsterHurterMap m_monster_hurter_map;				// 伤害怪物的用户列表

	int					m_monster_key;					// 怪物KEY

	int					m_special_param;				// 特殊参数

	bool                 m_need_notice_to_hurt_role_when_die; // 参与击杀的玩家弹出击杀面板

	// 掉落归属相关 =================================================================================
	int					OWNER_MAX_DISTANCE;				// 玩家归属范围，与玩家距离超过范围丢失归属（设置为追踪范围，所以不能const）
	int					m_drop_owner_uid;				// 掉落归属者UID
	int					m_drop_owner_team_index;		// 掉落归属者所在队伍index
	time_t				m_drop_owner_team_next_check_time;	// 组队归属，下次检查时间
	time_t				m_drop_owner_team_last_force_check_time; // 组队归属，上一次强制检查的时间

	long long			m_drop_owner_hurt;				// 归属者或队伍当前伤害量

	typedef std::map<int, RoleHurtInfo> RoleHurtInfoMap;
	typedef std::map<int, RoleHurtInfo>::iterator RoleHurtInfoMapIt;
	RoleHurtInfoMap		m_role_hurt_info_map;			// 伤害排名列表
};

#endif

