#include "roledailyfb.hpp"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "other/fb/dailyfbconfig.hpp"
#include "servercommon/errornum.h"
#include "protocal/msgfb.h"
#include "other/event/eventhandler.hpp"
#include "monster/monsterpool.h"
#include "gameworld/world.h"
#include "global/usercache/usercache.hpp"
#include "item/knapsack.h"
#include "task/taskrecorder.h"
#include "other/vip/vip.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "scene/scene.h"
#include "npc/transmit/fbmanager.hpp"
#include "other/itemlevelreward/itemlevelreward.hpp"
#include "task/newtaskmanager.h"

RoleDailyFb::RoleDailyFb() : m_role(NULL)
{

}

RoleDailyFb::~RoleDailyFb()
{

}

void RoleDailyFb::Init(Role *role, const DailyFbParam &param)
{
	m_role = role;
	m_fb_param = param;
}

void RoleDailyFb::GetInitParam(DailyFbParam *param)
{
	*param = m_fb_param;
}

void RoleDailyFb::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (now_dayid == old_dayid) return;
	
	m_fb_param.expfb_today_enter_times = 0;
	m_fb_param.expfb_today_pay_times = 0;
	this->SendAllInfo();
}

bool RoleDailyFb::CheckEenterCD()
{
	const ExpFbOtherCfg &exp_other_cfg = LOGIC_CONFIG->GetDailyFbCfg().GetExpOtherCfg();
	if ((unsigned int)(EngineAdapter::Instance().Time()) < m_fb_param.last_quit_fb_time + exp_other_cfg.interval_time) // 进入副本的冷却时间
	{
		m_role->NoticeNum(errornum::EN_FB_EXP_TIME_LIMIT);
		return false;
	}

	return true;
}

bool RoleDailyFb::CanEnter(int dailyfb_type)
{
	const ExpFbOtherCfg &exp_other_cfg = LOGIC_CONFIG->GetDailyFbCfg().GetExpOtherCfg();
	const DailyFbItem *fb_cfg = LOGIC_CONFIG->GetDailyFbCfg().GetDailyFbItemCfg(dailyfb_type);
	if (NULL == fb_cfg)
	{
		return false;
	}

	if (m_role->GetScene()->GetSceneType() == Scene::SCENE_TYPE_EXPFB)
	{
		return false;
	}

	if (!m_role->GetRoleStatusManager()->CanEnterFB(true, false))
	{
		return false;
	}

	if (m_role->GetLevel() < fb_cfg->role_level)
	{	
		m_role->NoticeNum(errornum::EN_FB_ROLE_LEVEL_LIMIT_EXP_FB);
		return false;
	}

	if (!m_role->GetTaskRecorder()->IsFinished(fb_cfg->complete_task_id))
	{
		m_role->NoticeNum(errornum::EN_TASK_NOT_COMPLETED_EXP_FB);
		return false;
	}

	if (m_fb_param.expfb_today_pay_times + exp_other_cfg.day_times - m_fb_param.expfb_today_enter_times <= 0) // 次数不足
	{
		m_role->NoticeNum(errornum::EN_FB_ENTER_FB_NOT_TIMES);
		return false;
	}
	
	// 新手首次不消耗
	if (m_fb_param.expfb_history_enter_times != 0)
	{
		// 检查材料
		static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = exp_other_cfg.item_stuff.item_id;
		stuff_list[0].num = exp_other_cfg.item_stuff.num;
		stuff_list[0].buyable = false;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, true))
		{
			return false;
		}
	}

	return true;
}

void RoleDailyFb::OnEnterExpFb()
{
	// 历史第一次进入经验副本，不消耗次数
	if (m_fb_param.expfb_history_enter_times != 0)
	{
		m_fb_param.expfb_today_enter_times++;
	}
	
	m_fb_param.expfb_history_enter_times++;

	EventHandler::Instance().OnEnterFBExp(m_role, m_fb_param.expfb_history_enter_times);
}

void RoleDailyFb::SetExpFbLastExp(long long exp)
{
	if (exp > m_fb_param.expfb_last_exp)
	{
		m_fb_param.expfb_last_exp = exp;
	}
}

void RoleDailyFb::SetExpFbRecordMaxExp(long long exp)
{
	if (exp > m_fb_param.expfb_record_max_exp)
	{
		m_fb_param.expfb_record_max_exp = exp;
	}
}
void RoleDailyFb::OnPayFbTimes()
{
	if (NULL == m_role) return;

	const int times = LOGIC_CONFIG->GetDailyFbCfg().GetVipPayTime(m_role->GetVip()->GetVipLevel());
	if (0 == times)
	{
		m_role->NoticeNum(errornum::EN_FB_EXP_PAY_TIME_VIP_LIMIT);
		return;
	}

	if (m_fb_param.expfb_today_pay_times >= times)
	{
		m_role->NoticeNum(errornum::EN_FB_EXP_PAY_TIME_LIMIT);
		return;
	}

	const int need_diamond = LOGIC_CONFIG->GetDailyFbCfg().GetExpFbResetGold(m_fb_param.expfb_today_pay_times + 1);
	if (0 == need_diamond)
	{
		return;
	}
	
	if (!m_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(need_diamond))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(need_diamond, "ExpFbTimesCost"))
	{
		return;
	}
	
	this->AddExpFbTodayPayTimes();
	this->SendAllInfo();
	
	m_role->GetRoleActivity()->AddTurntableScore(need_diamond);
}

void RoleDailyFb::SendAllInfo()
{
	static Protocol::SCDailyFBRoleInfo cmd;
	cmd.expfb_today_pay_times = m_fb_param.expfb_today_pay_times;
	cmd.expfb_today_enter_times = m_fb_param.expfb_today_enter_times;
	cmd.last_quit_fb_time = m_fb_param.last_quit_fb_time;
	cmd.max_exp = m_fb_param.expfb_last_exp;
	cmd.max_wave = m_fb_param.expfb_history_pass_max_wave;
	cmd.expfb_history_enter_times = m_fb_param.expfb_history_enter_times;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}

void RoleDailyFb::OnDailyFbFinish(int dailyfb_type, int pass_time, bool is_pass)
{
}

bool RoleDailyFb::AutoFBReq(int dailyfb_type)
{
	if (!this->CanEnter(dailyfb_type))
	{
		return false;
	}
	this->EnterDailyFb(dailyfb_type);
	this->AutoFBReward();
	EventHandler::Instance().OnPassExpFb(m_role);
	return true;
}

void RoleDailyFb::EnterDailyFb(int dailyfb_type)
{
	const DailyFbItem *fb_cfg = LOGIC_CONFIG->GetDailyFbCfg().GetDailyFbItemCfg(dailyfb_type);
	if (NULL == fb_cfg)
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (NULL == knapsack || knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (m_role->GetRoleDailyFb()->GetExpFbHistoryEnterTimes() != 0)
	{
		// 材料1
		const ExpFbOtherCfg &exp_other_cfg = LOGIC_CONFIG->GetDailyFbCfg().GetExpOtherCfg();
		static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = exp_other_cfg.item_stuff.item_id;
		stuff_list[0].num = exp_other_cfg.item_stuff.num;
		stuff_list[0].buyable = false;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, true))
		{
			m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return;
		}

		if (!ItemExtern::ConsumeItem(m_role, consume_list, "ExpFb"))
		{
			return;
		}
	}

	FBManager::Instance().LogFunctionStats(m_role, FBManager::FBCT_DAILY_FB, dailyfb_type, 0, 0);

	m_role->GetRoleDailyFb()->OnEnterExpFb();

	gamelog::g_log_debug.printf(LL_DEBUG, "RoleDailyFb::EnterDailyFb: %s[%d] ret:%d fb_param:%d,%d",
		m_role->GetName(), m_role->GetUID(), 1, FBManager::FBCT_DAILY_FB, dailyfb_type);
}

void RoleDailyFb::AutoFBReward()
{
	long long total_exp = 0;
	int add_percent = 0;
	const ExpFbOtherCfg &exp_other_cfg = LOGIC_CONFIG->GetDailyFbCfg().GetExpOtherCfg();
	
	const int max_wave = 100;	//防止配置错造成循环过大
	for (int i = 1; i <= exp_other_cfg.total_wave && i < max_wave; ++i)
	{
		long long exp = LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(m_role->GetLevel(), 0);	// 根据等级获取经验
		double exp_factor = 0;		//经验系数
		int wave_refresh_count = 0;	//每波刷新多少只怪
		// 根据当前波数获取经验系数
		switch (i)
		{
		case 1: { exp_factor = exp_other_cfg.exp_factor1; wave_refresh_count = exp_other_cfg.fir_wave_refresh_count; } break;
		case 2: { exp_factor = exp_other_cfg.exp_factor2; wave_refresh_count = exp_other_cfg.sec_wave_refresh_count; } break;
		case 3: { exp_factor = exp_other_cfg.exp_factor3; wave_refresh_count = exp_other_cfg.other_wave_refresh_count; } break;
		default: { wave_refresh_count = exp_other_cfg.other_wave_refresh_count; } break;
		}
		long long monster_exp = static_cast<long long>(exp * exp_factor);							//怪物经验 = 配置经验 * 系数
		const int total_monster_count = wave_refresh_count * exp_other_cfg.monster_count;			// 当前波的怪物总数 = 刷新次数 * 怪物数量
		total_exp += (monster_exp * total_monster_count);
	}

	m_role->AddExp(total_exp, "AutoExpFB", Role::EXP_ADD_EXP_FB, NULL, add_percent);
}
