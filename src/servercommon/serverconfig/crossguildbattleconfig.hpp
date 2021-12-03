#ifndef __CROSS_GUILD_BATTLE_CONFIG_HPP__
#define __CROSS_GUILD_BATTLE_CONFIG_HPP__

#include "servercommon/crossdef.hpp"
#include "servercommon/activitydef.hpp"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/serverdef.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include <vector>

static const int MAX_CROSS_GUILD_BATTLE_OWNREWARD_NUM = 4;

struct CrossGuildBattleOtherCfg
{
	CrossGuildBattleOtherCfg() : is_open(false), relive_time(0), kill_add_score(0), assist_time(0),
							assist_add_score(0), add_score_interval(0), add_score_once(0), level_limit(0), openserver_limit(0),
							skip_task_consume(0), skip_task_level_limit(0), 
							wing_drop_id(0), wing_drop_rate(0), wing_drop_daily_num_limit(0), wing_drop_total_num(0), extra_drop_times(0), main_scene_id(0),
							special_open_time(0), special_end_time(0), effective_range(0)
	{}

	bool is_open;

	int relive_time;
	int kill_add_score;	
	int assist_time;
	int assist_add_score;
	int add_score_interval;
	int add_score_once;

	int level_limit;
	int openserver_limit;

	int skip_task_consume;
	int skip_task_level_limit;

	int wing_drop_id;					// 羽翼掉落id
	int wing_drop_rate;					// 羽翼掉落概率
	int wing_drop_daily_num_limit;		// 羽翼掉落每日获得上限
	int wing_drop_total_num;			// 羽翼掉落总数

	int extra_drop_times;				// 占领城市后额外掉落次数
	int main_scene_id;					// 主城场景id

	int special_open_time;				// 三倍开启时间
	int special_end_time;				// 三倍结束时间

	int effective_range;
};

struct CrossGuildBattleActivityOpenTimeInfo
{
	CrossGuildBattleActivityOpenTimeInfo() : activity_week_day(0), activity_ready_time(0), activity_start_time(0), activity_end_time(0), ready_dur_s(0), open_dur_s(0) {}

	int activity_week_day;													// 活动所在周几
	int activity_ready_time;												// 活动等待时间点
	int activity_start_time;												// 活动开始时间点
	int activity_end_time;													// 活动结束时间点

	int ready_dur_s;														// 活动等待时间
	int open_dur_s;															// 开启持续时间
};

struct CrossGuildBattleSceneCfg
{
	CrossGuildBattleSceneCfg() : scene_id(0), max_score(0)
	{
		memset(enter_pos_list, 0, sizeof(enter_pos_list));
	}

	static constexpr int ENTER_POS_COUNT = 2;

	int scene_id;	
	int max_score;

	Posi enter_pos_list[ENTER_POS_COUNT];
};

struct CrossGuildBattleFlagCfg
{
	CrossGuildBattleFlagCfg() : scene_id(0), flag_id(0), flush_pos(0, 0), add_score_interval(0), add_score_once(0) {}

	int scene_id;
	int flag_id;
	Posi flush_pos;
	int add_score_interval;
	int add_score_once;
};

struct CrossGuildBattleOwnRewardCfg
{
	CrossGuildBattleOwnRewardCfg() : guild_boss_reward_count(0), part_0(0), index_0(0), part_1(0), index_1(0), guild_reward_gongzi(0), team_reward_item_count(0){}

	ItemConfigData server_reward_item[MAX_ATTACHMENT_ITEM_NUM];
	ItemConfigData guild_reward_item[MAX_ATTACHMENT_ITEM_NUM];
	int guild_boss_reward_count;
	ItemConfigData guild_boss_reward_item[MAX_CROSS_GUILD_BATTLE_OWNREWARD_NUM];
	int part_0;
	int index_0;
	int part_1;
	int index_1;
	int guild_reward_gongzi;
	int team_reward_item_count;
	ItemConfigData team_reward_item[MAX_CROSS_GUILD_BATTLE_OWNREWARD_NUM];
};

struct CrossGuildBattleScoreRewardCfg
{
	CrossGuildBattleScoreRewardCfg() : score(0), exp(0), convert_credit(0), cross_honor(0){}

	int score;
	long long exp;
	int convert_credit;
	int cross_honor;

	ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
};

struct CrossGuildBattleTaskCfg
{
	CrossGuildBattleTaskCfg() : task_index(-1), task_id(-1), scene_id(0), task_type(0), task_param(0), task_count(0),
		cross_honor(0), extra_cross_honor(0), auto_complete_need_gold(0)
	{}

	int task_index;
	int task_id;
	int scene_id;

	int task_type;
	int task_param;
	int task_count;

	int cross_honor;
	int extra_cross_honor;
	int auto_complete_need_gold;

	ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
	ItemConfigData extra_item[MAX_ATTACHMENT_ITEM_NUM];
	ItemConfigData special_time_item[MAX_ATTACHMENT_ITEM_NUM];
};

struct CrossGuildBattleBossCfg
{
	CrossGuildBattleBossCfg() : boss_id(0), scene_id(0), born_pos(0, 0), 
	start_refresh_time(0), end_refresh_time(0), refresh_interval(0), 
	start_refresh_time1(0), end_refresh_time1(0), refresh_interval1(0),
	kill_credit(0)
	{}

	int boss_id;
	int scene_id;
	Posi born_pos;

	int start_refresh_time;
	int end_refresh_time;
	int refresh_interval;

	int start_refresh_time1;  // 暂时不用这组，之前是刷新3波，现改为两波
	int end_refresh_time1;
	int refresh_interval1;

	int kill_credit;
};

struct CrossGuildBattleMonsterCredit 
{
	CrossGuildBattleMonsterCredit() : credit(0), extra_credit(0)
	{}

	int credit;
	int extra_credit;
};

struct CrossGuildBattleDropNumWeight
{
	CrossGuildBattleDropNumWeight() : drop_times(0), weight(0)
	{}

	int drop_times;
	int weight;
};

enum CROSS_GUILD_BATTLE_FLAG_HP_WARNING
{
	CROSS_GUILD_BATTLE_FLAG_HP_WARNING_20 = 20,
	CROSS_GUILD_BATTLE_FLAG_HP_WARNING_50 = 50,
};

struct CrossGuildBattleGather
{
	CrossGuildBattleGather(): gather_id(0),boss_id(0),gather_time(0),disappear_gather_max(0),disappear_s(0)
	{}
	int gather_id;
	int boss_id;
	int gather_time;
	int disappear_gather_max;
	int disappear_s;
	ItemConfigData guild_reward_item;
};

class CrossGuildBattleConfig
{
public:
	CrossGuildBattleConfig();
	~CrossGuildBattleConfig();

	static CrossGuildBattleConfig & Instance();
	bool Init(const std::string &path, std::string *err);
	
	const CrossGuildBattleOtherCfg &GetOtherCfg() { return m_other_cfg; }

	CrossGuildBattleActivityOpenTimeInfo *GetActivityOpenTimeInfo(int index);
	int GetOpenActivityConfigList(CrossGuildBattleActivityOpenTimeInfo cfg_list[CROSS_PASTURE_OPEN_TIME_MAX_COUNT]);

	const CrossGuildBattleSceneCfg *GetBattleSceneCfg(int scene_id);

	bool IsCrossGuildBattleScene(int scene_id);
	Posi GetRandEnterPosBySceneID(int scene_id);
	int GetRandEnterPosByIndex(int index, Posi *pos);
	Posi GetStandyEnterPosBySceneID(int scene_id);

	void GetGuildBattleSceneIdList(int scene_id_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM]);
	int GetCityIndex(const int scene_id);
	
	void GetFlagIdList(int scene_id, int flag_id_list[CROSS_GUILDBATTLE_MAX_FLAG_IN_SCENE]);
	const CrossGuildBattleFlagCfg *GetFlagCfg(int scene_id, int flag_id);
	bool IsFlag(int flag_id);

	const CrossGuildBattleScoreRewardCfg *GetScoreRewardCfg(int old_score, int new_score);
	const CrossGuildBattleOwnRewardCfg *GetOwnServerRewardCfg(int index);

	bool DayTimeFormatInvalid(int day_time);
	bool IsZhuMoTime(unsigned int second);

	const CrossGuildBattleTaskCfg *GetGuildBattleTaskCfg(int scene_id, int task_id);
	std::map<int, std::map<int, CrossGuildBattleTaskCfg> > & GetGuildBattleTaskMap() { return m_task_cfg_map; }

	const CrossGuildBattleBossCfg *GetGuildBattleBossCfg(int boss_id);
	const std::map<int, CrossGuildBattleBossCfg> & GetGuildBattleBossMap() { return m_boss_map; }

	const CrossGuildBattleMonsterCredit * GetMonsterConvertCreditCfg(unsigned short monster_id);
	int GetSpecialImgDropRate(unsigned short monster_id);

	int GetRandDropWingNum();

	bool CheckWarning(int old_percent, int now_percent, int *warning_percent);
	unsigned int GetWarningInterval() { return m_warning_interval_s; }
	const ItemConfigData * GetDailyDailyRewardItem(int index) const;
	const CrossGuildBattleGather* GetGatherCfgByBossId(int boss_id) const;
	const CrossGuildBattleGather* GetGatherCfgById(int gather_id) const;
	int GetSosCost(int times) const;
private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitActivityOpenTimeCfg(PugiXmlNode RootElement);
	int InitSceneCfg(PugiXmlNode RootElement);
	int InitFlagCfg(PugiXmlNode RootElement);
	int InitOwnRewardCfg(PugiXmlNode RootElement);
	int InitScoreRewardCfg(PugiXmlNode RootElement);
	int InitTaskCfg(PugiXmlNode RootElement);
	int InitBossCfg(PugiXmlNode RootElement);
	int InitMonsterConvertCreditCfg(PugiXmlNode RootElement);
	int InitSpecialImgCfg(PugiXmlNode RootElement);
	int InitDropNumWeightCfg(PugiXmlNode RootElement);
	int InitDailyRewardCfg(PugiXmlNode RootElement);
	int InitGatherCfg(PugiXmlNode RootElement);
	int InitSosCfg(PugiXmlNode RootElemen);
	
	CrossGuildBattleOtherCfg m_other_cfg;

	int m_activity_open_time_cfg_list_count;								// 活动开启时间配置数量
	CrossGuildBattleActivityOpenTimeInfo m_activity_open_time_cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT];	// 活动开启时间

	CrossGuildBattleSceneCfg m_scene_cfgs[CROSS_GUILDBATTLE_MAX_SCENE_NUM];

	static const int MAX_FALG_CFG_NUM = 18;
	int m_flag_num;
	CrossGuildBattleFlagCfg m_flag_cfgs[MAX_FALG_CFG_NUM];

	CrossGuildBattleOwnRewardCfg m_own_reward_cfgs[CROSS_GUILDBATTLE_MAX_SCENE_NUM];

	typedef std::vector<CrossGuildBattleScoreRewardCfg> ScoreRewardVec;
	ScoreRewardVec m_score_reward_vec;

	std::map<int, std::map<int, CrossGuildBattleTaskCfg> > m_task_cfg_map;
	std::map<int, CrossGuildBattleBossCfg> m_boss_map;
	std::map<unsigned short, CrossGuildBattleMonsterCredit> m_monster_convert_credit_cfg_map;
	std::map<unsigned short, int> m_special_img_cfg_map;

	int m_drop_total_weight;
	std::vector<CrossGuildBattleDropNumWeight> m_drop_weight_cfg_vec;

	static const int WARNING_HP_LIST_LEN = 2;

	unsigned int m_warning_interval_s;											// 挨打的报警间隔
	int m_warning_percent_list[WARNING_HP_LIST_LEN];							// 报警的血量百分比列表
	ItemConfigData m_daily_reward_item_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM][MAX_ATTACHMENT_ITEM_NUM];	//每日奖励
	std::map<int, CrossGuildBattleGather> m_boss_gather_map;
	std::vector<int> m_sos_cost_cfg_vec;
};

#endif

