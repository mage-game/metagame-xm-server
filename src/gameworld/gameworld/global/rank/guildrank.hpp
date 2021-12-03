/******************************************************************************************
FileName: guildrank.hpp
Author: 
Description: 军团排行榜  
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreanwang    2011/08/26/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __GUILD_RANK_DEF_HPP__
#define __GUILD_RANK_DEF_HPP__

#include "servercommon/serverdef.h"
#include "servercommon/rankdef.hpp"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"

#include <map>

namespace Protocol
{
	class CSGetGuildRankList;
}

class RankManager;
class Guild;
class Role;

struct GuildRankItem
{
	GuildRankItem() : guild_id(INVALID_GUILD_ID), guild_level(0), camp(0), member_count(0), max_member_count(0), rank_type(0), rank_value(0) 
	{
		memset(guild_name, 0, sizeof(guild_name));
		memset(tuan_zhang_name, 0, sizeof(tuan_zhang_name));
	}

	bool Invalid() { return INVALID_GUILD_ID == guild_id; }
	void Reset() { guild_id = INVALID_GUILD_ID; }

	GuildID guild_id;
	GuildName guild_name;
	UserID tuan_zhang_user_id;
	GameName tuan_zhang_name;
	short guild_level;
	short camp;
	short member_count;
	short max_member_count;
	int rank_type;
	long long rank_value;
};

class GuildRank
{
public:
	GuildRank();
	~GuildRank();

	void SetRankManager(RankManager *rank_manager) { m_rank_manager = rank_manager; }

	void Update(time_t now_second);

	// 从快照中取公会排行列表
	void OnGetRankList(Role *role, Protocol::CSGetGuildRankList &rank_list_req);

	// 从最新数据里取前max_guild条公会，注意，会立即对排行榜进行排序
	int GetRankTopGuild(int rank_type, int max_guild, GuildID *guild_list);

	// 从快照里取前max_guild条公会
	int GetSnapshotTopGuild(int rank_type, int max_guild, GuildID *guild_list);

	void OnBaseInfoChange(Guild *guild);

	void ClearRank(int rank_type);

	int GetGuildRankInTopRank(GuildID guild_id, int rank_type, int max_rank);

	void OnGuildRankInfoChange(gglobalprotocal::SyncGuildRankInfo &rank_info);

	void OnCrossRankInfoReq(crossgameprotocal::CrossGameRankInfoReq &cross_rank_info_req);
	void GuildRankChange(Guild *guild, int rank_type, long long rank_value);

	void Snapshot();

	// 更新所有公会排行榜数据
	void ReInitGuildRank(int rank_type);

private:
	bool IsValidIndex(int index) { return index >= 0 && index < GUILD_RANK_TYPE_MAX * GUILD_RANK_MAX_SIZE; }

	bool CanReInitGuildRank(int rank_type);
	long long CalcGuildRankValue(Guild *guild, int rank_type, bool *rank_value_valid);

	// 更新排行榜数据（插入、删除、更新）。该函数内没有对排行榜进行排序
	void OnGuildRankChange(Guild *guild, int rank_type, long long rank_value);
	
	bool GetSwapRankItemSlot(int rank_type, long long rank_value, int *pos);
	bool GetFreeRankItemSlot(int rank_type, int *pos);

	void UpdateBaseInfo(GuildRankItem &rank_item, Guild *guild);

	RankManager *m_rank_manager;

	GuildRankItem m_rank_list[GUILD_RANK_TYPE_MAX * GUILD_RANK_MAX_SIZE];

	typedef std::map<GuildID, int> GuildIndexMap;
	typedef std::map<GuildID, int>::iterator GuildIndexMapIt;

	GuildIndexMap m_guild_index_map[GUILD_RANK_TYPE_MAX];

	time_t m_last_snapshot_time;
	GuildRankItem m_rank_snapshot[GUILD_RANK_TYPE_MAX * GUILD_RANK_MAX_SIZE];
};

#endif  // __GUILD_RANK_DEF_HPP__
