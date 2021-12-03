#ifndef MSGSHOP_H
#define MSGSHOP_H

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/basedef.h"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/roleshopdef.hpp"
#include "servercommon/mysteriousshopdef.hpp"
#include "servercommon/struct/converrecordparam.hpp"
#include "servercommon/mysteriousshopinmalldef.hpp"

#include "gamedef.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	/*
		商城
	*/

	class CSShopBuy
	{
	public:
		CSShopBuy();
		MessageHeader		header;

		unsigned short item_id;
		unsigned short item_num;
		char	is_bind;			//	是否是使用绑定元宝购买
		char	if_use;				//  是否直接使用
		char	reserve_ch1;		//	是否是抢购		
		char	reserve_ch2;
	};

	/////////////////////////////////////////////     宝箱商店     /////////////////////////////////////////////
	class CSChestShopRecordList						// 4863获取寻宝记录 1装备寻宝，2装备寻宝, 3装备寻宝, 4精灵寻宝
	{
	public:
		CSChestShopRecordList();
		MessageHeader header;

		short shop_type;
		short reserve;
	};

	class SCChestShopRecordList						// 4864寻宝记录返回
	{
	public:
		SCChestShopRecordList();
		MessageHeader header;

		static const int MAX_RECORD_COUNT = 30;		// 寻宝记录最大数

		struct ListItem
		{
			ListItem() : item_id(0), reserve(0)
			{
				memset(role_name, 0, sizeof(role_name));
			}

			GameName role_name;
			ItemID item_id;
			short reserve;
		};

		short count;
		short record_type;
		ListItem record_list[MAX_RECORD_COUNT];
	};

	class CSGetSelfChestShopItemList				// 4850获取自己宝箱商店物品列表
	{
	public:
		CSGetSelfChestShopItemList();
		MessageHeader header;

		short shop_type;
		short reserve;
	};

	class CSBuyChestShopItem						// 4851宝箱商店购买
	{
	public:
		CSBuyChestShopItem();
		MessageHeader header;

		short shop_type;
		short mode;															
		short reserve_sh1;
		short reserve_sh2;
	};

	class CSFetchChestShopItem						// 4852提取宝箱商店物品
	{
	public:
		CSFetchChestShopItem();
		MessageHeader header;
		
		short shop_type;
		short grid_index;
		short if_fetch_all;
		short reserve;
	};

	class CSChestShopGetFreeInfo					// 4856获取宝箱商店免费时间		
	{
	public:
		CSChestShopGetFreeInfo();
		MessageHeader header;
	};

	class CSChestShopAutoRecycle					// 4858宝箱自动回收
	{
	public:
		CSChestShopAutoRecycle();
		MessageHeader header;

		short shop_type;
		short color;
		short is_auto;
		short gird_index;
	};

	class SCSelfChestShopItemList					// 4800返回自己宝箱商店物品列表
	{
	public:
		SCSelfChestShopItemList();
		MessageHeader header;

		short storehouse_type;						// 仓库类型
		short reserve;
		int count;
		ChestItemInfo item_list[MAX_ROLE_CHEST_GRID];
	};

	class SCChestShopItemListPerBuy					// 4801返回每次开宝箱得到的物品
	{
	public:
		SCChestShopItemListPerBuy();
		MessageHeader header;
		
		short shop_type;
		short mode;
		int count;
		ChestItemInfo item_list[MAX_ROLE_CHEST_GRID];
	};

	class SCChestShopFreeInfo						// 4802发送宝箱免费信息
	{
	public:
		SCChestShopFreeInfo();
		MessageHeader	header;

		unsigned int chest_shop_next_free_time_1;
		unsigned int chest_shop_1_next_free_time_1;
		unsigned int chest_shop_2_next_free_time_1;
		unsigned int chest_shop_jl_next_free_time_1;
	};

	class CSRareChestShopReq						// 4806奇珍异宝抽奖请求
	{
	public:
		CSRareChestShopReq();
		MessageHeader header;

		int times;
	};

	/////////////////////////////////////////////     走棋子     /////////////////////////////////////////////
	class CSMoveChessResetReq						// 5221请求重置
	{
	public:
		CSMoveChessResetReq();
		MessageHeader header;
	};


	class CSMoveChessShakeReq						// 5222请求摇骰子
	{
	public:
		CSMoveChessShakeReq();
		MessageHeader header;

		short is_use_item;							// 1使用物品，0不使用
		short reserve;

	};

	class SCMoveChessShakePoint						// 5224摇骰子摇到点数
	{
	public:
		SCMoveChessShakePoint();
		MessageHeader header;

		int shake_point;
	};

	class CSMoveChessFreeInfo						// 5223获取走棋子信息		
	{
	public:
		CSMoveChessFreeInfo();
		MessageHeader header;
	};

	/////////////////////////////////////////////     兑换商店     /////////////////////////////////////////////
	
	static const int MAX_CONVERT_SCORE_TO_ITEM_NUM = 128;

	class CSScoreToItemConvert								// 4855消耗积分兑换物品请求
	{
	public:
		CSScoreToItemConvert();
		MessageHeader	header;

		int					scoretoitem_type;
		short				index;
		short				num;
	};

	class CSGetConvertRecordInfo							// 4857兑换记录信息请求
	{
	public:
		CSGetConvertRecordInfo();
		MessageHeader	header;
	};

	class SCConvertRecordInfo								// 4803兑换记录信息
	{
	public:
		SCConvertRecordInfo();
		MessageHeader	header;

		struct RecordItem
		{
			RecordItem() { this->Reset(); }

			void Reset()
			{
				reserve = 0;
				convert_type = 0;
				reserve_1 = 0;
				convert_count = 0;
				seq = 0;
				timestamp = 0;
			}

			short reserve;
			char convert_type;
			char reserve_1;
			short convert_count;
			short seq;
			unsigned int timestamp;
		};

		short record_count;
		short lifetime_record_count; 
		int limittime_record_count;
		RecordItem record_list[MAX_CONVERT_RECORD_ITEM_COUNT + MAX_CONVERT_RECORD_ITEM_COUNT + MAX_TIME_CONVERT_RECORD_ITEM_COUNT];
	};

	class CSGetSocreInfoReq									// 4860获取积分数量请求
	{
	public:
		CSGetSocreInfoReq();
		MessageHeader	header;
	};

	class SCSendScoreInfo									// 4861发送积分信息
	{
	public:
		SCSendScoreInfo();
		MessageHeader	header;

		int chest_shop_mojing;
		int chest_shop_shengwang;
		int chest_shop_gongxun;
		int chest_shop_weiwang;
		int chest_shop_treasure_credit;
		int chest_shop_jingling_credit;
		int chest_shop_happytree_grow;
		int chest_shop_mysterious_shop_credit;
		int chest_shop_blue_lingzhi;
		int chest_shop_purple_lingzhi;
		int chest_shop_orange_lingzhi;
		int chest_shop_guanghui;
		int chest_shop_precious_boss_score;
		int chest_shop_shenzhouweapon_score;							// 异火积分
		int chest_shop_treasure_credit1;								// 巅峰寻宝积分
		int chest_shop_treasure_credit2;								// 至尊寻宝积分
		int chest_shop_treasure_credit3;								// 第四种寻宝积分（保留）
		int chest_shop_zhuanzhi_stone_score;							// 转职玉石精粹
		int chest_shop_hunjing;											// 魂晶
	};

	class SCSendScoreInfoNotice									//4862发送积分用于加积分右下角提示
	{
	public:
		SCSendScoreInfoNotice();
		MessageHeader	header;

		int chest_shop_mojing;
		int chest_shop_shengwang;
		int chest_shop_gongxun;
		int chest_shop_weiwang;
		int chest_shop_treasure_credit;
		int chest_shop_jingling_credit;
		int chest_shop_happytree_grow;
		int chest_mysterious_shop_credit;
		int chest_shop_blue_lingzhi;
		int chest_shop_purple_lingzhi;
		int chest_shop_orange_lingzhi;
		int chest_shop_guanghui;
		int chest_shop_precious_boss_score;
		int chest_shop_shenzhouweapon_score;
		int chest_shop_treasure_credit1;								// 巅峰寻宝积分
		int chest_shop_treasure_credit2;								// 至尊寻宝积分
		int chest_shop_treasure_credit3;								// 第四种寻宝积分（保留）
		int chest_shop_zhuanzhi_stone_score;							// 转职玉石精粹 
		int chest_shop_hunjing;											// 魂晶
	};

	/////////////////////////////////////////////     神秘商人     /////////////////////////////////////////////
	enum MYSTERIOUSSHOP_OPERATE_TYPE
	{
		MYSTERIOUSSHOP_OPERATE_TYPE_REQINFO = 0,
		MYSTERIOUSSHOP_OPERATE_TYPE_BUY = 1,
	};

	class CSMysteriosshopOperate
	{
	public:
		CSMysteriosshopOperate();
		MessageHeader		header;

		int operate_type;
		int param1;
		int param2;
	};

	static const int MAX_MYSTERIOUS_SALEITEM_NUM = 9;

	class SCMysteriosNpcRefresh
	{
	public:
		SCMysteriosNpcRefresh();
		MessageHeader	header;

		int disappeartime;
		int nextrefreshtime;
		int npc_sceneid;
		int npc_x;
		int npc_y;
		
	};

	class SCMysteriosshopInfo
	{
	public:
		SCMysteriosshopInfo();
		MessageHeader	header;

		struct SaleItem
		{
			SaleItem() : seq(0), buynum(0) {}

			int seq;
			int buynum;
		};

		SaleItem saleitem_list[MAX_MYSTERIOUS_SALEITEM_NUM];
	};

	/////////////////////////////////////////////     神秘商店     /////////////////////////////////////////////
	enum MYSTERIOUSSHOP_IN_MALL_OPERATE_TYPE
	{
		MYSTERIOUSSHOP_IN_MALL_OPERATE_TYPE_MONEY = 0,			// 神秘商店购买物品 p1:购买物品的索引
		MYSTERIOUSSHOP_IN_MALL_OPERATE_TYPE_REFRESH = 1,		// 刷新神秘商店物品 p1:0为第一个格子刷新 1为全部刷新
		MYSTERIOUSSHOP_IN_MALL_OPERATE_INFO_REQ = 2,			// 神秘商店物品信息 
		MYSTERIOUSSHOP_IN_MALL_OPERATE_OPEN_VIEW = 3,           // 客户端重置提醒标记
	};

	class CSMysteriosshopinMallOperate					// 4807
	{
	public:
		CSMysteriosshopinMallOperate();
		MessageHeader		header;

		int operate_type;								// 操作类型
		int param1;										// 
	};

	class SCSendMysteriosshopItemInfo									// 4808
	{
	public:
		SCSendMysteriosshopItemInfo();
		MessageHeader		header;

		unsigned int next_shop_item_refresh_time;							// 下次商品刷新时间
		int item_count;														// 物品总数
		char client_remind_flag;                                            // 客户端红点标记，特殊处理需要服务端记录
		char today_free_count;												// 今日免费次数
		short reserve_sh;													// 

		MysteriousShopInMallItemInfo item[MYSTERIOUSSHOP_ITEM_MAX_COUNT];	// 物品列表
	};

	class CSDiscountShopBuy					// 8950
	{
	public:
		CSDiscountShopBuy();
		MessageHeader		header;

		int seq;										
		int num;										 
	};

	class SCSendDiscounthopItemInfo									// 8951
	{
	public:
		SCSendDiscounthopItemInfo();
		MessageHeader		header;

		struct DiscountItem
		{
			DiscountItem() : seq(0), today_buy_count(0) {}

			int seq;
			int today_buy_count;		//今天已购买数量
		};

		int item_count;														// 物品总数
		int today_refresh_level;
		DiscountItem item_list[DISCOUNTSHOP_ITEM_MAX_COUNT];
	};
}

#pragma pack(pop)

#endif


