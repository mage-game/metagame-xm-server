#ifndef __SPECIAL_COINFB_HPP__
#define __SPECIAL_COINFB_HPP__

#include "scene/speciallogic/speciallogic.hpp"

class SpecialCoinFB : public SpecialLogic
{
public:
	SpecialCoinFB(Scene *scene);
	virtual ~SpecialCoinFB();

	virtual void Update(unsigned long interval, time_t now_second);
	bool StartCreateWaveMonsters(short wave);
	void CreateOneMonster(int monster_id);

	virtual bool SpecialCanGuWu() { return true; }

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnLeaveFB(Role *role);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual bool OnTimeout();
	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual bool SpecialCanMountOn(Role *role) { return false; }

protected:
	Role * GetOwnerRole();

	virtual void OnAllMonsterDead();
	void OnCoinFBFinish(bool is_pass, bool is_active_leave_fb = false);			// 副本结束
	void SyncFBSceneLogicInfo(bool is_active_leave_fb = false);					// 副本信息

	int m_dailyfb_type;															// 日常副本类型
	int m_max_wave;

	bool m_is_first;															
	bool m_is_finish;															// 是否结束
	bool m_is_pass;																// 是否通关

	long long m_get_exp;
	int m_get_coin;

	int m_wave;
	unsigned int m_next_wave_start_time;

	int m_create_monster_count;
	int m_create_monster_id;
	unsigned int m_next_create_time;

	ObjID m_owner_objid; 
	UserID m_owner_user_id;
};

#endif // __SPECIAL_COINFB_HPP__


