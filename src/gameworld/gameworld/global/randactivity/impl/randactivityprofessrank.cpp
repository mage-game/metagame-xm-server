#include "randactivityprofessrank.hpp"
#include "global/rank/rankmanager.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "item/itempool.h"
#include "obj/character/role.h"
#include "gamecommon.h"

RandActivityProfessRank::RandActivityProfessRank(RandActivityManager *rand_activity_manager) : RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_PROFESS_RANK)
{

}

RandActivityProfessRank::~RandActivityProfessRank()
{

}

void RandActivityProfessRank::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		const int GET_MAX_ROLE_NUM = 100;
		//UserID user_id_list[GET_MAX_ROLE_NUM];
		PersonRankItem rank_item_list[GET_MAX_ROLE_NUM];
		
		// ·ÖÄÐÅ®°ñ
		for (int rank_type = PERSON_RANK_TYPE_RA_PROFESS_MALE; rank_type <= PERSON_RANK_TYPE_RA_PROFESS_FEMALE; rank_type++)
		{
			int rank_count = RankManager::Instance().GetPersonRank()->GetRankTopItem(rank_type, GET_MAX_ROLE_NUM, rank_item_list);
			if (rank_count <= 0)
			{
				continue;
			}

			for (int i = 0; i < rank_count && i < GET_MAX_ROLE_NUM; ++i)
			{
				if (INVALID_USER_ID == rank_item_list[i].user_id)
				{
					continue;
				}

				const RandActivityProfessRankCfg::ConfigItem *item_data = LOGIC_CONFIG->GetRandActivityCfg().GetProfessRankRewardCfg(i);
				if (NULL == item_data)
				{
					continue;
				}

				if (0 == item_data->item.item_id || 0 == item_data->female_item.item_id)
				{
					continue;
				}

				int score = static_cast<int>(rank_item_list[i].rank_value);

				if (score >= item_data->limit_score)
				{
					const ItemConfigData item = (rank_type == PERSON_RANK_TYPE_RA_PROFESS_MALE) ? item_data->item : item_data->female_item;

					gamecommon::SendMultiMailToRole(rank_item_list[i].user_id, &item, 1, NULL, false, "profess_rank_reward", score, i + 1);
				}
				else
				{
					static MailContentParam contentparam; contentparam.Reset();
					int length = gstr::MailContent(contentparam.contenttxt, sizeof(contentparam.contenttxt), "profess_rank_score_not_enough", score, i + 1);
					if (length > 0)
					{
						MailRoute::MailToUser(rank_item_list[i].user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);
					}
				}

				gamelog::g_log_rand_activity.printf(LL_INFO, "RandActivityProfessRank::OnSpecialActivityStatusChange::user[%d], rank_type[%d], rank_val[%d], rank[%d]",
					UidToInt(rank_item_list[i].user_id), rank_type, static_cast<int>(rank_item_list[i].rank_value), i + 1);
			}

			RankManager::Instance().ClearPersonRankType(rank_type);
		}
	}
}


