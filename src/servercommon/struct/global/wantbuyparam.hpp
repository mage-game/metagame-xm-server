/******************************************************************************************
FileName: wantbuyparam.hpp
Author: dreamwang
Description: 求购行在数据库中的定义  
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/09/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __WANTBUY_PARAM_HPP__
#define __WANTBUY_PARAM_HPP__

#include "common/tlvprotocol.h"
#include "servercommon/serverdef.h"

class WantBuyParam
{
public:
	WantBuyParam() : count(0) {}
	~WantBuyParam() {}

	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct WantBuyItem
	{
		int buyer_db_index;
		int buyer_role_id;
		int buy_index;

		GameName buyer_name;

		int item_id;					// 物品ID
		int item_num;					// 个数 
		int color;						// 颜色
		int level;						// 等级
		int prof;						// 职业
		int gold_per;					// 出售单价 gold

		unsigned int wantbuy_time;		// 开始出售时间
		unsigned int due_time;			// 持续时间
	};

	struct WantBuyAttr
	{
		char change_state;
		long long id_wantbuy;
		WantBuyItem wantbuy_item;
	};

	static const int MAX_WANT_BUY_COUNT = 1000;

	int count;
	WantBuyAttr wantbuy_list[MAX_WANT_BUY_COUNT];
};

#endif  // __PUBLIC_WANTBUY_PARAM_HPP__


