#ifndef __PATAFB_CONFIG__
#define __PATAFB_CONFIG__

#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/maildef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "monster/monsterpool.h"
#include <map>

static const int ROLE_PATA_FB_MAX_LEVEL = 200;

struct PataLevelConfig
{
	PataLevelConfig() : level(0), scene_id(0), birth_pos(0, 0), boss_id(0), boss_pos(0, 0),
		is_broadcast(false), has_normal_reward(false), reward_exp(0), capability(0)
	{
	}

	int level;
	int scene_id;
	Posi birth_pos;
	UInt16 boss_id;
	Posi boss_pos;

	bool is_broadcast;
	ItemConfigData first_reward_list[MAX_ATTACHMENT_ITEM_NUM];	// Ê×Í¨½±Àø
	bool has_normal_reward;
	long long reward_exp;
	ItemConfigData normal_reward_list[MAX_ATTACHMENT_ITEM_NUM];	// ÆÕÍ¨½±Àø

	int capability;
};

struct PataOtherConfig
{
	PataOtherConfig(): add_hp_per(0), add_gongji_per(0), add_role_hurt_buff_percent(0){}

	int add_hp_per;
	int add_gongji_per;
	int add_role_hurt_buff_percent;
};

class PataFbConfig : public ConfigBase
{
public:
	PataFbConfig();
	~PataFbConfig();

	bool Init(const std::string &configname, std::string *err);

	const PataLevelConfig * GetLevelCfgByMonsterID(UInt16 monster_id);
	const PataLevelConfig * GetLevelCfgByLevel(int pass_level);
	const PataOtherConfig & GetOtherCfg(){return m_other_cfg;}

private:
	int InitLevelCfg(PugiXmlNode RootElement);
	int InitOtherCfg(PugiXmlNode RootElement);

	std::map<int, PataLevelConfig> m_level_cfg_map;

	PataOtherConfig m_other_cfg;
};

#endif


