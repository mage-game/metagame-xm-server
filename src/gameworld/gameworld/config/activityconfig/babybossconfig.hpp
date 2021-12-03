#ifndef _BABY_BOSS_CONFIG_HPP_
#define _BABY_BOSS_CONFIG_HPP_

#include "servercommon/configbase.h"
#include "gamedef.h"
#include <vector>
#include <map>
#include <set>

class TiXmlElement;

struct BabyBossOtherCfg
{
	BabyBossOtherCfg() : angry_value_add_per_minute(0), angry_value_limit(0), delay_kick_out_time(0), logout_kick_out_time(0), need_item_id(0), angry_value_die(0)
	{}

	int angry_value_add_per_minute;
	int angry_value_limit;
	unsigned int delay_kick_out_time;
	unsigned int logout_kick_out_time;
	short need_item_id;
	int angry_value_die;
};

struct BabyBossEnterCostCfg 
{
	BabyBossEnterCostCfg() : need_item_num(0)
	{}

	int need_item_num;
};

struct BabyBossCfg
{
	BabyBossCfg() : boss_id(0), scene_id(0), is_boss(0), refresh_interval(0), born_pos(0, 0), enter_pos(0, 0) {}

	int boss_id;
	int scene_id;
	int is_boss;
	int refresh_interval;
	Posi born_pos;
	Posi enter_pos;
};

struct BabyBossSceneCfg 
{
	BabyBossSceneCfg() : scene_id(0), pos(0, 0)
	{}

	int scene_id;
	Posi pos;
	std::vector<BabyBossCfg> baby_boss;
};

struct BabyBossFlushConfig
{
	BabyBossFlushConfig() : seq(0), time(0) {}

	int seq;
	int time;
};

class BabyBossConfig : public ConfigBase
{
public:
	BabyBossConfig();
	~BabyBossConfig();

	bool Init(const std::string &configname, std::string *err);

	const BabyBossOtherCfg & GetOtherCfg() { return m_other_cfg; }
	int GetKillAngryValue(unsigned short monster_id);
	const BabyBossEnterCostCfg * GetEnterCostCfg(int enter_times);
	bool NeedLog(ItemID item_id);
	const BabyBossCfg * GetBabyBossCfgBySceneId(int scene_id);
	const BabyBossCfg * GetBabyBossCfgByBossId(int boss_id);
	const std::map<int, BabyBossCfg> & GetBabyBossCfgMap() { return m_baby_boss_cfg; }
	bool IsBabyBoss(int scene_id, int scene_type, unsigned short monster_id);
	bool IsBabyBoss(unsigned short monster_id);
	int GetBabyBossNextFlushTimeInterval(unsigned int now_second);	// 计算当前时间与下次刷的间隔

private:
	int InitOtherCfg(TiXmlElement *root_element);
	int InitKillAngryValueCfg(TiXmlElement *root_element);
	int InitEnterCostCfg(TiXmlElement *root_element);
	int InitNeedLogItemConfig(TiXmlElement *root_element);
	int InitSceneCfg(TiXmlElement *root_element);
	int InitBabyBossFlushCfg(TiXmlElement *root_element);

	BabyBossOtherCfg m_other_cfg;
	std::map<unsigned short, int> m_kill_angry_value_cfg_map;
	std::vector<BabyBossEnterCostCfg> m_enter_cost_cfg_vec;
	std::set<ItemID> m_need_log_item_set;
	std::map<int, BabyBossCfg> m_baby_boss_cfg;
	std::vector<BabyBossFlushConfig> m_baby_boss_flush_cfg_vec;
};

#endif // _BABY_BOSS_CONFIG_HPP_



