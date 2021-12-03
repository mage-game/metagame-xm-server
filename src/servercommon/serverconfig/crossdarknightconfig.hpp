#ifndef __CROSS_DARK_NIGHT_CONFIG_HPP__
#define __CROSS_DARK_NIGHT_CONFIG_HPP__

#include "servercommon/crossdef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/serverdef.h"
#include "servercommon/configcommon.h"

struct CrossDarkNightOtherCfg
{
	CrossDarkNightOtherCfg() : is_open(false), open_level(0), scene_id(0), born_posi(0, 0), leave_reward_percent(0), flush_monster_interval_s(0),
		score_reward_interval_s(0), reward_score(0), kill_role_score(0), role_assis_score(0), boss_assis_score(0) { }

	bool is_open;
	int open_level;
	int scene_id;
	Posi born_posi;
	int leave_reward_percent;
	int flush_monster_interval_s;
	int score_reward_interval_s;
	int reward_score;
	int kill_role_score;
	int role_assis_score;
	int boss_assis_score;
};

struct CrossDarkNightOpenTimeInfo
{
	CrossDarkNightOpenTimeInfo() : activity_week_day(0), activity_ready_time(0), activity_start_time(0), activity_end_time(0), ready_dur_s(0), open_dur_s(0) {}

	int activity_week_day;												
	int activity_ready_time;												
	int activity_start_time;												
	int activity_end_time;													

	int ready_dur_s;														
	int open_dur_s;															
};

struct CrossDarkNightMonsterCfg
{
	CrossDarkNightMonsterCfg() : monster_id(0), kill_rewrd(0), reward_num(0), box_rate(0), score(0) { }

	int monster_id;
	int kill_rewrd;
	int reward_num;
	int box_rate;
	int score;
};

struct CrossDarkNightBossCfg
{
	CrossDarkNightBossCfg() :seq_index(0),boss_id(0), rand_posi_index_begin(0), rand_posi_index_end(0), kill_rewrd(0), score(0) { }

	int seq_index;
	int boss_id;
	int rand_posi_index_begin;
	int rand_posi_index_end;
	int kill_rewrd;
	int score;
	NumListConfig scene_role_num;
	NumListConfig reward_num;
	NumListConfig drop_num;
	PointListConfig drop_pos_list;
};

struct CrossDarkNightScoreCfg
{
	CrossDarkNightScoreCfg() : score(0), item_count(0), cross_honor(0)
	{
	}

	int score;
	int item_count;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
	int cross_honor;
};

struct CrossDarkNightMonsterInfo
{
	CrossDarkNightMonsterInfo() : monster_count(0) { }

	int monster_count;
	CrossDarkNightMonsterCfg monste_list[CROSS_DARK_NIGHT_MONSTER_MAX_COUNT];
};

struct CrossDarkNightBossCfgInfo
{
	CrossDarkNightBossCfgInfo() : boss_count(0) {}

	int boss_count;
	CrossDarkNightBossCfg boss_list[CROSS_DARK_NIGHT_BOSS_MAX_COUNT];
};

struct CrossDarkNightBoxCfg
{
	CrossDarkNightBoxCfg(): gather_id(0), another_reward_times(0), gather_time_s(0), disappear_time_s(0){ }

	int gather_id;
	int another_reward_times;
	int gather_time_s;
	int disappear_time_s;
	ItemConfigData reward_item;
	ItemConfigData reward_item2;
};

class CrossDarkNightConfig
{
public:
	CrossDarkNightConfig();
	~CrossDarkNightConfig();

	static CrossDarkNightConfig & Instance();
	bool Init(const std::string &path, std::string *err);

	const CrossDarkNightOpenTimeInfo *GetActivityOpenTimeInfo(int index);
	Posi GetRandomMonsterCreatePosByMonsterId(int monster_id);
	const CrossDarkNightMonsterCfg *GetMonsterInfoCfg(int monster_id);
	const CrossDarkNightBoxCfg *GetBoxInfo() { return &m_box_cfg; }
	const CrossDarkNightBossCfg *GetBossInfoById(int boss_id);
	const CrossDarkNightBossCfg *GetBossInfoByIndex(int pos_index,int index);
	void GetDropPosiListById(int boss_id, std::vector<Posi> &rand_pos_list);

	const CrossDarkNightOtherCfg & GetOtherCfg() { return m_other_cfg;  }
	const CrossDarkNightMonsterInfo &GetMonsterCfg() { return m_monster_cfg; }
	//const CrossDarkNightBossCfgInfo &GetBossCfg() { return m_boss_cfg; }
	const std::vector<CrossDarkNightScoreCfg> &GetScoreVec() { return m_score_cfg_vec; }

	int GetOpenActivityConfig(CrossDarkNightOpenTimeInfo cfg_list[CROSS_DARK_NIGHT_OPEN_TIME_MAX_COUNT]);

	void GetBossDropAndRewardNumByRoleNum(int *drop_num, int *reward_num, const CrossDarkNightBossCfg * boss_cfg, int role_num);

	Posi GetRandRealivePosi();
	Posi GetRealivePosiByKey(unsigned int key) const;
	Posi GetRandFreshBoxPosi();

private:
	int InitOtherCfg(PugiXmlNode Rootelement);
	int InitActivityOpenCfg(PugiXmlNode Rootelement);
	int InitBoxCfg(PugiXmlNode Rootelement);
	int InitMonsterCfg(PugiXmlNode Rootelement);
	int InitRandPosiCfg(PugiXmlNode Rootelement);
	int InitScoreCfg(PugiXmlNode Rootelement);
	int InitBossCfg(PugiXmlNode Rootelement);
	int InitRealivePosiCfg(PugiXmlNode Rootelement);
	int InitBoxFreshPosiCfg(PugiXmlNode Rootelement);

	bool DayTimeFormatInvalid(int day_time);

	CrossDarkNightOtherCfg m_other_cfg;

	int m_activity_open_time_count;
	CrossDarkNightOpenTimeInfo m_activity_open_cfg_list[CROSS_DARK_NIGHT_OPEN_TIME_MAX_COUNT];

	CrossDarkNightBoxCfg m_box_cfg;

	int m_rand_pos_count;
	Posi m_monster_create_pos_list[CROSS_DARK_NIGHT_MONSTER_POS_MAX_COUNT];

	CrossDarkNightMonsterInfo m_monster_cfg;
	CrossDarkNightBossCfgInfo m_boss_cfg[CROSS_DARK_NIGHT_BOSS_POS_INDEX_MAX];

	std::vector<CrossDarkNightScoreCfg> m_score_cfg_vec;

	std::vector<Posi> m_realive_posi_vec;
	std::vector<Posi> m_box_fresh_posi_vec;
};

#endif //__CROSS_DARK_NIGHT_CONFIG_HPP__