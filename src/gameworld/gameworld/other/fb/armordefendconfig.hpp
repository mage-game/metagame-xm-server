#ifndef _ARMORDEFENDCONFIG_HPP_
#define _ARMORDEFENDCONFIG_HPP_

#include "gamedef.h"
#include "servercommon/servercommon.h"
#include "towerdefendteamskill.hpp"
#include "servercommon/fbdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "servercommon/maildef.hpp"
#include <map>
#include <set>

static const int ARMORDEFEND_PATH_POINT_MAX = 20;					// 路线上的拐点数量
static const int ARMORDEFEND_HOME_REALIVE_LIMIT_INTERVAL = 0;		// 免费复活限制时间间隔 毫秒
static const int ARMORDEFEND_GOLD_REALIVE_LIMIT_INTERVAL = 0;		// 元宝复活限制时间间隔 毫秒
static const int PERSONAL_ARMORDEFEND_MAX_WAVE_PER_LEVEL = 10;		// 单人塔防每关最大波数
static const int ARMORDEFEND_WARNING_PERCENT_COUNT = 2;				// 生命塔报警的血量百分比阀值
static const int PERSONAL_ARMORDEFEND_SKILL_MAX_COUNT = 2;			// 单人塔防最大技能数

enum ARMORDEFEND_PATH												// 刷怪路线
{
	ARMORDEFEND_PATH_INVALID = -1,
	ARMORDEFEND_PATH_LEFT,											// 左
	ARMORDEFEND_PATH_MID,											// 中
	ARMORDEFEND_PATH_RIGHT,											// 右

	ARMORDEFEND_PATH_MAX,
};

enum ARMORDEFEND_WARNING_PERCENT
{
	ARMORDEFEND_WARNING_PERCENT_50 = 50,
	ARMORDEFEND_WARNING_PERCENT_10 = 10,
};

// 新手波次配置
struct NewPlayerArmorDefendWaveCfg
{
	NewPlayerArmorDefendWaveCfg() : wave(0), next_wave_s(0), total_monster_count(0), kill_score(0)
	{
		memset(monster_id_list, 0, sizeof(monster_id_list));
		memset(monster_count_list, 0, sizeof(monster_count_list));
	}

	int wave;
	int next_wave_s;													// 这么多秒后到来下一波
	int monster_id_list[ARMORDEFEND_PATH_MAX];							// 每条路线出的怪ID
	int monster_count_list[ARMORDEFEND_PATH_MAX];						// 每条路线出的怪数量
	int total_monster_count;											// 怪总量
	int kill_score;														// 击杀怪物加积分
};

struct ArmorDefendWaveCfg
{
	ArmorDefendWaveCfg() : wave(0), next_wave_s(0), total_monster_count(0), kill_score(0)
	{
		memset(monster_id_list, 0, sizeof(monster_id_list));
		memset(monster_count_list, 0, sizeof(monster_count_list));
	}

	int wave;
	int next_wave_s;													// 这么多秒后到来下一波
	int monster_id_list[ARMORDEFEND_PATH_MAX];							// 每条路线出的怪ID
	int monster_count_list[ARMORDEFEND_PATH_MAX];						// 每条路线出的怪数量
	int total_monster_count;											// 怪总量
	int kill_score;														// 击杀怪物加积分
};

struct ArmorDefendPassLevelCfg
{
	ArmorDefendPassLevelCfg() : req_wave(0), req_passtime_s(0) {}

	int req_wave;
	int req_passtime_s;
};

struct ArmorDefendLevelCfg
{
	static const int CONFIG_ITEM_MAX_COUNT = 30;
	UNSTD_STATIC_CHECK(CONFIG_ITEM_MAX_COUNT < 128);

	struct ConfigItem
	{
		ConfigItem() : level(0), role_level(0), scene_id(0), birth_pos(0, 0), wave_count(0), new_player_wave_count(0)
		{
			
		}

		int level;
		int role_level;
		int scene_id;
		Posi birth_pos;

		int wave_count;					
		ArmorDefendWaveCfg wave_list[PERSONAL_ARMORDEFEND_MAX_WAVE_PER_LEVEL];
		int new_player_wave_count;
		ArmorDefendWaveCfg new_player_wave_list[PERSONAL_ARMORDEFEND_MAX_WAVE_PER_LEVEL];
		ItemConfigData find_reward[MAX_ATTACHMENT_ITEM_NUM];
	};

	ArmorDefendLevelCfg() : cfg_count(0)
	{
	}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct ArmorDefendOtherCfg
{
	ArmorDefendOtherCfg() : free_join_times(0), auto_fb_empty_grid(0), auto_item_id(0), auto_item_count(0),
		escape_num_to_failure(0), boss_num_to_failure(0),escape_num_to_warn(0), guid_task_id(0), reward_count(0)
	{

	}

	int free_join_times;
	int auto_fb_empty_grid;
	ItemID auto_item_id;
	int auto_item_count;

	int escape_num_to_failure;
	int boss_num_to_failure;
	int escape_num_to_warn;

	int guid_task_id;
	int reward_count;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct ArmorDefendSkillCfg
{
	static const int CONFIG_ITEM_MAX_COUNT = PERSONAL_ARMORDEFEND_SKILL_MAX_COUNT ;

	struct ConfigItem
	{
		ConfigItem() : skill_index(0), attr_type(0), skill_seq(0), skill_id(0), hurt_percent(0), fix_hurt(0), cd_factor(0), cd_s(0), distance(0), energy_cost(0), param_a(0), param_b(0), param_c(0)
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
	};

	ArmorDefendSkillCfg()
	{

	}

	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct ArmorDefendMonsterPath
{
	ArmorDefendMonsterPath() : cfg_count(0) {}

	int cfg_count;
	Posi pos_list[ARMORDEFEND_PATH_POINT_MAX];
};

struct ArmorDefendBuyJoinTimesCfg
{
	const static int CONFIG_ITEM_MAX_COUNT = 16;

	struct ConfigItem
	{
		ConfigItem() : buy_times(0), gold_cost(0) {}

		int buy_times;
		int gold_cost;
	};

	ArmorDefendBuyJoinTimesCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};


class ArmorDefendConfig: public ConfigBase
{
public:
	ArmorDefendConfig();
	~ArmorDefendConfig();

	bool Init(const std::string &configname, std::string *err);

	int GetNewPlayerTaskId() { return m_other_cfg.guid_task_id; }

	int GetRefreshStartDelay() const { return m_wave_refresh_start_delay; }
	int GetRefreshInterval() const { return m_wave_refresh_interval; }

	int GetEscapeNumToFail()const { return m_other_cfg.escape_num_to_failure; }
	int GetEscapeNumToWarning()const { return m_other_cfg.escape_num_to_warn; }
	int GetWaveCountBySceneID(int scene_id, bool is_new_player) const;
	const ArmorDefendWaveCfg * GetWaveCfg(int scene_id, int wave, bool is_new_player) const;
	const ArmorDefendWaveCfg * GetWaveCfgByMonsterID(int scene_id, int monster_id, bool is_new_player) const;
	const ArmorDefendMonsterPath* GetArmorDefendPathByMonsterID(int scene_id, int monster_id, bool is_new_player) const;

	Posi GetBirthPos(int path) const;
	int GetArmorDefendPathPosCount(int path) const;
	Posi GetArmorDefendPathPos(int path, int pos_idx) ;
	

	int GetLifeTowerMonsterID() const { return m_life_tower_monster_id; }
	
	bool CheckWarning(int old_percent, int now_percent, int *warning_percent);

	int GetWarningInterval() const { return m_warning_interval_s; }
	int GetSyncInterval() const { return m_sync_interval_s; }

	const ArmorDefendLevelCfg::ConfigItem * GetLevelCfgBySceneID(int scene_id) const;
	const ArmorDefendLevelCfg::ConfigItem * GetLevelCfg(int level) const;
	const ArmorDefendOtherCfg & GetOtherCfg() { return m_other_cfg; }
	const ArmorDefendSkillCfg::ConfigItem * GetSkillCfg(int skill_index);
	int GetBuyJoinTimesCfg(int buy_times);
	int GetEnergyByMonsterId(int monster_id);

private:
	int InitNewPlayerWaveCfg(PugiXmlNode RootElement);
	int InitWaveCfg(PugiXmlNode RootElement);
	int InitLevelCfg(PugiXmlNode RootElement);
	int InitPathCfg(PugiXmlNode RootElement);
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitSkillCfg(PugiXmlNode RootElement);
	int InitBuyJoinTimesCfg(PugiXmlNode RootElement);
	int InitGetEnergyCfg(PugiXmlNode RootElement);

	bool CheckLevelMonsterID(const ArmorDefendLevelCfg::ConfigItem &cfg_item);

	int m_wave_refresh_start_delay;										// 第一波怪延迟秒
	int m_wave_refresh_interval;										// 出怪间隔

	ArmorDefendMonsterPath wave_pathway_list[ARMORDEFEND_PATH_MAX];

	int m_life_tower_monster_id;										// 生命塔怪ID
	int m_team_life_tower_monster_id;									// 组队塔防 生命塔怪ID

	int m_warning_interval_s;											// 生命塔挨打的报警间隔
	int m_warning_percent_list[ARMORDEFEND_WARNING_PERCENT_COUNT];		// 生命塔报警的血量百分比列表

	int m_sync_interval_s;												// 定时同步信息的间隔

	ArmorDefendLevelCfg m_level_cfg;									// 关卡配置
	ArmorDefendOtherCfg m_other_cfg;									// 其他配置
	ArmorDefendSkillCfg m_skill_cfg;									// 技能配置
	ArmorDefendBuyJoinTimesCfg m_buy_join_times_cfg;					// 购买参与次数配置

	std::map<int,int> m_get_energy;									//能量获取
};

#endif
