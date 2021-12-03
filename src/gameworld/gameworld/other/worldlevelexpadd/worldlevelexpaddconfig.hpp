#ifndef _WORLDLEVELEXPADDCONFIG_HPP_
#define _WORLDLEVELEXPADDCONFIG_HPP_

#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "servercommon/configbase.h"




struct WorldLevelDifferenceExpAddPer
{
	WorldLevelDifferenceExpAddPer() : min_level(0), max_level(0), exp_add_per(0) {}

	int min_level;												//世界等级经验加成范围的小等级，左区间闭合
	int max_level;												//世界等级经验加成范围的大等级，右区间闭合
	int exp_add_per;											//世界等级经验加成，万分比
};

class WorldLevelDifferenceExpAddConfig : public ConfigBase
{
public:
	WorldLevelDifferenceExpAddConfig() {}

	~WorldLevelDifferenceExpAddConfig() {}

	bool Init(const std::string &configname, std::string *err);

	const WorldLevelDifferenceExpAddPer* GetWorldLevelDifferenceExpAdd(size_t index);
	size_t GetMaxWorldLevelExpAddConfigCount() {	return world_level_difference_exp_add_vec.size(); }
	bool IsNoExpAddType(int type);

private:
	
	int InitWorldLevelDifferenceExpAddPercent(PugiXmlNode RootElement);
	int InitNoExpAddType(PugiXmlNode RootElement);
	std::vector< WorldLevelDifferenceExpAddPer> world_level_difference_exp_add_vec;
	std::vector<short> no_exp_add_type_vec;
};

#endif
