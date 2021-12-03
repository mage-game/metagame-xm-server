#ifndef __SPECIAL_FUN_OPEN_XIANNV_FB_H__
#define __SPECIAL_FUN_OPEN_XIANNV_FB_H__

#include "scene/speciallogic/speciallogic.hpp"
#include "other/fb/funopenfbconfig.hpp"
#include "protocal/msgfb.h"

class SpecialFunOpenXiannvFb: public SpecialLogic
{
 public:
	SpecialFunOpenXiannvFb(Scene *scene);
	virtual ~SpecialFunOpenXiannvFb();

	virtual void OnRoleEnterScene(Role *role);

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual bool SpecialCanMountOn(Role *role){return false;}

 	void RefreshStepMonsterClienReq(Role *role, Protocol::CSFunOpenStoryStep *cmd);
	void OnCreateGather(Role *role, int gather_x, int gather_y, int gather_id, int gather_time);
	void SetObjPos(Role * role, Protocol::CSFunOpenSetObjToPos *cmd);

protected:

	void SyncFBInfo();
	void SyncFBStepEndInfo();

	int m_owner_uid;											// 副本拥有者用户ID
	bool m_is_finish;											// 是否结束

	bool m_can_operate_next_step;								// 是否可以执行步骤 可以：不可以 1：0

	int m_cur_step_monster_total_num;							// 当前步骤怪物总数
	int m_cur_step_monster_kill_num;							// 当前步骤怪物击杀数

	int m_cur_step_gather_total_num;							// 当前步骤采集物生成总数
	int m_cur_step_gather_num;									// 当前步骤采集采集物总数

	short m_cur_step;											// 当前步骤

	std::map<int, FunOpenMonsterCfg> m_monster_cfg_map;
	std::set<int> m_monster_set;

	std::map<int, FunOpenGatherCfg> m_gather_cfg_map;
};

#endif


