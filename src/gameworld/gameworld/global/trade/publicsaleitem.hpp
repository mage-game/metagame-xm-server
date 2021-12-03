/******************************************************************************************
FileName: publicsaleitem.hpp
Author: dreamwang
Description: 拍卖物品定义
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/30/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __PUBLIC_SALE_ITEM_HPP__
#define __PUBLIC_SALE_ITEM_HPP__

#include "protocal/msgscpublicsale.h"

class PublicSaleItem
{
public:
	PublicSaleItem();
	~PublicSaleItem();

	void * operator new(size_t c);
	void operator delete(void *m);

	inline bool CondMatch(int level, int level_interval, int prof, int color, int color_interval, int order, int order_interval)
	{
		bool match = ((0 != prof && 0 != item_param.prof) ? item_param.prof == prof : true);
		match &= ((0 != color && 0 != item_param.color) ? (item_param.color >= color && item_param.color <= color + color_interval) : true);
		match &= ((0 != level && 0 != item_param.level) ? (item_param.level >= level && item_param.level < level + level_interval) : true);
		match &= ((0 != order) ? (item_param.order >= order && item_param.order <= order + order_interval) : true);
		return match;
	}

	UserID m_seller_user_id;                    // 卖家UID
	GameName m_seller_name;                     // 卖家用户名

	SaleItemParam item_param;
};

#endif  // __PUBLIC_SALE_ITEM_HPP__

