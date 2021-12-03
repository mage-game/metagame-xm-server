#include "servercommon/struct/itemlistparam.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"

#include "globalconfig/globalconfig.h"

#include "engineadapter.h"
#include "stonemananger.hpp"

#include "protocal/msgitem.h"
#include "protocal/msgscene.h"
#include "protocal/msgrole.h"

#include "item/knapsack.h"
#include "item/itempool.h"

#include "scene/scenemanager.h"
#include "scene/scene.h"

#include "gamelog.h"

#include "world.h"

#include "obj/character/role.h"

#include "other/event/eventhandler.hpp"

#include "other/route/mailroute.hpp"
#include "servercommon/noticenum.h"
#include "equipmentmanager.h"
#include "item/knapsack.h"
#include "equipment/equipmentfunction.h"
#include "config/logicconfigmanager.hpp"
#include "other/shop/shop.hpp"
#include "other/vip/vip.hpp"
#include "other/capability/capability.hpp"
#include "other/sublock/sublock.hpp"
#include "other/fb/rolepatafb.hpp"
#include "other/talent/talent.hpp"
#include "other/rolebigsmallgoal/rolebigsmallgoal.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "config/joinlimitconfig.hpp"
#include "task/taskrecorder.h"
#include "servercommon/serverconfig/crossconfig.hpp"

StoneManager::StoneManager() : m_role(NULL), m_total_stone_level(0), m_max_stone_level(0), m_system_msg_time(0)
{

}

StoneManager::~StoneManager()
{

}

void StoneManager::SetRole(Role *role)
{
	m_role = role;
}

void StoneManager::Init(Role *role, const StoneParam &stone_param)
{
	m_role = role;
	m_stone_param = stone_param;
}

void StoneManager::GetInitParam(StoneParam *stone_param)
{
	*stone_param = m_stone_param;
}

void StoneManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();
		CharIntAttrs attrs_add_pre;			//存放某些百分比对本系统属性的加成
		attrs_add_pre.Reset();

		CharIntAttrs attrs_add_extra;		//力量、守护宝石对全身宝石的加成
		attrs_add_extra.Reset();

		m_total_stone_level = 0;
		m_max_stone_level = 0;

		for (int i = 0; i < MAX_STONE_EQUIP_PART; ++i)
		{
			for (int j = 0; j < MAX_STONE_COUNT; ++j)
			{
				const StoneItem &stone_item =  m_stone_param.stones_list[i].stone_list[j];
				if (stone_item.Invalid())
				{
					continue;
				}

				const StoneCfg * stone_cfg = GLOBALCONFIG->GetEquipConfig().GetStoneCfg(stone_item.stone_id);
				if (NULL == stone_cfg)
				{
					continue;
				}

				m_total_stone_level += stone_cfg->level;
				if (stone_cfg->level > m_max_stone_level)
				{
					m_max_stone_level = stone_cfg->level;
				}
				
				if (stone_cfg->stone_type <= STONE_TYPE_2)
				{
					if (CharIntAttrs::IsRoleBaseAttr(stone_cfg->attr_type1) && stone_cfg->att_value1 > 0)
					{
//						m_attrs_add.m_attrs[stone_cfg->attr_type1] += stone_cfg->att_value1;
						m_attrs_add.AddValue(stone_cfg->attr_type1, stone_cfg->att_value1);
					}

					if (CharIntAttrs::IsRoleBaseAttr(stone_cfg->attr_type2) && stone_cfg->att_value2 > 0)
					{
//						m_attrs_add.m_attrs[stone_cfg->attr_type2] += stone_cfg->att_value2;
						m_attrs_add.AddValue(stone_cfg->attr_type2, stone_cfg->att_value2);
					}
				}
				//else if (stone_cfg->stone_type >= STONE_TYPE_3 && stone_cfg->stone_type < STONE_TYPE_5)
				//{
				//	if (CharIntAttrs::IsRoleBaseAttr(stone_cfg->attr_type1) && stone_cfg->att_value1 > 0)
				//	{
				//		attrs_add_pre.m_attrs[stone_cfg->attr_type1] += (int)(m_attrs_add.m_attrs[stone_cfg->attr_type1] * (stone_cfg->att_value1 * ROLE_ATTR_PER));	//力量、守护宝石加百分比加到本系统属性
				//	}

				//	if (CharIntAttrs::IsRoleBaseAttr(stone_cfg->attr_type2) && stone_cfg->att_value2 > 0)
				//	{
				//		attrs_add_pre.m_attrs[stone_cfg->attr_type2] += (int)(m_attrs_add.m_attrs[stone_cfg->attr_type2] * (stone_cfg->att_value2 * ROLE_ATTR_PER));	//力量、守护宝石加百分比加到本系统属性
				//	}
				//}
				else if (stone_cfg->stone_type == STONE_TYPE_5)	//荣耀宝石
				{
					if (CharIntAttrs::IsRoleBaseAttr(stone_cfg->attr_type1) && stone_cfg->att_value1 > 0)
					{
//						m_attrs_add.m_attrs[stone_cfg->attr_type1] += stone_cfg->att_value1;	
						m_attrs_add.AddValue(stone_cfg->attr_type1, stone_cfg->att_value1);
					}

					if (CharIntAttrs::IsRoleBaseAttr(stone_cfg->attr_type2) && stone_cfg->att_value2 > 0)
					{
//						m_attrs_add.m_attrs[stone_cfg->attr_type2] += stone_cfg->att_value2;	
						m_attrs_add.AddValue(stone_cfg->attr_type2, stone_cfg->att_value2);
					}
				}

				// 计算大目标对全身宝石加成的属性
				{
					int add_value = m_role->GetRoleBigSmallGoal()->GetRoleBigSmallGoalAddAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_STONE);
					if (add_value > 0)
					{
						if (stone_cfg->stone_type <= STONE_TYPE_2)
						{
							if (CharIntAttrs::IsRoleBaseAttr(stone_cfg->attr_type1) && stone_cfg->att_value1 > 0)
							{
//								m_attrs_add.m_attrs[stone_cfg->attr_type1] += static_cast<Attribute>(stone_cfg->att_value1 * (add_value * ROLE_ATTR_PER));
								Attribute attr_value1 = static_cast<Attribute>(stone_cfg->att_value1 * (add_value * ROLE_ATTR_PER));
								m_attrs_add.AddValue(stone_cfg->attr_type1, attr_value1);
							}

							if (CharIntAttrs::IsRoleBaseAttr(stone_cfg->attr_type2) && stone_cfg->att_value2 > 0)
							{
//								m_attrs_add.m_attrs[stone_cfg->attr_type2] += static_cast<Attribute>(stone_cfg->att_value2 * (add_value * ROLE_ATTR_PER));
								Attribute attr_value2 = static_cast<Attribute>(stone_cfg->att_value2 * (add_value * ROLE_ATTR_PER));
								m_attrs_add.AddValue(stone_cfg->attr_type2, attr_value2);
							}
						}
						
// 						else if (stone_cfg->stone_type == STONE_TYPE_5)	//荣耀宝石
// 						{
// 							if (CharIntAttrs::IsRoleBaseAttr(stone_cfg->attr_type1) && stone_cfg->att_value1 > 0)
// 							{
// 								m_attrs_add.m_attrs[stone_cfg->attr_type1] += static_cast<Attribute>(stone_cfg->att_value1 * (add_value * ROLE_ATTR_PER));
// 							}
// 
// 							if (CharIntAttrs::IsRoleBaseAttr(stone_cfg->attr_type2) && stone_cfg->att_value2 > 0)
// 							{
// 								m_attrs_add.m_attrs[stone_cfg->attr_type2] += static_cast<Attribute>(stone_cfg->att_value2 * (add_value * ROLE_ATTR_PER));
// 							}
// 						}
					}
				}
			}
		}

		//力量、守护宝石改成全身宝石攻击、防御、气血的百分比加成（策划需求，2019-01-22）
		for (int i = 0; i < MAX_STONE_EQUIP_PART; ++i)
		{
			for (int j = 0; j < MAX_STONE_COUNT; ++j)
			{
				const StoneItem &stone_item = m_stone_param.stones_list[i].stone_list[j];
				if (stone_item.Invalid())
				{
					continue;
				}

				const StoneCfg * stone_cfg = GLOBALCONFIG->GetEquipConfig().GetStoneCfg(stone_item.stone_id);
				if (NULL == stone_cfg)
				{
					continue;
				}

				//计算力量、守护宝石对全身宝石加成的属性
				if (stone_cfg->stone_type >= STONE_TYPE_3 && stone_cfg->stone_type < STONE_TYPE_5)
				{
					if (CharIntAttrs::IsRoleBaseAttr(stone_cfg->attr_type1) && stone_cfg->att_value1 > 0)
					{
//						attrs_add_extra.m_attrs[stone_cfg->attr_type1] += static_cast<Attribute>(m_attrs_add.m_attrs[stone_cfg->attr_type1] * (stone_cfg->att_value1 * ROLE_ATTR_PER));	//力量、守护宝石加百分比加到本系统属性
						Attribute attr_value1 = static_cast<Attribute>(m_attrs_add.m_attrs[stone_cfg->attr_type1] * (stone_cfg->att_value1 * ROLE_ATTR_PER));
						attrs_add_extra.AddValue(stone_cfg->attr_type1, attr_value1);
					}

					if (CharIntAttrs::IsRoleBaseAttr(stone_cfg->attr_type2) && stone_cfg->att_value2 > 0)
					{
//						attrs_add_extra.m_attrs[stone_cfg->attr_type2] += static_cast<Attribute>(m_attrs_add.m_attrs[stone_cfg->attr_type2] * (stone_cfg->att_value2 * ROLE_ATTR_PER));	//力量、守护宝石加百分比加到本系统属性
						Attribute attr_value2 = static_cast<Attribute>(m_attrs_add.m_attrs[stone_cfg->attr_type2] * (stone_cfg->att_value2 * ROLE_ATTR_PER));
						attrs_add_extra.AddValue(stone_cfg->attr_type2, attr_value2);
					}
				}

				// 计算大目标对全身宝石加成的属性（力量、守护宝石对全身宝石加成）
				{
					int add_value = m_role->GetRoleBigSmallGoal()->GetRoleBigSmallGoalAddAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_STONE);
					if (add_value > 0)
					{
						if (stone_cfg->stone_type >= STONE_TYPE_3 && stone_cfg->stone_type < STONE_TYPE_5)
						{
							if (CharIntAttrs::IsRoleBaseAttr(stone_cfg->attr_type1) && stone_cfg->att_value1 > 0)
							{
//								attrs_add_pre.m_attrs[stone_cfg->attr_type1] += static_cast<Attribute>(m_attrs_add.m_attrs[stone_cfg->attr_type1] * (stone_cfg->att_value1 * ROLE_ATTR_PER) * (add_value * ROLE_ATTR_PER));	//大目标加成 = 本系统属性 * 宝石加成 * 大目标加成
								Attribute attr_value1 = static_cast<Attribute>(m_attrs_add.m_attrs[stone_cfg->attr_type1] * (stone_cfg->att_value1 * ROLE_ATTR_PER) * (add_value * ROLE_ATTR_PER));
								attrs_add_pre.AddValue(stone_cfg->attr_type1, attr_value1);
							}

							if (CharIntAttrs::IsRoleBaseAttr(stone_cfg->attr_type2) && stone_cfg->att_value2 > 0)
							{
//								attrs_add_pre.m_attrs[stone_cfg->attr_type2] += static_cast<Attribute>(m_attrs_add.m_attrs[stone_cfg->attr_type2] * (stone_cfg->att_value2 * ROLE_ATTR_PER) * (add_value * ROLE_ATTR_PER)); //大目标加成 = 本系统属性 * 宝石加成 * 大目标加成
								Attribute attr_value2 = static_cast<Attribute>(m_attrs_add.m_attrs[stone_cfg->attr_type2] * (stone_cfg->att_value2 * ROLE_ATTR_PER) * (add_value * ROLE_ATTR_PER));
								attrs_add_pre.AddValue(stone_cfg->attr_type2, attr_value2);
							}
						}
					}
				}
			}
		}

		m_attrs_add.Add(attrs_add_pre.m_attrs);		//大小目标加成的属性
		m_attrs_add.Add(attrs_add_extra.m_attrs);	//力量、守护宝石加成的属性

		//if (m_total_stone_level > 0)
		//{
		//	const StoneExAdd *cfg = GLOBALCONFIG->GetEquipConfig().GetStoneExAdd(m_total_stone_level);
		//	if (NULL != cfg)
		//	{
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg->maxhp;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg->gongji;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg->fangyu;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg->mingzhong;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg->shanbi;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += cfg->baoji;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += cfg->jianren;
		//	}
		//}

		// 人物大目标固定属性
		m_role->GetRoleBigSmallGoal()->RoleBigSmallGoalAddSpecialAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_STONE, m_attrs_add);

		m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_STONE, m_attrs_add, m_attrs_add);

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_GEM, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

bool StoneManager::IsRepeatInlay(int equip_part, int stone_type)
{
	for (int i = 0; i < MAX_STONE_COUNT; i++)
	{
		StoneItem &stone_item = m_stone_param.stones_list[equip_part].stone_list[i];
		const StoneCfg *stone_cfg = GLOBALCONFIG->GetEquipConfig().GetStoneCfg(stone_item.stone_id);
		if (NULL != stone_cfg)
		{
			if (stone_cfg->stone_type == stone_type)
			{
				return true;
			}
		}
	}

	return false;
}

// 宝石镶嵌
bool StoneManager::InlayStone(int equip_part, int stone_slot, const int index_in_bag)
{
	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}
	
	if (m_role->GetSublock()->CheckHasLock()) return false;

	if (equip_part < 0 || equip_part >= MAX_STONE_EQUIP_PART)
	{
		return false;
	}

	if (stone_slot < 0 || stone_slot >= MAX_STONE_COUNT)
	{
		return false;
	}

	//根据强化等级判断孔数是否符合
	ItemGridData equip_data;
	if (!m_role->GetEquipmentManager()->GetEquipmentGridData(equip_part, &equip_data))
	{
		return false;
	}

	//EquipmentParam *equip_param = (EquipmentParam *)equip_data.param;
	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(equip_data.item_id);
	if (NULL == equip)
	{
		return false;
	}

	EquipmentManager *equip_mgr = m_role->GetEquipmentManager();
	if(equip_mgr == NULL)
	{
		return false;
	}

// 	const StrengthBaseConfig *strength_base_cfg = GLOBALCONFIG->GetEquipConfig().GetStrengthBaseCfg(equip->GetEquipType() - Equipment::E_TYPE_MIN, equip_mgr->GetEquiGridStrengthenLevel(equip_part));
// 	if (NULL == strength_base_cfg)
// 	{
// 		return false;
// 	}
// 
// 	if (strength_base_cfg->stone_hole_num < stone_slot + 1)
// 	{
// 		return false;
// 	}
	
	ItemGridData grid_data;
	if (!m_role->GetKnapsack()->GetItemGridData(index_in_bag, &grid_data) || grid_data.Invalid())
	{
		return false;
	}

	int stone_id = grid_data.item_id;
	
	const StoneCfg *target_stone_cfg = GLOBALCONFIG->GetEquipConfig().GetStoneCfg(stone_id);
	if (NULL == target_stone_cfg)
	{
		return false;
	}

// 	if (stone_slot != target_stone_cfg->stone_type)
// 	{
// 		return false;
// 	}

	if (this->IsRepeatInlay(equip_part, target_stone_cfg->stone_type))
	{
		return false;
	}

	StoneItem &stone_item = m_stone_param.stones_list[equip_part].stone_list[stone_slot];

	//已镶嵌的槽不能重复镶嵌
	if (!stone_item.Invalid())
	{
		m_role->NoticeNum(errornum::EN_EQUIP_FUNC_INLAY_HAVE_STONE_IN_SLOT);
		return false;
	}

	if (!m_role->GetKnapsack()->ConsumeItemByIndex(index_in_bag, 1, "StoneInlay"))
	{
		return false;	
	}

	stone_item.stone_id = stone_id;
	const int old_total_level = m_total_stone_level;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_STONE);
	m_role->NoticeNum(noticenum::NT_INLAY_GEMSTONE_SUCC);

	const int now_total_level = m_total_stone_level;
	const StoneExAdd *old_cfg = GLOBALCONFIG->GetEquipConfig().GetStoneExAdd(old_total_level);
	const StoneExAdd *now_cfg = GLOBALCONFIG->GetEquipConfig().GetStoneExAdd(now_total_level);
	if (NULL != now_cfg && (NULL == old_cfg || now_cfg->total_level > old_cfg->total_level))
	{
		if (1 == now_cfg->is_broadcast && static_cast<signed int>(EngineAdapter::Instance().Time()) >= m_system_msg_time)
		{
			m_system_msg_time = static_cast<signed int>(EngineAdapter::Instance().Time()) + SYSTEM_MSG_TIME_INTERVAL;

			/*int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_item_stone_up_level, UidToInt(m_role->GetUserId()),
				m_role->GetName(), (int)m_role->GetCamp(), now_cfg->total_level);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
			}*/
		}
	}

	this->SendToClient(Protocol::SCStoneInfo::STONE_REASON_INLAY);

	EventHandler::Instance().OnInlayStone(m_role, stone_id);

	gamelog::g_log_equipment.printf(LL_INFO, "StoneInlay role(%d,%s) role_level[%d] stone(%d,%d,%d)",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), equip_part, stone_slot, stone_id);

	return true;
}

// 拆除宝石
bool StoneManager::UnInlayStone(int equip_part, int stone_slot)
{
	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (m_role->GetSublock()->CheckHasLock()) return false;

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	if (equip_part < 0 || equip_part >= MAX_STONE_EQUIP_PART)
	{
		return false;
	}

	if (stone_slot < 0 || stone_slot >= MAX_STONE_COUNT)
	{
		return false;
	}

	StoneItem &stone_item = m_stone_param.stones_list[equip_part].stone_list[stone_slot];
	if (stone_item.Invalid())
	{
		return false;
	}
	
	const StoneCfg *target_stone_cfg = GLOBALCONFIG->GetEquipConfig().GetStoneCfg(stone_item.stone_id);
	if (NULL == target_stone_cfg || target_stone_cfg->item_id <= 0)
	{
		return false;
	}

	stone_item.Reset();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_STONE);
	m_role->NoticeNum(noticenum::NT_UNINLAY_GEMSTONE_SUCC);

	ItemDataWrapper item_wrapper(target_stone_cfg->item_id, 1, 1);
	if (!m_role->GetKnapsack()->Put(item_wrapper, PUT_REASON_NO_NOTICE)) return false;

	this->SendToClient(Protocol::SCStoneInfo::STONE_REASON_UNINLAY);

	return true;
}

//升级宝石
bool StoneManager::Uplevelstone(int equip_part, int stone_slot, int uplevel_type)
{
	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (m_role->GetSublock()->CheckHasLock()) return false;

	if (equip_part >= MAX_STONE_EQUIP_PART || equip_part < 0)
	{
		return false;
	}

	if (stone_slot >= MAX_STONE_COUNT || stone_slot < 0)
	{
		return false;
	}

	StoneItem &stone_item = m_stone_param.stones_list[equip_part].stone_list[stone_slot];
	if (stone_item.Invalid())
	{
		return false;
	}

	const int old_total_level = m_total_stone_level;
	const StoneCfg *now_stone_cfg = GLOBALCONFIG->GetEquipConfig().GetStoneCfg(stone_item.stone_id);
	const StoneCfg *target_stone_cfg = GLOBALCONFIG->GetEquipConfig().GetStoneCfg(stone_item.stone_id + 1);
	if (NULL == target_stone_cfg || NULL == now_stone_cfg)
	{
		return false;
	}

	if(target_stone_cfg->stone_type != now_stone_cfg->stone_type)
	{
		return false;
	}

	if (target_stone_cfg->level >= MAX_STONE_LEVEL)
	{
		return false;
	}
	
	int consume_count = 0;
	ItemID consume_id_list[MAX_STONE_LEVEL] = {0};
	short consume_num_list[MAX_STONE_LEVEL] = {0};
	bool is_succ = false;

	int need_num = target_stone_cfg->level_need_num - 1;
	int cur_level = target_stone_cfg->level - 1;

	for (int i = cur_level; i > 0 && i > cur_level - 10; i--)
	{
		ItemID item_id = stone_item.stone_id + i - cur_level;
		int total_item_num = m_role->GetKnapsack()->GetItemNum(item_id);
		if (total_item_num < need_num)
		{
			if (total_item_num > 0)
			{
				consume_id_list[consume_count] = item_id;
				consume_num_list[consume_count] = total_item_num;
				consume_count++;
			}

			need_num -= total_item_num;
			if (i > 1)
			{
				need_num *= target_stone_cfg->level_need_num;
			}
		}
		else
		{
			consume_id_list[consume_count] = item_id;
			consume_num_list[consume_count] = need_num;
			consume_count++;
			is_succ = true;
			break;
		}
	}

	int need_gold = 0;
	int need_bind_gold = 0;

	if (!is_succ && 0 == uplevel_type && target_stone_cfg->level_need_num > 0)
	{
		int buy_info_count = 0;
		ItemExtern::BuyItemInfo buy_info_list[2];

		int need_num_1 = need_num % (target_stone_cfg->level_need_num * target_stone_cfg->level_need_num);
		if (need_num_1 > 0)
		{
			buy_info_list[buy_info_count].item_id = stone_item.stone_id - cur_level + 1;
			buy_info_list[buy_info_count].item_count = need_num_1;

			buy_info_count++;
		}

		int need_num_3 = need_num / (target_stone_cfg->level_need_num * target_stone_cfg->level_need_num);
		if (need_num_3 > 0)
		{
			buy_info_list[buy_info_count].item_id = stone_item.stone_id - cur_level + 3;
			buy_info_list[buy_info_count].item_count = need_num_3;

			buy_info_count++;
		}
		
		for (int i = 0; i < buy_info_count; i++)
		{
			if (!LOGIC_CONFIG->GetShop().GetShopItemGoldPrice(buy_info_list[i].item_id, &buy_info_list[i].consume_gold, &buy_info_list[i].consume_bind_gold))
			{
				m_role->NoticeNum(errornum::EN_EQUIP_BAY_STONE_FAIL);
				return false;
			}
			
			need_gold += buy_info_list[i].consume_gold * buy_info_list[i].item_count;
			need_bind_gold += buy_info_list[i].consume_bind_gold * buy_info_list[i].item_count;
		}

		// 消耗绑定元宝
		if (need_bind_gold > 0) 
		{
			if (m_role->GetKnapsack()->GetMoney()->UseGoldBind(need_bind_gold, "StoneUpLevel"))
			{
				is_succ = true;
				EventHandler::Instance().OnShopBuyItem(m_role, buy_info_count, buy_info_list, 0, need_bind_gold);
			}
		}

		// 消耗非绑元宝
		if (!is_succ && need_gold > 0) 
		{	
			if (!m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "StoneUpLevel"))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return false;
			}

			is_succ = true;
			EventHandler::Instance().OnShopBuyItem(m_role, buy_info_count, buy_info_list, 0, need_bind_gold);
		}
	}

	if (!is_succ)
	{
		return false;
	}

	if (consume_count > 0)
	{
		for (int i = 0; i < consume_count; i++)
		{
			if (consume_num_list[i] > 0)
			{
				if(!m_role->GetKnapsack()->ConsumeItem(consume_id_list[i], consume_num_list[i], "StoneUpLevel"))
					return false;
			}
		}
	}

	m_role->NoticeNum(noticenum::NT_STONE_UP_SUCC);

	stone_item.stone_id += 1;
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_STONE);

	const int now_total_level = m_total_stone_level;
	const StoneExAdd *old_cfg = GLOBALCONFIG->GetEquipConfig().GetStoneExAdd(old_total_level);
	const StoneExAdd *now_cfg = GLOBALCONFIG->GetEquipConfig().GetStoneExAdd(now_total_level);
	if (NULL != now_cfg && (NULL == old_cfg || now_cfg->total_level > old_cfg->total_level))
	{
		if (1 == now_cfg->is_broadcast)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_item_stone_up_level, UidToInt(m_role->GetUserId()),
				m_role->GetName(), (int)m_role->GetCamp(), now_cfg->total_level);
			if (length > 0) 
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
			}
		}
	}

	this->SendToClient(Protocol::SCStoneInfo::STONE_REASON_UPLEVEL);
	
	EventHandler::Instance().OnItemCompose(m_role, stone_item.stone_id, target_stone_cfg->level_need_num);
	EventHandler::Instance().OnUplevelstone(m_role);

	return true;
}

UNSTD_STATIC_CHECK(Equipment::E_INDEX_MAX == MAX_STONE_EQUIP_PART);

void StoneManager::SendToClient(short reason)
{
	this->CheckFuncOpen();	//检查宝石大小目标

	Protocol::SCStoneInfo scinfo;
	scinfo.reason = reason;

	for (int i = 0; i < MAX_STONE_EQUIP_PART; ++i)
	{
		for (int j = 0; j < MAX_STONE_COUNT; ++j)
		{
			scinfo.stones_list[i].stone_list[j].stone_id = m_stone_param.stones_list[i].stone_list[j].stone_id;
		}
	}

	scinfo.total_stone_level = (short) m_total_stone_level;

	for (int i = 0; i< MAX_STONE_EQUIP_PART; ++i)
	{
		scinfo.stone_limit_flag[i] = this->GetStoneLimitFlag(i);
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scinfo, sizeof(scinfo));
}

short StoneManager::GetStoneLimitFlag(int equip_index)
{
	short limit_flag = 0;
	for (int i = 0; i < MAX_STONE_COUNT; i++)
	{	
		//const StoneGridOpenLimit *limit_cfg = GLOBALCONFIG->GetEquipConfig().GetStoneOpenLimit(i);
		//if (NULL == limit_cfg) continue;

		const StoneGridOpenLimitByStrengthLevel *limit_cfg = GLOBALCONFIG->GetEquipConfig().GetStoneOpenLimitByEquipIndex(i, equip_index);
		if (NULL == limit_cfg) continue;

		if (STONE_LIMIT_TYPE_STRENGTHEN == limit_cfg->limit)
		{
			EquipmentManager *equip_mgr = m_role->GetEquipmentManager();
			if (equip_mgr == NULL)
			{
				continue;
			}

			if (equip_mgr->GetEquipGridStrengthenLevel(equip_index) >= limit_cfg->param1)
			{
				limit_flag |= (1 << i);
			}
		}

		else if(STONE_LIMIT_TYPE_VIP == limit_cfg->limit)
		{
			if (m_role->GetVip()->GetVipLevel() >= limit_cfg->param1)
			{
				limit_flag |= (1 << i);
			}
		}

		else if(STONE_LIMIT_TYPE_EQUIP_LIMIT_LEVEL == limit_cfg->limit)
		{
			EquipmentManager *equip_mgr = m_role->GetEquipmentManager();
			if (equip_mgr == NULL)
			{
				continue;
			}
			const Equipment *equip = equip_mgr->GetEquipBaseInfoByIndex(equip_index);
			if (NULL == equip)
			{
				continue;
			}

			if (equip->GetLimitLevel() >= limit_cfg->param1)
			{
				limit_flag |= (1 << i);
			}
		}

		//if (STONE_LIMIT_TYPE_NULL == limit_cfg->limit)
		//{
		//	limit_flag |= (1 << i);
		//}

		//else if(STONE_LIMIT_TYPE_PATA == limit_cfg->limit)
		//{
		//	if (m_role->GetRolePataFB()->GetPassLevel() >= limit_cfg->param1)
		//	{
		//		limit_flag |= (1 << i);
		//	}
		//}
		//else if(STONE_LIMIT_TYPE_VIP == limit_cfg->limit)
		//{
		//	if (m_role->GetVip()->GetVipLevel() >= limit_cfg->param1)
		//	{
		//		limit_flag |= (1 << i);
		//	}
		//}


	}

	return limit_flag;
}

StonesOnPart StoneManager::GetStonePartByEquipType(int equip_type)
{
	StonesOnPart stone_info;

	if (equip_type >= Equipment::E_TYPE_MIN && equip_type <= Equipment::E_TYPE_MAX)
	{
		int equip_index = Equipment::E_INDEX_MAX;
		for (int i = Equipment::E_INDEX_TOUKUI; i < Equipment::E_INDEX_MAX; ++ i)
		{
			if (Equipment::IsMatchIndex(equip_type, i))
			{
				equip_index = i;
				break;
			}
		}

		stone_info = this->GetStonePartByEquipIndex(equip_index);
	}

	return stone_info;
}

StonesOnPart StoneManager::GetStonePartByEquipIndex(int equip_index)
{
	StonesOnPart stone_info;

	if (equip_index >= 0 && equip_index < MAX_STONE_EQUIP_PART)
	{
		stone_info = m_stone_param.stones_list[equip_index];
	}

	return stone_info;
}

bool StoneManager::IsSatisfyMagicalPrecious(int stone_level, int stone_type)
{
	if (stone_level <= 0 || stone_level > EquipConfig::MAX_STONE_LEVEL) return false;

	if (stone_type < 0 || stone_type >= MAX_STONE_COUNT) return false;

	for (int i = 0; i < MAX_STONE_EQUIP_PART; ++i)
	{
		for (int j = 0; j < MAX_STONE_COUNT; ++j)
		{
			StoneItem &stone_item =  m_stone_param.stones_list[i].stone_list[j];
			if (stone_item.Invalid())
			{
				continue;
			}

			const StoneCfg * stone_cfg = GLOBALCONFIG->GetEquipConfig().GetStoneCfg(stone_item.stone_id);
			if (NULL == stone_cfg)
			{
				continue;
			}

			if (stone_cfg->level < stone_level)
			{
				continue;
			}

			if (stone_cfg->stone_type != stone_type)
			{
				continue;
			}

			return true;
		}
	}

	return false;
}

bool StoneManager::IsInlayStoneByLevel(int stone_level)
{
	if (stone_level <= 0 || stone_level > EquipConfig::MAX_STONE_LEVEL) return false;

	for (int i = 0; i < MAX_STONE_EQUIP_PART; ++i)
	{
		for (int j = 0; j < MAX_STONE_COUNT; ++j)
		{
			StoneItem &stone_item = m_stone_param.stones_list[i].stone_list[j];
			if (stone_item.Invalid())
			{
				continue;
			}

			const StoneCfg * stone_cfg = GLOBALCONFIG->GetEquipConfig().GetStoneCfg(stone_item.stone_id);
			if (NULL == stone_cfg)
			{
				continue;
			}

			if (stone_cfg->level != stone_level)
			{
				continue;
			}

			return true;
		}
	}

	return false;
}

void StoneManager::CheckFuncOpen()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (m_role->GetRoleBigSmallGoal()->IsFuncActive(ROLE_BIG_SMALL_GOAL_SYSTEM_STONE))
	{
		return;
	}

	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_EQUIPMENT_FORGE);
	if (fun_open_cfg != nullptr && opengame_day >= fun_open_cfg->opengame_day)
	{
		if (m_role->GetLevel() >= fun_open_cfg->role_level || m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
		{
			m_role->GetRoleBigSmallGoal()->OnOpenFunc(ROLE_BIG_SMALL_GOAL_SYSTEM_STONE);
		}
	}
}
