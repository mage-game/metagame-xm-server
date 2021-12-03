#include "monsteraicoin.hpp"

#include "gamecommon.h"

#include "obj/character/monster.h"

MonsterAICoin::MonsterAICoin(Character *me)
: MonsterAIBase(me, MonsterInitParam::AITYPE_COIN),
m_cur_walk_pos(0), m_max_walk_pos(0), m_recalc_target_pos(false)
{

}

MonsterAICoin::~MonsterAICoin()
{

}

void MonsterAICoin::Init(const MonsterAIParam &mp)
{
	MonsterAIBase::Init(mp);
	m_cur_walk_pos = 0;
}

void MonsterAICoin::SetMovePath(std::vector<Posi> &path)
{
	if (static_cast<int>(path.size()) <= 0)
	{
		return;
	}

	m_walk_pos.clear();
	m_cur_walk_pos = 0;
	m_max_walk_pos = static_cast<int>(path.size()) - 1;

	for (int i = 0; i <= m_max_walk_pos; ++ i)
	{
		Posi &pos = path[i];
		m_walk_pos.push_back(Posi(pos.x, pos.y));
	}
};

void MonsterAICoin::AI(unsigned long interval)
{
	if (!CheckSpecialStatus())
	{
		return;
	}

	if (m_me->IsXuanYun() || m_me->IsDingShen())
	{
		m_recalc_target_pos = true;
		return;
	}

	if (m_recalc_target_pos)  // 在走路过程中被打断时，可以重新走时需要计算位置
	{
		m_recalc_target_pos = false;
		-- m_cur_walk_pos;
		if (m_cur_walk_pos < 0)
		{
			m_cur_walk_pos = 0;
		}
	}

	switch (m_me->GetLogicResult())
	{
	case Character::MOVE_STATIC:
	case Character::LOGIC_NONE:
	case Character::MOVE_RUN:
		this->GotoNextPosi();
		break;
	}
}

void MonsterAICoin::GotoNextPosi()
{
	if (m_cur_walk_pos >= static_cast<int>(m_walk_pos.size()))
	{
		return;
	}

	if (0 == m_cur_walk_pos || m_me->IsStatic()) // 到达 停止行走
	{
		if (m_cur_walk_pos >= m_max_walk_pos)
		{
			Monster *monster = (Monster *)m_me;
			monster->ForceSetDead();
		}
		else
		{
			++ m_cur_walk_pos;
			Posi deltaP = m_walk_pos[m_cur_walk_pos] - m_me->GetPos();
			Scalar distance = 0;
			Dir dir = gamecommon::GetMoveDir(deltaP, &distance);
			m_me->SetDir(dir, distance);
		}
	}
}
