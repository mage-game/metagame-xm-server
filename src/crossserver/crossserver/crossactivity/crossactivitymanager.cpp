#include "crossactivitymanager.hpp"
#include "crossactivity/impl/crossactivityxiuluotower.hpp"
#include "crossactivity.hpp"
#include "crossactivity/impl/crossactivity1v1.hpp"
#include "crossactivity/impl/crossactivitycrossmultiuserchallenge.hpp"
#include "crossactivity/impl/crossactivitytuanzhan.hpp"
#include "crossactivity/impl/crossactivitypasture.hpp"
#include "crossactivity/impl/crossactivitycrossboss.hpp"
#include "crossactivity/impl/crossactivityteamfb.hpp"
#include "crossactivity/impl/crossactivityhotspring.hpp"
#include "crossactivity/impl/crossactivitycrossguildbattle.hpp"
#include "crossactivity/impl/crossactivitydarknight.hpp"
#include "crossactivity/impl/crossactivityfishing.hpp"
#include "crossactivity/impl/crossactivitynightfight.hpp"
#include "crossactivity/impl/crossactivitymessbattle.hpp"
#include "impl/crossactivityvipboss.hpp"
#include "crossactivity/impl/crossactivitycrossbianjingzhidi.hpp"
#include "crossactivity/impl/crossactivitycrosshusongshuijing.hpp"

CrossActivityManager::CrossActivityManager() : m_cross_server(NULL)
{
	UNSTD_STATIC_CHECK(23 == CROSS_ACTIVITY_TYPE_MAX);

	memset(m_cross_activity_list, 0, sizeof(m_cross_activity_list));

	m_cross_activity_list[CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER - CROSS_ACTIVITY_TYPE_INVALID] = new CrossActivityXiuluoTower(this);
	m_cross_activity_list[CROSS_ACTIVITY_TYPE_CROSS_1V1 - CROSS_ACTIVITY_TYPE_INVALID] = new CrossActivity1V1(this);
	m_cross_activity_list[CROSS_ACTIVITY_TYPE_CROSS_MULTIUSER_CHALLENGE - CROSS_ACTIVITY_TYPE_INVALID] = new CrossActivityCrossMultiuserChallenge(this);
	m_cross_activity_list[CROSS_ACTIVITY_TYPE_TUANZHAN - CROSS_ACTIVITY_TYPE_INVALID] = new CrossActivityTuanzhan(this);
	m_cross_activity_list[CROSS_ACTIVITY_TYPE_PASTURE - CROSS_ACTIVITY_TYPE_INVALID] = new CrossActivityPasture(this);
	m_cross_activity_list[CROSS_ACTIVITY_TYPE_CROSS_BOSS - CROSS_ACTIVITY_TYPE_INVALID] = new CrossActivityCrossBoss(this);
	m_cross_activity_list[CROSS_ACTIVITY_TYPE_TEAM_FB - CROSS_ACTIVITY_TYPE_INVALID] = new CrossActivityTeamFB(this);
	m_cross_activity_list[CROSS_ACTIVITY_TYPE_HOT_SPRING - CROSS_ACTIVITY_TYPE_INVALID] = new CrossActivityHS(this);
	m_cross_activity_list[CROSS_ACTIVITY_TYPE_SHUIJING - CROSS_ACTIVITY_TYPE_INVALID] = new CrossActivityShuijing(this);
	m_cross_activity_list[CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE - CROSS_ACTIVITY_TYPE_INVALID] = new CrossActivityGuildBattle(this);
	m_cross_activity_list[CROSS_ACTIVITY_TYPE_DARK_NIGHT - CROSS_ACTIVITY_TYPE_INVALID] = new CrossActivityDarkNight(this);
	m_cross_activity_list[CROSS_ACTIVITY_TYPE_FISHING - CROSS_ACTIVITY_TYPE_INVALID] = new CrossActivityFishing(this);
	m_cross_activity_list[CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB - CROSS_ACTIVITY_TYPE_INVALID] = new CrossActivityNightFight(this);
	m_cross_activity_list[CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB - CROSS_ACTIVITY_TYPE_INVALID] = new CrossActivityMessBattle(this);
	m_cross_activity_list[CROSS_ACTIVITY_TYPE_LIEKUN_FB - CROSS_ACTIVITY_TYPE_INVALID] = new CrossActivityCrossLieKunFb(this);
	m_cross_activity_list[CROSS_ACTIVITY_TYPE_VIP_BOSS - CROSS_ACTIVITY_TYPE_INVALID] = new CrossActivityCrossVipBoss(this);
	m_cross_activity_list[CROSS_ACTIVITY_TYPE_CROSS_MIZANG_BOSS - CROSS_ACTIVITY_TYPE_INVALID] = new CrossActivityCrossMizangBoss(this);
	m_cross_activity_list[CROSS_ACTIVITY_TYPE_CROSS_YOUMING_BOSS - CROSS_ACTIVITY_TYPE_INVALID] = new CrossActivityCrossYouMingBoss(this);
	// 3091 ...
	m_cross_activity_list[CROSS_ACTIVITY_TYPE_CROSS_BIANJINGZHIDI - CROSS_ACTIVITY_TYPE_INVALID] = new CrossActivityCrossBianJingZhiDi(this);
	m_cross_activity_list[CROSS_ACTIVITY_TYPE_CROSS_CHALLENGEFIELD - CROSS_ACTIVITY_TYPE_INVALID] = new CrossActivity(this, CROSS_ACTIVITY_TYPE_CROSS_CHALLENGEFIELD);
	m_cross_activity_list[CROSS_ACTIVITY_TYPE_HUSONG_SHUIJING - CROSS_ACTIVITY_TYPE_INVALID] = new CrossActivityCrossHusongShuijing(this);
}

CrossActivityManager::~CrossActivityManager()
{
	for (int i = 1; i < CROSS_ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_cross_activity_list[i])
		{
			delete m_cross_activity_list[i];
		}
	}
}

void CrossActivityManager::InitCrossActivity(const CrossActivityParam &cross_activity_param)
{
	for (int i = 1; i < CROSS_ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_cross_activity_list[i])
		{
			m_cross_activity_list[i]->Init(cross_activity_param.data_list[i]);
		}
	}
}

void CrossActivityManager::GetCrossActivityParam(CrossActivityParam *cross_activity_param)
{
	for (int i = 0; i < CROSS_ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_cross_activity_list[i])
		{
			m_cross_activity_list[i]->GetInitParam(&cross_activity_param->data_list[i]);
		}
	}
}

void CrossActivityManager::Update(unsigned long interval, time_t now_second)
{	
	for (int i = 1; i < CROSS_ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_cross_activity_list[i])
		{
			m_cross_activity_list[i]->Update(interval, now_second);
		}
	}
}

void CrossActivityManager::OnDayChange()
{
	for (int i = 1; i < CROSS_ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_cross_activity_list[i])
		{
			m_cross_activity_list[i]->OnDayChange();
		}
	}
}


void CrossActivityManager::OnWeekChange()
{
	for (int i = 1; i < CROSS_ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_cross_activity_list[i])
		{
			m_cross_activity_list[i]->OnWeekChange();
		}
	}
}

bool CrossActivityManager::IsLoadFinish()
{
	for (int i = 1; i < CROSS_ACTIVITY_TYPE_MAX; ++ i)
	{
		if (NULL != m_cross_activity_list[i])
		{


			if (!m_cross_activity_list[i]->IsLoadFinish())
			{
				printf("\n [ERROR]CrossActivityManager::IsLoadFinish FALSE!! cross_activity[%d] \n", i + CROSS_ACTIVITY_TYPE_INVALID);
				return false;
			}
		}
	}

	return true;
}

void CrossActivityManager::OnRegisterGameServer(NetID global_netid)
{
	for (int i = 1; i < CROSS_ACTIVITY_TYPE_MAX; ++ i)
	{
		if (NULL != m_cross_activity_list[i])
		{
			m_cross_activity_list[i]->OnRegisterGameServer(global_netid);
		}
	}
}

void CrossActivityManager::OnInternalGSDisconnnect(NetID global_netid, const UniqueServerID &unique_server_id)
{
	for (int i = 1; i < CROSS_ACTIVITY_TYPE_MAX; ++i)
	{
		if (NULL != m_cross_activity_list[i])
		{
			m_cross_activity_list[i]->OnGSDisconnect(global_netid, unique_server_id);
		}
	}
}

void CrossActivityManager::OnCrossUserLogin(CrossUser *cross_user)
{
	for (int i = 1; i < CROSS_ACTIVITY_TYPE_MAX; ++ i)
	{
		if (NULL != m_cross_activity_list[i])
		{
			m_cross_activity_list[i]->OnCrossUserLogin(cross_user);
		}
	}
}

void CrossActivityManager::OnCrossUserLogout(CrossUser *cross_user)
{
	for (int i = 1; i < CROSS_ACTIVITY_TYPE_MAX; ++ i)
	{
		if (NULL != m_cross_activity_list[i])
		{
			m_cross_activity_list[i]->OnCrossUserLogout(cross_user);
		}
	}
}

bool CrossActivityManager::IsCrossActivityOpen(int cross_activity_type)
{
	CrossActivity *cross_activity = this->GetCrossActivity(cross_activity_type);
	if (NULL != cross_activity)
	{
		return ACTIVITY_STATUS_OPEN == cross_activity->GetActivityStatus();
	}

	return false;
}

bool CrossActivityManager::IsCrossActivityClose(int cross_activity_type)
{
	CrossActivity *cross_activity = this->GetCrossActivity(cross_activity_type);
	if (NULL != cross_activity)
	{
		return ACTIVITY_STATUS_CLOSE == cross_activity->GetActivityStatus();
	}

	return false;
}

CrossActivity * CrossActivityManager::GetCrossActivity(int cross_activity_type)
{
	int cross_activity_type_index = cross_activity_type - CROSS_ACTIVITY_TYPE_INVALID;
	if (cross_activity_type_index <= 0 || cross_activity_type_index >= CROSS_ACTIVITY_TYPE_MAX) return NULL;

	return m_cross_activity_list[cross_activity_type_index];
}

CrossActivity1V1 * CrossActivityManager::GetCrossActivity1V1()
{
	return (CrossActivity1V1*)this->GetCrossActivity(CROSS_ACTIVITY_TYPE_CROSS_1V1);
}

CrossActivityCrossMultiuserChallenge * CrossActivityManager::GetCrossActivityCrossMultiuserChallenge()
{
	return (CrossActivityCrossMultiuserChallenge*)this->GetCrossActivity(CROSS_ACTIVITY_TYPE_CROSS_MULTIUSER_CHALLENGE);
}

CrossActivityTuanzhan * CrossActivityManager::GetCrossActivityTuanzhan()
{
	return (CrossActivityTuanzhan*)this->GetCrossActivity(CROSS_ACTIVITY_TYPE_TUANZHAN);
}

CrossActivityShuijing * CrossActivityManager::GetCrossActivityShuijing()
{
	return (CrossActivityShuijing*)this->GetCrossActivity(CROSS_ACTIVITY_TYPE_SHUIJING);
}

CrossActivityGuildBattle *CrossActivityManager::GetCrossActivityGuildBattle()
{
	return (CrossActivityGuildBattle*)this->GetCrossActivity(CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE);
}

int CrossActivityManager::GetWaitSceneID(const UniqueUserID &unique_user_id, int cross_activity_type, Posi *pos)
{
	CrossActivity *cross_activity = this->GetCrossActivity(cross_activity_type);
	if (NULL != cross_activity)
	{
		return cross_activity->GetWaitSceneID(unique_user_id, pos);
	}

	return 0;
}

void CrossActivityManager::OnForceToNextState(int cross_activity_type)
{
	CrossActivity *cross_activity = this->GetCrossActivity(cross_activity_type);
	if (NULL != cross_activity)
	{
		cross_activity->ForceToNextState();
	}
}

bool CrossActivityManager::CheckCanStartCross(int cross_activity_type, const UniqueUserID &unique_uesr_id, int merge_server_id)
{
	CrossActivity *cross_activity = this->GetCrossActivity(cross_activity_type);
	if (NULL != cross_activity)
	{
		return cross_activity->CheckCanStartCross(unique_uesr_id, merge_server_id);
	}

	return false;
}

CrossActivityXiuluoTower * CrossActivityManager::GetCrossActivityXiuluoTower()
{
	return (CrossActivityXiuluoTower*)this->GetCrossActivity(CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER);
}

CrossActivityPasture * CrossActivityManager::GetCrossActivityPasture()
{
	return (CrossActivityPasture*)this->GetCrossActivity(CROSS_ACTIVITY_TYPE_PASTURE);
}

CrossActivityCrossBoss * CrossActivityManager::GetCrossActivityCrossBoss()
{
	return (CrossActivityCrossBoss*)this->GetCrossActivity(CROSS_ACTIVITY_TYPE_CROSS_BOSS);
}

CrossActivityHS * CrossActivityManager::GetCrossActivityHotSpring()
{
	return (CrossActivityHS*)this->GetCrossActivity(CROSS_ACTIVITY_TYPE_HOT_SPRING);
}

CrossActivityCrossLieKunFb * CrossActivityManager::GetCrossActivityCrossLieKunFb()
{
	return (CrossActivityCrossLieKunFb*)this->GetCrossActivity(CROSS_ACTIVITY_TYPE_LIEKUN_FB);
}

CrossActivityCrossMizangBoss * CrossActivityManager::GetCrossActivityCrossMizangBoss()
{
	return (CrossActivityCrossMizangBoss*)this->GetCrossActivity(CROSS_ACTIVITY_TYPE_CROSS_MIZANG_BOSS);
}

CrossActivityCrossYouMingBoss * CrossActivityManager::GetCrossActivityCrossYouMingBoss()
{
	return (CrossActivityCrossYouMingBoss*)this->GetCrossActivity(CROSS_ACTIVITY_TYPE_CROSS_YOUMING_BOSS);
}

CrossActivityCrossBianJingZhiDi * CrossActivityManager::GetCrossActivityCrossBianJingZhiDi()
{
	return (CrossActivityCrossBianJingZhiDi*)this->GetCrossActivity(CROSS_ACTIVITY_TYPE_CROSS_BIANJINGZHIDI);
}
