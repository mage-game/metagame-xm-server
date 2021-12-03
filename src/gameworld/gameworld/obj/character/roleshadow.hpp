#ifndef __ROLE_SHADOW_HPP__
#define __ROLE_SHADOW_HPP__

#include "character.h"

#include "servercommon/challengefielddef.hpp"

#include "skill/skilldef.hpp"
#include <set>
#include "servercommon/crossdef.hpp"
#include "ai/roleai/roleaibase.hpp"

class Role;

enum ROLE_SHADOW_TYPE
{
	ROLE_SHADOW_TYPE_INVALID = -1,
	ROLE_SHADOW_TYPE_FOOL,					// 静止机器人 木头人
	ROLE_SHADOW_TYPE_CHALLENGE_FIELD,		// 竞技场
	ROLE_SHADOW_THPE_WORLD_EVENT,			// 世界事件
	ROLE_SHADOW_TYPE_ROLE_BOSS,				// 角色boss
	ROLE_SHADOW_TYPE_CAMPDEFEND,			// 守卫雕像
	ROLE_SHADOW_TYPE_ELEMENT_FILED,			// 元素战场
	ROLE_SHADOW_TYPE_CLONE_ROLE,			// 玩家分身

	ROLE_SHADOW_TYPE_MAX
};

struct HurtRoleShadowInfo
{
	HurtRoleShadowInfo() : uid(0), hurt(0) {}

	int uid;
	long long hurt;
};

class RoleShadow : public Character
{
public:
	RoleShadow(int role_shadow_type);
	virtual ~RoleShadow();

	void Init(ChallengeParam &param, RoleAppearance &appearance, Attribute hp, Attribute mp);

	int GetRoleShadowType();
	RoleAIBase * GetRoleAI() { return m_ai; }
	int GetPlatType() const { return m_plat_type; }
	const UserID & GetUserId() const { return m_user_id; }
	virtual int GetLevel() { return m_level; }
	char GetAvatar() const { return m_avatar; }
	char GetSex() const { return m_sex; }
	char GetCamp() const { return m_camp; }
	char GetProf() const { return m_prof; }
	int GetCapability() const { return m_capability; }
	char GetVipLevel() const { return m_vip_level; }
	char GetGuildPost() const { return m_guild_post; }
	GuildID GetGuildID() const { return m_guild_id; }
	void GetGuildName(GuildName guild_name);
	long long GetAvatarTimestamp() { return m_avatar_timestamp; }
	const RoleAppearance & GetAppearance() { return m_appearance; }

	UniqueUserID GetUniqueUserID() const { return UniqueUserID(m_plat_type, m_user_id); }

	virtual	void Update(unsigned long interval, time_t now_second, unsigned int now_dayid);
	void CalcAI(unsigned long interval);

	void SetBirthPos(const Posi &birth_pos);

	virtual void OnEnterScene();
	virtual void OnLeaveScene(); 
	virtual void OnAOIEnter(ObjID obj_id);
	virtual void OnAOILeave(ObjID obj_id);

	virtual bool IsEnemy(Obj *obj);
	virtual bool IsFriend(Obj *obj, bool check_alive = true);

	virtual void OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp, bool from_skill);
	virtual void OnDie(ObjID killer);
	virtual bool MoveBySkill(Axis _x, Axis _y, Dir _dir, Scalar distance, bool check_special_state = true);

	void *operator new(size_t c);
	void operator delete(void *m);

	void ForceSetEnemy(ObjID enemy_objid);

	virtual int GetHurtIncPercent(Character *target);
	virtual int GetHurtDecPercent(Character *performer);

	bool GetTopHurtUid(int *uid, const std::set<int> &filter_uid);
	int RandGetHurtUid(int uid_count, int *uid_list, const std::set<int> &filter_uid);
	long long GetHurtByUid(int uid);
	void GetHurtInfo(std::vector<HurtRoleShadowInfo> &hurt_vector);

	void SetSpecialParam(int param) { m_special_param = param; }
	int GetSpecialParam() { return m_special_param; }

protected:
	virtual bool CheckSkillRangeUseDefaultSkill();

	void CheckSoulSkill();
	int GetSoulSkillHurtIncPercent(Character *target);
	int GetSoulSkillHurtDecPercent(Character *performer);
	
	RoleAIBase *m_ai;					// 机器人AI

	long long m_high_time_reg_id;

	int m_role_shadow_type;

	int m_plat_type;
	UserID m_user_id;
	int m_level;
	char m_avatar;
	char m_sex;
	char m_camp;
	char m_prof;
	int m_capability;
	long long m_avatar_timestamp;

	char m_vip_level;

	char m_guild_post;
	GuildID m_guild_id;
	GuildName m_guild_name;

	RoleAppearance m_appearance;

	// 仙剑技能相关
	int m_hurt_monster_inc_percent;
	int m_hurt_role_inc_percent;
	int m_hurt_building_inc_percent;

	int m_monster_hurt_dec_percent;
	int m_role_hurt_dec_percent;

	int m_absorb_percent;
	int m_absorb_regular;

	int m_once_hurt_inc_percent;
	int m_protect_gather_maxhp_percent;
	int m_gather_speed_up_percent;

	time_t m_soul_skill_last_perform_time_list[MAX_XIANJIAN_SOUL_SKILL_SLOT_COUNT];
	
	Posi m_birth_pos;

	struct HurtInfo
	{
		HurtInfo() : hurt(0), last_hurt_timestamp(0) {}

		long long hurt;
		time_t last_hurt_timestamp;
	};

	typedef std::map<int, HurtInfo> HurInfotMap;
	typedef std::map<int, HurtInfo>::iterator HurInfotMapIt;

	HurInfotMap m_hurt_info_map;

	bool m_is_addtimer;

	int m_special_param;
};

#endif // __ROLE_SHADOW_HPP__

