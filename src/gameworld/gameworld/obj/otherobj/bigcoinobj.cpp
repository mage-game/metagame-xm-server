#include "bigcoinobj.hpp"

#include "servercommon/errornum.h"

#include "gamecommon.h"
#include "engineadapter.h"

#include "scene/scene.h"
#include "scene/speciallogic/speciallogic.hpp"

#include "obj/character/role.h"

#include "protocal/msgscene.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "item/knapsack.h"

BigCoinObj::BigCoinObj() 
	: Obj(OBJ_TYPE_BIG_COIN), m_value(0), m_picker_objid(INVALID_OBJ_ID), m_has_pick(false), m_type(0),
	  m_disappear_time(0)
{

}

BigCoinObj::~BigCoinObj()
{

}

void BigCoinObj::Init(char type, long long value, int disappear_s)
{
	m_type = type;
	m_value = value;

	if (disappear_s > 0)
	{
		m_disappear_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + disappear_s;
	}	
}

void BigCoinObj::Pick(Role *role)
{
	if (m_has_pick) return;

	if (!role->IsAlive())
	{
		role->NoticeNum(errornum::EN_FALLING_ROLE_DEAD);
		return;
	}

	if ((role->GetPos()-m_posi).IsLongerThan(6))
	{
		return;
	}

	if (BIGCOIN_TYPE_HONGBAO == m_type && !role->GetKnapsack()->HaveSpace())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_picker_objid = role->GetId();

	switch (m_type)
	{
	case BIGCOIN_TYPE_BINDCOIN:
		{
			role->GetKnapsack()->GetMoney()->AddCoinBind(m_value, "PickBigCoin");
		}
		break;

	case BIGCOIN_TYPE_EXP:
		{
			role->AddExp(m_value, "PickBigCoin", Role::EXP_ADD_REASON_DEFAULT);
		}
		break;

	default:
		break;
	}

	m_scene->GetSpecialLogic()->OnRolePickBigCoin(role, this);
	ActivityShadow::Instance().OnRolePickBigCoin(role, this);

	Protocol::SCPickBigCoinSucc pbcs;
	pbcs.obj_id = m_obj_id;
	pbcs.reserved = 0;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&pbcs, sizeof(pbcs));

	m_has_pick = true;
	m_scene->DeleteObj(m_obj_id);
}

void BigCoinObj::Update(unsigned long interval, time_t now_second, unsigned int now_dayid)
{
	if (NULL == m_scene) return;

	if (0 != m_disappear_time && m_disappear_time < now_second && !m_has_pick)
	{
		m_has_pick = true;
		m_scene->DeleteObj(m_obj_id);
	}
}

