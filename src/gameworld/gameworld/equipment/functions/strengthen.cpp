#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"

#include "servercommon/struct/itemlistparam.h"

#include "equipment/equipmentfunction.h"
#include "equipment/equipment.h"

#include "item/itempool.h"

#include "obj/character/role.h"

#include "gamelog.h"

#include "globalconfig/globalconfig.h"
#include "globalconfig/equipconfig.h"

#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/servercommon.h"
#include "servercommon/activitydef.hpp"

#include "config/logicconfigmanager.hpp"
#include "other/vip/vipconfig.hpp"
#include "config/activityconfig/zhuxieconfig.hpp"

#include "scene/guildshadow/guildshadow.hpp"
#include "protocal/msgother.h"
#include "protocal/msgrole.h"
#include "equipment/equipmentmanager.h"
#include "other/shop/shop.hpp"
#include "other/event/eventhandler.hpp"
#include "other/vip/vip.hpp"
#include "other/sublock/sublock.hpp"
#include "other/xianzunka/xianzunka.hpp"

// װ��ǿ��
bool EquipFunction::EquipStrengthen(Role *role, bool is_puton, bool select_bind_first, bool is_auto_buy, short equip_index, bool use_protect_item, short flag, int *old_level, int *coin_cost, int *gold_cost)
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

	ItemGridData equip_data;		// ��Ʒ�ڽ�ɫ������ԣ��������Ƿ�󶨵ȵ�
	const Equipment *equip = NULL;	// ��Ʒ��������

	is_puton = true;
	EQUIP_FUNCTION_GET_EQUIP(is_puton, equip_index, equip_data, equip);

	if(NULL == equip){return false;}
	
	if (!equip->CanStrengthen())
	{
		role->NoticeNum(errornum::EN_EQUIP_FUNC_CANNOT_STRENGTHEN);
		return false;
	}

	if (!EquipFunction::CanStrength(equip->GetEquipType()))
	{
		role->NoticeNum(errornum::EN_EQUIP_FUNC_CANNOT_STRENGTHEN);
		return false;
	}

	//EquipmentParam *equip_param = (EquipmentParam *)equip_data.param;	// װ���ɱ�ĸ�������
	int grid_strengthen_level = equip_mgr->GetEquipGridStrengthenLevel(equip_index);

	if (grid_strengthen_level >= EquipConfig::MAX_STRENGTH_LEVEL)
	{
		role->NoticeNum(errornum::EN_EQUIP_FUNC_STRENGTHEN_REACH_LEVELBOUND);
		return false;
	}

	const StrengthBaseConfig *strength_base_cfg = GLOBALCONFIG->GetEquipConfig().GetStrengthBaseCfg(equip->GetEquipType() - Equipment::E_TYPE_MIN, grid_strengthen_level + 1);	// ǿ�����ñ�
	if (NULL == strength_base_cfg)	// ���ǿ���ȼ�
	{
		role->NoticeNum(errornum::EN_EQUIP_FUNC_CONFIG_ERROR);
		return false;
	}

	{
		//���װ���ȼ�
		if (equip->GetLimitLevel() < strength_base_cfg->equip_level)
		{
			role->NoticeNum(errornum::EN_EQUIP_FUNC_INLAY_EQUIP_LEVEL);
			return false;
		}
	}

	// ǿ�����ƣ��ĳ�װ���Դ��Ľ�������
	if (equip->GetOrder() < strength_base_cfg->need_order)	// ����
	{
		role->NoticeNum(errornum::EN_EQUIP_FUNC_QUALITY_NOT_ENOUGH);
		return false;
	}

	const UInt16 old_strengthen_level = grid_strengthen_level;

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	{
		// ��ȡ�����ĵı��������б�

		bool nobind_first = true;
		if (select_bind_first)					// �ֶ�ѡ���˰�����
		{
			nobind_first = false;
		}
	
		short stuff_count = 0;

		// ����1
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = strength_base_cfg->stuff_id;
		stuff_list[stuff_count].num = strength_base_cfg->stuff_count;
		stuff_list[stuff_count].buyable = is_auto_buy ? true : false;
		++ stuff_count;
		
		// ���˷�
		if (use_protect_item && strength_base_cfg->protect_stuff_id > 0 && strength_base_cfg->protect_stuff_count > 0)
		{
			stuff_list[stuff_count].item_id = strength_base_cfg->protect_stuff_id;
			stuff_list[stuff_count].num = strength_base_cfg->protect_stuff_count;
			stuff_list[stuff_count].buyable = false;
			++ stuff_count;
		}

		if (!ItemExtern::GetConsumeListByOrder(role, nobind_first, stuff_count, stuff_list, &consume_list,true))
		{
			role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return false;
		}
	}

	{
		// �����
		Money *money = knapsack->GetMoney();
		if (strength_base_cfg->coin > 0 && !money->AllCoinMoreThan(strength_base_cfg->coin))
		{
			role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
			return false;
		}
	}

	// ���
	if (strength_base_cfg->coin > 0)
	{
		if (!role->GetKnapsack()->GetMoney()->UseAllCoin(strength_base_cfg->coin, "EquipStrengthen", true)) return false;
		if (NULL != coin_cost) *coin_cost = strength_base_cfg->coin;

		EventHandler::Instance().OnStrengthenConsumeCoin(role, strength_base_cfg->coin);
	}

	// ����
	if (!ItemExtern::ConsumeItem(role, consume_list, "EquipStrengthen"))
	{
		return false;
	}

	{
		int vip_add_suc_rate = LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_STRENGTH_SUC_ADD_RATE);
		const int xianzunka_add_succ_rate = role->GetXianZunKa()->GetAddEquipStrengthSuccRate();

		short operate_result = 1;
		if ((RandomNum(100) < strength_base_cfg->base_succ_rate + vip_add_suc_rate + xianzunka_add_succ_rate)) // ǿ���ɹ�
		{
			equip_mgr->AddEquiGridStrengthen(equip_index, 1);	// ǿ���ӵ�װ��������

			if (strength_base_cfg->is_broadcast)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_item_equip_strengthen, role->GetUID(),
					role->GetName(), (int)role->GetCamp(), /*equip->GetItemName()*/(int)equip->GetItemId(),equip_mgr->GetEquipGridStrengthenLevel(equip_index));
				if (length > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
				}
			}

			//const StrengthTotalLvlAddConfig *old_cfg = GLOBALCONFIG->GetEquipConfig().GetStrengthTotallvlAddCfg(old_total_level);
			//const int total_level = role->GetEquipmentManager()->GetTotalStrengthen();
			//const StrengthTotalLvlAddConfig *now_cfg = GLOBALCONFIG->GetEquipConfig().GetStrengthTotallvlAddCfg(total_level);

			//if (NULL != now_cfg && (NULL == old_cfg || now_cfg->total_strength_level > old_cfg->total_strength_level))
			//{
			//	role->NoticeNum(noticenum::NT_STRENGTH_TAOZHUANG_SUCC);

				//if (1 == now_cfg->is_broadcast)
				//{
				//	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_item_equip_total_strengthen, UidToInt(role->GetUserId()),
				//		role->GetName(), (int)role->GetCamp(), total_level, STREGNGTHEN_TAOZHUANG, now_cfg->seq);
				//	if (length > 0) 
				//	{
				//		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
				//	}
				//}
			//}
		}
		else	// ǿ��ʧ��
		{
			operate_result = 0;
			if(!use_protect_item && strength_base_cfg->is_downlevel_on_fail == Equipment::E_STRENGTHEN_DOWNLEVEL_ON_FAIL)
			{
				equip_mgr->AddEquiGridStrengthen(equip_index, -1);	// ǿ��ʧ�ܵ���
			}
		}

		if (is_puton)
		{
			equip_mgr->RefreshPutOnInfo(equip_index, EquipmentManager::APRR_STRENGTH);
			role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);		
		}	

		if (is_puton)
		{
			// ���ð�
// 			if (consume_list.bind_item_num > 0)
// 			{
// 				equip_mgr->GridSetBind(equip_index, false);
// 			}

			equip_mgr->SetEquipGridDirty(equip_index);
		}
		else
		{
			// ���ð�
// 			if (consume_list.bind_item_num > 0)
// 			{
// 				knapsack->GridSetBind(equip_index, false);
// 			}

			knapsack->SetItemDirty(equip_index);
		}

		role->SendOperateResult(Protocol::SCOperateResult::OP_EQUIP_STRENGTHEN, operate_result);
	}

	gamelog::g_log_equipment.printf(LL_INFO,"[EquipFunction::EquipStrengthen Succ][user[%s %d] role_level[%d] [equip_name:%s equip_id:%d "
		"strengthen_level:%d new_strengthen_level:%d cost_gold:%d "
		"cost_stuff_id:%d cost_stuff_count:%d"
		"cost_stuff_bind_count:%d cost_lucky_item:%d cost_luck_count:%d]",
		role->GetName(), UidToInt(role->GetUserId()), role->GetLevel(), equip->GetItemName(), (int)equip->GetItemId(), 
		(int)old_strengthen_level, (int)equip_mgr->GetEquipGridStrengthenLevel(equip_index), (int)consume_list.need_gold_num,
		(int)strength_base_cfg->stuff_id, (int)strength_base_cfg->stuff_count,
		(int)consume_list.bind_item_num, (int)strength_base_cfg->protect_stuff_id, (int)strength_base_cfg->protect_stuff_count);

	EventHandler::Instance().OnStrengthenEquipOperate(role, equip_index, old_strengthen_level, equip_mgr->GetEquipGridStrengthenLevel(equip_index));

	return true;
}

// ����װ��ǿ���ȼ�
bool EquipFunction::GMResetStrengthenLevel(Role *role, bool is_puton, short equip_index, int lvl, bool dirty)
{
	if (NULL == role)
	{
		return false;
	}

	if (role->GetSublock()->CheckHasLock()) return false;

	Knapsack *knapsack = role->GetKnapsack();
	EquipmentManager *equip_mgr = role->GetEquipmentManager();

	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	ItemGridData equip_data;
	const Equipment *equip = NULL;

	is_puton = true;
	EQUIP_FUNCTION_GET_EQUIP(is_puton, equip_index, equip_data, equip);

	if (!equip->CanStrengthen())
	{
		role->NoticeNum(errornum::EN_EQUIP_FUNC_CANNOT_STRENGTHEN);
		return false;
	}

	EquipmentParam *equip_param = (EquipmentParam *)equip_data.param;
	equip_param->strengthen_level = lvl;

	if (dirty)
	{
		if (is_puton)
		{
			equip_mgr->SetEquipGridDirty(equip_index);
		}
		else
		{
			knapsack->SetItemDirty(equip_index);
		}
	}

	return true;
}
