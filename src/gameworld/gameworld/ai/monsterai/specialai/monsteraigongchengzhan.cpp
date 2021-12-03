#include "monsteraigongchengzhan.hpp"

#include "obj/character/role.h"
#include "scene/speciallogic/worldspecial/specialgongchengzhan.hpp"

MonsterAIGongchengzhan::MonsterAIGongchengzhan(Character *me)
	: MonsterAIStaticInitiative(me, MonsterInitParam::AITYPE_SPECIALAI_GONGCHENGZHAN), m_site(-1)
{

}


MonsterAIGongchengzhan::~MonsterAIGongchengzhan()
{

}

void MonsterAIGongchengzhan::Init(const MonsterAIParam &mp)
{
	MonsterAIStaticInitiative::Init(mp);
}

void MonsterAIGongchengzhan::AI(unsigned long interval)
{
	if (!CheckSpecialStatus())
	{
		return;
	}

	switch (m_me->GetLogicResult())
	{
	case Character::MOVE_STATIC:
	case Character::LOGIC_NONE:
	case Character::MOVE_RUN:
	case Character::ATTACK_NONE:
		if (m_enemy != INVALID_OBJ_ID || FindEnemy(interval))
		{
			if (CanStartAttack())
			{
				StartAttack(m_enemy, true);
			}
			else 
			{
				m_me->StopAttack();
			}
		}
		break;
	case Character::ATTACK_DONE:
	case Character::ATTACK_COOLDOWN:
	case Character::ATTACK_SPECIAL_STATE:
		if (CanStartAttack())				//	ս����˧ ��Ĭ�ϼ���ֻ��һ��Ⱥ�� ��CD=0 �᲻��SetNextSkillʩ�� ��ʹenemy����
		{
			SetNextSkill(m_enemy);
		}
		break;
	case Character::ATTACK_OUTOFRANG:
		{
			m_me->StopAttack();
			m_enemy = INVALID_OBJ_ID;
		}
		break;
	case Character::ACTION_PLAYING:
		break;
	default:
		break;
	}
}

bool MonsterAIGongchengzhan::IsEnemy(Obj *obj)
{
	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;

		SpecialGongChengZhan *gongchengzhan_logic = dynamic_cast<SpecialGongChengZhan*>(m_me->GetScene()->GetSpecialLogic());
		if (NULL != gongchengzhan_logic && Scene::SCENE_TYPE_GONGCHENGZHAN == m_me->GetScene()->GetSceneType())
		{
			return gongchengzhan_logic->GetRoleSite(role) != m_site && role->IsAlive();
		}
	}

	return false;
}

bool MonsterAIGongchengzhan::IsFriend(Obj *obj, bool check_alive)
{
	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;

		SpecialGongChengZhan *gongchengzhan_logic = dynamic_cast<SpecialGongChengZhan*>(m_me->GetScene()->GetSpecialLogic());
		if (NULL != gongchengzhan_logic && Scene::SCENE_TYPE_GONGCHENGZHAN == m_me->GetScene()->GetSceneType())
		{
			return gongchengzhan_logic->GetRoleSite(role) == m_site && role->IsAlive();
		}
	}

	return false;
}