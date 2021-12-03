#include "randgift.hpp"

#include "servercommon/errornum.h"

#include "item/itempool.h"
#include "item/knapsack.h"

#include "obj/character/role.h"

#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "equipment/equipmentmanager.h"
#include "protocal/msgitem.h"
#include "gameworld/global/guildrarelog/guildrarelogmanager.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "gameworld/other/cross/rolecross.h"
#include "gameworld/internalcomm.h"
#include "global/droplog/droplogmanager.hpp"

RandGift::RandGift() : GiftItem(GIFT_TYPE_RAND), m_rand_num(0), m_sum_rate(0), m_boardcast_string_id(0), m_need_gold(0)
{
	memset(m_item_rate_list, 0, sizeof(m_item_rate_list));
	memset(m_boardcast_item_list, 0, sizeof(m_boardcast_item_list));
}

RandGift::~RandGift()
{

}

int RandGift::Use(Role *role, const ItemGridData &item_data, short &num, short equip_index) const
{
	if (m_day_usetimes > 0)
	{
		short remain_times = m_day_usetimes - role->GetUseNorexItemDayTimes(m_item_id);
		if (remain_times <= 0)
		{
			role->NoticeNum(errornum::EN_ITEM_USE_DAY_TEIMS_LIMIT);
			return -1000;
		}
		else
		{
			num = num > remain_times ? remain_times : num;
		}
	}

	int index_count = 0;
	static ItemConfigData item_cfg_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];

	if (m_need_gold > 0)
	{
		if (!role->GetKnapsack()->GetMoney()->GoldMoreThan(m_need_gold * num))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return -100;
		}
	}

	for (int i = 0; i < num; ++i)
	{
		if (!this->GetRandGiftItemList(role, item_data.is_bind, ItemNamespace::MAX_KNAPSACK_GRID_NUM, item_cfg_list, &index_count))
		{
			role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return -1;
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
		memset(out_index_list, -1, sizeof(out_index_list));

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
			for (int i = 0; i < index_count && i < ItemNamespace::MAX_KNAPSACK_GRID_NUM; ++i)
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

	if (m_boardcast_string_id > 0 && m_boardcast_string_id < OPEN_RANDGIFT_BROADCAST_ID_MAX_NUM)
	{
		for (int k = 0; k < RAND_GIFT_BROADCAST_ITEM_NUM; ++k)
		{
			for (int i = 0; i < index_count; ++i)
			{
				if (item_cfg_list[i].item_id == m_boardcast_item_list[k])
				{
					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_open_randgift[m_boardcast_string_id],
						UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), (int)m_item_id, (int)m_boardcast_item_list[k]);
					if (length > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
					}

					break;
				}
			}
		}
	}

	role->GetKnapsack()->GetMoney()->Add(m_gold * num, m_gold_bind * num, 0, m_coin_bind * num, "RandGift");

	if (m_need_gold > 0)
	{
		role->GetKnapsack()->GetMoney()->UseGold(m_need_gold * num, "OpenRandGift");
	}

	if (m_day_usetimes > 0)
	{
		role->AddUseNorexItemDayTimes(m_item_id, num);
	}
	if (IS_BOX_TYPE_SAME_AS_THE_ITEM_CONFIG == is_box)
	{
		this->SendRandGiftItemInfo(role, item_cfg_list, index_count);

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

bool RandGift::GetRandGiftItemList(Role *role, bool is_bind, int list_num, ItemConfigData *config_list, int *index_count) const
{
	if (NULL == config_list || m_item_num > GIFT_ITEM_MAX_NUM || m_rand_num > GIFT_ITEM_MAX_NUM) return false;

	static short temp_rate_list[GIFT_ITEM_MAX_NUM] = {0};
	memcpy(temp_rate_list, m_item_rate_list, sizeof(temp_rate_list));

	int sum_rate = m_sum_rate;

	if (m_is_check_prof)
	{
		for (int i = 0; i < m_item_num; ++i)
		{
			const ItemBase *item = ITEMPOOL->GetItem(m_gift_item_list[i].item_id);
			if (NULL == item) return false;

			if (!role->IsProfSuitable((char)item->GetLimitProf()))
			{
				sum_rate -= temp_rate_list[i];
				temp_rate_list[i] = 0;
			}
		}
	}

	for (int i = 0; i < m_rand_num; ++i)
	{
		if (sum_rate <= 0) return false;
		int rand_int = RandomNum(sum_rate);

		for (int k = 0; k < m_item_num; ++k)
		{
			if (rand_int < temp_rate_list[k])
			{
				bool item_bind = is_bind || m_gift_item_list[k].is_bind;
				if (IS_BOX_TYPE_SAME_AS_THE_ITEM_CONFIG == is_box)
				{
					item_bind = m_gift_item_list[k].is_bind;
				}

				if (!ItemExtern::PutInItemConfigList(list_num, config_list, index_count, 
					m_gift_item_list[k].item_id, m_gift_item_list[k].num, item_bind)) return false;

				sum_rate -= temp_rate_list[k];
				temp_rate_list[k] = 0;
				break;
			}

			rand_int -= temp_rate_list[k];
		}
	}

	return true;
}

void RandGift::SendRandGiftItemInfo(Role* role, ItemConfigData* item_cfg_list, int count) const
{
	if (nullptr == role || nullptr == item_cfg_list || count <= 0 || count > GIFT_ITEM_MAX_NUM)
	{
		return;
	}
	if (DropLogManager::Instance().IsBopenBegin())
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
	info.gift_type = Protocol::SCRandGiftItemInfo::GIFT_BAG_TYPE_RAND;
	info.count = count;
	unsigned int len = sizeof(info) - sizeof(Protocol::SCRandGiftItemInfo::RewardItem) * (GIFT_ITEM_MAX_NUM - count);
	if (len <= 0)
	{
		return;
	}
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&info, len);
}
