#include "sceneeffect.hpp"
#include "sceneskillbase.hpp"

#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "obj/otherobj/effectobj.h"
#include "skill/skillbase.hpp"
#include "ai/monsterai/monsteraibase.hpp"
#include "scene/scene.h"

#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "gameworld/gameworld/scene/speciallogic/speciallogic.hpp"

#include "config/logicconfigmanager.hpp"

#include "gamecommon.h"
#include "engineadapter.h"

SceneEffectSpecialFaZhen::SceneEffectSpecialFaZhen(Scene *scene, ObjID deliver_objid, UInt16 product_id, char product_method) : 
	SceneEffectBase(scene, deliver_objid, product_id, product_method), m_pos(0, 0), m_range(0), m_interval(0),
	m_left_count(0), m_past_count(0), m_liveon_continue_skill(false), m_effect_target(0), m_effect_count(0),
	m_first(true), m_effect_time(0), m_effect_now(false), m_effect_obj_id(INVALID_OBJ_ID), m_side(0)
{

}

SceneEffectSpecialFaZhen::~SceneEffectSpecialFaZhen()
{

}

void SceneEffectSpecialFaZhen::SetFaZhenInfo(const Posi &pos, int range, int interval, int count, bool liveon_continue_skill, bool effect_now, int side)
{
	m_pos = pos;
	m_range = range;
	m_interval = interval;
	m_left_count = count;
	m_liveon_continue_skill = liveon_continue_skill;
	m_effect_now = effect_now;
	m_side = side;
}

void SceneEffectSpecialFaZhen::SetEffectParam(int effect_target, short effect_count, const SkillAttachEffectParam *effect_param_list)
{
	m_effect_target = effect_target;
	
	if (NULL != effect_param_list)
	{
		for (int i = 0; i < effect_count && m_effect_count < static_cast<short>(sizeof(m_effect_param_list) / sizeof(m_effect_param_list[0])); ++i)
		{
			m_effect_param_list[m_effect_count ++] = effect_param_list[i];
		}
	}
}

bool SceneEffectSpecialFaZhen::Effect(unsigned long interval, time_t now_second)
{
	if (m_first)
	{
		m_effect_obj_id = EffectObj::CreateEffectObject(m_scene, m_pos, m_product_id, m_product_method, (EngineAdapter::Instance().Time() + m_interval * m_left_count / 1000 + 1));

		m_first = false;

		if (m_effect_now)
		{
			++ m_past_count;

			this->Attach();
		}
	}
	else
	{
		if (m_liveon_continue_skill)
		{
			Obj *deliver_obj = m_scene->GetObj(m_deliver_objid);
			if (NULL == deliver_obj || Obj::OBJ_TYPE_MONSTER != deliver_obj->GetObjType()) 
			{
				EffectObj::RemoveEffectObject(m_scene, m_effect_obj_id);
				return false;
			}

			Monster *monster = (Monster *)deliver_obj;
			if (!monster->IsAlive()) 
			{
				EffectObj::RemoveEffectObject(m_scene, m_effect_obj_id);
				return false;
			}
			
			if (NULL == monster->GetMonsterAI() || !monster->GetMonsterAI()->IsContinuePerforming()) 
			{
				EffectObj::RemoveEffectObject(m_scene, m_effect_obj_id);
				return false;
			}
		}
	}

	m_effect_time += (int)interval;
	if (m_effect_time < m_interval)
	{
		return true;
	}

	++ m_past_count;
	this->Attach();

	m_effect_time = 0;
	--m_left_count;
	if (m_left_count > 0) return true;

	m_scene->DeleteObj(m_effect_obj_id);

	return false;
}

void SceneEffectSpecialFaZhen::Attach()
{
	{
		// 播特效
		Obj *fazhen_obj = m_scene->GetObj(m_effect_obj_id);
		if (NULL != fazhen_obj)
		{
			SkillBase::SendAOEAttackMsgHelper(m_scene, m_deliver_objid, INVALID_OBJ_ID,fazhen_obj->GetObserHandle(), m_product_id, m_pos, SkillBase::AOE_REASON_FAZHEN);
		}
	}

	Character *chara = NULL;

	Obj *deliver_obj = m_scene->GetObj(m_deliver_objid);
	if (NULL != deliver_obj && Obj::IsCharactor(deliver_obj->GetObjType())) 
	{
		chara = (Character *)deliver_obj;
	}

	static ObjID target_objid_list[32] = {INVALID_OBJ_ID};
	int obj_count = SkillBase::GetTargetList(m_scene, chara, m_pos, m_range, m_range, target_objid_list, sizeof(target_objid_list) / sizeof(target_objid_list[0]), SkillBase::AOE_RANGE_TYPE_TARGET_CENTERED_QUADRATE, m_effect_target);

	static Character *target_cha_list[32] = {NULL};
	int target_count = 0;
	int max_count = static_cast<int>(sizeof(target_cha_list) / sizeof(target_cha_list[0]));

	for (int i = 0; i < obj_count && target_count < max_count; ++i)
	{
		Character *target = (Character*)m_scene->GetObj(target_objid_list[i]);
		if (NULL == target) continue;

		SpecialLogic* sp = m_scene->GetSpecialLogic();
		//两者默认都是-1, 不设置不生效
		if (sp && (m_side & sp->GetObjSide(target_objid_list[i])))
		{
			target_cha_list[target_count ++] = target;
		}
	}

	for (int i = 0; i < m_effect_count; ++i)
	{
		m_effect_param_list[i].Attach(m_scene, m_deliver_objid, m_pos, target_count, target_cha_list, m_product_id, SKILL_EFFECT_SRC_FAZHEN, m_past_count);
	}
}

//////////////////////////////////////////////////////////////////////////
SceneEffectHurt::SceneEffectHurt(Scene *scene, ObjID deliver_objid, UInt16 product_id, char product_method)
	: SceneEffectBase(scene, deliver_objid, product_id, product_method), m_target_count(0), m_injure(0), m_fight_type(0)
{
	
}

SceneEffectHurt::~SceneEffectHurt()
{

}

void SceneEffectHurt::SetHurtInfo(long long injure, int fight_type)
{
	m_injure = injure;
	m_fight_type = fight_type;
}

void SceneEffectHurt::AddTarget(ObjID obj_id)
{
	if (m_target_count < MAX_TARGET_COUNT)
	{
		Obj *target_obj = m_scene->GetObj(obj_id);
		if (NULL != target_obj)
		{
			HurtTarget hurt_target;
			hurt_target.obj_id = obj_id;

			if (Obj::OBJ_TYPE_ROLE == target_obj->GetObjType())
			{
				Role *target_role = (Role *)target_obj;
				hurt_target.user_id = target_role->GetUserId();
			}

			m_target_list[m_target_count ++] = hurt_target;
		}
	}
}

bool SceneEffectHurt::Effect(unsigned long interval, time_t now_second)
{
	for (int i = 0; i < m_target_count && i < MAX_TARGET_COUNT; ++i)
	{
		if (m_target_list[i].obj_id == m_deliver_objid) continue;

		Obj *target_obj = m_scene->GetObj(m_target_list[i].obj_id);
		if (NULL != target_obj && 
			(Obj::OBJ_TYPE_ROLE == target_obj->GetObjType() || Obj::OBJ_TYPE_MONSTER == target_obj->GetObjType()))
		{
			if (Obj::OBJ_TYPE_ROLE == target_obj->GetObjType())
			{
				Role *target_role = (Role *)target_obj;
				if (target_role->GetUserId() != m_target_list[i].user_id)
				{
					continue;
				}
			}

			Character *target = (Character*)target_obj;

			EffectOnce *effect_first = new EffectOnce(m_deliver_objid, 0, -m_injure, EffectBase::FIGHT_TYPE_NORMAL);
			target->AddEffect(effect_first);
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
SceneEffectTowerDefendFaZhen::SceneEffectTowerDefendFaZhen(Scene *scene, ObjID deliver_objid, UInt16 product_id, char product_method)
	: SceneEffectBase(scene, deliver_objid, product_id, product_method), m_pos(0, 0), m_range(0), m_interval(0),
	m_left_count(0), m_hurt(0), m_first(true), m_effect_time(0), m_effect_obj_id(INVALID_OBJ_ID) 
{

}

SceneEffectTowerDefendFaZhen::~SceneEffectTowerDefendFaZhen()
{

}

void SceneEffectTowerDefendFaZhen::SetFaZhenInfo(const Posi &pos, int range, int interval, int count, int hurt)
{
	m_pos = pos;
	m_range = range;
	m_interval = interval;
	m_left_count = count;
	m_hurt = hurt;
}

bool SceneEffectTowerDefendFaZhen::Effect(unsigned long interval, time_t now_second)
{
	if (m_first)
	{
		m_effect_obj_id = EffectObj::CreateEffectObject(m_scene, m_pos, m_product_id, m_product_method, (EngineAdapter::Instance().Time() + m_interval * m_left_count / 1000 + 1));

		m_first = false;
	}

	m_effect_time += (int)interval;
	if (m_effect_time < m_interval)
	{
		return true;
	}

	this->Attach();

	m_effect_time = 0;

	--m_left_count;
	if (m_left_count > 0) return true;

	m_scene->DeleteObj(m_effect_obj_id);

	return false;
}

void SceneEffectTowerDefendFaZhen::Attach()
{
	Role *deliver_role = NULL;

	Obj *deliver_obj = m_scene->GetObj(m_deliver_objid);
	if (NULL != deliver_obj && Obj::OBJ_TYPE_ROLE == deliver_obj->GetObjType()) 
	{
		deliver_role = (Role *)deliver_obj;
	}

	static ObjID target_objid_list[32] = {INVALID_OBJ_ID};
	int obj_count = SkillBase::GetTargetList(m_scene, deliver_role, m_pos, m_range, m_range, target_objid_list, sizeof(target_objid_list) / sizeof(target_objid_list[0]), SkillBase::AOE_RANGE_TYPE_TARGET_CENTERED_QUADRATE, SKILL_ATTACH_EFFECT_TARGET_ALL_MONSTER);

	static Monster *monster_list[32] = {NULL};
	int target_count = 0;
	int max_count = static_cast<int>(sizeof(monster_list) / sizeof(monster_list[0]));

	for (int i = 0; i < obj_count && target_count < max_count; ++i)
	{
		Character *target = (Character*)m_scene->GetObj(target_objid_list[i]);
		if (NULL != target && Obj::OBJ_TYPE_MONSTER == target->GetObjType() && target->IsAlive())
		{
			Monster *target_monster = (Monster *)target;
			monster_list[target_count ++] = target_monster;
		}		
	}

	for (int i = 0; i < target_count; ++i)
	{
		if (NULL != monster_list[i])
		{
			EffectOnce *effect = new EffectOnce(m_deliver_objid, m_product_id, -m_hurt, EffectBase::FIGHT_TYPE_NORMAL);
			monster_list[i]->AddEffect(effect);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
SceneEffectFaZhen121::SceneEffectFaZhen121(Scene *scene, ObjID deliver_objid, UInt16 product_id, char product_method)
	: SceneEffectBase(scene, deliver_objid, product_id, product_method), 
	m_pos(0, 0), m_range(0), m_hurt(0), m_dec_movespeed_percent(0), m_dec_movespeed_dur_ms(0), m_xuanyun_dur_ms(0), m_chenmo_dur_ms(0),
	m_first(true), m_effect_time(0), m_left_time(0), m_effect_obj_id(INVALID_OBJ_ID) 
{

}

SceneEffectFaZhen121::~SceneEffectFaZhen121()
{

}

void SceneEffectFaZhen121::SetFaZhenInfo(const Posi &pos, int range, int left_time, int hurt, int dec_movespeed_percent, int dec_movespeed_dur_ms, int xuanyun_dur_ms, int chenmo_dur_ms)
{
	m_pos = pos;
	m_range = range;
	m_left_time = left_time;
	m_hurt = hurt;
	m_dec_movespeed_percent = dec_movespeed_percent;
	m_dec_movespeed_dur_ms = dec_movespeed_dur_ms;
	m_xuanyun_dur_ms = xuanyun_dur_ms;
	m_chenmo_dur_ms = chenmo_dur_ms;
}

bool SceneEffectFaZhen121::Effect(unsigned long interval, time_t now_second)
{
	if (m_first)
	{
		m_effect_obj_id = EffectObj::CreateEffectObject(m_scene, m_pos, m_product_id, m_product_method, (EngineAdapter::Instance().Time() + m_left_time / 1000 + 1));

		m_first = false;
	}

	m_effect_time += (int)interval;
	if (m_effect_time >= 1000)
	{
		m_effect_time = 0;

		this->Attach();
	}

	m_left_time -= (int)interval;
	if (m_left_time > 0)
	{
		return true;
	}

	{
		// 结束时，会使法阵内的敌人昏迷E秒
		if (m_xuanyun_dur_ms > 0)
		{
			Character *deliver_cha = NULL;
			Obj *deliver_obj = m_scene->GetObj(m_deliver_objid);
			if (NULL != deliver_obj && Character::IsCharactor(deliver_obj->GetObjType()))
			{
				deliver_cha = (Character *)deliver_obj;
			}

			static ObjID target_objid_list[32] = {INVALID_OBJ_ID};
			int obj_count = SkillBase::GetTargetList(m_scene, deliver_cha, m_pos, m_range, m_range, target_objid_list, sizeof(target_objid_list) / sizeof(target_objid_list[0]), SkillBase::AOE_RANGE_TYPE_TARGET_CENTERED_QUADRATE, SKILL_ATTACH_EFFECT_TARGET_ENEMY);

			for (int i = 0; i < obj_count; ++i)
			{
				Character *target = (Character*)m_scene->GetObj(target_objid_list[i]);
				if (NULL == target) continue;

				EffectSpecialState *effect_yun = new EffectSpecialState(m_deliver_objid, m_product_id, m_xuanyun_dur_ms, EBT_XUANYUN, 1);
				effect_yun->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
				target->AddEffect(effect_yun);
			}
		}
	}

	m_scene->DeleteObj(m_effect_obj_id);

	return false;
}

void SceneEffectFaZhen121::Attach()
{
	{
		// 播特效
		Obj *fazhen_obj = m_scene->GetObj(m_effect_obj_id);
		if (NULL != fazhen_obj)
		{
			SkillBase::SendAOEAttackMsgHelper(m_scene, m_deliver_objid, INVALID_OBJ_ID, fazhen_obj->GetObserHandle(), m_product_id, m_pos, SkillBase::AOE_REASON_FAZHEN);
		}
	}

	Character *deliver_cha = NULL;
	Obj *deliver_obj = m_scene->GetObj(m_deliver_objid);
	if (NULL != deliver_obj && Character::IsCharactor(deliver_obj->GetObjType()))
	{
		deliver_cha = (Character *)deliver_obj;
	}

	static ObjID target_objid_list[32] = {INVALID_OBJ_ID};
	int obj_count = SkillBase::GetTargetList(m_scene, deliver_cha, m_pos, m_range, m_range, target_objid_list, sizeof(target_objid_list) / sizeof(target_objid_list[0]), SkillBase::AOE_RANGE_TYPE_TARGET_CENTERED_QUADRATE, SKILL_ATTACH_EFFECT_TARGET_ENEMY);

	for (int i = 0; i < obj_count; ++i)
	{
		Character *target = (Character*)m_scene->GetObj(target_objid_list[i]);
		if (NULL == target) continue;

		// 对敌人造成基础攻击B%附加C点伤害 
		int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
		EffectOnce *effect = new EffectOnce(m_deliver_objid, m_product_id, -m_hurt, fight_type, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		target->AddEffect(effect);

		// 法阵内的敌人会被封技
		if (m_chenmo_dur_ms > 0)
		{
			EffectSpecialState *special_effect = new EffectSpecialState(m_deliver_objid, m_product_id, m_chenmo_dur_ms, EBT_CHENMO, 1, EffectBase::PRODUCT_METHOD_FAZHEN_HALO);
			special_effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
			special_effect->SetSave(false);
			target->AddEffect(special_effect);
		}

		// 法阵内的敌人会被减速D%
		if (m_dec_movespeed_percent > 0 && m_dec_movespeed_dur_ms > 0)
		{
			int dec_movespeed = static_cast<int>(target->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) * m_dec_movespeed_percent / 100);

			EffectMoveSpeed *effect_speed = new EffectMoveSpeed(m_deliver_objid, m_product_id, m_dec_movespeed_dur_ms, EBT_CHIHUAN, 1, EffectBase::PRODUCT_METHOD_FAZHEN_HALO);
			effect_speed->SetMoveSpeed(-dec_movespeed, -m_dec_movespeed_percent);
			effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
			effect_speed->SetSave(false);
			target->AddEffect(effect_speed);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
SceneEffectFaZhen421::SceneEffectFaZhen421(Scene *scene, ObjID deliver_objid, UInt16 product_id, char product_method)
	: SceneEffectBase(scene, deliver_objid, product_id, product_method), 
	m_pos(0, 0), m_range(0), m_hurt(0), m_dec_movespeed_percent(0), m_dec_movespeed_dur_ms(0), 
	m_first(true), m_effect_time(0), m_left_time(0), m_rand_pos_count(0), m_rand_range(0), m_effect_obj_id(INVALID_OBJ_ID) 
{

}

SceneEffectFaZhen421::~SceneEffectFaZhen421()
{

}

void SceneEffectFaZhen421::SetFaZhenInfo(const Posi &pos, int range, int left_time, int hurt, int dec_movespeed_percent, int dec_movespeed_dur_ms, int rand_pos_count, int rand_range)
{
	m_pos = pos;
	m_range = range;
	m_left_time = left_time;
	m_hurt = hurt;
	m_dec_movespeed_percent = dec_movespeed_percent;
	m_dec_movespeed_dur_ms = dec_movespeed_dur_ms;
	m_rand_pos_count = rand_pos_count;
	m_rand_range = rand_range;
}

bool SceneEffectFaZhen421::Effect(unsigned long interval, time_t now_second)
{
	bool tmp_first = false;

	if (m_first)
	{
		m_effect_obj_id = EffectObj::CreateEffectObject(m_scene, m_pos, m_product_id, m_product_method, (EngineAdapter::Instance().Time() + m_left_time / 1000 + 1));

		tmp_first = true;
		m_first = false;
	}

	m_effect_time += (int)interval;
	if (m_effect_time >= 1000)
	{
		m_effect_time = 0;

		this->Attach(tmp_first);
	}

	m_left_time -= (int)interval;
	if (m_left_time > 0)
	{
		return true;
	}

	m_scene->DeleteObj(m_effect_obj_id);

	return false;
}

void SceneEffectFaZhen421::Attach(bool is_first)
{
	Character *deliver_cha = NULL;
	Obj *deliver_obj = m_scene->GetObj(m_deliver_objid);
	if (NULL != deliver_obj && Character::IsCharactor(deliver_obj->GetObjType()))
	{
		deliver_cha = (Character *)deliver_obj;
	}

	if (NULL == deliver_cha) return;

	for (int i = 0; i < m_rand_pos_count; ++i)
	{
		Posi zone_pos;
		
		if (is_first && 0 == i)
		{
			zone_pos = m_pos;
		}
		else
		{
			zone_pos = gamecommon::GetDisRandPos(m_pos, m_range);
		}

		{
			// 播特效
			Obj *fazhen_obj = m_scene->GetObj(m_effect_obj_id);
			if (NULL != fazhen_obj)
			{
				SkillBase::SendAOEAttackMsgHelper(m_scene, m_deliver_objid, INVALID_OBJ_ID, fazhen_obj->GetObserHandle(), m_product_id, zone_pos, SkillBase::AOE_REASON_FAZHEN);
			}
		}

		static ObjID target_objid_list[32] = {INVALID_OBJ_ID};
		int obj_count = SkillBase::GetTargetList(m_scene, deliver_cha, zone_pos, m_rand_range, m_rand_range, target_objid_list, sizeof(target_objid_list) / sizeof(target_objid_list[0]), SkillBase::AOE_RANGE_TYPE_TARGET_CENTERED_QUADRATE, SKILL_ATTACH_EFFECT_TARGET_ENEMY);

		for (int i = 0; i < obj_count; ++i)
		{
			Character *target = (Character*)m_scene->GetObj(target_objid_list[i]);
			if (NULL == target) continue;

			// 造成攻击%D+E点固伤
			int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
			EffectOnce *effect = new EffectOnce(m_deliver_objid, m_product_id, -m_hurt, fight_type);
			target->AddEffect(effect);

			// 并使敌人速度减少C%
			if (m_dec_movespeed_percent > 0 && m_dec_movespeed_dur_ms > 0)
			{
				int dec_movespeed = static_cast<int>(target->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) * m_dec_movespeed_percent / 100);

				EffectMoveSpeed *effect_speed = new EffectMoveSpeed(m_deliver_objid, m_product_id, m_dec_movespeed_dur_ms, EBT_CHIHUAN, 1, EffectBase::PRODUCT_METHOD_FAZHEN_HALO);
				effect_speed->SetMoveSpeed(-dec_movespeed, -m_dec_movespeed_percent);
				effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
				effect_speed->SetSave(false);
				target->AddEffect(effect_speed);
			}
		}
	}		
}

SceneEffectClient::SceneEffectClient(Scene *scene, ObjID deliver_objid, UInt16 product_id, char product_method) : 
SceneEffectBase(scene, deliver_objid, product_id, product_method), m_first(true), m_pos(0, 0), m_effect_obj_id(INVALID_OBJ_ID), m_end_time(0), m_param1(0), m_param2(0)
{

}

SceneEffectClient::~SceneEffectClient()
{

}

void SceneEffectClient::SetEffectParam(const Posi &pos, const Posi &src_pos, unsigned int end_time, int param1, int param2)
{
	m_pos = pos;
	m_src_pos = src_pos;
	m_end_time = end_time;
	
	m_param1 = param1;
	m_param2 = param2;
}

bool SceneEffectClient::Effect(unsigned long interval, time_t now_second)
{
	if (m_first)
	{
		m_effect_obj_id = EffectObj::CreateEffectObject(m_scene, m_pos, m_product_id, m_product_method, m_end_time, m_param1, m_param2, m_src_pos);

		m_first = false;
	}
	else
	{
		if (INVALID_OBJ_ID != m_deliver_objid)  // 有释放者的情况下，释放者消失，认为该特效也消失
		{
			Obj *deliver_obj = m_scene->GetObj(m_deliver_objid);
			if (NULL == deliver_obj || (Obj::OBJ_TYPE_MONSTER != deliver_obj->GetObjType() && Obj::OBJ_TYPE_ROLE != deliver_obj->GetObjType())) 
			{
				EffectObj::RemoveEffectObject(m_scene, m_effect_obj_id);
				return false;
			}

			Character *cha = (Character *)deliver_obj;
			if (!cha->IsAlive()) 
			{
				EffectObj::RemoveEffectObject(m_scene, m_effect_obj_id);
				return false;
			}
		}
	}

	if (now_second < m_end_time) return true;

	m_scene->DeleteObj(m_effect_obj_id);

	return false;
}

void SceneEffectClient::DelEffectObj()
{
	EffectObj::RemoveEffectObject(m_scene, m_effect_obj_id);
}