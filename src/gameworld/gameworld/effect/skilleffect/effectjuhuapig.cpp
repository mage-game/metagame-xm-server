#include "effectjuhuapig.hpp"
#include "skill/skillbase.hpp"
#include "obj/character/character.h"
#include "scene/scene.h"
#include "protocal/msgfight.h"

EffectJuhuaPig::EffectJuhuaPig()
	: m_effect_time(0), m_injure_deadline(0), m_rebound_percent(0), m_range(0), m_init_hp(0)
{

}

EffectJuhuaPig::EffectJuhuaPig(ObjID deliverer, UInt16 skill, char product_level, char product_method)
	: m_effect_time(0), m_injure_deadline(0), m_rebound_percent(0), m_range(0), m_init_hp(0)
{
	m_merge_rule = EffectBase::MERGE_RULE_IGNORE;
}

EffectJuhuaPig::~EffectJuhuaPig()
{

}

bool EffectJuhuaPig::Update(unsigned long interval, Character *cha, bool *die)
{
	Attribute now_hp = cha->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
	long long injure = m_init_hp - now_hp;
	if (injure >= m_injure_deadline)
	{
		this->Explode(cha, m_injure_deadline);
		return false;
	}

	m_effect_time -= (int)interval;
	if (m_effect_time > 0)
	{
		return true;
	}

	this->Explode(cha, injure);
	return false;
}

void EffectJuhuaPig::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 4);

	param[0] = m_effect_time;
	param[1] = m_injure_deadline;
	param[2] = m_rebound_percent;
	param[3] = m_range;

	*count = 4;
}

bool EffectJuhuaPig::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_effect_time) && s.Push(m_injure_deadline) && s.Push(m_rebound_percent) && 
		s.Push(m_range) && s.Push(m_init_hp);
}

bool EffectJuhuaPig::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_effect_time) && s.Pop(&m_injure_deadline) && s.Pop(&m_rebound_percent) &&
		s.Pop(&m_range) && s.Pop(&m_init_hp);
}

void EffectJuhuaPig::SetParam(int effect_time, long long injure_deadline, int rebound_percent, int range, long long init_hp)
{
	m_effect_time = effect_time;
	m_injure_deadline = injure_deadline;
	m_rebound_percent = rebound_percent;
	m_range = range;
	m_init_hp = init_hp;
}

void EffectJuhuaPig::Explode(Character *cha, long long injure)
{
	if (NULL != cha && NULL != cha->GetScene() && injure > 0)
	{
		static ObjID target_list[DEFAULT_AOE_ENEMY_NUM] = {0};
		int target_count = 0; 

		{
			// 收集目标
			target_count = SkillBase::GetTargetByArea(cha, cha->GetPos(), DEFAULT_AOE_ATTACK_RANGE, target_list, sizeof(target_list) / sizeof(target_list[0]));
		}

		{
			// 广播爆炸
			Protocol::SCExplode psaoe;
			psaoe.explode_reason = Protocol::SCExplode::EXPLODE_REASON_JUHUAPIG;
			psaoe.pos_x = cha->GetPos().x;
			psaoe.pos_y = cha->GetPos().y;
			cha->GetScene()->GetZoneMatrix()->NotifyAreaMsg(cha->GetObserHandle(), &psaoe, sizeof(Protocol::SCExplode));
		}

		{
			// 炸群众
			long long tmp_injure = static_cast<long long>(injure * (m_rebound_percent / 100.0f));
			HurtOthers(cha, target_count, &target_list[0], tmp_injure, EffectBase::FIGHT_TYPE_NORMAL);
		}
	}
}
