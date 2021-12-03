/******************************************************************************************
FileName: userpublicsale.hpp
Author: dreamwang
Description: 用户拍卖的物品 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/30/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __USER_PUBLIC_SALE_HPP__
#define __USER_PUBLIC_SALE_HPP__

#include "inetworkmodule.h"

#include "protocal/msgscpublicsale.h"

#include "servercommon/struct/global/publicsaleparam.hpp"

#include "servercommon/internalprotocal/gglobalprotocal.h"

class PublicSaleItem;
class UserPublicSaleManager;

class UserPublicSale
{
	friend class RMIInitPublicSaleBackObjectImpl;

public:
	UserPublicSale(UserPublicSaleManager *user_publicsale_manager, int uid);
	~UserPublicSale();

	void * operator new(size_t c);
	void operator delete(void *m);

	void Update(time_t now);

	int GetPublicSaleCount() { return m_publicsale_count; }
	bool IsDirty() { return m_is_change; }

	bool InitPublicSaleItem(const PublicSaleParam::SaleItem &saleitem_param);

	bool OnAddPublicSaleItem(gglobalprotocal::AddPublicSaleItem *add_sale_req);						
	bool OnRemovePublicSaleItem(gglobalprotocal::RemovePublicSaleItem *remove_sale_req);				
	bool OnBuyPublicSaleItem(gglobalprotocal::BuyPublicSaleItem *buy_sale_req);
	bool OnGetPublicSaleItemList(Protocol::SCGetPublicSaleItemListAck &publicsale_item_list_ack);
	bool OnSendPublicSaleItemInfoToWorld(gglobalprotocal::PublicSaleSendItemInfoToWorld *send_iteminfo_req);	

	bool SaveUserPublicSaleToDB();

private:
	bool CheckFair(PublicSaleItem *publicsale_item, gglobalprotocal::BuyPublicSaleItem *buy_sale_req);

	bool AddUserPublicSaleItem(const PublicSaleItem &sale_item);
	bool RemoveUserPublicSaleItem(int sale_index);
	bool SendPublicSaleItemToWorld(int uid, GameName role_name, int camp, const SaleItemParam &sale_item);

	void ClearDirtyMark();
	void GetChangePublicSaleList(PublicSaleParam *sale_param);

	UserPublicSaleManager *m_user_publicsale_manager;
	int m_uid;

	int m_publicsale_count;
	PublicSaleItem *m_publicsale_list[PUBLICSALE_MAX_ITEM_COUNT];

	bool m_old_state[PUBLICSALE_MAX_ITEM_COUNT];
	bool m_dirty_mark[PUBLICSALE_MAX_ITEM_COUNT];
	bool m_is_change;

	time_t m_last_save_time;
	time_t m_last_check_due_time;
};

#endif  // __USER_PUBLIC_SALE_HPP__

