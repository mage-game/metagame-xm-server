#include "defgift.hpp"

#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "globalconfig/globalconfig.h"

#include "item/itempool.h"

#include "obj/character/role.h"
#include "world.h"

#include "config/logicconfigmanager.hpp"
#include "servercommon/jinglingdef.hpp"
#include "equipment/equipmentmanager.h"
#include "protocal/msgitem.h"
#include "gameworld/global/guildrarelog/guildrarelogmanager.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "gameworld/other/cross/rolecross.h"
#include "gameworld/internalcomm.h"

DefGift::DefGift() : GiftItem(GIFT_TYPE_DEF), m_need_gold(0)
{

}

DefGift::~DefGift()
{

}

int DefGift::Use(Role *role, const ItemGridData &item_data, short &num, short equip_index) const
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

	for (int i = 0; i < m_item_num; ++i)
	{
		if (m_is_check_prof)
		{
			const ItemBase *item = ITEMPOOL->GetItem(m_gift_item_list[i].item_id);
			if (NULL == item || !role->IsProfSuitable((char)item->GetLimitProf())) continue;
		}

		for (int k = 0; k < num; ++k)
		{
			bool item_bind = item_data.is_bind || m_gift_item_list[i].is_bind;
			if (IS_BOX_TYPE_SAME_AS_THE_ITEM_CONFIG == is_box)
			{
				item_bind = m_gift_item_list[i].is_bind;
			}

			if (!ItemExtern::PutInItemConfigList(ItemNamespace::MAX_KNAPSACK_GRID_NUM, item_cfg_list, &index_count, 
				m_gift_item_list[i].item_id, m_gift_item_list[i].num, item_bind))
			{
				role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
				return -1;
			}
		}
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
				return -3; // 之前使用putlist也没有保持原子性，所以这里有错误也直接中断
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

	role->GetKnapsack()->GetMoney()->Add(m_gold * num, m_gold_bind * num, 0, m_coin_bind * num, "DefGift");

	if (m_need_gold > 0)
	{
		role->GetKnapsack()->GetMoney()->UseGold(m_need_gold * num, "OpenGift");
	}

	if (IS_BOX_TYPE_SAME_AS_THE_ITEM_CONFIG == is_box)
	{
		this->SendDefGiftItemInfo(role, item_cfg_list, index_count);

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

ItemConfigData * DefGift::GetGiftItemItem(int index)
{
	if (index < 0 || index >= GIFT_ITEM_MAX_NUM) return NULL;

	return &m_gift_item_list[index];
}

void DefGift::SendDefGiftItemInfo(Role* role, ItemConfigData* item_cfg_list, int count) const
{
	if (nullptr == role || nullptr == item_cfg_list || count <= 0 || count > GIFT_ITEM_MAX_NUM)
	{
		return;
	}
	Protocol::SCRandGiftItemInfo info;
	for (int i = 0; i < count; ++i)
	{
		info.item_list[i].item_id = item_cfg_list[i].item_id;
		info.item_list[i].num = item_cfg_list[i].num;
		info.item_list[i].is_bind = item_cfg_list[i].is_bind;
	}
	info.gift_type = Protocol::SCRandGiftItemInfo::GIFT_BAG_TYPE_DEF;
	info.count = count;
	unsigned int len = sizeof(info) - sizeof(Protocol::SCRandGiftItemInfo::RewardItem) * (GIFT_ITEM_MAX_NUM - count);
	if (len <= 0)
	{
		return;
	}
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&info, len);
}