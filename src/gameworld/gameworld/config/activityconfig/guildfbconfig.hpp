#ifndef _GUILDFB_CONFIG_HPP_
#define _GUILDFB_CONFIG_HPP_

#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gamedef.h"

#include <vector>

static const int GUILD_FB_WAVE_MAX = 8;					// 最多8波怪物
static const int GUILD_FB_WAVE_MAX_REWARD_COUNT = 6;	// 每波奖励物品最大数
static const int GUILD_FB_ENEMY_POINT_MAX = 32;			// 总共32个怪点
static const int GUILD_FB_GUARD_PATH_POINT_MAX = 32;	// 守卫boss路径点个数

struct GuildFbEnemyCfg
{
	GuildFbEnemyCfg() : sub_wave(0), enemy_monster_id(0), birth_pos(0, 0) {}

	int sub_wave;
	int enemy_monster_id;
	Posi birth_pos;
};

struct GuildFbWaveCfg
{
	GuildFbWaveCfg() : sceneid(0), enter_pos(0, 0), guard_boss_maxhp(0), guard_boss_recover_per(0), reward_gongxian(0), guard_pt_count(0), enemy_count(0), reward_count(0) {}

	int sceneid;
	Posi enter_pos;
	int guard_boss_maxhp;
	int guard_boss_recover_per;
	int reward_gongxian;
  
	int guard_pt_count;
	Posi guard_path_ptlist[GUILD_FB_GUARD_PATH_POINT_MAX];

	int enemy_count;
	GuildFbEnemyCfg enemy_list[GUILD_FB_ENEMY_POINT_MAX];


	int reward_count;
	ItemConfigData reward_item_list[GUILD_FB_WAVE_MAX_REWARD_COUNT];
};

class GuildFbConfig : public ConfigBase
{
public:
	GuildFbConfig();
	~GuildFbConfig();

	bool Init(const std::string &configname, std::string *err);

	int GetEnterInfo(Posi *enter_pos);
	int GetGuardBossID(){ return m_guard_bossid; }
	
	int GetGuildExpCost() { return m_guildexp_cost; }
	int GetGuildLvLimit() { return m_guildlv_limit; }

	int GetGuardWaveCount() { return m_wave_count; }
	int GetGuardWaitWaveTime() { return m_next_wave_wait_time; }
	unsigned int GetDelayKickTime(){return m_delay_kick_time;}
	int GetGuardBossMaxHp(int wave_idx);
	bool GetGuardPathPos(int wave_idx, int pos_idx, Posi *pos);	

	int GetGuildFbIntervalS() { return m_activity_interval_s; }

	GuildFbWaveCfg * GetGuildFbWaveCfg(int wave_idx);

private:
	int m_sceneid;
	Posi m_enter_pos;

	int m_guildexp_cost;
	int m_guildlv_limit;

	int m_guard_bossid;

	int m_next_wave_wait_time;

	int m_wave_count;
	GuildFbWaveCfg m_wave_list[GUILD_FB_WAVE_MAX];

	unsigned int m_delay_kick_time;
	int m_activity_interval_s;
};

#endif // _GUILDFB_CONFIG_HPP_



