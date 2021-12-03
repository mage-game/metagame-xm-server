#include "jinglingskillimpl.hpp"
#include "skill/skilldef.hpp"
#include "obj/character/character.h"
#include "obj/character/monster.h"

#include "effect/effectbase.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectinterval.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effecthpstore.hpp"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "effect/skilleffect/effecthurtinc.hpp"
#include "effect/skilleffect/effecthurtdec.hpp"
#include "effect/skilleffect/effectcounter.hpp"
#include "effect/skilleffect/effectdoublehurt.hpp"
#include "effect/skilleffect/effectbomb.hpp"
#include "effect/skilleffect/effectattrhurtrandinc.hpp"
#include "effect/skilleffect/effectotherbuff.hpp"
#include "effect/skilleffect/effectreboundhurt.hpp"

#include "effect/sceneeffect/sceneeffect.hpp"

#include "scene/scene.h"
#include "scene/speciallogic/speciallogic.hpp"

#include "protocal/msgscene.h"
#include "protocal/msgfight.h"

#include "gamecommon.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void JingLingSkill151::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// 先判断是否闪避
	if (IsShanbi(chara, target)) return;

	const int effect_time = GetParamA(level);

	// 晕人
	{
		EffectSpecialState *special_effect = new EffectSpecialState(target->GetId(), GetSkillId(), effect_time, EBT_XUANYUN, 1);
		special_effect->SetMergeRule(EffectBase::MERGE_RULE_IGNORE);
		special_effect->SetClientEffectType(EffectBase::ECT_JL_SKILL_XY);
		special_effect->SetSave(true);
		target->AddEffect(special_effect);
	}

	// 伤害
	{
		int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
		// 是否受技伤影响
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);

		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
		target->AddEffect(effect);

		this->HurtTrigger(chara, target, injure, fight_type);
	}

	chara->CheckQTEEffect(target);	
}

//2分钟内，采集速度上升60%
void JingLingSkill152::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	const int effect_time = GetParamA(level);
	const int per = GetParamB(level);

	EffectOtherBuff *effect = new EffectOtherBuff(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_GATHER_SPEED, effect_time, 0, EffectBase::PRODUCT_METHOD_SKILL);
	effect->IncGatherSpeedPer(per);
	effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
	effect->SetClientEffectType(EffectBase::ECT_JL_SKILL_CJJS);
	chara->AddEffect(effect);

	chara->CheckQTEEffect(target);
}

// 2分钟内，对建筑物伤害提升50%
void JingLingSkill153::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	const int effect_time = GetParamA(level);
	const int inc_hurt_per = GetParamB(level);

	EffectOtherBuff *effect = new EffectOtherBuff(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_TO_BUILDINF_INC_HURT, effect_time, 0, EffectBase::PRODUCT_METHOD_SKILL);
	effect->IncToBuildingHurtPer(inc_hurt_per);
	effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
	effect->SetClientEffectType(EffectBase::ECT_JL_SKILL_JZGS);
	chara->AddEffect(effect);

	chara->CheckQTEEffect(target);
}

// 无敌2S
void JingLingSkill154::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	const int effect_time = GetParamA(level);

	EffectSpecialState *special_effect = new EffectSpecialState(INVALID_OBJ_ID, 0, effect_time, EBT_WUDI, 1, EffectBase::PRODUCT_METHOD_SKILL);
	special_effect->SetClientEffectType(EffectBase::ECT_JL_SKILL_WD);
	chara->AddEffect(special_effect);

	chara->CheckQTEEffect(target);
}

// 2分钟内，采集不被打断
void JingLingSkill155::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	const int effect_time = GetParamA(level);

	EffectOtherBuff *effect = new EffectOtherBuff(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_GATHER_NOT_STOP, effect_time, 0, EffectBase::PRODUCT_METHOD_SKILL);
	effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
	effect->SetClientEffectType(EffectBase::ECT_JL_SKILL_CJKDD);
	chara->AddEffect(effect);
	chara->CheckQTEEffect(target);
}

// 185%攻击伤害，并且5S内目标速度降低50%
void JingLingSkill156::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// 先判断是否闪避
	if (IsShanbi(chara, target)) return;

	const int effect_time = GetParamA(level);
	const int dec_speed_per = GetParamB(level);

	//迟缓
	{
		int dec_movespeed = static_cast<int>(target->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) * (dec_speed_per * SKILL_ATTR_PER));
		EffectMoveSpeed *effect_speed = new EffectMoveSpeed(target->GetId(), m_skill_id, effect_time, EBT_CHIHUAN, 1);
		effect_speed->SetMoveSpeed(-dec_movespeed, -1 * (int)(dec_speed_per * SKILL_ATTR_PER * 100));
		effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
		effect_speed->SetClientEffectType(EffectBase::ECT_JL_SKILL_JSHU);
		effect_speed->SetSave(true);
		target->AddEffect(effect_speed);
	}

	// 伤害
	{
		int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
		// 是否受技伤影响
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);

		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
		target->AddEffect(effect);

		this->HurtTrigger(chara, target, injure, fight_type);
	}

	chara->CheckQTEEffect(target);	
}

// 185%攻击伤害，5S内减少目标15%攻击
void JingLingSkill157::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// 先判断是否闪避
	if (IsShanbi(chara, target)) return;

	const int effect_time = GetParamA(level);
	const int dec_gongji_per = GetParamB(level);

	// 减对方攻击
	{
		EffectBuff *effect = new EffectBuff(target->GetId(), m_skill_id, effect_time, 1, EffectBase::PRODUCT_METHOD_SKILL);
		Attribute dec_gongji = static_cast<Attribute>(target->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * (dec_gongji_per * SKILL_ATTR_PER));
		effect->AddGongJiEffect(-dec_gongji, -1 * (int)(dec_gongji * SKILL_ATTR_PER * 100));
		effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
		effect->SetClientEffectType(EffectBase::ECT_JL_SKILL_JG);
		effect->SetSave(true);
		target->AddEffect(effect);
	}

	// 伤害
	{
		int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
		// 是否受技伤影响
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);

		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
		target->AddEffect(effect);

		this->HurtTrigger(chara, target, injure, fight_type);
	}

	chara->CheckQTEEffect(target);	
}

//185%攻击伤害，5S内减少目标25%防御
void JingLingSkill158::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// 先判断是否闪避
	if (IsShanbi(chara, target)) return;

	const int effect_time = GetParamA(level);
	const int dec_fangyu_per = GetParamB(level);

	// 减对方防御
	{
		EffectBuff *effect = new EffectBuff(target->GetId(), m_skill_id, effect_time, 1, EffectBase::PRODUCT_METHOD_SKILL);
		Attribute dec_fangyu = static_cast<Attribute>(target->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU) * (dec_fangyu_per * SKILL_ATTR_PER));
		effect->AddFangYuEffect(-dec_fangyu, -1 * (int)(dec_fangyu_per * SKILL_ATTR_PER * 100));
		effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
		effect->SetClientEffectType(EffectBase::ECT_JL_SKILL_JF);
		effect->SetSave(true);
		target->AddEffect(effect);
	}

	// 伤害
	{
		int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
		// 是否受技伤影响
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);

		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
		target->AddEffect(effect);

		this->HurtTrigger(chara, target, injure, fight_type);
	}

	chara->CheckQTEEffect(target);	
}

// 185%攻击伤害，将目标变为动物2S
void JingLingSkill159::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// 先判断是否闪避
	if (IsShanbi(chara, target)) return;

	const int effect_time = GetParamA(level);

	// 变形
	{
		EffectSpecialState *effect_state = new EffectSpecialState(chara->GetId(), m_skill_id, effect_time, EBT_BIANXING_FOOL, level);
		effect_state->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
		target->AddEffect(effect_state);
	}

	// 伤害
	{
		int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
		// 是否受技伤影响
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);

		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
		target->AddEffect(effect);

		this->HurtTrigger(chara, target, injure, fight_type);
	}

	chara->CheckQTEEffect(target);
}

void JingLingSkill160::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// 先判断是否闪避
	if (IsShanbi(chara, target)) return;

	const int effect_time = GetParamA(level);
	const int dec_hurt_per = GetParamB(level);

	// 减伤害
	{
		EffectDecHurt *effect = new EffectDecHurt(chara->GetId(), m_skill_id, effect_time, 1, EffectBase::PRODUCT_METHOD_SKILL);
		effect->SetDecPercent(dec_hurt_per);
		effect->SetClientEffectType(EffectBase::ECT_JL_SKILL_JSHAN);
		effect->SetSave(true);
		chara->AddEffect(effect);
	}

	// 伤害
	{
		int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
		// 是否受技伤影响
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);

		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
		target->AddEffect(effect);

		this->HurtTrigger(chara, target, injure, fight_type);
	}

	chara->CheckQTEEffect(target);
}

// 低级变羊
void JingLingSkill161::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// 先判断是否闪避
	if (IsShanbi(chara, target)) return;

	const int effect_time = GetParamA(level);

	// 变形
	{
		EffectSpecialState *effect_state = new EffectSpecialState(chara->GetId(), m_skill_id, effect_time, EBT_BIANXING_FOOL, level);
		effect_state->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
		target->AddEffect(effect_state);
	}

	// 伤害
	{
		int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
		// 是否受技伤影响
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);

		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
		target->AddEffect(effect);

		this->HurtTrigger(chara, target, injure, fight_type);
	}

	chara->CheckQTEEffect(target);
}

// 低级减速
void JingLingSkill162::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// 先判断是否闪避
	if (IsShanbi(chara, target)) return;

	const int effect_time = GetParamA(level);
	const int dec_speed_per = GetParamB(level);

	//迟缓
	{
		int dec_movespeed = static_cast<int>(target->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) * (dec_speed_per * SKILL_ATTR_PER));
		EffectMoveSpeed *effect_speed = new EffectMoveSpeed(target->GetId(), m_skill_id, effect_time, EBT_CHIHUAN, 1);
		effect_speed->SetMoveSpeed(-dec_movespeed, -1 * (int)(dec_speed_per * SKILL_ATTR_PER * 100));
		effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
		effect_speed->SetClientEffectType(EffectBase::ECT_JL_SKILL_JSHU);
		effect_speed->SetSave(true);
		target->AddEffect(effect_speed);
	}

	// 伤害
	{
		int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
		// 是否受技伤影响
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);

		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
		target->AddEffect(effect);

		this->HurtTrigger(chara, target, injure, fight_type);
	}

	chara->CheckQTEEffect(target);	
}

// 185%攻击伤害，5S内减少目标15%攻击
void JingLingSkill163::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// 先判断是否闪避
	if (IsShanbi(chara, target)) return;

	const int effect_time = GetParamA(level);
	const int dec_gongji_per = GetParamB(level);

	// 减对方攻击
	{
		EffectBuff *effect = new EffectBuff(target->GetId(), m_skill_id, effect_time, 1, EffectBase::PRODUCT_METHOD_SKILL);
		Attribute dec_gongji = static_cast<Attribute>(target->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * (dec_gongji_per * SKILL_ATTR_PER));
		effect->AddGongJiEffect(-dec_gongji, -1 * (int)(dec_gongji * SKILL_ATTR_PER * 100));
		effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
		effect->SetClientEffectType(EffectBase::ECT_JL_SKILL_JG);
		effect->SetSave(true);
		target->AddEffect(effect);
	}

	// 伤害
	{
		int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
		// 是否受技伤影响
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);

		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
		target->AddEffect(effect);

		this->HurtTrigger(chara, target, injure, fight_type);
	}

	chara->CheckQTEEffect(target);	
}

void JingLingSkill164::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param /* = 0 */) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// 先判断是否闪避
	if (IsShanbi(chara, target)) return;

	const int effect_time = GetParamA(level);

	// 晕人
	{
		EffectSpecialState *special_effect = new EffectSpecialState(target->GetId(), GetSkillId(), effect_time, EBT_XUANYUN, 1);
		special_effect->SetMergeRule(EffectBase::MERGE_RULE_IGNORE);
		special_effect->SetClientEffectType(EffectBase::ECT_JL_SKILL_XY);
		special_effect->SetSave(true);
		target->AddEffect(special_effect);
	}

	const int dec_fangyu_per = GetParamB(level);

	// 减对方防御
	{
		EffectBuff *effect = new EffectBuff(target->GetId(), m_skill_id, effect_time, 1, EffectBase::PRODUCT_METHOD_SKILL);
		Attribute dec_fangyu = static_cast<Attribute>(target->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU) * (dec_fangyu_per * SKILL_ATTR_PER));
		effect->AddFangYuEffect(-dec_fangyu, -1 * (int)(dec_fangyu_per * SKILL_ATTR_PER * 100));
		effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
		effect->SetClientEffectType(EffectBase::ECT_JL_SKILL_JF);
		effect->SetSave(true);
		target->AddEffect(effect);
	}

	// 伤害
	{
		int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
		// 是否受技伤影响
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);

		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
		target->AddEffect(effect);

		this->HurtTrigger(chara, target, injure, fight_type);
	}

	chara->CheckQTEEffect(target);
}

void JingLingSkill165::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param /* = 0 */) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// 先判断是否闪避
	if (IsShanbi(chara, target)) return;


	const int effect_time = GetParamA(level);
	const int rebound_precent = GetParamB(level);

	// 反伤
	{
		EffectReboundHurt *effect_rebound = new EffectReboundHurt(target->GetId(), GetSkillId(), effect_time, rebound_precent, 1);
		chara->AddEffect(effect_rebound);
	}

	//// 伤害
	//{
	//	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
	//	long long injure = CalcInjureAndType(chara, target, level, fight_type);

	//	EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
	//	target->AddEffect(effect);

	//	this->HurtTrigger(chara, target, injure, fight_type);
	//}

	chara->CheckQTEEffect(target);
}

void JingLingSkill166::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param /* = 0 */) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// 先判断是否闪避
	if (IsShanbi(chara, target)) return;

	const int effect_time = GetParamA(level);

	// 变形
	{
		EffectSpecialState *effect_state = new EffectSpecialState(chara->GetId(), m_skill_id, effect_time, EBT_BIANXING_FOOL, level);
		effect_state->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
		target->AddEffect(effect_state);
	}

	const int dec_speed_per = GetParamB(level);

	// 迟缓
	{
		int dec_movespeed = static_cast<int>(target->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) * (dec_speed_per * SKILL_ATTR_PER));
		EffectMoveSpeed *effect_speed = new EffectMoveSpeed(target->GetId(), m_skill_id, effect_time, EBT_CHIHUAN, 1);
		effect_speed->SetMoveSpeed(-dec_movespeed, -1 * (int)(dec_speed_per * SKILL_ATTR_PER * 100));
		effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
		effect_speed->SetClientEffectType(EffectBase::ECT_JL_SKILL_JSHU);
		effect_speed->SetSave(true);
		target->AddEffect(effect_speed);
	}

	// 伤害
	{
		int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
		// 是否受技伤影响
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);

		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
		target->AddEffect(effect);

		this->HurtTrigger(chara, target, injure, fight_type);
	}

	chara->CheckQTEEffect(target);
}

