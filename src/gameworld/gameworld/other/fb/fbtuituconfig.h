#pragma once
#ifndef __FB_TUITU_CONFIG_H__
#define __FB_TUITU_CONFIG_H__
#include <map>
#include "common/estring.h"
#include "servercommon/fbdef.hpp"
#include "servercommon/maildef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

static const int TUITU_FB_GUANQIA_MONSTER_NUM_MAX = 20;							/// 推图副本关卡怪物最大数

struct TuituFbOtherConfig
{
	TuituFbOtherConfig() : normal_free_join_times(0), hard_free_join_times(0), normal_buy_times_need_gold(0), hard_buy_times_need_gold(0), saodang_quan_itemid(0),
		inchurt_per(0), mianshang_per(0){}

	int normal_free_join_times;
	int hard_free_join_times;
	int normal_buy_times_need_gold;
	int hard_buy_times_need_gold;
	ItemID saodang_quan_itemid;
	int inchurt_per;
	int mianshang_per;
};

struct TuituFbInfoConfig
{
	struct MonsterInfo
	{
		MonsterInfo() : monster_id(0) {}

		int monster_id;
		Posi pos;
	};

	struct Layer
	{
		Layer() : enter_level_limit(0), need_pass_fb_type(0), need_pass_chapter(0), need_pass_level(0), saodang_star_num(0), time_limit_1_star(0), time_limit_2_star(0),
			time_limit_3_star(0), capability(0), scene_id(0), monster_count(0), item_count(0){}

		int enter_level_limit;
		int need_pass_fb_type;
		int need_pass_chapter;
		int need_pass_level;
		int saodang_star_num;
		int time_limit_1_star;
		int time_limit_2_star;
		int time_limit_3_star;
		int capability;
		int scene_id;
		Posi enter_pos;
		int monster_count;
		MonsterInfo monster_list[TUITU_FB_GUANQIA_MONSTER_NUM_MAX];

		int item_count;
		ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
	};

	struct Chapter
	{
		Chapter() : max_level(-1) {}

		int max_level;
		Layer  layer_cfg_list[TUITU_FB_LEVLE_NUM_MAX];
	};

	TuituFbInfoConfig() : max_chapter(-1) {}

	int max_chapter;
	Chapter chapter_cfg_list[TUITU_FB_CHAPER_NUM_MAX];
};

struct TuituFbStarRewardConfig
{
	struct ItemCfg
	{
		ItemCfg() : need_star(0), item_count(0) {}

		int seq;
		int need_star;
		int item_count;
		ItemConfigData reward_list[MAX_ATTACHMENT_ITEM_NUM];
	};

	TuituFbStarRewardConfig() : chapter(0), item_cfg_count(0) {}

	int chapter;
	int item_cfg_count;
	ItemCfg item_cfg_list[TUITU_FB_CHAPTER_STAR_REWARD_NUM_MAX];
};

class TuituFbConfig : public ConfigBase
{
public:
	TuituFbConfig();
	~TuituFbConfig();

	bool Init(const std::string &path, std::string *err);

	const TuituFbOtherConfig & GetOtherCfg() { return m_other_cfg; }
	const TuituFbInfoConfig * GetFbinfoCfg(int fb_type);
	const TuituFbInfoConfig::Layer * GetLayerCfg(int fb_type, int chapter, int level);
	const TuituFbStarRewardConfig::ItemCfg * GetStarRewardCfg(int fb_type, int chapter, int seq);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitFbInfoCfg(PugiXmlNode RootElement);
	int InitStarRewardCfg(PugiXmlNode RootElement);

	TuituFbOtherConfig m_other_cfg;
	TuituFbInfoConfig m_fb_info_cfg_list[TUTUI_FB_TYPE_NUM_MAX];
	TuituFbStarRewardConfig m_star_reward_cfg_list[TUTUI_FB_TYPE_NUM_MAX][TUITU_FB_CHAPER_NUM_MAX];
};

#endif