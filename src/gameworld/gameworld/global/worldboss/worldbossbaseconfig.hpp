#ifndef _WORLDBOSSBASECONFIG_HPP_
#define _WORLDBOSSBASECONFIG_HPP_

#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <map>

struct WorldBossOtherCfg
{
	WorldBossOtherCfg():refresh_time_one(0), refresh_time_two(0), refresh_time_three(0), refresh_hurt_rank(0), person_reward_rank_max(0), guild_reward_rank_max(0), week_reward_rank_max(0),roll_effective_time(0)
		{}

	unsigned int refresh_time_one;
	unsigned int refresh_time_two;
	unsigned int refresh_time_three;
	unsigned int refresh_hurt_rank;
	int person_reward_rank_max;
	int guild_reward_rank_max;
	int week_reward_rank_max;
	unsigned int roll_effective_time;
};

struct WorldBossInfoCfg
{
	WorldBossInfoCfg() :bossID(0), scene_id(0), born_pos(0, 0),per_bind_gold(0), shield_continue_time(0), shield_effective_times(0), shield_fix_hurt(0),
		first_trigger_hudun(0), second_trigger_hudun(0), roll_max_num(0), min_lv(0), max_lv(0)
	{
	}

	int bossID;
	int scene_id;
	Posi born_pos;

	int per_bind_gold;
	unsigned int shield_continue_time;
	int shield_effective_times;
	int shield_fix_hurt;
	int first_trigger_hudun;
	int second_trigger_hudun;
	int roll_max_num;
	int min_lv;
	int max_lv;
	ItemConfigData gift_ierm;
	ItemConfigData stuff_item;
};

struct WorldBossWeekRankRewardCfg
{
	WorldBossWeekRankRewardCfg() : rank_value(0)
	{
	}

	int rank_value;

	ItemConfigData stuff_item;
	ItemConfigData gift_item;
};

struct WorldBossPersonHurtRankRewardCfg
{
	WorldBossPersonHurtRankRewardCfg():bossID(0), rank_value(0){}

	int bossID;
	int rank_value;

	ItemConfigData stuff_item;
	ItemConfigData gift_item;
};

struct WorldBossGuildHurtRankRewardCfg
{
	WorldBossGuildHurtRankRewardCfg():bossID(0), rank_value(0) {}

	int bossID;
	int rank_value;

	ItemConfigData gift_item;
};

struct WorldBossReliveCfg
{
	WorldBossReliveCfg(): boss_scene_id(0), relive_scene_id(0), relive_pos(0,0) {}

	int boss_scene_id;
	int relive_scene_id;
	Posi relive_pos;
};

class WorldBossBaseConfig : public ConfigBase
{
public:
	WorldBossBaseConfig();
	~WorldBossBaseConfig();

	bool Init(const std::string &configname, std::string *err);

	const WorldBossInfoCfg *GetWorldBossByBossId(int boss_id) const;
	int GetWorldBossIdBySceneId(int scene_id);

	std::map<int, WorldBossInfoCfg> & GetWorldBossInfoCfgMap(){return m_boss_info_map;}
	int	WorldBossGetMaxBossCount(){return m_max_worldboss_count;}
	const WorldBossOtherCfg& GetWorldBossOtherCfg(){return m_other_cfg;}
	const WorldBossWeekRankRewardCfg * GetWorldBossWeekRankReward(int rank_value) const;
	const WorldBossPersonHurtRankRewardCfg * GetWorldBossPersonHurtReward(int rank_value, int boss_id) const;
	const WorldBossGuildHurtRankRewardCfg * GetWorldBossGuildHurtReward(int rank_value, int boss_id) const;
	const WorldBossInfoCfg * GetWorldBossInfo(int boss_id) const;

	const WorldBossReliveCfg * GetWorldBossReliveInfo(int scene_id) const;

	bool GetNextRefreshTimeOfBoss(unsigned int *next_refresh_time_one) const;

private:

	int InitWorldBossOtherCfg(PugiXmlNode RootElement);
	int InitWorldBossCfg(PugiXmlNode RootElement);
	int InitWorldBossWeekRewardCfg(PugiXmlNode RootElement);
	int InitWorldBossPersonHurtRewardCfg(PugiXmlNode RootElement);
	int InitWorldBossGuildHurtRewardCfg(PugiXmlNode RootElement);
	int InitWorldBossReliveCfg(PugiXmlNode RootElement);

	WorldBossOtherCfg m_other_cfg;

	std::map<int, WorldBossInfoCfg> m_boss_info_map;																	// 世界BOSS信息

	std::map<int, WorldBossWeekRankRewardCfg> m_guild_week_rank_reward_cfg;												// 世界BOSS公会周榜奖励

	std::map<long long, WorldBossPersonHurtRankRewardCfg> m_person_hurt_rank_reward;									// 世界BOSS个人伤害奖励

	std::map<long long, WorldBossGuildHurtRankRewardCfg> m_guild_hurt_rank_reward;										// 世界BOSS公会伤害奖励

	std::map<int, WorldBossReliveCfg> m_worldboss_relive_cfg_map;														// 世界BOSS普通复活配置
														
	int m_max_worldboss_count;					// 世界boss最大数量
};

#endif
