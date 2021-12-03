#ifndef _SHENZHUANG_CFG_HPP_
#define _SHENZHUANG_CFG_HPP_

#include "servercommon/serverdef.h"
#include "servercommon/shenzhuangdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <set>

struct SZOtherCfg
{
	SZOtherCfg() : drop_level_max_times(0), gain_max_times(0)
	{
	}

	int drop_level_max_times;
	int gain_max_times;
};

struct SZUplevelItemCfg
{
	SZUplevelItemCfg() : index(0), level(0), stuff_id(0), stuff_num(0), gongji(0), fangyu(0),
		maxhp(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), red_ratio_1(0), red_ratio_2(0), red_ratio_3(0), pink_ratio(0), downlevel_rate(0)
	{
	}

	int index;
	int level;
	int stuff_id;
	int stuff_num;

	Attribute gongji;
	Attribute fangyu;
	Attribute maxhp;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	int red_ratio_1;
	int red_ratio_2;
	int red_ratio_3;
	int pink_ratio;

	int downlevel_rate;
};

struct SZPartItemCfg
{
	SZPartItemCfg() : part_index(0) {}

	int part_index;
	SZUplevelItemCfg uplevel_list[SHENZHUANG_MAX_LEVEL + 1];
};

class ShenZhuangConfig : public ConfigBase
{
public:
	ShenZhuangConfig();
	~ShenZhuangConfig();

	bool Init(const std::string &configname, std::string *err);

	const SZUplevelItemCfg *GetUplevelItemCfg(int part_index, int level);
	int GetMaxLevel() { return m_max_level; }

	bool IsDownScene(int scene_id);
	int GetRewardPercent(int exceed_level);
	const SZOtherCfg & GetOtherCfg() { return m_other_cfg; }

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitUplevelConfig(PugiXmlNode RootElement);
	int InitDownSceneConfig(PugiXmlNode RootElement);
	int InitRewardPercentConfig(PugiXmlNode RootElement);

	int m_max_level;

	SZPartItemCfg m_part_list[SHENZHUANG_MAX_PART];

	std::set<int> m_scene_id_set;

	static const int REWARD_LEVEL_MAX = 100;
	int m_reward_level_max;
	int m_reward_percent_list[REWARD_LEVEL_MAX + 1];

	SZOtherCfg m_other_cfg;
};

#endif
