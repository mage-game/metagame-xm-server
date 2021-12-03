#ifndef _ZHANSHENFBCFG_HPP_
#define _ZHANSHENFBCFG_HPP_

#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "monster/monsterpool.h"
#include <map>

const static int ZHANSHEN_LEVEL_MAX_COUNT = 100;			// 总计100层
const static int ZHANSHEN_DAY_REWARD_MAX_COUNT = 5;			// 每日奖励5种物品

struct ZhanShenDianLevelCfg
{
	ZhanShenDianLevelCfg () : level(0), scene_id(0), monster_id(0), monster_pos(0, 0), brith_pos(0, 0), is_broadcast(false),
							  day_reward_count(0)
	{

	}

	int level;
	int scene_id;
	UInt16 monster_id;
	Posi monster_pos;
	Posi brith_pos;
	bool is_broadcast;

	ItemConfigData pass_raward;

	int day_reward_count;
	ItemConfigData day_reward_list[ZHANSHEN_DAY_REWARD_MAX_COUNT];
};

class ZhanShenDianConfig : public ConfigBase
{
public:
	ZhanShenDianConfig();
	~ZhanShenDianConfig();

	bool Init(const std::string &configname, std::string *err);

	const ZhanShenDianLevelCfg * GetZhanShenDianLevelCfgByMonster(UInt16 monster_id);
	const ZhanShenDianLevelCfg * GetZhanShenDianLevelCfgByLevel(int pass_level);

private:
	int InitLevelCfg(PugiXmlNode RootElement);

	int m_zhanshen_max_level;
	ZhanShenDianLevelCfg m_zhanshen_level_cfg_list[ZHANSHEN_LEVEL_MAX_COUNT + 1];

	std::map<UInt16, ZhanShenDianLevelCfg> m_level_cfg_map;
};

#endif


