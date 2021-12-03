#ifndef __CROSS_PASTURE_CONFIG_HPP__
#define __CROSS_PASTURE_CONFIG_HPP__

#include "servercommon/crossdef.hpp"
#include "servercommon/activitydef.hpp"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/serverdef.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct CrossPastureOtherCfg
{
	CrossPastureOtherCfg() : is_open(false), scene_id(0), day_max_get_score_times(0), monster_create_interval_s(0), player_relive_time_s(0), dec_speed_percent(0), rank_title_id(0){}

	bool is_open;															// 是否开放本功能-总开关

	int scene_id;															// 牧场场景ID
	Posi role_pos;															// 玩家出生点

	int day_max_get_score_times;											// 每天最大获取积分次数	
	int monster_create_interval_s;											// 怪物重新生成间隔（秒）
	int player_relive_time_s;												// 击杀玩家获得积分

	int dec_speed_percent;													// 减速百分比
	int rank_title_id;													    
};

struct CrossPastureActivityOpenTimeInfo
{
	CrossPastureActivityOpenTimeInfo() : activity_week_day(0), activity_ready_time(0), activity_start_time(0), activity_end_time(0), ready_dur_s(0), open_dur_s(0) {}

	int activity_week_day;													// 活动所在周几
	int activity_ready_time;												// 活动等待时间点
	int activity_start_time;												// 活动开始时间点
	int activity_end_time;													// 活动结束时间点

	int ready_dur_s;														// 活动等待时间
	int open_dur_s;															// 开启持续时间
};

struct CrossPastureMonsterInfoConfig
{
	CrossPastureMonsterInfoConfig() : monster_id(0), score(0), max_num(0), rand_pos_index_begin(0), rand_pos_index_end(0), weight(0), is_count(1), is_control_refresh(0){}

	int monster_id;
	int score;
	int max_num;
	int rand_pos_index_begin;
	int rand_pos_index_end;
	ItemConfigData reward_item;
	int weight;
	short is_count;
	short is_control_refresh;
};

struct CrossPastureFenceConfig
{
	CrossPastureFenceConfig() : fence_id(0), x1(0), x2(0), y1(0), y2(0) {}

	int fence_id;
	int x1;
	int x2;
	int y1;
	int y2;
};

struct CrossPastureSkillConfig
{
	CrossPastureSkillConfig() : index(0), skill_id(0) {}

	int index;
	int skill_id;
};

struct CrossPastureRankReward
{
	CrossPastureRankReward():min_rank(0),max_rank(0){}
	int min_rank;
	int max_rank;
	ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
};

class CrossPastureConfig
{
public:
	CrossPastureConfig();
	~CrossPastureConfig();

	static CrossPastureConfig & Instance();
	bool Init(const std::string &path, std::string *err);

	const CrossPastureOtherCfg & GetOtherCfg() { return m_other_cfg; }
	CrossPastureActivityOpenTimeInfo *GetCrossPastureActivityOpenTimeInfo(int index);// 获取活动的开启配置
	Posi GetRandomMonsterCreatePosByMonsterId(int monster_id);									 // 获取一个随机的怪物出生点

	int GetOpenActivityConfig(CrossPastureActivityOpenTimeInfo cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT]);
	int GetMonsterInfoCfgCount() { return m_monster_info_cfg_count; }
	const CrossPastureMonsterInfoConfig *GetMonsterInfoCfgWithIndex(int index);
	const CrossPastureMonsterInfoConfig *GetMonsterInfoCfgWithMonsterID(int monster_id);

	int GetFenceCfgCount() { return m_fence_cfg_count; }
	const CrossPastureFenceConfig *GetFenceCfg(int index);

	const CrossPastureSkillConfig *GetSkillCfgList() { return m_skill_cfg_list; }

	const CrossPastureMonsterInfoConfig *GetRandRewardWithMonsterID(int monster_id);

	int GetSpecialMonsterId() { return spical_monster_id; }

	const CrossPastureRankReward * GetRankRewardByRank(int rank) const;
private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitActivityOpenTimeCfg(PugiXmlNode RootElement);
	int InitReliveCfg(PugiXmlNode RootElement);
	int InitMonsterInfoCfg(PugiXmlNode RootElement);
	int InitFenceInfoCfg(PugiXmlNode RootElement);
	int InitSkillInfoCfg(PugiXmlNode RootElement);
	int InitRankReward(PugiXmlNode RootElement);

	bool DayTimeFormatInvalid(int day_time);

	CrossPastureOtherCfg m_other_cfg;

	int m_activity_open_time_cfg_list_count;								// 活动开启时间配置数量
	CrossPastureActivityOpenTimeInfo m_activity_open_time_cfg_list[CROSS_PASTURE_OPEN_TIME_MAX_COUNT];	// 活动开启时间

	int m_monster_create_pos_list_count;									// 复活点数量
	Posi m_monster_create_pos_list[CROSS_PASTURE_RELIVE_POS_MAX_COUNT];

	int m_monster_info_cfg_count;
	CrossPastureMonsterInfoConfig m_monster_info_cfg_list[CROSS_PASTURE_MONSTER_CFG_MAX_COUNT];// 动物信息

	int m_fence_cfg_count;
	CrossPastureFenceConfig m_fence_cfg_list[CROSS_PASTURE_FENCE_CFG_MAX_COUNT];// 栅栏信息

	CrossPastureSkillConfig m_skill_cfg_list[CROSS_PASTURE_SKILL_CFG_MAX_COUNT];// 技能信息

	int spical_monster_id;														//特殊monster id
	std::vector<CrossPastureRankReward> m_rank_reward;
};

#endif // __CROSS_1V1_COMMON_CONFIG_HPP__

