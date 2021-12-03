#include "orangeequipcollect.hpp"
#include "gamecommon.h"
#include "servercommon/maildef.hpp"
#include "globalconfig/otherconfig.h"
#include "protocal/msgother.h"
#include "config/logicconfigmanager.hpp"
#include "globalconfig/globalconfig.h"
#include "servercommon/string/gameworldstr.h"
#include "other/capability/capability.hpp"
#include "gameworld/equipment/equipmentmanager.h"
#include "gameworld/world.h"
#include "gameworld/scene/scenemanager.h"
#include "gameworld/other/route/mailroute.hpp"
#include "config/shenbingtuluconfig.h"

OrangeEquipCollect::OrangeEquipCollect() : m_role(nullptr)
{
	memset(m_collect_info_list, 0, sizeof(m_collect_info_list));
}

void OrangeEquipCollect::InitParam(Role *role, const OrangeEquipCollectParam &param)
{
	m_role = role;
	m_param = param;
}

void OrangeEquipCollect::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (!is_recalc)
	{
		base_add.Add(m_attrs_add.m_attrs, true);
		return;
	}

	this->RefreshCollectInfo();
	m_attrs_add.Reset();

	// 个数属性
	for (int seq = 0; seq < ORANGE_EQUIP_COLLECT_SEQ_MAX_COUNT; ++seq)
	{
		const auto &collect_info = m_collect_info_list[seq];

		double attr_factor = 1;
		for (int item_cnt = 1; item_cnt <= collect_info.item_count; ++item_cnt)
		{
			auto cfg = LOGIC_CONFIG->GetShenBingTuLuConfig().GetOrangeEquipCollectAttrCfg(seq, item_cnt);
			if (cfg)
			{
				cfg->attr_cfg.AddAttrs(m_attrs_add, attr_factor);
			}
		}
	}

	base_add.Add(m_attrs_add.m_attrs, true);
	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_ORANGE_EQUIP_COLLET, m_attrs_add);
}

void OrangeEquipCollect::OnRoleLogin()
{
	this->OnCheckActive(m_role->GetLevel());
}

void OrangeEquipCollect::OnEnterScene(int scene_id)
{
}

void OrangeEquipCollect::OnCheckActive(int to_level)
{
	bool has_change = false;
	for (int suit_seq = 0; suit_seq < ORANGE_EQUIP_COLLECT_SEQ_MAX_COUNT; ++suit_seq)
	{
		auto orange_suit_cfg = LOGIC_CONFIG->GetShenBingTuLuConfig().GetOrangeEquipCollectOtherCfg(suit_seq);
		if (orange_suit_cfg != nullptr)
		{
			if (to_level >= orange_suit_cfg->active_level && !IS_BIT_SET(m_param.seq_active_flag, orange_suit_cfg->seq))
			{
				SET_BIT(m_param.seq_active_flag, orange_suit_cfg->seq);
				has_change = true;
			}
		}
	}

	if (has_change)
	{
		this->SendOtherInfo();
	}
}

ItemDataWrapper * OrangeEquipCollect::GetItem(int seq, int slot)
{
	if (!IsValidOrangeEquipCollectSeq(seq) || !IsValidOrangeEquipCollectSlot(slot))
	{
		return nullptr;
	}

	return &m_param.equip_slot[seq * ORANGE_EQUIP_COLLECT_SLOT_COUNT + slot];
}

void OrangeEquipCollect::RefreshCollectInfo()
{
	memset(m_collect_info_list, 0, sizeof(m_collect_info_list));

	bool need_send = false;

	const ShenBingTuLuOtherConfig &other_cfg = LOGIC_CONFIG->GetShenBingTuLuConfig().GetOtherCfg();

	for (int seq = 0; seq < ORANGE_EQUIP_COLLECT_SEQ_MAX_COUNT; ++seq)
	{
		auto &collect_info = m_collect_info_list[seq];

		for (int slot = 0; slot < ORANGE_EQUIP_COLLECT_SLOT_COUNT; ++slot)
		{
			const auto &item = m_param.equip_slot[seq * ORANGE_EQUIP_COLLECT_SLOT_COUNT + slot];
			if (!item.Invalid())
			{
				++collect_info.item_count;

				int stars = m_role->GetEquipmentManager()->GetXianPinHightQualityCount(item.param_data.xianpin_type_list);
				collect_info.stars += stars;
			}
		}

		// 首次集齐套装，发传闻
		if (!IS_BIT_SET(m_param.collect_succ_msg_send_flag, seq))
		{
			if (collect_info.item_count >= other_cfg.orange_equip_collect_active_puton_count)
			{
				int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_orange_equip_collect_notice_content,
					m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), seq);

				if (sendlen > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);

					SET_BIT(m_param.collect_succ_msg_send_flag, seq);
					need_send = true;
				}
			}
		}
	}

	if (need_send)
	{
		this->SendOtherInfo();
	}
}

void OrangeEquipCollect::CheckGiveActReward()
{
	
}

void OrangeEquipCollect::SendAllInfo()
{
	for (int seq = 0; seq < ORANGE_EQUIP_COLLECT_SEQ_MAX_COUNT; ++seq)
	{
		this->SendSeqInfo(seq);
	}

	this->SendOtherInfo();
}

int OrangeEquipCollect::GetCollectCount() const
{
	int cnt = 0;
	for (int i = 0; i < ORANGE_EQUIP_COLLECT_SEQ_MAX_COUNT; ++i)
	{
		if (IS_BIT_SET(m_param.collect_succ_msg_send_flag, i))
		{
			++cnt;
		}
	}
	return cnt;
}

void OrangeEquipCollect::SendSeqInfo(int seq)
{
	if (!IsValidOrangeEquipCollectSeq(seq))
	{
		return;
	}

	static Protocol::SCOrangeEquipCollect msg;
	msg.seq = seq;

	int beg_index = seq * ORANGE_EQUIP_COLLECT_SLOT_COUNT;
	int count = 0;
	for (int i = beg_index; i < beg_index + ORANGE_EQUIP_COLLECT_SLOT_COUNT; ++i)
	{
		msg.equip_slot[count++] = m_param.equip_slot[i];
	}

	SEND_TO_ROLE(m_role, msg);
}

void OrangeEquipCollect::SendOtherInfo()
{
	static Protocol::SCOrangeEquipCollectOther msg;
	msg.seq_active_flag = m_param.seq_active_flag;
	msg.act_reward_can_fetch_flag = 0;
	msg.collect_count = this->GetCollectCount();
	msg.active_reward_fetch_flag = m_param.active_reward_fetch_flag;
	for (int seq = 0; seq < ORANGE_EQUIP_COLLECT_ACT_REWARD_MAX_COUNT; ++seq)
	{
		if (IS_BIT_SET(m_param.opengame_act_reward_fetch_flag, seq))
		{
			continue;
		}

		auto reward_cfg = LOGIC_CONFIG->GetShenBingTuLuConfig().GetOrangeEquipCollectActCfg(seq);
		if (reward_cfg && msg.collect_count >= reward_cfg->collect_count)
		{
			SET_BIT(msg.act_reward_can_fetch_flag, seq);
		}
	}

	ARRAYCPY(msg.collect_info_list, m_collect_info_list);

	SEND_TO_ROLE(m_role, msg);
}