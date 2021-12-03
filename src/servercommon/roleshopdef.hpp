#ifndef __ROLESHOP_DEF_HPP__
#define __ROLESHOP_DEF_HPP__

#include <string.h>
#include "servercommon/servercommon.h"

static const int MAX_GUILD_SHOP_ITEM_NUM = 30;

#pragma pack(push, 4)

// 商店类型
enum CHEST_SHOP_TYPE
{
	CHEST_SHOP_TYPE_INVALID = 0,
	CHEST_SHOP_TYPE_EQUIP = 1,							// 装备寻宝
	CHEST_SHOP_TYPE_EQUIP1 = 2,							// 装备寻宝1
	CHEST_SHOP_TYPE_EQUIP2 = 3,							// 装备寻宝2
	CHEST_SHOP_TYPE_JINGLING = 4,						// 精灵
	CHEST_SHOP_TYPE_EQUIP3 = 5,							// 装备寻宝3
	CHEST_SHOP_TYPE_SHENZHOUWEAPON = 6,					// 异火寻宝

	CHEST_SHOP_TYPE_MAX,
};

struct RoleGuildShopParam
{
	RoleGuildShopParam() { this->Reset(); }

	void Reset()
	{
		memset(guild_shop_buy_num_list, 0, sizeof(guild_shop_buy_num_list));
	}

	short guild_shop_buy_num_list[MAX_GUILD_SHOP_ITEM_NUM];

};

struct RoleShopParam
{
	RoleShopParam() { this->Reset(); }

	void Reset()
	{
		chest_shop_next_free_time_1 = 0;
		chest_shop_1_next_free_time_1 = 0;
		chest_shop_2_next_free_time_1 = 0;
		chest_shop_jl_next_free_time_1 = 0;
		chest_shop_mojing = 0;
		chest_shop_shengwang = 0;
		chest_shop_gongxun = 0;
		chest_shop_weiwang = 0;
		chest_shop_treasure_credit = 0;
		chest_shop_jingling_credit = 0;
		chest_shop_happytree_grow = 0;
		first_equip_is_behited_flag = 0;
		first_jingling_is_behited = 0;
		chest_shop_mysterious_credit = 0;
		chest_shop_guanghui = 0;

		chest_shop_bule_lingzhi = 0;
		chest_shop_purple_linzhi = 0;
		chest_shop_orange_linzhi = 0;

		chest_shop_precious_score = 0;
		chest_shop_shenzhouweapon_score = 0;
		chest_shop_treasure_credit1 = 0;
		chest_shop_treasure_credit2 = 0;
		chest_shop_treasure_credit3 = 0;
		chest_shop_hunjing = 0;

		cross_equip_chuanshi_score = 0;
	}

	unsigned int chest_shop_next_free_time_1;						// 装备寻宝下一次免费时间戳
	unsigned int chest_shop_1_next_free_time_1;						// 装备寻宝1下一次免费时间戳
	unsigned int chest_shop_2_next_free_time_1;						// 装备寻宝2下一次免费时间戳
	unsigned int chest_shop_jl_next_free_time_1;
	int chest_shop_mojing;
	int chest_shop_shengwang;
	int chest_shop_gongxun;
	int chest_shop_weiwang;
	int chest_shop_treasure_credit;									// 极品寻宝积分
	int chest_shop_jingling_credit;
	int chest_shop_happytree_grow;
	short first_equip_is_behited_flag;
	short first_jingling_is_behited;
	int chest_shop_mysterious_credit;
	int chest_shop_guanghui;

	int chest_shop_bule_lingzhi;
	int chest_shop_purple_linzhi;
	int chest_shop_orange_linzhi;

	int chest_shop_precious_score;
	int chest_shop_shenzhouweapon_score;							// 异火积分
	int chest_shop_treasure_credit1;								// 巅峰寻宝积分
	int chest_shop_treasure_credit2;								// 至尊寻宝积分
	int chest_shop_treasure_credit3;								// 第四种寻宝积分（保留）
	int chest_shop_hunjing;											// 魂晶
	int cross_equip_chuanshi_score;									// 斗气装备积分
};


// 仓库类型
enum CHEST_SHOP_STOREHOUSE_TYPE
{
	CHEST_SHOP_STOREHOUSE_TYPE_INVALID = 0,
	CHEST_SHOP_STOREHOUSE_TYPE_EQUIP = 1,							// 装备寻宝仓库
	CHEST_SHOP_STOREHOUSE_TYPE_JINGLING = 2,						// 精灵仓库

	CHEST_SHOP_STOREHOUSE_TYPE_MAX,
};

static const int MAX_CHEST_XIANPIN_NUM = 6;
struct ChestItemInfo
{
	ChestItemInfo()
	{
		this->Reset();
	}
	void Reset()
	{
		item_id = 0;
		num = 0;
		is_bind = 0;
		shop_type = 0;
		index = 0;
		has_lucky = 0;
		strength = 0;
		reserve2 = 0;
		reserve3 = 0;
		memset(xianpin_type_list, 0, sizeof(xianpin_type_list));
	}

	UInt16 item_id;
	short num;
	char is_bind;
	char shop_type;
	short index;

	short has_lucky;
	short strength;

	char reserve2;
	char reserve3;
	char xianpin_type_list[MAX_CHEST_XIANPIN_NUM];	//6

	UNSTD_STATIC_CHECK(MAX_CHEST_XIANPIN_NUM == 6);
};

static const int MAX_ROLE_CHEST_GRID = 640;
static const int MAX_ROLE_CHEST_GRID_HALF = 320;

UNSTD_STATIC_CHECK(MAX_ROLE_CHEST_GRID_HALF * 2 == MAX_ROLE_CHEST_GRID);

struct ChestShopNewsItem
{
	int news_id;
	int role_id;
	GameName role_name;
	int role_camp;
	int count;
	ChestItemInfo item_list[MAX_ROLE_CHEST_GRID];
};

struct MysteriousShopNewsItem
{
	int news_id;
	int role_id;
	GameName role_name;
	int role_camp;
	int buy_item_id;
};

static const int MAX_SUPPLY_ITEM_COUNT = 2;

enum SUPPLY_TYPE
{
	SUPPLY_TYPE_ROLE_HP = 0,
	SUPPLY_TYPE_ROLE_TILI,	

	SUPPLY_TYPE_MAX,
};

struct RoleSupplyItem
{
	RoleSupplyItem() { this->Reset(); }

	void Reset()
	{
		supply_left_value = 0;
		supply_range_per = 90;
		reserved1 = 0;
		last_supply_time = 0;
		reserved2 = 0;
	}

	long long supply_left_value;
	short supply_range_per;
	short reserved1;
	unsigned int last_supply_time;
	int reserved2;
};

struct RoleSupplyParam
{
	RoleSupplyParam() { this->Reset(); }

	void Reset()
	{
		for (int i = 0; i < SUPPLY_TYPE_MAX; ++ i)
		{
			supply_list[i].Reset();
		}
	}

	RoleSupplyItem supply_list[SUPPLY_TYPE_MAX];
};

typedef char RoleGuildShopParamHex[sizeof(RoleGuildShopParam) * 2 + 1];
typedef char RoleShopParamHex[sizeof(RoleShopParam) * 2 + 1];
typedef char RoleSupplyParamHex[sizeof(RoleSupplyParam) * 2 + 1];

UNSTD_STATIC_CHECK(sizeof(RoleGuildShopParamHex) < 128);
UNSTD_STATIC_CHECK(sizeof(RoleShopParamHex) < 256);
UNSTD_STATIC_CHECK(sizeof(RoleSupplyParamHex) < 128);

#pragma pack(pop)

#endif // __ROLESHOP_DEF_HPP__

