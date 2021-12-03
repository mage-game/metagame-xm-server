#ifndef __CROSS_RAND_ACTIVITY_CONSUME_RANK_HPP__
#define __CROSS_RAND_ACTIVITY_CONSUME_RANK_HPP__

#include "crossrandactivity/crossrandactivity.hpp"

#include <vector>

namespace crossgameramsg {
	struct GCConsumeRankSyncData;
}

class CrossRandActivityConsumeRank : public CrossRandActivity
{	
	struct RankDataItem
	{
		RankDataItem() : global_netid(0), plat_type(0), merge_server_id(0), total_consume(0), mvp_server_id(0)
		{
			memset(mvp_name, 0, sizeof(mvp_name));
		}

		~RankDataItem() {}

		bool operator <(const RankDataItem &other) const
		{
			return total_consume > other.total_consume;
		}

		NetID global_netid;

		int plat_type;													// 平台类型
		int merge_server_id;											// 当前服的服ID

		long long total_consume;										// 全服总充值

		GameName mvp_name;
		int mvp_server_id;
	};

public:
	CrossRandActivityConsumeRank();
	virtual ~CrossRandActivityConsumeRank();

	virtual void Update(unsigned int now_second);

	virtual void OnMessageFromGameworld(NetID global_netid, int msg_type, char *data);
	void OnSyncRankData(NetID global_netid, crossgameramsg::GCConsumeRankSyncData *czrd);
	void OnGetRankList(NetID global_netid, const UniqueUserID &unique_user_id,int modify_id);

protected:
	void OnActivityStatusChange(int from_status, int to_status);

	int m_modify_id;
	std::vector<RankDataItem> m_rank_list;
};

#endif // __CROSS_ACTIVITY_HPP__

