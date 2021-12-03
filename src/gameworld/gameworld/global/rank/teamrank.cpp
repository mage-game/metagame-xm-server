#include "teamrank.hpp"

#include "rankmanager.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/struct/structcommon.h"

#include "obj/character/role.h"
#include "gamelog.h"
#include "engineadapter.h"

TeamRank::TeamRank()
	: m_rank_manager(NULL), m_change_flag(false), m_load_flag(false), m_last_snapshot_time(0)
{
	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));
	memset(m_old_state, 0, sizeof(m_old_state));
}

TeamRank::~TeamRank()
{
}

void TeamRank::Init(const TeamRankParam &teamrank_param)
{
	int count = 0;

	if (teamrank_param.count < TEAM_RANK_TYPE_MAX * TEAM_RANK_MAX_SIZE)
	{
		count = teamrank_param.count;
	}
	else 
	{
		count = TEAM_RANK_TYPE_MAX * TEAM_RANK_MAX_SIZE;
	}
	
	for (int i = count - 1; i >= 0; i--)
	{
		int rank_type = teamrank_param.team_rank_list[i].rank_type;
		if (rank_type < 0 || rank_type >= TEAM_RANK_TYPE_MAX) 
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[TeamRank::Init INVALID RANK TYPE] [rank_type:%d]", (int)rank_type);
			continue;
		}

		int rank_index = teamrank_param.team_rank_list[i].index;
		if (rank_index < 0 || rank_index >= TEAM_RANK_TYPE_MAX * TEAM_RANK_MAX_SIZE) 
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[TeamRank::Init INVALID RANK INDEX] [rank_index:%d]", (int)rank_index);
			continue;
		}

		if (teamrank_param.team_rank_list[i].member_list[0].uid <= 0)		// 第一个队员必须有效
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[TeamRank::Init INVALID USER INFO] [user[%d %s]]", 
				(int)teamrank_param.team_rank_list[i].member_list[0].uid, (const char*)teamrank_param.team_rank_list[i].member_list[0].user_name);
			continue;
		}

		bool is_member_list_error = false;
		for (int j = 1; j < MAX_TEAM_MEMBER_NUM; ++ j)						// 队员按ID从大到小排列
		{
			if (teamrank_param.team_rank_list[i].member_list[j].uid > teamrank_param.team_rank_list[i].member_list[j - 1].uid || teamrank_param.team_rank_list[i].member_list[j].uid < 0)
			{
				is_member_list_error = true;
				gamelog::g_log_serious_error.printf(LL_CRITICAL, "[TeamRank::Init INVALID USER INFO] [user[%d %s]]", 
					(int)teamrank_param.team_rank_list[i].member_list[j].uid, (const char*)teamrank_param.team_rank_list[i].member_list[j].user_name);
				break;
			}
		}
		if (is_member_list_error)
		{
			continue;
		}

		if (!m_rank_list[rank_index].Invalid()) 
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[TeamRank::Init RANK ITEM REPEAT] [rank_index:%d]", (int)rank_index);
			continue;
		}
		
		for (int j = 0; j < MAX_TEAM_MEMBER_NUM; ++ j)
		{
			m_rank_list[rank_index].member_list[j].uid = teamrank_param.team_rank_list[i].member_list[j].uid;
			F_STRNCPY(m_rank_list[rank_index].member_list[j].user_name, teamrank_param.team_rank_list[i].member_list[j].user_name, sizeof(m_rank_list[0].member_list[0].user_name));
			m_rank_list[rank_index].member_list[j].camp = teamrank_param.team_rank_list[i].member_list[j].camp;
		}

		m_rank_list[rank_index].rank_type = teamrank_param.team_rank_list[i].rank_type;
		m_rank_list[rank_index].rank_value = teamrank_param.team_rank_list[i].rank_value;
		m_rank_list[rank_index].flexible_int = teamrank_param.team_rank_list[i].flexible_int;
		m_rank_list[rank_index].flexible_ll = teamrank_param.team_rank_list[i].flexible_ll;

		m_old_state[rank_index] = true;
	}

	m_load_flag = true;
}

void TeamRank::Update(time_t now_second)
{
	if (!m_load_flag) return;

	if (now_second > m_last_snapshot_time + TEAM_RANK_SNAPSHOT_INTERVAL)
	{
		this->Snapshot();
		m_last_snapshot_time = now_second;
	}
}

void TeamRank::Snapshot()
{
	memcpy(m_rank_snapshot, m_rank_list, sizeof(m_rank_snapshot));
}

void TeamRank::GetChangeRankItemList(TeamRankParam *teamrank_param)
{
	if (NULL == teamrank_param) return;

	if (!m_change_flag)
	{
		teamrank_param->count = 0;
		return;
	}
	
	int count = 0;
	for (int i = 0; i < TEAM_RANK_TYPE_MAX * TEAM_RANK_MAX_SIZE && count < TEAM_RANK_TYPE_MAX * TEAM_RANK_MAX_SIZE; ++i)
	{
		if (m_dirty_mark[i])
		{
			if (m_rank_list[i].Invalid())
			{
				if (!m_old_state[i]) continue;

				teamrank_param->team_rank_list[count].change_state = structcommon::CS_DELETE;
				teamrank_param->team_rank_list[count].index = i;
			}
			else
			{
				teamrank_param->team_rank_list[count].change_state = m_old_state[i] ? structcommon::CS_UPDATE : structcommon::CS_INSERT;
				teamrank_param->team_rank_list[count].index = i;

				for (int j = 0; j < MAX_TEAM_MEMBER_NUM; ++ j)
				{
					teamrank_param->team_rank_list[count].member_list[j].uid = m_rank_list[i].member_list[j].uid;
					F_STRNCPY(teamrank_param->team_rank_list[count].member_list[j].user_name, m_rank_list[i].member_list[j].user_name, sizeof(teamrank_param->team_rank_list[0].member_list[0].user_name));
					teamrank_param->team_rank_list[count].member_list[j].camp = m_rank_list[i].member_list[j].camp;
				}
				
				teamrank_param->team_rank_list[count].rank_type = m_rank_list[i].rank_type;
				teamrank_param->team_rank_list[count].rank_value = m_rank_list[i].rank_value;
				teamrank_param->team_rank_list[count].flexible_int = m_rank_list[i].flexible_int;
				teamrank_param->team_rank_list[count].flexible_ll = m_rank_list[i].flexible_ll;
			}

			++count;
		}
	}

	teamrank_param->count = count;
}

void TeamRank::ClearDirtyMark()
{
	if (!m_change_flag) return;

	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));

	for (int i = 0; i < TEAM_RANK_TYPE_MAX * TEAM_RANK_MAX_SIZE; ++i)
	{
		m_old_state[i] = !m_rank_list[i].Invalid();
	}

	m_change_flag = false;
}

void TeamRank::ClearRank(int rank_type)
{
	if (rank_type < 0 || rank_type >= TEAM_RANK_TYPE_MAX) return;

	int rank_start = rank_type * TEAM_RANK_MAX_SIZE;
	int rank_end =  rank_start + TEAM_RANK_MAX_SIZE;

	for (int i = rank_start; i < rank_end; ++i)
	{
		m_rank_list[i].Reset();
		m_dirty_mark[i] = true;
	}

	m_change_flag = true;
}

void TeamRank::OnGetRankList(Role *role, Protocol::CSGetTeamRankList &rank_list_req)
{
	if (!m_load_flag) return;
	if (NULL == role) return;

	int rank_type = rank_list_req.rank_type;
	if (rank_type < 0 || rank_type >= TEAM_RANK_TYPE_MAX) return;
	//if (!user.GetUserRegister()->CheckAndUpdateTeamRankGetStamp(rank_type, m_last_snapshot_time)) return;

	static Protocol::SCGetTeamRankListAck rank_list_ack;
	rank_list_ack.rank_type = rank_type;
	rank_list_ack.rank_count = 0;

	int rank_start = rank_type * TEAM_RANK_MAX_SIZE;
	int rank_end =  rank_start + TEAM_RANK_MAX_SIZE;

	int rank_count = 0;

	for (int i = rank_start; i < rank_end && i < TEAM_RANK_MAX_SIZE * TEAM_RANK_TYPE_MAX && rank_count < TEAM_RANK_MAX_SIZE; ++i)
	{
		if (m_rank_snapshot[i].Invalid()) continue;

		for (int j = 0; j < MAX_TEAM_MEMBER_NUM; ++ j)
		{
			rank_list_ack.rank_list[rank_count].member_list[j].uid = m_rank_snapshot[i].member_list[j].uid;
			F_STRNCPY(rank_list_ack.rank_list[rank_count].member_list[j].user_name, m_rank_snapshot[i].member_list[j].user_name, sizeof(rank_list_ack.rank_list[0].member_list[0].user_name));
			rank_list_ack.rank_list[rank_count].member_list[j].camp = m_rank_snapshot[i].member_list[j].camp;
		}

		rank_list_ack.rank_list[rank_count].rank_value = m_rank_snapshot[i].rank_value;
		rank_list_ack.rank_list[rank_count].flexible_int = m_rank_snapshot[i].flexible_int;
		rank_list_ack.rank_list[rank_count].flexible_ll = m_rank_snapshot[i].flexible_ll;

		++ rank_count;
	}

	rank_list_ack.rank_count = rank_count;
	int sendlen = sizeof(rank_list_ack) - (TEAM_RANK_MAX_SIZE - rank_list_ack.rank_count) * sizeof(rank_list_ack.rank_list[0]);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&rank_list_ack, sendlen);
}

static int TeamRankMemberInfoSortCmp(const void *v1, const void *v2)
{
	TeamRankMemberInfo *item1 = (TeamRankMemberInfo*)v1;
	TeamRankMemberInfo *item2 = (TeamRankMemberInfo*)v2;

	if (item1->uid == item2->uid) return 0;

	return (item1->uid > item2->uid ? -1 : 1);
}

void TeamRank::OnRankInfoChange(gglobalprotocal::SyncTeamRankInfo &rank_info)
{
	if (rank_info.rank_type < 0 || rank_info.rank_type >= TEAM_RANK_TYPE_MAX) return;

	qsort(rank_info.member_list, MAX_TEAM_MEMBER_NUM, sizeof(rank_info.member_list[0]), TeamRankMemberInfoSortCmp);		// 队员按ID从大到小排序

	if (rank_info.member_list[0].uid <= 0) return;

	for (int i = 1; i < MAX_TEAM_MEMBER_NUM; ++ i)
	{
		if (rank_info.member_list[i].uid < 0) return;
	}

	static TeamRankItem rank_item;
	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++ i)
	{
		rank_item.member_list[i].uid = rank_info.member_list[i].uid;
		F_STRNCPY(rank_item.member_list[i].user_name, rank_info.member_list[i].user_name, sizeof(rank_item.member_list[0].user_name));
		rank_item.member_list[i].camp = rank_info.member_list[i].camp;
	}
	
	rank_item.rank_type = rank_info.rank_type;
	rank_item.rank_value = rank_info.rank_value;
	rank_item.flexible_int = rank_info.flexible_int;
	rank_item.flexible_ll = rank_info.flexible_ll;

	this->UpdateRankList(rank_item);
}

int TeamRank::TeamRankItemSortCompare(const TeamRankItem &item1, const TeamRankItem &item2)
{
	if (item1.rank_type != item2.rank_type) return (item1.rank_type - item2.rank_type);

	if (item1.rank_value == item2.rank_value) return 0;

	return (item1.rank_value > item2.rank_value ? -1 : 1);		// value大的排在前面
}

bool TeamRank::GetRankItemIndex(int rank_type, int member_uid_list[MAX_TEAM_MEMBER_NUM], int *rank_index)
{
	if (rank_type < 0 || rank_type >= TEAM_RANK_TYPE_MAX) return false;

	int begin_index = rank_type * TEAM_RANK_MAX_SIZE;
	int end_index = (rank_type + 1) * TEAM_RANK_MAX_SIZE;

	for (int i = begin_index; i < end_index; ++ i)
	{
		if (!m_rank_list[i].Invalid())
		{
			bool is_team_match = true;
			for (int j = 0; j < MAX_TEAM_MEMBER_NUM; ++ j)
			{
				if (m_rank_list[i].member_list[j].uid != member_uid_list[j])
				{
					is_team_match = false;
					break;
				}
			}

			if (is_team_match)
			{
				if (NULL != rank_index) *rank_index = i;
				return true;
			}
		}
	}

	return false;
}

int TeamRank::GetRankTypeSize(int rank_type)
{
	if (rank_type < 0 || rank_type >= TEAM_RANK_TYPE_MAX) return false;

	int begin_index = rank_type * TEAM_RANK_MAX_SIZE;
	int end_index = (rank_type + 1) * TEAM_RANK_MAX_SIZE;

	int rank_type_size = 0;
	for (int i = begin_index; i < end_index; ++ i)
	{
		if (!m_rank_list[i].Invalid())
		{
			++ rank_type_size;
		}
	}

	return rank_type_size;
}

void TeamRank::UpdateRankList(TeamRankItem &rank_item)
{
	// 1：先查询是否排行榜上 如果是 直接更新即可 
	// 2：如果不是 则跟排行榜上最后一条记录比较 是否淘汰最后一条记录

	if (rank_item.rank_type < 0 || rank_item.rank_type >= TEAM_RANK_TYPE_MAX) return;

	int find_index = 0;

	int member_uid_list[MAX_TEAM_MEMBER_NUM] = {0};
	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++ i) member_uid_list[i] = rank_item.member_list[i].uid;

	bool is_find = this->GetRankItemIndex(rank_item.rank_type, member_uid_list, &find_index);
	if (is_find && this->IsValidIndex(find_index))
	{
		if (this->TeamRankItemSortCompare(rank_item, m_rank_list[find_index]) >= 0) return;

		m_rank_list[find_index] = rank_item;

		m_change_flag = true;
		m_dirty_mark[find_index] = true;
	}
	else 
	{
		int pos = -1;

		if (this->GetRankTypeSize(rank_item.rank_type) >= TEAM_RANK_MAX_SIZE)
		{
			if (!this->GetSwapRankItemSlot(rank_item.rank_type, rank_item, &pos)) return;
		}
		else 
		{
			if (!this->GetFreeRankItemSlot(rank_item.rank_type, &pos)) return;
		}

		if (pos >= rank_item.rank_type * TEAM_RANK_MAX_SIZE && pos < (rank_item.rank_type + 1) * TEAM_RANK_MAX_SIZE)
		{
			m_rank_list[pos] = rank_item;

			m_change_flag = true;
			m_dirty_mark[pos] = true;
		}
	}
}

bool TeamRank::GetSwapRankItemSlot(int rank_type, const TeamRankItem &rank_item, int *swap_pos)
{
	if (rank_type < 0 || rank_type >= TEAM_RANK_TYPE_MAX) return false;

	int begin_index = rank_type * TEAM_RANK_MAX_SIZE;
	int end_index = (rank_type + 1) * TEAM_RANK_MAX_SIZE;

	int last_pos = begin_index;
	for (int i = begin_index + 1; i < end_index; i++)
	{
		if (this->TeamRankItemSortCompare(m_rank_list[i], m_rank_list[last_pos]) > 0)
		{
			last_pos = i;
		}
	}

	if (this->TeamRankItemSortCompare(rank_item, m_rank_list[last_pos]) < 0)
	{
		if (swap_pos) *swap_pos = last_pos;
		return true;
	}

	return false;
}

bool TeamRank::GetFreeRankItemSlot(int rank_type, int *free_pos)
{
	if (rank_type < 0 || rank_type >= TEAM_RANK_TYPE_MAX) return false;

	int begin_index = rank_type * TEAM_RANK_MAX_SIZE;
	int end_index = (rank_type + 1) * TEAM_RANK_MAX_SIZE;
	
	int i = begin_index;
	for (; i < end_index; i++)
	{
		if (m_rank_list[i].Invalid())
		{
			break;
		}
	}

	if (i >= end_index) return false;

	if (NULL != free_pos) *free_pos = i;

	return true;
}

void TeamRank::UserResetName(const UserID &user_id, GameName name)
{
	int uid = UidToInt(user_id);

	for (int rank_index = 0; rank_index < TEAM_RANK_TYPE_MAX * TEAM_RANK_MAX_SIZE; ++rank_index)
	{
		if (!m_rank_list[rank_index].Invalid())
		{
			for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++ i)
			{
				if (m_rank_list[rank_index].member_list[i].uid == uid)
				{
					F_STRNCPY(m_rank_list[rank_index].member_list[i].user_name, name, sizeof(m_rank_list[0].member_list[0].user_name));

					m_dirty_mark[rank_index] = true;
					m_change_flag = true;

					break;
				}
			}
		}
	}
}

