#include "randactivitymarryme.hpp"
#include "global/randactivity/randactivitymanager.hpp"

#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "item/itempool.h"
#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "obj/character/role.h"
#include "servercommon/struct/global/worldstatusparam.hpp"
#include "global/worldstatus/worldstatus.hpp"

RandActivityMarryMe::RandActivityMarryMe(RandActivityManager *rand_activity_manager)
: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_MARRY_ME)
{

}

RandActivityMarryMe::~RandActivityMarryMe()
{

}

void RandActivityMarryMe::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data.data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data.data;
	m_data_param = *pTemp;
}

void RandActivityMarryMe::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data->data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data->data;
	*pTemp = m_data_param;
}

void RandActivityMarryMe::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
}

void RandActivityMarryMe::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		m_data_param.couple_count = 0;

		for (int index = 0; index < RA_MARRY_SHOW_COUPLE_COUNT_MAX; ++ index)
		{
			m_data_param.oga_show_couple_list[index].Reset();
		}
	}
}

bool RandActivityMarryMe::OnRoleAcceptProposal(Role *role, Role *propose_role)
{
	//if (m_data_param.couple_count >= RA_MARRY_REWARD_COUPLE_COUNT_MAX)
	//{
	//	return false;
	//}

	int index = -1;
	for (int i = 0; i < RA_MARRY_SHOW_COUPLE_COUNT_MAX; i++)
	{
		if (!m_data_param.oga_show_couple_list[i].is_valid())
		{
			index = i;
			break;
		}
	}

	if (index >= 0 && index < RA_MARRY_SHOW_COUPLE_COUNT_MAX && NULL != role && NULL != propose_role)
	{
		m_data_param.oga_show_couple_list[index].SetCouple(role->GetUID(), propose_role->GetUID());
	}

	m_data_param.couple_count ++;

	return true;
}

bool RandActivityMarryMe::IsRewardCountLimit()
{
	//if (LOGIC_CONFIG->GetRandActivityCfg().IsMarryMeRewardCountLimit(m_data_param.couple_count))
	//{
	//	return true;
	//}

	return false;
}

int RandActivityMarryMe::GetCoupleInfoList(CoupleInfo couple_info_list[RA_MARRY_SHOW_COUPLE_COUNT_MAX])
{
	if (NULL == couple_info_list)
	{
		return 0;
	}
	else
	{
		memcpy(couple_info_list, m_data_param.oga_show_couple_list, sizeof(couple_info_list[0]) * RA_MARRY_SHOW_COUPLE_COUNT_MAX);
	}

	return m_data_param.couple_count;
}
