#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"

#include "elementheart.hpp"
#include "config/logicconfigmanager.hpp"
#include "obj/character/role.h"

#include "protocal/msgelementheart.h"
#include "scene/scene.h"
#include "gameworld/engineadapter.h"
#include "elementheartconfig.hpp"
#include "item/knapsack.h"
#include "other/cross/rolecross.h"
#include "other/capability/capability.hpp"
#include "protocal/msgrole.h"
#include "other/event/eventhandler.hpp"
#include "servercommon/string/gameworldstr.h"
#include "world.h"

ElemnetHeart::ElemnetHeart() : m_role(NULL)
{
}

ElemnetHeart::~ElemnetHeart()
{
}

void ElemnetHeart::InitParam(Role *role, const ElementHeartParam &param)
{
	m_role = role;
	m_param = param;
}

void ElemnetHeart::GetInitParam(ElementHeartParam *param) const
{
	*param = m_param;
}

void ElemnetHeart::Update(unsigned int now_second)
{
	if (now_second >= m_param.next_fresh_shop_timestamp)
	{
		m_param.next_fresh_shop_timestamp = now_second + LOGIC_CONFIG->GetElementHeartConfig().GetOtherCfg().shop_refresh_time_interval;
		this->ShopRefresh(false, true);
	}
}

void ElemnetHeart::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		int total_element_grade_add_att_percent = 0;
		for (int element_id = 0; element_id < ELEMENT_HEART_MAX_COUNT; ++ element_id)
		{
			if (!this->IsActiveElement(element_id))
			{
				continue;
			}

 			const ElementSingle &element = m_param.element_heart_list[element_id];

			const ElementHeartGradeCfg *grade_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetElementGradeCfgByGrade(element.grade);
			if (NULL != grade_cfg)
			{
				total_element_grade_add_att_percent += grade_cfg->add_texture_percent_attr;

				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += grade_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += grade_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += grade_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += grade_cfg->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += grade_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += grade_cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += grade_cfg->jianren;
			}

			double total_add_percent = 1.0 + this->GetXilianExtraPercent(element_id); //洗练元素属性额外加成
			const ElementHeartWuxingCfg *wuxing_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetElementWuxingCfgByWuxing(element.wuxing_bless);
			if (NULL != wuxing_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (int)(wuxing_cfg->maxhp * total_add_percent);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (int)(wuxing_cfg->gongji * total_add_percent);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (int)(wuxing_cfg->fangyu * total_add_percent);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += (int)(wuxing_cfg->shanbi * total_add_percent);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += (int)(wuxing_cfg->mingzhong * total_add_percent);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += (int)(wuxing_cfg->baoji * total_add_percent);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += (int)(wuxing_cfg->jianren * total_add_percent);
			}

			// 洗练
			//{
			//	const ElementHeartXiLianParam &xilian_param = m_param.element_xilian_list[element_id];
			//	for (int slot_index = 0; slot_index < ELEMENT_HEART_MAX_XILIAN_SLOT; slot_index ++)
			//	{
			//		if (!xilian_param.IsSlotOpen(slot_index)) continue;

			//		const EHXilianShuxingCfg *shuxing_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetXiLianAttrCfg(element_id, slot_index);
			//		if (NULL == shuxing_cfg) continue;;

			//		int real_type = TransformXilianAttrType(shuxing_cfg->attr_type);
			//		if (real_type < CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP || real_type > CharIntAttrs::BASE_CHARINTATTR_TYPE_MAX)
			//		{
			//			continue;
			//		}

			//		m_attrs_add.m_attrs[real_type] += xilian_param.slot_list[slot_index].xilian_val;
			//	}
			//}

			// 装备
			{
				const ElementEquipItem &equip_item = element.equip_param;
				double total_rate = 1.0;

				const ElementHeartEquipLevelCfg *level_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetEquipLevelCfg(equip_item.real_level);
				if (NULL != level_cfg)
				{
					total_rate = 1.0 + level_cfg->attr_total_percent / 10000.0;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (int)(level_cfg->maxhp * total_rate);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (int)(level_cfg->gongji * total_rate);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (int)(level_cfg->fangyu * total_rate);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += (int)(level_cfg->shanbi * total_rate);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += (int)(level_cfg->mingzhong * total_rate);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += (int)(level_cfg->baoji * total_rate);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += (int)(level_cfg->jianren * total_rate);
				}

				for (int level = equip_item.real_level; level >= 0; --level)
				{
					const ElementHeartEquipLevelCfg *cur_level_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetEquipLevelCfg(level);
					if (NULL == cur_level_cfg) continue;

					for (int grid_index = 0; grid_index < ELEMENT_HEART_EQUIP_MAX_COUNT; ++ grid_index)
					{
						if (equip_item.real_level == level)
						{
							if (0 == (equip_item.slot_flag & (1 << grid_index)) || cur_level_cfg->item_id_list[grid_index] <= 0) continue;
						}

						const ElemenEquipAttrConfig *attr_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetEquipAttrCfg(cur_level_cfg->item_id_list[grid_index]);
						if (NULL != attr_cfg)
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (int)(attr_cfg->maxhp * total_rate);
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (int)(attr_cfg->gongji * total_rate);
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (int)(attr_cfg->fangyu * total_rate);
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += (int)(attr_cfg->shanbi * total_rate);
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += (int)(attr_cfg->mingzhong * total_rate);
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += (int)(attr_cfg->baoji * total_rate);
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += (int)(attr_cfg->jianren * total_rate);
						}
					}
				}
			}
		}

		//元素之纹
		double total_add_percent = 1.0;
		if (total_element_grade_add_att_percent > 0)
		{
			total_add_percent += total_element_grade_add_att_percent / 10000.0;
		}

		for (int i = 0; i < ItemNamespace::MAX_EQUIPMENT_GRID_NUM; ++i)
		{
			if (!m_param.element_texture_list[i].IsInvalid())
			{
				const ElementHeartLevelCfg* element_texture_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetElementTextureLevelCfg(m_param.element_texture_list[i].wuxing_type, m_param.element_texture_list[i].grade);
				if (NULL != element_texture_cfg)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (int)(element_texture_cfg->maxhp * total_add_percent);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (int)(element_texture_cfg->gongji * total_add_percent);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (int)(element_texture_cfg->fangyu * total_add_percent);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += (int)(element_texture_cfg->shanbi * total_add_percent);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += (int)(element_texture_cfg->mingzhong * total_add_percent);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += (int)(element_texture_cfg->baoji * total_add_percent);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += (int)(element_texture_cfg->jianren * total_add_percent);
				}
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_ELEMENT_HEART, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs);
}

void ElemnetHeart::OnDayChange(unsigned old_dayid, unsigned now_dayid)
{
	if (old_dayid == now_dayid)
	{
		return;
	}

	m_param.one_chou_free_times = 0;
	m_param.day_shop_flush_times = 0;

	this->SendAllInfo();
}

void ElemnetHeart::ReqOpera(int info_type, int param1, int param2, int param3, int param4)
{
	const ElementHeartOtherCfg &other_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetOtherCfg();
	if (m_role->GetLevel() < other_cfg.limit_level)
	{
		return;
	}

	switch (info_type)
	{
	case Protocol::ELEMENT_HEART_REQ_TYPE_ACTIVE_GHOST:
		{
			this->ActiveElement(param1);
		}
		break;

	case Protocol::ELEMENT_HEART_REQ_TYPE_CHANGE_GHOST_WUXING_TYPE:
		{
			this->ChangeElementWuxing(param1);
		}
		break;

	case Protocol::ELEMENT_HEART_REQ_TYPE_FEED_ELEMENT:
		{
			this->FeedElement(param1, param2, param3);
		}
		break;

	case Protocol::ELEMENT_HEART_REQ_TYPE_UPGRADE_GHOST:
		{
			bool is_suc = this->UpElementGrade(param1, 0 != param2, 0 != param3);
			m_role->SendOperateResult(Protocol::SCOperateResult::OP_ELEMENT_HEART_UPGRADE, is_suc);
		}
		break;

	case Protocol::ELEMENT_HEART_REQ_TYPE_GET_PRODUCT:
		{
			this->GetProduct(param1);
		}
		break;

	case Protocol::ELEMENT_HEART_REQ_TYPE_PRODUCT_UP_SEED:
		{
			this->ProductUpSpeed(param1);
		}
		break;

	case Protocol::ELEMENT_HEART_REQ_TYPE_UPGRADE_CHARM:
		{
			//bool is_succ = this->ElementTextureUpgrade(param1, param2);
			//m_role->SendOperateResult(Protocol::SCOperateResult::OP_ELEMENT_TEXTURE_UPGRADE, is_succ);
		}
		break;

	case Protocol::ELEMENT_HEART_REQ_TYPE_ALL_INFO:
		{
			this->SendAllInfo();
		}
		break;

	case Protocol::ELEMENT_HEART_REQ_TYPE_CHOUJIANG:
		{
			this->ChouJiang(param1, 0 != param2, 0 != param3);
		}
		break;

	case Protocol::ELEMENT_HEART_REQ_TYPE_FEED_GHOST_ONE_KEY:
		{
			this->FeedElementOnekey(param1);
		}
		break;

	case Protocol::ELEMENT_HEART_REQ_TYPE_SET_GHOST_WUXING_TYPE:
		{
			this->SetElementWuxing(param1);
		}
		break;

	case Protocol::ELEMENT_HEART_REQ_TYPE_SHOP_REFRSH:
		{
			this->ShopRefresh(0 != param1, false);
		}
		break;

	case Protocol::ELEMENT_HEART_REQ_TYPE_SHOP_BUY:
		{
			this->ShopBuy(param1);
		}
		break;

	case Protocol::ELEMENT_HEART_REQ_TYPE_XILIAN:
		{
			//this->XilianReq(param1, param2, param3, 0 != param4);
		}
		break;

	case Protocol::ELEMENT_HEART_REQ_TYPE_PUTON_EQUIP:
		{
			this->PutEquip(param1, param2);
		}
		break;

	case Protocol::ELEMENT_HEART_REQ_TYPE_UPGRADE_EQUIP:
		{
			bool is_succ = this->UpgradeEquip(param1, 0 != param2);
			m_role->SendOperateResult(Protocol::SCOperateResult::OP_ELEMENT_EQUIP_UPGRADE, is_succ);
		}
		break;

	case Protocol::ELEMENT_HEART_REQ_TYPE_EQUIP_RECYCLE:
		{
			this->OnRecycleEquip(param1, param2);
		}
		break;

	default:
		break;
	}
}

void ElemnetHeart::GmOpera(int type, int p1, int p2, int p3, int p4, int p5)
{
	#ifndef _DEBUG
	return;
	#endif

	switch (type)
	{
	case GM_TYPE_TEST:
		{
			this->ReqOpera(p1, p2, p3, p4, p5);
		}
		break;

	default:
		break;
	}
}

void ElemnetHeart::ActiveElement(int id)
{
	if (id < 0 || id >= ELEMENT_HEART_MAX_COUNT)
	{
		return;
	}

	if (this->IsActiveElement(id))
	{
		m_role->NoticeNum(errornum::EN_ELEMENT_HEART_ALREADY_ACTIVE);
		return;
	}

	int active_grade = 1;
	const ElementHeartGradeCfg *grade_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetElementGradeCfgByGrade(active_grade);
	if (NULL == grade_cfg)
	{
		return;
	}

	// 消耗
	const ElementHeartOpenCfg *open_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetElementIndexCfgById(id);
	if (NULL == open_cfg)
	{
		return;
	}

	int last_id = id - 1;
	if (last_id >= 0 && !this->IsActiveElement(last_id))
	{
		m_role->NoticeNum(errornum::EN_ELEMENT_HEART_LAST_ELEMENT_HEART_NOT_ACTIVE);
		return;
	}

	if (ELEMENT_HEART_OPEN_CONDITION_TYPE_LAST_FEED_LEVEL == open_cfg->condition_type)
	{
		if(last_id >= 0 && m_param.element_heart_list[last_id].wuxing_bless > 0)
		{
			const ElementHeartWuxingCfg *wuxing_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetElementWuxingCfgByWuxing(m_param.element_heart_list[last_id].wuxing_bless);
			if (NULL == wuxing_cfg)
			{
				return;
			}

			if (wuxing_cfg->feed_level < open_cfg->param)
			{
				m_role->NoticeNum(errornum::EN_ELEMENT_HEART_FEED_LEVEL_NOT_ENOGH);
				return;
			}
		}
	}
	else
	{
		if (!m_role->GetKnapsack()->GetMoney()->UseGold(open_cfg->param, "ElemnetHeart::ActiveGhost"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}

	//激活元素
	ElementSingle &element = m_param.element_heart_list[id];
	element.id = id;
	element.wuxing_bless = 1;
	element.grade = active_grade;
	//element.wuxing_type = RandomNum(ELEMENT_HEART_WUXING_TYPE_JIN, ELEMENT_HEART_WUXING_TYPE_MAX);
	switch (id)
	{
		case 0:
		{
			element.wuxing_type = ELEMENT_HEART_WUXING_TYPE_JIN;
		}
		break;

		case 1:
		{
			element.wuxing_type = ELEMENT_HEART_WUXING_TYPE_MU;
		}
		break;

		case 2:
		{
			element.wuxing_type = ELEMENT_HEART_WUXING_TYPE_SHUI;
		}
		break;

		case 3:
		{
			element.wuxing_type = ELEMENT_HEART_WUXING_TYPE_HUO;
		}
		break;

		case 4:
		{
			element.wuxing_type = ELEMENT_HEART_WUXING_TYPE_TU;
		}
		break;
	}

	//激活元素之纹,只在激活第一个元素之心执行一次
	//if (0 == id)
	//{
	//	for (int i = 0; i < ItemNamespace::MAX_EQUIPMENT_GRID_NUM; ++i)
	//	{
	//		int wuxing_type = LOGIC_CONFIG->GetElementHeartConfig().GetCharmWuxingType(i);
	//		m_param.element_texture_list[i].wuxing_type = wuxing_type;
	//		m_param.element_texture_list[i].grade = 1;
	//	}
	//}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ELEMENT_HEART);

	// 激活产出
	unsigned int now_sec= static_cast<unsigned int>(EngineAdapter::Instance().Time());
	element.next_product_timestamp = grade_cfg->product_time_s + now_sec;

	this->SendElementSingleInfo(id);
	this->SendCharmInfo();
	this->CheckOpenXiLianSlot(id);

	gamelog::g_log_element_heart.printf(LL_INFO, "ElemnetHeart::FeedGActiveGhosthost user[%d, %s], id[%d], last_ghost_wuxingzhi[%d], need_last_wuxing_zhi[%d], next_product_timestamp[%d], now_sec[%d]",
		m_role->GetUID(), m_role->GetName(), id, last_id, open_cfg->condition_type, element.next_product_timestamp, now_sec);
}

void ElemnetHeart::FeedElement(int id, int itme_id, int num)
{
	if (id < 0 || id >= ELEMENT_HEART_MAX_COUNT || num <= 0)
	{
		return;
	}
	ElementSingle &ghost = m_param.element_heart_list[id];

	int max_wuxing = LOGIC_CONFIG->GetElementHeartConfig().GetElementWuxingMax();
	if (ghost.wuxing_bless >= max_wuxing)
	{
		m_role->NoticeNum(errornum::EN_ELEMENT_HEART_MAX_WUXING_ZHI);
		return;
	}

	const ElementHeartItemCfg *item_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetITemCfgById(itme_id);
	if (NULL == item_cfg)
	{
		return;
	}

	if (item_cfg->type != ELEMENT_HEART_ITEM_TYPE_FOOD)
	{
		return;
	}

	int wuxing_type = item_cfg->param1;
	if (wuxing_type < ELEMENT_HEART_WUXING_TYPE_JIN || wuxing_type >= ELEMENT_HEART_WUXING_TYPE_MAX)
	{
		return;
	}

	int &feed_times = ghost.wuxing_food_feed_times_list[wuxing_type];
	if (INT_MAX - feed_times < 1)
	{
		m_role->NoticeNum(errornum::EN_ELEMENT_HEART_LAST_ELEMENT_HEART_LIMIT_FEED);
	}

	// 检查关系
	WUXING_TYPE_RELACTION type = this->GetTypeRelaction(ghost.wuxing_type, wuxing_type);
	if (WUXING_TYPE_RELACTION_RESTRAINT == type)
	{
		m_role->NoticeNum(errornum::EN_ELEMENT_HEART_RESTRAIN);
		return;
	}

	float feed_percent = this->GetFeedPercentByType(type);
	int feed_value = static_cast<int>(item_cfg->param2 * feed_percent * num);
	if (feed_value <= 0 || feed_value > INT_MAX - ghost.wuxing_bless )
	{
		return;
	}

	if (feed_value > (max_wuxing - ghost.wuxing_bless))
	{
		int residue_feed_value = feed_value - (max_wuxing - ghost.wuxing_bless);
		int residue_num = static_cast<int>(residue_feed_value / (item_cfg->param2 * feed_percent));
		num -= residue_num;
	}
	
	if (!m_role->GetKnapsack()->ConsumeItem(itme_id, num, "ElemnetHeart::FeedElement"))
	{
		m_role->NoticeNum(errornum::EN_ELEMENT_HEART_ITEM_NOT_ENOGH);
		return;
	}

	ghost.wuxing_bless += feed_value;	
	++feed_times;
	
	if (ghost.wuxing_bless > max_wuxing)
	{
		ghost.wuxing_bless = max_wuxing;
	}

	this->CheckOpenXiLianSlot(id);
	this->SendElementSingleInfo(id);
	m_role->NoticeNum(noticenum::NT_ELEMENT_HEART_FEED_SUCC);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ELEMENT_HEART);

	gamelog::g_log_element_heart.printf(LL_INFO, "ElemnetHeart::FeedGhost user[%d, %s], item_id[%d], num[%d], wuxingzhi[%d], feed_value[%d]",
		m_role->GetUID(), m_role->GetName(), itme_id, num, ghost.wuxing_bless, feed_value);
}

void ElemnetHeart::FeedElementOnekey(int id)
{
	if (id < 0 || id >= ELEMENT_HEART_MAX_COUNT)
	{
		return;
	}

	ElementSingle &ghost = m_param.element_heart_list[id];

	int max_wuxing = LOGIC_CONFIG->GetElementHeartConfig().GetElementWuxingMax();
	if (ghost.wuxing_bless >= max_wuxing)
	{
		m_role->NoticeNum(errornum::EN_ELEMENT_HEART_MAX_WUXING_ZHI);
		return;
	}
	
	int last_wuxingzhi = ghost.wuxing_bless;
	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	const std::map<int, ElementHeartItemCfg> &item_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetItemMap();
	std::map<int, ElementHeartItemCfg>::const_iterator it = item_cfg.begin();
	for (; it != item_cfg.end(); it ++)
	{
		int item_cout = knapsack->Count(it->second.item_id);
		if (item_cout <= 0) continue;

		if (it->second.type != ELEMENT_HEART_ITEM_TYPE_FOOD) continue;

		int wuxing_type = it->second.param1;
		if (wuxing_type < ELEMENT_HEART_WUXING_TYPE_JIN || wuxing_type >= ELEMENT_HEART_WUXING_TYPE_MAX) continue;

		int &feed_times = ghost.wuxing_food_feed_times_list[wuxing_type];
		if (INT_MAX - feed_times < 1) continue;

		WUXING_TYPE_RELACTION type = this->GetTypeRelaction(ghost.wuxing_type, wuxing_type);
		if (WUXING_TYPE_RELACTION_RESTRAINT == type) continue;

		float feed_percent = this->GetFeedPercentByType(type);
		int feed_value = static_cast<int>(it->second.param2 * feed_percent * item_cout);
		if (feed_value <= 0) continue;

		int consume_cout = item_cout;
		if (ghost.wuxing_bless + feed_value > max_wuxing)
		{
			feed_value = max_wuxing - ghost.wuxing_bless;
			consume_cout = feed_value / (int)(it->second.param2 * feed_percent);
		}

		if (!m_role->GetKnapsack()->ConsumeItem(it->second.item_id, consume_cout, "FeedElementOnekey"))
		{
			break;
		}

		feed_times += consume_cout;
		ghost.wuxing_bless += feed_value;
		if (ghost.wuxing_bless > max_wuxing)
		{
			ghost.wuxing_bless = max_wuxing;
			break;
		}

		gamelog::g_log_element_heart.buff_printf("consume_item[id:%d num:%d feed_value:%d ]",
			it->second.item_id, consume_cout, feed_value);
	}

	if (ghost.wuxing_bless <= last_wuxingzhi)
	{
		m_role->NoticeNum(errornum::EN_ELEMENT_HEART_NO_FOOD);
		return;
	}

	this->CheckOpenXiLianSlot(id);
	this->SendElementSingleInfo(id);
	m_role->NoticeNum(noticenum::NT_ELEMENT_HEART_FEED_SUCC);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ELEMENT_HEART);

	gamelog::g_log_element_heart.commit_buff(LL_INFO);
	gamelog::g_log_element_heart.buff_printf("ElemnetHeart::FeedGhostOnekey user[%d %s] consume_item_list[", m_role->GetUID(), m_role->GetName());
}

bool ElemnetHeart::UpElementGrade(int id, bool is_one_key, bool is_auto_buy)
{
	bool is_succeeded = false;
	if (id < 0 || id >= ELEMENT_HEART_MAX_COUNT)
	{
		return false;
	}
	ElementSingle &element = m_param.element_heart_list[id];

	const ElementHeartGradeCfg *cur_grade_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetElementGradeCfgByGrade(element.grade);
	if (NULL == cur_grade_cfg)
	{
		return false;
	}

	const ElementHeartGradeCfg *next_grade_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetElementGradeCfgByGrade(element.grade + 1);
	if (NULL == next_grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_ELEMENT_HEART_LAST_ELEMENT_HEART_LIMIT_GRADE);
		return false;
	}

	if (id > 0)
	{
		int last_element_need_grade = LOGIC_CONFIG->GetElementHeartConfig().GetElementUpgradeLimit(id);
		if (last_element_need_grade > m_param.element_heart_list[id - 1].grade || last_element_need_grade < 0)
		{
			m_role->NoticeNum(errornum::EN_ELEMENT_HEART_LAST_ELEMENT_GRADE_NOT_ENOUGH);
			return false;
		}
	}

	const ElementHeartItemCfg *item_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetITemCfgById(cur_grade_cfg->item_id);
	if (NULL == item_cfg)
	{
		return false;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();

	{
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = cur_grade_cfg->item_id;
		stuff_list[0].num = cur_grade_cfg->need_item_num;
		stuff_list[0].buyable = is_auto_buy;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, true))
		{
			m_role->NoticeNum(errornum::EN_ITEM_FUNC_ITEM_COMPOSE_NOT_ENOUGH);
			return false;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "ElemnetHeart::UpElementGrade"))
		{
			return false;
		}
	}

	int new_bless_val = element.bless + item_cfg->param1;
	if (new_bless_val >= cur_grade_cfg->bless_val_limit)
	{
		new_bless_val = 0;
		++element.grade;
		is_succeeded = true;
	}

	element.bless = new_bless_val;
	if (is_succeeded)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ELEMENT_HEART);
		m_role->NoticeNum(noticenum::NT_ELEMENT_HEART_UP_GRADE_GHOST_SUCC);
	}

	this->SendElementSingleInfo(id);

	gamelog::g_log_element_heart.printf(LL_INFO, "ElemnetHeart::UpGhostGrade user[%d, %s], item_id[%d], num[%d], grade[%d]",
		m_role->GetUID(), m_role->GetName(), cur_grade_cfg->item_id, cur_grade_cfg->need_item_num, element.grade);

	return  !is_succeeded && is_one_key;
}

void ElemnetHeart::GetProduct(int id)
{
	if (id < 0 || id >= ELEMENT_HEART_MAX_COUNT)
	{
		return;
	}
	ElementSingle &element = m_param.element_heart_list[id];

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_sec < element.next_product_timestamp)
	{
		m_role->NoticeNum(errornum::EN_ELEMENT_HEART_LAST_CAN_NOT_GET_PRODUCT_TIME);
		return;
	}

	const ElementHeartGradeCfg *grade_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetElementGradeCfgByGrade(element.grade);
	if (NULL == grade_cfg)
	{
		return;
	}

	const ElementHeartWuxingCfg *wuxing_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetElementWuxingCfgByWuxing(element.wuxing_bless);
	if (NULL == wuxing_cfg)
	{
		return;
	}

	int product_num = wuxing_cfg->product_num;
	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(product_num))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	int add_time_s =  grade_cfg->product_time_s;
	element.next_product_timestamp = now_sec + add_time_s;

	gamelog::g_log_element_heart.buff_printf("ElemnetHeart::GetProduct user[%d %s] product_num[%d] item_list[", m_role->GetUID(), m_role->GetName(), product_num);

	static ElementTextureParam reward_list[CHARM_GHOST_PRODUCT_NUM_MAX]; 
	memset(reward_list, 0, sizeof(reward_list));

	static Protocol::SCElementProductListInfo ccpli;
	ccpli.count = 0;
	ccpli.info_type = 0;
	memset(ccpli.product_list, 0, sizeof(ccpli.product_list));
	
	for (int reward_count = 0; reward_count < product_num && ccpli.count <	Protocol::SCElementProductListInfo::MAX_PRODUCT_LIST_COUNT; ++ reward_count)
	{
		//随机
		int rand_weight = RandomNum(wuxing_cfg->total_weight);

		for (int product_index = 0; product_index < CHARM_GHOST_PRODUCT_NUM_MAX; ++product_index)
		{
			if (rand_weight < wuxing_cfg->weight_list[product_index])
			{
				int quality = product_index + 1;
				const ElementHeartItemCfg *item = LOGIC_CONFIG->GetElementHeartConfig().GetItemCfg(ELEMENT_HEART_ITEM_TYPE_CHARM, element.wuxing_type, quality);
				if (NULL != item)
				{

					ccpli.product_list[ccpli.count] = item->item_id;
					++ccpli.count;

					ItemConfigData reward_item(item->item_id, true, 1);
					m_role->GetKnapsack()->Put(reward_item, PUT_REASON_ELEMENT_HEART_PRODUCT);
				}

				gamelog::g_log_element_heart.buff_printf("(item_type:%d wuxing_type:%d quality:%d)",
					ELEMENT_HEART_ITEM_TYPE_CHARM, element.wuxing_type, quality);

				break;
			}

			rand_weight -= wuxing_cfg->weight_list[product_index];
		}
	}

	this->SendElementSingleInfo(id);

	int length = sizeof(ccpli) - sizeof(ccpli.product_list[0]) * (Protocol::SCElementProductListInfo::MAX_PRODUCT_LIST_COUNT - ccpli.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&ccpli, length);

	gamelog::g_log_element_heart.buff_printf("]");
	gamelog::g_log_element_heart.commit_buff(LL_INFO);
}

void ElemnetHeart::ProductUpSpeed(int id)
{
	if (id < 0 || id >= ELEMENT_HEART_MAX_COUNT)
	{
		return;
	}
	ElementSingle &ghost = m_param.element_heart_list[id];

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_sec >= ghost.next_product_timestamp)
	{
		return;
	}

	int left_time_m = (ghost.next_product_timestamp - now_sec) / SECOND_PER_MIN + 1;
	const ElementHeartOtherCfg &other_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetOtherCfg();
	int need_gold = left_time_m * other_cfg.ghost_product_up_seed_need_gold_per_min;

	if (!m_role->GetKnapsack()->ConsumeItem(other_cfg.ghost_product_up_seed_need_item_id, CHARM_ELEMENT_UP_SPEED_NEED_ITEM_NUM, "ElemnetHeart::ProductUpSpeed")
		&& !m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "ElemnetHeart::ProductUpSpeed"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	ghost.next_product_timestamp = now_sec;
	m_role->NoticeNum(noticenum::NT_ELEMENT_HEART_UP_SPEED_SUCC);
	this->SendElementSingleInfo(id);

	gamelog::g_log_element_heart.printf(LL_INFO, "CharmGhost::ProductUpSpeed user[%d, %s], id[%d], left_time_m[%d], need_gold[%d]",
		m_role->GetUID(), m_role->GetName(), id, left_time_m, need_gold);
}

void ElemnetHeart::ChouJiang(int times, bool is_use_score, bool is_double)
{
	Knapsack* knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	const ElementHeartOtherCfg &other_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetOtherCfg();
	int need_item = 0;
	int need_gold = 0;
	//int need_score = 0;
	bool is_use_item = false;
	bool is_free_chou = false;
	switch (times)
	{
	case ELEMENT_HEART_MAX_CHOU_TIMES_ONE:
		{
			if (m_param.one_chou_free_times < other_cfg.one_chou_free_chou_times)
			{
				is_free_chou = true;
			}
			else if (knapsack->HasItem(other_cfg.one_chou_need_item))
			{
				is_use_item = true;
				need_item = other_cfg.one_chou_need_item;

				if (is_double)
				{
					if (knapsack->GetMoney()->AllGoldMoreThan(other_cfg.one_chou_need_gold))
					{
						need_gold = other_cfg.one_chou_need_gold;
						times += times;
					}
				}
			}
			else
			{
				m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
				return;
			}
			//else
			//{
			//	if (is_use_score)
			//	{
			//		need_score = other_cfg.one_chou_need_score;
			//	}
			//	else
			//	{
			//		need_gold = other_cfg.one_chou_need_gold;
			//	}
			//}
		}
		break;

	case ELEMENT_HEART_MAX_CHOU_TIMES_TEN:
		{
			if (knapsack->HasItem(other_cfg.ten_chou_need_item))
			{
				is_use_item = true;
				need_item = other_cfg.ten_chou_need_item;

				if (is_double)
				{
					if (knapsack->GetMoney()->AllGoldMoreThan(other_cfg.ten_chou_need_gold))
					{
						need_gold = other_cfg.ten_chou_need_gold;
						times += times;
					}
				}
			}
			else
			{
				m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
				return;
			}
			//else
			//{
			//	if (is_use_score)
			//	{
			//		need_score = other_cfg.ten_chou_need_score;
			//	}
			//	else
			//	{
			//		need_gold = other_cfg.ten_chou_need_gold;
			//	}
			//}
		}
		break;

	default:
		return;
	}

	if (!knapsack->CheckEmptyGridNoLessThan(times))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (is_free_chou)
	{
		++m_param.one_chou_free_times;
	}

	//if (!is_free_chou && !is_use_item && need_gold > 0 &&!knapsack->GetMoney()->UseGold(need_gold, "CharmGhost::CHouJiang"))
	//{
	//	m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
	//	return;
	//}

	//if (!is_free_chou && is_use_item && !knapsack->ConsumeItem(need_item, 1, "CharmGhost::CHouJiang"))
	//{
	//	m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
	//	return;
	//}

	//if (!is_free_chou && !is_use_item && need_score > 0 && !m_role->GetRoleCross()->ConsumePastureScore(need_score, "CharmGhost::CHouJiang"))
	//{
	//	m_role->NoticeNum(errornum::EN_PASTURE_SPIRIT_POINT_NOT_ENOUGH);
	//	return;
	//}

	if (!is_free_chou)
	{
		if (is_use_item && !knapsack->ConsumeItem(need_item, 1, "CharmGhost::CHouJiang"))
		{
			m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return;
		}

		if (need_gold > 0 && !knapsack->GetMoney()->UseAllGold(need_gold, "CharmGhost::CHouJiang"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}

	const int cfg_count = LOGIC_CONFIG->GetElementHeartConfig().GetChouCfgChout();
	int total_weight = LOGIC_CONFIG->GetElementHeartConfig().GetChouCfgTotalWeight();
	Protocol::SCElementHeartChouRewardListInfo cgcrli;
	cgcrli.free_chou_times = m_param.one_chou_free_times;
	cgcrli.count = 0;

	// 给予
	gamelog::g_log_element_heart.buff_printf("ElemnetHeart::ChouJiang user[%d %s] choujiang_times[%d], item_list[", m_role->GetUID(), m_role->GetName(), times);

	for (int count = 0; count < times && cgcrli.count < ELEMENT_HEART_MAX_CHOU_TIMES_MAX; ++ count)
	{
		int rand_weight = RandomNum(total_weight);
		for (int index = 0; index < cfg_count; ++index)
		{
			const ElementHeartChouCfg *chou_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetChouCfgByIndex(index);
			if (NULL == chou_cfg)
			{
				continue;
			}

			if (rand_weight < chou_cfg->weight)
			{
				Protocol::ElementChouRewardItem &reward_item = cgcrli.reward_list[cgcrli.count];
				reward_item.item_id = chou_cfg->reward_item.item_id;
				reward_item.num = chou_cfg->reward_item.num;
				reward_item.is_bind = chou_cfg->reward_item.is_bind ? 1 : 0;

				++cgcrli.count;
				m_role->GetKnapsack()->Put(chou_cfg->reward_item, PUT_REASON_ELEMENT_HEART_SHOP_BUY);

				break;
			}

			rand_weight -= chou_cfg->weight;
		}
	}

	int length = sizeof(cgcrli) - sizeof(cgcrli.reward_list[0]) * (ELEMENT_HEART_MAX_CHOU_TIMES_MAX - cgcrli.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cgcrli, length);

	this->SendAllElementInfo();
}

void ElemnetHeart::ChangeElementWuxing(int id)
{
	if (id < 0 || id >= ELEMENT_HEART_MAX_COUNT)
	{
		return;
	}

	if (!this->IsActiveElement(id))
	{
		m_role->NoticeNum(errornum::EN_ELEMENT_HEART_NOT_ACTIVE);
		return;
	}
	
	std::vector<int> type_set;
	for (int i = ELEMENT_HEART_WUXING_TYPE_JIN; i < ELEMENT_HEART_WUXING_TYPE_MAX; ++i)
	{
		if (i == m_param.element_heart_list[id].wuxing_type)
		{
			continue;
		}

		type_set.push_back(i);
	}
	int index = RandomNum(static_cast<int>(type_set.size()));
	
	const ElementHeartOtherCfg &other_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetOtherCfg();

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(other_cfg.change_wuxing_type_need_gold, __FUNCTION__))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_param.element_heart_list[id].tartget_wuxing_type = type_set[index];
	this->SendElementSingleInfo(id, Protocol::SCElementHeartInfo::INFO_TYPE_WUXING_CHANGE);

	gamelog::g_log_element_heart.printf(LL_INFO, "ElemnetHeart::ChangeElementWuxing user[%d, %s], id[%d], tartget_wuxing_type[%d], need_gold[%d]",
		m_role->GetUID(), m_role->GetName(), id, type_set[index], other_cfg.change_wuxing_type_need_gold);
}

void ElemnetHeart::SetElementWuxing(int id)
{
	if (id < 0 || id >= ELEMENT_HEART_MAX_COUNT)
	{
		return;
	}
	ElementSingle &element = m_param.element_heart_list[id];

	if (!this->IsActiveElement(id))
	{
		m_role->NoticeNum(errornum::EN_ELEMENT_HEART_NOT_ACTIVE);
		return;
	}

	if (element.tartget_wuxing_type < ELEMENT_HEART_WUXING_TYPE_JIN || element.tartget_wuxing_type >= ELEMENT_HEART_WUXING_TYPE_MAX)
	{
		return;
	}

	int from_type = element.wuxing_type;
	int to_type = element.tartget_wuxing_type;

	element.wuxing_type = element.tartget_wuxing_type;
	element.tartget_wuxing_type = 0;
	this->SendElementSingleInfo(id);

	this->OnChangeWuXing(id, from_type, to_type);

	gamelog::g_log_element_heart.printf(LL_INFO, "ElemnetHeart::SetElementWuxing user[%d, %s], id[%d], wuxing_type[%d], tartget_wuxing_type[%d]",
		m_role->GetUID(), m_role->GetName(), id, element.wuxing_type, element.tartget_wuxing_type);
}

ElemnetHeart::WUXING_TYPE_RELACTION ElemnetHeart::GetTypeRelaction(int src_type, int target_type)
{
	WUXING_TYPE_RELACTION type = WUXING_TYPE_RELACTION_NORMAL;

	if (src_type == target_type)
	{
		return WUXING_TYPE_RELACTION_SAME;
	}

	switch (src_type)
	{
	case ELEMENT_HEART_WUXING_TYPE_JIN:
		{
			if (ELEMENT_HEART_WUXING_TYPE_HUO == target_type)
			{
				type = WUXING_TYPE_RELACTION_RESTRAINT;
			}

			if (ELEMENT_HEART_WUXING_TYPE_TU == target_type)
			{
				type = WUXING_TYPE_RELACTION_PROMOTE;
			}
		}
		break;

	case ELEMENT_HEART_WUXING_TYPE_MU:
		{
			if (ELEMENT_HEART_WUXING_TYPE_JIN == target_type)
			{
				type = WUXING_TYPE_RELACTION_RESTRAINT;
			}

			if (ELEMENT_HEART_WUXING_TYPE_SHUI == target_type)
			{
				type = WUXING_TYPE_RELACTION_PROMOTE;
			}
		}
		break;

	case ELEMENT_HEART_WUXING_TYPE_SHUI:
		{
			if (ELEMENT_HEART_WUXING_TYPE_TU == target_type)
			{
				type = WUXING_TYPE_RELACTION_RESTRAINT;
			}

			if (ELEMENT_HEART_WUXING_TYPE_JIN == target_type)
			{
				type = WUXING_TYPE_RELACTION_PROMOTE;
			}
		}
		break;

	case ELEMENT_HEART_WUXING_TYPE_HUO:
		{
			if (ELEMENT_HEART_WUXING_TYPE_SHUI == target_type)
			{
				type = WUXING_TYPE_RELACTION_RESTRAINT;
			}

			if (ELEMENT_HEART_WUXING_TYPE_MU == target_type)
			{
				type = WUXING_TYPE_RELACTION_PROMOTE;
			}
		}
		break;

	case ELEMENT_HEART_WUXING_TYPE_TU:
		{
			if (ELEMENT_HEART_WUXING_TYPE_MU == target_type)
			{
				type = WUXING_TYPE_RELACTION_RESTRAINT;
			}

			if (ELEMENT_HEART_WUXING_TYPE_HUO == target_type)
			{
				type = WUXING_TYPE_RELACTION_PROMOTE;
			}
		}
		break;

	default: ;
		type = WUXING_TYPE_RELACTION_NORMAL;
		break;
	}

	return type;
}

bool ElemnetHeart::IsActiveElement(int id) const
{
	if (id < 0 || id >= ELEMENT_HEART_MAX_COUNT)
	{
		return false;
	}

	const ElementSingle &element = m_param.element_heart_list[id];
	return element.grade > 0 && element.wuxing_bless > 0;
}

float ElemnetHeart::GetFeedPercentByType(WUXING_TYPE_RELACTION type)
{
	const ElementHeartOtherCfg &other_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetOtherCfg();
	float value = static_cast<float>(0.01);
	switch (type)
	{
	case WUXING_TYPE_RELACTION_NORMAL:
		{
			value *= other_cfg.food_percent_normal;
		}
		break;

	case WUXING_TYPE_RELACTION_SAME:
		{
			value *= other_cfg.food_percent_same;
		}
		break;

	case WUXING_TYPE_RELACTION_PROMOTE:
		{
			value *= other_cfg.food_percent_promote;
		}
		break;

	case WUXING_TYPE_RELACTION_RESTRAINT:
		{
			value *= other_cfg.food_percent_restraint;
		}
		break;

		default:
			value = 0;
	}

	return value;
}

bool ElemnetHeart::ElementTextureUpgrade(int e_index, int grid_index)
{
	if (e_index < 0 || e_index >= SHENZHUANG_MAX_PART)
	{
		return false;
	}

	bool is_succ = false;
	if (!this->IsActiveElement(0))
	{
		m_role->NoticeNum(errornum::EN_ELEMENT_HEART_NOT_ACTIVE_GHOST);
		return false;
	}

	ElementTextureParam& element_texture_param = m_param.element_texture_list[e_index];
	const ElementHeartLevelCfg* cur_grade_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetElementTextureLevelCfg(element_texture_param.wuxing_type, element_texture_param.grade);
	if (NULL == cur_grade_cfg)
	{
		return false;
	}

	const ElementHeartLevelCfg* next_grade_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetElementTextureLevelCfg(element_texture_param.wuxing_type, element_texture_param.grade + 1);
	if (NULL == next_grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_ELEMENT_HEART_FUZHOU_MAX_GRADE);
		return false;
	}

	ItemGridData item_grid_data;
	m_role->GetKnapsack()->GetItemGridData(grid_index, &item_grid_data);

	if (item_grid_data.Invalid())
	{
		return false;
	}

	const ElementHeartItemCfg* item_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetITemCfgById(item_grid_data.item_id);
	if (NULL == item_cfg) return false;

	if (!m_role->GetKnapsack()->ConsumeItemByIndex(grid_index, 1, "ElementTextureUpgrade"))
	{
		m_role->NoticeNum(errornum::EN_ITEM_NOT_ENOUGH);
		return false;
	}

	// 给予
	char old_grade = element_texture_param.grade;
	int old_exp = element_texture_param.exp;

	element_texture_param.exp += item_cfg->param2;
	if (element_texture_param.exp >= cur_grade_cfg->exp_limit)
	{
		element_texture_param.exp -= cur_grade_cfg->exp_limit;
		++element_texture_param.grade;
		is_succ = true;

		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ELEMENT_HEART);
	}

	this->SendSingleCharmInfo(e_index);

	gamelog::g_log_element_heart.printf(LL_INFO, "ElementTexture [user[%d %s], ElementTexture[e_index:%d wuxing_type:%d, grade:%d->:%d exp:%d->%d]",
		m_role->GetUID(), m_role->GetName(), e_index, element_texture_param.wuxing_type, old_grade, element_texture_param.grade,
		old_exp, element_texture_param.exp);

	return !is_succ;
}

void ElemnetHeart::SendElementSingleInfo(int id, int info_type) const
{
	if (id < 0 || id >= ELEMENT_HEART_MAX_COUNT)
	{
		return;
	}
	const ElementSingle &element = m_param.element_heart_list[id];

	static Protocol::SCElementHeartInfo cggi;
	cggi.pasture_score = m_role->GetRoleCross()->GetPastureScore();
	cggi.info_type = static_cast<char>(info_type);
	cggi.free_chou_times = m_param.one_chou_free_times;
	cggi.count = 0;
	
	cggi.element_heart_list[0] = element;
	cggi.element_heart_list[0].id = id;
	++cggi.count;

	int lenth = sizeof(cggi) - sizeof(cggi.element_heart_list[0]) * (ELEMENT_HEART_MAX_COUNT - cggi.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cggi, lenth);
}

void ElemnetHeart::SendAllElementInfo() const
{
	static Protocol::SCElementHeartInfo cggi;
	cggi.pasture_score = m_role->GetRoleCross()->GetPastureScore();
	cggi.info_type = Protocol::SCElementHeartInfo::INFO_TYPE_ALL_ELEMENT;
	cggi.free_chou_times = m_param.one_chou_free_times;
	cggi.count = 0;

	for (int id = 0; id < ELEMENT_HEART_MAX_COUNT; ++ id)
	{
		const ElementSingle &element = m_param.element_heart_list[id];
		cggi.element_heart_list[cggi.count] = element;
		cggi.element_heart_list[cggi.count].id = id;
		++cggi.count;
	}

	int lenth = sizeof(cggi) - sizeof(cggi.element_heart_list[0]) * (ELEMENT_HEART_MAX_COUNT - cggi.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cggi, lenth);
}

void ElemnetHeart::SendAllInfo()
{
	this->SendCharmInfo();
	this->SendAllElementInfo();
	this->SendXilianAllInfo();
	this->SendShopInfo();
}

void ElemnetHeart::SendCharmInfo() const
{
	Protocol::SCElementTextureInfo cgci;

	UNSTD_STATIC_CHECK(sizeof(cgci.charm_list) == sizeof(m_param.element_texture_list));
	memcpy(cgci.charm_list, m_param.element_texture_list, sizeof(cgci.charm_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cgci, sizeof(cgci));
}

void ElemnetHeart::SendSingleCharmInfo(int e_index) const
{
	Protocol::SCElemnetHeartSingleCharmInfo cgsci;
	cgsci.e_index = e_index;
	cgsci.charm_info = m_param.element_texture_list[e_index];

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cgsci, sizeof(cgsci));
}

void ElemnetHeart::ShopRefresh(bool is_use_score, bool is_auto)
{
	const ElementHeartOtherCfg &other_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetOtherCfg();

	if (!is_auto)
	{
		if (m_param.day_shop_flush_times >= other_cfg.shop_refresh_free_times)
		{
			if (is_use_score)
			{
				if (!m_role->GetRoleCross()->ConsumePastureScore(other_cfg.shop_refresh_need_score, "ElemnetHeart::ShopRefresh"))
				{
					m_role->NoticeNum(errornum::EN_PASTURE_SPIRIT_POINT_NOT_ENOUGH);
					return;
				}
			}
			else
			{
				bool is_use_nobind = true;
				if (m_role->GetKnapsack()->GetMoney()->UseGoldBind(other_cfg.shop_refresh_need_gold_bind, "ElemnetHeart::ShopRefresh"))
				{
					is_use_nobind = false;
				}

				if (is_use_nobind && !m_role->GetKnapsack()->GetMoney()->UseGold(other_cfg.shop_refresh_need_gold, "ElemnetHeart::ShopRefresh"))
				{
					m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					return;
				}
			}
		}
		else
		{
			++m_param.day_shop_flush_times;
		}
	}

	LOGIC_CONFIG->GetElementHeartConfig().GetRandomShopItem(m_param.shop_item_list);

	this->SendShopInfo();
}

void ElemnetHeart::ShopBuy(int seq)
{
	if (seq < 0 || seq >= ELEMENT_SHOP_ITEM_COUNT)
	{
		return;
	}

	ElementHeartShopItem &shop_item = m_param.shop_item_list[seq];
	const ElementHeartShopConfig *shop_item_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetShopCfg(shop_item.shop_seq);
	if (NULL == shop_item_cfg)
	{
		return;
	}

	if (shop_item.has_buy > 0)
	{
		m_role->NoticeNum(noticenum::EN_ELEMENT_SHOP_HAS_ALEADY_BUY);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_HAS_NO_SPACING);
		return;
	}

	bool is_use_bind = true;
	if (!m_role->GetKnapsack()->GetMoney()->UseGoldBind(shop_item_cfg->cost_gold_bind, "ElemnetHeart::ShopBuy"))
	{
		is_use_bind = false;
	}

	if (!is_use_bind && !m_role->GetKnapsack()->GetMoney()->UseGold(shop_item_cfg->cost_gold, "ElemnetHeart::ShopBuy"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	shop_item.has_buy = 1;
	m_role->GetKnapsack()->Put(shop_item_cfg->reward_item, PUT_REASON_ELEMENT_HEART_SHOP_BUY);

	m_role->NoticeNum(noticenum::NT_ELEMENT_SHOP_BUY_SUCC);
	this->SendShopInfo();
	this->SendAllElementInfo();
}

void ElemnetHeart::PutEquip(int element_heart_type, int slot_index)
{
	if (element_heart_type < 0 || element_heart_type >= ELEMENT_HEART_MAX_COUNT || slot_index < 0 || slot_index >= ELEMENT_HEART_EQUIP_MAX_COUNT) return;

	if (!this->IsActiveElement(element_heart_type))
	{
		m_role->NoticeNum(errornum::EN_ELEMENT_HEART_NOT_ACTIVE);
		return;
	}

	ElementEquipItem &up_item = m_param.element_heart_list[element_heart_type].equip_param;
	const ElementHeartEquipLevelCfg *level_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetEquipLevelCfg(up_item.real_level);
	if (NULL == level_cfg) return;

	if (0 != (up_item.slot_flag & (1 << slot_index)))
	{
		m_role->NoticeNum(errornum::EN_ELEMENT_HEART_EQUIP_UPGRADE_ALEADY_PUT_SLOT);
		return;
	}

	if (!m_role->GetKnapsack()->ConsumeItem(level_cfg->item_id_list[slot_index], 1, "ElemnetHeart::PutEquip"))
	{
		m_role->NoticeNum(errornum::EN_ITEM_NOT_ENOUGH);
		return;
	}

	up_item.slot_flag |= (1 << slot_index);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ELEMENT_HEART);
	this->SendElementSingleInfo(element_heart_type);
}

bool ElemnetHeart::UpgradeEquip(int element_heart_type, bool is_auto_buy)
{
	bool is_succ = false;
	if (element_heart_type < 0 || element_heart_type >= ELEMENT_HEART_MAX_COUNT) return false;

	Knapsack* knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	ElementEquipItem &up_item = m_param.element_heart_list[element_heart_type].equip_param;

	const ElementHeartEquipLevelCfg *level_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetEquipLevelCfg(up_item.real_level);
	if (NULL == level_cfg) return false;

	for (int slot_type = 0; slot_type < ELEMENT_HEART_EQUIP_MAX_COUNT; ++slot_type)
	{
		if (level_cfg->item_id_list[slot_type] > 0 && 0 == (up_item.slot_flag & (1 << slot_type)))
		{
			m_role->NoticeNum(errornum::EN_ELEMENT_EQUIP_UPGRADE_HAS_SLOT_NOT_ACTIVE);
			return false;
		}
	}

	if (NULL == LOGIC_CONFIG->GetElementHeartConfig().GetEquipLevelCfg(up_item.real_level + 1))
	{
		m_role->NoticeNum(errornum::EN_ELEMENT_HEART_EQUIP_UPGRADE_ALEADY_MAX_LEVEL);
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();

	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	stuff_list[0].item_id = level_cfg->comsume_item_id;
	stuff_list[0].num = level_cfg->comsume_item_num;
	stuff_list[0].buyable = is_auto_buy;

	if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, true))
	{
		m_role->NoticeNum(errornum::EN_ITEM_FUNC_ITEM_COMPOSE_NOT_ENOUGH);
		return false;
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "ElemnetHeart::UpgradeEquip"))
	{
		return false;
	}

	++up_item.upgrade_progress;
	if (up_item.upgrade_progress >= level_cfg->upgrade_progress)
	{
		up_item.upgrade_progress -= level_cfg->upgrade_progress;
		++up_item.real_level;
		up_item.slot_flag = 0;

		is_succ = true;
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ELEMENT_HEART);
	}

	this->SendElementSingleInfo(element_heart_type);

	return !is_succ;
}

void ElemnetHeart::ActiveElementTexture(short equip_type)
{
	//if (Equipment::E_TYPE_ZHUANZHI_MIN > equip_type || Equipment::E_TYPE_ZHUANZHI_MAX <= equip_type) return -3;
	short equip_index = equip_type - Equipment::E_TYPE_ZHUANZHI_MIN;
	if (equip_index < 0 || equip_index >= static_cast<short>(static_array_size(m_param.element_texture_list)))
	{
		return;
	}

	auto& data = m_param.element_texture_list[equip_index];
	if (data.grade != 0)
	{
		return;
	}
	int wuxing_type = LOGIC_CONFIG->GetElementHeartConfig().GetCharmWuxingType(equip_index);
	data.wuxing_type = wuxing_type;
	data.grade = 1;

	SendSingleCharmInfo(equip_index);
}

void ElemnetHeart::SendShopInfo()
{
	static Protocol::SCElementShopInfo xysi;
	xysi.next_refresh_timestamp = m_param.next_fresh_shop_timestamp;
	xysi.today_shop_flush_times = m_param.day_shop_flush_times;
	memcpy(xysi.shop_item_list, m_param.shop_item_list, sizeof(xysi.shop_item_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&xysi, sizeof(xysi));
}

int ElemnetHeart::TransformXilianAttrType(int type)
{
	switch (type)
	{
	case EH_XL_SHUXING_TYPE_MAXHP:
		{
			return CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP;
		}
		break;

	case EH_XL_SHUXING_TYPE_GONGJI:
		{
			return CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI;
		}
		break;

	case EH_XL_SHUXING_TYPE_FANGYU:
		{
			return CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU;
		}
		break;

	case EH_XL_SHUXING_TYPE_MINGZHONG:
		{
			return CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG;
		}
		break;

	case EH_XL_SHUXING_TYPE_SHANBI:
		{
			return CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI;
		}
		break;

	case EH_XL_SHUXING_TYPE_BAOJI:
		{
			return CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI;
		}
		break;

	case EH_XL_SHUXING_TYPE_JIANREN:
		{
			return CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN;
		}
		break;

	case EH_XL_SHUXING_TYPE_BASE_CONSTANT_ZENGSHANG:
		{
			return CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG;
		}
		break;

	case EH_XL_SHUXING_TYPE_CONSTANT_MIANSHANG:
		{
			return CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG;
		}
		break;
	}

	return CharIntAttrs::CHARINTATTR_TYPE_INVALID;
}

void ElemnetHeart::XilianReq(int elemengt_id, int lock_flag, int xilian_comsume_color, bool is_auto_buy)
{
	if (elemengt_id < 0 || elemengt_id >= ELEMENT_HEART_MAX_COUNT) return;

	if (xilian_comsume_color < EH_COUMME_COLOR_INVALID || xilian_comsume_color >= EH_COUMME_COLOR_MAX) return;

	if (0 == m_param.element_xilian_list[elemengt_id].open_slot_flag)
	{
		m_role->NoticeNum(errornum::EN_ELEMENT_HEART_XILIAN_NOT_OPEN);
		return;
	}

	int lock_num = 0;
	for (int i = 0; i < ELEMENT_HEART_MAX_XILIAN_SLOT; i++)
	{
		if (0 != (lock_flag & (1 << i)))
		{
			++lock_num;
		}
	}

	const int star_count_total_weight = LOGIC_CONFIG->GetElementHeartConfig().GetXilianConsuemeTotalWeight(xilian_comsume_color);
	if (star_count_total_weight <= 0) return;

	const ElementHeartXilianComsumeCfg *comsume_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetXilianConsumeCfg(xilian_comsume_color);
	if (NULL == comsume_cfg) return;

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();

	short stuff_count = 0;
	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

	if (comsume_cfg->consume_item.item_id > 0)
	{
		stuff_list[stuff_count].item_id = comsume_cfg->consume_item.item_id;
		stuff_list[stuff_count].num = comsume_cfg->consume_item.num;
		stuff_list[stuff_count].buyable = is_auto_buy;

		if (lock_num > 0)
		{
			const ElementHeartXilianLockCfg *lock_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetXilianLockCfg(xilian_comsume_color, lock_num);
			if (NULL == lock_cfg) return;
			stuff_list[stuff_count].num += lock_cfg->lock_comsume_item.num;
		}

		++stuff_count;
	}

	//这里只需要消耗一种物品，只需要用stuff_list的一个元素
	//if (lock_num > 0)
	//{
	//	const ElementHeartXilianLockCfg *lock_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetXilianLockCfg(xilian_comsume_color, lock_num);
	//	if (NULL == lock_cfg) return;

	//	if (lock_cfg->lock_comsume_item.item_id > 0)
	//	{
	//		stuff_list[stuff_count].item_id = lock_cfg->lock_comsume_item.item_id;
	//		stuff_list[stuff_count].num = lock_cfg->lock_comsume_item.num;
	//		stuff_list[stuff_count].buyable = is_auto_buy;
	//		++stuff_count;
	//	}
	//}

	if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
	{
		m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
		return;
	}

	int notic_num = 0;
	int notic_star_list[ELEMENT_HEART_MAX_XILIAN_SLOT] = { 0 };
	ElementHeartXiLianParam temp_xilian_data;
	temp_xilian_data.Reset();
	temp_xilian_data.open_slot_flag = m_param.element_xilian_list[elemengt_id].open_slot_flag;

	//本次随机最大星数
	int color_max_star = 0;
	//本次刷新属性个数
	int refresh_count = 0;
	for (int slot = 0; slot < ELEMENT_HEART_MAX_XILIAN_SLOT; slot++)
	{
		if (0 == (temp_xilian_data.open_slot_flag & (1 << slot)))
		{
			continue;
		}

		if (0 != (lock_flag & (1 << slot)))
		{
			temp_xilian_data.slot_list[slot] = m_param.element_xilian_list[elemengt_id].slot_list[slot];
		}
		else
		{
			const EHXilianShuxingCfg *shuxing_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetXiLianAttrCfg(elemengt_id, slot);
			if (NULL == shuxing_cfg) return;

			int rand_star_value = RandomNum(star_count_total_weight);
			int rand_star = 0;

			for (int j=0; j < ELEMENT_HEART_XILIAN_MAX_STAR_COUNT; j++)
			{
				if (rand_star_value < comsume_cfg->star_weight[j])
				{
					rand_star = j + 1;

					//策划需求，使用洗练石必能练出一条最小star属性
					//先获取随机的最大星数
					if ( rand_star > color_max_star)
					{
						color_max_star = rand_star;
					}
					break;
				}
				else
				{
					rand_star_value -= comsume_cfg->star_weight[j];
				}
			}

			if (rand_star <= 0 || rand_star > ELEMENT_HEART_XILIAN_MAX_STAR_COUNT) return;

			int rand_attr_value = RandomNum(shuxing_cfg->star_min[rand_star - 1], shuxing_cfg->star_max[rand_star - 1] + 1);
			if (rand_attr_value <= 0) return;

			int rand_element_attr_type = LOGIC_CONFIG->GetElementHeartConfig().GetRandElementAttr(elemengt_id);

			temp_xilian_data.slot_list[slot].xilian_val = rand_attr_value;
			temp_xilian_data.slot_list[slot].element_attr_type = rand_element_attr_type;
			++refresh_count;
			if (rand_star >= 7 && notic_num >= 0 && notic_num < ELEMENT_HEART_MAX_XILIAN_SLOT)
			{
				notic_star_list[notic_num++] = rand_star;
			}
		}
	}

	{
		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "ElemnetHeart::XilianReq"))
		{
			return;
		}

		//没有随机到指定星级
		if (color_max_star < comsume_cfg->max_star_number - 1)
		{
			//获取该修复 第几个没锁住的属性
			int rerfesh_index = RandomNum(refresh_count);

			for (int slot = 0; slot < ELEMENT_HEART_MAX_XILIAN_SLOT; slot++)
			{
				if (0 == (temp_xilian_data.open_slot_flag & (1 << slot)))
				{
					continue;
				}
				if (0 != (lock_flag & (1 << slot)))
				{
					continue;
				}

				if (rerfesh_index != 0)
				{
					--rerfesh_index;
					continue;
				}

				const EHXilianShuxingCfg *shuxing_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetXiLianAttrCfg(elemengt_id, slot);
				if (!shuxing_cfg)
				{
					break;
				}
				int fix_star = comsume_cfg->max_star_number - 1;
				if (fix_star <= 0 || fix_star > ELEMENT_HEART_XILIAN_MAX_STAR_COUNT)
				{
					continue;
				}
				int rand_attr_value = RandomNum(shuxing_cfg->star_min[fix_star - 1], shuxing_cfg->star_max[fix_star - 1] + 1);
				if (rand_attr_value <= 0)
				{
					rand_attr_value = 1;
				}

				int rand_element_attr_type = LOGIC_CONFIG->GetElementHeartConfig().GetRandElementAttr(elemengt_id);

				temp_xilian_data.slot_list[slot].xilian_val = rand_attr_value;
				temp_xilian_data.slot_list[slot].element_attr_type = rand_element_attr_type;
				break;
			}
		}

		m_param.element_xilian_list[elemengt_id] = temp_xilian_data;
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ELEMENT_HEART);

	for (int i = 0; i < notic_num && i < ELEMENT_HEART_MAX_XILIAN_SLOT; i++)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_element_heart_xilian, m_role->GetUID(),
			m_role->GetName(), (int)m_role->GetCamp(), notic_star_list[i]);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}

	this->SendXiLianSingleInfo(elemengt_id);
}

void ElemnetHeart::CheckOpenXiLianSlot(int elementid)
{
	if (elementid < 0 || elementid >= ELEMENT_HEART_MAX_COUNT) return;

	ElementSingle &element = m_param.element_heart_list[elementid];
	ElementHeartXiLianParam &xilian_param = m_param.element_xilian_list[elementid];

	const ElementHeartWuxingCfg *wuxing_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetElementWuxingCfgByWuxing(element.wuxing_bless);
	if (NULL == wuxing_cfg)
	{
		return;
	}

	if (wuxing_cfg->feed_level <= 0)
	{
		return;
	}

	for (int slot = 0; slot < ELEMENT_HEART_MAX_XILIAN_SLOT;slot ++)
	{
		if (0 != (xilian_param.open_slot_flag & (1 << slot)))
		{
			continue;
		}

		int open_level = LOGIC_CONFIG->GetElementHeartConfig().GetXiLianSlotOpenLevel(elementid, slot);
		if (wuxing_cfg->feed_level >= open_level)
		{
			xilian_param.open_slot_flag |= (1 << slot);
			this->OnOpenXilianSlot(elementid, slot);
		}
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ELEMENT_HEART);
	this->SendXiLianSingleInfo(elementid);
}

void ElemnetHeart::SendXiLianSingleInfo(int element_id)
{
	Protocol::SCElementXiLianSingleInfo xlsi;

	xlsi.element_id = element_id;
	xlsi.xilian_info.Reset();

	const ElementHeartXiLianParam &xilian_param = m_param.element_xilian_list[element_id];
	xlsi.xilian_info = xilian_param;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&xlsi, sizeof(xlsi));
}

void ElemnetHeart::SendXilianAllInfo()
{
	Protocol::SCElementXiLianAllInfo xlai;

	memset(xlai.xilian_list_info, 0, sizeof(xlai.xilian_list_info));
	memcpy(xlai.xilian_list_info, m_param.element_xilian_list, sizeof(xlai.xilian_list_info));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&xlai, sizeof(xlai));
}

void ElemnetHeart::OnOpenXilianSlot(int elementid, int slot_id)
{
	if (elementid < 0 || elementid >= ELEMENT_HEART_MAX_COUNT || slot_id < 0 || slot_id >= ELEMENT_HEART_MAX_XILIAN_SLOT)
	{
		return;
	}

	const EHXilianShuxingCfg *shuxing_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetXiLianAttrCfg(elementid, slot_id);
	if (NULL == shuxing_cfg) return;

	int rand_attr_value = RandomNum(shuxing_cfg->star_min[0], shuxing_cfg->star_max[0] + 1);
	int rand_element_attr_type = LOGIC_CONFIG->GetElementHeartConfig().GetRandElementAttr(elementid);

	ElementHeartXiLianItem &slot_item = m_param.element_xilian_list[elementid].slot_list[slot_id];
	slot_item.xilian_val = rand_attr_value;
	slot_item.element_attr_type = rand_element_attr_type;
}

double ElemnetHeart::GetXilianExtraPercent(int element_id)
{
	if (element_id < 0 || element_id >= ELEMENT_HEART_MAX_COUNT)
	{
		return 0;
	}

	const ElementHeartXiLianParam &xilian_param = m_param.element_xilian_list[element_id];
	const ElementSingle &element = m_param.element_heart_list[element_id];

	int same_attr_count = 0;
	for (int i = 0; i < ELEMENT_HEART_MAX_XILIAN_SLOT; i++)
	{
		if (xilian_param.IsSlotOpen(i) && element.wuxing_type == xilian_param.slot_list[i].element_attr_type)
		{
			++ same_attr_count;
		}
	}

	int add_percent = LOGIC_CONFIG->GetElementHeartConfig().GetExtraAttrAddPercent(element.wuxing_type, same_attr_count);

	return add_percent / 100.0;
}

void ElemnetHeart::OnRecycleEquip(int knapsack_index, int count)
{
	if (count <= 0)
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	ItemGridData item_grid_data;
	knapsack->GetItemGridData(knapsack_index, &item_grid_data);

	if (item_grid_data.Invalid())
	{
		return;
	}

	const ElemenEquipAttrConfig * reward_cfg = LOGIC_CONFIG->GetElementHeartConfig().GetEquipAttrCfg(item_grid_data.item_id);
	if (NULL == reward_cfg)
	{
		return;
	}

	if (!knapsack->ConsumeItemByIndex(knapsack_index, count, "ElemnetHeart::OnRecycleEquip"))
	{
		m_role->NoticeNum(errornum::EN_ITEM_NOT_ENOUGH);
		return;
	}

	ItemConfigData temp_reward = reward_cfg->reward_item;
	temp_reward.num *= count;

	knapsack->Put(temp_reward, PUT_REASON_ELEMENT_EQUIP_RECYCLE);

	gamelog::g_log_element_heart.printf(LL_INFO, "OnRecycleEquip [user[%d %s], OnRecycleEquip[recytcle_id:%d reward_item_id:%d, reward_count:%d]",
		m_role->GetUID(), m_role->GetName(), item_grid_data.item_id, reward_cfg->reward_item.item_id, count);
}

void ElemnetHeart::OnChangeWuXing(int element_id, int from_type, int to_type)
{
	if (element_id < 0 || element_id >= ELEMENT_HEART_MAX_COUNT || from_type < ELEMENT_HEART_WUXING_TYPE_JIN ||
		to_type >= ELEMENT_HEART_WUXING_TYPE_MAX || to_type < ELEMENT_HEART_WUXING_TYPE_JIN || to_type >= ELEMENT_HEART_WUXING_TYPE_MAX)
	{
		return;
	}

	ElementHeartXiLianParam &xilian_data = m_param.element_xilian_list[element_id];
	for (int i = 0; i < ELEMENT_HEART_MAX_XILIAN_SLOT; i ++)
	{
		if (!xilian_data.IsSlotOpen(i)) continue;

		if (from_type == xilian_data.slot_list[i].element_attr_type)
		{
			xilian_data.slot_list[i].element_attr_type = to_type;
		}
		else if (to_type == xilian_data.slot_list[i].element_attr_type)
		{
			xilian_data.slot_list[i].element_attr_type = from_type;
		}
	}

	this->SendXiLianSingleInfo(element_id);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ELEMENT_HEART);
}