#ifndef _CHENGJIU_CFG_HPP_
#define _CHENGJIU_CFG_HPP_

#include <string>
#include <map>
#include "servercommon/serverdef.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "obj/character/attribute.hpp"
#include "item/itembase.h"
#include "item/itemgriddata.h"

struct TitleLevelCfg
{
	TitleLevelCfg() : level(0), chengjiu(0), 
		gongji(0), fangyu(0), maxhp(0), shanbi(0), is_broadcast(0) {}

	short level;
	int chengjiu;

	Attribute gongji;
	Attribute fangyu;
	Attribute maxhp;
	Attribute shanbi;

	short is_broadcast;
};

struct ChengJiuRewardCfg
{
	ChengJiuRewardCfg() : reward_id(0), condition(0), param1(0), chengjiu(0), bind_gold(0) {}

	short reward_id;
	short condition;
	unsigned int param1;

	int chengjiu;
	int bind_gold;
};

struct FuwenLevelCfg
{
	FuwenLevelCfg() : level(0), title_limit(0), max_times(0), 
		once_cost_chengjiu(0), once_add_gongji(0), once_add_fangyu(0), once_add_maxhp(0), once_add_shanbi(0), add_baoji_on_act(0), add_kangbao_on_act(0){}

	int level;
	short title_limit;
	int max_times;

	int once_cost_chengjiu;
	int once_add_gongji;
	int once_add_fangyu;
	int once_add_maxhp;
	int once_add_shanbi;	
	int add_baoji_on_act;	
	int add_kangbao_on_act;
};

struct FuwenGoldConfig
{
	FuwenGoldConfig(): times_min(0), times_max(0), cost_gold(0) {}

	int times_min;
	int times_max;
	int cost_gold;
};

class ChengJiuConfig : public ConfigBase
{
public:
	ChengJiuConfig();
	~ChengJiuConfig();

	bool Init(const std::string &configname, std::string *err);

	int InitTitleConfig(PugiXmlNode RootElement);
	int InitRewardConfig(PugiXmlNode RootElement);
	int InitFuwenConfig(PugiXmlNode RootElement);
	int InitFuwenGoldConfig(PugiXmlNode RootElement);

	const TitleLevelCfg *GetTitleLevelCfg(short level);
	const ChengJiuRewardCfg *GetRewardCfg(short reward_id);
	const FuwenLevelCfg *GetFuwenLevelCfg(short level);

	std::map<short, ChengJiuRewardCfg> *GetRewardMap() { return &m_reward_map; }

	void GetFuwenUpLevelCostGold(int times, int *cost_gold);

	short GetRewardMaxNum() { return m_reward_max_num; }
	short GetMaxFuwenLevel() { return m_max_fuwen_level; }

private:
	short m_reward_max_num;
	short m_max_fuwen_level;

	std::map<short, TitleLevelCfg> m_title_map;
	std::map<short, ChengJiuRewardCfg> m_reward_map;
	std::map<short, FuwenLevelCfg> m_fuwen_map;
	std::map<int, FuwenGoldConfig> m_fuwen_gold_map;
};

#endif
