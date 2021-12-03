#ifndef __MSG_ELEMENT_HEART_HPP__
#define __MSG_ELEMENT_HEART_HPP__

#include "servercommon/elementheartdef.hpp"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/shenzhuangdef.hpp"
#include "servercommon/struct/itemlistparam.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum ELEMENT_HEART_REQ_TYPE											// 操作请求
	{
		ELEMENT_HEART_REQ_TYPE_ACTIVE_GHOST,							// 激活元素之心 param1 元素之心id
		ELEMENT_HEART_REQ_TYPE_CHANGE_GHOST_WUXING_TYPE,				// 改变元素之心五行 param1 元素之心id
		ELEMENT_HEART_REQ_TYPE_FEED_ELEMENT,							// 喂养元素之心	param1 元素之心id param2 虚拟物品id param3 物品数量
		ELEMENT_HEART_REQ_TYPE_UPGRADE_GHOST,							// 元素之心进阶	param1 元素之心id param2 是否一键  param3 是否自动购买
		ELEMENT_HEART_REQ_TYPE_GET_PRODUCT,								// 元素之心采集	param1 元素之心id 
		ELEMENT_HEART_REQ_TYPE_PRODUCT_UP_SEED,							// 元素之心产出加速	param1 元素之心id 
		ELEMENT_HEART_REQ_TYPE_UPGRADE_CHARM,							// 元素之纹升级 para1 升级元素之纹下标 param2 消耗格子下标
		ELEMENT_HEART_REQ_TYPE_ALL_INFO,								// 请求所有信息
		ELEMENT_HEART_REQ_TYPE_CHOUJIANG,								// 元素之心抽奖 param1 次数
		ELEMENT_HEART_REQ_TYPE_FEED_GHOST_ONE_KEY,						// 一键喂养元素之心	param1 id
		ELEMENT_HEART_REQ_TYPE_SET_GHOST_WUXING_TYPE,					// 设置元素之心类型	param1 id
		ELEMENT_HEART_REQ_TYPE_SHOP_REFRSH,								// 商店刷新 param 1是否使用积分刷新
		ELEMENT_HEART_REQ_TYPE_SHOP_BUY,								// 商城购买 param 1 商品seq
		ELEMENT_HEART_REQ_TYPE_XILIAN,									// 洗练 param1 元素id， param2 锁洗标志 param3洗练颜色、 param4 是否自动购买
		ELEMENT_HEART_REQ_TYPE_PUTON_EQUIP,								// 穿装备 param1元素id param2装备格子
		ELEMENT_HEART_REQ_TYPE_UPGRADE_EQUIP,							// 装备升级 Parma1 元素id param2 是否一键升级
		ELEMENT_HEART_REQ_TYPE_EQUIP_RECYCLE,							// 装备分解 param1 背包索引 param 2 消耗数量
	};

	class CSElementHeartReq						// 8454 操作请求
	{
	public:
		CSElementHeartReq();
		MessageHeader header;

		int info_type;						// 对应 ELEMENT_HEART_REQ_TYPE 类型
		int param1;
		int param2;
		int param3;
		int param4;
	};

	class SCElementHeartInfo					// 元素之心信息	8455											
	{
	public:
		enum INFO_TYPE
		{
			INFO_TYPE_SINGLE_ELEMENT,			// 单个信息
			INFO_TYPE_ALL_ELEMENT,				// 全部信息
			INFO_TYPE_WUXING_CHANGE,			// 五行转换信息
		};
	
		SCElementHeartInfo();
		MessageHeader header;

		int pasture_score;
		char info_type;	
		char free_chou_times;
		short count;
		ElementSingle element_heart_list[ELEMENT_HEART_MAX_COUNT]; // 元素之心列表
	};
		
	class SCElementTextureInfo						// 元素之纹列表信息 8456
	{
	public:
		SCElementTextureInfo();
		MessageHeader header;
		
		ElementTextureParam charm_list[SHENZHUANG_MAX_PART];
	};
	
	class SCElemnetHeartSingleCharmInfo				// 单个元素之纹信息 8457
	{
	public:
		SCElemnetHeartSingleCharmInfo();
		MessageHeader header;

		int e_index;								// 元素之纹所在装备下标
		ElementTextureParam charm_info;				// 元素之纹信息
	};

	struct ElementHeartChouRewardParam
	{
		short v_item_id;
		short num;
		char is_bind;
		char reserve_ch;
		short reserve_sh;
	};

	struct ElementChouRewardItem
	{
		ItemID item_id;
		char num;
		char is_bind;
	};

	class SCElementHeartChouRewardListInfo			// 抽奖奖品	8458										
	{
	public:
		SCElementHeartChouRewardListInfo();
		MessageHeader header;

		short free_chou_times;
		short count;
		ElementChouRewardItem reward_list[ELEMENT_HEART_MAX_CHOU_TIMES_MAX];
	};

	class SCElementProductListInfo				// 产出列表	8459								
	{
	public:
		const static int MAX_PRODUCT_LIST_COUNT = 10;
		UNSTD_STATIC_CHECK(MAX_PRODUCT_LIST_COUNT % 2 == 0)

		SCElementProductListInfo();
		MessageHeader header;

		short info_type;
		short count;
		unsigned short product_list[MAX_PRODUCT_LIST_COUNT];
	};

	class SCElementShopInfo															// 商店信息  8460
	{
	public:
		SCElementShopInfo();
		MessageHeader header;

		unsigned int next_refresh_timestamp;
		short reserve_sh;
		short today_shop_flush_times;												// 当天商店刷新次数
		ElementHeartShopItem shop_item_list[ELEMENT_SHOP_ITEM_COUNT];				// 商店当前刷新出来的物品列表
	};

	class SCElementXiLianSingleInfo			// 8461
	{
	public:
		SCElementXiLianSingleInfo();
		MessageHeader header;

		int element_id;
		ElementHeartXiLianParam xilian_info;
	};

	class SCElementXiLianAllInfo			// 8462
	{
	public:
		SCElementXiLianAllInfo();
		MessageHeader header;

		ElementHeartXiLianParam xilian_list_info[ELEMENT_HEART_MAX_COUNT];
	};
}

#pragma pack(pop)

#endif	//__MSG_ELEMENT_HEART_HPP__
