#include "monsterhalo.hpp"
#include "engineadapter.h"
#include "scene/scene.h"
#include "obj/character/monster.h"
#include "skill/skillbase.hpp"

MonsterHalo::MonsterHalo(ObjID owner_obj_id, int owner_key, UInt16 product_id, char product_method)
	: Character(OBJ_TYPE_MONSTER_HALO), m_product_id(product_id), m_product_method(product_method),
	m_range(0), m_interval(0), m_left_count(0), m_past_count(0), m_effect_target(0), m_first(true), m_effect_time(0),
	m_owner_obj_id(owner_obj_id), m_owner_key(owner_key)
{

}

MonsterHalo::~MonsterHalo()
{

}

void MonsterHalo::Init(Attribute hp)
{
	CharIntAttrs int_attribute;
	int_attribute.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP] = hp;
	int_attribute.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP] = hp;

	SkillListParam skill_param; skill_param.count = 0;

	Character::Init(int_attribute, skill_param); 
}

void MonsterHalo::Update(unsigned long interval, time_t now_second, unsigned int now_dayid)
{
	Monster *owner_monster = this->GetOwnerMonster();
	if (NULL == owner_monster || !owner_monster->IsAlive()) 
	{
		m_scene->DeleteObj(m_obj_id);

		return;
	}

	if ((this->GetPos() - owner_monster->GetPos()).IsLongerThan(1))
	{
		this->Move(owner_monster->GetPos().x, owner_monster->GetPos().y, Dir_S, 0, 0, false);
	}

	if (m_first)
	{
		m_first = false;

		++ m_past_count;
		this->Attach(owner_monster->GetPos());
	}
	
	m_effect_time += (int)interval;
	if (m_effect_time < m_interval)
	{
		return;
	}

	++ m_past_count;

	this->Attach(owner_monster->GetPos());

	m_effect_time = 0;
	--m_left_count;
	
	if (m_left_count <= 0)
	{
		m_scene->DeleteObj(m_obj_id);
	}

	return;
}

void MonsterHalo::SetHaloInfo(int range, int interval, int count)
{
	m_range = range;
	m_interval = interval;
	m_left_count = count;
}

void MonsterHalo::SetEffectParam(int effect_target, const SkillAttachEffectParam &effect_param)
{
	m_effect_target = effect_target;
	m_effect_param = effect_param;
}

void MonsterHalo::Attach(const Posi &pos)
{
	Monster *owner_monster = this->GetOwnerMonster();

	static ObjID target_objid_list[32] = {INVALID_OBJ_ID};
	int obj_count = SkillBase::GetTargetList(this->GetScene(), owner_monster, pos, m_range, m_range, target_objid_list, sizeof(target_objid_list) / sizeof(target_objid_list[0]), SkillBase::AOE_RANGE_TYPE_TARGET_CENTERED_QUADRATE, m_effect_target);

	static Character *target_cha_list[32] = {NULL};
	int target_count = 0;
	int max_count = static_cast<int>(sizeof(target_cha_list) / sizeof(target_cha_list[0]));

	for (int i = 0; i < obj_count && target_count < max_count; ++i)
	{
		Character *target = (Character*)m_scene->GetObj(target_objid_list[i]);
		if (NULL == target) continue;

		target_cha_list[target_count ++] = target;
	}

	if (target_count > 0)
	{
		m_effect_param.Attach(m_scene, m_owner_obj_id, pos, target_count, target_cha_list, m_product_id, SKILL_EFFECT_SRC_HALO, m_past_count);
	}
}

Monster * MonsterHalo::GetOwnerMonster()
{
	Obj *owner_obj = m_scene->GetObj(m_owner_obj_id);
	if (NULL != owner_obj && Obj::OBJ_TYPE_MONSTER == owner_obj->GetObjType())
	{
		Monster *monster = (Monster *)owner_obj;
		if (monster->GetMonsterKey() == m_owner_key)
		{
			return monster;
		}
	}

	return NULL;
}
