#include "randactivityfishing.hpp"
#include "config/logicconfigmanager.hpp"
#include "global/rank/personrank.hpp"
#include "global/rank/rankmanager.hpp"
#include "item/itempool.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "gamelog.h"
#include "config/activityconfig/randactivityconfig.hpp"


RandActivityFishing::RandActivityFishing(RandActivityManager *rand_activity_manager) :
RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_FISHING)
{

}

RandActivityFishing::~RandActivityFishing()
{

}

void RandActivityFishing::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		static PersonRankItem fishing_rank_list[RA_FISHING_RANK_MAX_COUNT];
		//偷取榜奖励
		int rank_list_count = RankManager::Instance().GetPersonRank()->GetRankTopItem(PERSON_RANK_FISH_PLACE, RA_FISHING_RANK_MAX_COUNT, fishing_rank_list);
		if (rank_list_count > 0)
		{
			for (int i  = 0; i < rank_list_count && i < RA_FISHING_RANK_MAX_COUNT; i++)
			{
				RandActivityFishingRankCfg *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetFishingRewardInfo(i + 1);
				if (NULL != reward_cfg)
				{
					const ItemBase *item_base = ITEMPOOL->GetItem(reward_cfg->reward_item.item_id);
					if (NULL != item_base)
					{
						static MailContentParam contentparam; contentparam.Reset();

						contentparam.item_list[0].item_id = reward_cfg->reward_item.item_id;
						contentparam.item_list[0].num = reward_cfg->reward_item.num;
						contentparam.item_list[0].is_bind = reward_cfg->reward_item.is_bind;
						contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

						int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_steal_fish_rank_content, 
							RAND_ACTIVITY_TYPE_FISHING, i + 1);
						if (length > 0)
						{
							MailRoute::MailToUser(fishing_rank_list[i].user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);

							gamelog::g_log_rand_activity.printf(LL_INFO, "RandActivityStealCoin::OnSpecialActivityStatusChange rank_type[%d], user[%d,%s], rank[%d], rank_value[%lld], reward[%s]",
								RAND_ACTIVITY_TYPE_FISHING, UidToInt(fishing_rank_list[i].user_id), fishing_rank_list[i].user_name, i + 1, fishing_rank_list[i].rank_value, ItemConfigDataLog(&reward_cfg->reward_item, NULL));
						}
					}
				}
			}
		}

		//被偷取榜奖励
		static PersonRankItem be_steal_fish_rank_list[RA_FISHING_RANK_MAX_COUNT];
		int be_stealed_rank_list_count = RankManager::Instance().GetPersonRank()->GetRankTopItem(PERSON_RANK_STEAL_FISH_PLACE, RA_FISHING_RANK_MAX_COUNT, be_steal_fish_rank_list);
		if (be_stealed_rank_list_count > 0)
		{
			for (int i  = 0; i < be_stealed_rank_list_count && i < RA_FISHING_RANK_MAX_COUNT; i++)
			{
				RandActivityFishingRankCfg *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetFishingRewardInfo(i + 1);
				if (NULL != reward_cfg)
				{
					const ItemBase *item_base = ITEMPOOL->GetItem(reward_cfg->be_stealed_reward_item.item_id);
					if (NULL != item_base)
					{
						static MailContentParam contentparam; contentparam.Reset();

						contentparam.item_list[0].item_id = reward_cfg->be_stealed_reward_item.item_id;
						contentparam.item_list[0].num = reward_cfg->be_stealed_reward_item.num;
						contentparam.item_list[0].is_bind = reward_cfg->be_stealed_reward_item.is_bind;
						contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

						int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_be_steal_fish_rank_content, 
							RAND_ACTIVITY_TYPE_FISHING, i + 1);
						if (length > 0)
						{
							MailRoute::MailToUser(be_steal_fish_rank_list[i].user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);

							gamelog::g_log_rand_activity.printf(LL_INFO, "RandActivityStealCoin::OnSpecialActivityStatusChange rank_type[%d], user[%d,%s], rank[%d], rank_value[%lld], reward[%s]",
								RAND_ACTIVITY_TYPE_FISHING, UidToInt(be_steal_fish_rank_list[i].user_id), be_steal_fish_rank_list[i].user_name, i + 1, be_steal_fish_rank_list[i].rank_value, ItemConfigDataLog(&reward_cfg->be_stealed_reward_item, NULL));
						}
					}
				}
			}
		}

		//活动结束清空榜单
		RankManager::Instance().ClearPersonRankType(PERSON_RANK_FISH_PLACE);
		RankManager::Instance().ClearPersonRankType(PERSON_RANK_STEAL_FISH_PLACE);
	}
}