#include "effectonce.hpp"
#include "obj/character/character.h"
#include "scene/scene.h"
#include "obj/character/role.h"

void EffectOnce::Effect(Character *cha, bool *die)
{
	Attribute real_delta_hp = m_delta_hp, view_delta_hp = m_delta_hp;

	bool is_from_skill = false;
	if (EffectBase::PRODUCT_METHOD_SKILL == m_product_method || EffectBase::PRODUCT_METHOD_SCENE_SKILL == m_product_method) is_from_skill = true;

	EffectHP(&real_delta_hp, &view_delta_hp, cha, die, is_from_skill, m_fight_type);

	SendAttackBlock(cha, real_delta_hp, view_delta_hp, m_fight_type, m_blood_seq, true, m_passive_flag);

	// ºÏ≤È…À∫¶∑¥µØ
	if (real_delta_hp < 0 && is_from_skill)
	{
		int rebound_hurt_percent = cha->GetEffectList()->GetReboundHurtPercent();
		if (rebound_hurt_percent > 0 && NULL != cha->GetScene())
		{
			Obj* deliverer_obj = cha->GetScene()->GetObj(m_deliverer);
			if (NULL != deliverer_obj && Obj::IsCharactor(deliverer_obj->GetObjType()))
			{
				Character* deliverer_cha = (Character *)deliverer_obj;

				long long rebound_hurt = static_cast<long long>(real_delta_hp * (rebound_hurt_percent / 100.0f));

				EffectOnce* effect_once = new EffectOnce(cha->GetId(), 0, rebound_hurt, EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_REBOUNDHURT);
				deliverer_cha->AddEffect(effect_once);
			}
		}
	}
}

