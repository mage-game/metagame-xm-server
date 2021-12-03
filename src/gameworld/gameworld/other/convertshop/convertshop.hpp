#ifndef __CONVERT_SHOP_HPP__
#define __CONVERT_SHOP_HPP__

#include <string>

#include "protocal/msgshop.h"
#include "item/itembase.h"
#include <map>
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

class Role;

class ConvertShop : public ConfigBase
{
public:
	ConvertShop();
	~ConvertShop();

	enum SCORE_TO_ITEM_TYPE										
	{
		SCORE_TO_ITEM_TYPE_INVALID = 0, 

		SCORE_TO_ITEM_TYPE_HUNYIN,											// 魂印兑换
		SCORE_TO_ITEM_TYPE_NORMAL_ITEM,										// 道具兑换
		SCORE_TO_ITEM_TYPE_TREASURE_HUNT,									// 寻宝兑换		
		SCORE_TO_ITEM_TYPE_EQUIP,											// 装备兑换
		SCORE_TO_ITEM_TYPE_JINGLING,										// 精灵兑换
		SCORE_TO_ITEM_TYPE_HAPPYTREE,										// 欢乐果树兑换
		SCORE_TO_ITEM_TYPE_CS_EQUIP1,										// 装备寻宝商店兑换1		 6仙品 幸运
		SCORE_TO_ITEM_TYPE_MYSTERIOUS_SHOP,									// 神秘商店兑换
		SCORE_TO_ITEM_TYPE_TREASURE,										// 珍宝兑换
		SCORE_TO_ITEM_TYPE_ZHUANZHI_STONE_SCORE,							// 玉石精粹兑换
		SCORE_TO_ITEM_TYPE_HUNJING,											// 魂晶兑换
		SCORE_TO_ITEM_TYPE_GUILDSHOP,										// 仙盟商店
		SCORE_TO_ITEM_TYPE_JUST_USE_ITEM,									// 特殊兑换类型物品换物品

		SCORE_TO_ITEM_TYPE_MAX,
	};

	enum SCORE_TO_ITEM_PRICE_TYPE											// 兑换消耗价格类型
	{
		SCORE_TO_ITEM_PRICE_TYPE_INVALID = 0,

		SCORE_TO_ITEM_PRICE_TYPE_MOJING = 1,								// 魔晶-兑换用
		SCORE_TO_ITEM_PRICE_TYPE_SHENGWANG,									// 声望-兑换用
		SCORE_TO_ITEM_PRICE_TYPE_GONGXUN,									// 功勋-兑换用
		SCORE_TO_ITEM_PRICE_TYPE_WEIWANG,									// 威望-兑换用
		SCORE_TO_ITEM_PRICE_TYPE_TREASURE_HUNT_CREDIT,						// 寻宝积分-兑换用
		SCORE_TO_ITEM_PRICE_TYPE_JINGLING_CREDIT,							// 精灵寻宝积分-兑换用
		SCORE_TO_ITEM_PRICE_TYPE_HAPPY_TREE,								// 欢乐果树成长-兑换用
		SCORE_TO_ITEM_PRICE_TYPE_CROSS_HONOR,								// 跨服荣誉
		SCORE_TO_ITEM_PRICE_TYPE_GUANGHUI,									// 光辉-兑换用
		SCORE_TO_ITEM_PRICE_TYPE_MYSTERIOUS_SHOP,							// 神秘商店
		SCORE_TO_ITEM_PRICE_TYPE_BLUE_LINGZHI,								// 蓝灵值
		SCORE_TO_ITEM_PRICE_TYPE_PURPLE_LINGZHI,							// 紫灵值
		SCORE_TO_ITEM_PRICE_TYPE_ORANGE_LINGZHI,							// 橙灵值
		SCORE_TO_ITEM_PRICE_TYPE_PRECIOUS_BOSS_SCORE,						// 秘藏boss积分
		SCORE_TO_ITEM_PRICE_TYPE_SHENZHOUWEAPON_SCORE,						// 异火积分
		SCORE_TO_ITEM_PRICE_TYPE_TREASURE_HUNT_CREDIT1,						// 寻宝积分1-兑换用
		SCORE_TO_ITEM_PRICE_TYPE_TREASURE_HUNT_CREDIT2,						// 寻宝积分2-兑换用
		SCORE_TO_ITEM_PRICE_TYPE_TREASURE_HUNT_CREDIT3,						// 寻宝积分3-兑换用
		SCORE_TO_ITEM_PRICE_TYPE_ZHUANZHI_STONE_SCORE,						// 转职玉石精粹
		SCORE_TO_ITEM_PRICE_TYPE_HUNJING,									// 魂晶-兑换用
		SCORE_TO_ITEM_PRICE_TYPE_GONGXIAN,									// 仙盟贡献
		SCORE_TO_ITEM_PRICE_TYPE_JUST_USE_ITEM,								// 特殊类型物品换物品

		SCORE_TO_ITEM_PRICE_TYPE_MAX,
	};

	enum CONVERT_REQUIRE_TYPE													// 兑换物品需要满足的条件类型
	{
		CONVERT_REQUIRE_TYPE_INVALID = 0,									
		CONVERT_REQUIRE_TYPE_SUIT_LEVEL,											// 购买者需要满足等级区间

		CONVERT_REQUIRE_TYPE_MAX
	};

	bool Init(const std::string &configname, std::string *err);
	int InitMultipleCost(PugiXmlNode RootElement);
	int InitConverShop(PugiXmlNode RootElement);

	void ConvertScoreToItem(Role *role, Protocol::CSScoreToItemConvert &stic);
	void CheckLimitTimeConvertItem(Role *role, int level);

private:
	bool CheckConvertRequire(Role *role, int convert_type, int conver_index, int convert_num);
	bool CheckConvertReqValid(Role *role, Protocol::CSScoreToItemConvert &stic);
	
	int GetPriceMultipleCost(int multiplecost_id, int times);
	
	struct ConvertShopItem
	{
		ConvertShopItem() : item_id(ItemBase::INVALID_ITEM_ID), is_bind(0), price_type(0), price(0), need_stuff_id(0), need_stuff_count(0), 
			require_type(0), require_value_min(0), require_value_max(0), limit_convert_count(0), multiple_cost_id(0), lifetime_convert_count(0), is_world_notice(0),
			is_guild_notice(0), from_open_game_day(0), to_open_game_day(0), limit_conver_time(0) {}

		ItemID item_id;
		short is_bind;
		int price_type;
		int price;
		int need_stuff_id;
		int need_stuff_count;

		int require_type;
		int require_value_min;
		int require_value_max;
		
		int limit_convert_count;
		int multiple_cost_id;
		int lifetime_convert_count;

		short is_world_notice;
		short is_guild_notice;
		int from_open_game_day;				//从开服第几天开始
		int to_open_game_day;				//持续到开服第几天
		int limit_conver_time;				// 限时兑换时间(从等级开启时算)
	};

	struct ConvertShopItemList
	{
		ConvertShopItemList() : count(0) {}

		int count;
		ConvertShopItem item_list[Protocol::MAX_CONVERT_SCORE_TO_ITEM_NUM];
	};
	ConvertShopItemList m_convert_shop_item_list[SCORE_TO_ITEM_TYPE_MAX];

	struct MultipleCostItem
	{
		MultipleCostItem() : multiple_cost_id(0), times_min(0), times_max(0), price_multile(0) {}

		int multiple_cost_id;
		int times_min;
		int times_max;
		int price_multile;
	};

	static const int MAX_CONVERT_MULTIPLE_NUM = 10;
	struct MultipleCostItemList
	{
		MultipleCostItemList() : multiple_cost_id(0) {}

		int multiple_cost_id;
		MultipleCostItem item_list[MAX_CONVERT_MULTIPLE_NUM];
	};
	std::map<int, MultipleCostItemList> m_multiplecost_map;
};

#endif // __CONVERT_SHOP_HPP__

