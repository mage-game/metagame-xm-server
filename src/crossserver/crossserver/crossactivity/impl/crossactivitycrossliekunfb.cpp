
#include "crossactivitycrossliekunfb.hpp"
#include "crossuser/crossuser.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "crossserver.h"
#include "crossserver/engineadapter.h"
#include "servercommon/serverconfig/crossliekunfbconfig.hpp"

CrossActivityCrossLieKunFb::CrossActivityCrossLieKunFb(CrossActivityManager *cross_activity_manager)
	: CrossActivity(cross_activity_manager, CROSS_ACTIVITY_TYPE_LIEKUN_FB), m_open_cfg_idx(0)
{
	memset(m_scene_list, 0, sizeof(m_scene_list));	
}

CrossActivityCrossLieKunFb::~CrossActivityCrossLieKunFb()
{
	memset(m_scene_list, 0, sizeof(m_scene_list));
}

void CrossActivityCrossLieKunFb::Init(const CrossActivityData &data)
{
	m_status_end_time = this->CalcActivityNextOpenTime();
	m_is_init_finish = true;
}

void CrossActivityCrossLieKunFb::OnCrossUserLogin(CrossUser *cross_user)
{
	if (NULL == cross_user) return;

	if (ACTIVITY_STATUS_CLOSE == m_activity_status)
	{
		return;
	}

	UserID user_id = IntToUid(cross_user->GetCrossUid());
	if (CROSS_ACTIVITY_TYPE_LIEKUN_FB != GetCrossUserCrossActivityType(user_id))
	{
		return;
	}

	crossgameprotocal::CrossGameCrossActivityGotoScene cgtzgs;
	cgtzgs.cross_uid = cross_user->GetCrossUid();
	cgtzgs.cross_activity_type = CROSS_ACTIVITY_TYPE_LIEKUN_FB;
	CrossServer::GetInstCrossServer()->SendToHiddenGameWorld((const char*)&cgtzgs, sizeof(cgtzgs));
}

void CrossActivityCrossLieKunFb::OnRecvCrossLieKunFBInfo(crossgameprotocal::GameCrossLieKunFBInfoSync *gclkfbis)
{
	for (int i = LIEKUN_ZONE_TYPE_MAIN; i < LIEKUN_ZONE_TYPE_COUNT; ++i)
	{
		m_scene_list[i].role_num = gclkfbis->scene_list[i].role_num;
	}
}

void CrossActivityCrossLieKunFb::OnRecvCrossLieKunKillZoneBossGuildInfo(int zone, GuildID guild_id)
{
	if (zone < LIEKUN_ZONE_TYPE_MAIN || zone >= LIEKUN_ZONE_TYPE_COUNT)
	{
		return;
	}

	m_scene_list[zone].kill_zone_boss_guild_id = guild_id;
}

void CrossActivityCrossLieKunFb::SendCrossLieKunFBPlayerInfo(NetID global_netid, const UniqueUserID & unique_user_id, GuildID guild_id)
{
	if (guild_id <= 0)
	{
		return;
	}

	static Protocol::SCCrossLieKunFBPlayerInfo scclkfbrni;
	
	bool find = false;
	for (int i = LIEKUN_ZONE_TYPE_MAIN; i < LIEKUN_ZONE_TYPE_COUNT; ++i)
	{
		scclkfbrni.role_num[i] = m_scene_list[i].role_num;
		if (m_scene_list[i].kill_zone_boss_guild_id == guild_id)
		{
			find = true;
			break;
		}
	}

	if (find)
	{
		scclkfbrni.is_enter_main_zone = 1;
	}
	else
	{
		scclkfbrni.is_enter_main_zone = 0;
	}

	CrossServer::GetInstCrossServer()->SendToUser(global_netid, unique_user_id.user_id, (const char*)&scclkfbrni, sizeof(scclkfbrni));
}

time_t CrossActivityCrossLieKunFb::CalcActivityNextOpenTime()
{
	if (!CrossLieKunFbConfig::Instance().GetOtherCfg().is_open)
	{
		return 0;
	}

	CrossLieKunFbActivityOpenTimeCfg cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT];
	int cfg_count = CrossLieKunFbConfig::Instance().GetOpenActivityConfigList(cfg_list);

	int min_interval = INT_MAX;
	int min_locate_idx = 0;
	for (int index = 0; index < cfg_count; ++index)
	{
		CrossLieKunFbActivityOpenTimeCfg &open_cfg = cfg_list[index];
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

void CrossActivityCrossLieKunFb::OnChangeToNextStatus()
{
	if (!CrossLieKunFbConfig::Instance().GetOtherCfg().is_open)
	{
		m_activity_status = ACTIVITY_STATUS_CLOSE;
		m_status_end_time = 0;
		return;
	}

	// 设置默认的等待时间跟开启时间
	int ready_dur_s = CROSS_TUANZHAN_DEFAULT_READY_TIME_S;
	int open_dur_s = CROSS_TUANZHAN_DEFAULT_OPEN_TIME_S;

	const CrossLieKunFbActivityOpenTimeCfg *open_cfg = CrossLieKunFbConfig::Instance().GetActivityOpenTimeInfo(m_open_cfg_idx);
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

	if (ACTIVITY_STATUS_CLOSE == m_activity_status)
	{
		this->SendCrossLieKunFBTopReward();
		this->ResetCrossLieKunFBInfo();
	}
}

void CrossActivityCrossLieKunFb::SendCrossLieKunFBTopReward()
{
	crossgameprotocal::CrossGameLieKunFBTopRewardSync msg;

	CrossServer::GetInstCrossServer()->SendToAllGameWorld((const char *)&msg, sizeof(msg));
}

void CrossActivityCrossLieKunFb::ResetCrossLieKunFBInfo()
{
	memset(m_scene_list, 0, sizeof(m_scene_list));
}
