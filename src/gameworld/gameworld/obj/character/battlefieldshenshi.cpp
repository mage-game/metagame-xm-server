#include "battlefieldshenshi.hpp"

#include "engineadapter.h"

#include "scene/scene.h"

#include "obj/character/role.h"
#include "servercommon/activitydef.hpp"
#include "protocal/msgactivity.hpp"

#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/qunxianluandouconfig.hpp"

#include "other/roleactivity/roleactivity.hpp"

BattleFieldShenShi::BattleFieldShenShi()
	: Character(OBJ_TYPE_BATTLEFIELD_SHENSHI), m_owner_obj_id(INVALID_OBJ_ID), m_owner_user_id(INVALID_USER_ID)
{

}

BattleFieldShenShi::~BattleFieldShenShi()
{

}

void BattleFieldShenShi::Init(Attribute hp)
{
	CharIntAttrs int_attribute;
	int_attribute.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP] = hp;
	int_attribute.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP] = hp;

	SkillListParam skill_param; skill_param.count = 0;

	Character::Init(int_attribute, skill_param); 
}

void BattleFieldShenShi::Update(unsigned long interval, time_t now_second, unsigned int now_dayid)
{
	if (!this->IsAlive())
	{
		if (this->HaveOwner())
		{
			Obj *owner_obj = m_scene->GetObj(m_owner_obj_id);
			if (NULL != owner_obj && Obj::OBJ_TYPE_ROLE == owner_obj->GetObjType())
			{
				Role *owner_role = (Role*)owner_obj;
				if (owner_role->GetUserId() == m_owner_user_id)
				{
					if ((this->GetPos() - owner_role->GetPos()).IsLongerThan(BATTLEFIELD_SHENSHI_MAX_OWNER_DISTENCE))
					{
						this->Move(owner_role->GetPos().x, owner_role->GetPos().y, Dir_S, 0, 0, false);

						Posi dest_pos(0, 0);
						if (LOGIC_CONFIG->GetQunXianLuanDouConfig().GetRealivePos(owner_role->GetRoleActivity()->GetTempZhanchangSide(), &dest_pos))
						{
							if (!(m_posi - dest_pos).IsLongerThan(BATTLEFIELD_SHENSHI_DEST_POINT_DISTANCE))
							{
								m_scene->DeleteObj(m_obj_id);
							}
						}
					}

					return;
				}
			}
		}

		m_owner_obj_id = INVALID_OBJ_ID; m_owner_user_id = INVALID_USER_ID;
	}
	else
	{
		Character::Update(interval, now_second, now_dayid);
	}
}

void BattleFieldShenShi::OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill)
{

}

void BattleFieldShenShi::OnDie(ObjID killer_objid)
{
	Character::OnDie(killer_objid);
}

void BattleFieldShenShi::SetOwner(ObjID owner_objid, const UserID &owner_user_id)
{
	m_owner_obj_id = owner_objid; m_owner_user_id = owner_user_id;

	Protocol::SCQunXianLuanDouShenShiStatus qxldss;
	qxldss.has_owner = this->HaveOwner() ? 1 : 0;
	qxldss.obj_id = m_obj_id;
	qxldss.owner_obj_id = m_owner_obj_id;
	qxldss.owner_uid = UidToInt(m_owner_user_id);

	if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&qxldss, sizeof(qxldss));
}

