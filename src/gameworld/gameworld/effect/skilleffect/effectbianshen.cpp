#include "effectbianshen.hpp"

#include "obj/character/character.h"
#include "obj/character/role.h"
#include "protocal/msgfight.h"
#include "scene/scene.h"
#include "other/specialappearance/specialappearance.hpp"

bool EffectBianshen::Update(unsigned long interval, Character *cha, bool *die)
{
	m_effect_time -= (int)interval;

	if (m_effect_time < 0)
	{
		m_bianshen_image = APPEARANCE_NORMAL;

		// ¹ã²¥ÐÎÏó
		static Protocol::SCBianShenView cmd;
		cmd.obj_id = cha->GetId();
		cmd.bianshen_image = m_bianshen_image;
		cmd.obj_id_des = m_obj_id_des;
		cmd.model_size = m_model_size;
		cha->GetScene()->GetZoneMatrix()->NotifyAreaMsg(cha->GetObserHandle(), (void*)&cmd, sizeof(cmd));
	}

	return m_effect_time > 0;
}

void EffectBianshen::GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX])
{
	UNSTD_STATIC_CHECK(EFFECT_INFO_PARAM_MAX >= 5);

	param[0] = m_effect_time;
	param[1] = m_bianshen_image;
	param[2] = m_obj_id_des;
	param[3] = m_model_size;
	param[4] = m_extra_info;

	*count = 5;
}

bool EffectBianshen::Serialize(TLVSerializer &s) const
{
	return SerializeBase(s) && s.Push(m_effect_time) && s.Push(m_bianshen_image) && s.Push(m_buff_type) && s.Push(m_obj_id_des) && s.Push(m_model_size) && s.Push(m_extra_info);
}

bool EffectBianshen::Unserialize(TLVUnserializer &s)
{
	return UnserializeBase(s) && s.Pop(&m_effect_time) && s.Pop(&m_bianshen_image) && s.Pop(&m_buff_type) && s.Pop(&m_obj_id_des) && s.Pop(&m_model_size) && s.Pop(&m_extra_info);
}


