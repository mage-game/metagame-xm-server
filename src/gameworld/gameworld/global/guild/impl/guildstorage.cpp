#include "guildstorage.hpp"

#include <set>

#include "servercommon/maildef.hpp"
#include "servercommon/struct/structcommon.h"
#include "servercommon/string/globalstr.h"

#include "servercommon/internalprotocal/ggprotocal.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "servercommon/string/gameworldstr.h"

#include "servercommon/errornum.h"
#include "gamelog.h"

#include "item/itempool.h"
#include "item/itembase.h"
#include "item/itemgriddata.h"
#include "equipment/equipment.h"
#include "equipment/equipmentfunction.h"
#include "obj/character/role.h"
#include "global/guild/impl/guildmember.hpp"
#include "global/guild/guildmanager.hpp"
#include "global/guild/guild.hpp"
#include "global/guild/roleguild.hpp"
#include "config/logicconfigmanager.hpp"
#include "global/guild/impl/guildauthority.hpp"
#include "other/event/eventhandler.hpp"
#include "item/other/otheritem.hpp"
#include "item/knapsack.h"

GuildStorage::GuildStorage()
	: m_guild(NULL), m_count(0)
{
	memset(m_item_list, 0, sizeof(m_item_list));
}

GuildStorage::~GuildStorage()
{

}

void GuildStorage::Init(const GuildBaseParam &param)
{
	UNSTD_STATIC_CHECK(sizeof(m_item_list) == sizeof(param.storge_item_list))
	memcpy(m_item_list, param.storge_item_list, sizeof(m_item_list));

	m_count = 0;
	
	this->CheckAddConstantItem();  // 添加一个固定物品

	for (int i = 0; i < GUILD_STORAGE_MAX_GRID_NUM; ++ i)
	{
		if (m_item_list[i].Invalid())
		{
			break;
		}
		else
		{
			m_count ++;
		}
	}
}

void GuildStorage::GetStorgeItemList(GuildBaseParam *param)
{
	if (NULL == param)
	{
		return;
	}

	memset(param->storge_item_list, 0, sizeof(param->storge_item_list));

	for (int i = 0; i < GUILD_STORAGE_MAX_GRID_NUM; ++i)
	{
		if (!m_item_list[i].Invalid())
		{
			param->storge_item_list[i].Reset(m_item_list[i]);
		}
	}
}

void GuildStorage::ClearDirtyMark()
{
}

void GuildStorage::SetDirty(int item_index)
{
	if (item_index < 0 || item_index >= GUILD_STORAGE_MAX_GRID_NUM) return;

	this->SyncStorgeChangeToAllMember(item_index);
}

void GuildStorage::CheckAddConstantItem()
{
	// 固定物品ID添加到第0个索引
	GuildOtherConfig & other_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig();

	ItemDataWrapper & first_item = m_item_list[0];

	if (other_cfg.storage_constant_item_id <= 0)
	{
		return;
	}

	if (0 == first_item.item_id || other_cfg.storage_constant_item_id != first_item.item_id)
	{
		const ItemBase *itembase = ITEMPOOL->GetItem(other_cfg.storage_constant_item_id);
		if(itembase)
		{
			static ItemDataWrapper temp_item_wrapper; temp_item_wrapper.Reset();
			temp_item_wrapper.item_id = other_cfg.storage_constant_item_id;
			temp_item_wrapper.num = (short)1;  // 数量不削减
			temp_item_wrapper.is_bind = (short)1; // 绑定

			first_item.Reset(temp_item_wrapper);
		}
	}
}

void GuildStorage::OnPutItem(Role *role, int bag_index, int putitem_num)
{
	if (NULL == role || putitem_num <= 0)
	{
		return;
	}

	if (role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}
	
	GuildMember *member = m_guild->GetGuildMemberManager().GetMember(role->GetUserId());
	if (NULL == member)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	static ItemGridData item_data;
	item_data.Reset();

	role->GetKnapsack()->GetItemGridData(bag_index, &item_data);
	if (item_data.Invalid())
	{
		role->NoticeNum(errornum::EN_GUILD_STORAGE_NOT_HAVE_ITEM);
		return;
	}

	if (item_data.is_bind)
	{
		role->NoticeNum(errornum::EN_GUILD_STORAGE_ITEM_CANNOT_SALE);
		return;
	}

	const ItemBase *item = ITEMPOOL->GetItem(item_data.item_id);
	if (NULL == item)
	{
		role->NoticeNum(errornum::EN_GUILD_STORAGE_ITEM_NOT_PUT);
		return;
	}

	bool is_need_notice = false;
	int storage_score = 0;
	if (ItemBase::I_TYPE_EQUIPMENT == item->GetItemType())
	{
		if (NULL == item_data.param)
		{
			role->NoticeNum(errornum::EN_GUILD_STORAGE_ITEM_NOT_PUT);
			return;
		}

		const Equipment *equip = (const Equipment *)item;
		if (equip->GetGuildStorageScore() <= 0)
		{
			role->NoticeNum(errornum::EN_GUILD_STORAGE_ITEM_NOT_PUT);
			return;
		}

		storage_score = equip->GetGuildStorageScore();
		is_need_notice = true;
	}
	//else if (ItemBase::I_TYPE_OTHER == item->GetItemType())
	//{
	//	const OtherItem *other_item = (const OtherItem *)item;
	//	if (other_item->GetGuildStorageScore() <= 0)
	//	{
	//		role->NoticeNum(errornum::EN_GUILD_STORAGE_ITEM_NOT_PUT);
	//		return;
	//	}
	//
	//	storage_score = other_item->GetGuildStorageScore() * putitem_num;
	//}
	else
	{
		role->NoticeNum(errornum::EN_GUILD_STORAGE_ITEM_NOT_PUT);
		return;
	}

// 	const EquipmentParam *equip_param = (const EquipmentParam *)item_data.param;
// 	int xianpin_attr_count = 0;
// 	for (int i = 0; i < ItemNamespace::MAX_XIANPIN_NUM; i++)
// 	{
// 		if (equip_param->xianpin_type_list[i] > 0)
// 		{
// 			xianpin_attr_count++;
// 		}
// 	}
// 	if (xianpin_attr_count <= 0)
// 	{
// 		role->NoticeNum(errornum::EN_GUILD_STORAGE_ITEM_NOT_PUT);
// 		return;
// 	}

	const int open_count = LOGIC_CONFIG->GetGuildConfig().GetGuildStorgeOpenCount(m_guild->GetGuildLevel());
	if (m_count >= open_count || m_count >= GUILD_STORAGE_MAX_GRID_NUM || m_count < 0)
	{
		role->NoticeNum(errornum::EN_GUILD_STORAGE_FULL);
		return;
	}

	if (putitem_num > item_data.num)
	{
		role->NoticeNum(errornum::EN_GUILD_STORAGE_PUTON_ITEM_NOT_ENOUGH);
		return;
	}

	if (!m_item_list[m_count].Invalid())
	{
		return;
	}

	static ItemDataWrapper putitem_datawrapper;
	putitem_datawrapper.Reset();
	item_data.WrapTo(&putitem_datawrapper);
	putitem_datawrapper.num = putitem_num;
	
	// 消耗
	{
		if (!role->GetKnapsack()->ConsumeItemByIndex(bag_index, putitem_num, "PutItemGuildStorge"))
		{
			role->NoticeNum(errornum::EN_GUILD_STORAGE_PUT_ERROR);
			return;
		}
	}

	// 放进仓库
	{
		role->GetRoleGuild()->SetGuildStorageScore(role->GetRoleGuild()->GetGuildStorageScore() + storage_score);

		if (!ItemBase::CanMerge(item->GetItemId()))
		{
			ItemDataWrapper *storgeitem_datawrapper = &m_item_list[m_count];
			if (!storgeitem_datawrapper->Invalid())
			{
				return;
			}
			storgeitem_datawrapper->Reset(putitem_datawrapper);

			SetDirty(m_count);

			++ m_count;
		}
		else
		{
			int merge_item_index = -1;
			int temp_put_num = putitem_num;
			for (int i = 0; i < m_count && i < GUILD_STORAGE_MAX_GRID_NUM && temp_put_num > 0; i++)
			{
				if (m_item_list[i].item_id == item->GetItemId() && m_item_list[i].num < item->GetPileLimit())
				{
					merge_item_index = i;
					if (item->GetPileLimit() >= temp_put_num + m_item_list[merge_item_index].num)
					{
						ItemDataWrapper *storgeitem_datawrapper = &m_item_list[merge_item_index];
						storgeitem_datawrapper->num += temp_put_num;
						SetDirty(merge_item_index);

						temp_put_num = 0;
						break;
					}
					else
					{
						temp_put_num -= item->GetPileLimit() - m_item_list[merge_item_index].num ;

						ItemDataWrapper *storgeitem_datawrapper = &m_item_list[merge_item_index];
						storgeitem_datawrapper->num = item->GetPileLimit();
						SetDirty(merge_item_index);

						merge_item_index = -1;
					}
				}
			}

			if (merge_item_index == -1 && temp_put_num > 0)
			{
				ItemDataWrapper *storgeitem_datawrapper = &m_item_list[m_count];
				if (!storgeitem_datawrapper->Invalid())
				{
					return;
				}

				putitem_datawrapper.num = temp_put_num;
				storgeitem_datawrapper->Reset(putitem_datawrapper);

				SetDirty(m_count);

				++ m_count;
			}
		}

		m_guild->GetGuildEventManager().OnStorageOperate(member, GUILD_STORE_OPTYPE_PUTIN, putitem_datawrapper);
		
		EventHandler::Instance().OnPutEquipInGuild(role);
	}

	this->SyncStorgeInfo(role);
	this->SyncStorgeLog(nullptr);

	if (is_need_notice)
	{
		static ItemGridData grid_data;
		grid_data.Reset();
		grid_data.Set(putitem_datawrapper, item->GetColor());
		if (NULL != grid_data.param)
		{
			EquipmentParam *equip_param = (EquipmentParam *)grid_data.param;

			if (NULL != equip_param)
			{
				const char *equip_param_string = EquipFunction::EquipParamDataClientString(UidToInt(role->GetUserId()), item->GetItemId(), false, 0, equip_param, NULL);
				if (NULL != equip_param_string)
				{
					int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_storage_put_item,
						role->GetUID(), role->GetName(), role->GetCamp(), item->GetItemId(), equip_param_string, storage_score);

					if (sendlen > 0)
					{
						//m_guild->GuildSystemMsg(sendlen, gamestring::GAMESTRING_BUF);
						static Protocol::SCSystemMsg sm;
						int send_len = 0;

						if (World::GetInstWorld()->GetSceneManager()->CreateSystemMsg(&sm, &send_len, gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_ONLY_CHAT_GUILD))
						{
							m_guild->SendMessageToGuildAll(Protocol::GUILD_NOTIFY_TYPE_STORAGE_LOG, (const char *)&sm, send_len);
						}
					}
				}
			}
		}
	}

	gamelog::g_log_guild.printf(LL_INFO, "guild -> putitem storge succ [ guild_id:%d], uid %d, name %s, itemid %d, num %d",
		m_guild->GetGuildID(), role->GetUID(), role->GetName(), putitem_datawrapper.item_id, putitem_datawrapper.num);
}

void GuildStorage::OnPutItemOneKey(Role *role, Protocol::CSGuildStorgeOneKeyOperate *cmd)
{
	if (NULL == role || NULL == cmd)
	{
		return;
	}

	if (role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	GuildMember *member = m_guild->GetGuildMemberManager().GetMember(role->GetUserId());
	if (NULL == member)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	bool is_notice_client = false;
	for(int i = 0; i < cmd->item_count && i < GUILD_STORAGE_MAX_GRID_NUM; i++)
	{

		if (cmd->item_list[i].param_1 <= 0)
		{
			continue;
		}

		static ItemGridData item_data;
		item_data.Reset();

		role->GetKnapsack()->GetItemGridData(cmd->item_list[i].item_index, &item_data);
		if (item_data.Invalid())
		{
			continue;
		}

		if (item_data.is_bind)
		{
			continue;
		}

		const ItemBase *item = ITEMPOOL->GetItem(item_data.item_id);
		if (NULL == item)
		{
			continue;
		}

		bool is_need_notice = false;
		int storage_score = 0;
		if (ItemBase::I_TYPE_EQUIPMENT == item->GetItemType())
		{
			if (NULL == item_data.param)
			{
				continue;
			}

			const Equipment *equip = (const Equipment *)item;
			if (equip->GetGuildStorageScore() <= 0)
			{
				continue;
			}

			storage_score = equip->GetGuildStorageScore();
			is_need_notice = true;
		}
		//else if (ItemBase::I_TYPE_OTHER == item->GetItemType())
		//{
		//	const OtherItem *other_item = (const OtherItem *)item;
		//	if (other_item->GetGuildStorageScore() <= 0)
		//	{
		//		continue;
		//	}
		//
		//	storage_score = other_item->GetGuildStorageScore() * cmd->item_list[i].param_1;
		//}
		else
		{
			continue;
		}

// 		const EquipmentParam *equip_param = (const EquipmentParam *)item_data.param;
// 		int xianpin_attr_count = 0;
// 		for (int i = 0; i < ItemNamespace::MAX_XIANPIN_NUM; i++)
// 		{
// 			if (equip_param->xianpin_type_list[i] > 0)
// 			{
// 				xianpin_attr_count++;
// 			}
// 		}
// 		if (xianpin_attr_count <= 0)
// 		{
// 			continue;
// 		}

		const int open_count = LOGIC_CONFIG->GetGuildConfig().GetGuildStorgeOpenCount(m_guild->GetGuildLevel());
		if (m_count >= open_count || m_count >= GUILD_STORAGE_MAX_GRID_NUM || m_count < 0)
		{
			role->NoticeNum(errornum::EN_GUILD_STORAGE_FULL);
			break;
		}

		if (cmd->item_list[i].param_1 > item_data.num)
		{
			continue;
		}

		if (!m_item_list[m_count].Invalid())
		{
			break;
		}

		static ItemDataWrapper putitem_datawrapper;
		putitem_datawrapper.Reset();
		item_data.WrapTo(&putitem_datawrapper);
		putitem_datawrapper.num = cmd->item_list[i].param_1;

		// 消耗
		{
			if (!role->GetKnapsack()->ConsumeItemByIndex(cmd->item_list[i].item_index, cmd->item_list[i].param_1, "PutItemGuildStorge"))
			{
				continue;
			}
		}

		// 放进仓库
		{
			role->GetRoleGuild()->SetGuildStorageScore(role->GetRoleGuild()->GetGuildStorageScore() + storage_score);

			if (!ItemBase::CanMerge(item->GetItemId()))
			{
				ItemDataWrapper *storgeitem_datawrapper = &m_item_list[m_count];
				if (!storgeitem_datawrapper->Invalid())
				{
					continue;
				}
				storgeitem_datawrapper->Reset(putitem_datawrapper);

				SetDirty(m_count);

				++ m_count;
			}
			else
			{
				int merge_item_index = -1;
				int temp_put_num = cmd->item_list[i].param_1;
				for (int i = 0; i < m_count && i < GUILD_STORAGE_MAX_GRID_NUM && temp_put_num > 0; i++)
				{
					if (m_item_list[i].item_id == item->GetItemId() && m_item_list[i].num < item->GetPileLimit())
					{
						merge_item_index = i;
						if (item->GetPileLimit() >= temp_put_num + m_item_list[merge_item_index].num)
						{
							ItemDataWrapper *storgeitem_datawrapper = &m_item_list[merge_item_index];
							storgeitem_datawrapper->num += temp_put_num;
							SetDirty(merge_item_index);

							temp_put_num = 0;
							break;
						}
						else
						{
							temp_put_num -= item->GetPileLimit() - m_item_list[merge_item_index].num ;

							ItemDataWrapper *storgeitem_datawrapper = &m_item_list[merge_item_index];
							storgeitem_datawrapper->num = item->GetPileLimit();
							SetDirty(merge_item_index);

							merge_item_index = -1;
						}
					}
				}

				if (merge_item_index == -1 && temp_put_num > 0)
				{
					ItemDataWrapper *storgeitem_datawrapper = &m_item_list[m_count];
					if (!storgeitem_datawrapper->Invalid())
					{
						continue;
					}

					putitem_datawrapper.num = temp_put_num;
					storgeitem_datawrapper->Reset(putitem_datawrapper);

					SetDirty(m_count);

					++ m_count;
				}
			}

			if (is_need_notice)
			{
				static ItemGridData grid_data;
				grid_data.Reset();
				grid_data.Set(putitem_datawrapper, item->GetColor());
				if (NULL != grid_data.param)
				{
					EquipmentParam *equip_param = (EquipmentParam *)grid_data.param;

					if (NULL != equip_param)
					{
						const char *equip_param_string = EquipFunction::EquipParamDataClientString(UidToInt(role->GetUserId()), item->GetItemId(), false, 0, equip_param, NULL);
						if (NULL != equip_param_string)
						{
							int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_storage_put_item,
								role->GetUID(), role->GetName(), role->GetCamp(), item->GetItemId(), equip_param_string, storage_score);

							if (sendlen > 0)
							{
								//m_guild->GuildSystemMsg(sendlen, gamestring::GAMESTRING_BUF);
								static Protocol::SCSystemMsg sm;
								int send_len = 0;

								if (World::GetInstWorld()->GetSceneManager()->CreateSystemMsg(&sm, &send_len, gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_ONLY_CHAT_GUILD))
								{
									m_guild->SendMessageToGuildAll(Protocol::GUILD_NOTIFY_TYPE_STORAGE_LOG, (const char *)&sm, send_len);
								}
							}
						}
					}
				}
			}

			is_notice_client = true;

			m_guild->GetGuildEventManager().OnStorageOperate(member, GUILD_STORE_OPTYPE_PUTIN, putitem_datawrapper);
			EventHandler::Instance().OnPutEquipInGuild(role);
		}

		gamelog::g_log_guild.printf(LL_INFO, "guild -> putitem storge succ [ guild_id:%d], uid %d, name %s, itemid %d, num %d",
			m_guild->GetGuildID(), role->GetUID(), role->GetName(), putitem_datawrapper.item_id, putitem_datawrapper.num);
	}

	if (is_notice_client)
	{
		this->SyncStorgeInfo(role);
		this->SyncStorgeLog(nullptr);
	}
}

void GuildStorage::OnTakeItem(Role *role, int storge_index, int takeitem_num, ItemID item_id)
{
	if (NULL == role || takeitem_num <= 0 || item_id <= 0)
	{
		return;
	}
	
	if (role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!role->GetKnapsack()->HaveSpace())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	GuildMember *member = m_guild->GetGuildMemberManager().GetMember(role->GetUserId());
	if (NULL == member)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	int open_count = LOGIC_CONFIG->GetGuildConfig().GetGuildStorgeOpenCount(m_guild->GetGuildLevel());
	if (storge_index < 0 || storge_index >= GUILD_STORAGE_MAX_GRID_NUM || storge_index >= open_count || m_count <= 0)
	{
		role->NoticeNum(errornum::EN_GUILD_STORAGE_NO_ITEM);
		return;
	}

	ItemDataWrapper *storge_itemwrapper = &m_item_list[storge_index];

	if (storge_itemwrapper->Invalid())
	{
		role->NoticeNum(errornum::EN_GUILD_STORAGE_NO_ITEM);
		return;
	}

	if (storge_itemwrapper->item_id != item_id)
	{
		role->NoticeNum(errornum::EN_GUILD_STORAGE_ITEM_BE_TAKEN);
		return;
	}
	
	if (0 != storge_index && takeitem_num > storge_itemwrapper->num)
	{
		role->NoticeNum(errornum::EN_GUILD_STORAGE_PUTOFF_ITEM_NOT_ENOUGH);
		return;
	}

	const ItemBase *item = ITEMPOOL->GetItem(item_id);
	if (NULL == item)
	{
		return;
	}

	int need_storage_score = 0;
	bool is_need_notice = false;
	if (0 == storge_index)
	{
		// 去除固定物品
		GuildOtherConfig & other_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig();
		need_storage_score = other_cfg.constant_item_storage_score * takeitem_num;
		if (role->GetRoleGuild()->GetGuildStorageScore() < need_storage_score)
		{
			role->NoticeNum(errornum::EN_GUILD_STORAGE_TAKE_SCORE_NOT_ENOUGH);
			return;
		}
	}
	else if (ItemBase::I_TYPE_EQUIPMENT == item->GetItemType())
	{
		const Equipment *equip = (const Equipment *)item;
		need_storage_score = equip->GetGuildStorageScore() * takeitem_num;
		if (role->GetRoleGuild()->GetGuildStorageScore() < need_storage_score)
		{
			role->NoticeNum(errornum::EN_GUILD_STORAGE_TAKE_SCORE_NOT_ENOUGH);
			return;
		}

		//if (role->GetLevel() >= 300)
		//{
			is_need_notice = true;
		//}
	}
	//else if (ItemBase::I_TYPE_OTHER == item->GetItemType())
	//{
	//	const OtherItem *other_item = (const OtherItem *)item;
	//	need_storage_score = other_item->GetGuildStorageScore() * takeitem_num;
	//	if (role->GetRoleGuild()->GetGuildStorageScore() < need_storage_score)
	//	{
	//		role->NoticeNum(errornum::EN_GUILD_STORAGE_TAKE_SCORE_NOT_ENOUGH);
	//		return;
	//	}
	//}
	else
	{
		role->NoticeNum(errornum::EN_GUILD_STORAGE_TAKE_ERROR);
		return;
	}

	if (need_storage_score < 0)
	{
		return;
	}

	static ItemDataWrapper takeitem_datawrapper;
	takeitem_datawrapper.Reset(*storge_itemwrapper);
	takeitem_datawrapper.num = takeitem_num;

	// 消耗
	{
		// 固定物品一直存在，不减数量
		if (0 != storge_index)
		{
			storge_itemwrapper->num -= takeitem_num;
		}
		
		SetDirty(storge_index);

		if (storge_itemwrapper->num <= 0)
		{
			storge_itemwrapper->Reset();
			m_item_list[storge_index].Reset(m_item_list[m_count - 1]);

			m_item_list[m_count - 1].Reset();
			SetDirty(m_count - 1);

			-- m_count;
		}

		role->GetRoleGuild()->SetGuildStorageScore(role->GetRoleGuild()->GetGuildStorageScore() - need_storage_score);

		gamelog::g_log_guild.printf(LL_INFO, "guild -> takeitem storge consume [ guild_id:%d], uid %d, name %s, itemid %d, putoff_num %d, remain_num %d",
			m_guild->GetGuildID(), role->GetUID(), role->GetName(), takeitem_datawrapper.item_id, takeitem_datawrapper.num, storge_itemwrapper->num);
	}

	// 放进背包
	if (!role->GetKnapsack()->Put(takeitem_datawrapper, PUT_REASON_PUTOFF_GUILDSOTRGE))
	{
		role->NoticeNum(errornum::EN_GUILD_STORAGE_TAKE_ERROR);
		return;
	}

	if (0 != storge_index)
	{
		m_guild->GetGuildEventManager().OnStorageOperate(member, GUILD_STORE_OPTYPE_TAKEOUT, takeitem_datawrapper);
	}

	this->SyncStorgeInfo(role);
	this->SyncStorgeLog(nullptr);

	if (is_need_notice)
	{
		static ItemGridData grid_data;
		grid_data.Reset();
		grid_data.Set(takeitem_datawrapper, item->GetColor());
		if (NULL != grid_data.param)
		{
			EquipmentParam *equip_param = (EquipmentParam *)grid_data.param;

			if (NULL != equip_param)
			{
				const char *equip_param_string = EquipFunction::EquipParamDataClientString(UidToInt(role->GetUserId()), item->GetItemId(), false, 0, equip_param, NULL);
				if (NULL != equip_param_string)
				{
					int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_storage_take_item,
						role->GetUID(), role->GetName(), role->GetCamp(), item->GetItemId(), equip_param_string, need_storage_score);

					if (sendlen > 0)
					{
						//m_guild->GuildSystemMsg(sendlen, gamestring::GAMESTRING_BUF);
						static Protocol::SCSystemMsg sm;
						int send_len = 0;

						if (World::GetInstWorld()->GetSceneManager()->CreateSystemMsg(&sm, &send_len, gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_ONLY_CHAT_GUILD))
						{
							m_guild->SendMessageToGuildAll(Protocol::GUILD_NOTIFY_TYPE_STORAGE_LOG, (const char *)&sm, send_len);
						}
					}
				}
			}
		}
	}

	gamelog::g_log_guild.printf(LL_INFO, "guild -> takeitem storge success [ guild_id:%d], uid %d, name %s, itemid %d, num %d",
		m_guild->GetGuildID(), role->GetUID(), role->GetName(), takeitem_datawrapper.item_id, takeitem_datawrapper.num);
}

void GuildStorage::OnDiscardItem(Role *role, int storge_index, ItemID item_id)
{
	if (NULL == role || item_id <= 0)
	{
		return;
	}

	// 固定物品不能清理
	if (0 == storge_index)
	{
		return;
	}

	GuildMember *member = m_guild->GetGuildMemberManager().GetMember(role->GetUserId());
	if (NULL == member)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	if (!GuildAuthority::CanDiscardStorgeItem(member))
	{
		role->NoticeNum(errornum::EN_GUILD_STORAGE_DISCARD_NOPOWER);
		return;
	}

	if (storge_index < 0 || storge_index >= GUILD_STORAGE_MAX_GRID_NUM || m_count <= 0)
	{
		role->NoticeNum(errornum::EN_GUILD_STORAGE_NO_ITEM);
		return;
	}
	ItemDataWrapper *storge_itemwrapper = &m_item_list[storge_index];

	if (storge_itemwrapper->Invalid())
	{
		role->NoticeNum(errornum::EN_GUILD_STORAGE_NO_ITEM);
		return;
	}

	if (storge_itemwrapper->item_id != item_id)
	{
		role->NoticeNum(errornum::EN_GUILD_STORAGE_ITEM_BE_TAKEN);
		return;
	}

	{
		storge_itemwrapper->Reset();
		m_item_list[storge_index].Reset(m_item_list[m_count - 1]);

		m_item_list[m_count - 1].Reset();

		SetDirty(storge_index);
		SetDirty(m_count - 1);

		-- m_count;
	}

	//m_guild->GetGuildEventManager().OnStorageOperate(member, GUILD_STORE_OPTYPE_DESTROY, static_cast<int>(item_id), 0);

	this->SyncStorgeInfo(role);

	gamelog::g_log_guild.printf(LL_INFO, "guild -> DiscardItem success [guild_id:%d], uid %d, name %s, itemid %d",
		m_guild->GetGuildID(), role->GetUID(), role->GetName(), (int)item_id);
}

void GuildStorage::OnDiscardItemOneKey(Role *role, Protocol::CSGuildStorgeOneKeyOperate *cmd)
{
	if (NULL == role || NULL == cmd)
	{
		return;
	}

	GuildMember *member = m_guild->GetGuildMemberManager().GetMember(role->GetUserId());
	if (NULL == member)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	if (!GuildAuthority::CanDiscardStorgeItem(member))
	{
		role->NoticeNum(errornum::EN_GUILD_STORAGE_DISCARD_NOPOWER);
		return;
	}

	std::set<short> modify_storage_index; // 有修改的仓库格子

	bool is_notice_client = false;
	for(int i = 0; i < cmd->item_count && i < GUILD_STORAGE_MAX_GRID_NUM; i++)
	{
		if (cmd->item_list[i].param_1 <= 0)
		{
			continue;
		}

		// 等于0，因为固定物品不清理
		if (cmd->item_list[i].item_index <= 0 || cmd->item_list[i].item_index >= GUILD_STORAGE_MAX_GRID_NUM || m_count <= 0)
		{
			continue;
		}
		ItemDataWrapper *storge_itemwrapper = &m_item_list[cmd->item_list[i].item_index];

		if (storge_itemwrapper->Invalid())
		{
			continue;
		}

		if (storge_itemwrapper->item_id != cmd->item_list[i].param_1)
		{
			continue;
		}

		{
			storge_itemwrapper->Reset();  // 遍历模式，这里就要重置

			modify_storage_index.insert(cmd->item_list[i].item_index);
		}

		//m_guild->GetGuildEventManager().OnStorageOperate(member, GUILD_STORE_OPTYPE_DESTROY, static_cast<int>(cmd->item_list[i].param_1), 0);
		is_notice_client = true;

		gamelog::g_log_guild.printf(LL_INFO, "guild -> DiscardItem success [guild_id:%d], uid %d, name %s, itemid %d",
			m_guild->GetGuildID(), role->GetUID(), role->GetName(), (int)cmd->item_list[i].param_1);
	}
	
	if (modify_storage_index.size() > 0 && is_notice_client)
	{
		// 将有效的元素移动到前面，遍历模式可以保证顺序
		m_count = 0;
		for (int i = 0; i < GUILD_STORAGE_MAX_GRID_NUM && m_count <= GUILD_STORAGE_MAX_GRID_NUM; i++)
		{
			if (!m_item_list[i].Invalid())
			{
				if (i != m_count)
				{
					m_item_list[m_count].Reset(m_item_list[i]);
					m_item_list[i].Reset();

					modify_storage_index.insert(i);
					modify_storage_index.insert(m_count);
				}

				++ m_count;
			}
		}

		// 更新变动的仓库格子
		for (const auto it : modify_storage_index)
		{
			SetDirty(it);
		}
	}

	if (is_notice_client)
	{
		this->SyncStorgeInfo(role);
	}
}

void GuildStorage::SyncStorgeChangeToAllMember(int storge_index)
{
	if (storge_index < 0 || storge_index >= GUILD_STORAGE_MAX_GRID_NUM)
	{
		return;
	}

	static Protocol::SCGuildStorgeChange cmd;
	
	cmd.index = storge_index;
	cmd.item_datawrapper.Reset(m_item_list[storge_index]);
	
	m_guild->GetGuildMemberManager().OnSyncGuildSorgeChangeToAllMember(cmd);
}

void GuildStorage::SyncStorgeInfo(Role *role)
{
	if (role == nullptr) return;
	static Protocol::SCGuildStorgeInfo cmd;

	cmd.open_grid_count = LOGIC_CONFIG->GetGuildConfig().GetGuildStorgeOpenCount(m_guild->GetGuildLevel());
	cmd.storage_score = role->GetRoleGuild()->GetGuildStorageScore();

	cmd.count = m_count;

	for (int i = 0; i < m_count; ++ i)
	{
		cmd.storge_item_list[i].Reset(m_item_list[i]);
	}

	unsigned int sendlen = sizeof(cmd) - sizeof(cmd.storge_item_list[0]) * (GUILD_STORAGE_MAX_GRID_NUM - cmd.count);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&cmd, sendlen);
}

void GuildStorage::SyncStorgeLog(Role *role)
{
	static Protocol::SCGuildStorageLogList gsll;
	m_guild->GetGuildEventManager().GetStorageLogList(&gsll);

	unsigned int sendlen = sizeof(gsll) - sizeof(gsll.log_list[0]) * (MAX_STORAGE_EVENT_COUNT - gsll.count);
	if (nullptr == role)
	{
		m_guild->GetGuildMemberManager().SendMessageToPrincipal(Protocol::GUILD_NOTIFY_TYPE_STORAGE_LOG, (const char *)&gsll, sendlen);
	}
	else
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&gsll, sendlen);
	}
}
