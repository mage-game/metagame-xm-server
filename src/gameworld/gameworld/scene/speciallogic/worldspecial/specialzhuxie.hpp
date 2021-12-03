#ifndef __SPECIAL_ZHU_XIE_HPP__
#define __SPECIAL_ZHU_XIE_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"
#include <vector>

class SpecialZhuXie : public SpecialLogic
{
public:
	SpecialZhuXie(Scene *scene);
	virtual ~SpecialZhuXie();

	virtual void Update(unsigned long interval, time_t now_second);
	void CheckBornBoss(time_t now_second);
	void CheckBornMonster(time_t now_second);
	void CheckBornGather(time_t now_second);

	virtual bool CanDestroy() { return false; }

	virtual void OnCharacterDie(Character *dead_cha, ObjID killer_objid);
	void OnKillRole(Role *role, Role *dead_role);
	void OnKillBoss(Role *role, Monster *boss);
	void OnKillMonster(Role *role, Monster *monster);
	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
	virtual void OnRoleEnterScene(Role *role);

	virtual bool SpecialRelive(Role *role, int realive_type);

	virtual bool SpecialCanMountOn(Role *role);

	void OnActivityStatusChange(int status);

	void OnFetchTaskReward(Role *role, int task_id);

protected:
	void SendZhuXieInfoToAllUser();
	void SendZhuXieUserInfo(Role *role);
	void CheckUserTaskInfo(Role *role, short task_type, int param_id);
	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);

	void OnActivityStart();
	void OnActivityClose();

	int m_world_level;
	int m_cur_monster_num;
	int m_cur_gather_num;
	time_t m_last_check_reward_time;

	struct ZhuXieBossInfo
	{
		ZhuXieBossInfo()
		{
			this->Reset();
		}

		void Reset()
		{
			boss_objid = INVALID_OBJ_ID;
			next_boss_flush_time = 0;
		}

		ObjID boss_objid;
		unsigned int next_boss_flush_time;
	};

	ZhuXieBossInfo m_zhuxie_boss_info_list[ZHUXIE_BOSS_NUM];
	std::vector<int> m_gather_objid_vec;
};

#endif // __SPECIAL_ZHU_XIE_HPP__


