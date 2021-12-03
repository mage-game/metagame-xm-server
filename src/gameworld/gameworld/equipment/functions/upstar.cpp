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
#include "other/event/eventhandler.hpp"
#include "equipment/equipmentmanager.h"
#include "other/shop/roleshop.hpp"
#include "other/sublock/sublock.hpp"
#include "other/magicalprecious/magicalpreciouscfg.hpp"
#include "other/magicalprecious/magicalprecious.hpp"

// 装备升星
bool EquipFunction::EquipUpStar(Role *role, short equip_index)
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
	if (equip_mgr == NULL)
	{
		return false;
	}

	ItemGridData equip_data;
	const Equipment *equip = NULL;

	EQUIP_FUNCTION_GET_EQUIP(true, equip_index, equip_data, equip);

	int grid_star_level = equip_mgr->GetEquipGridStarLevel(equip_index);

	const UpStarConfig *star_cfg = GLOBALCONFIG->GetEquipConfig().GetUpStarCfg(equip->GetEquipType() - Equipment::E_TYPE_MIN, grid_star_level + 1);
	if (NULL == star_cfg)
	{
		role->NoticeNum(errornum::EN_EQUIP_FUNC_CANNOT_UPSTAR);
		return false;
	}

	if (equip->GetLimitLevel() < star_cfg->equip_level)
	{
		role->NoticeNum(errornum::EN_EQUIP_FUNC_CANNOT_UPSTAR);
		return false;
	}

	if (role->GetRoleShop()->GetChestShopMojing() >= star_cfg->need_mojing - equip_mgr->GetEquipGridStarExp(equip_index))
	{
		int mojing = star_cfg->need_mojing - equip_mgr->GetEquipGridStarExp(equip_index);
		role->GetRoleShop()->AddChestShopMojing(-mojing, "equipUpstar");

		equip_mgr->SetEquiGridStarExp(equip_index, 0);

		equip_mgr->AddEquiGridStar(equip_index);

		equip_mgr->RefreshPutOnInfo(equip_index, EquipmentManager::APPR_UPSTAR);

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);					
		//role->NoticeNum(noticenum::NT_EQUIP_UP_STAR_SUCC);

		//升星传闻
		if (1 == star_cfg->is_broadcast)
		{
			EquipmentParam *equip_param = (EquipmentParam *)equip_data.param;
			const char *equip_param_string = EquipFunction::EquipParamDataClientString(UidToInt(role->GetUserId()), equip->GetItemId(), true, equip_data.invalid_time, equip_param, NULL);
			if (NULL != equip_param_string)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_item_equip_up_star,
					UidToInt(role->GetUserId()), role->GetName(), (int)role->GetCamp(), equip_data.item_id, equip_param_string, equip_mgr->GetEquipGridStarLevel(equip_index));
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
			}
		}

		//全身升星传闻
		const UpStarTotalConfig *cfg = GLOBALCONFIG->GetEquipConfig().GetUpStarTotalCfg(equip_mgr->GetTotalStarLevel(), true);
		if(NULL != cfg)
		{
			if (1 == cfg->is_broadcast)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_item_equip_upstar_total, UidToInt(role->GetUserId()),
					role->GetName(), (int)role->GetCamp(), equip_mgr->GetTotalStarLevel(), UPSTAR_TAOZHUANG, cfg->seq);
				if (length > 0) 
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}

		EventHandler::Instance().OnUpStarEquipOperate(role, equip_index, grid_star_level, grid_star_level + 1);
	}
	else
	{
		equip_mgr->SetEquiGridStarExp(equip_index, equip_mgr->GetEquipGridStarExp(equip_index) + role->GetRoleShop()->GetChestShopMojing());
		role->GetRoleShop()->AddChestShopMojing(-role->GetRoleShop()->GetChestShopMojing(), "equipUpstar");
	}

	equip_mgr->SetEquipGridDirty(equip_index);

	return true;
}