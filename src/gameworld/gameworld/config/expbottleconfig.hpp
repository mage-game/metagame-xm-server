#ifndef __EXP_BOTTLE_HPP__
#define __EXP_BOTTLE_HPP__

#include "gamedef.h"

#include <vector>
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

class TiXmlElement;

struct ExpBottleLvCfg
{
	ExpBottleLvCfg() : min_lv(0), max_lv(0), use_maxcount(0), transform_maxcount(0)
	{

	}

	int min_lv;
	int max_lv;
	int use_maxcount;
	int transform_maxcount;
};

static const int EXP_BOTTLE_LV_CFG_MAX = 10;

struct ExpBottleItemCfg
{
	ExpBottleItemCfg() : seq(0), need_lv(0), cost_exp(0), bottle_itemid(0)
	{

	}
	
	int seq;
	int need_lv;
	int cost_exp;
	int bottle_itemid;
};

static const int EXP_BOTTLE_ITEM_CFG_MAX = 10;

class ExpBottleConfig : public ConfigBase
{
public:
	ExpBottleConfig();
	~ExpBottleConfig();

	bool Init(const std::string &configname, std::string *err);

	const ExpBottleLvCfg * GetExpBottleLvCfg(int role_lv);
	const ExpBottleItemCfg * GetExpBottleItemCfg(int seq);

private:
	int InitLvCfg(PugiXmlNode RootElement);
	int InitItemCfg(PugiXmlNode RootElement);

	ExpBottleLvCfg m_bottle_lv_list[EXP_BOTTLE_LV_CFG_MAX];

	int m_bottle_item_count;
	ExpBottleItemCfg m_bottle_item_list[EXP_BOTTLE_ITEM_CFG_MAX];
};

#endif // __EXP_BOTTLE_HPP__



