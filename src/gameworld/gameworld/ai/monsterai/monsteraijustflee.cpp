#include "monsteraijustflee.hpp"
#include "gamecommon.h"
#include "scene/scene.h"
#include "scene/map.h"

MonsterAIJustFlee::MonsterAIJustFlee(Character *me)
	: MonsterAIBase(me, MonsterInitParam::AITYPE_JUST_FLEE)
{

}

MonsterAIJustFlee::~MonsterAIJustFlee()
{

}

void MonsterAIJustFlee::AI(unsigned long interval)
{
	if (m_me->IsStatic() && !m_me->GetScene()->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, m_me->GetPos().x, m_me->GetPos().y))
	{
		if (m_birth_pos != m_me->GetPos())
		{
			Posi deltaP = m_birth_pos - m_me->GetPos();

			Scalar distance = 0;
			Dir dir = gamecommon::GetMoveDir(deltaP, &distance);

			m_me->SetDir(dir, distance);
		}
	}
}

void MonsterAIJustFlee::OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp)
{
	MonsterAIBase::OnAttack(attacker, skill_id, delta_hp);

	if (m_me->IsStatic())
	{
		Obj *attacker_obj = m_me->GetScene()->GetObj(attacker);
		if (NULL == attacker_obj)
		{
			return;
		}

		static const int MOVE_OFFSET_MIN = 3;

		const Posi &pos_me = m_me->GetPos();
		Posi pos_begin(0, 0), pos_end(0, 0), reverse_offset(0, 0);

		Posi deltaP = pos_me - attacker_obj->GetPos();
		if (deltaP.x >= deltaP.y && deltaP.x >= -deltaP.y)
		{
			// Right
			pos_begin.x = pos_me.x + MOVE_OFFSET_MIN;
			pos_begin.y = pos_me.y - m_run_range;
			pos_end.x = pos_me.x + m_run_range;
			pos_end.y = pos_me.y + m_run_range;

			reverse_offset.x = - 2 * MOVE_OFFSET_MIN - m_run_range;
		}
		else if (-deltaP.x >= deltaP.y && -deltaP.x >= -deltaP.y)
		{
			// Left
			pos_begin.x = pos_me.x - m_run_range;
			pos_begin.y = pos_me.y - m_run_range;
			pos_end.x = pos_me.x - MOVE_OFFSET_MIN;
			pos_end.y = pos_me.y + m_run_range;

			reverse_offset.x = 2 * MOVE_OFFSET_MIN + m_run_range;
		}
		else if (-deltaP.y >= deltaP.x && -deltaP.y >= -deltaP.x)
		{
			// Down
			pos_begin.x = pos_me.x - m_run_range;
			pos_begin.y = pos_me.y - m_run_range;
			pos_end.x = pos_me.x + m_run_range;
			pos_end.y = pos_me.y - MOVE_OFFSET_MIN;

			reverse_offset.y = 2 * MOVE_OFFSET_MIN + m_run_range;
		}
		else
		{
			// Up
			pos_begin.x = pos_me.x - m_run_range;
			pos_begin.y = pos_me.y + MOVE_OFFSET_MIN;
			pos_end.x = pos_me.x + m_run_range;
			pos_end.y = pos_me.y + m_run_range;

			reverse_offset.y = - 2 * MOVE_OFFSET_MIN - m_run_range;
		}

		if (pos_end.x > pos_begin.x && pos_end.y > pos_begin.y)
		{
			for (int i = 0; i < 30; i++)
			{
				Posi pos = pos_begin;
				pos.x = RandomNum(pos_begin.x, pos_end.x);
				pos.y = RandomNum(pos_begin.y, pos_end.y);
				if (i > 20)
				{
					pos.x += reverse_offset.x;
					pos.y += reverse_offset.y;
				}

				if (m_me->GetScene()->GetMap()->IsValidatePos(Obj::OBJ_TYPE_ROLE, pos.x, pos.y))
				{
					deltaP = pos - pos_me;
					Scalar distance = 0;

					Dir dir = gamecommon::GetMoveDir(deltaP, &distance);
					m_me->SetDir(dir, distance);
					break;
				}
			}
		}
	}
}
