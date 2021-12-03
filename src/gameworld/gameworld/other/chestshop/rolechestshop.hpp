#ifndef __ROLE_CHEST_SHOP_HPP__
#define __ROLE_CHEST_SHOP_HPP__

#include "protocal/msgshop.h"
#include "chestshop.hpp"

class Role;
struct ItemDataWrapper;

class RoleChestShop
{
public:
	struct ChestItem
	{
		ChestItem()
		{
			this->Reset();
		}

		bool operator <(const ChestItem &other_item) const
		{
			return item_id > other_item.item_id;
		}

		bool Invalid() { return 0 == item_id || 0 == num; }
		void Reset() 
		{ 
			item_id = 0; 
			num = 0; 
			is_bind = 0; 
			storehouse_type = 0; 
			index = 0; 
			has_lucky = 0;
			strength = 0;
			reserve2 = 0;
			reserve3 = 0;
			memset(xianpin_type_list, 0, sizeof(xianpin_type_list));
		}

		void Reset(ChestItem &other)
		{ 
			item_id = other.item_id;
			num = other.num;
			is_bind = other.is_bind;
			storehouse_type = other.storehouse_type;
			index = other.index;
			has_lucky = other.has_lucky;
			strength = other.strength;
			reserve2 = other.reserve2;
			reserve3 = other.reserve3;
			memcpy(xianpin_type_list, other.xianpin_type_list, sizeof(xianpin_type_list));
		}

		UInt16 item_id;
		short num;

		char is_bind;
		char storehouse_type;
		short index;
		
		short has_lucky;
		short strength;
		
		char reserve2;
		char reserve3;
		char xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM];	//6

		UNSTD_STATIC_CHECK(6 == ItemNamespace::MAX_XIANPIN_NUM);
	};

	struct ChestItemSort
	{
		bool operator <(const ChestItemSort &other_item) const 
		{
			return color > other_item.color;
		}

		int index = -1;
		int color = 0;
	};

	RoleChestShop();
	~RoleChestShop();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const char *buff, int buff_len);
	void GetInitParam(char *buff, int *buff_len);

	bool CheckHaveSpace(int shop_type, int item_count);
	void OnGetItemList(char shop_type);
	bool OnFetchItem(char shop_type, int grid_index);
	bool OnFetchAllItems(char shop_type);
	bool PutToKnapack(ChestItem &chest_item);
	
	bool OnBuyItemList(char shop_type, int list_num, ChestItemInfo item_list[MODE_MAX_COUNT], const char *reason);

	int GetStorehouseType(int shop_type);					// 获取仓库类型
	void GetFetchItemIndexList(std::vector<int> &index_list, int store_type, int put_count_limit); // 获取排序后的index列表

	bool ResetStoreData(int index, int shop_type);		// 清除格子数据
	void OrderStore();															// 整理仓库

	void OnAutoRecycle(int shop_type, int color);			// 精灵批量回收
	void OnRecycleOne(int shop_type, int grid_index);		// 精灵回收
	void SendFreeInfo();
	void SendScoreInfo();
	void SendScoreInfoNotice(int mojing = 0, int shengwang = 0, int gongxun = 0, int weiwang = 0, int treasure_credit = 0, int jingling_credit = 0, 
		int happytree_grow = 0, int mysterious_shop_credit = 0, int blue_lingzhi = 0, int purple_lingzhi = 0, int orange_lingzhi = 0, int guanghui = 0, 
		int precious_boss_score = 0, int shenzhou_weapon_score = 0, int treasure_credit1 = 0, int treasure_credit2 = 0, int treasure_credit3 = 0, int zhuanzhi_stone_score = 0, int chest_shop_hunjing = 0);

private:

	Role *m_role;

	int m_count;
	int m_count_list[CHEST_SHOP_TYPE_MAX];								// 各商店类型的物品数量
	int m_storehouse_count_list[CHEST_SHOP_STOREHOUSE_TYPE_MAX];		// 各仓库类型的物品数量
	ChestItem m_chest_item_list[MAX_ROLE_CHEST_GRID];
};

#endif  // __ROLE_CHEST_SHOP_HPP__

