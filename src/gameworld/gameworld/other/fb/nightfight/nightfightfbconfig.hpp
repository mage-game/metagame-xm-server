#ifndef __NIGHT_FIGHT_FB_CONFIG_HPP__
#define __NIGHT_FIGHT_FB_CONFIG_HPP__

#include "servercommon/configbase.h"
#include "servercommon/struct/itemlistparam.h"
#include "obj/character/attribute.hpp"
#include "monster/monsterinitparam.h"
#include "config/gameconfigcommon/gameconfigcommon.h"

#include <vector>

static const int REWARD_INTERVAL_MAX_NUM = 10;

struct NightFightFbOtherConfig
{
	NightFightFbOtherConfig() : open_cross_begin_day(0), limit_level(0), room_member_limit(0), activity_open_dur_s(0), redistribute_interval_time_s(0), interval_time_s(0),
								interval_time_get_score(0), kill_get_score_per(0), killed_reduce_score_per(0), relive_interval_time_s(0),
								kill_monster_add_score(0), scene_id(0), init_score(0), min_score(0), rank_update_interval_s(0), assisting_area(0),
								assisting_add_score(0), delay_kick_out_time(0), update_role_interval_s(0),boss_flush_interval_s(0), 
								boss_id(0), kill_get_score_range(0), add_score(0),title_first(0),title_second(0),title_third(0)
	{
	}

	int open_cross_begin_day;
	int limit_level;
	int room_member_limit;
	int activity_open_dur_s;		// 活动开启时长(ACTIVITY_STATUS_OPEN状态的时长)
	int redistribute_interval_time_s;
	int interval_time_s;
	int interval_time_get_score;
	int kill_get_score_per;
	int killed_reduce_score_per;
	int relive_interval_time_s;
	int kill_monster_add_score;
	int scene_id;
	int init_score;
	int min_score;
	int rank_update_interval_s;
	int assisting_area;
	int assisting_add_score;
	int delay_kick_out_time;
	int update_role_interval_s;

	int boss_flush_interval_s;
	int boss_id;
	PointConfig boss_born_pos;
	int kill_get_score_range;
	int add_score;
	int title_first;
	int title_second;
	int title_third;

};

struct RelivePos
{
	RelivePos() : relive_pos_x(0), relive_pos_y(0) {}

	int relive_pos_x;
	int relive_pos_y;
};

struct RewardCfg
{
	RewardCfg() : min_rank(0), max_rank(0), reward_count(0),cross_honor(0),shengwang(0) {}

	int min_rank;
	int max_rank;

	int reward_count;
	ItemConfigData reward_item_list[MonsterInitParam::MAX_DROP_ITEM_COUNT];
	int cross_honor;
	int shengwang;
};

class NightFightFbConfig : public ConfigBase
{
public:
	NightFightFbConfig();
	~NightFightFbConfig();

	bool Init(const std::string &configname, std::string *err);

	const NightFightFbOtherConfig &GetOtherCfg() { return m_other_cfg; }
	const Posi GetRelievePos();
	const Posi GetEnterPosStandy() const;
	const RewardCfg *GetRewardCfgByRank(const int rank);
	const int GetTitleIdByRank(const int rank);
private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitRelivePosCfg(PugiXmlNode RootElement);
	int InitRewardCfg(PugiXmlNode RootElement);

	NightFightFbOtherConfig m_other_cfg;

	std::vector<RelivePos> m_relive_pos;

	int m_reward_cfg_count;
	RewardCfg m_reward_cfg_list[REWARD_INTERVAL_MAX_NUM];
};

#endif // __NIGHT_FIGHT_FB_CONFIG_HPP__

