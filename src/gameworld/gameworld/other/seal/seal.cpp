//#include "stdafx.h"

#include "seal.hpp"
#include "other/seal/sealconfig.hpp"
#include "gamelog.h"
#include "gameworld/gameworld/gstr.h"
#include "servercommon/errornum.h"
#include "config/logicconfigmanager.hpp"
#include <algorithm>
//#include "gameworld/scene/speciallogic/worldspecial/specialcrossshengyinfb.hpp"
#include "servercommon/noticenum.h"
#include "other/capability/capability.hpp"
#include "other/event/eventhandler.hpp"
#include "world.h"
#include "scene/scene.h"
#include "obj/character/attribute.hpp"
#include "item/knapsack.h"
#include "gamelog.h"

Seal::Seal() : m_role(NULL), m_empty_grids(0)
{
}

Seal::~Seal()
{
}

void Seal::Init(Role *role, const SealParam &param)
{
	m_role = role;
	m_param = param;

	//this->SortBackpack();
	this->CheckActivateSealSystem();
}

void Seal::GetInitParam(SealParam *param)
{
	*param = m_param;
}

void Seal::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		CharIntAttrs total_base_attrs_add; 				// 基础总属性
		CharIntAttrs total_strength_attrs_add; 			// 强化总属性

		long long total_per_base_attr_jiacheng = 0;										// 基础属性总加成
		long long total_per_strength_attr_jiacheng = 0;									// 强化属性总加成

		int same_order_num_list[SEAL_SUIT_MAX_TYPE + 1];								// 相同套装数
		memset(same_order_num_list, 0, sizeof(same_order_num_list));

		int total_strength_level = 0;													// 强化总等级

		for (int i = 0; i < SEAL_SLOT_MAX_NUM; ++i)
		{
			SealItem *seal_item = &m_param.slot_seal_list[i];
			if (NULL == seal_item)
			{
				continue;
			}

			if (!seal_item->IsEmpty())
			{
				int equip_level = m_param.slot_seal_list[i].level;
				total_strength_level += equip_level;

				// 基础属性
				const SealInitialAttrCfg *init_attr_cfg = LOGIC_CONFIG->GetSealConfig().GetInitialAttrCfg(seal_item->slot_index, seal_item->order, seal_item->color);
				const SealItemCfg *attr_cfg = LOGIC_CONFIG->GetSealConfig().GetSealItemCfg(seal_item->slot_index);
				if (NULL != init_attr_cfg)
				{
					total_base_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += init_attr_cfg->gongji;
					total_base_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += init_attr_cfg->maxhp;
					total_base_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += init_attr_cfg->fangyu;
					total_base_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += init_attr_cfg->per_baoji;
					total_base_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += init_attr_cfg->baoji;
					total_base_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += init_attr_cfg->mingzhong;
					total_base_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += init_attr_cfg->shanbi;
					total_base_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += init_attr_cfg->jianren;

					// 保存基础属性总加成
					total_per_base_attr_jiacheng += init_attr_cfg->per_base_attr_jiacheng;
				}

				if (NULL != attr_cfg)
				{
					total_strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += attr_cfg->gongji * equip_level;
					total_strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += attr_cfg->maxhp * equip_level;
					total_strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += attr_cfg->fangyu * equip_level;
					total_strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += attr_cfg->per_baoji * equip_level;
					total_strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += attr_cfg->baoji * equip_level;
				}

				const SealSuitPartCfg *suit_part_cfg = LOGIC_CONFIG->GetSealConfig().GetSuitPartCfg(seal_item->order, seal_item->slot_index);
				if (NULL != suit_part_cfg)
				{
					++same_order_num_list[suit_part_cfg->suit_type];
				}
			}
		}

		m_attrs_add.Add(total_base_attrs_add.m_attrs);
		m_attrs_add.Add(total_strength_attrs_add.m_attrs);

		// 强化套装属性
		{
			const SealStrengthCfg *attr_cfg = LOGIC_CONFIG->GetSealConfig().GetStrengthCfg(total_strength_level);
			if (NULL != attr_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += attr_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += attr_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += attr_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += attr_cfg->per_baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += attr_cfg->baoji;
			}
		}

		// 套装属性
		for (int suit_type = SEAL_SUIT_MIN_TYPE; suit_type < SEAL_SUIT_MAX_TYPE + 1; ++suit_type)
		{
			int same_order_num = same_order_num_list[suit_type];

			for (; same_order_num >= SEAL_SUIT_SAME_ORDER_TYPE_ONE; same_order_num --)
			{
				const SealSuitCfg *attr_cfg = LOGIC_CONFIG->GetSealConfig().GetSuitCfg(suit_type, same_order_num);
				if (NULL != attr_cfg)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += attr_cfg->gongji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += attr_cfg->maxhp;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += attr_cfg->fangyu;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += attr_cfg->per_baoji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += attr_cfg->baoji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += attr_cfg->mingzhong;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += attr_cfg->shanbi;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += attr_cfg->jianren;

					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER] += attr_cfg->skill_zengshang_per;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += attr_cfg->per_jingzhun;
// 					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += attr_cfg->per_pofang;
// 					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += attr_cfg->per_mianshang;

					// 下面特殊属性用乘法
					m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, attr_cfg->per_mianshang);
					m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, attr_cfg->per_pofang);

					m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<float>(attr_cfg->per_gongji * ROLE_ATTR_PER);	//套装全身攻击加成
					m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<float>(attr_cfg->per_maxhp * ROLE_ATTR_PER);		//套装全身气血加成
					m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<float>(attr_cfg->per_fangyu * ROLE_ATTR_PER);	//套装全身防御加成

					// 保存基础属性总加成
					total_per_base_attr_jiacheng += attr_cfg->per_base_attr_jiacheng;

					// 保存强化属性总加成
					total_per_strength_attr_jiacheng += attr_cfg->per_strength_attr_jiacheng;
				}
			}
		}

		// 圣魂属性
		for (int soul_type = 0; soul_type < SEAL_SOUL_MAX_TYPE; ++soul_type)
		{
			const SealSoulCfg *attr_cfg = LOGIC_CONFIG->GetSealConfig().GetSoulCfg(soul_type + 1);
			if (NULL != attr_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += attr_cfg->gongji * m_param.soul_list[soul_type];
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += attr_cfg->maxhp * m_param.soul_list[soul_type];
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += attr_cfg->fangyu * m_param.soul_list[soul_type];
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += attr_cfg->per_baoji * m_param.soul_list[soul_type];
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += attr_cfg->baoji * m_param.soul_list[soul_type];

				// 保存基础属性总加成
				total_per_base_attr_jiacheng += attr_cfg->per_base_attr_jiacheng * m_param.soul_list[soul_type];

				// 保存强化属性总加成
				total_per_strength_attr_jiacheng += attr_cfg->per_strength_attr_jiacheng * m_param.soul_list[soul_type];
			}
		}

		// 基础属性加成
		{
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] +=
				static_cast<Attribute>(total_base_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (total_per_base_attr_jiacheng * ROLE_ATTR_PER));

			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] +=
				static_cast<Attribute>(total_base_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (total_per_base_attr_jiacheng * ROLE_ATTR_PER));
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] +=
				static_cast<Attribute>(total_base_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (total_per_base_attr_jiacheng * ROLE_ATTR_PER));

			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] +=
				static_cast<Attribute>(total_base_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (total_per_base_attr_jiacheng * ROLE_ATTR_PER));

			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] +=
				static_cast<Attribute>(total_base_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (total_per_base_attr_jiacheng * ROLE_ATTR_PER));

			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] +=
				static_cast<Attribute>(total_base_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (total_per_base_attr_jiacheng * ROLE_ATTR_PER));

			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] +=
				static_cast<Attribute>(total_base_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (total_per_base_attr_jiacheng * ROLE_ATTR_PER));

		}

		// 强化属性加成
		{
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] +=
				static_cast<Attribute>(total_strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (total_per_strength_attr_jiacheng * ROLE_ATTR_PER));

			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] +=
				static_cast<Attribute>(total_strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (total_per_strength_attr_jiacheng * ROLE_ATTR_PER));

			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] +=
				static_cast<Attribute>(total_strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (total_per_strength_attr_jiacheng * ROLE_ATTR_PER));

			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] +=
				static_cast<Attribute>(total_strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (total_per_strength_attr_jiacheng * ROLE_ATTR_PER));

			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] +=
				static_cast<Attribute>(total_strength_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] * (total_per_strength_attr_jiacheng * ROLE_ATTR_PER));
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_SEAL, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
	base_add.AddPercent(m_attrs_add.m_percent_attrs);
}

void Seal::SealOpera(int opera_type, int param1, int param2)
{
	if (opera_type < 0 || opera_type >= Protocol::SEAL_OPERA_TYPE_MAX)
	{
		return;
	}

	switch (opera_type)
	{
	case Protocol::SEAL_OPERA_TYPE_ALL_INFO:
	{
		this->SendAllInfo();
	}
	break;

	case Protocol::SEAL_OPERA_TYPE_PUT_ON:
	{
		//this->PutOn(param1);
	}
	break;

	case Protocol::SEAL_OPERA_TYPE_UPLEVLE:
	{
		this->UpLevel(param1);
	}
	break;

	case Protocol::SEAL_OPERA_TYPE_RECYCLE:
	{
		this->Recycle(1, &param2);
	}
	break;

	case Protocol::SEAL_OPERA_TYPE_USE_SOUL:
	{
		this->OnUseSoul(param1, param2);
	}
	break;
	}
}

void Seal::SendAllInfo()
{
	this->SendBaseInfo();
	this->SendSlotInfo();
	//this->SendBackpackInfo();
}

bool Seal::AddSealByItemId(ItemID item_id, int num, bool is_bind, int level)
{
// 	if (!this->CheckHasSpace(num))
// 	{
// 		gstr::SendError(m_role, "seal_backpack_no_enough_space");
// 		return false;
// 	}

	if (num > 1)
	{
		gstr::SendError(m_role, "seal_equip_one");
		return false;
	}

	const SealRealIdCfg *seal_cfg = LOGIC_CONFIG->GetSealConfig().GetRealIdCfg(item_id);
	if (NULL == seal_cfg)
	{
		return false;
	}

	SealItem seal_item = m_param.slot_seal_list[seal_cfg->slot_index - 1];

	if (!m_param.IsActivateSeal(seal_cfg->slot_index - 1))
	{
		gstr::SendError(m_role, "seal_equip_fail");
		return false;
	}

	if (seal_item.level >0 && seal_item.item_id <= 0)
	{
		gstr::SendError(m_role, "seal_equip_fail");
		return false;
	}
	
	if (!seal_item.IsEmpty())
	{
		Knapsack * knapsack = m_role->GetKnapsack();
		if (nullptr == knapsack || knapsack->IsLock())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
			return false;
		}
		
		if (!knapsack->HaveSpace())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return false;
		}

		ItemConfigData put_item;
		put_item.item_id = seal_item.item_id;
		put_item.num = 1;
		put_item.is_bind =  (1 == seal_item.is_bind) ? true : false;

		if (!knapsack->Put(put_item, PUT_REASON_NO_NOTICE))
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_EQUIP_TAKEOFF_ERROR);
			return false;
		}
	}
	else
	{
		m_param.slot_seal_list[seal_cfg->slot_index - 1].level = level;
	}

	m_param.slot_seal_list[seal_cfg->slot_index-1].slot_index = seal_cfg->slot_index;
	m_param.slot_seal_list[seal_cfg->slot_index-1].order = seal_cfg->order;
	m_param.slot_seal_list[seal_cfg->slot_index-1].color = seal_cfg->color;
	//m_param.slot_seal_list[seal_cfg->slot_index - 1].level = level;
	m_param.slot_seal_list[seal_cfg->slot_index-1].item_id = item_id;
	m_param.slot_seal_list[seal_cfg->slot_index - 1].is_bind = is_bind ? 1 : 0;

	gstr::SendNotice(m_role, "seal_equip_succ");
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SEAL);

	EventHandler::Instance().OnSealInlay(m_role);

	gamelog::g_log_seal.printf(LL_INFO,"Seal::AddSealByItemId user[%d, %s] seal_id[%d],old_seal_id[%d]",
		m_role->GetUID(), m_role->GetName(), item_id, seal_item.item_id);

	this->SendSlotInfo();
	return true;
}

bool Seal::DecSealByItemId(int item_id, const char *reason, int num, bool is_send)
{
// 	if (num <= 0 || NULL == reason)
// 	{
// 		return false;
// 	}
// 
// 	const SealRealIdCfg *seal_cfg = LOGIC_CONFIG->GetSealConfig().GetRealIdCfg(item_id);
// 	if (NULL == seal_cfg)
// 	{
// 		return false;
// 	}
// 
// 	gamelog::g_log_seal.buff_printf("Seal::DecSealByItemId user[%d, %s] item_id[%d] reason[%s] consum_count[%d] consum_list[",
// 		m_role->GetUID(), m_role->GetName(), item_id, reason, num);
// 
// 	int left_num = num;
// 	for (int i = 0; i < SEAL_BACKPACK_MAX_GRIDS; i++)
// 	{
// 		if (m_param.backpack_seal_list[i].item_id == item_id)
// 		{
// 			m_param.backpack_seal_list[i].Reset();
// 
// 			gamelog::g_log_seal.buff_printf("%d, ", i);
// 
// 			--left_num;
// 		}
// 
// 		if (left_num == 0)
// 		{
// 			break;
// 		}
// 	}
// 
// 	gamelog::g_log_seal.buff_printf("left_num %d", left_num);
// 	gamelog::g_log_seal.commit_buff(LL_INFO);
// 
// 	if (is_send)
// 	{
// 		this->SendBackpackInfo();
// 	}
// 
 	return true;
}

bool Seal::AddScore(int score, const char *reason, bool notify_change)
{
	if (NULL == reason) return false;

	long long target_score = (long long)m_param.hun_score + score;
	if (target_score < 0 || target_score >= INT_MAX)
	{
		return false;
	}

	m_param.hun_score = static_cast<int>(target_score);

	if (notify_change) this->SendBaseInfo();

	gamelog::g_log_seal.printf(LL_INFO, "Seal::AddScore user[%d, %s] add_hun_score[%d] hun_score[%d] reason[%s]",
		m_role->GetUID(), m_role->GetName(), score, m_param.hun_score, reason);

	return true;
}

void Seal::GetShengYinFBAttr(ShengYinFBAttr &attr)
{
	attr.Reset();

	// 相同套装数
	int same_order_num_list[SEAL_SUIT_MAX_TYPE + 1];
	memset(same_order_num_list, 0, sizeof(same_order_num_list));

	// 强化总等级
	int total_strength_level = 0;

	for (int i = 0; i < SEAL_SLOT_MAX_NUM; ++i)
	{
		SealItem *seal_item = &m_param.slot_seal_list[i];
		if (NULL == seal_item)
		{
			continue;
		}

		if (!seal_item->IsEmpty())
		{
			int equip_level = m_param.slot_seal_list[i].level;
			total_strength_level += equip_level;

			// 基础属性
			const SealInitialAttrCfg *init_attr_cfg = LOGIC_CONFIG->GetSealConfig().GetInitialAttrCfg(seal_item->slot_index, seal_item->order, seal_item->color);
			const SealItemCfg *attr_cfg = LOGIC_CONFIG->GetSealConfig().GetSealItemCfg(seal_item->slot_index);
			if (NULL != init_attr_cfg && NULL != attr_cfg)
			{
				attr.yaolichuantou += init_attr_cfg->yaolichuantou + attr_cfg->yaolichuantou * equip_level;
				attr.yaolidikang += init_attr_cfg->yaolidikang + attr_cfg->yaolidikang * equip_level;
				attr.molichuantou += init_attr_cfg->molichuantou + attr_cfg->molichuantou * equip_level;
				attr.molidikang += init_attr_cfg->molidikang + attr_cfg->molidikang * equip_level;
				attr.shenlichuantou += init_attr_cfg->shenlichuantou + attr_cfg->shenlichuantou * equip_level;
				attr.shenlidikang += init_attr_cfg->shenlidikang + attr_cfg->shenlidikang * equip_level;
			}
		}

		const SealSuitPartCfg *suit_part_cfg = LOGIC_CONFIG->GetSealConfig().GetSuitPartCfg(seal_item->order, seal_item->slot_index);
		if (NULL != suit_part_cfg)
		{
			++same_order_num_list[suit_part_cfg->suit_type];
		}
	}

	// 强化属性
	{
		const SealStrengthCfg *attr_cfg = LOGIC_CONFIG->GetSealConfig().GetStrengthCfg(total_strength_level);
		if (NULL != attr_cfg)
		{
			attr.yaolichuantou += attr_cfg->yaolichuantou;
			attr.yaolidikang += attr_cfg->yaolidikang;
			attr.molichuantou += attr_cfg->molichuantou;
			attr.molidikang += attr_cfg->molidikang;
			attr.shenlichuantou += attr_cfg->shenlichuantou;
			attr.shenlidikang += attr_cfg->shenlidikang;
		}
	}

	// 套装属性
	for (int suit_type = SEAL_SUIT_MIN_TYPE; suit_type < SEAL_SUIT_MAX_TYPE + 1; ++suit_type)
	{
		int same_order_num = same_order_num_list[suit_type];
		if (same_order_num >= SEAL_SUIT_SAME_ORDER_TYPE_TWO && same_order_num < SEAL_SUIT_SAME_ORDER_TYPE_THREE)
		{
			same_order_num = SEAL_SUIT_SAME_ORDER_TYPE_TWO;	// [2,3)
		}
		else if (same_order_num >= SEAL_SUIT_SAME_ORDER_TYPE_THREE && same_order_num < SEAL_SUIT_SAME_ORDER_TYPE_FOUR)
		{
			same_order_num = SEAL_SUIT_SAME_ORDER_TYPE_THREE;	// [3,4)
		}
		else if (same_order_num >= SEAL_SUIT_SAME_ORDER_TYPE_FOUR && same_order_num < SEAL_SUIT_SAME_ORDER_TYPE_SIX)
		{
			same_order_num = SEAL_SUIT_SAME_ORDER_TYPE_FOUR;	// [4,6)
		}
		else if (same_order_num >= SEAL_SUIT_SAME_ORDER_TYPE_SIX && same_order_num < SEAL_SUIT_SAME_ORDER_TYPE_EIGHT)
		{
			same_order_num = SEAL_SUIT_SAME_ORDER_TYPE_SIX;	// [6,8)
		}
		else if (same_order_num >= SEAL_SUIT_SAME_ORDER_TYPE_EIGHT && same_order_num < SEAL_SUIT_SAME_ORDER_TYPE_TEN)
		{
			same_order_num = SEAL_SUIT_SAME_ORDER_TYPE_EIGHT;	// [8,10)
		}
		else if (same_order_num >= SEAL_SUIT_SAME_ORDER_TYPE_TEN)
		{
			same_order_num = SEAL_SUIT_SAME_ORDER_TYPE_TEN;	// [10, )
		}
		else
		{
			continue;
		}

		for (; same_order_num >= SEAL_SUIT_SAME_ORDER_TYPE_TWO;)
		{
			const SealSuitCfg *attr_cfg = LOGIC_CONFIG->GetSealConfig().GetSuitCfg(suit_type, same_order_num);
			if (NULL != attr_cfg)
			{
				attr.yaolichuantou += attr_cfg->yaolichuantou;
				attr.yaolidikang += attr_cfg->yaolidikang;
				attr.molichuantou += attr_cfg->molichuantou;
				attr.molidikang += attr_cfg->molidikang;
				attr.shenlichuantou += attr_cfg->shenlichuantou;
				attr.shenlidikang += attr_cfg->shenlidikang;
			}

			if (same_order_num == SEAL_SUIT_SAME_ORDER_TYPE_TWO)
			{
				break;
			}
			else if (same_order_num == SEAL_SUIT_SAME_ORDER_TYPE_THREE)
			{
				same_order_num = SEAL_SUIT_SAME_ORDER_TYPE_TWO;
			}
			else if (same_order_num == SEAL_SUIT_SAME_ORDER_TYPE_FOUR)
			{
				same_order_num = SEAL_SUIT_SAME_ORDER_TYPE_THREE;
			}
			else if (same_order_num == SEAL_SUIT_SAME_ORDER_TYPE_SIX)
			{
				same_order_num = SEAL_SUIT_SAME_ORDER_TYPE_FOUR;
			}
			else if (same_order_num == SEAL_SUIT_SAME_ORDER_TYPE_EIGHT)
			{
				same_order_num = SEAL_SUIT_SAME_ORDER_TYPE_SIX;
			}
			else if (same_order_num >= SEAL_SUIT_SAME_ORDER_TYPE_TEN)
			{
				same_order_num = SEAL_SUIT_SAME_ORDER_TYPE_EIGHT;
			}
		}
	}

	return;
}

void Seal::CheckActivateSealSystem()
{
	int open_slot_index = LOGIC_CONFIG->GetSealConfig().GetOpenSlotIndex(m_role->GetLevel());
	for (int slot_index = 0; slot_index < open_slot_index; ++slot_index)
	{
		if (!m_param.IsActivateSeal(slot_index))
		{
			m_param.ActivateSeal(slot_index);
		}
	}
}

int Seal::GetInlaySealCount(int order)
{
	int count = 0;

	for (unsigned int i = 0; i < static_array_size(m_param.slot_seal_list); i ++)
	{
		if (m_param.IsActivateSeal(i) && m_param.slot_seal_list[i].order >= order)
		{
			count++;
		}
	}

	return count;
}

bool Seal::ComposeSeal(int product_num, const ProductConfig *product_cfg)
{
	if (NULL == product_cfg || product_num <= 0)//!this->CheckHasSpace(product_num))
	{
		return false;
	}

	const SealRealIdCfg *seal_cfg = LOGIC_CONFIG->GetSealConfig().GetRealIdCfg(product_cfg->m_product_id);
	if (NULL == seal_cfg) return false;

	short stuff_count = 0;
	short virtual_stuff_count = 0;
	ItemExtern::ItemConsumeConfig virtual_stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];            // 虚拟物品材料
	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];                    // 实体物品材料
	UNSTD_STATIC_CHECK(MAX_COMPOSE_STUFF_TYPE <= ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM);
	static ItemExtern::ItemList item_list;

	bool is_virtual = false;
	for (int i = 0; i < MAX_COMPOSE_STUFF_TYPE; ++i)
	{
		if (ItemBase::INVALID_ITEM_ID != product_cfg->m_stuff_id_list[i])
		{
			if (product_cfg->m_stuff_count_list[i] <= 0) return false;

			int need_total_count = product_num * product_cfg->m_stuff_count_list[i];
			int own_count = 0;
			if (NULL != LOGIC_CONFIG->GetSealConfig().GetRealIdCfg(product_cfg->m_stuff_id_list[i]))
			{
				//own_count = this->GetSealItemNumInVirtualBagById(product_cfg->m_stuff_id_list[i]);
				is_virtual = true;
			}
			else
			{
				item_list.Reset();

				if (!m_role->GetKnapsack()->GetListOfItem(product_cfg->m_stuff_id_list[i], ItemNamespace::MAX_KNAPSACK_GRID_NUM,
					item_list.bind_index_list, item_list.bind_num_list, &(item_list.bind_count),
					item_list.nobind_index_list, item_list.nobind_num_list, &(item_list.nobind_count)))
				{
					return false;
				}

				own_count = item_list.GetTotalCount();
				is_virtual = false;
			}
			if (own_count < need_total_count)
			{
				m_role->NoticeNum(errornum::EN_ITEM_FUNC_ITEM_COMPOSE_NOT_ENOUGH);
				return false;
			}

			if (is_virtual)
			{
				virtual_stuff_list[virtual_stuff_count].item_id = product_cfg->m_stuff_id_list[i];
				virtual_stuff_list[virtual_stuff_count].num = need_total_count;
				virtual_stuff_count++;
			}
			else
			{
				stuff_list[stuff_count].item_id = product_cfg->m_stuff_id_list[i];
				stuff_list[stuff_count].num = need_total_count;
				stuff_count++;
			}
		}
	}

	// 检查铜钱
	if (product_cfg->m_coin > 0 && !m_role->GetKnapsack()->GetMoney()->AllCoinMoreThan(product_num * product_cfg->m_coin))
	{
		m_role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
		return false;
	}

	// 检查女娲石
	if (product_cfg->m_nv_wa_shi > 0 && m_role->GetNvWaShi() < product_num * product_cfg->m_nv_wa_shi)
	{
		m_role->NoticeNum(errornum::EN_ROLE_NV_WA_SHI_NOT_ENOUGH);
		return false;
	}

	// 消耗虚拟背包物品
	for (int i = 0; i < virtual_stuff_count; i++)
	{
		if (virtual_stuff_list[i].item_id <= 0)
		{
			return false;
		}

		//this->DecSealByItemId(virtual_stuff_list[i].item_id, "Seal::ComposeSeal", virtual_stuff_list[i].num, false);
	}

	// 消耗背包中物品
	bool is_bind = false;
	{
		// 合成是绑定优先
		ItemExtern::ItemConsumeList consume_list;
		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list)) return false;

		if (product_cfg->m_coin > 0)
		{
			if (!m_role->GetKnapsack()->GetMoney()->UseAllCoin(product_num * product_cfg->m_coin, "ComposeSeal")) return false;
		}

		if (product_cfg->m_nv_wa_shi > 0)
		{
			if (!m_role->AddNvWaShi(-1 * product_num * product_cfg->m_nv_wa_shi, "ComposeSeal")) return false;
		}

		if (!ItemExtern::ConsumeItem(m_role, consume_list, "ComposeSeal"))
		{
			return false;
		}

		is_bind = consume_list.bind_item_num > 0;
	}

	//this->SortBackpack();
	this->AddSealByItemId(seal_cfg->seal_id, product_num, is_bind, 0);

	m_role->NoticeNum(noticenum::NT_EQUIP_COMPOSE_SUCC);
	return true;
}

void Seal::SendBaseInfo() const
{
	static Protocol::SCSealBaseInfo sbi;
	sbi.hun_score = m_param.hun_score;
	memcpy(sbi.soul_list, m_param.soul_list, sizeof(sbi.soul_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&sbi, sizeof(sbi));
}

void Seal::SendSlotInfo() const
{
	static Protocol::SCSealSlotInfo ssi;
	memcpy(ssi.grid_list, m_param.slot_seal_list, sizeof(ssi.grid_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&ssi, sizeof(ssi));
}

/*
void Seal::SendBackpackInfo()
{
	this->SortBackpack();

	static Protocol::SCSealBackpackInfo sbi;
	sbi.grid_num = 0;

	for (int index = 0; index < SEAL_BACKPACK_MAX_GRIDS; ++index)
	{
		SealItem *seal_item = &m_param.backpack_seal_list[index];
		if (NULL == seal_item || seal_item->IsEmpty()) // 有效ID才下发（0也下发，因为消耗后也要通知一下）
		{
			continue;
		}

		sbi.grid_list[sbi.grid_num].index = index;
		sbi.grid_list[sbi.grid_num].slot_index = seal_item->slot_index;
		sbi.grid_list[sbi.grid_num].order = seal_item->order;
		sbi.grid_list[sbi.grid_num].level = seal_item->level;
		sbi.grid_list[sbi.grid_num].item_id = seal_item->item_id;
		++sbi.grid_num;
	}

	int send_len = int(sizeof(sbi) - sizeof(sbi.grid_list[0]) * (SEAL_BACKPACK_MAX_GRIDS - sbi.grid_num));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&sbi, send_len);
}

void Seal::PutOn(short backpack_index)
{
	if (backpack_index < 0 || backpack_index >= SEAL_BACKPACK_MAX_GRIDS)
	{
		return;
	}

	int slot_index = m_param.backpack_seal_list[backpack_index].slot_index;
	if (slot_index <= SEAL_TYPE_JINGHUA || slot_index > SEAL_SLOT_MAX_NUM)
	{
		return;
	}

	if (!m_param.IsActivateSeal(slot_index - 1))
	{
		gstr::SendError(m_role, "seal_equip_fail");
		return;
	}

	SealItem &bag_item = m_param.backpack_seal_list[backpack_index];
	SealItem &grid_seal = m_param.slot_seal_list[slot_index - 1];

	if (bag_item.IsEmpty())
	{
		return;
	}

	const SealItemCfg *seal_cfg = LOGIC_CONFIG->GetSealConfig().GetSealItemCfg(bag_item.slot_index);
	if (NULL == seal_cfg)
	{
		return;
	}

	if (seal_cfg->slot_index != slot_index)
	{
		gstr::SendError(m_role, "seal_equip_fail");
		return;
	}

	gamelog::g_log_seal.printf(LL_INFO, "Seal::PutOn user[%d, %s], bag_item[slot_index:%d, order:%d, color:%d, level:%d, item_id:%d], grid_seal[slot_index:%d, order:%d, color:%d, level:%d, item_id:%d]",
		m_role->GetUID(), m_role->GetName(), bag_item.slot_index, bag_item.order, bag_item.color, bag_item.level, bag_item.item_id,
		grid_seal.slot_index, grid_seal.order, grid_seal.color, grid_seal.level, grid_seal.item_id);

	// 圣印孔初始等级置为0
	if (grid_seal.level < 0)
	{
		grid_seal.level = 0;
	}

	// 装备槽跟背包格子是一样的，直接交换位置就行
	SealItem tmp_item;
	tmp_item.Reset();
	tmp_item = grid_seal;
	grid_seal = bag_item;
	bag_item = tmp_item;

	// 等级继承装备槽的等级
	short bag_level = -1;
	short grid_level = -1;
	bag_level = bag_item.level;
	grid_level = grid_seal.level;
	bag_item.level = grid_level;
	grid_seal.level = bag_level;

	if (bag_item.IsEmpty())
	{
		++m_empty_grids;
	}

	gstr::SendNotice(m_role, "seal_equip_succ");
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SEAL);

	//if (m_role->GetScene() && Scene::SCENE_TYPE_CROSS_SHENGYIN_FB == m_role->GetScene()->GetSceneType())
	//{
	//	SpecialCrossShengYinFb* special_syfb = (SpecialCrossShengYinFb*)m_role->GetScene()->GetSpecialLogic();
	//	if (special_syfb)
	//	{
	//		special_syfb->OnPutOnInShengYinFB(m_role);
	//	}
	//}

	this->SendSlotInfo();
	//this->SendBackpackInfo();
}
*/

void Seal::UpLevel(short slot_index)
{
	SealItem *seal_item = NULL;
	if (slot_index <= 0 || slot_index > SEAL_SLOT_MAX_NUM)
	{
		return;
	}

	seal_item = &m_param.slot_seal_list[slot_index - 1];

	if (NULL == seal_item || seal_item->IsEmpty())
	{
		return;
	}

	const int hun_score = LOGIC_CONFIG->GetSealConfig().GetSealScore(seal_item->level);
	const int next_hun_score = LOGIC_CONFIG->GetSealConfig().GetSealScore(seal_item->level + 1);
	if (hun_score <= 0 || next_hun_score <= 0)
	{
		gstr::SendError(m_role, "seal_level_fail");
		return;
	}

	int old_score = m_param.hun_score;
	int old_level = seal_item->level;
	if (!this->AddScore(-hun_score, "Seal::UpLevel"))
	{
		gstr::SendError(m_role, "seal_hun_score_no_enough");
		return;
	}
	++seal_item->level;

	this->SendSlotInfo();

	gstr::SendNotice(m_role, "seal_level_succ");
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SEAL);

	gamelog::g_log_seal.printf(LL_INFO, "Seal::UpLevel user[%d, %s] index[%d] score(%d -> %d) level(%d -> %d)",
		m_role->GetUID(), m_role->GetName(), slot_index, old_score, m_param.hun_score, old_level, seal_item->level);
}

void Seal::Recycle(int recycle_num, int *recycle_backpack_index_list)
{
	if (recycle_backpack_index_list == nullptr)
	{
		return;
	}

	gamelog::g_log_seal.buff_printf("Seal::Recycle user[%d, %s] hunlist[", m_role->GetUID(), m_role->GetName());
	int total_add_score = 0;

	Knapsack * knapsack = m_role->GetKnapsack();
	if (nullptr == knapsack || knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	for (int i = 0; i < recycle_num; ++i)
	{

		int backpack_index = recycle_backpack_index_list[i];
		if (backpack_index < 0 || backpack_index >= ItemNamespace::MAX_KNAPSACK_GRID_NUM)
		{
			continue;
		}

		ItemGridData item_grid;
		if (!knapsack->GetItemGridData(backpack_index, &item_grid))
		{
			continue;
		}

		const SealRealIdCfg *seal_cfg = LOGIC_CONFIG->GetSealConfig().GetRealIdCfg(item_grid.item_id);

		if (nullptr == seal_cfg)
		{
			continue;
		}

		// 分解精华和分解圣印，积分获得不同
		int score = 0;
		if (SEAL_TYPE_JINGHUA == seal_cfg->slot_index)
		{
			const int jinghua_hun_score = LOGIC_CONFIG->GetSealConfig().GetInitialJinghuaScore(seal_cfg->color);
			if (jinghua_hun_score < 0) continue;

			score = jinghua_hun_score * item_grid.num;
		}
		else
		{
			const int init_score = LOGIC_CONFIG->GetSealConfig().GetInitialHunScore(seal_cfg->color);
			//const int hun_score = LOGIC_CONFIG->GetSealConfig().GetSealScore(seal_item.level - 1);
			if (init_score <= 0) continue;

			//if (hun_score <= 0)
			//{
			//	score = init_score;
			//}
			//else
			//{
			//	score = init_score + hun_score;
			//}

			score = init_score * item_grid.num;
		}

		if (!knapsack->Discard(backpack_index, item_grid.num, DISCARD_METHOD_RECYCL))
		{
			continue;
		}

		if (!this->AddScore(score, "Seal::Recycle", false))
		{
			continue;
		}

		total_add_score += score;

		gamelog::g_log_seal.buff_printf("(index:%d,id:%d,num:%d), ", seal_cfg->slot_index, seal_cfg->seal_id,item_grid.num);
	}

	gstr::SendNotice(m_role, "seal_recycle_succ");
	this->SendBaseInfo();
//	this->SendBackpackInfo();

	gamelog::g_log_seal.buff_printf("] total_add_score[%d]", total_add_score);
	gamelog::g_log_seal.commit_buff(LL_INFO);
}

void Seal::OnUseSoul(short soul_type, int use_count)
{
	if (soul_type <= 0 || soul_type > SEAL_SOUL_MAX_TYPE || use_count <= 0)
	{
		return;
	}

	const int use_limit_num = LOGIC_CONFIG->GetSealConfig().GetSoulUseLimit(m_role->GetLevel());
	if (use_limit_num <= 0)
	{
		return;
	}

	if (m_param.soul_list[soul_type - 1] + use_count > use_limit_num)
	{
		gstr::SendError(m_role, "seal_soul_use_limit_num");
		return;
	}

	const SealSoulCfg *soul_cfg = LOGIC_CONFIG->GetSealConfig().GetSoulCfg(soul_type);
	if (NULL == soul_cfg)
	{
		return;
	}

	if (!m_role->GetKnapsack()->ConsumeItem(soul_cfg->soul_id, use_count, "Seal::UseSoul"))
	{
		m_role->NoticeNum(errornum::EN_ITEM_NOT_ENOUGH);
		return;
	}

	m_param.soul_list[soul_type - 1] += use_count;

	this->SendBaseInfo();
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SEAL);

	gamelog::g_log_seal.printf(LL_INFO, "Seal::OnUseSoul user[%d, %s]  soul[%d, %d]",
		m_role->GetUID(), m_role->GetName(), soul_type, use_count);
}

/*
bool Seal::CheckHasSpace(int count) const
{
	if (count <= 0)
	{
		return true;
	}

	for (int index = 0; index < SEAL_BACKPACK_MAX_GRIDS; ++index)
	{
		if (m_param.backpack_seal_list[index].item_id <= 0)
		{
			if (--count <= 0)
			{
				return true;
			}
		}
	}

	return false;
}

int CompareSealStruc(const void *arg1, const void *arg2)
{
	SealItem *s1 = (SealItem*)arg1;
	SealItem *s2 = (SealItem*)arg2;

	if (s1->order > s2->order)
	{
		return -1;
	}
	else if (s1->order < s2->order)
	{
		return 1;
	}
	if (s1->item_id > s2->item_id)
	{
		return -1;
	}
	else if (s1->item_id < s2->item_id)
	{
		return 1;
	}
	return s1->level > s2->level;
}

static bool JHSortFunc(const SealItem &seal1, const SealItem &seal2)
{
	return seal1.color > seal2.color; // 这里请不要用 >= 不然标准库会断言 
}

void Seal::SortBackpack()
{
	std::vector<SealItem> seal_jinghua_vec; // 将符文精华找出来
	seal_jinghua_vec.clear();

	for (int i = 0; i < SEAL_BACKPACK_MAX_GRIDS; ++i)
	{
		SealItem &bag_seal = m_param.backpack_seal_list[i];

		if (bag_seal.IsEmpty()) continue;

		if (SEAL_TYPE_JINGHUA == bag_seal.slot_index)
		{
			seal_jinghua_vec.push_back(bag_seal);
			bag_seal.Reset();
		}
	}

	if (!seal_jinghua_vec.empty())
	{
		std::sort(seal_jinghua_vec.begin(), seal_jinghua_vec.end(), &JHSortFunc);
	}

	m_empty_grids = 0;
	qsort(m_param.backpack_seal_list, SEAL_BACKPACK_MAX_GRIDS, sizeof(SealItem), CompareSealStruc);

	for (int index = SEAL_BACKPACK_MAX_GRIDS - 1; index >= 0; --index)
	{
		SealItem &seal = m_param.backpack_seal_list[index];
		if (!seal.IsEmpty())
		{
			break;
		}

		++m_empty_grids;
	}

	// 再将符文精华加到最后面
	int empty_index = SEAL_BACKPACK_MAX_GRIDS - m_empty_grids;
	for (int i = 0; i < (int)seal_jinghua_vec.size(); ++i)
	{
		m_param.backpack_seal_list[empty_index] = seal_jinghua_vec[i];
		++empty_index;
		--m_empty_grids;
	}
}

int Seal::GetSealItemNumInVirtualBagById(int item_id)
{
	int count = 0;
	for (int i = 0; i < SEAL_BACKPACK_MAX_GRIDS; i++)
	{
		if (m_param.backpack_seal_list[i].item_id == item_id)
		{
			++count;
		}
	}

	return count;
}
*/