
#include "crossactivitycrossbianjingzhidi.hpp"
#include "crossuser/crossuser.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "crossserver.h"
#include "crossserver/engineadapter.h"
#include "servercommon/serverconfig/crossbianjingzhidiconfig.h"

CrossActivityCrossBianJingZhiDi::CrossActivityCrossBianJingZhiDi(CrossActivityManager *cross_activity_manager)
	: CrossActivity(cross_activity_manager, CROSS_ACTIVITY_TYPE_CROSS_BIANJINGZHIDI), m_open_cfg_idx(0), m_next_boss_refresh_time(0)
{
	memset(m_boss_list, 0, sizeof(m_boss_list));
}

CrossActivityCrossBianJingZhiDi::~CrossActivityCrossBianJingZhiDi()
{
}

void CrossActivityCrossBianJingZhiDi::Init(const CrossActivityData &data)
{
	m_status_end_time = this->CalcActivityNextOpenTime();
	m_is_init_finish = true;
}

void CrossActivityCrossBianJingZhiDi::OnCrossUserLogin(CrossUser *cross_user)
{
	if (NULL == cross_user) return;

	if (ACTIVITY_STATUS_CLOSE == m_activity_status)
	{
		return;
	}

	UserID user_id = IntToUid(cross_user->GetCrossUid());
	if (CROSS_ACTIVITY_TYPE_CROSS_BIANJINGZHIDI != GetCrossUserCrossActivityType(user_id))
	{
		return;
	}

	crossgameprotocal::CrossGameCrossActivityGotoScene cgtzgs;
	cgtzgs.cross_uid = cross_user->GetCrossUid();
	cgtzgs.cross_activity_type = CROSS_ACTIVITY_TYPE_CROSS_BIANJINGZHIDI;
	CrossServer::GetInstCrossServer()->SendToHiddenGameWorld((const char*)&cgtzgs, sizeof(cgtzgs));
}

time_t CrossActivityCrossBianJingZhiDi::CalcActivityNextOpenTime()
{
	if (!CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiOtherCfg().is_open)
	{
		return 0;
	}

	CrossBianJingZhiDiActivityOpenTimeCfg cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT];
	int cfg_count = CrossBianJingZhiDiConfig::Instance().GetOpenActivityConfigList(cfg_list);

	int min_interval = INT_MAX;
	int min_locate_idx = 0;
	for (int index = 0; index < cfg_count; ++index)
	{
		CrossBianJingZhiDiActivityOpenTimeCfg &open_cfg = cfg_list[index];
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

void CrossActivityCrossBianJingZhiDi::OnChangeToNextStatus()
{
	if (!CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiOtherCfg().is_open)
	{
		m_activity_status = ACTIVITY_STATUS_CLOSE;
		m_status_end_time = 0;
		return;
	}

	// 设置默认的等待时间跟开启时间
	int ready_dur_s = CROSS_TUANZHAN_DEFAULT_READY_TIME_S;
	int open_dur_s = CROSS_TUANZHAN_DEFAULT_OPEN_TIME_S;

	const CrossBianJingZhiDiActivityOpenTimeCfg *open_cfg = CrossBianJingZhiDiConfig::Instance().GetActivityOpenTimeInfo(m_open_cfg_idx);
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

void CrossActivityCrossBianJingZhiDi::SendCrossBianJingZhiDiBossInfo(NetID global_netid, const UniqueUserID & unique_user_id)
{
	static Protocol::SCCrossServerBianJingZhiDiBossInfo sccbjzdbi;

	sccbjzdbi.boss_num = 0;
	sccbjzdbi.boss_total_num = 0;
	sccbjzdbi.left_boss_refresh_time = -1;  // -1表示还未开始

	for (int boss_type = 0; boss_type < CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE; ++boss_type)
	{
		if (m_boss_list[boss_type].boss_id > 0)
		{
			sccbjzdbi.boss_total_num++;
			
			if (m_boss_list[boss_type].live_flag > 0)
			{
				sccbjzdbi.boss_num++;
			}
		}
	}

	if (m_next_boss_refresh_time > 0)
	{
		sccbjzdbi.left_boss_refresh_time = static_cast<int>(m_next_boss_refresh_time - EngineAdapter::Instance().Time());
		if (sccbjzdbi.left_boss_refresh_time < 0) sccbjzdbi.left_boss_refresh_time = 0;
	}

	CrossServer::GetInstCrossServer()->SendToUser(global_netid, unique_user_id.user_id, (const char*)&sccbjzdbi, sizeof(sccbjzdbi));
}

void CrossActivityCrossBianJingZhiDi::OnSyncCrossBianJingZhiDiBossInfo(crossgameprotocal::GameCrossSyncCrossBianJingZhiBossInfo *scrad)
{
	if (NULL == scrad) return;
	
	m_next_boss_refresh_time = scrad->next_boss_refresh_time;

	for (int boss_type = 0; boss_type < CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE; ++boss_type)
	{
		m_boss_list[boss_type].boss_id = scrad->boss_list[boss_type].boss_id;
		m_boss_list[boss_type].live_flag = scrad->boss_list[boss_type].live_flag;
	}
}
