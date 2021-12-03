#include "randactivitycrystaldalaran.hpp"
#include "config/logicconfigmanager.hpp"
#include "global/rank/personrank.hpp"
#include "global/rank/rankmanager.hpp"
#include "item/itempool.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "gamelog.h"
#include "config/activityconfig/randactivityconfig.hpp"

RandActivityCrytalDalaran::RandActivityCrytalDalaran(RandActivityManager *rand_activity_manager) :
RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_CRYSTAL_DALARAN)
{

}

RandActivityCrytalDalaran::~RandActivityCrytalDalaran()
{

}

void RandActivityCrytalDalaran::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		static PersonRankItem steal_coin_rank_list[RA_CRYSTAL_DALARAN_RANK_REWARD_COUNT];
		int rank_list_count = RankManager::Instance().GetPersonRank()->GetRankTopItem(PERSON_RANK_CRYSTA_DALARAN_STEAL_NUM, RA_CRYSTAL_DALARAN_RANK_REWARD_COUNT, steal_coin_rank_list);
		if (rank_list_count > 0)
		{
			for (int i  = 0; i < rank_list_count && i < RA_CRYSTAL_DALARAN_RANK_REWARD_COUNT; i++)
			{
				const RandActivityStealCrtstalDalaranRewardCfg *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetStealCrystalDalaranRewardCfg(i + 1);
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

						int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_steal_crystal_dalaran_reward_content, 
							RAND_ACTIVITY_TYPE_CRYSTAL_DALARAN, i + 1);
						if (length > 0)
						{
							MailRoute::MailToUser(steal_coin_rank_list[i].user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);

							gamelog::g_log_rand_activity.printf(LL_INFO, "RandActivityStealCoin::OnSpecialActivityStatusChange rank_type[%d], user[%d,%s], rank[%d], rank_value[%lld], reward[%s]",
								RAND_ACTIVITY_TYPE_CRYSTAL_DALARAN, UidToInt(steal_coin_rank_list[i].user_id), steal_coin_rank_list[i].user_name, i + 1, steal_coin_rank_list[i].rank_value, ItemConfigDataLog(&reward_cfg->reward_item, NULL));
						}
					}
				}
			}
		}

		static PersonRankItem be_stealed_coin_rank_list[RA_CRYSTAL_DALARAN_RANK_REWARD_COUNT];
		int be_stealed_rank_list_count = RankManager::Instance().GetPersonRank()->GetRankTopItem(PERSON_RANK_CRYSTA_DALARAN_BE_STEALED_NUM, RA_CRYSTAL_DALARAN_RANK_REWARD_COUNT, be_stealed_coin_rank_list);
		if (be_stealed_rank_list_count > 0)
		{
			for (int i  = 0; i < be_stealed_rank_list_count && i < RA_CRYSTAL_DALARAN_RANK_REWARD_COUNT; i++)
			{
				const RandActivityStealCrtstalDalaranRewardCfg *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetStealCrystalDalaranRewardCfg(i + 1);
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

						int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_crystal_dalaran_be_stealed_reward_content, 
							RAND_ACTIVITY_TYPE_CRYSTAL_DALARAN, i + 1);
						if (length > 0)
						{
							MailRoute::MailToUser(be_stealed_coin_rank_list[i].user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);

							gamelog::g_log_rand_activity.printf(LL_INFO, "RandActivityStealCoin::OnSpecialActivityStatusChange rank_type[%d], user[%d,%s], rank[%d], rank_value[%lld], reward[%s]",
								RAND_ACTIVITY_TYPE_CRYSTAL_DALARAN, UidToInt(be_stealed_coin_rank_list[i].user_id), be_stealed_coin_rank_list[i].user_name, i + 1, be_stealed_coin_rank_list[i].rank_value, ItemConfigDataLog(&reward_cfg->be_stealed_reward_item, NULL));
						}
					}
				}
			}
		}

		RankManager::Instance().ClearPersonRankType(PERSON_RANK_CRYSTA_DALARAN_STEAL_NUM);
		RankManager::Instance().ClearPersonRankType(PERSON_RANK_CRYSTA_DALARAN_BE_STEALED_NUM);
	}
}