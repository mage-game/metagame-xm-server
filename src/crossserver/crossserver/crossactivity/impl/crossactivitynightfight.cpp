#include "crossactivitynightfight.hpp"
#include "crossuser/crossuser.h"
#include "crossserver.h"
#include "crossserver/engineadapter.h"
#include "servercommon/serverconfig/doublesidecrossactivity/crossnightfightconfig.hpp"

CrossActivityNightFight::CrossActivityNightFight(CrossActivityManager *cross_activity_manager)
	: CrossActivity(cross_activity_manager, CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB), m_open_cfg_idx(0)
{

}

CrossActivityNightFight::~CrossActivityNightFight()
{

}

void CrossActivityNightFight::Init(const CrossActivityData &data)
{
	m_status_end_time = this->CalcActivityNextOpenTime();
	m_is_init_finish = true;
}

void CrossActivityNightFight::OnCrossUserLogin(CrossUser *cross_user)
{
	if (NULL == cross_user) return;

	if (ACTIVITY_STATUS_CLOSE == m_activity_status)
	{
		return;
	}

	UserID user_id = IntToUid(cross_user->GetCrossUid());
	if (CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB != GetCrossUserCrossActivityType(user_id))
	{
		return;
	}

	crossgameprotocal::CrossGameCrossActivityGotoScene cgtzgs;
	cgtzgs.cross_uid = cross_user->GetCrossUid();
	cgtzgs.cross_activity_type = CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB;
	CrossServer::GetInstCrossServer()->SendToHiddenGameWorld((const char*)&cgtzgs, sizeof(cgtzgs));
}

void CrossActivityNightFight::OnCrossUserLogout(CrossUser * cross_user)
{

}

void CrossActivityNightFight::OnRecvNightFightRankTop(const crossgameprotocal::GameCrossnNightFightInfoSync * cgnfi)
{
	crossgameprotocal::CrossGameNightFightInfoSync cginfo;
	cginfo.uid = UidToInt(LongLongToUniqueUserID(cgnfi->unique_user_id).user_id);
	cginfo.rank = cgnfi->rank;

	CrossServer::GetInstCrossServer()->SendToGameWorld(LongLongToUniqueUserID(cgnfi->unique_user_id), (const char*)&cginfo, sizeof(cginfo));
}

time_t CrossActivityNightFight::CalcActivityNextOpenTime()
{
	if (!CrossNightFightConfig::Instance().GetOtherCfg().is_open)
	{
		return 0;
	}

	CrossNightFightActivityOpenTimeInfo cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT];
	int cfg_count = CrossNightFightConfig::Instance().GetOpenActivityConfigList(cfg_list);

	int min_interval = INT_MAX;
	int min_locate_idx = 0;
	for (int index = 0; index < cfg_count; ++index)
	{
		CrossNightFightActivityOpenTimeInfo &open_cfg = cfg_list[index];
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

void CrossActivityNightFight::OnChangeToNextStatus()
{
	if (!CrossNightFightConfig::Instance().GetOtherCfg().is_open)
	{
		m_activity_status = ACTIVITY_STATUS_CLOSE;
		m_status_end_time = 0;
		return;
	}

	// 设置默认的等待时间跟开启时间
	int ready_dur_s = CROSS_TUANZHAN_DEFAULT_READY_TIME_S;
	int open_dur_s = CROSS_TUANZHAN_DEFAULT_OPEN_TIME_S;

	CrossNightFightActivityOpenTimeInfo *open_cfg = CrossNightFightConfig::Instance().GetActivityOpenTimeInfo(m_open_cfg_idx);
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

