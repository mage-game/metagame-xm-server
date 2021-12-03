#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "equipment/equipmentfunction.h"
#include "item/itempool.h"
#include "obj/character/role.h"
#include "gamelog.h"
#include "globalconfig/globalconfig.h"
#include "other/event/eventhandler.hpp"
#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "equipment/equipmentmanager.h"
#include "other/sublock/sublock.hpp"

// 装备合成
bool EquipFunction::EquipCompound(Role *role, int equip_index)
{
	if (NULL == role || equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX)
	{
		return false;
	}

	if (role->GetSublock()->CheckHasLock() || role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}
	
	ItemGridData equip_data;
	if (!role->GetEquipmentManager()->GetEquipmentGridData(equip_index, &equip_data) || equip_data.item_id <= 0)
	{
		return false;
	}

	const ItemBase *equip_base = ITEMPOOL->GetItem(equip_data.item_id);
	if (NULL == equip_base || equip_base->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)
	{
		return false;
	}

	EquipmentParam *equip_param = static_cast<EquipmentParam*>(equip_data.param);
	if (NULL == equip_param) return false;

	short xianpin_count = 0;
	for (int i = 0; i < ItemNamespace::MAX_XIANPIN_NUM; i++)
	{
		const EquipXianPinConfig *xianpin_cfg = GLOBALCONFIG->GetEquipConfig().GetXianPinEquipCfg(equip_param->xianpin_type_list[i]);
		if (NULL == xianpin_cfg)
		{
			equip_param->xianpin_type_list[i] = 0;
			continue;
		}

		xianpin_count++;
	}

	const EquipmentCompoundCfg *compound_cfg = GLOBALCONFIG->GetEquipConfig().GetEquipCompoundConfig(equip_data.item_id, xianpin_count);
	if (NULL == compound_cfg || compound_cfg->gain_item <= 0)
	{
		role->NoticeNum(errornum::EN_EQUI_COMPOUND_STUFF_ERROR);
		return false;
	}

	const ItemBase *compound_equip_base = ITEMPOOL->GetItem(compound_cfg->gain_item);
	if (NULL == compound_equip_base || compound_equip_base->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)
	{
		return false;
	}

	if (role->GetLevel() < compound_equip_base->GetLimitLevel())
	{
		role->NoticeNum(errornum::EN_EQUI_COMPOUND_LEVEL_LIMIT);
		return false;
	}

	if (!role->IsProfSuitable((char)compound_equip_base->GetLimitProf()))
	{
		return false;
	}

	const Equipment *equipment = (const Equipment *)(equip_base);
	const Equipment *compound_equipment = (const Equipment *)(compound_equip_base);
	if (equipment->GetEquipType() != compound_equipment->GetEquipType())
	{
		return false;
	}

	if (!Equipment::IsMatchIndex(compound_equipment->GetEquipType(), equip_index))
	{
		return false;
	}

	// 检查消耗
	g_consume_list.Reset();
	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	stuff_list[0].item_id = compound_cfg->stuff_item.item_id;
	stuff_list[0].num = compound_cfg->stuff_item.num;
	stuff_list[0].buyable = false;

	if (!ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &g_consume_list, true))
	{
		return false;
	}

	// 消耗
	if (g_consume_list.count <= 0 || !role->GetKnapsack()->ConsumeItemByIndexList(g_consume_list.count, g_consume_list.index_list, g_consume_list.num_list, "EquipCompound"))
	{
		return false;
	}

	char old_param_string[1024];
	const char *old_equip_param_string = EquipFunction::EquipParamDataClientString(role->GetUID(), compound_cfg->item_id, true, 0, equip_param, NULL);
	if (NULL != old_equip_param_string)
	{
		memcpy(old_param_string, old_equip_param_string, sizeof(old_param_string));
	}

	// 给与仙品属性
	const EquipXianPinConfig *add_xianpin_cfg = GLOBALCONFIG->GetEquipConfig().GetXianPinEquipCfg(compound_cfg->compound_xianpin);
	if (NULL != add_xianpin_cfg)
	{
		for (int i = 0;  i < ItemNamespace::MAX_XIANPIN_NUM; i++)
		{
			if (equip_param->xianpin_type_list[i] <= 0)
			{
				equip_param->xianpin_type_list[i] = add_xianpin_cfg->xianpin_type;
				break;
			}
		}
	}

	// 修改装备属性
	equip_data.item_id = compound_cfg->gain_item;
	equip_data.is_bind = true;
	role->GetEquipmentManager()->SetItemGridData(equip_index, equip_data);
	role->GetEquipmentManager()->NotifyStrengLevel(role);

	if (0 != compound_cfg->is_notice)
	{
		const char *new_equip_param_string = EquipFunction::EquipParamDataClientString(role->GetUID(), compound_cfg->gain_item, true, 0, equip_param, NULL);
		if (NULL != new_equip_param_string)
		{
			if (Equipment::I_COLOR_ORANGE == equip_base->GetColor())
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_item_equip_compound_1, role->GetUID(),
					role->GetName(), (int)role->GetCamp(), compound_cfg->item_id, old_param_string, compound_cfg->gain_item, new_equip_param_string);
				if (length > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
				}
			}
			else
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_item_equip_compound_2, role->GetUID(),
					role->GetName(), (int)role->GetCamp(), compound_cfg->item_id, old_param_string, compound_cfg->xianpin_count + 1, compound_cfg->gain_item, new_equip_param_string);
				if (length > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}
	}

	role->GetEquipmentManager()->RefreshPutOnInfo(static_cast<short>(equip_index), EquipmentManager::APPR_QUALITY);

	gamelog::g_log_equipment.printf(LL_INFO, "[EquipFunction::EquipCompound Succ][user[%s %d], old_equip_id[%d], new_equip_id[%d], add_xianpin_type:%d, stuff_id:%d stuff_count:%d",
		role->GetName(), role->GetUID(), equip_data.item_id, compound_cfg->gain_item, compound_cfg->compound_xianpin, compound_cfg->stuff_item.item_id, compound_cfg->stuff_item.num);

	return true;
}