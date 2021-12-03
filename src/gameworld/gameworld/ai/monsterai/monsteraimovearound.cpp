#include "monsteraimovearound.hpp"

#include "gamecommon.h"

#include "obj/character/monster.h"
#include "scene/scene.h"
#include "scene/map.h"

MonsterAIMoveAround::MonsterAIMoveAround(Character *me, int aitype)
	: MonsterAIBase(me, aitype),
	m_cur_walk_pos(0),
	m_walk_state(true),
	m_rest_time(1000),
	m_init_rest_time(1000),
	m_rest_deviation_time(1000)
{

}

MonsterAIMoveAround::~MonsterAIMoveAround()
{

}

void MonsterAIMoveAround::Init(const MonsterAIParam &mp)
{
	MonsterAIBase::Init(mp);
	m_cur_walk_pos = 0;
	m_walk_state = true;

	m_walk_pos.push_back(m_birth_pos);

	if (0 != mp.walk_range)
	{
		for(unsigned int i = 0; i < mp.walk_point_num; ++i)
		{
			m_walk_pos.push_back(Posi(m_birth_pos.x - mp.walk_range + RandomNum(mp.walk_range * 2), m_birth_pos.y - mp.walk_range + RandomNum(mp.walk_range * 2)));
		}
	}
}

void MonsterAIMoveAround::MoveAround(unsigned long interval)
{
	if (m_walk_state)
	{
 		if (m_walk_pos[m_cur_walk_pos] == m_me->GetPos()) // 到达 停止行走
		{
			m_walk_state = false;

			if ((int)m_walk_pos.size() > 0)
			{
				int walk_pos = RandomNum((int)m_walk_pos.size());
				if (!m_me->GetScene()->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, m_walk_pos[walk_pos].x, m_walk_pos[walk_pos].y))
				{
					return;
				}
				m_cur_walk_pos = walk_pos;
			}
			m_rest_time = m_init_rest_time;
			if(m_rest_deviation_time != 0)
			{
				m_rest_time += RandomNum(m_rest_deviation_time);
			}
		}

		if (m_walk_state && m_me->IsStatic())
		{
			Posi deltaP = m_walk_pos[m_cur_walk_pos] - m_me->GetPos();
			Scalar distance = 0;
			Dir dir = gamecommon::GetMoveDir(deltaP, &distance);
			m_me->SetDir(dir, distance);
		}
	}
	else
	{
		m_rest_time -= (int)interval;
		if (m_rest_time < 0)
		{
			m_walk_state = true;
		}
	}
}

