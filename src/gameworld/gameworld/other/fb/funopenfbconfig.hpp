#ifndef __FUN_OPEN_FB_CONFIG__
#define __FUN_OPEN_FB_CONFIG__

#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/maildef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <map>

enum FUN_OPEN_FB_TYPE
{
	INVALID_FB_TYPE = 0,

	FUN_OPEN_FB_WING,			// ÓðÒí
	FUN_OPEN_FB_MOUNT,			// ×øÆï
	FUN_OPEN_FB_XIANNV,			// Å®Éñ

	FUN_OPEN_FB_MAX,
};

enum FUN_OPEN_FB_CREATE_MONSTER_STEP
{
	INVALID_STEP = 0,

	CREATE_MONSTER_STEP_ONE,
	CREATE_MONSTER_STEP_TWO,
	CREATE_MONSTER_STEP_THREE,
	CREATE_MONSTER_STEP_FOUR,

	CREATE_MONSTER_STEP_MAX,
};

struct FunOpenOtherCfg
{
	FunOpenOtherCfg():shuijing_monster_id(0), shuijing_monster_num(0), shuijing_monster_interval(0), shuijing_monster_born_pos(0,0), shuijing_monster_born_pos_rand(0),wing_fb_boss_id(0),
	mount_gather_id(0){}

	int shuijing_monster_id;
	int shuijing_monster_num;
	unsigned int shuijing_monster_interval;
	Posi shuijing_monster_born_pos;
	int shuijing_monster_born_pos_rand;
	int wing_fb_boss_id;

	int mount_gather_id;
};

struct FunOpenSceneCfg
{
	FunOpenSceneCfg(): fb_type(0), task_id(0), scene_id(0), enter_pos(0,0){}

	int fb_type;
	int task_id;
	int scene_id;
	Posi enter_pos;
};

struct FunOpenMonsterCfg
{
	FunOpenMonsterCfg():monster_index(0), fb_type(0), step(0), monster_id(0), bron_pos(0,0){}

	int monster_index;
	int fb_type;
	int step;
	int monster_id;
	Posi bron_pos;
};

struct FunOpenGatherCfg
{
	FunOpenGatherCfg():gather_index(0), fb_type(0), step(0), gather_id(0), gather_time(0), bron_pos(0,0){}
	
	int gather_index;
	int fb_type;
	int step;
	int gather_id;
	unsigned int gather_time;
	Posi bron_pos;

};

class FunOpenFBConfig : public ConfigBase
{
public:
	FunOpenFBConfig();
	~FunOpenFBConfig();

	bool Init(const std::string &configname, std::string *err);

	const FunOpenOtherCfg & GetOtherCfg(){return m_other_cg;}
	const FunOpenSceneCfg * GetFunOpenSceneCfg(int fb_type);
	std::map<int, FunOpenMonsterCfg> & GetFunOpenMonsterCfg(){return m_monster_cfg_map;}
	std::map<int, FunOpenGatherCfg> & GetFunOpenGatherCfgMap(){return m_gather_cfg_map;}
	const FunOpenGatherCfg * GetFunOpenGatherCfg(int fb_type, int gather_id);

private:

	int InitOtherCfg(PugiXmlNode RootElement);
	int InitSceneCfg(PugiXmlNode RootElement);
	int InitMonsterCfg(PugiXmlNode RootElement);
	int InitGatherCfg(PugiXmlNode RootElement);

	FunOpenOtherCfg m_other_cg;
	std::map<int, FunOpenSceneCfg> m_scene_cfg_map;
	std::map<int, FunOpenMonsterCfg> m_monster_cfg_map;
	std::map<int, FunOpenGatherCfg> m_gather_cfg_map;
};

#endif
