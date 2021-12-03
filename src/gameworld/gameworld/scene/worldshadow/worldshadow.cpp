#include "worldshadow.hpp"
#include "globalconfig/globalconfig.h"
#include "config/sharedconfig/sharedconfig.h"
#include "other/worldlevelexpadd/worldlevelexpadd.hpp"
#include "config/logicconfigmanager.hpp"
#include "other/worldlevelexpadd/worldlevelexpaddconfig.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"

WorldShadow & WorldShadow::Instance()
{
	static WorldShadow ws;
	return ws;
}

WorldShadow::WorldShadow()
	: m_world_level(0), m_top_user_level(0), m_capablity_rank_mincap(0), m_gm_world_level(0), m_is_get_world_level(false), 
	m_hidden_world_level(0)
{

}

WorldShadow::~WorldShadow()
{

}

void WorldShadow::OnSyncWorldLevelInfo(gglobalprotocal::SyncWorldLevelInfo *wli)
{
	m_world_level = wli->world_level;
	m_top_user_level = wli->top_user_level;
	m_capablity_rank_mincap = wli->capability_rank_mincap;

	if (m_world_level < 0) m_world_level = 0;
	else if (m_world_level > MAX_ROLE_LEVEL) m_world_level = MAX_ROLE_LEVEL;

	if (m_top_user_level < 0) m_world_level = 0;
	else if (m_top_user_level > MAX_ROLE_LEVEL) m_top_user_level = MAX_ROLE_LEVEL;
	m_is_get_world_level = true;
}

int WorldShadow::GetWorldLevel()
{
	if (m_gm_world_level > 0)
	{
		return m_gm_world_level;
	}
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return m_hidden_world_level;
	}
	else
	{
		return m_world_level;
	}
}

long long WorldShadow::GetWorLeveldExtraExp(int role_level, long long add_exp)
{
	if (role_level < GLOBALCONFIG->GetWorldLevelOpenLevel()) return 0;												// 小于开放等级
	
	long long world_level_add_exp = (long long)((double)(add_exp) * GetWorLeveldExtraExpPer(role_level) / 100);

	return world_level_add_exp;
}

double WorldShadow::GetWorLeveldExtraExpPer(int role_level)
{
	if (SHAREDCONFIG->GetOpenServerConfig().IsPingbiWorldExpAdd()) return 0;

	if (role_level < GLOBALCONFIG->GetWorldLevelOpenLevel()) return 0;												// 小于开放等级

	double add_percent = 0;
	int world_level = WorldShadow::Instance().GetWorldLevel();
	if (role_level < world_level)
	{
		add_percent = (world_level - role_level) * GLOBALCONFIG->GetWorldLevelExpAddPercentPerLevel();
		if (static_cast<int>(add_percent) > GLOBALCONFIG->GetWorldLevelMaxExpAddPercent())
		{
			add_percent = GLOBALCONFIG->GetWorldLevelMaxExpAddPercent() * 1.0;
		}

		if (static_cast<int>(add_percent) < 0) add_percent = 0.0;
	}
	
	return add_percent;
}

double WorldShadow::GetWorLeveldExtraExpPerByConfig(int role_level, int exp_add_reason)
{
	if (SHAREDCONFIG->GetOpenServerConfig().IsPingbiWorldExpAdd()) return 0;

	if (role_level < GLOBALCONFIG->GetWorldLevelOpenLevel()) return 0;												// 小于开放等级
	if (LOGIC_CONFIG->GetWorldLevelDifferenceExpAddConfig().IsNoExpAddType(exp_add_reason)) return 0;				// 不提供世界等级差加成的类型

	double add_percent = WorldLevelDifferenceExpAdd::Instance().GetWorldLevelDifferenceExpAddPercent(role_level) * 0.0001;
	if (static_cast<int>(add_percent) < 0) add_percent = 0.0;
	

	return add_percent;
}
