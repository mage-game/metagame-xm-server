#include "dressingroommgr.hpp"
#include "obj/character/role.h"
#include "other/capability/capability.hpp"
#include "config/logicconfigmanager.hpp"
#include "dressingroomconfig.hpp"
#include "skill/skillbase.hpp"
#include "engineadapter.h"
#include "gamecommon.h"
#include "protocal/msgdressingroom.hpp"
#include "servercommon/errornum.h"
#include "item/knapsack.h"
#include "item/itemextern.hpp"

DressingRoomMgr::DressingRoomMgr() : m_role(nullptr)
{
	
}

void DressingRoomMgr::InitParam(const DressingRoomParam &param)
{
	m_param = param;
}

void DressingRoomMgr::GetInitParam(DressingRoomParam *param)
{
	*param = m_param;
}

void DressingRoomMgr::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		const int suit_count = LOGIC_CONFIG->GetDressingRoomConfig().GetSuitCount();
		for (int i = 0; i < suit_count && i < DressingRoomParam::MAX_SUIT_COUNT; ++i)
		{
			const int suit_img_count = this->CalcImgCount(i);
			const DressingRoomSuitCfg::SuitAttrCfg *cfg = LOGIC_CONFIG->GetDressingRoomConfig().GetSuitAttrCfg(i, suit_img_count);
			if (nullptr != cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg->hp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += cfg->jianren;

				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg->hp_per * SKILL_ATTR_PER;
				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg->gongji_per * SKILL_ATTR_PER;
				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg->fangyu_per * SKILL_ATTR_PER;
				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg->mingzhong_per * SKILL_ATTR_PER;
				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg->shanbi_per * SKILL_ATTR_PER;
				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += cfg->baoji_per * SKILL_ATTR_PER;
				m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += cfg->jianren_per * SKILL_ATTR_PER;
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_DRESSING_ROOM, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
	base_add.AddPercent(m_attrs_add.m_percent_attrs);
}

void DressingRoomMgr::OnDressingRoomOpera(int opera_type)
{
	switch (opera_type)
	{
	case Protocol::DRESSING_ROOM_OPEAR_TYPE_INFO:
		{
			this->SendDressingRoomInfo();
		}
		break;

	default:
		break;
	}
}

void DressingRoomMgr::SendDressingRoomInfo()
{
	static Protocol::SCDressingRoomInfo pro;
	pro.suit_count = LOGIC_CONFIG->GetDressingRoomConfig().GetSuitCount();

	for (int i = 0; i < pro.suit_count && i < DressingRoomParam::MAX_SUIT_COUNT; ++i)
	{
		pro.info_list[i] = m_param.suit_list[i];
	}

	int send_len = sizeof(pro) - sizeof(pro.info_list[0]) * (DressingRoomParam::MAX_SUIT_COUNT - pro.suit_count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&pro, send_len);
}

void DressingRoomMgr::CheckSuitChange(int system_type, int img_id, bool active)
{
	const auto cfg = LOGIC_CONFIG->GetDressingRoomConfig().GetSuitInfo(system_type, img_id);
	if (nullptr != cfg)
	{
		const int suit_cfg_count = LOGIC_CONFIG->GetDressingRoomConfig().GetSuitCount();
		if (cfg->suit_index >= 0 && cfg->suit_index < suit_cfg_count && cfg->suit_index < DressingRoomParam::MAX_SUIT_COUNT)
		{
			if (active)
			{
				m_param.suit_list[cfg->suit_index] |= 1 << cfg->sub_index;
			}
			else
			{
				m_param.suit_list[cfg->suit_index] &= ~(1 << cfg->sub_index);
			}
		}

		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_DRESSING_ROOM);
		this->SendDressingRoomSingleInfo(cfg->suit_index, cfg->sub_index, active);
	}
}

void DressingRoomMgr::SendDressingRoomSingleInfo(int suit_index, int img_index, bool active)
{
	Protocol::SCDressingRoomSingleInfo pro;
	pro.is_active = active ? 1 : 0;
	pro.info.suit_index = suit_index;
	pro.info.img_index = img_index;

	SEND_TO_ROLE(m_role, pro);
}

void DressingRoomMgr::OnDressingRoomExchange(int suit_index, int sub_index)
{
	time_t now = EngineAdapter::Instance().Time();
	unsigned int open_time = LOGIC_CONFIG->GetDressingRoomConfig().GetSuitOpenTimeStamp(suit_index);
	if (open_time <= 0 || now < open_time)
	{
		return;
	}

	const SuitIDCfg *cfg = LOGIC_CONFIG->GetDressingRoomConfig().GetSuitInfoByIndex(suit_index, sub_index);
	if (nullptr == cfg)
	{
		m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
		return;
	}
	if (cfg->need_exchange_ticket_num == -1)
	{
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(1))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	{
		// ÏûºÄ
		static ItemExtern::ItemConsumeList consume_list;
		consume_list.Reset();

		int stuff_count = 0;
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

		const DressRoomOtherCfg &other_cfg = LOGIC_CONFIG->GetDressingRoomConfig().GetOtherCfg();
		stuff_list[stuff_count].item_id = other_cfg.exchange_ticket_item_id;
		stuff_list[stuff_count].num = cfg->need_exchange_ticket_num;
		++stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, false))
		{
			m_role->NoticeNum(errornum::EN_DRESSING_ROOM_EXCHANGE_ITEM_NOT_ENOUGH);
			return;
		}

		if (consume_list.count > 0 && !m_role->GetKnapsack()->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "OnDressingRoomExchange"))
		{
			m_role->NoticeNum(errornum::EN_DRESSING_ROOM_EXCHANGE_ITEM_NOT_ENOUGH);
			return;
		}
	}

	{
		ItemConfigData item;
		item.item_id = cfg->img_item_id;
		item.num = 1;

		m_role->GetKnapsack()->Put(item, PUT_REASON_DRESSING_ROOM_EXCHANGE);
	}
	
	return;
}

void DressingRoomMgr::FirstInit()
{
	m_param.Reset();
	const int suit_count = LOGIC_CONFIG->GetDressingRoomConfig().GetSuitCount();
	for (int i = 0; i < suit_count; ++i)
	{
		auto vec_pointer = LOGIC_CONFIG->GetDressingRoomConfig().GetSuitIDCfgVec(i);
		if (nullptr != vec_pointer)
		{
			for (const auto &cfg : *vec_pointer)
			{
				if (cfg.suit_index >= 0 && cfg.suit_index < suit_count && cfg.suit_index < DressingRoomParam::MAX_SUIT_COUNT) 
				{
					if (this->CheckActiveSpecialImg(cfg.suit_system_type, cfg.img_id))
					{
						m_param.suit_list[cfg.suit_index] |= 1 << cfg.sub_index;
					}
				}
			}
		}
	}
	this->SendDressingRoomInfo();
}

bool DressingRoomMgr::CheckActiveSpecialImg(int system_type, int img_id)
{
	const SystemFuncHelper *func = LOGIC_CONFIG->GetDressingRoomConfig().GetFuncHelper(system_type);
	if (nullptr != func && nullptr != func->check_special_img_func)
	{
		return func->check_special_img_func(m_role, img_id);
	}

	return false;
}

int DressingRoomMgr::CalcImgCount(int suit_index)
{
	if (suit_index < 0 || suit_index >= DressingRoomParam::MAX_SUIT_COUNT)
	{
		return 0;
	}

	int count = 0;
	int flag = m_param.suit_list[suit_index];
	for (; flag != 0; flag &= (flag - 1))
	{
		++count;
	}

	return count;
}
