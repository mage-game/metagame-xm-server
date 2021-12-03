#include "worldlevelexpadd.hpp"




WorldLevelDifferenceExpAdd & WorldLevelDifferenceExpAdd::Instance()
{
	static WorldLevelDifferenceExpAdd wldea;
	return wldea;
}

WorldLevelDifferenceExpAdd::WorldLevelDifferenceExpAdd() {}

WorldLevelDifferenceExpAdd::~WorldLevelDifferenceExpAdd() {}


int WorldLevelDifferenceExpAdd::GetWorldLevelDifferenceExpAddPercent(int role_level)
{
	int world_level = WorldShadow::Instance().GetWorldLevel();
	if (role_level > world_level)
	{
		return 0;
	}
	int level_difference = world_level - role_level;
	size_t count = LOGIC_CONFIG->GetWorldLevelDifferenceExpAddConfig().GetMaxWorldLevelExpAddConfigCount();
	for (size_t i = 0; i < count; ++i)
	{
		const WorldLevelDifferenceExpAddPer* exp_add_cfg = LOGIC_CONFIG->GetWorldLevelDifferenceExpAddConfig().GetWorldLevelDifferenceExpAdd(i);
		if (nullptr == exp_add_cfg)
		{
			return 0;
		}
		if (level_difference >= exp_add_cfg->min_level && level_difference <= exp_add_cfg->max_level)
		{
			return exp_add_cfg->exp_add_per;
		}
	}

	return 0;
}

