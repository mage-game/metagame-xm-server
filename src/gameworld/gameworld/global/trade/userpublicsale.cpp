#include "userpublicsale.hpp"
#include "publicsaleindex.hpp"
#include "publicsaleitem.hpp"
#include "userpublicsalemanager.hpp"

#include "global/mail/usermail.hpp"

#include "servercommon/maildef.hpp"
#include "servercommon/struct/structcommon.h"
#include "servercommon/string/globalstr.h"

#include "gamelog.h"
#include "global/rmibackobjdef.h"
#include "internalcomm.h"
#include "engineadapter.h"
#include "world.h"

UserPublicSale::UserPublicSale(UserPublicSaleManager *user_publicsale_manager, int uid)
	: m_user_publicsale_manager(user_publicsale_manager), m_uid(uid), m_publicsale_count(0), m_is_change(false),  
	m_last_save_time(0), m_last_check_due_time(0)
{
	memset(m_publicsale_list, 0, sizeof(m_publicsale_list));

	memset(m_old_state, 0, sizeof(m_old_state));
	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));
}

UserPublicSale::~UserPublicSale()
{
	for (int i = 0; i < PUBLICSALE_MAX_ITEM_COUNT; ++i)
	{
		if (NULL != m_publicsale_list[i]) delete m_publicsale_list[i];
	}
}

void UserPublicSale::Update(time_t now)
{
	if (now - m_last_save_time >= PUBLICSALE_WRITE_DB_INTERVAL && this->IsDirty())
	{
		if (this->SaveUserPublicSaleToDB())
		{
			m_last_save_time = now;
			this->ClearDirtyMark();
		}
	}

	if (now - m_last_check_due_time >= PUBLICSALE_CHECK_DUE_INTERVAL)
	{
		m_last_check_due_time = now;

		for (int i = 0; i < PUBLICSALE_MAX_ITEM_COUNT; i++)
		{
			if (NULL != m_publicsale_list[i] && now > m_publicsale_list[i]->item_param.due_time)
			{
				m_user_publicsale_manager->SendMailToRole(IntToUid(m_uid), &m_publicsale_list[i]->item_param, SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_ITEM_EXPIRE);

				m_publicsale_list[i]->m_seller_name[sizeof(GameName) - 1] = 0;

				gamelog::g_log_publicsale.printf(LL_INFO, "public_sale -> overtime_remove item[ role[ uid:%d, name:%s], sale_item[%s] ]", 
					m_uid, m_publicsale_list[i]->m_seller_name, SaleItemParamLog(&m_publicsale_list[i]->item_param));

				LOG_QUICK12(gamelog::g_log_quick, 
					LOG_TYPE_PUBLIC_SALE_EXPIRED, m_uid, m_publicsale_list[i]->m_seller_name, NULL,
					m_publicsale_list[i]->item_param.item_wrapper.item_id, m_publicsale_list[i]->item_param.item_wrapper.num, NULL, NULL,
					0, m_publicsale_list[i]->item_param.price, 0, 0);

				this->RemoveUserPublicSaleItem(i);
			}
		}
	}
}

bool UserPublicSale::InitPublicSaleItem(const PublicSaleParam::SaleItem &saleitem_param)
{
	if (saleitem_param.item_param.sale_index < 0 || saleitem_param.item_param.sale_index >= PUBLICSALE_MAX_ITEM_COUNT) 
	{
		gamelog::g_log_serious_error.printf(LL_CRITICAL, "[UserPublicSale::InitPublicSaleItem INVALID SALE INDEX] [ uid:%d, name:%s, sale_index:%d]", 
			saleitem_param.seller_uid, saleitem_param.seller_name, (int)saleitem_param.item_param.sale_index);
		return false;
	}

	PublicSaleItem sale_item;

	sale_item.m_seller_user_id = IntToUid(saleitem_param.seller_uid);
	F_STRNCPY(sale_item.m_seller_name, saleitem_param.seller_name, sizeof(sale_item.m_seller_name));
	sale_item.item_param.sale_type = saleitem_param.item_param.sale_type;
	sale_item.item_param = saleitem_param.item_param;

	if (this->AddUserPublicSaleItem(sale_item))
	{
		m_old_state[sale_item.item_param.sale_index] = true;
		return true;
	}

	gamelog::g_log_serious_error.printf(LL_CRITICAL, "[UserPublicSale::InitPublicSaleItem  AddUserPublicSaleItem FAIL] [ uid:%d, name:%s, sale_index:%d]", 
		saleitem_param.seller_uid, saleitem_param.seller_name, (int)saleitem_param.item_param.sale_index);

	return false;
}

bool UserPublicSale::OnAddPublicSaleItem(gglobalprotocal::AddPublicSaleItem *add_sale_req)
{
	if (NULL == add_sale_req || add_sale_req->item_param.sale_index < 0 || add_sale_req->item_param.sale_index >= PUBLICSALE_MAX_ITEM_COUNT) return false;

	add_sale_req->role_name[sizeof(add_sale_req->role_name) - 1] = '\0';

	static PublicSaleItem sale_item;

	sale_item.m_seller_user_id = UserID(add_sale_req->db_index, add_sale_req->role_id);
	F_STRNCPY(sale_item.m_seller_name, add_sale_req->role_name, sizeof(sale_item.m_seller_name));

	sale_item.item_param = add_sale_req->item_param;

	if (this->AddUserPublicSaleItem(sale_item))
	{
		m_is_change = true;
		m_dirty_mark[sale_item.item_param.sale_index] = true;

		if (add_sale_req->is_to_world)
		{
			int uid = UidToInt(UserID(add_sale_req->db_index, add_sale_req->role_id));
			const SaleItemParam & param = add_sale_req->item_param;

			this->SendPublicSaleItemToWorld(uid, add_sale_req->role_name, add_sale_req->camp, param);
		}

		LOG_QUICK12(gamelog::g_log_quick, 
			LOG_TYPE_PUBLIC_SALE_ADD, UidToInt(sale_item.m_seller_user_id), sale_item.m_seller_name, NULL,
			sale_item.item_param.item_wrapper.item_id, sale_item.item_param.item_wrapper.num, NULL, NULL,
			0, sale_item.item_param.price, 0, 0);

		return true;
	}

	m_user_publicsale_manager->SendMailToRole(IntToUid(m_uid), &sale_item.item_param, SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_ITEM_FULL);
	
	return false;
}

bool UserPublicSale::OnRemovePublicSaleItem(gglobalprotocal::RemovePublicSaleItem *remove_sale_req)
{
	if (NULL == remove_sale_req || remove_sale_req->sale_index < 0 || remove_sale_req->sale_index >= PUBLICSALE_MAX_ITEM_COUNT) return false;
	if (NULL == m_publicsale_list[remove_sale_req->sale_index]) return false;

	PublicSaleItem *sale_item = m_publicsale_list[remove_sale_req->sale_index];

	m_user_publicsale_manager->SendMailToRole(IntToUid(m_uid), &sale_item->item_param, SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_ITEM_CANCEL); 

	sale_item->m_seller_name[sizeof(GameName) - 1] = 0;

	gamelog::g_log_publicsale.printf(LL_INFO, "public_sale -> user_remove item[ role[ uid:%d, name:%s], sale_item[%s] ]", 
		m_uid, sale_item->m_seller_name, SaleItemParamLog(&sale_item->item_param));

	LOG_QUICK12(gamelog::g_log_quick, 
			LOG_TYPE_PUBLIC_SALE_DEL, UidToInt(sale_item->m_seller_user_id), sale_item->m_seller_name, NULL,
			sale_item->item_param.item_wrapper.item_id, sale_item->item_param.item_wrapper.num, NULL, NULL,
			0, sale_item->item_param.price, 0, 0);

	this->RemoveUserPublicSaleItem(remove_sale_req->sale_index);

	return true;
}

bool UserPublicSale::OnBuyPublicSaleItem(gglobalprotocal::BuyPublicSaleItem *buy_sale_req)
{
	if (NULL == buy_sale_req || buy_sale_req->sale_index < 0 || buy_sale_req->sale_index >= PUBLICSALE_MAX_ITEM_COUNT) return false;
	if (NULL == m_publicsale_list[buy_sale_req->sale_index]) return false;

	UserID buyer_user_id(buy_sale_req->db_index, buy_sale_req->role_id);

	PublicSaleItem *sale_item = m_publicsale_list[buy_sale_req->sale_index];

	if (!this->CheckFair(sale_item, buy_sale_req)) return false;
	
	m_user_publicsale_manager->SendMailToRole(buyer_user_id, &sale_item->item_param, SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_ITEM_BUY_SUCC);
	m_user_publicsale_manager->SendMailToRole(IntToUid(m_uid), buy_sale_req, SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_MONEY_SALE_SUCC);

	buy_sale_req->role_name[sizeof(GameName) - 1] = 0;
	sale_item->m_seller_name[sizeof(GameName) - 1] = 0;

	gamelog::g_log_publicsale.printf(LL_INFO, "public_sale -> buy item[ buyer[ uid:%d, name:%s], seller[ uid:%d, name:%s], sale_item[%s] ]",
		UidToInt(buyer_user_id), buy_sale_req->role_name,
		m_uid, sale_item->m_seller_name,
		SaleItemParamLog(&sale_item->item_param));

	LOG_QUICK12(gamelog::g_log_quick, 
			LOG_TYPE_PUBLIC_SALE_BUY, m_uid, sale_item->m_seller_name, NULL,
			sale_item->item_param.item_wrapper.item_id, sale_item->item_param.item_wrapper.num, NULL, NULL,
			UidToInt(buyer_user_id), 0, 0, 0);

	this->RemoveUserPublicSaleItem(buy_sale_req->sale_index);

	return true;
}

bool UserPublicSale::OnGetPublicSaleItemList(Protocol::SCGetPublicSaleItemListAck &publicsale_item_list_ack)
{
	int count = 0;

	for (int i = 0; i < PUBLICSALE_MAX_ITEM_COUNT && count < PUBLICSALE_MAX_ITEM_COUNT; i++)
	{
		if (NULL == m_publicsale_list[i]) continue;
		publicsale_item_list_ack.sale_item_list[count] = m_publicsale_list[i]->item_param;

		count ++;
	}

	publicsale_item_list_ack.count = count;

	return true;
}

bool UserPublicSale::OnSendPublicSaleItemInfoToWorld(gglobalprotocal::PublicSaleSendItemInfoToWorld *send_iteminfo_req)
{
	if (NULL == send_iteminfo_req || send_iteminfo_req->sale_index < 0 || send_iteminfo_req->sale_index >= m_publicsale_count) return false;

	const PublicSaleItem *publicsale_item = m_publicsale_list[send_iteminfo_req->sale_index];
	if (NULL == publicsale_item) return false;

	const SaleItemParam &param = publicsale_item->item_param;
	int uid = UidToInt(UserID(send_iteminfo_req->db_index, send_iteminfo_req->role_id));

	return this->SendPublicSaleItemToWorld(uid, send_iteminfo_req->role_name, send_iteminfo_req->camp, param);
}

bool UserPublicSale::CheckFair(PublicSaleItem *publicsale_item, gglobalprotocal::BuyPublicSaleItem *buy_sale_req)
{
	if (NULL == publicsale_item || NULL == buy_sale_req) return false;
	if (buy_sale_req->price_type != publicsale_item->item_param.price_type || buy_sale_req->sale_item_type != publicsale_item->item_param.sale_item_type) return false;
	if (buy_sale_req->price != publicsale_item->item_param.price)  return false;  // 保证卖方利益
	if (buy_sale_req->item_id != publicsale_item->item_param.item_wrapper.item_id || buy_sale_req->item_num != publicsale_item->item_param.item_wrapper.num ||
		buy_sale_req->sale_value != publicsale_item->item_param.sale_value) 
		return false; // 保证买方利益

	return true;
}

bool UserPublicSale::AddUserPublicSaleItem(const PublicSaleItem &sale_item)
{
	int index = (int)sale_item.item_param.sale_index;
	if (index < 0 || index >= PUBLICSALE_MAX_ITEM_COUNT) return false;
	if (NULL != m_publicsale_list[index]) return false;

	PublicSaleItem *psale_item = new PublicSaleItem();
	if (NULL == psale_item) return false;

	memcpy(psale_item, &sale_item, sizeof(PublicSaleItem));

	if (!m_user_publicsale_manager->GetPublicSaleIndex().OnAddPublicSaleItem(m_uid, psale_item)) 
	{
		delete psale_item;
		return false;
	}

	m_publicsale_list[index] = psale_item;
	++ m_publicsale_count;

	return true;
}

bool UserPublicSale::RemoveUserPublicSaleItem(int sale_index)
{
	if (sale_index < 0 || sale_index >= PUBLICSALE_MAX_ITEM_COUNT || NULL == m_publicsale_list[sale_index]) return false;

	PublicSaleItem *sale_item = m_publicsale_list[sale_index];

	m_user_publicsale_manager->GetPublicSaleIndex().OnRemovePublicSaleItem(m_uid, sale_item);

	delete sale_item; 
	m_publicsale_list[sale_index] = NULL;

	m_dirty_mark[sale_index] = true;
	m_is_change = true;

	-- m_publicsale_count;

	{
		static Protocol::SCGetPublicSaleItemListAck publicsale_item_list_ack;

		this->OnGetPublicSaleItemList(publicsale_item_list_ack);

		int sendlen = sizeof(publicsale_item_list_ack) - (PUBLICSALE_MAX_ITEM_COUNT - publicsale_item_list_ack.count) * sizeof(SaleItemParam);
		World::GetInstWorld()->GetSceneManager()->SendToUser(IntToUid(m_uid), (const char*)&publicsale_item_list_ack, sendlen);
	}
	
	return true;
}

bool UserPublicSale::SendPublicSaleItemToWorld(int uid, GameName role_name, int camp, const SaleItemParam &sale_item)
{
	if (NULL == role_name) return false;

	role_name[sizeof(GameName) - 1] = '\0';

	//int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_publicsale_sell_to_world, 
	//	uid, role_name, camp, sale_item.item_wrapper.item_id, sale_item.item_wrapper.num, sale_item.price, GOLD_ITEM_ID, 
	//	uid, sale_item.sale_index, sale_item.item_wrapper.item_id, sale_item.item_wrapper.num, sale_item.price);
	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_publicsale_sell_to_world, 
		uid, role_name, camp, sale_item.item_wrapper.item_id, sale_item.item_wrapper.num, sale_item.price, RandomNum(1, 24));
	if (length > 0)
	{
		//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_ONLY_CHAT_WORLD, 0, 0);
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_ONLY_CHAT_WORLD);
		return true;
	}

	return false;
}

void UserPublicSale::ClearDirtyMark()
{
	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));

	for (int i = 0; i < PUBLICSALE_MAX_ITEM_COUNT; ++i)
	{
		m_old_state[i] = (NULL != m_publicsale_list[i]);
	}

	m_is_change = false;
}

void UserPublicSale::GetChangePublicSaleList(PublicSaleParam *sale_param)
{
	if (!m_is_change)
	{
		sale_param->count = 0;
		return;
	}

	int count = 0;

	for (int i = 0; i < PUBLICSALE_MAX_ITEM_COUNT && count < PUBLICSALE_MAX_ITEM_COUNT; ++i)
	{
		if (m_dirty_mark[i])
		{
			if (NULL == m_publicsale_list[i])
			{
				if (!m_old_state[i]) continue;

				sale_param->sale_list[count].change_state = structcommon::CS_DELETE;
				sale_param->sale_list[count].item_param.sale_index = i;

				sale_param->sale_list[count].seller_uid = m_uid;
			}
			else 
			{
				sale_param->sale_list[count].change_state = m_old_state[i] ?  structcommon::CS_UPDATE : structcommon::CS_INSERT;
				sale_param->sale_list[count].item_param.sale_index = i;

				sale_param->sale_list[count].seller_uid = m_uid;
				F_STRNCPY(sale_param->sale_list[count].seller_name, m_publicsale_list[i]->m_seller_name, sizeof(sale_param->sale_list[0].seller_name));
				sale_param->sale_list[count].item_param = m_publicsale_list[i]->item_param;
			}

			++count;
		}
	}

	sale_param->count = count;
}

bool UserPublicSale::SaveUserPublicSaleToDB()
{
	static PublicSaleParam user_publicsale_param;
	this->GetChangePublicSaleList(&user_publicsale_param);

	if (user_publicsale_param.count <= 0) return true;

	RMISavePublicSaleBackObjectImpl *impl = new RMISavePublicSaleBackObjectImpl();

	RMIGlobalClient ac;
	ac.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
	return ac.SavePublicSaleAsyn(user_publicsale_param, impl);
}

