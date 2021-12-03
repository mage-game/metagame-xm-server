#ifndef _GUILD_TOWER_DEFEND_CONFIG_HPP_
#define _GUILD_TOWER_DEFEND_CONFIG_HPP_

#include "servercommon/serverdef.h"
#include "servercommon/configcommon.h"
#include "servercommon/guilddef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include "gameworld/gamedef.h"

struct GuildTowerDefendDirectionCfg
{
	GuildTowerDefendDirectionCfg() : shouhu_monster_id(0), shouhu_monster_pos(0, 0), refresh_pos(0, 0) {}

	int shouhu_monster_id;
	Posi shouhu_monster_pos;
	Posi refresh_pos;
};

struct GuildTowerDefendWaveCfg
{
	GuildTowerDefendWaveCfg() : monster_id(0), count(0), is_boss(false), next_wave_time(0), reward_longhun(0) {}

	int monster_id;
	int count;
	bool is_boss;
	int next_wave_time;
	int reward_longhun;
	ItemConfigData item;
};

struct GuildTowerDefendSkillCfg
{
	GuildTowerDefendSkillCfg() : cfg_skill_id(0), skill_id(0), distance(0), range(0), cd(0), param_a(0), param_b(0), param_c(0) {}

	int cfg_skill_id;
	int skill_id;
	int distance;
	int range;
	int cd;
	int param_a;
	int param_b;
	int param_c;
};

class GuildTowerDefendConfig
{
public:
	GuildTowerDefendConfig();
	~GuildTowerDefendConfig();

	bool Init(const std::string &path, std::string *err);

	int GetActivityTime() { return m_activity_time; }
	int GetReadyTime() { return m_ready_time; }
	int GetLongZhuMonsterId() { return m_longzhu_monster_id; }
	const Posi & GetLongZhuPos() { return m_longzhu_pos; }
	int GetRefreshInterval() { return m_refresh_interval; }
	int GetRefreshAddPercent() { return m_refresh_add_percent; }
	int GetDeadKickCount() { return m_dead_kick_count; }
	int GetBossHurt() { return m_boss_hurt; }
	int GetBossHurtInterval() { return m_boss_hurt_interval; }

	int GetDirectionCfgCount() { return m_tower_defend_direction_cfg_count; }
	GuildTowerDefendDirectionCfg * GetGuildTowerDefendDirectionCfg(int index);

	int GetWaveCfgCount() { return m_tower_defend_wave_cfg_count; }
	GuildTowerDefendWaveCfg * GetGuildTowerDefendWaveCfg(int wave_index);
	int GetWaveOfMonster(int monster_id);

	GuildTowerDefendSkillCfg * GetGuildTowerDefendSkillCfg(int skill_id);

	const ItemConfigData * GetClearWaveReward(int clear_wave);

private:
	int m_activity_time;
	int m_ready_time;
	int m_longzhu_monster_id;
	Posi m_longzhu_pos;
	int m_refresh_interval;
	int m_refresh_add_percent;
	int m_dead_kick_count;
	int m_boss_hurt;
	int m_boss_hurt_interval;

	int m_tower_defend_direction_cfg_count;
	GuildTowerDefendDirectionCfg m_tower_defend_direction_cfg_list[MAX_TOWER_DEFEND_DIRECTION];

	int m_tower_defend_wave_cfg_count;
	GuildTowerDefendWaveCfg m_tower_defend_wave_cfg_list[MAX_TOWER_DEFEND_WAVE];
	std::map<int, int> m_monster_wave_map;

	std::map<int, GuildTowerDefendSkillCfg> m_skill_cfg_map;

	ItemConfigData m_clear_wave_reward_list[MAX_TOWER_DEFEND_WAVE + 1];
};

#endif	// _GUILD_CONFIG_HPP_

