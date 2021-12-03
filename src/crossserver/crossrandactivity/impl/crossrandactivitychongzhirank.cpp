#include "crossrandactivitychongzhirank.hpp"
#include "servercommon/internalprotocal/crossgamerandactivitymsg.h"
#include "protocal/msgcross.h"
#include "crossserver.h"
#include "crosslog.hpp"
#include <algorithm>

CrossRandActivityChongzhiRank::CrossRandActivityChongzhiRank() : CrossRandActivity(CROSS_RAND_ACTIVITY_TYPE_CHONGZHI_RANK) 
{

}

CrossRandActivityChongzhiRank::~CrossRandActivityChongzhiRank()
{

}

void CrossRandActivityChongzhiRank::Update(unsigned int now_second)
{
	CrossRandActivity::Update(now_second);

	if (!m_is_init_finish) return;
}

void CrossRandActivityChongzhiRank::OnMessageFromGameworld(NetID global_netid, int msg_type, char *data)
{
	if (crossgameramsg::MSG_CHONGZHI_RANK_SYNC_DATA_GC == msg_type)
	{
		this->OnSyncRankData(global_netid, (crossgameramsg::GCChongzhiRankSyncData *)data);
	}
}

void CrossRandActivityChongzhiRank::OnSyncRankData(NetID global_netid, crossgameramsg::GCChongzhiRankSyncData *czrd)
{
	std::vector<RankDataItem>::iterator invalid_it = m_rank_list.end(); // 存放无效数据迭代器

	bool has_found = false;
	for (std::vector<RankDataItem>::iterator it = m_rank_list.begin(); it != m_rank_list.end(); ++ it)
	{
		if (it->plat_type == czrd->plat_type && it->merge_server_id == czrd->merge_server_id)
		{
			it->global_netid = global_netid;

			it->total_chongzhi = czrd->total_chongzhi;
			it->mvp_server_id = czrd->mvp_server_id;
			F_STRNCPY(it->mvp_name, czrd->mvp_name, sizeof(it->mvp_name));

			has_found = true;
			break;
		}
		else if(it->global_netid == global_netid) // serverid不一样，netid却一样，说明旧的已经失效
		{
			it->total_chongzhi = 0;
			invalid_it = it;
		}
	}

	if (invalid_it != m_rank_list.end())
	{
		m_rank_list.erase(invalid_it);
	}

	if (!has_found)
	{
		RankDataItem item;
		item.global_netid = global_netid;
		item.plat_type = czrd->plat_type;
		item.merge_server_id = czrd->merge_server_id;

		item.total_chongzhi = czrd->total_chongzhi;

		F_STRNCPY(item.mvp_name, czrd->mvp_name, sizeof(item.mvp_name));
		item.mvp_server_id = czrd->mvp_server_id;

		m_rank_list.push_back(item);
	}
}

void CrossRandActivityChongzhiRank::OnGetRankList(NetID global_netid, const UniqueUserID &unique_user_id)
{
	static Protocol::SCCrossRAChongzhiRankGetRankACK mcgra;
	mcgra.rank_count = 0;
	for (std::vector<RankDataItem>::iterator it = m_rank_list.begin(); it != m_rank_list.end() && mcgra.rank_count < CROSS_RA_CHONGZHI_RANK_MAX_NUM; ++ it)
	{
		Protocol::SCCrossRAChongzhiRankGetRankACK::RankItem &rank_item = mcgra.rank_list[mcgra.rank_count];
		rank_item.total_chongzhi = it->total_chongzhi;
		F_STRNCPY(rank_item.mvp_name, it->mvp_name, sizeof(rank_item.mvp_name));
		rank_item.mvp_server_id = it->mvp_server_id;
		rank_item.mvp_plat_type = it->plat_type;

		++ mcgra.rank_count;
	}
	int sendlen = sizeof(mcgra) - sizeof(mcgra.rank_list[0]) * (CROSS_RA_CHONGZHI_RANK_MAX_NUM - mcgra.rank_count);
	CrossServer::GetInstCrossServer()->SendToUser(global_netid, unique_user_id.user_id, (const char*)&mcgra, sendlen);
}

void CrossRandActivityChongzhiRank::OnActivityStatusChange(int from_status, int to_status)
{
	switch (to_status)
	{
	case ACTIVITY_STATUS_OPEN:
		{

		}
		break;

	case ACTIVITY_STATUS_CLOSE:
		{	
			// 结算
			std::sort(m_rank_list.begin(), m_rank_list.end());
			int rank = 1;
			for (std::vector<RankDataItem>::iterator it = m_rank_list.begin(); it != m_rank_list.end(); ++ it)
			{
				static crossgameramsg::CGChongzhiRankCheckReward czrcr;
				czrcr.rank = rank;
				this->SendToGameworldActivity((char*)&czrcr, sizeof(czrcr), it->global_netid);

				crosslog::g_log_cross_rand_activity.printf(LL_INFO, "CrossRandActivityChongzhiRank SendReward : rank[%d] plat_type[%d] server_id[%d] total_chongzhi[%lld] mvp_name[%s]", 
					rank, it->plat_type, it->merge_server_id, it->total_chongzhi, it->mvp_name);

				++ rank;
			}
			m_rank_list.clear();
		}
		break;
	}
}

