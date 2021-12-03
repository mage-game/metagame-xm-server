#ifndef _MONSTERINVADECONFIG_HPP_
#define _MONSTERINVADECONFIG_HPP_

#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gamedef.h"

#include <vector>

struct PosItem
{
	PosItem() : scene_id(0) {}

	int scene_id;
	Posi born_pos;
};

class MonsterInvadeConfig : public ConfigBase
{
public:
	MonsterInvadeConfig();
	~MonsterInvadeConfig();

	bool Init(const std::string &configname, std::string *err);

	void RefreshAll();
	int GetRefreshIntervalS() { return m_refresh_interval_s; }

private:
	int InitPosCfg(PugiXmlNode RootElement);
	int InitMonsterCfg(PugiXmlNode RootElement);
	int InitOtherCfg(PugiXmlNode RootElement);

	int RandMonster();

	void CalcMonsterAddAttrs(int m_monster_id, int *add_maxhp, int *add_gongji, int *add_fangyu);

	int m_refresh_interval_s;

	static const int MAX_SCENE_COUNT = 4;
	static const int MAX_MONSTER_COUNT = 8;

	std::vector<PosItem> m_pos_vec;

	int m_monster_count;
	int m_monster_list[MAX_MONSTER_COUNT];
};

#endif

