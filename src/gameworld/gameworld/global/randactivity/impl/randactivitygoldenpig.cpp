#include "randactivitygoldenpig.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "scene/scene.h"
#include "world.h"

RandActivityGoldenPig::RandActivityGoldenPig(RandActivityManager *rand_activity_manager) :
	RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_GOLDEN_PIG)
{
	memset(m_golden_pig_boss_state, 0, sizeof(m_golden_pig_boss_state));
	memset(m_golden_pig_boss_id_list, 0, sizeof(m_golden_pig_boss_id_list));
	memset(m_last_golden_pig_summon_time, 0, sizeof(m_last_golden_pig_summon_time));
}

RandActivityGoldenPig::~RandActivityGoldenPig()
{

}

void RandActivityGoldenPig::Update(unsigned long interval, time_t now_second)
{
	RandActivity::Update(interval, now_second);

	unsigned int now_sec = static_cast<unsigned int>(now_second);
	for (int i = 0; i < MAX_SUMMON_TYPE; ++i)
	{
		if (0 != m_last_golden_pig_summon_time[i] && now_sec > m_last_golden_pig_summon_time[i] + BOSS_EXIST_TIME)
		{
			const RandActivityGoldenPigOtherCfg *other_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRAGoldenPigOtherCfg();
			if (NULL == other_cfg)
			{
				return;
			}

			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(other_cfg->scene_id, 0);
			if (NULL != scene && 0 != m_golden_pig_boss_id_list[i])
			{
				scene->DeleteObj(m_golden_pig_boss_id_list[i]);
				m_last_golden_pig_summon_time[i] = 0;
				m_golden_pig_boss_state[i] = GOLDEN_PIG_STATE_NOT_EXIST;
			}

			World::GetInstWorld()->GetSceneManager()->OnRAGoldenPigSendBossState();
		}
	}
}

void RandActivityGoldenPig::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	if (ACTIVITY_STATUS_OPEN == from_status && ACTIVITY_STATUS_CLOSE == to_status)
	{
		this->ResetGoldenPigBoss();
	}

	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);
}

void RandActivityGoldenPig::SetGoldenPigBossState(int summon_type, int state)
{
	if (summon_type < JUNIOR_SUMMON || summon_type >= MAX_SUMMON_TYPE)
	{
		return;
	}
	
	if (state < GOLDEN_PIG_STATE_NOT_EXIST || state > GOLDEN_PIG_STATE_EXIST)
	{
		return;
	}

	m_golden_pig_boss_state[summon_type] = state;
}

int RandActivityGoldenPig::GetGoldenPigBossState(int summon_type)
{
	if (summon_type < JUNIOR_SUMMON || summon_type >= MAX_SUMMON_TYPE)
	{
		return 0;
	}

	return m_golden_pig_boss_state[summon_type];
}

void RandActivityGoldenPig::SetGoldenPigBossObjID(int summon_type, ObjID obj_id)
{
	if (summon_type < JUNIOR_SUMMON || summon_type >= MAX_SUMMON_TYPE)
	{
		return;
	}

	m_golden_pig_boss_id_list[summon_type] = obj_id;
}

void RandActivityGoldenPig::SetGoldenPigBossSummonTime(int summon_type, unsigned int summon_time)
{
	if (summon_type < JUNIOR_SUMMON || summon_type >= MAX_SUMMON_TYPE)
	{
		return;
	}

	m_last_golden_pig_summon_time[summon_type] = summon_time;
}

int RandActivityGoldenPig::GetGoldenPigBossSummonTime(int summon_type)
{
	if (summon_type < JUNIOR_SUMMON || summon_type >= MAX_SUMMON_TYPE)
	{
		return 0;
	}

	return m_last_golden_pig_summon_time[summon_type];
}

void RandActivityGoldenPig::ResetGoldenPigBoss()
{
	for (int i = 0; i < MAX_SUMMON_TYPE; ++i)
	{
		const RandActivityGoldenPigOtherCfg *other_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRAGoldenPigOtherCfg();
		if (NULL == other_cfg)
		{
			return;
		}

		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(other_cfg->scene_id, 0);
		if (NULL != scene && 0 != m_golden_pig_boss_id_list[i])
		{
			scene->DeleteObj(m_golden_pig_boss_id_list[i]);
			m_last_golden_pig_summon_time[i] = 0;
			m_golden_pig_boss_state[i] = GOLDEN_PIG_STATE_NOT_EXIST;
		}
	}

	memset(m_golden_pig_boss_state, 0, sizeof(m_golden_pig_boss_state));
	memset(m_golden_pig_boss_id_list, 0, sizeof(m_golden_pig_boss_id_list));
	memset(m_last_golden_pig_summon_time, 0, sizeof(m_last_golden_pig_summon_time));
}