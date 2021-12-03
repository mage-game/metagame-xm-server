#include "jilian.hpp"
#include "jilianconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "obj/character/role.h"
#include "scene/scene.h"
#include "servercommon/errornum.h"
#include "servercommon/serverdef.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "global/team/team.hpp"
#include "world.h"
#include "other/event/eventhandler.hpp"

Jilian::Jilian() : m_role(NULL), m_once_join(0), m_jilian_type(0), m_is_working(0), m_progress(0), m_last_work_timestamp(0),
	m_qte_double_reward(false), m_qte_limit_timestamp(0), m_qte_succ_times(0)
{

}

Jilian::~Jilian()
{

}

void Jilian::Init(Role *role, char jilian_type, char is_working, short progress, char once_join, char qte_succ_times) 
{
	m_role = role;
	m_jilian_type = jilian_type;
	m_is_working = is_working;
	m_progress = progress;
	m_once_join = once_join;
	m_qte_succ_times = qte_succ_times;
}

void Jilian::GetInitParam(char *jilian_type, char *is_working, short *progress, char *once_join, char *qte_succ_times)
{
	*jilian_type = m_jilian_type;
	*is_working = m_is_working;
	*progress = m_progress;
	*once_join = m_once_join;
	*qte_succ_times = m_qte_succ_times;
}

void Jilian::Update(unsigned int now_second)
{
// 	const JilianOtherConfig &other_cfg = LOGIC_CONFIG->GetJilianCfg().GetOtherCfg();
// 
// 	if (0 != m_is_working && m_jilian_type > JILIAN_TYPE_INVALID && m_jilian_type < JILIAN_TYPE_MAX)
// 	{
// 		bool out_of_range = (m_role->GetPos() - other_cfg.lianlu_pos).IsLongerThan(other_cfg.lianlu_range);
// 		if (out_of_range || m_role->GetScene()->GetSceneID() != other_cfg.lianlu_scene_id)
// 		{
// 			m_is_working = 0;
// 
// 			this->SendInfo();
// 			this->BroadcastJilianType();
// 		}
// 		else
// 		{
// 			if (m_progress < other_cfg.jilian_times && now_second >= m_last_work_timestamp + other_cfg.cost_time_s)
// 			{
// 				m_last_work_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
// 				++ m_progress;
// 
// 				int reward_exp = LOGIC_CONFIG->GetJilianCfg().GetRewardExp(m_role->GetLevel(), m_jilian_type);
// 				int working_team_member_num = 0;
// 
// 				{
// 					TeamInfo *team_info = m_role->GetTeamInfo();
// 					if (NULL != team_info)
// 					{
// 						Role *role_list[16] = {NULL};
// 						int team_member_role_num = team_info->GetOtherMemberInTheSameScene(role_list, 16);
// 
// 						for (int i = 0; i < team_member_role_num; ++ i)
// 						{
// 							Role *member_role = role_list[i];
// 							if (NULL != member_role)
// 							{
// 								if (member_role->GetJilian()->IsWorking())
// 								{
// 									++ working_team_member_num;
// 								}
// 							}
// 						}
// 					}
// 				}
// 
// 				int reward_facotr = (m_qte_double_reward ? 2 : 1);
// 
// 				reward_exp = static_cast<int>(reward_exp * 1.0 * reward_facotr * (working_team_member_num * 10 + 100) / 100);
// 
// 				m_qte_double_reward = false;
// 				m_qte_limit_timestamp = 0;
// 
// 				m_role->AddExp(reward_exp, "JilianReward");
// 				this->SendInfo();
// 				this->CheckCanQte();
// 			}
// 		}
// 	}
// 
// 	if (m_progress >= other_cfg.jilian_times)
// 	{
// 		m_jilian_type = JILIAN_TYPE_INVALID;
// 		m_is_working = 0;
// 		m_progress = 0;
// 
// 		this->SendInfo();
// 		this->BroadcastJilianType();
// 
// 		EventHandler::Instance().OnCompleteJiLian(m_role);
// 	}
}

void Jilian::OnRoleLeaveScene()
{
// 	if (0 != m_is_working)
// 	{
// 		m_is_working = 0;
// 
// 		this->SendInfo();
// 		this->BroadcastJilianType();
// 	}
}

void Jilian::Start(int jilian_type)
{
// 	if (jilian_type <= JILIAN_TYPE_INVALID || jilian_type >= JILIAN_TYPE_MAX)
// 	{
// 		return;
// 	}
// 
// 	if (m_jilian_type > JILIAN_TYPE_INVALID && m_jilian_type < JILIAN_TYPE_MAX)
// 	{
// 		m_role->NoticeNum(errornum::EN_JILIAN_FINISH_JILIAN_FIRST);
// 		return;
// 	}
// 
// 	JoinLimitUnit limit_unit;
// 	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_JILIAN, &limit_unit);
// 
// 	if (is_limit && m_role->GetLevel() < limit_unit.level)
// 	{
// 		m_role->NoticeNum(errornum::EN_ROLE_LEVEL_NOT_ENOUGH);
// 		return;
// 	}
// 
// 	const JilianOtherConfig &other_cfg = LOGIC_CONFIG->GetJilianCfg().GetOtherCfg();
// 
// 	bool out_of_range = (m_role->GetPos() - other_cfg.lianlu_pos).IsLongerThan(other_cfg.lianlu_range);
// 	if (out_of_range || m_role->GetScene()->GetSceneID() != other_cfg.lianlu_scene_id)
// 	{
// 		m_role->NoticeNum(errornum::EN_JILIAN_OUT_OF_RANGE);
// 		return;
// 	}
// 
// 	if (!m_role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_JILIAN_TIMES, other_cfg.can_join_times))
// 	{
// 		m_role->NoticeNum(errornum::EN_JILIAN_JOIN_LIMIT_TIMES);
// 		return;
// 	}
// 
// 	int need_gold = 0;
// 	{
// 		if (0 != m_once_join)
// 		{
// 			if (JILIAN_TYPE_COMMON == jilian_type)
// 			{
// 				need_gold = other_cfg.common_jilian_cost;
// 			}
// 			else if (JILIAN_TYPE_SUPER == jilian_type)
// 			{
// 				need_gold = other_cfg.super_jilian_cost;
// 			}
// 
// 			if (need_gold <= 0) // 加强判断
// 			{
// 				return;
// 			}
// 		}
// 	}
// 	
// 	if (!m_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(need_gold))
// 	{
// 		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 		return;
// 	}
// 
// 	m_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_JILIAN_TIMES);
// 
// 	if (need_gold > 0)
// 	{
// 		if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(need_gold, "StartJilian"))
// 		{
// 			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 			return;
// 		}
// 	}
// 
// 	m_once_join = 1;
// 	m_jilian_type = jilian_type;
// 	m_is_working = 1;
// 	m_progress = 0;
// 	m_qte_succ_times = 0;
// 
// 	{
// 		static MailContentParam contentparam; contentparam.Reset();
// 
// 		ItemConfigData &reward_item = LOGIC_CONFIG->GetJilianCfg().GetOtherCfg().reward_item;
// 		contentparam.item_list[0].item_id = reward_item.item_id;
// 		contentparam.item_list[0].num = reward_item.num;
// 		contentparam.item_list[0].is_bind = reward_item.is_bind;
// 
// 		int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_jilian_start_reward_subject);
// 		int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_jilian_start_reward_content);
// 
// 		if (length1 > 0 && length2 > 0)
// 		{
// 			MailRoute::MailToUser(m_role->GetUserId() , SYSTEM_MAIL_REASON_INVALID, contentparam);
// 		}
// 	}
// 
// 	m_last_work_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
// 
// 	this->SendInfo();
// 	this->BroadcastJilianType();
// 	this->CheckCanQte();
// 
// 	{
// 		// 日志
// 		gamelog::g_log_other.printf(LL_INFO, "Jilian::Start user[%d, %s], level[%d], type[%d], use_all_gold[%d]",
// 			m_role->GetUID(), m_role->GetName(),  m_role->GetLevel(), m_jilian_type, need_gold);
// 
// 		ROLE_LOG_QUICK6(LOG_TYPE_JILIAN, m_role, m_role->GetLevel(), m_jilian_type, NULL, NULL);
// 	}
}

void Jilian::Continue()
{
// 	if (m_jilian_type <= JILIAN_TYPE_INVALID || m_jilian_type >= JILIAN_TYPE_MAX)
// 	{
// 		m_role->NoticeNum(errornum::EN_JILIAN_START_JILIAN_FIRST);
// 		return;
// 	}
// 
// 	const JilianOtherConfig &other_cfg = LOGIC_CONFIG->GetJilianCfg().GetOtherCfg();
// 
// 	if (m_progress >= other_cfg.jilian_times)
// 	{
// 		return;
// 	}
// 
// 	bool out_of_range = (m_role->GetPos() - other_cfg.lianlu_pos).IsLongerThan(other_cfg.lianlu_range);
// 	if (out_of_range || m_role->GetScene()->GetSceneID() != other_cfg.lianlu_scene_id)
// 	{
// 		m_role->NoticeNum(errornum::EN_JILIAN_OUT_OF_RANGE);
// 		return;
// 	}
// 
// 	m_is_working = 1;
// 
// 	m_last_work_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
// 
// 	this->SendInfo();
// 	this->BroadcastJilianType();
// 	this->CheckCanQte();
}

void Jilian::CheckCanQte()
{
// 	const JilianOtherConfig &other_cfg = LOGIC_CONFIG->GetJilianCfg().GetOtherCfg();
// 
// 	if (!m_qte_double_reward && 0 == m_qte_limit_timestamp && m_progress < other_cfg.jilian_times)
// 	{
// 		if (m_qte_succ_times < other_cfg.qte_succ_max_times)
// 		{
// 			int rand_rate = RandomNum(100);
// 			if (rand_rate < other_cfg.qte_rate)
// 			{
// 				m_qte_limit_timestamp = static_cast<int>(EngineAdapter::Instance().Time()) + other_cfg.qte_duration_s;
// 			}
// 		}
// 	}
// 
// 	this->SendQteInfo();
}

void Jilian::OnReportQteResult(bool qte_succ)
{
// 	if (qte_succ && 0 != m_qte_limit_timestamp && EngineAdapter::Instance().Time() <= m_qte_limit_timestamp + 1)
// 	{
// 		++ m_qte_succ_times;
// 		m_qte_double_reward = true;
// 	}
// 
// 	m_qte_limit_timestamp = 0;
// 	this->SendQteInfo();
}

void Jilian::SendQteInfo()
{
// 	Protocol::SCQTEInfo qtei;
// 	qtei.qte_type = Protocol::QTE_TYPE_JILIAN;
// 	qtei.is_double_reward = m_qte_double_reward ? 1 : 0;
// 	qtei.qte_limit_timestamp = static_cast<int>(m_qte_limit_timestamp);
// 	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&qtei, sizeof(qtei));
}

void Jilian::SendInfo()
{
// 	Protocol::SCJilianInfo jli;
// 	jli.is_working = m_is_working;
// 	jli.jilian_type = m_jilian_type;
// 	jli.progress = m_progress;
// 	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&jli, sizeof(jli));
}

void Jilian::BroadcastJilianType()
{
// 	Protocol::SCJianlianViewChange jlvc;
// 	jlvc.obj_id = m_role->GetId();
// 	jlvc.jilian_type = JILIAN_TYPE_INVALID;
// 	
// 	if (0 != m_is_working)
// 	{
// 		jlvc.jilian_type = m_jilian_type;
// 	}
// 
// 	m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void *)&jlvc, sizeof(jlvc));
}

