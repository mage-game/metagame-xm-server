#include "jingjie.hpp"
#include "obj/character/role.h"
#include "other/jingjie/jingjieconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "other/event/eventhandler.hpp"
#include "scene/scenemanager.h"
#include "servercommon/commondata.hpp"
#include "protocal/msgrole.h"
#include "item/knapsack.h"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "protocal/msgscene.h"
#include "other/magicalprecious/magicalpreciouscfg.hpp"
#include "other/magicalprecious/magicalprecious.hpp"

JingJie::JingJie() : m_role(NULL), m_last_yazhi_skill_perform_timestamp(0)
{
}

JingJie::~JingJie()
{
}

void JingJie::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attr_temp.Reset();

		const int jingjie_level = m_role->GetCommonDataParam()->jingjie_level;

		const JingJieLevelCfg *level_cfg = LOGIC_CONFIG->GetJingJieConfig().GetJingJieCfgByLevel(jingjie_level);
		if (NULL != level_cfg)
		{
			m_attr_temp.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += level_cfg->gongji;
			m_attr_temp.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += level_cfg->maxhp;
			m_attr_temp.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += level_cfg->fangyu;
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_JINGJIE, m_attr_temp);
	}

	base_add.Add(m_attr_temp.m_attrs, true);
}

void JingJie::OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure, int fight_type, int target_index)
{
	// 如果头衔超过主要敌人目标，有几率触发眩晕技能
	if (nullptr != target && 0 == target_index)
	{
		if (Obj::OBJ_TYPE_ROLE == target->GetObjType())
		{
			Role *target_role = static_cast<Role *>(target);

			const int jingjie_level = m_role->GetCommonDataParam()->jingjie_level;
			const int target_jingjie_level = target_role->GetCommonDataParam()->jingjie_level;

			if (jingjie_level > target_jingjie_level)
			{
				const JingjieOtherCfg &other_cfg = LOGIC_CONFIG->GetJingJieConfig().GetOtherCfg();

				unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				int trigger_rata = RandomNum(10000);

				if (now_sec >= m_last_yazhi_skill_perform_timestamp + other_cfg.yazhi_xuanyun_cds && trigger_rata < other_cfg.yazhi_xuanyun_trigger_rate)
				{
					EffectSpecialState *specail_state = new EffectSpecialState(0, 0, other_cfg.yazhi_xuanyun_durations, EBT_XUANYUN, 1);
					specail_state->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
					specail_state->SetClientEffectType(EffectBase::ECT_SKILL_XUANYUN);
					specail_state->SetClearOnDie(true);

					target->AddEffect(specail_state);

					m_last_yazhi_skill_perform_timestamp = now_sec;
				}
			}
		}
	}
}

void JingJie::GetRoleAppearance(RoleAppearance* data)
{
	return;
}

const int JingJie::GetIncreaHurtPer(Role *target_role)
{
	if (m_role->GetCommonDataParam()->jingjie_level > target_role->GetCommonDataParam()->jingjie_level)
	{
		const JingjieOtherCfg &other_cfg = LOGIC_CONFIG->GetJingJieConfig().GetOtherCfg();

		return other_cfg.yazhi_add_hurt_per;
	}

	return 0;
}

void JingJie::OnJingJieOpera(int opera_type, int param)
{
	if (opera_type < 0 || opera_type >= Protocol::CS_ROLE_JINGJIE_OPERA_MAX)
	{
		return;
	}

	switch (opera_type)
	{
	case Protocol::CS_ROLE_JINGJIE_OPERA_PROMOTE_LEVEL:
	{
		this->PromoteJingJie(param);
	}
	break;

	case Protocol::CS_ROLE_JINGJIE_OPERA_GET_INFO:
	{
		this->SendJingJieInfo();
	}
	break;

	default:
		break;
	}
}

void JingJie::OnLogin()
{
	SendJingJieInfo();
}

void JingJie::PromoteJingJie(int is_auto_buy)
{
	int cur_jingjie_level = m_role->GetJingJieLevel();

	if (cur_jingjie_level < 0)
	{
		cur_jingjie_level = 0;
	}

	if (cur_jingjie_level >= MAX_JINGJIE_LEVEL)
	{
		m_role->NoticeNum(errornum::EN_JINGJIE_LEVEL_MAX);
		return;
	}

	const JingJieLevelCfg *level_cfg = LOGIC_CONFIG->GetJingJieConfig().GetJingJieCfgByLevel(cur_jingjie_level + 1);
	if (NULL == level_cfg)
	{
		return;
	}

	if (m_role->GetCapability()->GetTotalCapability() < level_cfg->cap_limit)
	{
		m_role->NoticeNum(errornum::EN_JINGJIE_CAP_LIMIT);
		return;
	}

	if (level_cfg->stuff_num > 0)
	{
		static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
		{
			// 检查
			ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
			stuff_list[0].item_id = level_cfg->stuff_id;
			stuff_list[0].num = level_cfg->stuff_num;
			stuff_list[0].buyable = (1 == is_auto_buy);

			if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, true))
			{
				return;
			}

			// 消耗
			if (!ItemExtern::ConsumeItem(m_role, consume_list, "JingJie::PromoteJingJie"))
			{
				return;
			}
		}
	}

	m_role->SetJingJieLevel(cur_jingjie_level + 1);

	//{
	//	static Protocol::SCNotifyAppearanceChangeInfo naci;

	//	naci.reason_type = Protocol::SCNotifyAppearanceChangeInfo::NOTIFY_REASON_TYPE_JINGJIE_LEVEL_CHANGE;
	//	naci.obj_id = m_role->GetId();
	//	naci.param1 = m_role->GetCommonDataParam()->jingjie_level;

	//	m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void *)&naci, sizeof(naci));
	//}

	m_role->RefreshApperance(true);
	this->SendJingJieInfo();
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGJIE);

	EventHandler::Instance().OnJingjieUplevel(m_role);
}

void JingJie::SendJingJieInfo()
{
	Protocol::SCRoleJingJie rjj;

	rjj.jingjie_level = m_role->GetCommonDataParam()->jingjie_level;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&rjj, sizeof(rjj));
}

