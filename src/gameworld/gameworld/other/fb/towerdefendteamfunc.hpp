#ifndef _TOWERDEFENDTEAMFUNC_HPP_
#define _TOWERDEFENDTEAMFUNC_HPP_

#include "servercommon/servercommon.h"

#include "gamedef.h"
#include "itimermodule.h"

#include <stdlib.h>

class SceneManager;

class Role;

class RealiveDelayer : public ITimerCallback
{
public:
	RealiveDelayer(SceneManager *scene_manager, SceneIndex scene_index, ObjID obj_id, const UserID &user_id, int hp_percent);
	virtual ~RealiveDelayer(){}

	virtual void OnTimer();
	virtual void Free();

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	SceneManager	*m_scene_manager;

	SceneIndex		m_scene_index;
	ObjID			m_obj_id;
	UserID			m_user_id;
	int				m_hp_percent;
};

#endif
