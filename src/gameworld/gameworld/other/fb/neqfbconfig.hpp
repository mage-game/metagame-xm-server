#ifndef _NEQFBCFG_HPP_
#define _NEQFBCFG_HPP_

#include "servercommon/vipdef.hpp"
#include "servercommon/fbdef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "gamedef.h"
#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gamelog.h"

#include <map>
#include <vector>

struct NeqFBLevelCfg
{
	static const int MAX_MONSTER_TO_FLUSH = 10;

	NeqFBLevelCfg() : chapter(0), level(0), role_level(0), scene_id(0), pos_x(0), pos_y(0), boss_id(0), sec_3_star(0), sec_2_star(0),
		boss_x(0), boss_y(0), monster_id(0), limit_times(0)
	{}
	int invalid;
	int chapter;
	int level;
	int role_level;
	int scene_id;
	int pos_x;
	int pos_y;
	int boss_id;
	int sec_3_star;
	int sec_2_star;
	int boss_x;
	int boss_y;
	int monster_id;
	int limit_times;
	Posi monster_pos_list[MAX_MONSTER_TO_FLUSH];
	ItemConfigData find_reward[MAX_ATTACHMENT_ITEM_NUM];
};

struct NeqFBStarRewardList
{
	static const int MAX_REWARD_ITEM_COUNT = 4;

	NeqFBStarRewardList() : need_star(0), reward_count(0), add_challenge_count(0) {}

	int need_star;
	int reward_count;
	int add_challenge_count;
	ItemConfigData item_list[MAX_REWARD_ITEM_COUNT];
};

struct NeqRollItemToRole
{
	NeqRollItemToRole() : item_seq(0), i_type(0), weight(0) {}

	ItemConfigData item_cfg;
	short item_seq;
	short i_type;
	int weight;
};
struct NeqFbOtherCfg
{
	NeqFbOtherCfg():auto_item_id(0),auto_item_count(0), day_total_count(0), buy_times_gold(0),
		roll_times_1(0),roll_times_2(0),roll_times_3(0),auto_grid(0),auto_limit(0)
	{}
	ItemID auto_item_id;
	int auto_item_count;
	int day_total_count;
	int buy_times_gold;
	int roll_times_1;
	int roll_times_2;
	int roll_times_3;
	int auto_grid;
	int auto_limit;
};

class NeqFBConfig : public ConfigBase
{
public:
	NeqFBConfig();
	~NeqFBConfig();

	bool Init(const std::string &configname, std::string *err);

	const NeqFBLevelCfg * GetLevelCfg(int chapter, int level) const;
	const NeqFBLevelCfg * GetLevelByScene(int scene_id) const;
	int GetVipBuyTimes(int vip_level) const;
	int GetNonVipBuyTimes(int vip_level) const;
	const NeqFBStarRewardList * GetStarRewardList(int chapter, int seq) const;
	const NeqFbOtherCfg * GetNeqFbOtherCfg() const { return &m_neq_other;}

	ItemID GetAutoItemId() { return m_neq_other.auto_item_id; }
	int GetAutoItemCount() { return m_neq_other.auto_item_count; }
	int GetAutoMinKnapGridCount() { return m_neq_other.auto_grid; }
	int GetAutoLimitStar() { return m_neq_other.auto_limit; }
	int GetDayTotalCount() { return m_neq_other.day_total_count; }
	int GetBuyTimesGold() { return m_neq_other.buy_times_gold; }
	int GetRollTimes(int star);

	bool GetRollPool(NeqRollItemToRole roll_list[NEQFB_ROLLPOOL_TOTAL_COUNT], int chapter, int level, bool is_random_order = true);
	int GetGoldRollCost(int roll_times);

private:
	
	struct NeqRollItem
	{
		NeqRollItem() : min_chapter(0), min_level(0), max_chapter(0), max_level(0), i_type(0), weight(0) {}

		ItemConfigData item_cfg;
		int min_chapter;
		int min_level;
		int max_chapter;
		int max_level;
		int i_type;
		int weight;
	};

	struct RollItemAndWeight
	{
		RollItemAndWeight() : total_weight(0)
		{
			roll_item_vec.clear();
		}
		
		std::vector<NeqRollItem> roll_item_vec;
		int total_weight;
	};

	int InitLevelCfg(PugiXmlNode RootElement);
	int InitBuyCfg(PugiXmlNode RootElement);
	int InitStarRewardCfg(PugiXmlNode RootElement);
	int InitRollCfg(PugiXmlNode RootElement);
	int InitGoldRollCost(PugiXmlNode RootElement);
	int InitOtherCfg(PugiXmlNode RootElement);

	int MakeStarRewardKey(int chapter, int seq) const;
	int MakeChapterLevelKey(int chapter, int level) const;
	
	bool CheckRollItemChapterLevel(const NeqRollItem &tmp_roll_item, int chapter, int level);
	bool CheckRollCfgHelper(int chapter, int level);

	NeqFBLevelCfg m_level_list[NEQFB_MAX_CHAPTER][NEQFB_MAX_LEVEL_PER_CHAPTER];
	std::map<int, NeqFBLevelCfg> m_scene_level_map;
	int m_vip_buy_count_list[MAX_VIP_LEVEL + 1];
	int m_nonvip_buy_count_list[MAX_VIP_LEVEL + 1];
	std::map<int, NeqFBStarRewardList> m_star_reward_map;

	NeqFbOtherCfg m_neq_other;
	static const int MAX_ROLL_ITEM_TYPE = 3;
	std::vector<NeqRollItem> m_roll_list[MAX_ROLL_ITEM_TYPE];
	
	std::map<int, RollItemAndWeight> m_roll_item_map;

	int m_roll_gold_list[NEQFB_MAX_GOLD_ROLL_TIMES + 1];
};

#endif

