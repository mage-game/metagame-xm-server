#include "randactivitydayconsumerank.hpp"
#include "world.h"
#include "gamelog.h"
#include "scene/scenemanager.h"
#include "global/randactivity/randactivitymanager.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "global/rank/rankmanager.hpp"
#include "other/route/mailroute.hpp"
#include "servercommon/string/gameworldstr.h"
#include "item/itempool.h"
#include <string>
#include "global/usercache/usercache.hpp"

RandActivityDayConsumeRank::RandActivityDayConsumeRank(RandActivityManager *rand_activity_manager)
: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_DAY_CONSUME_RANK)
{

}

RandActivityDayConsumeRank::~RandActivityDayConsumeRank()
{
 
}

void RandActivityDayConsumeRank::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data.data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data.data;
	m_data_param = *pTemp;
}

void RandActivityDayConsumeRank::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data->data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data->data;
	*pTemp = m_data_param;
}

void RandActivityDayConsumeRank::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		m_data_param.give_reward_last_day = 0;
	}
	else if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		time_t now_second = EngineAdapter::Instance().Time();
		time_t now_zero_timestamp = GetZeroTime(now_second);
		time_t begin_zero_timestamp = GetZeroTime(m_begin_time);

		int give_day = GetDayIndex(begin_zero_timestamp, now_zero_timestamp);
		if (give_day == m_data_param.give_reward_last_day)
		{
			give_day += 1;
		}

		this->CheckGiveReward(give_day, true);
	}
}

void RandActivityDayConsumeRank::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid) return;

	time_t now_second = EngineAdapter::Instance().Time();
	time_t now_zero_timestamp = GetZeroTime(now_second);
	time_t begin_zero_timestamp = GetZeroTime(m_begin_time);

	int give_day = GetDayIndex(begin_zero_timestamp, now_zero_timestamp);
	this->CheckGiveReward(give_day, false);
}

void RandActivityDayConsumeRank::CheckGiveReward(int day, bool ignore_status)
{
	if (!ignore_status)
	{
		if (ACTIVITY_STATUS_CLOSE == m_rand_activity_status)
		{
			return;
		}
	}

	PersonRank *person_rank = RankManager::Instance().GetPersonRank();
	if (NULL == person_rank) return;

	person_rank->Snapshot();

	int rank_type = PERSON_RANK_TYPE_RA_DAY_CONSUME_NUM;
	PersonRankItem rank_item_list[PERSON_RANK_MAX_SIZE];
	int count = person_rank->GetRankTopItem(rank_type, PERSON_RANK_MAX_SIZE, rank_item_list);

	for (int i = 0; i < count; ++ i)
	{
		PersonRankItem &rank_item = rank_item_list[i];
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(rank_item.user_id);
		if (NULL == user_node)
		{
			continue;
		}

		int gold_num = static_cast<int>(rank_item.rank_value);
		const RandActivityDayConsumeRankRewardConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetDayConsumeRankRewardCfg(day, i, gold_num);
		if (NULL != cfg_item)
		{
			// 发邮件
			static MailContentParam contentparam; contentparam.Reset();
		
			contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_GOLD, static_cast<long long>(gold_num * 0.01 * cfg_item->fanli_rate));

			int reward_count = 0;
			for (int j = 0; j < cfg_item->item_count && reward_count < MAX_ATTACHMENT_ITEM_NUM; ++ j)
			{
				const ItemConfigData &item_data = cfg_item->reward_item_list[j];
				const ItemBase *item_base = ITEMPOOL->GetItem(item_data.item_id);

				if (NULL != item_base)
				{
					contentparam.item_list[reward_count].item_id = item_data.item_id;
					contentparam.item_list[reward_count].num = item_data.num;
					contentparam.item_list[reward_count].is_bind = (item_data.is_bind) ? 1 : 0;
					contentparam.item_list[reward_count].invalid_time = item_base->CalInvalidTime();
					++ reward_count;
				}
			}

			int length = 0;
			// 实际在第i+1名，但是消费金额不够
			if (i < cfg_item->limit_rank)
			{
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_consume_gold_rank_reward_content_reserve,
					m_rand_activity_type, gold_num, i + 1, cfg_item->limit_rank + 1);
			}
			else
			{
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_day_rank_consume_reweard_content,
					m_rand_activity_type, gold_num, i + 1);
			}
			if (length > 0) 
			{
				MailRoute::MailToUser(rank_item.user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);
			}

			std::string reward_str;
			ItemConfigDataListLog(reward_str, cfg_item->item_count, cfg_item->reward_item_list);

			gamelog::g_log_rand_activity.printf(LL_INFO, "RandActivityDayConsumeRank::CheckGiveReward user %d %s, ignore_status %d, give_day %d, rank %d, gold_num %d"
				"reward_limit_rank %d, reward_limit_gold %d, reward_item_count %d, reward_items %s",
				UidToInt(rank_item.user_id), rank_item.user_name, (ignore_status ? 1 : 0), day, i, gold_num, 
				cfg_item->limit_rank, cfg_item->limit_gold, reward_count, reward_str.c_str());
		}
		else
		{
			gamelog::g_log_rand_activity.printf(LL_INFO, "RandActivityDayConsumeRank::CheckGiveReward user %d %s, ignore_status %d, give_day %d, rank %d, gold_num %d, no reward",
				UidToInt(rank_item.user_id), rank_item.user_name, (ignore_status ? 1 : 0), day, i, gold_num);
		}
	}

	m_data_param.give_reward_last_day = day;
	person_rank->ClearRank(rank_type);
	person_rank->Snapshot();
	World::GetInstWorld()->GetSceneManager()->OnRADayConsumeRankGiveReward();
}


