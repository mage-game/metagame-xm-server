#include "monsteraiaddmianshangbuff.hpp"
#include "config/logicconfigmanager.hpp"
#include "effect/skilleffect/effecthurtdec.hpp"
#include "scene/scene.h"
#include "obj/character/monster.h"

MonsterAIAddMianshanBuff::MonsterAIAddMianshanBuff(Character *me)
	: MonsterAIBase(me, MonsterInitParam::AITYPE_STATIC_ADD_MIANSHANG_BUFF), m_mianshang_per(0), m_only_effect_boss(false), next_add_buff_time(0)
{

}


MonsterAIAddMianshanBuff::~MonsterAIAddMianshanBuff()
{

}

void MonsterAIAddMianshanBuff::Init(const MonsterAIParam &mp)
{
	MonsterAIBase::Init(mp);
}

void MonsterAIAddMianshanBuff::AI(unsigned long interval)
{

}

void MonsterAIAddMianshanBuff::Update(unsigned long interval, unsigned int now_second)
{
	if (m_mianshang_per <= 0) return;

	if (Obj::OBJ_TYPE_MONSTER != m_me->GetObjType()) return;

	if (!m_me->IsAlive()) return;

	if (now_second < next_add_buff_time)
	{
		return;
	}

	next_add_buff_time = now_second + 600;

	Scene *scene = m_me->GetScene();
	if (NULL != scene)
	{
		UInt32 monster_num = scene->MonsterNum();
		for (unsigned int i = 0; i < monster_num; i++)
		{
			Monster *monster = scene->GetMonsterByIndex(i);
			if (NULL != monster)
			{
				Monster *ai_monster = (Monster *)m_me;
				if (monster->GetMonsterId() != ai_monster->GetMonsterId())
				{
					if (m_only_effect_boss && !monster->IsBossMonster())
					{
						continue;
					}

					EffectDecHurt *mianshang_effect = new EffectDecHurt(m_me->GetId(), 0, 86400000, 1);
					mianshang_effect->SetDecPercent(m_mianshang_per);
					mianshang_effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
					monster->AddEffect(mianshang_effect);
				}
			}
		}
	}
}

void MonsterAIAddMianshanBuff::OnDie(ObjID killer)
{
	MonsterAIBase::OnDie(killer);

	if (m_me->IsAlive()) return;

	Scene *scene = m_me->GetScene();
	if (NULL != scene)
	{
		UInt32 monster_num = scene->MonsterNum();
		for (unsigned int i = 0; i < monster_num; i++)
		{
			Monster *monster = scene->GetMonsterByIndex(i);
			if (NULL != monster)
			{
				EffectList *effect_list = monster->GetEffectList();
				if (NULL != effect_list)
				{
					effect_list->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_DEC_HURT, EBT_DEC_HURT, m_me->GetId());
				}
			}
		}
	}
}