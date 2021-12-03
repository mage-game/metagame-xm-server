#ifndef __DAILY_TASK_FB_CONFIG__
#define __DAILY_TASK_FB_CONFIG__

#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <map>

enum DAILY_TASK_FB_TYPE
{
	DAILY_TASK_FB_TYPE_INVALID = 0,

	DAILY_TASK_FB_TYPE_1,
	DAILY_TASK_FB_TYPE_2,
	DAILY_TASK_FB_TYPE_3,
	DAILY_TASK_FB_TYPE_4,
	DAILY_TASK_FB_TYPE_5,

	DAILY_TASK_FB_TYPE_MAX,
};

struct DailyTaskFbKey
{
	DailyTaskFbKey(): scene_id(0), min_level(0), max_level(0){}

	int scene_id;
	int min_level;
	int max_level;

	bool operator < (const DailyTaskFbKey &v) const 
	{
		return (scene_id < v.scene_id || (scene_id == v.scene_id && min_level < v.min_level));
	}
};

struct DailyFBCfg
{
	DailyFBCfg():scene_id(0), enter_pos(0, 0), branch_fb_type(0), monster_1(0), monster_2(0), monster_3(0), boss_monster(0),
		param_1(0), param_2(0), param_3(0), finish_param(0), min_level(0), max_level(0)
	{}

	int scene_id;
	Posi enter_pos;
	int branch_fb_type;
	int monster_1;
	int monster_2;
	int monster_3;
	int boss_monster;
	int param_1;
	int param_2;
	int param_3;
	int finish_param;
	int min_level;
	int max_level;
};

struct DailyTaskFBMonsterCfg
{
	DailyTaskFBMonsterCfg(): scene_id(0), index(0), min_level(0), max_level(0), monster_id(0), create_pos(0, 0),
	add_mianshang(0), mianshang_only_boss(0), mianshang_per(0), add_recover(0), recover_only_boss(0),
	recover_by_attack(0), recover_per(0), recover_interval(0){}
	
	int scene_id;
	int index;
	int min_level;
	int max_level;
	int monster_id;
	Posi create_pos;
	short add_mianshang;
	short mianshang_only_boss;
	int mianshang_per;
	short add_recover;
	short recover_only_boss;
	short recover_by_attack;
	int recover_per;
	unsigned int recover_interval;
};

class DailyTaskFBConfig : public ConfigBase
{
public:
	DailyTaskFBConfig();
	~DailyTaskFBConfig();

	bool Init(const std::string &configname, std::string *err);

	DailyFBCfg * GetDailyTaskFBConfig(int scene_id, int level);
	std::map<int, std::map<int, DailyTaskFBMonsterCfg> > & GetDailyTaskFBMonsterMap() { return m_monster_cfg_map;}

private:

	int InitDailyTaskFbCfg(PugiXmlNode RootElement);
	int InitDailyTaskFBMonsterCfg(PugiXmlNode RootElement);

	std::map<DailyTaskFbKey, DailyFBCfg> m_daily_task_fb_cfg_map;

	std::map<int, std::map<int, DailyTaskFBMonsterCfg> > m_monster_cfg_map;
};

#endif
