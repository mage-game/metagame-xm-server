#ifndef __TURN_TABLE__
#define __TURN_TABLE__

#include <string>
#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

static const int TURNTABLE_REWARDS_MAX_COUNT = 8;

enum TURNTABLE_TYPE
{
	TURNTABLE_TYPE_MILLIIONAIRE = 0,

	TURNTABLE_TYPE_MAX_COUNT = 16,
};

UNSTD_STATIC_CHECK(0 == TURNTABLE_TYPE_MAX_COUNT % 2 && 16 == TURNTABLE_TYPE_MAX_COUNT);

struct TurnTableRewardCfg
{
	TurnTableRewardCfg() : item_index(0), weight(0), is_notice(false)
	{
	}

	int item_index;
	int weight;
	bool is_notice;
	ItemConfigData item;
};

struct TurnTableRollCfg
{
	TurnTableRollCfg() : type(-1), great_index(0), lucky_max(0), need_item(0), need_num(0)
	{
		memset(rewards, 0, sizeof(rewards));
	}

	int type;
	int great_index;
	int lucky_max;
	ItemID need_item;
	int need_num;
	TurnTableRewardCfg rewards[TURNTABLE_REWARDS_MAX_COUNT];
};

class TurnTableConfig : public ConfigBase
{
public:
	TurnTableConfig();

	~TurnTableConfig();

	bool Init(const std::string &path, std::string *err);

	const TurnTableRollCfg * GetRollCfgList(int req_type);

private:
	int InitTurnTableRewardsCfg(PugiXmlNode RootElement);
	int InitTurnTableRollCfg(PugiXmlNode RootElement);

	int m_roll_cfg_count;
	TurnTableRollCfg m_roll_cfg_list[TURNTABLE_TYPE_MAX_COUNT];
};

#endif 

