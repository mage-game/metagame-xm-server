#include "crosspastureskillmpl.hpp"
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
void CrossPastureSkill250::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// ����

	if (NULL == chara || NULL == target)
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE != chara->GetObjType())
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
	// �Ƿ��ܼ���Ӱ��
	bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
	long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);

	// ʩ����(x1,y1)��������(x2,y2)��Ŀ���(x3,y3)��param_aΪ�����˵ľ���a��ʩ���ߵ�Ŀ�ĵؾ���Ϊb
	// �������ǹ�ϵ���ɵó���(x2-x3)/(x1-x3) = (y2-y3)/(y1-y3) = a/b ������ a/b = k��
	// �ⷽ�̺�ó�Ŀ�����꣺x3 = (x1*k-x2)/(k-1) ,  y3 = (y1*k-y2)/(k-1)
	double x1 = chara->GetPos().x;
	double y1 = chara->GetPos().y;
	double x2 = target->GetPos().x;
	double y2 = target->GetPos().y;

	Posi target_pos;
	if (x1 == x2 && y1 == y2)
	{
		target_pos = gamecommon::GetDisRandPos(target->GetPos(), GetParamA(level));
	}
	else
	{
		if (GetParamA(level) == 0)
		{
			return;
		}
		double k = GetParamA(level) * 1.0 / (GetParamA(level) + 1.0 * std::sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)));
		if (k == 1)
		{
			return;
		}
		double x3 = (x1*k - x2) / (k - 1);
		double y3 = (y1*k - y2) / (k - 1);

		target_pos.x = (int)x3;
		target_pos.y = (int)y3;
	}

	// �������յ������ﲻ���������������ʧ�ܣ���ֹ���ﱻ���˵�3D��ͼ������
	if (chara->GetScene()->GetMap()->IsValidatePos(Obj::OBJ_TYPE_ROLE, target_pos.x, target_pos.y))
	{
		target_role->SkillResetPos(target_pos, Character::SKILL_RESET_POS_TYPE_CAPTURE, m_skill_id, true);
	}

	this->HurtTrigger(chara, target, injure, fight_type);

	chara->CheckQTEEffect(target);
}

void CrossPastureSkill251::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// ��ʯͷ��ѣ�Σ�

	if (NULL == chara || NULL == target)
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

void CrossPastureSkill252::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// ����

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

	SendAttackMsg(chara, chara, true);

	int SPEED_UP_TIME = 5000; // ����ʱ��̶�5000���룬����������

	int chg_movespeed = static_cast<int>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) * GetParamA(level) / 100);
	EffectMoveSpeed *effect_speed = new EffectMoveSpeed(chara->GetId(), m_skill_id, SPEED_UP_TIME, EBT_JIASU, 1);
	effect_speed->SetMoveSpeed(chg_movespeed, GetParamA(level));
	effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
	role->AddEffect(effect_speed);
}

void CrossPastureSkill253::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	// �����

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
	// �Ƿ��ܼ���Ӱ��
	bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);

	long long injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);

	target_monster->GetMonsterAI()->ForceSetEnemy(chara->GetId());

	this->HurtTrigger(chara, target, injure, fight_type);

	chara->CheckQTEEffect(target);
}
