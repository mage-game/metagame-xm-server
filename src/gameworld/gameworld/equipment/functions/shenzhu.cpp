#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"

#include "servercommon/struct/itemlistparam.h"

#include "equipment/equipmentfunction.h"
#include "equipment/equipment.h"

#include "item/itempool.h"
#include "item/itemfunction.h"
#include "item/itemextern.hpp"

#include "obj/character/role.h"

#include "other/event/eventhandler.hpp"

#include "gamelog.h"

#include "globalconfig/globalconfig.h"
#include "globalconfig/equipconfig.h"

#include "protocal/msgother.h"
#include "engineadapter.h"

#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "equipment/equipmentmanager.h"
#include "other/sublock/sublock.hpp"

// 装备神铸
bool EquipFunction::EquipShenOP(Role *role, bool is_puton, bool select_bind_first, short equip_index)
{
	if (NULL == role)
	{
		return false;
	}

	if (role->GetSublock()->CheckHasLock()) return false;

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	EquipmentManager *equip_mgr = role->GetEquipmentManager();
	if(equip_mgr == NULL)
	{
		return false;
	}

	ItemGridData equip_data;
	const Equipment *equip = NULL;

	is_puton = true;
	EQUIP_FUNCTION_GET_EQUIP(is_puton, equip_index, equip_data, equip);
	int grid_shenzhu_level = equip_mgr->GetEquipGridShenzhuLevel(equip_index); 

	const ShenOPConfig *shen_op_cfg = GLOBALCONFIG->GetEquipConfig().GetShenOPCfg(equip->GetEquipType() - Equipment::E_TYPE_MIN, grid_shenzhu_level + 1);
	if (NULL == shen_op_cfg)
	{
		role->NoticeNum(errornum::EN_EQUIP_FUNC_CANNOT_SHENZHU);
		return false;
	}

	if (equip->GetLimitLevel() < shen_op_cfg->equip_level)
	{
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	{
		// 获取可消耗的背包材料列表

		bool nobind_first = true;
		if (select_bind_first)					// 手动选择了绑定优先
		{
			nobind_first = false;
		}

		short stuff_count = 0;
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = shen_op_cfg->stuff_id;
		stuff_list[stuff_count].num = shen_op_cfg->stuff_count;
		stuff_list[stuff_count].buyable = false;
		++ stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(role, nobind_first, stuff_count, stuff_list, &consume_list, true))
		{
			return false;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(role, consume_list, "ShenZhu"))
	{
		return false;
	}

	equip_mgr->AddEquiGridShenzhu(equip_index);	// 神铸加到装备格子里

	if (is_puton)
	{
		// 设置绑定
// 		if (consume_list.bind_item_num > 0)
// 		{
// 			equip_mgr->GridSetBind(equip_index, false);
// 		}

		equip_mgr->SetEquipGridDirty(equip_index);
		equip_mgr->RefreshPutOnInfo(equip_index, EquipmentManager::APPR_SHENZHU);

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);					
	}
	else
	{
		// 设置绑定
// 		if (consume_list.bind_item_num > 0)
// 		{
// 			knapsack->GridSetBind(equip_index, false);
// 		}
		knapsack->SetItemDirty(equip_index);
	}

	EventHandler::Instance().OnEquipShenzhu(role);

	role->NoticeNum(noticenum::NT_EQUIP_SHENZHU_SUCC);

	if (0 != shen_op_cfg->is_broadcast)
	{
		EquipmentParam equip_param = *(EquipmentParam *)equip_data.param;
		const char *equip_param_string = EquipFunction::EquipParamDataClientString(UidToInt(role->GetUserId()), equip_data.item_id, true, 0, &equip_param, NULL);
		if (NULL != equip_param_string)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_item_equip_shenzhu_up_shen_level, UidToInt(role->GetUserId()),
				role->GetName(), (int)role->GetCamp(), (int)equip_data.item_id, equip_param_string, shen_op_cfg->shen_level, shen_op_cfg->attr_percent);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
			}
		}
	}
	
	gamelog::g_log_equipment.printf(LL_INFO,"[EquipFunction::Shenzhu Succ][user[%s %d] role_level[%d], [equip_name:%s equip_id:%d "
		"old_shen:%d new_shen:%d stuff_id:%d stuff_count:%d]",
		role->GetName(), UidToInt(role->GetUserId()), role->GetLevel(), equip->GetItemName(), (int)equip->GetItemId(), 
		(int)grid_shenzhu_level, (int)equip_mgr->GetEquipGridShenzhuLevel(equip_index), (int)shen_op_cfg->stuff_id, (int)shen_op_cfg->stuff_count);


	//传闻
	//const ShenOPTotalConfig *cfg = GLOBALCONFIG->GetEquipConfig().GetShenOPTotalCfg(equip_mgr->GetmTotalShenZhuLevel(), true);
	//if(NULL != cfg)
	//{
	//	if (1 == cfg->is_broadcast)
	//	{
	//		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_item_equip_shenzhu_total, UidToInt(role->GetUserId()),
	//			role->GetName(), (int)role->GetCamp(), equip_mgr->GetmTotalShenZhuLevel());
	//		if (length > 0)
	//		{
	//			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
	//		}
	//	}
	//}

	return true;
}