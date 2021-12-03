#include "commonactivityunit.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "engineadapter.h"
#include "config/logicconfigmanager.hpp"
#include "other/commonactivity/commonactivityconfig.hpp"
#include "obj/character/role.h"

CommonActivityUnitManager::CommonActivityUnitManager() : m_role(NULL)
{

}

CommonActivityUnitManager::~CommonActivityUnitManager()
{

}

void CommonActivityUnitManager::Init(Role *role, const CommonActivityParam &common_activity_param)
{
	m_role = role;
	m_param = common_activity_param;
}

void CommonActivityUnitManager::GetInitParam(CommonActivityParam *common_activity_param)
{
	*common_activity_param = m_param;
}

void CommonActivityUnitManager::OnUserLogin()
{
	this->CheckJoinInSameActivity(true);
}

void CommonActivityUnitManager::OnFetchReward(int unit_type, int param1, int param2)
{
	if (!ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMMON_ACTIVITY))
	{
		return;
	}

	if (!LOGIC_CONFIG->GetCommonActivityCfg().IsUnitOpen(unit_type))
	{
		return;
	}

	this->CheckJoinInSameActivity(true);

	bool ret = false;

	switch (unit_type)
	{
	case COMMON_ACTIVITY_UNIT_TYPE_FOR_TEST:
		{
			this->FetchRewardForTest(param1, param2);
		}
		break;
	}

	if (ret)
	{
		this->SendCommonActivityState();
	}
}

void CommonActivityUnitManager::SendCommonActivityState()
{
	if (!ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMMON_ACTIVITY)) 
	{
		return;
	}

	static Protocol::SCCommonActivityOpenUnitInfo caoui;
	caoui.unit_num = 0;

	for (int i = COMMON_ACTIVITY_UNIT_TYPE_INVALID + 1; i < COMMON_ACTIVITY_UNIT_TYPE_MAX && caoui.unit_num < COMMON_ACTIVITY_UNIT_MAX_NUM; i++)
	{		
		if (!LOGIC_CONFIG->GetCommonActivityCfg().IsUnitOpen(i)) continue;

		Protocol::SCCommonActivityOpenUnitInfo::UnitInfo &unit_info = caoui.unit_info_list[caoui.unit_num];
		unit_info.unit_type = i;

		switch (i)
		{
		case COMMON_ACTIVITY_UNIT_TYPE_FOR_TEST:
			{
				unit_info.param1 = 0;
				unit_info.param2 = 0;
				unit_info.reward_state = 0;
			}
			break;
		}

		++ caoui.unit_num;
	}

	int send_len = sizeof(caoui) - sizeof(caoui.unit_info_list[0]) * (COMMON_ACTIVITY_UNIT_MAX_NUM - caoui.unit_num);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&caoui, send_len);
}

void CommonActivityUnitManager::CheckJoinInSameActivity(bool is_notify)
{
	if (!ActivityShadow::Instance().IsInSpecialActivityOpenTime(SPECIAL_ACTIVITY_TYPE_COMMON_ACTIVITY, m_param.last_join_activity_timestamp))
	{
		this->OnSwithNewActivity();

		m_param.last_join_activity_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

		if (is_notify)
		{
			this->SendCommonActivityState();
		}
	}
}

void CommonActivityUnitManager::OnSwithNewActivity()
{
	// 结算奖励之类的一些回调
}

bool CommonActivityUnitManager::FetchRewardForTest(int param1, int patam2)
{
	return true;
}

