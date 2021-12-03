#pragma once
#ifndef __COUPLE_RANK_HPP__
#define __COUPLE_RANK_HPP__

#include "servercommon/serverdef.h"
#include "servercommon/rankdef.hpp"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include <map>

namespace Protocol
{
	class CSGetCoupleRankList;
};

struct CoupleRankInfo;

struct CoupleRankItem
{
	CoupleRankItem() : rank_type(-1), male_uid(0), female_uid(0), male_rank_value(0), female_rank_value(0), flexible_int(0), flexible_ll(0)
	{
		memset(male_name, 0, sizeof(male_name));
		memset(female_name, 0, sizeof(female_name));
	}

	bool Invalid() { return -1 == rank_type; }

	void Reset()
	{
		rank_type = -1;
	}

	int rank_type;
	int male_uid;
	int female_uid;
	GameName male_name;
	GameName female_name;
	long long male_avatar;
	long long female_avatar;
	char male_prof;
	char female_prof;
	short reserve_sh;
	int male_rank_value;
	int female_rank_value;
	int flexible_int;
	long long flexible_ll;
};

class CoupleRankManager
{
public:
	static CoupleRankManager & Instance();

	void Update(unsigned long interval, time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void OnGetRankList(Role *role, Protocol::CSGetCoupleRankList &rank_list_req);
	void OnDivorce(Role *role);

	void ClearRank(int rank_type);
	void OnRankInfoChange(const SyncCoupleRankInfo &rank_info);
	void Snapshot();

protected:
	CoupleRankManager();
	~CoupleRankManager();

	void DeleteUserFromRank(int uid);
	long long GetRankKey(CoupleRankItem &couple_rank_item);
	int GetInsertSlot(int rank_type);
	void OnTopChange();
	void AfterSnapshot();
	void SyncRankListToCross(int rank_type); 

	int GetCrossRankTypeToGameWorldRankType(int game_world_rank_type);

private:

	time_t m_last_snapshot_timestamp;

	CoupleRankItem m_rank_list[COUPLE_RANK_TYPE_MAX * COUPLE_RANK_MAX_SIZE];
	CoupleRankItem m_rank_snapshot_list[COUPLE_RANK_TYPE_MAX * COUPLE_RANK_MAX_SIZE];

	typedef std::map<long long, int> CoupleRankIndexMap;
	typedef std::map<long long, int>::iterator CoupleRankIndexMapIt;

	CoupleRankIndexMap m_couple_index_map_list[COUPLE_RANK_TYPE_MAX];

	struct TopItem
	{
		int male_uid;
		int female_uid;
	};

	TopItem m_top_item_list[COUPLE_RANK_TYPE_MAX * COUPLE_RANK_TOP_COUNT];
};

#endif

