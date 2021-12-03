#include "publicsaleindex.hpp"
#include "publicsaleitem.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/translateint.h"

#include "gamelog.h"
#include "engineadapter.h"

#include "obj/character/role.h"
#include "protocal/msgscpublicsale.h"

PublicSaleIndex::PublicSaleIndex()
{

}

PublicSaleIndex::~PublicSaleIndex()
{

}

bool PublicSaleIndex::OnSearch(Role *user, Protocol::CSPublicSaleSearch *search_req)
{
	if (NULL == user || search_req->page_item_count <= 0 || search_req->page_item_count > Protocol::PUBLICSALE_PAGE_ITEM_COUNT_MAX)
	{
		return false;
	}

	static Protocol::SCPublicSaleSearchAck search_ack;
	search_ack.count = 0;
	search_ack.cur_page = search_req->req_page;
	search_ack.total_page = search_req->total_page;

	int match_index = 0;

	if (0 == search_req->item_type)  
	{
		if (0 != search_req->fuzzy_type_count) 
		{
			bool continue_search = true;
			for (int fuzzy_idx = 0; continue_search && fuzzy_idx < search_req->fuzzy_type_count && fuzzy_idx < Protocol::FUZZY_SEARCH_ITEM_TYPE_COUNT; 
				++fuzzy_idx)
			{
				TypeIndexMapIt typeindex_it = m_type_index_map.find(search_req->fuzzy_type_list[fuzzy_idx].item_sale_type);
				if (typeindex_it == m_type_index_map.end()) continue;

				continue_search = this->SearchHelper(typeindex_it->second, search_req, &search_ack, &match_index, fuzzy_idx);
			}
		}
		else
		{
			this->SearchHelper(m_all_index_map, search_req, &search_ack, &match_index);
		}
	}
	else 
	{
		if (0 != search_req->fuzzy_type_count)
		{
			bool continue_search = true;
			for (int fuzzy_idx = 0; continue_search && fuzzy_idx < search_req->fuzzy_type_count && fuzzy_idx < Protocol::FUZZY_SEARCH_ITEM_TYPE_COUNT; 
				++fuzzy_idx)
			{
				TypeIndexMapIt typeindex_it = m_type_index_map.find(search_req->fuzzy_type_list[fuzzy_idx].item_sale_type);
				if (typeindex_it == m_type_index_map.end()) continue;

				continue_search = this->SearchHelper(typeindex_it->second, search_req, &search_ack, &match_index, fuzzy_idx);
			}
		}
		else
		{
			TypeIndexMapIt typeindex_it = m_type_index_map.find(search_req->item_type);
			if (typeindex_it != m_type_index_map.end())
			{
				this->SearchHelper(typeindex_it->second, search_req, &search_ack, &match_index);
			}
		}
	}

	if (0 == search_req->total_page && search_ack.count > 0) 
	{
		search_ack.total_page = (match_index + search_req->page_item_count - 1) / search_req->page_item_count;  
	}
	
	int sendlen = sizeof(search_ack) - (search_req->page_item_count - search_ack.count) * sizeof(Protocol::SCPublicSaleSearchAck::SaleItem);
	EngineAdapter::Instance().NetSend(user->GetNetId(), (const char*)&search_ack, sendlen);
	
	return true;
}

bool PublicSaleIndex::OnAddPublicSaleItem(int uid, PublicSaleItem *sale_item)
{
	PublicSaleItemIndex index_key = this->MakeIndexKey(uid, sale_item->item_param.sale_time, sale_item->item_param.price, sale_item->item_param.item_wrapper.item_id);

	ItemIndexMapIt itemindex_it = m_all_index_map.find(index_key);
	if (itemindex_it != m_all_index_map.end()) 
	{
		gamelog::g_log_serious_error.printf(LL_CRITICAL, "[PublicSaleIndex::IndexAddPublicSaleItem INDEX KEY REPEAT] [uid:%d sale_time:%u]", 
			uid, (unsigned int)sale_item->item_param.sale_time);

		return false;
	}

	m_all_index_map[index_key] = sale_item;

	TypeIndexMapIt typeindex_it = m_type_index_map.find(sale_item->item_param.sale_type);
	if (typeindex_it == m_type_index_map.end())
	{
		ItemIndexMap typeindex_map;
		typeindex_map[index_key] = sale_item;
		m_type_index_map[sale_item->item_param.sale_type] = typeindex_map;
	}
	else 
	{
		typeindex_it->second[index_key] = sale_item;
	}

	m_sale_type_count_map[sale_item->item_param.sale_type]++;

	return true;
}

void PublicSaleIndex::OnRemovePublicSaleItem(int uid, PublicSaleItem *sale_item)
{
	m_sale_type_count_map[sale_item->item_param.sale_type]--;

	PublicSaleItemIndex index_key = this->MakeIndexKey(uid, sale_item->item_param.sale_time, sale_item->item_param.price, sale_item->item_param.item_wrapper.item_id);

	m_all_index_map.erase(index_key);

	TypeIndexMapIt typeindex_it = m_type_index_map.find(sale_item->item_param.sale_type);
	if (typeindex_it != m_type_index_map.end()) 
	{
		typeindex_it->second.erase(index_key);
		if (typeindex_it->second.empty())
		{
			m_type_index_map.erase(typeindex_it);
		}
	}
}

void PublicSaleIndex::OnPublicSaleTypeCountReq(Role *role)
{
	static Protocol::SCPublicSaleTypeCountAck pro;
	pro.count = 0;

	for (std::map<int, int>::iterator it = m_sale_type_count_map.begin(); it != m_sale_type_count_map.end(); it++)
	{
		if (it->second > 0 && pro.count < Protocol::SCPublicSaleTypeCountAck::SALE_TYPE_COUNT_MAX)
		{
			pro.info_list[pro.count].sale_type = it->first;
			pro.info_list[pro.count].item_count = it->second;
			pro.count++;
		}
	}

	unsigned int length = sizeof(pro) - (Protocol::SCPublicSaleTypeCountAck::SALE_TYPE_COUNT_MAX - pro.count) * sizeof(pro.info_list[0]);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&pro, length);
}

PublicSaleItemIndex PublicSaleIndex::MakeIndexKey(int uid, unsigned int sale_time, int price, ItemID item_id) // 用时间S 跟角色ID 做Key 角色不可能在1S内放入两件物品的 
{
	PublicSaleItemIndex sale_item_index;
	sale_item_index.uid = uid;
	sale_item_index.sale_time = sale_time;
	sale_item_index.price = price;
	sale_item_index.item_id = item_id;
	
	return sale_item_index; 
}

static int IDCmp(const void *pvData1, const void *pvData2)
{
	if (NULL == pvData1 || NULL == pvData2) return 0;

	int *left = (int*)pvData1;
	int *right = (int*)pvData2;
	return *left - *right;
}

bool PublicSaleIndex::MatchIDSet(int item_id, int id_set_count, int *id_list)
{
	if (id_set_count <= 0 || NULL == id_list) return false;
	return NULL != bsearch(&item_id, &id_list[0], id_set_count, sizeof(int), &IDCmp);
}

bool PublicSaleIndex::SearchHelper(ItemIndexMap &itemindex_map, Protocol::CSPublicSaleSearch *search_req, Protocol::SCPublicSaleSearchAck *search_ack, 
								   int *match_index, int fuzzy_idx)
{
	if (NULL == search_req || NULL == search_ack || NULL == match_index || search_req->page_item_count <= 0) return false;

	for (ItemIndexMapIt itemindex_it = itemindex_map.begin(), timeindex_end = itemindex_map.end(); itemindex_it != timeindex_end; ++ itemindex_it)
	{
		PublicSaleItem *sale_item = itemindex_it->second;
		if (NULL == sale_item) continue;

		if (!sale_item->CondMatch(search_req->level, search_req->level_interval, search_req->prof,
			search_req->color, search_req->color_interval, search_req->order, search_req->order_interval)) continue;

		if (fuzzy_idx >= 0 && 
			!this->MatchIDSet(sale_item->item_param.item_wrapper.item_id, search_req->fuzzy_type_list[fuzzy_idx].item_count, &search_req->fuzzy_type_list[fuzzy_idx].item_id_list[0])) 
		{
			continue;
		}

		++ (*match_index);

		if (*match_index > (search_req->req_page - 1) * search_req->page_item_count && search_ack->count < Protocol::PUBLICSALE_PAGE_ITEM_COUNT_MAX)
		{
			search_ack->saleitem_list[search_ack->count].seller_uid = UidToInt(sale_item->m_seller_user_id);
			F_STRNCPY(search_ack->saleitem_list[search_ack->count].seller_name, sale_item->m_seller_name, 
				sizeof(search_ack->saleitem_list[0].seller_name));
			search_ack->saleitem_list[search_ack->count].item_param = sale_item->item_param;

			++ search_ack->count;

			if (search_ack->count >= Protocol::PUBLICSALE_PAGE_ITEM_COUNT_MAX && 0 != search_req->total_page) return false;
		}
	}

	return true;
}

