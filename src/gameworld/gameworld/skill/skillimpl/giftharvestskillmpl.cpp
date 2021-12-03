#include "giftharvestskillmpl.hpp"
#include "skill/skilldef.hpp"
#include "obj/character/character.h"
#include "obj/character/monster.h"
#include "obj/character/role.h"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "scene/map.h"
#include "scene/scene.h"
#include "servercommon/errornum.h"
#include "protocal/msgmultimount.h"
#include <cmath>
#include "effect/skilleffect/effectmovespeed.hpp"
#include "ai/monsterai/monsteraibase.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GiftHarvestSkill701::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// 击退

	if (NULL == chara || NULL == target)
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType() || Obj::OBJ_TYPE_MONSTER != target->GetObjType())
	{
		return;
	}

	Monster *target_monster = (Monster *)target;
	if (NULL == target_monster)
	{
		return;
	}

	SendAttackMsg(chara, target, true);

	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
	Attribute injure = CalcInjureAndType(chara, target, level, fight_type);

	// 施法者(x1,y1)，被打者(x2,y2)，目标点(x3,y3)，param_a为被击退的距离a，施法者到目的地距离为b
	// 根据三角关系，可得出：(x2-x3)/(x1-x3) = (y2-y3)/(y1-y3) = a/b （假设 a/b = k）
	// 解方程后得出目的坐标：x3 = (x1*k-x2)/(k-1) ,  y3 = (y1*k-y2)/(k-1)
	double x1 = chara->GetPos().x;
	double y1 = chara->GetPos().y;
	double x2 = target->GetPos().x;
	double y2 = target->GetPos().y;

	int param_a = GetParamA(level);
	if (param_a == 0)
	{
		return;
	}

	double k = GetParamA(level) * 1.0 / (GetParamA(level) + 1.0 * std::sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2)));
	if (k == 1)
	{
		return;
	}

	double x3 = (x1*k-x2)/(k-1);
	double y3 = (y1*k-y2)/(k-1);

	Posi target_pos;
	target_pos.x = (int)x3;
	target_pos.y = (int)y3;

	target_monster->SkillResetPos(target_pos, Character::SKILL_RESET_POS_TYPE_CAPTURE, m_skill_id, true);

	this->HurtTrigger(chara, target, injure, fight_type);

	chara->CheckQTEEffect(target);
}

void GiftHarvestSkill702::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// 眩晕

	if (NULL == chara || NULL == target)
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType() || Obj::OBJ_TYPE_MONSTER != target->GetObjType())
	{
		return;
	}

	Role *role = (Role *)chara;
	if (NULL == role)
	{
		return;
	}

	Scene *scene = target->GetScene();
	if (NULL != scene)
	{
		Map *map = scene->GetMap();
		if (NULL != map)
		{
			if (map->SaveArea(target->GetPos().x, target->GetPos().y))
			{
				role->NoticeNum(errornum::EN_SAVEAREA_FIGHT);
				return;
			}
		}
	}

	SendAttackMsg(chara, target, true);

	EffectSpecialState *effect_yun = new EffectSpecialState(chara->GetId(), m_skill_id, GetParamA(level) * 1000, EBT_XUANYUN, level);
	effect_yun->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
	target->AddEffect(effect_yun);

	chara->CheckQTEEffect(target);
}

void GiftHarvestSkill703::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// 嘲讽

	if (NULL == chara || NULL == target)
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType() || Obj::OBJ_TYPE_MONSTER != target->GetObjType())
	{
		return;
	}

	Monster *target_monster = (Monster *)target;
	if (NULL == target_monster)
	{
		return;
	}

	SendAttackMsg(chara, target, true);

	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
	Attribute injure = CalcInjureAndType(chara, target, level, fight_type);

	target_monster->GetMonsterAI()->ForceSetEnemy(chara->GetId());

	this->HurtTrigger(chara, target, injure, fight_type);

	chara->CheckQTEEffect(target);
}

