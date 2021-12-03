#include "marrymanager.h"

#include "obj/character/role.h"

#include "world.h"
#include "servercommon/errornum.h"

#include "gamelog.h"

MarryManager::MarryManager()
	: m_next_marry_index(0), m_next_marry_start_time(0)
{
	memset(m_last_oper_time_list, 0, sizeof(m_last_oper_time_list));
}

MarryManager::~MarryManager()
{
}

void MarryManager::Init(const MarryReserveParam &marry_reserve_param)
{
	m_marry_param = marry_reserve_param;

	this->CalcNextMarryStartTime();
}

void MarryManager::GetInitParam(MarryReserveParam *marry_reserve_param)
{
	*marry_reserve_param = m_marry_param;
}

void MarryManager::Update(unsigned long interval, time_t now_second)
{
	if (0 != m_next_marry_start_time && now_second >= m_next_marry_start_time && m_marry.IsMarryClose())
	{
		this->OnMarryStart(m_next_marry_index);

		this->CalcNextMarryStartTime();
	}

	m_marry.OnUpdate(interval, now_second);
}

void MarryManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		m_marry_param.Reset();
		m_next_marry_index = 0;
		m_next_marry_start_time = 0;
	}
}

void MarryManager::OnMarryReserveReq(Role *user, int reserve_index, int marry_level, int rune_index)
{
	if (NULL == user)
	{
		return;
	}

	if (reserve_index < 0 || reserve_index >= MARRY_MAX_DAILY_MERRY_NUM) return;
	if (marry_level <= MARRY_LEVEL_TYPE_INVALID || marry_level >= MARRY_LEVEL_TYPE_MAX) return;

	//if (m_global_server->GetActivityManager()->IsMergeForbidTime())
	//{
	//	m_global_server->NoticeToUser(user->GetUserId(), errornum::EN_MARRY_MERGE_FORBID_TIME);
	//	return;
	//}

	// 1: 检查操作锁时间
	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (0 != m_last_oper_time_list[reserve_index] && m_last_oper_time_list[reserve_index] + 2 > now_second)
	{
		return;
	}
	m_last_oper_time_list[reserve_index] = now_second;

	// 2: 当前时间是否允许预约 
	/*if (!GLOBAL_CONFIG->GetOtherConfig()->CanReserveThisMarry(reserve_index, marry_level))
	{
		user->NoticeNum(errornum::EN_MARRY_THIS_TIME_CANNOT_RESERVE);
		return;
	}*/

	// 3: 检查是否被预定
	MarryReserve &reserve_marry = m_marry_param.reserve_list[reserve_index];
	if (!reserve_marry.Invalid())
	{
		user->NoticeNum(errornum::EN_MARRY_THIS_TIME_IS_ALREADY_RESERVE);
		return;
	}

	/*ggprotocal::MarryReserveReq mrr;
	mrr.reserve_index = reserve_index;
	mrr.marry_level = marry_level;
	mrr.rune_index = rune_index;
	GlobalServer::GetInstGlobalServer()->SendToUserGameWorld(user->GetUserId(), (const char *)&mrr, sizeof(mrr));*/
}

void MarryManager::OnMarryReserveAck(gglobalprotocal::MarryReserveAck *mra)
{
	if (mra->marry_reserve_index < 0 || mra->marry_reserve_index >= MARRY_MAX_DAILY_MERRY_NUM) return;
	if (mra->marry_level <= MARRY_LEVEL_TYPE_INVALID || mra->marry_level >= MARRY_LEVEL_TYPE_MAX) return;

	MarryReserve &reserve_marry = m_marry_param.reserve_list[mra->marry_reserve_index];
	if (!reserve_marry.Invalid())
	{
		return;
	}

	memcpy(reserve_marry.uid_list, mra->uid_list, sizeof(reserve_marry.uid_list));
	memcpy(reserve_marry.user_name_list, mra->user_name_list, sizeof(reserve_marry.user_name_list));
	memcpy(reserve_marry.camp_list, mra->camp_list, sizeof(reserve_marry.camp_list));
	reserve_marry.marry_level = static_cast<char>(mra->marry_level);

	for (int i = 0; i < MARRY_COUPLE_USER_LIMIT_NUM; i++)
	{
		Role *user = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(mra->uid_list[i]));
		if (NULL != user)
		{
			this->OnMarryGetReserveInfo(user);
		}
	}

	this->CalcNextMarryStartTime();

	UNSTD_STATIC_CHECK(MARRY_COUPLE_USER_LIMIT_NUM == 2);

	gamelog::g_log_marry.printf(LL_INFO, "Marry -> marry_reserve [ marry_level[%d], user[%d %s], other_user[%d %s] ]", 
		reserve_marry.marry_level, reserve_marry.uid_list[0], reserve_marry.user_name_list[0], reserve_marry.uid_list[1], reserve_marry.user_name_list[1]);
}

void MarryManager::OnMarryHunyanInvite(gglobalprotocal::MarryHunyanInvite *mhi)
{
	m_marry.OnHunyanInviteUser(mhi->invite_uid, mhi->be_invite_uid, mhi->is_free_invite > 0);
}

void MarryManager::OnMarryHunyanEnterReq(Role *user)
{
	m_marry.OnHunyanEnterReq(user);
}

void MarryManager::OnMarryGetReserveInfo(Role *user)
{
	/*static Protocol::SCMarryReserveInfo mri;

	for (int i = 0; i < MARRY_MAX_DAILY_MERRY_NUM; i++)
	{
		memcpy(mri.reserve_info_list[i].uid_list, m_marry_param.reserve_list[i].uid_list, sizeof(mri.reserve_info_list[i].uid_list));
	}

	EngineAdapter::Instance().NetSend(user->GetNetId(), (const char*)&mri, sizeof(mri));*/
}

void MarryManager::OnMarryobjSyncPos(gglobalprotocal::MarryobjSyncPosToGlobal *msptg)
{
	m_marry.OnMarryobjSyncPos(msptg->pos_x, msptg->pos_y);
}

//void MarryManager::OnUserResetName(const UserID &user_id, GameName name)
//{
//	for (int i = 0; i < MARRY_MAX_DAILY_MERRY_NUM; i++)
//	{
//		if (!m_marry_param.reserve_list[i].Invalid())
//		{
//			for (int j = 0; j < MARRY_COUPLE_USER_LIMIT_NUM; j++)
//			{
//				if (m_marry_param.reserve_list[i].uid_list[j] == UidToInt(user_id))
//				{
//					F_STRNCPY(m_marry_param.reserve_list[i].user_name_list[j], name, sizeof(GameName));
//					return;
//				}
//			}
//		}
//	}
//}

void MarryManager::OnUserLogin(Role *user)
{
	m_marry.OnUserLogin(user);
}

void MarryManager::OnGmForceMarryStart()
{
	if (0 != m_next_marry_start_time && m_marry.IsMarryClose())
	{
		m_next_marry_start_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	}
}

void MarryManager::OnGmForceMaryyToNextStatus()
{
	if (!m_marry.IsMarryClose())
	{
		m_marry.OnGmForceToNextStatus();
	}
}

void MarryManager::OnMarryStart(int marry_index)
{
	if (marry_index < 0 || marry_index >= MARRY_MAX_DAILY_MERRY_NUM) return;
	
	MarryReserve &reserve_marry = m_marry_param.reserve_list[marry_index];
	if (!reserve_marry.Invalid())
	{
		++ m_marry_param.total_marry_count;
		m_marry.OnMarryStart(reserve_marry, m_marry_param.total_marry_count, m_next_marry_start_time);
		reserve_marry.is_already_start = 1;

		UNSTD_STATIC_CHECK(MARRY_COUPLE_USER_LIMIT_NUM == 2);

		gamelog::g_log_marry.printf(LL_INFO, "Marry -> marry_start [ marry_level[%d], user[%d %s], other_user[%d %s] ]", 
			reserve_marry.marry_level, reserve_marry.uid_list[0], reserve_marry.user_name_list[0], reserve_marry.uid_list[1], reserve_marry.user_name_list[1]);
	}
}

void MarryManager::OnMarryStop()
{

}

void MarryManager::CalcNextMarryStartTime()
{
	//const tm *local_time = EngineAdapter::Instance().LocalTime();
	//unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	//m_next_marry_index = 0; m_next_marry_start_time = 0;
	//int min_hour_interval = 24;
	//for (int i = 0; i < MARRY_MAX_DAILY_MERRY_NUM; i++)
	//{
	//	MarryReserve &reserve_marry = m_marry_param.reserve_list[i];
	//	if (!reserve_marry.Invalid() && !reserve_marry.IsAlreadyStart())
	//	{
	//		int start_hour = GLOBAL_CONFIG->GetOtherConfig()->GetReserveMarryStartHour(i);
	//		if (local_time->tm_hour < start_hour && start_hour - local_time->tm_hour < min_hour_interval)
	//		{
	//			min_hour_interval = start_hour - local_time->tm_hour;
	//			m_next_marry_index = i;
	//			m_next_marry_start_time = now + EngineAdapter::Instance().NextDayInterval(start_hour, 0, 0);

	//			// 婚礼准备阶段比正式整点开始要提前
	//			m_next_marry_start_time -= GLOBAL_CONFIG->GetOtherConfig()->GetMarryStatusDurTimeS(MARRY_STATUS_ZHUN_BEI);
	//		}
	//	}
	//}
}

