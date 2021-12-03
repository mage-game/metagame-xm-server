/******************************************************************************************
FileName: userpublicsalemanager.hpp
Author: dreamwang
Description: 用户拍卖管理
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/30/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __USER_PUBLIC_SALE_MANAGER_HPP__
#define __USER_PUBLIC_SALE_MANAGER_HPP__

#include <map>

#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "protocal/msgscpublicsale.h"

#include "userpublicsale.hpp"
#include "publicsaleindex.hpp"
#include "publicsaleitem.hpp"

class UserPublicSale;
class Role;

class UserPublicSaleManager
{
	friend class RMIInitPublicSaleBackObjectImpl;

public:
	static UserPublicSaleManager & Instance();

	PublicSaleIndex & GetPublicSaleIndex() { return m_publicsale_index; }

	void OnServerStart();
	void OnServerStop();

	bool IsLoadFinish();

	void Update(unsigned long interval, time_t now_second);

	bool OnAddPublicSaleItem(Role *user, gglobalprotocal::AddPublicSaleItem *add_publicsale_req);						
	bool OnRemovePublicSaleItem(Role *user, gglobalprotocal::RemovePublicSaleItem *remove_publicsale_req);				
	bool OnBuyPublicSaleItem(Role *user, gglobalprotocal::BuyPublicSaleItem *buy_publicsale_req);		
	bool OnGetPublicSaleItemList(Role *user);
	bool OnSendPublicSaleItemInfoToWorld(Role *user, gglobalprotocal::PublicSaleSendItemInfoToWorld *send_iteminfo_toworld_req);

	bool OnPublicSaleSearch(Role *user, Protocol::CSPublicSaleSearch *publicsale_search_req);

	void SendMailToRole(const UserID &user_id, const gglobalprotocal::BuyPublicSaleItem *buy_publicsale_req, int reason);
	void SendMailToRole(const UserID &user_id, const SaleItemParam *sale_item_param, int reason);

	void OnPublicSaleTypeCountReq(Role *role);

private:
	UserPublicSaleManager();
	~UserPublicSaleManager();

	bool LoadPublicSale(long long publicsale_id_from);
	void LoadPublicSaleSucc();

	UserPublicSale* GetUserPublicSale(int uid);
	UserPublicSale* CreateUserPublicSale(int uid);

	int m_data_state;

	typedef std::map<int, UserPublicSale*> UserPublicSaleMap; 
	typedef std::map<int, UserPublicSale*>::iterator UserPublicSaleMapIt;
	UserPublicSaleMap m_user_publicsale_map;

	PublicSaleIndex	m_publicsale_index;
};	

#define WRAP_GIVE_SALE_ITEM(item_id, num, sale_item_param, contentparam)\
{\
	if (PUBLIC_SALE_ITEM_TYPE_ITEM == sale_item_param->sale_item_type)\
	{\
		item_id = sale_item_param->item_wrapper.item_id;\
		num = sale_item_param->item_wrapper.num;\
		UNSTD_STATIC_CHECK(sizeof(contentparam.item_list[0]) == sizeof(sale_item_param->item_wrapper));\
		memcpy(&contentparam.item_list[0], &sale_item_param->item_wrapper, sizeof(contentparam.item_list[0]));\
	}\
	if (PUBLIC_SALE_ITEM_TYPE_COIN == sale_item_param->sale_item_type)\
	{\
		item_id = COIN_ITEM_ID;\
		num = sale_item_param->sale_value;\
		contentparam.coin = sale_item_param->sale_value;\
	}\
}

#define WRAP_GIVE_BACK_SELL_PRICE(item_id, num, sale_item_param, contentparam, price, expenses, coin_expenses_rate, gold_expense_rate)\
{\
	if (PUBLIC_SALE_ITEM_TYPE_ITEM == sale_item_param->sale_item_type)\
	{\
		item_id = sale_item_param->item_wrapper.item_id;\
		num = sale_item_param->item_wrapper.num;\
	}\
	if (PUBLIC_SALE_ITEM_TYPE_COIN == sale_item_param->sale_item_type)\
	{\
		item_id = COIN_ITEM_ID;\
		num = sale_item_param->sale_value;\
	}\
	if (PUBLIC_SALE_PRICE_TYPE_COIN == sale_item_param->price_type)\
	{\
		expenses = static_cast<int>(sale_item_param->price * coin_expenses_rate);\
		expenses = expenses <= 0 ? 1 : expenses;\
		price -= expenses;\
		price = price <= 0 ? 1 : price;\
		contentparam.coin = price;\
	}\
	if (PUBLIC_SALE_PRICE_TYPE_GOLD == sale_item_param->price_type)\
	{\
		expenses = static_cast<int>(sale_item_param->price * gold_expense_rate);\
		expenses = expenses <= 0 ? 1 : expenses;\
		price -= expenses;\
		price = price <= 0 ? 1 : price;\
		contentparam.gold = price;\
	}\
}



#endif  // __USER_PUBLIC_SALE_MANAGER_HPP__

