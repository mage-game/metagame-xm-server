

#include "greatesoldierskillimpl.hpp"
#include "gamecommon.h"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "other/greatesoldier/greatesoldier.hpp"

void GreateSoldierSkill600::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAOEAttackMsg(chara, target, pos);

	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
	static ObjID target_list[64];
	int count = GetAOESkillTarget(chara, level, target, pos, target_list, sizeof(target_list) / sizeof(target_list[0]));
	if (count > this->GetAttackRangeEnemyNum(level))
	{
		count = this->GetAttackRangeEnemyNum(level);
	}

	Scene *scene = chara->GetScene();
	for (int i = 0; i < count; ++i)
	{
		Character *target_cha = dynamic_cast<Character*>(scene->GetObj(target_list[i]));
		if (NULL == target_cha) continue;

		// ���ж��Ƿ�����
		if (IsShanbi(chara, target_cha)) continue;

		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		Attribute injure = CalcInjureAndType(chara, target_cha, level, fight_type, is_be_effect_skill_per);
		// ��ǿ�˺����Է�����������޵�%A
		if (target->GetObjType() == Obj::OBJ_TYPE_ROLE)
		{
			double add_hurt_per = GetParamA(level) * SKILL_ATTR_PER;
			injure += static_cast<Attribute>(target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * add_hurt_per);
		}

		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
		target_cha->AddEffect(effect);
	}

	chara->CheckQTEEffect(target);
}

void GreateSoldierSkill601::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAOEAttackMsg(chara, target, pos);

	Attribute max_injure = 0;
	{
		int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
		static ObjID target_list[64];
		int count = GetAOESkillTarget(chara, level, target, pos, target_list, sizeof(target_list) / sizeof(target_list[0]));
		if (count > this->GetAttackRangeEnemyNum(level))
		{
			count = this->GetAttackRangeEnemyNum(level);
		}

		Scene *scene = chara->GetScene();
		for (int i = 0; i < count; ++i)
		{
			Character *target_cha = dynamic_cast<Character*>(scene->GetObj(target_list[i]));
			if (NULL == target_cha) continue;

			// ���ж��Ƿ�����
			if (IsShanbi(chara, target_cha)) continue;

			bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
			Attribute injure = CalcInjureAndType(chara, target_cha, level, fight_type, is_be_effect_skill_per);
			// ��ǿ�˺����Է�����������޵�%A
			if (target->GetObjType() == Obj::OBJ_TYPE_ROLE)
			{
				double add_hurt_per = GetParamA(level) * SKILL_ATTR_PER;
				injure += static_cast<Attribute>(target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * add_hurt_per);
			}

			EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
			target_cha->AddEffect(effect);

			if (max_injure < injure)
			{
				max_injure = injure;
			}
		}
	}

	// ��Ѫ
	if (max_injure > 0 && chara->IsAlive())
	{
		EffectOnce *effect = new EffectOnce(INVALID_OBJ_ID, m_skill_id, max_injure, EffectBase::FIGHT_TYPE_NORMAL);
		chara->AddEffect(effect);
	}

	chara->CheckQTEEffect(target);
}

void GreateSoldierSkill602::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	Role* chara_role = dynamic_cast<Role*>(chara);
	if (NULL == chara_role)
	{
		return;
	}

	SendAOEAttackMsg(chara, target, pos);

	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
	static ObjID target_list[64];
	int count = GetAOESkillTarget(chara, level, target, pos, target_list, sizeof(target_list) / sizeof(target_list[0]));
	if (count > this->GetAttackRangeEnemyNum(level))
	{
		count = this->GetAttackRangeEnemyNum(level);
	}

	chara_role->GetGreateSoldier()->SetGreateSoldierBaoji(true);  // ���øü���ǿ�Ʊ���

	Scene *scene = chara->GetScene();
	for (int i = 0; i < count; ++i)
	{
		Character *target_cha = dynamic_cast<Character*>(scene->GetObj(target_list[i]));
		if (NULL == target_cha) continue;

		// ���ж��Ƿ�����
		if (IsShanbi(chara, target_cha)) continue;

		//const bool is_baoji = true;
		//fight_type = EffectBase::FIGHT_TYPE_BAOJI;
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		Attribute injure = CalcInjureAndType(chara, target_cha, level, fight_type, is_be_effect_skill_per);
		// ��ǿ�˺����Է�����������޵�%A
		if (target->GetObjType() == Obj::OBJ_TYPE_ROLE)
		{
			double add_hurt_per = GetParamA(level) * SKILL_ATTR_PER;
			injure += static_cast<Attribute>(target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * add_hurt_per);
		}

		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
		target_cha->AddEffect(effect);
	}

	chara_role->GetGreateSoldier()->SetGreateSoldierBaoji(false);  // ȡ���ü���ǿ�Ʊ���

	chara->CheckQTEEffect(target);
}


// �������⼼��11
// �����ڼ䣬�����ڼ䣬ÿ2(�ò���Ϊ�������⼼�����ñ�Ĳ���a)���Բ�η�Χ(10)��x��Ŀ�����80%�������˺����߹�ʽ
void GreateSoldierSkill603::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAOEAttackMsg(chara, target, pos);

	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
	static ObjID target_list[64];
	int count = GetAOESkillTarget(chara, level, target, pos, target_list, sizeof(target_list) / sizeof(target_list[0]));
	if (count > this->GetAttackRangeEnemyNum(level))
	{
		count = this->GetAttackRangeEnemyNum(level);
	}

	Scene *scene = chara->GetScene();
	for (int i = 0; i < count; ++i)
	{
		Character *target_cha = dynamic_cast<Character*>(scene->GetObj(target_list[i]));
		if (NULL == target_cha) continue;

		// ���ж��Ƿ�����
		if (IsShanbi(chara, target_cha)) continue;

		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		Attribute injure = CalcInjureAndType(chara, target_cha, level, fight_type, is_be_effect_skill_per);

		EffectOnce *effect = new EffectOnce(chara->GetId(), EffectBase::PRODUCT_ID_GREATE_SOLDIER_SPECIAL_SKILL, -injure, fight_type);
		target_cha->AddEffect(effect);
	}

	chara->CheckQTEEffect(target);
}

// �������⼼��14
// ����ʱ����Բ�η�Χ��x��Ŀ�����200%�������˺���ʹ��ѣ�Σ�����param_a(�ò���Ϊ���ܱ�Ĳ���a)��
void GreateSoldierSkill604::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAOEAttackMsg(chara, target, pos);

	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
	static ObjID target_list[64];
	int count = GetAOESkillTarget(chara, level, target, pos, target_list, sizeof(target_list) / sizeof(target_list[0]));
	if (count > this->GetAttackRangeEnemyNum(level))
	{
		count = this->GetAttackRangeEnemyNum(level);
	}

	Scene *scene = chara->GetScene();
	for (int i = 0; i < count; ++i)
	{
		Character *target_cha = dynamic_cast<Character*>(scene->GetObj(target_list[i]));
		if (NULL == target_cha) continue;

		// ���ж��Ƿ�����
		if (IsShanbi(chara, target_cha)) continue;

		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		Attribute injure = CalcInjureAndType(chara, target_cha, level, fight_type, is_be_effect_skill_per);

		EffectOnce *effect = new EffectOnce(chara->GetId(), EffectBase::PRODUCT_ID_GREATE_SOLDIER_SPECIAL_SKILL, -injure, fight_type);
		target_cha->AddEffect(effect);

		// ѣ��
		auto effect_time_ms = GetParamA(level) * 1000;
		EffectSpecialState* special_effect = new EffectSpecialState(chara->GetId(), EffectBase::PRODUCT_ID_GREATE_SOLDIER_SPECIAL_SKILL, effect_time_ms, EBT_XUANYUN, 1);
		special_effect->SetClientEffectType(EffectBase::ECT_SKILL_XUANYUN);
		target_cha->AddEffect(special_effect);
	}

	chara->CheckQTEEffect(target);
}

// �������⼼��15
// ����ʱ����Բ�η�Χ��12����6��Ŀ�������Լ���ߣ����200%�Ĺ��������˺���ʹ���ƶ��ٶȽ���param_a��ֱȣ�����param_b��
void GreateSoldierSkill605::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAOEAttackMsg(chara, target, pos);

	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
	int product_id = EffectBase::PRODUCT_ID_GREATE_SOLDIER_SPECIAL_SKILL;
	static ObjID target_list[64];
	int count = GetAOESkillTarget(chara, level, target, pos, target_list, sizeof(target_list) / sizeof(target_list[0]));
	if (count > this->GetAttackRangeEnemyNum(level))
	{
		count = this->GetAttackRangeEnemyNum(level);
	}

	Scene *scene = chara->GetScene();
	for (int i = 0; i < count; ++i)
	{
		Character *target_cha = dynamic_cast<Character*>(scene->GetObj(target_list[i]));
		if (NULL == target_cha) continue;

		// ���ж��Ƿ�����
		if (IsShanbi(chara, target_cha)) continue;

		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		Attribute injure = CalcInjureAndType(chara, target_cha, level, fight_type, is_be_effect_skill_per);

		// ��ɹ����˺�
		EffectOnce *effect = new EffectOnce(chara->GetId(), product_id, -injure, fight_type);
		target_cha->AddEffect(effect);

		if (target_cha->IsAlive())
		{
			// ���ܻ�������ǰ������Ŀ��λ��
			target_cha->SkillResetPos(chara->GetPos(), Character::SKILL_RESET_POS_TYPE_CAPTURE, m_skill_id, false, chara->GetId());
		}

		// �����ƶ��ٶ�
		int effect_time_ms = GetParamB(level) * 1000;
		int dec_speed_percent = GetParamA(level);
		int value = static_cast<int>(target_cha->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED) * (dec_speed_percent * ROLE_ATTR_PER));

		EffectMoveSpeed *effect_speed = new EffectMoveSpeed(chara->GetId(), product_id, effect_time_ms, EBT_MOJIESKILL_SLOW, level);
		effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
		effect_speed->SetMoveSpeed(-value, -dec_speed_percent / 100);
		effect_speed->SetClientEffectType(EffectBase::ECT_SKILL_DEC_SPEED);
		target_cha->AddEffect(effect_speed);
	}

	chara->CheckQTEEffect(target);
}

// �������⼼��16
// ���������һ˲�䣬����Χ8��Ŀ�����120%�������˺���ʹ���Ĭparam_a��
void GreateSoldierSkill606::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	SendAOEAttackMsg(chara, target, pos);

	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
	int product_id = EffectBase::PRODUCT_ID_GREATE_SOLDIER_SPECIAL_SKILL;
	static ObjID target_list[64];
	int count = GetAOESkillTarget(chara, level, target, pos, target_list, sizeof(target_list) / sizeof(target_list[0]));
	if (count > this->GetAttackRangeEnemyNum(level))
	{
		count = this->GetAttackRangeEnemyNum(level);
	}

	Scene *scene = chara->GetScene();
	for (int i = 0; i < count; ++i)
	{
		Character *target_cha = dynamic_cast<Character*>(scene->GetObj(target_list[i]));
		if (NULL == target_cha) continue;

		// ���ж��Ƿ�����
		if (IsShanbi(chara, target_cha)) continue;

		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		Attribute injure = CalcInjureAndType(chara, target_cha, level, fight_type, is_be_effect_skill_per);

		EffectOnce *effect = new EffectOnce(chara->GetId(), product_id, -injure, fight_type);
		target_cha->AddEffect(effect);

		// ��Ĭ
		int effect_time_ms = GetParamA(level) * 1000;
		EffectSpecialState *special_effect = new EffectSpecialState(target_cha->GetId(), product_id, effect_time_ms, EBT_CHENMO, level, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
		special_effect->SetMergeRule(EffectBase::MERGE_RULE_IGNORE);
		special_effect->SetSave(false);
		special_effect->SetClientEffectType(EffectBase::ECT_SKILL_CHENMO);
		target_cha->AddEffect(special_effect);
	}

	chara->CheckQTEEffect(target);
}



