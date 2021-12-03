

#include "weekendequipskillimpl.hpp"
#include "gamecommon.h"
#include "effect/skilleffect/effectotherbuff.hpp"
#include "effect/skilleffect/effecthurtinc.hpp"
#include "effect/skilleffect/effecthurtdec.hpp"
#include "effect/skilleffect/effectinvisible.hpp"
#include "effect/skilleffect/effectbianshen.hpp"
#include "other/specialappearance/specialappearance.hpp"
#include "protocal/msgfight.h"

void WeekendEquipSkill610::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// 增伤
	EffectIncHurt *effect_inc_hurt = new EffectIncHurt(chara->GetId(), EffectBase::PRODUCT_ID_WEEKEND_EQUIP_PASSIVE_SKILL1, GetParamA(level) * 1000, 1, 0);
	effect_inc_hurt->SetIncPercent(GetParamB(level));
	effect_inc_hurt->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
	chara->AddEffect(effect_inc_hurt);

	// 模型大小改变
	EffectBianshen *effect_bianshen = new EffectBianshen(chara->GetId(), EffectBase::PRODUCT_ID_WEEKEND_EQUIP_PASSIVE_SKILL1, GetParamA(level) * 1000, 1, 0, EBT_BIAN_XING);
	effect_bianshen->SetModelSize(GetParamC(level)); 
	effect_bianshen->SetMergeRule(EffectBase::MERGE_RULE_IGNORE);
	effect_bianshen->SetImage(APPEARANCE_INITAL_STATUS);
	effect_bianshen->SetExtra(GetParamB(level));
	effect_bianshen->SetClientEffectType(EffectBase::ECT_SKILL_WEEKEND_EQUIP_INC_HURT);
	chara->AddEffect(effect_bianshen);

	// 查找BUFF是否添加成功
	EffectBase *add_buff = chara->GetEffectList()->FindEffect(EffectBase::EFFECT_TYPE_BIANSHEN, EBT_BIAN_XING, chara->GetId());
	if (nullptr != add_buff)
	{
		// 广播形象
		static Protocol::SCBianShenView cmd;
		cmd.obj_id = chara->GetId();
		cmd.bianshen_image = APPEARANCE_MODEL_SIZE_BIG;
		cmd.model_size = GetParamC(level);
		chara->GetScene()->GetZoneMatrix()->NotifyAreaMsg(chara->GetObserHandle(), (void*)&cmd, sizeof(cmd));
	}

}

void WeekendEquipSkill611::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// 减伤
	EffectDecHurt *effect_dec_hurt = new EffectDecHurt(chara->GetId(), EffectBase::PRODUCT_ID_WEEKEND_EQUIP_PASSIVE_SKILL2, GetParamA(level) * 1000, 1, 0);
	effect_dec_hurt->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
	effect_dec_hurt->SetDecPercent(GetParamB(level));
	chara->AddEffect(effect_dec_hurt);

	// 模型大小改变
	EffectBianshen *effect_bianshen = new EffectBianshen(chara->GetId(), EffectBase::PRODUCT_ID_WEEKEND_EQUIP_PASSIVE_SKILL2, GetParamA(level) * 1000, 1, 0, EBT_BIAN_XING);
	effect_bianshen->SetModelSize(GetParamC(level));
	effect_bianshen->SetMergeRule(EffectBase::MERGE_RULE_IGNORE);
	effect_bianshen->SetImage(APPEARANCE_INITAL_STATUS);
	effect_bianshen->SetExtra(GetParamB(level));
	effect_bianshen->SetClientEffectType(EffectBase::ECT_SKILL_WEEKEND_EQUIP_DEC_HURT);
	chara->AddEffect(effect_bianshen);

	// 查找BUFF是否添加成功
	EffectBase *add_buff = chara->GetEffectList()->FindEffect(EffectBase::EFFECT_TYPE_BIANSHEN, EBT_BIAN_XING, chara->GetId());
	if (nullptr != add_buff)
	{
		// 广播形象
		static Protocol::SCBianShenView cmd;
		cmd.obj_id = chara->GetId();
		cmd.bianshen_image = APPEARANCE_MODEL_SIZE_SMALL;
		cmd.model_size = GetParamC(level);
		chara->GetScene()->GetZoneMatrix()->NotifyAreaMsg(chara->GetObserHandle(), (void*)&cmd, sizeof(cmd));
	}

}

void WeekendEquipSkill612::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// 隐身
	EffectInvisible *effect_invisible = new EffectInvisible(chara->GetId(), EffectBase::PRODUCT_ID_WEEKEND_EQUIP_PASSIVE_SKILL3, GetParamA(level) * 1000, 1);
	effect_invisible->SetClientEffectType(EffectBase::ECT_SKILL_WEEKEND_EQUIP_INC_HURT_INVISIABLE);
	effect_invisible->SetBuffType(EBT_INVISIBLE_HURT_ABLE);
	chara->AddEffect(effect_invisible);

	// 广播隐身
	static Protocol::SCInvisibleView cmd;
	cmd.obj_id = chara->GetId();
	cmd.is_invisible = INVISIBLE_SATTUS_CAN_NOT_SEE;
	chara->GetScene()->GetZoneMatrix()->NotifyAreaMsg(chara->GetObserHandle(), (void*)&cmd, sizeof(cmd));

	// 增伤
	EffectIncHurt *effect_inc_hurt = new EffectIncHurt(chara->GetId(), EffectBase::PRODUCT_ID_WEEKEND_EQUIP_PASSIVE_SKILL3, GetParamA(level) * 1000, 1, 0);
	effect_inc_hurt->SetIncPercent(GetParamB(level));
	effect_inc_hurt->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
	chara->AddEffect(effect_inc_hurt);
}