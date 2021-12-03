#include "worldlevelexpaddconfig.hpp"





bool WorldLevelDifferenceExpAddConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;
	PUGI_XML_LOAD_CONFIG("world_level_difference_exp_add_percent", InitWorldLevelDifferenceExpAddPercent);
	PUGI_XML_LOAD_CONFIG("no_exp_add_type", InitNoExpAddType);
	return true;
}

int WorldLevelDifferenceExpAddConfig::InitWorldLevelDifferenceExpAddPercent(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_min_level = -1;
	int last_max_level = -1;
	while (!dataElement.empty())
	{
		WorldLevelDifferenceExpAddPer exp_add_cfg;

		if (!PugiGetSubNodeValue(dataElement, "min_level", exp_add_cfg.min_level) || exp_add_cfg.min_level < 0)
		{
			return -2;
		}
		if (last_min_level >= exp_add_cfg.min_level)
		{
			return -3;
		}
		last_min_level = exp_add_cfg.min_level;
		if (!PugiGetSubNodeValue(dataElement, "max_level", exp_add_cfg.max_level) || exp_add_cfg.max_level < 0)
		{
			return -4;
		}
		if (last_max_level >= exp_add_cfg.max_level)
		{
			return -5;
		}
		if (exp_add_cfg.min_level > exp_add_cfg.max_level)
		{
			return -6;
		}
		last_max_level = exp_add_cfg.max_level;
		if (!PugiGetSubNodeValue(dataElement, "exp_add_per", exp_add_cfg.exp_add_per) || exp_add_cfg.exp_add_per < 0)
		{
			return -7;
		}

		world_level_difference_exp_add_vec.push_back(exp_add_cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int WorldLevelDifferenceExpAddConfig::InitNoExpAddType(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int type = 0;
		if (!PugiGetSubNodeValue(dataElement, "type", type) || type <= Role::EXP_ADD_REASON_INVALID || type >= Role::EXP_ADD_REASON_MAX)
		{
			return -1;
		}
		no_exp_add_type_vec.push_back(type);
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const WorldLevelDifferenceExpAddPer* WorldLevelDifferenceExpAddConfig::GetWorldLevelDifferenceExpAdd(size_t index)
{
	if (index < 0 || index >= world_level_difference_exp_add_vec.size())
	{
		return nullptr;
	}
	return &world_level_difference_exp_add_vec[index];
}

bool WorldLevelDifferenceExpAddConfig::IsNoExpAddType(int type)
{
	for (size_t i = 0; i < no_exp_add_type_vec.size(); ++i)
	{
		if (no_exp_add_type_vec[i] == type)
		{
			return true;
		}
	}
	return false;
}

