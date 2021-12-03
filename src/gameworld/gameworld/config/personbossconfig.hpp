#ifndef __PERSONBOSS_CONFIG_HPP__
#define __PERSONBOSS_CONFIG_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "servercommon/fbdef.hpp"

#include <vector>
struct PersonBossOtherCfg
{
	PersonBossOtherCfg() : limit_level(0), buy_times_gold(0) {}
	int limit_level;
	int buy_times_gold;
};

struct PersonBossCfgItem
{
	PersonBossCfgItem() : layer(0), need_level(0), need_item_id(0), need_item_num(0), enter_pos(0, 0),
		scene_id(0), flush_pos(0, 0), day_enter_times(0)
	{
		memset(boss_id, 0, sizeof(boss_id));
	}

	int layer;
	int need_level;
	ItemID need_item_id;
	int need_item_num;
	Posi enter_pos;
	int scene_id;
	int boss_id[PROF_TYPE_PROF_NO_LIMIT];
	Posi flush_pos;
	int day_enter_times;
};

struct PersonBossBuyTimesCfg
{
	PersonBossBuyTimesCfg() : times(0), need_gold(0){}
	int times;
	int need_gold;
};


class PersonBossConfig : public ConfigBase
{
public:
	PersonBossConfig();
	~PersonBossConfig();

	bool Init(const std::string &configname, std::string *err);

	const PersonBossOtherCfg & GetOtherCfg() { return m_other_cfg; }
	const PersonBossCfgItem * GetBossSceneCfg(int layer);
	const PersonBossCfgItem * GetBossSceneCfgBySceneId(int scene_id);
	bool IsPersonBoss(int boss_id);
	int GetBuyCostByTimes(int times);

protected:

	int InitOtherCfg(PugiXmlNode RootElement);
	int InitBossSceneCfg(PugiXmlNode RootElement);
	int InitBossBuyTimesCfg(PugiXmlNode RootElement);

	PersonBossOtherCfg m_other_cfg;

	int m_max_boss_layer;
	PersonBossCfgItem m_personal_boss_list[MAX_PERSON_BOSS_LAYER + 1];
	std::vector<PersonBossBuyTimesCfg> m_personboss_buy_times_vec;
};

#endif // __PERSONBOSS_CONFIG_HPP__



