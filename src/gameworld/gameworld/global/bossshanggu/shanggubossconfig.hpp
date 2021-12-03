#ifndef _SHANGGUBOSS_CONFIG_HPP_
#define _SHANGGUBOSS_CONFIG_HPP_

#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>

static const int SHANGGU_BOSS_CFG_MAX_LAYER = 6;					// 上古BOSS - 最大层数
static const int SHANGGU_BOSS_HIDE_BOSS_MAX_LEVEL_NUM = 4;			// 隐藏BOSS最多的等级段
static const int SHANGGU_BOSS_MAX_ENTER_TIMES = 30;					// 上古BOSS - 最大进入次数

enum SHANGGU_BOSS_TYPE
{
	SHANGGU_BOSS_TYPE_BOSS = 0,		// BOSS
	SHANGGU_BOSS_TYPE_GOLD_MONSTER,	// 黄金怪

	SHANGGU_BOSS_TYPE_MAX,
};

struct ShangGuBossOtherCfg
{
	ShangGuBossOtherCfg(): shanggu_open_level(0), shanggu_day_free_times(0), shanggu_day_max_tire(0), shanggu_kill_boss_add_tire(0), 
		shanggu_tiky_item_id(0), shanggu_angry_add_interval_s(0), shanggu_angry_kill_monster_add(0), shanggu_angry_min_box_add(0),
		shanggu_min_box_id(0), shanggu_gather_min_box_time_s(0), shanggu_angry_max_box_add(0), shanggu_max_box_id(0),
		shanggu_gather_max_box_time_s(0), shanggu_angry_die_add(0), shanggu_angry_kick_need_tims_s(0), can_use_bind_gold(false),
		is_open(false){}
	int shanggu_open_level;
	int shanggu_day_free_times;
	int shanggu_day_max_tire;
	int shanggu_kill_boss_add_tire;
	int shanggu_tiky_item_id;
	int shanggu_angry_add_interval_s;
	int shanggu_angry_kill_monster_add;
	int shanggu_angry_min_box_add;
	int shanggu_min_box_id;
	int shanggu_gather_min_box_time_s;
	int shanggu_angry_max_box_add;
	int shanggu_max_box_id;
	int shanggu_gather_max_box_time_s;
	int shanggu_angry_die_add;
	int shanggu_angry_kick_need_tims_s;
	ItemConfigData reward_item_min;
	ItemConfigData reward_item_max;
	bool can_use_bind_gold;
	bool is_open;
};

struct ShangGuBossLayerConfig
{
	ShangGuBossLayerConfig() : layer(0), scene_id(0), pos(0, 0), need_role_level(0), min_box_num(0), max_box_num(0) {}

	int layer;
	int scene_id;
	Posi pos;
	int need_role_level;
	int min_box_num;
	int max_box_num;
};

struct ShangGuBossFlushConfig
{
	ShangGuBossFlushConfig() : seq(0), time(0) {}

	int seq;
	int time;
};

struct ShangGuBossInfoConfig
{
	static const int MAX_BOSS_COUNT = 50;	// 一层最多的BOSS个数(不可超过64)

	struct ItemConfig
	{
		ItemConfig() : index(0), type(-1), boss_id(0), kill_add_angry(0), pos(0, 0), min_delta_level(0), max_delta_level(0), boss_lv_seq(0) {}

		int index;	// 代码里加的，方便存储数据库定位，配置里没有此字段
		int type;
		int boss_id;
		int kill_add_angry;
		Posi pos;
		int min_delta_level;	// 低于boss等级N级以后无法获得收益
		int max_delta_level;	// 超过boss等级N级以后无法获得收益
		int boss_lv_seq;
	};

	ShangGuBossInfoConfig() : layer(0), scene_id(0), need_role_level(0), boss_item_count(0) {}

	int layer;
	int scene_id;
	int need_role_level;

	int boss_item_count;
	ItemConfig boss_item_list[MAX_BOSS_COUNT];
};

struct ShangGuBossHideBossConfig
{
	static const int POSI_MAX_COUNT = 8;
	static const int LEVEL_MAX_MONSTER_COUNT = 6;

	struct LevelConfig
	{
		LevelConfig() : seq(0), monster_id(0), kill_add_angry(0)
		{
			for (int i = 0; i < POSI_MAX_COUNT; ++i)
			{
				pos_list[i].x = 0;
				pos_list[i].y = 0;
			}
		}

		int seq;
		int monster_id;
		int kill_add_angry;
		Posi pos_list[POSI_MAX_COUNT];
	};


	ShangGuBossHideBossConfig() : boss_lv_seq(0), monster_count(0) {}

	int boss_lv_seq;
	int monster_count;
	LevelConfig monster_list[LEVEL_MAX_MONSTER_COUNT];
};


class ShangGuBossConfig : public ConfigBase
{
public:
	ShangGuBossConfig();
	~ShangGuBossConfig();

	bool Init(const std::string &configname, std::string *err);

	const ShangGuBossLayerConfig * GetShangGuBossLayerCfgBySceneId(int scene_id);
	const ShangGuBossLayerConfig * GetShangGuBossLayerCfg(int layer);
	const ShangGuBossHideBossConfig::LevelConfig * GetShangGuBossHideBossCfg(int boss_lv_seq, int seq);
	const ShangGuBossHideBossConfig::LevelConfig * GetShangGuBossHideBossCfgByMonsterId(int monster_id);
	const ShangGuBossInfoConfig::ItemConfig * GetShangGuBossInfo(int layer, int index);
	const ShangGuBossInfoConfig::ItemConfig * GetShangGuBossInfoCfg(int layer, int boss_id);
	const ShangGuBossInfoConfig::ItemConfig * GetShangGuBossInfoCfgByBossId(int boss_id);
	const ShangGuBossInfoConfig *GetShangGuBossCfgByBossId(int boss_id);
	const ShangGuBossInfoConfig::ItemConfig * GetShangGuBossInfoWithSceneID(int scene_id, int boss_id) const;

	
	const ShangGuBossOtherCfg& GetShangGuBossOtherCfg() { return m_other_cfg; }
	int GetShangGuBossLayerCount() { return m_shanggu_boss_layer_count; }
	int GetShangGuBossCount(int layer);
	int GetShangGuBossHideBossCount(int boss_lv_seq);
	int GetShangGuBossNextFlushTimeInterval(unsigned int now_second);	// 计算当前时间与下次刷的间隔
	unsigned int GetShangGuBossGatherItemNextFlushTime(unsigned int now_second);
	int GetShangGuBossLvSeqCount(int layer, int boss_lv_seq);
	int GetShangGuBossEnterConsumeStuffNum(short enter_times);
	bool IsShangguBossMonster(int boss_id);
	bool IsShangGuBossOpen() { return m_other_cfg.is_open; }

private:
	int InitShangGuBossOtherCfg(PugiXmlNode RootElement);
	int InitShangGuBossLayerCfg(PugiXmlNode RootElement);
	int InitShangGuBossFlushCfg(PugiXmlNode RootElement);
	int InitShangGuBossInfoCfg(PugiXmlNode RootElement);
	int InitShangGuBossHideCfg(PugiXmlNode RootElement);
	int InitShangGuBossConsumeCfg(PugiXmlNode RootElement);

	ShangGuBossOtherCfg m_other_cfg;

	ShangGuBossLayerConfig m_shanggu_boss_layer_cfg_list[SHANGGU_BOSS_CFG_MAX_LAYER];

	std::vector<ShangGuBossFlushConfig> m_shanggu_boss_flush_cfg_vec;

	ShangGuBossInfoConfig m_shanggu_boss_info_cfg_list[SHANGGU_BOSS_CFG_MAX_LAYER];

	ShangGuBossHideBossConfig m_shanggu_boss_hide_boss_cfg_list[SHANGGU_BOSS_HIDE_BOSS_MAX_LEVEL_NUM + 1];

	int m_shanggu_boss_layer_count;
	int m_shanggu_boss_max_enter_times;
	int m_shanggu_boss_enter_times_need_consume_item_num_list[SHANGGU_BOSS_MAX_ENTER_TIMES + 1];
};

#endif
