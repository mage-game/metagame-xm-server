#ifndef _WORLDBOSSGAMECONFIG_HPP_
#define _WORLDBOSSGAMECONFIG_HPP_

#include "servercommon/servercommon.h"
#include "gamedef.h"
#include <map>

struct WorldBossRefreshPos
{
	WorldBossRefreshPos() : scene_id(0), pos_x(0), pos_y(0) {}

	int scene_id;
	int pos_x;
	int pos_y;
};

struct JIngyingBossRefreshPos
{
	static const int MAX_JINGYING_REFRESH_POS = 5;						// 精英boss出生点数量

	JIngyingBossRefreshPos() : scene_id(0)
	{
		memset(born_x_list, 0, sizeof(born_x_list)); memset(born_y_list, 0, sizeof(born_y_list));
	}

	Posi RandPos() const
	{
		Posi pos(0, 0);

		int rand_idx = RandomNum(MAX_JINGYING_REFRESH_POS);
		if (rand_idx >= 0 && rand_idx < MAX_JINGYING_REFRESH_POS)
		{
			return Posi(born_x_list[rand_idx], born_y_list[rand_idx]);
		}

		return pos;
	}

	int scene_id;
	int born_x_list[MAX_JINGYING_REFRESH_POS];
	int born_y_list[MAX_JINGYING_REFRESH_POS];
};

class TiXmlElement;

class WorldBossGameConfig
{
public:
	WorldBossGameConfig();
	~WorldBossGameConfig();

	bool Init(const std::string &configname, std::string *err);

	const WorldBossRefreshPos * GetWorldBossRefreshPos(int boss_id);
	const JIngyingBossRefreshPos * GetJingyingBossRefreshPos(int boss_id);

private:
	int InitWorldBossCfg(TiXmlElement *RootElement);
	int InitJingyingBossCfg(TiXmlElement *RootElement);

	std::map<int, WorldBossRefreshPos> m_worldboss_pos_map;
	std::map<int, JIngyingBossRefreshPos> m_jingying_pos_map;
};

#endif
