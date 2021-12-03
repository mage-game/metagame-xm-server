#include "crossactivity.hpp"

#include "engineadapter.h"

#include "internalcomm.h"

#include "servercommon/internalprotocal/crossglobalprotocal.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"

#include "servercommon/serverconfig/crosscommonconfig.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "crossserver.h"

CrossActivity::CrossActivity(CrossActivityManager *cross_activity_manager, int cross_activity_type)
	: m_cross_activity_manager(cross_activity_manager), m_cross_activity_type(cross_activity_type), m_is_init_finish(false), 
	m_activity_status(ACTIVITY_STATUS_CLOSE), m_status_begin_time(0), m_status_end_time(0)
{

}

CrossActivity::~CrossActivity()
{

}

void CrossActivity::Init(const CrossActivityData & data)
{
	if (CROSS_ACTIVITY_TYPE_CROSS_CHALLENGEFIELD == m_cross_activity_type)
	{
		m_is_init_finish = true;
	}
}

void CrossActivity::GetInitParam(CrossActivityData * data)
{
}

void CrossActivity::Update(unsigned long interval, time_t now_second)
{
	if (!m_is_init_finish) return;

	if (0 != m_status_end_time && now_second >= m_status_end_time)
	{
		m_status_begin_time = m_status_end_time;
		this->OnChangeToNextStatus();

		this->SyncActivityStatusToGlobal();
	}
}

void CrossActivity::OnChangeToNextStatus()
{
	m_activity_status = ACTIVITY_STATUS_CLOSE;
	m_status_end_time = 0;

	if (CROSS_ACTIVITY_TYPE_CROSS_CHALLENGEFIELD == m_cross_activity_type)
	{
		const static int ACTIVITY_OPEN_DUR_S = 3600 * 24 * 365; 

		m_activity_status = ACTIVITY_STATUS_OPEN;
		m_status_end_time = EngineAdapter::Instance().Time() + ACTIVITY_OPEN_DUR_S;
	}
}

void CrossActivity::OnRegisterGameServer(NetID global_netid)
{
	this->SyncActivityStatusToGlobal(global_netid);
	this->SyncCrossChallengefieldToGame(global_netid);
}

void CrossActivity::OnCrossUserLogin(CrossUser *cross_user)
{
	
}

void CrossActivity::OnCrossUserLogout(CrossUser *cross_user)
{
	
}

void CrossActivity::SyncActivityStatusToGlobal(NetID global_netid)
{
	if (!CrossConfig::Instance().CheckCrossServerHasCrossActivity(m_cross_activity_type)) return;

	static crossgameprotocal::CrossGameSyncActivityStatus cgsas;
	cgsas.cross_activity_type = m_cross_activity_type;
	cgsas.activity_status = m_activity_status;
	cgsas.status_begin_time = static_cast<unsigned int>(m_status_begin_time);
	cgsas.status_end_time = static_cast<unsigned int>(m_status_end_time);
	cgsas.activity_special_param = this->GetActivitySpecialParam();
	cgsas.cross_now_scecond = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (UINT_MAX == global_netid)
	{
		CrossServer::GetInstCrossServer()->SendToAllGameWorld((const char *)&cgsas, sizeof(cgsas));
	}
	else
	{
		InternalComm::Instance().NetSend(global_netid, (const char *)&cgsas, sizeof(cgsas));
	}
}

int CrossActivity::GetWaitSceneID(const UniqueUserID &unique_user_id, Posi *pos)
{
	return CrossCommonConfig::Instance().GetWaitSceneID(m_cross_activity_type, pos);
}

void CrossActivity::ForceToNextState()
{
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	m_status_end_time = now;
}

void CrossActivity::SyncCrossChallengefieldToGame(NetID global_netid)
{
	if (!CrossConfig::Instance().CheckCrossServerHasCrossActivity(m_cross_activity_type) ||
		CROSS_ACTIVITY_TYPE_CROSS_CHALLENGEFIELD != m_cross_activity_type)
	{
		return;
	}

	NetID hidden_netid = CrossServer::GetInstCrossServer()->GetHiddenServerNetid();
	if (global_netid == hidden_netid)
	{
		return;
	}

	// 当游戏服连接上，同步跨服竞技场信息
	{
		crossgameprotocal::CrossGameSyncCrossChallengefieldReq msg;

		msg.global_netid = global_netid;

		CrossServer::GetInstCrossServer()->SendToHiddenGameWorld((const char *)&msg, sizeof(msg));
	}
}

