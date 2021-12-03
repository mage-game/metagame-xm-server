#ifndef __YAOJIANGCONFIG_HPP__
#define __YAOJIANGCONFIG_HPP__

#include "servercommon/configcommon.h"
#include "servercommon/yaojiangdef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include <vector>
#include <map>

class Role;

struct YaojiangItemCfg
{
	YaojiangItemCfg() : weight(0), is_notice(false) {}

	int weight;																	// 随机权重
	bool is_notice;																// 是否广播
	ItemConfigData item;														// 物品配置
};

struct YaojiangItemPool
{
	YaojiangItemPool() : cost_item_id(0), total_weight(0), item_count(0) {}

	ItemID cost_item_id;
	int total_weight;
	int item_count;
	YaojiangItemCfg item_list[YAOJIANG_ITEM_NUM];
};

class YaojiangConfig : public ConfigBase
{
public:
	YaojiangConfig();
	~YaojiangConfig();

	bool Init(const std::string &path, std::string *err);

	ItemID GetCostItemId(int yaojiang_type);
	const YaojiangItemCfg * RandYaojiangItemCfg(int yaojiang_type, int *index);

private:
	typedef std::map<int, YaojiangItemPool> YaojiangItemPoolMap;
	YaojiangItemPoolMap m_yaojiang_item_pool_map;
};

#endif // __YAOJIANGCONFIG_HPP__

