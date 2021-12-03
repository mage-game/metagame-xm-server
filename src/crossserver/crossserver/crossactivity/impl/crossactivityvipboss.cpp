#include "crossactivityvipboss.hpp"
#include "crossuser/crossuser.h"
#include "crossserver.h"
#include "crossserver/engineadapter.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "servercommon/serverconfig/crossvipbossconfig.hpp"

CrossActivityCrossVipBoss::CrossActivityCrossVipBoss(CrossActivityManager *cross_activity_manager)
	: CrossActivity(cross_activity_manager, CROSS_ACTIVITY_TYPE_VIP_BOSS)
{

}

CrossActivityCrossVipBoss::~CrossActivityCrossVipBoss()
{

}

void CrossActivityCrossVipBoss::Init(const CrossActivityData &data)
{
	m_status_end_time = EngineAdapter::Instance().Time();
	m_is_init_finish = true;
}

void CrossActivityCrossVipBoss::OnRegisterGameServer(NetID global_netid)
{
	if (!CrossConfig::Instance().CheckCrossServerHasCrossActivity(m_cross_activity_type)) return;
	CrossActivity::OnRegisterGameServer(global_netid);

	// 当服务器连接上，同步一下怪物信息
	{
		crossgameprotocal::CrossGameSyncCrossCommonBossInfoReq sccbir;

		sccbir.global_netid = global_netid;

		CrossServer::GetInstCrossServer()->SendToHiddenGameWorld((const char *)&sccbir, sizeof(sccbir));
	}
}

void CrossActivityCrossVipBoss::Update(unsigned long interval, time_t now_second)
{
	CrossActivity::Update(interval, now_second);
}

void CrossActivityCrossVipBoss::OnCrossUserLogin(CrossUser *cross_user)
{
	if (NULL == cross_user) return;

	if (ACTIVITY_STATUS_CLOSE == m_activity_status)
	{
		return;
	}

	UserID user_id = IntToUid(cross_user->GetCrossUid());
	if (CROSS_ACTIVITY_TYPE_VIP_BOSS != GetCrossUserCrossActivityType(user_id))
	{
		return;
	}

	crossgameprotocal::CrossGameCrossActivityGotoScene cgtzgs;
	cgtzgs.cross_uid = cross_user->GetCrossUid();
	cgtzgs.cross_activity_type = CROSS_ACTIVITY_TYPE_VIP_BOSS;
	CrossServer::GetInstCrossServer()->SendToHiddenGameWorld((const char*)&cgtzgs, sizeof(cgtzgs));
}

void CrossActivityCrossVipBoss::OnChangeToNextStatus()
{
	if (!CrossVipBossConfig::Instance().GetOtherCfg().is_open)
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


