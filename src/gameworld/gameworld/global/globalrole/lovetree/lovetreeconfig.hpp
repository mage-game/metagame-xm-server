#ifndef __LOVETREE_CONFIG_HPP__
#define __LOVETREE_CONFIG_HPP__

#include "servercommon/lovetreedef.hpp"
#include "common/tinyxml/tinyxml.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include <map>

struct LoveTreeLevelConfig
{
	LoveTreeLevelConfig(): tree_star(0), need_exp(0), baoji_add_exp_rate(0), extra_add_exp_per(0), maxhp(0), gongji(0), fangyu(0), shanbi(0), mingzhong(0), baoji(0), jianren(0), add_exp(0) {}

	int tree_star;
	int need_exp;
	int baoji_add_exp_rate;
	int extra_add_exp_per;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute shanbi;
	Attribute mingzhong;
	Attribute baoji;
	Attribute jianren;
	int add_exp;
	ItemConfigData male_up_star_item;
	ItemConfigData female_up_star_item;
};

struct LoveTreeOtherConfig
{
	LoveTreeOtherConfig(): self_free_water_time(0), assist_free_water_time(0), rename_cost(0), marry_add_per(0) {}

	int self_free_water_time;
	int assist_free_water_time;
	int rename_cost;
	int marry_add_per;
};

class LoveTreeCfg : public ConfigBase
{
public:
	LoveTreeCfg();
	~LoveTreeCfg();

	bool Init(std::string path, std::string *err);

	const LoveTreeLevelConfig * GetLoveTreeLevelCfg(int star_level);
	const LoveTreeOtherConfig & GetLoveTreeOtherCfg() { return m_other_cfg; }

private:
	
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitUpStarLevelCfg(PugiXmlNode RootElement);
	
	LoveTreeOtherConfig m_other_cfg;
	std::map<int, LoveTreeLevelConfig> m_love_tree_star_level_map;
};


#endif // __LITTLEPET_CONFIG_HPP__
