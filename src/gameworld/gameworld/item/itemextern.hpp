#ifndef __ITEM_EXTERN_H__
#define __ITEM_EXTERN_H__

#include "servercommon/basedef.h"
#include "servercommon/serverdef.h"
#include "servercommon/struct/itemlistparam.h"
#include <map>
#include <list>

class Role;
struct ItemGridData;

enum class COIN_TYPE {
	BIND_COIN,
	UNBIND_COIN,
	BIND_GOLD,
	UNBIND_GOLD,
	MOJING,
};


class ItemExtern
{
public:
	struct ItemConsumeConfig
	{
		ItemConsumeConfig() : item_id(0), buyable(false), num(0) {}
		ItemConsumeConfig(ItemID _item_id, bool _buyable, int _num) : item_id(_item_id), buyable(_buyable), num(_num) {}

		ItemID item_id;
		bool buyable;
		int num;
	};

	struct ItemConsumeConfigList
	{
		void add_item(const ItemConsumeConfig& config);
		void add_coin(COIN_TYPE typ, Int64 num);
		bool done(Role& role, std::string);
		bool check(Role& role);
	private:
		bool checkitem(Role& role);
		bool checkcoin(Role& role);
		int consume_item_by_bagindex(Role& role, size_t index, short max_count, std::string);
	private:
		struct item_num_info {
			item_num_info(int n, bool b) :count(n), buyable(b) {}
			int count;
			bool buyable;

			std::list<unsigned short> m_item_index;
		};

		std::map<COIN_TYPE, Int64> m_coin_list;
		std::map<ItemID, item_num_info> m_item_list;

	};

	struct ItemConsumeInfo
	{
		ItemConsumeInfo() { this->Reset(); }

		void Reset()
		{
			consume_bind_item_num = 0;
			consume_nobind_item_num = 0;
			memset(consume_num_list, 0, sizeof(consume_num_list));
		}

		int consume_bind_item_num;
		int consume_nobind_item_num;
		short consume_num_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];
	};

	struct BuyItemInfo
	{
		BuyItemInfo() { this->Reset(); }

		void Reset() { item_id = 0; item_count = 0; consume_gold = 0; consume_bind_gold = 0;}

		void Set(const BuyItemInfo &item_info)
		{
			item_id = item_info.item_id;
			item_count = item_info.item_count;
			consume_gold = item_info.consume_gold;
			consume_bind_gold = item_info.consume_bind_gold;
		}

		ItemID item_id;
		short item_count;
		int consume_gold;
		int consume_bind_gold;
	};

	struct ItemConsumeList
	{
		ItemConsumeList() { this->Reset(); }

		void Reset()
		{
			bind_item_num = 0;
			nobind_item_num = 0;
			need_gold_num = 0;
			need_bind_gold_num = 0;
			real_need_gold_num = 0;
			real_need_bind_gold_num = 0;

			cost_bindcoin_first = true;
			count = 0;
			memset(index_list, 0, sizeof(index_list));
			memset(num_list, 0, sizeof(num_list));
			buy_count = 0;
			for (int i = 0; i < (int)(sizeof(buy_list) / sizeof(buy_list[0])); ++i)
			{
				buy_list[i].Reset();
			}
		}

		void Set(const ItemConsumeList &consume_list)
		{
			bind_item_num = consume_list.bind_item_num;
			nobind_item_num = consume_list.nobind_item_num;
			need_gold_num = consume_list.need_gold_num;
			need_bind_gold_num = consume_list.need_bind_gold_num;
			real_need_gold_num = consume_list.real_need_gold_num;
			real_need_bind_gold_num = consume_list.real_need_bind_gold_num;

			cost_bindcoin_first = consume_list.cost_bindcoin_first;
			count = consume_list.count;
			UNSTD_STATIC_CHECK(sizeof(index_list) == sizeof(consume_list.index_list));
			memcpy(index_list, consume_list.index_list, sizeof(index_list));
			UNSTD_STATIC_CHECK(sizeof(num_list) == sizeof(consume_list.num_list));
			memcpy(num_list, consume_list.num_list, sizeof(num_list));
			buy_count = consume_list.buy_count;

			UNSTD_STATIC_CHECK(sizeof(buy_list) == sizeof(consume_list.buy_list));
			for (int i = 0; i < (int)(sizeof(buy_list) / sizeof(buy_list[0])); ++i)
			{
				buy_list[i].Set(consume_list.buy_list[i]);
			}
		}

		int bind_item_num;
		int nobind_item_num;
		int need_gold_num;
		int need_bind_gold_num;
		int real_need_gold_num;
		int real_need_bind_gold_num;

		bool cost_bindcoin_first;
		short count;
		short index_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];
		short num_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];
		short buy_count;
		BuyItemInfo buy_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	};

	struct ItemList
	{
		ItemList() { this->Reset(); }

		void Reset()
		{
			bind_count = 0;
			memset(bind_index_list, 0, sizeof(bind_index_list));
			memset(bind_num_list, 0, sizeof(bind_num_list));
			nobind_count = 0;
			memset(nobind_index_list, 0, sizeof(nobind_index_list));
			memset(nobind_num_list, 0, sizeof(nobind_num_list));
		}

		int GetTotalCount()
		{
			int count = 0;
			for (int i = 0; i < bind_count; ++i) count += bind_num_list[i];
			for (int i = 0; i < nobind_count; ++i) count += nobind_num_list[i];
			return count;
		}

		int GetNobindCount()
		{
			int count = 0;
			for (int i = 0; i < nobind_count; ++i) count += nobind_num_list[i];
			return count;
		}

		int GetBindCount()
		{
			int count = 0;
			for (int i = 0; i < bind_count; ++i) count += bind_num_list[i];
			return count;
		}

		short bind_count;
		short bind_index_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];
		short bind_num_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];

		short nobind_count;
		short nobind_index_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];
		short nobind_num_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];
	};

	static bool CheckIndexListNoDuplicate(short index_list_num, short *index_list);

	static bool CheckItemByIndexList(Role *role, ItemID item_id, short check_num, short index_list_num, short *index_list, ItemConsumeInfo *item_check_info, short *consume_num = NULL);
	//static bool CheckItemByIndex(Role *role, ItemID item_id, short check_num, short index_num, ItemGridData *item_data);

	static bool PutInItemConfigList(int list_num, ItemConfigData *config_list, int *index_count, ItemID item_id, int item_num, bool is_bind);

	static int GetGoldPriceByNum(ItemID item_id, short total_num, int total_gold_price, short num);

	//获取消耗列表， 与 ConsumeItem() 配套使用
	static bool GetConsumeListByOrder(Role *role, bool nobind_first, int stuff_num, ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM], ItemConsumeList *result_list, bool notify_lack = true);

	static bool GetItemDataWrapperFromConfig(const ItemConfigData *item_cfg, ItemDataWrapper *item_wrapper);

	static void SendRewardListInfo(Role *role, ItemConfigData *reward_list, int reward_num, int notice_type = 0, int mojing = 0);

	static bool ConsumeItem(Role *role, ItemConsumeList & consume_list, const char * reason);

	static bool CheckBindGoldAndGoldByConsumeList(Role *role, const ItemConsumeList & consume_list);		// 根据消耗列表检查绑元和元宝

private:
	static bool SelectItemByOrderHelper(ItemList *item_list, int need_count, ItemConsumeList *result_list, bool nobind_first);
};

extern ItemExtern::ItemConsumeList g_consume_list; 

#endif

