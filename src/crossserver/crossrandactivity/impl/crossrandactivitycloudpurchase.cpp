#include "crossrandactivitycloudpurchase.hpp"

#include "servercommon/internalprotocal/crossgamerandactivitymsg.h"
#include "protocal/msgcross.h"
#include "crossserver.h"
#include "crosslog.hpp"
#include "servercommon/serverconfig/cloudpurchaseconfig.hpp"
#include "servercommon/string/crossstr.h"

CrossRandActivityCloudPurchase::CrossRandActivityCloudPurchase() : CrossRandActivity(CROSS_RAND_ACTIVITY_TYPE_CLOUD_PURCHASE)
{
	memset(m_check_givereward_timestamp_list, 0, sizeof(m_check_givereward_timestamp_list));

	for (int i = 0; i < CLOUD_PURCHASE_ITEM_COUNT_MAX; i ++)
	{
		m_purchase_data_vec_list[i].clear();
	}
}

CrossRandActivityCloudPurchase::~CrossRandActivityCloudPurchase()
{

}

void CrossRandActivityCloudPurchase::Update(unsigned int now_second)
{
	CrossRandActivity::Update(now_second);

	if (!m_is_init_finish) return;

	for (int i = 0; i < CLOUD_PURCHASE_ITEM_COUNT_MAX; i ++)
	{
		const CloudPurchaseItemCfg *cfg = CloudPurchaseConfig::Instance().GetItemCfg(i);
		if (NULL == cfg)
		{
			continue;
		}

		if (0 != m_check_givereward_timestamp_list[i] && now_second >= m_check_givereward_timestamp_list[i])
		{
			m_check_givereward_timestamp_list[i] = 0;

			int size = (int)m_purchase_data_vec_list[i].size();
			if (size > 0)
			{
				int reward_index = RandomNum(size);

				const PurchaseDataItem &item = m_purchase_data_vec_list[i].at(reward_index);

				static crossgameramsg::CGCloudPurchaseGiveReward cpgr;
				cpgr.type = i;
				cpgr.uid = item.uid;
				cpgr.server_id = item.server_id;
				F_STRNCPY(cpgr.name, item.name, sizeof(cpgr.name));

				this->SendToGameworldActivity((char*)&cpgr, sizeof(cpgr));

				{
					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_ra_cloud_reward_notic,
						item.name, item.server_id, CROSS_RAND_ACTIVITY_TYPE_CLOUD_PURCHASE, cfg->reward_item.item_id);
					if (length > 0)
					{
						CrossServer::GetInstCrossServer()->SystemMsgToAllGlobalUser(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
					}
				}

				m_purchase_data_vec_list[i].clear();
			}
		}
	}
}

void CrossRandActivityCloudPurchase::OnRegisterGameServer(NetID global_netid)
{
	CrossRandActivity::OnRegisterGameServer(global_netid);

	// 通知gameworld 同步数据
	{
		static crossgameramsg::CGCloudPurchaseSycinfoNotice	cpsn;

		this->SendToGameworldActivity((char*)&cpsn, sizeof(cpsn));
	}
}

void CrossRandActivityCloudPurchase::OnMessageFromGameworld(NetID global_netid, int msg_type, char *data)
{
	if (crossgameramsg::MSG_CLOUDPURCHASE_SYNC_DATA_GC == msg_type)
	{
		this->OnSyncRankData(global_netid, (crossgameramsg::GCCloudPurchaseSyncData *)data);
	}

	if (crossgameramsg::MSG_CLOUDPURCHASE_SYCINFO_GC == msg_type)
	{
		this->OnSycAllServerInfo(global_netid, (crossgameramsg::GCCloudPurchaseSycAllInfo *)data);
	}
}

void CrossRandActivityCloudPurchase::OnSyncRankData(NetID global_netid, crossgameramsg::GCCloudPurchaseSyncData *czrd)
{
	if (czrd->buy_times <= 0 || czrd->purchase_type < 0 || czrd->purchase_type >= CLOUD_PURCHASE_ITEM_COUNT_MAX)
	{
		return;
	}

	const CloudPurchaseItemCfg *cfg = CloudPurchaseConfig::Instance().GetItemCfg(czrd->purchase_type);
	if (nullptr == cfg)
	{
		return;
	}

	for (int i = 0; i < czrd->buy_times && i < 1000; i ++)
	{
		PurchaseDataItem buy_item;

		buy_item.global_netid = global_netid;
		buy_item.server_id = czrd->server_id;
		F_STRNCPY(buy_item.name, czrd->name, sizeof(buy_item.name));
		buy_item.uid = czrd->uid;

		m_purchase_data_vec_list[czrd->purchase_type].emplace_back(buy_item);
	}

	if ((int)m_purchase_data_vec_list[czrd->purchase_type].size() >= cfg->draw_lottery_need_buy_count && 0 == m_check_givereward_timestamp_list[czrd->purchase_type])
	{
		m_check_givereward_timestamp_list[czrd->purchase_type] = (unsigned int)EngineAdapter::Instance().Time() + CloudPurchaseConfig::Instance().GetOtherCfg().draw_lottery_interval;
	}

	static crossgameramsg::CGCloudPurchaseTimes czrcr;

	czrcr.purchase_type = czrd->purchase_type;
	czrcr.times = (int)m_purchase_data_vec_list[czrd->purchase_type].size();
	czrcr.givereward_timestamp = m_check_givereward_timestamp_list[czrd->purchase_type];

	this->SendToGameworldActivity((char*)&czrcr, sizeof(czrcr));
}

void CrossRandActivityCloudPurchase::OnSycAllServerInfo(NetID global_netid, crossgameramsg::GCCloudPurchaseSycAllInfo *all_data)
{
	for (int i = 0; i < CLOUD_PURCHASE_ITEM_COUNT_MAX; i ++)
	{
		const CloudPurchaseItemCfg *cfg = CloudPurchaseConfig::Instance().GetItemCfg(i);
		if (nullptr == cfg) continue;

		for (int j = 0; j < CLOUD_PURCHASE_MAX_JOIN_USER_COUNT; j ++)
		{
			const crossgameramsg::GCCloudPurchaseSycAllInfo::CloudPurchaseUserItem &user_info = all_data->type_info_list[i].info_list[j]; 
			for (int k = 0; k < user_info.times; k ++)
			{
				PurchaseDataItem buy_item;

				buy_item.global_netid = global_netid;
				buy_item.server_id = user_info.server_id;
				F_STRNCPY(buy_item.name, user_info.user_name, sizeof(buy_item.name));
				buy_item.uid = user_info.uid;

				m_purchase_data_vec_list[i].emplace_back(buy_item);
			}
		}

		if ((int)m_purchase_data_vec_list[i].size() >= cfg->draw_lottery_need_buy_count && 0 == m_check_givereward_timestamp_list[i])
		{
			m_check_givereward_timestamp_list[i] = (unsigned int)EngineAdapter::Instance().Time() + CloudPurchaseConfig::Instance().GetOtherCfg().draw_lottery_interval;
		}
	}
}

void CrossRandActivityCloudPurchase::OnActivityStatusChange(int from_status, int to_status)
{
	switch (to_status)
	{
	case ACTIVITY_STATUS_OPEN:
		{

		}
		break;

	case ACTIVITY_STATUS_CLOSE:
		{	
			memset(m_check_givereward_timestamp_list, 0, sizeof(m_check_givereward_timestamp_list));

			for (int i = 0; i < CLOUD_PURCHASE_ITEM_COUNT_MAX; i ++)
			{
				m_purchase_data_vec_list[i].clear();
			}
		}
		break;
	}
}
