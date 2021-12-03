#include "crossramanagerlocal.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "servercommon/internalprotocal/crossgamerandactivitymsg.h"
#include "crossralocal.hpp"
#include "impl/crossralocalchongzhirank.hpp"
#include "impl/crossralocalconsumerank.hpp"
#include "impl/crossralocalcloudpurchase.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "protocal/msgcsrandactivity.hpp"
#include "config/activityconfig/activityswitchconfig.hpp"
#include "config/logicconfigmanager.hpp"

CrossRAManagerLocal & CrossRAManagerLocal::Instance()
{
	static CrossRAManagerLocal instance;
	return instance;
}

CrossRAManagerLocal::CrossRAManagerLocal()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	UNSTD_STATIC_CHECK(3 == CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT);

	m_rand_activity_list[CROSS_RAND_ACTIVITY_TYPE_CHONGZHI_RANK - CROSS_RAND_ACTIVITY_TYPE_BEGIN] = new CrossRALocalChongzhiRank();
	m_rand_activity_list[CROSS_RAND_ACTIVITY_TYPE_CLOUD_PURCHASE - CROSS_RAND_ACTIVITY_TYPE_BEGIN] = new CrossRALocalCloudPurchase();
	m_rand_activity_list[CROSS_RAND_ACTIVITY_TYPE_CONSUME_RANK - CROSS_RAND_ACTIVITY_TYPE_BEGIN] = new CrossRALocalConsumeRank();
}

CrossRAManagerLocal::~CrossRAManagerLocal()
{

}

void CrossRAManagerLocal::OnSyncActivityData(crossgameprotocal::CrossGameSyncCrossRandActivityData * srad, int length)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	CrossRALocal * act = this->GetActivity(srad->activity_type);
	if (NULL == act)
	{
		return;
	}

	int header_len = sizeof(crossgameprotocal::CrossGameSyncCrossRandActivityData);
	int msg_len = length - header_len;

	if (msg_len <= 0)
	{
		return;
	}

	crossgameramsg::MessageHeader *header = (crossgameramsg::MessageHeader *)((char*)srad + header_len);

	if (header->msg_type > crossgameramsg::MSG_COMMON_BEGIN && header->msg_type <= crossgameramsg::MSG_COMMON_END)
	{
		act->OnCommonMessageFromCross(header->msg_type, (char*)header);
	}
	else if (header->msg_type > crossgameramsg::MSG_ACT_BEGIN && header->msg_type <= crossgameramsg::MSG_ACT_END)
	{
		act->OnMessageFromCross(header->msg_type, (char*)header);
	}
}

void CrossRAManagerLocal::Init(const CrossRandActivityParam &param, const CrossRandActivityBigParam &big_param)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	for (int act_type = CROSS_RAND_ACTIVITY_TYPE_BEGIN; act_type < CROSS_RAND_ACTIVITY_TYPE_END; ++ act_type)
	{
		CrossRALocal *activity = this->GetActivity(act_type);
		if (NULL != activity)
		{
			int act_index = act_type - CROSS_RAND_ACTIVITY_TYPE_BEGIN;
			if (act_index >= 0 && act_index < CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT)
			{
				activity->Init(param.data_list[act_index]);
			}

			activity->SetBigData(&m_act_big_param);
		}
	}

	m_act_big_param = big_param;
}

void CrossRAManagerLocal::Init2(const CrossRandActivityBigParam2 &big_param)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	for (int act_type = CROSS_RAND_ACTIVITY_TYPE_BEGIN; act_type < CROSS_RAND_ACTIVITY_TYPE_END; ++act_type)
	{
		CrossRALocal *activity = this->GetActivity(act_type);
		if (NULL != activity)
		{
			activity->SetBigData2(&m_act_big_param2);
		}
	}

	m_act_big_param2 = big_param;
}

void CrossRAManagerLocal::GetInitParam(CrossRandActivityParam *param, CrossRandActivityBigParam *big_param)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	for (int act_type = CROSS_RAND_ACTIVITY_TYPE_BEGIN; act_type < CROSS_RAND_ACTIVITY_TYPE_END; ++ act_type)
	{
		CrossRALocal *activity = this->GetActivity(act_type);
		if (NULL != activity)
		{
			int act_index = act_type - CROSS_RAND_ACTIVITY_TYPE_BEGIN;
			activity->GetInitParam(&param->data_list[act_index]);
		}
	}

	*big_param = m_act_big_param;
}

void CrossRAManagerLocal::GetInitParam2(CrossRandActivityBigParam2 *big_param)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	*big_param = m_act_big_param2;
}

void CrossRAManagerLocal::Update(unsigned long interval, time_t now_second)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	for (int act_index = 0; act_index < CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT; ++ act_index)
	{
		if (NULL != m_rand_activity_list[act_index])
		{
			m_rand_activity_list[act_index]->Update(interval, (unsigned int)now_second);
		}
	}
}

void CrossRAManagerLocal::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	for (int act_index = 0; act_index < CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT; ++ act_index)
	{
		if (NULL != m_rand_activity_list[act_index])
		{
			m_rand_activity_list[act_index]->OnDayChange();
		}
	}
}

void CrossRAManagerLocal::OnWeekChange()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	for (int act_index = 0; act_index < CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT; ++ act_index)
	{
		if (NULL != m_rand_activity_list[act_index])
		{
			m_rand_activity_list[act_index]->OnWeekChange();
		}
	}
}

void CrossRAManagerLocal::OnUserLogin(Role *user)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	for (int act_index = 0; act_index < CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT; ++ act_index)
	{
		if (NULL != m_rand_activity_list[act_index])
		{
			m_rand_activity_list[act_index]->OnUserLogin(user);
		}
	}
}

void CrossRAManagerLocal::OnUserLogout(Role *user)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	for (int act_index = 0; act_index < CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT; ++ act_index)
	{
		if (NULL != m_rand_activity_list[act_index])
		{
			m_rand_activity_list[act_index]->OnUserLogout(user);
		}
	}
}

bool CrossRAManagerLocal::OnForceSetActivityTime(int rand_activity_type, unsigned int begin_time, unsigned int end_time)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return false;
	}
	
	if (LOGIC_CONFIG->GetActivitySwitchConfig().IsBanActivity(rand_activity_type)) return false;

	CrossRALocal *act = this->GetActivity(rand_activity_type);
	if (NULL != act)
	{
		return act->OnForceSetActivityTime(begin_time, end_time);
	}

	return false;
}

void CrossRAManagerLocal::OnForceToNextState(int rand_activity_type)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	if (LOGIC_CONFIG->GetActivitySwitchConfig().IsBanActivity(rand_activity_type)) return;

	CrossRALocal *act = this->GetActivity(rand_activity_type);
	if (NULL != act)
	{
		act->OnForceToNextState();
	}
}

CrossRALocal * CrossRAManagerLocal::GetActivity(int activity_type)
{
	int act_index = activity_type - CROSS_RAND_ACTIVITY_TYPE_BEGIN;
	if (act_index < 0 || act_index >= CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT)
	{
		return NULL;
	}

	return m_rand_activity_list[act_index];
}