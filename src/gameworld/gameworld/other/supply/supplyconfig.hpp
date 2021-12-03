#ifndef __SUPPLY_CONFIG_HPP__
#define __SUPPLY_CONFIG_HPP__

#include "servercommon/roleshopdef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct SupplyItem
{
	SupplyItem() : price(0), value(0), is_use_gold(false) {}

	int price;
	int value;
	bool is_use_gold;
};

struct SupplyTypeCfg
{
	SupplyTypeCfg() : recover_interval_s(0), item_count(0) 
	{
		memset(recover_value_list, 0, sizeof(recover_value_list));
	}

	int recover_interval_s;

	int item_count;
	SupplyItem supply_item_list[MAX_SUPPLY_ITEM_COUNT]; 

	int recover_value_list[MAX_ROLE_LEVEL + 1];
};

class SupplyConfig : public ConfigBase
{
public:
	SupplyConfig();
	~SupplyConfig();

	bool Init(const std::string &path, std::string *err);

	const SupplyItem * GetSupplyItem(int supply_type, int index);

	int GetSupplyRecoverValue(int supply_type, int level);
	int GetSupplyRecoverInterval(int supply_type);

	int GetNotFightRate() { return m_not_fight_rate; }
	int GetRecoverPer(){return m_recover_per;}

private:
	SupplyTypeCfg m_supply_cfg_list[SUPPLY_TYPE_MAX];

	int m_not_fight_rate;
	int m_recover_per;
};

#endif // __SUPPLY_CONFIG_HPP__

