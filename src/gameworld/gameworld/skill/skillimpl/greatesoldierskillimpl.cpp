

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

		// 先判断是否闪避
		if (IsShanbi(chara, target_cha)) continue;

		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		Attribute injure = CalcInjureAndType(chara, target_cha, level, fight_type, is_be_effect_skill_per);
		// 增强伤害：对方玩家生命上限的%A
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

			// 先判断是否闪避
			if (IsShanbi(chara, target_cha)) continue;

			bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
			Attribute injure = CalcInjureAndType(chara, target_cha, level, fight_type, is_be_effect_skill_per);
			// 增强伤害：对方玩家生命上限的%A
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

	// 回血
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

	chara_role->GetGreateSoldier()->SetGreateSoldierBaoji(true);  // 设置该技能强制暴击

	Scene *scene = chara->GetScene();
	for (int i = 0; i < count; ++i)
	{
		Character *target_cha = dynamic_cast<Character*>(scene->GetObj(target_list[i]));
		if (NULL == target_cha) continue;

		// 先判断是否闪避
		if (IsShanbi(chara, target_cha)) continue;

		//const bool is_baoji = true;
		//fight_type = EffectBase::FIGHT_TYPE_BAOJI;
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		Attribute injure = CalcInjureAndType(chara, target_cha, level, fight_type, is_be_effect_skill_per);
		// 增强伤害：对方玩家生命上限的%A
		if (target->GetObjType() == Obj::OBJ_TYPE_ROLE)
		{
			double add_hurt_per = GetParamA(level) * SKILL_ATTR_PER;
			injure += static_cast<Attribute>(target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * add_hurt_per);
		}

		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
		target_cha->AddEffect(effect);
	}

	chara_role->GetGreateSoldier()->SetGreateSoldierBaoji(false);  // 取消该技能强制暴击

	chara->CheckQTEEffect(target);
}


// 名将特殊技能11
// 变身期间，变身期间，每2(该参数为名将特殊技能配置表的参数a)秒对圆形范围(10)内x个目标造成80%攻击的伤害，走公式
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

		// 先判断是否闪避
		if (IsShanbi(chara, target_cha)) continue;

		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		Attribute injure = CalcInjureAndType(chara, target_cha, level, fight_type, is_be_effect_skill_per);

		EffectOnce *effect = new EffectOnce(chara->GetId(), EffectBase::PRODUCT_ID_GREATE_SOLDIER_SPECIAL_SKILL, -injure, fight_type);
		target_cha->AddEffect(effect);
	}

	chara->CheckQTEEffect(target);
}

// 名将特殊技能14
// 变身时，对圆形范围内x个目标造成200%攻击的伤害并使其眩晕，持续param_a(该参数为技能表的参数a)秒
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

		// 先判断是否闪避
		if (IsShanbi(chara, target_cha)) continue;

		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		Attribute injure = CalcInjureAndType(chara, target_cha, level, fight_type, is_be_effect_skill_per);

		EffectOnce *effect = new EffectOnce(chara->GetId(), EffectBase::PRODUCT_ID_GREATE_SOLDIER_SPECIAL_SKILL, -injure, fight_type);
		target_cha->AddEffect(effect);

		// 眩晕
		auto effect_time_ms = GetParamA(level) * 1000;
		EffectSpecialState* special_effect = new EffectSpecialState(chara->GetId(), EffectBase::PRODUCT_ID_GREATE_SOLDIER_SPECIAL_SKILL, effect_time_ms, EBT_XUANYUN, 1);
		special_effect->SetClientEffectType(EffectBase::ECT_SKILL_XUANYUN);
		target_cha->AddEffect(special_effect);
	}

	chara->CheckQTEEffect(target);
}

// 名将特殊技能15
// 变身时，对圆形范围（12）内6个目标拉到自己身边，造成200%的攻击力的伤害并使其移动速度降低param_a万分比，持续param_b秒
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

		// 先判断是否闪避
		if (IsShanbi(chara, target_cha)) continue;

		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		Attribute injure = CalcInjureAndType(chara, target_cha, level, fight_type, is_be_effect_skill_per);

		// 造成攻击伤害
		EffectOnce *effect = new EffectOnce(chara->GetId(), product_id, -injure, fight_type);
		target_cha->AddEffect(effect);

		if (target_cha->IsAlive())
		{
			// 拉受击对象到身前。重置目标位置
			target_cha->SkillResetPos(chara->GetPos(), Character::SKILL_RESET_POS_TYPE_CAPTURE, m_skill_id, false, chara->GetId());
		}

		// 减缓移动速度
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

// 名将特殊技能16
// 变身结束的一瞬间，对周围8个目标造成120%攻击的伤害并使其沉默param_a秒
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

		// 先判断是否闪避
		if (IsShanbi(chara, target_cha)) continue;

		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
		Attribute injure = CalcInjureAndType(chara, target_cha, level, fight_type, is_be_effect_skill_per);

		EffectOnce *effect = new EffectOnce(chara->GetId(), product_id, -injure, fight_type);
		target_cha->AddEffect(effect);

		// 沉默
		int effect_time_ms = GetParamA(level) * 1000;
		EffectSpecialState *special_effect = new EffectSpecialState(target_cha->GetId(), product_id, effect_time_ms, EBT_CHENMO, level, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
		special_effect->SetMergeRule(EffectBase::MERGE_RULE_IGNORE);
		special_effect->SetSave(false);
		special_effect->SetClientEffectType(EffectBase::ECT_SKILL_CHENMO);
		target_cha->AddEffect(special_effect);
	}

	chara->CheckQTEEffect(target);
}



