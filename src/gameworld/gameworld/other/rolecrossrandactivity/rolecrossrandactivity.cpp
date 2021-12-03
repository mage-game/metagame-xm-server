#include "rolecrossrandactivity.hpp"

#include "world.h"
#include "engineadapter.h"
#include "gamecommon.h"
#include "protocal/msgcsrandactivity.hpp"
#include "servercommon/errornum.h"
#include "gameworld/item/knapsack.h"
#include "gameworld/item/itempool.h"
#include "config/activityconfig/chongzhiconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "global/crossrandactivity/crossramanagerlocal.hpp"
#include "global/crossrandactivity/impl/crossralocalchongzhirank.hpp"
#include "global/crossrandactivity/impl/crossralocalconsumerank.hpp"
#include "global/crossrandactivity/impl/crossralocalcloudpurchase.hpp"
#include "servercommon/serverconfig/cloudpurchaseconfig.hpp"
#include "gameworld/other/route/mailroute.hpp"
#include "servercommon/string/crossstr.h"

RoleCrossRandActivity::RoleCrossRandActivity()
	: m_role(NULL), m_act_shadow(NULL)
{

}

RoleCrossRandActivity::~RoleCrossRandActivity()
{

}

void RoleCrossRandActivity::Init(Role *role, const RoleCrossRandActivityParam &p)
{
	m_role = role;
	m_act_shadow = &ActivityShadow::Instance();

	memcpy(&m_activity_info, &p, sizeof(RoleCrossRandActivityParam));
}

void RoleCrossRandActivity::GetInitParam(RoleCrossRandActivityParam *p)
{
	memcpy(p, &m_activity_info, sizeof(RoleCrossRandActivityParam));
}

void RoleCrossRandActivity::OnResetData()
{
	
}

void RoleCrossRandActivity::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid) return;

}

void RoleCrossRandActivity::OnWeekChange()
{
	
}

void RoleCrossRandActivity::OnRoleLogin()
{
	this->CheckAllActRewardStatus();

	if (ActivityShadow::Instance().IsCrossRandActivityOpen(CROSS_RAND_ACTIVITY_TYPE_CHONGZHI_RANK))
	{
		this->SendChongzhiInfo();
	}

	if (ActivityShadow::Instance().IsCrossRandActivityOpen(CROSS_RAND_ACTIVITY_TYPE_CONSUME_RANK))
	{
		this->SendConsumeInfo();
	}
}

void RoleCrossRandActivity::Update(unsigned int now_second)
{
}

void RoleCrossRandActivity::OnActivityOpen(int activity_type)
{

}

void RoleCrossRandActivity::OnActivityClose(int activity_type)
{
	switch (activity_type)
	{
	case CROSS_RAND_ACTIVITY_TYPE_CHONGZHI_RANK:
		{
			this->OnChongzhiRankCheckReward();
		}
		break;

	case CROSS_RAND_ACTIVITY_TYPE_CLOUD_PURCHASE:
		{
			this->OnCheckResetCloudPurchaseData();
		}
		break;

	case CROSS_RAND_ACTIVITY_TYPE_CONSUME_RANK:
		{
			this->OnConsumeRankCheckReward();
		}
		break;
	}
}

void RoleCrossRandActivity::OnClientRequest(Protocol::CSCrossRandActivityRequest * msg)
{
	switch (msg->activity_type)
	{
	case CROSS_RAND_ACTIVITY_TYPE_CHONGZHI_RANK:
		{
			this->OnChongzhiRankReq(msg->opera_type, msg->param_1, msg->param_2, msg->param_3);
		}
		break;

	case CROSS_RAND_ACTIVITY_TYPE_CLOUD_PURCHASE:
		{
			this->OnCloudPurchaseOpera(msg->opera_type, msg->param_1, msg->param_2);
		}
		break;

	case CROSS_RAND_ACTIVITY_TYPE_CONSUME_RANK:
		{
			this->OnConsumeRankReq(msg->opera_type, msg->param_1, msg->param_2, msg->param_3);
		}
		break;

	}
}

void RoleCrossRandActivity::CheckAllActRewardStatus()
{
	// 若上一次参与活动未结算，检查发放奖励并清空数据

	if (this->HasRewardNotGive(CROSS_RAND_ACTIVITY_TYPE_CHONGZHI_RANK, m_activity_info.chongzhi_rank_join_timestamp))
	{
		this->OnChongzhiRankCheckReward();
	}

	if (this->HasRewardNotGive(CROSS_RAND_ACTIVITY_TYPE_CLOUD_PURCHASE, m_activity_info.cloud_purchase_join_timestamp))
	{
		this->OnCheckResetCloudPurchaseData();
	}

	if (this->HasRewardNotGive(CROSS_RAND_ACTIVITY_TYPE_CONSUME_RANK, m_activity_info.consume_rank_join_timestamp))
	{
		this->OnConsumeRankCheckReward();
	}
}

bool RoleCrossRandActivity::HasRewardNotGive(int activity_type, unsigned int last_join_act_time)
{
	if (0 == last_join_act_time)
	{
		return false;
	}

	// 上一次参与活动时间是否在当前活动配置的开启时间戳内
	bool is_in_cur_activity_time = m_act_shadow->IsInCurrentCrossRandActivityOpenTime(activity_type, last_join_act_time);

	// 当前活动是否在开启中
	bool cur_activity_is_opening = m_act_shadow->IsCrossRandActivityOpen(activity_type);

	return (cur_activity_is_opening ^ is_in_cur_activity_time);
}

void RoleCrossRandActivity::OnAddChongZhi(long long chongzhi)
{
	this->OnChongzhiRankAddChongzhi(chongzhi);
}

void RoleCrossRandActivity::OnConsumeGold(long long consume)
{
	this->OnConsumeRankAddConsume(consume);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RoleCrossRandActivity::OnChongzhiRankCheckReward()
{
	m_activity_info.chongzhi_rank_join_timestamp = 0;
	m_activity_info.chongzhi_rank_chongzhi = 0;	

	this->SendChongzhiInfo();
}

void RoleCrossRandActivity::OnChongzhiRankAddChongzhi(long long chongzhi)
{
	if (!m_act_shadow->IsCrossRandActivityOpen(CROSS_RAND_ACTIVITY_TYPE_CHONGZHI_RANK))
	{
		return;
	}

	CrossRALocalChongzhiRank *chongzhi_rank_act = (CrossRALocalChongzhiRank *)CrossRAManagerLocal::Instance().GetActivity(CROSS_RAND_ACTIVITY_TYPE_CHONGZHI_RANK);
	if (NULL == chongzhi_rank_act)
	{
		return;
	}

	m_activity_info.chongzhi_rank_join_timestamp = (unsigned int)EngineAdapter::Instance().Time();
	m_activity_info.chongzhi_rank_chongzhi += (unsigned int)chongzhi;

	chongzhi_rank_act->OnSomeOneChongzhi(m_role, chongzhi, m_activity_info.chongzhi_rank_chongzhi);

	this->SendChongzhiInfo();
}

void RoleCrossRandActivity::OnChongzhiRankReq(int opera_type, int param_1, int param_2, int param_3)
{
	switch (opera_type)
	{
	case Protocol::CS_CROSS_RA_CHONGZHI_RANK_REQ_TYPE_INFO:
		{
			this->SendChongzhiInfo();
		}
		break;
	}
}

void RoleCrossRandActivity::SendChongzhiInfo()
{
	static Protocol::SCCrossRAChongzhiRankChongzhiInfo crci;
	crci.total_chongzhi = m_activity_info.chongzhi_rank_chongzhi;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&crci, sizeof(crci));
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RoleCrossRandActivity::OnCloudPurchaseOpera(int opera_type, int param1, int param2)
{
	if (!m_act_shadow->IsCrossRandActivityOpen(CROSS_RAND_ACTIVITY_TYPE_CLOUD_PURCHASE))
	{
		return;
	}

	CrossRALocalCloudPurchase *cloud_purchase_act = (CrossRALocalCloudPurchase *)CrossRAManagerLocal::Instance().GetActivity(CROSS_RAND_ACTIVITY_TYPE_CLOUD_PURCHASE);
	if (NULL == cloud_purchase_act)
	{
		return;
	}

	switch (opera_type)
	{
	case Protocol::RA_CLOUDPURCHASE_OPERA_TYPE_INFO:
	{
		cloud_purchase_act->SendAllInfoToUser(m_role);
	}
	break;

	case Protocol::RA_CLOUDPURCHASE_OPERA_TYPE_BUY:
	{
		cloud_purchase_act->OnUserBuy(m_role, param1, param2);
	}
	break;

	case Protocol::RA_CLOUDPURCHASE_OPERA_TYPE_BUY_RECORD:
	{
		this->SendCloudPurchaseBuyInfo();
	}
	break;

	case Protocol::RA_CLOUDPURCHASE_OPERA_TYPE_CONVERT:
	{
		this->OnCloudScoreConvertItem(param1, param2);
	}
	break;

	case Protocol::RA_CLOUDPURCHASE_OPERA_TYPE_CONVERT_INFO:
	{
		this->SendCluodPurchaseConvertInfo();
	}
	break;

	case Protocol::RA_CLOUDPURCHASE_OPERA_TYPE_SERVER_RECORD_INFO:
	{
		cloud_purchase_act->SendServerRecordToUser(m_role);
	}
	break;
	}
}

bool RoleCrossRandActivity::OnCloudPurchaseRMBBuyTicket(int gold_num)
{
	if (gold_num <= 0) return false;

	if (!ActivityShadow::Instance().IsCrossRandActivityOpen(CROSS_RAND_ACTIVITY_TYPE_CLOUD_PURCHASE))
	{
		return false;
	}

	const CloudPurchaseOtherCfg &other_cfg = CloudPurchaseConfig::Instance().GetOtherCfg();

	if (other_cfg.qianggou_ticket_gold_price <= 0)
	{
		return false;
	}

	bool is_cloud_purchase_chongzhi = false;
	const int ticket_cfg_count = CloudPurchaseConfig::Instance().GetCloudPurchaseTicketCount();

	for (int i = 0; i < ticket_cfg_count && i < CLOUD_PURCHASE_TICKET_TYPE_COUNT_MAX; ++i)
	{
		const CloudPurchaseTicketCfg *ticket_cfg = CloudPurchaseConfig::Instance().GetCloudPurchaseTicketCfg(i);
		if (nullptr == ticket_cfg || ticket_cfg->gold_price <= 0)
		{
			return false;
		}

		if (gold_num == ticket_cfg->gold_price)
		{
			is_cloud_purchase_chongzhi = true;
			break;
		}
	}

	if (!is_cloud_purchase_chongzhi)
	{
		return false;
	}

	int get_score = gold_num * other_cfg.score_per_gold;
	int get_ticket_num = gold_num / other_cfg.qianggou_ticket_gold_price;

	//购买过抢购券即视为参与活动
	if (0 == m_activity_info.cloud_purchase_join_timestamp)
	{
		m_activity_info.cloud_purchase_join_timestamp = (unsigned int)EngineAdapter::Instance().Time();
	}

	m_role->GetRoleCrossRandActivity()->OnAddCloudPurchaseScore(get_score, "RoleCrossRandActivity::OnCloudPurchaseRMBBuyTicket");
	m_activity_info.cloud_purchase_ticket_num += get_ticket_num;

	this->SendCloudPureChaseUserInfo();

	return true;
}

void RoleCrossRandActivity::OnCloudScoreConvertItem(int seq, int item_count)
{
	if (seq < 0 || seq >= MAX_CLOUD_PUECHASE_CONVERT_RECORD_ITEM_COUNT)
	{
		return;
	}

	const CloudPurchaseConvertCfg *item_cfg = CloudPurchaseConfig::Instance().GetConvertCfg(seq);
	if (NULL == item_cfg)
	{
		return;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(item_cfg->item_id);
	if (nullptr == item_base || item_base->GetPileLimit() <= 0)
	{
		return;
	}

	int need_grid_count = item_count / item_base->GetPileLimit();
	if (0 != item_count % item_base->GetPileLimit())
	{
		++need_grid_count;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!knapsack->CheckEmptyGridNoLessThan(need_grid_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (item_cfg->count_limit > 0)
	{
		int record_count = this->GetCloudConvertRecordCount(item_cfg->item_id);
		if (record_count + item_count > item_cfg->count_limit)
		{
			m_role->NoticeNum(errornum::EN_CLOUD_PURCHASE_CONVERT_COUNT_LIMIT);
			return;
		}
	}

	int need_score = item_count * item_cfg->cost_score;
	if (need_score <= 0)
	{
		return;
	}

	if (m_activity_info.cloud_purchase_score < need_score)
	{
		m_role->NoticeNum(errornum::EN_CLOUD_PURCHASE_CONVERT_SCORE_LIMIT);
		return;
	}

	m_activity_info.cloud_purchase_score -= need_score;

	ItemConfigData reward_item;
	reward_item.item_id = item_cfg->item_id;
	reward_item.num = item_count;
	reward_item.is_bind = true;

	knapsack->Put(reward_item, PUT_REASON_CLOUD_PURCHASE_CONVERT_REWARD);

	if (item_cfg->count_limit > 0)
	{
		this->OnCloudPurchaseConvertAddRecord(item_cfg->item_id, item_count);
	}

	this->SendCluodPurchaseConvertInfo();

	gamelog::g_log_cross_rand_activity.printf(LL_INFO, "RoleCrossRandActivity::OnCloudScoreConvertItem user[%d %s], dec_score[%d], cur_score[%d], reason[%s]",
		m_role->GetUID(), m_role->GetName(), need_score, m_activity_info.cloud_purchase_score, "OnCloudScoreConvertItem");
}

void RoleCrossRandActivity::OnAddCloudPurchaseScore(int score, const char *reason)
{
	if (score <= 0 || NULL == reason)
	{
		return;
	}

	m_activity_info.cloud_purchase_score += score;

	this->SendCluodPurchaseConvertInfo();

	gamelog::g_log_cross_rand_activity.printf(LL_INFO, "RoleCrossRandActivity::OnAddCloudPurchaseScore user[%d %s], add_score[%d], cur_score[%d], reason[%s]",
		m_role->GetUID(), m_role->GetName(), score, m_activity_info.cloud_purchase_score, reason);
}

bool RoleCrossRandActivity::ConsumeCloudPurchaseTicket(const int ticket_num, const char *reason)
{
	if (nullptr == reason)
	{
		return false;
	}

	if (ticket_num <= 0)
	{
		return false;
	}

	if (m_activity_info.cloud_purchase_ticket_num < ticket_num)
	{
		return false;
	}

	m_activity_info.cloud_purchase_ticket_num -= ticket_num;

	this->SendCloudPureChaseUserInfo();

	gamelog::g_log_cross_rand_activity.printf(LL_INFO, "RoleCrossRandActivity::ConsumeCloudPurchaseTicket [user %d %s] reason[%s] consume_count[%d], leftp[%d]",
		m_role->GetUID(), m_role->GetName(), reason, ticket_num, m_activity_info.cloud_purchase_ticket_num);

	return true;
}

void RoleCrossRandActivity::SendCluodPurchaseConvertInfo()
{
	static Protocol::SCCloudPurchaseConvertInfo cpsi;

	cpsi.score = m_activity_info.cloud_purchase_score;
	cpsi.record_count = 0;

	for (int i = 0; i < m_activity_info.cloud_purchase_record_convert_count && i < MAX_CLOUD_PUECHASE_CONVERT_RECORD_ITEM_COUNT; ++i)
	{
		cpsi.convert_record_list[i] = m_activity_info.convert_record_list[i];
		++cpsi.record_count;
	}

	int send_len = sizeof(cpsi) - sizeof(cpsi.convert_record_list[0]) * (MAX_CLOUD_PUECHASE_CONVERT_RECORD_ITEM_COUNT - cpsi.record_count);

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cpsi, send_len);
}

void RoleCrossRandActivity::OnCloudPurchaseConvertAddRecord(ItemID item_id, int count)
{
	if (m_activity_info.cloud_purchase_record_convert_count >= MAX_CLOUD_PUECHASE_CONVERT_RECORD_ITEM_COUNT)
	{
		return;
	}

	int record_index = -1;
	for (int i = 0; i < m_activity_info.cloud_purchase_record_convert_count && i < MAX_CLOUD_PUECHASE_CONVERT_RECORD_ITEM_COUNT; ++i)
	{
		const CloudPurchaseCovertRecord &record = m_activity_info.convert_record_list[i];

		if (item_id == record.item_id)
		{
			record_index = i;
			break;
		}
	}

	if (-1 == record_index)
	{
		record_index = m_activity_info.cloud_purchase_record_convert_count;
		++m_activity_info.cloud_purchase_record_convert_count;
	}

	if (record_index >= 0 && record_index < MAX_CLOUD_PUECHASE_CONVERT_RECORD_ITEM_COUNT)
	{
		m_activity_info.convert_record_list[record_index].item_id = item_id;
		m_activity_info.convert_record_list[record_index].convert_count += count;
	}
}

void RoleCrossRandActivity::OnCheckResetCloudPurchaseData()
{
	for (int i = 0; i < m_activity_info.cloud_purchase_record_convert_count && i < MAX_CLOUD_PUECHASE_CONVERT_RECORD_ITEM_COUNT; ++i)
	{
		m_activity_info.convert_record_list[i].Reset();
	}

	for (int i = 0; i < m_activity_info.cloud_purchase_record_buy_count && i < MAX_CLOUD_PUECHASE_BUY_RECORD_ITEM_COUNT; ++i)
	{
		m_activity_info.cloud_purchase_buy_record_list[i].Reset();
	}

	if (m_activity_info.cloud_purchase_ticket_num > 0)
	{
		const CloudPurchaseOtherCfg &other_cfg = CloudPurchaseConfig::Instance().GetOtherCfg();

		int return_gold = m_activity_info.cloud_purchase_ticket_num * other_cfg.qianggou_ticket_gold_price;

		static MailContentParam contentparam; contentparam.Reset();

		//contentparam.virtual_item_list.virtual_item[MAIL_VIRTUAL_ITEM_GOLD] = return_gold;
		contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_GOLD, return_gold);

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_cross_ra_cloud_purchase_act_close,
			CROSS_RAND_ACTIVITY_TYPE_CLOUD_PURCHASE, m_activity_info.cloud_purchase_ticket_num, return_gold);

		if (length > 0)
		{
			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
		}

		gamelog::g_log_cross_rand_activity.printf(LL_INFO, "RoleCrossRandActivity::OnCheckResetCloudPurchaseData user[%d %s], left_ticket[%d], return_gold [%d]",
			m_role->GetUID(), m_role->GetName(), m_activity_info.cloud_purchase_ticket_num, return_gold);
	}

	m_activity_info.cloud_purchase_score = 0;
	m_activity_info.cloud_purchase_ticket_num = 0;
	m_activity_info.cloud_purchase_join_timestamp = 0;
	m_activity_info.cloud_purchase_record_buy_count = 0;
	m_activity_info.cloud_purchase_record_convert_count = 0;
	
	this->SendCloudPureChaseUserInfo();
}

void RoleCrossRandActivity::OnRoleBuyCluodPurchaseSucc(ItemID item_id, int buy_count)
{
	if (m_activity_info.cloud_purchase_record_buy_count < MAX_CLOUD_PUECHASE_BUY_RECORD_ITEM_COUNT)
	{
		m_activity_info.cloud_purchase_buy_record_list[m_activity_info.cloud_purchase_record_buy_count].item_id = item_id;
		m_activity_info.cloud_purchase_buy_record_list[m_activity_info.cloud_purchase_record_buy_count].buy_count = buy_count;
		m_activity_info.cloud_purchase_buy_record_list[m_activity_info.cloud_purchase_record_buy_count].buy_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

		++m_activity_info.cloud_purchase_record_buy_count;
	}
	else
	{
		int insert_index = m_activity_info.cloud_purchase_record_buy_count % MAX_CLOUD_PUECHASE_BUY_RECORD_ITEM_COUNT;

		m_activity_info.cloud_purchase_buy_record_list[insert_index].item_id = item_id;
		m_activity_info.cloud_purchase_buy_record_list[insert_index].buy_count = buy_count;
		m_activity_info.cloud_purchase_buy_record_list[insert_index].buy_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

		++m_activity_info.cloud_purchase_record_buy_count;
	}

	this->SendCloudPurchaseBuyInfo();
}

int RoleCrossRandActivity::GetCloudConvertRecordCount(ItemID item_id)
{
	for (int i = 0; i < MAX_CLOUD_PUECHASE_BUY_RECORD_ITEM_COUNT; i++)
	{
		if (item_id == m_activity_info.convert_record_list[i].item_id)
		{
			return m_activity_info.convert_record_list[i].convert_count;
		}
	}

	return 0;
}

void RoleCrossRandActivity::SendCloudPurchaseBuyInfo()
{
	static Protocol::SCCloudPurchaseBuyRecordInfo cpbi;
	cpbi.record_count = 0;

	if (m_activity_info.cloud_purchase_record_buy_count < MAX_CLOUD_PUECHASE_BUY_RECORD_ITEM_COUNT)
	{
		for (int i = 0; i < MAX_CLOUD_PUECHASE_BUY_RECORD_ITEM_COUNT && i < m_activity_info.cloud_purchase_record_buy_count; i++)
		{
			cpbi.buy_record_list[i] = m_activity_info.cloud_purchase_buy_record_list[i];
			cpbi.record_count++;
		}
	}
	else
	{
		int beg_idx = m_activity_info.cloud_purchase_record_buy_count;

		for (int i = 0; i < MAX_CLOUD_PUECHASE_BUY_RECORD_ITEM_COUNT; ++i)
		{
			int cur_idx = beg_idx % MAX_CLOUD_PUECHASE_BUY_RECORD_ITEM_COUNT;

			cpbi.buy_record_list[i] = m_activity_info.cloud_purchase_buy_record_list[cur_idx];
			cpbi.record_count++;
			beg_idx++;
		}
	}

	int send_len = sizeof(cpbi) - sizeof(cpbi.buy_record_list[0]) * (MAX_CLOUD_PUECHASE_BUY_RECORD_ITEM_COUNT - cpbi.record_count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cpbi, send_len);
}

void RoleCrossRandActivity::SendCloudPureChaseUserInfo()
{
	static Protocol::SCCloudPurchaseUserInfo cpui;

	cpui.score = m_activity_info.cloud_purchase_score;
	cpui.ticket_num = m_activity_info.cloud_purchase_ticket_num;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cpui, sizeof(cpui));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RoleCrossRandActivity::OnConsumeRankCheckReward()
{
	m_activity_info.consume_rank_join_timestamp = 0;
	m_activity_info.consume_rank_consume = 0;

	this->SendConsumeInfo();
}

void RoleCrossRandActivity::OnConsumeRankAddConsume(long long consume)
{
	if (!m_act_shadow->IsCrossRandActivityOpen(CROSS_RAND_ACTIVITY_TYPE_CONSUME_RANK))
	{
		return;
	}

	CrossRALocalConsumeRank *consume_rank_act = (CrossRALocalConsumeRank *)CrossRAManagerLocal::Instance().GetActivity(CROSS_RAND_ACTIVITY_TYPE_CONSUME_RANK);
	if (NULL == consume_rank_act)
	{
		return;
	}

	m_activity_info.consume_rank_join_timestamp = (unsigned int)EngineAdapter::Instance().Time();
	m_activity_info.consume_rank_consume += (unsigned int)consume;

	consume_rank_act->OnSomeOneConsume(m_role, consume, m_activity_info.consume_rank_consume);

	this->SendConsumeInfo();
}

void RoleCrossRandActivity::OnConsumeRankReq(int opera_type, int param_1, int param_2, int param_3)
{
	switch (opera_type)
	{
		case Protocol::CS_CROSS_RA_CONSUME_RANK_REQ_TYPE_INFO:
			{
				this->SendConsumeInfo();
			}
			break;
	}
}

void RoleCrossRandActivity::SendConsumeInfo()
{
	static Protocol::SCCrossRAConsumeRankConsumeInfo crci;
	crci.total_consume = m_activity_info.consume_rank_consume;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&crci, sizeof(crci));
}
