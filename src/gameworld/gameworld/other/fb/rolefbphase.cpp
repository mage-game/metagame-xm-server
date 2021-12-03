#include "rolefbphase.hpp"

#include "servercommon/errornum.h"

#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "monster/monsterpool.h"
#include "other/vip/vipconfig.hpp"
#include "other/fb/phasefbconfig.hpp"

#include "protocal/msgfb.h"
#include "engineadapter.h"
#include "scene/scene.h"

#include "item/knapsack.h"
#include "other/event/eventhandler.hpp"
#include "other/vip/vip.hpp"
#include "other/daycounter/daycounter.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "task/newtaskmanager.h"
#include "npc/transmit/fbmanager.hpp"
#include "task/taskrecorder.h"

RolePhaseFB::RolePhaseFB(): m_role(NULL)
{

}

RolePhaseFB::~RolePhaseFB()
{

}

void RolePhaseFB::Init(Role *role, const PhaseFbParam &phase_fb_param)
{
	m_role = role;
	m_param = phase_fb_param;
}

void RolePhaseFB::GetInitParam(PhaseFbParam *phase_fb_param)
{
	*phase_fb_param = m_param;
}

void RolePhaseFB::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	for (int i = 0; i < FB_PHASE_MAX_COUNT; i++)
	{
		m_param.fb_list[i].today_times = 0;
		m_param.fb_list[i].today_buy_times = 0;
	}

	this->SendInfo();
}

bool RolePhaseFB::CanEnter(int fb_index)
{
	if (fb_index < 0 || fb_index >= FB_PHASE_MAX_COUNT)
	{
		return false;
	}

	if (this->GetPassLevel(fb_index) >= FB_PHASE_MAX_LEVEL)
	{
		m_role->NoticeNum(errornum::EN_PHASE_FB_PASS_ALL_LIMIT);
		return false;
	}

	int next_fb_level = this->GetPassLevel(fb_index) + 1;  // 准备进入的关卡

	const PhaseLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetPhaseFBConfig().GetLevelCfg(fb_index, next_fb_level);
	if (NULL == level_cfg)
	{
		m_role->NoticeNum(errornum::EN_PHASE_FB_PASS_ALL_LIMIT);
		return false;
	}

	if (m_role->GetLevel() < level_cfg->role_level)
	{
		m_role->NoticeNum(errornum::EN_PHASE_FB_FUN_OPEN_LEVEL_LIMIT);
		return false;
	}

	if (!m_role->GetTaskRecorder()->IsFinished(level_cfg->open_task_id))
	{
		m_role->NoticeNum(errornum::EN_PHASE_FB_FUN_OPEN_PRE_TASK_LIMIT);
		return false;
	}

	// 现在进入副本不消耗次数
	/*if (m_param.fb_list[fb_index].today_times >= level_cfg->free_times)
	{
		m_role->NoticeNum(errornum::EN_PHASE_FB_TIMES_LIMIT);
		return false;
	}*/
	// 上面的判断逻辑似乎有点问题
	//if (m_param.fb_list[fb_index].today_times >= level_cfg->free_times + m_param.fb_list[fb_index].today_buy_times)		//挑战改为不需要判断次数（理文需求，19/3/25）
	//{
	//	m_role->NoticeNum(errornum::EN_PHASE_FB_TIMES_LIMIT);
	//	return false;
	//}

	return true;
}

void RolePhaseFB::OnEnterFB(int fb_index)
{
	if (fb_index < 0 || fb_index >= FB_PHASE_MAX_COUNT)
	{
		return;
	}

	int next_fb_level = this->GetPassLevel(fb_index) + 1;
	m_param.curr_fb_index = fb_index;
	m_param.curr_fb_level = next_fb_level;

// 	const PhaseLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetPhaseFBConfig().GetLevelCfg(fb_index, next_fb_level);
// 	if (NULL != level_cfg)
// 	{
// 		this->OnJoinFb(level_cfg->fb_type);
// 	}

	this->SendInfo();
}

void RolePhaseFB::AddFBTodayTimes(int fb_index)
{
	if (fb_index < 0 || fb_index >= FB_PHASE_MAX_COUNT)
	{
		return;
	}

	m_param.fb_list[fb_index].today_times++;
}

void RolePhaseFB::DecFBTodayTimes(int fb_index)
{
	if (fb_index < 0 || fb_index >= FB_PHASE_MAX_COUNT)
	{
		return;
	}

	m_param.fb_list[fb_index].today_times--;
}

void RolePhaseFB::BuyFBTimes(int fb_index)
{
	if (fb_index < 0 || fb_index >= FB_PHASE_MAX_COUNT)
	{
		return;
	}

	// VIP可购买次数
	const int vip_buy_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role->GetVip()->GetVipLevel(), VAT_FB_PAHSE_COUNT);
	if (m_param.fb_list[fb_index].today_buy_times >= vip_buy_times)
	{
		m_role->NoticeNum(errornum::EN_BUY_PAHSE_FB_TIME_LIMIT);
		return;
	}

	// 消耗元宝
	const int need_gold = LOGIC_CONFIG->GetPhaseFBConfig().GetPhaseResetGold(fb_index);

	if (!m_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (need_gold > 0 && !m_role->GetKnapsack()->GetMoney()->UseAllGold(need_gold, "RolePhaseFB::BuyFBTimes"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_param.fb_list[fb_index].today_buy_times++;

	this->SendInfo();
}

void RolePhaseFB::AutoFBReq(int fb_index, int fb_level)
{
	Scene *scene = m_role->GetScene();
	if (NULL == scene)
	{
		return;
	}

	if (Scene::SCENE_TYPE_PHASE_FB == scene->GetSceneType())
	{
		m_role->NoticeNum(errornum::EN_FB_OPERATE_LIMIT);
		return;
	}

	if (fb_index < 0 || fb_index >= FB_PHASE_MAX_COUNT)
	{
		return;
	}

	if (fb_level <= 0 || fb_level >= FB_PHASE_MAX_LEVEL)
	{
		return;
	}

	const PhaseLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetPhaseFBConfig().GetLevelCfg(fb_index, fb_level);
	if (NULL == level_cfg)
	{
		return;
	}

	if (fb_level > m_param.fb_list[fb_index].is_pass || m_param.fb_list[fb_index].is_pass <= 0)
	{
		m_role->NoticeNum(errornum::EN_PHASE_FB_NO_PASS);
		return;
	}

	if (m_role->GetLevel() < level_cfg->auto_level)
	{
		m_role->NoticeNum(errornum::EN_PHASE_FB_AUTO_LEVEL_LIMIT);
		return;
	}

	if (m_param.fb_list[fb_index].today_times >= level_cfg->free_times + m_param.fb_list[fb_index].today_buy_times)
	{
		m_role->NoticeNum(errornum::EN_BUY_PAHSE_FB_TIME_LIMIT);
		return;

// 		const int vip_buy_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role->GetVip()->GetVipLevel(), VAT_FB_PAHSE_COUNT);
// 		if (m_param.fb_list[fb_index].today_times >= level_cfg->free_times + vip_buy_times)
// 		{
// 			m_role->NoticeNum(errornum::EN_BUY_PAHSE_FB_TIME_LIMIT);
// 			return;
// 		}
// 
// 		int reset_times = m_param.fb_list[fb_index].today_times - level_cfg->free_times + 1;
// 		const int need_gold = LOGIC_CONFIG->GetPhaseFBConfig().GetPhaseResetGold(fb_index, reset_times);
// 		if (need_gold <= 0)
// 		{
// 			return;
// 		}
// 
// 		if (!m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "PhaseFBAuto"))
// 		{
// 			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 			return;
// 		}
// 
// 		m_role->GetRoleActivity()->AddTurntableScore(need_gold);
	}
	
	m_param.fb_list[fb_index].today_times++;

	//m_role->AddExp(level_cfg->reward_exp, "PhaseFBAuto");  // 扫荡没有经验
	//if (is_clean)
	//{
	//	m_role->GetKnapsack()->PutListOrMail(level_cfg->normal_reward_list, PUT_REASON_PHASE_AUTO);
	//}
	//else
	{
		m_role->GetKnapsack()->PutListOrMail(level_cfg->reset_reward_list, PUT_REASON_PHASE_AUTO);
	}

	this->OnJoinFb(level_cfg->fb_type);

	this->SendInfo();

	m_role->GetTaskManager()->OnPassFB(FBManager::FBCT_PHASE);
	EventHandler::Instance().OnPassPhaseFB(m_role, level_cfg->fb_type);
}

void RolePhaseFB::SendInfo()
{
	Protocol::SCPhaseFBInfo pfi;
	for (int i = 0; i < FB_PHASE_MAX_COUNT; i++)
	{
		pfi.info_list[i].is_pass = m_param.fb_list[i].is_pass;
		pfi.info_list[i].today_buy_times = m_param.fb_list[i].today_buy_times;
		pfi.info_list[i].today_times = m_param.fb_list[i].today_times;
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&pfi, sizeof(pfi));
}

void RolePhaseFB::OnPassLevel(int fb_index, int fb_level)
{
	if (fb_index < 0 || fb_index >= FB_PHASE_MAX_COUNT)
	{
		return;
	}

	if (fb_level <= 0 || fb_level >= FB_PHASE_MAX_LEVEL)
	{
		return;
	}

	const PhaseLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetPhaseFBConfig().GetLevelCfg(fb_index, fb_level);
	if (NULL == level_cfg)
	{
		return;
	}

	m_param.fb_list[fb_index].is_pass = static_cast<char>(fb_level);
	m_role->GetKnapsack()->PutListOrMail(level_cfg->first_reward_list, PUT_REASON_PHASE_AUTO);
	m_role->AddExp(level_cfg->reward_exp, "PhaseFBPass", Role::EXP_ADD_REASON_DEFAULT);

	this->SendInfo();
}

bool RolePhaseFB::IsPassLevel(int fb_index)
{
	if (fb_index < 0 || fb_index >= FB_PHASE_MAX_COUNT)
	{
		return false;
	}

	return 0 != m_param.fb_list[fb_index].is_pass;
}

char RolePhaseFB::GetPassLevel(int fb_index)
{
	if (fb_index < 0 || fb_index >= FB_PHASE_MAX_COUNT)
	{
		return 0;
	}

	return m_param.fb_list[fb_index].is_pass;
}

void RolePhaseFB::OnJoinFb(int fb_type)
{
	switch (fb_type)
	{
	case PHASE_FB_TYPE_MOUNT:
		m_role->GetDayCounter()->DayCountSet(DayCounter::DAYCOUNT_ID_FB_PHASE_MOUNT, 1);
		break;

	case PHASE_FB_TYPE_SHIZHUANG_BODY:
		m_role->GetDayCounter()->DayCountSet(DayCounter::DAYCOUNT_ID_FB_PHASE_SHIZHUANG_BODY, 1);
		break;

	case PHASE_FB_TYPE_SHIZHUANG_WUQI:
		m_role->GetDayCounter()->DayCountSet(DayCounter::DAYCOUNT_ID_FB_PHASE_SHIZHUANG_WUQI, 1);
		break;

	case PHASE_FB_TYPE_LINGTONG:
		m_role->GetDayCounter()->DayCountSet(DayCounter::DAYCOUNT_ID_FB_PHASE_LINGTONG, 1);
		break;

	case PHASE_FB_TYPE_FIGHT_MOUNT:
		m_role->GetDayCounter()->DayCountSet(DayCounter::DAYCOUNT_ID_FB_PHASE_FIGHT_MOUNT, 1);
		break;

	case PHASE_FB_TYPE_LINGGONG:
		m_role->GetDayCounter()->DayCountSet(DayCounter::DAYCOUNT_ID_FB_PHASE_LINGGONG, 1);
		break;

	case PHASE_FB_TYPE_LINGQI:
		m_role->GetDayCounter()->DayCountSet(DayCounter::DAYCOUNT_ID_FB_PHASE_LINGQI, 1);
		break;
	}
}
