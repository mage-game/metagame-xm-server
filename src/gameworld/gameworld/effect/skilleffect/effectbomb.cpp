#include "effectbomb.hpp"
#include "skill/skilldef.hpp"
#include "obj/character/character.h"
#include "obj/character/role.h"
#include "scene/scene.h"
#include "scene/map.h"
#include "protocal/msgfight.h"
#include "skill/skillbase.hpp"

EffectBomb::EffectBomb() 
	: m_interval_time(0), m_buff_type(0), m_deliverer_uid(0), m_delta_hp(0), m_interval(0), m_count(0),
	m_injure(0), m_hurt_percent_to_holder(0), m_hurt_percent_to_other(0)
{

}

EffectBomb::EffectBomb(ObjID deliverer, UInt16 skill, unsigned char buff_type, char product_level, int deliver_uid, char product_method)
	: EffectBase(deliverer, skill, product_method, product_level),
	m_interval_time(0), m_buff_type(buff_type), m_deliverer_uid(deliver_uid), m_delta_hp(0), m_interval(0), m_count(0),
	m_injure(0), m_hurt_percent_to_holder(0), m_hurt_percent_to_other(0)
{
	m_merge_rule = EffectBase::MERGE_RULE_NOMERGE_4;
}

EffectBomb::~EffectBomb()
{

}

bool EffectBomb::Update(unsigned long interval, Character *cha, bool *die)
{
	m_interval_time += (int)interval;
	if (m_interval_time < m_interval)
	{
		return true;
	}

	m_interval_time = 0;

	if (m_count > 0)
	{
		-- m_count;

		Attribute real_delta_hp = m_delta_hp, view_delta_hp = m_delta_hp;
		EffectHP(&real_delta_hp, &view_delta_hp, cha, die, false);

		SendAttackBlock(cha, real_delta_hp, view_delta_hp, FIGHT_TYPE_NORMAL, 0, false);
		
		return true;
	}

	this->Explode(cha, die);

	return false;
}

void EffectBomb::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 7);

	param[0] = m_delta_hp;
	param[1] = m_interval;
	param[2] = this->GetLeftTimeMS();
	param[3] = m_buff_type;
	param[4] = m_injure;
	param[5] = m_hurt_percent_to_holder;
	param[6] = m_hurt_percent_to_other;

	*count = 7;
}

bool EffectBomb::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_interval_time) && s.Push(m_buff_type) && s.Push(m_deliverer_uid) &&
		s.Push(m_delta_hp) && s.Push(m_interval) && s.Push(m_count) &&
		s.Push(m_injure) && s.Push(m_hurt_percent_to_holder) && s.Push(m_hurt_percent_to_other);
}

bool EffectBomb::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_interval_time) && s.Pop(&m_buff_type) && s.Pop(&m_deliverer_uid) &&
		s.Pop(&m_delta_hp) && s.Pop(&m_interval) && s.Pop(&m_count) &&
		s.Pop(&m_injure) && s.Pop(&m_hurt_percent_to_holder) && s.Pop(&m_hurt_percent_to_other);
}

void EffectBomb::SetBloodParam(int interval, int count, Attribute delta_hp)
{
	m_interval = interval;
	m_count = count;
	m_delta_hp = delta_hp;
}

void EffectBomb::SetBombParam(long long injure, int hurt_percent_to_holder, int hurt_percent_to_other)
{
	m_injure = injure;
	m_hurt_percent_to_holder = hurt_percent_to_holder;
	m_hurt_percent_to_other = hurt_percent_to_other;
}

void EffectBomb::Explode(Character *cha, bool *die)
{
	if (NULL != cha && NULL != cha->GetScene())
	{
		Scene *scene = cha->GetScene();

		Obj *deliverer_obj = scene->GetObj(m_deliverer);
		if (NULL != deliverer_obj && Obj::OBJ_TYPE_ROLE == deliverer_obj->GetObjType())
		{
			Role *deliverer_role = (Role *)deliverer_obj;

			if (m_deliverer_uid == UidToInt(deliverer_role->GetUserId()))							// 释放者还在
			{
				static ObjID target_list[DEFAULT_AOE_ENEMY_NUM] = {0};
				int target_count = 0; 

				{
					// 收集目标
					target_count = SkillBase::GetTargetByArea(deliverer_role, cha->GetPos(), DEFAULT_AOE_ATTACK_RANGE, target_list, sizeof(target_list) / sizeof(target_list[0]));
				}

				{
					// 广播爆炸
					Protocol::SCExplode psaoe;
					psaoe.explode_reason = Protocol::SCExplode::EXPLODE_REASON_ZHUOSHAO;
					psaoe.pos_x = cha->GetPos().x;
					psaoe.pos_y = cha->GetPos().y;
					scene->GetZoneMatrix()->NotifyAreaMsg(cha->GetObserHandle(), &psaoe, sizeof(Protocol::SCExplode));
				}

				{
					// 炸自己 注意：在buff里对自己的伤害不能用effectonce，因为一旦死了会走进ondie，进而清除buff，在遍历中迭代器出问题
					/*if (cha->IsAlive() && !scene->GetMap()->SaveArea(cha->GetPos().x, cha->GetPos().y))
					{
						long long injure = static_cast<long long>(m_injure * (m_hurt_percent_to_holder / 100.0f));

						Attribute real_delta_hp = -injure, view_delta_hp = -injure;
						EffectHP(&real_delta_hp, &view_delta_hp, cha, die, false);

						SendAttackBlock(cha, real_delta_hp, view_delta_hp, FIGHT_TYPE_NORMAL, false);
					}*/
				}

				{
					// 炸群众
					long long injure = static_cast<long long>(m_injure * (m_hurt_percent_to_other / 100.0f));
					HurtOthers(cha, target_count, &target_list[0], injure, EffectBase::FIGHT_TYPE_NORMAL);
				}
			}
		}
	}
}

void EffectBomb::ForceToExplode()
{
	m_count = 0;
}
