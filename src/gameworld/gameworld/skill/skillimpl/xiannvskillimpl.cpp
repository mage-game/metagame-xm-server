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
	// ��Ŀ�����param_a%�������˺��������ж�Ч����ÿparam_b����۳�Ŀ��param_c%���������ֵ������param_d ���롣PVP
	if (NULL == chara || NULL == target || Obj::OBJ_TYPE_ROLE != target->GetObjType())
	{
		return;
	}

	SendAttackMsg(chara, target);

	// �ж�
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
		// �Ƿ��ܼ���Ӱ��
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
	// ��Ŀ�����param_a%�������˺���ʹ�����ܵ��˺�����param_b%������param_c ���롣
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// ���������˺�
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
	// ��Ŀ�����param_a%�������˺�����ʹ�������ܼ������param_b%������param_c ���롣
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// �������ܼ���
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
	// �����ѪЧ����ÿparam_a����ָ��Լ�param_b%���������ֵ������param_c����
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// �����Ѫ
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

	// ��ȡ����Ч��
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
	// ��Ŀ�꼰��Χ��8��Ŀ�����param_a%�������˺���
	if (NULL == chara || NULL == target)
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType()) return;

	SendAOEAttackMsg(chara, target, pos);

	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
	//long long injure = (long long)(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (this->GetParamA(level) * ROLE_ATTR_PER));
	// �Ƿ��ܼ���Ӱ��
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
	// ��Ŀ�����param_a%�������˺�����ʹĿ���ƶ��ٶȽ���param_b%������param_c ���롣

	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	// Ŀ�����
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
		// �Ƿ��ܼ���Ӱ��
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
	// ��Ŀ�����param_a%�������˺������ָ��˺���param_b%������ֵ����PVP��
	if (NULL == chara || NULL == target || Obj::OBJ_TYPE_ROLE != target->GetObjType())
	{
		return;
	}

	SendAttackMsg(chara, target);

	{
		int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
		//long long injure = (long long)(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (this->GetParamA(level) * ROLE_ATTR_PER));
		// �Ƿ��ܼ���Ӱ��
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);
		if (injure <= 0) injure = 1;

		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
		target->AddEffect(effect);

		// �Լ��ָ�Ѫ��
		int effect_per = this->GetParamB(level);
		Attribute recover_hp = (Attribute)(injure * effect_per * ROLE_ATTR_PER);
		chara->ChangeHp(&recover_hp, true);
	}

	chara->CheckQTEEffect(target);
}


