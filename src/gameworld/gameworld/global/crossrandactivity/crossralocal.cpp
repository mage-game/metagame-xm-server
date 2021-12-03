#include "crossralocal.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "internalcomm.h"
#include "servercommon/internalprotocal/crossgamerandactivitymsg.h"
#include "protocal/msgcsrandactivity.hpp"
#include "obj/character/role.h"
#include "engineadapter.h"
#include "world.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "servercommon/crossrandactivitydef.hpp"
#include "global/cross/crossmanager.h"

CrossRALocal::CrossRALocal(int activity_type) : 
	m_activity_type(activity_type), m_activity_status(ACTIVITY_STATUS_CLOSE), m_begin_time(0),
	m_end_time(0), m_big_param_data(NULL), m_big_param_data2(NULL)
{
}

CrossRALocal::~CrossRALocal()
{
}

void CrossRALocal::Init(const CrossRandActivityData &data)
{
	m_custom_data = data;
}

void CrossRALocal::SetBigData(CrossRandActivityBigParam *big_data)
{
	m_big_param_data = big_data;
}

void CrossRALocal::SetBigData2(CrossRandActivityBigParam2 *big_data)
{
	m_big_param_data2 = big_data;
}

void CrossRALocal::GetInitParam(CrossRandActivityData *data)
{
	*data = m_custom_data;
}

void CrossRALocal::OnUserLogin(Role *user)
{
	this->NotifyActStatusToClient(user);
}

void CrossRALocal::OnCommonMessageFromCross(int msg_type, char *data)
{
	if (crossgameramsg::MSG_COMMON_ACTIVITY_STATUS_CG == msg_type)
	{
		unsigned int now_sec = (unsigned int)EngineAdapter::Instance().Time();
		crossgameramsg::CGSyncActivityStatus *msg = (crossgameramsg::CGSyncActivityStatus*)data;
		m_activity_status = msg->activity_status;

		// 计算本服跟跨服的时间差
		int delta_time = msg->cross_cur_time - now_sec;

		m_begin_time = msg->begin_time - delta_time;
		m_end_time = msg->end_time - delta_time;

		if (0 != msg->is_status_change) // 跨服那边是状态切换的通知，调用对应函数
		{
			if (ACTIVITY_STATUS_OPEN == msg->activity_status)
			{
				this->OnActivityStatusChange(ACTIVITY_STATUS_CLOSE, ACTIVITY_STATUS_OPEN);
			}
			else if (ACTIVITY_STATUS_CLOSE == msg->activity_status)
			{
				this->OnActivityStatusChange(ACTIVITY_STATUS_OPEN, ACTIVITY_STATUS_CLOSE);
			}
		}

		static ActivityStatus ggas;
		ggas.activity_type = (short)m_activity_type;
		ggas.status = m_activity_status;
		ggas.activity_begin_time = m_begin_time;
		ggas.activity_end_time = m_end_time;
		ActivityShadow::Instance().SetActivityStatus(&ggas);

		this->NotifyActStatusToClient();
	}
}

void CrossRALocal::OnForceToNextState()
{
	static crossgameramsg::GCForceToNextStatus ftns;
	this->SendToCrossActivity((char*)&ftns, sizeof(ftns));
}

bool CrossRALocal::OnForceSetActivityTime(unsigned int begin_time, unsigned int end_time)
{
	if (begin_time < 0 || begin_time > end_time)
	{
		return false;
	}

	static crossgameramsg::GCForceSetActivityTime fsat;
	fsat.begin_time = begin_time;
	fsat.end_time = end_time;
	fsat.gameworld_cur_time = (unsigned int)EngineAdapter::Instance().Time();
	this->SendToCrossActivity((char*)&fsat, sizeof(fsat));

	return true;
}

void CrossRALocal::SendToCrossActivity(char *data, int length)
{
	int header_len = sizeof(crossgameprotocal::GameCrossSyncCrossRandActivityData);

	if (NULL == data || length <= 0 || length >= (crossgameprotocal::MAX_CROSSGAMEPROTOCAL_LENGTH - header_len))
	{
		return;
	}

	static char send_buff[crossgameprotocal::MAX_CROSSGAMEPROTOCAL_LENGTH];

	crossgameprotocal::GameCrossSyncCrossRandActivityData *cgsas = (crossgameprotocal::GameCrossSyncCrossRandActivityData*)send_buff;
	*cgsas = crossgameprotocal::GameCrossSyncCrossRandActivityData();
	cgsas->activity_type = m_activity_type;

	memcpy(send_buff + header_len, data, length);

	CrossManager::Instance().SendToCrossWithCrossRandActivityType(m_activity_type, (const char *)&send_buff, header_len + length);
	//InternalComm::Instance().SendToCross((const char *)&send_buff, header_len + length);
}

void CrossRALocal::NotifyActStatusToClient(Role *user)
{
	static Protocol::SCCrossRandActivityStatus as;
	as.activity_type = (short)m_activity_type;
	as.status = static_cast<char>(m_activity_status);
	as.begin_time = m_begin_time;
	as.end_time = m_end_time;

	
	if (NULL != user)
	{
		EngineAdapter::Instance().NetSend(user->GetNetId(), (const char*)&as, sizeof(as));
	}
	else 
	{
		World::GetInstWorld()->SendToAllGateway((const char *)&as, sizeof(as));
	}
}

void CrossRALocal::OnActivityStatusChange(int from_status, int to_status)
{
	static ActivityStatus ggas;
	ggas.activity_type = (short)m_activity_type;
	ggas.status = m_activity_status;
	ggas.activity_begin_time = m_begin_time;
	ggas.activity_end_time = m_end_time;
	World::GetInstWorld()->GetSceneManager()->OnSyncActivityStatus(&ggas);
}