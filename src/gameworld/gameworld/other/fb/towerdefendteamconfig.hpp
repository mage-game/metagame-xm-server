#ifndef _TOWERDEFENDTEAMCONFIG_HPP_
#define _TOWERDEFENDTEAMCONFIG_HPP_

#include "gamedef.h"
#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "towerdefendteamskill.hpp"
#include "servercommon/fbdef.hpp"
#include <map>
#include <set>

#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "monster/drop/droppool.hpp"

static const int TEAM_TOWERDEFEND_PATH_POINT_COUNT = 4;				// 组队副本路径拐点数量
static const int TEAM_TOWERDEFEND_NORMAL_SKILL_COUNT = 4;			// 默认技能数量		
static const int TEAM_TOWERDEFEND_EXTRA_SKILL_COUNT = 0;			// 额外技能数量
static const int TEAM_TOWERDEFEND_FROZEN_MAX_COUNT = 0;				// 最大冰冻次数
static const int TEAN_TOWERDEFEND_WAVE_COUNT_PER_LEVEL = 10;		// 组队塔防每关最大波数
static const int TEAM_TOWERDEFEND_WAVE_COUNT = 80;					// 最大波数
static const int TEAM_TOWERDEFEND_HOME_REALIVE_LIMIT_INTERVAL = 0;	// 免费复活限制时间间隔 毫秒
static const int TEAM_TOWERDEFEND_GOLD_REALIVE_LIMIT_INTERVAL = 0;	// 元宝复活限制时间间隔 毫秒
static const int TEAM_TOWERDEFEND_MAX_SKILL_LEVEL = 30;				// 技能最大等级
static const int TEAM_TOWERDEFEND_MAX_SKILL_COUNT = 12;				// 最大技能数量
static const int TEAM_TOWERDEFEND_WARNING_PERCENT_COUNT = 2;		// 生命塔报警的血量百分比阀值

enum TEAM_TOWERDEFEND_PATH											// 刷怪路线
{
	TEAM_TOWERDEFEND_PATH_INVALID = -1,
	TEAM_TOWERDEFEND_PATH_LEFT,											// 左
	TEAM_TOWERDEFEND_PATH_MID,											// 中
	TEAM_TOWERDEFEND_PATH_RIGHT,										// 右

	TEAM_TOWERDEFEND_PATH_MAX,
};

enum TEAM_TOWERDEFEND_WARNING_PERCENT
{
	TEAM_TOWERDEFEND_WARNING_PERCENT_50 = 50,
	TEAM_TOWERDEFEND_WARNING_PERCENT_10 = 10,
};

enum TEAM_TOWERDEFEND_ATTRTYPE											// 加成类型
{
	TEAM_TOWERDEFEND_ATTRTYPE_INVALID = 0,
	TEAM_TOWERDEFEND_ATTRTYPE_GONGJI,									// 加攻 朱雀
	TEAM_TOWERDEFEND_ATTRTYPE_FANGYU,									// 加防 玄武
	TEAM_TOWERDEFEND_ATTRTYPE_ASSIST,									// 辅助 青龙

	TEAM_TOWERDEFEND_ATTRTYPE_MAX,
};

struct TeamTowerDefendWaveCfg
{
	TeamTowerDefendWaveCfg() : wave(0), next_wave_s(0), total_monster_count(0), kill_score(0),sweep_calue(0)
	{
		memset(monster_id_list, 0, sizeof(monster_id_list));
		memset(monster_count_list, 0, sizeof(monster_count_list));
	}

	int wave;
	int next_wave_s;													// 这么多秒后到来下一波
	int monster_id_list[TEAM_TOWERDEFEND_PATH_MAX];						// 每条路线出的怪ID
	int monster_count_list[TEAM_TOWERDEFEND_PATH_MAX];					// 每条路线出的怪数量
	int total_monster_count;											// 怪总量
	int kill_score;														// 击杀怪物加积分

	std::vector<UInt16> dropid_list;									// 掉落ID列表
	int sweep_calue;													// 扫荡仙气值
};

struct TeamTowerDefendLevelCfg
{
	static const int CONFIG_ITEM_MAX_COUNT = 10;
	UNSTD_STATIC_CHECK(CONFIG_ITEM_MAX_COUNT < 128);

	struct ConfigItem
	{
		ConfigItem() : level(0), role_level(0), scene_id(0), birth_pos(0, 0), wave_count(0)
		{

		}

		int level;
		int role_level;
		int scene_id;
		Posi birth_pos;

		int wave_count;
		TeamTowerDefendWaveCfg wave_list[TEAN_TOWERDEFEND_WAVE_COUNT_PER_LEVEL];
	};

	TeamTowerDefendLevelCfg() : cfg_count(0)
	{
	}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct TeamTowerDefendOtherCfg
{
	TeamTowerDefendOtherCfg() : free_join_times(0), auto_fb_empty_grid(0), auto_item_id(0), auto_item_count(0), team_enter_sceneid(0), team_enter_pos(0, 0), team_wave_count(0),
		buy_double_reward_need_gold(0), buy_triple_reward_need_gold(0), double_reward_need_vip_level(0), triple_reward_need_vip_level(0)
	{

	}

	int free_join_times;
	int auto_fb_empty_grid;
	ItemID auto_item_id;
	int auto_item_count;

	int team_enter_sceneid;
	Posi team_enter_pos;

	int team_wave_count;
	int buy_double_reward_need_gold;
	int buy_triple_reward_need_gold;
	int double_reward_need_vip_level;
	int triple_reward_need_vip_level;
	TeamTowerDefendWaveCfg team_wave_list[TEAM_TOWERDEFEND_WAVE_COUNT];

	std::vector<UInt16> dropid_list;									// 额外奖励
};

struct TeamTowerDefendSkillCfg
{
	static const int CONFIG_ITEM_MAX_COUNT = TEAM_TOWERDEFEND_MAX_SKILL_COUNT;

	struct ConfigItem
	{
		ConfigItem() : skill_index(0), attr_type(0), skill_seq(0), skill_id(0), hurt_percent(0), fix_hurt(0), cd_factor(0), cd_s(0), distance(0), energy_cost(0), param_a(0), param_b(0), param_c(0), param_d(0)
		{

		}

		bool Invalid() const { return 0 == skill_index; }

		int skill_index;
		int attr_type;
		int skill_seq;
		int skill_id;
		int hurt_percent;
		int fix_hurt;
		int cd_factor;
		int cd_s;
		int distance;
		int energy_cost;
		int param_a;
		int param_b;
		int param_c;
		int param_d;
	};

	TeamTowerDefendSkillCfg()
	{

	}

	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct TeamTowerDefendBuyJoinTimesCfg
{
	const static int CONFIG_ITEM_MAX_COUNT = 16;

	struct ConfigItem
	{
		ConfigItem() : buy_times(0), gold_cost(0) {}

		int buy_times;
		int gold_cost;
	};

	TeamTowerDefendBuyJoinTimesCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

class TeamTowerDefendConfig : public ConfigBase
{
public:
	TeamTowerDefendConfig();
	~TeamTowerDefendConfig();

	bool Init(const std::string &configname, std::string *err);

	int GetRefreshStartDelay() const { return m_wave_refresh_start_delay; }
	int GetRefreshInterval() const { return m_wave_refresh_interval; }

	int GetWaveCountBySceneID(int scene_id) const;
	const TeamTowerDefendWaveCfg * GetWaveCfg(int scene_id, int wave) const;
	const TeamTowerDefendWaveCfg * GetWaveCfgByMonsterID(int scene_id, int monster_id) const;

	int GetTeamDefendWaveCount() { return m_other_cfg.team_wave_count; }
	const TeamTowerDefendWaveCfg * GetTeamWaveCfg(int wave) const;
	const TeamTowerDefendWaveCfg * GetTeamWaveCfgByMonsterID(int monster_id) const;
	int GetMonsterAngryValue(int monster_id);
	Posi GetBirthPos(int path) const;

	int GetLifeTowerMonsterID() const { return m_life_tower_monster_id; }
	int GetTeamLifeTowerMonsterID() const { return m_team_life_tower_monster_id; }
	bool GetNextPosi(int birth_path, const Posi &curr_pos, Posi *pos);

	bool CheckWarning(int old_percent, int now_percent, int *warning_percent);

	int GetWarningInterval() const { return m_warning_interval_s; }
	int GetSyncInterval() const { return m_sync_interval_s; }

	const TeamTowerDefendLevelCfg::ConfigItem * GetLevelCfgBySceneID(int scene_id) const;
	const TeamTowerDefendLevelCfg::ConfigItem * GetLevelCfg(int level) const;
	const TeamTowerDefendOtherCfg & GetOtherCfg() { return m_other_cfg; }
	const TeamTowerDefendSkillCfg::ConfigItem * GetSkillCfg(int skill_index);
	int GetBuyJoinTimesCfg(int buy_times);

	const TeamTowerDefendSkillCfg::ConfigItem * GetTeamSkillCfg(int skill_id);
	void GetTeamSkillListByAttr(int attr_type, int skill_id_list[TEAM_TOWERDEFEND_NORMAL_SKILL_COUNT]) const;
	int GetUpTeamSkillCost(int skill_id, int skill_level) const;
	const TeamTowerDefendSkill * GetTeamSkillBase(int skill_id) const;


private:
	int InitTeamWaveCfg(PugiXmlNode RootElement);
	int InitLevelCfg(PugiXmlNode RootElement);
	int InitPathCfg(PugiXmlNode RootElement);
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitSkillCfg(PugiXmlNode RootElement);
	int InitBuyJoinTimesCfg(PugiXmlNode RootElement);
	int InitSkillLearnCfg(PugiXmlNode RootElement);

	bool CheckLevelMonsterID(const TeamTowerDefendLevelCfg::ConfigItem &cfg_item);

	int m_wave_refresh_start_delay;										// 第一波怪延迟秒
	int m_wave_refresh_interval;										// 出怪间隔

	Posi wave_pathway_list[TEAM_TOWERDEFEND_PATH_MAX][TEAM_TOWERDEFEND_PATH_POINT_COUNT];

	int m_life_tower_monster_id;										// 生命塔怪ID
	int m_team_life_tower_monster_id;									// 组队塔防 生命塔怪ID

	int m_warning_interval_s;											// 生命塔挨打的报警间隔
	int m_warning_percent_list[TEAM_TOWERDEFEND_WARNING_PERCENT_COUNT];	// 生命塔报警的血量百分比列表

	int m_sync_interval_s;												// 定时同步信息的间隔

	TeamTowerDefendLevelCfg m_level_cfg;									// 关卡配置
	TeamTowerDefendOtherCfg m_other_cfg;									// 其他配置
	TeamTowerDefendSkillCfg m_skill_cfg;									// 技能配置
	TeamTowerDefendBuyJoinTimesCfg m_buy_join_times_cfg;					// 购买参与次数配置

	int m_skill_up_cost_list[TEAM_TOWERDEFEND_MAX_SKILL_COUNT][TEAM_TOWERDEFEND_MAX_SKILL_LEVEL + 1];	// 技能升级列表
	TeamTowerDefendSkill m_team_skill_base_list[TEAM_TOWERDEFEND_MAX_SKILL_COUNT+1];

	std::map<int, int> m_monster_angry_value;								//仙气值对应表

};

#endif
