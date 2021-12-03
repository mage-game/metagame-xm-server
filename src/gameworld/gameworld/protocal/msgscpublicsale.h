/***************************************************************************
FileName: msgscpublicsale.h
Author: lzj
Description: ���� ���Э�� 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
lzj		   2011/08/12/ 18:53:05     1.0           ����
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
	
	// �����������辭��Gameworld ��Ȼ�е㲻һ�� �����Gameworld���� ����ٴ�����鷳 
	class CSPublicSaleSearch
	{
	public:
		CSPublicSaleSearch();

		MessageHeader header;
		
		int item_type;													// ָ������ 0Ϊ��ָ��
		short level;													// ָ���ȼ� 0Ϊ��ָ�� 
		short level_interval;											// �ȼ�����
		short prof;														// ָ��ְҵ 0Ϊ��ָ��
		short color;													// ָ����ɫ 0Ϊ��ָ��
		short color_interval;											// ��ɫ����
		short order;													// ָ������ 0Ϊ��ָ��
		short order_interval;											// ��������
		short page_item_count;											// һҳ������Ʒ
		int req_page;													// ����ڼ�ҳ 
		int total_page;													// ��ҳ�� �ͻ���û����ҳ����ʱ�� Ҫ���0 ����Ѿ�����ҳ�� ������ҳ�� 
		
		int fuzzy_type_count;											// ģ��������������
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


