#ifndef __RAND_ACTIVITY_GOLDEN_PIG_HPP__
#define __RAND_ACTIVITY_GOLDEN_PIG_HPP__

#include "global/randactivity/randactivity.hpp"

class RandActivityGoldenPig : public RandActivity
{
public:
	RandActivityGoldenPig(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityGoldenPig();

	enum RA_GOLDEN_PIG_SUMMON_TYPE
	{
		JUNIOR_SUMMON = 0,
		MEDIUM_SUMMON,
		SENIOR_SUMMON,

		MAX_SUMMON_TYPE,
	};

	enum GOLDEN_PIG_STATE
	{
		GOLDEN_PIG_STATE_NOT_EXIST = 0,
		GOLDEN_PIG_STATE_EXIST,
	};

	static const unsigned int BOSS_EXIST_TIME = 60 * 30;

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	void SetGoldenPigBossState(int summon_type, int state);
	int GetGoldenPigBossState(int summon_type);

	void SetGoldenPigBossObjID(int summon_type, ObjID obj_id);

	void SetGoldenPigBossSummonTime(int summon_type, unsigned int summon_time);
	int GetGoldenPigBossSummonTime(int summon_type);

private:
	void ResetGoldenPigBoss();

	int m_golden_pig_boss_state[MAX_SUMMON_TYPE];
	ObjID m_golden_pig_boss_id_list[MAX_SUMMON_TYPE];
	unsigned int m_last_golden_pig_summon_time[MAX_SUMMON_TYPE];
};

#endif 