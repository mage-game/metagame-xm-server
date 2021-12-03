#include "roleskillimpl.hpp"
#include "skill/skilldef.hpp"
#include "obj/character/character.h"
#include "obj/character/monster.h"
#include "ai/monsterai/monsteraibase.hpp"
#include "global/team/team.hpp"
#include "world.h"

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
#include "effect/skilleffect/effectattrstore.hpp"
#include "effect/skilleffect/effectfire.hpp"
#include "effect/skilleffect/effectinvisible.hpp"

#include "effect/sceneeffect/sceneeffect.hpp"
#include "skill/skillmanager.hpp"
#include "skill/skillpool.hpp"

#include "scene/scene.h"
#include "scene/speciallogic/speciallogic.hpp"

#include "protocal/msgscene.h"
#include "protocal/msgfight.h"

#include "gamecommon.h"

// 普通单体技能
#define PERFORM_SINGLE_SKILL(SKILL_ID)\
void RoleSkill##SKILL_ID::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const\
{\
	PerformSingleSkill(chara, level, target, pos, special_param);\
}\
long long RoleSkill##SKILL_ID::PerformSpecial(Character *chara, int level, Character *target, bool is_main_target,int &fight_type, bool is_baoji, double add_hurt_percent) const\
{\
	return 0;\
}\

// 普通AOE技能
#define PERFORM_AOE_SKILL(SKILL_ID)\
void RoleSkill##SKILL_ID::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const\
{\
	PerformAOESkill(chara, level, target, pos, special_param);\
}\
long long RoleSkill##SKILL_ID::PerformSpecial(Character *chara, int level, Character *target, bool is_main_target,int &fight_type, bool is_baoji, double add_hurt_percent) const\
{\
	return 0;\
}\

// 特殊AOE技能
#define PERFORM_SPECIAL_AOE_SKILL(SKILL_ID)\
void RoleSkill##SKILL_ID::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const\
{\
	PerformAOESkill(chara, level, target, pos, special_param);\
}\

//////////////////////////////////////////////////////////////////////////
// 剑斗士

// 普攻
PERFORM_AOE_SKILL(111)

// 技能1，按范围类型伤害周围敌人
void RoleSkill121::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara) return;

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType()) return;

	SendAOEAttackMsg(chara, target, pos);

	int interval = GetParamB(level);
	if (interval <= 0) interval = 1000;

	int count = GetParamA(level) / interval;
	if (count <= 0) count = 1;

// 	Attribute delta_hp = (Attribute)(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (GetHurtPercent(level) * ROLE_ATTR_PER));
// 	delta_hp += GetFixHurt(level);
// 	if (delta_hp <= 0) delta_hp = 1;

	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
	// 是否受技伤影响
	bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);

	ObjID target_list[32] = { INVALID_OBJ_ID };
	const int target_count = GetAOESkillTarget(chara, level, target, pos, target_list, sizeof(target_list) / sizeof(target_list[0]));
	Scene *scene = chara->GetScene();

	for (int i = 0; i < target_count; ++i)
	{
		Character *target_cha = (Character*)scene->GetObj(target_list[i]);
		if (NULL == target_cha) continue;

		Attribute delta_hp = CalcInjureAndType(chara, target_cha, level, fight_type, is_be_effect_skill_per);

		EffectInterval *effect = new EffectInterval(chara->GetId(), m_skill_id, EBT_DROP_BLOOD, level);
		effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
		effect->SetParam(interval, count, -delta_hp, ((Role *)chara)->GetUID());
		effect->SetSave(false);
		effect->SetClientEffectType(EffectBase::ECT_SKILL_DROP_BLOOD);
		target_cha->AddEffect(effect);
	}

// 	EffectFire *effect = new EffectFire(chara->GetId(), m_skill_id, level);
// 	effect->SetFireParam(interval, count, 0, delta_hp);
// 	effect->SetIntervalTime(interval);
// 	effect->SetRandPercent(10);
// 	chara->AddEffect(effect);
}

long long RoleSkill121::PerformSpecial(Character *chara, int level, Character *target, bool is_main_target,int &fight_type, bool is_baoji, double add_hurt_percent) const
{
	return 0;
}

// 技能2，主目标持续掉血
PERFORM_SPECIAL_AOE_SKILL(131)
long long RoleSkill131::PerformSpecial(Character *chara, int level, Character *target, bool is_main_target,int &fight_type, bool is_baoji, double add_hurt_percent) const
{
	if (is_main_target)
	{
		int interval = GetParamB(level);
		if (interval <= 0) interval = 1000;

		int count = GetParamA(level) / interval;
		if (count <= 0) count = 1;

		Attribute delta_hp = (Attribute)(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (GetParamC(level) * ROLE_ATTR_PER));
		delta_hp = chara->GetScene()->GetSpecialLogic()->SpecialRecalcInjure(chara, target, delta_hp);
		if (delta_hp <= 0) delta_hp = 1;

		EffectInterval *effect = new EffectInterval(chara->GetId(), m_skill_id, EBT_DROP_BLOOD, level);
		effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
		effect->SetParam(interval, count, -delta_hp, ((Role *)chara)->GetUID());
		effect->SetSave(false);
		effect->SetClientEffectType(EffectBase::ECT_SKILL_DROP_BLOOD);
		target->AddEffect(effect);
	}
	return 0;
}

// 技能3，对主目标伤害加深
PERFORM_SPECIAL_AOE_SKILL(141)
long long RoleSkill141::PerformSpecial(Character *chara, int level, Character *target, bool is_main_target,int &fight_type, bool is_baoji, double add_hurt_percent) const
{
	if (is_main_target)
	{
		EffectDecHurt *effect = new EffectDecHurt(chara->GetId(), m_skill_id, GetParamA(level), level, EffectBase::PRODUCT_METHOD_SKILL, EBT_BE_HURT_ADD);
		effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
		effect->SetDecPercent(-GetParamB(level));
		target->AddEffect(effect);
	}
	return 0;
}

// 魔导士
// 普攻
PERFORM_AOE_SKILL(211)

// 技能1
PERFORM_AOE_SKILL(221)

// 技能2，无视主目标百分比防御
PERFORM_SPECIAL_AOE_SKILL(231)
long long RoleSkill231::PerformSpecial(Character *chara, int level, Character *target, bool is_main_target,int &fight_type, bool is_baoji, double add_hurt_percent) const
{
	if (is_main_target)
	{
		Attribute target_fangyu = target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU);

		Attribute temp_fangyu = (Attribute)(target_fangyu * (1.0 - GetParamA(level) * ROLE_ATTR_PER));
		if (temp_fangyu < 0) temp_fangyu = 0;
		target->SetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU, temp_fangyu);

		// 是否受技伤影响
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);

		long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per, add_hurt_percent);

		target->SetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU, target_fangyu);
		return injure;
	}
	return 0;
}

// 技能3，对主目标额外百分比攻击的伤害
PERFORM_SPECIAL_AOE_SKILL(241)
long long RoleSkill241::PerformSpecial(Character *chara, int level, Character *target, bool is_main_target,int &fight_type, bool is_baoji, double add_hurt_percent) const
{
	if (is_main_target)
	{
		// 是否受技伤影响
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);

		long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per, add_hurt_percent);
		injure += (long long)(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (GetParamA(level) * ROLE_ATTR_PER));
		injure = chara->GetScene()->GetSpecialLogic()->SpecialRecalcInjure(chara, target, injure);
		return injure;
	}
	return 0;
}

// 攻城战城主技能
//  释放后对自身n范围内的所有小怪（注意，对boss无效）造成必杀，对所有玩家造成玩家当前血量（注意是当前血量 不是最大血量）paramA%的伤害
void RoleSkill6::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara) return;

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType()) return;

	SendAOEAttackMsg(chara, target, pos);

	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;

	ObjID target_list[32] = { INVALID_OBJ_ID };
	const int count = GetAOESkillTarget(chara, level, target, pos, target_list, sizeof(target_list) / sizeof(target_list[0]));
	Scene *scene = chara->GetScene();

	for (int i = 0; i < count; ++i)
	{
		Character *target_cha = dynamic_cast<Character*>(scene->GetObj(target_list[i]));
		if (NULL == target_cha) continue;

		Attribute hurt = 0;

		if (Obj::OBJ_TYPE_ROLE == target_cha->GetObjType())
		{
			Attribute max_hp = target_cha->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			hurt = static_cast<Attribute>(max_hp * (GetParamA(level) * ROLE_ATTR_PER) );
		}

		// 去除小怪必杀 modify 2019-03-21 高文均需求
		//else if (Obj::OBJ_TYPE_MONSTER == target_cha->GetObjType())
		//{
		//	Monster *target_monster = dynamic_cast<Monster *>(target_cha);
		//	if (!target_monster->IsBossMonster())
		//	{
		//		hurt = target_monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
		//	}
		//}

		if (hurt > 0)
		{
			EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -hurt, fight_type, EffectBase::PRODUCT_METHOD_GCZCZ_SKILL);
			target_cha->AddEffect(effect);
		}
	}
}

long long RoleSkill6::PerformSpecial(Character *chara, int level, Character *target, bool is_main_target, int &fight_type, bool is_baoji, double add_hurt_percent) const
{
	return 0;
}

// 跨服BOSS排行榜技能
//  释放后对自身隐身持续paramA毫秒
void RoleSkill7::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara) return;

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType()) return;

	Role *role = (Role *)chara;
	if (NULL == role)
	{
		return;
	}

	SendAttackMsg(chara, target, true);

	//EffectSpecialState *effect = new EffectSpecialState(role->GetId(), m_skill_id, this->GetParamA(level), EBT_INVISIBLE, 1);
	//effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
	//role->AddEffect(effect);
	// 直接用这个隐身效果吧
	EffectInvisible *effect_invisible = new EffectInvisible(role->GetId(), m_skill_id, this->GetParamA(level), 1, EffectBase::PRODUCT_METHOD_SKILL);
	role->AddEffect(effect_invisible);

	chara->CheckQTEEffect(target);
}

long long RoleSkill7::PerformSpecial(Character *chara, int level, Character *target, bool is_main_target, int &fight_type, bool is_baoji, double add_hurt_percent) const
{
	return 0;
}

/*
// 猎魔人
// 普攻
PERFORM_AOE_SKILL(311)

// 技能1
PERFORM_AOE_SKILL(321)

// 技能2，对主目标暴击提高
void RoleSkill331::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara) return;

	SendAOEAttackMsg(chara, target, pos);

	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
	double add_hurt_percent = 0.0f;

	static ObjID target_list[32] = {INVALID_OBJ_ID};
	const int count = GetAOESkillTarget(chara, level, target, pos, target_list, sizeof(target_list) / sizeof(target_list[0]));
	Scene *scene = chara->GetScene();

	for (int i = 0; i < count; ++i)
	{
		Character *target_cha = (Character*)scene->GetObj(target_list[i]);
		if (NULL == target_cha) continue;

		// 暴击和闪避计算
		bool is_baoji = false;
		if (target_cha == target)
		{
			Attribute baoji = chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI);

			Attribute temp_baoji = baoji + (Attribute)(baoji * (GetParamA(level) * ROLE_ATTR_PER));
			if (temp_baoji < 0) temp_baoji = 0;
			chara->SetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI, temp_baoji);

			is_baoji = IsBaoji(chara, target_cha);

			chara->SetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI, baoji);
		}
		else
		{
			is_baoji = IsBaoji(chara, target_cha);
		}
		if (!is_baoji && IsShanbi(chara, target_cha)) continue;

		if (target_cha == target)
		{
			add_hurt_percent = 0.0f;
		}
		else if (Obj::OBJ_TYPE_ROLE == target_cha->GetObjType())
		{
			add_hurt_percent = GetParamE(level) * ROLE_ATTR_PER - 1.0;
		}
		else
		{
			add_hurt_percent = GetParamF(level) * ROLE_ATTR_PER - 1.0;
		}

		long long injure = CalcInjureAndType(chara, target_cha, level, fight_type, is_baoji, add_hurt_percent);

		int passive_flag = this->HurtTrigger(chara, target_cha, injure, fight_type, i);
		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
		effect->SetPassiveFlag(passive_flag);
		target_cha->AddEffect(effect);

		if (m_is_repel && Obj::OBJ_TYPE_ROLE != target_cha->GetObjType() && Obj::OBJ_TYPE_ROLE_SHADOW != target_cha->GetObjType())
		{
			Posi deltaP = chara->GetPos() - target_cha->GetPos();
			Scalar distance = (Scalar)GetParamG(level);
			Dir dir = gamecommon::GetMoveDir(deltaP, NULL);
			if (dir != Dir_S)
			{
				Posi diff_pos((Axis)(distance * cos(dir)), (Axis)(distance * sin(dir)));
				Posi dest_pos = target_cha->GetPos() + diff_pos;
				target_cha->SkillResetPos(dest_pos, Character::SKILL_RESET_POS_TYPE_JITUI, m_skill_id, true, chara->GetId());
			}
		}
	}
}

int RoleSkill331::PerformSpecial(Character *chara, int level, Character *target, bool is_main_target,int &fight_type, bool is_baoji, double add_hurt_percent) const
{
	return 0;
}

// 技能3，有概率对主目标伤害加深
PERFORM_SPECIAL_AOE_SKILL(341)
int RoleSkill341::PerformSpecial(Character *chara, int level, Character *target, bool is_main_target,int &fight_type, bool is_baoji, double add_hurt_percent) const
{
	if (is_main_target)
	{
		int percent = (int)(GetParamA(level) * ROLE_ATTR_PER * 100.0);
		if (RandomNum(100) < percent)
		{
			add_hurt_percent += GetParamB(level) * ROLE_ATTR_PER;
		}
	}
	return CalcInjureAndType(chara, target, level, fight_type, is_baoji, add_hurt_percent);
}
*/
//职业三技能和职业一技能一样，职业四和职业二技能一样
// 普攻
PERFORM_AOE_SKILL(311)

// 技能1，按范围类型伤害周围敌人
void RoleSkill321::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara) return;

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType()) return;

	SendAOEAttackMsg(chara, target, pos);

	int interval = GetParamB(level);
	if (interval <= 0) interval = 1000;

	int count = GetParamA(level) / interval;
	if (count <= 0) count = 1;

// 	Attribute delta_hp = (Attribute)(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (GetHurtPercent(level) * ROLE_ATTR_PER));
// 	delta_hp += GetFixHurt(level);
// 	if (delta_hp <= 0) delta_hp = 1;

	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
	// 是否受技伤影响
	bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);

	ObjID target_list[32] = { INVALID_OBJ_ID };
	const int target_count = GetAOESkillTarget(chara, level, target, pos, target_list, sizeof(target_list) / sizeof(target_list[0]));
	Scene *scene = chara->GetScene();

	for (int i = 0; i < target_count; ++i)
	{
		Character *target_cha = (Character*)scene->GetObj(target_list[i]);
		if (NULL == target_cha) continue;

		Attribute delta_hp = CalcInjureAndType(chara, target_cha, level, fight_type, is_be_effect_skill_per);

		EffectInterval *effect = new EffectInterval(chara->GetId(), m_skill_id, EBT_DROP_BLOOD, level);
		effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
		effect->SetParam(interval, count, -delta_hp, ((Role *)chara)->GetUID());
		effect->SetSave(false);
		effect->SetClientEffectType(EffectBase::ECT_SKILL_DROP_BLOOD);

		target_cha->AddEffect(effect);
	}

// 	EffectFire *effect = new EffectFire(chara->GetId(), m_skill_id, level);
// 	effect->SetFireParam(interval, count, 0, delta_hp);
// 	effect->SetIntervalTime(interval);
// 	effect->SetRandPercent(10);
// 	chara->AddEffect(effect);
}

long long RoleSkill321::PerformSpecial(Character *chara, int level, Character *target, bool is_main_target, int &fight_type, bool is_baoji, double add_hurt_percent) const
{
	return 0;
}

// 技能2，主目标持续掉血
PERFORM_SPECIAL_AOE_SKILL(331)
long long RoleSkill331::PerformSpecial(Character *chara, int level, Character *target, bool is_main_target, int &fight_type, bool is_baoji, double add_hurt_percent) const
{
	if (is_main_target)
	{
		int interval = GetParamB(level);
		if (interval <= 0) interval = 1000;

		int count = GetParamA(level) / interval;
		if (count <= 0) count = 1;

		Attribute delta_hp = (Attribute)(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (GetParamC(level) * ROLE_ATTR_PER));
		delta_hp = chara->GetScene()->GetSpecialLogic()->SpecialRecalcInjure(chara, target, delta_hp);
		if (delta_hp <= 0) delta_hp = 1;

		EffectInterval *effect = new EffectInterval(chara->GetId(), m_skill_id, EBT_DROP_BLOOD, level);
		effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
		effect->SetParam(interval, count, -delta_hp, ((Role *)chara)->GetUID());
		effect->SetSave(false);
		effect->SetClientEffectType(EffectBase::ECT_SKILL_DROP_BLOOD);
		target->AddEffect(effect);
	}
	return 0;
}

// 技能3，对主目标伤害加深
PERFORM_SPECIAL_AOE_SKILL(341)
long long RoleSkill341::PerformSpecial(Character *chara, int level, Character *target, bool is_main_target, int &fight_type, bool is_baoji, double add_hurt_percent) const
{
	if (is_main_target)
	{
		EffectDecHurt *effect = new EffectDecHurt(chara->GetId(), m_skill_id, GetParamA(level), level, EffectBase::PRODUCT_METHOD_SKILL, EBT_BE_HURT_ADD);
		effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
		effect->SetDecPercent(-GetParamB(level));
		target->AddEffect(effect);
	}
	return 0;
}

// 魔导士
// 普攻
PERFORM_AOE_SKILL(411)

// 技能1
PERFORM_AOE_SKILL(421)

// 技能2，无视主目标百分比防御
PERFORM_SPECIAL_AOE_SKILL(431)
long long RoleSkill431::PerformSpecial(Character *chara, int level, Character *target, bool is_main_target, int &fight_type, bool is_baoji, double add_hurt_percent) const
{
	if (is_main_target)
	{
		Attribute target_fangyu = target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU);

		Attribute temp_fangyu = (Attribute)(target_fangyu * (1.0 - GetParamA(level) * ROLE_ATTR_PER));
		if (temp_fangyu < 0) temp_fangyu = 0;
		target->SetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU, temp_fangyu);

		// 是否受技伤影响
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);

		long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per, add_hurt_percent);

		target->SetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU, target_fangyu);
		return injure;
	}
	return 0;
}

// 技能3，对主目标额外百分比攻击的伤害
PERFORM_SPECIAL_AOE_SKILL(441)
long long RoleSkill441::PerformSpecial(Character *chara, int level, Character *target, bool is_main_target, int &fight_type, bool is_baoji, double add_hurt_percent) const
{
	if (is_main_target)
	{
		// 是否受技伤影响
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);

		long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per, add_hurt_percent);
		injure += (long long)(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (GetParamA(level) * ROLE_ATTR_PER));
		injure = chara->GetScene()->GetSpecialLogic()->SpecialRecalcInjure(chara, target, injure);
		return injure;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
void RoleSkill41::ReCalcAttr(Character *chara, int skill_level, CharIntAttrs &base_add) const
{
	base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += GetParamA(skill_level);
}

void RoleSkill42::ReCalcAttr(Character *chara, int skill_level, CharIntAttrs &base_add) const
{
	base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += GetParamA(skill_level);
}

void RoleSkill43::ReCalcAttr(Character *chara, int skill_level, CharIntAttrs &base_add) const
{
	base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += GetParamA(skill_level);
}

void RoleSkill44::ReCalcAttr(Character *chara, int skill_level, CharIntAttrs &base_add) const
{
	base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += GetParamA(skill_level);
}

void RoleSkill45::ReCalcAttr(Character *chara, int skill_level, CharIntAttrs &base_add) const
{
	base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += GetParamA(skill_level);
}

void RoleSkill46::ReCalcAttr(Character *chara, int skill_level, CharIntAttrs &base_add) const
{
	base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += GetParamA(skill_level);
}

void RoleSkill47::ReCalcAttr(Character *chara, int skill_level, CharIntAttrs &base_add) const
{
	base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += GetParamA(skill_level);
}
