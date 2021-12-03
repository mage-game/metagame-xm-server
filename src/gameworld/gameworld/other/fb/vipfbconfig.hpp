#ifndef __VIPFB_CONFIG__
#define __VIPFB_CONFIG__

#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/maildef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "monster/monsterpool.h"
#include <map>

struct VipLevelConfig
{
	VipLevelConfig() : level(0), free_times(0), scene_id(0), birth_pos(0, 0), boss_id(0), boss_pos(0, 0), enter_level(0)
	{
	}

	int level;
	int free_times;
	int scene_id;
	Posi birth_pos;
	UInt16 boss_id;
	Posi boss_pos;
	int enter_level;
	ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
};

class VipFbConfig : public ConfigBase
{
public:
	VipFbConfig();
	~VipFbConfig();

	bool Init(const std::string &configname, std::string *err);

	const VipLevelConfig * GetLevelCfgByMonsterID(UInt16 monster_id);
	const VipLevelConfig * GetLevelCfgByLevel(int pass_level);
	const VipLevelConfig * GetLevelCfgBySceneID(int scene_id);

private:
	int InitLevelCfg(PugiXmlNode RootElement);

	std::map<int, VipLevelConfig> m_level_cfg_map;
};

#endif
