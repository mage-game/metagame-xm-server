#ifndef __UPGRADE_RUSH_CONFIG_H__
#define __UPGRADE_RUSH_CONFIG_H__

#include "common/tinyxml/tinyxml.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include <map>

struct UpgradeRushReturnCfg
{
	UpgradeRushReturnCfg() : upgrade_type(0), grade(0), limit_time(0), has_reward(false) {}

	int upgrade_type;
	int grade;
	int limit_time;

	bool has_reward;
	ItemConfigData reward_item;
};

enum UPGRADE_RUSH_TYPE
{		
	UPGRADE_RUSH_TYPE_INVALID = 0,
	UPGRADE_RUSH_TYPE_MOUNT,							// 1×øÆï
	UPGRADE_RUSH_TYPE_HALO,								// 2·ÉÉý
	UPGRADE_RUSH_TYPE_FABAO,							// 3·¨±¦
	UPGRADE_RUSH_TYPE_WING,								// 4ÓðÒí
	UPGRADE_RUSH_TYPE_FIGHTMOUNT,						// 5·¨Õó
	UPGRADE_RUSH_TYPE_FOOTPRINT,						// 6×ã¼£
	UPGRADE_RUSH_TYPE_SHENGBING,						// 7Éñ±ø
	UPGRADE_RUSH_TYPE_SHIZHUANG,						// 8Ê±×°

	UPGRADE_RUSH_TYPE_MAX,
};

const static int UPGRADE_RUSH_MAX_TYPE_COUNT = 8;
UNSTD_STATIC_CHECK(UPGRADE_RUSH_MAX_TYPE_COUNT == UPGRADE_RUSH_TYPE_MAX - 1);

class UpgradeRushConfig : public ConfigBase
{
public:
	UpgradeRushConfig();
	~UpgradeRushConfig();

	bool Init(const std::string &configname, std::string *err);

	const UpgradeRushReturnCfg * GetUpgradeRushReturnCfg(int upgrade_type, int grade) const;

private:
	int InitRushReturnCfg(PugiXmlNode RootElement);

	std::map<long long, UpgradeRushReturnCfg> m_upgrade_rush_reward_map;
};

#endif

