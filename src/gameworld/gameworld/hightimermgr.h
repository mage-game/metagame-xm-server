#ifndef _FAST_UPDATER_H_
#define _FAST_UPDATER_H_

#include <cstddef>
#include <map>
#include "common/memory/mempool.h"

class TimerObjBase
{
public:
	TimerObjBase() : m_is_delete(false) {}
	virtual ~TimerObjBase() {}

	inline bool IsDelete() { return m_is_delete; }
	void SetIsDelete(bool is_delete) { m_is_delete = is_delete; }

	virtual void OnTrigger(unsigned long interval) {};

protected:
	bool m_is_delete;
};

extern MemPool *g_update_obj_mem_pool;

template<class T>
class TimerObj : public TimerObjBase
{
	typedef void (T::*TimerFunc)(unsigned long interval);
public:
	TimerObj(T *obj, TimerFunc func) : TimerObjBase(), m_obj(obj), m_func(func) {}
	~TimerObj() {}

	void *operator new(size_t c)
	{
		return g_update_obj_mem_pool->Alloc();
	}
	void operator delete(void *m)
	{
		g_update_obj_mem_pool->Free(m);
	}

	inline virtual void OnTrigger(unsigned long interval)
	{
		if (NULL != m_obj && NULL != m_func)
		{
			(m_obj->*m_func)(interval);
		}
	}

protected:
	T *m_obj;
	TimerFunc m_func;
};

class HighTimerMgr
{
	typedef std::map<void*, TimerObjBase*> UPDATE_OBJ_MAP;
public:
	static HighTimerMgr &Instance();

	template<class T>
	void Register(T *obj, void (T::*func)(unsigned long interval))
	{
		if (NULL == obj || NULL == func) return;

		TimerObjBase *(&update_obj) = m_update_obj_map[obj];
		if (NULL != update_obj)
		{
			delete update_obj;
		}
		update_obj = new TimerObj<T>(obj, func);
	}

	void UnRegister(void *obj);
	void Update(unsigned long pi_time);

protected:
	HighTimerMgr();
	~HighTimerMgr();

	bool m_is_update;
	unsigned long	m_last_pi_time;
	UPDATE_OBJ_MAP m_update_obj_map;
};

#endif
