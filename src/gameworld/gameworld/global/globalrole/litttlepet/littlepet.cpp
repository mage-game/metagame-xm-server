#include "littlepet.hpp"
#include "config/logicconfigmanager.hpp"
#include "global/globalrole/litttlepet/littlepetconfig.hpp"
#include "servercommon/errornum.h"
#include "obj/character/role.h"
#include "item/itempool.h"
#include "item/itembase.h"
#include "scene/scene.h"
#include "global/globalrole/globalrolemanager.hpp"
#include "global/usercache/usercache.hpp"
#include "world.h"
#include "gstr.h"
#include "gamelog.h"
#include "servercommon/noticenum.h"
#include "item/knapsack.h"
#include "friend/friendmanager.h"
#include "servercommon/struct/friendparam.h"
#include "other/capability/capability.hpp"
#include "gameworld/item/itemextern.hpp"
#include "gameworld/equipment/equipment.h"
#include "gameworld/other/chestshop/chestshop.hpp"
#include "gameworld/other/chestshop/rolechestshop.hpp"
#include "servercommon/string/gameworldstr.h"
#include "other/lotterycount/lotterycount.hpp"

LittlePet::LittlePet(GlobalRole *g_role) : IGlobalRoleItem(g_role), m_next_check_feed_degree_time(0)
{
}

LittlePet::~LittlePet()
{
}

void LittlePet::Init(const LittlePetData &little_pet_attr)
{
	m_little_pet_param = little_pet_attr;

	this->RecalNextFeedDegreeTime();
}

void  LittlePet::GetAttr(LittlePetData &little_pet_attr)
{
	little_pet_attr = m_little_pet_param;
}

void LittlePet::Update(unsigned int now_second)
{
	//Role *role = this->GetRole();
	//if (NULL == role) return;

	//if (m_next_check_feed_degree_time > 0 && now_second >= m_next_check_feed_degree_time)
	//{
	//	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LITTLE_PET);
	//	this->RecalNextFeedDegreeTime();
	//}
}

void LittlePet::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid)	return;

	for (int index = 0; index < LITTLE_PET_PERSON_MAX_NUM; ++index)
	{
		LittlePetParam &pet_info = m_little_pet_param.pet_list[index];
		if (pet_info.pet_id <= 0) continue;

		pet_info.interact_times = 0;
	}

	m_little_pet_param.interact_times = 0;

	this->SendAllInfo();
}

void LittlePet::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc, Role *role)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		this->GetTotalAttr(m_attrs_add);

		LittlePet * lover_pet_info = GLOBAL_ROLE_MGR.GetLittlePet(this->GetLoverId());
		if (NULL != lover_pet_info)
		{
			lover_pet_info->GetTotalAttr(m_attrs_add);
		}

		role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_LITTLE_PET, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

void LittlePet::OnRoleLogin(Role *role)
{
	this->SetLoverId(role->GetLoverUid(), false);
}

bool LittlePet::ActiveLittlePet(ItemID item_id)
{
	//Role *role = this->GetRole();
	//if (NULL == role) return false;

	//int index = 0;
	//for (; index < LITTLE_PET_PERSON_MAX_NUM; ++index)
	//{
	//	if (m_little_pet_param.pet_list[index].pet_id <= 0)	break;
	//}

	//if (index >= LITTLE_PET_PERSON_MAX_NUM)
	//{
	//	role->NoticeNum(errornum::EN_LITTLE_PET_MAX_NUM);
	//	return false;
	//}

	//const LittlePetBaseCfg *base_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetBaseCfgByItemID(item_id);
	//if (NULL == base_cfg) return false;

	//int pet_id = base_cfg->little_pet_id;
	////const LittlePetQualityCfg *quality_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetLittlePetQualityCfg(base_cfg->quality_type);
	////if (NULL == quality_cfg) return false;

	//LittlePetParam &tem_attrs = m_little_pet_param.pet_list[index];
	//tem_attrs.Reset();
	//tem_attrs.pet_id = pet_id;
	//F_STRNCPY(tem_attrs.pet_name, base_cfg->pet_name, sizeof(GameName));

	//this->RandAttr(tem_attrs, base_cfg);

	//this->OnDataChange();

	//this->SendSingleInfo(role, index, 1);
	//role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LITTLE_PET);

	//Role *lover_role = this->GetLoverRole();
	//if (NULL != lover_role)
	//{
	//	this->SendSingleInfo(lover_role, index, 0);
	//	lover_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LITTLE_PET);
	//}

	//// 日志
	//gamelog::g_log_little_pet.printf(LL_INFO, "LittlePet::ActiveLittlePet user[%d %s],index[%d], use_item_id[%d]",
	//	role->GetUID(), role->GetName(), index, (int)item_id);

	//return true;

	return false;
}

void LittlePet::Remove(int index)
{
	//if (index < 0 || index >= LITTLE_PET_PERSON_MAX_NUM) return;

	//Role *role = this->GetRole();
	//if (NULL == role) return;

	//int pet_id = m_little_pet_param.pet_list[index].pet_id;
	//if (pet_id <= 0) return;

	//const LittlePetBaseCfg * base_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetBaseCfgById(pet_id);
	//if (NULL == base_cfg) return;

	//this->RecyclePetItem(base_cfg->active_item_id, 1, true, true);
	//this->RecycleStuffItem(base_cfg->active_item_id, index, true, true);

	//m_little_pet_param.pet_list[index].Reset();

	//if (index == m_little_pet_param.using_index)
	//{
	//	this->BroadcastTaskStatus(role);
	//}

	//this->SendSingleInfo(role, index, 1);
	//role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LITTLE_PET);

	//Role *lover_role = this->GetLoverRole();
	//if (NULL != lover_role)
	//{
	//	this->SendSingleInfo(lover_role, index, 0);
	//	lover_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LITTLE_PET);
	//}

	//this->OnDataChange();
	//this->SendAllInfo();

	//gamelog::g_log_little_pet.printf(LL_INFO, "LittlePet::OnReLive user[%d %s], ReLive_pet_id[%d], ReLive_pet_index[%d]",
	//	role->GetUID(), role->GetName(), pet_id, index);
}

void LittlePet::RecyclePetItem(int item_id, int num, bool is_bind, bool is_relive)
{
	//Role *role = this->GetRole();
	//if (NULL == role) return;

	//if (item_id <= 0) return;

	//const LittlePetBaseCfg *base_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetBaseCfgByItemID((ItemID)item_id);
	//if (NULL == base_cfg) return;

	//ItemConfigData item = base_cfg->return_item;
	//item.num = base_cfg->return_item.num * num;
	//item.is_bind = is_bind;

	//// 检查背包空间是否足够
	//const ItemBase *itembase = ITEMPOOL->GetItem(item.item_id);
	//if (NULL == itembase) return;
	//int stack_num = itembase->GetPileLimit();
	//if (stack_num <= 0) return;
	//int need_grid = (stack_num + item.num) / stack_num;

	//if (!role->GetKnapsack()->CheckEmptyGridNoLessThan(need_grid))
	//{
	//	role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
	//	return;
	//}

	////consume，回收时用
	//if (!is_relive)
	//{
	//	if (!role->GetKnapsack()->ConsumeItem(item_id, num, "littlepetrecycle")) return;
	//}

	//role->GetKnapsack()->Put(item, PUT_REASON_LITTLE_PET_EXCHANGE_ITEM);

	//// 日志
	//gamelog::g_log_little_pet.printf(LL_INFO, "LittlePet::RecyclePetItem user[%d %s], recycle_item_id[%d], recycle_item_num[%d], add_score[%d]", 
	//	role->GetUID(), role->GetName(), item_id, num, base_cfg->recycly_score);
}

void LittlePet::RecycleStuffItem(int item_id, int index, bool is_bind, bool is_relive)
{
	//Role *role = this->GetRole();
	//if (NULL == role) return;

	//if (item_id <= 0) return;
	//if (index < 0) return;

	//const LittlePetBaseCfg *base_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetBaseCfgByItemID((ItemID)item_id);
	//if (NULL == base_cfg) return;

	//const LittlePetQualityCfg *quality_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetLittlePetQualityCfg(base_cfg->quality_type);
	//if (NULL == quality_cfg) return;

	//const LittlePetOtherCfg &other_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetLittlePetOtherCfg();

	//// 强化消耗品物品id
	//int attri_num = 0;
	//for (int point_type = 0; point_type < LITTLEPET_QIANGHUAPOINT_CURRENT_NUM; ++point_type)
	//{
	//	for (int grid_index = 0; grid_index < LITTLEPET_QIANGHUAGRID_MAX_NUM; ++grid_index)
	//	{
	//		const LittleStarItem::AttributeItem &grid_cfg = m_little_pet_param.pet_list[index].star_list[point_type].attr_list[grid_index];
	//		if (-1 == grid_cfg.arrt_type) continue;

	//		attri_num ++;
	//	}
	//}

	//ItemConfigData suff_item;
	//suff_item.item_id = quality_cfg->qianghua_need_item_id;
	//suff_item.num = attri_num * other_cfg.fangsheng_grid_add_num;
	//suff_item.is_bind = is_bind;

	//// 需要的背包格子数
	//const ItemBase *suff_itembase = ITEMPOOL->GetItem(suff_item.item_id);
	//if (NULL == suff_itembase) return;
	//int suff_stack_num = suff_itembase->GetPileLimit();
	//if (suff_stack_num <= 0)  return;
	//int need_grid = (suff_stack_num + suff_item.num) / suff_stack_num;

	//// 检查背包格子数
	//if (!role->GetKnapsack()->CheckEmptyGridNoLessThan(need_grid))
	//{
	//	role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
	//	return;
	//}

	//role->GetKnapsack()->Put(suff_item, PUT_REASON_LITTLE_PET_EXCHANGE_ITEM);

	//// 日志
	//gamelog::g_log_little_pet.printf(LL_INFO, "LittlePet::RecycleSuffItem user[%d %s], recycle_item_id[%d], recycle_item_num[%d], return_item_id[%d], return_item_num[%d]",
	//	role->GetUID(), role->GetName(), item_id, suff_item.num, suff_item.item_id, suff_item.num);
}

void LittlePet::Replace(int index, ItemID item_id)
{
	//Role *role = this->GetRole();
	//if (NULL == role) return;

	//if (index < 0 || index >= LITTLE_PET_PERSON_MAX_NUM) return;

	//if (m_little_pet_param.pet_list[index].pet_id <= 0)	return;

	//const LittlePetBaseCfg *base_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetBaseCfgByItemID(item_id);
	//if (NULL == base_cfg) return;

	////const LittlePetQualityCfg *quality_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetLittlePetQualityCfg(base_cfg->quality_type);
	////if (NULL == quality_cfg) return;

	////检空
	//if (!role->GetKnapsack()->HaveSpace()) return;

	////消耗
	//if (!role->GetKnapsack()->ConsumeItem(item_id, 1, "littepetactive")) return;

	////返回材料
	//ItemConfigData item = base_cfg->return_item;
	//role->GetKnapsack()->Put(item, PUT_REASON_LITTLE_PET_RECYCLE);

	////替换
	//LittlePetParam &tem_attrs = m_little_pet_param.pet_list[index];
	//tem_attrs.pet_id = base_cfg->little_pet_id;
	//F_STRNCPY(tem_attrs.pet_name, base_cfg->pet_name, sizeof(GameName));

	//this->RandAttr(tem_attrs, base_cfg);

	//role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LITTLE_PET);

	//this->OnDataChange();
	//this->SendSingleInfo(role, index);

	//Role *lover_role = this->GetLoverRole();
	//if (NULL != lover_role)
	//{
	//	this->SendSingleInfo(lover_role, index, 0);
	//	lover_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LITTLE_PET);
	//}

	//// 日志
	//gamelog::g_log_little_pet.printf(LL_INFO, "LittlePet::ReplaceLittlePet user[%d %s],pet_index[%d], use_item_id[%d], return_item_id[%d], return_tiem_num[%d]",
	//	role->GetUID(), role->GetName(), index, (int)item_id, (int)item.item_id, item.num);
}

void LittlePet::ChangeImg(int index)
{
	Role *role = this->GetRole();
	if (NULL == role)	return;

	if (index < 0 || index >= LITTLE_PET_PERSON_MAX_NUM) return;

	if (m_little_pet_param.pet_list[index].pet_id <= 0)	return;

	m_little_pet_param.using_index = index;

	this->OnDataChange();
	this->BroadcastTaskStatus(role);
}

void LittlePet::ReName(int index, GameName new_name)
{
	if (index < 0 || index >= LITTLE_PET_PERSON_MAX_NUM) return;

	if (m_little_pet_param.pet_list[index].pet_id <= 0)	return;

	Role *role = this->GetRole();
	if (NULL == role) return;
	
	const LittlePetOtherCfg &other_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetLittlePetOtherCfg();

	if (!role->GetKnapsack()->GetMoney()->UseGold(other_cfg.rename_consume, "littlepet_rename"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	GameName tmp_new_name; memset(tmp_new_name, 0, sizeof(tmp_new_name));
	F_STRNCPY(tmp_new_name, new_name, sizeof(tmp_new_name));

	InvalidCharFilte(tmp_new_name);

	F_STRNCPY(m_little_pet_param.pet_list[index].pet_name, tmp_new_name, sizeof(GameName));

	this->OnDataChange();

	gstr::SendNotice(role, "little_pet_rename_succ");

	this->SendPetNewName(index);

	if (index == m_little_pet_param.using_index)
	{
		this->BroadcastTaskStatus(role);
	}
}

void  LittlePet::Strengthen(Role *role, int index, int point_type, int grid_index)
{
	//if (index < 0 || index >= LITTLE_PET_PERSON_MAX_NUM) return;

	//if (point_type < 0 || point_type >= LITTLEPET_QIANGHUAPOINT_CURRENT_NUM) return;

	//if (grid_index < 0 || grid_index >= LITTLEPET_QIANGHUAGRID_MAX_NUM)	return;

	//if (m_little_pet_param.pet_list[index].pet_id <= 0) return;

	//LittleStarItem::AttributeItem &pet_info = m_little_pet_param.pet_list[index].star_list[point_type].attr_list[grid_index];

	//const LittlePetBaseCfg *base_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetBaseCfgById(m_little_pet_param.pet_list[index].pet_id);
	//if (NULL == base_cfg) return;

	//const LittlePetQualityCfg * quanlity_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetLittlePetQualityCfg(base_cfg->quality_type);
	//if (NULL == quanlity_cfg) return;

	//const LittlePetStrengthenCfg *strengthen_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetLittlePetStrengthenCfg(base_cfg->quality_type, point_type);
	//if (NULL == strengthen_cfg)	return;

	//if (!role->GetKnapsack()->ConsumeItem(quanlity_cfg->qianghua_need_item_id, quanlity_cfg->qianghua_need_item_num, "littlepetintensify"))
	//{
	//	role->NoticeNum(errornum::EN_LITTLE_PET_NO_ENOUGH_QIANGHUA_DAN);
	//	return;
	//}
	//int rand_num = RandomNum(strengthen_cfg->total_weight);
	//int value_type = 0;
	//for (; value_type < LITTLEPET_VALUE_TYPE_MAX_NUM; ++value_type)
	//{
	//	if (rand_num < strengthen_cfg->strengthen_value_cfg[value_type].weight)	break;
	//	rand_num -= strengthen_cfg->strengthen_value_cfg[value_type].weight;
	//}

	//const LittlePetStrengthenValueCfg &strengthen_value = strengthen_cfg->strengthen_value_cfg[value_type];
	//int qianghua_count = pet_info.qianghua_times;
	//if (qianghua_count < strengthen_cfg->baodi_count)
	//{
	//	pet_info.attr_value = RandomNum(strengthen_value.value_min, strengthen_value.value_criticality);
	//	++pet_info.qianghua_times;
	//}
	//else
	//{
	//	pet_info.attr_value = RandomNum(strengthen_value.value_criticality, strengthen_value.value_max);
	//}

	//pet_info.arrt_type = strengthen_value.value_type;

	//this->OnDataChange();
	//this->SendChangeInfo(index, point_type, grid_index);

	//Role *self_role = this->GetRole();
	//if (NULL != self_role)
	//{
	//	self_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LITTLE_PET);
	//}

	//Role *lover_role = this->GetLoverRole();
	//if (NULL != lover_role)
	//{
	//	lover_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LITTLE_PET);
	//}

	//// 日志
	//gamelog::g_log_little_pet.printf(LL_INFO, "LittlePet::OnIntensify user[%d %s],indes[%d], point_type[%d], grid_index[%d], final_attr_value[%d]",
	//	role->GetUID(), role->GetName(), index, point_type, grid_index, pet_info.attr_value);
}

void LittlePet::FeedLittlePet(Role *target_role, int index)
{
	//if (index < 0 || index >= LITTLE_PET_PERSON_MAX_NUM) return;

	//Role *other_role = NULL;
	//if (target_role->GetUID() == this->GetLoverId())
	//{
	//	other_role = this->GetRole();
	//}
	//else
	//{
	//	other_role = this->GetLoverRole();
	//}

	//LittlePetParam &pet_info = m_little_pet_param.pet_list[index];

	//const LittlePetOtherCfg &other_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetLittlePetOtherCfg();

	//const LittlePetBaseCfg *base_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetBaseCfgById(pet_info.pet_id);
	//if (NULL == base_cfg) return;

	//const LittlePetQualityCfg *quality_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetLittlePetQualityCfg(base_cfg->quality_type);
	//if (NULL == quality_cfg) return;

	//if (pet_info.feed_degree >= quality_cfg->max_feed_degree)
	//{
	//	target_role->NoticeNum(errornum::EN_LITTLE_PET_MAX_FEED_DEGREE);
	//	return;
	//}

	//// check gold enough and then consume
	//if (!target_role->GetKnapsack()->GetMoney()->UseGold(other_cfg.feed_need_gold, "feed_little_pet"))
	//{
	//	target_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
	//	return;
	//}

	//pet_info.feed_degree += other_cfg.feed_value;
	//if (pet_info.feed_degree >= quality_cfg->max_feed_degree)
	//{
	//	pet_info.feed_degree = quality_cfg->max_feed_degree;
	//	unsigned int now_second = (unsigned int)EngineAdapter::Instance().Time();
	//	pet_info.baoshi_active_time = now_second;
	//	target_role->ReCalcAttr();
	//	if (NULL != other_role)
	//	{
	//		other_role->ReCalcAttr();
	//	}
	//	this->RecalNextFeedDegreeTime();
	//	LittlePet *target_little_pet = GLOBAL_ROLE_MGR.GetLittlePet(this->GetLoverId());
	//	if (NULL != target_little_pet)
	//	{
	//		target_little_pet->RecalNextFeedDegreeTime();
	//	}
	//}

	//this->OnDataChange();

	//this->SendFeedDegree(index, pet_info.feed_degree);
	//target_role->NoticeNum(noticenum::NT_LITTLE_PET_FEED_SUCC);

	//gamelog::g_log_little_pet.printf(LL_INFO, "LittlePet::OnFeed user[%d %s]_lover, lover_pet_index[%d], cur_feed_degreed[%d]",
	//	target_role->GetUID(), target_role->GetName(), index, pet_info.feed_degree);
}

void LittlePet::FeedLittlePetNew(Role *role, int index, bool is_aut_buy)
{
	// 小宠物喂养材料分为4个品阶：蓝、紫、橙、红
	// 每种小宠物喂养现在只消耗对应品阶的一种材料，消耗的数量会随着等级的提升而增加

	if (index < 0 || index >= LITTLE_PET_PERSON_MAX_NUM) return;

	LittlePetParam &pet_info = m_little_pet_param.pet_list[index];

	const LittlePetBaseCfg *base_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetBaseCfgById(pet_info.pet_id);
	if (NULL == base_cfg) return;

	const int old_feed_level = pet_info.feed_level;
	const LittlePetFeedCfg *feed_level_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetFeedCfg(base_cfg->quality_type, old_feed_level);
	if (nullptr == feed_level_cfg)
	{
		role->NoticeNum(errornum::EN_LITTLE_PET_MAX_FEED_DEGREE);
		return;
	}

	const LittlePetFeedCfg *next_feed_level_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetFeedCfg(base_cfg->quality_type, old_feed_level + 1);
	if (nullptr == next_feed_level_cfg)
	{
		gstr::SendError(role, "little_pet_feed_level_limit");
		return;
	}

	int pet_quality_type = base_cfg->quality_type;
	if (pet_quality_type < 0 || pet_quality_type >= LITTLEPET_MAX_QUALITY_TYPE)
	{
		return;
	}

	{
		// 计算喂养消耗材料，只消耗对应品质一种
		int stuff_count = 0;
		static ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

		//for (; stuff_count < ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM; ++stuff_count)
		//{
		//	if (feed_level_cfg->stuff_id_list[stuff_count] <= 0) break;
		//
		//	stuff_list[stuff_count].item_id = feed_level_cfg->stuff_id_list[stuff_count];
		//	stuff_list[stuff_count].num = feed_level_cfg->stuff_num_list[stuff_count];
		//	stuff_list[stuff_count].buyable = is_aut_buy;
		//}

		if (feed_level_cfg->stuff_id_list[pet_quality_type] <= 0) return;

		stuff_list[stuff_count].item_id = feed_level_cfg->stuff_id_list[pet_quality_type];
		stuff_list[stuff_count].num = feed_level_cfg->stuff_num_list[pet_quality_type];
		stuff_list[stuff_count].buyable = is_aut_buy;
		stuff_count++;

		if (stuff_count <= 0) return;

		// 检查
		static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();

		if (!ItemExtern::GetConsumeListByOrder(role, true, stuff_count, stuff_list, &consume_list))
		{
			role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(role, consume_list, "LittlePet::FeedLittlePetNew"))
		{
			return;
		}
	}

	++pet_info.feed_level;
	this->OnDataChange();

	if (role->GetUID() == m_role_id)
	{
		this->SendSingleInfo(role, index);
	}
	else
	{
		this->SendSingleInfo(role, index, 0);
	}

	this->SendFeedDegree(index, pet_info.feed_degree);

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LITTLE_PET);

	{
		Role *lover_role = NULL;
		if (role->GetUID() == this->GetLoverId())		// 帮助对方升级
		{
			lover_role = this->GetRole();
		}
		else
		{
			lover_role = this->GetLoverRole();			// 自己升级
		}

		if (nullptr != lover_role)
		{
			lover_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LITTLE_PET);

			if (lover_role->GetUID() == m_role_id)
			{
				this->SendSingleInfo(lover_role, index);
			}
			else
			{
				this->SendSingleInfo(lover_role, index, 0);
			}
		}
	}

	gamelog::g_log_little_pet.printf(LL_INFO, "%s user[%d %s] old_feed_level:[%d], now_feed_level:[%d]", __FUNCTION__,
		role->GetUID(), role->GetName(), old_feed_level, pet_info.feed_level);
}

void LittlePet::Interact(Role *target_role, int index)
{
	//if (index < 0 || index >= LITTLE_PET_PERSON_MAX_NUM) return;


	//if (target_role->GetUID() == m_role_id || target_role->GetUID() == this->GetLoverId())
	//{
	//	target_role->NoticeNum(errornum::EN_LITTLE_PET_CAN_NOT_INTERACT_SEFL_PET);
	//	return;
	//}

	//LittlePetParam &pet_info = m_little_pet_param.pet_list[index];
	//if (pet_info.pet_id <= 0) return;

	//GlobalRole *target = GlobalRoleManager::Instance().GetGlobalRole(target_role->GetUID());
	//if (NULL == target)	return;

	//LittlePet *target_pet = target->GetLittlePet();
	//if (NULL == target_pet)	return;

	//const LittlePetOtherCfg &other_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetLittlePetOtherCfg();


	//if (pet_info.interact_times >= other_cfg.per_pet_interact_times)
	//{
	//	target_role->NoticeNum(errornum::EN_LITTLE_PET_NO_PET_INTERACT_TIMES);
	//	return;
	//}
	//if (target_pet->m_little_pet_param.interact_times >= other_cfg.per_user_total_interact_times)
	//{
	//	target_role->NoticeNum(errornum::EN_LITTLE_PET_NO_PERSONAL_INTERACT_TIMES);
	//	return;
	//}

	//++target_pet->m_little_pet_param.interact_times;
	//++pet_info.interact_times;

	//this->SetInteractLog(target_role, pet_info);
	//this->AddScore(other_cfg.per_interact_add_score, "interact"); //被互动的主人增加的积分
	//target_pet->AddScore(other_cfg.per_interact_add_score, "interact"); //发起请求

	//LittlePet * lover_pet = GLOBAL_ROLE_MGR.GetLittlePet(m_little_pet_param.lover_id);
	//if (NULL != lover_pet)
	//{
	//	lover_pet->AddScore(other_cfg.per_interact_add_score, "interatc");
	//}

	//target_pet->SendRoleInteractTimes();
	//this->SendPetInteractTimes(target_role, index);
//	target_role->NoticeNum(noticenum::NT_LITTLE_PET_INTERACT_SUCC);

	//// 日志
	//gamelog::g_log_little_pet.printf(LL_INFO, "LittlePet::OnIntreact target_user[%d %s],target_role_cur_interact_times[%d], target_cur_score[%d] m_user[%d], m_role_littlepet_index[%d], m_cur_score[%d]",
	//	target_role->GetUID(), target_role->GetName(), m_little_pet_param.interact_times, target_pet->GetScore(), m_role_id, index, this->GetScore());
}

void LittlePet::Choujiang(int type)
{
	Role *role = this->GetRole();
	if (NULL == role) return;

	int chou_times = 0;
	int need_gold = 0;
	bool is_free_chou = false;

	const LittlePetOtherCfg &other_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetLittlePetOtherCfg();

	unsigned int now_time = (unsigned int)EngineAdapter::Instance().Time();
	switch (type)
	{
	case LITTLE_PET_CHONGJIANG_TYPE_ONE:
		{
			if (m_little_pet_param.last_free_chou_timestamp + other_cfg.free_chou_interval_h * SECOND_PER_HOUR < now_time)
			{
				is_free_chou = true;
			}
			else
			{
				need_gold = other_cfg.one_chou_consume_gold;
			}

			chou_times = LITTLE_PET_CHONGJIANG_TYPE_ONE;
		}
		break;

	case LITTLE_PET_CHONGJIANG_TYPE_TEN:
		{
			chou_times = LITTLE_PET_CHONGJIANG_TYPE_TEN;
			need_gold = other_cfg.ten_chou_consume_gold;
		}
		break;

	default:
		return;
	}

	// 检查背包空间是否足够
	if (!role->GetRoleChestShop()->CheckHaveSpace(CHEST_SHOP_TYPE_EQUIP, chou_times))
	{
		role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_SPACE);
		return;
	}

	//if (!role->GetKnapsack()->CheckEmptyGridNoLessThan(chou_times))
	//{
	//	role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
	//	return;
	//}

	int chou_cfg_count = LOGIC_CONFIG->GetLittlePetCfg().GetLittlePetChouCfgCount();
	if (chou_cfg_count <= 0) return;

	int chou_total_weight = LOGIC_CONFIG->GetLittlePetCfg().GetLittlePetChouTotalWeight();
	if (chou_total_weight <= 0) return;

	const LittlePetChouJiangCfg &chou_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetLittlePetChouCfg();
	char is_bind = 0;

	if (!is_free_chou)
	{
		//consume_gold
		//if (!role->GetKnapsack()->GetMoney()->UseGold(need_gold, "little_pet_Choujiang_TenTimes"))
		//{
		//	role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		//	return;
		//}

		if (other_cfg.can_use_item)
		{
			if (type == LITTLE_PET_CHONGJIANG_TYPE_ONE)
			{
				if (!role->GetKnapsack()->ConsumeItem(other_cfg.replacement_id, other_cfg.consume_replacement, "little_pet_Choujiang_OneTime"))
				{
					if (!role->GetKnapsack()->GetMoney()->UseGold(need_gold, "little_pet_Choujiang_OneTime"))
					{
						role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
						return;
					}
				}
			}
			else if (type == LITTLE_PET_CHONGJIANG_TYPE_TEN)
			{
				if (!role->GetKnapsack()->ConsumeItem(other_cfg.ten_chou_use_itemid, other_cfg.ten_chou_use_item_num, "little_pet_Choujiang_TenTime"))
				{
					if (!role->GetKnapsack()->GetMoney()->UseGold(need_gold, "little_pet_Choujiang_TenTime"))
					{
						role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
						return;
					}
				}
			}
		}
		else
		{
			if (!role->GetKnapsack()->GetMoney()->UseGold(need_gold, "little_pet_Choujiang"))
			{
				role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}
		}
	}
	else
	{
		//free
		m_little_pet_param.last_free_chou_timestamp = now_time;
	}

	if (is_free_chou) is_bind = 1;
	bool is_first_choujiang = (0 != m_little_pet_param.is_first_choujiang);

	int list_num = 0;
	static ChestItemInfo item_list[MODE_MAX_COUNT];
	for (int i = 0; i < MODE_MAX_COUNT; ++i)
	{
		item_list[i].Reset();
	}

	static Protocol::SCLittlePetChouRewardList reward_info;
	reward_info.list_count = 0;
	reward_info.final_reward_seq = -1;

	gamelog::g_log_little_pet.buff_printf("LittlePet::Choujiang user[%d %s] chou_count[%d] reward_item_list(", role->GetUID(), role->GetName(), chou_times);

	for (int i = 0; i < chou_times && list_num < MODE_MAX_COUNT; ++i)
	{
		if (!is_first_choujiang && (LITTLE_PET_CHONGJIANG_TYPE_ONE == chou_times))
		{
			m_little_pet_param.is_first_choujiang = 1;

			reward_info.reward_list[reward_info.list_count].item_id = other_cfg.first_reward_item_cfg.item_id;
			reward_info.reward_list[reward_info.list_count].item_num = other_cfg.first_reward_item_cfg.num;
			reward_info.reward_list[reward_info.list_count].is_bind = other_cfg.first_reward_item_cfg.is_bind;
			++reward_info.list_count;
			//int type_item_put = PUT_REASON_LITTLE_PET_CHOUJIANG_ONE;
			//role->GetKnapsack()->Put(item, type_item_put);

			item_list[list_num].item_id = other_cfg.first_reward_item_cfg.item_id;
			item_list[list_num].num = other_cfg.first_reward_item_cfg.num;
			item_list[list_num].is_bind= other_cfg.first_reward_item_cfg.is_bind;
			++list_num;

			// 日志
			gamelog::g_log_little_pet.buff_printf("[%d, %d],", other_cfg.first_reward_item_cfg.item_id, other_cfg.first_reward_item_cfg.num);
		}
		else
		{
			int rand_num = RandomNum(chou_total_weight);
			for (int seq = 0; seq < chou_cfg_count; seq++)
			{
				if (rand_num < chou_cfg.chou_cfg_list[seq].weight)
				{
					LotterySequence replace_seq = static_cast<LotterySequence>(seq);
					role->GetLotteryCount()->ReplaceLotterySequence(LOTTERY_REPLACE_TYPE_LITTERY_PET, replace_seq);
					if (replace_seq < 0 || replace_seq >= chou_cfg_count)
					{
						replace_seq = seq;
					}
					reward_info.final_reward_seq = chou_cfg.chou_cfg_list[replace_seq].seq;
					reward_info.reward_list[reward_info.list_count].item_id = chou_cfg.chou_cfg_list[replace_seq].reward_item_cfg.item_id;
					reward_info.reward_list[reward_info.list_count].item_num = chou_cfg.chou_cfg_list[replace_seq].reward_item_cfg.num;
					reward_info.reward_list[reward_info.list_count].is_bind = is_bind;
					++reward_info.list_count;
					ItemConfigData item = chou_cfg.chou_cfg_list[replace_seq].reward_item_cfg;
					//int type_item_put = (LITTLE_PET_CHONGJIANG_TYPE_ONE == chou_times) ? PUT_REASON_LITTLE_PET_CHOUJIANG_ONE : PUT_REASON_LITTLE_PET_CHOUJIANG_TEN;
					//role->GetKnapsack()->Put(item, type_item_put);

					item_list[list_num].item_id = item.item_id;
					item_list[list_num].num = item.num;
					item_list[list_num].is_bind = is_bind;
					++list_num;

					if (chou_cfg.chou_cfg_list[replace_seq].is_broadcast > 0)
					{
						int send_len = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "choujiang_broadcast_content", role->GetUID(), role->GetName(), role->GetCamp(), chou_cfg.chou_cfg_list[replace_seq].reward_item_cfg.item_id);
						if (send_len > 0)
						{
							World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, send_len, SYS_MSG_CENTER_NOTICE_3);
						}
					}

					// 日志
					gamelog::g_log_little_pet.buff_printf("[%d, %d],", item.item_id, item.num);
					break;
				}
				rand_num -= chou_cfg.chou_cfg_list[seq].weight;
			}
		}
	}

	if (other_cfg.choujiang_get_score > 0)
	{
		this->AddScore(chou_times * other_cfg.choujiang_get_score, "LittlePet::Choujiang");
	}

	role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_EQUIP, list_num, item_list, "LittlePet::Choujiang");

	int len = sizeof(reward_info) - sizeof(reward_info.reward_list[0]) * (LITTLE_PET_CHONGJIANG_TYPE_TEN - reward_info.list_count);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&reward_info, len);

	this->SendFreeChouTimestamp();

	gamelog::g_log_little_pet.buff_print(")");
	gamelog::g_log_little_pet.commit_buff(LL_INFO);
}

void LittlePet::AddScore(int score, const char *reason)
{
	Role *role = this->GetRole();
	if (NULL == role) return;

	long long tem_score = m_little_pet_param.score;
	tem_score += score;
	if (tem_score >= INT_MAX)
	{
		m_little_pet_param.score = INT_MAX;
	}
	else
	{
		m_little_pet_param.score = (int)tem_score;
	}

	this->OnDataChange();
	this->SendScoreInfo(role);

	// 日志
	gamelog::g_log_little_pet.printf(LL_INFO, "LittlePet::OnAddScore user[%d %s], cur_score[%d]",
		role->GetUID(), role->GetName(), this->GetScore());
}

void LittlePet::ExchangeItem(int seq, int num)
{
	Role *role = this->GetRole();
	if (NULL == role) return;

	if (seq < 0 || seq >= LITTLEPET_EXCHANGE_CFG_MAX_NUM) return;

	const LittlePetExchangeCfg *item_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetLittlePetExchangeCfg(seq);
	if (NULL == item_cfg) return;

	// check enough score
	if (item_cfg->need_score * num > m_little_pet_param.score)
	{
		gstr::SendError(role, "little_pet_no_enough_score");
		return;
	}
	// 检查背包空间是否足够
	const ItemBase *itembase = ITEMPOOL->GetItem(item_cfg->exchange_item_cfg.item_id);
	if (nullptr == itembase)
	{
		return;
	}

	int stack_num = itembase->GetPileLimit();
	if (stack_num <= 0) return;
	int need_grid = (stack_num + num) / stack_num;

	if (!role->GetKnapsack()->CheckEmptyGridNoLessThan(need_grid))
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	// 消耗
	this->AddScore(-item_cfg->need_score * num, "LittlePet::ExchangeItem");

	// 给予
	ItemConfigData item;
	item.item_id = item_cfg->exchange_item_cfg.item_id;
	item.num = item_cfg->exchange_item_cfg.num * num;
	item.is_bind = item_cfg->exchange_item_cfg.is_bind;
	role->GetKnapsack()->Put(item, PUT_REASON_LITTLE_PET_EXCHANGE_ITEM);
	gstr::SendNotice(role, "little_pet_exchange_succ");

	// 日志
	gamelog::g_log_little_pet.printf(LL_INFO, "LittlePet::ExchangeItem user[%d %s], item_seq[%d], num[%d], need_score[%d], cur_score[%d]",
		role->GetUID(), role->GetName(), seq, num, item_cfg->need_score * num, this->GetScore());
}

bool LittlePet::PutOn(int pet_index, int knapsack_index)
{
	if (pet_index < 0 || pet_index >= LITTLE_PET_PERSON_MAX_NUM) return false;

	Role *role = this->GetRole();
	if (nullptr == role) return false;

	if (role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	ItemGridData item_grid;
	if (!role->GetKnapsack()->GetItemGridData(knapsack_index, &item_grid) && 1 != item_grid.num)
	{
		gstr::SendError(role, "little_pet_no_pet_limit");
		return false;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_grid.item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType()) return false;

	if (Equipment::E_TYPE_SPECIAL_LITTLEPET != equip->GetEquipType()) return false;

	int feed_level = 0;

	EquipmentParam *param = (EquipmentParam*)item_grid.param;
	if (NULL != param)
	{
		feed_level = param->param1;   // param1用于存储小宠物喂养等级
	}

	const LittlePetBaseCfg *pet_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetBaseCfgByItemID(item_grid.item_id);
	if (nullptr == pet_cfg)
	{
		gstr::SendError(role, "little_pet_no_pet_limit");
		return false;
	}

	LittlePetParam &pet_info = m_little_pet_param.pet_list[pet_index];

	if (pet_info.pet_id > 0 && !this->TakeOff(pet_index))
	{
		return false;
	}

	if (!role->GetKnapsack()->ConsumeItemByIndex(knapsack_index, 1, "LittlePet::PutOn"))
	{
		return false;
	}

	pet_info.pet_id = pet_cfg->little_pet_id;
	pet_info.feed_level = (short)feed_level;

	this->GetPetBaseAttr(pet_info, pet_cfg);

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LITTLE_PET);
	this->OnDataChange();
	this->SendSingleInfo(role, pet_index);

	Role *lover_role = this->GetLoverRole();
	if (nullptr != lover_role)
	{
		lover_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LITTLE_PET);
		this->SendSingleInfo(lover_role, pet_index, 0);
	}

	gamelog::g_log_little_pet.printf(LL_INFO, "%s User[%d %s] consume item[%d %d]",
		__FUNCTION__, role->GetUID(), role->GetName(), item_grid.item_id, item_grid.num);

	return true;
}

bool LittlePet::TakeOff(int pet_index)
{
	if (pet_index < 0 || pet_index >= LITTLE_PET_PERSON_MAX_NUM)
	{
		return false;
	}

	Role *role = this->GetRole();
	if (nullptr == role)
	{
		return false;
	}

	if (role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (!role->GetKnapsack()->HaveSpace())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	LittlePetParam &pet_info = m_little_pet_param.pet_list[pet_index];
	if (pet_info.pet_id <= 0)
	{
		return false;
	}

	const LittlePetBaseCfg *pet_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetBaseCfgById(pet_info.pet_id);
	if (nullptr == pet_cfg)
	{
		return false;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(pet_cfg->active_item_id);
	if (nullptr == item_base)
	{
		return false;
	}

	//重置数据
	int feed_level = pet_info.feed_level;
	pet_info.pet_id = 0;
	pet_info.feed_level = 0;

	this->OnDataChange();
	this->SendSingleInfo(role, pet_index);
	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LITTLE_PET);

	// 返还物品
	ItemDataWrapper item_wrapper(item_base->GetItemId(), 1, 1);
	item_wrapper.has_param = 1;
	item_wrapper.param_data.param1 = feed_level;   // param1用于存储小宠物喂养等级
	role->GetKnapsack()->Put(item_wrapper, PUT_REASON_LITTLE_PET_TAKEOFF);

	Role *lover_role = this->GetLoverRole();
	if (nullptr != lover_role)
	{
		this->SendSingleInfo(lover_role, pet_index, 0);
		lover_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LITTLE_PET);
	}

	gamelog::g_log_little_pet.printf(LL_INFO, "%s User[%d %s] return_item[%d %d %d]",
		__FUNCTION__, role->GetUID(), role->GetName(), item_base->GetItemId(), 1, 1);

	return true;
}

bool LittlePet::EquipmentPutOn(Role *role, int pet_index, int knapsack_index)
{
	if (nullptr == role) return false;

	if (pet_index < 0 || pet_index >= LITTLE_PET_PERSON_MAX_NUM) return false;

	ItemGridData item_grid;

	if (!role->GetKnapsack()->GetItemGridData(knapsack_index, &item_grid))
	{
		role->NoticeNum(errornum::EN_KNAPSACK_LOAD_ERROR);
		return false;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(item_grid.item_id);
	if (nullptr == item_base || ItemBase::I_TYPE_EQUIPMENT != item_base->GetItemType())
	{
		return false;
	}

	const Equipment *equipment = static_cast<const Equipment *>(item_base);
	if (equipment->GetEquipType() < Equipment::E_TYPE_LITTLE_PET_MIN || equipment->GetEquipType() >= Equipment::E_TYPE_LITTLE_PET_MAX)
	{
		return false;
	}

	LittlePetParam &pet_info = m_little_pet_param.pet_list[pet_index];

	int equip_index = equipment->GetEquipType() - Equipment::E_TYPE_LITTLE_PET_MIN;
	if (equip_index < 0 || equip_index >= LITTLEPET_EQUIP_INDEX_MAX_NUM)
	{
		return false;
	}

	// 小宠物最高可装备与自己品阶相同颜色的玩具，可以向下装备(比如紫色的小宠物可以装备蓝色的玩具)
	int quality_type = LOGIC_CONFIG->GetLittlePetCfg().GetQualityTypeByPetId(pet_info.pet_id);
	if (quality_type < 0 || quality_type >= LITTLEPET_MAX_QUALITY_TYPE)
	{
		return false;
	}

	// 小宠物最低为蓝色装备
	int convert_quality_type = quality_type + ItemBase::I_COLOR_BLUE;
	if (convert_quality_type < item_base->GetColor())
	{
		gstr::SendError(role, "little_pet_equipment_quality_limit");
		return false;
	}

	if (0 != pet_info.equipment_list[equip_index].equipment_id && !this->EquipmentTakeOff(role, pet_index, equip_index))
	{
		return false;
	}

	if (!role->GetKnapsack()->ConsumeItemByIndex(knapsack_index, 1, "LittlePet::EquipmentPutOn"))
	{
		return false;
	}

	pet_info.equipment_list[equip_index].equipment_id = item_grid.item_id;
	pet_info.equipment_list[equip_index].is_bind = item_grid.is_bind ? (char)1 : (char)0;
	this->OnDataChange();
	this->SendSingleInfo(role, pet_index);

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LITTLE_PET);

	Role *lover_role = this->GetLoverRole();
	if (nullptr != lover_role)
	{
		this->SendSingleInfo(lover_role, pet_index, 0);
		lover_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LITTLE_PET);
	}

	return true;
}

bool LittlePet::EquipmentTakeOff(Role *role, int pet_index, int equip_index)
{
	if (nullptr == role) return false;

	if (pet_index < 0 || pet_index >= LITTLE_PET_PERSON_MAX_NUM) return false;
	if (equip_index < 0 || equip_index >= LITTLEPET_EQUIP_INDEX_MAX_NUM) return false;

	LittlePetParam &pet_info = m_little_pet_param.pet_list[pet_index];

	const ItemBase *item_base = ITEMPOOL->GetItem(pet_info.equipment_list[equip_index].equipment_id);
	if (nullptr == item_base || ItemBase::I_TYPE_EQUIPMENT != item_base->GetItemType())
	{
		return false;
	}

	if (!role->GetKnapsack()->HaveSpace())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	// 脱下
	pet_info.equipment_list[equip_index].equipment_id = 0;
	this->OnDataChange();
	this->SendSingleInfo(role, pet_index);

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LITTLE_PET);

	ItemDataWrapper item_wrapper(item_base->GetItemId(), 1, pet_info.equipment_list[equip_index].is_bind);
	role->GetKnapsack()->Put(item_wrapper, PUT_REASON_LITTLE_PET_EQUIPMENT_TAKEOFF);

	Role *lover_role = this->GetLoverRole();
	if (nullptr != lover_role)
	{
		this->SendSingleInfo(lover_role, pet_index, 0);
		lover_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LITTLE_PET);
	}

	return true;
}

bool LittlePet::OnEquipmentUplevel(Role *role, int pet_index, int equip_index, bool is_auto_buy)
{
	if (nullptr == role) return false;

	if (pet_index < 0 || pet_index >= LITTLE_PET_PERSON_MAX_NUM) return false;

	if (equip_index < 0 || equip_index >= LITTLEPET_EQUIP_INDEX_MAX_NUM) return false;

	LittlePetEquipParam &equip_info = m_little_pet_param.pet_list[pet_index].equipment_list[equip_index];

	//对应部位上必须有装备
	const ItemBase *item_base = ITEMPOOL->GetItem(equip_info.equipment_id);
	if (nullptr == item_base || ItemBase::I_TYPE_EQUIPMENT != item_base->GetItemType())
	{
		return false;
	}

	const int old_level = equip_info.level;

	const LittlePetEquipUplevelCfg *cur_level_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetEquipmentUplevelCfg(equip_index, old_level);
	if (nullptr == cur_level_cfg)
	{
		gstr::SendError(role, "little_pet_equipment_level_limit");
		return false;
	}

	const LittlePetEquipUplevelCfg *next_level_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetEquipmentUplevelCfg(equip_index, old_level + 1);
	if (nullptr == next_level_cfg)
	{
		gstr::SendError(role, "little_pet_equipment_level_limit");
		return false;
	}

	// 检查消耗
	{
		int stuff_count = 0;
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

		stuff_list[stuff_count].item_id = cur_level_cfg->stuff_id;
		stuff_list[stuff_count].num = cur_level_cfg->stuff_num;
		stuff_list[stuff_count].buyable = is_auto_buy;
		++stuff_count;

		static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();

		// 检查
		if (!ItemExtern::GetConsumeListByOrder(role, true, stuff_count, stuff_list, &consume_list))
		{
			return false;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(role, consume_list, "LittlePet::OnEquipmentUplevel"))
		{
			return false;
		}
	}

	++equip_info.level;

	this->OnDataChange();
	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LITTLE_PET);

	if (role->GetUID() == m_role_id)
	{
		this->SendSingleInfo(role, pet_index);
	}
	else
	{
		this->SendSingleInfo(role, pet_index, 0);
	}

	{
		Role *lover_role = NULL;
		if (role->GetUID() == this->GetLoverId())			// 帮助对方升级
		{
			lover_role = this->GetRole();
		}
		else
		{
			lover_role = this->GetLoverRole();				// 自己升级
		}

		if (nullptr != lover_role)
		{
			if (lover_role->GetUID() == m_role_id)
			{
				this->SendSingleInfo(lover_role, pet_index);
			}
			else
			{
				this->SendSingleInfo(lover_role, pet_index, 0);
			}

			lover_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LITTLE_PET);
		}
	}

	gamelog::g_log_little_pet.printf(LL_INFO, "%s user[%d %s] level_change[%d -> %d]",
		__FUNCTION__, role->GetUID(), role->GetName(), old_level, equip_info.level);

	return true;
}

int LittlePet::GetLoverId()
{
	return m_little_pet_param.lover_id;
}

void LittlePet::GetTotalAttr(CharIntAttrs &tmp_attr)
{
	for (int index = 0; index < LITTLE_PET_PERSON_MAX_NUM; ++index)
	{
		LittlePetParam &tem_pet = m_little_pet_param.pet_list[index];
		if (tem_pet.pet_id <= 0) continue;

		//基础属性加成
		tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += tem_pet.base_attr[LITTLEPET_VALUE_TYPE_HP];
		tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += tem_pet.base_attr[LITTLEPET_VALUE_TYPE_GONGJI];
		tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += tem_pet.base_attr[LITTLEPET_VALUE_TYPE_FANGYU];
		tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += tem_pet.base_attr[LITTLEPET_VALUE_TYPE_MINGZHONG];
		tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += tem_pet.base_attr[LITTLEPET_VALUE_TYPE_SHANBI];
		tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += tem_pet.base_attr[LITTLEPET_VALUE_TYPE_BAOJI];
		tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += tem_pet.base_attr[LITTLEPET_VALUE_TYPE_KANGBAO];

		//const LittlePetBaseCfg *base_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetBaseCfgById(tem_pet.pet_id);
		//if (NULL == base_cfg) continue;

		//const LittlePetQualityCfg *quality_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetLittlePetQualityCfg(base_cfg->quality_type);
		//if (NULL == quality_cfg) continue;

		//饱食度加成
		//unsigned int now_time = (unsigned int)EngineAdapter::Instance().Time();
		//const LittlePetOtherCfg &other_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetLittlePetOtherCfg();

		//double percent = quality_cfg->add_attr_percent / 100.0;
		//if (now_time <= 3600 * other_cfg.feed_clear_interval_h + tem_pet.baoshi_active_time)
		//{
		//	tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<int>(tem_pet.base_attr[LITTLEPET_VALUE_TYPE_HP] * percent);
		//	tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<int>(tem_pet.base_attr[LITTLEPET_VALUE_TYPE_GONGJI] * percent);
		//	tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<int>(tem_pet.base_attr[LITTLEPET_VALUE_TYPE_FANGYU] * percent);
		//	tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<int>(tem_pet.base_attr[LITTLEPET_VALUE_TYPE_MINGZHONG] * percent);
		//	tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<int>(tem_pet.base_attr[LITTLEPET_VALUE_TYPE_SHANBI] * percent);
		//	tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<int>(tem_pet.base_attr[LITTLEPET_VALUE_TYPE_BAOJI] * percent);
		//	tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<int>(tem_pet.base_attr[LITTLEPET_VALUE_TYPE_KANGBAO] * percent);
		//}

		// 喂养等级
		{
			int quality_type = LOGIC_CONFIG->GetLittlePetCfg().GetQualityTypeByPetId(tem_pet.pet_id);
			const LittlePetFeedCfg *feed_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetFeedCfg(quality_type, tem_pet.feed_level);
			if (nullptr != feed_cfg)
			{
				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += feed_cfg->max_hp;
				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += feed_cfg->gongji;
				tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += feed_cfg->fangyu;

				// 宠物基础属性万分比加成
				if (feed_cfg->base_attr_add_per > 0)
				{
					double ratio = feed_cfg->base_attr_add_per * ROLE_ATTR_PER;

					tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(tem_pet.base_attr[LITTLEPET_VALUE_TYPE_HP] * ratio);
					tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(tem_pet.base_attr[LITTLEPET_VALUE_TYPE_GONGJI] * ratio);
					tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(tem_pet.base_attr[LITTLEPET_VALUE_TYPE_FANGYU] * ratio);
					tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(tem_pet.base_attr[LITTLEPET_VALUE_TYPE_MINGZHONG] * ratio);
					tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(tem_pet.base_attr[LITTLEPET_VALUE_TYPE_SHANBI] * ratio);
					tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(tem_pet.base_attr[LITTLEPET_VALUE_TYPE_BAOJI] * ratio);
					tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(tem_pet.base_attr[LITTLEPET_VALUE_TYPE_KANGBAO] * ratio);
				}
			}
		}
		
		// 装备属性
		{
			CharIntAttrs equip_attrs;
			equip_attrs.Reset();

			for (int equip_index = 0; equip_index < LITTLEPET_EQUIP_INDEX_MAX_NUM; ++equip_index)
			{
				const Equipment *equipment = static_cast<const Equipment *>(ITEMPOOL->GetItem(tem_pet.equipment_list[equip_index].equipment_id));
				if (nullptr != equipment && equipment->GetEquipType() - Equipment::E_TYPE_LITTLE_PET_MIN == equip_index)
				{
					//LittlePetParam &pet_info = m_little_pet_param.pet_list[pet_index];

					// 小宠物最高可装备与自己品阶相同颜色的玩具，可以向下装备(比如紫色的小宠物可以装备蓝色的玩具)
					int quality_type = LOGIC_CONFIG->GetLittlePetCfg().GetQualityTypeByPetId(tem_pet.pet_id);
					if (quality_type < 0 || quality_type >= LITTLEPET_MAX_QUALITY_TYPE)
					{
						continue;
					}

					// 小宠物最低为蓝色装备。如果小宠物的品质低于玩具的品质则屏蔽该装备的属性
					int convert_quality_type = quality_type + ItemBase::I_COLOR_BLUE;
					if (convert_quality_type < equipment->GetColor())
					{
						continue;
					}

					//////////////////////////////////////////////////////////////////////////
					equip_attrs.Reset();
					equipment->GetBaseAttr(equip_attrs);

					tmp_attr.AddAttrs(&equip_attrs);

					// 装备格子等级加成当前格子属性,必须有装备才有加成
					LittlePetEquipParam &equip_info = tem_pet.equipment_list[equip_index];

					const LittlePetEquipUplevelCfg *equip_level_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetEquipmentUplevelCfg(equip_index, equip_info.level);
					if (nullptr != equip_level_cfg)
					{
						// 等级属性
						tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += equip_level_cfg->maxhp;
						tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += equip_level_cfg->gongji;
						tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += equip_level_cfg->fangyu;

						//增加当前装备基础属性x%
						if (equip_level_cfg->base_attrs_add_per > 0)
						{
							for (int base_attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_BEGIN; base_attr_type < CharIntAttrs::BASE_CHARINTATTR_TYPE_MAX; ++base_attr_type)
							{
		//						tmp_attr.m_attrs[base_attr_type] += static_cast<Attribute>(equip_attrs.m_attrs[base_attr_type] * equip_level_cfg->base_attrs_add_per * ROLE_ATTR_PER);
								Attribute attr_val = static_cast<Attribute>(equip_attrs.m_attrs[base_attr_type] * equip_level_cfg->base_attrs_add_per * ROLE_ATTR_PER);
								tmp_attr.AddValue(base_attr_type, attr_val);
							}
						}
					}
				}
			}
		}

		//强化属性加成
		//for (int point_type = 0; point_type < LITTLEPET_QIANGHUAPOINT_CURRENT_NUM - 1; ++point_type)
		//{
		//	for (int grid_index = 0; grid_index < LITTLEPET_QIANGHUAGRID_MAX_NUM; ++grid_index)
		//	{
		//		const LittleStarItem::AttributeItem &grid_cfg = m_little_pet_param.pet_list[index].star_list[point_type].attr_list[grid_index];
		//		if (grid_cfg.arrt_type > LITTLEPET_VALUE_TYPE_MAX) continue;

		//		int real_type = this->GetRealType(grid_cfg.arrt_type);
		//		if (real_type < 0) continue;

		//		tmp_attr.m_attrs[real_type] += grid_cfg.attr_value;
		//	}
		//}

		//for (int grid_index = 0; grid_index < LITTLEPET_QIANGHUAGRID_MAX_NUM; ++grid_index)
		//{
		//	LittleStarItem::AttributeItem &special_grid_cfg = m_little_pet_param.pet_list[index].star_list[QIANG_HUA_POINT_TYPE_SPECIAL].attr_list[grid_index];
		//	if (special_grid_cfg.arrt_type > LITTLEPET_VALUE_TYPE_MAX) continue;

		//	int real_type = this->GetRealType(special_grid_cfg.arrt_type);
		//	if (real_type < 0) continue;

		//	tmp_attr.m_attrs[real_type] += static_cast<int>(tem_pet.base_attr[special_grid_cfg.arrt_type] * (special_grid_cfg.attr_value / 100.0));
		//}
	}
}

int LittlePet::GetUsingPetId()
{
	if (m_little_pet_param.using_index < 0 || m_little_pet_param.using_index >= LITTLE_PET_PERSON_MAX_NUM)	return 0;

	return m_little_pet_param.pet_list[m_little_pet_param.using_index].pet_id;
}

void LittlePet::GetCurPetName(GameName &name)
{
	if (m_little_pet_param.using_index >= 0 && m_little_pet_param.using_index < LITTLE_PET_PERSON_MAX_NUM)
	{
		F_STRNCPY(name, m_little_pet_param.pet_list[m_little_pet_param.using_index].pet_name, sizeof(GameName));
	}
}

int LittlePet::GetScore()
{
	return m_little_pet_param.score;
}

void LittlePet::GetInteractLog(Protocol::SCLittlePetInteractLog &log_info)
{
	for (int index = 0; index < LITTLE_PET_PERSON_MAX_LOG_NUM; ++index)
	{
		if (m_little_pet_param.log_list[index].pet_id <= 0) continue;

		log_info.log_list[log_info.count].pet_id = m_little_pet_param.log_list[index].pet_id;
		log_info.log_list[log_info.count].timestamp = m_little_pet_param.log_list[index].timestamp;
		F_STRNCPY(log_info.log_list[log_info.count].pet_name, m_little_pet_param.log_list[index].pet_name, sizeof(GameName));

		UserCacheNode *target_role = UserCacheManager::Instance().GetUserNode(m_little_pet_param.log_list[index].uid);
		if (NULL != target_role)
		{
			target_role->GetName(log_info.log_list[log_info.count].name);
		}

		++log_info.count;
	}
}

void LittlePet::GetPetBriefInfo(Protocol::SCLittlePetFriendPetListInfo &pet_info, int info_type)
{
	for (int index = 0; index < LITTLE_PET_PERSON_MAX_NUM; index++)
	{
		const LittlePetParam &tmp_pet = m_little_pet_param.pet_list[index];

		if (tmp_pet.pet_id <= 0) continue;

		Protocol::LittlePetFriendPet &tmp_info = pet_info.pet_list[pet_info.count];
		tmp_info.index = index;
		tmp_info.pet_id = tmp_pet.pet_id;
		tmp_info.info_type = info_type;
		tmp_info.interact_times = tmp_pet.interact_times;

		F_STRNCPY(tmp_info.pet_name, tmp_pet.pet_name, sizeof(GameName));
		++pet_info.count;
	}
}

void LittlePet::GetAllPetInfo(Protocol::SCLittlePetAllInfo &all_info, int info_type)
{
	for (int index = 0; index < LITTLE_PET_PERSON_MAX_NUM; index++)
	{
		Protocol::LittlePetSingleInfo &tmp_pet = all_info.pet_list[all_info.pet_count];

		if (this->GetSingleInfo(tmp_pet, info_type, index))
		{
			++all_info.pet_count;
		}
	}
}

void LittlePet::SendInteractLog()
{
	Role * role = this->GetRole();
	if (NULL == role) return;

	static Protocol::SCLittlePetInteractLog log_info;
	log_info.count = 0;

	this->GetInteractLog(log_info);

	LittlePet * lover_pet_param = GLOBAL_ROLE_MGR.GetLittlePet(m_little_pet_param.lover_id);
	if (NULL != lover_pet_param)
	{
		lover_pet_param->GetInteractLog(log_info);
	}

	int len = sizeof(log_info) - sizeof(log_info.log_list[0]) * (LITTLE_PET_SHARE_MAX_LOG_NUM - log_info.count);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&log_info, len);
}

void LittlePet::SendRoleInteractTimes()
{
	Role *role = this->GetRole();
	if (NULL == role) return;

	static Protocol::SCLittlePetNotifyInfo sc;
	sc.param_type = Protocol::LITTLE_PET_NOTIFY_INFO_INTERACT_TIMES;
	sc.param1 = m_little_pet_param.interact_times;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&sc, sizeof(sc));
}

void LittlePet::SendPetInteractTimes(Role *role, int index)
{
	static Protocol::SCLittlePetNotifyInfo sc;
	sc.param_type = Protocol::LITTLE_PET_NOTIFY_INFO_PET_INTERACT_TIMES;
	sc.param1 = m_little_pet_param.pet_list[index].interact_times;
	sc.param2 = index;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&sc, sizeof(sc));
}

void LittlePet::RecalNextFeedDegreeTime()
{
	m_next_check_feed_degree_time = this->GetNextCheckFeedDegreeTime();

	LittlePet *target_little_pet = GLOBAL_ROLE_MGR.GetLittlePet(this->GetLoverId());
	if (NULL != target_little_pet)
	{
		unsigned int target_next_time = target_little_pet->GetNextCheckFeedDegreeTime();
		if (target_next_time < m_next_check_feed_degree_time)
		{
			m_next_check_feed_degree_time = target_next_time;
		}
	}
}

unsigned int LittlePet::GetNextCheckFeedDegreeTime()
{
	unsigned int now_second = (unsigned int)EngineAdapter::Instance().Time();

	unsigned int next_check_feed_degree_time = 0;
	const LittlePetOtherCfg &other_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetLittlePetOtherCfg();
	for (int index = 0; index < LITTLE_PET_PERSON_MAX_NUM; ++index)
	{
		LittlePetParam &pet_info = m_little_pet_param.pet_list[index];
		if (pet_info.pet_id <= 0) continue;

		unsigned int cd_end_time = pet_info.baoshi_active_time + 3600 * other_cfg.feed_clear_interval_h;
		if (now_second >= cd_end_time)
		{
			int quality_type = LOGIC_CONFIG->GetLittlePetCfg().GetQualityTypeByPetId(pet_info.pet_id);
			const LittlePetQualityCfg *quality_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetLittlePetQualityCfg(quality_type);
			if (NULL != quality_cfg)
			{
				if (pet_info.feed_degree >= quality_cfg->max_feed_degree)
				{
					pet_info.feed_degree = 0;
					this->SendFeedDegree(index, pet_info.feed_degree);
				}
			}

			continue;
		}

		if (0 == next_check_feed_degree_time || cd_end_time < next_check_feed_degree_time)
		{
			next_check_feed_degree_time = cd_end_time;
		}
	}

	return next_check_feed_degree_time;
}

void LittlePet::SendPetFriendList()
{
	Role *role = this->GetRole();
	if (NULL == role) return;

	static FriendListParam friend_list_param;
	friend_list_param.count = 0;
	role->GetFriendManager()->GetFriendList(&friend_list_param);

	static Protocol::SCLittlePetFriendInfo fpfgi;
	fpfgi.count = 0;

	for (int i = 0; i < friend_list_param.count && i < MAX_FRIEND_NUM && fpfgi.count < MAX_FRIEND_NUM; ++i)
	{
		int f_uid = friend_list_param.friend_list[i].friend_uid;
		if ( f_uid == this->GetLoverId()) continue;

		
		LittlePet *friend_pet = GLOBAL_ROLE_MGR.GetLittlePet(f_uid);
		GlobalRole *friend_littlepet = GLOBAL_ROLE_MGR.GetGlobalRole(f_uid);
		if (NULL != friend_littlepet && NULL != friend_pet)
		{
			const UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(f_uid);
			if (NULL != user_node)
			{
				Protocol::LittlePetFriendInfo &info_item = fpfgi.pet_friend_list[fpfgi.count];
				user_node->GetName(info_item.owner_name);
				info_item.friend_uid = f_uid;
				info_item.prof = user_node->prof;
				info_item.sex = user_node->sex;

				// 获取好友的宠物数量
				static Protocol::SCLittlePetFriendPetListInfo pet_info;
				pet_info.count = 0;
				friend_pet->GetPetBriefInfo(pet_info);
				info_item.pet_num = pet_info.count;

				// 好友伴侣的宠物数量
				LittlePet *friend_lover_pet = GLOBAL_ROLE_MGR.GetLittlePet(friend_pet->GetLoverId());
				GlobalRole *friend_lover_littlepet = GLOBAL_ROLE_MGR.GetGlobalRole(friend_pet->GetLoverId());
				if (NULL != friend_lover_pet && NULL != friend_lover_littlepet)
				{
					static Protocol::SCLittlePetFriendPetListInfo pet_lover_info;
					pet_lover_info.count = 0;
					friend_lover_pet->GetPetBriefInfo(pet_lover_info);
					info_item.pet_num = info_item.pet_num + pet_lover_info.count;
				}

				++fpfgi.count;
			}
		}
	}

	int send_len = sizeof(fpfgi) - sizeof(fpfgi.pet_friend_list[0]) * (MAX_FRIEND_NUM - fpfgi.count);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&fpfgi, send_len);
}

void LittlePet::SendToFrienPetInfo(Role *tatget_role)
{
	static Protocol::SCLittlePetFriendPetListInfo pet_info;
	pet_info.count = 0;

	this->GetPetBriefInfo(pet_info);

	//伴侣
	LittlePet *lover_pet = GLOBAL_ROLE_MGR.GetLittlePet(m_little_pet_param.lover_id);
	if (NULL != lover_pet)  lover_pet->GetPetBriefInfo(pet_info, 0);

	int len = sizeof(pet_info) - sizeof(pet_info.pet_list[0]) * (LITTLE_PET_COUPLE_MAX_SHARE_NUM - pet_info.count);
	EngineAdapter::Instance().NetSend(tatget_role->GetNetId(), (char*)& pet_info, len);
}

void LittlePet::SendAllInfo()
{
	Role *role = this->GetRole();
	if (NULL == role) return;

	static Protocol::SCLittlePetAllInfo  all_info;
	all_info.last_free_chou_timestamp = m_little_pet_param.last_free_chou_timestamp;
	all_info.interact_times = m_little_pet_param.interact_times;
	all_info.score = m_little_pet_param.score;
	all_info.pet_count = 0;

	//自己
	this->GetAllPetInfo(all_info, 1);

	LittlePet * lover_pet_param = GLOBAL_ROLE_MGR.GetLittlePet(m_little_pet_param.lover_id);
	if (NULL != lover_pet_param)
	{
		lover_pet_param->GetAllPetInfo(all_info, 0);
	}

	int len = sizeof(all_info) - sizeof(all_info.pet_list[0]) * (LITTLE_PET_COUPLE_MAX_SHARE_NUM - all_info.pet_count);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)& all_info, len);
}

void LittlePet::BroadcastTaskStatus(Role *role)
{
	static Protocol::SCLittlePetUsingImg img_info;
	img_info.role_id = role->GetId();
	img_info.using_pet_id = 0;
	memset(img_info.pet_name, 0, sizeof(img_info.pet_name));

	if (m_little_pet_param.using_index >= 0 && m_little_pet_param.using_index < LITTLE_PET_PERSON_MAX_NUM)
	{
		img_info.using_pet_id = m_little_pet_param.pet_list[m_little_pet_param.using_index].pet_id;
		F_STRNCPY(img_info.pet_name, m_little_pet_param.pet_list[m_little_pet_param.using_index].pet_name, sizeof(GameName));
	}

	role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(), (void*)&img_info, sizeof(img_info));
}

void LittlePet::SetLoverId(int lover_id, bool is_calc)
{
	Role * role = this->GetRole();
	if (NULL == role) return;

	if (m_little_pet_param.lover_id != lover_id)
	{
		m_little_pet_param.lover_id = lover_id;
		this->OnDataChange();
	}

	if (is_calc)
	{
		this->SendAllInfo();
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_LITTLE_PET);
	}
}

Role * LittlePet::GetLoverRole()
{
	return World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_little_pet_param.lover_id));
}

int LittlePet::GetRealType(int vutual_type)
{
	int real_attr_type = -1;
	switch (vutual_type)
	{
	case LITTLEPET_VALUE_TYPE_HP:
		{
			real_attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP;
		}
		break;

	case LITTLEPET_VALUE_TYPE_GONGJI:
		{
			real_attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI;
		}
		break;

	case LITTLEPET_VALUE_TYPE_FANGYU:
		{
			real_attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU;
		}
		break;

	case LITTLEPET_VALUE_TYPE_MINGZHONG:
		{
			real_attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG;
		}
		break;

	case LITTLEPET_VALUE_TYPE_SHANBI:
		{
			real_attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI;
		}
		break;

	case LITTLEPET_VALUE_TYPE_BAOJI:
		{
			real_attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI;
		}
		break;

	case LITTLEPET_VALUE_TYPE_KANGBAO:
		{
			real_attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN;
		}
		break;

	default:
		break;
	}

	return real_attr_type;
}

void LittlePet::SetInteractLog(Role *target_role, LittlePetParam &pet_info)
{
	int log_index = 0;
	for (; log_index < LITTLE_PET_PERSON_MAX_LOG_NUM; log_index++)
	{
		if (m_little_pet_param.log_list[log_index].pet_id <= 0) break;
	}

	if (log_index >= LITTLE_PET_PERSON_MAX_LOG_NUM)
	{
		for (int i = 0; i < LITTLE_PET_PERSON_MAX_LOG_NUM - 1; i++)
		{
			m_little_pet_param.log_list[i] = m_little_pet_param.log_list[i + 1];
		}

		LittlePetInteractLog &last_log = m_little_pet_param.log_list[LITTLE_PET_PERSON_MAX_LOG_NUM - 1];
		last_log.uid = target_role->GetUID();
		last_log.pet_id = pet_info.pet_id;
		F_STRNCPY(last_log.pet_name, pet_info.pet_name, sizeof(GameName));
		last_log.timestamp = (unsigned int)EngineAdapter::Instance().Time();
	}
	else
	{
		LittlePetInteractLog &log_tmp = m_little_pet_param.log_list[log_index];
		log_tmp.uid = target_role->GetUID();
		log_tmp.pet_id = pet_info.pet_id;
		F_STRNCPY(log_tmp.pet_name, pet_info.pet_name, sizeof(GameName));
		log_tmp.timestamp = (unsigned int)EngineAdapter::Instance().Time();
	}
}

void LittlePet::GetPetBaseAttr(LittlePetParam &tem_attrs, const LittlePetBaseCfg *base_cfg)
{
	if (base_cfg == nullptr)return;

	tem_attrs.base_attr[LITTLEPET_VALUE_TYPE_HP] = base_cfg->attr_list[LITTLEPET_VALUE_TYPE_HP];
	tem_attrs.base_attr[LITTLEPET_VALUE_TYPE_GONGJI] = base_cfg->attr_list[LITTLEPET_VALUE_TYPE_GONGJI];
	tem_attrs.base_attr[LITTLEPET_VALUE_TYPE_FANGYU] = base_cfg->attr_list[LITTLEPET_VALUE_TYPE_FANGYU];
	tem_attrs.base_attr[LITTLEPET_VALUE_TYPE_MINGZHONG] = base_cfg->attr_list[LITTLEPET_VALUE_TYPE_MINGZHONG];
	tem_attrs.base_attr[LITTLEPET_VALUE_TYPE_SHANBI] = base_cfg->attr_list[LITTLEPET_VALUE_TYPE_SHANBI];
	tem_attrs.base_attr[LITTLEPET_VALUE_TYPE_BAOJI] = base_cfg->attr_list[LITTLEPET_VALUE_TYPE_BAOJI];
	tem_attrs.base_attr[LITTLEPET_VALUE_TYPE_KANGBAO] = base_cfg->attr_list[LITTLEPET_VALUE_TYPE_KANGBAO];

}

bool LittlePet::GetSingleInfo(Protocol::LittlePetSingleInfo &tmp_pet, int info_type, int index)
{
	const LittlePetParam &pet_info = m_little_pet_param.pet_list[index];

	tmp_pet.index = index;
	tmp_pet.id = pet_info.pet_id;
	tmp_pet.info_type = info_type;

	F_STRNCPY(tmp_pet.pet_name, pet_info.pet_name, sizeof(GameName));

	tmp_pet.baoshi_active_time = pet_info.baoshi_active_time;
	tmp_pet.feed_degree = pet_info.feed_degree;
	tmp_pet.interact_times = pet_info.interact_times;
	tmp_pet.feed_level = pet_info.feed_level;
	tmp_pet.reserve_sh = 0;

	tmp_pet.maxhp = pet_info.base_attr[LITTLEPET_VALUE_TYPE_HP];
	tmp_pet.gongji = pet_info.base_attr[LITTLEPET_VALUE_TYPE_GONGJI];
	tmp_pet.fangyu = pet_info.base_attr[LITTLEPET_VALUE_TYPE_FANGYU];
	tmp_pet.mingzhong = pet_info.base_attr[LITTLEPET_VALUE_TYPE_MINGZHONG];
	tmp_pet.shanbi = pet_info.base_attr[LITTLEPET_VALUE_TYPE_SHANBI];
	tmp_pet.baoji = pet_info.base_attr[LITTLEPET_VALUE_TYPE_BAOJI];
	tmp_pet.kangbao = pet_info.base_attr[LITTLEPET_VALUE_TYPE_KANGBAO];

	for (int ponit_type = 0; ponit_type < LITTLEPET_QIANGHUAPOINT_CURRENT_NUM; ++ponit_type)
	{
		Protocol::LittlePetPointValue &tmp_point = tmp_pet.point_list[ponit_type];

		for (int grid_count = 0; grid_count < LITTLEPET_QIANGHUAGRID_MAX_NUM; ++grid_count)
		{
			tmp_point.gridvaluelist[grid_count].attr_value = pet_info.star_list[ponit_type].attr_list[grid_count].attr_value;
			tmp_point.gridvaluelist[grid_count].arrt_type = pet_info.star_list[ponit_type].attr_list[grid_count].arrt_type;
			tmp_point.gridvaluelist[grid_count].grid_index = grid_count;
		}
	}

	for (int equip_index = 0; equip_index < LITTLEPET_EQUIP_INDEX_MAX_NUM; ++equip_index)
	{
		tmp_pet.equipment_llist[equip_index].equipment_id = pet_info.equipment_list[equip_index].equipment_id;
		tmp_pet.equipment_llist[equip_index].level = pet_info.equipment_list[equip_index].level;
	}

	if (pet_info.pet_id <= 0) return false;

	return true;
}

void LittlePet::SendSingleInfo(Role *role, int index, int info_type)
{
	static Protocol::SCLittlePetSingleInfo singel_info;
	Protocol::LittlePetSingleInfo &tmp_pet = singel_info.pet_single;

	this->GetSingleInfo(tmp_pet, info_type, index);

	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&singel_info, sizeof(singel_info));
}

void LittlePet::SendChangeInfo(int index,int point_type, int grid_index)
{
	Role *role = this->GetRole();
	Role *lover_role = this->GetLoverRole();

	const LittlePetParam &pet_info = m_little_pet_param.pet_list[index];

	static Protocol::SCLittlePetChangeInfo change_info;
	change_info.pet_index = index;
	
	change_info.point_type = point_type;
	change_info.grid_index = grid_index;
	change_info.gridvalue.arrt_type = pet_info.star_list[point_type].attr_list[grid_index].arrt_type;
	change_info.gridvalue.attr_value = pet_info.star_list[point_type].attr_list[grid_index].attr_value;

	if (NULL != role)
	{
		change_info.is_self = 1;
		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&change_info, sizeof(change_info));
	}
	if (NULL != lover_role)
	{
		change_info.is_self = 0;
		EngineAdapter::Instance().NetSend(lover_role->GetNetId(), (char*)&change_info, sizeof(change_info));
	}
}

void LittlePet::SendFeedDegree(int index, int value)
{
	Role *role = this->GetRole();
	Role *lover_role = this->GetLoverRole();

	static Protocol::SCLittlePetNotifyInfo sc;
	sc.param_type = Protocol::LITTLE_PET_NOTIFY_INFO_FEED_DEGREE;
	sc.param1 = index;
	sc.param2 = value;
	sc.param4 = m_little_pet_param.pet_list[index].baoshi_active_time;

	if (NULL != role)
	{
		sc.param3 = 1;
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&sc, sizeof(sc));
	}
	if (NULL != lover_role)
	{
		sc.param3 = 0;
		EngineAdapter::Instance().NetSend(lover_role->GetNetId(), (const char*)&sc, sizeof(sc));
	}
}

void LittlePet::SendPetNewName(int index)
{
	if (index < 0 || index >= LITTLE_PET_PERSON_MAX_NUM) return;
	Role *role = this->GetRole();
	Role *lover_role = this->GetLoverRole();

	static Protocol::SCLittlePetRename sc;
	sc.index = index;
	F_STRNCPY(sc.pet_name, m_little_pet_param.pet_list[index].pet_name, sizeof(GameName));

	if (NULL != role)
	{
		sc.info_type = 1;
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&sc, sizeof(sc));
	}
	if (NULL != lover_role)
	{
		sc.info_type = 0;
		EngineAdapter::Instance().NetSend(lover_role->GetNetId(), (const char *)&sc, sizeof(sc));
	}
}

void LittlePet::SendFreeChouTimestamp()
{
	Role *role = this->GetRole();
	if (NULL == role) return;

	static Protocol::SCLittlePetNotifyInfo sc;
	sc.param_type = Protocol::LITTLE_PET_NOTIFY_INFO_FREE_CHOU_TIMESTAMP;
	sc.param1 = m_little_pet_param.last_free_chou_timestamp;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&sc, sizeof(sc));
}

void LittlePet::SendScoreInfo(Role * m_role)
{
	static Protocol::SCLittlePetNotifyInfo sc;
	sc.param_type = Protocol::LITTLE_PET_NOTIFY_INFO_SCORE;
	sc.param1 = this->GetScore();

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&sc, sizeof(sc));
}

void LittlePet::OnLittlePetWalk(Role *role, int is_idle_status)
{
	if (nullptr == role)
	{
		return;
	}

	static Protocol::SCLittlePetWalk msg;
	msg.obj_id = role->GetId();

	if (static_cast<int>(LITTLEPET_ROLE_IDLE_STATUS::NOT_IN_IDLE_STATUS) == is_idle_status)
	{
		msg.pet_id = 0;
	}
	else
	{
		std::vector<int> valid_pet_idx_vec;
		for (int idx = 0; idx < LITTLE_PET_PERSON_MAX_NUM; ++idx)
		{
			if (m_little_pet_param.pet_list[idx].pet_id > 0)
			{
				valid_pet_idx_vec.push_back(idx);
			}
		}

		if (valid_pet_idx_vec.empty())
		{
			return;
		}

		int rand_vec_idx = RandomNum(valid_pet_idx_vec.size());
		int rand_idx = valid_pet_idx_vec[rand_vec_idx];
		if (rand_idx < LITTLE_PET_PERSON_MAX_NUM)
		{
			msg.pet_id = m_little_pet_param.pet_list[rand_idx].pet_id;
			memcpy(msg.pet_name, m_little_pet_param.pet_list[rand_idx].pet_name, sizeof(m_little_pet_param.pet_list[rand_idx].pet_name));
		}
	}

	role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(), (void*)&msg, sizeof(msg));
}

int LittlePet::GetPetId(int index)
{
	if (index >= 0 && index < LITTLE_PET_PERSON_MAX_NUM)
	{
		return m_little_pet_param.pet_list[index].pet_id;
	}

	return 0;
}

void LittlePet::RecycleReturnStuffList(int cur_feed_level, ItemID recycle_itemid, std::vector<ItemConfigData> &item_list)
{
	if (cur_feed_level <= 0 || recycle_itemid <= 0 || NULL == ITEMPOOL->GetItem(recycle_itemid))
	{
		return;
	}

	Role *role = this->GetRole();
	if (nullptr == role) return;

	// 获取物列表
//	int list_num = 0;

	// 返还当前喂养等级前的所有消耗材料
	for (int level = 0; level < cur_feed_level; level++)
	{
		const LittlePetBaseCfg *base_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetBaseCfgByItemID(recycle_itemid);
		if (nullptr == base_cfg)
		{
			continue;
		}

		int pet_quality_type = base_cfg->quality_type;
		if (pet_quality_type < 0 || pet_quality_type >= LITTLEPET_MAX_QUALITY_TYPE)
		{
			continue;
		}

		const LittlePetFeedCfg *feed_level_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetFeedCfg(pet_quality_type, level);
		if (nullptr == feed_level_cfg)
		{
			continue;
		}

		ItemConfigData item;

		item.item_id = feed_level_cfg->stuff_id_list[pet_quality_type];
		item.num  = feed_level_cfg->stuff_num_list[pet_quality_type];
		item.is_bind  = (0 != feed_level_cfg->stuff_bind_list[pet_quality_type]);

		item_list.push_back(item);

// 		if (!ItemExtern::PutInItemConfigList(ItemNamespace::MAX_KNAPSACK_GRID_NUM, item_list, &list_num,
// 			stuff_itemid, stuff_num, stuff_bind))
// 		{
// 			break;
// 		}
	}

	// 发放奖励
// 	for (int i = 0; i < list_num && i <= ItemNamespace::MAX_KNAPSACK_GRID_NUM - MAX_ATTACHMENT_ITEM_NUM; i += MAX_ATTACHMENT_ITEM_NUM)
// 	{
// 		role->GetKnapsack()->PutListOrMail(item_list + i, PUT_REASON_RECYCLE_LITTLEPET_RETURN_FEED_STUFF);
// 	}
}
