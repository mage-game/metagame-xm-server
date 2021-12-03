#ifndef __CROSS_RAND_ACTIVITY_CLOUD_PURCHASE_HPP__
#define __CROSS_RAND_ACTIVITY_CLOUD_PURCHASE_HPP__

#include "crossrandactivity/crossrandactivity.hpp"

#include <vector>

namespace crossgameramsg {
	struct GCCloudPurchaseSyncData;
	struct GCCloudPurchaseSycAllInfo;
}

class CrossRandActivityCloudPurchase : public CrossRandActivity
{	
	struct PurchaseDataItem
	{
		PurchaseDataItem() : global_netid(0), server_id(0), merge_server_id(0), uid(0)
		{
			memset(name, 0, sizeof(name));
		}

		~PurchaseDataItem() {}

		NetID global_netid;

		int server_id;			// 原服id
		int merge_server_id;	// 合服id
		int uid;				// 玩家id
		GameName name;			// 玩家名字
	};

public:
	CrossRandActivityCloudPurchase();
	virtual ~CrossRandActivityCloudPurchase();

	virtual void Update(unsigned int now_second);
	virtual void OnRegisterGameServer(NetID global_netid);

	virtual void OnMessageFromGameworld(NetID global_netid, int msg_type, char *data);
	void OnSyncRankData(NetID global_netid, crossgameramsg::GCCloudPurchaseSyncData *czrd);
	void OnSycAllServerInfo(NetID global_netid, crossgameramsg::GCCloudPurchaseSycAllInfo *all_data);

protected:
	void OnActivityStatusChange(int from_status, int to_status);

	unsigned int m_check_givereward_timestamp_list[CLOUD_PURCHASE_ITEM_COUNT_MAX];
	std::vector<PurchaseDataItem> m_purchase_data_vec_list[CLOUD_PURCHASE_ITEM_COUNT_MAX];
};

#endif // __CROSS_RAND_ACTIVITY_CLOUD_PURCHASE_HPP__

