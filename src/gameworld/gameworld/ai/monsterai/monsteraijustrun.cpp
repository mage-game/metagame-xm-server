#include "monsteraijustrun.hpp"

MonsterAIJustRun::MonsterAIJustRun(Character *me)
	: MonsterAIMoveAround(me, MonsterInitParam::AITYPE_JUST_RUN)
{

}

MonsterAIJustRun::~MonsterAIJustRun()
{

}

void MonsterAIJustRun::AI(unsigned long interval)
{
	switch (m_me->GetLogicResult())
	{
	case Character::MOVE_STATIC:
	case Character::LOGIC_NONE:
	case Character::MOVE_RUN:
		this->MoveAround(interval);
		break;
	}
}

