#include "molong.h"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "item/itempool.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "protocal/msgother.h"
#include "gameworld/engineadapter.h"
#include "molongconfig.h"
#include "other/capability/capability.hpp"

Molong::Molong() : m_role(NULL), m_curr_loop(0), m_accumulate_consume_gold(0), m_today_consume_gold(0), m_total_move_step(0), m_today_move_step(0)
{

}

Molong::~Molong()
{

}

void Molong::Init(int curr_loop, int accumulate_consume_gold, int today_consume_gold, short total_move_step, short today_move_step,int rank_grade,int rank_cumulate_gold)
{
	m_curr_loop = curr_loop;
	m_accumulate_consume_gold = accumulate_consume_gold;
	m_today_consume_gold = today_consume_gold;
	m_total_move_step = total_move_step;
	m_today_move_step = today_move_step;
	m_rank_grade = rank_grade;
	m_rank_cumulate_gold = rank_cumulate_gold;
}

void Molong::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs.Reset();
	}
	const MoLongRankConfig * leve_rank = LOGIC_CONFIG->GetMolongCfg().GetRankGradeParamCfg(m_rank_grade);
	if (leve_rank == NULL) return;
	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_MOLONG, leve_rank->war_value);
	base_add.Add(m_attrs.m_attrs, true);
}

void Molong::GetInitParam(int *curr_loop, int *accumulate_consume_gold, int *today_consume_gold, short *total_move_step, short *today_move_step,int *rank_grade, int *rank_cumulate_gold)
{
	*curr_loop = m_curr_loop;
	*accumulate_consume_gold = m_accumulate_consume_gold;
	*today_consume_gold = m_today_consume_gold;
	*total_move_step = m_total_move_step;
	*today_move_step = m_today_move_step;
	*rank_grade = m_rank_grade;
	*rank_cumulate_gold = m_rank_cumulate_gold;
}

void Molong::OnDayChange(int old_day, int new_day)
{
	m_today_move_step = 0;
	m_today_consume_gold = 0;
	this->PrintInfo("OnDayChange");
	this->SendInfo();
}

void Molong::OnConsumeGold(int gold)
{
	if (gold < 0) return;
	m_rank_cumulate_gold += gold;
	int max_loop = LOGIC_CONFIG->GetMolongCfg().GetMaxLoop();
	int day_max_move_step = LOGIC_CONFIG->GetMolongCfg().GetDayMaxMoveStep();
	if (m_curr_loop > max_loop || m_today_move_step >= day_max_move_step)
	{
		//满了就不再累计,智扬的需求2019 03 11
		//m_accumulate_consume_gold += gold;
		//this->SendInfo();
		return;
	}

	int day_effect_accumulate_gold = m_today_consume_gold;
	m_today_consume_gold += gold;
	
	for (int i = m_today_move_step + 1; i <= day_max_move_step; ++ i)
	{
		if (m_curr_loop >  max_loop)
		{
			break;
		}

		const MolongMoveConfig *last_move_cfg = LOGIC_CONFIG->GetMolongCfg().GetMoveCfg(i - 1);
		const MolongMoveConfig *move_cfg = LOGIC_CONFIG->GetMolongCfg().GetMoveCfg(i);
		if (NULL == move_cfg || m_today_consume_gold < move_cfg->consume_gold) 
		{
			break;
		}

		++ m_today_move_step;
		++ m_total_move_step;

		int detal_gold = move_cfg->consume_gold - day_effect_accumulate_gold;
		
		//m_accumulate_consume_gold += detal_gold;				//不给差值,成功走一步就给配置里的金额2019 03 11
		if (nullptr != last_move_cfg)
		{
			m_accumulate_consume_gold = m_accumulate_consume_gold + move_cfg->consume_gold - last_move_cfg->consume_gold;
		}
		else
		{
			m_accumulate_consume_gold += move_cfg->consume_gold;
		}
		day_effect_accumulate_gold += detal_gold;

		// 给予奖励
		const MolongRewardConfig *reward_cfg = LOGIC_CONFIG->GetMolongCfg().GetRewardCfg(m_curr_loop, m_total_move_step);
		if (NULL != reward_cfg)
		{
			const ItemBase *item_base = ITEMPOOL->GetItem(reward_cfg->reward_item.item_id);
			if (reward_cfg->fanli_rate > 0 || NULL != item_base)
			{
				if (NULL != item_base)
				{
					static MailContentParam contentparam; contentparam.Reset();
					contentparam.item_list[0].item_id = reward_cfg->reward_item.item_id;
					contentparam.item_list[0].num = reward_cfg->reward_item.num;
					contentparam.item_list[0].is_bind = reward_cfg->reward_item.is_bind;
					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_molong_reward_content_item);
					if (length > 0)
					{
						if (m_role->IsInCross())
						{
							MailRoute::MailToCrossUser(m_role->GetUniqueUserID(), SYSTEM_MAIL_REASON_INVALID, contentparam);
						}
						else
						{
							MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
						}

						this->PrintInfo("GiveReward_item", reward_cfg->reward_item.item_id, reward_cfg->reward_item.num);
					}
				}

				int old_accumulate_consume_gold = m_accumulate_consume_gold;
				if (reward_cfg->fanli_rate > 0)
				{
					static MailContentParam contentparam; contentparam.Reset();
					contentparam.AddVirtualItem(0, reward_cfg->virtual_type, static_cast<long long>(m_accumulate_consume_gold * 0.01 * reward_cfg->fanli_rate));
		
					m_accumulate_consume_gold = 0;

					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_molong_reward_content, old_accumulate_consume_gold, reward_cfg->fanli_rate);
					if (length > 0)
					{
						if (m_role->IsInCross())
						{
							MailRoute::MailToCrossUser(m_role->GetUniqueUserID(), SYSTEM_MAIL_REASON_INVALID, contentparam);
						}
						else
						{
							MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
						}

						this->PrintInfo("GiveReward_virtual_item", old_accumulate_consume_gold, contentparam.virtual_item_list.virtual_item[0]);
					}
				}
			}
		}
		else
		{
			// 理论不可能走到这里
			this->PrintInfo("GiveRewardError");
		}

		// 修正 m_total_move_step 和 m_curr_loop
		if (MOLONG_MAX_MOVE_STEP == m_total_move_step)
		{
			++ m_curr_loop;
			m_total_move_step = 0;
		}
	}

	//总消耗只有在达到配置数的时候才改变
	/*int day_max_consume_gold = LOGIC_CONFIG->GetMolongCfg().GetDayMaxConsumeGold();
	if (day_effect_accumulate_gold < day_max_consume_gold && day_effect_accumulate_gold < m_today_consume_gold)
	{
		int tmp_gold = m_today_consume_gold;
		if (tmp_gold > day_max_consume_gold)
		{
			tmp_gold = day_max_consume_gold;
		}

		m_accumulate_consume_gold += (tmp_gold - day_effect_accumulate_gold);
	}*/

	this->PrintInfo("ConsumeGold");
	this->SendInfo();
}

int Molong::GetRankValuePercent()
{
	const MoLongRankConfig * leve_rank = LOGIC_CONFIG->GetMolongCfg().GetRankGradeParamCfg(m_rank_grade);
	if (leve_rank == NULL) return 0;
	return leve_rank->value_percent;
}

void Molong::OnRankUpgrade()
{
	if (m_rank_grade == MOLONG_MAX_RANK_STEP)
	{
		return;
	}
	const MoLongRankConfig * leve_rank = LOGIC_CONFIG->GetMolongCfg().GetRankGradeParamCfg(m_rank_grade);
	if (leve_rank == NULL) return;

	if (leve_rank->cumulate_gold > m_rank_cumulate_gold)
	{
		return;
	}
	m_rank_cumulate_gold = m_rank_cumulate_gold - leve_rank->cumulate_gold;
	m_rank_grade = m_rank_grade + 1;
	this->SendInfo();
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOLONG);
}

bool Molong::OnJudgeRankGrade(Role *m_target)
{

	if (GetRankGrade()>m_target->GetMolong()->GetRankGrade())
	{
		return true;
	}

	return false;
}

void Molong::PrintInfo(const char *reason, int param_1, long long param_2)
{
	if (NULL == reason) return;

	gamelog::g_log_other.printf(LL_INFO, "Molong reason %s : user %d %s, curr_lop %d, accumulate_consume_gold %d, today_move_step %d, total_move step %d, param_1 %d, param_2 %lld",
		reason, m_role->GetUID(), m_role->GetName(), m_curr_loop, m_accumulate_consume_gold, m_today_move_step, m_total_move_step, param_1, param_2);
}

void Molong::SendInfo()
{
	Protocol::SCMolongInfo mli;
	mli.curr_loop = m_curr_loop;
	mli.accumulate_consume_gold = m_accumulate_consume_gold;
	mli.today_consume_gold = m_today_consume_gold;
	mli.today_move_step = m_today_move_step;
	mli.total_move_step = m_total_move_step;

	mli.rank_grade = m_rank_grade;
	mli.rank_cumulate_gold = m_rank_cumulate_gold;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&mli, sizeof(mli));
}

