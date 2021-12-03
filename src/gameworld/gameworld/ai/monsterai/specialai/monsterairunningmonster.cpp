#include "monsterairunningmonster.hpp"
#include "gamecommon.h"

MonsterAIRunningMonster::MonsterAIRunningMonster(Character *me)
	: MonsterAIMoveAround(me, MonsterInitParam::AITYPE_BUILD_TOWER_RUNNING)
{
	m_init_rest_time = 0;
	m_rest_time = 0;
	m_rest_deviation_time = 0;
}

MonsterAIRunningMonster::~MonsterAIRunningMonster()
{

}

void MonsterAIRunningMonster::Init(const MonsterAIParam &mp)
{
	MonsterAIBase::Init(mp);
	m_cur_walk_pos = 0;
	m_walk_state = true;
}

void MonsterAIRunningMonster::SetWalkPosList(int pos_count, Posi walk_pos_list[MAX_RUNNING_MONSTER_AI_RUNNING_STEP])
{
	if (pos_count <= 0)
	{
		return;
	}

	for (int i = 0; i < pos_count; ++i)
	{
		m_walk_pos.push_back(walk_pos_list[i]);
	}
}

void MonsterAIRunningMonster::AI(unsigned long interval)
{
	if (!CheckSpecialStatus())
	{
		return;
	}

	if (m_me->IsXuanYun() || m_me->IsDingShen())
	{
		return;
	}

	switch (m_me->GetLogicResult())
	{
	case Character::MOVE_STATIC:
	case Character::LOGIC_NONE:
	case Character::MOVE_RUN:
		this->MoveAround(interval);
		break;
	}
}

void MonsterAIRunningMonster::MoveAround(unsigned long interval)
{
	if (m_walk_pos.size() <= 0) return;

	if (m_walk_pos[m_cur_walk_pos] == m_me->GetPos()) // 到达 停止行走
	{
		if (m_cur_walk_pos < m_walk_pos.size() - 1)
		{
			m_cur_walk_pos++;
		}
	}

	Posi deltaP = m_walk_pos[m_cur_walk_pos] - m_me->GetPos();
	Scalar distance = 0;
	Dir dir = gamecommon::GetMoveDir(deltaP, &distance);
	m_me->SetDir(dir, distance);
}

bool MonsterAIRunningMonster::IsAtWalkEndPos()
{
	return m_me->IsStatic() && m_walk_pos.size() > 0 && m_cur_walk_pos >= m_walk_pos.size() - 1 && m_walk_pos[m_cur_walk_pos] == m_me->GetPos();
}



