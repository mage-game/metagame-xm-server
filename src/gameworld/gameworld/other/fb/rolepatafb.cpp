#include "rolepatafb.hpp"
#include "engineadapter.h"
#include "protocal/msgother.h"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "other/fb/patafbconfig.hpp"
#include "scene/scene.h"
#include "servercommon/errornum.h"
#include "protocal/msgfb.h"
#include "global/worldstatus/worldstatus.hpp"
#include "global/usercache/usercache.hpp"
#include "other/event/eventhandler.hpp"
#include "item/knapsack.h"
#include "other/daycounter/daycounter.hpp"
#include "other/achieve/title/titlemanager.hpp"
#include "other/mojie/mojieskill.hpp"
#include "item/itempool.h"
#include "gamecommon.h"
#include "scene/speciallogic/personspecial/specialpatafb.hpp"
#include "world.h"

RolePataFB::RolePataFB() 
	: m_role(NULL), m_pass_level(0), m_today_level(0)
{
}

RolePataFB::~RolePataFB()
{
}

void RolePataFB::Init(Role *role, short pass_level, short today_level)
{
	m_role = role;

	m_pass_level = pass_level;
	m_today_level = today_level;
}

void RolePataFB::GetInitParam(short *pass_level, short *today_level)
{
	*pass_level = m_pass_level;
	*today_level = m_today_level;
}

void RolePataFB::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	/*if (0 != m_today_level)
	{
		m_today_level = 0;
		this->SendAllInfo();
	}*/
}

void  RolePataFB::OnEnterScene(int scene_id)
{
	if (nullptr != m_role->GetMojieSkill())
	{
		m_role->GetMojieSkill()->InitSkillTypeMaxSkillId();
	}
}

void RolePataFB::OnRoleLogin()
{
	
}

bool RolePataFB::CanEnter(int level)
{
	if (level <= m_today_level || level > m_pass_level + 1)
	{
		return false;
	}

	const PataLevelConfig *level_cfg = LOGIC_CONFIG->GetPataFbCfg().GetLevelCfgByLevel(level);
	if (NULL == level_cfg)
	{
		return false;
	}

	return true;
}

void RolePataFB::OnFinish(int level)
{
	const PataLevelConfig *level_cfg = LOGIC_CONFIG->GetPataFbCfg().GetLevelCfgByLevel(level);
	if (NULL == level_cfg)
	{
		return;
	}

	m_today_level = level;

	int old_pass_level = m_pass_level;
	if (m_pass_level < level)
	{
		m_pass_level = level;
		m_role->GetMojieSkill()->CheckActiveSkill(m_pass_level);
		m_role->GetKnapsack()->PutListOrMail(level_cfg->first_reward_list, PUT_REASON_PATAFB_PASS_REWARD);
	}

	m_role->AddExp(level_cfg->reward_exp, "PataFB", Role::EXP_ADD_REASON_DEFAULT);
	m_role->GetKnapsack()->PutListOrMail(level_cfg->normal_reward_list, PUT_REASON_PATAFB_PASS_REWARD);

	m_role->GetDayCounter()->DayCountSet(DayCounter::DAYCOUNT_ID_FB_PATA, 1);
	//m_role->GetTitleManager()->RecalPataFbTitle();

	this->SendAllInfo();

	EventHandler::Instance().OnPataFBPassLevel(m_role, old_pass_level, level);
	gamelog::g_log_fb.printf(LL_INFO, "RolePataFB::OnFinish user[%d, %s] role_level[%d], pass_level[%d]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), level);
}

void RolePataFB::AutoFBReq()
{
	Scene *scene = m_role->GetScene();
	if (NULL == scene)
	{
		return;
	}

	if (Scene::SCENE_TYPE_PATA_FB == scene->GetSceneType())
	{
		m_role->NoticeNum(errornum::EN_FB_OPERATE_LIMIT);
		return;
	}

	if (m_today_level >= m_pass_level)
	{
		return;
	}

	long long reward_exp = 0;
	int reward_count = 0;
	ItemConfigData reward_list[ROLE_PATA_FB_MAX_LEVEL * MAX_ATTACHMENT_ITEM_NUM];
	for (int i = m_today_level + 1; i <= m_pass_level && i < ROLE_PATA_FB_MAX_LEVEL && reward_count < (ROLE_PATA_FB_MAX_LEVEL * MAX_ATTACHMENT_ITEM_NUM); i++)
	{
		const PataLevelConfig *level_cfg = LOGIC_CONFIG->GetPataFbCfg().GetLevelCfgByLevel(i);
		if (NULL == level_cfg)
		{
			continue;;
		}

		reward_exp += level_cfg->reward_exp;

		for (int count = 0; count < MAX_ATTACHMENT_ITEM_NUM; ++count)
		{
			if (level_cfg->normal_reward_list[count].item_id <= 0 || NULL == ITEMPOOL->GetItem(level_cfg->normal_reward_list[count].item_id))
			{
				continue;
			}

			reward_list[reward_count] = level_cfg->normal_reward_list[count];
			++reward_count;

			if (reward_count >= ROLE_PATA_FB_MAX_LEVEL * MAX_ATTACHMENT_ITEM_NUM)
			{
				break;
			}
		}

		//m_role->GetKnapsack()->PutListOrMail(level_cfg->normal_reward_list, PUT_REASON_PATAFB_PASS_REWARD);

		m_role->GetDayCounter()->DayCountSet(DayCounter::DAYCOUNT_ID_FB_PATA, 1);

		EventHandler::Instance().OnPassPataFB(m_role);
	}

	if (reward_count > 0)
	{
		gamecommon::SendMultiMailToRole(m_role->GetUserId(), reward_list, reward_count, NULL, false, "patafb_auto_reward");
	}

	m_today_level = m_pass_level;
	m_role->AddExp(reward_exp, "AutoPataFB", Role::EXP_ADD_REASON_DEFAULT);

	this->SendAllInfo();

	gamelog::g_log_fb.printf(LL_INFO, "RolePataFB::AutoFBReq user[%d, %s] role_level[%d], pass_level[%d]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), m_today_level);
}

void RolePataFB::SendAllInfo()
{
	static Protocol::SCPataFbAllInfo cmd;
	cmd.pass_level = m_pass_level;
	cmd.today_level = m_today_level;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}
