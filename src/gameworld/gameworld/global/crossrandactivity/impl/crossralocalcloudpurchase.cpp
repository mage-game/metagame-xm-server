#include "crossralocalcloudpurchase.hpp"

#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "internalcomm.h"
#include "servercommon/internalprotocal/crossgamerandactivitymsg.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "item/itempool.h"
#include "servercommon/servercommon.h"
#include "other/route/mailroute.hpp"
#include "gamelog.h"
#include "servercommon/string/crossstr.h"
#include "servercommon/serverconfig/cloudpurchaseconfig.hpp"
#include "servercommon/errornum.h"
#include "protocal/msgcsrandactivity.hpp"
#include "gameworld/global/worldstatus/worldstatus.hpp"
#include "servercommon/struct/global/worldstatus2param.hpp"
#include "world.h"
#include "gameworld/global/usercache/usercache.hpp"
#include "gameworld/item/knapsack.h"
#include "other/rolecrossrandactivity/rolecrossrandactivity.hpp"

CrossRALocalCloudPurchase::CrossRALocalCloudPurchase() : CrossRALocal(CROSS_RAND_ACTIVITY_TYPE_CLOUD_PURCHASE)
{

}

CrossRALocalCloudPurchase::~CrossRALocalCloudPurchase()
{

}

void CrossRALocalCloudPurchase::Update(unsigned long interval, unsigned int now_second)
{

}

void CrossRALocalCloudPurchase::OnMessageFromCross(int msg_type, char *data)
{
	if (crossgameramsg::MSG_CLOUDPURCHASE_SYNC_DATA_CG == msg_type)
	{
		crossgameramsg::CGCloudPurchaseTimes *czrcr = (crossgameramsg::CGCloudPurchaseTimes*)data;
		this->OnInfoChange(czrcr->purchase_type, czrcr->times, czrcr->givereward_timestamp);
	}
	else if (crossgameramsg::MSG_CLOUDPURCHASE_GIVE_REWARD_CG == msg_type)
	{
		crossgameramsg::CGCloudPurchaseGiveReward *czrcr = (crossgameramsg::CGCloudPurchaseGiveReward*)data;
		this->CheckGiveRewards(czrcr->type, czrcr->uid, czrcr->server_id, czrcr->name);
	}
	else if (crossgameramsg::MSG_CLOUDPURCHASE_SYCINFO_NOTICE_CG == msg_type)
	{
		this->SyncAllInfoToCross();
	}
}

void CrossRALocalCloudPurchase::OnActivityStatusChange(int from_status, int to_status)
{
	CrossRALocal::OnActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
	}
	else if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		const CloudPurchaseOtherCfg &other_cfg = CloudPurchaseConfig::Instance().GetOtherCfg();

		CrossCloudPurchaseParam &cloud_purchase_param = WorldStatus::Instance().GetCrossCloudPurchaseParam();
		for (int i = 0; i < CLOUD_PURCHASE_ITEM_COUNT_MAX; i ++)
		{
			const CloudPurchaseTypeParam &param = cloud_purchase_param.ra_cloud_purchase_list[i];
			const CloudPurchaseItemCfg *item_cfg = CloudPurchaseConfig::Instance().GetItemCfg(i);
			if (NULL == item_cfg)
			{
				continue;; 
			}

			for (int j = 0; j < CLOUD_PURCHASE_MAX_JOIN_USER_COUNT; j ++)
			{
				if (0 != param.item_list[j].uid)
				{
					static MailContentParam contentparam; contentparam.Reset();

					const int return_gold = item_cfg->need_ticket_num * param.item_list[j].times * other_cfg.qianggou_ticket_gold_price;
					//contentparam.virtual_item_list.virtual_item[MAIL_VIRTUAL_ITEM_GOLD] = return_gold;
					contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_GOLD, return_gold);

					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_cross_ra_cloud_purchase_return_gold,
						CROSS_RAND_ACTIVITY_TYPE_CLOUD_PURCHASE, item_cfg->reward_item.item_id);
					if (length > 0)
					{
						MailRoute::MailToUser(IntToUid(param.item_list[j].uid), SYSTEM_MAIL_REASON_INVALID, contentparam);

						gamelog::g_log_cross_rand_activity.printf(LL_INFO, "CrossRALocalCloudPurchase::CheckGiveBackGold user %d, gold %d",
							param.item_list[j].uid, return_gold);
					}
				}
			}
		}

		cloud_purchase_param.Reset();
	}
}

void CrossRALocalCloudPurchase::CheckGiveRewards(int type, int uid, int server_id, GameName name)
{
	if (0 == uid || type < 0 || type >= CLOUD_PURCHASE_ITEM_COUNT_MAX)
	{
		return;
	}

	const CloudPurchaseItemCfg *item_cfg = CloudPurchaseConfig::Instance().GetItemCfg(type);
	if (NULL == item_cfg)
	{
		return;
	}

	CrossCloudPurchaseParam &cloud_purchase_param = WorldStatus::Instance().GetCrossCloudPurchaseParam();

	if (cloud_purchase_param.ra_cloud_purchase_list[type].HasUser(uid))
	{
		const ItemBase *item_base = ITEMPOOL->GetItem(item_cfg->reward_item.item_id);
		if (NULL != item_base)
		{
			// ·¢ÓÊ¼þ
			static MailContentParam contentparam; contentparam.Reset();

			contentparam.item_list[0].item_id = item_cfg->reward_item.item_id;
			contentparam.item_list[0].num = item_cfg->reward_item.num;
			contentparam.item_list[0].is_bind = (item_cfg->reward_item.is_bind) ? 1 : 0;
			contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_cross_ra_cloud_purchase_get_reward, 
				CROSS_RAND_ACTIVITY_TYPE_CLOUD_PURCHASE, item_cfg->reward_item.item_id);
			if (length > 0)
			{
				MailRoute::MailToUser(IntToUid(uid), SYSTEM_MAIL_REASON_INVALID, contentparam);

				gamelog::g_log_cross_rand_activity.printf(LL_INFO, "CrossRALocalCloudPurchase::CheckGiveRewards user %d, type %d, reward[%s]",
					uid, type, ItemConfigDataLog(&item_cfg->reward_item, NULL));
			}
		}
	}

	this->GiveScoreToAllJoinUser(type, uid);

	cloud_purchase_param.ra_cloud_purchase_list[type].Reset();

	const CloudPurchaseOtherCfg &other_cfg = CloudPurchaseConfig::Instance().GetOtherCfg();
	cloud_purchase_param.ra_cloud_purchase_list[type].can_buy_timestamp = static_cast<unsigned>(EngineAdapter::Instance().Time()) + other_cfg.buy_interval;

	this->AddRecordReward(type, uid, server_id, name, item_cfg->reward_item.item_id);
	this->SendAllInfoToUser();
	this->SendServerRecordToUser();
}

void CrossRALocalCloudPurchase::GiveScoreToAllJoinUser(int type, int except_uid)
{
	const CloudPurchaseItemCfg *item_cfg = CloudPurchaseConfig::Instance().GetItemCfg(type);
	if (NULL == item_cfg)
	{
		return;
	}

	const CloudPurchaseOtherCfg &other_cfg = CloudPurchaseConfig::Instance().GetOtherCfg();

	CrossCloudPurchaseParam &cloud_purchase_param = WorldStatus::Instance().GetCrossCloudPurchaseParam();

	const CloudPurchaseTypeParam &item_param = cloud_purchase_param.ra_cloud_purchase_list[type];
	for (int i = 0; i < CLOUD_PURCHASE_MAX_JOIN_USER_COUNT; i ++)
	{
		if (0 != item_param.item_list[i].uid && except_uid != item_param.item_list[i].uid && item_param.item_list[i].times > 0)
		{
			int return_gold = item_cfg->need_ticket_num * item_param.item_list[i].times * other_cfg.qianggou_ticket_gold_price;

			static MailContentParam contentparam; contentparam.Reset();

			//contentparam.virtual_item_list.virtual_item[MAIL_VIRTUAL_ITEM_GOLD] = return_gold;
			contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_GOLD, return_gold);

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_cross_ra_cloud_purchase_reward_score,
				CROSS_RAND_ACTIVITY_TYPE_CLOUD_PURCHASE);
			if (length > 0)
			{
				MailRoute::MailToUser(IntToUid(item_param.item_list[i].uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}

			gamelog::g_log_cross_rand_activity.printf(LL_INFO, "CrossRALocalCloudPurchase::GiveScoreToAllJoinUser user %d, type %d, return_gold [%d]",
				item_param.item_list[i].uid, type, return_gold);
		}
	}
}

void CrossRALocalCloudPurchase::OnInfoChange(int type, int times, unsigned int timestamp)
{
	if (type < 0 || type >= CLOUD_PURCHASE_ITEM_COUNT_MAX || times < 0)
	{
		return;
	}

	ServerPurchaseInfo &total_info = WorldStatus::Instance().GetCrossCloudPurchaseParam().ra_cloud_purchase_list[type].server_info;

	total_info.total_buy_times = times;
	total_info.give_reward_timestamp = timestamp;

	this->SendAllInfoToUser();
}

void CrossRALocalCloudPurchase::OnUserBuy(Role *role, int seq, int times)
{
	if (NULL == role || seq < 0 || seq >= CLOUD_PURCHASE_ITEM_COUNT_MAX || times <= 0)
	{
		return;
	}

	const CloudPurchaseItemCfg *item_cfg = CloudPurchaseConfig::Instance().GetItemCfg(seq);
	if (NULL == item_cfg)
	{
		return;
	}

	CrossCloudPurchaseParam &cloud_purchase_param = WorldStatus::Instance().GetCrossCloudPurchaseParam();
	const ServerPurchaseInfo &total_info = cloud_purchase_param.ra_cloud_purchase_list[seq].server_info;
	
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_sec < cloud_purchase_param.ra_cloud_purchase_list[seq].can_buy_timestamp)
	{
		role->NoticeNum(errornum::EN_CLOUD_PURCHASE_BUY_TIME_LIMIT);
		return;
	}

	if (times + total_info.total_buy_times > item_cfg->draw_lottery_need_buy_count)
	{
		role->NoticeNum(errornum::EN_CLOUD_PURCHASE_BUY_COUNT_LIMIT);
		return;
	}

	if (!role->GetRoleCrossRandActivity()->ConsumeCloudPurchaseTicket(item_cfg->need_ticket_num * times, "CrossRALocalCloudPurchase::OnUserBuy"))
	{
		role->NoticeNum(errornum::EN_CLOUD_PURCHASE_TICKET_LIMIT);
		return;
	}

	bool is_has_user = false;
	if (cloud_purchase_param.ra_cloud_purchase_list[seq].HasUser(role->GetUID()))
	{
		is_has_user = true;
	}

	bool is_user_count_limit = true;

	CloudPurchaseTypeParam &item_param = cloud_purchase_param.ra_cloud_purchase_list[seq];

	for (int i = 0; i < CLOUD_PURCHASE_MAX_JOIN_USER_COUNT; i ++)
	{
		if (0 == item_param.item_list[i].uid && !is_has_user)
		{
			item_param.item_list[i].uid = role->GetUID();
			item_param.item_list[i].times = times;

			is_user_count_limit = false;
			break;
		}

		if (role->GetUID() == item_param.item_list[i].uid && is_has_user)
		{
			item_param.item_list[i].times += times;

			is_user_count_limit = false;
			break;
		}
	}

	if (is_user_count_limit)
	{
		return;
	}

	static crossgameramsg::GCCloudPurchaseSyncData czrd;

	czrd.plat_type = LocalConfig::Instance().GetPlatType();
	czrd.merge_server_id = LocalConfig::Instance().GetMergeServerId();
	czrd.server_id = role->GetUserId().db_index;
	czrd.purchase_type = seq;
	czrd.buy_times = times;
	role->GetName(czrd.name);
	czrd.uid = role->GetUID();

	this->SendToCrossActivity((char*)&czrd, sizeof(czrd));

	role->GetRoleCrossRandActivity()->OnRoleBuyCluodPurchaseSucc(item_cfg->reward_item.item_id, times);
}

void CrossRALocalCloudPurchase::SendAllInfoToUser(Role *role)
{
	static Protocol::SCCloudPurchaseInfo cpi;

	for (int i = 0; i < CLOUD_PURCHASE_ITEM_COUNT_MAX; i ++)
	{
		cpi.item_list[i].Reset();
	}

	const CrossCloudPurchaseParam &cloud_purchase_param = WorldStatus::Instance().GetCrossCloudPurchaseParam();
	for (int i = 0; i < CLOUD_PURCHASE_ITEM_COUNT_MAX; i ++)
	{
		const ServerPurchaseInfo &total_info = cloud_purchase_param.ra_cloud_purchase_list[i].server_info;

		cpi.can_buy_timestamp_list[i] = cloud_purchase_param.ra_cloud_purchase_list[i].can_buy_timestamp;
		cpi.item_list[i].total_buy_times = total_info.total_buy_times;
		cpi.item_list[i].give_reward_timestamp = total_info.give_reward_timestamp;
	}

	if (NULL != role)
	{
		role->GetRoleCrossRandActivity()->SendCloudPureChaseUserInfo();

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&cpi, sizeof(cpi));
	}
	else
	{
		World::GetInstWorld()->SendToAllGateway((const char *)&cpi, sizeof(cpi));
	}
}

void CrossRALocalCloudPurchase::SyncAllInfoToCross()
{
	crossgameramsg::GCCloudPurchaseSycAllInfo cpsyl;

	const CrossCloudPurchaseParam &cloud_purchase_param = WorldStatus::Instance().GetCrossCloudPurchaseParam();

	for (int i = 0; i < CLOUD_PURCHASE_ITEM_COUNT_MAX; i ++)
	{
		for (int j = 0; j < CLOUD_PURCHASE_MAX_JOIN_USER_COUNT; j ++)
		{
			cpsyl.type_info_list[i].info_list[j].times = cloud_purchase_param.ra_cloud_purchase_list[i].item_list[j].times;
			cpsyl.type_info_list[i].info_list[j].uid = cloud_purchase_param.ra_cloud_purchase_list[i].item_list[j].uid;
			cpsyl.type_info_list[i].info_list[j].server_id = IntToUid(cpsyl.type_info_list[i].info_list[j].uid).db_index;

			UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(cloud_purchase_param.ra_cloud_purchase_list[i].item_list[j].uid);
			if (NULL != user_node)
			{
				user_node->GetName(cpsyl.type_info_list[i].info_list[j].user_name);
			}
		}
	}

	this->SendToCrossActivity((char*)&cpsyl, sizeof(cpsyl));
}

void CrossRALocalCloudPurchase::SendServerRecordToUser(Role *role)
{
	static Protocol::SCCloudPurchaseServerRecord cpsr;
	cpsr.count = 0;

	const CrossCloudPurchaseParam &cloud_purchase_param = WorldStatus::Instance().GetCrossCloudPurchaseParam();

	if (cloud_purchase_param.record_count < CLOUD_PURCHASE_RECORD_MAX_COUNT)
	{
		for (int i = 0; i < CLOUD_PURCHASE_RECORD_MAX_COUNT && i < cloud_purchase_param.record_count; i++)
		{
			cpsr.cloud_reward_record_list[i] = cloud_purchase_param.ra_cloud_reward_record_list[i];
			cpsr.count++;
		}
	}
	else
	{
		int beg_idx = cloud_purchase_param.record_cover_index;

		for (int i = 0; i < CLOUD_PURCHASE_RECORD_MAX_COUNT; ++i)
		{
	
			int idx = beg_idx % CLOUD_PURCHASE_RECORD_MAX_COUNT;
			cpsr.cloud_reward_record_list[i] = cloud_purchase_param.ra_cloud_reward_record_list[idx];
			cpsr.count++;
			++beg_idx;
		}
	}


	if (cpsr.count > 0)
	{
		int send_len = sizeof(cpsr) - sizeof(cpsr.cloud_reward_record_list[0]) * (CLOUD_PURCHASE_RECORD_MAX_COUNT - cpsr.count);

		if (NULL != role)
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&cpsr, send_len);
		}
		else
		{
			World::GetInstWorld()->SendToAllGateway((const char *)&cpsr, send_len);
		}
	}
}

void CrossRALocalCloudPurchase::AddRecordReward(int type, int uid, int server_id, GameName name, ItemID item_id)
{
	if (type < 0 || uid < 0 || server_id < 0 || item_id <= 0)
	{
		return;
	}

	CrossCloudPurchaseParam &cloud_purchase_param = WorldStatus::Instance().GetCrossCloudPurchaseParam();

	if (cloud_purchase_param.record_count >= CLOUD_PURCHASE_RECORD_MAX_COUNT && 
		cloud_purchase_param.record_cover_index >= 0 && cloud_purchase_param.record_cover_index < CLOUD_PURCHASE_RECORD_MAX_COUNT)
	{
		ServerPurchaseRecord &record_info = cloud_purchase_param.ra_cloud_reward_record_list[cloud_purchase_param.record_cover_index];

		record_info.reward_server_id = server_id;
		F_STRNCPY(record_info.user_name, name, sizeof(record_info.user_name));
		record_info.reward_item_id = item_id;

		++cloud_purchase_param.record_cover_index;

		if (cloud_purchase_param.record_cover_index >= CLOUD_PURCHASE_RECORD_MAX_COUNT)
		{
			cloud_purchase_param.record_cover_index = 0;
		}
	}

	if (cloud_purchase_param.record_count >= 0 && cloud_purchase_param.record_count < CLOUD_PURCHASE_RECORD_MAX_COUNT)
	{
		ServerPurchaseRecord &record_info = cloud_purchase_param.ra_cloud_reward_record_list[cloud_purchase_param.record_count];

		++cloud_purchase_param.record_count;

		record_info.reward_server_id = server_id;
		F_STRNCPY(record_info.user_name, name, sizeof(record_info.user_name));
		record_info.reward_item_id = item_id;
	}
}