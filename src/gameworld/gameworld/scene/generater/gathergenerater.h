#ifndef __GATHERGENERATER_H__
#define __GATHERGENERATER_H__

#include <stdlib.h>

#include "gamedef.h"
#include "config/dynamicadjustconfig.hpp"

class GatherObj;
class Role;
class Scene;

class GatherGenerater
{
public:
	GatherGenerater(Scene *scene);
	~GatherGenerater();

	bool Init(unsigned int create_interval, const Posi &pos, UInt16 gather_id, int gather_time, int evil_add, bool disappear_after_gather, int gather_index);

	void Update(unsigned long interval);
	void Gather(GatherObj *gather_obj);

	int GetGatherId() { return m_gather_id; }
	int GetGatherIndex() { return m_gather_index; }
	Posi GetGatherPosi() { return m_pos; }

	void SetPause(bool is_pause);
	void SetIsOnce(bool is_once);
	void RefreshNow();

	unsigned int GetGatherNextFlushTime();

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	Scene *m_scene;

	bool m_first;
	bool m_not_created;
	bool m_is_pause;
	bool m_is_once;
	unsigned int m_create_interval;
	unsigned int m_create_time;
	Posi m_pos;
	UInt16 m_gather_id;
	int m_gather_time;
	int m_evil_add;
	bool m_disappear_after_gather;
	int m_gather_index;

	bool m_had_update;
	bool m_need_adjust;
	RefreshAdjustKeyObj m_refresh_adjust_obj;
};

#endif

