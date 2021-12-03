#include "userpublicsalemanager.hpp"

#include "gamelog.h"

#include "protocal/msgscpublicsale.h"

#include "servercommon/errornum.h"
#include "servercommon/string/globalstr.h"
#include "servercommon/internalprotocal/ggprotocal.h"

#include "serverlogic.h"
#include "world.h"
#include "global/rmibackobjdef.h"
#include "rmiclient/rmiglobal.hpp"
#include "internalcomm.h"

#include "global/mail/mailmanager.hpp"
#include "obj/character/role.h"
#include "servercommon/tradedef.hpp"
#include "globalconfig/globalconfig.h"

UserPublicSaleManager::UserPublicSaleManager()
	: m_data_state(PUBLICSALE_DATA_STATE_INVALID)
{

}

UserPublicSaleManager::~UserPublicSaleManager()
{
	for (UserPublicSaleMapIt it = m_user_publicsale_map.begin(), end = m_user_publicsale_map.end(); it != end; ++it)
	{
		if (NULL != it->second) delete it->second;
	}

	m_user_publicsale_map.clear();
}

UserPublicSaleManager & UserPublicSaleManager::Instance()
{
	static UserPublicSaleManager upsm;
	return upsm;
}

void UserPublicSaleManager::OnServerStart()
{
	this->LoadPublicSale(0);
}

void UserPublicSaleManager::OnServerStop()
{
	for (UserPublicSaleMapIt it = m_user_publicsale_map.begin(), end = m_user_publicsale_map.end(); it != end; ++it)
	{
		it->second->SaveUserPublicSaleToDB();
	}
}

void UserPublicSaleManager::Update(unsigned long interval, time_t now_second)
{
	if (PUBLICSALE_DATA_STATE_LOAD_FINISH != m_data_state) return;

	for (UserPublicSaleMapIt it = m_user_publicsale_map.begin(), end = m_user_publicsale_map.end(); it != end; ++it)
	{
		UserPublicSale *user_publicsale = it->second;
	
		// 如果没有拍卖的物品并且已经回写过DB了 就删掉
		if (!user_publicsale->IsDirty() && user_publicsale->GetPublicSaleCount() <= 0)
		{
			delete user_publicsale;
			m_user_publicsale_map.erase(it);
			break;
		}
		else 
		{
			it->second->Update(now_second);
		}
	}
}

bool UserPublicSaleManager::LoadPublicSale(long long publicsale_id_from)
{
	if (PUBLICSALE_DATA_STATE_LOAD_FINISH == m_data_state) return false;

	m_data_state = PUBLICSALE_DATA_STATE_LOADING;

	RMIInitPublicSaleBackObjectImpl *impl = new RMIInitPublicSaleBackObjectImpl();
	
	RMIGlobalClient ac;
	ac.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
	return ac.InitPublicSaleAsyn(publicsale_id_from, impl);
}	

void RMIInitPublicSaleBackObjectImpl::InitPublicSaleRet(int ret, const PublicSaleParam &publicsale_param)
{
	if (ret < 0 || publicsale_param.count < 0 || publicsale_param.count > PublicSaleParam::MAX_PUBLIC_SALE_COUNT) 
	{
		printf(" RMIInitPublicSaleBackObjectImpl::Init Failed \n");
		ServerLogic::GetInstServerLogic()->StopServer();
		return;
	}

	long long next_id_from = 0;

	for (int i = 0; i < publicsale_param.count; i++)
	{
		if (publicsale_param.sale_list[i].id_pulicsale > next_id_from)
		{
			next_id_from = publicsale_param.sale_list[i].id_pulicsale;
		}

		if (publicsale_param.sale_list[i].seller_uid <= 0) 
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[RMIInitPublicSaleBackObjectImpl::InitPublicSaleRet INVALID USER INFO] user[%d %s]", 
				(int)publicsale_param.sale_list[i].seller_uid, (const char*)publicsale_param.sale_list[i].seller_name);
			continue;
		}

		UserPublicSale *user_publicsale = UserPublicSaleManager::Instance().GetUserPublicSale(publicsale_param.sale_list[i].seller_uid);
		if (NULL == user_publicsale)
		{
			user_publicsale = UserPublicSaleManager::Instance().CreateUserPublicSale(publicsale_param.sale_list[i].seller_uid);
		}

		user_publicsale->InitPublicSaleItem(publicsale_param.sale_list[i]);
	}

	if (publicsale_param.count > 0)	
	{
		UserPublicSaleManager::Instance().LoadPublicSale(next_id_from);
	}
	else
	{
		UserPublicSaleManager::Instance().LoadPublicSaleSucc();
	}
}

void UserPublicSaleManager::LoadPublicSaleSucc()
{
	m_data_state = PUBLICSALE_DATA_STATE_LOAD_FINISH;
}

bool UserPublicSaleManager::OnAddPublicSaleItem(Role *user, gglobalprotocal::AddPublicSaleItem *add_publicsale_req)
{
	if (NULL == user)
	{
		return false;
	}

	if (NULL == add_publicsale_req || add_publicsale_req->db_index < 0 || add_publicsale_req->role_id <= 0 
		|| add_publicsale_req->item_param.sale_index < 0 || add_publicsale_req->item_param.sale_index >= PUBLICSALE_MAX_ITEM_COUNT) 
	{
		return false;
	}
		
	if (PUBLIC_SALE_ITEM_TYPE_ITEM == add_publicsale_req->item_param.sale_item_type && add_publicsale_req->item_param.item_wrapper.Invalid()) 
	{
		return false;
	}

	if (PUBLIC_SALE_ITEM_TYPE_COIN == add_publicsale_req->item_param.sale_item_type && add_publicsale_req->item_param.sale_value <= 0)
	{
		return false;
	}

	const char *log_ret = "";

	UserPublicSale *user_publicsale = this->GetUserPublicSale(user->GetUID());
	if (NULL == user_publicsale)
	{
		user_publicsale = this->CreateUserPublicSale(user->GetUID());
	}

	if (user_publicsale->GetPublicSaleCount() < PUBLICSALE_MAX_ITEM_COUNT)
	{
		if (user_publicsale->OnAddPublicSaleItem(add_publicsale_req))
		{
			Protocol::SCAddPublicSaleItemAck addpublicsale_ack;
			addpublicsale_ack.ret = 0;
			addpublicsale_ack.sale_index = add_publicsale_req->item_param.sale_index;
			EngineAdapter::Instance().NetSend(user->GetNetId(), (const char*)&addpublicsale_ack, sizeof(addpublicsale_ack));
			log_ret = "succ";
		}
		else 
		{
			user->NoticeNum(errornum::EN_PUBLICSALE_ADD_ITEM_FAIL);
			log_ret = "fail";
		}
	}
	else 
	{
		user->NoticeNum(errornum::EN_PUBLICSALE_ADD_ITEM_FULL);
		log_ret = "full";
	}

	add_publicsale_req->role_name[sizeof(add_publicsale_req->role_name) - 1] = '\0';
	gamelog::g_log_publicsale.printf(LL_INFO, "public_sale -> add item[ ret:%s, role[ uid:%d, name:%s ], sale_item[%s] ]", 
		(const char*)log_ret, UidToInt(UserID((int)add_publicsale_req->db_index, (int)add_publicsale_req->role_id)), 
		(const char*)add_publicsale_req->role_name, SaleItemParamLog(&add_publicsale_req->item_param));

	return true;
}

bool UserPublicSaleManager::OnRemovePublicSaleItem(Role *user, gglobalprotocal::RemovePublicSaleItem *remove_publicsale_req)
{
	if (NULL == user)
	{
		return false;
	}

	if (NULL == remove_publicsale_req || remove_publicsale_req->db_index < 0 || remove_publicsale_req->role_id <= 0 || 
		remove_publicsale_req->sale_index < 0 || remove_publicsale_req->sale_index >= PUBLICSALE_MAX_ITEM_COUNT) return false;

	const char *log_ret = "";

	UserPublicSale *user_publicsale = this->GetUserPublicSale(user->GetUID());
	if (NULL != user_publicsale)
	{
		if (user_publicsale->GetPublicSaleCount() > 0)
		{
			if (user_publicsale->OnRemovePublicSaleItem(remove_publicsale_req))
			{
				Protocol::SCRemovePublicSaleItemAck removepublicsale_ack;
				removepublicsale_ack.ret = 0;
				removepublicsale_ack.sale_index = remove_publicsale_req->sale_index;
				EngineAdapter::Instance().NetSend(user->GetNetId(), (const char*)&removepublicsale_ack, sizeof(removepublicsale_ack));
				log_ret = "succ";
			}
			else 
			{
				user->NoticeNum(errornum::EN_PUBLICSALE_REMOVE_ITEM_NOT_EXIST);
				log_ret = "fail";
			}
		}
		else 
		{
			user->NoticeNum(errornum::EN_PUBLICSALE_NOT_HAVE_ITEM);
			log_ret = "no item";
		}
	}
	else
	{
		user->NoticeNum(errornum::EN_PUBLICSALE_NOT_HAVE_ITEM);
		log_ret = "no user";
	}

	gamelog::g_log_publicsale.printf(LL_INFO, "public_sale -> del item[ ret:%s, uid:%d, sale_index:%d ]",
		(const char*)log_ret,
		UidToInt(UserID((int)remove_publicsale_req->db_index, (int)remove_publicsale_req->role_id)), (int)remove_publicsale_req->sale_index);

	return true;
}

bool UserPublicSaleManager::OnBuyPublicSaleItem(Role *user, gglobalprotocal::BuyPublicSaleItem *buy_publicsale_req)
{
	if (NULL == user)
	{
		return false;
	}

	if (NULL == buy_publicsale_req || buy_publicsale_req->db_index < 0 || buy_publicsale_req->role_id <= 0 || 
		buy_publicsale_req->sale_index < 0 || buy_publicsale_req->sale_index >= PUBLICSALE_MAX_ITEM_COUNT || 
		buy_publicsale_req->seller_db_index < 0 || buy_publicsale_req->seller_role_id <= 0) return false;

	UserID seller_user_id(buy_publicsale_req->seller_db_index, buy_publicsale_req->seller_role_id);
	UserID buyer_user_id = user->GetUserId();
	
	const char *log_ret = "";

	UserPublicSale *user_publicsale = this->GetUserPublicSale(UidToInt(seller_user_id));
	if (NULL != user_publicsale)
	{
		if (user_publicsale->OnBuyPublicSaleItem(buy_publicsale_req))
		{
			Protocol::SCBuyPublicSaleItemAck buypublicsale_ack;
			buypublicsale_ack.ret = 0;	
			buypublicsale_ack.seller_uid = UidToInt(seller_user_id);
			buypublicsale_ack.sale_index = buy_publicsale_req->sale_index;
			EngineAdapter::Instance().NetSend(user->GetNetId(), (const char*)&buypublicsale_ack, sizeof(buypublicsale_ack));
			log_ret = "succ";
		}
		else 
		{
			this->SendMailToRole(buyer_user_id, buy_publicsale_req, SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_MONEY_BUY_FAIL);

			user->NoticeNum(errornum::EN_PUBLICSALE_BUY_ITEM_NOT_EXIST);
			log_ret = "fail";
		}
	}
	else 
	{
		this->SendMailToRole(buyer_user_id, buy_publicsale_req, SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_MONEY_BUY_FAIL);

		user->NoticeNum(errornum::EN_PUBLICSALE_BUY_ITEM_NOT_EXIST);
		log_ret = "no user";
	}

	gamelog::g_log_publicsale.printf(LL_INFO, "public_sale -> buy item[ ret:%s, buyer[ uid:%d ], "
		"seller[ uid:%d ], item[ sale_index:%d,id:%d,num:%d,gold_price:%d] ]",
		(const char*)log_ret,
		UidToInt(UserID((int)buy_publicsale_req->db_index, (int)buy_publicsale_req->role_id)), 
		UidToInt(UserID((int)buy_publicsale_req->seller_db_index, (int)buy_publicsale_req->seller_role_id)), 
		(int)buy_publicsale_req->sale_index, (int)buy_publicsale_req->item_id, (int)buy_publicsale_req->item_num,
		(int)buy_publicsale_req->price);

	return true;
}

bool UserPublicSaleManager::OnGetPublicSaleItemList(Role *user)
{
	if (NULL == user)
	{
		return false;
	}
	
	static Protocol::SCGetPublicSaleItemListAck publicsale_item_list_ack;

	UserPublicSale *user_publicsale = this->GetUserPublicSale(user->GetUID());
	if (NULL != user_publicsale)
	{
		user_publicsale->OnGetPublicSaleItemList(publicsale_item_list_ack);
	}
	else 
	{
		publicsale_item_list_ack.count = 0;
	}

	int sendlen = sizeof(publicsale_item_list_ack) - (PUBLICSALE_MAX_ITEM_COUNT - publicsale_item_list_ack.count) * sizeof(SaleItemParam);
	EngineAdapter::Instance().NetSend(user->GetNetId(), (const char*)&publicsale_item_list_ack, sendlen);

	return true;
}

bool UserPublicSaleManager::OnSendPublicSaleItemInfoToWorld(Role *user, gglobalprotocal::PublicSaleSendItemInfoToWorld *send_iteminfo_toworld_req)
{
	if (NULL == user)
	{
		return false;
	}

	if (NULL == send_iteminfo_toworld_req || send_iteminfo_toworld_req->db_index < 0 || send_iteminfo_toworld_req->role_id <= 0 ||
		send_iteminfo_toworld_req->sale_index < 0 || send_iteminfo_toworld_req->sale_index >= PUBLICSALE_MAX_ITEM_COUNT) return false;

	const char *log_ret = "";

	UserPublicSale *user_publicsale = this->GetUserPublicSale(user->GetUID());
	if (NULL != user_publicsale && user_publicsale->OnSendPublicSaleItemInfoToWorld(send_iteminfo_toworld_req))
	{
		log_ret = "succ";
	}
	else
	{
		log_ret = "fail";
	}

	gamelog::g_log_publicsale.printf(LL_INFO, "public_sale -> broadcast item info [ ret:%s, user[ uid:%d ], item [ sale_index:%d ] ",
		log_ret, user->GetUID(), send_iteminfo_toworld_req->sale_index);

	return true;
}

bool UserPublicSaleManager::OnPublicSaleSearch(Role *user, Protocol::CSPublicSaleSearch *publicsale_search_req)
{
	if (NULL == user || NULL == publicsale_search_req || publicsale_search_req->total_page < 0 || publicsale_search_req->req_page < 1 || 
		(publicsale_search_req->total_page > 0 && publicsale_search_req->req_page > publicsale_search_req->total_page)) return false;

	return m_publicsale_index.OnSearch(user, publicsale_search_req);
}

UserPublicSale * UserPublicSaleManager::GetUserPublicSale(int uid)
{
	UserPublicSaleMapIt it = m_user_publicsale_map.find(uid);
	if (it == m_user_publicsale_map.end())
	{
		return NULL;
	}

	return it->second;
}

UserPublicSale * UserPublicSaleManager::CreateUserPublicSale(int uid)
{
	UserPublicSaleMapIt it = m_user_publicsale_map.find(uid);
	if (it == m_user_publicsale_map.end())
	{
		UserPublicSale *user_publicsale = new UserPublicSale(this, uid);
		m_user_publicsale_map[uid] = user_publicsale;
		return user_publicsale;
	}
	
	return it->second;
}

bool UserPublicSaleManager::IsLoadFinish()
{
	return (PUBLICSALE_DATA_STATE_LOAD_FINISH == m_data_state);
}

void UserPublicSaleManager::SendMailToRole(const UserID &user_id, const gglobalprotocal::BuyPublicSaleItem *buy_publicsale_req, int reason)
{
	if (NULL == buy_publicsale_req) return;

	static SaleItemParam sale_item_param;
	sale_item_param.item_wrapper.Reset();
	sale_item_param.item_wrapper.item_id = buy_publicsale_req->item_id;
	sale_item_param.item_wrapper.num = buy_publicsale_req->item_num;

	sale_item_param.price = buy_publicsale_req->price;
	sale_item_param.sale_value = buy_publicsale_req->sale_value;
	sale_item_param.sale_item_type = buy_publicsale_req->sale_item_type;
	sale_item_param.price_type = buy_publicsale_req->price_type;

	this->SendMailToRole(user_id, &sale_item_param, reason);
}

void UserPublicSaleManager::SendMailToRole(const UserID &user_id, const SaleItemParam *sale_item_param, int reason)
{
	if (NULL == sale_item_param) return;
	if (reason < SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_ITEM_FULL || reason > SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_MONEY_SALE_SUCC) return;

	static MailContentParam contentparam; contentparam.Reset();
	static MailSubject subject = {0}; static MailContentTxt contenttxt = {0};
	int sub_len = 0;
	int cont_len = 0;
	int item_id = 0;
	int num = 0;
	int client_money_type = 0;

	int price = sale_item_param->price;
	int expenses = 0;
	float coin_expenses_rate = 0.0f;
	float gold_expense_rate = 0.0f;

	if (PUBLIC_SALE_PRICE_TYPE_COIN == sale_item_param->price_type)
	{
		client_money_type = 2;
	}

	if (PUBLIC_SALE_PRICE_TYPE_GOLD == sale_item_param->price_type)
	{
		client_money_type = 3;
	}

	if (SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_ITEM_FULL == reason ||				 // 出售的物品已满 退还
		SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_ITEM_CANCEL == reason ||				 // 取消拍卖 
		SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_ITEM_EXPIRE == reason)				// 过期
	{
		WRAP_GIVE_SALE_ITEM(item_id, num, sale_item_param, contentparam);
		sub_len = SNPRINTF(subject, sizeof(subject), GetMailReasonSubject(reason));
		cont_len = SNPRINTF(contenttxt, sizeof(contenttxt), GetMailReasonContent(reason), item_id, num);
	}
	
	if (SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_ITEM_BUY_SUCC == reason)				 // 购买成功
	{	
		WRAP_GIVE_SALE_ITEM(item_id, num, sale_item_param, contentparam);
		sub_len = SNPRINTF(subject, sizeof(subject), GetMailReasonSubject(reason));
		cont_len = SNPRINTF(contenttxt, sizeof(contenttxt), GetMailReasonContent(reason), item_id, num, price, client_money_type);
	}

	if (SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_MONEY_BUY_FAIL == reason)			// 购买失败通知 
	{
		WRAP_GIVE_BACK_SELL_PRICE(item_id, num, sale_item_param, contentparam, price, expenses, coin_expenses_rate, gold_expense_rate);
		sub_len = SNPRINTF(subject, sizeof(subject), GetMailReasonSubject(reason));
		cont_len = SNPRINTF(contenttxt, sizeof(contenttxt), GetMailReasonContent(reason), item_id, num, price, client_money_type);
	}

	if (SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_MONEY_SALE_SUCC == reason)			// 通知出售人，物品已被购
	{
		coin_expenses_rate = 0.05f;

		int temp_tax = GLOBALCONFIG->GetPublicSaleOnceTax(price);
		gold_expense_rate = temp_tax * 0.01f;

		WRAP_GIVE_BACK_SELL_PRICE(item_id, num, sale_item_param, contentparam, price, expenses, coin_expenses_rate, gold_expense_rate);
		sub_len = SNPRINTF(subject, sizeof(subject), GetMailReasonSubject(reason));
		
		int show_expense_tax = 0;
		char temp_show_expense_rate[64] = {0};

		if (PUBLIC_SALE_PRICE_TYPE_COIN == sale_item_param->price_type)
		{
			show_expense_tax = static_cast<int>(coin_expenses_rate * 100);
		}
		else if (PUBLIC_SALE_PRICE_TYPE_GOLD == sale_item_param->price_type)
		{
			show_expense_tax = temp_tax;
		}

		SNPRINTF(temp_show_expense_rate, sizeof(temp_show_expense_rate), "%d%%", show_expense_tax);

		cont_len = SNPRINTF(contenttxt, sizeof(contenttxt), GetMailReasonContent(reason), item_id, num, temp_show_expense_rate, price, client_money_type);
	}

 	if (sub_len > 0 && cont_len > 0)
 	{
 		F_STRNCPY(contentparam.subject, subject, sizeof(contentparam.subject));
 		F_STRNCPY(contentparam.contenttxt, contenttxt, sizeof(contentparam.contenttxt));
 
 		UserMailManager::Instance().MailToUser(MAIL_TYPE_SYSTEM, user_id, 0, contentparam);
 	}
}

void UserPublicSaleManager::OnPublicSaleTypeCountReq(Role *role)
{
	m_publicsale_index.OnPublicSaleTypeCountReq(role);
}
