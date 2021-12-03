#include "gamelog.h"
#include "item/knapsack.h"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "config/logicconfigmanager.hpp"
#include "other/lieming/liemingconfig.hpp"
#include "protocal/msglieming.h"
#include "protocal/msgrole.h"
#include "servercommon/noticenum.h"
#include "obj/character/attribute.hpp"
#include "other/route/mailroute.hpp"
#include "other/event/eventhandler.hpp"
#include "liemingmanager.h"
#include "globalconfig/globalconfig.h"

#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/servercommon.h"
#include "other/capability/capability.hpp"
#include "other/magicalprecious/magicalpreciouscfg.hpp"
#include "other/magicalprecious/magicalprecious.hpp"

#include <set>
#include <algorithm>
#include <iterator>
#include "item/itempool.h"

LieMingManager::LieMingManager() : m_role(NULL), m_liehun_pool_hunshou_count(0)
{

}

LieMingManager::~LieMingManager()
{

}
//
//void LieMingManager::Init(Role *role, const LieMingParam &lieming_param, const LieMingHunshouBagParam &hunshou_bag)
//{
//	m_role = role;
//	m_param = lieming_param;
//	m_bag_param = hunshou_bag;
//
//	for (int i = 0; i < LIEMING_LIEHUN_POOL_MAX_COUNT; ++ i)
//	{
//		if (0 == m_bag_param.chouhun_pool[i])
//		{
//			break;
//		}
//
//		++ m_liehun_pool_hunshou_count;
//	}
//}
//
//void LieMingManager::GetInitParam(LieMingParam *lieming_param, LieMingHunshouBagParam *bag_param)
//{
//	*lieming_param = m_param;
//	*bag_param = m_bag_param;
//}
//
//void LieMingManager::GeiInitLieMingParam(LieMingParam *lieming_param)
//{
//	*lieming_param = m_param;
//}
//
//void LieMingManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
//{
//	if (is_recalc)
//	{
//		m_attrs_add.Reset();
//
//		// 附魂
//		for (int hunshou_slot_idx = 0; hunshou_slot_idx < LIEMING_FUHUN_SLOT_COUNT; ++ hunshou_slot_idx)
//		{
//			if (m_param.slot_list[hunshou_slot_idx].IsValid())
//			{
//				int attr_type = 0; int attr_val = 0;
//				if (LOGIC_CONFIG->GetLieMingCfg().GetHunshouAddAttr(m_param.slot_list[hunshou_slot_idx].hunshou_id,
//					m_param.slot_list[hunshou_slot_idx].level, &attr_type, &attr_val))
//				{
//					m_attrs_add.m_attrs[attr_type] += GETPROFVALUE(m_role->GetProf(), attr_type, attr_val);
//				}
//			}
//		}
//	}
//
//	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_LIEMING, m_attrs_add);
//
//	//策划需求，猎命的战力算入精灵战力中
//	m_role->GetCapability()->SetCap(CAPABILITY_TYPE_JINGLING, m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_JINGLING) + m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_LIEMING));
//
//	base_add.Add(m_attrs_add.m_attrs);
//}
//
//void LieMingManager::OnFuhun(int slot_idx, int grid_idx)
//{
//	if (slot_idx < 0 || slot_idx >= LIEMING_FUHUN_SLOT_COUNT || 
//		grid_idx < 0 || grid_idx >= LIEMING_HUNSHOU_BAG_GRID_MAX_COUNT)
//	{
//		return;
//	}
//
//	if (!m_param.IsSoltActive(slot_idx + 1))
//	{
//		m_role->NoticeNum(errornum::EN_LIEMING_SOLT_NOT_ACTIVTY);
//		return;
//	}
//	
//	LieMingHunShou &grid_item = m_bag_param.grid_list[grid_idx];
//	if (!grid_item.IsValid())
//	{
//		return;
//	}
//
//	const LieMingHunshouConfig *grid_hunshou_cfg = LOGIC_CONFIG->GetLieMingCfg().GetHunshouCfg(grid_item.hunshou_id);
//	if (NULL == grid_hunshou_cfg)
//	{
//		return;
//	}
//	
//	if (m_param.slot_list[slot_idx].IsValid())
//	{
//		m_role->NoticeNum(errornum::EN_LIEMING_EQUIP_FULING_SLOT_USED);
//		return;
//	}
//
//	for (int i = 0; i < LIEMING_FUHUN_SLOT_COUNT; ++ i)
//	{
//		int hunshou_id = m_param.slot_list[i].hunshou_id;
//		const LieMingHunshouConfig *slot_hunshou_cfg = LOGIC_CONFIG->GetLieMingCfg().GetHunshouCfg(hunshou_id);
//		
//		if (NULL != slot_hunshou_cfg)
//		{
//			if (grid_hunshou_cfg->hunshou_type == slot_hunshou_cfg->hunshou_type)
//			{
//				m_role->NoticeNum(errornum::EN_LIEMING_EQUIP_FULING_SAME_HUNSHOU_LIMIT);
//				return;
//			}
//		}
//	}
//
//	m_param.slot_list[slot_idx] = grid_item;
//	grid_item.Reset();
//
//	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LIEMING);
//
//	this->SendEquipInfo();
//	this->SendBagInfo();
//}
//
//void LieMingManager::OnTakeoffFuhun(int slot_idx)
//{
//	if (slot_idx < 0 || slot_idx >= LIEMING_FUHUN_SLOT_COUNT)
//	{
//		return;
//	}
//
//	if (!m_param.slot_list[slot_idx].IsValid())
//	{
//		return;
//	}
//
//	int grid_idx = this->GetBagFreeSlot();
//	if (grid_idx < 0 || grid_idx >= LIEMING_HUNSHOU_BAG_GRID_MAX_COUNT)
//	{
//		m_role->NoticeNum(errornum::EN_LIEMING_BAG_ALREAD_FULL);
//		return;
//	}
//
//	m_bag_param.grid_list[grid_idx] = m_param.slot_list[slot_idx];
//	m_param.slot_list[slot_idx].Reset();
//
//	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LIEMING);
//
//	this->SendEquipInfo();
//	this->SendBagInfo();
//}
//
//void LieMingManager::OnFuhunAddExp(int slot_idx, int target_level)
//{
//	if (slot_idx < 0 || slot_idx >= LIEMING_FUHUN_SLOT_COUNT)
//	{
//		return;
//	}
//
//	LieMingHunShou &hunshou_item = m_param.slot_list[slot_idx];
//	if (!hunshou_item.IsValid())
//	{
//		return;
//	}
//
//	const LieMingHunshouConfig *hunshou_cfg = LOGIC_CONFIG->GetLieMingCfg().GetHunshouCfg(hunshou_item.hunshou_id);
//	if (NULL == hunshou_cfg)
//	{
//		return;
//	}
//
//	if (m_bag_param.hunshou_exp <= 0)
//	{
//		m_role->NoticeNum(errornum::EN_LIEMING_FUHUN_EXP_LIMIT);
//		return;
//	}
//
//	int hunshou_max_level = LOGIC_CONFIG->GetLieMingCfg().GetHunshouMaxLevel(hunshou_cfg->hunshou_color);
//	if (target_level > hunshou_max_level)
//	{
//		target_level = hunshou_max_level;
//	}
//
//	if (hunshou_item.level >= target_level)
//	{
//		m_role->NoticeNum(errornum::EN_LIEMING_FUHUN_ADD_EXP_ALREADY_REACHED_LEVEL);
//		return;
//	}
//
//	long long tmp_total_exp = m_bag_param.hunshou_exp;
//	int old_level = hunshou_item.level;
//	long long old_exp = hunshou_item.curr_exp;
//	int loop_times = 0;
//
//	while (tmp_total_exp > 0 && loop_times ++ < 1024)
//	{
//		if (hunshou_item.level >= target_level)
//		{
//			break;
//		}
//
//		const LieMingHunshouLevelConfigItem *level_cfg = LOGIC_CONFIG->GetLieMingCfg().GetHunshouLevelCfg(hunshou_cfg->hunshou_color, hunshou_item.level);
//		if (level_cfg == nullptr)
//		{
//			continue;
//		}
//
//		long long delta_exp = level_cfg->need_exp - hunshou_item.curr_exp;
//
//		if (tmp_total_exp >= delta_exp)
//		{
//			tmp_total_exp -= delta_exp;
//			hunshou_item.curr_exp = 0;
//			++ hunshou_item.level;
//		}
//		else
//		{
//			hunshou_item.curr_exp += tmp_total_exp;
//			tmp_total_exp = 0;
//		}
//	}
//
//	long long old_bag_hunshou_exp = m_bag_param.hunshou_exp;
//
//	m_bag_param.hunshou_exp = tmp_total_exp;
//
//	if (old_level != hunshou_item.level)
//	{
//		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LIEMING);
//	}
//
//	this->SendEquipInfo();
//	this->SendBagInfo();
//
//	m_role->NoticeNum(noticenum::NT_LIEMING_UP_LEVEL_SUCC);
//
//	EventHandler::Instance().OnLieMingHunAddExp(m_role);
//
//	gamelog::g_log_lieming.printf(LL_INFO, "OnFuhunAddExp user %d %s, slot_idx %d, hunshou_id %d, old_level %d, old_exp %lld, new_level %d, new_exp %lld, "
//		"old_bag_hunshou_exp %lld, new_bag_hunshou_exp %lld",
//		m_role->GetUID(), m_role->GetName(), slot_idx, hunshou_item.hunshou_id, old_level, old_exp, hunshou_item.level, hunshou_item.curr_exp,
//		old_bag_hunshou_exp, m_bag_param.hunshou_exp);
//}
//
//bool LieMingManager::AddHunshou(int hunshou_id)
//{
//	int bag_free_slot = this->GetBagFreeSlot();
//	if (bag_free_slot < 0 || bag_free_slot >= LIEMING_HUNSHOU_BAG_GRID_MAX_COUNT)
//	{
//		m_role->NoticeNum(errornum::EN_LIEMING_BAG_ALREAD_FULL);
//		return false;
//	}
//
//	const LieMingHunshouConfig *hunshou_cfg = LOGIC_CONFIG->GetLieMingCfg().GetHunshouCfg(hunshou_id);
//	if (NULL == hunshou_cfg) return false;
//
//	m_bag_param.grid_list[bag_free_slot].hunshou_id = hunshou_id;
//	m_bag_param.grid_list[bag_free_slot].level = 1;
//	m_bag_param.grid_list[bag_free_slot].curr_exp = 0;
//
//	this->SendBagInfo();
//
//	return true;
//}
//
//int LieMingManager::Chouhun(bool notify_client, int chouhun_type, int *result_color)
//{
//	if (NULL == result_color)
//	{
//		return -100;
//	}
//
//	const char *chouhun_type_str = "";
//	bool notify_bag_change = false;
//
//	if (Protocol::LIEMING_HUNSHOU_OPERA_TYPE_CHOUHUN == chouhun_type)
//	{
//		notify_bag_change = true;
//		chouhun_type_str = "Chouhun";
//	}
//	else if (Protocol::LIEMING_HUNSHOU_OPERA_TYPE_BATCH_HUNSHOU == chouhun_type)
//	{
//		chouhun_type_str = "BatchChouhun";
//	}
//	else if (Protocol::LIEMING_HUNSHOU_OPERA_TYPE_SPECIAL_BATCH_HUNSHOU == chouhun_type)
//	{
//		chouhun_type_str = "SpecialBatchChouhun";
//	}
//	else 
//	{
//		return -1;
//	}
//
//	if (m_liehun_pool_hunshou_count >= LIEMING_LIEHUN_POOL_MAX_COUNT)
//	{
//		if (notify_client) m_role->NoticeNum(errornum::EN_LIEMING_LIEHUN_POOL_FULL);
//		return -2;
//	}
//
//	const LieMingChouhunConfig *chouhun_cfg = LOGIC_CONFIG->GetLieMingCfg().GetChouhunCfg(m_bag_param.chouhun_color);
//	if (NULL == chouhun_cfg)
//	{
//		return -3;
//	}
//
//	if (m_role->GetHunli() < chouhun_cfg->cost_hunli)
//	{
////		m_role->NoticeNum(errornum::EN_ROLE_HUNLI_NOT_ENOUGH);
//		return -4;
//	}
//
//	int ret_color = LIEMING_HUNSHOU_COLOR_INVALID;
//	int color_rand_val = RandomNum(100);
//
//	for (int i = 0; i < LIEMING_HUNSHOU_COLOR_MAX; ++ i)
//	{
//		if (color_rand_val < chouhun_cfg->rate_list[i])
//		{
//			ret_color = i;
//			break;
//		}
//
//		color_rand_val -= chouhun_cfg->rate_list[i];
//	}
//
//	int reward_hunshou_id = LIEMING_HUNSHOU_EXP_ID;
//
//	if (LIEMING_HUNSHOU_COLOR_INVALID != ret_color)
//	{
//		const LieMingChouhunHunshouWeightCfg *hunshou_weight_cfg = LOGIC_CONFIG->GetLieMingCfg().GetChouhunHunshouWeightCfg(ret_color);
//		if (NULL == hunshou_weight_cfg || hunshou_weight_cfg->cfg_count <= 0 || hunshou_weight_cfg->total_weight <= 0)
//		{
//			return -5;
//		}
//
//		if (hunshou_weight_cfg->total_weight <= 0)
//		{
//			return -6;
//		}
//
//		int hunshou_rand_val = RandomNum(hunshou_weight_cfg->total_weight);
//
//		for (int i = 0; i <hunshou_weight_cfg->cfg_count; ++ i)
//		{
//			if (hunshou_rand_val < hunshou_weight_cfg->cfg_list[i].weight)
//			{
//				reward_hunshou_id = hunshou_weight_cfg->cfg_list[i].hunshou_id;
//				break;
//			}
//
//			hunshou_rand_val -= hunshou_weight_cfg->cfg_list[i].weight;
//		}
//	}
//
//	//auto *cfg = LOGIC_CONFIG->GetLieMingCfg().GetHunshouCfg(reward_hunshou_id);	//经验兽不在魂兽配置，但此处仍使用经验兽id获取配置
//
//	//if (LIEMING_HUNSHOU_EXP_ID != reward_hunshou_id && NULL == cfg)	     
//	//{
//	//	return -7;
//	//}
//
//	//if (LIEMING_HUNSHOU_EXP_ID != reward_hunshou_id)
//	//{
//	//	*result_color = cfg->hunshou_color;
//	//}
//
//	if (reward_hunshou_id != LIEMING_HUNSHOU_EXP_ID)
//	{
//		auto *cfg = LOGIC_CONFIG->GetLieMingCfg().GetHunshouCfg(reward_hunshou_id);
//		if (cfg == NULL)
//		{
//			return -7;
//		}
//
//		*result_color = cfg->hunshou_color;
//	}
//
//	m_role->AddHunli(-1 * abs(chouhun_cfg->cost_hunli), "lieming");
//
//	int put_index  = m_liehun_pool_hunshou_count;
//	m_bag_param.chouhun_pool[put_index] = reward_hunshou_id;
//	++ m_liehun_pool_hunshou_count;
//
//	int old_chouhun_color = m_bag_param.chouhun_color;
//	if (RandomNum(100) < chouhun_cfg->advance_rate)
//	{
//		m_bag_param.chouhun_color = (m_bag_param.chouhun_color + 1) % LIEMING_CHOUHUN_COLOR_MAX;
//	}
//	else
//	{
//		m_bag_param.chouhun_color = 0;
//	}
//
//	if (notify_bag_change)
//	{
//		this->SendBagInfo();
//	}
//
//	EventHandler::Instance().OnLieMingChouHun(m_role);
//
//	//传闻
//	{
//		const LieMingHunshouConfig *hunshou_cfg = LOGIC_CONFIG->GetLieMingCfg().GetHunshouCfg(reward_hunshou_id);
//		if (NULL != hunshou_cfg && hunshou_cfg->hunshou_color >= LIEMING_CHOUHUN_COLOR_ROANGE)
//		{
//			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_link_lieming,
//				UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), reward_hunshou_id);
//			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
//		}
//	}
//	
//	// 日志
//	{
//		gamelog::g_log_lieming.printf(LL_INFO, "Chouhun type %s, user %d %s, reward_hunshou_id %d, put_idx %d, m_pool_count %d, "
//			"old_chouhun_color %d, new_chouhun_color %d, cost_hunli %d",
//			chouhun_type_str, m_role->GetUID(), m_role->GetName(), reward_hunshou_id, put_index, m_liehun_pool_hunshou_count,
//			old_chouhun_color, m_bag_param.chouhun_color, chouhun_cfg->cost_hunli);
//	}
//
//	return put_index;
//}
//
//bool LieMingManager::SuperChouhun()
//{
//	if (m_bag_param.chouhun_color >= LIEMING_CHOUHUN_COLOR_PURPLE)
//	{
//		m_role->NoticeNum(errornum::EN_LIEMING_ALREADY_MAX_CHOUHUN_COLOR);
//		return true;
//	}
//
//	int gold_price = LOGIC_CONFIG->GetLieMingCfg().GetOtherCfg().super_chouhun_price;
//
//	if (!m_role->GetKnapsack()->GetMoney()->UseGold(gold_price, "SuperChouhun"))
//	{
//		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
//		return false;
//	}
//
//	int old_chouhun_color = m_bag_param.chouhun_color;
//
//	m_bag_param.chouhun_color = LIEMING_CHOUHUN_COLOR_PURPLE;
//	this->SendBagInfo();
//	m_role->NoticeNum(noticenum::NT_OPERATE_SUCC);
//
//	EventHandler::Instance().OnLieMingChouHun(m_role);
//
//	gamelog::g_log_lieming.printf(LL_INFO, "SuperChouhun user %d %s, use_all_gold %d, old_chouhun_color %d, new_chouhun_color %d",
//		m_role->GetUID(), m_role->GetName(), gold_price, old_chouhun_color, m_bag_param.chouhun_color);
//
//	return true;
//}
//
//void LieMingManager::BatchChouhun()
//{
//	int chouhun_count = 0;
//	for (int i = 0; i < LIEMING_LIEHUN_POOL_MAX_COUNT; ++ i)
//	{
//		int result_color = -1;
//		int put_idx = this->Chouhun(0 == i, Protocol::LIEMING_HUNSHOU_OPERA_TYPE_BATCH_HUNSHOU, &result_color);
//		if (-4 == put_idx)	//魂力值不足，不能参与猎魂
//		{
//			m_role->SendOperateResult(Protocol::SCOperateResult::OP_LIEMING_CHOUHUN, 0);
//			break;
//		}
//
//		if (put_idx < 0 || put_idx >= LIEMING_LIEHUN_POOL_MAX_COUNT)
//		{
//			break;
//		}
//
//		chouhun_count ++;
//	}
//
//	if (chouhun_count > 0)
//	{
//		m_role->SendOperateResult(Protocol::SCOperateResult::OP_LIEMING_CHOUHUN, 1);
//	}
//
//	this->SendBagInfo();
//}
//
//void LieMingManager::SpecialBatchChouhun()
//{
//	int loop_count = 0;
//
//	if (m_liehun_pool_hunshou_count >= LIEMING_LIEHUN_POOL_MAX_COUNT)
//	{
//		m_role->NoticeNum(errornum::EN_LIEMING_LIEHUN_POOL_FULL);
//		return;
//	}
//	
//	while (m_liehun_pool_hunshou_count < LIEMING_LIEHUN_POOL_MAX_COUNT && loop_count ++ < 1024)
//	{
//		int result_color = -1;
//		int put_idx = this->Chouhun(1 == loop_count, Protocol::LIEMING_HUNSHOU_OPERA_TYPE_SPECIAL_BATCH_HUNSHOU, &result_color);
//		if (put_idx < 0 || put_idx >= LIEMING_LIEHUN_POOL_MAX_COUNT)
//		{
//			break;
//		}
//
//		int reward_hunshou_id = m_bag_param.chouhun_pool[put_idx];
//		long long sell_exp = 0;
//
//		if (LIEMING_HUNSHOU_EXP_ID == reward_hunshou_id)
//		{
//			sell_exp = LOGIC_CONFIG->GetLieMingCfg().CalSellExp(reward_hunshou_id, 1, 0);
//		}
//		else
//		{
//			const LieMingHunshouConfig *hunshou_cfg = LOGIC_CONFIG->GetLieMingCfg().GetHunshouCfg(reward_hunshou_id);
//			if (NULL != hunshou_cfg && hunshou_cfg->hunshou_color <= LIEMING_HUNSHOU_COLOR_BLUE)
//			{
//				sell_exp = LOGIC_CONFIG->GetLieMingCfg().CalSellExp(reward_hunshou_id, 1, 0);
//			}
//		}
//
//		if (sell_exp > 0)
//		{
//			long long old_hunshou_exp = m_bag_param.hunshou_exp; 
//
//			m_bag_param.hunshou_exp += sell_exp;
//			m_bag_param.chouhun_pool[put_idx] = 0;
//			-- m_liehun_pool_hunshou_count;
//
//			gamelog::g_log_lieming.printf(LL_INFO, "SpecialBatchChouhun_ConverToExp user %d %s, put_idx %d, hunshou_id %d, sell_exp %lld, old_hunshou_exp %lld, new_hunshou_exp %lld",
//				m_role->GetUID(), m_role->GetName(), put_idx, reward_hunshou_id, sell_exp, old_hunshou_exp, m_bag_param.hunshou_exp);
//		}
//	}
//
//	EventHandler::Instance().OnLieMingChouHun(m_role);
//
//	this->SendBagInfo();
//}
//
//void LieMingManager::PutHunshouBag(int pool_idx, bool is_notice)
//{
//	if (pool_idx < 0 || pool_idx >= m_liehun_pool_hunshou_count)
//	{
//		return;
//	}
//
//	int bag_free_slot = this->GetBagFreeSlot();
//	if (bag_free_slot < 0 || bag_free_slot >= LIEMING_HUNSHOU_BAG_GRID_MAX_COUNT)
//	{
//		m_role->NoticeNum(errornum::EN_LIEMING_BAG_ALREAD_FULL);
//		return;
//	}
//
//	short hunshou_id = m_bag_param.chouhun_pool[pool_idx];
//	if (hunshou_id <= 0 ||NULL == LOGIC_CONFIG->GetLieMingCfg().GetHunshouCfg(hunshou_id))
//	{
//		return;
//	}
//
//	m_bag_param.chouhun_pool[pool_idx] = m_bag_param.chouhun_pool[m_liehun_pool_hunshou_count - 1];
//	m_bag_param.chouhun_pool[m_liehun_pool_hunshou_count - 1] = 0;
//	-- m_liehun_pool_hunshou_count;
//
//	m_bag_param.grid_list[bag_free_slot].hunshou_id = hunshou_id;
//	m_bag_param.grid_list[bag_free_slot].level = 1;
//	m_bag_param.grid_list[bag_free_slot].curr_exp = 0;
//
//	if (is_notice)
//	{
//		this->SendBagInfo();
//	}
//
//	gamelog::g_log_lieming.printf(LL_INFO, "PutHunshouBag user %d %s, pool_idx %d, free_bag_idx %d, hunshou_id %d",
//		m_role->GetUID(), m_role->GetName(), pool_idx, bag_free_slot, hunshou_id);
//}
//
//void LieMingManager::OnOnekeyPutHunshouBag()
//{
//	for (int i = m_liehun_pool_hunshou_count - 1; i >= 0; i--)
//	{
//		this->PutHunshouBag(i, false);
//	}
//
//	this->SendBagInfo();
//}
//
//void LieMingManager::OnChouhunPoolConverToExp()
//{
//	if (m_liehun_pool_hunshou_count <= 0) 
//	{
//		return;
//	}
//
//	long long all_sell_exp = 0;
//	for (int i = 0; i < m_liehun_pool_hunshou_count; ++ i)
//	{
//		long long single_sell_exp = LOGIC_CONFIG->GetLieMingCfg().CalSellExp(m_bag_param.chouhun_pool[i], 1, 0);
//		if (single_sell_exp <= 0)
//		{
//			continue;
//		}
//
//		all_sell_exp += single_sell_exp;
//	}
//
//	int consume_hunshou_count = m_liehun_pool_hunshou_count; 
//
//	m_liehun_pool_hunshou_count = 0;
//	memset(m_bag_param.chouhun_pool, 0, sizeof(m_bag_param.chouhun_pool));
//
//	long long old_hunshou_exp = m_bag_param.hunshou_exp;
//
//	m_bag_param.hunshou_exp += all_sell_exp;
//
//	this->SendBagInfo();
//
//	gamelog::g_log_lieming.printf(LL_INFO, "OnChouhunPoolConverToExp user %d %s, all_sell_exp %lld, old_hunshou_exp %lld, new_hunshou_exp %lld, consume_hunsohou_count %d",
//		m_role->GetUID(), m_role->GetName(), all_sell_exp, old_hunshou_exp, m_bag_param.hunshou_exp, consume_hunshou_count);
//}
//
//void LieMingManager::OnChouhunPoolSingleItemConverToExp(int pool_idx)
//{
//	if (m_liehun_pool_hunshou_count <= 0 || pool_idx >= m_liehun_pool_hunshou_count) 
//	{
//		return;
//	}
//
//	long long single_sell_exp = LOGIC_CONFIG->GetLieMingCfg().CalSellExp(m_bag_param.chouhun_pool[pool_idx], 1, 0);
//	if (single_sell_exp <= 0)
//	{
//		return;
//	}
//
//	m_bag_param.chouhun_pool[pool_idx] = m_bag_param.chouhun_pool[m_liehun_pool_hunshou_count - 1];   // 当前pool_idx的消耗掉，把最后一个补到这个位置
//	m_bag_param.chouhun_pool[m_liehun_pool_hunshou_count - 1] = 0;
//	-- m_liehun_pool_hunshou_count;
//
//	long long old_hunshou_exp = m_bag_param.hunshou_exp;
//
//	m_bag_param.hunshou_exp += single_sell_exp;
//
//	this->SendBagInfo();
//
//	gamelog::g_log_lieming.printf(LL_INFO, "OnChouhunPoolSingleItemConverToExp user %d %s, single_sell_exp %lld, old_hunshou_exp %lld, new_hunshou_exp %lld, consume_hunsohou_count %d",
//		m_role->GetUID(), m_role->GetName(), single_sell_exp, old_hunshou_exp, m_bag_param.hunshou_exp, 1);
//}
//
//void LieMingManager::OnChouhunBagSingleItemConverToExp(int bag_idx)
//{
//	if (bag_idx < 0 || bag_idx >= LIEMING_HUNSHOU_BAG_GRID_MAX_COUNT) 
//	{
//		return;
//	}
//
//	LieMingHunShou *hunshou = &m_bag_param.grid_list[bag_idx];
//	if (!hunshou->IsValid())
//	{
//		return;
//	}
//
//	long long single_sell_exp = LOGIC_CONFIG->GetLieMingCfg().CalSellExp(hunshou->hunshou_id, hunshou->level, 0);
//	if (single_sell_exp <= 0)
//	{
//		return;
//	}
//
//	hunshou->Reset();
//
//	long long old_hunshou_exp = m_bag_param.hunshou_exp;
//
//	m_bag_param.hunshou_exp += single_sell_exp;
//
//	this->SendBagInfo();
//
//	gamelog::g_log_lieming.printf(LL_INFO, "OnChouhunBagSingleItemConverToExp user %d %s, single_sell_exp %lld, old_hunshou_exp %lld, new_hunshou_exp %lld, consume_hunsohou_count %d",
//		m_role->GetUID(), m_role->GetName(), single_sell_exp, old_hunshou_exp, m_bag_param.hunshou_exp, 1);
//}
//
//void LieMingManager::OnHunshouBagMerge()
//{
//	std::set<int> be_merge_set;
//	std::set<int> same_color_set;
//	int merge_index = -1, merge_color = LIEMING_HUNSHOU_COLOR_INVALID; // , merge_level = 0;
//
//	for (int i = 0; i < LIEMING_HUNSHOU_BAG_GRID_MAX_COUNT; ++ i)
//	{
//		const LieMingHunShou &item = m_bag_param.grid_list[i];
//		
//		if (m_bag_param.grid_list[i].IsValid())
//		{
//			const LieMingHunshouConfig *hunshou_cfg = LOGIC_CONFIG->GetLieMingCfg().GetHunshouCfg(item.hunshou_id);
//			if (NULL != hunshou_cfg)
//			{
//				int hunshou_max_level = LOGIC_CONFIG->GetLieMingCfg().GetHunshouMaxLevel(hunshou_cfg->hunshou_color);
//				if (item.level > hunshou_max_level)
//				{
//					continue;
//				}
//
//				bool new_item_better = false;
//
//				if (hunshou_cfg->hunshou_color != merge_color)
//				{
//					new_item_better = (hunshou_cfg->hunshou_color > merge_color);
//				}
//				else
//				{
//					if (LIEMING_HUNSHOU_COLOR_PURPLE < merge_color)
//					{
//						same_color_set.insert(i);
//						continue;
//					}
//				}
//
//				if (new_item_better)
//				{
//					if (merge_index >= 0 && merge_index < LIEMING_HUNSHOU_BAG_GRID_MAX_COUNT)
//					{
//						be_merge_set.insert(merge_index);
//						set_union(be_merge_set.begin(), be_merge_set.end(), same_color_set.begin(), same_color_set.end(), inserter(be_merge_set, be_merge_set.begin()));
//						same_color_set.clear();
//					}
//
//					merge_index = i;
//					merge_color = hunshou_cfg->hunshou_color;
//					//merge_level = item.level;
//				}
//				else
//				{
//					be_merge_set.insert(i);
//				}
//			}
//		}
//	}
//
//	if (merge_index < 0 || merge_index >= LIEMING_HUNSHOU_BAG_GRID_MAX_COUNT || !m_bag_param.grid_list[merge_index].IsValid() || be_merge_set.empty())
//	{
//		return; 
//	}
//
//	long long total_sell_exp = 0;
//	for (std::set<int>::iterator it = be_merge_set.begin(); it != be_merge_set.end(); ++ it)
//	{
//		LieMingHunShou &item = m_bag_param.grid_list[*it];
//		if (item.IsValid())
//		{
//			long long single_sell_exp = LOGIC_CONFIG->GetLieMingCfg().CalSellExp(item.hunshou_id, item.level, item.curr_exp);
//			if (single_sell_exp <= 0)
//			{
//				return;
//			}
//
//			total_sell_exp += single_sell_exp;
//			item.Reset();
//		}
//	}
//
//	long long tmp_total_sell_exp = total_sell_exp;
//	LieMingHunShou &merge_item = m_bag_param.grid_list[merge_index];
//	int merge_max_level = LOGIC_CONFIG->GetLieMingCfg().GetHunshouMaxLevel(merge_color);
//	int old_level = merge_item.level;
//	long long old_exp = merge_item.curr_exp;
//	int loop_times = 0;
//	
//	while (tmp_total_sell_exp > 0 && loop_times ++ < 1024)
//	{
//		if (merge_item.level >= merge_max_level)
//		{
//			break;
//		}
//
//		const LieMingHunshouLevelConfigItem *level_cfg = LOGIC_CONFIG->GetLieMingCfg().GetHunshouLevelCfg(merge_color, merge_item.level);
//		if (level_cfg == nullptr)
//		{
//			continue;
//		}
//
//		long long delta_exp = level_cfg->need_exp - merge_item.curr_exp;
//
//		if (tmp_total_sell_exp >= delta_exp)
//		{
//			tmp_total_sell_exp -= delta_exp;
//			merge_item.curr_exp = 0;
//			++ merge_item.level;
//		}
//		else
//		{
//			merge_item.curr_exp += tmp_total_sell_exp;
//			tmp_total_sell_exp = 0;
//		}
//	}
//
//	long long old_bag_hunshou_exp = m_bag_param.hunshou_exp;
//
//	m_bag_param.hunshou_exp += tmp_total_sell_exp;
//
//	if (merge_index > 0 && merge_index < LIEMING_HUNSHOU_BAG_GRID_MAX_COUNT)
//	{
//		LieMingHunShou &first_item = m_bag_param.grid_list[0];
//		LieMingHunShou &merge_item = m_bag_param.grid_list[merge_index];
//		first_item.hunshou_id = merge_item.hunshou_id;
//		first_item.curr_exp = merge_item.curr_exp;
//		first_item.level = merge_item.level;
//		merge_item.Reset();
//	}
//
//	if (!same_color_set.empty())
//	{
//		int i = 1;
//		for (std::set<int>::iterator it = same_color_set.begin(); it != same_color_set.end(); ++ it)
//		{
//			if (i >= LIEMING_HUNSHOU_BAG_GRID_MAX_COUNT)
//			{
//				break;
//			}
//
//			if (i == *it)
//			{
//				i ++;
//				continue;
//			}
//
//			LieMingHunShou &item = m_bag_param.grid_list[*it];
//			if (item.IsValid())
//			{
//				LieMingHunShou &dest_item = m_bag_param.grid_list[i];
//				dest_item = item;
//				item.Reset();
//				i ++;
//			}
//		}
//	}
//
//	this->SendBagInfo(Protocol::LIEMING_BAG_NOTIFY_REASON_BAG_MERGE);
//	
//	gamelog::g_log_lieming.printf(LL_INFO, "OnHunshouBagMerge user %d %s, merge_index %d, merge_hunshou_id %d, old_level %d, old_exp %lld, new_level %d, new_exp %lld, "
//		" bemerge_hunshou_count %d, all_sell_exp %lld, use_exp %lld, left_exp %lld, old_bag_hunshou_exp %lld, new_bag_hunshou_exp %lld",
//		m_role->GetUID(), m_role->GetName(), merge_index, merge_item.hunshou_id, old_level, old_exp, merge_item.level, merge_item.curr_exp,
//		(int)be_merge_set.size(), total_sell_exp, total_sell_exp - tmp_total_sell_exp, tmp_total_sell_exp, old_bag_hunshou_exp, m_bag_param.hunshou_exp);
//}
//
//void LieMingManager::OneHunshouExchange(short source_index, short dest_index)
//{
//	if (source_index < 0 || source_index >= LIEMING_LIEHUN_POOL_MAX_COUNT || dest_index < 0 || dest_index >= LIEMING_FUHUN_SLOT_COUNT)
//	{
//		return;
//	}
//
//	short source_id = m_bag_param.chouhun_pool[source_index];
//	if (source_id <= 0)
//	{
//		return;
//	}
//
//	const LieMingHunshouConfig *source_cfg = LOGIC_CONFIG->GetLieMingCfg().GetHunshouCfg(source_id);
//	if (NULL == source_cfg)
//	{
//		return;
//	}
//
//	const LieMingHunShou &dest_item = m_param.slot_list[dest_index];
//	if (m_param.slot_list[dest_index].IsValid())
//	{
//		const LieMingHunshouConfig *dest_cfg = LOGIC_CONFIG->GetLieMingCfg().GetHunshouCfg(dest_item.hunshou_id);
//		if (NULL == dest_cfg)
//		{
//			return;
//		}
//
//		if (source_cfg->hunshou_type != dest_cfg->hunshou_type || source_cfg->hunshou_color <= dest_cfg->hunshou_color)
//		{
//			return;
//		}
//
//		int grid_idx = this->GetBagFreeSlot();
//		if (grid_idx < 0 || grid_idx >= LIEMING_HUNSHOU_BAG_GRID_MAX_COUNT)
//		{
//			m_role->NoticeNum(errornum::EN_LIEMING_BAG_ALREAD_FULL);
//			return;
//		}
//
//		m_bag_param.grid_list[grid_idx] = m_param.slot_list[dest_index];
//	}
//
//	m_param.slot_list[dest_index].Reset();
//	for (int i = 0; i < LIEMING_FUHUN_SLOT_COUNT; ++ i)
//	{
//		int hunshou_id = m_param.slot_list[i].hunshou_id;
//		const LieMingHunshouConfig *slot_hunshou_cfg = LOGIC_CONFIG->GetLieMingCfg().GetHunshouCfg(hunshou_id);
//
//		if (NULL != slot_hunshou_cfg)
//		{
//			if (source_cfg->hunshou_type == slot_hunshou_cfg->hunshou_type)
//			{
//				return;
//			}
//		}
//	}
//
//	m_param.slot_list[dest_index].hunshou_id = source_cfg->hunshou_id;
//	m_param.slot_list[dest_index].level = 1;
//	m_param.slot_list[dest_index].curr_exp = 0;
//	m_bag_param.chouhun_pool[source_index] = 0;
//	-- m_liehun_pool_hunshou_count;
//}
//
//void LieMingManager::OnBatchHunshouExchange(Protocol::CSLieMingExchangeList *lmel)
//{
//	if (0 >= lmel->exchange_count || LIEMING_FUHUN_SLOT_COUNT < lmel->exchange_count) return;
//	for (int i = 0; i < lmel->exchange_count; ++i)
//	{
//		short source_index = lmel->exchange_list[i].source_index;
//		short dest_index = lmel->exchange_list[i].dest_index;
//		this->OneHunshouExchange(source_index, dest_index);
//	}
//
//	short temp_pool[LIEMING_LIEHUN_POOL_MAX_COUNT];
//	memset(temp_pool, 0, sizeof(temp_pool));
//	for (int i = 0, j = 0; i < m_liehun_pool_hunshou_count && i < LIEMING_LIEHUN_POOL_MAX_COUNT && j < LIEMING_LIEHUN_POOL_MAX_COUNT; ++j )
//	{
//		if (m_bag_param.chouhun_pool[j] != 0)
//		{
//			temp_pool[i++] = m_bag_param.chouhun_pool[j];
//		}
//	}
//
//	for (int i = 0; i < LIEMING_LIEHUN_POOL_MAX_COUNT; ++ i)
//	{
//		m_bag_param.chouhun_pool[i] = temp_pool[i];
//	}
//
//	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LIEMING);
//
//	this->SendEquipInfo();
//	this->SendBagInfo();
//}
//
//int LieMingManager::GetBagFreeSlot()
//{
//	for (int i = 0; i < LIEMING_HUNSHOU_BAG_GRID_MAX_COUNT; ++ i)
//	{
//		if (!m_bag_param.grid_list[i].IsValid())
//		{
//			return i;
//		}
//	}
//
//	return -1;
//}
//
//void LieMingManager::SendEquipInfo(int notify_reason)
//{
// 	static Protocol::SCLieMingSlotInfo wsei;
//	wsei.slot_activity_flag = m_param.solt_activity_flag;
// 	wsei.notify_reason = notify_reason;
// 
// 	for (int i = 0; i < LIEMING_FUHUN_SLOT_COUNT; ++ i)
// 	{
// 		Protocol::LieMingHunShou &protocal_equip = wsei.slot_list[i];
// 		
//		protocal_equip.hunshou_id = m_param.slot_list[i].hunshou_id;
//		protocal_equip.level = m_param.slot_list[i].level;
//		protocal_equip.curr_exp = m_param.slot_list[i].curr_exp;
// 	}
// 
// 	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&wsei, sizeof(wsei));
//}
//
//void LieMingManager::SendBagInfo(int notify_reason)
//{
//	static Protocol::SCLieMingBagInfo wsbi;
//	wsbi.notify_reason = notify_reason;
//	wsbi.liehun_color = m_bag_param.chouhun_color;
//	wsbi.hunshou_exp = m_bag_param.hunshou_exp;
//	wsbi.hunli = m_role->GetHunli();
//
//	memcpy(wsbi.liehun_pool, m_bag_param.chouhun_pool, sizeof(wsbi.liehun_pool));
//
//	for (int j = 0; j < LIEMING_HUNSHOU_BAG_GRID_MAX_COUNT; ++ j)
//	{
//		wsbi.grid_list[j].hunshou_id = m_bag_param.grid_list[j].hunshou_id;
//		wsbi.grid_list[j].level = m_bag_param.grid_list[j].level;
//		wsbi.grid_list[j].curr_exp = m_bag_param.grid_list[j].curr_exp;
//	}
//
//	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&wsbi, sizeof(wsbi));
//}
//
void LieMingManager::ActiveMinggeSolt(int slot_index)
{
	if (slot_index < 0 || slot_index > LIEMING_FUHUN_SLOT_COUNT) return;

	if (!m_param.IsSoltActive(slot_index))
	{
		m_param.ActiveSolt(slot_index);
	}
}
//
//void LieMingManager::GmAddHunshouExp(int exp)
//{
//	m_bag_param.hunshou_exp += exp;
//
//	this->SendBagInfo(Protocol::LIEMING_BAG_NOTIFY_REASON_INVALID);
//}

//void LieMingManager::SendEquipInfo(int notify_reason)
//{
//	static Protocol::SCLieMingSlotInfo wsei;
//	wsei.notify_reason = notify_reason;
//	wsei.slot_activity_flag = m_param.solt_activity_flag;
//
//	int count = 0;
//	for (int i = 0; i < LIEMING_FUHUN_SLOT_COUNT; ++i)
//	{
//		if (count < 0 || count >= LIEMING_FUHUN_SLOT_COUNT) continue;
//
//		wsei.minghun_list[count].index = i;
//		wsei.minghun_list[count].item_wrapper.Reset();
//	}
//
//	wsei.count = count;
//
//	wsei.total_exp = 0;
//
//	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&wsei, sizeof(wsei));
//}


//new
void LieMingManager::Init(Role *role, const LieMingParam2 &lieming_param2)
{
	m_role = role;
	m_param = lieming_param2;

	for (int i = 0; i < LIEMING_FUHUN_SLOT_COUNT; ++i)
	{
		ItemGridData *grid_data = &m_minghun_list[i];
		grid_data->item_id = m_param.slot_list[i].item_id;
		grid_data->is_bind = m_param.slot_list[i].is_bind;
		EquipmentParam *equip_param = new EquipmentParam();		//此处内存
		equip_param->strengthen_level = m_param.slot_list[i].level;
		equip_param->param1 = m_param.slot_list[i].hunli;
		grid_data->param = equip_param;
	}
}

void LieMingManager::GetInitParam(LieMingParam2 *lieming_param2)
{
	*lieming_param2 = m_param;
}

void LieMingManager::GeiInitLieMingParam(LieMingParam2 *lieming_param2)
{
	*lieming_param2 = m_param;
}

void LieMingManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		// 附魂
		for (int hunshou_slot_idx = 0; hunshou_slot_idx < LIEMING_FUHUN_SLOT_COUNT; ++hunshou_slot_idx)
		{
			if (!m_minghun_list[hunshou_slot_idx].Invalid())
			{
				ItemGridData *grid_data = &m_minghun_list[hunshou_slot_idx];
				if (grid_data == NULL || grid_data->Invalid())
				{
					continue;
				}

				EquipmentParam *equip_param = (EquipmentParam *)grid_data->param;
				if (equip_param == NULL)
				{
					continue;
				}

				int attr_type = 0;
				Attribute attr_val = 0;
				if (LOGIC_CONFIG->GetLieMingCfg().GetHunshouAddAttr(m_minghun_list[hunshou_slot_idx].item_id,
					equip_param->strengthen_level, &attr_type, &attr_val))
				{
//					m_attrs_add.m_attrs[attr_type] += GETPROFVALUE(m_role->GetProf(), attr_type, attr_val);
					m_attrs_add.AddValue(attr_type, GETPROFVALUE(m_role->GetProf(), attr_type, attr_val));
				}
			}
		}
	}

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_LIEMING, m_attrs_add);

	//策划需求，猎命的战力算入精灵战力中
	m_role->GetCapability()->SetCap(CAPABILITY_TYPE_JINGLING, m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_JINGLING) + m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_LIEMING));

	base_add.Add(m_attrs_add.m_attrs, true);
}

void LieMingManager::SendEquipInfo(int notify_reason)
{
	static Protocol::SCLieMingSlotInfo wsei;
	wsei.notify_reason = notify_reason;
	wsei.slot_activity_flag = m_param.solt_activity_flag;

	for (int i = 0; i < LIEMING_FUHUN_SLOT_COUNT; ++i)
	{
		wsei.minghun_list[i].index = i;
		wsei.minghun_list[i].item_wrapper.Reset();
		m_minghun_list[i].WrapTo(&(wsei.minghun_list[i].item_wrapper));
	}

	wsei.count = LIEMING_FUHUN_SLOT_COUNT;
	wsei.total_exp = m_param.personal_minghun_exp;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&wsei, sizeof(wsei));
}

//策划需求，命魂改成物品
int LieMingManager::PutOn(const ItemDataWrapper &item_wrapper)
{
	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_wrapper.item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType()) return -1;

	if (0 == item_wrapper.has_param) return -2;

	if (Equipment::E_TYPE_SPECIAL_MINGHUN != equip->GetSubType()) return -3;

	short equip_index = -1 ;

	const LieMingHunshouConfig *equip_hunshou_cfg = LOGIC_CONFIG->GetLieMingCfg().GetHunshouCfg(item_wrapper.item_id);
	if (equip_hunshou_cfg == NULL)
	{
		return -4;
	}

	for (int i = 0; i < LIEMING_FUHUN_SLOT_COUNT; ++i)
	{
		int item_id = m_param.slot_list[i].item_id;

		if (item_id <= 0)continue;

		const LieMingHunshouConfig *slot_hunshou_cfg = LOGIC_CONFIG->GetLieMingCfg().GetHunshouCfg(item_id);

		if (NULL != slot_hunshou_cfg)
		{
			if (equip_hunshou_cfg->hunshou_type == slot_hunshou_cfg->hunshou_type)
			{
				if (equip_hunshou_cfg->hunshou_color <= slot_hunshou_cfg->hunshou_color)
				{
					m_role->NoticeNum(errornum::EN_LIEMING_EQUIP_FULING_SAME_HUNSHOU_LIMIT);
					return -5;
				}

				if(this->TakeOff(i, true) < 0)
				{
					return -100;
				}
				else
				{
					equip_index = i;
				}
			}
		}
	}

	if (equip_index < 0)
	{
		equip_index = this->GetFirstEmptyGridIndex();
	}

	if (equip_index < 0 || equip_index >= LIEMING_FUHUN_SLOT_COUNT)
	{
		m_role->NoticeNum(errornum::EN_LIEMING_SOLT_NO_SPACE);
		return -6;
	}

	if (!m_param.IsSoltActive(equip_index + 1))
	{
		m_role->NoticeNum(errornum::EN_LIEMING_SOLT_NOT_ACTIVTY);
		return -7;
	}

	EquipmentParam *equip_param = equip->CreateParamFromStruct(&item_wrapper.param_data);
	if (NULL == equip_param)
	{
		return -8;
	}

	if (equip_param->strengthen_level <= 0)
	{
		equip_param->strengthen_level = 1; //用强化等级认为是命魂等级
	}

	if (equip_param->param1 <= 0)		   //命魂经验
	{
		equip_param->param1 = 0;
	}

	m_minghun_list[equip_index].item_id = item_wrapper.item_id;
	m_minghun_list[equip_index].param = equip_param;
	m_minghun_list[equip_index].num = 1;
	m_minghun_list[equip_index].invalid_time = item_wrapper.invalid_time;
	m_minghun_list[equip_index].is_bind = (item_wrapper.is_bind == 1);

	{
		m_param.slot_list[equip_index].item_id = m_minghun_list[equip_index].item_id;
		m_param.slot_list[equip_index].is_bind = m_minghun_list[equip_index].is_bind;
		m_param.slot_list[equip_index].level = equip_param->strengthen_level;
		m_param.slot_list[equip_index].hunli = equip_param->param1;
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LIEMING);

	m_role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_LIEMING_COUNT); // 嘉年华

	this->SendEquipInfo();

	return 0;
}

int LieMingManager::TakeOff(short equip_index, bool is_puton)
{
	if (equip_index < 0 || equip_index >= LIEMING_FUHUN_SLOT_COUNT) return -1;

	if (m_minghun_list[equip_index].Invalid()) return -2;

	EquipmentParam *equip_param = (EquipmentParam *)m_minghun_list[equip_index].param;
	if (NULL == equip_param) return -3;

	const Equipment *equip = (const Equipment*)ITEMPOOL->GetItem(m_minghun_list[equip_index].item_id);
	if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT || equip->GetSubType() != Equipment::E_TYPE_SPECIAL_MINGHUN)
	{
		return -4;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return -5;
	}

	ItemDataWrapper item_wrapper(m_minghun_list[equip_index].item_id, 1, (short)m_minghun_list[equip_index].is_bind,
		m_minghun_list[equip_index].param, m_minghun_list[equip_index].invalid_time, 0);

	int put_index = -1;
	if (!knapsack->Put(item_wrapper, PUT_REASON_NO_NOTICE, &put_index))
	{
		return -6;
	}

	m_minghun_list[equip_index].Clear();		// 指针已无用 删除之

	m_param.slot_list[equip_index].Reset();		// 重置装备槽（存库）

	if (!is_puton)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LIEMING);

		m_role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_LIEMING_COUNT); // 嘉年华

		this->SendEquipInfo();
	}

	return 0;
}

void LieMingManager::UpLevel(short index, bool is_auto)
{
	if (index < 0 || index >= LIEMING_FUHUN_SLOT_COUNT)
	{
		return;
	}

	if (m_param.personal_minghun_exp <= 0)
	{
		m_role->NoticeNum(errornum::EN_LIEMING_FUHUN_EXP_LIMIT);
		return;
	}

	ItemGridData *grid_data = &m_minghun_list[index];
	if (grid_data->Invalid())
	{
		return;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(grid_data->item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType() || equip->GetSubType() != Equipment::E_TYPE_SPECIAL_MINGHUN)
	{
		return;
	}

	EquipmentParam *equip_param = (EquipmentParam *)grid_data->param;
	if (equip_param == NULL)
	{
		return;
	}
	if (equip_param->strengthen_level >= LIEMING_HUNSHOU_MAX_LEVEL)
	{
		if (!is_auto)
		{
			m_role->NoticeNum(errornum::EN_LIEMING_FUHUN_ADD_EXP_ALREADY_REACHED_LEVEL);
		}
		return;
	}

	const LieMingHunshouLevelConfigItem *level_cfg = LOGIC_CONFIG->GetLieMingCfg().GetHunshouLevelCfg(equip->GetColor(), equip_param->strengthen_level);
	if (level_cfg == nullptr)
	{
		return;
	}

	const LieMingHunshouLevelConfigItem *next_level_cfg = LOGIC_CONFIG->GetLieMingCfg().GetHunshouLevelCfg(equip->GetColor(), equip_param->strengthen_level + 1);
	if (next_level_cfg == nullptr)
	{
		if (!is_auto)
		{
			m_role->NoticeNum(errornum::EN_LIEMING_FUHUN_ADD_EXP_ALREADY_REACHED_LEVEL);
		}
		return;
	}

	const short old_strength_level = equip_param->strengthen_level;
	int old_exp = m_param.personal_minghun_exp;			//玩家总魂力

	int cost_exp = 0;
	if (m_param.personal_minghun_exp + equip_param->param1 < level_cfg->need_exp)
	{
		cost_exp = m_param.personal_minghun_exp;
		equip_param->param1 += m_param.personal_minghun_exp;
	}
	else
	{
		cost_exp = level_cfg->need_exp - equip_param->param1;

		int cur_strength_level = equip_param->strengthen_level;

		const LieMingHunshouLevelConfigItem *level_cfg_cmp = LOGIC_CONFIG->GetLieMingCfg().GetHunshouLevelCfg(equip->GetColor(), cur_strength_level + 1);
		if (level_cfg_cmp != NULL)
		{
			equip_param->param1 = 0;
			++equip_param->strengthen_level;

			if (!is_auto)
			{
				m_role->SendOperateResult(Protocol::SCOperateResult::OP_JINGLING_LINGHUN_UPLEVEL, 1, index, 1);
			}
			else
			{
				m_role->SendOperateResult(Protocol::SCOperateResult::OP_JINGLING_LINGHUN_UPLEVEL, 1, index, 0);
			}
			
		}
		else if (equip_param->param1 < level_cfg->need_exp)		//	已达到最大等级，仅提升经验
		{
			equip_param->param1 += cost_exp;
		}
	}

	this->AddExp(-cost_exp);
	grid_data->is_bind = true;

	{
		m_param.slot_list[index].item_id = m_minghun_list[index].item_id;
		m_param.slot_list[index].is_bind = m_minghun_list[index].is_bind;
		m_param.slot_list[index].level = equip_param->strengthen_level;
		m_param.slot_list[index].hunli = equip_param->param1;
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LIEMING);

	this->SendEquipInfo();

	gamelog::g_log_lieming.printf(LL_INFO, "hunshouuplevel user %d %s, slot_idx %d, hunshou_id %d, old_level %d, new_level %d, old_exp %d, cur_exp %d",
		m_role->GetUID(), m_role->GetName(), index, grid_data->item_id, old_strength_level, equip_param->strengthen_level, old_exp, m_param.personal_minghun_exp);
}

void LieMingManager::AutoUpLevel()
{
	for (int index = 0; index < LIEMING_FUHUN_SLOT_COUNT; index ++)
	{
		if (m_param.personal_minghun_exp <= 0)
		{
			m_role->SendOperateResult(Protocol::SCOperateResult::OP_JINGLING_LINGHUN_UPLEVEL, 0, LIEMING_FUHUN_SLOT_COUNT, 1);		//通知客户端播特效
			m_role->NoticeNum(errornum::EN_LIEMING_FUHUN_EXP_LIMIT);
			break;
		}

		this->UpLevel(index, true);

		if (index == LIEMING_FUHUN_SLOT_COUNT - 1)
		{
			m_role->SendOperateResult(Protocol::SCOperateResult::OP_JINGLING_LINGHUN_UPLEVEL, 0, LIEMING_FUHUN_SLOT_COUNT, 1);		//通知客户端播特效
		}
	}
}

short LieMingManager::GetFirstEmptyGridIndex()
{
	for (int i = 0; i < LIEMING_FUHUN_SLOT_COUNT; ++i)
	{
		if (m_minghun_list[i].Invalid())
		{
			return i;
		}
	}

	return -1;
}

int LieMingManager::GetSatisfyMagicalPreciousCount(int order)
{
	int suit_count = 0;
	for (const auto &slot : m_param.slot_list)
	{
		const LieMingHunshouConfig *hunshou_cfg = LOGIC_CONFIG->GetLieMingCfg().GetHunshouCfg(slot.item_id);
		if (nullptr != hunshou_cfg && hunshou_cfg->hunshou_color >= order)
		{
			++suit_count;
		}
	}

	return suit_count;
}

void LieMingManager::AddExp(int exp)
{
	m_param.personal_minghun_exp += exp;
	this->SendEquipInfo();
}

void LieMingManager::GmAddHunshouExp(int exp)
{
	this->AddExp(exp);
}

void LieMingManager::AddDecomposeExp(int color, int level)
{
	//for (int i = 0; i < level; i++)
	//{
	const LieMingHunshouLevelConfigItem *level_cfg = LOGIC_CONFIG->GetLieMingCfg().GetHunshouLevelCfg(color, level);
	if (level_cfg == NULL)
	{
		return;
	}

	this->AddExp(level_cfg->total_exp);
	//}
}

int LieMingManager::GetDecomposeExp(int color, int level)
{
	const LieMingHunshouLevelConfigItem *level_cfg = LOGIC_CONFIG->GetLieMingCfg().GetHunshouLevelCfg(color, level);
	if (level_cfg == NULL)
	{
		return 0;
	}

	return level_cfg->total_exp;
}
