#include "rolestoryfb.hpp"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "other/vip/vipconfig.hpp"
#include "other/fb/storyfbconfig.hpp"

#include "servercommon/errornum.h"
#include "protocal/msgfb.h"
#include "other/event/eventhandler.hpp"
#include "monster/monsterpool.h"
#include "gameworld/world.h"
#include "servercommon/string/gameworldstr.h"
#include "scene/scene.h"
#include "item/itempool.h"
#include "item/knapsack.h"
#include "other/route/mailroute.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "global/usercache/usercache.hpp"
#include "other/vip/vip.hpp"
#include "other/daycounter/daycounter.hpp"

RoleStoryFB::RoleStoryFB() : m_role(NULL)
{

}

RoleStoryFB::~RoleStoryFB()
{

}

void RoleStoryFB::Init(Role *role, const StoryFBParam &param)
{
	m_role = role;
	m_param = param;
}

void RoleStoryFB::GetInitParam(StoryFBParam *param)
{
	*param = m_param;
}

void RoleStoryFB::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	for (int i = 0; i < FB_STORY_MAX_COUNT; i++)
	{
		m_param.fb_list[i].today_times = 0;
	}

	this->SendInfo();
}

bool RoleStoryFB::CanEnter(int fb_index)
{
	if (fb_index < 0 || fb_index >= FB_STORY_MAX_COUNT)
	{
		return false;
	}

	if (fb_index > 0 && !this->IsPassLevel(fb_index - 1) && 0 == m_param.fb_list[fb_index - 1].today_times)
	{
		m_role->NoticeNum(errornum::EN_PRVE_FB_NOT_COMPLETED);
		return false;
	}

	const StoryFBLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetStoryFBConfig().GetLevelCfg(fb_index);
	if (NULL == level_cfg)
	{
		return false;
	}

	if (m_role->GetLevel() < level_cfg->role_level)
	{
		m_role->NoticeNum(errornum::EN_STORY_FB_FUN_OPEN_LEVEL_LIMIT);
		return false;
	}

	if (m_param.fb_list[fb_index].today_times >= level_cfg->free_times)
	{
		m_role->NoticeNum(errornum::EN_STORY_FB_TIMES_LIMIT);
		return false;
	}

	return true;
}

void RoleStoryFB::OnEnterFB(int fb_index)
{
	if (fb_index < 0 || fb_index >= FB_STORY_MAX_COUNT)
	{
		return;
	}

	m_param.curr_fb_index = fb_index;
	m_param.fb_list[fb_index].today_times++;

	m_role->GetDayCounter()->DayCountSet(DayCounter::DAYCOUNT_ID_FB_STORY, 1);

	this->SendInfo();
}

void RoleStoryFB::AutoFBReqOne(int fb_index)
{
	Scene *scene = m_role->GetScene();
	if (NULL == scene)
	{
		return;
	}

	if (Scene::SCENE_TYPE_STORY_FB == scene->GetSceneType())
	{
		m_role->NoticeNum(errornum::EN_FB_OPERATE_LIMIT);
		return;
	}

	if (fb_index < 0 || fb_index >= FB_STORY_MAX_COUNT)
	{
		return;
	}

	const StoryFBLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetStoryFBConfig().GetLevelCfg(fb_index);
	if (NULL == level_cfg)
	{
		return;
	}

	if (0 == m_param.fb_list[fb_index].is_pass)
	{
		m_role->NoticeNum(errornum::EN_STORY_FB_NO_PASS);
		return;
	}

	if (m_param.fb_list[fb_index].today_times >= level_cfg->free_times)
	{
		const int vip_buy_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role->GetVip()->GetVipLevel(), VAT_FB_STROY_COUNT);
		if (m_param.fb_list[fb_index].today_times >= level_cfg->free_times + vip_buy_times)
		{
			m_role->NoticeNum(errornum::EN_BUY_STROY_FB_TIME_LIMIT);
			return;
		}

		if (level_cfg->reset_gold > 0 && !m_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(level_cfg->reset_gold))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}

	ItemConfigData reward_item_list[MonsterInitParam::MAX_DROP_ITEM_COUNT];
	int index_count = MonsterInitParam::MAX_DROP_ITEM_COUNT;
	long long reward_exp = 0;

	for(int i = 0; i < level_cfg->monster_count && i < StoryFBLevelCfg::MONSTER_COUNT_MAX; i++)
	{
		if (0 != level_cfg->monster_id_list[i])
		{
			MONSTERPOOL->GetMonsterDrop(level_cfg->monster_id_list[i], reward_item_list, &index_count, &reward_exp, NULL, NULL);
		}
	}

	short item_count = 0;
	for (int j = 0; j < index_count && j < MonsterInitParam::MAX_DROP_ITEM_COUNT; j++)
	{
		if (0 == reward_item_list[j].item_id)
		{
			break;
		}
		item_count++;
	}
	
	if (m_param.fb_list[fb_index].today_times >= level_cfg->free_times && !m_role->GetKnapsack()->GetMoney()->UseAllGold(level_cfg->reset_gold, "StoryFBAuto"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_param.fb_list[fb_index].today_times++;

	EventHandler::Instance().OnAutoFbStory(m_role);

	m_role->AddExp(reward_exp, "StoryFBAuto", Role::EXP_ADD_REASON_DEFAULT);

	if (item_count > 0)
	{
		m_role->GetKnapsack()->PutList(item_count, reward_item_list, PUT_REASON_STORY_FB);
	}

	this->SendInfo();
}

void RoleStoryFB::AutoFBReqAll()
{
	Scene *scene = m_role->GetScene();
	if (NULL == scene)
	{
		return;
	}

	if (Scene::SCENE_TYPE_STORY_FB == scene->GetSceneType())
	{
		m_role->NoticeNum(errornum::EN_FB_OPERATE_LIMIT);
		return;
	}

	bool is_auto_succ = false;

	for(int i = 0; i < FB_STORY_MAX_COUNT; i ++)
	{
		const StoryFBLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetStoryFBConfig().GetLevelCfg(i);
		if (NULL == level_cfg)
		{
			break;
		}

		if (0 == m_param.fb_list[i].is_pass)
		{
			continue;
		}

		if (m_param.fb_list[i].today_times >= level_cfg->free_times)
		{
			continue;
		}

		ItemConfigData reward_item_list[MonsterInitParam::MAX_DROP_ITEM_COUNT];
		int index_count = MonsterInitParam::MAX_DROP_ITEM_COUNT;
		long long reward_exp = 0;

		for(int j = 0; j < level_cfg->monster_count && j < StoryFBLevelCfg::MONSTER_COUNT_MAX; j++)
		{
			if (0 != level_cfg->monster_id_list[j])
			{
				MONSTERPOOL->GetMonsterDrop(level_cfg->monster_id_list[j], reward_item_list, &index_count, &reward_exp, NULL, NULL);
			}
		}

		short item_count = 0;
		for(int k = 0; k < index_count && k < MonsterInitParam::MAX_DROP_ITEM_COUNT; k++)
		{
			if (0 == reward_item_list[i].item_id)
			{
				break;
			}
			item_count++;
		}

		is_auto_succ = true;
		m_param.fb_list[i].today_times++;

		m_role->AddExp(reward_exp, "StoryFBAuto", Role::EXP_ADD_REASON_DEFAULT);

		if (index_count > 0)
		{
			m_role->GetKnapsack()->PutList(item_count, reward_item_list , PUT_REASON_STORY_FB);
		}

		EventHandler::Instance().OnAutoFbStory(m_role);
	}

	if (is_auto_succ)
	{
		m_role->GetDayCounter()->DayCountSet(DayCounter::DAYCOUNT_ID_FB_STORY, 1);
		this->SendInfo();
	}
	else
	{
		m_role->NoticeNum(errornum::EN_STORY_FB_NOT_LEVEL);
	}
}

void RoleStoryFB::SendInfo()
{
	static Protocol::SCStoryFBInfo cmd;
	for (int i = 0; i < FB_STORY_MAX_COUNT; i++)
	{
		cmd.info_list[i].is_pass = m_param.fb_list[i].is_pass;
		cmd.info_list[i].today_times = m_param.fb_list[i].today_times;
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}

void RoleStoryFB::OnPassLevel(int fb_index)
{
	if (fb_index < 0 || fb_index >= FB_STORY_MAX_COUNT)
	{
		return;
	}

	const StoryFBLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetStoryFBConfig().GetLevelCfg(fb_index);
	if (NULL == level_cfg)
	{
		return;
	}

	if (0 == m_param.fb_list[fb_index].is_pass)
	{
		m_param.fb_list[fb_index].is_pass = 1;
		m_role->GetKnapsack()->PutListOrMail(level_cfg->first_reward_list, PUT_REASON_STORY_FB);
	}
	else
	{
		m_role->GetKnapsack()->PutListOrMail(level_cfg->normal_reward_list, PUT_REASON_STORY_FB);
	}
	m_role->AddExp(level_cfg->reward_exp, "StoryFBPass", Role::EXP_ADD_REASON_DEFAULT);

	this->SendInfo();
}

bool RoleStoryFB::IsPassLevel(int fb_index)
{
	if (fb_index < 0 || fb_index >= FB_STORY_MAX_COUNT)
	{
		return false;
	}

	return 0 != m_param.fb_list[fb_index].is_pass;
}
