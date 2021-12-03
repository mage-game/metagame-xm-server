#include "worldeventobj.h"
#include "scene/scene.h"
#include "obj/character/role.h"

#include "other/event/eventhandler.hpp"

#include "config/logicconfigmanager.hpp"

#include "scene/activityshadow/activityshadow.hpp"
#include "protocal/msgactivity.hpp"
#include "global/worldevent/worldeventmanager.hpp"
#include "protocal/msgfight.h"
#include "servercommon/errornum.h"

#include "scene/speciallogic/teamspecial/zhuaguimanager.hpp"

WorldEventObj::WorldEventObj()
	: Obj(OBJ_TYPE_WORLDEVENT_OBJ), m_world_event_id(0), m_event_type(0), m_event_param(0), m_disappear_time(0), m_max_hp(0), m_hp(0), m_touch_distance(0)
{

}

WorldEventObj::~WorldEventObj()
{

}

void WorldEventObj::Init(int world_event_id, int event_type, int event_param, int max_hp, int touch_distance, time_t disappear_time)
{
	m_world_event_id = world_event_id;
	m_event_type = event_type;
	m_event_param = event_param;
	m_disappear_time = disappear_time;
	m_max_hp = max_hp;
	m_hp = max_hp;
	m_touch_distance = touch_distance;
}

void WorldEventObj::Update(unsigned long interval, time_t now_second, unsigned int now_dayid)
{
	if (m_disappear_time > 0 && now_second > m_disappear_time)	
	{
		m_scene->DeleteObj(m_obj_id);
	}

	if (m_hp <= 0)
	{
		m_scene->DeleteObj(m_obj_id);
	}
}

void WorldEventObj::Touch(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (m_touch_distance <= 0)
	{
		return;
	}

	if ((role->GetPos() - m_posi).IsLongerThan(m_touch_distance))
	{
		role->NoticeNum(errornum::EN_WORLD_EVENT_DISTANCE_LIMIT);
		return;
	}

	if (NULL != m_scene)
	{
		if (WORLD_EVENT_OBJ_ZHUAGUI_NPC == m_event_type)
		{
			ZhuaGuiMgr::Instance().OnTouchNpc(role, m_scene->GetSceneID(), m_obj_id);
		}
		else 
		{
			WorldEventManager::Instance().OnTouchEventObj(role, m_scene->GetSceneID(), m_obj_id);
		}
	}
}

void WorldEventObj::DecHp(long long dec_hp)
{
	if (dec_hp > 0)
	{
		m_hp -= dec_hp;
		if (m_hp < 0) m_hp = 0;

		Protocol::SCObjChangeBlood oba;
		oba.obj_id = m_obj_id;
		oba.deliverer = 0;
		oba.product_method = 0;
		oba.fighttype = EffectBase::FIGHT_TYPE_NORMAL;
		oba.real_blood = -dec_hp;
		oba.blood = -dec_hp;
		oba.skill = 0;
		if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, &oba, sizeof(Protocol::SCObjChangeBlood));
	}
}



