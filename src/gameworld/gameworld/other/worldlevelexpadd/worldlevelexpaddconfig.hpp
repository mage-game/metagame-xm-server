#ifndef _WORLDLEVELEXPADDCONFIG_HPP_
#define _WORLDLEVELEXPADDCONFIG_HPP_

#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "servercommon/configbase.h"




struct WorldLevelDifferenceExpAddPer
{
	WorldLevelDifferenceExpAddPer() : min_level(0), max_level(0), exp_add_per(0) {}

	int min_level;												//����ȼ�����ӳɷ�Χ��С�ȼ���������պ�
	int max_level;												//����ȼ�����ӳɷ�Χ�Ĵ�ȼ���������պ�
	int exp_add_per;											//����ȼ�����ӳɣ���ֱ�
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
