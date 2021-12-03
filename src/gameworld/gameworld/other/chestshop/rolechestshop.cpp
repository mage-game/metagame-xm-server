#include "rolechestshop.hpp"

#include "gamelog.h"
#include "engineadapter.h"

#include "obj/character/role.h"

#include "item/itemextern.hpp"
#include "item/knapsack.h"
#include "item/itempool.h"
#include "equipment/equipment.h"

#include "servercommon/errornum.h"
#include "servercommon/struct/roleotherinitparam.hpp"
#include "other/shop/roleshop.hpp"
#include "other/zhuanzhiequip/zhuanzhiequip.h"
#include <algorithm>

RoleChestShop::RoleChestShop()
	: m_role(NULL), m_count(0)
{
	memset(m_count_list, 0, sizeof(m_count_list));
	memset(m_storehouse_count_list, 0, sizeof(m_storehouse_count_list));
}

RoleChestShop::~RoleChestShop()
{

}

void RoleChestShop::Init(Role *role, const char *buff, int buff_len)
{
	UNSTD_STATIC_CHECK((int)sizeof(m_chest_item_list) + sizeof(int) <= RoleOtherInitParam::ROLE_CHEST_SHOP_BUFF_LENGTH);

	m_role = role;

	if (buff_len < (int)sizeof(int)) { m_count = 0; return; }

	m_count = *((int*)buff); buff += sizeof(int); buff_len -= sizeof(int);

	int copy_len = sizeof(ChestItem) * m_count; 
	if (copy_len != buff_len) { m_count = 0; return;}
	memcpy(m_chest_item_list, buff, copy_len); buff += copy_len; buff_len -= copy_len;

	memset(m_count_list, 0, sizeof(m_count_list));
	memset(m_storehouse_count_list, 0, sizeof(m_storehouse_count_list));
	for (int i = 0; i < m_count; ++ i)
	{
// 		int shop_type = m_chest_item_list[i].storehouse_type;
// 		if (shop_type > CHEST_SHOP_TYPE_INVALID && shop_type < CHEST_SHOP_TYPE_MAX)
// 		{
// 			++ m_count_list[shop_type];	
// 		}

		// 由商店类型改为仓库类型
		int storehouse_type = m_chest_item_list[i].storehouse_type;
		if (storehouse_type > CHEST_SHOP_STOREHOUSE_TYPE_INVALID && storehouse_type < CHEST_SHOP_STOREHOUSE_TYPE_MAX)
		{
			++m_storehouse_count_list[storehouse_type];
		}
	}
}

void RoleChestShop::GetInitParam(char *buff, int *buff_len)
{
	*buff_len = 0;

	*((int*)buff) = m_count; buff += sizeof(int); *buff_len += sizeof(int);

	int copy_len = sizeof(ChestItem) * m_count; 
	memcpy(buff, m_chest_item_list, copy_len); buff += copy_len; *buff_len += copy_len; 
}

bool RoleChestShop::CheckHaveSpace(int shop_type, int item_count)
{
	if (shop_type > CHEST_SHOP_TYPE_INVALID && shop_type < CHEST_SHOP_TYPE_MAX)
	{
		int storehouse_type = this->GetStorehouseType(shop_type);
		return item_count <= MAX_ROLE_CHEST_GRID_HALF - m_storehouse_count_list[storehouse_type];
	}

	return false;
}

void RoleChestShop::OnGetItemList(char shop_type)
{
	static Protocol::SCSelfChestShopItemList info;
	int count = 0;

	int storehouse_type = this->GetStorehouseType(shop_type);

	for (int i = 0; i < m_count; i++)
	{
//		int item_storehouse_type = this->GetStorehouseType(m_chest_item_list[i].shop_type);
		if (storehouse_type == m_chest_item_list[i].storehouse_type)
		{
			info.item_list[count].item_id = m_chest_item_list[i].item_id; 
			info.item_list[count].num = m_chest_item_list[i].num; 
			info.item_list[count].is_bind = m_chest_item_list[i].is_bind;
			info.item_list[count].shop_type = m_chest_item_list[i].storehouse_type;
			info.item_list[count].index = m_chest_item_list[i].index;
			info.item_list[count].has_lucky = m_chest_item_list[i].has_lucky;
			info.item_list[count].strength = m_chest_item_list[i].strength;
			memcpy(info.item_list[count].xianpin_type_list, m_chest_item_list[i].xianpin_type_list, sizeof(info.item_list[count].xianpin_type_list));
			++ count;
		}
	}
	
	info.storehouse_type = storehouse_type;
	info.count = count;
	int sendLen = sizeof(info) - (MAX_ROLE_CHEST_GRID - info.count) * sizeof(ChestItemInfo);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&info, sendLen);
}

bool RoleChestShop::OnFetchItem(char shop_type, int grid_index)
{
	if (grid_index < 0 || grid_index >= m_count || m_chest_item_list[grid_index].Invalid() || shop_type < 0 || shop_type >= CHEST_SHOP_TYPE_MAX) 
	{
		return false;
	}
	
	ChestItem &chest_item = m_chest_item_list[grid_index];
	int item_storehouse_type = this->GetStorehouseType(shop_type);
	if (item_storehouse_type != chest_item.storehouse_type)
	{
		return false;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	if (!this->PutToKnapack(chest_item))
	{
		return false;
	}

	gamelog::g_log_chestshop.printf(LL_INFO, "[ChestShop::Fetch Succ][user[%d %s] item_id:%d num:%d is_bind:%d]", UidToInt(m_role->GetUserId()), 
		m_role->GetName(), chest_item.item_id, chest_item.num, chest_item.is_bind);

	if (grid_index != m_count - 1)
	{
		chest_item.Reset(m_chest_item_list[m_count-1]);
		chest_item.index = grid_index;
	}

	m_chest_item_list[m_count-1].Reset();

	-- m_count;
	if (shop_type > CHEST_SHOP_TYPE_INVALID && shop_type < CHEST_SHOP_TYPE_MAX)
	{
		-- m_count_list[(int)shop_type]; 

		int storehouse_type = this->GetStorehouseType(shop_type);
		-- m_storehouse_count_list[storehouse_type];
	}

	this->OnGetItemList(shop_type);

	return true;
}

bool RoleChestShop::OnFetchAllItems(char shop_type)
{
	if (m_count <= 0)
	{
		return false;
	}

	int storehouse_type = this->GetStorehouseType(shop_type);

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	int put_count = m_count;
	std::vector<int> put_index_vec;
 	this->GetFetchItemIndexList(put_index_vec, storehouse_type, put_count);

	for (auto &index : put_index_vec)
	{
		int empty_grid_num = m_role->GetKnapsack()->GetCurrKnapscakEmptyGridNum();
		if (empty_grid_num <= 0)
		{
			break;
		}

		if (index < 0 || index >= MAX_ROLE_CHEST_GRID)
		{
			continue;
		}

		if (!this->PutToKnapack(m_chest_item_list[index]))
		{
			continue;
		}

		this->ResetStoreData(index, shop_type);
	}

	// 整理
	this->OrderStore();

	this->OnGetItemList(shop_type);

	return true;
}

bool RoleChestShop::PutToKnapack(ChestItem &chest_item)
{
	int knapack_grid_index = -1;
	ItemConfigData item_config_data(chest_item.item_id, chest_item.is_bind != 0, chest_item.num);
	if (!m_role->GetKnapsack()->Put(item_config_data, PUT_REASON_CHEST_SHOP, &knapack_grid_index))
	{
		return false;
	}

	//赋值
	if (knapack_grid_index >= 0)
	{
		ItemGridData item_grid_data;
		if (m_role->GetKnapsack()->GetItemGridData(knapack_grid_index, &item_grid_data) && 
			item_grid_data.item_id == chest_item.item_id) //确保是同个物品
		{
			const ItemBase* item = ITEMPOOL->GetItem(item_grid_data.item_id);
			if (NULL != item && ItemBase::I_TYPE_EQUIPMENT == item->GetItemType())
			{
				EquipmentParam *equip_param = (EquipmentParam *)item_grid_data.param;
				equip_param->has_lucky = chest_item.has_lucky;
				equip_param->strengthen_level = chest_item.strength;
				for (int i = 0; i < ItemNamespace::MAX_XIANPIN_NUM; ++ i)
				{
					equip_param->xianpin_type_list[i] = static_cast<int>(chest_item.xianpin_type_list[i]);
				}
				m_role->GetKnapsack()->SetItemDirty(knapack_grid_index);
			}
		}
	}

	return true;
}

bool RoleChestShop::ResetStoreData(int index, int shop_type)
{
	if (index < 0 || index >= MAX_ROLE_CHEST_GRID)
	{
		return false;
	}

	ChestItem &chest_item = m_chest_item_list[index];

	if (chest_item.Invalid())
	{
		return false;
	}

	gamelog::g_log_chestshop.printf(LL_INFO, "[ChestShop::Fetch Succ][user[%d %s] item_id:%d num:%d is_bind:%d]", UidToInt(m_role->GetUserId()),
		m_role->GetName(), chest_item.item_id, chest_item.num, chest_item.is_bind);

	m_chest_item_list[index].Reset();

	--m_count;

	if (shop_type > CHEST_SHOP_TYPE_INVALID && shop_type < CHEST_SHOP_TYPE_MAX)
	{
		--m_count_list[(int)shop_type];

		int storehouse_type = this->GetStorehouseType(shop_type);
		--m_storehouse_count_list[storehouse_type];
	}

	return true;
}

void RoleChestShop::OrderStore()
{
	std::sort(std::begin(m_chest_item_list), std::end(m_chest_item_list));
}

bool SortFunc(const RoleChestShop::ChestItem &item_1, const RoleChestShop::ChestItem &item_2)
{
	if (item_1.item_id != item_2.item_id)
	{
		return item_1.item_id < item_2.item_id;
	}

	return item_1.num > item_2.num;
}

bool RoleChestShop::OnBuyItemList(char shop_type, int list_num, ChestItemInfo item_list[MODE_MAX_COUNT], const char *reason)
{
	if (NULL == reason)
	{
		return false;
	}

	int storehouse_type = this->GetStorehouseType(shop_type);

	ChestItem old_chest_item_list[MAX_ROLE_CHEST_GRID];
	memcpy(old_chest_item_list, m_chest_item_list, sizeof(old_chest_item_list));

	for (int i = 0; i < list_num; i++)
	{
		gamelog::g_log_chestshop.printf(LL_INFO, "[ChestShop:: %s][user[%d %s] item_id:%d num:%d is_bind:%d]",
			reason, UidToInt(m_role->GetUserId()), m_role->GetName(), item_list[i].item_id, item_list[i].num, (item_list[i].is_bind ? 1 : 0));

		for (int grid_i = 0; grid_i < m_count; grid_i++)
		{
			const ItemBase *itembase = ITEMPOOL->GetItem(m_chest_item_list[grid_i].item_id);
			if (NULL == itembase) continue;

			char is_bind = item_list[i].is_bind ? 1 : 0;
			if (m_chest_item_list[grid_i].storehouse_type == storehouse_type && m_chest_item_list[grid_i].item_id == item_list[i].item_id && m_chest_item_list[grid_i].is_bind == is_bind)
			{
				int can_put_num = itembase->GetPileLimit() - m_chest_item_list[grid_i].num;
				int real_put = item_list[i].num > can_put_num ? can_put_num : item_list[i].num;

				m_chest_item_list[grid_i].num += real_put;
				item_list[i].num -= real_put;

				if (item_list[i].num <= 0) break;
			}
		}

		if (item_list[i].num > 0)
		{
			const ItemBase *itembase = ITEMPOOL->GetItem(item_list[i].item_id);
			if (NULL == itembase) break;

			for (int grid_i = m_count; grid_i < MAX_ROLE_CHEST_GRID && item_list[i].num > 0; grid_i++)
			{
				if (item_list[i].num > itembase->GetPileLimit())
				{
					m_chest_item_list[m_count].item_id = item_list[i].item_id;
					m_chest_item_list[m_count].num = itembase->GetPileLimit();
					m_chest_item_list[m_count].is_bind = item_list[i].is_bind ? 1 : 0;
					m_chest_item_list[m_count].storehouse_type = storehouse_type;
					m_chest_item_list[m_count].has_lucky = item_list[i].has_lucky;
					m_chest_item_list[m_count].strength = item_list[i].strength;
					memcpy(m_chest_item_list[m_count].xianpin_type_list, item_list[i].xianpin_type_list, sizeof(m_chest_item_list[m_count].xianpin_type_list));
					
					++ m_count;
					if (shop_type > CHEST_SHOP_TYPE_INVALID && shop_type < CHEST_SHOP_TYPE_MAX)
					{
						++ m_count_list[(int)shop_type]; 

						int storehouse_type = this->GetStorehouseType(shop_type);
						++ m_storehouse_count_list[storehouse_type];
					}

					item_list[i].num -= itembase->GetPileLimit();
				}
				else
				{
					m_chest_item_list[m_count].item_id = item_list[i].item_id;
					m_chest_item_list[m_count].num = item_list[i].num;
					m_chest_item_list[m_count].is_bind = item_list[i].is_bind ? 1 : 0;
					m_chest_item_list[m_count].storehouse_type = storehouse_type;
					m_chest_item_list[m_count].has_lucky = item_list[i].has_lucky;
					m_chest_item_list[m_count].strength = item_list[i].strength;
					memcpy(m_chest_item_list[m_count].xianpin_type_list, item_list[i].xianpin_type_list, sizeof(m_chest_item_list[m_count].xianpin_type_list));
					
					++ m_count;
					if (shop_type > CHEST_SHOP_TYPE_INVALID && shop_type < CHEST_SHOP_TYPE_MAX)
					{
						++ m_count_list[(int)shop_type];

						int storehouse_type = this->GetStorehouseType(shop_type);
						++ m_storehouse_count_list[storehouse_type];
					}

					item_list[i].num = 0;
				}
			}
		}
	}

	std::sort(&m_chest_item_list[0], &m_chest_item_list[m_count], &SortFunc);

	for(int i = 0; i < m_count && i < MAX_ROLE_CHEST_GRID; ++ i)
	{
		m_chest_item_list[i].index = i;
	}

	this->OnGetItemList(shop_type);

	return true;
}

int RoleChestShop::GetStorehouseType(int shop_type)
{
	int storehouse_type = CHEST_SHOP_STOREHOUSE_TYPE_INVALID;

	if (CHEST_SHOP_TYPE_EQUIP == shop_type || CHEST_SHOP_TYPE_EQUIP1 == shop_type || CHEST_SHOP_TYPE_EQUIP2 == shop_type || CHEST_SHOP_TYPE_EQUIP3 == shop_type || CHEST_SHOP_TYPE_SHENZHOUWEAPON == shop_type)
	{
		storehouse_type = CHEST_SHOP_STOREHOUSE_TYPE_EQUIP;
	}

	if (CHEST_SHOP_TYPE_JINGLING == shop_type)
	{
		storehouse_type = CHEST_SHOP_STOREHOUSE_TYPE_JINGLING;
	}

	return storehouse_type;
}

void RoleChestShop::GetFetchItemIndexList(std::vector<int> &index_list, int store_type, int put_count_limit)
{
	if (put_count_limit <= 0)
	{
		return;
	}

	std::vector<ChestItemSort> vec;

	// 筛选
	for(int index = 0; index < m_count && index < MAX_ROLE_CHEST_GRID; index ++)
	{
		ChestItem &chest_item = m_chest_item_list[index];

		if (chest_item.Invalid() || store_type != chest_item.storehouse_type)
		{
			continue;
		}

		const ItemBase* item_base = ITEMPOOL->GetItem(chest_item.item_id);
		if (NULL == item_base)
		{
			continue;
		}

		ChestItemSort item;
		item.index = index;
		item.color = item_base->GetColor();

		vec.push_back(item);
	}

	// 按颜色排序
	std::stable_sort(vec.begin(), vec.end());

	int put_count = 0;
	for (auto &it : vec)
	{
		if (put_count >= put_count_limit)
		{
			break;
		}

		index_list.push_back(it.index);
		put_count++;
	}
}

void RoleChestShop::OnAutoRecycle(int shop_type, int color)
{
	if (CHEST_SHOP_TYPE_JINGLING != shop_type)
	{
		return;
	}

	if (color <= ItemBase::I_COLOR_INVALID || color >= ItemBase::I_COLOR_MAX)
	{
		return;
	}

	int storehouse_type = this->GetStorehouseType(shop_type);

	int max_loop_count = m_count;
	int loop_count = 0;

	int index = 0;
	int add_lingjing = 0;

	while (loop_count < max_loop_count)
	{
		++ loop_count;

		if (index >= MAX_ROLE_CHEST_GRID)
		{
			break;
		}

		ChestItem &chest_item = m_chest_item_list[index];
		if (chest_item.Invalid() || storehouse_type != chest_item.storehouse_type)
		{
			++ index;
			continue;
		}

		const ItemBase* item_base = ITEMPOOL->GetItem(chest_item.item_id);
		if (NULL == item_base || item_base->GetColor() > color || ItemBase::I_TYPE_EQUIPMENT != item_base->GetItemType())
		{
			++ index;
			continue;
		}

		chest_item.Reset(m_chest_item_list[m_count - 1]);
		chest_item.index = index;

		m_chest_item_list[m_count - 1].Reset();
		
		-- m_count;
		if (shop_type > CHEST_SHOP_TYPE_INVALID && shop_type < CHEST_SHOP_TYPE_MAX)
		{
			-- m_count_list[(int)shop_type]; 

			int storehouse_type = this->GetStorehouseType(shop_type);
			--m_storehouse_count_list[storehouse_type];
		}

		// 回收
		if (RECYCL_LINGJING == item_base->GetRecyclType(NULL))
		{
			add_lingjing += item_base->GetRecyclGet(NULL);
		}
	}

	if (add_lingjing > 0)
	{
		m_role->AddLingJing(add_lingjing, "AutoRecycle In ChestShop");
	}

	this->OnGetItemList(shop_type);
}

void RoleChestShop::OnRecycleOne(int shop_type, int grid_index)
{
	if (CHEST_SHOP_TYPE_JINGLING != shop_type) return;

	int storehouse_type = this->GetStorehouseType(shop_type);

	if (grid_index < 0 || grid_index >= m_count || m_chest_item_list[grid_index].Invalid() || shop_type <= 0 || shop_type >= CHEST_SHOP_TYPE_MAX) 
	{
		return;
	}

	ChestItem &chest_item = m_chest_item_list[grid_index];
	if (storehouse_type != chest_item.storehouse_type)
		return;
	
	const ItemBase* item_base = ITEMPOOL->GetItem(chest_item.item_id);
	if (ItemBase::I_TYPE_EQUIPMENT != item_base->GetItemType()) 
		return;

	chest_item.Reset(m_chest_item_list[m_count - 1]);
	chest_item.index = grid_index;

	m_chest_item_list[m_count - 1].Reset();

	-- m_count;
	if (shop_type > CHEST_SHOP_TYPE_INVALID && shop_type < CHEST_SHOP_TYPE_MAX)
	{
		-- m_count_list[(int)shop_type]; 

		int storehouse_type = this->GetStorehouseType(shop_type);
		--m_storehouse_count_list[storehouse_type];
	}

	// 回收
	if (RECYCL_LINGJING == item_base->GetRecyclType(NULL))
	{
		int add_lingjing = item_base->GetRecyclGet(NULL);
		m_role->AddLingJing(add_lingjing, "Recycle In ChestShop");
	}

	this->OnGetItemList(shop_type);
}

void RoleChestShop::SendFreeInfo()
{
	static Protocol::SCChestShopFreeInfo info;
	info.chest_shop_next_free_time_1 = m_role->GetRoleShop()->GetChestShopNextFreeTime(CHEST_SHOP_MODE_1);
	info.chest_shop_1_next_free_time_1 = m_role->GetRoleShop()->GetChestShopNextFreeTime(CHEST_SHOP_MODE1_1);
	info.chest_shop_2_next_free_time_1 = m_role->GetRoleShop()->GetChestShopNextFreeTime(CHEST_SHOP_MODE2_1);
	info.chest_shop_jl_next_free_time_1 = m_role->GetRoleShop()->GetChestShopNextFreeTime(CHEST_SHOP_JL_MODE_1);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&info, sizeof(info));
}

void RoleChestShop::SendScoreInfo()
{
	Protocol::SCSendScoreInfo info;

	info.chest_shop_mojing = m_role->GetRoleShop()->GetChestShopMojing();
	info.chest_shop_shengwang = m_role->GetRoleShop()->GetChestShopShengwang();
	info.chest_shop_gongxun = m_role->GetRoleShop()->GetChestShopGongXun();
	info.chest_shop_weiwang = m_role->GetRoleShop()->GetChestShopWeiWang();
	info.chest_shop_treasure_credit = m_role->GetRoleShop()->GetChestShopTreasureCredit();
	info.chest_shop_jingling_credit = m_role->GetRoleShop()->GetChestShopJingLingCredit();
	info.chest_shop_happytree_grow = m_role->GetRoleShop()->GetChestShopHappyTreeGrow();
	info.chest_shop_mysterious_shop_credit = m_role->GetRoleShop()->GetChestShopMysteriousShopCredit();
	info.chest_shop_blue_lingzhi = m_role->GetRoleShop()->GetChestShopBuleLingzhi();
	info.chest_shop_purple_lingzhi = m_role->GetRoleShop()->GetChestShopPurpleLingzhi();
	info.chest_shop_orange_lingzhi = m_role->GetRoleShop()->GetChestShopOrangeLingzhi();
	info.chest_shop_guanghui = m_role->GetRoleShop()->GetChestShopGuanghui();
	info.chest_shop_precious_boss_score = m_role->GetRoleShop()->GetChestShopPreciousBossScore();
	info.chest_shop_shenzhouweapon_score = m_role->GetRoleShop()->GetChestShopShenZhouWeaponCredit();
	info.chest_shop_treasure_credit1 = m_role->GetRoleShop()->GetChestShopTreasureCredit1();
	info.chest_shop_treasure_credit2 = m_role->GetRoleShop()->GetChestShopTreasureCredit2();
	info.chest_shop_treasure_credit3 = m_role->GetRoleShop()->GetChestShopTreasureCredit3();
	info.chest_shop_zhuanzhi_stone_score = m_role->GetZhuanzhiEquip()->GetStoneScore();
	info.chest_shop_hunjing = m_role->GetRoleShop()->GetChestShopHunjing();
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&info, sizeof(info));
}

void RoleChestShop::SendScoreInfoNotice(int mojing, int shengwang, int gongxun, int weiwang, int treasure_credit, int jingling_credit, 
	int happytree_grow, int mysterious_shop_credit, int blue_lingzhi, int purple_lingzhi, int orange_lingzhi, int guanghui, int precious_boss_score, 
	int shenzhou_weapon_score, int treasure_credit1, int treasure_credit2, int treasure_credit3, int zhuanzhi_stone_score, int chest_shop_hunjing)
{
	Protocol::SCSendScoreInfoNotice info;

	info.chest_shop_mojing = mojing;
	info.chest_shop_shengwang = shengwang;
	info.chest_shop_gongxun = gongxun;
	info.chest_shop_weiwang = weiwang;
	info.chest_shop_treasure_credit = treasure_credit;
	info.chest_shop_jingling_credit = jingling_credit;
	info.chest_shop_happytree_grow = happytree_grow;
	info.chest_mysterious_shop_credit = mysterious_shop_credit;
	info.chest_shop_blue_lingzhi = blue_lingzhi;
	info.chest_shop_purple_lingzhi = purple_lingzhi;
	info.chest_shop_orange_lingzhi = orange_lingzhi;
	info.chest_shop_guanghui = guanghui;
	info.chest_shop_precious_boss_score = precious_boss_score;
	info.chest_shop_shenzhouweapon_score = shenzhou_weapon_score;
	info.chest_shop_treasure_credit1 = treasure_credit1;
	info.chest_shop_treasure_credit2 = treasure_credit2;
	info.chest_shop_treasure_credit3 = treasure_credit3;
	info.chest_shop_zhuanzhi_stone_score = zhuanzhi_stone_score;							// 转职玉石精粹
	info.chest_shop_hunjing = chest_shop_hunjing;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&info, sizeof(info));
}