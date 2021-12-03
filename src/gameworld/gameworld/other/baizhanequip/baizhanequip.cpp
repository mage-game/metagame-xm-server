#include <algorithm>

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"

#include "gamecommon.h"

#include "globalconfig/globalconfig.h"

#include "engineadapter.h"
#include "baizhanequip.h"

#include "protocal/msgrole.h"
#include "protocal/msgother.h"

#include "item/knapsack.h"
#include "item/itempool.h"
#include "item/itemextern.hpp"

#include "scene/scenemanager.h"
#include "scene/scene.h"

#include "world.h"
#include "gamelog.h"
#include "gstr.h"

#include "obj/character/role.h"

#include "other/event/eventhandler.hpp"

#include "config/logicconfigmanager.hpp"

#include "baizhanequipconfig.hpp"
#include "other/sublock/sublock.hpp"
#include "equipment/equipmentmanager.h"
#include "equipment/equipmentfunction.h"
#include "other/tianshuxunzhu/tianshuxunzhu.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////
BaizhanEquip::BaizhanEquip()
{
}

BaizhanEquip::~BaizhanEquip()
{

}

void BaizhanEquip::SetRole(Role *role)
{
	m_role = role;
}

void BaizhanEquip::Init(Role *role, const BaizhanEquipParam &param)
{
	m_role = role;
	m_param = param;
}

void BaizhanEquip::GetInitParam(BaizhanEquipParam *param)
{
	*param = m_param;
}

void BaizhanEquip::OnInitFinish()
{
	
}

void BaizhanEquip::Update(time_t now_second)
{

}

void BaizhanEquip::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		CharIntAttrs base_equip_attrs;

		for (int part_index = 0; part_index < E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_MAX; ++part_index)
		{
			BaizhanEquipItem &equip_data = m_param.baizhan_equip_list[part_index];

			//装备属性
			{
				ItemDataWrapper &itemdata = equip_data.baizhan_equip_data;
				if (itemdata.Invalid())
				{
					continue;
				}

				const Equipment *equip = GetBaizhanEquip(itemdata.item_id);
				if (nullptr == equip)
				{
					continue;
				}

				// 基础属性,注意每次获取都reset
				base_equip_attrs.Reset();
				equip->GetBaseAttr(base_equip_attrs);

				m_attrs_add.Add(base_equip_attrs.m_attrs);

			}
		}

		// 套装
		this->GetSuitTotalAttr(m_attrs_add);

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_BAIZHAN_EQUIP, m_attrs_add);
	}

	m_param.capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_BAIZHAN_EQUIP);

	base_add.Add(m_attrs_add.m_attrs, true);
	base_add.AddPercent(m_attrs_add.m_percent_attrs);
}

int BaizhanEquip::GetEquipIndexByType(int equip_type)
{
	switch (equip_type)
	{
	case Equipment::E_TYPE_BAIZHAN_WUQI:
		return E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_WUQI;

	case Equipment::E_TYPE_BAIZHAN_YIFU:
		return E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_YIFU;

	case Equipment::E_TYPE_BAIZHAN_HUSHOU:
		return E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_HUSHOU;

	case Equipment::E_TYPE_BAIZHAN_YAODAI:
		return E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_YAODAI;

	case Equipment::E_TYPE_BAIZHAN_TOUKUI:
		return E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_TOUKUI;

	case Equipment::E_TYPE_BAIZHAN_XIANGLIAN:
		return E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_XIANGLIAN;

	case Equipment::E_TYPE_BAIZHAN_SHOUZHUO:
		return E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_SHOUZHUO;

	case Equipment::E_TYPE_BAIZHAN_JIEZHI:
		return E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_JIEZHI;

	case Equipment::E_TYPE_BAIZHAN_XIEZI:
		return E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_XIEZI;

	case Equipment::E_TYPE_BAIZHAN_YUPEI:
		return E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_YUPEI;

	default: return -1;

	}
}		

void BaizhanEquip::OnOperaReq(int opera_type, int param1, int param2)
{
	switch (opera_type)
	{
	case Protocol::BAIZHAN_EQUIP_OPERATE_TYPE_ALL_INFO:
	{
		this->SyncBaizhanEquipAllInfo();
	}
	break;

	case Protocol::BAIZHAN_EQUIP_OPERATE_TYPE_TAKE_OFF:
	{
		this->TakeOff(param1, false);
	}
	break;

	case Protocol::BAIZHAN_EQUIP_OPERATE_TYPE_UP_LEVEL:
	{
		this->OnPutonOrUplevel(param1, param2);
	}
	break;

	default:
		break;
	}
}

int BaizhanEquip::PutOn(const ItemDataWrapper &item_wrapper)
{
	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_wrapper.item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType()) return -1;
	
	if (0 == item_wrapper.has_param) return -2;

	short equip_type = equip->GetEquipType();
	if (Equipment::E_TYPE_BAIZHAN_MIN > equip_type || Equipment::E_TYPE_BAIZHAN_MAX <= equip_type) return -3;

	int real_index = GetEquipIndexByType(equip_type);
	if (real_index < E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_WUQI || real_index >= E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_MAX) return -4;

	// 检查能否穿戴
	if (!IsCanPuton(real_index, equip->GetOrder(), item_wrapper.item_id))
	{
		return -5;
	}

	ItemDataWrapper &itemdata = m_param.baizhan_equip_list[real_index].baizhan_equip_data;
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

	m_param.suit_order_list[BAIZHAN_SUIT_TYPE_1].order_list[real_index] = equip->GetOrder();

	EventHandler::Instance().OnPutOnBaizhanEquip(m_role, equip_type, equip->GetColor(), equip->GetLimitLevel());	// 放在Reset下面

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_BAIZHAN_EQUIP);

	this->SyncBaizhanEquipAllInfo();
	TianShuXunZhu::CheckBaiZhanTianshu(m_role);
	// log
	this->PrintLog("ZhuanzhiEquip::PutOn", real_index);

	return 0;
}

int BaizhanEquip::TakeOff(short equip_index, bool is_from_puton)
{
	if (equip_index < E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_WUQI || equip_index >= E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_MAX) return -1;

	ItemDataWrapper &itemdata = m_param.baizhan_equip_list[equip_index].baizhan_equip_data;
	if (itemdata.Invalid()) return -2;

	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(itemdata.item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType() ||
		equip->GetEquipType() < Equipment::E_TYPE_BAIZHAN_MIN || equip->GetEquipType() >= Equipment::E_TYPE_BAIZHAN_MAX)
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
	 
	itemdata.Reset();

	if (!is_from_puton)
	{
		m_param.suit_order_list[BAIZHAN_SUIT_TYPE_1].order_list[equip_index] = 0;
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_BAIZHAN_EQUIP);
		this->SyncBaizhanEquipAllInfo();
	}

	// log
	this->PrintLog("BaizhanEquip::TakeOff", equip_index, itemdata.item_id);

	return 0;
}

bool BaizhanEquip::CheckKnapsack()
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

bool BaizhanEquip::IsCanPuton(int equip_part, int equip_order, ItemID item_id)
{
	if (equip_part < 0 || equip_part >= E_INDEX_BAIZHAN_MAX || equip_order <= 0 || equip_order > ItemPool::MAX_EQUIPMENT_ORDER)
	{
		return false;
	}

	const BaizhanEquipCfg *limit_cfg = LOGIC_CONFIG->GetBaizhanEquipConfig().GetBaizhanEquipCfg(equip_part, equip_order);
	if (NULL == limit_cfg)
	{
		return false;
	}

	int role_level = limit_cfg->role_need_min_level;

	if (m_role->GetLevel() < role_level)
	{
		m_role->NoticeNum(errornum::EN_ITEM_LEVEL_LIMIT);
		return false;
	}

	return true;
}

const Equipment * BaizhanEquip::GetBaizhanEquip(ItemID item_id)
{
	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_id);

	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType())
	{
		return nullptr;
	}

	if (equip->GetEquipType() < Equipment::E_TYPE_BAIZHAN_MIN || equip->GetEquipType() >= Equipment::E_TYPE_BAIZHAN_MAX)
	{
		return nullptr;
	}

	return equip;
}

const ItemDataWrapper * BaizhanEquip::GetEquipmentGridData(short equip_index)
{
	if (equip_index < E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_WUQI || equip_index >= E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_MAX)
	{
		return nullptr;
	}

	if (m_param.baizhan_equip_list[equip_index].baizhan_equip_data.Invalid()) return nullptr;

	return &m_param.baizhan_equip_list[equip_index].baizhan_equip_data;
}

int BaizhanEquip::ReCalcSameOrderCount(int order)
{
	int same_order_count = 0;

	for (int equip_index = 0; equip_index < E_INDEX_BAIZHAN_MAX; equip_index ++)
	{
		int equip_order = m_param.suit_order_list[BAIZHAN_SUIT_TYPE_1].order_list[equip_index];
	
		if (order != equip_order)
		{
			continue;
		}

		same_order_count++;
	}

	return same_order_count;
}

void BaizhanEquip::GetSuitTotalAttr(CharIntAttrs &tmp_attr)
{
	// 套装
	for (int equip_order = 0; equip_order < ItemPool::MAX_EQUIPMENT_ORDER; ++equip_order)
	{
		int cur_order_count = this->ReCalcSameOrderCount(equip_order);

		const SuitAttrCfg *suit_cfg = LOGIC_CONFIG->GetBaizhanEquipConfig().GetSuitAttrCfg(equip_order, cur_order_count);
		if (NULL != suit_cfg)
		{
			tmp_attr.AddAttrFromConfig(suit_cfg->attr_cfg);

			tmp_attr.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, suit_cfg->per_pvp_zengshenang);
			tmp_attr.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, suit_cfg->per_pvp_mianshang);
			tmp_attr.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_HUIXINYIJI, suit_cfg->huixinyiji);
			tmp_attr.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN, suit_cfg->per_jingzhun);
			tmp_attr.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_ZHUFUYIJI_PER, suit_cfg->per_zhufuyiji);
			tmp_attr.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER, suit_cfg->skill_zengshang_per);
			tmp_attr.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER, suit_cfg->skill_jianshang_per);
			tmp_attr.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, suit_cfg->per_pofang);

			tmp_attr.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += suit_cfg->per_maxhp * ROLE_ATTR_PER;
			tmp_attr.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += suit_cfg->per_gongji * ROLE_ATTR_PER;
		}
	}
}

void BaizhanEquip::OnPutonOrUplevel(int part_index, int item_id)
{
	if (part_index < 0 || part_index >= E_INDEX_BAIZHAN_MAX)
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

	// 判断是穿戴或升级
	ItemDataWrapper &itemdata = m_param.baizhan_equip_list[part_index].baizhan_equip_data;
	if (item_id <= 0 && itemdata.item_id <= 0)
	{
		gstr::SendError(m_role, "illegal_param");
		return;
	}

	ItemID old_item_id = itemdata.item_id > 0 ? itemdata.item_id : item_id;
	const BaizhanEquipLevelCfg * level_cfg = LOGIC_CONFIG->GetBaizhanEquipConfig().GetBaizhanEquipLevelCfg(old_item_id, part_index);
	if (nullptr == level_cfg)
	{
		gstr::SendError(m_role, "get_cfg_error");
		return;
	}

	const Equipment *equip = GetBaizhanEquip(level_cfg->new_equip_item_id);
	if (nullptr == equip) return;

	// 检查配置装备id对应部位是否正确
	int real_index = GetEquipIndexByType(equip->GetEquipType());
	if (real_index != part_index)
	{
		gstr::SendError(m_role, "get_cfg_error");
		return;
	}

	if (m_role->GetLevel() < equip->GetLimitLevel())
	{
		gstr::SendError(m_role, "role_level_limit");
		return;
	}

	//检查材料
	int need_stuff_num = itemdata.item_id > 0 ? level_cfg->need_num - 1 : level_cfg->need_num;
	int old_equip_item_id = old_item_id;
	bool is_stuff_enough = false;

	ItemExtern::ItemConsumeConfigList stuff_list;

	for (int count = 0; count < 15; count++)
	{
		int bag_stuff_count = knapsack->Count(old_equip_item_id);
		if (bag_stuff_count >= need_stuff_num)
		{
			stuff_list.add_item(ItemExtern::ItemConsumeConfig(old_equip_item_id, false, need_stuff_num));
			is_stuff_enough = true;
			break;
		}
		else
		{
			const BaizhanEquipLevelCfg * reverse_level_cfg = LOGIC_CONFIG->GetBaizhanEquipConfig().GetBaizhanEquipLevelReverseCfg(old_equip_item_id, part_index);
			if (nullptr == reverse_level_cfg)
			{
				continue;
			}
			if (bag_stuff_count > 0)
			{
				stuff_list.add_item(ItemExtern::ItemConsumeConfig(old_equip_item_id, false, bag_stuff_count));
			}

			old_equip_item_id = reverse_level_cfg->old_equip_item_id;
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
		if (!stuff_list.done(*m_role, "BaizhanEquip::OnEquipLevelUp"))
		{
			return;
		}
	}

	// 生效

	//如果是穿戴
	if (itemdata.item_id <= 0)
	{
		itemdata.item_id = level_cfg->new_equip_item_id;
		itemdata.num = 1;
		m_param.suit_order_list[BAIZHAN_SUIT_TYPE_1].order_list[part_index] = equip->GetOrder();
		EventHandler::Instance().OnPutOnBaizhanEquip(m_role, equip->GetEquipType(), equip->GetColor(), equip->GetLimitLevel());	// 放在Reset下面
	}
	else
	{
		itemdata.item_id = level_cfg->new_equip_item_id;
		m_param.suit_order_list[BAIZHAN_SUIT_TYPE_1].order_list[part_index] = equip->GetOrder();
	}
	
	if (1 == level_cfg->is_broadcast)
	{
		int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "baizhuan_equip_uplevel", UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(),
			level_cfg->new_equip_item_id);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, length, SYS_MSG_CENTER_ROLL_2);
		}
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_BAIZHAN_EQUIP);
	this->SyncBaizhanEquipAllInfo();
	TianShuXunZhu::CheckBaiZhanTianshu(m_role);
	gstr::SendNotice(m_role, "level_up_succ");
	// log
	this->PrintLog("BaizhanEquip::OnEquipLevelUp", part_index, level_cfg->old_equip_item_id);
}

void BaizhanEquip::SyncBaizhanEquipAllInfo()
{
	static Protocol::SCBaizhanEquipAllInfo beai;
	memset(beai.part_order_list, 0, sizeof(beai.part_order_list));

	for (int i = 0; i < E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_MAX; ++i)
	{
		beai.baizhan_equip_list[i].Reset();
	}

	for (int i = 0; i < E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_MAX; ++i)
	{
		if (m_param.baizhan_equip_list[i].baizhan_equip_data.Invalid()) continue;

		beai.baizhan_equip_list[i].Reset(m_param.baizhan_equip_list[i].baizhan_equip_data);
	}

	UNSTD_STATIC_CHECK(sizeof(beai.part_order_list) == sizeof(m_param.suit_order_list[BAIZHAN_SUIT_TYPE_1].order_list));
	memcpy(beai.part_order_list, m_param.suit_order_list[BAIZHAN_SUIT_TYPE_1].order_list, sizeof(beai.part_order_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&beai, sizeof(beai));
}

void BaizhanEquip::PrintLog(const char *reason, int part_index, int param1, int param2, int param3, int param4)
{
	if (part_index < 0 || part_index >= E_INDEX_BAIZHAN_MAX)
	{
		return;
	}

	//const BaizhanEquipItem &equip_item = m_param.baizhan_equip_list[part_index];

	gamelog::g_log_baizhan_equip.printf(LL_INFO, "BaizhanEquip role(%d,%s) %s:part_index(%d)(%d,%d,%d,%d) ",
		m_role->GetUID(), m_role->GetName(),
		reason, part_index, param1, param2, param3, param4
		);
}

int BaizhanEquip::HaveEquipNum()
{
	int count = 0;
	for (int i = E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_WUQI; i < E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_MAX; ++i)
	{
		if (m_param.baizhan_equip_list[i].baizhan_equip_data.Invalid())
		{
			continue;
		}
		count++;
	}

	return count;
}
