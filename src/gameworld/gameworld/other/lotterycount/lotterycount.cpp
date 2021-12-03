#include "lotterycount.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/lotterycountconfig.hpp"
#include "global/worldstatus3/worldstatus3.hpp"
#include "item/itempool.h"

LotteryCount::LotteryCount()
{

}

LotteryCount::~LotteryCount()
{

}

void LotteryCount::SetRole(Role * role)
{
	m_role = role;
}

void LotteryCount::Init(Role *role, const RoleLotteryCountParam &param)
{
	m_role = role;
	m_param = param;
}

void LotteryCount::GetInitParam(RoleLotteryCountParam * param)
{
	*param = m_param;
}

void LotteryCount::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
}

void LotteryCount::OnRoleLogin()
{
}



int LotteryCount::ReplaceLotterySequence(int lottery_type, LotterySequence &lottery_sequence, int* is_replace_garbage_sequence)
{
	if (lottery_type <= LOTTERY_REPLACE_TYPE_INVALID || lottery_type >= LOTTERY_REPLACE_TYPE_MAX)
	{
		return 0;
	}
	int world_count = WorldStatus3::Instance().GetWorldLotteryCount(lottery_type);
	bool is_replace_rare_sequence = false;
	for (LotterySequence i = 0; i < MAX_LOTTERY_REPLACE_SEQUENCE_COUNT; ++i)
	{
		const LotteryCountReplaceSequenceCfg *cfg = LOGIC_CONFIG->GetLotteryCountConfig().GetReplaceListCfg(lottery_type, i);
		if (nullptr == cfg)
		{
			continue;
		}
		if (cfg->can_replace_need_count_person == 0)
		{
			continue;		// 防止没填的序列也加次数
		}
		if (is_replace_rare_sequence == true)
		{
			this->AddRoleLotteryCount(lottery_type, i, 1);
			continue;
		}
		int role_count = this->GetRoleLotteryCount(lottery_type, i);
		// 替换需要达到次数才能抽的序列
		if (role_count < cfg->can_replace_need_count_person)
		{
			if (cfg->replace_sequence[LOTTERY_SEQUENCE_TYPE_RARE] == lottery_sequence)
			{
				lottery_sequence = cfg->replace_sequence[LOTTERY_SEQUENCE_TYPE_GARBAGE];
				if (nullptr != is_replace_garbage_sequence)
				{
					*is_replace_garbage_sequence = 1;
				}
			}
			this->AddRoleLotteryCount(lottery_type, i, 1);
		}
		else
		{
			//达到了次数或世界次数，替换并且重设
			if ((cfg->replace_need_count_person > 0 && role_count >= cfg->replace_need_count_person) || 
				(cfg->replace_need_count_world > 0 && world_count >= cfg->replace_need_count_world))
			{
				lottery_sequence = cfg->replace_sequence[LOTTERY_SEQUENCE_TYPE_RARE];
				this->ResetRoleLotteryCount(lottery_type, i);
				is_replace_rare_sequence = true;
				if (nullptr != is_replace_garbage_sequence)
				{
					*is_replace_garbage_sequence = 0;
				}
			}
			else
			{
				if (cfg->replace_sequence[LOTTERY_SEQUENCE_TYPE_RARE] == lottery_sequence)
				{
					this->ResetRoleLotteryCount(lottery_type, i);
					is_replace_rare_sequence = true;
					if (nullptr != is_replace_garbage_sequence)
					{
						*is_replace_garbage_sequence = 0;
					}
				}
				else
				{
					this->AddRoleLotteryCount(lottery_type, i, 1);
				}
			}
		}
	}
	if (is_replace_rare_sequence == true)
	{
		WorldStatus3::Instance().ResetWorldLotteryCount(lottery_type);
	}
	else
	{
		WorldStatus3::Instance().AddWorldLotteryCount(lottery_type, 1);
	}
	return 0;
}

int LotteryCount::ReplaceLotterySequenceBySeqType(int lottery_type, int seq_type, LotterySequence & lottery_sequence)
{
	if (lottery_type <= LOTTERY_REPLACE_TYPE_INVALID || lottery_type >= LOTTERY_REPLACE_TYPE_MAX || seq_type <= LOTTERY_SEQUENCE_TYPE_INVALID || seq_type >= LOTTERY_SEQUENCE_TYPE_MAX)
	{
		return 0;
	}
	for (LotterySequence i = 0; i < MAX_LOTTERY_REPLACE_SEQUENCE_COUNT; ++i)
	{
		const LotteryCountReplaceSequenceCfg *cfg = LOGIC_CONFIG->GetLotteryCountConfig().GetReplaceListCfg(lottery_type, i);
		if (nullptr == cfg)
		{
			continue;
		}
		lottery_sequence = cfg->replace_sequence[seq_type];
		return 1;
	}
	return 0;
}

int LotteryCount::GetRoleLotteryCount(int lottery_type, LotterySequence sequence)
{
	if (lottery_type <= LOTTERY_REPLACE_TYPE_INVALID || lottery_type >= LOTTERY_REPLACE_TYPE_MAX || sequence < 0 || sequence >= MAX_LOTTERY_REPLACE_SEQUENCE_COUNT)
	{
		return 0;
	}
	return m_param.lottery_count[lottery_type][sequence];
}

bool LotteryCount::AddRoleLotteryCount(int lottery_type, LotterySequence sequence, UserLotteryCount count)
{
	if (lottery_type <= LOTTERY_REPLACE_TYPE_INVALID || lottery_type >= LOTTERY_REPLACE_TYPE_MAX || sequence < 0 || sequence >= MAX_LOTTERY_REPLACE_SEQUENCE_COUNT || count == 0)
	{
		return false;
	}
	UserLotteryCount new_count = m_param.lottery_count[lottery_type][sequence] += count;
	if (new_count < m_param.lottery_count[lottery_type][sequence])
	{
		return false;
	}
	m_param.lottery_count[lottery_type][sequence] = new_count;
	return true;
}

bool LotteryCount::ResetRoleLotteryCount(int lottery_type, LotterySequence sequence)
{
	if (lottery_type <= LOTTERY_REPLACE_TYPE_INVALID || lottery_type >= LOTTERY_REPLACE_TYPE_MAX || sequence < 0 || sequence >= MAX_LOTTERY_REPLACE_SEQUENCE_COUNT)
	{
		return false;
	}
	m_param.lottery_count[lottery_type][sequence] = 0;
	return true;
}