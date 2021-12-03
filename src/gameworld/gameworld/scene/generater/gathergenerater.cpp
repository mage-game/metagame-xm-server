#include "gathergenerater.h"
#include "obj/otherobj/gatherobj.h"
#include "scene/scene.h"
#include "item/itempool.h"
#include "item/itembase.h"

#include "engineadapter.h"
#include "config/logicconfigmanager.hpp"

GatherGenerater::GatherGenerater(Scene *scene)
	: m_scene(scene), m_first(true), m_not_created(true), m_is_pause(false), m_is_once(false), m_create_interval(3000), m_create_time(3000), 
	m_gather_id(0), m_gather_time(0), m_evil_add(0), m_disappear_after_gather(true), m_gather_index(0), m_had_update(false), m_need_adjust(false)
{

}

GatherGenerater::~GatherGenerater()
{

}

bool GatherGenerater::Init(unsigned int create_interval, const Posi &pos, UInt16 gather_id, int gather_time, int evil_add, bool disappear_after_gather, int gather_index)
{
	m_create_interval = create_interval;
	m_create_time = m_create_interval;
	m_pos = pos;
	m_gather_id = gather_id;
	m_gather_time = gather_time;
	m_evil_add = evil_add;
	m_disappear_after_gather = disappear_after_gather;
	m_gather_index = gather_index;

	return true;
}

void GatherGenerater::Update(unsigned long interval)
{
	if (!m_had_update)
	{
		if (nullptr != m_scene)
		{
			m_refresh_adjust_obj.refresh_id = m_gather_id;
			m_refresh_adjust_obj.scene_id = m_scene->GetSceneID();
			m_refresh_adjust_obj.refresh_type = RefreshAdjustKeyObj::REFRESH_OBJ_TYPE_GATHER;

			m_need_adjust = LOGIC_CONFIG->GetDynamicAdjustConfig().IsAdjustRefreshTime(m_refresh_adjust_obj);
		}

		m_had_update = true;
	}

	if (m_not_created && !m_is_pause)
	{
		m_create_time += interval;
		if (m_create_time > m_create_interval)
		{
			m_not_created = false;
			m_create_time = 0;

			if (m_is_once)
			{
				m_is_pause = true;
			}

			GatherObj *gather_obj = new GatherObj();
			gather_obj->SetPos(m_pos);
			gather_obj->Init(this, m_gather_id, m_gather_time, m_evil_add, m_disappear_after_gather);

			m_scene->AddObj(gather_obj);

			m_first = false;
		}
	}
}

void GatherGenerater::Gather(GatherObj *gather_obj)
{
	m_not_created = true;

	// 调整刷新时间
	if (m_need_adjust)
	{
		int adjust_interval = LOGIC_CONFIG->GetDynamicAdjustConfig().GetRefreshAdjustInterval(m_refresh_adjust_obj, m_scene->RoleNum());
		if (adjust_interval > 0)
		{
			m_create_interval = adjust_interval;
		}
	}
}

void GatherGenerater::SetPause(bool is_pause)
{
	m_is_pause = is_pause;
}

void GatherGenerater::SetIsOnce(bool is_once)
{
	m_is_once = is_once;
}

void GatherGenerater::RefreshNow()
{
	if (m_not_created)
	{
		m_is_pause = false;
		m_create_time = m_create_interval;
	}
}

unsigned int GatherGenerater::GetGatherNextFlushTime()
{
	if (!m_not_created || m_is_pause || m_create_time >= m_create_interval)
	{
		return 0;
	}

	int next_flush_interval = m_create_interval - m_create_time;
	return static_cast<unsigned int>(EngineAdapter::Instance().Time() + next_flush_interval / 1000);
}