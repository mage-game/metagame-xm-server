#include "crossactivitytuanzhan.hpp"
#include "crossuser/crossuser.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "crossserver.h"
#include "servercommon/serverconfig/crosstuanzhanconfig.hpp"
#include "crossserver/engineadapter.h"

CrossActivityTuanzhan::CrossActivityTuanzhan(CrossActivityManager *cross_activity_manager) 
	:CrossActivity(cross_activity_manager, CROSS_ACTIVITY_TYPE_TUANZHAN), m_open_cfg_idx(0)
{

}

CrossActivityTuanzhan::~CrossActivityTuanzhan()
{

}

void CrossActivityTuanzhan::Init(const CrossActivityData &data)
{
	m_status_end_time = this->CalcActivityNextOpenTime();
	m_is_init_finish = true;
}

void CrossActivityTuanzhan::OnCrossUserLogin(CrossUser *cross_user)
{
	if (NULL == cross_user) return;

	if (ACTIVITY_STATUS_CLOSE == m_activity_status)
	{
		return;
	}

	UserID user_id = IntToUid(cross_user->GetCrossUid());
	if (CROSS_ACTIVITY_TYPE_TUANZHAN != GetCrossUserCrossActivityType(user_id))
	{
		return;
	}

	crossgameprotocal::CrossGameTuanzhanGotoScene cgtzgs;
	cgtzgs.cross_uid = cross_user->GetCrossUid();
	CrossServer::GetInstCrossServer()->SendToHiddenGameWorld((const char*)&cgtzgs, sizeof(cgtzgs));
}

time_t CrossActivityTuanzhan::CalcActivityNextOpenTime()
{
	if (!CrossTuanzhanConfig::Instance().GetOtherCfg().is_open)
	{
		return 0;
	}

	ActivityOpenTimeInfo cfg_list[CROSS_TUANZHAN_OPEN_TIME_MAX_COUNT];
	int cfg_count = CrossTuanzhanConfig::Instance().GetOpenActivityConfig(cfg_list);

	int min_interval = INT_MAX;
	int min_locate_idx = 0;
	for (int index = 0; index < cfg_count; ++ index)
	{
		ActivityOpenTimeInfo &open_cfg = cfg_list[index];
		int interval = EngineAdapter::Instance().NextWeekInterval(open_cfg.activity_week_day, 
			open_cfg.activity_ready_time / 100, open_cfg.activity_ready_time % 100, 0);

		if (interval < min_interval)
		{
			min_interval = interval;
			min_locate_idx = index;
		}
	}

	m_open_cfg_idx = min_locate_idx;

	return EngineAdapter::Instance().Time() + min_interval;
}

void CrossActivityTuanzhan::OnChangeToNextStatus()
{
	if (!CrossTuanzhanConfig::Instance().GetOtherCfg().is_open)
	{
		return;
	}

	// 设置默认的等待时间跟开启时间
	int ready_dur_s = CROSS_TUANZHAN_DEFAULT_READY_TIME_S;
	int open_dur_s = CROSS_TUANZHAN_DEFAULT_OPEN_TIME_S;

	ActivityOpenTimeInfo *open_cfg = CrossTuanzhanConfig::Instance().GetActivityOpenTimeInfo(m_open_cfg_idx);
	if (NULL != open_cfg)
	{
		ready_dur_s = open_cfg->ready_dur_s;
		open_dur_s = open_cfg->open_dur_s;
	}

	if (ACTIVITY_STATUS_CLOSE == m_activity_status)
	{
		int standby_time = ready_dur_s;
		if (standby_time > 0)
		{
			m_activity_status = ACTIVITY_STATUS_STANDY;
			m_status_end_time = EngineAdapter::Instance().Time() + ready_dur_s;
		}
		else
		{
			m_activity_status = ACTIVITY_STATUS_OPEN;
			m_status_end_time = EngineAdapter::Instance().Time() + open_dur_s;
		}
	}
	else if (ACTIVITY_STATUS_STANDY == m_activity_status)
	{
		m_activity_status = ACTIVITY_STATUS_OPEN;
		m_status_end_time = EngineAdapter::Instance().Time() + open_dur_s;
	}
	else
	{
		m_activity_status = ACTIVITY_STATUS_CLOSE;
		m_status_end_time = this->CalcActivityNextOpenTime();
	}
}

void CrossActivityTuanzhan::OnFightResult(crossgameprotocal::GameCrossTuanzhanSyncFightResult * tzsfr)
{
	static crossgameprotocal::CrossGameUpdateCrossRecord cgucr; 
	cgucr.update_cross_record_type = crossgameprotocal::CrossGameUpdateCrossRecord::UPDATE_CROSS_RECORD_CROSS_TUANZHAN;
	cgucr.uid = UidToInt(LongLongToUniqueUserID(tzsfr->uuid).user_id);
	cgucr.record_param_list[0] = tzsfr->score;
	cgucr.record_param_list[1] = tzsfr->user_is_win;
	cgucr.record_param_list[2] = tzsfr->side_rank;
	CrossServer::GetInstCrossServer()->SendToGameWorld(LongLongToUniqueUserID(tzsfr->uuid), (const char *)&cgucr, sizeof(cgucr));
}

