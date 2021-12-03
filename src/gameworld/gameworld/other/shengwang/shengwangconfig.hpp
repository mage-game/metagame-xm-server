#ifndef _SHENGWANG_CFG_HPP_
#define _SHENGWANG_CFG_HPP_

#include <string>
#include <map>
#include "servercommon/serverdef.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "obj/character/attribute.hpp"
#include "item/itembase.h"
#include "item/itemgriddata.h"

struct XianJieLevelCfg
{
	XianJieLevelCfg() : level(0), shengwang(0), 
		gongji(0), fangyu(0), maxhp(0), mingzhong(0), is_broadcast(0) {}

	short level;
	int shengwang;

	Attribute gongji;
	Attribute fangyu;
	Attribute maxhp;
	Attribute mingzhong;

	short is_broadcast;
};

struct XianDanLevelCfg
{
	XianDanLevelCfg() : level(0), xianjie_limit(0), max_times(0), 
		once_cost_shengwang(0), once_add_gongji(0), once_add_fangyu(0), once_add_maxhp(0), once_add_mingzhong(0), add_baoji_on_act(0), add_kangbao_on_act(0){}

	int level;
	short xianjie_limit;
	int max_times;

	int once_cost_shengwang;
	Attribute once_add_gongji;
	Attribute once_add_fangyu;
	Attribute once_add_maxhp;
	Attribute once_add_mingzhong;
	Attribute add_baoji_on_act;
	Attribute add_kangbao_on_act;
};

struct XianDanGoldConfig
{
	XianDanGoldConfig(): times_min(0), times_max(0), cost_gold(0) {}

	int times_min;
	int times_max;
	int cost_gold;
};

class ShengWangConfig : public ConfigBase
{
public:
	ShengWangConfig();
	~ShengWangConfig();

	bool Init(const std::string &configname, std::string *err);

	int InitXianJieLevelConfig(PugiXmlNode RootElement);
	int InitXianDanLevelConfig(PugiXmlNode RootElement);
	int InitXianDanGoldConfig(PugiXmlNode RootElement);

	const XianJieLevelCfg *GetXianJieLevelCfg(short level);
	const XianDanLevelCfg *GetXianDanLevelCfg(short level);

	void GetXianDanUpLevelCostGold(int times, int *cost_gold);
	int GetMaxXianDanLevel() { return m_max_xiandan_level; }

private:
	short m_max_xiandan_level;

	std::map<short, XianJieLevelCfg> m_xianjie_map;
	std::map<short, XianDanLevelCfg> m_xiandan_map;
	std::map<int, XianDanGoldConfig> m_xiandan_gold_map;
};

#endif


