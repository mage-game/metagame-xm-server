#include "crosspersonrank.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/struct/structcommon.h"

#include "protocal/msgcross.h"

#include "internalcomm.h"
#include "crossserver.h"
#include "rmibackobjdef.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "crossserver/crosslog.hpp"
#include "servercommon/serverconfig/crossrankrewardconfig.hpp"
#include "servercommon/string/crossstr.h"

CrossPersonRank::CrossPersonRank()
	: m_change_flag(false), m_rank_list_load_flag(false), m_last_snapshot_time(0), m_rank_data_next_save_time(0),
	m_rank_data_next_update_time(0), m_rank_title_load_flag(false)
{
	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));
	memset(m_old_state, 0, sizeof(m_old_state));
	memset(m_next_rank_reward_time_list, 0, sizeof(m_next_rank_reward_time_list));
}

CrossPersonRank::~CrossPersonRank()
{

}

void RMIInitCrossPersonRankBackObjectImpl::InitCrossPersonRankRet(int ret, const CrossPersonRankParam &cross_person_rank_param)
{
	if (ret < 0)
	{
		cross_person_rank->GetCrossServer()->StopServer();
	}
	else
	{
		cross_person_rank->Init(cross_person_rank_param);
	}
}

void CrossPersonRank::OnServerStart()
{
	RMIInitCrossPersonRankBackObjectImpl *impl = new RMIInitCrossPersonRankBackObjectImpl();
	impl->cross_person_rank = this;

	RMICrossClient cc;
	cc.__bind_session(InternalComm::Instance().GetDBHandler().GetCrossDB());
	cc.InitCrossPersonRankAsyn(impl);

	this->CalcNextRankRewardTime();
}

void CrossPersonRank::OnServerStop()
{
	this->SaveRankList();
}

void CrossPersonRank::InitRankTitleParam(const CrossPersonRankTitleParam &cross_personrank_title_param)
{
#ifndef __CROSS_PERSON_RANK_SHIELD_SAVE_RANK
	memcpy(m_rank_title_uuid_info_list, cross_personrank_title_param.rank_title_uuid_info_list, sizeof(m_rank_title_uuid_info_list));
#endif // __CROSS_PERSON_RANK_SHIELD_SAVE_RANK
	m_rank_title_load_flag = true;

	for (int i = 0; i < CROSS_RANK_TOP_TYPE_MAX; i++)
	{
		this->SyncRankTitleInfoToGameWorld(i);
	}
}

void CrossPersonRank::GetRankTitleParam(CrossPersonRankTitleParam *cross_personrank_title_param)
{
#ifndef __CROSS_PERSON_RANK_SHIELD_SAVE_RANK
	memcpy(cross_personrank_title_param->rank_title_uuid_info_list, m_rank_title_uuid_info_list, sizeof(m_rank_title_uuid_info_list));
#endif // __CROSS_PERSON_RANK_SHIELD_SAVE_RANK
}

void CrossPersonRank::Init(const CrossPersonRankParam &cross_personrank_param)
{
#ifndef __CROSS_PERSON_RANK_SHIELD_SAVE_RANK
	int count = 0;
	if (cross_personrank_param.count < CROSS_PERSON_RANK_TYPE_MAX * CROSS_PERSON_RANK_MAX_SIZE)
	{
		count = cross_personrank_param.count;
	}
	else
	{
		count = CROSS_PERSON_RANK_TYPE_MAX * CROSS_PERSON_RANK_MAX_SIZE;
	}

	for (int i = count - 1; i >= 0; i--)
	{
		int rank_type = cross_personrank_param.cross_person_rank_list[i].rank_type;
		if (rank_type < 0 || rank_type >= CROSS_PERSON_RANK_TYPE_MAX)
		{
			continue;
		}

		int rank_index = cross_personrank_param.cross_person_rank_list[i].index;
		if (rank_index < 0 || rank_index >= CROSS_PERSON_RANK_TYPE_MAX * CROSS_PERSON_RANK_MAX_SIZE)
		{
			continue;
		}

		if (cross_personrank_param.cross_person_rank_list[i].uuid <= 0)
		{
			continue;
		}

		if (!m_rank_list[rank_index].Invalid())
		{
			continue;
		}

		m_rank_list[rank_index].plat_type = LongLongToUniqueUserID(cross_personrank_param.cross_person_rank_list[i].uuid).plat_type;
		m_rank_list[rank_index].role_id = UidToInt(LongLongToUniqueUserID(cross_personrank_param.cross_person_rank_list[i].uuid).user_id);
		F_STRNCPY(m_rank_list[rank_index].name, cross_personrank_param.cross_person_rank_list[i].user_name, sizeof(m_rank_list[0].name));
		m_rank_list[rank_index].level = cross_personrank_param.cross_person_rank_list[i].level;
		m_rank_list[rank_index].prof = cross_personrank_param.cross_person_rank_list[i].prof;
		m_rank_list[rank_index].sex = cross_personrank_param.cross_person_rank_list[i].sex;
		m_rank_list[rank_index].camp = cross_personrank_param.cross_person_rank_list[i].camp;
		m_rank_list[rank_index].exp = cross_personrank_param.cross_person_rank_list[i].exp;
		m_rank_list[rank_index].rank_type = cross_personrank_param.cross_person_rank_list[i].rank_type;
		m_rank_list[rank_index].rank_value = cross_personrank_param.cross_person_rank_list[i].rank_value;
		m_rank_list[rank_index].record_index = cross_personrank_param.cross_person_rank_list[i].record_index;
		m_rank_list[rank_index].flexible_ll = cross_personrank_param.cross_person_rank_list[i].flexible_ll;
		F_STRNCPY(m_rank_list[rank_index].flexible_name, cross_personrank_param.cross_person_rank_list[i].flexible_name, sizeof(m_rank_list[0].flexible_name));
		m_rank_list[rank_index].flexible_int = cross_personrank_param.cross_person_rank_list[i].flexible_int;
		m_rank_list[rank_index].vip_level = cross_personrank_param.cross_person_rank_list[i].vip_level;

		long long uuid = UniqueUserIDToLongLong(UniqueUserID(m_rank_list[rank_index].plat_type, IntToUid(m_rank_list[rank_index].role_id)));
		this->AddUserIndex(rank_type, uuid, rank_index);

		m_old_state[rank_index] = true;
	}
#endif // __CROSS_PERSON_RANK_SHIELD_SAVE_RANK

	m_rank_list_load_flag = true;
	m_rank_data_next_save_time = EngineAdapter::Instance().Time() + RANK_DATA_SAVE_TO_DB_TIME;
	m_rank_data_next_update_time = EngineAdapter::Instance().Time() + RANK_DATA_SAVE_TO_DB_TIME;
}

void CrossPersonRank::Update(unsigned long interval, time_t now_second)
{
	if (!this->IsLoadFinish()) return;

	if (now_second > m_last_snapshot_time + CROSS_RANK_SNAPSHOT_INTERVAL)
	{
		this->Snapshot();
		m_last_snapshot_time = now_second;
	}

	if (m_rank_data_next_save_time > 0 && now_second > m_rank_data_next_save_time)
	{
		this->SaveRankList();
		m_rank_data_next_save_time = now_second + RANK_DATA_SAVE_TO_DB_TIME;
	}

	if (m_rank_data_next_update_time > 0 && now_second > m_rank_data_next_update_time)
	{
		this->UpdateHiddenWorldLevel();
		m_rank_data_next_update_time = now_second + RANK_DATA_SAVE_TO_DB_TIME;
	}
}

void CrossPersonRank::OnDayChange()
{
	for (int i = 0; i < CROSS_RANK_TOP_TYPE_MAX; i++)
	{
		if (CROSS_RANK_TOP_TYPE_1V1 != i &&
			CROSS_RANK_TOP_TYPE_3V3 != i)
		{
			this->RefreshRankTitle(i);
		}
	}

	this->CalcNextRankRewardTime();
}

void CrossPersonRank::OnWeekChange()
{
	return;
}

void CrossPersonRank::OnSeasonChange(int cross_rank_top_tpye)
{
	switch (cross_rank_top_tpye)
	{
		case CROSS_RANK_TOP_TYPE_1V1:
		{
			this->RefreshRankTitle(CROSS_RANK_TOP_TYPE_1V1);
		}
		break;

		case CROSS_RANK_TOP_TYPE_3V3:
		{
			this->RefreshRankTitle(CROSS_RANK_TOP_TYPE_3V3);
		}
		break;
	}
}

void CrossPersonRank::OnRegisterGameServer(NetID global_netid)
{
	for (int i = 0; i < CROSS_RANK_TOP_TYPE_MAX; i++)
	{
		this->SyncRankTitleInfoToGameWorld(i, global_netid);
	}
}

void CrossPersonRank::GetChangeRankItemList(CrossPersonRankParam *cross_personrank_param)
{
	if (NULL == cross_personrank_param) return;

	if (!m_change_flag)
	{
		cross_personrank_param->count = 0;
		return;
	}

	int count = 0;
	for (int i = 0; i < CROSS_PERSON_RANK_TYPE_MAX * CROSS_PERSON_RANK_MAX_SIZE && count < CROSS_PERSON_RANK_TYPE_MAX *CROSS_PERSON_RANK_MAX_SIZE; ++i)
	{
		if (m_dirty_mark[i])
		{
			if (m_rank_list[i].Invalid())
			{
				if (!m_old_state[i]) continue;

				cross_personrank_param->cross_person_rank_list[count].change_state = structcommon::CS_DELETE;
				cross_personrank_param->cross_person_rank_list[count].index = i;
			}
			else
			{
				cross_personrank_param->cross_person_rank_list[count].change_state = m_old_state[i] ? structcommon::CS_UPDATE : structcommon::CS_INSERT;
				cross_personrank_param->cross_person_rank_list[count].index = i;

				cross_personrank_param->cross_person_rank_list[count].uuid = UniqueUserIDToLongLong(UniqueUserID(m_rank_list[i].plat_type, IntToUid(m_rank_list[i].role_id)));
				cross_personrank_param->cross_person_rank_list[count].level = m_rank_list[i].level;
				cross_personrank_param->cross_person_rank_list[count].prof = m_rank_list[i].prof;
				cross_personrank_param->cross_person_rank_list[count].sex = m_rank_list[i].sex;
				cross_personrank_param->cross_person_rank_list[count].camp = m_rank_list[i].camp;
				cross_personrank_param->cross_person_rank_list[count].exp = m_rank_list[i].exp;
				cross_personrank_param->cross_person_rank_list[count].rank_type = m_rank_list[i].rank_type;
				cross_personrank_param->cross_person_rank_list[count].rank_value = m_rank_list[i].rank_value;
				cross_personrank_param->cross_person_rank_list[count].record_index = m_rank_list[i].record_index;
				cross_personrank_param->cross_person_rank_list[count].flexible_ll = m_rank_list[i].flexible_ll;
				cross_personrank_param->cross_person_rank_list[count].flexible_int = m_rank_list[i].flexible_int;
				cross_personrank_param->cross_person_rank_list[count].vip_level = m_rank_list[i].vip_level;
				F_STRNCPY(cross_personrank_param->cross_person_rank_list[count].user_name, m_rank_list[i].name, sizeof(m_rank_list[i].name));
				F_STRNCPY(cross_personrank_param->cross_person_rank_list[count].flexible_name, m_rank_list[i].flexible_name, sizeof(m_rank_list[i].flexible_name));
			}

			++count;
		}
	}

	cross_personrank_param->count = count;
}

void CrossPersonRank::ClearDirtyMark()
{
	if (!m_change_flag) return;

	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));

	for (int i = 0; i < CROSS_PERSON_RANK_TYPE_MAX * CROSS_PERSON_RANK_MAX_SIZE; ++i)
	{
		m_old_state[i] = !m_rank_list[i].Invalid();
	}

	m_change_flag = false;
}

void CrossPersonRank::OnGetRankList(NetID global_netid, const UniqueUserID &unique_user_id, int rank_type)
{
	 if (!m_rank_list_load_flag) return;

	if (rank_type < 0 || rank_type >= CROSS_PERSON_RANK_TYPE_MAX) return;

	static Protocol::SCGetCrossPersonRankListAck rank_list_ack;
	rank_list_ack.rank_type = rank_type;
	rank_list_ack.count = 0;

	int rank_start = rank_type * CROSS_PERSON_RANK_MAX_SIZE;
	int rank_end = rank_start + CROSS_PERSON_RANK_MAX_SIZE;

	int rank_count = 0;

	for (int i = rank_start; i < rank_end && i < CROSS_PERSON_RANK_MAX_SIZE * CROSS_PERSON_RANK_TYPE_MAX && rank_count < CROSS_PERSON_RANK_MAX_SIZE; ++i)
	{
		if (m_rank_snapshot[i].Invalid()) continue;

		rank_list_ack.rank_list[rank_count].plat_type = m_rank_snapshot[i].plat_type;
		rank_list_ack.rank_list[rank_count].role_id = m_rank_snapshot[i].role_id;
		rank_list_ack.rank_list[rank_count].server_id = IntToUid(m_rank_snapshot[i].role_id).db_index;
		rank_list_ack.rank_list[rank_count].level = m_rank_snapshot[i].level;
		rank_list_ack.rank_list[rank_count].prof = m_rank_snapshot[i].prof;
		rank_list_ack.rank_list[rank_count].sex = m_rank_snapshot[i].sex;
		rank_list_ack.rank_list[rank_count].camp = m_rank_snapshot[i].camp;
		rank_list_ack.rank_list[rank_count].exp = m_rank_snapshot[i].exp;
		rank_list_ack.rank_list[rank_count].rank_value = m_rank_snapshot[i].rank_value;
		rank_list_ack.rank_list[rank_count].flexible_ll = m_rank_snapshot[i].flexible_ll;
		rank_list_ack.rank_list[rank_count].vip_level = m_rank_snapshot[i].vip_level;
		rank_list_ack.rank_list[rank_count].flexible_int = m_rank_snapshot[i].flexible_int;
		F_STRNCPY(rank_list_ack.rank_list[rank_count].name, m_rank_snapshot[i].name, sizeof(m_rank_snapshot[i].name));
		F_STRNCPY(rank_list_ack.rank_list[rank_count].flexible_name, m_rank_snapshot[i].flexible_name, sizeof(m_rank_snapshot[i].flexible_name));

		++rank_count;
	}

	rank_list_ack.count = rank_count;
	int sendlen = sizeof(rank_list_ack) - (CROSS_PERSON_RANK_MAX_SIZE - rank_list_ack.count) * sizeof(rank_list_ack.rank_list[0]);
	CrossServer::GetInstCrossServer()->SendToUser(global_netid, unique_user_id.user_id, (const char*)&rank_list_ack, sendlen);
}

void CrossPersonRank::OnRankInfoChange(crossgameprotocal::GameCrossSyncCrossPersonRankInfo &rank_info)
{
	if (rank_info.role_id <= 0 || rank_info.item_count <= 0) return;

	CrossrPersonRankItem rank_item;
	rank_item.plat_type = rank_info.plat_type;
	rank_item.role_id = rank_info.role_id;
	F_STRNCPY(rank_item.name, rank_info.name, sizeof(rank_item.name));
	rank_item.level = rank_info.level;
	rank_item.prof = rank_info.prof;
	rank_item.sex = rank_info.sex;
	rank_item.camp = rank_info.camp;
	rank_item.exp = rank_info.exp;
	rank_item.vip_level = rank_info.vip_level;

	bool is_guild_rank = false;
	for (int i = 0; i < rank_info.item_count && i < crossgameprotocal::SYNC_CROSS_PERSON_RANK_INFO_SIZE; ++i)
	{
		rank_item.rank_type = rank_info.items[i].rank_type;
		rank_item.rank_value = rank_info.items[i].rank_value;
		rank_item.record_index = rank_info.items[i].record_index;
		rank_item.flexible_ll = rank_info.items[i].flexible_ll;
		rank_item.flexible_int = rank_info.items[i].flexible_int;
		F_STRNCPY(rank_item.flexible_name, rank_info.items[i].flexible_name, sizeof(rank_item.flexible_name));
		this->UpdateRankList(rank_item);

		if (IsGuildRank(rank_item.rank_type))
		{
			is_guild_rank = true;
		}
	}

	long long uuid = UniqueUserIDToLongLong(UniqueUserID(rank_info.plat_type, IntToUid(rank_info.role_id)));
	for (int rank_type = 0; rank_type < CROSS_PERSON_RANK_TYPE_MAX; ++rank_type)
	{
		if (is_guild_rank && !IsGuildRank(rank_type))
		{
			continue;
		}

		UserIndexMapIt it = m_user_index_map[rank_type].find(uuid);
		if (it != m_user_index_map[rank_type].end())
		{
			for (std::set<int>::iterator index_it = it->second.begin(), index_end = it->second.end(); index_it != index_end;)
			{
				int rank_index = *index_it;

				if (this->IsValidIndex(rank_index))
				{
					F_STRNCPY(m_rank_list[rank_index].name, rank_info.name, sizeof(rank_item.name));
					m_rank_list[rank_index].level = rank_info.level;
					m_rank_list[rank_index].prof = rank_info.prof;
					m_rank_list[rank_index].sex = rank_info.sex;
					m_rank_list[rank_index].camp = rank_info.camp;
					m_rank_list[rank_index].exp = rank_info.exp;

					++ index_it;

					m_change_flag = true;
					m_dirty_mark[rank_index] = true;
				}
				else
				{
					++ index_it;
				}
			}
		}
	}
}

void CrossPersonRank::ClearRank(int rank_type)
{
	if (rank_type < 0 || rank_type >= CROSS_PERSON_RANK_TYPE_MAX) return;


	int rank_start = rank_type * CROSS_PERSON_RANK_MAX_SIZE;
	int rank_end = rank_start + CROSS_PERSON_RANK_MAX_SIZE;

	for (int i = rank_start; i < rank_end; ++i)
	{
		m_rank_list[i].Reset();
		m_dirty_mark[i] = true;
	}

	m_user_index_map[rank_type].clear();
	m_change_flag = true;
}

void CrossPersonRank::ClearAllRank()
{
	for (int i = CROSS_PERSON_RANK_TYPE_CAPABILITY_ALL; i <  CROSS_PERSON_RANK_TYPE_MAX; i++)
	{
		ClearRank(i);
	}
}

static int RankInfoSortCmp(const void *v1, const void *v2)
{
	CrossrPersonRankItem *item1 = *((CrossrPersonRankItem**)v1);
	CrossrPersonRankItem *item2 = *((CrossrPersonRankItem**)v2);

	if (NULL == item1 || NULL == item2) return false;

	if (item2->rank_value == item1->rank_value)
	{
		return item2->reserved > item1->reserved ? 1 : -1;
	}

	return item2->rank_value > item1->rank_value ? 1 : -1;
}

int CrossPersonRank::GetRankTopUser(int rank_type, int max_user, long long *user_list)
{
	if (rank_type < 0 || rank_type >= CROSS_PERSON_RANK_TYPE_MAX || max_user <= 0 || max_user > CROSS_PERSON_RANK_MAX_SIZE || NULL == user_list) return 0;

	int rank_start = rank_type * CROSS_PERSON_RANK_MAX_SIZE;
	int rank_end = rank_start + CROSS_PERSON_RANK_MAX_SIZE;

	static CrossrPersonRankItem *temp_list[CROSS_PERSON_RANK_MAX_SIZE] = {NULL};
	memset(temp_list, 0, sizeof(temp_list));

	int temp_list_size = 0;
	for (int i = rank_start; i < rank_end && temp_list_size < CROSS_PERSON_RANK_MAX_SIZE; ++i)
	{
		if (!m_rank_list[i].Invalid() && 0 != m_rank_list[i].rank_value)
		{
			temp_list[temp_list_size] = &m_rank_list[i];
			temp_list[temp_list_size]->reserved = CROSS_PERSON_RANK_MAX_SIZE - i;
			
			++ temp_list_size;
		}
	}

	qsort(temp_list, temp_list_size, sizeof(temp_list[0]), RankInfoSortCmp);

	int count = 0;
	for (int i = 0; i < temp_list_size && count < max_user; ++i)
	{
		if (NULL != temp_list[i])
		{
			user_list[count] = UniqueUserIDToLongLong(UniqueUserID(temp_list[i]->plat_type, IntToUid(temp_list[i]->role_id)));
			++ count;
		}
	}

	return count;
}

void CrossPersonRank::SyncRankTitleInfoToGameWorld(int cross_top_rank_type, NetID global_netid)
{
	if (cross_top_rank_type < 0 || cross_top_rank_type >= CROSS_RANK_TOP_TYPE_MAX) return;

	static crossgameprotocal::CrossGameSyncCrossPersonRankTitleInfo cgscprti;
	cgscprti.rank_type = cross_top_rank_type;
	cgscprti.uuid_count = m_rank_title_uuid_info_list[cross_top_rank_type].uuid_count;
	memcpy(cgscprti.rank_title_user_uuid_list, m_rank_title_uuid_info_list[cross_top_rank_type].rank_title_uuid_list, sizeof(m_rank_title_uuid_info_list[cross_top_rank_type].rank_title_uuid_list));

	if (UINT_MAX == global_netid)
	{
		m_cross_server->SendToAllGameWorld((const char*)&cgscprti, sizeof(cgscprti));
	}
	else
	{
		m_cross_server->SendToGameWorld(global_netid, (const char*)&cgscprti, sizeof(cgscprti));
	}
}

long long CrossPersonRank::GetRankValue(int rank_type, long long uuid)
{
	if (rank_type < 0 || rank_type >= CROSS_PERSON_RANK_TYPE_MAX) return 0;

	int rank_start = rank_type * CROSS_PERSON_RANK_MAX_SIZE;
	int rank_end = rank_start + CROSS_PERSON_RANK_MAX_SIZE;

	for (int i = rank_start; i < rank_end; ++i)
	{
		if (!m_rank_list[i].Invalid() && 0 != m_rank_list[i].rank_value)
		{
			if ((m_rank_list[i].plat_type == LongLongToUniqueUserID(uuid).plat_type &&
				m_rank_list[i].role_id == UidToInt(LongLongToUniqueUserID(uuid).user_id)))
			{
				return m_rank_list[i].rank_value;
			}
		}
	}

	return 0;
}

void CrossPersonRank::Cross1v1UpdateRankList(CrossrPersonRankItem &rank_item)
{
	this->UpdateRankList(rank_item);

	long long uuid = UniqueUserIDToLongLong(UniqueUserID(rank_item.plat_type, IntToUid(rank_item.role_id)));

	int rank_type = CROSS_PERSON_RANK_TYPE_1V1_SCORE;
	UserIndexMapIt it = m_user_index_map[rank_type].find(uuid);
	if (it != m_user_index_map[rank_type].end())
	{
		for (std::set<int>::iterator index_it = it->second.begin(), index_end = it->second.end(); index_it != index_end;)
		{
			int rank_index = *index_it;

			if (this->IsValidIndex(rank_index))
			{
				F_STRNCPY(m_rank_list[rank_index].name, rank_item.name, sizeof(rank_item.name));
				m_rank_list[rank_index].level = rank_item.level;
				m_rank_list[rank_index].prof = rank_item.prof;
				m_rank_list[rank_index].sex = rank_item.sex;
				m_rank_list[rank_index].camp = rank_item.camp;
				m_rank_list[rank_index].exp = rank_item.exp;
				m_rank_list[rank_index].jingjie = rank_item.jingjie;

				++index_it;

				m_change_flag = true;
				m_dirty_mark[rank_index] = true;
			}
			else
			{
				++index_it;
			}
		}
	}
}

const CrossrPersonRankItem *CrossPersonRank::GetRankItem(int rank_type, long long uuid)
{
	int rank_start = rank_type * CROSS_PERSON_RANK_MAX_SIZE;
	int rank_end = rank_start + CROSS_PERSON_RANK_MAX_SIZE;

	for (int i = rank_start; i < rank_end; ++i)
	{
		if (!m_rank_list[i].Invalid() && 0 != m_rank_list[i].rank_value)
		{
			if ((m_rank_list[i].plat_type == LongLongToUniqueUserID(uuid).plat_type &&
				m_rank_list[i].role_id == UidToInt(LongLongToUniqueUserID(uuid).user_id)))
			{
				return &m_rank_list[i];
			}
		}
	}

	return NULL;
}

void CrossPersonRank::UpdateHiddenWorldLevel()
{
	int world_level = 0;
	static const int CORSS_WORLD_LEVEL_ROLE_COUNT = 20;

	long long uuid_list[CORSS_WORLD_LEVEL_ROLE_COUNT] = { 0 };
	int top_user_count = CrossServer::GetInstCrossServer()->GetCrossPersonRank()->GetRankTopUser(CROSS_PERSON_RANK_TYPE_ROLE_LEVEL, CORSS_WORLD_LEVEL_ROLE_COUNT, uuid_list);
	if (top_user_count <= 0)
	{
		return;
	}
	for (int i = 0; i < top_user_count; ++i)
	{
		int role_level = static_cast<int>(CrossServer::GetInstCrossServer()->GetCrossPersonRank()->GetRankValue(CROSS_PERSON_RANK_TYPE_ROLE_LEVEL, uuid_list[i]));
		const CrossrPersonRankItem *rank_item = CrossServer::GetInstCrossServer()->GetCrossPersonRank()->GetRankItem(CROSS_PERSON_RANK_TYPE_ROLE_LEVEL, uuid_list[i]);
		if (NULL != rank_item)
		{
			world_level += role_level;
		}
	}

	crossgameprotocal::CrossGameCrossWorldLevelInfo crossmsg;
	crossmsg.world_level = world_level / top_user_count;

	CrossServer::GetInstCrossServer()->SendToHiddenGameWorld((const char *)&crossmsg, sizeof(crossmsg));
}

bool CrossPersonRank::IsGuildRank(int rank_type)
{
	bool is_guild_rank = false;
	if (rank_type == CROSS_PERSON_RANK_TYPE_GUILD_KILL_BOSS)
	{
		is_guild_rank = true;
	}

	return is_guild_rank;
}

void CrossPersonRank::UpdateRankList(CrossrPersonRankItem &rank_item)
{
	if (rank_item.rank_type < 0 || rank_item.rank_type >= CROSS_PERSON_RANK_TYPE_MAX) return;

	switch (rank_item.rank_type)
	{
	case CROSS_PERSON_RANK_TYPE_XIULUO_TOWER:
	{
		this->ClearRank(rank_item.rank_type);
	}
		break;

	case CROSS_PERSON_RANK_TYPE_ADD_CAPABILITY:
	{
		//跨服增战榜增加最低上榜限制（智扬需求）
		const CrossRankReward *reward_cfg = CrossRankRewardConfig::Instance().GetReward(CROSS_PERSON_RANK_TYPE_ADD_CAPABILITY, 1);
		if (nullptr != reward_cfg)
		{
			if (rank_item.rank_value < reward_cfg->limit_value)
			{
				return;
			}
		}
	}
	break;

	case CROSS_PERSON_RANK_TYPE_ADD_CHARM:
	{
		//跨服增魅榜增加最低上榜限制（智扬需求）
		const CrossRankReward *reward_cfg = CrossRankRewardConfig::Instance().GetReward(CROSS_PERSON_RANK_TYPE_ADD_CHARM, 1);
		if (nullptr != reward_cfg)
		{
			if (rank_item.rank_value < reward_cfg->limit_value)
			{
				return;
			}
		}
	}
	break;

	case CROSS_PERSON_RANK_TYPE_GUILD_KILL_BOSS:
	{
		const CrossGuildKillBossRankReward *reward_cfg = CrossRankRewardConfig::Instance().GetCrossGuildRankReward(CROSS_PERSON_RANK_TYPE_GUILD_KILL_BOSS, 1);
		if (nullptr != reward_cfg)
		{
			if (rank_item.rank_value < reward_cfg->limit_value)
			{
				return;
			}
		}
	}
	break;

	}

	int find_index = 0;

	long long uuid = UniqueUserIDToLongLong(UniqueUserID(rank_item.plat_type, IntToUid(rank_item.role_id)));

	bool is_find = this->GetUserRecordIndex(rank_item.rank_type, uuid, rank_item.record_index, &find_index);
	if (is_find && this->IsValidIndex(find_index))
	{
		m_rank_list[find_index] = rank_item;

		m_change_flag = true;
		m_dirty_mark[find_index] = true;

	}
	else 
	{
		int pos = -1;

		if (this->GetRankTypeSize(rank_item.rank_type) >= CROSS_PERSON_RANK_MAX_SIZE)
		{
			if (!this->GetSwapRankItemSlot(rank_item.rank_type, rank_item, &pos)) return;

			if (this->IsValidIndex(pos))
			{
				long long remove_uuid = UniqueUserIDToLongLong(UniqueUserID(m_rank_list[pos].plat_type, IntToUid(m_rank_list[pos].role_id)));
				this->RemoveUserIndex(m_rank_list[pos].rank_type, remove_uuid, pos);
			}
		}
		else 
		{
			if (!this->GetFreeRankItemSlot(rank_item.rank_type, &pos)) return;
		}

		if (pos >= rank_item.rank_type * CROSS_PERSON_RANK_MAX_SIZE && pos < (rank_item.rank_type + 1) * CROSS_PERSON_RANK_MAX_SIZE)
		{
			m_rank_list[pos] = rank_item;
			this->AddUserIndex(rank_item.rank_type, uuid, pos);

			m_change_flag = true;
			m_dirty_mark[pos] = true;
		}
	}
	if (CROSS_PERSON_RANK_TYPE_XIULUO_TOWER == rank_item.rank_type)
	{
		this->RefreshRankTitle(CROSS_RANK_TOP_TYPE_XIULUO_TOWER);
	}
}

void CrossPersonRank::UpdateBaseInfo(CrossrPersonRankItem &rank_item)
{

}

bool CrossPersonRank::GetSwapRankItemSlot(int rank_type, const CrossrPersonRankItem &rank_item, int *swap_post)
{
	if (rank_type < 0 || rank_type >= CROSS_PERSON_RANK_TYPE_MAX) return false;

	int begin_index = rank_type * CROSS_PERSON_RANK_MAX_SIZE;
	int end_index = (rank_type + 1) * CROSS_PERSON_RANK_MAX_SIZE;

	int min_pos = begin_index;
	for (int i = begin_index + 1; i < end_index; i++)
	{
		if (m_rank_list[i].rank_value < m_rank_list[min_pos].rank_value)
		{
			min_pos = i;
		}
	}

	if (m_rank_list[min_pos].rank_value < rank_item.rank_value)
	{
		if (NULL != swap_post) *swap_post = min_pos;
		return true;
	}

	return false;
}

bool CrossPersonRank::GetFreeRankItemSlot(int rank_type, int *free_pos)
{
	if (rank_type < 0 || rank_type >= CROSS_PERSON_RANK_TYPE_MAX) return false;

	int begin_index = rank_type * CROSS_PERSON_RANK_MAX_SIZE;
	int end_index = (rank_type + 1) * CROSS_PERSON_RANK_MAX_SIZE;

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

static int CrossRankInfoSortCmp(const void *v1, const void *v2)
{
	CrossrPersonRankItem *item1 = *((CrossrPersonRankItem**)v1);
	CrossrPersonRankItem *item2 = *((CrossrPersonRankItem**)v2);

	if (NULL == item1 || NULL == item2) return false;

	if (item2->rank_value == item1->rank_value)
	{
		return item2->reserved > item1->reserved ? 1 : -1;
	}

	return item2->rank_value > item1->rank_value ? 1 : -1;
}

void CrossPersonRank::Snapshot()
{
	for (int rank_type = 0; rank_type < CROSS_PERSON_RANK_TYPE_MAX; ++ rank_type)
	{
		CrossrPersonRankItem *tmp_rank_item[CROSS_PERSON_RANK_MAX_SIZE]; 
		memset(tmp_rank_item, 0, sizeof(tmp_rank_item));
		int cmp_item_count = 0;
		int rank_start = rank_type * CROSS_PERSON_RANK_MAX_SIZE; 

		for (int index = 0; index < CROSS_PERSON_RANK_MAX_SIZE && cmp_item_count < CROSS_PERSON_RANK_MAX_SIZE; ++ index)
		{
			int rank_idx = rank_start + index;
			m_rank_snapshot[rank_idx].Reset();
			
			if (!m_rank_list[rank_idx].Invalid() && m_rank_list[rank_idx].rank_value != 0)
			{
				tmp_rank_item[cmp_item_count] = &m_rank_list[rank_idx];
				tmp_rank_item[cmp_item_count]->reserved = 0;

				++ cmp_item_count;
			}
		}

		qsort(tmp_rank_item, cmp_item_count, sizeof(tmp_rank_item[0]), CrossRankInfoSortCmp);

		for (int cmp_index = 0; cmp_index < 3 && cmp_index < cmp_item_count && cmp_index < CROSS_PERSON_RANK_MAX_SIZE; ++cmp_index)
		{
			if (NULL != tmp_rank_item)
			{
				if (tmp_rank_item[cmp_index]->role_id != m_rank_snapshot[rank_start + cmp_index].role_id)
				{
					bool is_broadcast_to_guild = true;

					for (int last_pos = 0; last_pos < 3; last_pos++)
					{
						if (tmp_rank_item[cmp_index]->role_id == m_rank_snapshot[rank_start + last_pos].role_id)
						{
							if (cmp_index > last_pos)
							{
								is_broadcast_to_guild = false;
							}
						}
					}

					if (is_broadcast_to_guild)
					{
						UserID target_userid = IntToUid(tmp_rank_item[cmp_index]->role_id);
						UniqueUserID target_unique_userid(tmp_rank_item[cmp_index]->plat_type, target_userid);

						crossgameprotocal::SyncCrossRankInfoToOriginGs info;

						info.cross_rank_type = rank_type;
						info.param1 = tmp_rank_item[cmp_index]->role_id;
						info.param2 = 0;
						info.rank_value = tmp_rank_item[cmp_index]->rank_value;
						info.rank_pos = cmp_index + 1;

						m_cross_server->SendToGameWorld(target_unique_userid, (const char*)&info, sizeof(info));
					}
				}
			}
		}

		for (int index = 0; index < cmp_item_count && index < CROSS_PERSON_RANK_MAX_SIZE; ++ index)
		{
			if (NULL != tmp_rank_item)
			{
				int rank_idx = rank_start + index;
				m_rank_snapshot[rank_idx] = *tmp_rank_item[index];
			}
		}
	}
}

bool CrossPersonRank::AddUserIndex(int rank_type, const long long uuid, int rank_index)
{
	if (rank_type < 0 || rank_type >= CROSS_PERSON_RANK_TYPE_MAX) return false;

	UserIndexMapIt it = m_user_index_map[rank_type].find(uuid);
	if (it != m_user_index_map[rank_type].end())
	{
		it->second.insert(rank_index);
	}
	else
	{
		std::set<int> rank_index_vec; rank_index_vec.insert(rank_index);
		m_user_index_map[rank_type][uuid] = rank_index_vec;
	}

	return true;
}

bool CrossPersonRank::RemoveUserIndex(int rank_type, const long long uuid, int rank_index)
{
	if (rank_type < 0 || rank_type >= CROSS_PERSON_RANK_TYPE_MAX) return false;

	UserIndexMapIt it = m_user_index_map[rank_type].find(uuid);
	if (it != m_user_index_map[rank_type].end())
	{
		if (IsValidIndex(rank_index))
		{
			m_rank_list[rank_index].Reset();
		}
		it->second.erase(rank_index);
		if (it->second.empty()) m_user_index_map[rank_type].erase(uuid);
	}

	return true;
}

bool CrossPersonRank::GetUserRecordIndex(int rank_type, const long long uuid, int record_index, int *get_rank_index)
{
	if (rank_type < 0 || rank_type >= CROSS_PERSON_RANK_MAX_SIZE) return false;

	UserIndexMapIt it = m_user_index_map[rank_type].find(uuid);
	if (it != m_user_index_map[rank_type].end())
	{
		for (std::set<int>::iterator index_it = it->second.begin(), index_end = it->second.end(); index_it != index_end; ++ index_it)
		{
			int rank_index = *index_it;
			if (this->IsValidIndex(rank_index))
			{
				if (m_rank_list[rank_index].record_index == record_index)
				{
					if (NULL != get_rank_index) *get_rank_index = rank_index;

					return true;
				}
			}
		}
	}

	return false;
}

int CrossPersonRank::GetRankTypeSize(int rank_type)
{
	if (rank_type < 0 || rank_type >= CROSS_PERSON_RANK_TYPE_MAX) return false;

	int rank_type_size = 0;
	for (UserIndexMapIt it = m_user_index_map[rank_type].begin(), end = m_user_index_map[rank_type].end(); it != end; ++ it)
	{
		rank_type_size += static_cast<int>(it->second.size());
	}

	return rank_type_size;
}

void CrossPersonRank::RefreshRankTitle(int top_rank_type)
{
	if (top_rank_type < 0 || top_rank_type >= CROSS_RANK_TOP_TYPE_MAX) return;

	
	int rank_type = -1, uuid_count = 0;
	long long uuid_list[CROSS_PERSON_RANK_MAX_SIZE] = {0};

	switch (top_rank_type)
	{
	case CROSS_RANK_TOP_TYPE_CAPABILITY:
		{
			rank_type = CROSS_PERSON_RANK_TYPE_CAPABILITY_ALL;
		}
		break;

	case CROSS_RANK_TOP_TYPE_XIULUO_TOWER:
		{
			rank_type = CROSS_PERSON_RANK_TYPE_XIULUO_TOWER;
		}
		break;

	case CROSS_RANK_TOP_TYPE_1V1:
		{
			rank_type = CROSS_PERSON_RANK_TYPE_1V1_SCORE;
		}
		break;

	case CROSS_RANK_TOP_TYPE_3V3:
		{
			rank_type = CROSS_PERSON_RANK_TYPE_3V3_SCORE;
		}
		break;
	}

	uuid_count = this->GetRankTopUser(rank_type, CROSS_PERSON_RANK_MAX_SIZE, uuid_list);
	m_rank_title_uuid_info_list[top_rank_type].rank_type = rank_type;
	m_rank_title_uuid_info_list[top_rank_type].uuid_count = uuid_count;
	memcpy(m_rank_title_uuid_info_list[top_rank_type].rank_title_uuid_list, uuid_list, sizeof(uuid_list));

	this->SyncRankTitleInfoToGameWorld(top_rank_type);
}

void CrossPersonRank::SaveRankList()
{
#ifndef __CROSS_PERSON_RANK_SHIELD_SAVE_RANK
	if (!m_rank_list_load_flag) return;

	static CrossPersonRankParam cross_person_rank_param;

	this->GetChangeRankItemList(&cross_person_rank_param);
	if (0 != cross_person_rank_param.count)
	{
		RMISaveCrossPersonRankBackObjectImpl *impl = new RMISaveCrossPersonRankBackObjectImpl();
		RMICrossClient cc;
		cc.__bind_session(InternalComm::Instance().GetDBHandler().GetCrossDB());

		bool ret = cc.SaveCrossPersonRankAsyn(cross_person_rank_param, impl);
		if (ret)
		{
			this->ClearDirtyMark();
		}
	}
#endif // __CROSS_PERSON_RANK_SHIELD_SAVE_RANK
}

void CrossPersonRank::GiveWeekCharmRankReward()
{
	this->Snapshot();

	static crossgameprotocal::CrossGameRankWeekRewardCommand cgrwrc;
	cgrwrc.rank_type = CROSS_PERSON_RANK_TYPE_WEEK_ADD_CHARM;
	cgrwrc.item_count = 0;


	int rank_start = CROSS_PERSON_RANK_TYPE_WEEK_ADD_CHARM * CROSS_PERSON_RANK_MAX_SIZE;
	for (int index = 0; index < CROSS_RANK_WEEK_REWARD_MAX_ROLE_COUNT && index < CROSS_PERSON_RANK_MAX_SIZE; ++ index)
	{
		int rank_idx = rank_start + index;
		if (m_rank_snapshot[rank_idx].Invalid())
		{
			// 造映像的时候已经排序，如果非法，没必要继续了
			break;
		}

		cgrwrc.item_list[cgrwrc.item_count].plat_type = m_rank_snapshot[rank_idx].plat_type;
		cgrwrc.item_list[cgrwrc.item_count].uid = m_rank_snapshot[rank_idx].role_id;
		cgrwrc.item_list[cgrwrc.item_count].rank_value = m_rank_snapshot[rank_idx].rank_value;
		++ cgrwrc.item_count;
	}

	for (int i = 0; i < cgrwrc.item_count; ++ i)
	{
		crossgameprotocal::CrossGameRankWeekRewardCommand::Item &item = cgrwrc.item_list[i];
		PlatName plat_name;
		LocalConfig::GetPlatName(item.plat_type, plat_name);
		UserID user_id = IntToUid(item.uid);
		UniqueUserID uuid = UniqueUserID(item.plat_type, user_id);
		bool server_connected = m_cross_server->IsUserLocateServerConnect(uuid);
		
		crosslog::g_log_cross_rank.printf(LL_INFO, "GiveWeekCharmRankReward : server_connected %d, rank %d, plat_id %d, plat_name[%s], server_id %d uid %d, rank_value %lld", 
			(server_connected ? 1 : 0), i, item.plat_type, plat_name, user_id.db_index, item.uid, item.rank_value);
	}

	m_cross_server->SendToAllGameWorld((const char *)&cgrwrc, sizeof(cgrwrc));
	this->ClearRank(CROSS_PERSON_RANK_TYPE_WEEK_ADD_CHARM);
}

void CrossPersonRank::CalcNextRankRewardTime()
{
	time_t now_sec = EngineAdapter::Instance().Time();
	for (int i = CROSS_PERSON_RANK_TYPE_ADD_CAPABILITY; i < CROSS_PERSON_RANK_TYPE_MAX; i++)
	{
		time_t &next_clear_time = m_next_rank_reward_time_list[i];
		if (now_sec >= next_clear_time)
		{
			const CrossRankRewardDateCfg *date_cfg = CrossRankRewardConfig::Instance().GetDateCfg(i);
			if (nullptr != date_cfg)
			{
				if (0 != next_clear_time)
				{
					this->GiveRankReward(i);
					this->ClearRank(i);
				}

				const tm *tm_now = EngineAdapter::Instance().LocalTime();
				if (nullptr != tm_now)
				{
					int day_count = 0;
					for (int day = tm_now->tm_wday + 1; day < DAY_PER_WEEK; ++day)
					{
						if (date_cfg->CanReward(day))
						{
							day_count = day - tm_now->tm_wday;
							break;
						}
					}

					if (0 == day_count)
					{
						for (int day = 0; day <= tm_now->tm_wday; ++day)
						{
							if (date_cfg->CanReward(day))
							{
								day_count = day + DAY_PER_WEEK - tm_now->tm_wday;
								break;
							}
						}
					}

					next_clear_time = now_sec + EngineAdapter::Instance().NextDayInterval(0, 0, 0) + (day_count - 1) * SECOND_PER_DAY;
				}
			}
		}
	}
}

void CrossPersonRank::GiveRankReward(int rank_type)
{
	const char *content = nullptr;
	switch (rank_type)
	{
	case CROSS_PERSON_RANK_TYPE_ADD_CAPABILITY:
	{
		content = gamestring::g_cross_add_cap_rank_content;
	}
	break;

	case CROSS_PERSON_RANK_TYPE_ADD_CHARM:
	{
		content = gamestring::g_cross_add_charm_rank_content;
	}
	break;

	case CROSS_PERSON_RANK_TYPE_GUILD_KILL_BOSS:
	{
		this->SyncGuildRankFirstFiveToGameWorld(rank_type);

		//this->ClearRank(CROSS_PERSON_RANK_TYPE_GUILD_KILL_BOSS);
		return;
	}
	break;

	default:
		break;
	}

	if (nullptr == content)
	{
		return;
	}

	int start_index = rank_type * CROSS_PERSON_RANK_MAX_SIZE;
	int end_index = start_index + CROSS_PERSON_RANK_MAX_SIZE;
	for (int i = start_index; i < end_index; ++i)
	{
		int rank_pos = i - start_index + 1;
		const CrossrPersonRankItem &item = m_rank_snapshot[i];
		if (m_rank_snapshot[i].Invalid())
		{
			// 造映像的时候已经排序，如果非法，没必要继续了
			break;
		}

		const CrossRankReward *reward_cfg = CrossRankRewardConfig::Instance().GetReward(rank_type, rank_pos);
		if (nullptr != reward_cfg)
		{
			static MailContentParam contentparam; contentparam.Reset();

			if (reward_cfg->reward_item.item_id > 0)
			{
				contentparam.AddItem(reward_cfg->reward_item);
			}

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), content, rank_pos);
			if (length > 0 && contentparam.item_count > 0)
			{
				PlatName plat_name;
				LocalConfig::GetPlatName(item.plat_type, plat_name);
				UserID user_id = IntToUid(item.role_id);
				UniqueUserID uuid = UniqueUserID(item.plat_type, user_id);
				m_cross_server->MailToGameWorldUser(uuid, contentparam);

				bool server_connected = m_cross_server->IsUserLocateServerConnect(uuid);

				crosslog::g_log_cross_rank.printf(LL_INFO, "CrossPersonRank::GiveRankReward : server_connected %d, rank %d, plat_id %d, plat_name[%s], server_id %d uid %d, rank_value %lld",
					(server_connected ? 1 : 0), i, item.plat_type, plat_name, user_id.db_index, item.role_id, item.rank_value);
			}
		}

		if (rank_pos < 3)
		{
			UniqueUserID target_unique_userid(item.plat_type, IntToUid(item.role_id));

			crossgameprotocal::SyncCrossRankInfoToOriginGs info;

			info.cross_rank_type = rank_type;
			info.param1 = item.role_id;
			info.param2 = 0;
			info.rank_value = item.rank_value;
			info.rank_pos = rank_pos + 1;

			m_cross_server->SendToGameWorld(target_unique_userid, (const char*)&info, sizeof(info));
		}
	}

	this->SyncRankFirstFiveToGameWorld(rank_type);

	//发奖后清理排行榜
	ClearRank(rank_type);
}

void CrossPersonRank::SyncRankFirstFiveToGameWorld(int rank_type)
{
	if (rank_type < 0 || rank_type >= CROSS_PERSON_RANK_TYPE_MAX) return;

	static crossgameprotocal::CrossGameSyncRankFirst pro;
	pro.rank_type = rank_type;

	int start_index = rank_type * CROSS_PERSON_RANK_MAX_SIZE;
	int end_index = start_index + CROSS_PERSON_RANK_MAX_SIZE;
	int pro_rank_count = 0;
	for (int i = start_index; i < end_index && pro_rank_count < CROSS_RANK_REWARD_COUNT; ++i)
	{
		pro.uid_list[pro_rank_count] = m_rank_snapshot[i].role_id;
		pro_rank_count++;

		//log
		PlatName plat_name;
		LocalConfig::GetPlatName(m_rank_snapshot[i].plat_type, plat_name);
		UserID user_id = IntToUid(m_rank_snapshot[i].role_id);
		UniqueUserID uuid = UniqueUserID(m_rank_snapshot[i].plat_type, user_id);
		bool server_connected = m_cross_server->IsUserLocateServerConnect(uuid);

		crosslog::g_log_cross_rank.printf(LL_INFO, "CrossPersonRank::GiveRankReward : server_connected %d, rank %d, plat_id %d, plat_name[%s], server_id %d uid %d, rank_value %lld",
			(server_connected ? 1 : 0), i, m_rank_snapshot[i].plat_type, plat_name, user_id.db_index, m_rank_snapshot[i].role_id, m_rank_snapshot[i].rank_value);
	}

	m_cross_server->SendToAllGameWorld((const char *)&pro, sizeof(pro));
}

void CrossPersonRank::SyncGuildRankFirstFiveToGameWorld(int rank_type)
{
	if (rank_type < 0 || rank_type >= CROSS_PERSON_RANK_TYPE_MAX) return;

	static crossgameprotocal::CrossGameSyncRankFirst pro;
	pro.rank_type = rank_type;

	//int start_index = rank_type * CROSS_PERSON_RANK_MAX_SIZE;
	//int end_index = start_index + CROSS_PERSON_RANK_MAX_SIZE;
	//int pro_rank_count = 0;
	//for (int i = start_index; i < end_index && pro_rank_count < CROSS_RANK_REWARD_COUNT; ++i)
	//{
	//	pro.uid_list[pro_rank_count] = m_rank_snapshot[i].role_id;
	//	pro_rank_count++;
	//}

	for (int i = 0; i < CROSS_RANK_REWARD_COUNT; ++i)
	{
		pro.uid_list[i] = m_rank_snapshot[i].role_id;
	}

	m_cross_server->SendToAllGameWorld((const char *)&pro, sizeof(pro));
}

void CrossPersonRank::GmRankReward(int rank_type, bool clear_rank)
{
	if (rank_type < 0 || rank_type >= CROSS_PERSON_RANK_TYPE_MAX) return;

	if (clear_rank)
	{
		m_next_rank_reward_time_list[rank_type] = EngineAdapter::Instance().Time();
		this->CalcNextRankRewardTime();
	}
	else
	{
		this->GiveRankReward(rank_type);
	}
}