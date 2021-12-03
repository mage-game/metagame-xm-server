/******************************************************************************************
FileName: rankmanager.hpp
Author: 
Description: 排行榜管理 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreanwang    2011/08/26/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __RANK_MANAGER_DEF_HPP__
#define __RANK_MANAGER_DEF_HPP__

#include "personrank.hpp"
#include "teamrank.hpp"
#include "guildrank.hpp"


class RankManager
{
	friend class RMIInitPersonRankBackObjectImpl;
	friend class RMIInitTeamRankBackObjectImpl;

public:
	static RankManager & Instance();

	bool IsLoadFinish();

	PersonRank * GetPersonRank() { return &m_person_rank; }
	//TeamRank * GetTeamRank() { return &m_team_rank; }
	GuildRank * GetGuildRank() { return &m_guild_rank; }

	void OnServerStart();
	void OnServerStop();

	void Update(unsigned long interval, time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnWeekChange();

	void OnUserLogin(Role *user);

	void OnGetPersonRankList(Role *user, Protocol::CSGetPersonRankList *rank_list_req);
	void OnGetPersonRankTopUser(Role *user, Protocol::CSGetPersonRankTopUser *top_user_req);
	void SyncPersonBaseInfo(Role *user);
	void OnSyncPersonRankInfo(const UserID &user_id, SyncPersonRankInfo *person_rank_info);

	void OnGetGuildRankList(Role *user, Protocol::CSGetGuildRankList *rank_list_req);

	void OnGetTeamRankList(Role *user, Protocol::CSGetTeamRankList *rank_list_req);
	void SyncTeamRankInfo(gglobalprotocal::SyncTeamRankInfo *team_rank_info);

	void SyncPersonRankBattleFieldRankInfo(gglobalprotocal::SyncLastBattleFieldRankInfo *last_battlefield_rank_info);
	void SyncGuildRankInfo(gglobalprotocal::SyncGuildRankInfo *guild_rank_info);

	void OnCrossRankInfoReq(crossgameprotocal::CrossGameRankInfoReq *cross_rank_info_req);

	void Snapshot();
	void ClearPersonRankType(int rank_type);
	void ClearGuildRankType(int rank_type);

private:
	RankManager();
	~RankManager();

	void LoadPersonRankSucc();
	void LoadTeamRankSucc();
	void LoadAllRankSucc();

	void Save();

	/*void HonourRankReward();
	void Rank1v1Reward();*/

	PersonRank m_person_rank;
	//TeamRank m_team_rank;
	GuildRank m_guild_rank;

	time_t m_rank_data_next_save_time;
};

#endif  // __RANK_MANAGER_DEF_HPP__

