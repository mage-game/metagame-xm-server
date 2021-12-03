#include "randactivityspecialappearancerank.hpp"
#include "obj/character/role.h"
#include "servercommon/struct/itemlistparam.h"
#include "item/itempool.h"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "servercommon/servercommon.h"
#include "other/capability/capability.hpp"

RandActivitySpecialAppearanceRank::RandActivitySpecialAppearanceRank(RandActivityManager *rand_activity_manager)
: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_RANK), m_player_rank_count(0)
{
	memset(m_player_rank_list, 0, sizeof(m_player_rank_list));
}

RandActivitySpecialAppearanceRank::~RandActivitySpecialAppearanceRank()
{

}

void RandActivitySpecialAppearanceRank::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		m_player_rank_count = 0;
		memset(m_player_rank_list, 0, sizeof(m_player_rank_list));
	}
	else if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		this->CheckGiveRankReward();
	}
}

void RandActivitySpecialAppearanceRank::UpdateRankUser(Role *role, int change_num)
{
	if (NULL == role)
	{
		return;
	}

	int cur_uid = role->GetUID();

	for (int index = 0; index < m_player_rank_count && index < RA_SPECIAL_APPEARANCE_RANK_MAX_COUNT; ++ index)
	{
		if (m_player_rank_list[index].uid == cur_uid)
		{
			if (change_num > m_player_rank_list[index].change_num)
			{
				m_player_rank_list[index].change_num = change_num;
			}

			return;
		}
	}


	if (m_player_rank_count < RA_SPECIAL_APPEARANCE_RANK_MAX_COUNT)
	{
		// 排行榜还没有满，直接往后插
		m_player_rank_list[m_player_rank_count].uid = cur_uid;
		m_player_rank_list[m_player_rank_count].change_num = change_num;
		m_player_rank_list[m_player_rank_count].total_capablity = role->GetCapability()->GetTotalCapability();
		F_STRNCPY(m_player_rank_list[m_player_rank_count].user_name, role->GetName(), sizeof(GameName));
		m_player_rank_list[m_player_rank_count].avatar = role->GetAvatar();
		m_player_rank_list[m_player_rank_count].sex = role->GetSex();
		m_player_rank_list[m_player_rank_count].prof = role->GetProf();

		++ m_player_rank_count;

		return;
	}

	// 找到已满排行榜里最小变身次数的玩家
	int min_change_num_role_index = 0;
	for (int index = 1; index < m_player_rank_count && index < RA_SPECIAL_APPEARANCE_RANK_MAX_COUNT; ++ index)
	{
		if (m_player_rank_list[index].change_num < m_player_rank_list[min_change_num_role_index].change_num)
		{
			min_change_num_role_index = index;
		}
	}

	if (min_change_num_role_index >= 0 && min_change_num_role_index < RA_SPECIAL_APPEARANCE_RANK_MAX_COUNT)
	{
		if (change_num > m_player_rank_list[min_change_num_role_index].change_num)
		{
			m_player_rank_list[min_change_num_role_index].uid = cur_uid;
			m_player_rank_list[min_change_num_role_index].change_num = change_num;
			m_player_rank_list[min_change_num_role_index].total_capablity = role->GetCapability()->GetTotalCapability();
			F_STRNCPY(m_player_rank_list[min_change_num_role_index].user_name, role->GetName(), sizeof(GameName));
			m_player_rank_list[m_player_rank_count].avatar = role->GetAvatar();
			m_player_rank_list[m_player_rank_count].sex = role->GetSex();
			m_player_rank_list[m_player_rank_count].prof = role->GetProf();
		}
	}
}

int RandActivitySpecialAppearanceRank::GetRankList(RASpecialAppearacePlayerRankInfo rank_list[RA_SPECIAL_APPEARANCE_RANK_MAX_COUNT])
{
	if (NULL == rank_list)
	{
		return 0;
	}

	memcpy(rank_list, m_player_rank_list, sizeof(m_player_rank_list));

	return m_player_rank_count;
}

int RandActivitySpecialAppearanceRankSortCompare(const void *arg1, const void *arg2)
{
	RASpecialAppearacePlayerRankInfo *s1 = (RASpecialAppearacePlayerRankInfo*)arg1;
	RASpecialAppearacePlayerRankInfo *s2 = (RASpecialAppearacePlayerRankInfo*)arg2;

	if (s1->change_num < s2->change_num)
	{
		return 1;
	}
	else if (s1->change_num > s2->change_num)
	{
		return -1;
	}
	else
	{
		return s2->total_capablity - s1->total_capablity;
	}
}

void RandActivitySpecialAppearanceRank::CheckGiveRankReward()
{
	qsort(m_player_rank_list, RA_SPECIAL_APPEARANCE_RANK_MAX_COUNT, sizeof(RASpecialAppearacePlayerRankInfo), RandActivitySpecialAppearanceRankSortCompare);

	for (int index = 0; index < m_player_rank_count && index < RA_SPECIAL_APPEARANCE_RANK_MAX_COUNT; ++ index)
	{
		const ItemConfigData *rank_reward = LOGIC_CONFIG->GetRandActivityCfg().GetAppearaceRankReward(index + 1);
		if (NULL != rank_reward)
		{
			const ItemBase *item_base = ITEMPOOL->GetItem(rank_reward->item_id);
			if (NULL != item_base)
			{
				static MailContentParam contentparam; contentparam.Reset();
				contentparam.item_list[0].item_id = rank_reward->item_id;
				contentparam.item_list[0].num = rank_reward->num;
				contentparam.item_list[0].is_bind = rank_reward->is_bind;
				contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

				RASpecialAppearacePlayerRankInfo &player = m_player_rank_list[index];
				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_appearace_rank_reward_content, index + 1);
				if (length > 0)
				{
					MailRoute::MailToUser(IntToUid(player.uid), SYSTEM_MAIL_REASON_INVALID, contentparam);

					gamelog::g_log_rand_activity.printf(LL_INFO, "RandActivitySpecialAppearanceRank::CheckGiveRankReward user %d %s, change_num %d, rank %d reward %s,",
						player.uid, player.user_name, player.change_num, (index + 1), ItemConfigDataLog(rank_reward, NULL));
				}
			}
		}
	}
}
