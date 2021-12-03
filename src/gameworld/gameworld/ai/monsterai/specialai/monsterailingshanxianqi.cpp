#include "monsterailingshanxianqi.hpp"

#include "obj/character/role.h"

#include "scene/scene.h"

#include "gamecommon.h"

MonsterAILingShanXianQi::MonsterAILingShanXianQi(Character *me)
	: MonsterAIBase(me, MonsterInitParam::AITYPE_SPECIALAI_LINGSHANXIANQI), m_owner_user_id(INVALID_USER_ID), m_owner_user_objid(INVALID_OBJ_ID)
{

}

MonsterAILingShanXianQi::~MonsterAILingShanXianQi()
{

}

void MonsterAILingShanXianQi::Init(const MonsterAIParam &mp)
{
	MonsterAIBase::Init(mp);
}

void MonsterAILingShanXianQi::SetOwnerInfo(const UserID &user_id, ObjID user_objid)
{
	m_owner_user_id = user_id;
	m_owner_user_objid = user_objid;
}

void MonsterAILingShanXianQi::ClearOwnerInfo()
{
	m_owner_user_id = INVALID_USER_ID;
	m_owner_user_objid = INVALID_OBJ_ID;
}

void MonsterAILingShanXianQi::AI(unsigned long interval)
{
	this->FollowRole(interval);
}

Role * MonsterAILingShanXianQi::GetFollowRole()
{
	if (NULL == m_me->GetScene() || INVALID_OBJ_ID == m_owner_user_objid) return NULL;

	Obj *obj = m_me->GetScene()->GetObj(m_owner_user_objid);
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;
		if (m_owner_user_id == role->GetUserId())
		{
			return role;
		}
	}

	m_owner_user_objid	= INVALID_OBJ_ID;

	return NULL;
}

void MonsterAILingShanXianQi::FollowRole(unsigned long interval)
{
	Role *role = this->GetFollowRole();
	if (NULL == role) return;

	if (!(m_me->GetPos() - role->GetPos()).IsLongerThan(BESIDE_ROLE_LENGTH))
	{
		return;
	}

	if ((m_me->GetPos() - role->GetPos()).IsLongerThan(JUMP_TO_ROLE_DISTANCE))
	{
		Posi fly_pos = gamecommon::GetDisRandPos(role->GetPos(), BESIDE_ROLE_LENGTH);
		m_me->Move(fly_pos.x, fly_pos.y, Dir_S, 0, 0);

		return;
	}

	if (m_me->IsStatic())
	{
		Posi role_pos = role->GetPos();
		Posi deltaP = gamecommon::GetDisRandPos(role_pos, BESIDE_ROLE_LENGTH) - m_me->GetPos();
		Scalar distance = 0;
		Dir dir = gamecommon::GetMoveDir(deltaP, &distance);
		m_me->SetDir(dir, distance);
		m_current_runtime = 0;

		return;
	}

	m_current_runtime += interval;

	if ((int)m_current_runtime > FOLLOW_ROLE_INTERVAL)
	{
		Posi role_pos = role->GetPos();
		Posi deltaP = gamecommon::GetDisRandPos(role_pos, BESIDE_ROLE_LENGTH) - m_me->GetPos();
		Scalar distance = 0;
		Dir dir = gamecommon::GetMoveDir(deltaP, &distance);
		if (dir != m_me->GetDir())
		{
			m_me->SetDir(dir, distance);
		}
		m_current_runtime = 0;
	}
}

