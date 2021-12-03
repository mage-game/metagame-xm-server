#include "activitywedding.hpp"
#include "servercommon/qingyuandef.hpp"
#include "scene/speciallogic/worldspecial/specialhunyan.hpp"
#include "config/logicconfigmanager.hpp"
#include "world.h"
#include "global/worldstatus2/worldstatus2.hpp"
#include "other/qingyuan/qingyuanconfig.hpp"

ActivityWedding::ActivityWedding(ActivityManager *activity_manager)
	: Activity(activity_manager, ACTIVITY_TYPE_WEDDING), m_old_activity_status(0)
{
	
}

ActivityWedding::~ActivityWedding()
{

}

void ActivityWedding::Update(unsigned long interval, time_t now_second)
{
	WeddingParam &wedding_param = WorldStatus2::Instance().GetWeddingParam();
	const HunyanConfig &hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig();
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	unsigned int zero_time =static_cast<unsigned int>(GetZeroTime(now));

	int cur_wedding_seq = -1;

	const WeddingYuYueTimeCfg &time_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetWeddingYuYueTime();

	for (int i = WEDDING_YUYUE_TIME_MAX - 1; i >= 0; -- i)
	{
		int standby_time = time_cfg.cfg_list[i].apply_time;
		if (now >= HHMMToNow(zero_time, standby_time))
		{
			cur_wedding_seq = time_cfg.cfg_list[i].seq;
			break;
		}
	}

	if (cur_wedding_seq < 0 || cur_wedding_seq >= WEDDING_YUYUE_TIME_MAX)
	{
		return;
	}

	int begin_standby_time = time_cfg.cfg_list[cur_wedding_seq].apply_time;
	UInt32 hunyan_standby_time = HHMMToNow(zero_time, begin_standby_time);
	UInt32 hunyan_end_time = hunyan_standby_time + hunyan_cfg.standby_s + hunyan_cfg.xunyou_interval_s + hunyan_cfg.start_s;
	if (!wedding_param.yuyue_info[cur_wedding_seq].is_has_done)
	{
		if (now >= hunyan_standby_time &&
			now <= hunyan_end_time &&
			0 != wedding_param.yuyue_info[cur_wedding_seq].role_id &&
			0 != wedding_param.yuyue_info[cur_wedding_seq].lover_role_id)
		{		
			// 检查创建婚宴副本
			this->CheckCreateHunyanFb(hunyan_standby_time);
		}
	}

	SpecialHunyan *hunyan_scene = World::GetInstWorld()->GetSceneManager()->GetSpecialHunyan(COMMON_SCENE_KEY);
	if (NULL != hunyan_scene)
	{
		if (HUNYAN_STATE_TYPE_STANDBY == m_activity_status && now >= m_next_status_switch_time)
		{
			m_activity_status = HUNYAN_STATE_TYPE_XUNYOU;
			m_next_status_switch_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + hunyan_cfg.xunyou_interval_s;
			hunyan_scene->OnHunyanXunyou();
		}

		else if (HUNYAN_STATE_TYPE_XUNYOU == m_activity_status && now >= m_next_status_switch_time)
		{
			m_activity_status = HUNYAN_STATE_TYPE_START;
			m_next_status_switch_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + hunyan_cfg.start_s;
			hunyan_scene->OnHunyanStart();
		}

		else if (HUNYAN_STATE_TYPE_START == m_activity_status && now >= m_next_status_switch_time)
		{
			m_activity_status = HUNYAN_STATE_TYPE_STOP;
			hunyan_scene->OnHunyanStop();
			m_next_status_switch_time = 0;
		}
	}

	if (now >= hunyan_end_time && m_activity_status != HUNYAN_STATE_TYPE_STOP)
	{
		m_activity_status = HUNYAN_STATE_TYPE_STOP;
	}
	
	if (m_old_activity_status != m_activity_status)
	{
		this->NotfiyActivityStatus(true);
		m_old_activity_status = m_activity_status;
	}
}

int ActivityWedding::GetCurHunyanStatus(unsigned int hunyan_standby_time, int &status)
{
	const HunyanConfig &hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig();

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	
	unsigned int standby_end_time = hunyan_standby_time + hunyan_cfg.standby_s;
	unsigned int xunyou_end_time = standby_end_time + hunyan_cfg.xunyou_interval_s;
	unsigned int hunyan_end_time = xunyou_end_time + hunyan_cfg.start_s;

	if (now < standby_end_time)
	{
		status = HUNYAN_STATE_TYPE_STANDBY;
		return standby_end_time - now;
	}

	if (now < xunyou_end_time)
	{
		status = HUNYAN_STATE_TYPE_XUNYOU;
		return xunyou_end_time - now;
	}

	if (now < hunyan_end_time)
	{
		status = HUNYAN_STATE_TYPE_START;
		return hunyan_end_time - now;
	}

	status = HUNYAN_STATE_TYPE_STOP;
	return 0;
}

void ActivityWedding::CheckCreateHunyanFb(unsigned int hunyan_standby_time)
{
	SpecialHunyan *hunyan_scene = World::GetInstWorld()->GetSceneManager()->GetSpecialHunyan(COMMON_SCENE_KEY, true);
	if (NULL != hunyan_scene && HUNYAN_STATE_TYPE_STOP == m_activity_status)
	{
		int next_status_interval_s = this->GetCurHunyanStatus(hunyan_standby_time, m_activity_status);
		m_next_status_switch_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + next_status_interval_s;

		switch (m_activity_status)
		{
		case HUNYAN_STATE_TYPE_STANDBY:
		{
			hunyan_scene->OnHunyanStandBy();
		}
		break;

		case HUNYAN_STATE_TYPE_XUNYOU:
		{
			hunyan_scene->OnHunyanStandBy();
			hunyan_scene->OnHunyanXunyou();
		}
		break;

		case HUNYAN_STATE_TYPE_START:
		{
			hunyan_scene->OnHunyanStandBy();
			hunyan_scene->OnHunyanStart();
		}
		break;
		}
	}
}

