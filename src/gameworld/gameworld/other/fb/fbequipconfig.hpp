#ifndef _FBEQUIPCONFIG_HPP_
#define _FBEQUIPCONFIG_HPP_

#include "servercommon/fbdef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "gamedef.h"

#include <map>
#include "servercommon/configbase.h"

// 组队爬塔的配置 F-副本-组队爬塔.xls

struct FBEquipJumpLayerCfg
{
	FBEquipJumpLayerCfg() : target_layer(0), limit_time(0), enter_pos(0, 0) {}

	bool CanJump() const { return target_layer > 0; } 

	int target_layer;											// 跳层 目标层
	int limit_time;												// 跳层 限时
	ItemConfigData gift_item;									// 跳层礼包
	Posi enter_pos;												// 目标层入口坐标
};

struct FBEquipLayerCfg
{
	FBEquipLayerCfg() : layer(0), scene_id(0), is_mystery(true), show_layer(0), pass_exp(0), is_pass_notice(false)
	{
		dropid_list.clear();
	}

	int layer;													// 层
	int scene_id;												// 场景ID
	bool is_mystery;											// 是否是神秘层
	int show_layer;												// 显示层
	long long pass_exp;												// 通关经验
	bool is_pass_notice;
	FBEquipJumpLayerCfg jump_layer_cfg;							// 跳层配置

	std::vector<UInt16> dropid_list;							// 掉落ID列表(给玩家跳层之前的层数的奖励)
};

struct FBEquipMysteryShopItem
{
	FBEquipMysteryShopItem() : mystery_layer(0), gold_price(0), buy_limit(0) {}

	int mystery_layer;											// 对应的神秘层
	ItemConfigData item_cfg_data;								// 商品对应的物品信息
	int gold_price;												// 价格 非绑元宝
	int buy_limit;												// 限购
};

struct FBEquipOtherCfg
{
	FBEquipOtherCfg() : team_double_cost_gold(0), trial_task_id(0), 
		guild_add_gongji_percent(0), join_times(0), kick_time_s(0),
		buy_double_reward_need_gold(0), buy_triple_reward_need_gold(0), double_reward_need_vip_level(0), triple_reward_need_vip_level(0){}

	int team_double_cost_gold;									// 组队双倍消耗元宝
	int trial_task_id;											// 新手引导 任务id
	int guild_add_gongji_percent;								// 仙盟加攻击百分比
	int join_times;												// 参与次数
	int kick_time_s;                                            // 踢出时间
	int buy_double_reward_need_gold;
	int buy_triple_reward_need_gold;
	int double_reward_need_vip_level;
	int triple_reward_need_vip_level;

	std::vector<UInt16> dropid_list;							// 额外奖励
};

class TiXmlElement;

class FBEquipConfig : public ConfigBase
{
public:
	FBEquipConfig();
	~FBEquipConfig();

	bool Init(const std::string &configname, std::string *err);

	const FBEquipOtherCfg & GetOtherCfg() { return m_other_cfg; }

	const FBEquipLayerCfg * GetTeamLayerCfgByLayer(int layer) const;
	const FBEquipLayerCfg * GetTeamLayerCfgBySceneID(int scene_id) const;
	const FBEquipJumpLayerCfg * GetTeamJumpCfgByLayer(int layer) const;
	const FBEquipJumpLayerCfg * GetTeamJumpCfgBySceneID(int scene_id) const;
	const FBEquipMysteryShopItem * GetTeamMysteryShopCfg(int shop_item_seq) const;
	int GetTeamSceneIDByLayer(int layer);
	int GetTeamLayerBySceneID(int scene_id);
	const int GetTeamMaxLayer() { return m_team_max_layer; }
	
private:
	int InitOtherCfg(TiXmlElement *RootElement);
	int InitTeamLayerCfg(TiXmlElement *RootElement);
	int InitTeamMysteryShopCfg(TiXmlElement *RootElement);

	// 其他配置
	FBEquipOtherCfg m_other_cfg;

	// 组队 层与场景对应关系
	std::map<int, FBEquipLayerCfg> m_team_scene_to_layer_map;
	int m_team_max_layer;
	FBEquipLayerCfg m_team_layer_cfg[FB_EQUIP_MAX_LAYER_ID + 1];

	// 组队 神秘层商店
	FBEquipMysteryShopItem m_team_mystery_shop_list[FB_EQUIP_MAX_GOODS_SEQ];

};

#endif
