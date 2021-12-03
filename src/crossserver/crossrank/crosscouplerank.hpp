#ifndef __CROSS_COUPLE_RANK_DEF_HPP__
#define __CROSS_COUPLE_RANK_DEF_HPP__

#include "servercommon/rankdef.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include <unordered_map>

class CrossServer;

struct CrossrCoupleRankItem
{
	CrossrCoupleRankItem()
	{
		this->Reset();
	}

	CrossrCoupleRankItem(int _rank_type, int _plat_type, const crossgameprotocal::GameCrossSyncCrossCoupleRankInfo::RankItem &_rank_item)
		: rank_type(_rank_type), plat_type(_plat_type), rank_item(_rank_item)
	{}

	int rank_type;				// 排行榜类型
	int plat_type;				// 平台
	crossgameprotocal::GameCrossSyncCrossCoupleRankInfo::RankItem rank_item;

	void Reset()
	{
		rank_type = -1;
		plat_type = 0;
		memset(&rank_item, 0, sizeof(rank_item));
	}

	bool IsValid()
	{
		return rank_item.male_uid > 0;
	}
};

class CrossCoupleRank
{
public:
	CrossCoupleRank();
	~CrossCoupleRank() = default;

	void SetCrossServer(CrossServer *cross_server) { m_cross_server = cross_server; }
	CrossServer * GetCrossServer() { return m_cross_server; }

	void OnServerStart();

	void OnDayChange();
	void CalcNextRankRewardTime();

	void OnRegisterGameServer(NetID global_netid);

	void OnSyncGameWorldRankList(crossgameprotocal::GameCrossSyncCrossCoupleRankInfo *pro);
	void OnGetCoupleRankList(NetID global_netid, const UniqueUserID &unique_user_id, int rank_type);
	void GmGiveRankReward(int rank_type, bool clear_rank);
	void OnDivorce(int plat_type, int uid);
	void GmFakeUser(int rank_type);

	static int GetCrossRankTypeToGameWorldRankType(int game_world_rank_type);
	static int GetGameWorldRankTypeToCrossRankType(int cross_world_rank_type);
private:

	void ForceSnap(NetID net_id = UINT_MAX);
	void Merge(crossgameprotocal::GameCrossSyncCrossCoupleRankInfo *pro);

	void GiveRankReward(int rank_type);
	void ClearRank(int rank_type);

	void SyncRankFirstUidToGameWorld(int rank_type);

	CrossServer *m_cross_server;

	time_t m_next_rank_reward_time[CROSS_COUPLE_RANK_TYPE_MAX];
	CrossrCoupleRankItem m_rank_list[CROSS_COUPLE_RANK_TYPE_MAX * CROSS_COUPLE_RANK_MAX_SIZE];

	std::unordered_map<long long, CrossrCoupleRankItem> m_rank_user_dict_list[CROSS_COUPLE_RANK_TYPE_MAX];
};

#endif // !__CROSS_COUPLE_RANK_DEF_HPP__

