#include "activedegree.hpp"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "config/activedegreeconfig.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "protocal/msgactivedegree.hpp"
#include "config/logicconfigmanager.hpp"
#include "world.h"
#include "other/event/eventhandler.hpp"
#include "global/guild/guild.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/wabao/wabao.hpp"
#include "other/zhibao/zhibaomanager.hpp"
#include "other/magicalprecious/magicalprecious.hpp"
#include "other/magicalprecious/magicalpreciouscfg.hpp"
#include "crossserver/config/sharedconfig.h"
#include "other/itemlevelreward/itemlevelreward.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"

ActiveDegree::ActiveDegree() : m_role(NULL), m_last_online_time(0), m_day_online_time(0), m_total_degree(0), m_activedegree_fetch_flag(0)
{
	memset(m_activedegree_list, 0, sizeof(m_activedegree_list));
	memset(m_reward_on_day_flag_list, 0, sizeof(m_reward_on_day_flag_list));
}

ActiveDegree::~ActiveDegree()
{

}

void ActiveDegree::Init(Role *role, const ActiveDegreeParam &param)
{
	m_role = role;

	m_last_online_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	m_day_online_time = param.day_online_time;
	m_total_degree = param.total_degree;
	m_role->GetRoleActivity()->SetRADayActivityDegree(m_total_degree);
	memcpy(m_activedegree_list, param.activedegree_list, sizeof(m_activedegree_list));
	memcpy(m_reward_flag_list, param.reward_flag_list, sizeof(m_reward_flag_list));
	m_activedegree_fetch_flag = param.activedegree_fetch_flag;
	memcpy(m_reward_on_day_flag_list, param.reward_on_day_flag_list, sizeof(m_reward_on_day_flag_list));
}

void ActiveDegree::GetInitParam(ActiveDegreeParam *param)
{
	param->day_online_time = m_day_online_time;
	param->total_degree = m_total_degree;
	memcpy(param->activedegree_list, m_activedegree_list, sizeof(param->activedegree_list));
	memcpy(param->reward_flag_list, m_reward_flag_list, sizeof(param->reward_flag_list));
	param->activedegree_fetch_flag = m_activedegree_fetch_flag;
	memcpy(param->reward_on_day_flag_list, m_reward_on_day_flag_list, sizeof(param->reward_on_day_flag_list));
}

void ActiveDegree::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid) 
	{
		m_day_online_time = 0;
		m_activedegree_fetch_flag = 0;

		m_total_degree = 0;
		m_role->GetRoleActivity()->SetRADayActivityDegree(m_total_degree);

		for (int i = 0; i < ACTIVEDEGREE_TYPE_MAX; ++ i)
		{
			m_activedegree_list[i] = 0;
		}

		memset(m_reward_flag_list, 0, sizeof(m_reward_flag_list));
		memset(m_reward_on_day_flag_list, 0, sizeof(m_reward_on_day_flag_list));

		this->SendActiveDegreeInfo();
	}
}

void ActiveDegree::Update(unsigned int now_second)
{
	if (now_second > m_last_online_time + 3)
	{
		m_day_online_time += (now_second - m_last_online_time);
		m_last_online_time = now_second;
		
		// 计算在线时长
		{
			UNSTD_STATIC_CHECK(ACTIVEDEGREE_TYPE_ONLIME_TIME < ACTIVEDEGREE_TYPE_MAX)
			int now_online_hour = m_day_online_time / 3600;
			int record_hour = m_activedegree_list[ACTIVEDEGREE_TYPE_ONLIME_TIME];
		
			if (now_online_hour > record_hour)
			{
				this->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_ONLIME_TIME, now_online_hour - record_hour);
			}
			else if (now_online_hour < record_hour)
			{
				m_activedegree_list[ACTIVEDEGREE_TYPE_ONLIME_TIME] = now_online_hour;
				this->SendActiveDegreeInfo();
			}
		}
	}
}

void ActiveDegree::SendActiveDegreeInfo()
{
	static Protocol::SCActiveDegreeInfo cmd;
	cmd.total_degree = m_total_degree;
	cmd.activedegree_fetch_flag = m_activedegree_fetch_flag;
	memcpy(cmd.reward_flag_list, m_reward_flag_list, sizeof(cmd.reward_flag_list));
	memcpy(cmd.degree_list, m_activedegree_list, sizeof(cmd.degree_list));
	memcpy(cmd.reward_on_day_flag_list, m_reward_on_day_flag_list, sizeof(cmd.reward_on_day_flag_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cmd, sizeof(cmd));
}

void ActiveDegree::OnFetchReward(int reward_index)
{
	if (reward_index < 0 || reward_index >= ACTIVEDEGREE_REWARD_ITEM_MAX_NUM)
	{
		return;
	}

	if (0 != m_reward_flag_list[reward_index]) //领取过
	{
		return;
	}
	
	const ActiveDegreeReward *reward_cfg = LOGIC_CONFIG->GetActiveDegreeConfig().GetActiveDegreeReward(reward_index);
	if (NULL == reward_cfg)
	{
		return;
	}

	if (m_total_degree < reward_cfg->degree_limit)
	{
		m_role->NoticeNum(errornum::EN_ACTIVEDEGREE_NOT_ENOUGH);
		return;
	}

	m_reward_flag_list[reward_index] = 1;

	{
		// 传闻
		if (0 != reward_cfg->is_notice)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_active_degree,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), reward_cfg->degree_limit);
			if (length > 0) 
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
			}
		}
	}

	m_role->GetKnapsack()->PutOrMail(reward_cfg->item, PUT_REASON_ACTIVE_DEGREE);
	
	this->SendActiveDegreeInfo();
}

void ActiveDegree::OnFetchRewardOnDay(int reward_index)
{
	if (reward_index < 0 || reward_index >= ACTIVEDEGREE_REWARD_ITEM_MAX_NUM)
	{
		return;
	}

	if (0 != m_reward_on_day_flag_list[reward_index]) //领取过
	{
		return;
	}

	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	const ActiveDegreeReward *reward_cfg = LOGIC_CONFIG->GetActiveDegreeConfig().GetActiveDegreeRewardOnDay(opengame_day, reward_index);
	if (nullptr == reward_cfg)
	{
		return;
	}

	if (m_total_degree < reward_cfg->degree_limit)
	{
		m_role->NoticeNum(errornum::EN_ACTIVEDEGREE_NOT_ENOUGH);
		return;
	}

	m_reward_on_day_flag_list[reward_index] = 1;

	{
		// 传闻
		if (0 != reward_cfg->is_notice)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_active_degree,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), reward_cfg->degree_limit);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
			}
		}
	}

	m_role->GetKnapsack()->PutOrMail(reward_cfg->item, PUT_REASON_ACTIVE_DEGREE);

	this->SendActiveDegreeInfo();
}

void ActiveDegree::OnUserLogin()
{
	//do nothing
}

void ActiveDegree::AddActiveDegreeHelper(int activedegree_type, int count)
{
	if (activedegree_type < 0 || activedegree_type >= ACTIVEDEGREE_TYPE_NUM || count <= 0) return;

	const ActiveDegreeInfo *cfg_item = LOGIC_CONFIG->GetActiveDegreeConfig().GetActiveDegreeInfo(activedegree_type);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_activedegree_list[activedegree_type] >= cfg_item->max_times)  //满了
	{
		return;
	}

	int remain_times = cfg_item->max_times - m_activedegree_list[activedegree_type];
	int real_add_times = count > remain_times ? remain_times : count;
	m_activedegree_list[activedegree_type] += real_add_times;
	
	this->SendActiveDegreeInfo();
}

void ActiveDegree::OnFetchActiveReward(int activedegree_type)
{
	if (activedegree_type < 0 || activedegree_type >= ACTIVEDEGREE_TYPE_NUM) return;

	const ActiveDegreeInfo *cfg_item = LOGIC_CONFIG->GetActiveDegreeConfig().GetActiveDegreeInfo(activedegree_type);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_activedegree_list[activedegree_type] < cfg_item->max_times)  
	{
		m_role->NoticeNum(errornum::EN_ACTIVEDEGREE_TARGET_LIMIT);
		return;
	}

	if (0 != (m_activedegree_fetch_flag & (LL_ONE_BIT << activedegree_type)))
	{
		m_role->NoticeNum(errornum::EN_ACTIVEDEGREE_HAVE_FETCH_LIMIT);
		return;
	}

	m_activedegree_fetch_flag |= (LL_ONE_BIT << activedegree_type);

	m_total_degree += cfg_item->add_degree;
	m_role->GetRoleActivity()->SetRADayActivityDegree(m_total_degree);
	m_role->GetZhiBaoManager()->AddZhiBaoExp(cfg_item->add_degree);
	m_role->GetRoleActivity()->AddOpenServerInvestFinishParam(RandActivityOpenServerInvestOtherCfg::INVEST_TYPE_ACTIVE, m_total_degree);

	// 活跃获得经验乘以等级经验系数
	//const ActiveDegreeExpLevelRatio * ratio_item = LOGIC_CONFIG->GetActiveDegreeConfig().getActiveDegreeExpLevelRatio(m_role->GetLevel());
	//if (NULL != ratio_item)
	//{
	//	m_role->AddExp(static_cast<long long>(cfg_item->add_exp * ratio_item->exp_ratio), "AddActiveDegreeHelper");
	//}

	//策划需求，修改增加经验规则（2018-1-3）
	long long add_exp = static_cast<long long>(cfg_item->exp_factor_type * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(m_role->GetLevel(), 0)));
	m_role->AddExp(add_exp, "AddActiveDegreeHelper", Role::EXP_ADD_REASON_DEFAULT);

	this->SendActiveDegreeInfo();
	m_role->GetWabao()->OnFetchBaotuReward(m_total_degree);

	EventHandler::Instance().OnActiveDegreeComplete(m_role, activedegree_type);
	m_role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_ACTIVE_DEGREE, cfg_item->add_degree);
}

bool ActiveDegree::CanAddActiveDegree(int activedegree_type)
{
	if (activedegree_type < 0 || activedegree_type >= ACTIVEDEGREE_TYPE_NUM) return false;

	const ActiveDegreeInfo *cfg_item = LOGIC_CONFIG->GetActiveDegreeConfig().GetActiveDegreeInfo(activedegree_type);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (m_activedegree_list[activedegree_type] >= cfg_item->max_times)  //满了
	{
		return false;
	}
	return true;
}

void ActiveDegree::AddSetOnlineTime(int minute)
{
	m_day_online_time += minute * 60;
	if (m_day_online_time < 0 ) m_day_online_time = 0;
}

void ActiveDegree::GMAddActiveDegree()
{
	for (int i = 0; i < ACTIVEDEGREE_TYPE_NUM; ++i)
	{
		const ActiveDegreeInfo *active_degree_cfg = LOGIC_CONFIG->GetActiveDegreeConfig().GetActiveDegreeInfo(i);
		if (nullptr == active_degree_cfg)
		{
			continue;
		}

		m_activedegree_list[i] = active_degree_cfg->max_times;
	}

	this->SendActiveDegreeInfo();
}