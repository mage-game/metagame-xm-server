#include "crossactivitycrossyoumingboss.hpp"
#include "crossuser/crossuser.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "crossserver.h"
#include "crossserver/engineadapter.h"
#include "servercommon/serverconfig/crossbossconfig.hpp"

CrossActivityCrossYouMingBoss::CrossActivityCrossYouMingBoss(CrossActivityManager *cross_activity_manager)
	: CrossActivity(cross_activity_manager, CROSS_ACTIVITY_TYPE_CROSS_YOUMING_BOSS)
{

}

CrossActivityCrossYouMingBoss::~CrossActivityCrossYouMingBoss()
{

}

void CrossActivityCrossYouMingBoss::Init(const CrossActivityData &data)
{
	m_status_end_time = EngineAdapter::Instance().Time();
	m_is_init_finish = true;
}

void CrossActivityCrossYouMingBoss::OnCrossUserLogin(CrossUser *cross_user)
{
	if (NULL == cross_user) return;

	if (ACTIVITY_STATUS_CLOSE == m_activity_status)
	{
		return;
	}

	UserID user_id = IntToUid(cross_user->GetCrossUid());
	if (CROSS_ACTIVITY_TYPE_CROSS_YOUMING_BOSS != GetCrossUserCrossActivityType(user_id))
	{
		return;
	}

	crossgameprotocal::CrossGameCrossActivityGotoScene cgtzgs;
	cgtzgs.cross_uid = cross_user->GetCrossUid();
	cgtzgs.cross_activity_type = CROSS_ACTIVITY_TYPE_CROSS_YOUMING_BOSS;
	CrossServer::GetInstCrossServer()->SendToHiddenGameWorld((const char*)&cgtzgs, sizeof(cgtzgs));
}

void CrossActivityCrossYouMingBoss::OnChangeToNextStatus()
{
	if (!CrossBossConfig::Instance().GetOtherCfg().is_open)
	{
		m_activity_status = ACTIVITY_STATUS_CLOSE;
		m_status_end_time = 0;
		return;
	}

	const static int ACTIVITY_OPEN_DUR_S = 3600 * 24 * 365; // 默认给开一年时间，理论上不会有隐藏服一年不重启的

	if (ACTIVITY_STATUS_CLOSE == m_activity_status)
	{
		m_activity_status = ACTIVITY_STATUS_OPEN;
		m_status_end_time = EngineAdapter::Instance().Time() + ACTIVITY_OPEN_DUR_S;
	}
	else if (ACTIVITY_STATUS_STANDY == m_activity_status)
	{
		m_activity_status = ACTIVITY_STATUS_OPEN;
		m_status_end_time = EngineAdapter::Instance().Time() + ACTIVITY_OPEN_DUR_S;
	}
	else
	{
		m_activity_status = ACTIVITY_STATUS_CLOSE;
		m_status_end_time = 0;
	}
}

void CrossActivityCrossYouMingBoss::OnPlayerInfoResult(crossgameprotocal::GameCrossBossSyncPlayerInfo * tzsfr)
{
	static crossgameprotocal::CrossGameUpdateCrossRecord cgucr; 
	cgucr.update_cross_record_type = crossgameprotocal::CrossGameUpdateCrossRecord::UPDATE_CROSS_RECORD_CROSS_YOUMING_BOSS;
	cgucr.uid = UidToInt(LongLongToUniqueUserID(tzsfr->uuid).user_id);
	cgucr.record_param_list[0] = tzsfr->tire_value;
	cgucr.record_param_list[1] = tzsfr->today_gather_ordinary_crystal_times;
	cgucr.record_param_list[2] = tzsfr->today_gather_treasure_crystal_times;
	cgucr.record_param_list[3] = tzsfr->concern_flag[0];
	cgucr.record_param_list[4] = tzsfr->concern_flag[1];
	cgucr.record_param_list[5] = tzsfr->concern_flag[2];
	cgucr.record_param_list[6] = tzsfr->concern_flag[3];
	cgucr.record_param_list[7] = tzsfr->concern_flag[4];
	CrossServer::GetInstCrossServer()->SendToGameWorld(LongLongToUniqueUserID(tzsfr->uuid), (const char *)&cgucr, sizeof(cgucr));
}


