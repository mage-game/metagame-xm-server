#include "randactivity.hpp"
#include "protocal/msgother.h"
#include "engineadapter.h"
#include "internalcomm.h"
#include "servercommon/string/globalstr.h"
#include "global/randactivity/randactivitymanager.hpp"
#include "globalconfig/globalconfig.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/activitydef.hpp"
#include "protocal/msgactivity.hpp"
#include "obj/character/role.h"
#include "world.h"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "config/activityconfig/randactivityopencfg.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/rankdef.hpp"
#include "global/rank/rankmanager.hpp"
#include "global/rank/topranker.hpp"

RandActivity::RandActivity(RandActivityManager *rand_activity_manager, int activity_type)
	: m_rand_activity_manager(rand_activity_manager), m_rand_activity_type(activity_type), m_rand_activity_status(ACTIVITY_STATUS_MAX), 
	m_begin_time(0), m_end_time(0), m_open_type(RAND_ACTIVITY_OPEN_TYPE_BEGIN), m_last_notice_time(0)
{

}

RandActivity::~RandActivity()
{

}

void RandActivity::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	this->SyncActivityStatusToGameworld();
	this->NotfiyRandActivityStatus(true);

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnSpecialActivityStatusChange::activity_type %d, from_status %d, to_status %d, begin %u, end %u", 
		m_rand_activity_type, from_status, to_status, m_begin_time, m_end_time);
}

void RandActivity::Init(const RandActivityData &data)
{
	m_begin_time = data.begin_time;
	m_end_time = data.end_time;
	m_open_type = data.open_type;
}

void RandActivity::GetInitParam(RandActivityData *data)
{
	data->begin_time = m_begin_time;
	data->end_time = m_end_time;
	data->open_type = static_cast<char>(m_open_type);
}

void RandActivity::Update(unsigned long interval, time_t now_second)
{
	if (0 == m_begin_time || 0 == m_end_time) 
	{
		if (ACTIVITY_STATUS_MAX == m_rand_activity_status)
		{
			m_rand_activity_status = ACTIVITY_STATUS_CLOSE;
		}

		return;
	}

	unsigned int now = static_cast<unsigned int>(now_second);

	if (ACTIVITY_STATUS_MAX == m_rand_activity_status)
	{
		if (m_begin_time <= now && now < m_end_time)
		{
			m_rand_activity_status = ACTIVITY_STATUS_OPEN;
		}
		else
		{
			m_rand_activity_status = ACTIVITY_STATUS_CLOSE;
		}

		static ActivityStatus ggas;
		ggas.activity_type = (short)m_rand_activity_type;
		ggas.status = m_rand_activity_status;
		ggas.activity_begin_time = m_begin_time;
		ggas.activity_end_time = m_end_time;
		ActivityShadow::Instance().SetActivityStatus(&ggas);
	}

	switch (m_rand_activity_status)
	{
	case ACTIVITY_STATUS_CLOSE:
		{
			if (m_begin_time <= now && now < m_end_time)
			{
				m_rand_activity_status = ACTIVITY_STATUS_OPEN;
				this->OnSpecialActivityStatusChange(ACTIVITY_STATUS_CLOSE, ACTIVITY_STATUS_OPEN);
			}
		}
		break;
	
	case ACTIVITY_STATUS_OPEN:
		{
			if (now < m_begin_time || m_end_time <= now)
			{
				m_rand_activity_status = ACTIVITY_STATUS_CLOSE;
				this->OnSpecialActivityStatusChange(ACTIVITY_STATUS_OPEN, ACTIVITY_STATUS_CLOSE);
			}

			Broadcast(now_second);
		}
		break;
	}
}

void RandActivity::OnUserLogin(Role *user)
{
	if (ACTIVITY_STATUS_CLOSE != m_rand_activity_status)
	{
		this->NotfiyRandActivityStatus(false, user);
	}
}

void RandActivity::CalActivityOpenTime()
{
	if (ACTIVITY_STATUS_OPEN == m_rand_activity_status)
	{
		return;
	}

	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	
	const RandActivityOpenDetail *open_cfg = LOGIC_CONFIG->GetRandActivityOpenCfg().GetOpenCfg(m_rand_activity_type);
	if (NULL != open_cfg)
	{
		if (RAND_ACTIVITY_OPEN_TYPE_NORMAL == open_cfg->open_type)
		{
			if (0 == m_end_time && 0 == m_begin_time)
			{
				this->SetRandActivityTime(open_cfg->begin_timestamp, open_cfg->end_timestamp, open_cfg->open_type);
				this->NotfiyRandActivityStatus(false, NULL);
			}
		}
		else if (RAND_ACTIVITY_OPEN_TYPE_VERSION == open_cfg->open_type)
		{
			this->SetRandActivityTime(open_cfg->begin_timestamp, open_cfg->end_timestamp, open_cfg->open_type);
			this->NotfiyRandActivityStatus(false, NULL);
		}
	}
}

void RandActivity::SetRandActivityTime(unsigned int begin_time, unsigned int end_time, int open_type)
{
	m_begin_time = begin_time;
	m_end_time = end_time;
	m_open_type = open_type;

	static ActivityStatus ggas;
	ggas.activity_type = (short)m_rand_activity_type;
	ggas.status = m_rand_activity_status;
	ggas.activity_begin_time = m_begin_time;
	ggas.activity_end_time = m_end_time;
	ActivityShadow::Instance().SetActivityStatus(&ggas);

	gamelog::g_log_rand_activity.printf(LL_INFO, "SetRandActivityTime::activity_type %d, begin %u, end %u type %d", m_rand_activity_type, m_begin_time, m_end_time, m_open_type);
}

void RandActivity::OnForceToNextState()
{
	if (ACTIVITY_STATUS_OPEN == m_rand_activity_status)
	{
		m_end_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	}
	else
	{
		int duration_day = 0;

		const RandActivityCfg *cfg = GLOBALCONFIG->GetDynamicConfig().GetRandActivityCfg(m_rand_activity_type);
		if (NULL != cfg && cfg->duration_day > 0)
		{
			duration_day = cfg->duration_day;
		}
		else
		{
			duration_day = 7; // 配置没有 默认开启7天
		}

		m_begin_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		m_end_time = m_begin_time + EngineAdapter::Instance().NextDayInterval(0, 0, 0) + (duration_day - 1) * 24 * 3600;
	}
}

void RandActivity::SyncActivityStatusToGameworld()
{
	static ActivityStatus ggas;
	ggas.activity_type = (short)m_rand_activity_type;
	ggas.status = m_rand_activity_status;
	ggas.activity_begin_time = m_begin_time;
	ggas.activity_end_time = m_end_time;
	World::GetInstWorld()->GetSceneManager()->OnSyncActivityStatus(&ggas);
}

void RandActivity::NotfiyRandActivityStatus(bool is_broadcast, Role *user)
{
	static Protocol::SCActivityStatus as;
	as.activity_type = (short)m_rand_activity_type;
	as.status = static_cast<char>(m_rand_activity_status);
	as.is_broadcast = is_broadcast ? 1 : 0;
	as.next_status_switch_time = (ACTIVITY_STATUS_OPEN == m_rand_activity_status ? m_end_time : m_begin_time);
	as.param_1 = m_begin_time;
	as.param_2 = m_end_time;
	as.open_type = m_open_type;

	if (NULL != user)
	{
		EngineAdapter::Instance().NetSend(user->GetNetId(), (const char*)&as, sizeof(as));
	}
	else 
	{
		World::GetInstWorld()->SendToAllGateway((const char *)&as, sizeof(as));
	}
}

void RandActivity::Broadcast(time_t now_second)
{
	int rank_type = -1;
	int person_rank_type = -1;
	switch(m_rand_activity_type)
	{
	case RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_RANK:
		{
			person_rank_type = PERSON_RANK_TYPE_MOUNT;
			rank_type = RANK_TOP_TYPE_MOUNT;
		}
		break;

	case RAND_ACTIVITY_TYPE_UPGRADE_HALO_RANK:
		{
			person_rank_type = PERSON_RANK_TYPE_HALO;
			rank_type = RANK_TOP_TYPE_HALO;
		}
		break;

	case RAND_ACTIVITY_TYPE_UPGRADE_WING_RANK:
		{
			person_rank_type = PERSON_RANK_TYPE_WING;
			rank_type = RANK_TOP_TYPE_WING;
		}
		break;

	case RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_RANK:
		{
			person_rank_type = PERSON_RANK_TYPE_SHENGONG;
			rank_type = RANK_TOP_TYPE_SHENGONG;
		}
		break;

	case RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_RANK:
		{
			person_rank_type = PERSON_RANK_TYPE_SHENYI;
			rank_type = RANK_TOP_TYPE_SHENYI;
		}
		break;

	case RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL_RANK:
		{
			person_rank_type = PERSON_RANK_TYPE_EQUIP_STRENGTH_LEVEL;
			rank_type = RANK_TOP_TYPE_EQUIPT_STRENGTH_LEVEL;
		}
		break;

	case RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL_RANK:
		{
			person_rank_type = PERSON_RANK_TYPE_STONE_TOATAL_LEVEL;
			rank_type = RANK_TOP_TYPE_STONE_TOTAL_LEVEL;
		}
		break;

	case RAND_ACTIVITY_TYPE_XIANNV_RANK:
		{
			person_rank_type = PERSON_RANK_TYPE_XIANNV_CAPABILITY;
			rank_type = RANK_TOP_TYPE_XIANNV;
		}
		break;

	case RAND_ACTIVITY_TYPE_JINGLING_RANK:
		{
			person_rank_type = PERSON_RANK_TYPE_CAPABILITY_JINGLING;
			rank_type = RANK_TOP_TYPE_JINGLING;
		}
		break;

	case RAND_ACTIVITY_TYPE_FIGHT_MOUNT_RANK:
		{
			person_rank_type = PERSON_RANK_TYPE_FIGHT_MOUNT;
			rank_type = RANK_TOP_TYPE_FIGHT_MOUNT;
		}
		break;

	case RAND_ACTIVITY_TYPE_UPGRADE_QILINBI_RANK:
	{
		person_rank_type = PERSON_RANK_TYPE_QILINBI;
		rank_type = RANK_TOP_TYPE_QILINBI;
	}
	break;

	case RAND_ACTIVITY_TYPE_UPGRADE_TOUSHI_RANK:
	{
		person_rank_type = PERSON_RANK_TYPE_TOUSHI;
		rank_type = RANK_TOP_TYPE_TOUSHI;
	}
	break;

	case RAND_ACTIVITY_TYPE_UPGRADE_YAOSHI_RANK:
	{
		person_rank_type = PERSON_RANK_TYPE_YAOSHI;
		rank_type = RANK_TOP_TYPE_YAOSHI;
	}
	break;

	case RAND_ACTIVITY_TYPE_UPGRADE_LINGTONG_RANK:
	{
		person_rank_type = PERSON_RANK_TYPE_LINGTONG;
		rank_type = RANK_TOP_TYPE_LINGTONG;
	}
	break;

	case RAND_ACTIVITY_TYPE_UPGRADE_LINGGONG_RANK:
	{
		person_rank_type = PERSON_RANK_TYPE_LINGGONG;
		rank_type = RANK_TOP_TYPE_LINGGONG;
	}
	break;

	case RAND_ACTIVITY_TYPE_UPGRADE_LINGQI_RANK:
	{
		person_rank_type = PERSON_RANK_TYPE_LINGQI;
		rank_type = RANK_TOP_TYPE_LINGQI;
	}
	break;

	case RAND_ACTIVITY_TYPE_UPGRADE_FLYPET_RANK:
	{
		person_rank_type = PERSON_RANK_TYPE_FEICHONG;
		rank_type = RANK_TOP_TYPE_FlYPET;
	}
	break;

	case RAND_ACTIVITY_TYPE_UPGRADE_WEIYAN_RANK:
	{
		person_rank_type = PERSON_RANK_TYPE_WEIYAN;
		rank_type = RANK_TOP_TYPE_WEIYAN;
	}
	break;

	case RAND_ACTIVITY_TYPE_TOTAL_CAP_RANK:
		{
			person_rank_type = PERSON_RANK_TYPE_CAPABILITY_ALL;
			rank_type = RANK_TOP_TYPE_MIN;
		}
		break;
	}

	if (person_rank_type < 0 || rank_type < 0)
	{
		return;
	}

	PersonRank *person_rank = RankManager::Instance().GetPersonRank();
	if (NULL == person_rank)
	{
		return;
	}

	{
		unsigned int now_sec = static_cast<unsigned int>(now_second);
		if (m_last_notice_time == 0)
		{
			m_last_notice_time = now_sec;
		}

		// 传闻
		static const unsigned int BIPIN_CAP_INTERVAL = 1800;
		if (now_sec >= m_last_notice_time + BIPIN_CAP_INTERVAL)
		{
			/*int uid_first = TopRanker::Instance().GetTopRankUid(rank_type, 0);
			int uid_second = TopRanker::Instance().GetTopRankUid(rank_type, 1);

			if (uid_first < 0 || uid_second < 0)
			{
				return;
			}

			int length = 0;
			const PersonRankItem *rank_item_first = person_rank->GetPersonRankItem(person_rank_type, IntToUid(uid_first));
			if (NULL != rank_item_first)
			{
				const PersonRankItem *rank_item_second = person_rank->GetPersonRankItem(person_rank_type, IntToUid(uid_second));
				if (NULL != rank_item_second)
				{
					length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_bipin_two,
						m_rand_activity_type, UidToInt(rank_item_first->user_id), rank_item_first->user_name, rank_item_first->camp,
						UidToInt(rank_item_second->user_id), rank_item_second->user_name, rank_item_second->camp);
				}
				else
				{
					length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_bipin,
						m_rand_activity_type, UidToInt(rank_item_first->user_id), rank_item_first->user_name, rank_item_first->camp);
				}
			}*/

			const int rank_count = 2;
			const int rank_index_0 = 0;
			const int rank_index_1 = 1;
			PersonRankItem rank_item_list[rank_count];
			UNSTD_STATIC_CHECK(rank_index_0 >= 0 && rank_index_0 < rank_count && rank_index_1 >= 0 && rank_index_1 < rank_count);
			int real_rank_count = person_rank->GetSnapShotTopItem(rank_type, rank_count, rank_item_list);
			int length = 0;
			if (1 == real_rank_count)
			{
				length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_bipin,
					m_rand_activity_type, UidToInt(rank_item_list[rank_index_0].user_id), rank_item_list[rank_index_0].user_name, rank_item_list[rank_index_0].camp);
			}
			else if (2 == real_rank_count)
			{
				length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_bipin_two,
					m_rand_activity_type, UidToInt(rank_item_list[rank_index_0].user_id), rank_item_list[rank_index_0].user_name, rank_item_list[rank_index_0].camp,
					UidToInt(rank_item_list[rank_index_1].user_id), rank_item_list[rank_index_1].user_name, rank_item_list[rank_index_1].camp);
			}

			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
			}

			m_last_notice_time = now_sec;
		}
	}
}
