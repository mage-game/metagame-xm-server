#include "randactivitychongzhirank2.hpp"
#include "obj/character/role.h"
#include "world.h"
#include "gamelog.h"
#include "global/rank/rankmanager.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "item/itempool.h"

RandActivityChongzhiRank2::RandActivityChongzhiRank2(RandActivityManager *rand_activity_manager) : RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_CHONGZHI_RANK2)
{

}

RandActivityChongzhiRank2::~RandActivityChongzhiRank2()
{

}

void RandActivityChongzhiRank2::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		const int GET_MAX_ROLE_NUM = 100;
		PersonRankItem rank_item_list[GET_MAX_ROLE_NUM];
		//UserID user_id_list[GET_MAX_ROLE_NUM];

		int rank_count = RankManager::Instance().GetPersonRank()->GetRankTopItem(PERSON_RANK_TYPE_RA_CHONGZHI2, GET_MAX_ROLE_NUM, rank_item_list);
		if (rank_count > 0)
		{
			for (int i = 0; i < rank_count && i < GET_MAX_ROLE_NUM; ++i)
			{
				if (INVALID_USER_ID != rank_item_list[i].user_id)
				{
					const RandActivityChongzhiRankCfg::ConfigItem *item_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetChongzhiRank2RewardCfg(i, static_cast<int>(rank_item_list[i].rank_value));
					if (NULL != item_cfg)
					{
						const ItemBase *item_base = ITEMPOOL->GetItem(item_cfg->item.item_id);
						if (NULL != item_base)
						{
							static MailContentParam contentparam; contentparam.Reset();

							contentparam.item_list[0].item_id = item_cfg->item.item_id;
							contentparam.item_list[0].num = item_cfg->item.num;
							contentparam.item_list[0].is_bind = item_cfg->item.is_bind;
							contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

							int rank_count = LOGIC_CONFIG->GetRandActivityCfg().GetChongzhiRank2CfgCount();
							int length = 0;

							if (rank_count > 0 && i < rank_count - 1 && i != item_cfg->rank)
							{
								length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_chongzhi_rank2_reward_content_reserve,
									static_cast<int>(rank_item_list[i].rank_value), i + 1, item_cfg->rank + 1);
							}
							else
							{
								length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_chongzhi_rank2_reward_content,
									static_cast<int>(rank_item_list[i].rank_value), i + 1);
							}

							if (length > 0)
							{
								MailRoute::MailToUser(rank_item_list[i].user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);
							}

							gamelog::g_log_rand_activity.printf(LL_INFO, "RoleActivity::OnRaChongzhiRank2 user[%d] rank[%d] item[%s]",
								UidToInt(rank_item_list[i].user_id), i, ItemConfigDataLog(&(item_cfg->item), NULL));
						}
					}
				}
			}
		}

		RankManager::Instance().ClearPersonRankType(PERSON_RANK_TYPE_RA_CHONGZHI2);
	}
}


