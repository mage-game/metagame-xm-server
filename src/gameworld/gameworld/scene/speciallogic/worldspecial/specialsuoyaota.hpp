#ifndef __SPECIAL_SUOYAOTA_HPP__
#define __SPECIAL_SUOYAOTA_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgfb.h"

class SpecialSuoYaoTa : public SpecialLogic
{
public:
	SpecialSuoYaoTa(Scene *scene);
	virtual ~SpecialSuoYaoTa();

	virtual bool CanDestroy() { return this->ActivityIsStop(); }

	virtual void Update(unsigned long interval, time_t now_second);
	virtual bool SpecialCanGuWu() { return true; }
	virtual void OnRoleEnterScene(Role *role);
	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);

	virtual bool OnTimeout();

	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);

	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialIsFriend(Role *role, Obj *obj, bool check_alive = true);

	void OnActivityStop();
	void OnActivityStart();

protected:
	bool ActivityIsStop();

	void NotifyFbInfoToRole(Role *role, int reason);
	void FlushMonsterHelper();
	void AddUserScoreHelper(const UserID &user_id, int score);

	void InitSYTTask();
	void OnTaskParamChange(int task_type, int param0);
	bool GetTaskCurrStepIdx(int *step_idx);

	void FBFinish(bool is_pass); 

	bool m_is_finish;

	int m_state;
	int m_fb_lv;												// 副本等级

	Protocol::SCSuoYaoTaFBInfo m_fb_info;
};

#endif // __SPECIAL_SUOYAOTA_HPP__







