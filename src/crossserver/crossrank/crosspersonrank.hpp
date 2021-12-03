#ifndef __CROSS_PERSON_RANK_DEF_HPP__
#define __CROSS_PERSON_RANK_DEF_HPP__

#include <map>
#include <set>

#include "servercommon/struct/global/rankparam.hpp"

#include "servercommon/rankdef.hpp"

#include "servercommon/crossdef.hpp"
#include "servercommon/struct/cross/crossstatusparam.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"

class CrossServer;

//屏蔽保存跨服排行榜数据,因为经常调整战区导致数据混乱
#define __CROSS_PERSON_RANK_SHIELD_SAVE_RANK

struct CrossrPersonRankItem
{
	CrossrPersonRankItem() : plat_type(0), role_id(0), level(0), prof(0), sex(0), camp(0), jingjie(0),
		exp(0), rank_type(-1), rank_value(0), record_index(0), flexible_ll(0), reserved(0), vip_level(0), flexible_int(0)
	{
		memset(name, 0, sizeof(name));
		memset(flexible_name, 0, sizeof(flexible_name));
	}

	bool Invalid() { return -1 == rank_type; }
	void Reset() { rank_type = -1; }

	int plat_type;
	int role_id;
	GameName name;
	int level;
	char prof;
	char sex;
	char camp;
	char jingjie;
	long long exp;
	int rank_type;
	long long rank_value;
	int record_index;
	long long flexible_ll;
	long long reserved;
	GameName flexible_name;
	int vip_level;
	int flexible_int;
};

class CrossPersonRank
{
	friend class RMIInitCrossPersonRankBackObjectImpl;

public:
	CrossPersonRank();
	~CrossPersonRank();

	void SetCrossServer(CrossServer *cross_server) { m_cross_server = cross_server; }
	CrossServer * GetCrossServer() { return m_cross_server; }

	void OnServerStart();
	void OnServerStop();

	bool IsLoadFinish() const { return m_rank_list_load_flag && m_rank_title_load_flag; }

	void InitRankTitleParam(const CrossPersonRankTitleParam &cross_personrank_title_param);
	void GetRankTitleParam(CrossPersonRankTitleParam *cross_personrank_title_param);

	void Init(const CrossPersonRankParam &cross_personrank_param);
	void Update(unsigned long interval, time_t now_second);
	void OnDayChange();
	void OnWeekChange();
	void OnSeasonChange(int cross_rank_top_tpye);					// 用于1v1、3v3赛季

	void OnRegisterGameServer(NetID global_netid);

	void GetChangeRankItemList(CrossPersonRankParam *cross_personrank_param);
	void ClearDirtyMark();

	void OnGetRankList(NetID global_netid, const UniqueUserID &unique_user_id, int rank_type);		// 获取排行榜信息

	void OnRankInfoChange(crossgameprotocal::GameCrossSyncCrossPersonRankInfo &rank_info);

	void ClearRank(int rank_type);
	void ClearAllRank();

	int GetRankTopUser(int rank_type, int max_user, long long *user_list);

	void SyncRankTitleInfoToGameWorld(int cross_top_rank_type, NetID global_netid = UINT_MAX);

	long long GetRankValue(int rank_type, long long uuid);
	void Cross1v1UpdateRankList(CrossrPersonRankItem &rank_item);

	const CrossrPersonRankItem *GetRankItem(int rank_type, long long uuid);
	void UpdateHiddenWorldLevel();
	bool IsGuildRank(int rank_type);
	void GmRankReward(int rank_type, bool clear_rank);
private:
	bool IsValidIndex(int index) { return index >= 0 && index < CROSS_PERSON_RANK_TYPE_MAX * CROSS_PERSON_RANK_MAX_SIZE; }

	void UpdateRankList(CrossrPersonRankItem &rank_item);
	void UpdateBaseInfo(CrossrPersonRankItem &rank_item);
	bool GetSwapRankItemSlot(int rank_type, const CrossrPersonRankItem &rank_item, int *swap_post);
	bool GetFreeRankItemSlot(int rank_type, int *free_pos);
	void Snapshot();

	bool AddUserIndex(int rank_type, const long long uuid, int rank_index);
	bool RemoveUserIndex(int rank_type, const long long uuid, int rank_index);
	bool GetUserRecordIndex(int rank_type, const long long uuid, int record_index, int *rank_index);
	int GetRankTypeSize(int rank_type);

	void RefreshRankTitle(int top_rank_type);
	
	void SaveRankList();
	
	void GiveWeekCharmRankReward();

	void CalcNextRankRewardTime();
	void GiveRankReward(int rank_type);					// 下发物品奖励
	void SyncRankFirstFiveToGameWorld(int rank_type);
	void SyncGuildRankFirstFiveToGameWorld(int rank_type);

	CrossServer *m_cross_server;

	CrossrPersonRankItem m_rank_list[CROSS_PERSON_RANK_TYPE_MAX * CROSS_PERSON_RANK_MAX_SIZE];

	bool m_dirty_mark[CROSS_PERSON_RANK_TYPE_MAX * CROSS_PERSON_RANK_MAX_SIZE];
	bool m_old_state[CROSS_PERSON_RANK_TYPE_MAX * CROSS_PERSON_RANK_MAX_SIZE];
	bool m_change_flag;
	bool m_rank_list_load_flag;

	typedef std::map<long long, std::set<int> > UserIndexMap;
	typedef std::map<long long, std::set<int> >::iterator UserIndexMapIt;

	UserIndexMap m_user_index_map[CROSS_PERSON_RANK_TYPE_MAX];

	time_t m_last_snapshot_time;
	CrossrPersonRankItem m_rank_snapshot[CROSS_PERSON_RANK_TYPE_MAX * CROSS_PERSON_RANK_MAX_SIZE];

	time_t m_rank_data_next_save_time;
	time_t m_rank_data_next_update_time;

	CrossPersonRankTitleUuidInfo m_rank_title_uuid_info_list[CROSS_RANK_TOP_TYPE_MAX]; 
	bool m_rank_title_load_flag;

	time_t m_next_rank_reward_time_list[CROSS_PERSON_RANK_TYPE_MAX];
};

#endif // __CROSS_RANK_DEF_HPP__

