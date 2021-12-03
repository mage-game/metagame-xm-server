#ifndef __CHANGESCENEDELAYER_H__
#define __CHANGESCENEDELAYER_H__

#include "gamedef.h"
#include "itimermodule.h"

#include <stdlib.h>

class SceneManager;

/*
	�����ڲ����л��������߼���[�������ú��ϲ㺯�����н�һ�����߼�����]����������
	��Ʒ��Use�����У���ʱ����л�����������Ʒ��Ч����Ʒ�����л��ĳ����Ľ�ɫ���Դ���
*/

enum CHANGE_SCENE_REASON
{
	CHANGE_SCENE_REASON_INVALID = 0,
	CHANGE_SCENE_REASON_NO_LIFE,					// ��ʣ�ิ�����
	CHANGE_SCENE_REASON_FB_FINISH,					// �����Ѿ�����
	CHANGE_SCENE_REASON_DABAO_FINISH,				// �򱦵�ͼ�Ѿ�����

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
