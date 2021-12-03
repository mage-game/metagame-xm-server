#include "roleaielementfield.hpp"
#include "obj/character/roleshadow.hpp"
#include "gamecommon.h"
#include "scene/scene.h"
#include "other/roleactivity/roleactivity.hpp"
#include "globalconfig/globalconfig.h"

RoleAIElementField::RoleAIElementField(Character *me)
	: RoleAIBase(me, ROLE_SHADOW_TYPE_ELEMENT_FILED), m_side(0), m_aoi_handle(-1)
{

}

RoleAIElementField::~RoleAIElementField()
{

}

void RoleAIElementField::AI(unsigned long interval)
{
	switch (m_me->GetLogicResult())
	{
	case Character::MOVE_STATIC:
	case Character::ATTACK_NONE:
	case Character::LOGIC_NONE:
	case Character::MOVE_RUN:
		if (INVALID_OBJ_ID == m_enemy_objid)
		{
			this->GoBack();
		}
		else
		{
			if (this->CanStartAttack())
			{
				this->ForceStartAttack();
			}
			else
			{
				this->Follow(interval);
			}
		}
		break;

	case Character::ACTION_PLAYING:
		break;
	case Character::ATTACK_DONE:
	case Character::ATTACK_COOLDOWN:
	case Character::ATTACK_SPECIAL_STATE:
		if (this->CanStartAttack())
		{
			this->SetNextSkill();
		}
		break;

	case Character::ATTACK_OUTOFRANG:
		this->Follow(interval);
		break;
	}
}

void RoleAIElementField::OnEnterScene()
{
	// role关注区域
	static const int CLIENT_SCREEN_X = (int)GLOBALCONFIG->GetKeyConfig_RoleAoiX();			// 真实屏幕50 该数字用于计算Role的AOI进入范围
	static const int CLIENT_SCREEN_Y = (int)GLOBALCONFIG->GetKeyConfig_RoleAoiY();			// 真实屏幕30
	static const int ROLE_AOI_LEAVE_MORE_THEN_ENTER = (int)GLOBALCONFIG->GetKeyConfig_RoleAoiLeaveMoreThenEnter();	// Role的AOI离开大于进入范围的值

	int screen_x = CLIENT_SCREEN_X;
	int screen_y = CLIENT_SCREEN_Y;

	m_aoi_handle = m_me->GetScene()->GetZoneMatrix()->CreateRectAOI(m_me->GetId(), m_me->GetPos(), Posi(screen_x / 2, screen_y / 2),
		Posi(screen_x / 2 + ROLE_AOI_LEAVE_MORE_THEN_ENTER, screen_y / 2 + ROLE_AOI_LEAVE_MORE_THEN_ENTER));
}

void RoleAIElementField::OnLeaveScene()
{
	m_me->GetScene()->GetZoneMatrix()->DestroyAOI(m_aoi_handle);
}

void RoleAIElementField::OnAOILeave(ObjID obj_id)
{
	if (obj_id == m_enemy_objid)
	{
		m_enemy_objid = INVALID_OBJ_ID;
	}
}

void RoleAIElementField::OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp)
{
	if (Character::LOGIC_NONE > m_me->GetLogicResult() || Character::ATTACK_OUTOFRANG <= m_me->GetLogicResult()) return;

	m_enemy_objid = attacker;
}

bool RoleAIElementField::IsEnemy(Obj *obj)
{
	if (NULL != obj)
	{
		if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
		{
			Role *role = (Role*)obj;
			if (role->GetRoleActivity()->GetTempZhanchangSide() != m_side)
			{
				return true;
			}
		}
		else if (Obj::OBJ_TYPE_ROLE_SHADOW == obj->GetObjType())
		{
			RoleShadow *role_shadow = (RoleShadow*)obj;
			if (ROLE_SHADOW_TYPE_ELEMENT_FILED == role_shadow->GetRoleShadowType())
			{
				if (NULL != role_shadow->GetRoleAI())
				{
					RoleAIElementField *ai = (RoleAIElementField*)role_shadow->GetRoleAI();
					if (ai->GetSide() != m_side)
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool RoleAIElementField::FindEnemy(unsigned long interval)
{
	if (INVALID_OBJ_ID != m_enemy_objid) return false;

	int obj_num = 0;
	m_me->GetScene()->GetSceneStatus(&obj_num, NULL, NULL, NULL, NULL);
	if (obj_num > 0)
	{
		for (int i = 0; i < 200; i++)
		{
			int rand_obj_id = RandomNum(obj_num);
			Obj *rand_obj = m_me->GetScene()->GetObj(rand_obj_id);
			if (NULL != rand_obj && this->IsEnemy(rand_obj))
			{
				m_enemy_objid = rand_obj_id;
				return true;
			}
		}
	}

	return false;
}

void RoleAIElementField::GoBack()
{
	Posi deltaP = m_birth_pos - m_me->GetPos();
	int m = static_cast<int>(deltaP.x * deltaP.x + deltaP.y * deltaP.y);
	if (m > 3600)
	{
		Scalar distance = 0;
		Dir dir = gamecommon::GetMoveDir(deltaP, &distance);
		m_me->SetDir(dir, distance);
	}
	else
	{
		this->FindEnemy(0);
	}
}


