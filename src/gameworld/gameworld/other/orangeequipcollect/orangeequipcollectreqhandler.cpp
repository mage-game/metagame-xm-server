#include "protocal/msgrole.h"
#include "orangeequipcollectreqhandler.hpp"
#include "gamecommon.h"
#include "other/orangeequipcollect/orangeequipcollect.hpp"
#include "servercommon/struct/orangeequipcollectdef.hpp"
#include "globalconfig/otherconfig.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "servercommon/errornum.h"
#include "config/logicconfigmanager.hpp"
#include "servercommon/noticenum.h"
#include "gameworld/item/knapsack.h"
#include "other/achieve/title/titlemanager.hpp"
#include "servercommon/string/gameworldstr.h"
#include "gameworld/world.h"
#include "config/shenbingtuluconfig.h"

// ��װ�ռ�-����
namespace reqhandler_orange_equip_collect
{
	bool TakeOn::ParamCheck(void *cs)
	{
		Protocol::CSReqCommonOpreate *req = (Protocol::CSReqCommonOpreate*)cs;
		m_seq = req->param1;
		if (!IsValidOrangeEquipCollectSeq(m_seq))
		{
			return false;
		}

		m_slot = req->param2;
		if (!IsValidOrangeEquipCollectSlot(req->param2))
		{
			return false;
		}

		m_item_wrap = m_role->GetOrangeEquipCollect()->GetItem(req->param1, req->param2);
		if (!m_item_wrap)
		{
			return false;
		}

		m_knapsack_index = req->param3;
		if (!m_role->GetKnapsack()->GetItemGridData(m_knapsack_index, &m_item_data))
		{
			return false;
		}

		return true;
	}

	bool TakeOn::PreHandleCheck()
	{
		// ���뼤��ſɴ���
		auto active_flag = m_role->GetOrangeEquipCollect()->m_param.seq_active_flag;
		if (!IS_BIT_SET(active_flag, m_seq))
		{
			m_role->NoticeNum(errornum::EN_RED_EQUIP_COLLECT_TAKEON_LIMIT);
			return false;
		}

		const ItemBase *item_cfg = ITEMPOOL->GetItem(m_item_data.item_id);
		if (!item_cfg || item_cfg->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)
		{
			return false;
		}

		const Equipment *equip_cfg = (const Equipment*)item_cfg;
		if (equip_cfg->GetEquipType() < Equipment::E_TYPE_ZHUANZHI_MIN || equip_cfg->GetEquipType() >= Equipment::E_TYPE_ZHUANZHI_MAX)
		{
			return false;
		}

		// ÿ����ֻ��װ���̶���һ����Ʒ
		auto cfg = LOGIC_CONFIG->GetShenBingTuLuConfig().GetOrangeEquipCollectItemCfg(m_seq, m_role->GetBaseProf());
		if (!cfg || !cfg->equip_items.IsValidIndex(m_slot) || cfg->equip_items[m_slot] != m_item_data.item_id)
		{
			return false;
		}
		return true;
	}

	bool TakeOn::DoHandle()
	{
		// ����۷ǿգ�������
		if (m_item_wrap == nullptr) return false;

		auto old_item_wrap = *m_item_wrap;
		if (!m_item_wrap->Invalid())
		{
			if (!m_role->GetKnapsack()->HaveSpace())
			{
				m_role->NoticeNum(errornum::EN_HAS_NO_SPACING);
				return false;
			}

			if (!m_role->GetKnapsack()->Put(old_item_wrap, PUT_REASON_RED_EQUIP_COLLECT))
			{
				return false;
			}

			m_item_wrap->Reset();
		}

		// ����
		ItemDataWrapper item_wrap_tmp;
		if (!m_item_data.WrapTo(&item_wrap_tmp))
		{
			return false;
		}

		// ����
		if (!m_role->GetKnapsack()->ConsumeItemByIndex(m_knapsack_index, 1, "RedEquipCollectTakeOn"))
		{
			return false; // impossible
		}

		*m_item_wrap = item_wrap_tmp;

		m_role->GetOrangeEquipCollect()->RefreshCollectInfo();
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ORANGE_EQUIP_COLLECT);

		m_role->GetOrangeEquipCollect()->SendSeqInfo(m_seq);
		m_role->GetOrangeEquipCollect()->SendOtherInfo();

		m_role->NoticeNum(noticenum::NT_ORANGE_EQUIP_COLLECT_SUCC);

		gamelog::g_log_other.printf(LL_INFO, "OrangeEquipCollectTakeOn, role_id: %d, role_name: %s, seq: %d, slot: %d, item_id: %d -> %d",
			m_role->GetUID(), m_role->GetName(), m_seq, m_slot, old_item_wrap.item_id, m_item_wrap->item_id);

		return true;
	}
}

// ��װ�ռ�-��ȡ���������
namespace reqhandler_orange_equip_collect
{
	bool FetchActReward::ParamCheck(void *cs)
	{
		Protocol::CSReqCommonOpreate *req = (Protocol::CSReqCommonOpreate*)cs;
		m_seq = req->param1;
		if (!IsValidRedEquipActRewardSeq(m_seq))
		{
			return false;
		}
		return true;
	}

	bool FetchActReward::PreHandleCheck()
	{
		if (!ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_OPENGAME))
		{
			m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_NOT_OPEN);
			return false;
		}

		if (IS_BIT_SET(m_role->GetOrangeEquipCollect()->m_param.opengame_act_reward_fetch_flag, m_seq))
		{
			m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_FETCHED);
			return false;
		}

		m_reward_cfg = LOGIC_CONFIG->GetShenBingTuLuConfig().GetOrangeEquipCollectActCfg(m_seq);
		if (!m_reward_cfg)
		{
			return false;
		}

		if (m_role->GetOrangeEquipCollect()->GetCollectCount() < m_reward_cfg->collect_count)
		{
			m_role->NoticeNum(errornum::EN_ILLEGAL_OPERA);
			return false;
		}

		return true;
	}

	bool FetchActReward::DoHandle()
	{
		// ����
		SET_BIT(m_role->GetOrangeEquipCollect()->m_param.opengame_act_reward_fetch_flag, m_seq);

		m_role->GetKnapsack()->PutOrMail(m_reward_cfg->reward_item, PUT_REASON_OPENGAME_ACTIVITY);

		m_role->GetOrangeEquipCollect()->SendOtherInfo();

		return false;
	}
}
