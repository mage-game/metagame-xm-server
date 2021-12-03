#include "selectgift.hpp"

#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "globalconfig/globalconfig.h"

#include "item/itempool.h"

#include "obj/character/role.h"
#include "world.h"

#include "config/logicconfigmanager.hpp"
#include "equipment/equipmentmanager.h"
#include "gameworld/global/guildrarelog/guildrarelogmanager.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "gameworld/other/cross/rolecross.h"
#include "gameworld/internalcomm.h"

SelectGift::SelectGift() : GiftItem(GIFT_TYPE_SELECT), m_need_gold(0)
{

}

SelectGift::~SelectGift()
{

}

int SelectGift::Use(Role *role, const ItemGridData &item_data, short &num, short select_index) const
{
	if (m_item_num > GIFT_ITEM_MAX_NUM) return -1;

	if (m_need_gold > 0)
	{
		if (!role->GetKnapsack()->GetMoney()->GoldMoreThan(m_need_gold * num))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return -100;
		}
	}

	int index_count = 0;
	static ItemConfigData item_cfg_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];

	if (select_index >= 0 && select_index < m_item_num && select_index < GIFT_ITEM_MAX_NUM)
	{
		if (m_gift_item_list[select_index].item_id > 0)
		{
			for (int k = 0; k < num; ++k)
			{
				bool item_bind = item_data.is_bind || m_gift_item_list[select_index].is_bind;
				if (IS_BOX_TYPE_SAME_AS_THE_ITEM_CONFIG == is_box)
				{
					item_bind = m_gift_item_list[select_index].is_bind;
				}

				if (!ItemExtern::PutInItemConfigList(ItemNamespace::MAX_KNAPSACK_GRID_NUM, item_cfg_list, &index_count, 
					m_gift_item_list[select_index].item_id, m_gift_item_list[select_index].num, item_bind))
				{
					role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
					return -1;
				}
			}
		}
	}
	else 
	{
		role->NoticeNum(errornum::EN_KNAPSACK_USE_ITEM_ERROR);
		return -8;
	}

	if (!role->GetKnapsack()->CheckEmptyGridNoLessThan(index_count))
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return -2;
	}

	if (index_count > 0)
	{
		static int out_index_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];
		memset(out_index_list, -1 , sizeof(out_index_list));

		//if (!role->GetKnapsack()->PutList(index_count, item_cfg_list, PUT_REASON_GIFT, out_index_list)) return -3;
		for (int i = 0; i < index_count && i < ItemNamespace::MAX_KNAPSACK_GRID_NUM; ++i)
		{
			int *out_index = NULL;
			if (i >= 0 && i < ItemNamespace::MAX_KNAPSACK_GRID_NUM) // 防止越界
			{
				out_index = &out_index_list[i];
			}

			if (!role->GetKnapsack()->PutOrSendToOtherSystem(item_cfg_list[i], PUT_REASON_GIFT, out_index))
			{
				return -5; // 之前使用putlist也没有保持原子性，所以这里有错误也直接中断
			}
		}

		//为装备类加固定属性
		{
			for (int i = 0; i < index_count && i < ItemNamespace::MAX_KNAPSACK_GRID_NUM; ++ i)
			{
				if (out_index_list[i] >= 0)
				{
					ItemGridData item_grid_data;
					if (role->GetKnapsack()->GetItemGridData(out_index_list[i], &item_grid_data) && 
						EquipmentManager::GetFixSpecialAttr(&item_grid_data, Equipment::EQUIPMENT_FIX_GETWAY_DEFAULT_GIFT, m_item_id))
					{
						role->GetKnapsack()->SetItemDirty(out_index_list[i]);
					}
				}
			}
		}
	}

	role->GetKnapsack()->GetMoney()->Add(m_gold * num, m_gold_bind * num, 0, m_coin_bind * num, "SelectGift");

	if (m_need_gold > 0)
	{
		role->GetKnapsack()->GetMoney()->UseGold(m_need_gold * num, "OpenSelectGift");
	}

	if (IS_BOX_TYPE_SAME_AS_THE_ITEM_CONFIG == is_box)
	{
		{
			//仙盟珍稀
			for (int i = 0; i < index_count && i < ItemNamespace::MAX_KNAPSACK_GRID_NUM; ++i)
			{
				if (!item_cfg_list[i].is_bind)
				{
					if (!CrossConfig::Instance().IsHiddenServer())
					{
						//仙盟珍稀日志
						GuildID guild_id = role->GetGuildID();
						if (INVALID_GUILD_ID != guild_id)
						{
							GuildRareLogItem temp_item;
							//temp_item.uid = role->GetUID();
							temp_item.uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
							strcpy(temp_item.role_name, role->GetName());
							temp_item.item_id = item_cfg_list[i].item_id;
							temp_item.item_num = item_cfg_list[i].num;
							temp_item.is_from_gift = 1;
							temp_item.gift_item_id = item_data.item_id;
							temp_item.timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

							GuildRareLogManager::Instance().AddLog(guild_id, temp_item);
						}
					}
					else
					{
						//仙盟珍稀日志
						GuildID guild_id = role->GetGuildID();
						if (INVALID_GUILD_ID != guild_id)
						{
							crossgameprotocal::PickGuildRareItemInHiddenGs info;

							info.item.uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
							info.item.item_id = item_cfg_list[i].item_id;
							info.item.item_num = item_cfg_list[i].num;
							//info.item.uid = role->GetUID();
							strcpy(info.item.role_name, role->GetName());
							info.guild_id = role->GetGuildID();
							info.item.is_from_gift = 1;
							info.item.gift_item_id = item_data.item_id;
							info.item.timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

							InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_BY_UUID,
								UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()),
								(const char*)&info, sizeof(info));
						}
					}
				}
			}
		}
	}

	return 0;
}

