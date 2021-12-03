#include "monsteraiaddrecoverbuff.hpp"
#include "config/logicconfigmanager.hpp"
#include "effect/skilleffect/effectinterval.hpp"
#include "scene/scene.h"
#include "obj/character/monster.h"

MonsterAIAddRecoverBuff::MonsterAIAddRecoverBuff(Character *me)
: MonsterAIBase(me, MonsterInitParam::AITYPE_STATCI_ADD_RECOVER_BUFF), m_only_effect_boss(false), m_recover_by_attack(false), m_pass_interval(0), m_hp_recover_per(0), m_hp_recover_interval(0)
{

}


MonsterAIAddRecoverBuff::~MonsterAIAddRecoverBuff()
{

}

void MonsterAIAddRecoverBuff::Init(const MonsterAIParam &mp)
{
	MonsterAIBase::Init(mp);
}

void MonsterAIAddRecoverBuff::AI(unsigned long interval)
{
	if (!m_recover_by_attack) return;

	if (!CheckSpecialStatus())
	{
		return;
	}

	switch (m_me->GetLogicResult())
	{
	case Character::MOVE_STATIC:
	case Character::ATTACK_NONE:
	case Character::LOGIC_NONE:
	case Character::MOVE_RUN:
		{
			if (FindEnemy())
			{
				if ( (m_enemy != INVALID_OBJ_ID))
				{
					if (CanStartAttack())
					{
						StartAttack(m_enemy, true);
					}
					else
					{
						Follow(m_enemy, interval);
					}
				}
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

void MonsterAIAddRecoverBuff::Update(unsigned long interval, unsigned int now_second)
{
	if (m_recover_by_attack) return;

	if (m_hp_recover_per <= 0 || m_hp_recover_interval <= 0) return;

	if (Obj::OBJ_TYPE_MONSTER != m_me->GetObjType()) return;

	m_pass_interval += interval;

	if (m_pass_interval > m_hp_recover_interval)
	{
		m_pass_interval = 0;

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
					if (monster->GetMonsterId() != ai_monster->GetMonsterId() && monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP) < monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP))
					{
						if (m_only_effect_boss && !monster->IsBossMonster())
						{
							continue;
						}

						Attribute recover_hp = static_cast<Attribute>(monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (m_hp_recover_per * 0.01f));
						if (recover_hp > 0)
						{
							EffectInterval *effect_interval = new EffectInterval(ai_monster->GetId(), 1, EBT_ADD_BLOOD, 1);
							effect_interval->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
							effect_interval->SetParam(m_hp_recover_interval, 1, recover_hp);
							effect_interval->SetSave(false);
							monster->AddEffect(effect_interval);
						}
					}
				}
			}
		}
	}
}

bool MonsterAIAddRecoverBuff::IsEnemy(Obj *obj)
{
	if (m_recover_by_attack)
	{
		if (NULL != obj && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
		{
			Monster * monster = (Monster*)obj;
			if (monster->IsAlive())
			{
				if (m_only_effect_boss && !monster->IsBossMonster())
				{
					return false;
				}

				return true;
			}
		}
	}

	return false;
}

bool MonsterAIAddRecoverBuff::FindEnemy()
{
	if (!m_recover_by_attack) return false;

	if (Obj::OBJ_TYPE_MONSTER != m_me->GetObjType()) return false;

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

					ForceSetEnemy(monster->GetId());
					return true;
				}
			}
		}
	}

	return false;
}

