//#include "stdafx.h"

#include "randactivityperfectlover.hpp"
#include "global/worldstatus2/worldstatus2.hpp"

RandActivityPerfectLover::RandActivityPerfectLover(RandActivityManager *rand_activity_manager) : RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_PERFECT_LOVER)
{

}

RandActivityPerfectLover::~RandActivityPerfectLover()
{

}

void RandActivityPerfectLover::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		WorldStatus2CommonDataParam &common_data_2 = WorldStatus2::Instance().GetCommonData2();

		common_data_2.perfect_lover_couple_count = 0;
		memset(common_data_2.perfect_lover_role_id_list, 0, sizeof(common_data_2.perfect_lover_role_id_list));
	}

}

void RandActivityPerfectLover::OnCoupleMarryRAPerfectLover(int role_id1, int role_id2)
{
	WorldStatus2CommonDataParam &common_data_2 = WorldStatus2::Instance().GetCommonData2();

	if (common_data_2.perfect_lover_couple_count < RAND_PERFECT_LOVER_MAX_RECORD_COUPLE_COUNT)
	{
		common_data_2.perfect_lover_role_id_list[common_data_2.perfect_lover_couple_count * 2] = role_id1;
		common_data_2.perfect_lover_role_id_list[common_data_2.perfect_lover_couple_count * 2 + 1] = role_id2;
	}

	common_data_2.perfect_lover_couple_count++;
}

int RandActivityPerfectLover::GetPerfectLoverCoupleNum()
{
	WorldStatus2CommonDataParam &common_data_2 = WorldStatus2::Instance().GetCommonData2();

	return common_data_2.perfect_lover_couple_count;
}

int RandActivityPerfectLover::GetPerfectLoverIdByIndex(int index)
{
	WorldStatus2CommonDataParam &common_data_2 = WorldStatus2::Instance().GetCommonData2();
	return common_data_2.perfect_lover_role_id_list[index];
}

