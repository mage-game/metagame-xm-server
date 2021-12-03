#include "mojieskillmpl.hpp"
#include "obj/character/character.h"
#include "obj/character/monster.h"

#include "skill/skilldef.hpp"
#include "effect/effectbase.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectinterval.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "effect/skilleffect/effecthurtdec.hpp"
#include "effect/skilleffect/effectcounter.hpp"
#include "effect/skilleffect/effectbianshen.hpp"
#include "effect/skilleffect/effectwudiprotect.hpp"
#include "effect/skilleffect/effectmojiewudiprotect.hpp"
#include "effect/sceneeffect/sceneeffect.hpp"

#include "gamecommon.h"
#include "scene/scene.h"
#include "scene/speciallogic/speciallogic.hpp"

#include "protocal/msgscene.h"
#include "protocal/msgfight.h"

#include "other/specialappearance/specialappearance.hpp"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MojieSkill70::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// 对目标及周围N个目标造成param_a%攻击的伤害，并且把所有受击对象拉到身前。有param_b概率禁锢目标param_c毫秒
	if (NULL == chara || NULL == target)
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType()) return;

	SendAOEAttackMsg(chara, target, pos);

	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
	long long injure = (long long)(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (this->GetParamA(level) * ROLE_ATTR_PER));
	if (injure <= 0) injure = 1;

	static const int TARGET_NUM = 32;
	ObjID target_list[TARGET_NUM] = { INVALID_OBJ_ID };
	const int count = GetAOESkillTarget(chara, level, target, target->GetPos(), target_list, sizeof(target_list) / sizeof(target_list[0]));
	Scene *scene = chara->GetScene();

	int rand_value = RandomNum(10000);

	for (int i = 0; i < count && i < TARGET_NUM; ++i)
	{
		Character *target_cha = (Character*)scene->GetObj(target_list[i]);
		if (NULL == target_cha) continue;

		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
		target_cha->AddEffect(effect);

		if (target_cha->IsAlive())
		{
			// 拉受击对象到身前。重置目标位置
			target_cha->SkillResetPos(chara->GetPos(), Character::SKILL_RESET_POS_TYPE_CAPTURE, m_skill_id, false, chara->GetId());
		
		
			// 禁锢
			if (rand_value < this->GetParamB(level))
			{
				int buff_continue_time = GetParamC(level);
				EffectSpecialState *effect_dingshen = new EffectSpecialState(chara->GetId(), m_skill_id, buff_continue_time, EBT_DINGSHEN, level, EffectBase::PRODUCT_METHOD_SKILL);
				effect_dingshen->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
				effect_dingshen->SetClientEffectType(EffectBase::ECT_SKILL_DINGSHEN);
				target_cha->AddEffect(effect_dingshen);
			}
		}
	}

	chara->CheckQTEEffect(target);
}

void MojieSkill71::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// 攻击时有概率把主目标变成怪物（要有内置CD）。
	if (NULL == chara || NULL == target)
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE != target->GetObjType()) return;

	SendAttackMsg(chara, target);

	int buff_continue_time = this->GetParamA(level);
	//int buff_dec_speed_per = this->GetParamB(level);

	//目标变身
	EffectBianshen *effect_bianshen = new EffectBianshen(chara->GetId(),m_skill_id, buff_continue_time, 1, EffectBase::PRODUCT_METHOD_SKILL);
	effect_bianshen->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
	effect_bianshen->SetImage(APPEARANCE_MOJIE_GUAIWU);
	effect_bianshen->SetClientEffectType(EffectBase::ECT_SKILL_BIANSHEN);
	target->AddEffect(effect_bianshen);

	// 查找BUFF是否添加成功
	EffectBase *add_buff = target->GetEffectList()->FindEffect(EffectBase::EFFECT_TYPE_BIANSHEN, EBT_BIANXING_FOOL, chara->GetId());
	if (nullptr != add_buff)
	{
		// 广播形象
		static Protocol::SCBianShenView cmd;
		cmd.obj_id = target->GetId();
		cmd.bianshen_image = APPEARANCE_MOJIE_GUAIWU;
		target->GetScene()->GetZoneMatrix()->NotifyAreaMsg(target->GetObserHandle(), (void*)&cmd, sizeof(cmd));
	}

	// 目标减速
	//EffectMoveSpeed *effect_speed = new EffectMoveSpeed(chara->GetId(), m_skill_id, buff_continue_time, EBT_CHIHUAN, level, EffectBase::PRODUCT_METHOD_SKILL);
	//effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
	//int value = static_cast<int>(target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED) * (buff_dec_speed_per * ROLE_ATTR_PER));
	//effect_speed->SetMoveSpeed(-value, -buff_dec_speed_per / 100);
	//target->AddEffect(effect_speed);

	chara->CheckQTEEffect(target);
}

void MojieSkill72::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// 自身生命值低于param_a%时，无敌param_b毫秒，冷却120秒。
	if (NULL == chara || NULL == target || chara != target)
	{
		return;
	}

	SendAttackMsg(chara, target);

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType()) return;

	// 无敌
	int buff_continue_time = this->GetParamB(level);
	if (buff_continue_time < 1000) buff_continue_time = 1000;
	
	EffectMojieWudiProtect *effect_wudi = new EffectMojieWudiProtect(chara->GetId(), m_skill_id, buff_continue_time, level);
	effect_wudi->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
	effect_wudi->SetClientEffectType(EffectBase::ECT_MOJIE_SKILL_WUDI);
	chara->AddEffect(effect_wudi);

	chara->CheckQTEEffect(target);
}

void MojieSkill73::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// 受到致死一击时，拦截死亡，并且恢复param_a%最大生命值。
	if (NULL == chara || NULL == target || chara != target) return;

	SendAttackMsg(chara, target);

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType()) return;
	if (!chara->IsAlive()) return;
	
	// 恢复血量
	Attribute recover_hp = static_cast<Attribute>(chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (GetParamA(level) * ROLE_ATTR_PER));
	EffectOnce *addhp_effect = new EffectOnce(INVALID_OBJ_ID, 0, recover_hp, EffectBase::FIGHT_TYPE_NORMAL);
	chara->AddEffect(addhp_effect);

	// 加回血特效
	EffectInterval *effect_interval = new EffectInterval(chara->GetId(), m_skill_id, EBT_MOJIE_RESTORE_HP, 1);
	effect_interval->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
	//effect_interval->SetClientEffectType(EffectBase::ECT_MOJIE_SKILL_RESTORE_HP);
	effect_interval->SetParam(1000, 1, 0);
	effect_interval->SetSave(false);
	chara->AddEffect(effect_interval);

	chara->CheckQTEEffect(target);
}



