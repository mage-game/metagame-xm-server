#ifndef __BUILD_TOWER_FB_CONFIG_HPP__
#define __BUILD_TOWER_FB_CONFIG_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configcommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "monster/monsterinitparam.h"
#include <map>

static const int BUILD_TOWER_MAX_TYPE = 4;
static const int BUILD_TOWER_MAX_LEVEL = 5;
static const int BUILD_TOWER_MAX_TOWER_POS_INDEX = 19;
static const int BUILD_TOWER_MAX_PATH_POS_INDEX = 7;
static const int BUILD_TOWER_MAX_MONSTER_WAVE = 9;
static const int BUILD_TOWER_MAX_DROP_ID_NUM = 64;

struct BuildTowerFbOtherCfg
{
	BuildTowerFbOtherCfg() : scene_id(0), born_pos(0, 0), enter_level(0), enter_free_times(0), day_buy_times(0), buy_times_gold(0), sweep_need_gold(0), sweep_item_id(0), sweep_item_num(0),
		prepare_time_second(0), two_wave_second(0), extra_monster_id(0), extra_monster_maxhp_gongji_times(0), extra_call_gold(0), tower_plant_range(0), init_douhun(0), escape_num_to_failure(0),
		max_extra_monster_num(0),add_speed_per(0){}

	int scene_id;
	Posi born_pos;
	int enter_level;
	int enter_free_times;
	int day_buy_times;
	int buy_times_gold;
	int sweep_need_gold;
	ItemID sweep_item_id;
	int sweep_item_num;
	int prepare_time_second;
	int two_wave_second;
	int extra_monster_id;
	int extra_monster_maxhp_gongji_times;
	int extra_call_gold;
	int tower_plant_range;
	int init_douhun;
	int escape_num_to_failure;
	int max_extra_monster_num;
	short add_speed_per;
};

struct BuildTowerFbTowerCfg
{
	BuildTowerFbTowerCfg() : tower_type(0) {}

	struct ConfigItem
	{
		ConfigItem() : tower_level(0), monster_id(0), need_douhun(0), return_douhun(0){}

		int tower_level;
		int monster_id;
		int need_douhun;
		int return_douhun;
	};

	int tower_type;
	ConfigItem cfg_list[BUILD_TOWER_MAX_LEVEL + 1];
};

struct BuildTowerFbTowerPosCfg
{
	BuildTowerFbTowerPosCfg() : cfg_count(0) {}

	int cfg_count;
	Posi pos_list[BUILD_TOWER_MAX_TOWER_POS_INDEX + 1];
};

struct BuildTowerFbMonsterPathCfg 
{
	BuildTowerFbMonsterPathCfg() : cfg_count(0) {}

	int cfg_count;
	Posi pos_list[BUILD_TOWER_MAX_PATH_POS_INDEX + 1];
};

struct BuildTowerFbMonsterFlushCfg
{
	struct ConfigItem
	{
		ConfigItem() : wave(0),  monster_id(0), monster_maxhp_gongji_times(0), flush_num(0), boss_id(0), monster_douhun(0), boss_douhun(0), boss_maxhp_gongji_times(0) {}

		int wave;
		int monster_id;
		int monster_maxhp_gongji_times;
		int flush_num;
		int boss_id;
		int monster_douhun;
		int boss_douhun;
		int boss_maxhp_gongji_times;
	};

	ConfigItem cfg_list[BUILD_TOWER_MAX_MONSTER_WAVE + 1];
};

struct BuildTowerFbMonsterExpCfg
{
	BuildTowerFbMonsterExpCfg():role_level(0), boss_exp(0), monster_exp(0) {}

	int role_level;
	long long boss_exp;
	long long monster_exp;
};
class BuildTowerFbConfig : public ConfigBase
{
public:

	BuildTowerFbConfig();
	~BuildTowerFbConfig();

	bool Init(const std::string &configname, std::string *err);

	const BuildTowerFbOtherCfg &GetBuildTowerFbOtherCfg() { return m_other_cfg; };

	const BuildTowerFbTowerCfg::ConfigItem *GetBuildTowerFbTowerCfg(int tower_type, int level);
	const Posi *GetBuildTowerFbTowerPos(int pos_index);

	const int GetBuildTowerFbMonsterPathPosCount();
	const Posi *GetBuildTowerFbMonsterPathPos(int pos_index);
	
	const BuildTowerFbMonsterFlushCfg::ConfigItem *GetBuildTowerFbMonsterFlushCfg(int wave);
	const int GetBuildTowerRewardDouHun(int monster_id);

	const int GetDropRecordType(ItemID item_id);

	void GetSweeepReward(int flush_extra_num, int *index_count, ItemConfigData reward_item_list[MonsterInitParam::MAX_DROP_ITEM_COUNT], long long *reward_exp, int role_level);

	long long GetThisMonsterExp(int monster_id, int role_level);
	const BuildTowerFbMonsterExpCfg * GetBuildTowerFbMonsterExpCfg(int role_level);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitTowerCfg(PugiXmlNode RootElement);
	int InitTowerPosCfg(PugiXmlNode RootElement);
	int InitMonsterPathCfg(PugiXmlNode RootElement);
	int InitMonsterFlushCfg(PugiXmlNode RootElement);
	int InitDropRecordCfg(PugiXmlNode RootElement);
	int InitBuildTowerFbMonsterExpCfg(PugiXmlNode RootElement);

	BuildTowerFbOtherCfg m_other_cfg;

	BuildTowerFbTowerCfg m_build_tower_cfg[BUILD_TOWER_MAX_TYPE];
	BuildTowerFbTowerPosCfg m_tower_pos_cfg;
	BuildTowerFbMonsterPathCfg m_monster_path_cfg;
	BuildTowerFbMonsterFlushCfg m_monster_flush_cfg;
	BuildTowerFbMonsterExpCfg m_monster_exp_cfg_list[MAX_ROLE_LEVEL + 1];

	typedef std::map<ItemID, int> BuildTowerDropRecordCfgMap;
	typedef BuildTowerDropRecordCfgMap::iterator BuildTowerDropRecordCfgMapIt;
	BuildTowerDropRecordCfgMap m_drop_record_cfg_map;
};

#endif // __BUILD_TOWER_FB_CONFIG_HPP__

