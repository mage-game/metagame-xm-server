#ifndef __SPECIAL_FUN_OPEN_MOUNT_FB_H__
#define __SPECIAL_FUN_OPEN_MOUNT_FB_H__

#include "scene/speciallogic/speciallogic.hpp"
#include "other/fb/funopenfbconfig.hpp"
#include "protocal/msgfb.h"

class SpecialFunOpenMountFb: public SpecialLogic
{
public:
	SpecialFunOpenMountFb(Scene *scene);
	virtual ~SpecialFunOpenMountFb();

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual bool SpecialCanMountOn(Role *role){return false;}

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);

 	void RefreshStepMonsterClienReq(Role *role, Protocol::CSFunOpenStoryStep *cmd);
	void OnCreateGather(Role *role, int gather_x, int gather_y, int gather_id, int gather_time);
	void SetObjPos(Role * role, Protocol::CSFunOpenSetObjToPos *cmd);

protected:

	void SyncFBInfo();
	void SyncFBStepEndInfo();

	int m_owner_uid;											// ����ӵ�����û�ID
	bool m_is_finish;											// �Ƿ����

	bool m_can_operate_next_step;								// �Ƿ����ִ�в��� ���ԣ������� 1��0
	
	int m_cur_step_monster_total_num;							// ��ǰ�����������
	int m_cur_step_monster_kill_num;							// ��ǰ��������ɱ��

	int m_cur_step_gather_total_num;							// ��ǰ����ɼ�����������
	int m_cur_step_gather_num;									// ��ǰ����ɼ��ɼ�������

	short m_cur_step;											// ��ǰ����

	int m_mount_gather_id;

	std::map<int, FunOpenMonsterCfg> m_monster_cfg_map;
	std::set<int> m_monster_set;

	std::map<int, FunOpenGatherCfg> m_gather_cfg_map;
};

#endif


