#ifndef _PHASEFBCFG_HPP_
#define _PHASEFBCFG_HPP_

#include "servercommon/fbdef.hpp"
#include "servercommon/maildef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gamedef.h"

enum PHASE_FB_TYPE
{
	PHASE_FB_TYPE_NONE = 0,
	PHASE_FB_TYPE_MOUNT =1,
	PHASE_FB_TYPE_SHIZHUANG_BODY = 2,
	PHASE_FB_TYPE_SHIZHUANG_WUQI = 3,
	PHASE_FB_TYPE_LINGTONG = 4,
	PHASE_FB_TYPE_FIGHT_MOUNT = 5,
	PHASE_FB_TYPE_LINGGONG = 6,
	PHASE_FB_TYPE_LINGQI = 7,
};

static const int MAX_PHASE_RESET_COUNT_CFG = 10;
struct PhaseLevelCfg
{
	static const int MONSTER_COUNT_MAX = 20;						// 最大刷怪数量

	struct ConfigItem
	{
		ConfigItem() : fb_index(0), fb_type(0), fb_level(0), open_task_id(0), role_level(0), auto_level(0), scene_id(0), enter_pos(0, 0),
			free_times(0), reset_gold(0), reward_exp(0), monster_count(0)
		{
			memset(monster_id_list, 0, sizeof(monster_id_list));
			for (int i = 0; i < MONSTER_COUNT_MAX; i++)
			{
				monster_pos_list[i].Init(0, 0);
			}
		}

		int fb_index;		// 大关卡索引
		int fb_type;
		int fb_level;		// 小关卡
		int open_task_id;
		int role_level;
		int auto_level;
		int scene_id;
		Posi enter_pos;
		int free_times;
		int reset_gold;
		long long reward_exp;
		

		// 奖励物品不能超过邮件附件数量
		ItemConfigData first_reward_list[MAX_ATTACHMENT_ITEM_NUM];	// 首通奖励
		//ItemConfigData normal_reward_list[MAX_ATTACHMENT_ITEM_NUM];	// 普通奖励
		ItemConfigData reset_reward_list[MAX_ATTACHMENT_ITEM_NUM];	// 重置奖励

		int monster_count;

		int monster_id_list[MONSTER_COUNT_MAX];
		Posi monster_pos_list[MONSTER_COUNT_MAX];					// 刷怪点
	};

	ConfigItem item_list[FB_PHASE_MAX_COUNT][FB_PHASE_MAX_LEVEL];
};


class PhaseFBConfig : public ConfigBase
{
public:
	PhaseFBConfig();
	~PhaseFBConfig();

	bool Init(const std::string &configname, std::string *err);

	const PhaseLevelCfg::ConfigItem * GetLevelCfg(int fb_index, int fb_level) const;
	int GetLevelLimit(int fb_type, int fb_level);
	const int GetPhaseResetGold(int fb_type);

private:
	int InitLevelCfg(PugiXmlNode RootElement);
	int InitPhaseResetCfg(PugiXmlNode RootElement);

	PhaseLevelCfg m_level_cfg;
	//PhaseFbResetCfg m_phase_reset_cfg[FB_PHASE_MAX_COUNT][MAX_PHASE_RESET_COUNT_CFG];
	int m_phase_reset_cfg[FB_PHASE_MAX_COUNT];  // 购买次数消耗
};

#endif

