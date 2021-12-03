#ifndef __ROLESHADOWN_AIBASE_HPP__
#define __ROLESHADOWN_AIBASE_HPP__

#include <map>
#include "ai/aibase.hpp"
#include "servercommon/challengefielddef.hpp"


#include "monster/monsterinitparam.h"
#include "obj/character/character.h"

#include "servercommon/servercommon.h"

class Role;

class RoleAIBase : public AIBase
{
	friend class RoleShadow;

public:
	RoleAIBase(Character *me, int aitype);
	virtual ~RoleAIBase();

	virtual void OnEnterScene() {}
	virtual void OnLeaveScene() {}
	virtual void OnAOIEnter(ObjID obj_id) {}
	virtual void OnAOILeave(ObjID obj_id) {}
	virtual int GetAIType() { return m_aitype; }

	virtual Posi GetBirthPos() { return m_birth_pos; }
	virtual void SetBirthPos(const Posi &posi) { m_birth_pos = posi; }

	ObjID GetEnemy() { return m_enemy_objid; }
	void ForceSetEnemy(ObjID obj_id);

	virtual void Init(const ChallengeParam &mp);

	virtual void OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp);
	virtual void Update(unsigned long interval, unsigned int now_second);

protected:
	virtual bool IsEnemy(Obj *obj);
	virtual bool IsFriend(Obj *obj, bool check_alive = true);

	void Follow(unsigned long interval);
	bool CanStartAttack();
	void ForceStartAttack();

	void InitCondSkillList();
	void SetNextSkill();
	bool PerformNextSkillFromList(int count, unsigned short skill_index_list[SkillListParam::MAX_SKILL_NUM]);
	bool PerformSkill(int skill_index, const Posi &pos, ObjID target_id, int check_flag = -1, int special_param = 0, int skill_data = 0);

	int m_aitype;										// AI类型
	Posi  m_birth_pos;									// 出生点

	ObjID m_enemy_objid;
	unsigned int m_current_runtime;
	unsigned int m_follow_interval;

	struct RoleCondSkill
	{
		RoleCondSkill() : count(0)
		{
			memset(skill_index_list, 0, sizeof(skill_index_list));
		}

		int count;
		unsigned short skill_index_list[SkillListParam::MAX_SKILL_NUM];
	};
	RoleCondSkill m_cond_skill_list[ROLE_SHADOW_SKILL_COND_MAX];

};

#endif // __MONSTER_AIBASE_HPP__

