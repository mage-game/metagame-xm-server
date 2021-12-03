/******************************************************************************************
FileName: teamrank.hpp
Author: 
Description: 组队排行榜  
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreanwang    2011/08/26/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __TEAM_RANK_HPP__
#define __TEAM_RANK_HPP__

#include <map>
#include <set>

#include "servercommon/struct/global/rankparam.hpp"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "protocal/msgcsrank.h"
#include "protocal/msgscrank.h"
#include "servercommon/activitydef.hpp"

class RankManager;
class Role;

struct TeamRankItem
{
	TeamRankItem() : rank_type(-1), rank_value(0), flexible_int(0), flexible_ll(0) { }

	bool Invalid() { return (rank_type < 0); }

	void Reset()
	{
		for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++ i) member_list[i].Reset();
		rank_type = -1; rank_value = 0; flexible_int = 0; flexible_ll = 0;
	}

	TeamRankMemberInfo member_list[MAX_TEAM_MEMBER_NUM];
	int rank_type;
	long long rank_value;
	int flexible_int;
	long long flexible_ll;
};

class TeamRank
{
public:
	TeamRank();
	~TeamRank();

	void SetRankManager(RankManager *rank_manager) { m_rank_manager = rank_manager; }

	void Init(const TeamRankParam &teamrank_param);

	bool IsRankLoadFinish() { return m_load_flag; } 

	void Update(time_t now_second);

	void GetChangeRankItemList(TeamRankParam *teamrank_param);
	void ClearDirtyMark();

	void ClearRank(int rank_type);

	void OnGetRankList(Role *role, Protocol::CSGetTeamRankList &rank_list_req);
	void OnRankInfoChange(gglobalprotocal::SyncTeamRankInfo &rank_info);

	void UserResetName(const UserID &user_id, GameName name);

private:
	bool IsValidIndex(int index) { return index >= 0 && index < TEAM_RANK_TYPE_MAX * TEAM_RANK_MAX_SIZE; }

	static int TeamRankItemSortCompare(const TeamRankItem &item1, const TeamRankItem &item2);

	bool GetRankItemIndex(int rank_type, int member_uid_list[MAX_TEAM_MEMBER_NUM], int *rank_index);
	int GetRankTypeSize(int rank_type);

	void UpdateRankList(TeamRankItem &rank_item);
	bool GetSwapRankItemSlot(int rank_type, const TeamRankItem &rank_item, int *swap_post);
	bool GetFreeRankItemSlot(int rank_type, int *free_post);

	void Snapshot();
	
	RankManager *m_rank_manager;
	TeamRankItem m_rank_list[TEAM_RANK_TYPE_MAX * TEAM_RANK_MAX_SIZE];

	bool m_dirty_mark[TEAM_RANK_TYPE_MAX * TEAM_RANK_MAX_SIZE];
	bool m_old_state[TEAM_RANK_TYPE_MAX * TEAM_RANK_MAX_SIZE];
	bool m_change_flag;
	bool m_load_flag;

	time_t m_last_snapshot_time;
	TeamRankItem m_rank_snapshot[TEAM_RANK_TYPE_MAX * TEAM_RANK_MAX_SIZE];
};

#endif  // __TEAM_RANK_HPP__

