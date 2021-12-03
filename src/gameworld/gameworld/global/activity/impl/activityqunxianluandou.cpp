#include "activityqunxianluandou.hpp"

#include "engineadapter.h"
#include "protocal/msgother.h"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/qunxianluandouconfig.hpp"

#include "global/rank/rankmanager.hpp"
#include "global/worldstatus/worldstatus.hpp"

ActivityQunXianLuanDou::ActivityQunXianLuanDou(ActivityManager *activity_manager)
	: Activity(activity_manager, ACTIVITY_TYPE_QUNXIANLUANDOU),m_room_count(0)
{

}

ActivityQunXianLuanDou::~ActivityQunXianLuanDou()
{

}

void ActivityQunXianLuanDou::OnActivityStatusChange(int from_status, int to_status)
{
	switch (to_status)
	{
		case ACTIVITY_STATUS_OPEN:
			{
				this->ClearRankTitle();
			}
			break;

		case ACTIVITY_STATUS_CLOSE:
			{
				RankManager::Instance().ClearPersonRankType(PERSON_RANK_TYPE_QUNXIANLUANDOU);
			}
			break;
	}

	Activity::OnActivityStatusChange(from_status, to_status);

	switch (to_status)
	{
	case ACTIVITY_STATUS_CLOSE:
		{
			RankManager::Instance().Snapshot();
		}
		break;
	}
}

int ActivityQunXianLuanDou::GetRoomMaxUserCount()
{
	int max_role_num = LOGIC_CONFIG->GetQunXianLuanDouConfig().GetRoomMaxRoleNum();
	if (max_role_num <= 0)
	{
		max_role_num = 200;
	}

	return max_role_num;
}

bool ActivityQunXianLuanDou::IsRecordRoomStatus(int *activity_sceneid)
{
	if (NULL != activity_sceneid)
	{
		*activity_sceneid = LOGIC_CONFIG->GetQunXianLuanDouConfig().GetEnterInfo(0, NULL);
	}

	return true;
}

void ActivityQunXianLuanDou::CheckFirstThreeRankUser(QunXianLuanDouUserRankItem rank_list[], int list_len)
{
	if (list_len != QUNXIANLUANDOU_SIDE_MAX)
	{
		return;
	}

	for (int i = 0; i < QUNXIANLUANDOU_SIDE_MAX; ++i)
	{
		if (m_total_rank_list[i].score < rank_list[i].score)
		{
			m_total_rank_list[i] = rank_list[i];
		}
	}

	++m_room_count;
	if (m_room_count >= ACTIVITY_ROOM_MAX)
	{
		m_room_count = 0;

		int rank_title_user_id_list[QUNXIANLUANDOU_SIDE_MAX] = { 0 };

		for (int i = 0; i < QUNXIANLUANDOU_SIDE_MAX; ++i)
		{
			if (m_total_rank_list[i].user_id.role_id > 0)
			{
				rank_title_user_id_list[i] = UidToInt(m_total_rank_list[i].user_id);
			}
		}

		WorldStatus::Instance().SetQunxianlundouSideFirstUser(rank_title_user_id_list, QUNXIANLUANDOU_SIDE_MAX);
	}
}

void ActivityQunXianLuanDou::ClearRankTitle()
{
	memset(m_total_rank_list, 0, sizeof(m_total_rank_list));
	int rank_title_user_id_list[QUNXIANLUANDOU_SIDE_MAX] = { 0 };
	WorldStatus::Instance().SetQunxianlundouSideFirstUser(rank_title_user_id_list, QUNXIANLUANDOU_SIDE_MAX);
}
