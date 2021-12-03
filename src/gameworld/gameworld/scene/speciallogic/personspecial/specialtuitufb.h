#pragma once
#ifndef __SPECIAL_TUITU_FB_H__
#define __SPECIAL_TUITU_FB_H__
#include "gameworld/gameworld/scene/speciallogic/speciallogic.hpp"
#include <vector>

class SpecialTuitu : public SpecialLogic
{
public:
	SpecialTuitu(Scene *scene);
	virtual ~SpecialTuitu();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnAddObj(Obj *obj);
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual bool OnTimeout();
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual void Reset();

	void OnReqNextLevel();

protected:
	virtual void OnAllMonsterDead();

private:
	void OnFinish(int star);
	void SyncFBSceneLogicInfo(Role *role);

	bool m_is_finish;				//是否结束
	bool m_is_pass;					//是否通关
	int m_pass_star;				//通关星数

	int m_owner_uid;				//副本主人
	int m_fb_type;					//推图副本类型
	int m_chapter;					//章节
	int m_level;					//关卡
	unsigned int m_start_timestamp;	//开始时间戳

	int m_total_boss_num;
	int m_cur_boss_num;
	int m_total_monster_num;
	int m_cur_monster_num;

	unsigned int m_die_timestamp;	//死亡时间戳

	std::vector<ItemConfigData> fall_item_vec;		//掉落物品列表用于展示
};

#endif