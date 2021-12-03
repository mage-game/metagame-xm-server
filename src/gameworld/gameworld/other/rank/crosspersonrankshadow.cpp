#include "crosspersonrankshadow.hpp"
#include "servercommon/crossdef.hpp"
#include <set>
#include "world.h"
#include "obj/character/role.h"
#include "other/achieve/title/titlemanager.hpp"
#include "other/shizhuang/shizhuang.h"

CrossPersonRankShadow & CrossPersonRankShadow::Instance()
{
	static CrossPersonRankShadow _instance;
	return _instance;
}

CrossPersonRankShadow::CrossPersonRankShadow()
{
	for (int i = CROSS_RANK_TOP_TYPE_MIN; i < CROSS_RANK_TOP_TYPE_MAX; ++ i)
	{
		m_rank_title_uuid_info_list[i].rank_type = i;
	}
}

CrossPersonRankShadow::~CrossPersonRankShadow()
{

}

void CrossPersonRankShadow::OnSyncCrossTitleInfo(crossgameprotocal::CrossGameSyncCrossPersonRankTitleInfo *title_info)
{
	if (NULL == title_info) return;

	if (title_info->rank_type < CROSS_RANK_TOP_TYPE_MIN || title_info->rank_type >= CROSS_RANK_TOP_TYPE_MAX)
	{
		return;
	}

	CrossPersonRankTitleUuidInfo &title_uuid_info = m_rank_title_uuid_info_list[title_info->rank_type];
	std::set<int> top_uid_two_day_set;

	for (int i = 0; i < title_uuid_info.uuid_count; ++ i)
	{
		UserID rank_user_id = LongLongToUniqueUserID(title_uuid_info.rank_title_uuid_list[i]).user_id;
		int rank_uid = UidToInt(rank_user_id);

		top_uid_two_day_set.insert(rank_uid);
	}

	title_uuid_info.uuid_count = title_info->uuid_count;
	memset(title_uuid_info.rank_title_uuid_list, 0, sizeof(title_uuid_info.rank_title_uuid_list));

	for (int i = 0; i < title_info->uuid_count; ++ i)
	{
		title_uuid_info.rank_title_uuid_list[i] = title_info->rank_title_user_uuid_list[i];

		UserID rank_user_id = LongLongToUniqueUserID(title_uuid_info.rank_title_uuid_list[i]).user_id;
		int rank_uid = UidToInt(rank_user_id);
		top_uid_two_day_set.insert(rank_uid);
	}

	// 给可能变化的top们发通知
	for (std::set<int>::const_iterator iter = top_uid_two_day_set.begin(); top_uid_two_day_set.end() != iter; ++iter)
	{
		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(*iter));
		if (NULL == role) continue;

		CrossPersonRankInfo personrank_info;
		this->GetTopRankInfo(role->GetUID(), &personrank_info);
		role->GetTitleManager()->OnCrossTopRankInfo(personrank_info, SYNC_REASON_RANK);

		if (CROSS_RANK_TOP_TYPE_1V1 == title_info->rank_type)
		{
			role->GetShizhuang()->CheckActiveByCondition(SHIZHUANG_CHECK_ACTIVE_TYPE_1V1);
		}


	}
}

void CrossPersonRankShadow::GetTopRankInfo(int uid, CrossPersonRankInfo *personrank_info)
{
	if (NULL == personrank_info) return;

	personrank_info->Reset();

	for (int top_type = CROSS_RANK_TOP_TYPE_MIN; top_type < CROSS_RANK_TOP_TYPE_MAX; ++top_type)
	{
		for (int rank = 0; rank < m_rank_title_uuid_info_list[top_type].uuid_count; ++rank)
		{
			UserID rank_user_id = LongLongToUniqueUserID(m_rank_title_uuid_info_list[top_type].rank_title_uuid_list[rank]).user_id;
			int rank_uid = UidToInt(rank_user_id);

			if (uid == rank_uid)
			{
				personrank_info->rank_list[top_type] = rank + 1;
				break;
			}
		}
	}
}

long long CrossPersonRankShadow::GetTopUuid(int top_type, int rank_index)
{
	if (top_type < CROSS_RANK_TOP_TYPE_MIN || top_type >= CROSS_RANK_TOP_TYPE_MAX
		|| rank_index < 0 || rank_index >= CROSS_PERSON_RANK_MAX_SIZE)
	{
		return 0;
	}

	return m_rank_title_uuid_info_list[top_type].rank_title_uuid_list[rank_index];
}


