#include "hightimermgr.h"

MemPool *g_update_obj_mem_pool = NULL;

HighTimerMgr::HighTimerMgr() : m_is_update(false), m_last_pi_time(0)
{
	g_update_obj_mem_pool = new MemPool(sizeof(TimerObj<TimerObjBase>), 64, "TimerObj");
}

HighTimerMgr::~HighTimerMgr()
{
	delete g_update_obj_mem_pool;
}

HighTimerMgr &HighTimerMgr::Instance()
{
	static HighTimerMgr instance;
	return instance;
}

void HighTimerMgr::UnRegister(void *obj)
{
	if (NULL == obj) return;

	UPDATE_OBJ_MAP::iterator it = m_update_obj_map.find(obj);
	if (m_update_obj_map.end() != it)
	{
		if (m_is_update)
		{
			it->second->SetIsDelete(true);
		}
		else
		{
			delete it->second;
			m_update_obj_map.erase(it);
		}
	}
}

void HighTimerMgr::Update(unsigned long pi_time)
{
	if (pi_time - m_last_pi_time < 50)
	{
		return;
	}

	unsigned long interval = pi_time - m_last_pi_time;
	m_last_pi_time = pi_time;

	m_is_update = true;
	for (UPDATE_OBJ_MAP::iterator it = m_update_obj_map.begin(); it != m_update_obj_map.end();)
	{
		if (it->second->IsDelete())
		{
			delete it->second;
			m_update_obj_map.erase(it++);
		}
		else
		{
			it->second->OnTrigger(interval);
			it++;
		}
	}
	m_is_update = false;
}