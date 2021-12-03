#ifndef _SPECIAL_COMBINE_SERVER_BOSS_HPP_
#define _SPECIAL_COMBINE_SERVER_BOSS_HPP_

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"
#include "servercommon/serverdef.h"

#include <map>

class Role;

class SpecialCombineServerBoss : public SpecialLogic
{
public:
	struct CSABossInfo
	{
		CSABossInfo() : boss_id(0), born_pos(0, 0), next_flush_time(0), original_monster_id(0){}
		
		int boss_id;
		Posi born_pos;
		unsigned int next_flush_time;
		int original_monster_id;
	};

	SpecialCombineServerBoss(Scene *scene);
	virtual ~SpecialCombineServerBoss();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnCharacterDie(Character *dead_cha, ObjID killer_objid);
	virtual bool CanDestroy() { return m_is_finish; }
	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid) { m_common_boss_next_refresh_time = 0; }
	void SendBossInfo(Role *role);

	void OnActivityStatusChange(int status);

private:
	enum CSA_BOSS_STATUS
	{
		CSA_BOSS_STATE_NOT_EXIST = 0,			//不存在
		CSA_BOSS_STATE_EXIST = 1,				//存在
	};

	enum CSA_GREAT_BOSS_STATUS
	{
		STATUS_NOT_IN_TIME = 0,					//不在刷新时间
		STATUS_IN_TIME = 1,						//在刷新时间
	};

	void CheckRefreshCommonBoss(unsigned int now_second, bool is_force = false);
	void RefreshGreatBoss();
	void RefreshGather();

	bool IsInCommonBossRefreshTime();
	bool IsInGreatBossRefreshTime();

	long long CalcBossExtraHpHelper();

	void KillAllCommonBoss();

	void RefreshCommonBoss(bool is_force = false);

	int m_great_boss_refresh_status;

	int m_great_boss_count;
	int m_gather_count;

	unsigned int m_common_boss_refresh_begin_time;
	unsigned int m_common_boss_refresh_end_time;

	bool m_is_finish;

	std::map<int, CSABossInfo> m_common_boss_map;
	std::map<int, CSABossInfo> m_great_boss_map;

	std::map<int, CSABossInfo> m_real_common_boss_map;
	std::map<int, CSABossInfo> m_real_great_boss_map;

	unsigned int m_common_boss_next_refresh_time;

	bool m_can_refresh_common_boss;

	bool is_activity_start;
};


#endif // _SPECIAL_COMBINE_SERVER_BOSS_HPP_

