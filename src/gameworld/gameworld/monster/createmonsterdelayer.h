#ifndef __CREATEMONSTERDELAYER_H__
#define __CREATEMONSTERDELAYER_H__

#include "gamedef.h"
#include "itimermodule.h"

#include <stdlib.h>

class SceneManager;

class CreateMonsterDelayer : public ITimerCallback
{
public:
	CreateMonsterDelayer(SceneManager *scene_manager, 
						SceneIndex scene_index, 
						UInt16 monster_id,
						const Posi &pos,
						ObjID enemy_objid = INVALID_OBJ_ID,
						const UserID &enemy_user_id = INVALID_USER_ID,
						const UserID &owner_user_id = INVALID_USER_ID,
						int add_maxhp = 0);

	virtual ~CreateMonsterDelayer(){}

	virtual void OnTimer();
	virtual void Free();

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	SceneManager	*m_scene_manager;

	SceneIndex		m_scene_index;
	UInt16			m_monster_id;
	Posi			m_pos;
	ObjID			m_enemy_objid;
	UserID			m_enemy_user_id;
	UserID			m_owner_user_id;
	int				m_add_maxhp;
};

#endif

