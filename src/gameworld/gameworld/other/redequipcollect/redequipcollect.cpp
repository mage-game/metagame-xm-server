#include "redequipcollect.hpp"
#include "gamecommon.h"
#include "servercommon/maildef.hpp"
#include "protocal/msgother.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "config/logicconfigmanager.hpp"
#include "globalconfig/globalconfig.h"
#include "servercommon/string/gameworldstr.h"
#include "other/capability/capability.hpp"
#include "gameworld/equipment/equipmentmanager.h"
#include "gameworld/world.h"
#include "gameworld/scene/scenemanager.h"
#include "gameworld/other/route/mailroute.hpp"
#include "config/shenbingtuluconfig.h"

RedEquipCollect::RedEquipCollect() : m_role(0)
{
	memset(m_collect_info_list, 0, sizeof(m_collect_info_list));
}

void RedEquipCollect::InitParam(Role *role, const RedEquipCollectParam &param)
{
	m_role = role;
	m_param = param;
	this->RefreshCollectInfo();//初始化星数，称号初始化属性计算要用
}

bool RedEquipCollect::GetActiveTitleList(std::vector<unsigned short> &title_list)
{
	for (int i = 0; i < 32; ++i)
	{
		if (!IS_BIT_SET(m_param.active_reward_fetch_flag, i))
		{
			continue;
		}

		auto cfg = LOGIC_CONFIG->GetShenBingTuLuConfig().GetRedEquipCollectOtherCfg(i, m_role->GetBaseProf());
		if (cfg != nullptr && cfg->reward_title_id > 0)
		{
			title_list.push_back(cfg->reward_title_id);
		}
	}

	if (!title_list.empty())
	{
		return true;
	}

	return false;
}

void RedEquipCollect::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (!is_recalc)
	{
		base_add.Add(m_attrs_add.m_attrs, true);
		return;
	}

	this->RefreshCollectInfo();
	m_attrs_add.Reset();

	// 个数属性
	for (int seq = 0; seq < RED_EQUIP_COLLECT_SEQ_MAX_COUNT; ++seq)
	{
		const auto &collect_info = m_collect_info_list[seq];

		double attr_factor = 1;
		/*auto other_cfg = LOGIC_CONFIG->GetShenBingTuLuConfig().GetRedEquipCollectOtherCfg(seq, m_role->GetBaseProf());
		if (other_cfg)
		{
			attr_factor += (collect_info.stars * 0.01 * other_cfg->star_add_attr_percent);
		}*/

		for (int item_cnt = 1; item_cnt <= collect_info.item_count; ++item_cnt)
		{
			auto cfg = LOGIC_CONFIG->GetShenBingTuLuConfig().GetRedEquipCollectAttrCfg(seq, item_cnt);
			if (cfg)
			{
				cfg->attr_cfg.AddAttrs(m_attrs_add, attr_factor);
			}
		}
	}

	base_add.Add(m_attrs_add.m_attrs, true);
	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_RED_EQUIP_COLLECT, m_attrs_add);
}

void RedEquipCollect::OnDayChange()
{
	//this->CheckGiveActReward();
}

void RedEquipCollect::OnPutonEquip()
{
	bool is_change = false;

	// 检查一下激活标记
	const ShenBingTuLuOtherConfig &other_cfg = LOGIC_CONFIG->GetShenBingTuLuConfig().GetOtherCfg();

	for (int seq = 0; seq < RED_EQUIP_COLLECT_SEQ_MAX_COUNT; ++seq)
	{
		if (IS_BIT_SET(m_param.seq_active_flag, seq)) continue;

		auto cfg = LOGIC_CONFIG->GetShenBingTuLuConfig().GetRedEquipCollectOtherCfg(seq, m_role->GetBaseProf());
		if (!cfg) continue;

		short put_on_count = 0;
		for (int equip_idx = 0; equip_idx < Equipment::E_INDEX_MAX; ++equip_idx)
		{
			// 玉佩手镯不检查
			if (equip_idx == Equipment::E_INDEX_YUPEI || equip_idx == Equipment::E_INDEX_SHOUZHUO)
			{
				continue;
			}

			ItemGridData equip_data;
			if (m_role->GetEquipmentManager()->GetEquipmentGridData(equip_idx, &equip_data))
			{
				auto item_cfg = ITEMPOOL->GetItem(equip_data.item_id);
				if (item_cfg != nullptr && item_cfg->GetColor() >= cfg->color 
					&& ItemBase::I_TYPE_EQUIPMENT == item_cfg->GetItemType() && ((Equipment*)item_cfg)->GetOrder() >= cfg->order)
				{
					++put_on_count;
				}
			}
		}

		// 激活
		if (put_on_count >= other_cfg.red_equip_collect_active_puton_count)
		{
			SET_BIT(m_param.seq_active_flag, seq);
			is_change = true;
		}
	}

	if (is_change)
	{
		this->SendOtherInfo();
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TITLE);    // 称号星数加成
		gamelog::g_log_other.printf(LL_INFO, "RedEquipCollect::OnPutonEquip, role_id: %d, role_name: %s, seq_active_flag: %u",
			m_role->GetUID(), m_role->GetName(), m_param.seq_active_flag);
	}
}

void RedEquipCollect::OnRoleLogin()
{
	if (0 == m_param.check_level)
	{
		this->OnRoleLevelUp(m_role->GetLevel());
		m_param.check_level = 1;
	}
}

void RedEquipCollect::OnEnterScene(int scene_id)
{
}

void RedEquipCollect::OnRoleLevelUp(int to_level)
{
	bool is_change = false;

	// 检查一下激活标记
	for (int seq = 0; seq < RED_EQUIP_COLLECT_SEQ_MAX_COUNT; ++seq)
	{
		if (IS_BIT_SET(m_param.seq_active_flag, seq)) continue;

		auto cfg = LOGIC_CONFIG->GetShenBingTuLuConfig().GetRedEquipCollectOtherCfg(seq, m_role->GetBaseProf());
		if (!cfg) continue;

		// 激活
		if (to_level >= cfg->level)
		{
			SET_BIT(m_param.seq_active_flag, seq);
			is_change = true;
		}
	}

	if (is_change)
	{
		this->SendOtherInfo();

		gamelog::g_log_other.printf(LL_INFO, "RedEquipCollect::OnRoleLevelUp, role_id: %d, role_name: %s, role_level: %d seq_active_flag: %u",
			m_role->GetUID(), m_role->GetName(), to_level, m_param.seq_active_flag);
	}
}

bool RedEquipCollect::IsSuitActive(int seq)
{
	if (!IsValidRedEquipCollectSeq(seq))
	{
		return false;
	}

	const ShenBingTuLuOtherConfig &other_cfg = LOGIC_CONFIG->GetShenBingTuLuConfig().GetOtherCfg();

	int put_on_count = 0;
	const int beg_slot = seq * RED_EQUIP_COLLECT_SLOT_COUNT;

	for (int i = beg_slot; i < beg_slot + RED_EQUIP_COLLECT_SLOT_COUNT; ++i)
	{
		if (!m_param.equip_slot[i].Invalid())
		{
			++put_on_count;
		}
	}

	if (put_on_count >= other_cfg.red_equip_collect_active_puton_count)
	{
		return true;
	}

	return false;
}

ItemDataWrapper * RedEquipCollect::GetItem(int seq, int slot)
{
	if (!IsValidRedEquipCollectSeq(seq) || !IsValidRedEquipCollectSlot(slot))
	{
		return nullptr;
	}

	return &m_param.equip_slot[seq * RED_EQUIP_COLLECT_SLOT_COUNT + slot];
}

void RedEquipCollect::RefreshCollectInfo()
{
	memset(m_collect_info_list, 0, sizeof(m_collect_info_list));

	bool need_send = false;

	const ShenBingTuLuOtherConfig &other_cfg = LOGIC_CONFIG->GetShenBingTuLuConfig().GetOtherCfg();

	for (int seq = 0; seq < RED_EQUIP_COLLECT_SEQ_MAX_COUNT; ++seq)
	{
		auto &collect_info = m_collect_info_list[seq];

		for (int slot = 0; slot < RED_EQUIP_COLLECT_SLOT_COUNT; ++slot)
		{
			const auto &item = m_param.equip_slot[seq * RED_EQUIP_COLLECT_SLOT_COUNT + slot];
			if (!item.Invalid())
			{
				++collect_info.item_count;

				int stars = m_role->GetEquipmentManager()->GetXianPinHightQualityCount(item.param_data.xianpin_type_list, true);
				collect_info.stars += stars;
			}
		}

		// 首次集齐套装，发传闻
		if (!IS_BIT_SET(m_param.collect_succ_msg_send_flag, seq))
		{
			if (collect_info.item_count >= other_cfg.red_equip_collect_active_puton_count)
			{
				int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_red_equip_collect_notice_content,
					m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), seq);

				if (sendlen > 0)
				{
					//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);

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

void RedEquipCollect::CheckGiveActReward()
{
	// 在开服活动结束前创建的角色，才可以补发奖励
	if ((UInt32)m_role->GetCreateRoleTimestamp() >= ActivityShadow::Instance().GetSpecialActivityEndTime(SPECIAL_ACTIVITY_TYPE_OPENGAME))
	{
		return;
	}

	int collect_count = this->GetCollectCount();
	for (int seq = 0; seq < RED_EQUIP_COLLECT_ACT_REWARD_MAX_COUNT; ++seq)
	{
		if ((m_param.opengame_act_reward_fetch_flag & (1 << seq)) != 0)
		{
			continue;
		}

		// 如果不在活动时间内了，也设置已领取，活动结束后就无法领取了
		if (!ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_OPENGAME))
		{
			m_param.opengame_act_reward_fetch_flag |= 1 << seq;
		}

		auto reward_cfg = LOGIC_CONFIG->GetShenBingTuLuConfig().GetRedEquipCollectActCfg(seq);
		if (!reward_cfg || collect_count < reward_cfg->collect_count)
		{
			continue;
		}

		// 邮件发送奖励
		static MailContentParam mail_param;  mail_param.Reset();

		mail_param.AddItem(reward_cfg->reward_item);

		int length = SNPRINTF(mail_param.contenttxt, sizeof(mail_param.contenttxt), gamestring::g_red_equip_collect_act_reward_mail_content);
		if (length > 0)
		{
			MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, mail_param);
		}
		
		SET_BIT(m_param.opengame_act_reward_fetch_flag, seq);
	}
}

void RedEquipCollect::SendAllInfo()
{
	for (int seq = 0; seq < RED_EQUIP_COLLECT_SEQ_MAX_COUNT; ++seq)
	{
		this->SendSeqInfo(seq);
	}

	this->SendOtherInfo();
}

int RedEquipCollect::GetCollectCount() const
{
	int cnt = 0;
	for (int i = 0; i < RED_EQUIP_COLLECT_SEQ_MAX_COUNT; ++i)
	{
		if (IS_BIT_SET(m_param.collect_succ_msg_send_flag, i))
		{
			++cnt;
		}
	}
	return cnt;
}

void RedEquipCollect::SendSeqInfo(int seq)
{
	if (!IsValidRedEquipCollectSeq(seq))
	{
		return;
	}

	static Protocol::SCRedEquipCollect msg;
	msg.seq = seq;

	int beg_index = seq * RED_EQUIP_COLLECT_SLOT_COUNT;
	int count = 0;
	for (int i = beg_index; i < beg_index + RED_EQUIP_COLLECT_SLOT_COUNT; ++i)
	{
		msg.equip_slot[count++] = m_param.equip_slot[i];
	}

	SEND_TO_ROLE(m_role, msg);
}

void RedEquipCollect::SendOtherInfo()
{
	static Protocol::SCRedEquipCollectOther msg;
	msg.seq_active_flag = m_param.seq_active_flag;
	msg.act_reward_can_fetch_flag = 0;
	msg.collect_count = this->GetCollectCount();
	msg.active_reward_fetch_flag = m_param.active_reward_fetch_flag;
	for (int seq = 0; seq < RED_EQUIP_COLLECT_ACT_REWARD_MAX_COUNT; ++seq)
	{
		if (IS_BIT_SET(m_param.opengame_act_reward_fetch_flag, seq))
		{
			continue;
		}

		auto reward_cfg = LOGIC_CONFIG->GetShenBingTuLuConfig().GetRedEquipCollectActCfg(seq);
		if (reward_cfg && msg.collect_count >= reward_cfg->collect_count)
		{
			SET_BIT(msg.act_reward_can_fetch_flag, seq);
		}
	}

	ARRAYCPY(msg.collect_info_list, m_collect_info_list);

	SEND_TO_ROLE(m_role, msg);
}

int RedEquipCollect::GetRedCollectStarAttrPer(int title_id)
{
	for (int seq = 0; seq < RED_EQUIP_COLLECT_SEQ_MAX_COUNT; ++seq)
	{
		const auto &collect_info = m_collect_info_list[seq];

		auto other_cfg = LOGIC_CONFIG->GetShenBingTuLuConfig().GetRedEquipCollectOtherCfg(seq, m_role->GetBaseProf());
		if (other_cfg && title_id == other_cfg->reward_title_id)
		{
			return collect_info.stars * other_cfg->star_add_attr_percent;
		}
	}

	return 0;
}