#include "magiccardskillmpl.hpp"
#include "skill/skilldef.hpp"
#include "obj/character/character.h"
#include "obj/character/monster.h"
#include "obj/character/role.h"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "scene/map.h"
#include "scene/scene.h"
#include "servercommon/errornum.h"
#include "protocal/msgmultimount.h"
#include "other/mount/mountmanager.hpp"
#include "other/multimount/multimount.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MagicCardSkill200::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// 把玩家打落马

	if (NULL == chara || NULL == target)
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType() || Obj::OBJ_TYPE_ROLE != target->GetObjType())
	{
		return;
	}

	Role *target_role = (Role *)target;
	if (NULL == target_role)
	{
		return;
	}

	SendAttackMsg(chara, target, true);

	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
	// 是否受技伤影响
	bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
	long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);

	target_role->GetMountManager()->GoonMount(false);
	target_role->GetMultiMount()->MountOn(false);

	if (chara->GetId() != target_role->GetId())
	{
		target_role->OnStartFight();
	}

	this->HurtTrigger(chara, target, injure, fight_type);

	chara->CheckQTEEffect(target);
}

void MagicCardSkill201::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// 隐形

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

	EffectSpecialState *effect_yun = new EffectSpecialState(role->GetId(), m_skill_id, GetParamA(1) * 1000, EBT_INVISIBLE, 1);
	effect_yun->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
	role->AddEffect(effect_yun);

	chara->CheckQTEEffect(target);
}

void MagicCardSkill202::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// 拉玩家

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

	Scene *scene = target_role->GetScene();
	if (NULL != scene)
	{
		Map *map = scene->GetMap();
		if (NULL != map)
		{
			if (map->SaveArea(target_role->GetPos().x, target_role->GetPos().y))
			{
				role->NoticeNum(errornum::EN_SAVEAREA_FIGHT);
				return;
			}
		}
	}

	SendAttackMsg(chara, target, true);

	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
	// 是否受技伤影响
	bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
	long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);

	target_role->SkillResetPos(chara->GetPos(), Character::SKILL_RESET_POS_TYPE_CAPTURE, m_skill_id, true);

	this->HurtTrigger(chara, target, injure, fight_type);

	chara->CheckQTEEffect(target);
}

void MagicCardSkill203::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// 把玩家从空中打下来

	if (NULL == chara || NULL == target)
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType() || Obj::OBJ_TYPE_ROLE != target->GetObjType())
	{
		return;
	}

	Role *target_role = (Role *)target;
	if (NULL == target_role)
	{
		return;
	}

	SendAttackMsg(chara, target, true);

	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
	// 是否受技伤影响
	bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
	long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);

	target_role->SkillResetPos(chara->GetPos(), Character::SKILL_RESET_POS_TYPE_CAPTURE, m_skill_id, true);

	if (Role::MOVE_MODE_FLY == target_role->GetMoveMode())
	{
		target_role->SetMoveMode(Role::MOVE_MODE_NORMAL, Posi(0, 0), 0);
	}
	target_role->GetMountManager()->GoonMount(false);
	target_role->GetMultiMount()->MountOn(false);

	if (chara->GetId() != target->GetId())
	{
		target_role->OnStartFight();
	}
	
	this->HurtTrigger(chara, target, injure, fight_type);

	chara->CheckQTEEffect(target);
}
