#include "equipment/equipmentfunction.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "equipment/equipmentmanager.h"
#include "item/itempool.h"
#include "obj/character/role.h"
#include "globalconfig/globalconfig.h"
#include "other/event/eventhandler.hpp"
#include "world.h"
#include "other/sublock/sublock.hpp"
#include "protocal/msgrole.h"

bool EquipFunction::EquipUpEternity(Role *role, int equip_index, bool is_auto_buy)
{
	if (NULL == role) return false;

	if (role->GetSublock()->CheckHasLock()) return false;

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	EquipmentManager *equip_mgr = role->GetEquipmentManager();
	if (equip_mgr == NULL) return false;

	ItemGridData equip_data;
	const Equipment *equip = NULL;

	EQUIP_FUNCTION_GET_EQUIP(true, equip_index, equip_data, equip);

	if (NULL == equip) { return false; }

	int eternity_level = equip_mgr->GetEquipGridEternityLevel(equip_index);

	const EquipEternityCfg *eternity_cfg = GLOBALCONFIG->GetEquipConfig().GetEquipEternityCfg(equip_index, eternity_level);
	const EquipEternityCfg *next_eternity_cfg = GLOBALCONFIG->GetEquipConfig().GetEquipEternityCfg(equip_index, eternity_level + 1);
	if (NULL == eternity_cfg || NULL == next_eternity_cfg)
	{
		role->NoticeNum(errornum::EN_EQUIP_FUNC_ETERNITY_LEVEL_MAX);
		return false;
	}

	if (next_eternity_cfg->show_level > equip->GetLimitLevel())
	{
		role->NoticeNum(errornum::EN_EQUIP_FUNC_ETERNITY_EQUIP_LEVEL_LIMIT);
		return false;
	}

	if (role->GetJingJieLevel() < next_eternity_cfg->jingjie_level)
	{
		role->NoticeNum(errornum::EN_EQUIP_FUNC_ETERNITY_EQUIP_LEVEL_LIMIT);
		return false;
	}

	g_consume_list.Reset();

	{
		int stuff_count = 0;
		// 材料1
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = eternity_cfg->stuff_id;
		stuff_list[stuff_count].num = eternity_cfg->stuff_count;
		stuff_list[stuff_count].buyable = is_auto_buy ? true : false;
		++stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(role, false, stuff_count, stuff_list, &g_consume_list, true))
		{
			return false;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(role, g_consume_list, "EquipFunction::EquipUpEternity"))
		{
			return false;
		}
	}

	short old_min_eternity_level = equip_mgr->GetMinEternityLevel();

	equip_mgr->AddEquiGridEternityLevel(equip_index, 1);
	equip_mgr->RefreshPutOnInfo(equip_index, EquipmentManager::APRR_ETERNITY);

	short new_min_eternity_level = equip_mgr->GetMinEternityLevel();
	if (new_min_eternity_level > old_min_eternity_level)
	{
		equip_mgr->UseEternityLevel(new_min_eternity_level);
		role->RefreshApperance(true);

		const EquipEternitySuitCfg *suit_cfg = GLOBALCONFIG->GetEquipConfig().GetEquipEternitySuitCfg(new_min_eternity_level);
		if (NULL != suit_cfg && suit_cfg->is_broadcast)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_item_eternity_suit, UidToInt(role->GetUserId()),
				role->GetName(), (int)role->GetCamp(), suit_cfg->name);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
			}
		}
	}

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);

	equip_mgr->SendEquipmentGridInfoToRole();
	role->SendOperateResult(Protocol::SCOperateResult::OP_UP_ETERNITY, 1);

	if (next_eternity_cfg->is_broadcast)
	{
		EquipmentParam equip_param = *(EquipmentParam *)equip_data.param;
		equip_param.param1 = eternity_level;
		const char *equip_param_string = EquipFunction::EquipParamDataClientString(UidToInt(role->GetUserId()), equip_data.item_id, true, 0, &equip_param, NULL);

		char temp_equip_param_string[1024] = {0};
		F_STRNCPY(temp_equip_param_string, equip_param_string, sizeof(temp_equip_param_string));				//记录提升前的永恒等级

		equip_param.param1 = eternity_level + 1;
		equip_param_string = EquipFunction::EquipParamDataClientString(UidToInt(role->GetUserId()), equip_data.item_id, true, 0, &equip_param, NULL);

		if (NULL != equip_param_string)
		{
			int length = 0;

			length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_item_eternity_up_level_1, UidToInt(role->GetUserId()),
					role->GetName(), (int)role->GetCamp(), (int)equip_data.item_id, temp_equip_param_string, (int)equip_data.item_id, equip_param_string);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
			}
		}
	}

	return true;
}

bool EquipFunction::UseEternityLevel(Role *role, short eternity_level)
{
	EquipmentManager *equip_mgr = role->GetEquipmentManager();
	if (equip_mgr == NULL) return false;

	short min_eternity_level = equip_mgr->GetMinEternityLevel();
	if (eternity_level > min_eternity_level)
	{
		return false;
	}

	if (0 != eternity_level)
	{
		const EquipEternitySuitCfg *eternity_suit_cfg = GLOBALCONFIG->GetEquipConfig().GetEquipEternitySuitCfg(eternity_level);
		if (NULL == eternity_suit_cfg)
		{
			return false;
		}
	}

	equip_mgr->UseEternityLevel(eternity_level);
	equip_mgr->SendEquipmentGridInfoToRole();
	role->RefreshApperance(true);

	return true;
}