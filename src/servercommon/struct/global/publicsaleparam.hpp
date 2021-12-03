/******************************************************************************************
FileName: publicsaleparam.hpp
Author: dreamwang
Description: 拍卖行在数据库中的定义  
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/09/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __PUBLIC_SALE_PARAM_HPP__
#define __PUBLIC_SALE_PARAM_HPP__

#include "common/tlvprotocol.h"
#include "servercommon/serverdef.h"
#include "servercommon/tradedef.hpp"

class PublicSaleParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct SaleItem
	{
		long long id_pulicsale;				// 仅仅初始化的时候有用 

		char change_state;
		int seller_uid;
		GameName seller_name;

		SaleItemParam item_param;
	};

	static const int MAX_PUBLIC_SALE_COUNT = 1000;

	int count;
	SaleItem sale_list[MAX_PUBLIC_SALE_COUNT];
};

#endif  // __PUBLIC_SALE_PARAM_HPP__

