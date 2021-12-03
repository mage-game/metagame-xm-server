#include "randactivitybipincapabilityjinglian.h"
#include "world.h"
#include "gamelog.h"
#include "global/randactivity/randactivitymanager.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "global/rank/rankmanager.hpp"
#include "other/route/mailroute.hpp"
#include "servercommon/string/gameworldstr.h"
#include "item/itempool.h"
#include "global/usercache/usercache.hpp"

UNSTD_STATIC_CHECK(4 == CAMP_TYPE_MAX);

const static int rank_type = PERSON_RANK_TYPE_CAPABILITY_WASH;

RandActivityBipinCapabilityWash::RandActivityBipinCapabilityWash(RandActivityManager *rand_activity_manager)
: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH), m_check_rank_reward_timestamp(0)
{

}

RandActivityBipinCapabilityWash::~RandActivityBipinCapabilityWash()
{

}


void RandActivityBipinCapabilityWash::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data.data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data.data;
	m_data_param = *pTemp;
}

void RandActivityBipinCapabilityWash::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data->data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data->data;
	*pTemp = m_data_param;
}


void RandActivityBipinCapabilityWash::Update(unsigned long interval, time_t now_second)
{
	RandActivity::Update(interval, now_second);

	if (0 == m_check_rank_reward_timestamp && 0 != m_data_param.need_give_reward)
	{
		if (0 != m_begin_time)
		{
			const RandActivityRankBipinCfg *bipin_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRankBipinCfg(m_rand_activity_type);
			if (NULL != bipin_cfg)
			{
				m_check_rank_reward_timestamp = GetZeroTime(m_begin_time) + SECOND_PER_DAY * bipin_cfg->give_reward_day; 
			}
		}
	}

	if (0 != m_data_param.need_give_reward)
	{
		if (0 != m_check_rank_reward_timestamp && now_second >= m_check_rank_reward_timestamp)
		{
			this->CheckGiveRankReward();
		}		
	}
}

void RandActivityBipinCapabilityWash::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		m_data_param.need_give_reward = 1;
	}
	else if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		this->CheckGiveRankReward();
	}
}

void RandActivityBipinCapabilityWash::OnPersonRankSnapshot(int old_top_uid_list[PERSON_RANK_TYPE_MAX], int new_top_uid_list[PERSON_RANK_TYPE_MAX])
{
	const RandActivityRankBipinCfg *bipin_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRankBipinCfg(m_rand_activity_type);
	if (NULL == bipin_cfg)
	{
		return;
	}

	if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX)
	{
		return;
	}


	time_t now_second = EngineAdapter::Instance().Time();
	time_t notify_top_user_change_begin_timestamp = GetZeroTime(m_begin_time) + SECOND_PER_DAY * (bipin_cfg->give_reward_day - 1);

	if (0 != m_data_param.need_give_reward && now_second >= notify_top_user_change_begin_timestamp)
	{
		if (0 != old_top_uid_list[rank_type] && 0 != new_top_uid_list[rank_type] && old_top_uid_list[rank_type] != new_top_uid_list[rank_type])
		{
			UserCacheNode *old_user_node = UserCacheManager::Instance().GetUserNode(old_top_uid_list[rank_type]);
			UserCacheNode *new_user_node = UserCacheManager::Instance().GetUserNode(new_top_uid_list[rank_type]);
			if (NULL != old_user_node && NULL != new_user_node)
			{
				{
					// 邮件
					static MailContentParam contentparam; contentparam.Reset();
					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_bipin_topuser_change_content, 
						m_rand_activity_type, new_user_node->uid, new_user_node->user_name, new_user_node->camp, bipin_cfg->reward_item.item_id);
					if (length > 0)
					{
						MailRoute::MailToUser(IntToUid(old_user_node->uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
					}
				}

				{
					// 传闻
					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_bipin_topuser_change_notice,
						m_rand_activity_type, new_user_node->uid, new_user_node->user_name, new_user_node->camp, 
						old_user_node->uid, old_user_node->user_name, old_user_node->camp, bipin_cfg->reward_item.item_id);
					if (length > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
					}
				}
			}
		}
	}
}

void RandActivityBipinCapabilityWash::CheckGiveRankReward()
{
	if (0 == m_data_param.need_give_reward)
	{
		return;
	}

	const RandActivityRankBipinCfg *bipin_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRankBipinCfg(m_rand_activity_type);
	if (NULL == bipin_cfg)
	{
		return;
	}

	time_t now_second = EngineAdapter::Instance().Time();
	time_t give_rank_reward_timestamp = GetZeroTime(m_begin_time) + SECOND_PER_DAY * bipin_cfg->give_reward_day;

	if (ACTIVITY_STATUS_CLOSE == m_rand_activity_status || now_second >= give_rank_reward_timestamp)
	{
		PersonRank *person_rank = RankManager::Instance().GetPersonRank();
		if (NULL == person_rank)
		{
			return;
		}

		person_rank->Snapshot();

		m_data_param.need_give_reward = 0;

		int all_name_len = 0;
		char all_name_str[512] = {0};

		PersonRankItem rank_item[Protocol::TOP_COUNT];
		int count = person_rank->GetRankTopItem(rank_type, Protocol::TOP_COUNT, rank_item);
		std::string reward_str("NO_REWARD");

		for (int i = 0; i < count && i < Protocol::TOP_COUNT; ++ i)
		{
			if (rank_item[i].rank_value >= bipin_cfg->need_capability)
			{
				m_data_param.role_id_list[i] = UidToInt(rank_item[i].user_id);
				m_data_param.rank_value_list[i] = rank_item[i].rank_value;

				const ItemConfigData &reward_item = bipin_cfg->reward_item;
				const ItemBase *item_base = ITEMPOOL->GetItem(reward_item.item_id);

				if (NULL != item_base)
				{
					// 发邮件
					static MailContentParam contentparam; contentparam.Reset();

					contentparam.item_list[0].item_id = reward_item.item_id;
					contentparam.item_list[0].num = reward_item.num;
					contentparam.item_list[0].is_bind = (reward_item.is_bind) ? 1 : 0;
					contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_bipin_rank_reward_content, 
						m_rand_activity_type, i + 1);
					if (length > 0)
					{
						MailRoute::MailToUser(rank_item[i].user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);
						reward_str = ItemDataWrapperLog(&contentparam.item_list[0]);
					}
				}

				{
					// 拼接名字
					char member_name_str[64] = {0};
					int namelen = SNPRINTF(member_name_str, sizeof(member_name_str), "{r;%d;%s;%d} ", UidToInt(rank_item[i].user_id), rank_item[i].user_name, rank_item[i].camp);
					if (namelen > 0 && all_name_len + namelen < (int)sizeof(all_name_str))
					{
						STRNCPY(all_name_str + all_name_len, member_name_str, sizeof(all_name_str) - all_name_len);
						all_name_len += namelen;
					}
				}

				gamelog::g_log_rand_activity.printf(LL_INFO, "RandActivityBipinCapabilityWash::CheckGiveRankReward user %d %s, cur_capability %lld, need_capability %d, reward_list %s,"
					" now_second %lld, give_reward_second %lld, check_rank_reward_timestamp %lld, activity_status %d",
					UidToInt(rank_item[i].user_id), rank_item[i].user_name, rank_item[i].rank_value, bipin_cfg->need_capability, reward_str.c_str(),
					(long long)now_second, (long long)give_rank_reward_timestamp, (long long)m_check_rank_reward_timestamp, m_rand_activity_status);
			}
		}	

		if (all_name_len > 0)
		{
			all_name_str[sizeof(all_name_str) - 1] = '\0';

			int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_bipin_rank_reward_notice, 
				all_name_str, m_rand_activity_type, bipin_cfg->reward_item.item_id);
			if (len > 0) 
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_ROLL);
			}
		}
	}
}

void RandActivityBipinCapabilityWash::RankBipinHistory(PersonRankItem top_user_list[Protocol::TOP_COUNT])
{
	if (!this->IsSyncRankData())
	{
		for (int i = 0; i < Protocol::TOP_COUNT; ++ i)
		{
			if (m_data_param.role_id_list[i] <= 0)
			{
				continue;
			}

			top_user_list[i].user_id = IntToUid(m_data_param.role_id_list[i]);
			top_user_list[i].rank_value = m_data_param.rank_value_list[i];

			UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(top_user_list[i].user_id);
			if (NULL == user_node)
			{
				continue;
			}

			UNSTD_STATIC_CHECK(sizeof(top_user_list[i].user_name) == sizeof(user_node->user_name));
			memcpy(top_user_list[i].user_name, user_node->user_name, sizeof(top_user_list[i].user_name));
			top_user_list[i].prof = user_node->prof;
			top_user_list[i].flexible_ll = user_node->avatar_timestamp;
		}
	}
}
