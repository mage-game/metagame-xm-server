#ifndef __GIFT_HARVEST_CONFIG_HPP__
#define __GIFT_HARVEST_CONFIG_HPP__

#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/serverdef.h"
#include "gamedef.h"


// 其他配置
struct GiftHarvestOtherCfg
{
	GiftHarvestOtherCfg() : is_open(false), scene_id(0), day_max_get_score_times(0), monster_create_interval_s(0) {}

	bool is_open;						// 是否开放本功能-总开关
	int scene_id;						// 礼物收割场景ID
	Posi role_pos;						// 玩家出生点
	int day_max_get_score_times;		// 每天最大获取积分次数	
	int monster_create_interval_s;		// 怪物重新生成间隔（秒）
	ItemConfigData  reward_item_list[MAX_ATTACHMENT_ITEM_NUM];			// 参与奖
};

// 轮次开启时间配置
struct GiftHarvestRoundOpenTimeInfo
{
	GiftHarvestRoundOpenTimeInfo() : round(0), round_ready_time(0), round_start_time(0), round_end_time(0), ready_dur_s(0), open_dur_s(0) {}


	int round;							// 轮次
	int round_ready_time;				// 准备时间
	int round_start_time;				// 开始时间
	int round_end_time;					// 结束时间

	int ready_dur_s;					// 活动等待时间
	int open_dur_s;						// 开启持续时间
};

// 怪物信息配置
struct GiftHarvestMonsterInfoConfig
{
	GiftHarvestMonsterInfoConfig() : monster_id(0), score(0), max_num(0) {}

	int monster_id;						// 怪物id
	int score;							// 引入区内可得分数
	int max_num;						// 数量
};

// 围栏区配置
struct GiftHarvestFenceConfig
{
	GiftHarvestFenceConfig() : fence_id(0), x1(0), x2(0), y1(0), y2(0) {}

	int fence_id;						// 围栏安全区id
	int x1;								
	int x2;
	int y1;
	int y2;
};

// 技能配置
struct GiftHarvestSkillConfig
{
	GiftHarvestSkillConfig() : index(0), skill_id(0) {}

	int index;
	int skill_id;
};


class GiftHarvestConfig : public ConfigBase
{
public:
	GiftHarvestConfig();
	~GiftHarvestConfig();

	bool Init(const std::string &path, std::string *err);

	Posi GetRandomMonsterCreatePos();													// 怪物随机出生点
	const GiftHarvestOtherCfg & GetOtherCfg() { return m_other_cfg; }					// 其他配置
	const GiftHarvestRoundOpenTimeInfo *GetGiftHarvestActivityOpenTimeInfo(int round);	// 轮次开启时间配置配置
	const GiftHarvestMonsterInfoConfig *GetMonsterInfoCfgWithIndex(int index);			// 怪物信息配置
	const GiftHarvestMonsterInfoConfig *GetMonsterInfoCfgWithMonsterID(int monster_id); // 怪物信息配置
	const GiftHarvestFenceConfig *GetFenceCfg(int index);								// 安全区配置
	const GiftHarvestSkillConfig *GetSkillCfgList() { return m_skill_cfg_list; }		// 技能配置

	int GetRoundCount() {return m_activity_open_time_cfg_list_count;}
	int GetMonsterInfoCfgCount() { return m_monster_info_cfg_count; }	// 怪物信息配置数量
	int GetFenceCfgCount() { return m_fence_cfg_count; }				// 安全区信息配置数量

	int GetEnterInfo(Posi *pos)
	{
		if(nullptr == pos) return 0;
		*pos = m_other_cfg.role_pos;
		return m_other_cfg.scene_id;
	}

private:
	int InitOtherCfg(TiXmlElement *RootElement);
	int InitActivityOpenTimeCfg(TiXmlElement *RootElement);
	int InitReliveCfg(TiXmlElement *RootElement);
	int InitMonsterInfoCfg(TiXmlElement *RootElement);
	int InitFenceInfoCfg(TiXmlElement *RootElement);
	int InitSkillInfoCfg(TiXmlElement *RootElement);

	bool DayTimeFormatInvalid(int day_time);

	GiftHarvestOtherCfg m_other_cfg;																// 其他配置

	int m_activity_open_time_cfg_list_count;														// 轮次开启时间配置数量
	GiftHarvestRoundOpenTimeInfo m_activity_open_time_cfg_list[GIFT_HARVEST_OPEN_TIME_MAX_COUNT];	// 轮次开启时间配置

	int m_monster_create_pos_list_count;															// 怪物复活点数量
	Posi m_monster_create_pos_list[GIFT_HARVEST_RELIVE_POS_MAX_COUNT];								// 怪物复活点

	int m_monster_info_cfg_count;																	// 怪物配置数量
	GiftHarvestMonsterInfoConfig m_monster_info_cfg_list[GIFT_HARVEST_MONSTER_CFG_MAX_COUNT];		// 怪物信息配置
	
	int m_fence_cfg_count;																			// 围栏区数量
	GiftHarvestFenceConfig m_fence_cfg_list[GIFT_HARVEST_FENCE_CFG_MAX_COUNT];						// 围栏区信息配置

	GiftHarvestSkillConfig m_skill_cfg_list[GIFT_HARVEST_SKILL_CFG_MAX_COUNT];						// 技能信息配置
};

#endif // __CROSS_1V1_COMMON_CONFIG_HPP__

