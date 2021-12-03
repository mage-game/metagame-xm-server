/******************************************************************************************
FileName: publicsaleindex.hpp
Author: dreamwang
Description: 拍卖市场索引
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/30/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __PUBLIC_SALE_INDEX_HPP__
#define __PUBLIC_SALE_INDEX_HPP__

#include <map>

#include "servercommon/serverdef.h"
#include "protocal/msgscpublicsale.h"

class PublicSaleItem;
class UserPublicSaleManager;
class Role;

struct PublicSaleItemIndex
{
	PublicSaleItemIndex() : uid(0), sale_time(0), price(0) {}

	bool operator<(const PublicSaleItemIndex &sale_item_index) const
	{
		if (sale_item_index.item_id != item_id)
		{
			return item_id < sale_item_index.item_id;
		}
		else if (sale_item_index.price != price)
		{
			return price < sale_item_index.price;
		}
		else if (sale_item_index.sale_time != sale_time)
		{
			return sale_time > sale_item_index.sale_time;
		}

		return uid < sale_item_index.uid;
	}

	int uid;
	unsigned int sale_time;
	int price;
	ItemID item_id;
};

class PublicSaleIndex
{
public:
	PublicSaleIndex();
	~PublicSaleIndex();

	bool OnSearch(Role *user, Protocol::CSPublicSaleSearch *search_req);

	bool OnAddPublicSaleItem(int uid, PublicSaleItem *sale_item);
	void OnRemovePublicSaleItem(int uid, PublicSaleItem *sale_item);

	void OnPublicSaleTypeCountReq(Role *role);

private:
	PublicSaleItemIndex MakeIndexKey(int uid, unsigned int sale_time, int price, ItemID item_id);
	bool MatchIDSet(int item_id, int id_set_count, int *id_list);

	typedef std::map<PublicSaleItemIndex, PublicSaleItem*> ItemIndexMap;
	typedef std::map<PublicSaleItemIndex, PublicSaleItem*>::iterator ItemIndexMapIt;

	typedef std::map<int, ItemIndexMap> TypeIndexMap;
	typedef std::map<int, ItemIndexMap>::iterator TypeIndexMapIt;

	bool SearchHelper(ItemIndexMap &timeindex_map, Protocol::CSPublicSaleSearch *search_req, Protocol::SCPublicSaleSearchAck *search_ack, 
		int *match_index, int fuzzy_idx = -1);

	ItemIndexMap m_all_index_map;
	TypeIndexMap m_type_index_map;

	std::map<int, int> m_sale_type_count_map;
};

#endif  // __PUBLIC_SALE_INDEX_HPP__

