/******************************************************************************************
FileName: msgcsmarket.hpp
Author: lzj
Description: ��ҵ������г���Э�� 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
lzj		   2011/08/09/ 09:04:05     1.0           ����
*****************************************************************************************/

#ifndef __MSGCS_PUBLICSALE_HPP_
#define __MSGCS_PUBLICSALE_HPP_

#include "servercommon/userprotocal/msgheader.h"

#include "gamedef.h"
#include "servercommon/serverdef.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{

	//  C -> S

	class CSAddPublicSaleItem
	{
	public:
		CSAddPublicSaleItem();
		MessageHeader			header;

		int						sale_index;
		short					knapsack_index;	
		short					item_num;
		int						price;
		int						keep_time_type;
		int						is_to_world;
		
		int						sale_value;
		short					sale_item_type;
		short					price_type;
	};

	class CSRemovePublicSaleItem
	{
	public:
		CSRemovePublicSaleItem();
		MessageHeader			header;

		int						sale_index;
	};

	class CSBuyPublicSaleItem
	{
	public:
		CSBuyPublicSaleItem();
		MessageHeader			header;

		int						seller_uid;
		int						sale_index;

		int						item_id;
		int						item_num;
		int						price;
		int						sale_value;
		short					sale_item_type;
		short					price_type;
	};

	class CSPublicSaleGetUserItemList
	{
	public:
		CSPublicSaleGetUserItemList();
		MessageHeader			header;
	};

	class CSPublicSaleSendItemInfoToWorld				// �û�����㲥������Ʒ��Ϣ
	{
	public:
		CSPublicSaleSendItemInfoToWorld();
		MessageHeader			header;

		short					sale_index;
		short					reserve;
	};

	class CSPublicSaleTypeCountReq						// ����ÿ���������͵�����
	{
	public:
		CSPublicSaleTypeCountReq();
		MessageHeader			header;
	};
}

#pragma pack(pop)

#endif		// __MSGCS_PUBLICSALE_HPP_

