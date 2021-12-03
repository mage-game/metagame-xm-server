#include "commonskill.hpp"

#include "obj/character/character.h"
#include "effect/skilleffect/effectonce.hpp"

#include "protocal/msgscene.h"

#include "scene/scene.h"
#include "obj/character/monster.h"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effectinterval.hpp"
#include "effect/skilleffect/effectreboundhurt.hpp"
#include "effect/skilleffect/effectabsorbhp.hpp"
#include "scene/speciallogic/speciallogic.hpp"

#define PERFORM_NORMAL_SKILL(SKILL_ID)\
void NormalSkill##SKILL_ID::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const\
{\
	if (NULL == chara || NULL == target) return;\
	SendAttackMsg(chara, target);\
	if (IsShanbi(chara, target)) return;\
	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;\
	bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);\
	long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);\
	int passive_flag = this->HurtTrigger(chara, target, injure, fight_type, 0);\
	EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);\
	effect->SetPassiveFlag(passive_flag);\
	target->AddEffect(effect);\
}\

//////////////////////////////////////////////////////////////////////////
PERFORM_NORMAL_SKILL(1)
PERFORM_NORMAL_SKILL(2)
PERFORM_NORMAL_SKILL(3)
PERFORM_NORMAL_SKILL(4)

// 转职技能 ///////////////////////////////////////////////////////////////////////

void NormalSkill180::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target) return;

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType()) return;
	SendAttackMsg(chara, target);

	int add_per = this->GetParamA(level);
	int effect_ms = this->GetParamB(level);

	Attribute deta_hp = static_cast<Attribute>(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (add_per * SKILL_ATTR_PER));
	EffectOnce *dec_effect = new EffectOnce(chara->GetId(), m_skill_id, -deta_hp, EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	target->AddEffect(dec_effect);

	if (Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		EffectSpecialState *effect_dingshen = new EffectSpecialState(chara->GetId(), m_skill_id, effect_ms, EBT_DINGSHEN, 1, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
		effect_dingshen->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
		effect_dingshen->SetClientEffectType(EffectBase::ECT_SKILL_DINGSHEN);
		effect_dingshen->SetSave(false);

		target->AddEffect(effect_dingshen);
	}
}


void NormalSkill181::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target) return;

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType()) return;
	SendAttackMsg(chara, target);

	int delim_hp_per = this->GetParamA(level);
	int add_hp_per1 = this->GetParamB(level);
	int add_hp_per2 = this->GetParamC(level);
	int add_fangyu = this->GetParamD(level);
	int effect_ms = this->GetParamE(level);

	Attribute cur_target_hp = chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
	Attribute max_target_hp = chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
	int hp_per = static_cast<int>(cur_target_hp / max_target_hp * 10000.0f);
	Attribute recover_hp = 0;
	if (hp_per >= delim_hp_per)
	{
		recover_hp = static_cast<Attribute>(max_target_hp * (add_hp_per1 * SKILL_ATTR_PER));
	}
	else
	{
		recover_hp = static_cast<Attribute>(max_target_hp * (add_hp_per2 * SKILL_ATTR_PER));
	}

// 	if (chara->IsAlive() && !chara->IsFullHp())
// 	{
// 		chara->Recover(&recover_hp, true);
// 	}

	EffectInterval *effect_interval = new EffectInterval(chara->GetId(), m_skill_id, EBT_ADD_BLOOD, 1, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	effect_interval->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
	effect_interval->SetParam(1000, 1, recover_hp);
	effect_interval->SetSave(false);

	chara->AddEffect(effect_interval);


	EffectBuff *effect_buff = new EffectBuff(chara->GetId(), m_skill_id, effect_ms, 1, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
	effect_buff->AddFangYuEffect(add_fangyu);
	effect_buff->SetSave(false);

	chara->AddEffect(effect_buff);
}

void NormalSkill182::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target) return;

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType() || (Obj::OBJ_TYPE_ROLE != target->GetObjType() && Obj::OBJ_TYPE_ROLE_SHADOW != target->GetObjType())) return;
	SendAttackMsg(chara, target);

	int effect_ms = this->GetParamA(level);

	ObjID target_list[32] = { INVALID_OBJ_ID };
	const int target_count = GetAOESkillTarget(chara, level, target, pos, target_list, sizeof(target_list) / sizeof(target_list[0]));
	Scene *scene = chara->GetScene();

	for (int i = 0; i < target_count; ++i)
	{
		Character *target_cha = (Character*)scene->GetObj(target_list[i]);
		if (NULL == target_cha || Obj::OBJ_TYPE_ROLE != target_cha->GetObjType()) continue;

		EffectSpecialState *specail_state = new EffectSpecialState(chara->GetId(), m_skill_id, effect_ms, EBT_XUANYUN, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
		specail_state->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
		specail_state->SetClearOnDie(true);
		specail_state->SetClientEffectType(EffectBase::ECT_SKILL_XUANYUN);
		target_cha->AddEffect(specail_state);
	}
}

void NormalSkill183::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target) return;

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType() || (Obj::OBJ_TYPE_ROLE != target->GetObjType() && Obj::OBJ_TYPE_ROLE_SHADOW != target->GetObjType())) return;
	SendAttackMsg(chara, target);

	int rebound_per = this->GetParamA(level);
	int effect_ms = this->GetParamB(level);

	EffectReboundHurt *effect_rebound = new EffectReboundHurt(chara->GetId(), m_skill_id, effect_ms, rebound_per / 100, level, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	effect_rebound->SetClientEffectType(EffectBase::ECT_SKILL_REBOUNDHURT);
	chara->AddEffect(effect_rebound);
}

void NormalSkill184::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target) return;

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType() || (Obj::OBJ_TYPE_ROLE != target->GetObjType() && Obj::OBJ_TYPE_ROLE_SHADOW != target->GetObjType())) return;
	SendAttackMsg(chara, target);

	int down_fangyu_per = this->GetParamA(level);
	int effect_ms = this->GetParamB(level);

	ObjID target_list[32] = { INVALID_OBJ_ID };
	const int target_count = GetAOESkillTarget(chara, level, target, pos, target_list, sizeof(target_list) / sizeof(target_list[0]));
	Scene *scene = chara->GetScene();

	for (int i = 0; i < target_count; ++i)
	{
		Character *target_cha = (Character*)scene->GetObj(target_list[i]);
		if (NULL == target_cha || Obj::OBJ_TYPE_ROLE != target_cha->GetObjType()) continue;

		EffectBuff *down_fangyu = new EffectBuff(chara->GetId(), m_skill_id, effect_ms, level, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
		down_fangyu->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
		Attribute value = static_cast<Attribute>(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU) * (down_fangyu_per * ROLE_ATTR_PER));
		down_fangyu->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU, -value, -down_fangyu_per);
		down_fangyu->SetClientEffectType(EffectBase::ECT_SKILL_DEC_FANGYU);
		target_cha->AddEffect(down_fangyu);
	}
}

void NormalSkill185::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target) return;

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType() || (Obj::OBJ_TYPE_ROLE != target->GetObjType() && Obj::OBJ_TYPE_ROLE_SHADOW != target->GetObjType())) return;
	SendAttackMsg(chara, target);

	int absorb_per = this->GetParamA(level);
	int effect_ms = this->GetParamB(level);

	EffectAbsorbHP *effect = new EffectAbsorbHP(chara->GetId(), m_skill_id, effect_ms, absorb_per / 100, level, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL, EBT_ZHUANZHI_ABSORB_HP);
	effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
	effect->SetClientEffectType(EffectBase::ECT_IMG_FULING_ABSORB);
	chara->AddEffect(effect);
}

void NormalSkill186::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target) return;

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType() || (Obj::OBJ_TYPE_ROLE != target->GetObjType() && Obj::OBJ_TYPE_ROLE_SHADOW != target->GetObjType())) return;
	SendAttackMsg(chara, target);

	int interval = this->GetParamB(level);
	if (interval <= 0) interval = 1000;

	int count = GetParamA(level) / interval;
	if (count <= 0) count = 1;

	int delta_hp_per = GetParamC(level);

	ObjID target_list[32] = { INVALID_OBJ_ID };
	const int target_count = GetAOESkillTarget(chara, level, target, pos, target_list, sizeof(target_list) / sizeof(target_list[0]));
	Scene *scene = chara->GetScene();

	for (int i = 0; i < target_count; ++i)
	{
		Character *target_cha = (Character*)scene->GetObj(target_list[i]);
		if (NULL == target_cha || Obj::OBJ_TYPE_ROLE != target_cha->GetObjType()) continue;

		Attribute delta_hp = static_cast<Attribute>(target_cha->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (delta_hp_per * ROLE_ATTR_PER));
		Attribute max_delta_hp = static_cast<Attribute>(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (GetParamD(level) * ROLE_ATTR_PER));
		Attribute real_delta_hp = delta_hp * count > max_delta_hp ? max_delta_hp / count : delta_hp;
		
		EffectInterval *effect = new EffectInterval(chara->GetId(), m_skill_id, EBT_DROP_BLOOD_POISON, level);
		effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
		effect->SetParam(interval, count, -real_delta_hp, ((Role *)chara)->GetUID());
		effect->SetSave(false);
		effect->SetClientEffectType(EffectBase::ECT_SKILL_POISON);
		target_cha->AddEffect(effect);
	}
}

void NormalSkill187::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target) return;

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType() || (Obj::OBJ_TYPE_ROLE != target->GetObjType() && Obj::OBJ_TYPE_ROLE_SHADOW != target->GetObjType()) ) return;
	SendAttackMsg(chara, target);

	int add_shanbi_per = this->GetParamA(level);
	int effect_ms = this->GetParamB(level);

	EffectBuff *add_shanbi_per_effect = new EffectBuff(chara->GetId(), m_skill_id, effect_ms, level, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL, EBT_ZHUANZHI_ADD_SHANBI);
	add_shanbi_per_effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
	Attribute value = static_cast<Attribute>(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI_PER) * (add_shanbi_per * ROLE_ATTR_PER));
	add_shanbi_per_effect->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI_PER, value, add_shanbi_per);
	add_shanbi_per_effect->SetClientEffectType(EffectBase::ECT_SKILL_ADD_SHANBI_PER);
	chara->AddEffect(add_shanbi_per_effect);
}

void NormalSkill188::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target) return;

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType() || (Obj::OBJ_TYPE_ROLE != target->GetObjType() && Obj::OBJ_TYPE_ROLE_SHADOW != target->GetObjType())) return;
	SendAttackMsg(chara, target);

	int effect_ms = this->GetParamA(level);

	ObjID target_list[32] = { INVALID_OBJ_ID };
	const int target_count = GetAOESkillTarget(chara, level, target, pos, target_list, sizeof(target_list) / sizeof(target_list[0]));
	Scene *scene = chara->GetScene();

	for (int i = 0; i < target_count; ++i)
	{
		Character *target_cha = (Character*)scene->GetObj(target_list[i]);
		if (NULL == target_cha || Obj::OBJ_TYPE_ROLE != target_cha->GetObjType()) continue;		

		EffectSpecialState *special_effect = new EffectSpecialState(target_cha->GetId(), m_skill_id, effect_ms, EBT_CHENMO, level, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
		special_effect->SetMergeRule(EffectBase::MERGE_RULE_IGNORE);
		special_effect->SetSave(false);
		special_effect->SetClientEffectType(EffectBase::ECT_SKILL_CHENMO);
		target_cha->AddEffect(special_effect);
	}
}

void NormalSkill189::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara || NULL == target) return;

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType() || (Obj::OBJ_TYPE_ROLE != target->GetObjType() && Obj::OBJ_TYPE_ROLE_SHADOW != target->GetObjType())) return;
	SendAttackMsg(chara, target);

	int add_hurt_per = this->GetParamA(level);
	int effect_ms = this->GetParamB(level);

	EffectBuff *add_gongji = new EffectBuff(chara->GetId(), m_skill_id, effect_ms, level, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL, EBT_ZHUANZHI_ADD_SHANGHAI);
	add_gongji->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
	Attribute value = static_cast<Attribute>(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (add_hurt_per * ROLE_ATTR_PER));
	add_gongji->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, value, add_hurt_per);
	add_gongji->SetClientEffectType(EffectBase::ECT_SKILL_ADD_GONGJI);
	chara->AddEffect(add_gongji);
}

//////////////////////////////////////////////////////////////////////////
//必杀技
void NuqiSkill101::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType() && Obj::OBJ_TYPE_ROLE_SHADOW != chara->GetObjType())
	{
		return;
	}

	PerformAOESkill(chara, level, target, pos, special_param);
}

long long NuqiSkill101::PerformSpecial(Character *chara, int level, Character *target, bool is_main_target,int &fight_type, bool is_baoji, double add_hurt_percent) const
{
	// 是否受技伤影响
	bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);

	long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per, add_hurt_percent);
	if (is_main_target && NULL != target && Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		long long min_injure = (long long)(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (GetParamA(level) * ROLE_ATTR_PER));
		if (injure < min_injure)
		{
			double rate = 0.0;

			int param_b = GetParamB(level);
			if (param_b > 0)
			{
				rate = RandomNum(-param_b, param_b + 1) * ROLE_ATTR_PER;
			}

			injure = min_injure + (long long)(min_injure * rate);

			if (injure <= 0)
			{
				injure = 1;
			}
		}
	}

	// 普通场景打小怪直接秒杀
	if (Scene::IsStaticScene(chara->GetScene()->GetSceneType()) && NULL != target && Obj::OBJ_TYPE_MONSTER == target->GetObjType())
	{
		Monster *monster = (Monster *)target;
		if (!monster->IsBossMonster())
		{
			injure = (std::max)(injure, target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP));
		}
	}

	return injure;
}

void NuqiSkill201::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType() && Obj::OBJ_TYPE_ROLE_SHADOW != chara->GetObjType())
	{
		return;
	}

	PerformAOESkill(chara, level, target, pos, special_param);
}

long long NuqiSkill201::PerformSpecial(Character *chara, int level, Character *target, bool is_main_target, int &fight_type, bool is_baoji, double add_hurt_percent) const
{
	// 是否受技伤影响
	bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);

	long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per, add_hurt_percent);
	if (is_main_target && NULL != target && Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		long long min_injure = (long long)(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (GetParamA(level) * ROLE_ATTR_PER));
		if (injure < min_injure)
		{
			double rate = 0.0;

			int param_b = GetParamB(level);
			if (param_b > 0)
			{
				rate = RandomNum(-param_b, param_b + 1) * ROLE_ATTR_PER;
			}

			injure = min_injure + (long long)(min_injure * rate);

			if (injure <= 0)
			{
				injure = 1;
			}
		}
	}

	// 普通场景打小怪直接秒杀
	if (Scene::IsStaticScene(chara->GetScene()->GetSceneType()) && NULL != target && Obj::OBJ_TYPE_MONSTER == target->GetObjType())
	{
		Monster *monster = (Monster *)target;
		if (!monster->IsBossMonster())
		{
			injure = (std::max)(injure, target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP));
		}
	}

	return injure;
}

void NuqiSkill301::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType() && Obj::OBJ_TYPE_ROLE_SHADOW != chara->GetObjType())
	{
		return;
	}

	PerformAOESkill(chara, level, target, pos, special_param);
}

long long NuqiSkill301::PerformSpecial(Character *chara, int level, Character *target, bool is_main_target, int &fight_type, bool is_baoji, double add_hurt_percent) const
{
	// 是否受技伤影响
	bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);

	long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per, add_hurt_percent);
	if (is_main_target && NULL != target && Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		long long min_injure = (long long)(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (GetParamA(level) * ROLE_ATTR_PER));
		if (injure < min_injure)
		{
			double rate = 0.0;

			int param_b = GetParamB(level);
			if (param_b > 0)
			{
				rate = RandomNum(-param_b, param_b + 1) * ROLE_ATTR_PER;
			}

			injure = min_injure + (long long)(min_injure * rate);

			if (injure <= 0)
			{
				injure = 1;
			}
		}
	}

	// 普通场景打小怪直接秒杀
	if (Scene::IsStaticScene(chara->GetScene()->GetSceneType()) && NULL != target && Obj::OBJ_TYPE_MONSTER == target->GetObjType())
	{
		Monster *monster = (Monster *)target;
		if (!monster->IsBossMonster())
		{
			injure = (std::max)(injure, target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP));
		}
	}

	return injure;
}

void NuqiSkill401::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType() && Obj::OBJ_TYPE_ROLE_SHADOW != chara->GetObjType())
	{
		return;
	}

	PerformAOESkill(chara, level, target, pos, special_param);
}

long long NuqiSkill401::PerformSpecial(Character *chara, int level, Character *target, bool is_main_target, int &fight_type, bool is_baoji, double add_hurt_percent) const
{
	// 是否受技伤影响
	bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);

	long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per, add_hurt_percent);
	if (is_main_target && NULL != target && Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		long long min_injure = (long long)(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (GetParamA(level) * ROLE_ATTR_PER));
		if (injure < min_injure)
		{
			double rate = 0.0;

			int param_b = GetParamB(level);
			if (param_b > 0)
			{
				rate = RandomNum(-param_b, param_b + 1) * ROLE_ATTR_PER;
			}

			injure = min_injure + (long long)(min_injure * rate);

			if (injure <= 0)
			{
				injure = 1;
			}
		}
	}

	// 普通场景打小怪直接秒杀
	if (Scene::IsStaticScene(chara->GetScene()->GetSceneType()) && NULL != target && Obj::OBJ_TYPE_MONSTER == target->GetObjType())
	{
		Monster *monster = (Monster *)target;
		if (!monster->IsBossMonster())
		{
			injure = (std::max)(injure, target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP));
		}
	}

	return injure;
}