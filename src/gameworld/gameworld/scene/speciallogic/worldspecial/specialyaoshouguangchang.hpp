#ifndef __SPECIAL_YAOSHOU_GUANGCHANG_HPP__
#define __SPECIAL_YAOSHOU_GUANGCHANG_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgfb.h"

class SpecialYaoShouGuangChang : public SpecialLogic
{
public:
	SpecialYaoShouGuangChang(Scene *scene);
	virtual ~SpecialYaoShouGuangChang();

	virtual bool CanDestroy() { return this->ActivityIsStop(); }

	virtual void Update(unsigned long interval, time_t now_second);
	virtual bool SpecialCanGuWu() { return true; }

	virtual void OnRoleEnterScene(Role *role);
	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual void OnRemoveObj(Obj *obj);
	virtual bool OnTimeout();

	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialIsFriend(Role *role, Obj *obj, bool check_alive = true);

	void OnActivityStop();
	void OnActivityStart();

protected:
	bool ActivityIsStop();

	void NotifyFbInfoToRole(Role *role, int reason);

	virtual void OnAllMonsterDead();
	void FlushMonsterHelper();
	void AddUserScoreHelper(const UserID &user_id, int score);

	void FBFinish(bool is_pass); 

	bool m_is_finish;

	int m_state;

	int m_wave_index;											// 波数
	int m_fb_lv;												// 副本等级

	Protocol::SCYaoShouGuangChangFBInfo m_fb_info;
};

#endif // __SPECIAL_YAOSHOU_GUANGCHANG_HPP__








