#include "specialshenmozhixi.hpp"
#include "obj/character/role.h"
#include "global/worldevent/worldeventconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "obj/otherobj/fallingitem.h"
#include "gameworld/item/itempool.h"
#include "engineadapter.h"

SpecialShenmozhixi::SpecialShenmozhixi(Scene *scene) : SpecialLogic(scene), m_is_finish(false), kick_user_time(0), m_user_objid(INVALID_OBJ_ID)
{

}

SpecialShenmozhixi::~SpecialShenmozhixi() 
{

}

void SpecialShenmozhixi::OnRoleEnterScene(Role *role)
{
	if (NULL != role)
	{
		m_user_objid = role->GetId();
	}
}

void SpecialShenmozhixi::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (!m_is_finish)
	{
		this->CheckSceneMonsterStatus(interval, now_second);
	}
	else
	{
		if (0 != kick_user_time && now_second >= kick_user_time)
		{
			kick_user_time = 0;

			Role *role = m_scene->GetPrincipal(m_user_objid);
			if (NULL != role)
			{
	
			}

			this->DelayKickOutAllRole();
		}
	}
}

void SpecialShenmozhixi::OnAllMonsterDead()
{
	m_is_finish = true;

	kick_user_time = EngineAdapter::Instance().Time() + 3;
}



