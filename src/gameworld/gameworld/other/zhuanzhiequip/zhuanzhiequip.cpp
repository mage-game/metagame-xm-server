#include <algorithm>

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/string/gameworldstr.h"

#include "gamecommon.h"

#include "globalconfig/globalconfig.h"

#include "engineadapter.h"
#include "zhuanzhiequip.h"

#include "protocal/msgrole.h"
#include "protocal/msgzhuanzhiequip.hpp"

#include "item/knapsack.h"
#include "item/itempool.h"
#include "item/itemextern.hpp"

#include "scene/scenemanager.h"
#include "scene/scene.h"

#include "world.h"
#include "gamelog.h"
#include "gstr.h"

#include "obj/character/role.h"

#include "skill/skillbase.hpp"
#include "effect/skilleffect/effectonce.hpp"

#include "other/event/eventhandler.hpp"
#include "other/vip/vip.hpp"

#include "config/logicconfigmanager.hpp"

#include "zhuanzhiequipconfig.hpp"
#include "other/sublock/sublock.hpp"
#include "other/shop/roleshop.hpp"
#include "equipment/equipmentmanager.h"
#include "equipment/equipmentfunction.h"
#include "other/talent/talent.hpp"
#include "other/magicalprecious/magicalpreciouscfg.hpp"
#include "other/magicalprecious/magicalprecious.hpp"
#include "task/newtaskmanager.h"
#include "config/activityconfig/chongzhiconfig.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/equipbaptize/equipbaptize.hpp"
#include "other/chestshop/rolechestshop.hpp"
#include "other/tianshuxunzhu/tianshuxunzhu.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////
ZhuanzhiEquip::ZhuanzhiEquip()
{
	memset(m_xianpin_value_list, 0, sizeof(m_xianpin_value_list));
}

ZhuanzhiEquip::~ZhuanzhiEquip()
{

}

void ZhuanzhiEquip::SetRole(Role *role)
{
	m_role = role;
}

void ZhuanzhiEquip::Init(Role *role, const ZhuanzhiEquipParam &param)
{
	m_role = role;
	m_param = param;
}

void ZhuanzhiEquip::GetInitParam(ZhuanzhiEquipParam *param)
{
	*param = m_param;
}

void ZhuanzhiEquip::OnInitFinish()
{
	this->ReCalZhuanZhiEquipTotalLevel();
	this->CheckActiveEquipEffect();
}

void ZhuanzhiEquip::Update(time_t now_second)
{

}

void ZhuanzhiEquip::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		CharIntAttrs base_equip_attrs;

		for (int part_index = 0; part_index < E_INDEX_ZHUANZHI::E_INDEX_MAX; ++part_index)
		{
			ZhuanzhiEquipItem &equip_data = m_param.zhuanzhi_equip_list[part_index];

			//装备属性
			{
				ItemDataWrapper &itemdata = equip_data.zhuanzhi_equip_data;
				if (itemdata.Invalid())
				{
					continue;
				}

				const Equipment *equip = GetZhuanzhiEquip(itemdata.item_id);
				if (nullptr == equip)
				{
					continue;
				}

				// 基础属性,注意每次获取都reset
				base_equip_attrs.Reset();
				equip->GetBaseAttr(base_equip_attrs);

				// 天赋加成 - 转职装备基础属性
				{
					m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_ZHUANZHI_EQUIP, base_equip_attrs, m_attrs_add);
					m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_ZHUANZHI_EQUIP_BASE, base_equip_attrs, m_attrs_add);

					for (int i = CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP; i < CharIntAttrs::BASE_CHARINTATTR_TYPE_MAX; ++i)
					{
						if (base_equip_attrs.m_attrs[i] > 0)
						{
//							m_attrs_add.m_attrs[i] += base_equip_attrs.m_attrs[i];
							m_attrs_add.AddValue(i, static_cast<Attribute>(base_equip_attrs.m_attrs[i]));
						}
					}
				}
			}

			// 装备特殊属性
			{
				this->GetEquipSpecialAttr(m_attrs_add, base_equip_attrs);
			}

			// 升星
			const ZhuanzhiUpStarConfig *star_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetUpStarCfg(part_index, this->GetEquipStarLevel(part_index));
			if (NULL != star_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(star_cfg->add_maxhp);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(star_cfg->add_gongji);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(star_cfg->add_fangyu);
			}

			// 附灵 屏蔽
			/*for (int fuling_index = 0; fuling_index < MAX_ZHUANZHI_FULING_ATTR_COUNT; fuling_index ++)
			{

				const ZhuanzhiFulingStuffCfg * fuling_stuff_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetFulingStuffCfg(fuling_index);
				if (nullptr == fuling_stuff_cfg)
				{
					continue;
				}

				m_attrs_add.m_attrs[fuling_stuff_cfg->add_attr_type] += fuling_stuff_cfg->add_attr_val * static_cast<int>(equip_data.fuling_count_list[fuling_index]);
			}*/


			// 玉石总属性
			CharIntAttrs stone_attr;
			for (int slot_index = 0; slot_index < MAX_ZHUANZHI_STONE_SLOT; ++slot_index)
			{
				ZhuanzhiStoneItem &stone_item = m_param.stone_list[part_index].slot_list[slot_index];
				if (stone_item.Invalid())
				{
					continue;
				}

				const ZhuanzhiStoneCfg * stone_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhuanzhiStoneCfg(stone_item.stone_id);
				if (NULL == stone_cfg)
				{
					continue;
				}


				if (CharIntAttrs::IsRoleBaseAttr(stone_cfg->attr_type1) && stone_cfg->attr_val1 > 0)
				{
// 					stone_attr.m_attrs[stone_cfg->attr_type1] += stone_cfg->attr_val1;
// 					m_attrs_add.m_attrs[stone_cfg->attr_type1] += stone_cfg->attr_val1;
					stone_attr.AddValue(stone_cfg->attr_type1, static_cast<Attribute>(stone_cfg->attr_val1) );
					m_attrs_add.AddValue(stone_cfg->attr_type1, static_cast<Attribute>(stone_cfg->attr_val1) );

				}

				if (CharIntAttrs::IsRoleBaseAttr(stone_cfg->attr_type2) && stone_cfg->attr_val2 > 0)
				{
// 					stone_attr.m_attrs[stone_cfg->attr_type2] += stone_cfg->attr_val2;
// 					m_attrs_add.m_attrs[stone_cfg->attr_type2] += stone_cfg->attr_val2;
					stone_attr.AddValue(stone_cfg->attr_type2, static_cast<Attribute>(stone_cfg->attr_val2));
					m_attrs_add.AddValue(stone_cfg->attr_type2, static_cast<Attribute>(stone_cfg->attr_val2));
				}	
			}
			m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_YUSHI, stone_attr, m_attrs_add);
		}

		// 玉石精炼
		for (int part_index = 0; part_index < E_INDEX_ZHUANZHI::E_INDEX_MAX; ++part_index)
		{
			int refine_level = m_param.stone_list[part_index].refine_level;
			const ZhuanzhiStoneRefineCfg * refine_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhuanzhiStoneRefineCfg(refine_level);
			if (nullptr == refine_cfg)
			{
				continue;
			}

			for (int slot_index = 0; slot_index < MAX_ZHUANZHI_STONE_SLOT; ++slot_index)
			{
				ZhuanzhiStoneItem &stone_item = m_param.stone_list[part_index].slot_list[slot_index];
				if (stone_item.Invalid())
				{
					continue;
				}

				const ZhuanzhiStoneCfg * stone_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhuanzhiStoneCfg(stone_item.stone_id);
				if (NULL == stone_cfg)
				{
					continue;
				}

				if (CharIntAttrs::IsRoleBaseAttr(stone_cfg->attr_type1) && stone_cfg->attr_val1 > 0)
				{
//					m_attrs_add.m_attrs[stone_cfg->attr_type1] += (int)(stone_cfg->attr_val1 * (refine_cfg->per_attr_add * ROLE_ATTR_PER));
					m_attrs_add.AddValue(stone_cfg->attr_type1, static_cast<Attribute>(stone_cfg->attr_val1 * (refine_cfg->per_attr_add * ROLE_ATTR_PER)) );

				}

				if (CharIntAttrs::IsRoleBaseAttr(stone_cfg->attr_type2) && stone_cfg->attr_val2 > 0)
				{
//					m_attrs_add.m_attrs[stone_cfg->attr_type2] += (int)(stone_cfg->attr_val2 * (refine_cfg->per_attr_add * ROLE_ATTR_PER));
					m_attrs_add.AddValue(stone_cfg->attr_type2, static_cast<Attribute>(stone_cfg->attr_val2 * (refine_cfg->per_attr_add * ROLE_ATTR_PER)));
				}
			}
		}

		// 全身升星
		if (m_total_up_star_level > 0)
		{
			const ZhuanzhiUpStarTotalConfig *cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetUpStarTotalCfg(m_total_up_star_level, false);
			if (NULL != cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg->add_maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg->add_gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg->add_fangyu;
			}
		}

		// 全身玉石
		if (m_total_stone_level > 0)
		{
			const ZhuanzhiStoneTotalCfg *cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhuanzhiStoneTotalCfg(m_total_stone_level);
			if (NULL != cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg->fangyu;
			}
		}

		// 套装 （诛仙，诛神）
		this->GetSuitTotalAttr(m_attrs_add);

		// 仙品属性
		this->ReCalcXianpinAttr(m_attrs_add, m_role->GetPutOnLevel());

		//觉醒属性
		this->ReCalcAttrAwakening(m_attrs_add);

		//条件属性
		this->ReCalcAttrConditionAttr(m_attrs_add);

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_ZHUANZHI_EQUIP, m_attrs_add);
	}

	TianShuXunZhu::CheckGuBenTianShu(m_role, m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU]);

	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_ZHUANZHI_EQUIP, m_other_capability);

	m_param.capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_ZHUANZHI_EQUIP);

	base_add.Add(m_attrs_add.m_attrs, true);
	base_add.AddPercent(m_attrs_add.m_percent_attrs);
}

void ZhuanzhiEquip::ReCalcAttrConditionAttr(CharIntAttrs & base_add)
{
	for (int part_index = 0; part_index < E_INDEX_ZHUANZHI::E_INDEX_MAX; ++part_index)
	{
		int order = this->GetZhuanzhiEquipOrder(part_index);
		const ZhuanzhiEquipConditionAttrCfg *cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetConditionAttrCfg(part_index, order);
		if (nullptr == cfg)
		{
			continue;
		}
		size_t size = cfg->attribute_type.size();
		if (size != cfg->attribute_value.size())
		{
			continue;
		}
		int xianpin_count = this->GetXianpinCount(part_index);
		if (xianpin_count < cfg->xianpin_num)
		{
			continue;
		}
		for (size_t j = 0; j < cfg->attribute_type.size(); ++j)
		{
			base_add.AddValue(cfg->attribute_type[j], cfg->attribute_value[j]);
		}
	}
}

int ZhuanzhiEquip::GetEquipIndexByType(int equip_type)
{
	switch (equip_type)
	{
	case Equipment::E_TYPE_ZHUANZHI_WUQI:
		return E_INDEX_ZHUANZHI::E_INDEX_WUQI;

	case Equipment::E_TYPE_ZHUANZHI_YIFU:
		return E_INDEX_ZHUANZHI::E_INDEX_YIFU;

	case Equipment::E_TYPE_ZHUANZHI_HUSHOU:
		return E_INDEX_ZHUANZHI::E_INDEX_HUSHOU;

	case Equipment::E_TYPE_ZHUANZHI_YAODAI:
		return E_INDEX_ZHUANZHI::E_INDEX_YAODAI;

//	case Equipment::E_TYPE_ZHUANZHI_HUTUI:
//		return E_INDEX_ZHUANZHI::E_INDEX_HUTUI;

	case Equipment::E_TYPE_ZHUANZHI_TOUKUI:
		return E_INDEX_ZHUANZHI::E_INDEX_TOUKUI;

	case Equipment::E_TYPE_ZHUANZHI_XIANGLIAN:
		return E_INDEX_ZHUANZHI::E_INDEX_XIANGLIAN;

	case Equipment::E_TYPE_ZHUANZHI_SHOUZHUO:
		return E_INDEX_ZHUANZHI::E_INDEX_SHOUZHUO;

	case Equipment::E_TYPE_ZHUANZHI_JIEZHI:
		return E_INDEX_ZHUANZHI::E_INDEX_JIEZHI;

	case Equipment::E_TYPE_ZHUANZHI_XIEZI:
		return E_INDEX_ZHUANZHI::E_INDEX_XIEZI;

	case Equipment::E_TYPE_ZHUANZHI_YUPEI:
		return E_INDEX_ZHUANZHI::E_INDEX_YUPEI;

	default: return -1;
	}
}		

void ZhuanzhiEquip::OnOperaReq(int opera_type, int param1, int param2, int param3, int param4, int param5)
{
	switch (opera_type)
	{
	case Protocol::ZHUANZHI_EQUIP_OPERATE_TYPE_EQUIP_INFO:
	{
		this->SyncZhuanzhiEquipInfo();
	}
	break;

	case Protocol::ZHUANZHI_EQUIP_OPERATE_TYPE_STONE_INFO:
	{
		this->SyncStoneInfo();
	}
	break;

	case Protocol::ZHUANZHI_EQUIP_OPERATE_TYPE_SUIT_INFO:
	{
		this->SyncZhuanzhiSuitInfo();
	}
	break;

	case Protocol::ZHUANZHI_EQUIP_OPERATE_TYPE_TAKE_OFF:
	{
		this->TakeOff(param1, false);
	}

	case Protocol::ZHUANZHI_EQUIP_OPERATE_TYPE_UP_STAR:
	{
		this->EquipUpStar(param1);
	}
	break;

	case Protocol::ZHUANZHI_EQUIP_OPERATE_TYPE_FULING:
	{
		if (this->OnFuling(param1))
		{
			m_role->SendOperateResult(Protocol::SCOperateResult::OP_ZHUANZHI_FULING, 1);
		}
		else
		{
			m_role->SendOperateResult(Protocol::SCOperateResult::OP_ZHUANZHI_FULING, 0);
		}
	}
	break;

	case Protocol::ZHUANZHI_EQUIP_OPERATE_TYPE_INLAY_STONE:
	{
		this->OnInlayStone(param1, param2, param3);
	}
	break;

	case Protocol::ZHUANZHI_EQUIP_OPERATE_TYPE_UNINLAY_STONE:
	{
		this->OnUnInlayStone(param1, param2);
	}
	break;

	case Protocol::ZHUANZHI_EQUIP_OPERATE_TYPE_UP_LEVEL:
	{
		this->OnStoneLevelUp(param1, param2);
	}
	break;

	case Protocol::ZHUANZHI_EQUIP_OPERATE_TYPE_REFINE_STONE:
	{
		if (this->UpRefineLevel(param1, param2, param3))
		{
			m_role->SendOperateResult(Protocol::SCOperateResult::OP_ZHUANZHI_STONE_REFINE, 1);
		}
		else
		{
			m_role->SendOperateResult(Protocol::SCOperateResult::OP_ZHUANZHI_STONE_REFINE, 0);
		}
		
	}
	break;

	case Protocol::ZHUANZHI_EQUIP_OPERATE_TYPE_FORGE_SUIT:
	{
		this->OnForgeZhuanzhiSuit(param1, param2);
	}
	break;

	case Protocol::ZHUANZHI_EQUIP_OPERATE_TYPE_ZHIZUN_COMPOSE:
	{
		this->OnZhiZunEquipCompose(param1, param2, param3, param4, param5);
	}
	break;

	case Protocol::ZHUANZHI_EQUIP_OPERATE_TYPE_STONE_RESOLVE:
	{
		this->OnStoneResolve(param1);
	}
	break;

	case Protocol::ZHUANZHI_EQUIP_OPERATE_TYPE_STONE_CONVERT:
	{
		this->OnStoneConvert(param1);
	}
	break;

	case Protocol::ZHUANZHI_EQUIP_OPERATE_TYPE_AWAKENING_INFO:
	{
		this->SyncAwakeningAllInfo();
	}
	break;

	case Protocol::ZHUANZHI_EQUIP_OPERATE_TYPE_AWAKENING_WAKE:
	{
		this->Wake(param1, param2);
	}
	break;

	case Protocol::ZHUANZHI_EQUIP_OPERATE_TYPE_AWAKENING_REPLACE:
	{
		this->CoverAwakening(param1);
	}
	break;

	case Protocol::ZHUANZHI_EQUIP_OPERATE_TYPE_AWAKENING_LOCK:
	{
		this->LockAwakening(param1, param2);
	}
	break;

	case Protocol::ZHUANZHI_EQUIP_OPERATE_TYPE_AWAKENING_UNLOCK:
	{
		this->UnLockAwakening(param1, param2);
	}
	break;

	default:
		break;
	}
}

int ZhuanzhiEquip::PutOn(const ItemDataWrapper &item_wrapper)
{
	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_wrapper.item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType()) return -1;
	
	if (0 == item_wrapper.has_param) return -2;

	short equip_type = equip->GetEquipType();
	if (Equipment::E_TYPE_ZHUANZHI_MIN > equip_type || Equipment::E_TYPE_ZHUANZHI_MAX <= equip_type) return -3;

	int real_index = GetEquipIndexByType(equip_type);
	if (real_index < E_INDEX_ZHUANZHI::E_INDEX_WUQI || real_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX) return -4;

	// 检查能否穿戴
	if (!IsCanPuton(real_index, equip->GetOrder(), item_wrapper.item_id))
	{
		return -5;
	}

	ItemDataWrapper &itemdata = m_param.zhuanzhi_equip_list[real_index].zhuanzhi_equip_data;
	if (!itemdata.Invalid()) // 该位置有装备 脱之
	{
		int ret = this->TakeOff(real_index, true);
		if (0 != ret) 
		{
			return -6;
		}
	}

	if (!itemdata.Invalid() || item_wrapper.Invalid())	// 最后再加强检查
	{
		return -7;
	}

	itemdata.Reset(item_wrapper);

	EventHandler::Instance().OnPutOnZhuanZhiEquip(m_role, equip_type, equip->GetColor(), equip->GetLimitLevel());	// 放在Reset下面，嘉年华要检查物品数量

	this->RefreshEquipInfo();
	this->CheckActiveEquipEffect();

	this->CheckAllPartChangeSlot(ZHUANZHI_STONE_SLOT_OPEN_TYPE_ORDER, real_index);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANZHI_EQUIP);
	this->SyncZhuanzhiEquipInfo();
	this->SyncZhuanzhiSuitInfo();

	// log
	this->PrintLog("ZhuanzhiEquip::PutOn", real_index);

	if (real_index == E_INDEX_ZHUANZHI::E_INDEX_WUQI)
	{
		int wuqi_color = this->GetEquipNoticeColor(real_index);
		if (wuqi_color > 0 && m_role->GetScene() != nullptr)
		{
			Protocol::SCRoleAppeChange rac;
			rac.appe_type = Protocol::ROLE_APPE_CHANGE_TYPE_WUQICOLOR;
			rac.obj_id = m_role->GetId();
			rac.param = wuqi_color;

			m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void *)&rac, sizeof(rac));
		}
	}

	// 检查装备洗炼槽开启
	m_role->GetEquipBaptize()->CheckOpenPartAndSlot();

	return 0;
}

int ZhuanzhiEquip::TakeOff(short equip_index, bool is_from_puton)
{
	if (equip_index < E_INDEX_ZHUANZHI::E_INDEX_WUQI || equip_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX) return -1;

	ItemDataWrapper &itemdata = m_param.zhuanzhi_equip_list[equip_index].zhuanzhi_equip_data;
	if (itemdata.Invalid()) return -2;

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(itemdata.item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType() ||
		equip->GetEquipType() < Equipment::E_TYPE_ZHUANZHI_MIN || equip->GetEquipType() >= Equipment::E_TYPE_ZHUANZHI_MAX)
	{
		return -3;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return -5;
	}

	if (!knapsack->Put(itemdata, PUT_REASON_NO_NOTICE))
	{
		return -6;
	}

	// 检查返还套装材料
	this->CheckPutBackStuff(equip_index, equip->GetOrder());
	 
	itemdata.Reset();
	this->RefreshEquipInfo();

	if (!is_from_puton)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANZHI_EQUIP);

		this->SyncZhuanzhiEquipInfo();
	}

	// log
	this->PrintLog("ZhuanzhiEquip::TakeOff", equip_index, itemdata.item_id);

	return 0;
}

bool ZhuanzhiEquip::CheckKnapsack()
{
	if (m_role->GetSublock()->CheckHasLock())
	{
		return false;
	}
	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	return true;
}

bool ZhuanzhiEquip::IsCanPuton(int equip_part, int equip_order, ItemID item_id)
{
	if (equip_part < 0 || equip_part >= E_INDEX_MAX || equip_order <= 0 || equip_order > ItemPool::MAX_EQUIPMENT_ORDER)
	{
		return false;
	}

	const ZhuanzhiEquipCfg *limit_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhuanzhiEquipCfg(equip_part, equip_order);
	if (NULL == limit_cfg)
	{
		return false;
	}

	int role_level = limit_cfg->role_need_min_level;
	int prof_level = limit_cfg->role_need_min_prof_level;

	// 至尊装备限制
	{
		// 等级、转职等级
		const ZhiZunEquipCfg * zhizun_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhiZunEquipCfg(item_id);
		if (nullptr != zhizun_cfg)
		{
			role_level = zhizun_cfg->role_need_min_level;
			prof_level = zhizun_cfg->role_need_min_prof_level;
		}
	}

	// 特殊装备限制（如首充送的）
	{
		const SpecialEquipCfg * special_equip_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetSpecialEquipCfg(item_id);
		if (nullptr != special_equip_cfg)
		{
			role_level = special_equip_cfg->role_need_min_level;
			prof_level = special_equip_cfg->role_need_min_prof_level;
		}
	}

	if (m_role->GetLevel() < role_level)
	{
		m_role->NoticeNum(errornum::EN_ITEM_LEVEL_LIMIT);
		return false;
	}

	if (m_role->GetProfLevel() < prof_level)
	{
		gstr::SendError(m_role, "zhuanzhi_level_limit", prof_level);
		return false;
	}

	return true;
}

void ZhuanzhiEquip::ReCalZhuanZhiEquipTotalLevel()
{
	m_total_up_star_level = 0;
	m_total_stone_level = 0;

	for (int part_index = 0; part_index < Equipment::E_INDEX_MAX; part_index ++)
	{
		//星级
		{
			m_total_up_star_level += this->GetEquipStarLevel(part_index);

			if (this->GetEquipStarLevel(part_index) > m_max_up_star_level)
			{
				m_max_up_star_level = this->GetEquipStarLevel(part_index);
			}
		}

		// 玉石
		for (int slot_index = 0; slot_index < MAX_ZHUANZHI_STONE_SLOT; slot_index++)
		{
			int stone_id = m_param.stone_list[part_index].slot_list[slot_index].stone_id;
			const ZhuanzhiStoneCfg * stone_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhuanzhiStoneCfg(stone_id);
			if (nullptr == stone_cfg)
			{
				continue;
			}

			m_total_stone_level += stone_cfg->level;
		}
	}
}

void ZhuanzhiEquip::RefreshEquipInfo()
{
	ReCalZhuanZhiEquipTotalLevel();
	m_role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_ZHUANZHI_STONE_TOTAL_LEVEL); // 嘉年华
}

void ZhuanzhiEquip::GetEquipSpecialAttr(CharIntAttrs &tmp_attr, const CharIntAttrs &base_equip_attr)
{
	m_other_capability = 0;

	for (int part_index = 0; part_index < E_INDEX_ZHUANZHI::E_INDEX_MAX; ++part_index)
	{
		ItemDataWrapper &itemdata = m_param.zhuanzhi_equip_list[part_index].zhuanzhi_equip_data;
		if (itemdata.Invalid())
		{
			continue;
		}

		const Equipment *equip = GetZhuanzhiEquip(itemdata.item_id);
		if (nullptr == equip)
		{
			continue;
		}

		const ZhuanzhiEquipCfg *equip_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhuanzhiEquipCfg(part_index, equip->GetOrder());
		if (nullptr == equip_cfg)
		{
			continue;
		}
		if (equip->GetColor() < equip_cfg->special_attr_color)
		{
			continue;
		}

		// 策划需求，pvp/pve增减伤不增加战力
// 		tmp_attr.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += static_cast<Attribute>(equip_cfg->pvp_jianshang_per);
// 		tmp_attr.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += static_cast<Attribute>(equip_cfg->pvp_zengshang_per);
// 		tmp_attr.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER] += static_cast<Attribute>(equip_cfg->pve_jianshang_per);
// 		tmp_attr.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] += static_cast<Attribute>(equip_cfg->pve_zengshang_per);

		m_special_attr_pvp_jianshang = equip_cfg->pvp_jianshang_per;
		m_special_attr_pvp_zengshang = equip_cfg->pvp_zengshang_per;
		m_special_attr_pve_jianshang = equip_cfg->pve_jianshang_per;
		m_special_attr_pve_zengshang = equip_cfg->pve_zengshang_per;

		Attribute add_baoji = static_cast<Attribute>(base_equip_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (equip_cfg->sys_baoji_per * ROLE_ATTR_PER));
		Attribute add_jianren = static_cast<Attribute>(base_equip_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (equip_cfg->sys_kangbao_per * ROLE_ATTR_PER));
		tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += add_baoji;
		tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += add_jianren;

		m_other_capability += equip_cfg->add_cap;
	}

	m_other_capability += GetZhiZunEquipOtherCap();
}

const Equipment * ZhuanzhiEquip::GetZhuanzhiEquip(ItemID item_id)
{
	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_id);

	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType())
	{
		return nullptr;
	}

	if (equip->GetEquipType() < Equipment::E_TYPE_ZHUANZHI_MIN || equip->GetEquipType() >= Equipment::E_TYPE_ZHUANZHI_MAX)
	{
		return nullptr;
	}

	return equip;
}

void ZhuanzhiEquip::OnRoleLevelChange()
{
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANZHI_EQUIP);
}

bool ZhuanzhiEquip::EquipUpStar(short equip_index)
{
	if (m_role->GetSublock()->CheckHasLock()) return false;

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (equip_index < 0 || equip_index >= E_INDEX_MAX)
	{
		return false;
	}

	const Equipment *equip = NULL;

	ZhuanzhiEquipItem &equip_data = m_param.zhuanzhi_equip_list[equip_index];

	if (equip_data.zhuanzhi_equip_data.Invalid())
	{
		gstr::SendError(m_role, "select_zhuanzhi_equip_limit");
		return false;
	}

	equip = (const Equipment *)ITEMPOOL->GetItem(equip_data.zhuanzhi_equip_data.item_id);
	if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)
	{
		m_role->NoticeNum(errornum::EN_FUNC_ITEM_LOAD);
		return false;
	}

	int old_star_level = this->GetEquipStarLevel(equip_index);

	const ZhuanzhiUpStarConfig *star_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetUpStarCfg(equip_index, old_star_level + 1);
	if (NULL == star_cfg)
	{
		m_role->NoticeNum(errornum::EN_EQUIP_FUNC_CANNOT_UPSTAR);
		return false;
	}

	if (equip->GetLimitLevel() < star_cfg->equip_level)
	{
		m_role->NoticeNum(errornum::EN_EQUIP_FUNC_CANNOT_UPSTAR);
		return false;
	}

	// 生效
	if (m_role->GetRoleShop()->GetChestShopMojing() >= star_cfg->need_mojing - this->GetEquipStarExp(equip_index))
	{
		int mojing = star_cfg->need_mojing - this->GetEquipStarExp(equip_index);
		m_role->GetRoleShop()->AddChestShopMojing(-mojing, "equipUpstar");

		this->SetEquipStarExp(equip_index, 0);
		this->AddEquipStarLevel(equip_index);
		this->RefreshEquipInfo();

		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANZHI_EQUIP);
		//m_role->NoticeNum(noticenum::NT_EQUIP_UP_STAR_SUCC);

		//嘉年华
		m_role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_EQUIP_STAR);

		//升星传闻
		if (1 == star_cfg->is_broadcast)
		{
			EquipmentParam *equip_param = equip->CreateParamFromStruct(&equip_data.zhuanzhi_equip_data.param_data);
			if (NULL != equip_param)
			{
				EquipBaptizePartInfo *equip_baptize = m_role->GetEquipBaptize()->GetEquipBaptizePartInfo(equip_index);    // 装备洗炼

				const char *equip_param_string = EquipFunction::EquipParamDataClientString(UidToInt(m_role->GetUserId()), equip->GetItemId(), true, equip_data.zhuanzhi_equip_data.invalid_time, equip_param, equip_baptize);
				if (NULL != equip_param_string)
				{
					int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "zhuanzhi_equip_up_star", UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), 
						equip_data.zhuanzhi_equip_data.item_id, equip_param_string, this->GetEquipStarLevel(equip_index));
					if (length > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, length, SYS_MSG_CENTER_NOTICE_3);
					}
				}
			}
		}

		//全身升星传闻
		const ZhuanzhiUpStarTotalConfig *cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetUpStarTotalCfg(m_total_up_star_level, true);
		if (NULL != cfg)
		{
			if (1 == cfg->is_broadcast)
			{
				int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "zhuanzhi_equip_upstar_total", UidToInt(m_role->GetUserId()),
					m_role->GetName(), (int)m_role->GetCamp(), m_total_up_star_level, UPSTAR_TAOZHUANG, cfg->seq);
				if (length > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, length, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}
	}
	else
	{
		this->SetEquipStarExp(equip_index, this->GetEquipStarExp(equip_index) + m_role->GetRoleShop()->GetChestShopMojing());
		m_role->GetRoleShop()->AddChestShopMojing(-m_role->GetRoleShop()->GetChestShopMojing(), "equipUpstar");
	}

	this->SyncZhuanzhiEquipInfo();

	// log
	this->PrintLog("ZhuanzhiEquip::EquipUpStar", equip_index, old_star_level);

	return true;
}

bool ZhuanzhiEquip::OnFuling(int equip_index)
{
	bool is_succ = false;

	if (equip_index < 0 || equip_index >= E_INDEX_MAX)
	{
		return false;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	ZhuanzhiEquipItem &equip_data = m_param.zhuanzhi_equip_list[equip_index];

	int equip_order = this->GetZhuanzhiEquipOrder(equip_index);
	if (equip_order <= 0)
	{
		return false;
	}

	const ZhuanzhiEquipFulingCfg * fuling_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhuanzhiFulingCfg(equip_order);
	if (nullptr == fuling_cfg)
	{
		gstr::SendError(m_role, "get_cfg_error");
		return false;
	}

	//消耗

	for (int i = 0; i < MAX_ZHUANZHI_FULING_ATTR_COUNT; i ++)
	{
		if (equip_data.fuling_count_list[i] >= fuling_cfg->fuling_max_count)
		{
			continue;
		}

		const ZhuanzhiFulingStuffCfg * fuling_stuff_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetFulingStuffCfg(i);
		if (nullptr == fuling_stuff_cfg)
		{
			continue;
		}

		if (knapsack->ConsumeItem(fuling_stuff_cfg->stuff_id, 1, "zhuanzhi_equip_fuling"))
		{
			equip_data.fuling_count_list[i] ++;
			is_succ = true;
		}
	}

	// 生效
	if (is_succ)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANZHI_EQUIP);
		gstr::SendNotice(m_role, "zuanzhi_fuling_succ");

		this->SyncZhuanzhiEquipInfo();
	}
	else
	{
		m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
	}

	// log
	this->PrintLog("ZhuanzhiEquip::OnFuling", equip_index);

	return is_succ;
}

void ZhuanzhiEquip::SyncZhuanzhiEquipInfo()
{
	static Protocol::SCZhuanzhiEquipInfo eia; 

	for (int i = 0; i < E_INDEX_ZHUANZHI::E_INDEX_MAX; ++i)
	{
		eia.zhuanzhi_equip_list[i].Reset();
	}

	for (int i = 0; i < E_INDEX_ZHUANZHI::E_INDEX_MAX; ++i)
	{
		if (m_param.zhuanzhi_equip_list[i].zhuanzhi_equip_data.Invalid()) continue;

		eia.zhuanzhi_equip_list[i].zhuanzhi_equip_data.Reset(m_param.zhuanzhi_equip_list[i].zhuanzhi_equip_data);

		ARRAYCPY(eia.zhuanzhi_equip_list[i].fuling_count_list, m_param.zhuanzhi_equip_list[i].fuling_count_list);

		eia.zhuanzhi_equip_list[i].star_level = m_param.zhuanzhi_equip_list[i].star_level;
		eia.zhuanzhi_equip_list[i].star_exp = m_param.zhuanzhi_equip_list[i].star_exp;
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&eia, sizeof(eia));
}

void ZhuanzhiEquip::OnInlayStone(int part_index, int slot_index, int bag_index)
{
	if (part_index < 0 || part_index >= E_INDEX_MAX || slot_index < 0 || slot_index >= MAX_ZHUANZHI_STONE_SLOT)
	{
		return;
	}

	if (m_role->GetSublock()->CheckHasLock()) return;

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return ;
	}

	ZhuanzhiStonesPartData &stone_data = m_param.stone_list[part_index];

	if (!IS_BIT_SET(stone_data.slot_open_flag, slot_index))
	{
		gstr::SendError(m_role, "zhuanzhi_stone_slot_not_open");
		return;
	}

	ItemGridData grid_data;
	if (!m_role->GetKnapsack()->GetItemGridData(bag_index, &grid_data) || grid_data.Invalid())
	{
		gstr::SendError(m_role, "select_stone_limit");
		return;
	}

	const ZhuanzhiStoneCfg * stone_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhuanzhiStoneCfg(grid_data.item_id);
	if (nullptr == stone_cfg)
	{
		gstr::SendError(m_role, "get_cfg_error");
		return; 
	}

	if (m_role->GetLevel() < stone_cfg->role_level_limit)
	{
		gstr::SendError(m_role, "role_level_limit");
		return;
	}

	if (!this->IsCanInlayStone(part_index, slot_index, grid_data.item_id))
	{
		return;
	}

	// 消耗
	if (!knapsack->ConsumeItemByIndex(bag_index, 1, "zhuanzhi_stone_inlay"))
	{
		m_role->NoticeItemLack(grid_data.item_id, 1);
		return;
	}

	//生效
	stone_data.slot_list[slot_index].stone_id = grid_data.item_id;
	stone_data.slot_list[slot_index].is_bind = grid_data.is_bind;

	this->RefreshEquipInfo();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANZHI_EQUIP);
	gstr::SendNotice(m_role, "zhuanzhi_stone_inlay_succ");

	this->SyncStoneInfo();

	// log
	this->PrintLog("ZhuanzhiEquip::OnInlayStone", part_index, slot_index, bag_index);
}

bool ZhuanzhiEquip::OnUnInlayStone(int part_index, int slot_index, bool is_from_inlay)
{
	if (part_index < 0 || part_index >= E_INDEX_MAX || slot_index < 0 || slot_index >= MAX_ZHUANZHI_STONE_SLOT)
	{
		return false;
	}

	if (m_role->GetSublock()->CheckHasLock()) return false;

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	ZhuanzhiStoneItem &stone_data = m_param.stone_list[part_index].slot_list[slot_index];
	if (stone_data.stone_id <= 0)
	{
		gstr::SendError(m_role, "select_stone_limit");
		return false;
	}

	const ZhuanzhiStoneCfg * stone_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhuanzhiStoneCfg(stone_data.stone_id);
	if (nullptr == stone_cfg)
	{
		gstr::SendError(m_role, "get_cfg_error");
		return false;
	}

	// 放回背包
	ItemConfigData zhuanzhi_stone_item(stone_data.stone_id, 1 == stone_data.is_bind, 1);
	knapsack->Put(zhuanzhi_stone_item, PUT_REASON_ZHAUNGZHI_SRTONE_UNLAY);

	//清除
	int old_stone_id = stone_data.stone_id;
	stone_data.Reset();

	if (!is_from_inlay)
	{
		this->RefreshEquipInfo();
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANZHI_EQUIP);

		this->SyncStoneInfo();
	}

	// log
	this->PrintLog("ZhuanzhiEquip::OnUnInlayStone", part_index, slot_index, old_stone_id);

	return true;
}

void ZhuanzhiEquip::OnStoneLevelUp(int part_index, int slot_index)
{
	if (part_index < 0 || part_index >= E_INDEX_MAX || slot_index < 0 || slot_index >= MAX_ZHUANZHI_STONE_SLOT)
	{
		return;
	}

	if (m_role->GetSublock()->CheckHasLock()) return;

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	ZhuanzhiStoneItem &stone_data = m_param.stone_list[part_index].slot_list[slot_index];
	if (stone_data.stone_id <= 0)
	{
		gstr::SendError(m_role, "select_stone_limit");
		return;
	}

	const ZhuanzhiStoneLevelCfg * level_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhuanzhiStoneLevelCfg(stone_data.stone_id);
	if (nullptr == level_cfg)
	{
		gstr::SendError(m_role, "get_cfg_error");
		return;
	}

	const ZhuanzhiStoneCfg * stone_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhuanzhiStoneCfg(level_cfg->new_stone_item_id);
	if (nullptr == stone_cfg)
	{
		gstr::SendError(m_role, "get_cfg_error");
		return;
	}

	if (m_role->GetLevel() < stone_cfg->role_level_limit)
	{
		gstr::SendError(m_role, "role_level_limit");
		return;
	}

	//检查材料
	int need_stuff_num = level_cfg->need_num - 1;
	int old_stone_item_id = stone_data.stone_id;
	bool is_stuff_enough = false;

	ItemExtern::ItemConsumeConfigList stuff_list;
	
	for (int count = 0; count < 15; count ++)
	{
		int bag_stuff_count = knapsack->Count(old_stone_item_id);
		if (bag_stuff_count >= need_stuff_num)
		{
			stuff_list.add_item(ItemExtern::ItemConsumeConfig(old_stone_item_id, false, need_stuff_num));
			is_stuff_enough = true;
			break;		
		}
		else
		{
			const ZhuanzhiStoneLevelCfg * reverse_level_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhuanzhiStoneLevelReverseCfg(old_stone_item_id);
			if (nullptr == reverse_level_cfg)
			{
				continue;
			}
			if (bag_stuff_count > 0)
			{
				stuff_list.add_item(ItemExtern::ItemConsumeConfig(old_stone_item_id, false, bag_stuff_count));
			}

			old_stone_item_id = reverse_level_cfg->old_stone_item_id;
			need_stuff_num = reverse_level_cfg->need_num * (need_stuff_num - bag_stuff_count);
		}
	}

	if (!is_stuff_enough)
	{
		m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
		return;
	}
	else
	{
		// 消耗
		if (!stuff_list.done(*m_role, "ZhuanzhiEquip::OnStoneLevelUp"))
		{
			return;
		}
	}

	// 生效
	m_param.stone_list[part_index].slot_list[slot_index].stone_id = level_cfg->new_stone_item_id;

	this->RefreshEquipInfo();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANZHI_EQUIP);
	gstr::SendNotice(m_role, "zhuanzhi_stone_inlay_succ");

	this->SyncStoneInfo();

	// log
	this->PrintLog("ZhuanzhiEquip::OnStoneLevelUp", part_index, slot_index, level_cfg->old_stone_item_id);
}

bool ZhuanzhiEquip::UpRefineLevel(int part_index, int seq, int is_autobuy)
{
	if (part_index < 0 || part_index >= E_INDEX_MAX || seq < 0 || seq >= MAX_REFINE_STUFF_COUNT)
	{
		return false;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	int equip_order = this->GetZhuanzhiEquipOrder(part_index);
	if (equip_order <= 0)
	{
		return false;
	}

	// 阶数判断
	int equip_order_limit = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetOtherCfg().refine_min_order;
	if (equip_order < equip_order_limit)
	{
		gstr::SendError(m_role, "equip_order_limit");
		return false;
	}

	int old_refine_level = m_param.stone_list[part_index].refine_level;
	int next_refine_level = old_refine_level + 1;
	const ZhuanzhiStoneRefineCfg * next_refine_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhuanzhiStoneRefineCfg(next_refine_level);
	if (nullptr == next_refine_cfg)
	{
		gstr::SendError(m_role, "stone_equip_refine_level_full");
		return false;
	}

	const ZhuanzhiStoneRefineCfg * old_refine_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhuanzhiStoneRefineCfg(old_refine_level);
	if (nullptr == old_refine_cfg)
	{
		gstr::SendError(m_role, "get_cfg_error");
		return false;
	}

	const RefineStuffCfg * stuff_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetRefineStuffCfg(part_index, seq);
	if (nullptr == stuff_cfg)
	{
		gstr::SendError(m_role, "get_cfg_error");
		return false;
	}

	// 消耗
	ItemExtern::ItemConsumeConfigList stuff_list;
	stuff_list.add_item(ItemExtern::ItemConsumeConfig(stuff_cfg->stuff_id, 1 == is_autobuy, 1));
	if (!stuff_list.done(*m_role, "ZhuanzhiEquip::UpRefineLevel"))
	{
		m_role->NoticeItemLack(stuff_cfg->stuff_id, 1);
		return false;
	}
	
	// 计算提升的精炼等级
	int left_add_refine_val = stuff_cfg->add_refine;
	int loop_count = 50;
	for (int i = 0; i < loop_count; i ++)
	{
		if (left_add_refine_val <= 0)
		{
			break;
		}

		const ZhuanzhiStoneRefineCfg * tmp_refine_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhuanzhiStoneRefineCfg(m_param.stone_list[part_index].refine_level);
		if (nullptr == tmp_refine_cfg)
		{
			return false;
		}

		int diff_add_refine_val = tmp_refine_cfg->consume_refine_val - m_param.stone_list[part_index].refine_val;
		if (left_add_refine_val < diff_add_refine_val)
		{
			m_param.stone_list[part_index].refine_val += left_add_refine_val;
			left_add_refine_val = 0;
		}
		else
		{
			m_param.stone_list[part_index].refine_val = 0;
			m_param.stone_list[part_index].refine_level += 1;

			left_add_refine_val -= diff_add_refine_val;
		}
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANZHI_EQUIP);

	this->SyncStoneInfo();

	// log
	this->PrintLog("ZhuanzhiEquip::UpRefineLevel", part_index, seq, old_refine_level);

	return true;
}

void ZhuanzhiEquip::OnStoneResolve(int bag_index)
{
	if (!CheckKnapsack())
	{
		return;
	}

	if (bag_index < 0 || bag_index >= ItemNamespace::MAX_KNAPSACK_GRID_NUM)
	{
		return;
	}

	ItemGridData item_grid_data;
	m_role->GetKnapsack()->GetItemGridData(bag_index, &item_grid_data);
	if (item_grid_data.Invalid())
	{
		return;
	}

	int per_item_score = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetStoneResolveScore(item_grid_data.item_id);
	int total_get_score = per_item_score * item_grid_data.num;
	if (total_get_score <= 0)
	{
		gstr::SendError(m_role, "get_cfg_error");
		return;
	}

	if (!m_role->GetKnapsack()->ConsumeItem(item_grid_data.item_id, item_grid_data.num, "ZhuanzhiEquip::OnStoneResolve"))
	{
		return;
	}

	m_param.stone_score += total_get_score;

	this->SyncStoneInfo();

	// 用于客户端兑换商店显示
	m_role->GetRoleChestShop()->SendScoreInfo();
	m_role->GetRoleChestShop()->SendScoreInfoNotice(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, total_get_score);

	// log
	this->PrintLog("ZhuanzhiEquip::OnStoneResolve", bag_index, total_get_score, m_param.stone_score);
}

void ZhuanzhiEquip::OnStoneConvert(int seq)
{
	if (!CheckKnapsack())
	{
		return;
	}

	if (seq < 0)
	{
		return;
	}

	const StoneConvertCfg *convert_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetStoneConvertCfg(seq);
	if (nullptr == convert_cfg)
	{
		return;
	}

	unsigned int need_score = static_cast<unsigned int>(convert_cfg->convert_need_score);
	if (m_param.stone_score < need_score)
	{
		gstr::SendError(m_role, "zhuanzhi_stone_score_not_enough");
		return;
	}

	m_param.stone_score -= convert_cfg->convert_need_score;

	ItemConfigData convert_stone_id(convert_cfg->convert_stone_id, false, 1);
	m_role->GetKnapsack()->Put(convert_stone_id, PUT_REASON_ZHUANZHI_SOTNE_CONVERT);

	this->SyncStoneInfo();

	// log
	this->PrintLog("ZhuanzhiEquip::OnStoneConvert", seq, convert_cfg->convert_need_score, m_param.stone_score);
}

bool ZhuanzhiEquip::OnComsumeStoneScore(int consume_score)
{
	if (consume_score <= 0) return false;

	if (m_param.stone_score < static_cast<unsigned int>(consume_score))
	{
		gstr::SendError(m_role, "zhuanzhi_stone_score_not_enough");
		return false;
	}

	m_param.stone_score -= consume_score;
	
	// 用于客户端兑换商店显示
	m_role->GetRoleChestShop()->SendScoreInfo();

	// log
	this->PrintLog("ZhuanzhiEquip::OnStoneConvert", consume_score, m_param.stone_score);

	return true;
}

void ZhuanzhiEquip::CheckAllPartChangeSlot(int open_type, int part_index)
{
	bool is_slot_change = false;
	switch (open_type)
	{
	case ZHUANZHI_STONE_SLOT_OPEN_TYPE_ORDER:
		{
			if (part_index < 0 || part_index >= E_INDEX_MAX)
			{
				return;
			}

			for (int slot_index = 0; slot_index < MAX_ZHUANZHI_STONE_SLOT; slot_index++)
			{
				const ZhuanzhiStoneSlotCfg * slot_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhuanzhiStoneSlotCfg(part_index, slot_index);
				if (nullptr == slot_cfg)
				{
					continue;
				}

				int equip_order = this->GetZhuanzhiEquipOrder(part_index);
				if (equip_order <= 0)
				{
					continue;
				}

				if (LOGIC_CONFIG->GetZhuanzhiEquipConfig().IsVipLimitSlot(part_index, slot_index))
				{
					continue;
				}

				if (equip_order >= slot_cfg->order_limit)
				{
					// 开孔
					if (!IS_BIT_SET(m_param.stone_list[part_index].slot_open_flag, slot_index))
					{
						SET_BIT(m_param.stone_list[part_index].slot_open_flag, slot_index);
						is_slot_change = true;

						// log
						this->PrintLog("ZhuanzhiEquip::CheckAllPartOpenSlot", part_index);
					}
				}
				else
				{
					//关孔
					if (IS_BIT_SET(m_param.stone_list[part_index].slot_open_flag, slot_index))
					{
						// 检查返还
						this->CheckPutBackStone(part_index, slot_index);

						CLEAR_BIT(m_param.stone_list[part_index].slot_open_flag, slot_index);
						is_slot_change = true;

						// log
						this->PrintLog("ZhuanzhiEquip::CheckAllPartOpenSlot", part_index);
					}
				}
			}
		}
		break;

	case ZHUANZHI_STONE_SLOT_OPEN_TYPE_VIP:
		{
			int vip_level = m_role->GetVip()->GetVipLevel();

			for (int part_index = 0; part_index < E_INDEX_ZHUANZHI::E_INDEX_MAX; part_index ++)
			{
				for (int slot_index = 0; slot_index < MAX_ZHUANZHI_STONE_SLOT; slot_index ++)
				{
					if (!LOGIC_CONFIG->GetZhuanzhiEquipConfig().IsVipLimitSlot(part_index, slot_index))
					{
						continue;
					}

					const ZhuanzhiStoneSlotCfg * slot_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhuanzhiStoneSlotCfg(part_index, slot_index);
					if (nullptr == slot_cfg)
					{
						continue;
					}

					if (IS_BIT_SET(m_param.stone_list[part_index].slot_open_flag, slot_index))
					{
						continue;
					}

					if (vip_level >= slot_cfg->vip_level_limit && slot_cfg->vip_level_limit > 0)
					{
						SET_BIT(m_param.stone_list[part_index].slot_open_flag, slot_index);

						is_slot_change = true;

						// log
						this->PrintLog("ZhuanzhiEquip::CheckAllPartOpenSlot", part_index);
					}
				}
			}

		}
		break;

	default:
		break;
	}

	if (is_slot_change)
	{
		this->SyncStoneInfo();
	}
}

void ZhuanzhiEquip::CheckPutBackStone(int part_index, int slot_index)
{
	if (part_index < 0 || part_index >= E_INDEX_MAX || slot_index < 0 || slot_index >= MAX_ZHUANZHI_STONE_SLOT)
	{
		return;
	}

	ZhuanzhiStoneItem &stone_itme = m_param.stone_list[part_index].slot_list[slot_index];
	if (stone_itme.Invalid())
	{
		return;
	}

	ItemConfigData item(stone_itme.stone_id, 1 == stone_itme.is_bind, 1);

	m_role->GetKnapsack()->PutOrMail(item, PUT_REASON_ZHAUNGZHI_SRTONE_UNLAY);

	stone_itme.Reset();
}

int ZhuanzhiEquip::GetTotalStoneLevel()
{
	return m_total_stone_level;
}

int ZhuanzhiEquip::GetZhuanzhiEquipOrder(int part_index)
{
	if (part_index < 0 || part_index >= E_INDEX_MAX)
	{
		return 0;
	}

	ItemDataWrapper &itemdata = m_param.zhuanzhi_equip_list[part_index].zhuanzhi_equip_data;
	if (itemdata.Invalid())
	{
		return 0;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(itemdata.item_id);
	if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)
	{
		return 0;
	}

	int equip_order = equip->GetOrder();

	return equip_order;
}

bool ZhuanzhiEquip::IsCanInlayStone(int part_index, int slot_index, int stone_id)
{
	if (part_index < 0 || part_index >= E_INDEX_MAX || slot_index < 0 || slot_index >= MAX_ZHUANZHI_STONE_SLOT)
	{
		return false;
	}

	// 判断是否已有玉石
	if (!m_param.stone_list[part_index].slot_list[slot_index].Invalid())
	{
		if (!this->OnUnInlayStone(part_index, slot_index, true))
		{
			return false;
		}
	}

	// 判断玉石类型镶嵌限制
	int inlay_stone_type = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhuanzhiStoneType(stone_id);
	const ZhuanzhiStoneSlotCfg * slot_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhuanzhiStoneSlotCfg(part_index, slot_index);
	if (nullptr == slot_cfg)
	{
		gstr::SendError(m_role, "get_cfg_error");
		return false;
	}

	if (inlay_stone_type != slot_cfg->stone_type_limit)
	{
		gstr::SendError(m_role, "stone_type_limit");
		return false;
	}

	return true;
}

void ZhuanzhiEquip::SyncStoneInfo()
{
	static Protocol::SCZhuanzhiStoneInfo msg;
	msg.stone_score = m_param.stone_score;

	memset(msg.stone_list, 0, sizeof(msg.stone_list));

	UNSTD_STATIC_CHECK(sizeof(msg.stone_list) == sizeof(m_param.stone_list));
	memcpy(msg.stone_list, m_param.stone_list, sizeof(msg.stone_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&msg, sizeof(msg));
}

void ZhuanzhiEquip::OnForgeZhuanzhiSuit(int suit_index, int equip_index)
{
	// 检查
	if (suit_index <= ZHUANZHI_SUIT_TYPE_INVALID || suit_index >= ZHUANZHI_SUIT_TYPE_MAX || equip_index < 0 || equip_index >= E_INDEX_MAX)
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (0 != m_param.zhuanzhi_suit_order_list[suit_index].order_list[equip_index])
	{
		gstr::SendError(m_role, "zhuanzhi_suit_already_active");
		return;
	}

	// 检查前面套装对应激活情况（第一套不用检查）
	if (suit_index > 0)
	{
		if (0 == m_param.zhuanzhi_suit_order_list[suit_index - 1].order_list[equip_index])
		{
			gstr::SendError(m_role, "zhuanzhi_suit_pre_not_active");
			return;
		}
	}

	for (int i = suit_index + 1; i < ZHUANZHI_SUIT_TYPE_MAX; ++i)
	{

		if (0 != m_param.zhuanzhi_suit_order_list[i].order_list[equip_index])
		{
			gstr::SendError(m_role, "zhuanzhi_suit_already_active");
			return;
		}
	}

	ZhuanzhiEquipItem &equip_data = m_param.zhuanzhi_equip_list[equip_index];

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(equip_data.zhuanzhi_equip_data.item_id);
	if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)
	{
		m_role->NoticeNum(errornum::EN_EQUIP_NOT_EXIST);
		return;
	}

	// 限制条件判断
	int equip_color = equip->GetColor();
	int equip_order = equip->GetOrder();

	const SuitForgeLimitCfg *forge_limit_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetSuitForgeLimitCfg(suit_index);
	if (nullptr == forge_limit_cfg)
	{
		m_role->NoticeNum(errornum::EN_EQUIP_FUNC_CONFIG_ERROR);
		return;
	}

	if (equip_color < forge_limit_cfg->suit_min_color)
	{
		gstr::SendError(m_role, "zhuanzhi_suit_limit_color");
		return;
	}

	if (equip_order < forge_limit_cfg->suit_min_order)
	{
		gstr::SendError(m_role, "zhuanzhi_suit_limit_order", forge_limit_cfg->suit_min_order);
		return;
	}

	const SuitOneEquipActiveCfg * act_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetSuitOneEquipActCfg(m_role->GetBaseProf(), suit_index, equip_index, equip_order);
	if (nullptr == act_cfg)
	{
		m_role->NoticeNum(errornum::EN_EQUIP_FUNC_CONFIG_ERROR);
		return;
	}

	//ItemID item1_id = act_cfg->stuff_1_id;
	//int num1 = act_cfg->stuff_1_num;
	//ItemID item2_id = act_cfg->stuff_2_id;
	//int num2 = act_cfg->stuff_2_num;
	//ItemID item3_id = act_cfg->stuff_3_id;
	//int num3 = act_cfg->stuff_3_num;

	//// 检查材料数量
	//if (item1_id > 0)
	//{
	//	if (knapsack->Count(item1_id) < num1)
	//	{
	//		m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
	//		return;
	//	}
	//}
	//if (item2_id > 0)
	//{
	//	if (knapsack->Count(item2_id) < num2)
	//	{
	//		m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
	//		return;
	//	}
	//}
	//if (item3_id > 0)
	//{
	//	if (knapsack->Count(item3_id) < num3)
	//	{
	//		m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
	//		return;
	//	}
	//}

	g_consume_list.Reset();
	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

	size_t stuff_list_count = 0;
	if (act_cfg->stuff_1_id > 0 && act_cfg->stuff_1_num > 0 && stuff_list_count < static_array_size(stuff_list))
	{
		stuff_list[stuff_list_count].item_id = act_cfg->stuff_1_id;
		stuff_list[stuff_list_count].num = act_cfg->stuff_1_num;
		stuff_list[stuff_list_count].buyable = false;
		stuff_list_count++;
	}
	if (act_cfg->stuff_2_id > 0 && act_cfg->stuff_2_num > 0 && stuff_list_count < static_array_size(stuff_list))
	{
		stuff_list[stuff_list_count].item_id = act_cfg->stuff_2_id;
		stuff_list[stuff_list_count].num = act_cfg->stuff_2_num;
		stuff_list[stuff_list_count].buyable = false;
		stuff_list_count++;
	}
	if (act_cfg->stuff_3_id > 0 && act_cfg->stuff_3_num > 0 && stuff_list_count < static_array_size(stuff_list))
	{
		stuff_list[stuff_list_count].item_id = act_cfg->stuff_3_id;
		stuff_list[stuff_list_count].num = act_cfg->stuff_3_num;
		stuff_list[stuff_list_count].buyable = false;
		stuff_list_count++;
	}

	if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_list_count, stuff_list, &g_consume_list, true))
	{
		return;
	}

	//储存消耗的物品信息
	OneSuitInfo& param_info = m_param.zhuanzhi_suit_order_list[suit_index];
	for (short i = 0; i < g_consume_list.count; i++)
	{
		ItemGridData* item_data = m_role->GetKnapsack()->GetItemGridData(g_consume_list.index_list[i]);
		if (!item_data || item_data->Invalid())
		{
			continue;
		}
		bool is_put = false;
		if (item_data->is_bind)
		{
			for (auto& value : param_info.stuff_bind_list[equip_index])
			{
				if (value.item_id == 0)
				{
					value.item_id = item_data->item_id;
					value.count = g_consume_list.num_list[i];
					is_put = true;
					break;
				}
				else if (value.item_id == item_data->item_id)
				{
					value.count += g_consume_list.num_list[i];
					is_put = true;
					break;
				}
			}
		}
		else
		{
			for (auto& value : param_info.stuff_unbind_list[equip_index])
			{
				if (value.item_id == 0)
				{
					value.item_id = item_data->item_id;
					value.count = g_consume_list.num_list[i];
					is_put = true;
					break;
				}
				else if (value.item_id == item_data->item_id)
				{
					value.count += g_consume_list.num_list[i];
					is_put = true;
					break;
				}
			}

		}
		if (!is_put)
		{
			return;
		}
	}

	// 消耗
	if (g_consume_list.count <= 0 || !m_role->GetKnapsack()->ConsumeItemByIndexList(g_consume_list.count, g_consume_list.index_list, g_consume_list.num_list, "EquipCompound"))
	{
		return;
	}




	//// 消耗
	//if (item1_id > 0)
	//{
	//	knapsack->ConsumeItem(item1_id, num1, "ZhuanzhiEquip::OnActZhuanzhiSuit");
	//}
	//if (item2_id > 0)
	//{
	//	knapsack->ConsumeItem(item2_id, num2, "ZhuanzhiEquip::OnActZhuanzhiSuit");
	//}
	//if (item3_id > 0)
	//{
	//	knapsack->ConsumeItem(item3_id, num3, "ZhuanzhiEquip::OnActZhuanzhiSuit");
	//}

	// 给予
	m_param.zhuanzhi_suit_order_list[suit_index].order_list[equip_index] = equip_order;
	if (suit_index > 0) // 使前面套装失效
	{
		for (int i = 0; i < suit_index; ++i)
		{
			m_param.zhuanzhi_suit_order_list[i].order_list[equip_index] = 0;
		}
	}

	long long suit_old_cap = m_role->GetCapability()->GetTotalCapability();
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANZHI_EQUIP);
	long long suit_new_cap = m_role->GetCapability()->GetTotalCapability();

	int cur_order_count = 0;
	int group_type = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetGroupType(equip_index);
	if (group_type >= ZHUANZHI_SUIT_GROUP_TYPE_FANG)
	{
		cur_order_count = this->ReCalcSameOrderCount(suit_index, group_type, equip_order);
	}
	
	bool is_active_suit = LOGIC_CONFIG->GetZhuanzhiEquipConfig().IsActiveFangSuit(m_role->GetBaseProf(), suit_index, equip_order, cur_order_count);
	if (is_active_suit)
	{
		//传闻
		int change_cap = static_cast<int>(suit_new_cap - suit_old_cap);

		int sendlen = gstr::SysMsgContent(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "zhuanzhi_suit_active_notice", m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(),
			suit_index, equip_index, cur_order_count, suit_index, equip_index, change_cap);
		if (sendlen > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
		}
	}

	// 发送
	//gstr::SendNotice(m_role, "zhuanzhi_suit_forge_succ");
	this->SyncZhuanzhiSuitInfo();

	TianShuXunZhu::CheckShouHuTianShu(m_role, equip_index, equip_order);

	EventHandler::Instance().OnForgeZhuanZhiSuit(m_role);

	// log
	this->PrintLog("ZhuanzhiEquip::OnForgeZhuanzhiSuit", equip_index);
}

void ZhuanzhiEquip::CheckPutBackStuff(int equip_index, int equip_order)
{
	if (equip_index < 0 || equip_index >= E_INDEX_MAX || equip_order <= 0)
	{
		return;
	}

	ItemConfigData put_item_info[MAX_ATTACHMENT_ITEM_NUM];
	// 当前部位所涉及的所有套装的套装石卸下来，将之放入背包
	for (int suit_index = ZHUANZHI_SUIT_TYPE_MAX - 1; suit_index >= 0; --suit_index)
	{
		m_param.zhuanzhi_suit_order_list[suit_index].order_list[equip_index] = 0;
		memset(put_item_info, 0, sizeof(put_item_info));
		int put_list_count = 0;
		for (auto& value : m_param.zhuanzhi_suit_order_list[suit_index].stuff_bind_list[equip_index])
		{
			if (value.item_id > 0 && value.count > 0)
			{
				put_item_info[put_list_count].is_bind = true;
				put_item_info[put_list_count].item_id = value.item_id;
				put_item_info[put_list_count].num = value.count;
				if (++put_list_count == MAX_ATTACHMENT_ITEM_NUM)
				{
					m_role->GetKnapsack()->PutListOrMail(put_item_info, PUT_REASON_ZHUANZHI_SUIT_STUFF);
					put_list_count = 0;
					memset(put_item_info, 0, sizeof(put_item_info));
				}
			}
			value.item_id = 0;
			value.count = 0;
		}

		for (auto& value : m_param.zhuanzhi_suit_order_list[suit_index].stuff_unbind_list[equip_index])
		{
			if (value.item_id > 0 && value.count > 0)
			{
				put_item_info[put_list_count].is_bind = false;
				put_item_info[put_list_count].item_id = value.item_id;
				put_item_info[put_list_count].num = value.count;
				if (++put_list_count == MAX_ATTACHMENT_ITEM_NUM)
				{
					m_role->GetKnapsack()->PutListOrMail(put_item_info, PUT_REASON_ZHUANZHI_SUIT_STUFF);
					put_list_count = 0;
					memset(put_item_info, 0, sizeof(put_item_info));
				}
			}
			value.item_id = 0;
			value.count = 0;
		}

		if (put_list_count > 0)
		{
			m_role->GetKnapsack()->PutListOrMail(put_item_info, PUT_REASON_ZHUANZHI_SUIT_STUFF);
			put_list_count = 0;
			memset(put_item_info, 0, sizeof(put_item_info));
		}




		//for (int i = 0; i <= suit_index; ++i)
		//{
		//	m_param.zhuanzhi_suit_order_list[i].order_list[equip_index] = 0;

		//	const SuitOneEquipActiveCfg * act_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetSuitOneEquipActCfg(m_role->GetBaseProf(), i, equip_index, equip_order);
		//	if (nullptr == act_cfg)
		//	{
		//		continue;
		//	}

		//	// 根据配置表拿下套装石
		//	ItemID item1_id = act_cfg->stuff_1_id;
		//	int num1 = act_cfg->stuff_1_num;
		//	ItemID item2_id = act_cfg->stuff_2_id;
		//	int num2 = act_cfg->stuff_2_num;
		//	ItemID item3_id = act_cfg->stuff_3_id;
		//	int num3 = act_cfg->stuff_3_num;

		//	if (item1_id > 0 && num1 > 0)
		//	{
		//		const ItemBase *item_base = ITEMPOOL->GetItem(item1_id);
		//		if (NULL != item_base)
		//		{
		//			const int pile_max_num = item_base->GetPileLimit();
		//			const int put_times = num1 / pile_max_num;	// 堆叠数量的倍数
		//			const int remain_num = num1 % pile_max_num;	// 剩余数量
		//			if (put_times > 0)
		//			{
		//				for (int times = 0; times < put_times; ++times)
		//				{
		//					ItemConfigData item_data(item1_id, true, pile_max_num);
		//					m_role->GetKnapsack()->PutOrMail(item_data, PUT_REASON_ZHUANZHI_SUIT_STUFF);
		//				}
		//			}

		//			if (remain_num > 0)
		//			{
		//				ItemConfigData item_data(item1_id, true, remain_num);
		//				m_role->GetKnapsack()->PutOrMail(item_data, PUT_REASON_ZHUANZHI_SUIT_STUFF);
		//			}

		//			// log
		//			this->PrintLog("ZhuanzhiEquip::CheckPutBackStuff", equip_index, equip_order, item1_id, num1);
		//		}
		//	}

		//	if (item2_id > 0 && num2 > 0)
		//	{
		//		const ItemBase *item_base = ITEMPOOL->GetItem(item2_id);
		//		if (NULL != item_base)
		//		{
		//			const int pile_max_num = item_base->GetPileLimit();
		//			const int put_times = num2 / pile_max_num;	// 堆叠数量的倍数
		//			const int remain_num = num2 % pile_max_num;	// 剩余数量

		//			if (put_times > 0)
		//			{
		//				for (int times = 0; times < put_times; ++times)
		//				{
		//					ItemConfigData item_data(item2_id, true, pile_max_num);
		//					m_role->GetKnapsack()->PutOrMail(item_data, PUT_REASON_ZHUANZHI_SUIT_STUFF);
		//				}
		//			}

		//			if (remain_num > 0)
		//			{
		//				ItemConfigData item_data(item2_id, true, remain_num);
		//				m_role->GetKnapsack()->PutOrMail(item_data, PUT_REASON_ZHUANZHI_SUIT_STUFF);
		//			}

		//			// log
		//			this->PrintLog("ZhuanzhiEquip::CheckPutBackStuff", equip_index, equip_order, item2_id, num2);
		//		}
		//	}

		//	if (item3_id > 0 && num3 > 0)
		//	{
		//		const ItemBase *item_base = ITEMPOOL->GetItem(item3_id);
		//		if (NULL != item_base)
		//		{
		//			const int pile_max_num = item_base->GetPileLimit();
		//			const int put_times = num3 / pile_max_num;	// 堆叠数量的倍数
		//			const int remain_num = num3 % pile_max_num;	// 剩余数量

		//			if (put_times > 0)
		//			{
		//				for (int times = 0; times < put_times; ++times)
		//				{
		//					ItemConfigData item_data(item3_id, true, pile_max_num);
		//					m_role->GetKnapsack()->PutOrMail(item_data, PUT_REASON_ZHUANZHI_SUIT_STUFF);
		//				}
		//			}

		//			if (remain_num > 0)
		//			{
		//				ItemConfigData item_data(item3_id, true, remain_num);
		//				m_role->GetKnapsack()->PutOrMail(item_data, PUT_REASON_ZHUANZHI_SUIT_STUFF);
		//			}

		//			// log
		//			this->PrintLog("ZhuanzhiEquip::CheckPutBackStuff", equip_index, equip_order, item3_id, num3);
		//		}
		//	}
		//}
		//break;
	}
}

int ZhuanzhiEquip::ReCalcSameOrderCount(int suit_index, int group_type, int order)
{
	int same_order_count = 0;

	const SuitForgeLimitCfg *forge_limit_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetSuitForgeLimitCfg(suit_index);
	if (nullptr == forge_limit_cfg)
	{
		return 0;
	}

	for (int equip_index = 0; equip_index < E_INDEX_MAX; equip_index ++)
	{
		int equip_group_type = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetGroupType(equip_index);
		if (equip_group_type < ZHUANZHI_SUIT_GROUP_TYPE_FANG || equip_group_type != group_type)
		{
			continue;
		}

		int equip_order = m_param.zhuanzhi_suit_order_list[suit_index].order_list[equip_index];
		if (equip_order < forge_limit_cfg->suit_min_order)
		{
			continue;
		}

		if (ZHUANZHI_SUIT_GROUP_TYPE_FANG == group_type)
		{
			if (order != equip_order)
			{
				continue;
			}
		}

		same_order_count++;
	}

	return same_order_count;
}

void ZhuanzhiEquip::GetSuitTotalAttr(CharIntAttrs &tmp_attr)
{
	for (int suit_index = 0; suit_index < SUIT_ROLE_HAS_MAX_SUIT_NUM; ++suit_index)
	{
		CharIntAttrs suit_attr; suit_attr.Reset();

		// 防具组合
		for (int equip_order = 0; equip_order < ItemPool::MAX_EQUIPMENT_ORDER; ++equip_order)
		{
			int cur_order_count = this->ReCalcSameOrderCount(suit_index, ZHUANZHI_SUIT_GROUP_TYPE_FANG, equip_order);

			const FangGroupAttrCfg *fang_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetFangGroupAttrCfg(m_role->GetBaseProf(), suit_index, equip_order, cur_order_count);
			if (NULL != fang_cfg)
			{
				suit_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += fang_cfg->maxhp;
				suit_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += fang_cfg->gongji;
				suit_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += fang_cfg->fangyu;

				suit_attr.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += fang_cfg->per_maxhp * EQUIP_ATTR_PER;
				suit_attr.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += fang_cfg->per_gongji * EQUIP_ATTR_PER;
				suit_attr.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += fang_cfg->per_fangyu * EQUIP_ATTR_PER;
			}
		}

		// 攻击组合
		int cur_order_count = this->ReCalcSameOrderCount(suit_index, ZHUANZHI_SUIT_GROUP_TYPE_GONG);

		const GongGroupAttrCfg *gong_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetGongGroupAttrCfg(suit_index, cur_order_count);
		if (NULL != gong_cfg)
		{
			suit_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += gong_cfg->maxhp;
			suit_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += gong_cfg->gongji;
			suit_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += gong_cfg->fangyu;
			suit_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER] += gong_cfg->skill_zengshang_per;
// 			suit_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += gong_cfg->per_mianshang;
// 			suit_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += gong_cfg->per_pofang;

			// 下面特殊属性用乘法
			suit_attr.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, gong_cfg->per_mianshang);
			suit_attr.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, gong_cfg->per_pofang);

			suit_attr.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += gong_cfg->per_maxhp * EQUIP_ATTR_PER;
			suit_attr.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += gong_cfg->per_gongji * EQUIP_ATTR_PER;
			suit_attr.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += gong_cfg->per_fangyu * EQUIP_ATTR_PER;
		}

		{
			// 天赋加成 - 诛仙诛神套装
			switch (suit_index)
			{
			case ZHUANZHI_SUIT_TYPE_ZHUXIAN:	
			{
				m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_ZHUXIAN_SUIT, suit_attr, suit_attr);
				break;
			}
			case ZHUANZHI_SUIT_TYPE_ZHUSHEN:
			{
				m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_ZHUSHEN_SUIT, suit_attr, suit_attr);
				break;
			}
			default:
				break;
			}
		}

		tmp_attr.Add(suit_attr.m_attrs);
		tmp_attr.AddPercent(suit_attr.m_percent_attrs);
	}

	{
		//至尊组合
		int index_list[ZHIZUNEQUIP_PART_TYPE_MAX]; memset(index_list, 0, sizeof(index_list));

		int zhizun_count = this->GetZhizunEquipIndexList(index_list);
		if (zhizun_count == ZHIZUNEQUIP_PART_TYPE_MAX)
		{
			ItemID zhizun_equip1 = m_param.zhuanzhi_equip_list[index_list[ZHIZUNEQUIP_PART_TYPE_0]].zhuanzhi_equip_data.item_id;
			ItemID zhizun_equip2 = m_param.zhuanzhi_equip_list[index_list[ZHIZUNEQUIP_PART_TYPE_1]].zhuanzhi_equip_data.item_id;
			int order = LOGIC_CONFIG->GetZhuanzhiEquipConfig().IsZhizunEquipSuit(zhizun_equip1, zhizun_equip2);
			if (order >= 0)
			{
				const ZhizunGroupAttrCfg *zhizun_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhizunGroupAttrCfg(order);
				if (NULL != zhizun_cfg)
				{
//					tmp_attr.m_percent_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += static_cast<float>(zhizun_cfg->pvp_zengshang_per * ROLE_ATTR_PER);
//					tmp_attr.m_percent_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] += static_cast<float>(zhizun_cfg->pve_zengshang_per * ROLE_ATTR_PER);
					tmp_attr.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, zhizun_cfg->pvp_zengshang_per);
					tmp_attr.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER, zhizun_cfg->pve_zengshang_per);
				}
			}
		}
	}
}

void ZhuanzhiEquip::SyncZhuanzhiSuitInfo()
{
	static Protocol::SCZhuanzhiSuitInfo info;
	memset(info.part_suit_type_list, -1, sizeof(info.part_suit_type_list));
	memset(info.part_order_list, 0, sizeof(info.part_order_list));

	for (int i = 0; i < ZHUANZHI_SUIT_TYPE_MAX; ++i)
	{
		for (int part = 0; part < E_INDEX_MAX; ++part)
		{
			if (m_param.zhuanzhi_suit_order_list[i].order_list[part] > 0)
			{
				info.part_suit_type_list[part] = i;
				info.part_order_list[part] = m_param.zhuanzhi_suit_order_list[i].order_list[part];
			}
		}
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&info, sizeof(info));
}

void ZhuanzhiEquip::OnZhiZunEquipCompose(ItemID compose_id, int best_attr_num, int equip_index, int bag_index1, int bag_index2)
{
	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	const Equipment *equip_item = (const Equipment *)ITEMPOOL->GetItem(compose_id);
	if (NULL == equip_item || ItemBase::I_TYPE_EQUIPMENT != equip_item->GetItemType())
	{
		m_role->NoticeNum(errornum::EN_SHENSHOU_ITEM_IS_NOT_EQUIP);
		return;
	}

	// 合成的必须是至尊装备
	short compose_equip_type = equip_item->GetEquipType();
	if (!LOGIC_CONFIG->GetZhuanzhiEquipConfig().IsZhiZunEquip(compose_id))
	{
		gstr::SendError(m_role, "en_equip_compose_not_zhizun_equip");
		return;
	}

	const EquipZhiZunComposeCfg *compose_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetEquipZhiZunComposeCfg(compose_id, best_attr_num);
	if (NULL == compose_cfg)
	{
		m_role->NoticeNum(errornum::EN_EQUIP_FUNC_CONFIG_ERROR);
		return;
	}

	// 从背包合成
	if (equip_index < 0 || equip_index >= E_INDEX_MAX)
	{
		bool is_bind = false;
		std::set<int> real_bag_index_set;	// 有效的背包索引

		int bag_index_list[EQUIP_ZHIZUN_COMPOSE_CONSUME_NUM];//背包索引
		memset(bag_index_list, -1, sizeof(bag_index_list));
		bag_index_list[0] = bag_index1;
		bag_index_list[1] = bag_index2;

		// 计算有效索引
		for (int i = 0; i < EQUIP_ZHIZUN_COMPOSE_CONSUME_NUM; i ++)
		{
			ItemGridData grid_data;
			if (!m_role->GetKnapsack()->GetItemGridData(bag_index_list[i], &grid_data))
			{
				return;
			}

			if (!this->IsCanCompose(compose_equip_type, bag_index_list[i]))
			{
				return;
			}

			if (real_bag_index_set.find(bag_index_list[i]) != real_bag_index_set.end())
			{
				return;
			}
			real_bag_index_set.insert(bag_index_list[i]);

			if (grid_data.is_bind)
			{
				is_bind = true;
			}
		}

		if ((int)real_bag_index_set.size() < EQUIP_ZHIZUN_COMPOSE_CONSUME_NUM)
		{
			m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return;
		}

		// 检查材料
		{
			if (compose_cfg->stuff1_id > 0 && compose_cfg->stuff1_num > 0)
			{
				if (knapsack->Count(compose_cfg->stuff1_id) < compose_cfg->stuff1_num)
				{
					m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
					return;
				}
			}
			if (compose_cfg->equip1_id > 0 && compose_cfg->equip1_num > 0)
			{
				if (knapsack->Count(compose_cfg->equip1_id) < compose_cfg->equip1_num)
				{
					m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
					return;
				}
			}
			if (compose_cfg->equip2_id > 0 && compose_cfg->equip2_num > 0)
			{
				if (knapsack->Count(compose_cfg->equip2_id) < compose_cfg->equip2_num)
				{
					m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
					return;
				}
			}
		}

		// 消耗
		{
			for (std::set<int>::const_iterator set_it = real_bag_index_set.begin(); set_it != real_bag_index_set.end(); ++set_it)
			{
				//装备
				knapsack->ConsumeItemByIndex(*set_it, 1, "ZhuanzhiEquip::OnComposeEquip");
			}

			if (compose_cfg->stuff1_id > 0 && compose_cfg->stuff1_num > 0)
			{
				//材料
				knapsack->ConsumeItem(compose_cfg->stuff1_id, compose_cfg->stuff1_num, "ZhuanzhiEquip::OnZhiZunEquipCompose");
			}
		}

		//给予随机属性
		{
			static ItemGridData item_grid_data; item_grid_data.Reset();

			int grid_index = -1;

			knapsack->Put(ItemConfigData(compose_id, is_bind, 1), PUT_REASON_ZHIZUN_EQUIP_COMPOSE, &grid_index);
			if (grid_index >= 0)
			{
				knapsack->GetItemGridData(grid_index, &item_grid_data);

				if (GLOBALCONFIG->GetEquipConfig().GetRandomXianpin(&item_grid_data, Equipment::EQUIPMENT_FIX_GETWAY_COMPOUND, 0, best_attr_num))
				{
					knapsack->SetItemDirty(grid_index);

					m_role->NoticeNum(noticenum::NT_EQUIP_COMPOSE_SUCC);
				}
			}
		}
	}
	// 从身上合成
	else
	{
		// 合法性检查
		ItemDataWrapper &itemdata = m_param.zhuanzhi_equip_list[equip_index].zhuanzhi_equip_data;
		if (itemdata.Invalid())
		{
			gstr::SendError(m_role, "select_zhuanzhi_equip_limit");
			return;
		}

		const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(itemdata.item_id);
		if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)
		{
			m_role->NoticeNum(errornum::EN_SHENSHOU_ITEM_IS_NOT_EQUIP);
			return;
		}

		if (compose_equip_type != equip->GetEquipType())
		{
			gstr::SendError(m_role, "en_equip_compose_equip_type_not_same");
			return;
		}

		if (!this->IsCanCompose(compose_equip_type, bag_index1))
		{
			return;
		}

		// 检查材料
		{
			if (compose_cfg->stuff1_id > 0 && compose_cfg->stuff1_num > 0)
			{
				if (knapsack->Count(compose_cfg->stuff1_id) < compose_cfg->stuff1_num)
				{
					m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
					return;
				}
			}
			if (compose_cfg->equip1_id > 0 && compose_cfg->equip1_num > 0)
			{
				if (knapsack->Count(compose_cfg->equip1_id) < compose_cfg->equip1_num)
				{
					m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
					return;
				}
			}
		}

		// 消耗
		{
			if (compose_cfg->equip1_id > 0 && compose_cfg->equip1_num > 0)
			{
				// 加强同id装备不同星级装备的消耗判断
				if (!knapsack->ConsumeItemByIndex(bag_index1, compose_cfg->equip1_num, "ZhuanzhiEquip::OnZhiZunEquipCompose"))
				{
					m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
					return;
				}
			}

			if (compose_cfg->stuff1_id > 0 && compose_cfg->stuff1_num > 0)
			{
				knapsack->ConsumeItem(compose_cfg->stuff1_id, compose_cfg->stuff1_num, "ZhuanzhiEquip::OnZhiZunEquipCompose");
			}
		}

		//给予随机属性
		{
			static ItemGridData item_grid_data; item_grid_data.Reset();
			item_grid_data.item_id = compose_id;
			item_grid_data.num = 1;
			item_grid_data.is_bind = (1 == itemdata.is_bind) ? true : false;
			item_grid_data.param = new EquipmentParam();

			if (GLOBALCONFIG->GetEquipConfig().GetRandomXianpin(&item_grid_data, Equipment::EQUIPMENT_FIX_GETWAY_COMPOUND, 0, best_attr_num))
			{
				item_grid_data.WrapTo(&itemdata);

				this->RefreshEquipInfo();

				this->CheckActiveEquipEffect();
				this->CheckAllPartChangeSlot(ZHUANZHI_STONE_SLOT_OPEN_TYPE_ORDER, equip_index);

				m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANZHI_EQUIP);

				m_role->NoticeNum(noticenum::NT_EQUIP_COMPOSE_SUCC);
				this->SyncZhuanzhiEquipInfo();
			}	

			if (item_grid_data.param)
			{
				delete item_grid_data.param;
				item_grid_data.param = nullptr;
			}
		}
	}
}

bool ZhuanzhiEquip::IsCanCompose(int equip_type, int index)
{
	ItemGridData grid_data;
	if (!m_role->GetKnapsack()->GetItemGridData(index, &grid_data))
	{
		return false;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(grid_data.item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType())
	{
		gstr::SendError(m_role, "en_equip_compose_puton_not_equip");
		return false;
	}

	short bag_equip_type = equip->GetEquipType();
	if (equip_type != bag_equip_type)
	{
		gstr::SendError(m_role, "en_equip_compose_equip_type_not_same");
		return false;
	}

	EquipmentParam *equip_param = (EquipmentParam *)grid_data.param;
	if (NULL == equip_param)
	{
		gstr::SendError(m_role, "select_zhuanzhi_equip_limit");
		return false;
	}

	return true;
}

int ZhuanzhiEquip::GetZhizunEquipIndexList(int *index_list)
{
	int zhizun_count = 0;
	if (nullptr == index_list)
	{
		return 0;
	}


	for (int equip_index = 0; equip_index < E_INDEX_MAX; equip_index++)
	{
		ItemID equip_id = m_param.zhuanzhi_equip_list[equip_index].zhuanzhi_equip_data.item_id;

		if (LOGIC_CONFIG->GetZhuanzhiEquipConfig().IsZhiZunEquip(equip_id))
		{
			if (zhizun_count >= ZHIZUNEQUIP_PART_TYPE_MAX)
			{
				break;
			}

			index_list[zhizun_count++] = equip_index;
		}	
	}

	return zhizun_count;
}

int ZhuanzhiEquip::GetZhiZunEquipOtherCap()
{
	int other_cap = 0;
	for (int equip_index = 0; equip_index < E_INDEX_MAX; equip_index++)
	{
		ItemID equip_id = m_param.zhuanzhi_equip_list[equip_index].zhuanzhi_equip_data.item_id;

		const ZhiZunEquipCfg * zhizun_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhiZunEquipCfg(equip_id);
		if (nullptr == zhizun_cfg)
		{
			continue;
		}

		other_cap += zhizun_cfg->add_cap;
	}

	return other_cap;
}

void ZhuanzhiEquip::CheckActiveEquipEffect()
{
	for (int part_index = 0; part_index < E_INDEX_ZHUANZHI::E_INDEX_MAX; ++part_index)
	{
		ItemDataWrapper &itemdata = m_param.zhuanzhi_equip_list[part_index].zhuanzhi_equip_data;

		const Equipment *equip = GetZhuanzhiEquip(itemdata.item_id);
		if (nullptr == equip)
		{
			continue;
		}

		const ZhuanzhiEquipCfg *equip_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhuanzhiEquipCfg(part_index, equip->GetOrder());
		if (nullptr == equip_cfg)
		{
			continue;
		}

		if (equip->GetColor() < equip_cfg->special_attr_color)
		{
			continue;
		}

		if (equip_cfg->effect_type <= 0)
		{
			continue;
		}

		m_active_effect_type_set.emplace(equip_cfg->effect_type);
	}
}

void ZhuanzhiEquip::OnHurtOther(Character *target, long long base_injure, long long &injure)
{
	if (NULL == target)
	{
		return;
	}

	OnHurtOtherZhiZunEffect(target, injure);
	OnHurtOtherUpGongji(target);
	OnHurtOtherPVPUpShanghai(target, injure);
	OnHurtOtherPVEUpShanghai(target, injure);
}

void ZhuanzhiEquip::OnHurtOtherZhiZunEffect(Character *target, long long &injure)
{
	// 判断对方是否是玩家
	if (Obj::OBJ_TYPE_ROLE != target->GetObjType())
	{
		return;
	}

	int cur_level = m_role->GetLevel();
	int zhizun_add_max_level = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetOtherCfg().zhizun_add_max_level;
	if (cur_level > zhizun_add_max_level)
	{
		cur_level = zhizun_add_max_level;
	}

	int index_list[ZHIZUNEQUIP_PART_TYPE_MAX]; memset(index_list, 0, sizeof(index_list));

	int zhizun_count = this->GetZhizunEquipIndexList(index_list);
	if (zhizun_count <= 0)
	{
		return;
	}

	for (int i = 0; i < zhizun_count; i++)
	{
		ItemID zhizun_equip = m_param.zhuanzhi_equip_list[index_list[i]].zhuanzhi_equip_data.item_id;
		const ZhiZunEquipCfg *active_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetZhiZunEquipCfg(zhizun_equip);
		if (NULL == active_cfg)
		{
			continue;
		}

		if (0 < active_cfg->param1)			// 每次攻击玩家附带额外伤害
		{
			injure += active_cfg->param1 * cur_level;
		}
		else if (0 < active_cfg->param2)	// 每次攻击玩家吸血
		{
			Attribute add_hp = active_cfg->param2 * cur_level;
			m_role->ChangeHp(&add_hp, true);
		}
	}
}

void ZhuanzhiEquip::OnHurtOtherUpGongji(Character * target)
{
	if (nullptr == target)
	{
		return;
	}

	auto it = m_active_effect_type_set.find(EQUIP_SPECIAL_EFFECT_TYPE_1);
	if (it == m_active_effect_type_set.end())
	{
		return;
	}

	const EquipSpecialEffectCfg *effect_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetEquipEffectCfg(EQUIP_SPECIAL_EFFECT_TYPE_1);
	if (nullptr == effect_cfg)
	{
		return;
	}

	int rate = RandomNum(10000);
	if (rate > effect_cfg->rate)
	{
		return;
	}

	Attribute dec_hp = static_cast<Attribute>(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (effect_cfg->param_1 * SKILL_ATTR_PER));

	EffectOnce *dec_effect = new EffectOnce(m_role->GetId(), 0, -dec_hp, EffectBase::FIGHT_TYPE_NORMAL);
	target->AddEffect(dec_effect);
}

void ZhuanzhiEquip::OnHurtOtherPVPUpShanghai(Character *target, long long base_injure)
{
	if (nullptr == target || Obj::OBJ_TYPE_ROLE != target->GetObjType())
	{
		return;
	}

	if (m_special_attr_pvp_zengshang <= 0)
	{
		return;
	}

	Attribute up_shanghai = static_cast<Attribute>((m_special_attr_pvp_zengshang * SKILL_ATTR_PER) * base_injure);

	EffectOnce *dec_effect = new EffectOnce(m_role->GetId(), 0, -up_shanghai, EffectBase::FIGHT_TYPE_NORMAL);
	target->AddEffect(dec_effect);
}

void ZhuanzhiEquip::OnHurtOtherPVEUpShanghai(Character *target, long long base_injure)
{
	if (nullptr == target || Obj::OBJ_TYPE_MONSTER != target->GetObjType())
	{
		return;
	}

	if (m_special_attr_pve_zengshang <= 0)
	{
		return;
	}

	Attribute up_shanghai = static_cast<Attribute>((m_special_attr_pve_zengshang * SKILL_ATTR_PER) * base_injure);

	EffectOnce *dec_effect = new EffectOnce(m_role->GetId(), 0, -up_shanghai, EffectBase::FIGHT_TYPE_NORMAL);
	target->AddEffect(dec_effect);
}

void ZhuanzhiEquip::OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill)
{
	Obj* obj = m_role->GetScene()->GetObj(attacker);
	if (NULL == obj)
	{
		return;
	}

	OnBeendAttackDownShanghai(attacker, delta_hp);
	OnBeendAttackPVPDownShanghai(attacker, delta_hp);
	OnBeendAttackPVEDownShanghai(attacker, delta_hp);
}

void ZhuanzhiEquip::OnBeendAttackDownShanghai(ObjID attacker, Attribute &delta_hp)
{
	Obj* obj = m_role->GetScene()->GetObj(attacker);
	if (NULL == obj)
	{
		return;
	}

	auto it = m_active_effect_type_set.find(EQUIP_SPECIAL_EFFECT_TYPE_2);
	if (it == m_active_effect_type_set.end())
	{
		return;
	}

	const EquipSpecialEffectCfg *effect_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetEquipEffectCfg(EQUIP_SPECIAL_EFFECT_TYPE_2);
	if (nullptr == effect_cfg)
	{
		return;
	}

	int rate = RandomNum(10000);
	if (rate > effect_cfg->rate)
	{
		return;
	}

	Attribute dec_hp = static_cast<Attribute>(delta_hp * (effect_cfg->param_1 * SKILL_ATTR_PER));
	delta_hp -= dec_hp;
}

void ZhuanzhiEquip::OnBeendAttackPVPDownShanghai(ObjID attacker, Attribute &delta_hp)
{
	Obj* obj = m_role->GetScene()->GetObj(attacker);
	if (NULL == obj || Obj::OBJ_TYPE_ROLE != obj->GetObjType())
	{
		return;
	}

	if (m_special_attr_pvp_jianshang <= 0)
	{
		return;
	}

	Attribute dec_hp = static_cast<Attribute>(delta_hp * (m_special_attr_pvp_jianshang* SKILL_ATTR_PER));
	delta_hp -= dec_hp;
}

void ZhuanzhiEquip::OnBeendAttackPVEDownShanghai(ObjID attacker, Attribute &delta_hp)
{
	Obj* obj = m_role->GetScene()->GetObj(attacker);
	if (NULL == obj || Obj::OBJ_TYPE_MONSTER != obj->GetObjType())
	{
		return;
	}

	if (m_special_attr_pve_jianshang <= 0)
	{
		return;
	}

	Attribute dec_hp = static_cast<Attribute>(delta_hp * (m_special_attr_pve_jianshang* SKILL_ATTR_PER));
	delta_hp -= dec_hp;
}

Attribute ZhuanzhiEquip::EquipXianPinFixValue(int attr_type)
{
	if (attr_type <= Equipment::E_XIANPIN_INVALID || attr_type >= Equipment::E_XIANPIN_MAX)
	{
		return 0;
	}

	return m_xianpin_value_list[attr_type];
}

void ZhuanzhiEquip::ReCalcXianpinTotalStar(int &xianpin_total_star)
{
	int total_star = 0;

	for (int i = 0; i < E_INDEX_ZHUANZHI::E_INDEX_MAX; ++i)
	{
		int total_capbility = 0;

		ItemDataWrapper &itemdata = m_param.zhuanzhi_equip_list[i].zhuanzhi_equip_data;
		if (itemdata.Invalid())
		{
			continue;
		}

		const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(itemdata.item_id);
		if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)
		{
			continue;
		}

		int attr_type = 0;

		for (int k = 0; k < ItemNamespace::MAX_XIANPIN_NUM; ++k)
		{
			attr_type = itemdata.param_data.xianpin_type_list[k];
			const EquipXianPinConfig *xianpin_cfg = GLOBALCONFIG->GetEquipConfig().GetXianPinEquipCfg(attr_type);
			if (NULL == xianpin_cfg)
			{
				continue;
			}

			total_capbility += xianpin_cfg->capability;
		}

		if (total_capbility > 0)
		{
			int xianpin_star_level = GLOBALCONFIG->GetEquipConfig().GetXianPinStarLevel(total_capbility);

			total_star += xianpin_star_level;
		}
	}

	xianpin_total_star = total_star;
}

int ZhuanzhiEquip::GetSatisfyPutonCount(int order, int color)
{
	int suit_equip_count = 0;
	for (int part_index = 0; part_index < E_INDEX_ZHUANZHI::E_INDEX_MAX; ++part_index)
	{
		ItemDataWrapper &itemdata = m_param.zhuanzhi_equip_list[part_index].zhuanzhi_equip_data;
		if (itemdata.Invalid())
		{
			continue;
		}

		const Equipment *equip = GetZhuanzhiEquip(itemdata.item_id);
		if (nullptr == equip)
		{
			continue;
		}
		// 策划需求，粉装以上直接满足条件		
		if (equip->GetColor() >= ItemBase::I_COLOR_PINK)
		{
			++suit_equip_count;
			continue;
		}

		if (equip->GetColor() >= color && equip->GetOrder() >= order)
		{
			++suit_equip_count;
		}
	}

	return suit_equip_count;
}

int ZhuanzhiEquip::GetSuitTypeCount(int suit_index)
{
	if (suit_index <= ZHUANZHI_SUIT_TYPE_INVALID || suit_index >= ZHUANZHI_SUIT_TYPE_MAX)
	{
		return 0;
	}

	int suit_count = 0;
	OneSuitInfo &one_suit_info = m_param.zhuanzhi_suit_order_list[suit_index];
	int index_count = static_array_size(one_suit_info.order_list);
	for (int i = 0; i < index_count; i ++)
	{
		if (one_suit_info.order_list[i] > 0)
		{
			suit_count++;
		}
	}

	return suit_count;
}

bool ZhuanzhiEquip::HasSuit(int part_index)
{
	if (part_index < 0 || part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX)
	{
		return false;
	}
	ItemDataWrapper &itemdata = m_param.zhuanzhi_equip_list[part_index].zhuanzhi_equip_data;
	if (itemdata.Invalid())
	{
		return false;
	}
	const Equipment *equip = GetZhuanzhiEquip(itemdata.item_id);
	if (nullptr == equip)
	{
		return false;
	}
	return true;
}

void ZhuanzhiEquip::PrintLog(const char *reason, int part_index, int param1, int param2, int param3, int param4)
{
	if (part_index < 0 || part_index >= E_INDEX_MAX)
	{
		return;
	}

	const ZhuanzhiEquipItem &equip_item = m_param.zhuanzhi_equip_list[part_index];
	const ZhuanzhiStonesPartData &stone_item = m_param.stone_list[part_index];

	gamelog::g_log_zhaunzhi_equip.printf(LL_INFO, "ZhuanzhiEquip role(%d,%s) %s:part_index(%d)(%d,%d,%d,%d) "
		"star_level:%d, star_exp:%d, equip_id:%d"
		"stone_data: slot_open_flag(%d), refine_level(%d), refine_val(%d)"
		"stone_slot:0(%d), 1(%d), 2(%d), 3(%d), 4(%d), 5(%d)",
		m_role->GetUID(), m_role->GetName(),
		reason, part_index, param1, param2, param3, param4,
		equip_item.star_level, equip_item.star_exp, equip_item.zhuanzhi_equip_data.item_id,
		(int)stone_item.slot_open_flag, (int)stone_item.refine_level, stone_item.refine_val,
		(int)stone_item.slot_list[0].stone_id, (int)stone_item.slot_list[1].stone_id,
		(int)stone_item.slot_list[2].stone_id, (int)stone_item.slot_list[3].stone_id,
		(int)stone_item.slot_list[4].stone_id, (int)stone_item.slot_list[5].stone_id
		);
}

void ZhuanzhiEquip::ReCalcXianpinAttr(CharIntAttrs &attrs_add, int puton_level)
{
	memset(m_xianpin_value_list, 0, sizeof(m_xianpin_value_list));

	for (int i = 0; i < E_INDEX_ZHUANZHI::E_INDEX_MAX; ++i)
	{
		ItemDataWrapper &itemdata = m_param.zhuanzhi_equip_list[i].zhuanzhi_equip_data;
		if (itemdata.Invalid())
		{
			continue;
		}

		const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(itemdata.item_id);
		if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)
		{
			continue;
		}

		if (puton_level < equip->GetLimitLevel())
		{
			continue;
		}

		int attr_type = 0;
		Attribute attr_value = 0;

		for (int k = 0; k < ItemNamespace::MAX_XIANPIN_NUM; ++k)
		{
			attr_type = itemdata.param_data.xianpin_type_list[k];
			const EquipXianPinConfig *xianpin_cfg = GLOBALCONFIG->GetEquipConfig().GetXianPinEquipCfg(attr_type);
			if (NULL == xianpin_cfg)
			{
				continue;
			}
			attr_value = xianpin_cfg->add_value;

			m_xianpin_value_list[xianpin_cfg->shuxing_type] += attr_value;
		}
	}

	CharIntAttrs equip_base_attrs;

	for (int i = 0; i < E_INDEX_ZHUANZHI::E_INDEX_MAX; ++i)
	{
		ItemDataWrapper &itemdata = m_param.zhuanzhi_equip_list[i].zhuanzhi_equip_data;
		if (itemdata.Invalid())
		{
			continue;
		}

		const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(itemdata.item_id);
		if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)
		{
			continue;
		}

		if (puton_level < equip->GetLimitLevel())
		{
			continue;
		}

		equip_base_attrs.Reset();
		equip->GetBaseAttr(equip_base_attrs);

		if (equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] > 0 && m_xianpin_value_list[Equipment::E_XIANPIN_ADD_ALL_EQUIP_BASE_GONGJI_PER] > 0) // 转职装备基础攻击增加  X %
		{
			attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_ALL_EQUIP_BASE_GONGJI_PER] / 10000.0));
		}

		if (equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] > 0 && m_xianpin_value_list[Equipment::E_XIANPIN_ADD_ALL_EQUIP_BASE_FANGYU_PER] > 0) // 转职装备基础防御增加  X %
		{
			attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_ALL_EQUIP_BASE_FANGYU_PER] / 10000.0));
		}

		if (equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] > 0 && m_xianpin_value_list[Equipment::E_XIANPIN_ADD_ALL_EQUIP_BASE_MAXHP_PER] > 0) // 转职装备基础气血增加  X %
		{
			attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(equip_base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (m_xianpin_value_list[Equipment::E_XIANPIN_ADD_ALL_EQUIP_BASE_MAXHP_PER] / 10000.0));
		}
	}
}

bool ZhuanzhiEquip::AttachAttr(const ItemGridData &item_data, CharIntAttrs &base_add, int puton_level, int &flush_maxhp_count, int &flush_gongji_count)
{
	if (item_data.Invalid()) return false;

	const EquipmentParam *param = (const EquipmentParam *)item_data.param;
	if (NULL == param) return false;

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_data.item_id);
	if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT) return false;

	// 基础与强化属性
	if (puton_level >= equip->GetLimitLevel())
	{
		equip->GetAttr(param, base_add, m_role->GetProf());
	}

	return true;
}

bool ZhuanzhiEquip::WakeBase(int part_index, int lock_index)
{
	if (part_index < 0 || part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX || lock_index < 0 || lock_index >= MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT)
	{
		return false;
	}

	bool level_ret = this->ChangeAwakeningLevel(part_index, lock_index);
	bool type_ret = this->ChangeAwakeningType(part_index, lock_index);
	return level_ret && type_ret;
}

bool ZhuanzhiEquip::AddAwakeningLevelBase(int part_index, int lock_index, wake_level_t level)
{
	if (part_index < 0 || part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX || lock_index < 0 || lock_index >= MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT)
	{
		return false;
	}
	const int equip_order = this->GetZhuanzhiEquipOrder(part_index);
	const wake_level_t max_level = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetWakeLevelLimitCfg(equip_order);
	wake_level_t old_level = this->GetAwakeningEquipLevel(part_index, lock_index);
	wake_level_t new_level = old_level + level;
	if (new_level < old_level || new_level < 0)	//处理溢出
	{
		new_level = max_level;
	}
	if (new_level > max_level)	//处理超过满级
	{
		new_level = max_level;
	}
	if (old_level > max_level)	//防止玩家换下低阶装备后，把max_level(实际比之前更低）赋值给new_level
	{
		new_level = old_level;
	}
	m_param.zhuanzhi_equip_awakening_list[part_index].awakening_in_displacement[lock_index].level = new_level;
	gamelog::g_log_zhaunzhi_equip.printf(LL_INFO, "ZhuanzhiEquip::AddAwakeningLevelBase role(%d,%s) %d %d",
		m_role->GetUID(), m_role->GetName(), (int)old_level, (int)new_level);
	return true;
}

bool ZhuanzhiEquip::DecAwakeningLevelBase(int part_index, int lock_index, wake_level_t level)
{
	if (part_index < 0 || part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX || lock_index < 0 || lock_index >= MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT)
	{
		return false;
	}
	wake_level_t old_level = this->GetAwakeningEquipLevel(part_index, lock_index);
	wake_level_t new_level = old_level - level;
	wake_level_t min_level = 1;
	if (new_level > old_level || new_level < min_level)
	{
		new_level = min_level;
	}
	m_param.zhuanzhi_equip_awakening_list[part_index].awakening_in_displacement[lock_index].level = new_level;
	gamelog::g_log_zhaunzhi_equip.printf(LL_INFO, "ZhuanzhiEquip::AddAwakeningLevelBase role(%d,%s) %d %d",
		m_role->GetUID(), m_role->GetName(), (int)old_level, (int)new_level);
	return true;
}

bool ZhuanzhiEquip::SetAwakeningTypeBase(int part_index, int lock_index, wake_type_t type)
{
	if (part_index < 0 || part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX || lock_index < 0 || lock_index >= MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT)
	{
		return false;
	}
	const wake_level_t init_level = 0;
	const AwakeningAttributeCfg *cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetWakeAttributeCfg(type, init_level);
	if (nullptr == cfg)
	{
		return false;
	}
	wake_type_t old_type = m_param.zhuanzhi_equip_awakening_list[part_index].awakening_in_displacement[lock_index].type;
	m_param.zhuanzhi_equip_awakening_list[part_index].awakening_in_displacement[lock_index].type = type;
	gamelog::g_log_zhaunzhi_equip.printf(LL_INFO, "ZhuanzhiEquip::AddAwakeningLevelBase role(%d,%s) %d %d",
		m_role->GetUID(), m_role->GetName(), (int)old_type, (int)type);
	return true;
}

bool ZhuanzhiEquip::WakeConsume(int part_index, int is_auto_buy)
{
	if (part_index < 0 || part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX)
	{
		return false;
	}
	Money *money = m_role->GetKnapsack()->GetMoney();
	if (nullptr == money)
	{
		return false;
	}
	if (money->IsLock())
	{
		m_role->NoticeNum(errornum::EN_MONEY_IS_LOCK);
		return false;
	}
	unsigned int lock_num = this->GetLockAwakeningNum(part_index);
	const AwakeningLockConsumeCfg *cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetWakeLockConsumeCfg(lock_num);
	if (nullptr == cfg)
	{
		return false;
	}
	if (is_auto_buy && !money->GoldMoreThan(cfg->gold_num))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return false;
	}
	ItemExtern::ItemConsumeList consume_list;												//消耗列表
	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	int stuff_count = 0;
	if (cfg->consume_stuff_num > 0 && stuff_count >= 0 && stuff_count < ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM)
	{
		stuff_list[stuff_count].item_id = cfg->consume_stuff_id;
		stuff_list[stuff_count].num = cfg->consume_stuff_num;
		if (!is_auto_buy && cfg->consume_stuff_num_1 > 0)
		{
			stuff_list[stuff_count].num += cfg->consume_stuff_num_1;
		}
		stuff_list[stuff_count].buyable = false;	//策划(子豪)不想走商店逻辑	2019 04 15
		++stuff_count;
	}
	/*if (cfg->lock_stuff_num > 0 && stuff_count >= 0 && stuff_count < ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM)
	{
		stuff_list[stuff_count].item_id = cfg->lock_stuff_id;
		stuff_list[stuff_count].num = cfg->lock_stuff_num;
		stuff_list[stuff_count].buyable = is_auto_buy;
		++stuff_count;
	}*/
	//获取消耗列表
	if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, false))
	{
		return false;
	}
	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "ZhuanzhiEquip::WakeConsume"))
	{
		return false;
	}
	if (is_auto_buy && cfg->gold_num > 0)
	{
		if (!money->UseGold(cfg->gold_num, "ZhuanzhiEquip::WakeConsume"))
		{
			return false;
		}
	}
	return true;
}

int ZhuanzhiEquip::GetEquipNoticeColor(int equip_index)
{
	if (equip_index < E_INDEX_ZHUANZHI::E_INDEX_WUQI || equip_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX)
	{
		return 0;
	}

	bool broadcast_color = false;
	const DailyChongzhiRewradConfig *cfg = LOGIC_CONFIG->GetChongzhiCfg().GetDailyChongzhiRewradConfig(m_role->GetLevel(), 0);
	if (NULL != cfg)
	{
		if (m_role->GetRoleActivity()->GetParam()->history_chongzhi >= static_cast<long long>(cfg->need_total_chongzhi))
		{
			broadcast_color = true;
		}
	}

	if (broadcast_color)
	{
		return this->GetEquipColor(equip_index);
	}

	return 0;
}

void ZhuanzhiEquip::Wake(int part_index, int is_auto_buy)
{
	if (part_index < 0 || part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX)
	{
		return;
	}
	if (!this->HasSuit(part_index))
	{
		m_role->NoticeNum(errornum::EN_ZHUANZHI_EQUIP_WAKE_NOT_WEAR);
		return;
	}
	const ZhuanzhiOtherConfig &other_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetOtherCfg();
	int order = this->GetZhuanzhiEquipOrder(part_index);
	if (order < other_cfg.wake_min_order)
	{
		m_role->NoticeNum(errornum::EN_ZHUANZHI_EQUIP_WAKE_ORDER_NOT_ENOUGH);
		return;
	}
	int color = this->GetEquipColor(part_index);
	if (color < other_cfg.wake_min_color)
	{
		m_role->NoticeNum(errornum::EN_ZHUANZHI_EQUIP_WAKE_COLOR_NOT_ENOUGH);
		return;
	}
	if (!this->WakeConsume(part_index, is_auto_buy))
	{
		return;
	}
	std::vector<int> lock_index;
	std::vector<int> unlock_index;
	for (int i = 0; i < MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT; ++i)
	{
		if (this->IsLockAwakening(part_index, i))
		{
			lock_index.push_back(i);
		}
		else
		{
			unlock_index.push_back(i);
		}
	}
	// 先随锁定，再随未锁定的，避免出现重复类型
	for (size_t i = 0; i < lock_index.size(); ++i)
	{
		this->WakeBase(part_index, lock_index[i]);
	}
	for (size_t i = 0; i < unlock_index.size(); ++i)
	{
		this->WakeBase(part_index, unlock_index[i]);
	}
	this->SyncAwakeningInfo(part_index);
}

void ZhuanzhiEquip::CoverAwakening(int part_index)
{
	if (part_index < 0 || part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX)
	{
		return;
	}
	if (!this->HasSuit(part_index))
	{
		return;
	}
	//替换位必须有等级
	for (int i = 0; i < MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT; ++i)
	{
		if (0 == m_param.zhuanzhi_equip_awakening_list[part_index].awakening_in_displacement[i].level)
		{
			return;
		}
	}
	this->WakeRumor(part_index);	//发传闻

	UNSTD_STATIC_CHECK(sizeof(m_param.zhuanzhi_equip_awakening_list[part_index].awakening_in_equip) == sizeof(m_param.zhuanzhi_equip_awakening_list[part_index].awakening_in_displacement));
	memcpy(m_param.zhuanzhi_equip_awakening_list[part_index].awakening_in_equip, 
			m_param.zhuanzhi_equip_awakening_list[part_index].awakening_in_displacement, 
			sizeof(m_param.zhuanzhi_equip_awakening_list[part_index].awakening_in_equip));
	memset(m_param.zhuanzhi_equip_awakening_list[part_index].awakening_in_displacement, 0, sizeof(m_param.zhuanzhi_equip_awakening_list[part_index].awakening_in_displacement));
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANZHI_EQUIP);
	this->SyncAwakeningInfo(part_index);
}

void ZhuanzhiEquip::SyncAwakeningAllInfo()
{
	static Protocol::SCZhuanzhiEquipAwakeningAllInfo info;
	UNSTD_STATIC_CHECK(sizeof(info.zhuanzhi_equip_awakening_lock_flag) == sizeof(m_param.zhuanzhi_equip_awakening_lock_flag));
	memcpy(info.zhuanzhi_equip_awakening_lock_flag, m_param.zhuanzhi_equip_awakening_lock_flag, sizeof(info.zhuanzhi_equip_awakening_lock_flag));
	UNSTD_STATIC_CHECK(sizeof(info.zhuanzhi_equip_awakening_list) == sizeof(m_param.zhuanzhi_equip_awakening_list));
	memcpy(info.zhuanzhi_equip_awakening_list, m_param.zhuanzhi_equip_awakening_list, sizeof(info.zhuanzhi_equip_awakening_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&info, sizeof(info));
}

void ZhuanzhiEquip::SyncAwakeningInfo(int part_index)
{
	if (part_index < 0 || part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX)
	{
		return;
	}
	static Protocol::SCZhuanzhiEquipAwakeningInfo info;
	info.index = part_index;
	UNSTD_STATIC_CHECK(sizeof(info.zhuanzhi_equip_awakening_lock_flag) == sizeof(m_param.zhuanzhi_equip_awakening_lock_flag));
	memcpy(info.zhuanzhi_equip_awakening_lock_flag, m_param.zhuanzhi_equip_awakening_lock_flag, sizeof(info.zhuanzhi_equip_awakening_lock_flag));
	UNSTD_STATIC_CHECK(sizeof(info.zhuanzhi_equip_awakening) == sizeof(m_param.zhuanzhi_equip_awakening_list[part_index]));
	memcpy(&info.zhuanzhi_equip_awakening, &m_param.zhuanzhi_equip_awakening_list[part_index], sizeof(info.zhuanzhi_equip_awakening));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&info, sizeof(info));
}

bool ZhuanzhiEquip::LockAwakening(int part_index, int lock_index)
{
	if (part_index < 0 || part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX || lock_index < 0 || lock_index >= MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT)
	{
		return false;
	}
	wake_level_t wake_level = this->GetAwakeningEquipLevel(part_index, lock_index);
	if (0 == wake_level)
	{
		return false;
	}
	int offset_value = part_index * MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT + lock_index;
	bool ret = BitmapFunction::SetBit(m_param.zhuanzhi_equip_awakening_lock_flag, MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_BYTE, offset_value);
	this->SyncAwakeningInfo(part_index);
	return ret;
}

bool ZhuanzhiEquip::UnLockAwakening(int part_index, int lock_index)
{
	if (part_index < 0 || part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX || lock_index < 0 || lock_index >= MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT)
	{
		return false;
	}
	wake_level_t wake_level = this->GetAwakeningEquipLevel(part_index, lock_index);
	if (0 == wake_level)
	{
		return false;
	}
	int offset_value = part_index * MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT + lock_index;
	bool ret = BitmapFunction::ClearBit(m_param.zhuanzhi_equip_awakening_lock_flag, MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_BYTE, offset_value);
	this->SyncAwakeningInfo(part_index);
	return ret;
}

bool ZhuanzhiEquip::IsLockAwakening(int part_index, int lock_index)
{
	if (part_index < 0 || part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX || lock_index < 0 || lock_index >= MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT)
	{
		return false;
	}
	int offset_value = part_index * MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT + lock_index;
	return BitmapFunction::IsBitSet(m_param.zhuanzhi_equip_awakening_lock_flag, MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_BYTE, offset_value);
}

unsigned int ZhuanzhiEquip::GetLockAwakeningNum(int part_index)
{
	if (part_index < 0 || part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX)
	{
		return 0;
	}
	int count = 0;
	for (int i = 0; i < MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT; ++i)
	{
		if (this->IsLockAwakening(part_index, i))
		{
			++count;
		}
	}
	return count;
}

void ZhuanzhiEquip::ReCalcAttrAwakening(CharIntAttrs & base_add)
{
	for (int part_index = 0; part_index < E_INDEX_ZHUANZHI::E_INDEX_MAX; ++part_index)
	{
		if (!this->HasSuit(part_index))
		{
			continue;
		}
		for (int lock_index = 0; lock_index < MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT; ++lock_index)
		{
			int level = this->GetAwakeningEquipLevel(part_index, lock_index);
			if (0 == level)
			{
				continue;
			}
			int type = this->GetAwakeningEquipType(part_index, lock_index);
			const AwakeningAttributeCfg *cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetWakeAttributeCfg(type, level);
			if (nullptr == cfg)
			{
				continue;
			}
			base_add.AddValue(cfg->attribute_type, cfg->attribute_value);
			base_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<float>(cfg->max_hp_per * ROLE_ATTR_PER);
			base_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<float>(cfg->max_gongji_per * ROLE_ATTR_PER);
			base_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<float>(cfg->max_fangyu_per * ROLE_ATTR_PER);
		}
	}
}

wake_level_t ZhuanzhiEquip::GetAwakeningLevel(int part_index, int lock_index)
{
	if (part_index < 0 || part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX || lock_index < 0 || lock_index >= MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT)
	{
		return 0;
	}
	return m_param.zhuanzhi_equip_awakening_list[part_index].awakening_in_displacement[lock_index].level;
}

wake_level_t ZhuanzhiEquip::GetAwakeningEquipLevel(int part_index, int lock_index)
{
	if (part_index < 0 || part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX || lock_index < 0 || lock_index >= MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT)
	{
		return 0;
	}
	return m_param.zhuanzhi_equip_awakening_list[part_index].awakening_in_equip[lock_index].level;
}

wake_type_t ZhuanzhiEquip::GetAwakeningType(int part_index, int lock_index)
{
	if (part_index < 0 || part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX || lock_index < 0 || lock_index >= MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT)
	{
		return 0;
	}
	return m_param.zhuanzhi_equip_awakening_list[part_index].awakening_in_displacement[lock_index].type;
}

wake_type_t ZhuanzhiEquip::GetAwakeningEquipType(int part_index, int lock_index)
{
	if (part_index < 0 || part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX || lock_index < 0 || lock_index >= MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT)
	{
		return 0;
	}
	return m_param.zhuanzhi_equip_awakening_list[part_index].awakening_in_equip[lock_index].type;
}

bool ZhuanzhiEquip::ChangeAwakeningLevel(int part_index, int lock_index)
{
	if (part_index < 0 || part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX || lock_index < 0 || lock_index >= MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT)
	{
		return false;
	}
	wake_level_t equip_level = this->GetAwakeningEquipLevel(part_index, lock_index);
	//wake_level_t level = this->GetAwakeningLevel(part_index, lock_index);
	const AwakeningLevelWeightCfg::LevelRangeWeightCfg *cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetWakeLevelWeightCfg(equip_level);
	if (nullptr == cfg)
	{
		return false;
	}
	const wake_weight_t rand_value = RandomNum(cfg->total_weight);
	wake_weight_t cur_weight = 0;
	for (size_t i = 0; i < cfg->weight.size(); ++i)
	{
		cur_weight += cfg->weight[i];
		if (cur_weight > rand_value)
		{
			bool is_lock = this->IsLockAwakening(part_index, lock_index);
			short add_level = cfg->level_left + (short)i;
			if (is_lock && add_level < 0)
			{
				add_level = 0;
			}
			if (add_level >= 0)
			{
				this->AddAwakeningLevelBase(part_index, lock_index, add_level);
			}
			else if (add_level < 0)
			{
				this->DecAwakeningLevelBase(part_index, lock_index, -add_level);
			}
			return true;
		}
	}
	return false;
}

bool ZhuanzhiEquip::ChangeAwakeningType(int part_index, int lock_index)
{
	if (part_index < 0 || part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX || lock_index < 0 || lock_index >= MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT)
	{
		return false;
	}
	if (this->IsLockAwakening(part_index, lock_index))
	{
		wake_type_t wake_equip_type = this->GetAwakeningEquipType(part_index, lock_index);
		wake_type_t wake_type = this->GetAwakeningType(part_index, lock_index);
		if (wake_type != wake_equip_type)
		{
			this->SetAwakeningTypeBase(part_index, lock_index, wake_equip_type);
			return true;
		}
		else
		{
			return false;
		}
	}
	AwakeningTypeWeightCfg cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetWakeTypeWeightCfg(part_index);
	if (cfg.total_weight <= 0 || cfg.type_cfg_map.size() == 0)
	{
		return false;
	}
	for (int i = 0; i < MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT; ++i)
	{
		if (i == lock_index)
		{
			continue;
		}
		wake_type_t type = this->GetAwakeningType(part_index, i);
		if (cfg.type_cfg_map.find(type) == cfg.type_cfg_map.end())
		{
			continue;
		}
		cfg.total_weight -= cfg.type_cfg_map[type].weight;
		cfg.type_cfg_map[type].weight = 0;
	}
	if (cfg.total_weight <= 0)
	{
		return false;
	}
	const wake_weight_t rand_value = RandomNum(cfg.total_weight);
	wake_weight_t cur_weight = 0;
	for (auto &map_it : cfg.type_cfg_map)
	{
		cur_weight += map_it.second.weight;
		if (cur_weight > rand_value)
		{
			this->SetAwakeningTypeBase(part_index, lock_index, map_it.first);
			return true;
		}
	}
	return false;
}

bool ZhuanzhiEquip::IsAwakeningMaxLevel(int part_index, int lock_index)
{
	const int equip_order = this->GetZhuanzhiEquipOrder(part_index);
	const wake_level_t max_level = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetWakeLevelLimitCfg(equip_order);
	wake_level_t wake_level = this->GetAwakeningLevel(part_index, lock_index);
	if (wake_level < max_level)
	{
		return false;
	}
	return true;
}

void ZhuanzhiEquip::WakeRumor(int part_index)
{
	if (part_index < 0 || part_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX)
	{
		return;
	}
	const int equip_order = this->GetZhuanzhiEquipOrder(part_index);
	const wake_level_t max_level = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetWakeLevelLimitCfg(equip_order);
	for (int i = 0; i < MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT; ++i)
	{
		wake_level_t old_level = this->GetAwakeningEquipLevel(part_index, i);
		wake_level_t new_level = this->GetAwakeningLevel(part_index, i);
		wake_type_t wake_type = this->GetAwakeningType(part_index, i);
		//传闻
		if (old_level < max_level && new_level == max_level && LOGIC_CONFIG->GetZhuanzhiEquipConfig().WakeTypeIsPrecious(part_index, wake_type))
		{
			int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "zhuanzhi_wake_notice", UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(),
				(int)wake_type, (int)new_level);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, length, SYS_MSG_CENTER_ROLL);
			}
		}
	}
}

void ZhuanzhiEquip::ComposeZhuanzhiEquip(const Protocol::CSZhuanzhiEquipCompose *req)
{
	if (req == nullptr || req->xianpin_num <= 0 || req->bag_index_list == nullptr || req->bag_index_count <= 0 || req->bag_index_count > MAX_ZHUANZHI_EQUIP_COMPOSE_COUNT)
	{
		return;
	}

	const ZhuanzhiEquipComposeCfg *compose_cfg = LOGIC_CONFIG->GetZhuanzhiEquipConfig().GetEquipComposeCfg(req->item_id, req->xianpin_num);
	if (NULL == compose_cfg)
	{
		m_role->NoticeNum(errornum::EN_EQUIP_FUNC_CONFIG_ERROR);
		return;
	}
	if (req->bag_index_count < compose_cfg->need_equip_num_min || req->bag_index_count > compose_cfg->need_equip_num_max)
	{
		m_role->NoticeNum(errornum::EN_ZHUANZHI_EQUIP_COMPOSE_REQ_FAIL);
		return;
	}
	int success_index = req->bag_index_count - compose_cfg->need_equip_num_min;
	if (success_index < 0 || (size_t)success_index >= compose_cfg->success_rate.size())
	{
		m_role->NoticeNum(errornum::EN_ZHUANZHI_EQUIP_COMPOSE_REQ_FAIL);
		return;
	}


	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	const Equipment *equip_item = (const Equipment *)ITEMPOOL->GetItem(req->item_id);
	if (NULL == equip_item || ItemBase::I_TYPE_EQUIPMENT != equip_item->GetItemType())
	{
		m_role->NoticeNum(errornum::EN_SHENSHOU_ITEM_IS_NOT_EQUIP);
		return;
	}

	short compose_equip_type = equip_item->GetEquipType();
	if (!this->IsZhuanzhiEquipType(compose_equip_type))
	{
		gstr::SendError(m_role, "en_equip_compose_not_zhuanzhi_equip");
		return;
	}

	bool is_bind = false;
	std::set<int> real_bag_index_set;	// 有效的背包索引

	// 计算有效索引
	for (short i = 0; i < req->bag_index_count; i++)
	{
		ItemGridData grid_data;
		if (!m_role->GetKnapsack()->GetItemGridData(req->bag_index_list[i], &grid_data))
		{
			return;
		}

		if (!this->IsCanComposeZhuanzhi(equip_item, req->bag_index_list[i]))
		{
			return;
		}

		if (this->GetXianpinCountByBag(req->bag_index_list[i]) < compose_cfg->best_attr_count)
		{
			m_role->NoticeNum(errornum::EN_ZHUANZHI_EQUIP_COMPOSE_XIANPIN_NUM_ERROR);
			return;
		}

		if (real_bag_index_set.find(req->bag_index_list[i]) != real_bag_index_set.end())
		{
			m_role->NoticeNum(errornum::EN_ZHUANZHI_EQUIP_COMPOSE_INDEX_ERROR);
			return;
		}
		real_bag_index_set.insert(req->bag_index_list[i]);

		if (grid_data.is_bind)
		{
			is_bind = true;
		}
	}

	if ((int)real_bag_index_set.size() < compose_cfg->need_equip_num_min)
	{
		m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
		return;
	}

	// 消耗
	{
		for (std::set<int>::const_iterator set_it = real_bag_index_set.begin(); set_it != real_bag_index_set.end(); ++set_it)
		{
			//装备
			knapsack->ConsumeItemByIndex(*set_it, 1, "ZhuanzhiEquip::ComposeZhuanzhiEquip");
		}
		
	}

	int rand_value = RandomNum(10000);
	
	if (rand_value >= compose_cfg->success_rate[success_index])
	{
		m_role->NoticeNum(errornum::EN_ZHUANZHI_EQUIP_COMPOSE_FAIL);
		this->SyncComposeSucceed(false);
		return;
	}
	//给予随机属性
	{
		static ItemGridData item_grid_data; item_grid_data.Reset();

		int grid_index = -1;

		knapsack->Put(ItemConfigData(req->item_id, is_bind, 1), PUT_REASON_ZHIZUN_EQUIP_COMPOSE2, &grid_index);
		if (grid_index >= 0)
		{
			knapsack->GetItemGridData(grid_index, &item_grid_data);

			if (GLOBALCONFIG->GetEquipConfig().GetRandomXianpin(&item_grid_data, Equipment::EQUIPMENT_FIX_GETWAY_ZHUANZHI_EQUIP_COMPOSE, req->xianpin_num, req->xianpin_num))
			{
				knapsack->SetItemDirty(grid_index);

				m_role->NoticeNum(noticenum::NT_EQUIP_COMPOSE_SUCC);
				gamelog::g_log_zhaunzhi_equip.printf(LL_INFO, "ZhuanzhiEquip::ComposeZhuanzhiEquip success role(%d,%s) type:%d compose_id:%d xianpin_num:%d",
					m_role->GetUID(), m_role->GetName(), Equipment::EQUIPMENT_FIX_GETWAY_ZHUANZHI_EQUIP_COMPOSE, (int)req->item_id, (int)req->xianpin_num);
			}
			else
			{
				gamelog::g_log_zhaunzhi_equip.printf(LL_INFO, "ZhuanzhiEquip::ComposeZhuanzhiEquip fail role(%d,%s) type:%d compose_id:%d xianpin_num:%d",
					m_role->GetUID(), m_role->GetName(), Equipment::EQUIPMENT_FIX_GETWAY_ZHUANZHI_EQUIP_COMPOSE, (int)req->item_id, (int)req->xianpin_num);
			}
			this->SyncComposeSucceed(true);
		}

		//传闻
		if (compose_cfg->is_broadcast)
		{
			const EquipmentParam *equip_param = dynamic_cast<const EquipmentParam *>(item_grid_data.param);
			if (NULL != equip_param)
			{
				const char *equip_param_string = EquipFunction::EquipParamDataClientString(UidToInt(m_role->GetUserId()), req->item_id, is_bind, item_grid_data.invalid_time, equip_param, NULL);
				if (NULL != equip_param_string)
				{
					int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "zhuanzhi_compose_notice", UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(),
						req->xianpin_num, req->item_id, equip_param_string);
					if (length > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, length, SYS_MSG_CENTER_ROLL);
					}
				}
			}
		}
	}
}

void ZhuanzhiEquip::SyncComposeSucceed(bool is_succeed)
{
	Protocol::SCZhuanzhiEquipComposeSucceed info;
	info.is_succeed = is_succeed;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&info, sizeof(info));
}

bool ZhuanzhiEquip::IsCanComposeZhuanzhi(const Equipment *compose_equip, int index)
{
	if (compose_equip == nullptr)
	{
		return false;
	}
	ItemGridData grid_data;
	if (!m_role->GetKnapsack()->GetItemGridData(index, &grid_data))
	{
		return false;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(grid_data.item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType())
	{
		gstr::SendError(m_role, "en_equip_compose_puton_not_equip");
		return false;
	}

	short bag_equip_type = equip->GetEquipType();
	if (!this->IsZhuanzhiEquipType(bag_equip_type))
	{
		gstr::SendError(m_role, "en_equip_compose_not_zhuanzhi_equip");
		return false;
	}

	if (compose_equip->GetOrder() != equip->GetOrder())
	{
		gstr::SendError(m_role, "en_equip_compose_equip_order_not_same");
		return false;
	}

	const int compose_prof_limit = compose_equip->GetLimitProf();
	const int equip_prof_limit = equip->GetLimitProf();
	if (compose_prof_limit != PROF_TYPE_PROF_NO_LIMIT && equip_prof_limit != PROF_TYPE_PROF_NO_LIMIT && compose_prof_limit != equip_prof_limit)
	{
		gstr::SendError(m_role, "en_equip_compose_equip_type_not_same");
		return false;
	}

	EquipmentParam *equip_param = (EquipmentParam *)grid_data.param;
	if (NULL == equip_param)
	{
		gstr::SendError(m_role, "select_zhuanzhi_equip_limit");
		return false;
	}

	return true;
}

bool ZhuanzhiEquip::IsZhuanzhiEquipType(int equip_type)
{
	if (equip_type < Equipment::E_TYPE_ZHUANZHI_MIN || equip_type >= Equipment::E_TYPE_ZHUANZHI_MAX)
	{
		return false;
	}
	return true;
}

int ZhuanzhiEquip::GetXianpinCountByBag(short bag_index)
{
	ItemGridData grid_data;
	if (!m_role->GetKnapsack()->GetItemGridData(bag_index, &grid_data))
	{
		return 0;
	}
	int attr_type = 0;
	int count = 0;
	for (int k = 0; k < ItemNamespace::MAX_XIANPIN_NUM; ++k)
	{
		const EquipmentParam *equip_param = dynamic_cast<const EquipmentParam *>(grid_data.param);
		if (nullptr == equip_param)
		{
			continue;
		}
		
		attr_type = equip_param->xianpin_type_list[k];
		const EquipXianPinConfig *xianpin_cfg = GLOBALCONFIG->GetEquipConfig().GetXianPinEquipCfg(attr_type);
		if (NULL == xianpin_cfg)
		{
			continue;
		}
		++count;
	}
	return count;
}

int ZhuanzhiEquip::GetXianpinCount(int part_index)
{
	ItemDataWrapper &itemdata = m_param.zhuanzhi_equip_list[part_index].zhuanzhi_equip_data;
	if (itemdata.Invalid())
	{
		return 0;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(itemdata.item_id);
	if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)
	{
		return 0;
	}

	if (m_role->GetLevel() < equip->GetLimitLevel())
	{
		return 0;
	}

	int attr_type = 0;
	int count = 0;
	for (int k = 0; k < ItemNamespace::MAX_XIANPIN_NUM; ++k)
	{
		attr_type = itemdata.param_data.xianpin_type_list[k];
		const EquipXianPinConfig *xianpin_cfg = GLOBALCONFIG->GetEquipConfig().GetXianPinEquipCfg(attr_type);
		if (NULL == xianpin_cfg)
		{
			continue;
		}
		++count;
	}
	return count;
}

int ZhuanzhiEquip::GetForgeZhuanzhiNum() const
{
	int count = 0;
	const OneSuitInfo & suit_info = m_param.zhuanzhi_suit_order_list[0];
	for (int index = E_INDEX_ZHUANZHI::E_INDEX_WUQI; index < E_INDEX_ZHUANZHI::E_INDEX_MAX; ++index)
	{
		if ((suit_info.stuff_bind_list[index][0].item_id >0 && suit_info.stuff_bind_list[index][0].count >0) ||
			(suit_info.stuff_unbind_list[index][0].item_id > 0 && suit_info.stuff_unbind_list[index][0].count > 0))
			{
				count++;
			}
	}
	return count;
}

int ZhuanzhiEquip::GetEquipColor(int equip_index)
{
	if (equip_index < E_INDEX_ZHUANZHI::E_INDEX_WUQI || equip_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX)
	{
		return 0;
	}

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(m_param.zhuanzhi_equip_list[equip_index].zhuanzhi_equip_data.item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType())
	{
		return 0;
	}

	return equip->GetColor();
}

int ZhuanzhiEquip::GetEquipTotalStarLevel()
{
	int total_star = 0;
	for (int i = 0; i < E_INDEX_ZHUANZHI::E_INDEX_MAX; ++i)
	{
		total_star += m_param.zhuanzhi_equip_list[i].star_level;
	}
	return total_star;
}

const ItemDataWrapper * ZhuanzhiEquip::GetEquipmentGridData(short equip_index)
{
	if (equip_index < E_INDEX_ZHUANZHI::E_INDEX_WUQI || equip_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX)
	{
		return nullptr;
	}

	if (m_param.zhuanzhi_equip_list[equip_index].zhuanzhi_equip_data.Invalid()) return nullptr;

	return &m_param.zhuanzhi_equip_list[equip_index].zhuanzhi_equip_data;
}