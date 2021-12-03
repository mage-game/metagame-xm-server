#include "pinkequipskillmpl.hpp"
#include "obj/character/character.h"
#include "obj/character/monster.h"
#include "obj/character/role.h"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "scene/map.h"
#include "scene/scene.h"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "effect/skilleffect/effectwudiprotect.hpp"

void PinkEquipSkill170::Perform(Character* chara, int level, Character* target, const Posi& pos, int special_param) const
{
	// 1）武器绝杀技：暗影突袭，主动使用后隐身，隐身期间对方不能选中自己，技能持续Y秒，隐身消除后的第一下攻击，额外扣除对方Z%最大血量

	if (NULL == chara)
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType())
	{
		return;
	}

	Role *role = (Role *)chara;
	if (NULL == role)
	{
		return;
	}

	SendAttackMsg(chara, target, true);

	EffectSpecialState *effect = new EffectSpecialState(role->GetId(), m_skill_id, this->GetParamA(1) * 1000, EBT_INVISIBLE, 1);
	effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
	role->AddEffect(effect);

	role->NextAttackExtHurtPercent(this->GetParamB(1));

	chara->CheckQTEEffect(target);
}

void PinkEquipSkill171::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// 2）护手绝杀技：主动使用后，可以把选中的人及其周边X格子的人变小，并且减速Y%，持续Z秒

	if (NULL == chara || NULL == target)
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType() || Obj::OBJ_TYPE_ROLE != target->GetObjType())
	{
		return;
	}

	Role *role = (Role *)chara;
	if (NULL == role)
	{
		return;
	}

	Role *target_role = (Role *)target;
	if (NULL == target_role)
	{
		return;
	}

	SendAttackMsg(chara, target);

	const int MAX_EFFECT_CHARACTER_NUM = 30; // 最大影响角色数
	ObjID effect_obj_list[MAX_EFFECT_CHARACTER_NUM]; memset(effect_obj_list, 0, sizeof(effect_obj_list));

	int range_value = this->GetParamA(1);
	short dec_movespeed_percent = this->GetParamB(1);
	int last_time_s = this->GetParamC(1);

	Scene *scene = target_role->GetScene();
	if (NULL != scene)
	{
		int real_role_num = scene->GetZoneMatrix()->GetObjByArea(target_role->GetPos(), range_value, range_value, effect_obj_list, MAX_EFFECT_CHARACTER_NUM);
		for (int i = 0; i < real_role_num && i < MAX_EFFECT_CHARACTER_NUM; ++i)
		{
			Obj *obj = scene->GetObj(effect_obj_list[i]);
			if (NULL != obj && Obj::IsCharactor(obj->GetObjType()) && role->IsEnemy(obj))
			{
				Character *t_charactor = (Character *)obj;

				EffectSpecialState *effect_state = new EffectSpecialState(chara->GetId(), 0, last_time_s * 1000, EBT_PINK_EQUIP_NARROW, 1);
				effect_state->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
				t_charactor->AddEffect(effect_state);

				int dec_movespeed = static_cast<int>(t_charactor->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) * dec_movespeed_percent / 100);

				EffectMoveSpeed *effect_speed = new EffectMoveSpeed(role->GetId(), 0, last_time_s * 1000, EBT_CHIHUAN, 1, EffectBase::PRODUCT_METHOD_FAZHEN_HALO);
				effect_speed->SetMoveSpeed(-dec_movespeed, -dec_movespeed_percent);
				effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
				effect_speed->SetClientEffectType(EffectBase::ECT_JL_SKILL_JSHU);
				effect_speed->SetSave(false);
				t_charactor->AddEffect(effect_speed);
			}
		}
	}

	chara->CheckQTEEffect(target);
}

void PinkEquipSkill172::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// 3）项链绝杀技能：天神庇佑，主动使用后，在%X秒内，可以免疫所有负面效果：眩晕、沉默、变身、减速等

	if (NULL == chara)
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType() || Obj::OBJ_TYPE_ROLE != target->GetObjType())
	{
		return;
	}
	
	SendAttackMsg(chara, chara, true);

	int dur_ms = GetParamA(1) * 1000;

	EffectSpecialState *effect_buff = new EffectSpecialState(chara->GetId(), m_skill_id, dur_ms, EBT_SUPER_MIANYI, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
	effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
	chara->AddEffect(effect_buff);

	chara->CheckQTEEffect(target);
}
