#include "crossactivitypasture.hpp"
#include "crossuser/crossuser.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "crossserver.h"
#include "servercommon/serverconfig/crosspastureconfig.hpp"
#include "crossserver/engineadapter.h"

CrossActivityPasture::CrossActivityPasture(CrossActivityManager *cross_activity_manager) 
:CrossActivity(cross_activity_manager, CROSS_ACTIVITY_TYPE_PASTURE), m_open_cfg_idx(0)
{

}

CrossActivityPasture::~CrossActivityPasture()
{

}

void CrossActivityPasture::Init(const CrossActivityData &data)
{
	m_status_end_time = this->CalcActivityNextOpenTime();
	m_is_init_finish = true;
}

void CrossActivityPasture::OnCrossUserLogin(CrossUser *cross_user)
{
	if (NULL == cross_user) return;

	if (ACTIVITY_STATUS_CLOSE == m_activity_status)
	{
		return;
	}

	UserID user_id = IntToUid(cross_user->GetCrossUid());
	if (CROSS_ACTIVITY_TYPE_PASTURE != GetCrossUserCrossActivityType(user_id))
	{
		return;
	}

	crossgameprotocal::CrossGameCrossActivityGotoScene cgtzgs;
	cgtzgs.cross_uid = cross_user->GetCrossUid();
	cgtzgs.cross_activity_type = CROSS_ACTIVITY_TYPE_PASTURE;
	CrossServer::GetInstCrossServer()->SendToHiddenGameWorld((const char*)&cgtzgs, sizeof(cgtzgs));
}

time_t CrossActivityPasture::CalcActivityNextOpenTime()
{
	if (!CrossPastureConfig::Instance().GetOtherCfg().is_open)
	{
		return 0;
	}

	CrossPastureActivityOpenTimeInfo cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT];
	int cfg_count = CrossPastureConfig::Instance().GetOpenActivityConfig(cfg_list);

	int min_interval = INT_MAX;
	int min_locate_idx = 0;
	for (int index = 0; index < cfg_count; ++ index)
	{
		CrossPastureActivityOpenTimeInfo &open_cfg = cfg_list[index];
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

void CrossActivityPasture::OnChangeToNextStatus()
{
	if (!CrossPastureConfig::Instance().GetOtherCfg().is_open)
	{
		return;
	}

	// 设置默认的等待时间跟开启时间
	int ready_dur_s = CROSS_TUANZHAN_DEFAULT_READY_TIME_S;
	int open_dur_s = CROSS_TUANZHAN_DEFAULT_OPEN_TIME_S;

	CrossPastureActivityOpenTimeInfo *open_cfg = CrossPastureConfig::Instance().GetCrossPastureActivityOpenTimeInfo(m_open_cfg_idx);
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

void CrossActivityPasture::OnReceiveInfoSync(crossgameprotocal::GameCrossPastureInfoSync * pis)
{
	static crossgameprotocal::CrossGameUpdateCrossRecord cgucr; 
	cgucr.update_cross_record_type = crossgameprotocal::CrossGameUpdateCrossRecord::UPDATE_CROSS_RECORD_CROSS_PASTURE;
	cgucr.uid = UidToInt(LongLongToUniqueUserID(pis->uuid).user_id);
	cgucr.record_param_list[0] = pis->get_score;
	cgucr.record_param_list[1] = pis->get_score_times;
	cgucr.record_param_list[2] = pis->rank;
	if (cgucr.uid == 0)
	{
		CrossServer::GetInstCrossServer()->SendToAllGameWorld((const char *)&cgucr, sizeof(cgucr));
	}
	else 
	{
		CrossServer::GetInstCrossServer()->SendToGameWorld(LongLongToUniqueUserID(pis->uuid), (const char *)&cgucr, sizeof(cgucr));
	}
}

