
#include "activitynightfight.hpp"
#include "engineadapter.h"
#include "config/sharedconfig/sharedconfig.h"
#include "world.h"
#include "gamelog.h"
#include "config/logicconfigmanager.hpp"
#include "other/fb/nightfight/nightfightfbconfig.hpp"
#include "scene/speciallogic/doublesidefbspecial/specialnightfight.hpp"

ActivityNightFight::ActivityNightFight(ActivityManager *activity_manager)
	: Activity(activity_manager, ACTIVITY_TYPE_NIGHT_FIGHT_FB)
{

}

ActivityNightFight::~ActivityNightFight()
{

}

void ActivityNightFight::Update(unsigned long interval, time_t now_second)
{
	const NightFightFbOtherConfig &other_cfg = LOGIC_CONFIG->GetNightFightFbConfig().GetOtherCfg();
	const int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	const int open_cross_day = other_cfg.open_cross_begin_day;

	// 根据天数检测,跨服活动开启时不会开启本服活动
	if (opengame_day >= open_cross_day)
	{
		return;
	}

	Activity::Update(interval, now_second);
}

bool ActivityNightFight::ForceToNextState()
{
	const NightFightFbOtherConfig &other_cfg = LOGIC_CONFIG->GetNightFightFbConfig().GetOtherCfg();
	const int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	const int open_cross_day = other_cfg.open_cross_begin_day;

	// 根据天数检测,天数限制内禁止强开
	if (opengame_day >= open_cross_day)
	{
		gamelog::g_log_activity.printf(LL_WARNING, "ActivityNightFight::ForceToNextState. Frobid force open activity[%d].", 
			(int)ACTIVITY_TYPE_NIGHT_FIGHT_FB);

		return false;
	}

	return Activity::ForceToNextState();
}

void ActivityNightFight::OnActivityStatusChange(int from_status, int to_status)
{
	if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		const NightFightFbOtherConfig &other_cfg = LOGIC_CONFIG->GetNightFightFbConfig().GetOtherCfg();

		for (int i = 0; i < CROSS_NIGHT_FIGHT_MAX_SCENE_NUM; ++i)
		{
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(other_cfg.scene_id, Scene::SCENE_TYPE_NIGHT_FIGHT_FB + i);
			
			if (NULL != scene && Scene::SCENE_TYPE_NIGHT_FIGHT_FB == scene->GetSceneType())
			{
				SpecialNightFight *special_nightfight = dynamic_cast<SpecialNightFight *>(scene->GetSpecialLogic());
				if (NULL != special_nightfight)
				{
					special_nightfight->OnActivityClose();
				}
			}
		}
	}

	Activity::OnActivityStatusChange(from_status, to_status);
}
