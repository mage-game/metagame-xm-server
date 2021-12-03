#ifndef _TOWERDEFENDCONFIG_HPP_
#define _TOWERDEFENDCONFIG_HPP_

#include "gamedef.h"
#include "servercommon/servercommon.h"
#include "towerdefendteamskill.hpp"
#include "servercommon/fbdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <map>
#include <set>

static const int TOWERDEFEND_PATH_POINT_COUNT = 4;					// 路线上的拐点数量
static const int TOWERDEFEND_HOME_REALIVE_LIMIT_INTERVAL = 0;		// 免费复活限制时间间隔 毫秒
static const int TOWERDEFEND_GOLD_REALIVE_LIMIT_INTERVAL = 0;		// 元宝复活限制时间间隔 毫秒
static const int PERSONAL_TOWERDEFEND_MAX_WAVE_PER_LEVEL = 10;		// 单人塔防每关最大波数
static const int TOWERDEFEND_WARNING_PERCENT_COUNT = 2;				// 生命塔报警的血量百分比阀值
static const int PERSONAL_TOWERDEFEND_SKILL_MAX_COUNT = 2;			// 单人塔防最大技能数
static const int TOWERDEFEND_STAR_LEVEL_COUNT = 3;					// 通关最大星级数
static const int TOWERDEFEND_BUFF_DROP_WEIGHT_COUNT = 5;			// BUFF掉落权重最大数量
static const int TOWERDEFEND_NEW_PLAYER_FIRST_LEVEL = 0;			// 新手

enum TOWERDEFEND_PATH												// 刷怪路线
{
	TOWERDEFEND_PATH_INVALID = -1,
	TOWERDEFEND_PATH_LEFT,											// 左
	TOWERDEFEND_PATH_MID,											// 中
	TOWERDEFEND_PATH_RIGHT,											// 右

	TOWERDEFEND_PATH_MAX,
};

//const static int TEAM_TOWERDEFEND_PATH_MAX = TOWERDEFEND_PATH_MAX;

enum TOWERDEFEND_WARNING_PERCENT
{
	TOWERDEFEND_WARNING_PERCENT_50 = 50,
	TOWERDEFEND_WARNING_PERCENT_10 = 10,
};

struct TowerDefendOtherCfg
{
	TowerDefendOtherCfg() : free_join_times(0), auto_fb_empty_grid(0), auto_cost(0), first_wave_refresh_delay(0),
		wave_refresh_interval(0), life_tower_monster_id(0), warning_interval_s(0), sync_interval_s(0), buff_exit_time(0), add_role_gongji_per(0),
		relive_back_home_interval(0), relive_gold_interval(0),life_tower_monster_id_test(0), refresh_interval_test(0)
	{
		memset(first_reward, 0, sizeof(first_reward));
	}

	int free_join_times;
	int auto_fb_empty_grid;
	int auto_cost;

	int first_wave_refresh_delay;
	int wave_refresh_interval;
	int life_tower_monster_id;
	int warning_interval_s;
	int sync_interval_s;
	int buff_exit_time;
	int add_hp_per;
	int add_gongji_per;
	int add_role_gongji_per;
	int relive_back_home_interval;
	int relive_gold_interval;
	ItemConfigData first_reward[MAX_ATTACHMENT_ITEM_NUM];
	int life_tower_monster_id_test;
	int refresh_interval_test;
};

struct TowerDefendWaveCfg
{
	TowerDefendWaveCfg() : wave(0), next_wave_s(0), total_monster_count(0)
	{
		memset(monster_id_list, 0, sizeof(monster_id_list));
		memset(monster_count_list, 0, sizeof(monster_count_list));
	}

	int wave;
	int next_wave_s;													// 这么多秒后到来下一波
	int monster_id_list[TOWERDEFEND_PATH_MAX];							// 每条路线出的怪ID
	int monster_count_list[TOWERDEFEND_PATH_MAX];						// 每条路线出的怪数量
	int total_monster_count;											// 怪总量
};

struct TowerDefendLevelCfg
{
	static const int CONFIG_ITEM_MAX_COUNT = 25;
	UNSTD_STATIC_CHECK(CONFIG_ITEM_MAX_COUNT < 128);

	struct ConfigItem
	{
		ConfigItem() : level(0), role_level(0), scene_id(0), birth_pos(0, 0), capability(0), wave_count(0)
		{

		}

		int level;
		int role_level;
		int scene_id;
		Posi birth_pos;
		int capability;

		int wave_count;
		TowerDefendWaveCfg wave_list[PERSONAL_TOWERDEFEND_MAX_WAVE_PER_LEVEL];

		ItemConfigData find_reward[MAX_ATTACHMENT_ITEM_NUM];
	};

	TowerDefendLevelCfg() : cfg_count(0)
	{
	}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct TowerDefendBuyJoinTimesCfg
{
	const static int CONFIG_ITEM_MAX_COUNT = 16;

	struct ConfigItem
	{
		ConfigItem() : buy_times(0), gold_cost(0) {}

		int buy_times;
		int gold_cost;
	};

	TowerDefendBuyJoinTimesCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct TowerDefendStarLevelCfg
{
	TowerDefendStarLevelCfg():death_count(0), star_level(0)
	{}

	short death_count;
	short star_level;
};

struct TowerDefendBuffDropWeightCfg
{
	TowerDefendBuffDropWeightCfg(): monster_id(0), total_weight(0)
	{
		memset(weight, 0, sizeof(weight));
	}

	int monster_id;
	int weight[TOWERDEFEND_BUFF_DROP_WEIGHT_COUNT];
	int total_weight;
};

enum TOWER_DEFEND_BUff_TYPE
{
	DEFEND_BUff_TYPE_INVALID = 0,

	DEFEND_BUff_TYPE_RECOVER,		// 回复雕像血量
	DEFEND_BUff_TYPE_ADD_GONGJI,	// 提升自身攻击
	DEFEND_BUff_TYPE_KILL_MONSTER,	// 秒杀已出现的怪物
	DEFEND_BUff_TYPE_ADD_HUDUN,		// 给雕像加护盾

	DEFEND_BUff_TYPE_MAX,
};

struct TowerDefendBuffCfg
{
	TowerDefendBuffCfg() : buff_type(0)
	{
		memset(param, 0, sizeof(param));
	}

	int buff_type;
	int param[4];
};

class TowerDefendConfig : public ConfigBase
{
public:
	TowerDefendConfig();
	~TowerDefendConfig();

	bool Init(const std::string &configname, std::string *err);

	int GetRefreshStartDelay() const { return m_other_cfg.first_wave_refresh_delay; }
	int GetRefreshInterval() const { return m_other_cfg.wave_refresh_interval; }
	int GetLifeTowerMonsterID() const { return m_other_cfg.life_tower_monster_id; }
	int GetLifeTowerMonsterIDTest() const { return m_other_cfg.life_tower_monster_id_test; }
	int GetWarningInterval() const { return m_other_cfg.warning_interval_s; }
	int GetSyncInterval() const { return m_other_cfg.sync_interval_s; }
	int GetBuffDropExitTime() const { return m_other_cfg.buff_exit_time; }
	void GetFirstReward(ItemConfigData* first_reward,int num)const; 
	int GetWaveCountBySceneID(int scene_id) const;
	int GetLevelBySceneID(int scene_id) const;
	const TowerDefendWaveCfg * GetWaveCfg(int scene_id, int wave) const;
	const TowerDefendWaveCfg * GetWaveCfgByMonsterID(int scene_id, int monster_id) const;

	const TowerDefendWaveCfg * GetWaveTestCfgRand() const;
	const TowerDefendWaveCfg * GetWaveTestCfgByWave(int wave) const;

	Posi GetBirthPos(int path) const;
	Posi GetBirthPosTest(int path) const;
	bool GetNextPosi(int birth_path, const Posi &curr_pos, Posi *pos);
	bool GetNextPosiTest(int birth_path, const Posi &curr_pos, Posi *pos);

	void GetPathWayList(int path, std::vector<Posi> & walk_pos);
	void GetPathWayListTest(int path, std::vector<Posi> & walk_pos);

	bool CheckWarning(int old_percent, int now_percent, int *warning_percent);

	const TowerDefendLevelCfg::ConfigItem * GetLevelCfgBySceneID(int scene_id) const;
	const TowerDefendLevelCfg::ConfigItem * GetLevelCfg(int level) const;
	const TowerDefendOtherCfg & GetOtherCfg() { return m_other_cfg; }
	int GetBuyJoinTimesCfg(int buy_times);
	short GetStarByDeathCount(short death_count);
	short GetPassStar() { return m_pass_star_cfg[0].star_level; }

	TowerDefendBuffDropWeightCfg * GetBuffDropCfg(int monster_id);
	TowerDefendBuffCfg * GetBufffCfg(int buff_type);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitLevelCfg(PugiXmlNode RootElement);
	int InitWaveCfg(PugiXmlNode RootElement);
	int InitPathCfg(PugiXmlNode RootElement);
	int InitBuyJoinTimesCfg(PugiXmlNode RootElement);
	int InitStarLevelCfg(PugiXmlNode RootElement);
	int InitBuffDropWeightCfg(PugiXmlNode RootElement);
	int InitBuffCfg(PugiXmlNode RootElement);

	int InitWaveTestCfg(PugiXmlNode RootElement);
	int InitPathTestCfg(PugiXmlNode RootElement);

	bool CheckLevelMonsterID(const TowerDefendLevelCfg::ConfigItem &cfg_item);   // 场景中移除obj的时候根据场景id和怪物id确认的波数，所以每一波的Montser需要不一样

	TowerDefendOtherCfg m_other_cfg;									// 其他配置
	TowerDefendLevelCfg m_level_cfg;									// 关卡配置
	TowerDefendBuyJoinTimesCfg m_buy_join_times_cfg;					// 购买参与次数配置

	Posi wave_pathway_list[TOWERDEFEND_PATH_MAX][TOWERDEFEND_PATH_POINT_COUNT];

	int m_warning_percent_list[TOWERDEFEND_WARNING_PERCENT_COUNT];		// 生命塔报警的血量百分比列表

	TowerDefendStarLevelCfg m_pass_star_cfg[TOWERDEFEND_STAR_LEVEL_COUNT];

	std::map<int, TowerDefendBuffDropWeightCfg> m_buff_drop_weight_map;		// buff掉落配置
	std::map<int, TowerDefendBuffCfg> m_buff_cfg_map;		// buff配置

	Posi m_wave_pathway_list_test[TOWERDEFEND_PATH_MAX][TOWERDEFEND_PATH_POINT_COUNT];
	std::vector<TowerDefendWaveCfg> m_wave_cfg_test;
};

#endif
