#include "petskillmpl.hpp"
#include "skill/skilldef.hpp"
#include "obj/character/character.h"
#include "obj/character/monster.h"
#include "obj/character/role.h"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "scene/map.h"
#include "scene/scene.h"
#include "servercommon/errornum.h"
#include "effect/skilleffect/effectwudiprotect.hpp"
#include "effect/effectbase.hpp"
#include "obj/character/attribute.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectreboundhurt.hpp"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "protocal/msgfight.h"
#include "skill/skillbase.hpp"

void PetSkill146::ReCalcAttr(Character *chara, int skill_level, CharIntAttrs &base_add) const
{
	//������Ѫ
	if (NULL != chara)
	{
		int add_value = (GetParamA(skill_level) * chara->GetLevel()) / PRCENT_OF_PET_SKILL;
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += add_value;
	}
}

void PetSkill147::ReCalcAttr(Character *chara, int skill_level, CharIntAttrs &base_add) const
{
	//���ӹ���
	if (NULL != chara)
	{
		int add_value = (GetParamA(skill_level) * chara->GetLevel()) / PRCENT_OF_PET_SKILL;
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += add_value;
	}	
}

void PetSkill148::ReCalcAttr(Character *chara, int skill_level, CharIntAttrs &base_add) const
{
	//���ӷ���
	if (NULL != chara)
	{
		int add_value = (GetParamA(skill_level) * chara->GetLevel()) / PRCENT_OF_PET_SKILL;
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += add_value;
	}	
}

void PetSkill149::ReCalcAttr(Character *chara, int skill_level, CharIntAttrs &base_add) const
{
	//��������
	if (NULL != chara)
	{
		int add_value = (GetParamA(skill_level) * chara->GetLevel()) / PRCENT_OF_PET_SKILL;
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += add_value;
	}	
}


void PetSkill150::ReCalcAttr(Character *chara, int skill_level, CharIntAttrs &base_add) const
{
	//��������
	if (NULL != chara)
	{
		int add_value = (GetParamA(skill_level) * chara->GetLevel()) / PRCENT_OF_PET_SKILL;
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += add_value;
	}
}

