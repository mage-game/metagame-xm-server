#include "randactivitycornucopia.hpp"


RandActivityCornucopia::RandActivityCornucopia(RandActivityManager *rand_activity_manager)
	: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_CORNUCOPIA), m_reward_record_count(0), m_reward_record_cursor(0)
{

}

RandActivityCornucopia::~RandActivityCornucopia()
{

}

void RandActivityCornucopia::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		m_reward_record_count = 0;
		m_reward_record_cursor = 0;

		for (int i = 0; i < LUCKY_ROLL_REWARD_RECORD_MAX_COUNT; ++ i)
		{
			m_reward_record_list[i].Reset();
		}
	}
}

void RandActivityCornucopia::AddRewardRecord(const RACornucopiaRewardRecord &record)
{
	if (record.user_id <= 0)
	{
		return;
	}

	int index = m_reward_record_cursor % RAND_ACTIVITY_CORNUCOPIA_RECORD_MAX_COUNT;
	m_reward_record_list[index] = record;
	m_reward_record_cursor = index + 1;

	if (m_reward_record_count < RAND_ACTIVITY_CORNUCOPIA_RECORD_MAX_COUNT)
	{
		++ m_reward_record_count;
	}
}

int RandActivityCornucopia::GetRewardRecordList(RACornucopiaRewardRecord reward_record_list[RAND_ACTIVITY_CORNUCOPIA_RECORD_MAX_COUNT])
{
	int get_record_count = 0;

	if (m_reward_record_count > 0)
	{
		int start_index = (m_reward_record_cursor - m_reward_record_count + RAND_ACTIVITY_CORNUCOPIA_RECORD_MAX_COUNT) % RAND_ACTIVITY_CORNUCOPIA_RECORD_MAX_COUNT;

		for (int i = 0; i < m_reward_record_count && get_record_count < RAND_ACTIVITY_CORNUCOPIA_RECORD_MAX_COUNT; ++ i)
		{
			int curr_idx = (start_index + i) % RAND_ACTIVITY_CORNUCOPIA_RECORD_MAX_COUNT;

			reward_record_list[get_record_count] = m_reward_record_list[curr_idx];
			++ get_record_count;
		}
	}

	return get_record_count;
}

