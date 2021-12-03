#include <memory>

#include "activity.hpp"
#include "activitymanager.hpp"
#include "gstr.h"

#include "impl/activityzhuxie.hpp"
#include "impl/activitymonsterinvade.hpp"
#include "impl/activityhusong.hpp"
#include "impl/activityqunxianluandou.hpp"
#include "impl/activityxianmengzhan.hpp"
#include "impl/activitynationalboss.hpp"
#include "impl/activitygongchengzhan.hpp"
#include "impl/activityyizhandaodi.hpp"
#include "impl/activitymoshenjianglin.hpp"
#include "impl/activitycishatask.hpp"
#include "impl/activityguaji.hpp"
#include "impl/activitywuxingguaji.hpp"
#include "impl/activityshuijing.hpp"
#include "impl/acthuangchenghuizhan.hpp"
#include "impl/actcampdefend.hpp"
#include "impl/activitytianjiangcaibao.h"
#include "impl/activityguildbattle.hpp"
#include "impl/activityhappytreegrowexchange.hpp"
#include "impl/activitynightfight.hpp"
#include "impl/activityguildboss.hpp"
#include "impl/activitymillionaire.hpp"
#include "impl/activityterritorywar.hpp"
#include "impl/activitywanglingexplore.h"
#include "impl/activityguildbonfire.h"
#include "impl/activityxingzuoyiji.hpp"
#include "impl/activitywedding.hpp"
#include "impl/activityguildquestion.hpp"
#include "impl/activitymessbattle.hpp"
#include "impl/activityworship.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "protocal/msgrole.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "config/activityconfig/activityswitchconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "scene/activityshadow/activityshadow.hpp"

ActivityManager::ActivityManager()
{
	UNSTD_STATIC_CHECK(36 == ACTIVITY_TYPE_MAX - ACTIVITY_TYPE_INVALID);

	memset(m_activity_list, 0, sizeof(m_activity_list));

	m_activity_list[ACTIVITY_TYPE_ZHUXIE] = new ActivityZhuXie(this);
	m_activity_list[ACTIVITY_TYPE_MONSTER_INVADE] = new ActivityMonsterInvade(this);
	m_activity_list[ACTIVITY_TYPE_HUSONG] = new ActivityHusong(this);
	m_activity_list[ACTIVITY_TYPE_QUNXIANLUANDOU] = new ActivityQunXianLuanDou(this);
	m_activity_list[ACTIVITY_TYPE_XIANMENGZHAN] = new ActivityXianMengZhan(this);
	m_activity_list[ACTIVITY_TYPE_NATIONAL_BOSS] = new ActivityNationalBoss(this);
	m_activity_list[ACTIVITY_TYPE_GONGCHENGZHAN] = new ActivityGongChengZhan(this);
	m_activity_list[ACTIVITY_TYPE_YIZHANDAODI] = new ActivityYiZhanDaoDi(this);
	m_activity_list[ACTIVITY_TYPE_MOSHENJIANGLIN] = new	ActivityMoShenJiangLin(this);
	m_activity_list[ACTIVITY_TYPE_CISHA_TASK] = new ActivityCishaTask(this);
	m_activity_list[ACTIVITY_TYPE_GUAJI] = new ActivityGuaji(this);
	m_activity_list[ACTIVITY_TYPE_WUXING_GUAJI] = new ActivityWuXingGuaJi(this);
 	m_activity_list[ACTIVITY_TYPE_SHUIJING] = new ActivityShuiJing(this);
	m_activity_list[ACTIVITY_TYPE_HUANGCHENGHUIZHAN] = new ActivityHuangchenghuizhan(this);
	m_activity_list[ACTIVITY_TYPE_CAMP_DEFEND1] = new ActivityCampDefend(this, ACTIVITY_TYPE_CAMP_DEFEND1);
	m_activity_list[ACTIVITY_TYPE_CAMP_DEFEND2] = new ActivityCampDefend(this, ACTIVITY_TYPE_CAMP_DEFEND2);
	m_activity_list[ACTIVITY_TYPE_CAMP_DEFEND3] = new ActivityCampDefend(this, ACTIVITY_TYPE_CAMP_DEFEND3);
	m_activity_list[ACTIVITY_TYPE_TIANJIANGCAIBAO] = new ActivityTianJianbCaiBao(this);
	m_activity_list[ACTIVITY_TYPE_GUILDBATTLE] = new ActivityGuildBattle(this);
	m_activity_list[ACTIVITY_TYPE_HAPPYTREE_GROW_EXCHANGE] = new ActivityHappyTreeExchange(this);
	m_activity_list[ACTIVITY_TYPE_NIGHT_FIGHT_FB] = new ActivityNightFight(this);
	m_activity_list[ACTIVITY_TYPE_GUILD_BOSS] = new ActivityGuildBoss(this);
	m_activity_list[ACTIVITY_TYPE_MILLIONAIRE] = new ActivityMillionare(this);
	m_activity_list[ACTIVITY_TYPE_WANGLINGEXPLORE] = new ActivityWangLingExplore(this);
	m_activity_list[ACTIVITY_TYPE_TERRITORY_WAR] = new ActivityTerritoryWar(this);
	m_activity_list[ACTIVITY_TYPE_GUILD_BONFIRE] = new ActivityGuildBonfire(this);
//	m_activity_list[ACTIVITY_TYPE_XINGZUOYIJI] = new ActivityXingzuoYiji(this);
	m_activity_list[ACTIVITY_TYPE_WEDDING] = new ActivityWedding(this);
	m_activity_list[ACTIVITY_TYPE_GUILD_QUESTION] = new ActivityGuildQuestion(this);
	m_activity_list[ACTIVITY_TYPE_MESS_BATTLE_FB] = new ActivityMessBattle(this);
	m_activity_list[ACTIVITY_TYPE_WORSHIP] = new ActivityWorship(this);
	m_activity_list[ACTIVITY_TYPE_GUILD_FB] = new Activity(this, ACTIVITY_TYPE_GUILD_FB);
	m_activity_list[ACTIVITY_TYPE_GUILD_YUNBIAO] = new Activity(this, ACTIVITY_TYPE_GUILD_YUNBIAO);
}

ActivityManager::~ActivityManager()
{
	for (int i = ACTIVITY_TYPE_INVALID; i < ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_activity_list[i])
		{
			delete m_activity_list[i];
		}
	}

	memset(m_activity_list, 0, sizeof(m_activity_list));
}


ActivityManager & ActivityManager::Instance()
{
	static ActivityManager am;
	return am;
}

void ActivityManager::Update(unsigned long interval, time_t now_second)
{
	if (!WorldStatus::Instance().IsLoadFinish()) return;

	for (int i = ACTIVITY_TYPE_INVALID; i < ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_activity_list[i])
		{
			m_activity_list[i]->Update(interval, now_second);
		}
	}
}

bool ActivityManager::IsActivityOpen(int activity_type)
{
	if (activity_type <= ACTIVITY_TYPE_INVALID || activity_type >= ACTIVITY_TYPE_MAX) return false;
	if (NULL == m_activity_list[activity_type]) return false;

	return (ACTIVITY_STATUS_OPEN == m_activity_list[activity_type]->GetActivityStatus());
}

bool ActivityManager::IsActivityClose(int activity_type)
{
	if (activity_type <= ACTIVITY_TYPE_INVALID || activity_type >= ACTIVITY_TYPE_MAX) return true;
	if (NULL == m_activity_list[activity_type]) return true;

	return (ACTIVITY_STATUS_CLOSE == m_activity_list[activity_type]->GetActivityStatus());
}

void ActivityManager::OnUserLogin(Role *role)
{
	role->SyncCommonInfo(Protocol::SCIT_RAND_ACT_ZHUANFU_INFO, LocalConfig::Instance().GetRandActZhuanfuType());

	for (int i = ACTIVITY_TYPE_INVALID; i < ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_activity_list[i])
		{
			m_activity_list[i]->OnUserLogin(role);
		}
	}
}

void ActivityManager::OnUserLogout(Role *role)
{
	for (int i = ACTIVITY_TYPE_INVALID; i < ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_activity_list[i])
		{
			m_activity_list[i]->OnUserLogout(role);
		}
	}
}

void ActivityManager::OnUserEnterScene(Role *role)
{
	for (int i = ACTIVITY_TYPE_INVALID; i < ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_activity_list[i])
		{
			m_activity_list[i]->OnUserEnterScene(role);
		}
	}
}

void ActivityManager::OnUserLeaveScene(Role *role)
{
	for (int i = ACTIVITY_TYPE_INVALID; i < ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_activity_list[i])
		{
			m_activity_list[i]->OnUserLeaveScene(role);
		}
	}
}

bool ActivityManager::IsActivityLimitGuildOper(int guild_oper_type, Role *role)
{
	static const int oper_limit_mark = (1 << GUILD_OPER_TYPE_DISMISS) | (1 << GUILD_OPER_TYPE_QUIT) | (1 << GUILD_OPER_TYPE_KICK) | (1 << GUILD_OPER_TYPE_APPOINT);

	if (NULL != m_activity_list[ACTIVITY_TYPE_GONGCHENGZHAN])
	{
		if (ACTIVITY_STATUS_CLOSE != m_activity_list[ACTIVITY_TYPE_GONGCHENGZHAN]->GetActivityStatus() && 0 != (oper_limit_mark & (1 << guild_oper_type)))
		{
			role->NoticeNum(errornum::EN_GUILD_GCZ_LIMIT);
			return true;
		}
	}

	if (NULL != m_activity_list[ACTIVITY_TYPE_XIANMENGZHAN])
	{
		if (ACTIVITY_STATUS_OPEN == m_activity_list[ACTIVITY_TYPE_XIANMENGZHAN]->GetActivityStatus() && 0 != (oper_limit_mark & (1 << guild_oper_type)))
		{
			role->NoticeNum(errornum::EN_GUILD_XMZ_LIMIT);
			return true;
		}
	}

	if (NULL != m_activity_list[ACTIVITY_TYPE_TERRITORY_WAR])
	{
		if (ACTIVITY_STATUS_OPEN == m_activity_list[ACTIVITY_TYPE_TERRITORY_WAR]->GetActivityStatus() && 0 != (oper_limit_mark & (1 << guild_oper_type)))
		{
			role->NoticeNum(errornum::EN_GUILD_TW_LIMIT);
			return true;
		}
	}

	if (NULL != m_activity_list[ACTIVITY_TYPE_GUILDBATTLE])
	{
		if (ACTIVITY_STATUS_CLOSE != m_activity_list[ACTIVITY_TYPE_GUILDBATTLE]->GetActivityStatus() && 0 != (oper_limit_mark & (1 << guild_oper_type)))
		{
			role->NoticeNum(errornum::EN_GUILD_BATTLE_LIMIT);
			return true;
		}
	}

	if (NULL != m_activity_list[ACTIVITY_TYPE_GUILD_BOSS])
	{
		if (ACTIVITY_STATUS_OPEN == m_activity_list[ACTIVITY_TYPE_GUILD_BOSS]->GetActivityStatus() && 0 != (oper_limit_mark & (1 << guild_oper_type)))
		{
			role->NoticeNum(errornum::EN_GUILD_BOSS_LIMIT);
			return true;
		}
	}

	if (NULL != m_activity_list[ACTIVITY_TYPE_GUILD_QUESTION])
	{
		if (ACTIVITY_STATUS_CLOSE != m_activity_list[ACTIVITY_TYPE_GUILD_QUESTION]->GetActivityStatus() && 0 != (oper_limit_mark & (1 << guild_oper_type)))
		{
			gstr::SendError(role, "guild_question_limit");
			return true;
		}
	}

	if (NULL != m_activity_list[ACTIVITY_TYPE_GUILD_FB])
	{
		if (ACTIVITY_STATUS_CLOSE != m_activity_list[ACTIVITY_TYPE_GUILD_FB]->GetActivityStatus() && 0 != (oper_limit_mark & (1 << guild_oper_type)))
		{
			gstr::SendError(role, "guild_fb_limit");
			return true;
		}
	}

	if (!ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_CROSS_BIANJINGZHIDI) && 0 != (oper_limit_mark & (1 << guild_oper_type)))
	{
		gstr::SendError(role, "cross_bjzd_guild_fb_limit");
		return true;
	}

	return false;
}

void ActivityManager::OnQueryRoomStatusReq(Role *role, int activity_type)
{
	if (activity_type <= ACTIVITY_TYPE_INVALID || activity_type >= ACTIVITY_TYPE_MAX) return;

	Activity *activity = m_activity_list[activity_type];
	if (NULL != activity)
	{
		activity->OnQueryRoomStatusReq(role);
	}
}

bool ActivityManager::OnForceToNextState(int activity_type)
{
	if (activity_type <= ACTIVITY_TYPE_INVALID || activity_type >= ACTIVITY_TYPE_MAX) return false;

	if (LOGIC_CONFIG->GetActivitySwitchConfig().IsBanActivity(activity_type)) return false;
	
	if (NULL != m_activity_list[activity_type])
	{
		return m_activity_list[activity_type]->ForceToNextState();
	}

	return false;
}

Activity * ActivityManager::ForceGetActivity(int activity_type)
{
	if (activity_type <= ACTIVITY_TYPE_INVALID || activity_type >= ACTIVITY_TYPE_MAX) return nullptr;

	if (NULL != m_activity_list[activity_type])
	{
		return m_activity_list[activity_type];
	}

	return nullptr;
}