#include "rolefbzhanshendian.hpp"
#include "engineadapter.h"
#include "protocal/msgother.h"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "scene/scene.h"
#include "servercommon/errornum.h"

RoleZhanshenDian::RoleZhanshenDian() 
	: m_role(NULL), m_zhanshendian_pass_level(0), m_yesterday_pass_level(0), m_fetch_day_reward_timestamp(0)
{
}

RoleZhanshenDian::~RoleZhanshenDian()
{
}

void RoleZhanshenDian::Init(Role *role, short zhanshendian_pass_level, short yesterday_pass_level, unsigned int fetch_day_reward_timestamp)
{
	m_role = role;

	m_zhanshendian_pass_level = zhanshendian_pass_level;
	m_yesterday_pass_level = yesterday_pass_level;
	m_fetch_day_reward_timestamp = fetch_day_reward_timestamp;
}

void RoleZhanshenDian::GetInitParam(short *zhanshendian_pass_level, short *yesterday_pass_level, unsigned int *fetch_day_reward_timestamp)
{
	*zhanshendian_pass_level = m_zhanshendian_pass_level;
	*yesterday_pass_level = m_yesterday_pass_level;
	*fetch_day_reward_timestamp = m_fetch_day_reward_timestamp;
}

void RoleZhanshenDian::SetPassLevel(int set_level)
{
// 	if (set_level < 0)
// 	{
// 		set_level = 0;
// 	}
// 
// 	if (set_level > ZHANSHEN_LEVEL_MAX_COUNT)
// 	{
// 		set_level = ZHANSHEN_LEVEL_MAX_COUNT;
// 	}
// 	
// 	if (m_zhanshendian_pass_level != set_level)
// 	{
// 		m_zhanshendian_pass_level = static_cast<short>(set_level);
// 
// 		m_role->GetTitleManager()->RecalZhanShenDianTitle();
// 		this->SendInfo();
// 	}
}

void RoleZhanshenDian::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
// 	if (old_dayid != now_dayid)
// 	{
// 		m_yesterday_pass_level = m_zhanshendian_pass_level;
// 		m_fetch_day_reward_timestamp = 0;
// 
// 		this->SendInfo();
// 	}
}

void RoleZhanshenDian::FetchDayReward()
{
// 	if (m_yesterday_pass_level <= 0 || m_yesterday_pass_level > m_zhanshendian_pass_level || m_yesterday_pass_level > ZHANSHEN_LEVEL_MAX_COUNT)
// 	{
// 		return;
// 	}
// 
// 	if (m_fetch_day_reward_timestamp > 0)
// 	{
// 		m_role->NoticeNum(errornum::EN_ZHANSHENDIAN_ALREADY_FETCH_DAY_REWARD);
// 		return;
// 	}
// 
// 	if (m_role->GetSublock()->CheckHasLock())
// 	{
// 		return;
// 	}
// 
// 	if (m_role->GetKnapsack()->IsLock())
// 	{
// 		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
// 		return;
// 	}
// 
// 	const ZhanShenDianLevelCfg *cfg = LOGIC_CONFIG->GetZhanShenDianCfg().GetZhanShenDianLevelCfgByLevel(m_yesterday_pass_level);
// 	if (NULL == cfg)
// 	{
// 		return;
// 	}
// 
// 	if (cfg->day_reward_count <= 0)
// 	{
// 		return;
// 	}
// 
// 	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(cfg->day_reward_count))
// 	{
// 		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
// 		return;
// 	}
// 
// 	if (m_role->GetKnapsack()->PutList(cfg->day_reward_count, cfg->day_reward_list, PUT_REASON_ZHANSHENDIAN_DAY_REWARD))
// 	{
// 		m_fetch_day_reward_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
// 		this->SendInfo();
// 	}
}

void RoleZhanshenDian::SendInfo()
{
// 	Protocol::SCZhanShenDianFBInfo zsdfbi;
// 	zsdfbi.pass_level = m_zhanshendian_pass_level;
// 	zsdfbi.yesterday_pass_level = m_yesterday_pass_level;
// 	zsdfbi.fetch_day_reward_timestamp = m_fetch_day_reward_timestamp;
// 	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&zsdfbi, sizeof(zsdfbi));
}


