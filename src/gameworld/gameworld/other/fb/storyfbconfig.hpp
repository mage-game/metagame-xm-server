#ifndef __STORYFB_CONFIG_HPP__
#define __STORYFB_CONFIG_HPP__

#include "servercommon/fbdef.hpp"
#include "servercommon/maildef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gamedef.h"

struct StoryFBLevelCfg
{
	static const int MONSTER_COUNT_MAX = 20;						// 最大刷怪数量

	struct ConfigItem
	{
		ConfigItem() : fb_index(0), role_level(0), scene_id(0), enter_pos(0, 0),
			free_times(0), reset_gold(0), reward_exp(0), monster_count(0)
		{
			memset(monster_id_list, 0, sizeof(monster_id_list));
			for (int i = 0; i < MONSTER_COUNT_MAX; i++)
			{
				monster_pos_list[i].Init(0, 0);
			}
		}

		int fb_index;
		int fb_level;
		int role_level;
		int scene_id;
		Posi enter_pos;
		int free_times;
		int reset_gold;
		long long reward_exp;
		
		// 奖励物品不能超过邮件附件数量
		ItemConfigData first_reward_list[MAX_ATTACHMENT_ITEM_NUM];	// 首通奖励
		ItemConfigData normal_reward_list[MAX_ATTACHMENT_ITEM_NUM];	// 普通奖励
		ItemConfigData reset_reward_list[MAX_ATTACHMENT_ITEM_NUM];	// 重置奖励

		int monster_count;

		int monster_id_list[MONSTER_COUNT_MAX];
		Posi monster_pos_list[MONSTER_COUNT_MAX];					// 刷怪点
	};

	ConfigItem item_list[FB_STORY_MAX_COUNT];
};

class StoryFBConfig : public ConfigBase
{
public:
	StoryFBConfig();
	~StoryFBConfig();

	bool Init(const std::string &configname, std::string *err);

	const StoryFBLevelCfg::ConfigItem * GetLevelCfg(int fb_index) const;

private:
	int InitLevelCfg(PugiXmlNode RootElement);

	StoryFBLevelCfg m_level_cfg;
};

#endif // __STORYFB_CONFIG_HPP__

