#include "randactivityholidayguard.hpp"
#include <algorithm>
#include "config/logicconfigmanager.hpp"
#include "item/itempool.h"
#include "other/route/mailroute.hpp"
#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "other/capability/capability.hpp"
#include "other/fb/holidayguardconfig.hpp"
#include "other/fb/rolefbtowerdefend.hpp"
#include "obj/character/role.h"
#include "gamecommon.h"

RandActivityHolidayGuard::RandActivityHolidayGuard(RandActivityManager *rand_activity_manager)
	: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_HOLIDAY_GUARD), m_cur_rank_num(0), m_send_rank_info_interval(0), m_can_enter_fb_begin_timestamp(0), m_can_enter_fb_end_timestamp(0), 
	m_refresh_interval_timestamp(0), m_left_npc_num(0), m_is_need_refresh(0), m_fixed_point_refresh_npc_timestamp(0)
{

}

RandActivityHolidayGuard::~RandActivityHolidayGuard()
{

}

void RandActivityHolidayGuard::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	UNSTD_STATIC_CHECK(sizeof(m_rank_param_list) <= sizeof(data.data));

	memcpy(m_rank_param_list, ((KillMonsterRank *)data.data), sizeof(m_rank_param_list));

	for (int i = 0; i < RAND_ACTIVITY_HOLIDAYGUARD_PERSON_RANK_MAX; ++i)
	{
		if (m_rank_param_list[i].uid > 0)
		{
			++m_cur_rank_num;
		}
	}
}

void RandActivityHolidayGuard::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_rank_param_list) <= sizeof(data->data));

	memcpy(data->data, m_rank_param_list, sizeof(m_rank_param_list));
}

static bool RankSortFunc(const KillMonsterRank &item1, const KillMonsterRank &item2)
{
	if (item1.kill_monster_count == item2.kill_monster_count)
	{
		return item1.meet_requirement_timestamp < item2.meet_requirement_timestamp;
	}

	return item1.kill_monster_count > item2.kill_monster_count;
}

static bool RankSortFunc2(const KillRankNoticeInfo &item1, const KillRankNoticeInfo &item2)
{
	if (item1.kill_monster_count == item2.kill_monster_count)
	{
		return item1.pass_time < item2.pass_time;
	}

	return item1.kill_monster_count > item2.kill_monster_count;
}

void RandActivityHolidayGuard::OnSpecialActivityStatusChange(int from_status, int to_status)
{

	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		this->SetNpcRefreshTime();

		memset(m_person_kill_notice_list, 0, sizeof(m_person_kill_notice_list));
		memset(m_rank_param_list, 0, sizeof(m_rank_param_list));
		m_cur_rank_num = 0;
	}


	// 活动结束，邮件发放排行榜奖励
	if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		this->DelAllNpc();
	}

	this->SendRankInfo();
}

void RandActivityHolidayGuard::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	RandActivity::OnDayChange(old_dayid, now_dayid);

	this->SetNpcRefreshTime();
}

void RandActivityHolidayGuard::Update(unsigned long interval, time_t now_second)
{
	RandActivity::Update(interval, now_second);

	if (m_rand_activity_status != ACTIVITY_STATUS_OPEN) return;

	if (m_can_enter_fb_begin_timestamp == 0 || m_can_enter_fb_end_timestamp == 0)
	{
		this->SetNpcRefreshTime();
	}

	if (now_second > m_can_enter_fb_begin_timestamp && now_second < m_can_enter_fb_end_timestamp)
	{
		if (m_refresh_interval_timestamp != 0 && now_second >= m_refresh_interval_timestamp)
		{
			this->OnRefreshNpc();
			m_fixed_point_refresh_npc_timestamp = (unsigned int)now_second;
			m_is_need_refresh = 0;
			m_refresh_interval_timestamp = (unsigned int)now_second + LOGIC_CONFIG->GetHolidayGuardConfig().GetNpcRefreshTime();
		}
	}

	if (now_second >= m_can_enter_fb_end_timestamp && m_refresh_interval_timestamp != 0)
	{
		this->DelAllNpc();
		m_refresh_interval_timestamp = 0;
	}
}

void RandActivityHolidayGuard::OnUserLogin(Role* user)
{
	if (m_rand_activity_status != ACTIVITY_STATUS_OPEN) return;

	RandActivity::OnUserLogin(user);

	this->SetRankInfo(user);
	this->SendNpcINfo(user);
}

void RandActivityHolidayGuard::AddOrReplaceRole(Role *role)
{
	if (nullptr == role)
	{
		return;
	}

	RoleFBTowerDefend *tower_defend = role->GetFBTowerDefend();
	if (nullptr == tower_defend)
	{
		return;
	}

	std::sort(m_person_kill_notice_list, m_person_kill_notice_list + RAND_ACTIVITY_HOLIDAYGUARD_PERSON_RANK_MAX, RankSortFunc2);

	int kill_count = tower_defend->GetPersonalHolidayGuardFBKillMonsterCount();
	unsigned int meet_requirement_time = (unsigned int)EngineAdapter::Instance().Time();

	for (int i = 0; i < RAND_ACTIVITY_HOLIDAYGUARD_PERSON_RANK_MAX; ++i)
	{
		if (m_person_kill_notice_list[i].uid == role->GetUID())
		{
			m_person_kill_notice_list[i].kill_monster_count = kill_count;
			m_person_kill_notice_list[i].pass_time = meet_requirement_time;

			this->SyncRankInfo();
			return;
		}
	}

	if (kill_count > m_person_kill_notice_list[RAND_ACTIVITY_HOLIDAYGUARD_PERSON_RANK_MAX - 1].kill_monster_count)
	{
		this->UpdateRankInfo(role, kill_count, meet_requirement_time);
	}
	else if (kill_count == m_person_kill_notice_list[RAND_ACTIVITY_HOLIDAYGUARD_PERSON_RANK_MAX - 1].kill_monster_count &&
		meet_requirement_time < m_person_kill_notice_list[RAND_ACTIVITY_HOLIDAYGUARD_PERSON_RANK_MAX - 1].pass_time)
	{
		this->UpdateRankInfo(role, kill_count, meet_requirement_time);
	}

	this->SyncRankInfo();
}

void RandActivityHolidayGuard::SendRankInfo(Role *role)
{
	static Protocol::SCRAHolidayGuardRanKInfo schgri;

	for(int i = 0; i < RAND_ACTIVITY_HOLIDAYGUARD_PERSON_RANK_MAX; ++i)
	{
		schgri.kill_rank[i] = m_person_kill_notice_list[i];
	}
	
	if (role != nullptr)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&schgri, sizeof(schgri));
	}
	else
	{
		SEND_TO_ALLGATEWAY(schgri);
	}
}

void RandActivityHolidayGuard::SendNpcINfo(Role * role)
{
	int npc_count = LOGIC_CONFIG->GetHolidayGuardConfig().GetNpcNeedRefreshCount();

	static Protocol::SCRAExtremeChallengeNpcInfo scraecni;

	for (int i = 0; i < npc_count && i < HOLIDAYGUARD_NPC_CFG_MAX_COUNT; ++i)
	{
		scraecni.npc_info_list[i].npc_index = m_npc_info_list[i].index;
		scraecni.npc_info_list[i].npc_id = m_npc_info_list[i].npc_id;
		scraecni.npc_info_list[i].scene_id = m_npc_info_list[i].scene_id;
		scraecni.npc_info_list[i].npc_x = m_npc_info_list[i].coordinate_x;
		scraecni.npc_info_list[i].npc_y = m_npc_info_list[i].coordinate_y;
	}

	if (nullptr != role)
	{
		SEND_TO_ROLE(role, scraecni);
	}
	else
	{
		SEND_TO_ALLGATEWAY(scraecni);
	}
}

void RandActivityHolidayGuard::DelNpc(int index)
{
	int npc_count = LOGIC_CONFIG->GetHolidayGuardConfig().GetNpcNeedRefreshCount();

	for (int i = 0; i < npc_count && i < HOLIDAYGUARD_NPC_CFG_MAX_COUNT; ++i)
	{
		if (m_npc_info_list[i].index == index && m_npc_info_list[i].npc_id > 0)
		{
			m_npc_info_list[i].index = 0;
			m_npc_info_list[i].npc_id = 0;
			m_npc_info_list[i].scene_id = 0;
			m_npc_info_list[i].coordinate_x = 0;
			m_npc_info_list[i].coordinate_y = 0;
			
			--m_left_npc_num;
			break;
		}
	}

	static Protocol::SCRAExtremeChallengeNpcInfo scraecni;
	for (int i = 0; i < HOLIDAYGUARD_NPC_CFG_MAX_COUNT; ++i)
	{
		scraecni.npc_info_list[i].npc_index = m_npc_info_list[i].index;
		scraecni.npc_info_list[i].npc_id = m_npc_info_list[i].npc_id;
		scraecni.npc_info_list[i].scene_id = m_npc_info_list[i].scene_id;
		scraecni.npc_info_list[i].npc_x = m_npc_info_list[i].coordinate_x;
		scraecni.npc_info_list[i].npc_y = m_npc_info_list[i].coordinate_y;
	}

	if (m_left_npc_num == 0)
	{
		if (EngineAdapter::Instance().Time() - m_fixed_point_refresh_npc_timestamp < LOGIC_CONFIG->GetHolidayGuardConfig().GetTimeWithinTreadFinishRefreshTime() && 
			m_is_need_refresh == 0)
		{
			this->OnRefreshNpc();
			m_is_need_refresh = 1;
			return;
		}
	}

	SEND_TO_ALLGATEWAY(scraecni);
}

void RandActivityHolidayGuard::DelAllNpc()
{
	memset(m_npc_info_list, 0, sizeof(m_npc_info_list));
	m_left_npc_num = 0;

	this->SendNpcINfo(nullptr);
}

 int RandActivityHolidayGuard::GetRankListInfo(UID role_uid, int &kill_monster_count)
{
	for (int i = 0; i < RAND_ACTIVITY_HOLIDAYGUARD_PERSON_RANK_MAX; ++i)
	{
		if (role_uid == m_rank_param_list[i].uid)
		{
			kill_monster_count = m_rank_param_list[i].kill_monster_count;
			return i;
		}
	}

	return -1;
}

void RandActivityHolidayGuard::SetRankInfo(Role *role)
{
	if(nullptr == role)
	{
		return;
	}

	for(int i = 0; i < RAND_ACTIVITY_HOLIDAYGUARD_PERSON_RANK_MAX; ++i)
	{
		if (m_rank_param_list[i].uid == role->GetUID())
		{
			this->AddOrReplaceRole(role);
		}
	}
}

void RandActivityHolidayGuard::OnRefreshNpc()
{
	int npc_cfg_count = 0;
	static HolidayGuardNpcRefreshCfg temp_npc_cfg_list[HOLIDAYGUARD_NPC_CFG_MAX_COUNT];
	npc_cfg_count = LOGIC_CONFIG->GetHolidayGuardConfig().GetNpcCfg(temp_npc_cfg_list, HOLIDAYGUARD_NPC_CFG_MAX_COUNT);
	if (npc_cfg_count <= 0)
	{
		return;
	}
	std::random_shuffle(temp_npc_cfg_list, temp_npc_cfg_list + npc_cfg_count);

	m_left_npc_num = 0;
	auto scene_npc_num_limit_map = LOGIC_CONFIG->GetHolidayGuardConfig().GetSceneNpcCountMap();		// 限制npc刷新
	int need_npc_count = LOGIC_CONFIG->GetHolidayGuardConfig().GetNpcNeedRefreshCount();			// 需要刷新npc数
	std::map<int, int> scene_npc_num_map;

	for (int i = 0; i < npc_cfg_count && i < HOLIDAYGUARD_NPC_CFG_MAX_COUNT; ++i)
	{
		if (m_left_npc_num >= need_npc_count || m_left_npc_num >= npc_cfg_count) break;

		HolidayGuardNpcRefreshCfg &npc_cfg = temp_npc_cfg_list[i];

		if (scene_npc_num_map[npc_cfg.scene_id] >= scene_npc_num_limit_map[npc_cfg.scene_id])
		{
			continue;
		}

		m_npc_info_list[m_left_npc_num].index = npc_cfg.index;
		m_npc_info_list[m_left_npc_num].npc_id = npc_cfg.npc_id;
		m_npc_info_list[m_left_npc_num].scene_id = npc_cfg.scene_id;
		m_npc_info_list[m_left_npc_num].coordinate_x = npc_cfg.coordinate_x;
		m_npc_info_list[m_left_npc_num].coordinate_y = npc_cfg.coordinate_y;
		++m_left_npc_num;
		++scene_npc_num_map[npc_cfg.scene_id];
	}

	static Protocol::SCRAExtremeChallengeNpcInfo raecni;
	memset(raecni.npc_info_list, 0, sizeof(raecni.npc_info_list));

	for (int i = 0; i < m_left_npc_num && i < HOLIDAYGUARD_NPC_CFG_MAX_COUNT; ++i)
	{
		raecni.npc_info_list[i].npc_index = m_npc_info_list[i].index;
		raecni.npc_info_list[i].npc_id = m_npc_info_list[i].npc_id;
		raecni.npc_info_list[i].scene_id = m_npc_info_list[i].scene_id;
		raecni.npc_info_list[i].npc_x = m_npc_info_list[i].coordinate_x;
		raecni.npc_info_list[i].npc_y = m_npc_info_list[i].coordinate_y;
	}

	SEND_TO_ALLGATEWAY(raecni);
}

void RandActivityHolidayGuard::SetNpcRefreshTime()
{
	unsigned int time = (unsigned int)EngineAdapter::Instance().Time();

	unsigned int zero_time = (unsigned int)GetZeroTime(time);
	m_refresh_interval_timestamp = time;

	unsigned int begin_time = 0, end_time = 0;
	if (LOGIC_CONFIG->GetHolidayGuardConfig().GetFBBeginTimeAndEndTime(&begin_time, &end_time))
	{
		m_can_enter_fb_begin_timestamp = (begin_time + zero_time);
		m_can_enter_fb_end_timestamp = (end_time + zero_time);
	}
}

void RandActivityHolidayGuard::UpdateRankInfo(Role * role, int kill_count, int meet_requirement_timestamp)
{	
	int index = RAND_ACTIVITY_HOLIDAYGUARD_PERSON_RANK_MAX - 1;

	m_person_kill_notice_list[index].uid = role->GetUID();
	m_person_kill_notice_list[index].kill_monster_count = kill_count;
	m_person_kill_notice_list[index].pass_time = meet_requirement_timestamp;
	m_person_kill_notice_list[index].prof = role->GetProf();
	m_person_kill_notice_list[index].sex = role->GetSex();
	memcpy(m_person_kill_notice_list[index].user_name, role->GetName(), sizeof(m_person_kill_notice_list[index].user_name));
	m_person_kill_notice_list[index].total_capablity = role->GetCapability()->GetTotalCapability();

	this->SyncRankInfo();
}

void RandActivityHolidayGuard::SyncRankInfo()
{
	memset(m_rank_param_list, 0, sizeof(m_rank_param_list));

	for (int index = 0; index < RAND_ACTIVITY_HOLIDAYGUARD_PERSON_RANK_MAX; ++index)
	{
		m_rank_param_list[index].SetData(m_person_kill_notice_list[index]);
	}

	std::sort(m_rank_param_list, m_rank_param_list + RAND_ACTIVITY_HOLIDAYGUARD_PERSON_RANK_MAX, RankSortFunc);
}
