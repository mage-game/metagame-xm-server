#include "crossactivityxiuluotower.hpp"

#include "engineadapter.h"
#include "internalcomm.h"

#include "servercommon/serverconfig/crosscommonconfig.hpp"
#include "servercommon/serverconfig/crossxiuluotowercommonconfig.hpp"

#include "crossserver.h"
#include "crossserver/crossuser/crossuser.h"
#include "crossserver/crosslog.hpp"

#include "servercommon/noticenum.h"
#include "servercommon/errornum.h"

#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "servercommon/internalprotocal/ggprotocal.h"
#include "servercommon/string/crossstr.h"
#include "servercommon/serverconfig/localconfig.hpp"

CrossActivityXiuluoTower::CrossActivityXiuluoTower(CrossActivityManager *cross_activity_manager)
	: CrossActivity(cross_activity_manager, CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER), m_open_room_count(0), m_drop_log_count(0)
{

}

CrossActivityXiuluoTower::~CrossActivityXiuluoTower()
{

}

void CrossActivityXiuluoTower::Init(const CrossActivityData &data)
{
	m_status_end_time = this->CalcActivityNextOpenTime();
	m_is_init_finish = true;
}

void CrossActivityXiuluoTower::Update(unsigned long interval, time_t now_second)
{
	CrossActivity::Update(interval, now_second);

	if (ACTIVITY_STATUS_OPEN != m_activity_status)
	{
		return;
	}
}

void CrossActivityXiuluoTower::OnDayChange()
{

}

bool CrossActivityXiuluoTower::CheckCanStartCross(const UniqueUserID &unique_user_id, int merge_server_id)
{
	return true;
}

void CrossActivityXiuluoTower::OnCrossUserLogin(CrossUser *cross_user)
{
	if (NULL == cross_user) return;

	if (ACTIVITY_STATUS_CLOSE == m_activity_status)
	{
		return;
	}

	UserID user_id = IntToUid(cross_user->GetCrossUid());
	if (CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER != GetCrossUserCrossActivityType(user_id))
	{
		return;
	}

	crossgameprotocal::CrossGameXiuluotaGotoScene cgxltgs;
	cgxltgs.cross_uid = cross_user->GetCrossUid();
	CrossServer::GetInstCrossServer()->SendToHiddenGameWorld((const char*)&cgxltgs, sizeof(cgxltgs));
}

void CrossActivityXiuluoTower::OnSyncUserResult(crossgameprotocal::GameCrossSyncXiuluoTowerResult *gcsxltr)
{
	for (int i = 0; i < gcsxltr->user_result_count && i < crossgameprotocal::XIULUO_TOWER_USER_RESULT_MAX; i++)
	{
		static crossgameprotocal::CrossGameSyncXiuluoTowerUserResult gcsxltur;
		gcsxltur.uuid = gcsxltr->user_result_list[i].uuid;
		gcsxltur.max_layer = gcsxltr->user_result_list[i].max_layer;
		gcsxltur.rank_pos = gcsxltr->user_result_list[i].rank_pos;
		gcsxltur.kill_role_count = gcsxltr->user_result_list[i].kill_role_count;
		gcsxltur.kill_boss_count = gcsxltr->user_result_list[i].kill_boss_count;
		gcsxltur.user_level = gcsxltr->user_result_list[i].user_level;
		gcsxltur.reward_cross_honor = gcsxltr->user_result_list[i].reward_cross_honor;

		CrossServer::GetInstCrossServer()->SendToGameWorld(LongLongToUniqueUserID(gcsxltur.uuid), (const char *)&gcsxltur, sizeof(gcsxltur));
	}
}

void CrossActivityXiuluoTower::OnGetDropLog(const UniqueUserID &unique_user_id)
{
	static Protocol::SCCrossXiuluoTowerDropLog cxtdl;
	cxtdl.log_count = 0;

	if (m_drop_log_count < CROSS_ACTIVITY_XIULUO_TOWER_DROP_LOG_COUNT_MAX)
	{
		for (; cxtdl.log_count < m_drop_log_count && cxtdl.log_count < CROSS_ACTIVITY_XIULUO_TOWER_DROP_LOG_COUNT_MAX; ++cxtdl.log_count)
		{
			cxtdl.log_list[cxtdl.log_count].log_type = m_drop_log_list[cxtdl.log_count].log_type;
			UNSTD_STATIC_CHECK(sizeof(cxtdl.log_list[cxtdl.log_count].name) == sizeof(m_drop_log_list[cxtdl.log_count].name));
			memcpy(cxtdl.log_list[cxtdl.log_count].name, m_drop_log_list[cxtdl.log_count].name, sizeof(m_drop_log_list[cxtdl.log_count].name));
			cxtdl.log_list[cxtdl.log_count].timestamp = m_drop_log_list[cxtdl.log_count].timestamp;
			cxtdl.log_list[cxtdl.log_count].item_id = m_drop_log_list[cxtdl.log_count].item_id;
			cxtdl.log_list[cxtdl.log_count].item_num = m_drop_log_list[cxtdl.log_count].item_num;
		}
	}
	else
	{
		int  lasted_index = m_drop_log_count % CROSS_ACTIVITY_XIULUO_TOWER_DROP_LOG_COUNT_MAX;
		for (int i = lasted_index; i < lasted_index + CROSS_ACTIVITY_XIULUO_TOWER_DROP_LOG_COUNT_MAX && cxtdl.log_count < CROSS_ACTIVITY_XIULUO_TOWER_DROP_LOG_COUNT_MAX; ++i)
		{
			int cur_index = i % CROSS_ACTIVITY_XIULUO_TOWER_DROP_LOG_COUNT_MAX;

			cxtdl.log_list[cxtdl.log_count].log_type = m_drop_log_list[cur_index].log_type;
			UNSTD_STATIC_CHECK(sizeof(cxtdl.log_list[cxtdl.log_count].name) == sizeof(m_drop_log_list[cur_index].name));
			memcpy(cxtdl.log_list[cxtdl.log_count].name, m_drop_log_list[cur_index].name, sizeof(m_drop_log_list[cur_index].name));
			cxtdl.log_list[cxtdl.log_count].timestamp = m_drop_log_list[cur_index].timestamp;
			cxtdl.log_list[cxtdl.log_count].item_id = m_drop_log_list[cur_index].item_id;
			cxtdl.log_list[cxtdl.log_count].item_num = m_drop_log_list[cur_index].item_num;

			++cxtdl.log_count;
		}
	}

	int send_len = UNSTD_OFFSETOFF(Protocol::SCCrossXiuluoTowerDropLog, log_list) + sizeof(cxtdl.log_list[0]) * cxtdl.log_count;

	CrossServer::GetInstCrossServer()->SendToUser(unique_user_id, (const char *)&cxtdl, send_len);
}

void CrossActivityXiuluoTower::OnSyncDropLog(const UniqueUserID unique_user_id, crossgameprotocal::GameCrossSyncXiuluoTowerDropInfo *gcdi)
{
	int insert_index = m_drop_log_count % CROSS_ACTIVITY_XIULUO_TOWER_DROP_LOG_COUNT_MAX;

	if (insert_index >= 0 && insert_index < CROSS_ACTIVITY_XIULUO_TOWER_DROP_LOG_COUNT_MAX)
	{
		m_drop_log_list[insert_index].log_type = gcdi->log_type;
		memcpy(m_drop_log_list[insert_index].name, gcdi->name, sizeof(gcdi->name));
		m_drop_log_list[insert_index].timestamp = gcdi->timestamp;
		m_drop_log_list[insert_index].item_id = gcdi->item_id;
		m_drop_log_list[insert_index].item_num = gcdi->item_num;

		++m_drop_log_count;
	}
}

time_t CrossActivityXiuluoTower::CalcActivityNextOpenTime()
{
	const tm * now_tm = EngineAdapter::Instance().LocalTime();
	int now_day_second = now_tm->tm_hour * 3600 + now_tm->tm_min * 60 + now_tm->tm_sec;

	CrossXiuluoTowerTimeCfg *time_cfg = CrossXiuluoTowerCommonConfig::Instance().GetActivityTimeCfg();
	if (NULL == time_cfg || !time_cfg->is_open) return 0;

	int min_interval = 0;
	for (int i = 0; i < 7; i++)
	{
		int interval = 0;
		int week_day = (now_tm->tm_wday + i) % 7;
		if (0 <= week_day && week_day < CROSS_ACTIVITY_XIULUO_TOWER_MAX_WEEK_DAYS)
		{
			if (time_cfg->open_day_list[week_day])
			{
				if (now_tm->tm_wday == week_day)
				{
					if (now_day_second >= time_cfg->open_day_second)
					{
						interval = EngineAdapter::Instance().NextWeekInterval(week_day, 0, 0, 0) + time_cfg->open_day_second;
					}
					else
					{
						interval = time_cfg->open_day_second - now_day_second;
					}
				}
				else
				{
					interval = EngineAdapter::Instance().NextWeekInterval(week_day, 0, 0, 0) + time_cfg->open_day_second;
				}

				if (min_interval <= 0 || interval < min_interval)
				{
					min_interval = interval;
				}
			}
		}
	}

	return EngineAdapter::Instance().Time() + min_interval;
}

void CrossActivityXiuluoTower::OnChangeToNextStatus()
{
	if (ACTIVITY_STATUS_CLOSE == m_activity_status)
	{
		int standby_time = CrossXiuluoTowerCommonConfig::Instance().GetActivityStandbyTime();
		if (standby_time > 0)
		{
			m_activity_status = ACTIVITY_STATUS_STANDY;
			m_status_end_time = EngineAdapter::Instance().Time() + standby_time;
		}
		else
		{
			m_activity_status = ACTIVITY_STATUS_OPEN;
			m_status_end_time = EngineAdapter::Instance().Time() + CrossXiuluoTowerCommonConfig::Instance().GetActivityOpenTime();
		}
	}
	else if (ACTIVITY_STATUS_STANDY == m_activity_status)
	{
		m_activity_status = ACTIVITY_STATUS_OPEN;
		m_status_end_time = EngineAdapter::Instance().Time() + CrossXiuluoTowerCommonConfig::Instance().GetActivityOpenTime();
	}
	else
	{
		m_activity_status = ACTIVITY_STATUS_CLOSE;
		m_status_end_time = this->CalcActivityNextOpenTime();
	}
}

void CrossActivityXiuluoTower::OnRecvSyncRankTile(crossgameprotocal::GameCrossSyncXiuluoTowerRankTitle * gcrt)
{
	crossgameprotocal::CrossGameSyncXiuluoTowerRankTitle cgxlt;
	cgxlt.uid = UidToInt(LongLongToUniqueUserID(gcrt->uuid).user_id);
	cgxlt.rank = gcrt->rank;
	CrossServer::GetInstCrossServer()->SendToGameWorld(LongLongToUniqueUserID(gcrt->uuid),(const char*)&cgxlt, sizeof(cgxlt));
}