/***************************************************************************
FileName: msgscpublicsale.h
Author: lzj
Description: 拍卖 相关协议 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
lzj		   2011/08/12/ 18:53:05     1.0           创建
**************************************************************************/

#ifndef __MSG_SC_PUBLICSALE_HPP__
#define __MSG_SC_PUBLICSALE_HPP__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/serverdef.h"
#include "servercommon/struct/global/publicsaleparam.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/tradedef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCAddPublicSaleItemAck
	{
	public:
		SCAddPublicSaleItemAck();

		MessageHeader header;
		
		int ret;
		int sale_index;
	};

	class SCRemovePublicSaleItemAck
	{
	public:
		SCRemovePublicSaleItemAck();

		MessageHeader header;

		int ret;
		int sale_index;
	};

	class SCBuyPublicSaleItemAck
	{
	public:
		SCBuyPublicSaleItemAck();

		MessageHeader header;

		int ret;
		int seller_uid;		
		int sale_index;
	};

	class SCGetPublicSaleItemListAck
	{
	public:
		SCGetPublicSaleItemListAck();

		MessageHeader header;

		int count;
		SaleItemParam sale_item_list[PUBLICSALE_MAX_ITEM_COUNT];
	};

	static const int FUZZY_SEARCH_ITEM_TYPE_COUNT = 24;
	static const int FUZZY_SEARCH_ITEM_ID_COUNT = 256;
	static const int SEARCH_COND_LEVEL_INTERVAL = 10;
	static const int PUBLICSALE_PAGE_ITEM_COUNT_MAX = 20;

	struct FuzzySearch
	{
		int item_sale_type;
		int item_count;
		int item_id_list[FUZZY_SEARCH_ITEM_ID_COUNT];
	}; 
	
	// 搜索这种无需经过Gameworld 虽然有点不一致 如果从Gameworld过来 解包再打包很麻烦 
	class CSPublicSaleSearch
	{
	public:
		CSPublicSaleSearch();

		MessageHeader header;
		
		int item_type;													// 指定类型 0为不指定
		short level;													// 指定等级 0为不指定 
		short level_interval;											// 等级区间
		short prof;														// 指定职业 0为不指定
		short color;													// 指定颜色 0为不指定
		short color_interval;											// 颜色区间
		short order;													// 指定阶数 0为不指定
		short order_interval;											// 阶数区间
		short page_item_count;											// 一页几个物品
		int req_page;													// 请求第几页 
		int total_page;													// 总页数 客户端没有总页数的时候 要填成0 如果已经有总页数 填上总页数 
		
		int fuzzy_type_count;											// 模糊搜索相关组个数
		FuzzySearch fuzzy_type_list[FUZZY_SEARCH_ITEM_TYPE_COUNT];
	};

	class SCPublicSaleSearchAck
	{
	public:
		SCPublicSaleSearchAck();

		MessageHeader header;

		struct SaleItem
		{
			int seller_uid;
			GameName seller_name;

			SaleItemParam item_param;
		};

		int cur_page;
		int total_page;
		int count;
		SaleItem saleitem_list[PUBLICSALE_PAGE_ITEM_COUNT_MAX];
	};

	class SCPublicSaleTypeCountAck
	{
	public:
		SCPublicSaleTypeCountAck();
		MessageHeader			header;

		static const int SALE_TYPE_COUNT_MAX = 100;
		struct ItemInfo
		{
			ItemInfo() : sale_type(0), item_count(0) {}
			int sale_type;
			int item_count;
		};

		int count;
		ItemInfo info_list[SALE_TYPE_COUNT_MAX];
	};
}

#pragma pack(pop)

#endif // __MSG_SC_PUBLICSALE_HPP__


