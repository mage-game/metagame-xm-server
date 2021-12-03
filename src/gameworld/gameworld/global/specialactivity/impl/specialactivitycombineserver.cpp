#include "specialactivitycombineserver.hpp"

#include "config/logicconfigmanager.hpp"
#include "servercommon/string/globalstr.h"
#include "global/combineserveractivity/combineserveractivityconfig.hpp"
#include "global/combineserveractivity/combineserveractivitymanager.hpp"
#include "global/combineserveractivity/combineserveractivitybossconfig.hpp"
#include "world.h"

SpecialActivityCombineServer::SpecialActivityCombineServer(SpecialActivityManager *special_activity_manager)
: SpecialActivity(special_activity_manager, SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER), m_is_init_sub_activity_time(false), m_last_update_time(0), m_activity_day(0)
{
	memset(m_sub_activity_begin_time_list, 0, sizeof(m_sub_activity_begin_time_list));
	memset(m_sub_activity_end_time_list, 0, sizeof(m_sub_activity_end_time_list));
}

SpecialActivityCombineServer::~SpecialActivityCombineServer()
{

}

void SpecialActivityCombineServer::Update(unsigned long interval, time_t now_second)
{
	SpecialActivity::Update(interval, now_second);

	if (ACTIVITY_STATUS_OPEN == m_special_activity_status)
	{
		if (0 == m_last_update_time) m_last_update_time = now_second;

		if (!m_is_init_sub_activity_time)
		{
			m_activity_day = static_cast<int>(m_end_time - m_begin_time - 1) / (24 * 3600) + 1;

			this->InitSubActivityTime();

			m_is_init_sub_activity_time = true;
		}

		this->UpdateSubActivity(now_second);
	}
}

bool SpecialActivityCombineServer::ForceSetSpecialActivityTime(unsigned int begin_time, unsigned int end_time)
{
	if (SpecialActivity::ForceSetSpecialActivityTime(begin_time, end_time))
	{
		m_activity_day = static_cast<int>(m_end_time - m_begin_time - 1) / (24 * 3600) + 1;

		this->InitSubActivityTime();

		return true;
	}

	return false;
}

void SpecialActivityCombineServer::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	CombineServerActivityManager::Instance().OnCSABossRankReward();
	CombineServerActivityManager::Instance().OnCSABossClearRank();
}

void SpecialActivityCombineServer::GetSubActivitState(int list_count, char *state_list)
{
	if (0 != m_last_update_time)
	{
		for (int i = CSA_SUB_TYPE_INVALID + 1; i < list_count && i < CSA_SUB_TYPE_MAX; ++i)
		{
			char old_state = state_list[i];
			state_list[i] = m_last_update_time < m_sub_activity_begin_time_list[i] ? CSA_SUB_ACTIVITY_STATE_NO_START : 
				(m_last_update_time < m_sub_activity_end_time_list[i] ? CSA_SUB_ACTIVITY_STATE_OPEN : CSA_SUB_ACTIVITY_STATE_FINISH);

			if (CSA_SUB_TYPE_BOSS == i && CSA_SUB_ACTIVITY_STATE_OPEN == state_list[i] && old_state != CSA_SUB_ACTIVITY_STATE_OPEN)
			{
				int scene_id = LOGIC_CONFIG->GetCSABossConfig().GetCSABossOtherCfg().scene_id;

				Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, 1);
				if (NULL == scene)
				{
					World::GetInstWorld()->GetSceneManager()->CreateFB(scene_id, 1, &scene);
				}
			}
		}
	}
}

bool SpecialActivityCombineServer::SubActivitIsOpening(int sub_type)
{
	if (sub_type <= CSA_SUB_TYPE_INVALID || sub_type >= CSA_SUB_TYPE_MAX)
	{
		return false;
	}

	return m_last_update_time >= m_sub_activity_begin_time_list[sub_type] && m_last_update_time < m_sub_activity_end_time_list[sub_type];
}

void SpecialActivityCombineServer::InitSubActivityTime()
{
	time_t begin_time = m_begin_time;

	struct tm *local_begin_time = ::localtime(&begin_time);
	if (NULL == local_begin_time) return;

	time_t zero_time = begin_time - (local_begin_time->tm_hour * 3600 + local_begin_time->tm_min * 60 + local_begin_time->tm_sec);

	for (int i = CSA_SUB_TYPE_INVALID + 1; i <	CSA_SUB_TYPE_MAX; ++i)
	{
		CSATimeConfig *time_config = LOGIC_CONFIG->GetCombineServerActivityConfig().GetCSATimeConfig(i);
		if (NULL != time_config && time_config->is_open)
		{
			m_sub_activity_begin_time_list[i] = zero_time + (time_config->begin_day - 1) * 3600 * 24;
			m_sub_activity_end_time_list[i] = zero_time + (time_config->end_day - 1) * 3600 * 24 + time_config->end_time / 100 * 3600 + time_config->end_time % 100 * 60;

			if (m_sub_activity_end_time_list[i] > m_end_time - 5) 
			{
				m_sub_activity_end_time_list[i] = m_end_time - 5;
			}
		}
	}

	CombineServerActivityManager::Instance().SyncInfoToGameWorld();
}

void SpecialActivityCombineServer::UpdateSubActivity(time_t now_second)
{
	bool is_change_state = false;

	if (now_second > m_last_update_time)
	{
		for (int i = CSA_SUB_TYPE_INVALID + 1; i < CSA_SUB_TYPE_MAX; ++i)
		{
			if (m_last_update_time < m_sub_activity_begin_time_list[i] && m_sub_activity_begin_time_list[i] <= now_second)
			{
				this->SubActivityOpen(i);
				is_change_state = true;
			}

			if (m_last_update_time < m_sub_activity_end_time_list[i] && m_sub_activity_end_time_list[i] <= now_second)
			{
				this->SubActivityFinish(i);
				is_change_state = true;
			}
		}

		m_last_update_time = now_second;
	}

	if (is_change_state) CombineServerActivityManager::Instance().SyncInfoToGameWorld();
}

void SpecialActivityCombineServer::SubActivityFinish(int sub_type)
{
	CombineServerActivityManager::Instance().SubActivityFinish(sub_type);
}

void SpecialActivityCombineServer::SubActivityOpen(int sub_type)
{
	CombineServerActivityManager::Instance().SubActivityOpen(sub_type);
}

