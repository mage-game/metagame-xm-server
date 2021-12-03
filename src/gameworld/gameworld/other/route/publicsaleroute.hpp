/******************************************************************************************
FileName: marketrout.hpp
Author: lzj
Description: 玩家到交易市场路由功能 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/09/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __PUBLICSALE_ROUTE_HPP__
#define __PUBLICSALE_ROUTE_HPP__

#include "protocal/msgcspublicsale.hpp"

class Role;

class PublicSaleRoute
{
public:
	static PublicSaleRoute & Instance();
	
	void OnAddPublicSaleItem(Role *role, Protocol::CSAddPublicSaleItem *add_publicsale_item);
	void OnRemovePublicSaleItem(Role *role, Protocol::CSRemovePublicSaleItem *remove_sale_req);
	void OnBuyPublicSaleItem(Role *role, Protocol::CSBuyPublicSaleItem *buy_publicsale_item);
	void OnGetPublicSaleUserItemList(Role *role, Protocol::CSPublicSaleGetUserItemList *get_publicsale_itemlist);
	void OnSendPublicSaleItemInfoToWorld(Role *role, Protocol::CSPublicSaleSendItemInfoToWorld *send_publicsale_iteminfo);

private:
	PublicSaleRoute() {}
	~PublicSaleRoute() {}
};

#endif // __PUBLICSALE_ROUTE_HPP__

