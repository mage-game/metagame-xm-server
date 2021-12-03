#include "randactivityconsumegoldrank.hpp"
#include "global/rank/rankmanager.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "item/itempool.h"
#include "world.h"
#include "obj/character/role.h"

RandActivityConsumeGoldRank::RandActivityConsumeGoldRank(RandActivityManager *rand_activity_manager) : RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_CONSUME_GOLD_RANK)
{

}

RandActivityConsumeGoldRank::~RandActivityConsumeGoldRank()
{

}

void RandActivityConsumeGoldRank::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		const int GET_MAX_ROLE_NUM = 100;
		//UserID user_id_list[GET_MAX_ROLE_NUM];
		PersonRankItem rank_item_list[GET_MAX_ROLE_NUM];

		int rank_count = RankManager::Instance().GetPersonRank()->GetRankTopItem(PERSON_RANK_TYPE_RA_CONSUME_GOLD, GET_MAX_ROLE_NUM, rank_item_list);
		if (rank_count > 0)
		{
			for (int i = 0; i < rank_count && i < GET_MAX_ROLE_NUM; ++ i)
			{
				if (INVALID_USER_ID != rank_item_list[i].user_id)
				{
					const int need_chongzhi = LOGIC_CONFIG->GetRandActivityCfg().GetConsumeGoldRankNeedChonghzi(i);
					const RandActivityConsumeGoldRankCfg::ConfigItem *item_data = LOGIC_CONFIG->GetRandActivityCfg().GetConsumeGoldRankRewardCfg(i, static_cast<int>(rank_item_list[i].rank_value));
					if (NULL != item_data)
					{
						const ItemBase *item_base = ITEMPOOL->GetItem(item_data->item.item_id);
						if (NULL != item_base)
						{
							static MailContentParam contentparam; contentparam.Reset();

							contentparam.item_list[0].item_id = item_data->item.item_id;
							contentparam.item_list[0].num = item_data->item.num;
							contentparam.item_list[0].is_bind = item_data->item.is_bind;
							contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

							int rank_count = LOGIC_CONFIG->GetRandActivityCfg().GetConsumeRankCfgCount();
							int length = 0;

							if (rank_count > 0 && i < rank_count - 1 && need_chongzhi > (int)rank_item_list[i].rank_value)
							{
								length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_consume_gold_rank_reward_content_reserve, 
									RAND_ACTIVITY_TYPE_CONSUME_GOLD_RANK, static_cast<int>(rank_item_list[i].rank_value), i + 1, item_data->rank + 1);
							}
							else
							{
								length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_consume_gold_rank_reward_content, 
									RAND_ACTIVITY_TYPE_CONSUME_GOLD_RANK, static_cast<int>(rank_item_list[i].rank_value), i + 1);
							}

							if (length > 0)
							{
								MailRoute::MailToUser(rank_item_list[i].user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);
							}

							gamelog::g_log_rand_activity.printf(LL_INFO, "RoleActivity::OnRaConsumeGoldRank user[%d] rank[%d] item[%s]",
								UidToInt(rank_item_list[i].user_id), i, ItemConfigDataLog(&(item_data->item), NULL));
						}
					}
				}
			}
		}

		RankManager::Instance().ClearPersonRankType(PERSON_RANK_TYPE_RA_CONSUME_GOLD);
	}
}


