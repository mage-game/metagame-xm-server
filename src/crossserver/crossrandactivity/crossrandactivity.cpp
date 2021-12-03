#include "crossrandactivity.hpp"

#include "engineadapter.h"

#include "internalcomm.h"

#include "servercommon/internalprotocal/crossglobalprotocal.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"

#include "servercommon/serverconfig/crosscommonconfig.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"

#include "crossserver.h"
#include "servercommon/servercommon.h"
#include "servercommon/internalprotocal/crossgamerandactivitymsg.h"

CrossRandActivity::CrossRandActivity(int cross_activity_type)
	: m_is_init_finish(false), m_activity_type(cross_activity_type), 
	m_activity_status(ACTIVITY_STATUS_MAX), m_begin_time(0), m_end_time(0)
{
}

CrossRandActivity::~CrossRandActivity()
{

}

void CrossRandActivity::Init()
{
	const CrossRandActivityTimeCfg *open_cfg = CrossRandActivityOpenConfig::Instance().GetOpenCfg(m_activity_type);
	if (NULL == open_cfg)
	{
		return;
	}

	unsigned int now = (unsigned int)EngineAdapter::Instance().Time();

	for (int index = 0; index < open_cfg->time_cfg_count; ++ index)
	{
		if (open_cfg->time_cfg_list[index].end_timestamp < now) // 该活动时间已过去，则无视
		{
			continue;
		}

		m_open_time_list.push(open_cfg->time_cfg_list[index]);
	}

	this->ReadNextActivityTime();
	
	m_is_init_finish = true;
}

void CrossRandActivity::Update(unsigned int now_second)
{
	if (!m_is_init_finish) return;

	if (!CrossConfig::Instance().CheckCrossServerHasCrossRandActivity(m_activity_type)) return;

	if (0 == m_begin_time || 0 == m_end_time) 
	{
		if (ACTIVITY_STATUS_MAX == m_activity_status)
		{
			m_activity_status = ACTIVITY_STATUS_CLOSE;
			this->SyncActivityStatusToGameworld();
		}

		return;
	}

	unsigned int now = static_cast<unsigned int>(now_second);

	if (ACTIVITY_STATUS_MAX == m_activity_status)
	{
		if (m_begin_time <= now && now < m_end_time)
		{
			m_activity_status = ACTIVITY_STATUS_OPEN;
		}
		else
		{
			m_activity_status = ACTIVITY_STATUS_CLOSE;
		}
	}

	switch (m_activity_status)
	{
	case ACTIVITY_STATUS_CLOSE:
		{
			if (m_begin_time <= now && now < m_end_time)
			{
				m_activity_status = ACTIVITY_STATUS_OPEN;
				this->OnActivityStatusChange(ACTIVITY_STATUS_CLOSE, ACTIVITY_STATUS_OPEN);
				this->SyncActivityStatusToGameworld(true);
			}
		}
		break;

	case ACTIVITY_STATUS_OPEN:
		{
			if (now < m_begin_time || now >= m_end_time)
			{
				m_activity_status = ACTIVITY_STATUS_CLOSE;
				this->OnActivityStatusChange(ACTIVITY_STATUS_OPEN, ACTIVITY_STATUS_CLOSE);
				this->SyncActivityStatusToGameworld(true);

				this->ReadNextActivityTime();
			}
		}
		break;
	}
}

void CrossRandActivity::OnRegisterGameServer(NetID global_netid)
{
	this->SyncActivityStatusToGameworld(false, global_netid);
}

void CrossRandActivity::ForceToNextState()
{
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (ACTIVITY_STATUS_CLOSE == m_activity_status)
	{
		m_begin_time = now;

		if (m_end_time < now)
		{
			m_end_time = now + SECOND_PER_DAY;
		}
	}
	if (ACTIVITY_STATUS_OPEN == m_activity_status)
	{
		m_end_time = now;
	}

	this->SyncActivityStatusToGameworld();
}

void CrossRandActivity::ForceSetActivityTime(unsigned int begin_time, unsigned int end_time)
{
	if (begin_time > end_time)
	{
		return;
	}

	if (begin_time == m_begin_time && end_time == m_end_time)
	{
		return;
	}

	m_begin_time = begin_time;
	m_end_time = end_time;

	// 若新的结束时间影响到后续活动开启时间，把后续受影响的活动都删除
	while (!m_open_time_list.empty() && m_open_time_list.front().begin_timestamp < end_time)
	{
		m_open_time_list.pop();
	}

	this->SyncActivityStatusToGameworld();
}

void CrossRandActivity::OnCommonMessageFromGameworld(NetID global_netid, int msg_type, char *data)
{
	if (crossgameramsg::MSG_COMMON_FORCE_TO_NEXT_STATUS_GC == msg_type)
	{
		this->ForceToNextState();
	}
	else if(crossgameramsg::MSG_COMMON_FORCE_SET_ACTIVITY_TIME_GC == msg_type)
	{
		crossgameramsg::GCForceSetActivityTime *msg = (crossgameramsg::GCForceSetActivityTime *)data;
		//int delta_time = msg->gameworld_cur_time - (unsigned int)EngineAdapter::Instance().Time();
		int delta_time = 0; // gameworld服发过来的设置时间，不做修正，防止多个gameworld服都发设置时间不统一
		this->ForceSetActivityTime(msg->begin_time - delta_time, msg->end_time - delta_time);
	}
}

void CrossRandActivity::SyncActivityStatusToGameworld(bool is_status_change, NetID global_netid)
{
	static crossgameramsg::CGSyncActivityStatus msg;
	msg.is_status_change = is_status_change ? 1 : 0;
	msg.activity_status = m_activity_status;
	msg.begin_time = m_begin_time;
	msg.end_time = m_end_time;
	msg.cross_cur_time = (unsigned int)EngineAdapter::Instance().Time();
	this->SendToGameworldActivity((char*)&msg, sizeof(msg), global_netid);
}

void CrossRandActivity::SendToGameworldActivity(char *data, int length, NetID global_netid)
{
	if (!CrossConfig::Instance().CheckCrossServerHasCrossRandActivity(m_activity_type)) return;

	int header_len = sizeof(crossgameprotocal::CrossGameSyncCrossRandActivityData);

	if (NULL == data || length <= 0 || length >= (crossgameprotocal::MAX_CROSSGAMEPROTOCAL_LENGTH - header_len))
	{
		return;
	}

	static char send_buff[crossgameprotocal::MAX_CROSSGAMEPROTOCAL_LENGTH];

	crossgameprotocal::CrossGameSyncCrossRandActivityData *cgsas = (crossgameprotocal::CrossGameSyncCrossRandActivityData*)send_buff;
	*cgsas = crossgameprotocal::CrossGameSyncCrossRandActivityData();
	cgsas->activity_type = m_activity_type;

	memcpy(send_buff + header_len, data, length);

	if (UINT_MAX == global_netid)
	{
		CrossServer::GetInstCrossServer()->SendToAllGameWorld((const char *)&send_buff, header_len + length);
	}
	else
	{
		InternalComm::Instance().NetSend(global_netid, (const char *)&send_buff, header_len + length);
	}
}

bool CrossRandActivity::ReadNextActivityTime()
{
	// 读取下一段开启结束时间
	if (!m_open_time_list.empty())
	{
		m_begin_time = m_open_time_list.front().begin_timestamp;
		m_end_time = m_open_time_list.front().end_timestamp;
		m_open_time_list.pop();
		return true;
	}

	return false;
}


