#include "randactivityluckroll.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"

const static long long INIT_BIND_GOLD_POLL = 1000;

RandActivityLuckRoll::RandActivityLuckRoll(RandActivityManager *rand_activity_manager)
: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_LUCKY_ROLL), m_reward_record_count(0), m_reward_record_cursor(0)
{

}

RandActivityLuckRoll::~RandActivityLuckRoll()
{
 
}

void RandActivityLuckRoll::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data.data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data.data;
	m_data_param = *pTemp;

	// ³õÊ¼½±½ð³Ø¿ÉÅäÖÃ
	const RandActivityLuckyRollCfg *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLuckyRollCfg();
	if (NULL != cfg)
	{
		if (m_data_param.gold_poll <= 0)
		{
			m_data_param.gold_poll = cfg->cfg.get_gold_min_gold_poll;
		}
	}
	else
	{
		if (m_data_param.gold_poll <= 0)
		{
			m_data_param.gold_poll = INIT_BIND_GOLD_POLL;
		}
	}
}

void RandActivityLuckRoll::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data->data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data->data;
	*pTemp = m_data_param;
}

void RandActivityLuckRoll::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		if (m_data_param.last_open_time != m_begin_time)
		{
			m_data_param.last_open_time = m_begin_time;
		}
	}
	else if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		const RandActivityLuckyRollCfg *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLuckyRollCfg();
		if (NULL != cfg)
		{
			m_data_param.gold_poll = cfg->cfg.get_gold_min_gold_poll;
		}
		else
		{
			m_data_param.gold_poll = INIT_BIND_GOLD_POLL;
		}

		m_reward_record_count = 0;
		m_reward_record_cursor = 0;

		for (int i = 0; i < LUCKY_ROLL_REWARD_RECORD_MAX_COUNT; ++ i)
		{
			m_reward_record_list[i].Reset();
		}
	}
}

void RandActivityLuckRoll::AddGoldPoll(long long add_value)
{
	m_data_param.gold_poll += add_value;

	if (m_data_param.gold_poll < 0)
	{
		m_data_param.gold_poll = 0;
	}
}

void RandActivityLuckRoll::AddRewardRecord(const LuckyRollRewardRecord &record)
{
	if (record.user_id <= 0)
	{
		return;
	}

	int index = m_reward_record_cursor % LUCKY_ROLL_REWARD_RECORD_MAX_COUNT;
	m_reward_record_list[index] = record;
	m_reward_record_cursor = index + 1;

	if (m_reward_record_count < LUCKY_ROLL_REWARD_RECORD_MAX_COUNT)
	{
		++ m_reward_record_count;
	}
}

int RandActivityLuckRoll::GetRewardRecordList(LuckyRollRewardRecord reward_record_list[LUCKY_ROLL_REWARD_RECORD_MAX_COUNT])
{
	memcpy(reward_record_list, m_reward_record_list, sizeof(m_reward_record_list));
	return m_reward_record_count;
}