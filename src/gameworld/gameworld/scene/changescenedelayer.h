#ifndef __CHANGESCENEDELAYER_H__
#define __CHANGESCENEDELAYER_H__

#include "gamedef.h"
#include "itimermodule.h"

#include <stdlib.h>

class SceneManager;

/*
	适用于不能切换场景的逻辑中[函数调用后上层函数还有进一步的逻辑操作]，例如适用
	物品的Use函数中，此时如果切换场景，则物品生效但物品在新切换的场景的角色中仍存在
*/

enum CHANGE_SCENE_REASON
{
	CHANGE_SCENE_REASON_INVALID = 0,
	CHANGE_SCENE_REASON_NO_LIFE,					// 无剩余复活次数
	CHANGE_SCENE_REASON_FB_FINISH,					// 副本已经结束
	CHANGE_SCENE_REASON_DABAO_FINISH,				// 打宝地图已经结束

	CHANGE_SCENE_REASON_MAX
};

class ChangeSceneDelayer : public ITimerCallback
{
public:
	ChangeSceneDelayer(SceneManager *scene_manager, 
						SceneIndex scene_index, 
						ObjID obj_id, 
						const UserID &user_id, 
						int target_scene_id, 
						int target_scene_key, 
						const Posi &target_pos,
						int change_scene_reason = CHANGE_SCENE_REASON_INVALID);

	virtual ~ChangeSceneDelayer(){}

	virtual void OnTimer();
	virtual void Free();

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	SceneManager	*m_scene_manager;

	SceneIndex		m_scene_index;
	ObjID			m_obj_id;
	UserID			m_user_id;
	int				m_target_scene_id;
	int				m_target_scene_key;
	Posi			m_target_pos;
	int				m_change_scene_reason;
};

#endif
