#include "obj/character/monster.h"
#include "obj/character/role.h"
#include "ai/monsterai/specialai/monsteraicishacamp1.hpp"
#include "gamecommon.h"
#include "servercommon/campdef.hpp"

const static int MONSTER_AI_CISHA_CAMP_RAND_DIS = 10;

MonsterAiCishaCamp1::MonsterAiCishaCamp1(Character *me)
: MonsterAIMoveAround(me, MonsterInitParam::AITYPE_CISHA_CAMP1)
{

}

MonsterAiCishaCamp1::~MonsterAiCishaCamp1()
{

}

void MonsterAiCishaCamp1::AI(unsigned long interval)
{
	if (!CheckSpecialStatus())
	{
		return;
	}

	switch (m_me->GetLogicResult())
	{
	case Character::MOVE_STATIC:
	case Character::LOGIC_NONE:
	case Character::ATTACK_NONE:
	case Character::MOVE_RUN:
		if ((m_enemy == INVALID_OBJ_ID && !FindEnemy()) || IsOutRange())
		{
			if (m_enemy != INVALID_OBJ_ID)
			{
				m_rest_time = 0;		// 让其立刻回去 不然主动怪就会在该处休息 则可以在这段时间中不断攻击它
				m_enemy = INVALID_OBJ_ID;
				m_me->MoveStop();
			}
			MoveAround(interval);
		}
		else
		{
			if (CanStartAttack())
			{
				StartAttack(m_enemy, true);
			}
			else
			{
				Follow(m_enemy,interval);
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
			SetNextSkill(m_enemy);
		}
		break;
	case Character::ATTACK_OUTOFRANG:
		Follow(m_enemy, interval);
		break;
	}
}

void MonsterAiCishaCamp1::Init(const MonsterAIParam &mp)
{
	MonsterAIMoveAround::Init(mp);

	const static int RAND_POS_COUNT = 5;
	
	for (int i = 0; i < RAND_POS_COUNT; ++ i)
	{
		const Posi rand_pos = gamecommon::GetDisRandPos(m_birth_pos, MONSTER_AI_CISHA_CAMP_RAND_DIS);
		m_walk_pos.push_back(rand_pos);
	}
}

bool MonsterAiCishaCamp1::IsEnemy(Obj *obj)
{
	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;

		return role->GetCamp() != CAMP_TYPE_FEIXING && role->IsAlive();
	}

	return false;
}

bool MonsterAiCishaCamp1::IsFriend(Obj *obj, bool check_alive)
{
	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;
		return (!check_alive || role->IsAlive()) && role->GetCamp() == CAMP_TYPE_FEIXING && role->IsAlive();
	}

	return false;
}

bool MonsterAiCishaCamp1::FindEnemy()
{
	if (this->CheckHateDrive()) return true;

	this->OnEnemyLost();

	return false;
}

bool MonsterAiCishaCamp1::IsAttackWhenOnAttack(ObjID attacker)
{
	static const int MAX_ALLOW_DISTANT = 1 + static_cast<int>(sqrt(2 * MONSTER_AI_CISHA_CAMP_RAND_DIS * MONSTER_AI_CISHA_CAMP_RAND_DIS * 1.0));

	if ((m_birth_pos - m_me->GetPos()).IsLongerThan(MAX_ALLOW_DISTANT))
	{
		return false;
	}

	return INVALID_OBJ_ID == m_enemy;
}

