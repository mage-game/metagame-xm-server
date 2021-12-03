#include "xiannvskillimpl.hpp"
#include "obj/character/character.h"
#include "obj/character/monster.h"

#include "effect/effectbase.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "effect/skilleffect/effecthurtinc.hpp"
#include "effect/skilleffect/effectinterval.hpp"
#include "effect/skilleffect/effecthurtdec.hpp"
#include "effect/skilleffect/effectreboundhurt.hpp"

#include "scene/scene.h"
#include "scene/speciallogic/speciallogic.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void XiannvSkill80::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// 对目标造成param_a%攻击的伤害，附加中毒效果，每param_b毫秒扣除目标param_c%的最大生命值，持续param_d 毫秒。PVP
	if (NULL == chara || NULL == target || Obj::OBJ_TYPE_ROLE != target->GetObjType())
	{
		return;
	}

	SendAttackMsg(chara, target);

	// 中毒
	int interval = GetParamB(level);
	if (interval <= 1000) interval = 1000;

	int count = GetParamD(level) / interval;
	if (count <= 0) count = 1;

	Attribute delta_hp = (Attribute)(target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * this->GetParamC(level) * ROLE_ATTR_PER);
	EffectInterval *effect = new EffectInterval(chara->GetId(), m_skill_id, EBT_DROP_BLOOD_POISON, level);
	effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
	effect->SetParam(interval, count, -delta_hp, ((Role *)chara)->GetUID());
	effect->SetSave(false);
	effect->SetClientEffectType(EffectBase::ECT_SKILL_POISON);
	target->AddEffect(effect);

	{
		int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
		//long long injure = (long long)(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (this->GetParamA(level) * ROLE_ATTR_PER));
		// 是否受技伤影响
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);
		if (injure <= 0) injure = 1;

		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
		target->AddEffect(effect);
	}

	chara->CheckQTEEffect(target);	
}

void XiannvSkill81::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// 对目标造成param_a%攻击的伤害，使自身受到伤害降低param_b%，持续param_c 毫秒。
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// 持续降低伤害
	int skill_continue_time = this->GetParamC(level);
	int effect_per = this->GetParamB(level);

	EffectDecHurt* dec_hurt_effect = new EffectDecHurt(chara->GetId(), m_skill_id, skill_continue_time, level, 
		EffectBase::PRODUCT_METHOD_SKILL, EBT_DEC_HURT);
	dec_hurt_effect->SetDecPercent(effect_per);
	dec_hurt_effect->SetClientEffectType(EffectBase::ECT_SKILL_DEC_ON_HURT);
	chara->AddEffect(dec_hurt_effect);

// 	{
// 		int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
// 		long long injure = (long long)(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (this->GetParamA(level) * ROLE_ATTR_PER));
// 		if (injure <= 0) injure = 1;
// 
// 		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
// 		target->AddEffect(effect);
// 	}

	chara->CheckQTEEffect(target);	
}

void XiannvSkill82::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// 对目标造成param_a%攻击的伤害，并使自身闪避几率提高param_b%，持续param_c 毫秒。
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// 提升闪避几率
	int skill_continue_time = this->GetParamC(level);
	int effect_per = this->GetParamB(level);

	EffectBuff *effect_buff = new EffectBuff(chara->GetId(), m_skill_id, skill_continue_time, level);
	effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
	effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI_PER, effect_per, effect_per);
	effect_buff->SetClientEffectType(EffectBase::ECT_SKILL_ADD_SHANBI_PER);
	chara->AddEffect(effect_buff);

// 	{
// 		int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
// 		long long injure = (long long)(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (this->GetParamA(level) * ROLE_ATTR_PER));
// 		if (injure <= 0) injure = 1;
// 
// 		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
// 		target->AddEffect(effect);
// 	}

	chara->CheckQTEEffect(target);	
}

void XiannvSkill83::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// 自身回血效果，每param_a毫秒恢复自己param_b%的最大生命值，持续param_c毫秒
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// 自身回血
	int interval = GetParamA(level);
	if (interval <= 1000) interval = 1000;
	int count = GetParamC(level) / interval;
	if (count <= 0) count = 1;

	Attribute delta_hp = (Attribute)(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * this->GetParamB(level) * ROLE_ATTR_PER);
	EffectInterval *effect = new EffectInterval(chara->GetId(), m_skill_id, EBT_XIANNV_RECOVER_HP, level);
	effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
	effect->SetParam(interval, count, delta_hp, ((Role *)chara)->GetUID());
	effect->SetSave(false);
	chara->AddEffect(effect);

	// 获取反弹效果
	//int skill_continue_time = this->GetParamC(level);
	//int effect_per = (int)(this->GetParamB(level) / 100);
	//
	//EffectReboundHurt *effect_rebound = new EffectReboundHurt(target->GetId(), m_skill_id, skill_continue_time, effect_per, level);
	//chara->AddEffect(effect_rebound);

// 	{
// 		int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
// 		long long injure = (long long)(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (this->GetParamA(level) * ROLE_ATTR_PER));
// 		if (injure <= 0) injure = 1;
// 
// 		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
// 		target->AddEffect(effect);
// 	}

	chara->CheckQTEEffect(target);	
}

void XiannvSkill84::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// 对目标及周围共8个目标造成param_a%攻击的伤害。
	if (NULL == chara || NULL == target)
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType()) return;

	SendAOEAttackMsg(chara, target, pos);

	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
	//long long injure = (long long)(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (this->GetParamA(level) * ROLE_ATTR_PER));
	// 是否受技伤影响
	bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
	long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);
	if (injure <= 0) injure = 1;

	static const int TARGET_NUM = 32;
	ObjID target_list[TARGET_NUM] = { INVALID_OBJ_ID };
	const int count = GetAOESkillTarget(chara, level, target, target->GetPos(), target_list, sizeof(target_list) / sizeof(target_list[0]));
	Scene *scene = chara->GetScene();

	for (int i = 0; i < count && i < TARGET_NUM; ++i)
	{
		Character *target_cha = (Character*)scene->GetObj(target_list[i]);
		if (NULL == target_cha) continue;

		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
		target_cha->AddEffect(effect);
	}

	chara->CheckQTEEffect(target);
}

void XiannvSkill85::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// 对目标造成param_a%攻击的伤害，并使目标移动速度降低param_b%，持续param_c 毫秒。

	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// 目标减速
	int skill_continue_time = this->GetParamC(level);
	int effect_per = this->GetParamB(level);

	EffectMoveSpeed *effect_speed = new EffectMoveSpeed(chara->GetId(), m_skill_id, skill_continue_time, EBT_CHIHUAN, level);
	effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
	int value = static_cast<int>(target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED) * (effect_per * ROLE_ATTR_PER));
	effect_speed->SetMoveSpeed(-value, -effect_per / 100);
	effect_speed->SetClientEffectType(EffectBase::ECT_SKILL_DEC_SPEED);
	target->AddEffect(effect_speed);

	{
		int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
		//long long injure = (long long)(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (this->GetParamA(level) * ROLE_ATTR_PER));
		// 是否受技伤影响
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);
		if (injure <= 0) injure = 1;

		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
		target->AddEffect(effect);
	}

	chara->CheckQTEEffect(target);	
}

void XiannvSkill86::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// 对目标造成param_a%攻击的伤害，并恢复伤害量param_b%的生命值。（PVP）
	if (NULL == chara || NULL == target || Obj::OBJ_TYPE_ROLE != target->GetObjType())
	{
		return;
	}

	SendAttackMsg(chara, target);

	{
		int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
		//long long injure = (long long)(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (this->GetParamA(level) * ROLE_ATTR_PER));
		// 是否受技伤影响
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);
		if (injure <= 0) injure = 1;

		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
		target->AddEffect(effect);

		// 自己恢复血量
		int effect_per = this->GetParamB(level);
		Attribute recover_hp = (Attribute)(injure * effect_per * ROLE_ATTR_PER);
		chara->ChangeHp(&recover_hp, true);
	}

	chara->CheckQTEEffect(target);
}


