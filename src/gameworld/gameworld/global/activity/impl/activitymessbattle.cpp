#include "activitymessbattle.hpp"
#include "engineadapter.h"
#include "config/sharedconfig/sharedconfig.h"
#include "world.h"
#include "gamelog.h"
#include "config/logicconfigmanager.hpp"
#include "other/fb/messbattle/messbattleconfig.hpp"
#include "scene/speciallogic/doublesidefbspecial/specialmessbattle.hpp"

ActivityMessBattle::ActivityMessBattle(ActivityManager *activity_manager)
	: Activity(activity_manager, ACTIVITY_TYPE_MESS_BATTLE_FB)
{

}

ActivityMessBattle::~ActivityMessBattle()
{

}

void ActivityMessBattle::Update(unsigned long interval, time_t now_second)
{
	const MessBattleFbOtherConfig  &other_cfg = LOGIC_CONFIG->GetMessBattleFbConfig().GetOtherCfg();
	const int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	const int open_cross_day = other_cfg.open_cross_begin_day;

	// 根据天数检测,跨服活动开启时不会开启本服活动
	if (opengame_day >= open_cross_day)
	{
		return;
	}

	Activity::Update(interval, now_second);
}

bool ActivityMessBattle::ForceToNextState()
{
	const MessBattleFbOtherConfig  &other_cfg = LOGIC_CONFIG->GetMessBattleFbConfig().GetOtherCfg();
	const int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	const int open_cross_day = other_cfg.open_cross_begin_day;

	// 根据天数检测,天数限制内禁止强开
	if (opengame_day >= open_cross_day)
	{
		gamelog::g_log_activity.printf(LL_WARNING, "ActivityMessBattle::ForceToNextState. Frobid force open activity[%d].",
			(int)ACTIVITY_TYPE_MESS_BATTLE_FB);

		return false;
	}

	return Activity::ForceToNextState();
}

void ActivityMessBattle::OnActivityStatusChange(int from_status, int to_status)
{
	if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		const MessBattleFbOtherConfig  &other_cfg = LOGIC_CONFIG->GetMessBattleFbConfig().GetOtherCfg();

		for (int i = 0; i < CROSS_MESS_BATTLE_MAX_SCENE_NUM; ++i)
		{
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(other_cfg.scene_id, Scene::SCENE_TYPE_MESS_BATTLE_FB + i);

			if (NULL != scene && Scene::SCENE_TYPE_MESS_BATTLE_FB == scene->GetSceneType())
			{
				SpecialMessBattle *special_mess = dynamic_cast<SpecialMessBattle *>(scene->GetSpecialLogic());
				if (NULL != special_mess)
				{
					special_mess->OnActivityClose();
				}
			}
		}
	}

	Activity::OnActivityStatusChange(from_status, to_status);
}
