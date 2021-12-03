#include "crossralocalconsumerank.hpp"
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
#include "config/activityconfig/crossrandactivityconfig.hpp"
#include "engineadapter.h"
#include "servercommon/string/gameworldstr.h"

CrossRALocalConsumeRank::CrossRALocalConsumeRank() : CrossRALocal(CROSS_RAND_ACTIVITY_TYPE_CONSUME_RANK),
m_next_sync_to_cross_time(0)
{
	UNSTD_STATIC_CHECK(sizeof(LocalConsumeInfo) < sizeof(CrossRandActivityData));

	m_consume_info = (LocalConsumeInfo *)m_custom_data.data;
}

CrossRALocalConsumeRank::~CrossRALocalConsumeRank()
{
}

void CrossRALocalConsumeRank::Update(unsigned long interval, unsigned int now_second)
{
	if (now_second > m_next_sync_to_cross_time)
	{
		m_next_sync_to_cross_time = now_second + SECOND_PER_MIN;

		static crossgameramsg::GCConsumeRankSyncData czrd;
		czrd.plat_type = LocalConfig::Instance().GetPlatType();
		czrd.merge_server_id = LocalConfig::Instance().GetMergeServerId();
		F_STRNCPY(czrd.mvp_name, m_consume_info->mvp_name, sizeof(czrd.mvp_name));
		czrd.mvp_server_id = m_consume_info->mvp_server_id;
		czrd.total_consume = m_consume_info->server_total_consume;
		this->SendToCrossActivity((char*)&czrd, sizeof(czrd));
	}
}

void CrossRALocalConsumeRank::OnMessageFromCross(int msg_type, char *data)
{
	if (crossgameramsg::MSG_CONSUME_RANK_CHECK_REWARD_CG == msg_type)
	{
		crossgameramsg::CGConsumeRankCheckReward *czrcr = (crossgameramsg::CGConsumeRankCheckReward*)data;
		this->CheckSendRewards(czrcr->rank); // 因为结算需要排名信息，所以就放这里结算了，一般要放OnActivityStatusChange里
	}
}

void CrossRALocalConsumeRank::OnSomeOneConsume(Role *role, long long add_consume, long long total_consume)
{
	if (add_consume > 0 && total_consume > 0)
	{
		m_consume_info->server_total_consume += add_consume;

		if (total_consume > m_consume_info->mvp_total_consume) // 超过了当前最高充值的玩家，替换一下信息
		{
			m_consume_info->mvp_uid = role->GetUID();
			F_STRNCPY(m_consume_info->mvp_name, role->GetName(), sizeof(m_consume_info->mvp_name));
			m_consume_info->mvp_server_id = role->GetUserId().db_index;
			m_consume_info->mvp_total_consume = total_consume;
		}
	}

	bool aleady_has_this_user = false;
	int uid = role->GetUID();

	for (int i = 0; i < m_big_param_data2->ra_consume_rank_user_count; ++ i)
	{
		if (uid == m_big_param_data2->ra_consume_rank_all_uid_list[i])
		{
			aleady_has_this_user = true;
		}
	}

	if (!aleady_has_this_user 
		&& m_big_param_data2->ra_consume_rank_user_count < CROSS_RA_CONSUME_RANK_MAX_JOIN_USER_COUNT
		&& total_consume >= LOGIC_CONFIG->GetCrossRandActivityCfg().GetOtherCfg().consume_rank_join_reward_need)
	{
		m_big_param_data2->ra_consume_rank_all_uid_list[m_big_param_data2->ra_consume_rank_user_count] = uid;
		++m_big_param_data2->ra_consume_rank_user_count;
	}

	m_next_sync_to_cross_time = (unsigned int)EngineAdapter::Instance().Time();
}

void CrossRALocalConsumeRank::CheckSendRewards(int rank)
{
	const CrossRandActivityConsumeRankConfig *rank_cfg = LOGIC_CONFIG->GetCrossRandActivityCfg().GetConsumeRankCfg(rank, m_consume_info->server_total_consume);
	if (NULL == rank_cfg)
	{
		this->ResetAllData();
		return;
	}

	const CrossRandActivityConsumeRankConfig *enough_cfg =
		LOGIC_CONFIG->GetCrossRandActivityCfg().GetConsumeRankCfg(rank, LLONG_MAX);	// 传一个最大值，保证拿到排名对应的配置
	if (NULL == enough_cfg)
	{
		this->ResetAllData();
		return;
	}

	const bool enough = m_consume_info->server_total_consume >= enough_cfg->need_total_consume;

	for (int i = 0; i < m_big_param_data2->ra_consume_rank_user_count; ++ i)
	{
		// 参与充值的玩家发排行奖励
		const ItemBase *item_base = ITEMPOOL->GetItem(rank_cfg->rank_join_reward_item.item_id);
		if (NULL != item_base)
		{
			// 发邮件
			static MailContentParam contentparam; contentparam.Reset();

			contentparam.item_list[0].item_id = rank_cfg->rank_join_reward_item.item_id;
			contentparam.item_list[0].num = rank_cfg->rank_join_reward_item.num;
			contentparam.item_list[0].is_bind = (rank_cfg->rank_join_reward_item.is_bind) ? 1 : 0;
			contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

			int length = 0;
			if (enough)
			{
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_cross_ra_consume_rank_join_reward,
					CROSS_RAND_ACTIVITY_TYPE_CONSUME_RANK, rank);
			}
			else
			{
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_cross_ra_consume_rank_low_join_reward,
					CROSS_RAND_ACTIVITY_TYPE_CONSUME_RANK, enough_cfg->rank, rank_cfg->rank);
			}

			if (length > 0)
			{
				MailRoute::MailToUser(IntToUid(m_big_param_data2->ra_consume_rank_all_uid_list[i]), SYSTEM_MAIL_REASON_INVALID, contentparam);

				gamelog::g_log_cross_rand_activity.printf(LL_INFO, "CrossRALocalConsumeRank::CheckSendRewards user %d, rank %d, join_reward[%s]",
					m_big_param_data2->ra_consume_rank_all_uid_list[i], rank, ItemConfigDataLog(&rank_cfg->rank_join_reward_item, NULL));
			}
		}
	}

	// 发放本服充值第一名奖励
	const ItemBase *item_base = ITEMPOOL->GetItem(rank_cfg->personal_reward_item.item_id);
	if (NULL != item_base && m_consume_info->mvp_uid > 0)
	{
		// 发邮件
		static MailContentParam contentparam; contentparam.Reset();

		contentparam.item_list[0].item_id = rank_cfg->personal_reward_item.item_id;
		contentparam.item_list[0].num = rank_cfg->personal_reward_item.num;
		contentparam.item_list[0].is_bind = (rank_cfg->personal_reward_item.is_bind) ? 1 : 0;
		contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

		int length = 0;
		if (enough)
		{
			length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_cross_ra_consume_rank_mvp_reward,
				CROSS_RAND_ACTIVITY_TYPE_CONSUME_RANK, rank);
		}
		else
		{
			length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_cross_ra_consume_rank_low_mvp_reward,
				CROSS_RAND_ACTIVITY_TYPE_CONSUME_RANK, enough_cfg->rank, rank_cfg->rank);
		}

		if (length > 0)
		{
			MailRoute::MailToUser(IntToUid(m_consume_info->mvp_uid), SYSTEM_MAIL_REASON_INVALID, contentparam);

			gamelog::g_log_cross_rand_activity.printf(LL_INFO, "CrossRALocalConsumeRank::CheckSendRewards user[%d,%s] total_consume[%lld] rank[%d] mvp_reward[%s]",
				m_consume_info->mvp_uid, m_consume_info->mvp_name, m_consume_info->server_total_consume, rank, ItemConfigDataLog(&rank_cfg->personal_reward_item, NULL));
		}
	}

	this->ResetAllData();
}

void CrossRALocalConsumeRank::ResetAllData()
{
	m_big_param_data2->ra_consume_rank_user_count = 0;
	memset(m_big_param_data2->ra_consume_rank_all_uid_list, 0, sizeof(m_big_param_data2->ra_consume_rank_all_uid_list));

	m_consume_info->server_total_consume = 0;
	m_consume_info->mvp_server_id = 0;
	m_consume_info->mvp_uid = 0;
	m_consume_info->mvp_total_consume = 0;
	memset(m_consume_info->mvp_name, 0, sizeof(m_consume_info->mvp_name));
}

void CrossRALocalConsumeRank::OnActivityStatusChange(int from_status, int to_status)
{
	CrossRALocal::OnActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
	}
	else if (ACTIVITY_STATUS_CLOSE == to_status)
	{
	}
}

