#include "guildrank.hpp"
#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"

#include "global/rank/rankmanager.hpp"

#include "protocal/msgcsrank.h"
#include "protocal/msgscrank.h"

#include "engineadapter.h"
#include "internalcomm.h"
#include "servercommon/string/globalstr.h"

#include "obj/character/role.h"

union LongLongGuildLevelRankValue
{
	struct
	{
		unsigned int creat_time;
		short member_count;
		short level;
	};

	long long val;
};

UNSTD_STATIC_CHECK(sizeof(LongLongGuildLevelRankValue) == sizeof(long long));

static long long GuildLevelRankValueToLongLong(short level, short member_count, unsigned int creat_time)
{
	LongLongGuildLevelRankValue ll;

	ll.creat_time = static_cast<unsigned int>(-1) - creat_time;		// 创建时间是越小排越前
	ll.member_count = member_count;
	ll.level = level;
	
	return ll.val;
}

GuildRank::GuildRank()
	: m_rank_manager(NULL), m_last_snapshot_time(0)
{
}

GuildRank::~GuildRank()
{
}

void GuildRank::Update(time_t now_second)
{
	if (!GuildManager::Instance().IsGuildLoadFinish())
	{
		return;
	}

	if (now_second > m_last_snapshot_time + GUILD_RANK_SNAPSHOT_INTERVAL)
	{
		this->Snapshot();

		const GuildManager::GuildMap &guild_map = GuildManager::Instance().GetGuildMap();
		for (GuildManager::GuildMapConstIt it = guild_map.begin(); it != guild_map.end(); it++)
		{
			it->second->SetRank(0);
		}

		GuildID guild_id_list[GUILD_RANK_MAX_SIZE];
		//int count = this->GetRankTopGuild(GUILD_RANK_TYPE_CAPABILITY, GUILD_RANK_MAX_SIZE, guild_id_list);
		int count = this->GetSnapshotTopGuild(GUILD_RANK_TYPE_CAPABILITY, GUILD_RANK_MAX_SIZE, guild_id_list);
		for (int i = 0; i < count && i < GUILD_RANK_MAX_SIZE; i++)
		{
			Guild *guild = GuildManager::Instance().GetGuild(guild_id_list[i]);
			if (NULL != guild)
			{
				guild->SetRank(i + 1);
			}
		}
	}
}

bool GuildRank::CanReInitGuildRank(int rank_type)
{
	if (rank_type < GUILD_RANK_TYPE_LEVEL || rank_type >= GUILD_RANK_TYPE_MAX) return false;

	UNSTD_STATIC_CHECK(GUILD_RANK_TYPE_MAX == 9);

	switch (rank_type)
	{
	case GUILD_RANK_TYPE_LEVEL:
	case GUILD_RANK_TYPE_XIANMENGZHAN:
	case GUILD_RANK_TYPE_KILL_WORLD_BOSS:
	case GUILD_RANK_TYPE_XIANMENG_BIPIN_KILL_WORLD_BOSS:
	case GUILD_RANK_TYPE_DAY_INCREASE_CAPABILITY:
	case GUILD_RANK_TYPE_CAPABILITY:
	case GUILD_RANK_TYPE_GUILDBATTLE:
	case GUILD_RANK_TYPE_TERRITORYWAR:
	case GUILD_RANK_TYPE_KILL_CROSS_BOSS:
		return true;
	}
	
	return false;
}

long long GuildRank::CalcGuildRankValue(Guild *guild, int rank_type, bool *rank_value_valid)
{
	if (rank_type < GUILD_RANK_TYPE_LEVEL || rank_type >= GUILD_RANK_TYPE_MAX) return 0;

	if (NULL != rank_value_valid)
	{
		*rank_value_valid = true;
	}
	else
	{
		return 0;
	}

	long long rank_value = 0;

	switch (rank_type)
	{
	case GUILD_RANK_TYPE_LEVEL:
		rank_value = GuildLevelRankValueToLongLong(guild->GetGuildLevel(), guild->GetGuildRegister().GetGuildMemberCount(), static_cast<unsigned int>(guild->GetGuildCreateTime()));
		break;

	case GUILD_RANK_TYPE_KILL_WORLD_BOSS:
		rank_value = guild->GetKillWorldBossCount();
		if (rank_value <= 0)
		{
			*rank_value_valid = false;
		}
		break;

	case GUILD_RANK_TYPE_XIANMENG_BIPIN_KILL_WORLD_BOSS:
		rank_value = guild->GetXianmengBipinKillWorldBoss();
		if (rank_value <= 0)
		{
			*rank_value_valid = false;
		}
		break;

	case GUILD_RANK_TYPE_DAY_INCREASE_CAPABILITY:
		rank_value = guild->GetDayIncreaseCapability();
		if (rank_value <= 0)
		{
			*rank_value_valid = false;
		}
		break;

	case GUILD_RANK_TYPE_CAPABILITY:
		//rank_value = TranslateInt64(guild->GetCapability(), guild->GetGuildLevel());
		rank_value = TranslateInt64(guild->GetGuildMaxCapability(), guild->GetGuildLevel()); // 直接取每分钟公会缓存的战力
		if (rank_value <= 0)
		{
			*rank_value_valid = false;
		}
		break;

	case GUILD_RANK_TYPE_XIANMENGZHAN:
		rank_value = guild->GetXianmengzhanRankValue();
		if (rank_value <= 0)
		{
			*rank_value_valid = false;
		}
		break;

	case GUILD_RANK_TYPE_GUILDBATTLE:
		rank_value = guild->GetGuildBattleRank();
		if (rank_value <= 0)
		{
			*rank_value_valid = false;
		}
		else
		{
			rank_value = 1000 - rank_value;
		}
		break;

	case GUILD_RANK_TYPE_TERRITORYWAR:
		rank_value = guild->GetTerritoryWarRank();
		if (rank_value <= 0)
		{
			*rank_value_valid = false;
		}
		else
		{
			rank_value = 1000 - rank_value;
		}
		break;
	case GUILD_RANK_TYPE_KILL_CROSS_BOSS:
		rank_value = guild->GetKillCrossBossNum();
		if (rank_value <= 0)
		{
			*rank_value_valid = false;
		}
		break;
	}
	
	return rank_value;
}

void GuildRank::ReInitGuildRank(int rank_type)
{
	if (rank_type < GUILD_RANK_TYPE_LEVEL || rank_type >= GUILD_RANK_TYPE_MAX) return;
	
	this->ClearRank(rank_type);

	const GuildManager::GuildMap &guild_map = GuildManager::Instance().GetGuildMap();
	for (GuildManager::GuildMapConstIt it = guild_map.begin(), end = guild_map.end(); it != end; ++ it)
	{
		Guild *guild = it->second;
		if (NULL != guild)
		{
			bool rank_value_valid = true;
			long long rank_value = this->CalcGuildRankValue(guild, rank_type, &rank_value_valid);

			if (rank_value_valid)
			{
				this->OnGuildRankChange(guild, rank_type, rank_value);
			}
		}
	}
}

static int RankInfoSortCmp(const void *v1, const void *v2)
{
	GuildRankItem *item1 = *((GuildRankItem**)v1);
	GuildRankItem *item2 = *((GuildRankItem**)v2);

	if (NULL == item1 || NULL == item2) return -1;

	return item2->rank_value > item1->rank_value ? 1 : -1;
}

void GuildRank::OnGetRankList(Role *role, Protocol::CSGetGuildRankList &rank_list_req)
{
	if (NULL == role)
	{
		return;
	}

	int rank_type = rank_list_req.rank_type;

	if (rank_type < GUILD_RANK_TYPE_LEVEL || rank_type >= GUILD_RANK_TYPE_MAX) return;


	int begin_index = rank_type * GUILD_RANK_MAX_SIZE;
	int end_index = begin_index + GUILD_RANK_MAX_SIZE;

	static Protocol::SCGetGuildRankListAck rank_ack;

	rank_ack.rank_type = rank_type;
	rank_ack.rank_count = 0;

	int rank_count = 0;
	for (int i = begin_index; i < end_index && rank_count < GUILD_RANK_MAX_SIZE; ++i)
	{
		if (m_rank_snapshot[i].Invalid()) continue;

		rank_ack.rank_list[rank_count].guild_id = m_rank_snapshot[i].guild_id;
		rank_ack.rank_list[rank_count].tuan_zhang_uid = UidToInt(m_rank_snapshot[i].tuan_zhang_user_id);
		F_STRNCPY(rank_ack.rank_list[rank_count].guild_name, m_rank_snapshot[i].guild_name, sizeof(rank_ack.rank_list[0].guild_name));
		F_STRNCPY(rank_ack.rank_list[rank_count].tuan_zhang_name, m_rank_snapshot[i].tuan_zhang_name, sizeof(rank_ack.rank_list[0].tuan_zhang_name));
		rank_ack.rank_list[rank_count].guild_level = m_rank_snapshot[i].guild_level;
		rank_ack.rank_list[rank_count].camp = m_rank_snapshot[i].camp;
		rank_ack.rank_list[rank_count].member_count = m_rank_snapshot[i].member_count;
		rank_ack.rank_list[rank_count].max_member_count = m_rank_snapshot[i].max_member_count;
		rank_ack.rank_list[rank_count].rank_value = m_rank_snapshot[i].rank_value;

		++ rank_count;
	}


	rank_ack.rank_count = rank_count;

	unsigned int sendlen = sizeof(rank_ack) - (GUILD_RANK_MAX_SIZE - rank_ack.rank_count) * sizeof(rank_ack.rank_list[0]);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&rank_ack, sendlen);
}

int GuildRank::GetRankTopGuild(int rank_type, int max_guild, GuildID *guild_list)
{
	if (rank_type < 0 || rank_type >= GUILD_RANK_TYPE_MAX || NULL == guild_list) return 0;

	if (this->CanReInitGuildRank(rank_type))
	{
		this->ReInitGuildRank(rank_type);
	}

	int rank_start = rank_type * GUILD_RANK_MAX_SIZE;
	int rank_end =  rank_start + GUILD_RANK_MAX_SIZE;

	static GuildRankItem *temp_list[GUILD_RANK_MAX_SIZE] = {NULL};
	memset(temp_list, 0, sizeof(temp_list));

	int temp_list_size = 0;
	for (int i = rank_start; i < rank_end && temp_list_size < GUILD_RANK_MAX_SIZE; ++i)
	{
		if (!m_rank_list[i].Invalid())
		{
			temp_list[temp_list_size] = &m_rank_list[i];
			++ temp_list_size;
		}
	}

	qsort(temp_list, temp_list_size, sizeof(temp_list[0]), RankInfoSortCmp);

	int count = 0;
	for (int i = 0; i < temp_list_size && count < max_guild; ++i)
	{
		if (NULL != temp_list[i])
		{
			guild_list[count] = temp_list[i]->guild_id;
			++ count;
		}
	}

	return count;
}

int GuildRank::GetSnapshotTopGuild(int rank_type, int max_guild, GuildID *guild_list)
{
	if (rank_type < 0 || rank_type >= GUILD_RANK_TYPE_MAX || NULL == guild_list) return 0;

	int rank_start = rank_type * GUILD_RANK_MAX_SIZE;
	int rank_end = rank_start + GUILD_RANK_MAX_SIZE;

	int count = 0;
	for (int i = rank_start; i < rank_end && count < GUILD_RANK_MAX_SIZE && count < max_guild; ++i)
	{
		if (!m_rank_snapshot[i].Invalid())
		{
			guild_list[count] = m_rank_snapshot[i].guild_id;
			++count;
		}
	}

	return count;
}

void GuildRank::OnBaseInfoChange(Guild *guild)
{
	if (NULL == guild) return;

	for (int i = 0; i < GUILD_RANK_TYPE_MAX; ++i)
	{
		if (!this->CanReInitGuildRank(i))
		{
			GuildIndexMapIt it = m_guild_index_map[i].find(guild->GetGuildID());
			if (it != m_guild_index_map[i].end() && this->IsValidIndex(it->second))
			{
				this->UpdateBaseInfo(m_rank_list[it->second], guild);
			}
		}
	}
}

void GuildRank::ClearRank(int rank_type)
{
	if (rank_type < 0 || rank_type >= GUILD_RANK_TYPE_MAX) return;

	int rank_start = rank_type * GUILD_RANK_MAX_SIZE;
	int rank_end =  rank_start + GUILD_RANK_MAX_SIZE;

	for (int i = rank_start; i < rank_end; ++i)
	{
		m_rank_list[i].Reset();
	}

	m_guild_index_map[rank_type].clear();
}

int GuildRank::GetGuildRankInTopRank(GuildID guild_id, int rank_type, int max_rank)
{
	if (INVALID_GUILD_ID == guild_id || rank_type < 0 || rank_type >= GUILD_RANK_TYPE_MAX || max_rank <= 0) return 0;

	Guild *guild = GuildManager::Instance().GetGuild(guild_id);
	if (NULL == guild) return 0;

	bool rank_value_valid = true;
	long long rank_value = this->CalcGuildRankValue(guild, rank_type, &rank_value_valid);

	if (!rank_value_valid)
	{
		return 0;
	}

	int count = 0; int rank_pos = 0;

	int rank_start = rank_type * GUILD_RANK_MAX_SIZE;
	int rank_end = rank_start + GUILD_RANK_MAX_SIZE;

	for (int i = rank_start; i < rank_end; ++i)
	{
		if (!m_rank_snapshot[i].Invalid())
		{
			++count;

			if (m_rank_snapshot[i].rank_value > rank_value)
			{
				++rank_pos;
				if (rank_pos >= max_rank) return 0;
			}
		}
	}

	if (rank_pos >= count)
	{
		return 0;
	}

	return rank_pos + 1;
}

void GuildRank::OnGuildRankInfoChange(gglobalprotocal::SyncGuildRankInfo &rank_info)
{
	if (rank_info.guild_rank_type < 0 || rank_info.guild_rank_type >= GUILD_RANK_TYPE_MAX) return;

	if (GUILD_RANK_TYPE_XIANMENGZHAN == rank_info.guild_rank_type)
	{
		GuildManager::Instance().ClearXianmengzhanRankValue();

		for (int i = 0; i < rank_info.guild_rank_count && i < gglobalprotocal::SyncGuildRankInfo::MAX_GUILD_RANK_NUM; i++)
		{
			Guild *guild = GuildManager::Instance().GetGuild(rank_info.guild_rank_list[i].guild_id);
			if (NULL != guild)
			{
				guild->SetXianmengzhanRankValue(rank_info.guild_rank_list[i].rank_value);
			}
		}

		this->ReInitGuildRank(GUILD_RANK_TYPE_XIANMENGZHAN);
	}
	else if (GUILD_RANK_TYPE_GUILDBATTLE == rank_info.guild_rank_type)
	{
		GuildManager::Instance().ClearGuildBattleRank();

		for (int i = 0; i < rank_info.guild_rank_count && i < gglobalprotocal::SyncGuildRankInfo::MAX_GUILD_RANK_NUM; i++)
		{
			Guild *guild = GuildManager::Instance().GetGuild(rank_info.guild_rank_list[i].guild_id);
			if (NULL != guild)
			{
				guild->SetGuildBattleRank((int)rank_info.guild_rank_list[i].rank_value);
			}
		}

		this->ReInitGuildRank(GUILD_RANK_TYPE_GUILDBATTLE);
	}
	else if (GUILD_RANK_TYPE_TERRITORYWAR == rank_info.guild_rank_type)
	{
		GuildManager::Instance().ClearTerritoryWarRank();

		for (int i = 0; i < rank_info.guild_rank_count && i < gglobalprotocal::SyncGuildRankInfo::MAX_GUILD_RANK_NUM; i++)
		{
			Guild *guild = GuildManager::Instance().GetGuild(rank_info.guild_rank_list[i].guild_id);
			if (NULL != guild)
			{
				guild->SetTerritoryWarRank((int)rank_info.guild_rank_list[i].rank_value);
			}
		}

		this->ReInitGuildRank(GUILD_RANK_TYPE_TERRITORYWAR);
	}
	else
	{
		this->ClearRank(rank_info.guild_rank_type);

		for (int i = 0; i < rank_info.guild_rank_count && i < gglobalprotocal::SyncGuildRankInfo::MAX_GUILD_RANK_NUM; i++)
		{
			Guild *guild = GuildManager::Instance().GetGuild(rank_info.guild_rank_list[i].guild_id);
			if (NULL != guild)
			{
				this->OnGuildRankChange(guild, rank_info.guild_rank_type, rank_info.guild_rank_list[i].rank_value);
			}
		}
	}
}

void GuildRank::OnCrossRankInfoReq(crossgameprotocal::CrossGameRankInfoReq &cross_rank_info_req)
{
	int rank_type = cross_rank_info_req.rank_type;

	if (rank_type < GUILD_RANK_TYPE_LEVEL || rank_type >= GUILD_RANK_TYPE_MAX) return;


	int begin_index = rank_type * GUILD_RANK_MAX_SIZE;
	int end_index = begin_index + GUILD_RANK_MAX_SIZE;

	static Protocol::SCGetGuildRankListAck rank_ack;

	rank_ack.rank_type = rank_type;
	rank_ack.rank_count = 0;

	int rank_count = 0;
	for (int i = begin_index; i < end_index && rank_count < GUILD_RANK_MAX_SIZE; ++i)
	{
		if (m_rank_snapshot[i].Invalid()) continue;

		rank_ack.rank_list[rank_count].guild_id = m_rank_snapshot[i].guild_id;
		rank_ack.rank_list[rank_count].tuan_zhang_uid = UidToInt(m_rank_snapshot[i].tuan_zhang_user_id);
		F_STRNCPY(rank_ack.rank_list[rank_count].guild_name, m_rank_snapshot[i].guild_name, sizeof(rank_ack.rank_list[0].guild_name));
		F_STRNCPY(rank_ack.rank_list[rank_count].tuan_zhang_name, m_rank_snapshot[i].tuan_zhang_name, sizeof(rank_ack.rank_list[0].tuan_zhang_name));
		rank_ack.rank_list[rank_count].guild_level = m_rank_snapshot[i].guild_level;
		rank_ack.rank_list[rank_count].camp = m_rank_snapshot[i].camp;
		rank_ack.rank_list[rank_count].member_count = m_rank_snapshot[i].member_count;
		rank_ack.rank_list[rank_count].max_member_count = m_rank_snapshot[i].max_member_count;
		rank_ack.rank_list[rank_count].rank_value = m_rank_snapshot[i].rank_value;

		++ rank_count;
	}

	rank_ack.rank_count = rank_count;

	unsigned int sendlen = sizeof(rank_ack) - (GUILD_RANK_MAX_SIZE - rank_ack.rank_count) * sizeof(rank_ack.rank_list[0]);
	InternalComm::Instance().UserProtocalTransferToGame(cross_rank_info_req.uuid, (const char*)&rank_ack, sendlen);
}

void GuildRank::GuildRankChange(Guild *guild, int rank_type, long long rank_value)
{
	if (NULL != guild)
	{
		this->OnGuildRankChange(guild,rank_type,rank_value);
	}
}

void GuildRank::Snapshot()
{
	m_last_snapshot_time = EngineAdapter::Instance().Time();

	for (int i = 0; i < GUILD_RANK_TYPE_MAX * GUILD_RANK_MAX_SIZE; ++ i)
	{
		m_rank_snapshot[i].Reset();
	}

	for (int rank_type = 0; rank_type < GUILD_RANK_TYPE_MAX; ++ rank_type)
	{
		if (this->CanReInitGuildRank(rank_type))
		{
			this->ReInitGuildRank(rank_type);
		}

		int rank_start = rank_type * GUILD_RANK_MAX_SIZE;
		int rank_end =  rank_start + GUILD_RANK_MAX_SIZE;
		int temp_list_size = 0;
		static GuildRankItem *temp_list[GUILD_RANK_MAX_SIZE] = {NULL};
		memset(temp_list, 0, sizeof(temp_list));

		for (int i = rank_start; i < rank_end && temp_list_size < GUILD_RANK_MAX_SIZE; ++i)
		{
			if (!m_rank_list[i].Invalid())
			{
				temp_list[temp_list_size] = &m_rank_list[i];
				++ temp_list_size;
			}
		}

		qsort(temp_list, temp_list_size, sizeof(temp_list[0]), RankInfoSortCmp);

		for (int i = 0; i < temp_list_size && i < GUILD_RANK_MAX_SIZE && rank_start + i < rank_end; ++ i)
		{
			if (NULL != temp_list[i])
			{
				m_rank_snapshot[rank_start + i] = *temp_list[i];
			}
		}
	}
}

void GuildRank::OnGuildRankChange(Guild *guild, int rank_type, long long rank_value)
{
	// 1：如果找到相关的军团 直接更新
	// 2：如果找不到 还没满 直接加上去 如果满了 淘汰一个旧的

	if (rank_type < GUILD_RANK_TYPE_LEVEL || rank_type >= GUILD_RANK_TYPE_MAX) return;

	GuildIndexMapIt it = m_guild_index_map[rank_type].find(guild->GetGuildID());
	if (it != m_guild_index_map[rank_type].end() && this->IsValidIndex(it->second))
	{
		m_rank_list[it->second].tuan_zhang_user_id = guild->GetGuildRegister().GetGuildTuanZhangInfo(m_rank_list[it->second].tuan_zhang_name);
		m_rank_list[it->second].guild_level = guild->GetGuildLevel();
		m_rank_list[it->second].camp = guild->GetCamp();
		m_rank_list[it->second].member_count = guild->GetGuildRegister().GetGuildMemberCount();
		m_rank_list[it->second].max_member_count = guild->GetGuildMaxMemberCount();
		m_rank_list[it->second].rank_value = rank_value;
	}
	else 
	{
		int pos = -1;
		
		if (static_cast<int>(m_guild_index_map[rank_type].size()) >= GUILD_RANK_MAX_SIZE)
		{
			if (!this->GetSwapRankItemSlot(rank_type, rank_value, &pos)) return;
			m_guild_index_map[rank_type].erase(m_rank_list[pos].guild_id);
		}
		else 
		{
			if (!this->GetFreeRankItemSlot(rank_type, &pos)) return;
		}

		if (pos >= rank_type * GUILD_RANK_MAX_SIZE && pos < (rank_type + 1) * GUILD_RANK_MAX_SIZE)
		{
			m_rank_list[pos].guild_id = guild->GetGuildID();
			guild->GetGuildName(m_rank_list[pos].guild_name);
			m_rank_list[pos].tuan_zhang_user_id = guild->GetGuildRegister().GetGuildTuanZhangInfo(m_rank_list[pos].tuan_zhang_name);
			m_rank_list[pos].guild_level = guild->GetGuildLevel();
			m_rank_list[pos].camp = guild->GetCamp();
			m_rank_list[pos].member_count = guild->GetGuildRegister().GetGuildMemberCount();
			m_rank_list[pos].max_member_count = guild->GetGuildMaxMemberCount();

			m_rank_list[pos].rank_type = rank_type;
			m_rank_list[pos].rank_value = rank_value;

			m_guild_index_map[rank_type][guild->GetGuildID()] = pos;
		}
	}
}

bool GuildRank::GetSwapRankItemSlot(int rank_type, long long rank_value, int *swap_pos)
{
	if (rank_type < 0 || rank_type >= GUILD_RANK_TYPE_MAX) return false;

	int begin_index = rank_type * GUILD_RANK_MAX_SIZE;
	int end_index = (rank_type + 1) * GUILD_RANK_MAX_SIZE;

	int min_pos = begin_index;
	for (int i = begin_index + 1; i < end_index; i++)
	{
		if (m_rank_list[i].rank_value < m_rank_list[min_pos].rank_value)
		{
			min_pos = i;
		} 
	}

	if (m_rank_list[min_pos].rank_value < rank_value)
	{
		if (NULL != swap_pos) *swap_pos = min_pos;
		return true;
	}

	return false;
}

bool GuildRank::GetFreeRankItemSlot(int rank_type, int *free_pos)
{
	if (rank_type < 0 || rank_type >= GUILD_RANK_TYPE_MAX) return false;

	int begin_index = rank_type * GUILD_RANK_MAX_SIZE;
	int end_index = (rank_type + 1) * GUILD_RANK_MAX_SIZE;

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

void GuildRank::UpdateBaseInfo(GuildRankItem &rank_item, Guild *guild)
{
	guild->GetGuildName(rank_item.guild_name);
	rank_item.tuan_zhang_user_id = guild->GetGuildRegister().GetGuildTuanZhangInfo(rank_item.tuan_zhang_name);
	rank_item.guild_level = guild->GetGuildLevel();
	rank_item.camp = guild->GetCamp();
	rank_item.member_count = guild->GetGuildRegister().GetGuildMemberCount();
	rank_item.max_member_count = guild->GetGuildMaxMemberCount();
}
