#include "trigger.h"

#include "gamecommon.h"

#include "scene/scene.h"
#include "scene/speciallogic/speciallogic.hpp"

#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "obj/otherobj/effectobj.h"

#include "monster/monsterpool.h"

#include "protocal/msgscene.h"

#include "config/scenemanagerconfig.h"

#include "effect/effectbase.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectmovespeed.hpp"

#include "engineadapter.h"

Trigger::Trigger()
	: Obj(OBJ_TYPE_TRIGGER), m_trigger_id(0), m_range(0), m_trigger_cond_type(TRIGGER_COND_TYPE_INVALID), 
	m_trigger_action_type(TRIGGER_ACTION_TYPE_INVALID), m_trigger_max_count(0), m_aoi_handle(-1), m_trigger_count(0), 
	m_is_in_effect(false), m_effect_interval(0), m_effect_time(0), m_is_hidden(true), m_is_notify_triggered(true), m_affiliation(-1)
{
	memset(m_trigger_cond_param_list, 0, sizeof(m_trigger_cond_param_list));
	memset(m_trigger_action_param_list, 0, sizeof(m_trigger_action_param_list));
}

Trigger::~Trigger()
{

}

bool Trigger::CheckTriggerConfig(const ConfigSceneTrigger &trigger_cfg, SceneManagerConfig *scene_manager_config, int mapid, std::string *err)
{
	UNSTD_STATIC_CHECK(TRIGGER_COND_PARAM_MAX >= 1);
	UNSTD_STATIC_CHECK(TRIGGER_ACTION_PARAM_MAX >= 3);
	UNSTD_STATIC_CHECK(13 == TRIGGER_ACTION_TYPE_MAX);

	char errinfo[1024] = {0};

	if (trigger_cfg.trigger_cond_type <= TRIGGER_COND_TYPE_INVALID || trigger_cfg.trigger_cond_type >= TRIGGER_COND_TYPE_MAX)
	{
		SNPRINTF(errinfo, sizeof(errinfo), "trigger_cond_type %d is error", trigger_cfg.trigger_cond_type);
		if (NULL != err) *err = errinfo;
		return false;
	}

	if (!scene_manager_config->CheckMap(mapid, trigger_cfg.x, trigger_cfg.y) ||
		!scene_manager_config->CheckMap(mapid, trigger_cfg.x - trigger_cfg.range, trigger_cfg.y - trigger_cfg.range) ||
		!scene_manager_config->CheckMap(mapid, trigger_cfg.x + trigger_cfg.range, trigger_cfg.y + trigger_cfg.range))
	{
		SNPRINTF(errinfo, sizeof(errinfo), "trigger [x:%d y:%d range:%d] is error.", trigger_cfg.x, trigger_cfg.y, trigger_cfg.range);
		if (NULL != err) *err = errinfo;
		return false;
	}

	int require_cond_param_count = 0;
	switch (trigger_cfg.trigger_cond_type)
	{
	case TRIGGER_COND_TYPE_MONSTER_DIE:
		{
			require_cond_param_count = 1;
			if (0 != trigger_cfg.trigger_cond_param_list[0] && NULL == MONSTERPOOL->GetMonsterParam(trigger_cfg.trigger_cond_param_list[0]))
			{
				SNPRINTF(errinfo, sizeof(errinfo), "trigger_cond_type %d, param[0] should be monster_id", trigger_cfg.trigger_cond_type);
				if (NULL != err) *err = errinfo;
				return false;
			}
		}
		break;
	}

	for (int i = require_cond_param_count; i < TRIGGER_COND_PARAM_MAX; ++ i)
	{
		if (0 != trigger_cfg.trigger_cond_param_list[i])
		{
			SNPRINTF(errinfo, sizeof(errinfo), "trigger_cond_type %d, param[%d] should be 0", trigger_cfg.trigger_cond_type, i);
			if (NULL != err) *err = errinfo;
			return false;
		}
	}
	
	if (trigger_cfg.trigger_action_type <= TRIGGER_ACTION_TYPE_INVALID || trigger_cfg.trigger_action_type >= TRIGGER_ACTION_TYPE_MAX)
	{
		SNPRINTF(errinfo, sizeof(errinfo), "trigger_action_type %d is error", trigger_cfg.trigger_action_type);
		if (NULL != err) *err = errinfo;
		return false;
	}

	int require_action_param_count = 0;
	switch (trigger_cfg.trigger_action_type)
	{
	case TRIGGER_ACTION_TYPE_CREATE_MONSTER:
	case TRIGGER_ACTION_TYPE_SURROUND_MONSTER:
		{
			require_action_param_count = (TRIGGER_ACTION_TYPE_SURROUND_MONSTER == trigger_cfg.trigger_action_type ? 3 : 2);
			if (NULL == MONSTERPOOL->GetMonsterParam(trigger_cfg.trigger_action_param_list[0]))
			{
				SNPRINTF(errinfo, sizeof(errinfo), "trigger_action_type %d, param[0] should be monster_id", trigger_cfg.trigger_action_type);
				if (NULL != err) *err = errinfo;
				return false;
			}
			if (trigger_cfg.trigger_action_param_list[1] <= 0)
			{
				SNPRINTF(errinfo, sizeof(errinfo), "trigger_action_type %d, param[1] should be monster num", trigger_cfg.trigger_action_type);
				if (NULL != err) *err = errinfo;
				return false;
			}
			if (TRIGGER_ACTION_TYPE_SURROUND_MONSTER == trigger_cfg.trigger_action_type)
			{
				if (trigger_cfg.trigger_action_param_list[2] <= 0 || trigger_cfg.trigger_action_param_list[2] >= 32
					|| !scene_manager_config->CheckMap(mapid, trigger_cfg.x - trigger_cfg.trigger_action_param_list[2], trigger_cfg.y - trigger_cfg.trigger_action_param_list[2])
					|| !scene_manager_config->CheckMap(mapid, trigger_cfg.x + trigger_cfg.trigger_action_param_list[2], trigger_cfg.y + trigger_cfg.trigger_action_param_list[2]))
				{
					SNPRINTF(errinfo, sizeof(errinfo), "trigger_action_type %d, param[2] = %d is not valid monster surround range", trigger_cfg.trigger_action_type, trigger_cfg.trigger_action_param_list[2]);
					if (NULL != err) *err = errinfo;
					return false;
				}
			}
		}
		break;

	case TRIGGER_ACTION_TYPE_SET_FB_FOLLOW_NPC:
		{
			require_action_param_count = 1;

			if (trigger_cfg.trigger_action_param_list[0] <= 0)
			{
				SNPRINTF(errinfo, sizeof(errinfo), "trigger_action_type %d, param[0] should be follow_npc_id", trigger_cfg.trigger_action_type);
				if (NULL != err) *err = errinfo;
				return false;
			}
		}
		break;

	case TRIGGER_ACTION_TYPE_SPECIAL_DICI:
		{
			require_action_param_count = 3;

			if (trigger_cfg.trigger_action_param_list[0] <= 0)
			{
				SNPRINTF(errinfo, sizeof(errinfo), "trigger_action_type %d, param[0] should be hurt interval", trigger_cfg.trigger_action_type);
				if (NULL != err) *err = errinfo;
				return false;
			}
			if (trigger_cfg.trigger_action_param_list[1] <= 0)
			{
				SNPRINTF(errinfo, sizeof(errinfo), "trigger_action_type %d, param[1] should be hurt value", trigger_cfg.trigger_action_type);
				if (NULL != err) *err = errinfo;
				return false;
			}
			if (trigger_cfg.trigger_action_param_list[2] <= 0 || trigger_cfg.trigger_action_param_list[2] >= 100)
			{
				SNPRINTF(errinfo, sizeof(errinfo), "trigger_action_type %d, param[2] should be reduce movespeed percent", trigger_cfg.trigger_action_type);
				if (NULL != err) *err = errinfo;
				return false;
			}
		}
		break;

	case TRIGGER_ACTION_TYPE_SPECIAL_BEILAO:
		{
			require_action_param_count = 2;

			if (trigger_cfg.trigger_action_param_list[0] <= 0)
			{
				SNPRINTF(errinfo, sizeof(errinfo), "trigger_action_type %d, param[0] should be hurt interval", trigger_cfg.trigger_action_type);
				if (NULL != err) *err = errinfo;
				return false;
			}
			if (trigger_cfg.trigger_action_param_list[1] <= 0)
			{
				SNPRINTF(errinfo, sizeof(errinfo), "trigger_action_type %d, param[1] should be hurt value", trigger_cfg.trigger_action_type);
				if (NULL != err) *err = errinfo;
				return false;
			}
		}
		break;

	case TRIGGER_ACTION_TYPE_SPECIAL_BANMASUO:
		{
			require_action_param_count = 1;

			if (trigger_cfg.trigger_action_param_list[0] <= 0 || trigger_cfg.trigger_action_param_list[0] > 60 * 1000)
			{
				SNPRINTF(errinfo, sizeof(errinfo), "trigger_action_type %d, param[0] should be xuanyun million_seconds", trigger_cfg.trigger_action_type);
				if (NULL != err) *err = errinfo;
				return false;
			}
		}
		break;

	case TRIGGER_ACTION_TYPE_PLAY_STORY:
		{
			require_action_param_count = 1;

			if (trigger_cfg.trigger_action_param_list[0] <= 0)
			{
				SNPRINTF(errinfo, sizeof(errinfo), "trigger_action_type %d, param[0] should be story id", trigger_cfg.trigger_action_type);
				if (NULL != err) *err = errinfo;
				return false;
			}
		}
		break;

	case TRIGGER_ACTION_TYPE_CREATE_MONSTER_APPOINT_POS:
		{
			require_action_param_count = 3;

			if (NULL == MONSTERPOOL->GetMonsterParam(trigger_cfg.trigger_action_param_list[0]))
			{
				SNPRINTF(errinfo, sizeof(errinfo), "trigger_action_type %d, param[0] should be monster_id", trigger_cfg.trigger_action_type);
				if (NULL != err) *err = errinfo;
				return false;
			}
			if (trigger_cfg.trigger_action_param_list[1] <= 0)
			{
				SNPRINTF(errinfo, sizeof(errinfo), "trigger_action_type %d, param[1] should be pos_x", trigger_cfg.trigger_action_type);
				if (NULL != err) *err = errinfo;
				return false;
			}
			if (trigger_cfg.trigger_action_param_list[2] <= 0)
			{
				SNPRINTF(errinfo, sizeof(errinfo), "trigger_action_type %d, param[2] should be pos_y", trigger_cfg.trigger_action_type);
				if (NULL != err) *err = errinfo;
				return false;
			}
		}
		break;

	case TRIGGER_ACTION_TYPE_ICE_LANDMINE:
		{
			require_action_param_count = 0;
		}
		break;

	case TRIGGER_ACTION_TYPE_FIRE_LANDMINE:
		{
			require_action_param_count = 0;
		}
		break;
	}
	
	for (int i = require_action_param_count; i < TRIGGER_ACTION_PARAM_MAX; ++ i)
	{
		if (0 != trigger_cfg.trigger_action_param_list[i])
		{
			SNPRINTF(errinfo, sizeof(errinfo), "trigger_action_type %d, param[%d] should be 0", trigger_cfg.trigger_action_type, i);
			if (NULL != err) *err = errinfo;
			return false;
		}
	}

	if (trigger_cfg.trigger_max_count < 0)
	{
		SNPRINTF(errinfo, sizeof(errinfo), "trigger_max_count must >= 0");
		if (NULL != err) *err = errinfo;
		return false;
	}

	return true;
}

void Trigger::Init(UInt16 id, int range, int trigger_cond_type, const int trigger_cond_param_list[TRIGGER_COND_PARAM_MAX], int trigger_action_type, const int trigger_action_param_list[TRIGGER_ACTION_PARAM_MAX], int trigger_max_count)
{
	m_trigger_id = id;
	m_range = range;

	m_trigger_cond_type = trigger_cond_type;
	memcpy(m_trigger_cond_param_list, trigger_cond_param_list, sizeof(m_trigger_cond_param_list));

	m_trigger_action_type = trigger_action_type;
	memcpy(m_trigger_action_param_list, trigger_action_param_list, sizeof(m_trigger_action_param_list));

	m_trigger_max_count = trigger_max_count;
}

void Trigger::InitVisible(bool is_hiden, bool notify_triggered)
{
	m_is_hidden = is_hiden;
	m_is_notify_triggered = notify_triggered;
}

void Trigger::OnEnterScene()
{
	Obj::OnEnterScene();

	if (NULL != m_scene && TRIGGER_COND_TYPE_ENTER_AREA == m_trigger_cond_type)
	{
		m_aoi_handle = m_scene->GetZoneMatrix()->CreateRectAOI(m_obj_id, m_posi, Posi(m_range, m_range), Posi(m_range + 2, m_range + 2));
	}
}

void Trigger::OnLeaveScene()
{
	if (NULL != m_scene && TRIGGER_COND_TYPE_ENTER_AREA == m_trigger_cond_type)
	{
		m_scene->GetZoneMatrix()->DestroyAOI(m_aoi_handle);
	}

	Obj::OnLeaveScene();
}

void Trigger::OnAOIEnter(ObjID obj_id)
{
	if (NULL != m_scene && !m_scene->InStaticScene())
	{
		Obj *obj = m_scene->GetObj(obj_id);
		if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
		{
			Role *role = (Role*)obj;

			this->OnTrigger(TRIGGER_COND_TYPE_ENTER_AREA, role);
		}
	}
}

void Trigger::OnAOILeave(ObjID obj_id)
{
	
}

void Trigger::Update(unsigned long interval, time_t now_second, unsigned int now_dayid)
{
	if (m_is_in_effect)
	{
		m_effect_time += interval;
		if (m_effect_time > m_effect_interval)
		{
			if (!this->Effect())
			{
				this->StopEffect();
			}

			m_effect_time = 0;
		}
	}
}

void Trigger::OnTrigger(int trigger_cond_type, Role *role, int trigger_cond_param0, int trigger_cond_param1)
{
	UNSTD_STATIC_CHECK(TRIGGER_COND_PARAM_MAX == 2);

	if ((trigger_cond_type != m_trigger_cond_type) ||
		(0 != m_trigger_cond_param_list[0] && m_trigger_cond_param_list[0] != trigger_cond_param0) ||
		(0 != m_trigger_cond_param_list[1] && m_trigger_cond_param_list[1] != trigger_cond_param1))
	{
		return;
	}

	if (0 == m_trigger_max_count || m_trigger_count < m_trigger_max_count)
	{
		if (TRIGGER_ACTION_TYPE_ICE_LANDMINE == m_trigger_action_type || TRIGGER_ACTION_TYPE_FIRE_LANDMINE == m_trigger_action_type)
		{
		}
		else
		{
			++ m_trigger_count;
		}
		
		if (m_is_notify_triggered)
		{
			static Protocol::SCTriggerNotify tn;
			tn.trigger_id = m_trigger_id;
			tn.trigger_role_objid = (NULL != role ? role->GetId() : INVALID_OBJ_ID);
			if (NULL != m_scene) m_scene->SendToRole((const char *)&tn, sizeof(tn));
		}		

		this->TriggerAction(role);
	}
}

void Trigger::TriggerAction(Role *role)
{
	UNSTD_STATIC_CHECK(TRIGGER_ACTION_PARAM_MAX >= 3);
	UNSTD_STATIC_CHECK(13 == TRIGGER_ACTION_TYPE_MAX);

	switch (m_trigger_action_type)
	{
	case TRIGGER_ACTION_TYPE_CREATE_MONSTER:
		{
			for (int i = 0; i < m_trigger_action_param_list[1] && i < 64; ++ i)
			{
				Monster *monster = MONSTERPOOL->CreateMonster(m_trigger_action_param_list[0], m_scene, gamecommon::GetDisRandPos(m_posi, m_range));
				if (NULL != monster && NULL != role)
				{
					
				}
			}
		}
		break;

	case TRIGGER_ACTION_TYPE_SURROUND_MONSTER:
		{
			if (NULL != role)
			{
				for (int i = 0; i < m_trigger_action_param_list[1] && i < 64; ++ i)
				{
					Monster *monster = MONSTERPOOL->CreateMonster(m_trigger_action_param_list[0], m_scene, gamecommon::GetDisRandPos(role->GetPos(), m_trigger_action_param_list[2]));
					if (NULL != monster)
					{
						
					}
				}
			}
		}
		break;

	case TRIGGER_ACTION_TYPE_SET_FB_FOLLOW_NPC:
		{
			//if (NULL != role)
			//{
			//	m_scene->GetSpecialLogic()->SetUserFBFollowNPC(role, m_trigger_action_param_list[0]);
			//}
		}
		break;

	case TRIGGER_ACTION_TYPE_RESET_FB_FOLLOW_NPC:
		{
			//if (NULL != role)
			//{
			//	m_scene->GetSpecialLogic()->SetUserFBFollowNPC(role, 0);
			//}
		}
		break;

	case TRIGGER_ACTION_TYPE_SPECIAL_DICI:
		{
			this->StartEffect(m_trigger_action_param_list[0]);
		}
		break;

	case TRIGGER_ACTION_TYPE_SPECIAL_BEILAO:
		{
			this->StartEffect(m_trigger_action_param_list[0]);
		}
		break;

	case TRIGGER_ACTION_TYPE_SPECIAL_BANMASUO:
		{
			
		}
		break;

	case TRIGGER_ACTION_TYPE_PLAY_STORY:
		break;

	case TRIGGER_ACTION_TYPE_CREATE_MONSTER_APPOINT_POS:
		{
			Posi pos(m_trigger_action_param_list[1], m_trigger_action_param_list[2]);
			MONSTERPOOL->CreateMonster(m_trigger_action_param_list[0], m_scene, pos);	
		}
		break;

	case TRIGGER_ACTION_TYPE_SPECIALLOGIC:
	case TRIGGER_ACTION_TYPE_ICE_LANDMINE:
	case TRIGGER_ACTION_TYPE_FIRE_LANDMINE:
		{
			if (NULL != m_scene)
			{
				bool to_remove = false;
				m_scene->GetSpecialLogic()->OnTrigger(role, this, m_trigger_action_param_list, &to_remove);

				if (to_remove) m_scene->DeleteObj(m_obj_id);
			}
		}
		break;

	default:
		break;
	}
}

void Trigger::StartEffect(int interval)
{
	if (m_is_in_effect) return;

	m_is_in_effect = true;
	m_effect_interval = interval;
	m_effect_time = interval;
}

void Trigger::StopEffect()
{
	m_is_in_effect = false;
	m_effect_interval = 0;
	m_effect_time = 0;
}

bool Trigger::Effect()
{
	bool has_role = false;
	static ObjID obj_list[64] = {0};
	int count = m_scene->GetZoneMatrix()->GetObjByArea(m_posi, m_range, m_range, obj_list, sizeof(obj_list) / sizeof(obj_list[0]));
	for (int i = 0; i < count && i < (int)sizeof(obj_list) / (int)sizeof(obj_list[0]); ++ i)
	{
		Obj *obj = m_scene->GetObj(obj_list[i]);
		if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
		{
			Role *role = (Role*)obj;
			if (role->IsAlive()) 
			{
				this->AttachEffect(role);
				has_role = true;
			}
		}
	}

	if (has_role)
	{
		switch (m_trigger_action_type)
		{
		case TRIGGER_ACTION_TYPE_SPECIAL_DICI:
			{	
				EffectObj::CreateEffectObject(m_scene, m_posi, PRODUCT_ID_TRIGGER_SPECIAL_DICI, EffectBase::PRODUCT_METHOD_TRIGGER, EngineAdapter::Instance().Time() + m_effect_interval / 1000);
			}
			break;

		case TRIGGER_ACTION_TYPE_SPECIAL_BEILAO:
			{
				EffectObj::CreateEffectObject(m_scene, m_posi, PRODUCT_ID_TRIGGER_SPECIAL_BEILAO, EffectBase::PRODUCT_METHOD_TRIGGER, EngineAdapter::Instance().Time() + m_effect_interval / 1000);
			}
			break;
		}
	}

	return has_role;
}

void Trigger::AttachEffect(Role *role)
{
	switch (m_trigger_action_type)
	{
	case TRIGGER_ACTION_TYPE_SPECIAL_DICI:
		{	
			{
				EffectOnce *effect_once = new EffectOnce(m_obj_id, PRODUCT_ID_TRIGGER_SPECIAL_DICI, -m_trigger_action_param_list[1], EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_TRIGGER);
				role->AddEffect(effect_once);
			}
			
			{
				int dec_movespeed = static_cast<int>(-m_trigger_action_param_list[2] * role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) / 100);

				EffectMoveSpeed *effect_speed = new EffectMoveSpeed(m_obj_id, PRODUCT_ID_TRIGGER_SPECIAL_DICI, m_effect_interval, EBT_CHIHUAN, 1, EffectBase::PRODUCT_METHOD_TRIGGER);
				effect_speed->SetMoveSpeed(dec_movespeed, -m_trigger_action_param_list[2]);
				role->AddEffect(effect_speed);
			}
		}
		break;
	
	case TRIGGER_ACTION_TYPE_SPECIAL_BEILAO:
		{	
			{
				EffectOnce *effect_once = new EffectOnce(m_obj_id, PRODUCT_ID_TRIGGER_SPECIAL_BEILAO, -m_trigger_action_param_list[1], EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_TRIGGER);
				role->AddEffect(effect_once);
			}
		}
		break;
	}
}

bool Trigger::HasHiddenMonster()
{
	UNSTD_STATIC_CHECK(13 == TRIGGER_ACTION_TYPE_MAX);

	return ((TRIGGER_ACTION_TYPE_CREATE_MONSTER == m_trigger_action_type || TRIGGER_ACTION_TYPE_SURROUND_MONSTER == m_trigger_action_type) && 
		m_trigger_count < m_trigger_max_count); 
}

void Trigger::Reset()
{
	this->StopEffect();
	
	m_trigger_count = 0;
}

int Trigger::GetActionParam(int param_idx)
{
	if (param_idx < 0 || param_idx >= TRIGGER_ACTION_PARAM_MAX) return 0;
	return m_trigger_action_param_list[param_idx];
}
