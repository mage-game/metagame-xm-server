#ifndef __MOSHENJIANGLIN_CONFIG_HPP__
#define __MOSHENJIANGLIN_CONFIG_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

static const int MAX_DAY_IN_WEEK = 7;
static const int WORLD_LEVEL_BOSS_MAX = 6;

struct MoShenJiangLinBossID
{
	MoShenJiangLinBossID() : world_lv(0), boss_id(0) {}

	int world_lv;
	int boss_id;
};

struct MoShenJiangLinBossCfg
{
	MoShenJiangLinBossCfg() 
		: tm_wday(0), scene_id(0), pos(0, 0), 
		base_exp(0), base_xianhun(0), base_yuanli(0), base_tongbi(0)
	{
		memset(boss_idlist, 0, sizeof(boss_idlist));
	}

	int GetBossIDByWorldLv(int world_lv)
	{
		for (int i = WORLD_LEVEL_BOSS_MAX - 1; i >= 0; i--)
		{
			if (world_lv >= boss_idlist[i].world_lv)
			{
				return boss_idlist[i].boss_id;
			}
		}

		return boss_idlist[0].boss_id;
	}

	int tm_wday;

	MoShenJiangLinBossID boss_idlist[WORLD_LEVEL_BOSS_MAX];

	int scene_id;
	Posi pos;

	int base_exp;
	int base_xianhun;
	int base_yuanli;
	int base_tongbi;
};

class MoShenJiangLinConfig : public ConfigBase
{
public:
	MoShenJiangLinConfig();
	~MoShenJiangLinConfig();

	bool Init(const std::string &path, std::string *err);

	MoShenJiangLinBossCfg * GetMoShenJiangLinBossCfg(int tm_wday);

private:
	MoShenJiangLinBossCfg m_boss_list[MAX_DAY_IN_WEEK];
};

#endif // __MOSHENJIANGLIN_CONFIG_HPP__

