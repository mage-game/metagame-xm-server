#include "upgraderush.hpp"
#include "gamecommon.h"
#include "config/logicconfigmanager.hpp"
#include "engineadapter.h"
#include "servercommon/noticenum.h"
#include "item/knapsack.h"
#include "protocal/msgother.h"

UpgradeRush::UpgradeRush() : m_role(NULL)
{
	
}

UpgradeRush::~UpgradeRush()
{
	
}

void UpgradeRush::Init(Role *role, const unsigned int upgrade_return_timestamp_list[UPGRADE_RUSH_MAX_TYPE_COUNT])
{
	m_role = role;
	memcpy(m_upgrade_return_end_time_list, upgrade_return_timestamp_list, sizeof(m_upgrade_return_end_time_list));
}

void UpgradeRush::GetInitParam(unsigned int upgrade_return_timestamp_list[UPGRADE_RUSH_MAX_TYPE_COUNT])
{
	memcpy(upgrade_return_timestamp_list, m_upgrade_return_end_time_list, sizeof(m_upgrade_return_end_time_list));
}

void UpgradeRush::OnRoleLogin()
{
	this->SendToClient();
}

void UpgradeRush::OnUpgradeLevelChange(int upgrade_type, int new_grade)
{
	return;  // ÆÁ±Î¹¦ÄÜ

	if (upgrade_type <= UPGRADE_RUSH_TYPE_INVALID || upgrade_type >= UPGRADE_RUSH_TYPE_MAX)
	{
		return;
	}

	const UpgradeRushReturnCfg * return_config = LOGIC_CONFIG->GetUpgradeRushConfig().GetUpgradeRushReturnCfg(upgrade_type, new_grade);
	if (NULL == return_config) return;

	unsigned int now_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	unsigned int &last_upgrade_end_timestamp = m_upgrade_return_end_time_list[upgrade_type - 1];

	if (last_upgrade_end_timestamp > 0 && now_timestamp < last_upgrade_end_timestamp && return_config->has_reward)
	{
		m_role->GetKnapsack()->PutOrMail(return_config->reward_item, PUT_REASON_UPGRADE_RUSH_RETURN);
		m_role->NoticeNum(noticenum::NT_UPGRADE_RUSH_RETURN_REWARD);
	}
	
	if (return_config->limit_time > 0)
	{
		last_upgrade_end_timestamp = now_timestamp + return_config->limit_time * SECOND_PER_HOUR;
	}
	else
	{
		last_upgrade_end_timestamp = 0;
	}

	this->SendToClient();
}

void UpgradeRush::SendToClient()
{
	static Protocol::SCUpgradeRushInfo uri;
	memcpy(uri.upgrade_return_end_time_list, m_upgrade_return_end_time_list, sizeof(uri.upgrade_return_end_time_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&uri, sizeof(uri));
}
