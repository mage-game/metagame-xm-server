#ifndef __MESS_BATTLE_FB_CONFIG_HPP__
#define __MESS_BATTLE_FB_CONFIG_HPP__

#include "obj/character/attribute.hpp"
#include "servercommon/xml_adapter.hpp"
#include "servercommon/struct/itemlistparam.h"
#include <vector>
#include "monster/monsterinitparam.h"
#include "servercommon/configbase.h"

static const int MESS_BATTLE_REWARD_INTERVAL_MAX_NUM = 10;
static const int MESS_BATTLE_RANK_TOP_TITLE_MAX = 3;

struct MessBattleFbOtherConfig
{
	MessBattleFbOtherConfig() : open_cross_begin_day(0),limit_level(0), room_member_limit(0), kill_item_get_score(0),
								snatch_score_per(0), min_score(0), max_score(0), scene_id(0),one_score_to_honor(0), 
								init_score(0), activity_open_dur_s(0),rank_update_interval_s(0), redistribute_interval_time_s(0), update_role_interval_s(0),
								boss_id(0), delay_kick_out_time(0), boss_posi(0, 0), submit_report_limit_score(0), wudi_time_s(0) {}

	int open_cross_begin_day;
	int limit_level;
	int room_member_limit;
	int kill_item_get_score;
	int snatch_score_per;
	int min_score;
	int max_score;
	int scene_id;
	int one_score_to_honor;
	int init_score;
	int activity_open_dur_s;		// 活动开启时长(ACTIVITY_STATUS_OPEN状态的时长)
	int rank_update_interval_s;
	int redistribute_interval_time_s;
	int update_role_interval_s;
	int boss_id;
	int delay_kick_out_time;
	Posi boss_posi;
	int submit_report_limit_score;
	int wudi_time_s;
};

struct MessBattleRelivePos
{
	MessBattleRelivePos() : relive_pos_x(0), relive_pos_y(0) {}

	int relive_pos_x;
	int relive_pos_y;
};

struct MessBattleKillBossGetScoreCfg
{
	MessBattleKillBossGetScoreCfg() : min_rank(0), max_rank(0), get_score(0) {}

	int min_rank;
	int max_rank;

	int get_score;
};

struct MessBattleRewardCfg
{
	MessBattleRewardCfg() : min_rank(0), max_rank(0), reward_count(0),cross_honor(0),shengwang(0) {}

	int min_rank;
	int max_rank;

	int reward_count;
	ItemConfigData reward_item_list[MonsterInitParam::MAX_DROP_ITEM_COUNT];
	int cross_honor;
	int shengwang;
};

class MessBattleFbConfig : public ConfigBase
{
public:
	MessBattleFbConfig();
	~MessBattleFbConfig();

	bool Init(const std::string &configname, std::string *err);

	const MessBattleFbOtherConfig &GetOtherCfg() { return m_other_cfg; }
	const Posi GetRelievePos();
	const Posi GetRelievePosByKey(int key);
	int GetRelievePosSize() const { return static_cast<int>(m_relive_pos.size()); }
	const MessBattleRewardCfg *GetRewardCfgByRank(const int rank);
	int GetKillBossGetScoreCfgByRank(const int rank) const;
	int GetTitleIdByRank(const int rank)const;
private:
	int InitOtherCfg(YY_XML_NODE RootElement);
	int InitRelivePosCfg(YY_XML_NODE RootElement);
	int InitKillBossGetScoreCfg(YY_XML_NODE RootElement);
	int InitRewardCfg(YY_XML_NODE RootElement);
	int InitRewardTitle(YY_XML_NODE RootElement);

	MessBattleFbOtherConfig m_other_cfg;

	std::vector<MessBattleRelivePos> m_relive_pos;

	int m_kill_boss_get_score_cfg_count;
	MessBattleKillBossGetScoreCfg m_kill_boss_get_score_cfg_list[MESS_BATTLE_REWARD_INTERVAL_MAX_NUM];

	int m_reward_cfg_count;
	MessBattleRewardCfg m_reward_cfg_list[MESS_BATTLE_REWARD_INTERVAL_MAX_NUM];

	int m_title_reward[MESS_BATTLE_RANK_TOP_TITLE_MAX];
};

#endif // __MESS_BATTLE_FB_CONFIG_HPP__

