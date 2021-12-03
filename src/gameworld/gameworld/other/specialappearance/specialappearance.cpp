#include "specialappearance.hpp"

#include "obj/character/role.h"
#include "scene/scene.h"
#include "engineadapter.h"
#include "other/event/eventhandler.hpp"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/errornum.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "global/worldevent/worldeventmanager.hpp"
#include "gameworld/config/logicconfigmanager.hpp"
#include "other/huashen/huashenconfig.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/huashen/huashen.hpp"
#include "protocal/msgrole.h"
#include "skill/skillbase.hpp"
#include "other/greatesoldier/greatesoldier.hpp"

SpecialAppearance::SpecialAppearance() 
	: m_role(NULL), m_skill_appearance(0), m_skill_appearance_end_time(0), m_fb_appearance(0), m_fb_appearance_param(0), m_fb_appearance_end_time(0),
	m_item_appearance(0), m_item_appearance_param(0), m_item_appearance_end_time(0), m_huashen_appearance(0), m_huashen_appearance_param(0), 
	m_huashen_appearance_end_time(0), m_show_appearance(0), m_show_appearance_param(0),
	m_greate_soldier_appearance(0), m_greate_soldier_appearance_param(0), m_greate_soldier_appearance_end_time(0)
{

}

SpecialAppearance::~SpecialAppearance()
{

}

void SpecialAppearance::Init(Role *role, int skill_appearance, unsigned int skill_appearance_end_time,
	int item_appearance, int item_aaearance_param, unsigned int item_appearance_end_time,
	int huashen_appearance, int huashen_appearance_param, unsigned int huashen_appearance_end_time,
	int greate_soldier_appearance, int greate_soldier_appearance_param, unsigned int greate_soldier_appearance_end_time)
{
	m_role = role;
	m_skill_appearance = (short)skill_appearance;
	m_skill_appearance_end_time = skill_appearance_end_time;
	m_item_appearance = (short)item_appearance;
	m_item_appearance_param = item_aaearance_param;
	m_item_appearance_end_time = item_appearance_end_time;

	m_huashen_appearance = (short)huashen_appearance;
	m_huashen_appearance_param = huashen_appearance_param;
	m_huashen_appearance_end_time = huashen_appearance_end_time;

	m_greate_soldier_appearance = (short)greate_soldier_appearance;
	m_greate_soldier_appearance_param = greate_soldier_appearance_param;
	m_greate_soldier_appearance_end_time = greate_soldier_appearance_end_time;

	this->CalcAppearance();
}

void SpecialAppearance::GetInitParam(int *skill_appearance, unsigned int *skill_appearance_end_time,
	int *item_appearance, int *item_aaearance_param, unsigned int *item_appearance_end_time,
	int *huashen_appearance, int *huashen_appearance_param, unsigned int *huashen_appearance_end_time,
	int *greate_soldier_appearance, int *greate_soldier_appearance_param, unsigned int *greate_soldier_appearance_end_time)
{
	*skill_appearance = m_skill_appearance;
	*skill_appearance_end_time = m_skill_appearance_end_time;
	*item_appearance = m_item_appearance;
	*item_aaearance_param = m_item_appearance_param;
	*item_appearance_end_time = m_item_appearance_end_time;

	*huashen_appearance = m_huashen_appearance;
	*huashen_appearance_param = m_huashen_appearance_param;
	*huashen_appearance_end_time = m_huashen_appearance_end_time;

	*greate_soldier_appearance = m_greate_soldier_appearance;
	*greate_soldier_appearance_param = m_greate_soldier_appearance_param;
	*greate_soldier_appearance_end_time = m_greate_soldier_appearance_end_time;
}

void SpecialAppearance::Update(unsigned long interval, unsigned int now_second)
{
	short old_show_appearance = m_show_appearance;
	int old_show_appearance_param = m_show_appearance_param;

	if (m_skill_appearance_end_time > 0 && now_second >= m_skill_appearance_end_time)
	{
		m_skill_appearance = 0;
		m_skill_appearance_end_time = 0;
		old_show_appearance_param = 0;
		this->CalcAppearance();
	}

	if (m_fb_appearance_end_time > 0 && now_second >= m_fb_appearance_end_time)
	{
		m_fb_appearance = 0;
		m_fb_appearance_param = 0;
		m_fb_appearance_end_time = 0;
		old_show_appearance_param = 0;
		this->CalcAppearance();
	}

	if (m_item_appearance_end_time > 0 && now_second >= m_item_appearance_end_time)
	{
		m_item_appearance = 0;
		m_item_appearance_end_time = 0;
		old_show_appearance_param = 0;
		this->CalcAppearance();
	}

	// 化神形象没有倒计时
	if (!this->InAppearnceCountdown())
	{
		int huashen_id = m_role->GetHuaShen()->GetCurHuaShenID();
		const HuaShenInfoConfig *info_cfg = LOGIC_CONFIG->GetHuaShenCfg().GetHuaShenInfoCfg(huashen_id);
		if (NULL != info_cfg)
		{
			m_huashen_appearance = SPECIAL_APPERANCE_TYPE_CHANGE_HUASHEN_IMAGE;
			m_huashen_appearance_end_time = 0;
			m_huashen_appearance_param = info_cfg->monster_id;

			this->CalcAppearance();
		}
	}

	if (m_shenqi_appearance_end_time > 0 && now_second >= m_shenqi_appearance_end_time)
	{
		m_shenqi_appearance = 0;
		m_shenqi_appearance_end_time = 0;
		old_show_appearance_param = 0;
		this->CalcAppearance();
	}

	if (m_greate_soldier_appearance_end_time > 0 && now_second >= m_greate_soldier_appearance_end_time)
	{
		m_greate_soldier_appearance = 0;
		m_greate_soldier_appearance_end_time = 0;
		old_show_appearance_param = 0;
		GREATE_SOLDIER(m_role)->SetBianshenTrail(0);

		this->CalcAppearance();
	}

	if (old_show_appearance != m_show_appearance || old_show_appearance_param != m_show_appearance_param)
	{
		this->SendAppearance(true);
	}
}

void SpecialAppearance::SetSkillAppearance(short appearance, unsigned int end_time, bool is_broadcast)
{
	//if (appearance < SPECIAL_APPEARANCE_TYPE_NORMAL || appearance >= SPECIAL_APPEARANCE_TYPE_MAX) return;	

	//m_skill_appearance = appearance;
	//m_skill_appearance_end_time = end_time;

	//short old_show_appearance = m_show_appearance;
	//this->CalcAppearance();

	//if (old_show_appearance != m_show_appearance)
	//{
	//	this->SendAppearance(is_broadcast);
	//}

	//EventHandler::Instance().OnSetSpecialAppearance(m_role, appearance);
}

void SpecialAppearance::SetFbAppearance(short appearance, int param, unsigned int end_time, bool is_broadcast)
{
	if (appearance < SPECIAL_APPEARANCE_TYPE_NORMAL || appearance >= SPECIAL_APPEARANCE_TYPE_MAX) return;	

	m_fb_appearance = appearance;
	m_fb_appearance_param = param;
	m_fb_appearance_end_time = end_time;

	short old_show_appearance = m_show_appearance;
	this->CalcAppearance();

	if (old_show_appearance != m_show_appearance)
	{
		this->SendAppearance(is_broadcast);
	}

	EventHandler::Instance().OnSetSpecialAppearance(m_role, appearance);
}

void SpecialAppearance::SetItemAppearance(short appearance, int param, unsigned int end_time, bool is_broadcast)
{
	if (appearance < SPECIAL_APPEARANCE_TYPE_NORMAL || appearance >= SPECIAL_APPEARANCE_TYPE_MAX) return;	

	m_item_appearance = appearance;
	m_item_appearance_param = param;
	m_item_appearance_end_time = end_time;

	short old_show_appearance = m_show_appearance;
	int old_show_appearance_param = m_show_appearance_param;
	this->CalcAppearance();

	if (old_show_appearance != m_show_appearance || old_show_appearance_param != m_show_appearance_param)
	{
		this->SendAppearance(is_broadcast);
	}

	EventHandler::Instance().OnSetSpecialAppearance(m_role, appearance);
}

void SpecialAppearance::SendAppearance(bool is_broadcast)
{
	static Protocol::SCRoleSpecialAppearanceChange rsac;
	rsac.obj_id = m_role->GetId();
	rsac.special_appearance = m_show_appearance;
	rsac.appearance_param = m_show_appearance_param;

	if (is_broadcast)
	{
		if (NULL != m_role->GetScene()) m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void *)&rsac, sizeof(rsac));
	}
	else
	{
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&rsac, sizeof(rsac));
	}
}

bool SpecialAppearance::CanUserSpecialAppearanceCard()
{
 	if (CrossConfig::Instance().IsHiddenServer())
 	{
 		return false;
 	}

	const int scene_type = m_role->GetScene()->GetSceneType();
	if (scene_type == Scene::SCENE_TYPE_FISHING)
	{
		return false;
	}
	
	if (scene_type != Scene::SCENE_TYPE_COMMON_SCENE)
	{
		return false;
	}

	return true;
}

bool SpecialAppearance::OnUseSpecialAppearanceCard(short appearance, int last_time)
{
	if (appearance < SPECIAL_APPEARANCE_TYPE_NORMAL || appearance >= SPECIAL_APPEARANCE_TYPE_MAX) 
	{
		return false;	
	}

	if (!this->CanUserSpecialAppearanceCard())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_USE_SCENE_LIMIT);
		return false;
	}

	if (SPECIAL_APPERANCE_TYPE_WORD_EVENT_YURENCARD == appearance)
	{
		Scene *scene = m_role->GetScene();
		if (NULL != scene)
		{
			const static int WORLD_EVENT_YURENKA_EFFECT_RANGE = 40;
			const static int OBJ_LIST_MAX_LEN = 128;

			static ObjID obj_list[OBJ_LIST_MAX_LEN] = {INVALID_OBJ_ID}; 
			int count = SkillBase::GetSceneObjByArea(scene, m_role->GetPos(), WORLD_EVENT_YURENKA_EFFECT_RANGE, obj_list, OBJ_LIST_MAX_LEN);

			int role_obj_count = 0;
			static ObjID role_obj_list[OBJ_LIST_MAX_LEN];

			for (int obj_idx = 0; obj_idx < count && role_obj_count < OBJ_LIST_MAX_LEN; obj_idx ++)
			{
				Role *role = scene->GetPrincipal(obj_list[obj_idx]);
				if (NULL != role && m_role->GetUID() != role->GetUID())	// 排除自己
				{
					role_obj_list[role_obj_count] = obj_list[obj_idx];
					++ role_obj_count;
				}
			}

			if (role_obj_count > 0)
			{
				int rand_value = RandomNum(role_obj_count);
				Role *role = scene->GetPrincipal(role_obj_list[rand_value]);

				if (NULL != role)
				{
					time_t end_time = EngineAdapter::Instance().Time() + last_time;
					int monster_appearance = WorldEventManager::Instance().RandSpecialAppearanceParam();

					role->GetSpecialAppearance()->SetItemAppearance(appearance, monster_appearance, static_cast<unsigned int>(end_time));

					{
						int length_0 = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_world_event_be_set_yurenka_appearance, m_role->GetName());
						if (length_0 > 0) 
						{
							role->SendPersonalMsg(length_0, gamestring::GAMESTRING_BUF);
						}

						int length_1 = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_world_event_set_yurenka_appearance, role->GetName(), role->GetName());
						if (length_1 > 0) 
						{
							m_role->SendPersonalMsg(length_1, gamestring::GAMESTRING_BUF);
						}
					}

					EventHandler::Instance().OnSetOtherSpecialAppearance(m_role, appearance);

					m_role->GetRoleActivity()->OnUseSpecialAppearanceCard();
					role->GetRoleActivity()->OnPassiveUseSpecialAppearanceCard();

					gamelog::g_log_appearance.printf(LL_INFO, "SpecialAppearance::OnUseSpecialAppearanceCard user %d %s, appearance %d, end_time %d",
						m_role->GetUID(), m_role->GetName(), appearance, (int)end_time);


					return true;
				}
			}
			else
			{
				m_role->NoticeNum(errornum::EN_WORLD_EVENT_USE_FOOL_CARD_LIMIT);
			}
		}
	}

	return false;
}

void SpecialAppearance::SetHuaShenAppearance(short appearance, int param, unsigned int end_time, bool is_broadcast)
{
	if (appearance < SPECIAL_APPEARANCE_TYPE_NORMAL || appearance >= SPECIAL_APPEARANCE_TYPE_MAX) return;	

	m_huashen_appearance = appearance;
	m_huashen_appearance_param = param;
	m_huashen_appearance_end_time = end_time;

	short old_show_appearance = m_show_appearance;
	int old_show_appearance_param = m_show_appearance_param;
	this->CalcAppearance();

	if (old_show_appearance != m_show_appearance || old_show_appearance_param != m_show_appearance_param)
	{
		this->SendAppearance(is_broadcast);
	}

	EventHandler::Instance().OnSetSpecialAppearance(m_role, appearance);
}

void SpecialAppearance::SetShenqiAppearance(int param, unsigned int end_time, bool is_broadcast)
{
	m_shenqi_appearance = SPECIAL_APPEARANCE_TYPE_SHNEQI;
	m_shenqi_appearance_end_time = end_time;
	m_shenqi_appearance_param = param;

	short old_show_appearance = m_show_appearance;
	int old_show_appearance_param = m_show_appearance_param;

	this->CalcAppearance();

	if (old_show_appearance != m_show_appearance || old_show_appearance_param != m_show_appearance_param)
	{
		this->SendAppearance(is_broadcast);
	}
}

void SpecialAppearance::SetGreateSoldierAppearance(int param, unsigned int end_time, bool is_broadcast /*= true*/)
{
	m_greate_soldier_appearance = SPECIAL_APPERANCE_TYPE_GREATE_SOLDIER;
	m_greate_soldier_appearance_param = param;
	m_greate_soldier_appearance_end_time = end_time;

	short old_show_appearance = m_show_appearance;
	int old_show_appearance_param = m_show_appearance_param;
	this->CalcAppearance();

	if (old_show_appearance != m_show_appearance || old_show_appearance_param != m_show_appearance_param)
	{
		this->SendAppearance(is_broadcast);
	}

	EventHandler::Instance().OnSetSpecialAppearance(m_role, SPECIAL_APPERANCE_TYPE_GREATE_SOLDIER);
}

void SpecialAppearance::CalcAppearance()
{
	m_show_appearance = 0;
	m_show_appearance_param = 0;

	if (m_skill_appearance > 0 && m_skill_appearance < SPECIAL_APPEARANCE_TYPE_MAX)
	{
		m_show_appearance = m_skill_appearance;
	}
	else if (m_fb_appearance > 0 && m_fb_appearance < SPECIAL_APPEARANCE_TYPE_MAX)
	{
		m_show_appearance = m_fb_appearance;
		m_show_appearance_param = m_fb_appearance_param;
	}
	else if (m_item_appearance > 0 && m_item_appearance < SPECIAL_APPEARANCE_TYPE_MAX)
	{
		m_show_appearance = m_item_appearance;
		m_show_appearance_param = m_item_appearance_param;
	}
	else if (m_huashen_appearance > 0 && m_huashen_appearance < SPECIAL_APPEARANCE_TYPE_MAX)
	{
		m_show_appearance = m_huashen_appearance;
		m_show_appearance_param = m_huashen_appearance_param;
	}
	else if (m_shenqi_appearance > 0 && m_shenqi_appearance < SPECIAL_APPEARANCE_TYPE_MAX)
	{
		m_show_appearance = m_shenqi_appearance;
		m_show_appearance_param = m_shenqi_appearance_param;
	}
	else if (m_greate_soldier_appearance > 0 && m_greate_soldier_appearance < SPECIAL_APPEARANCE_TYPE_MAX)
	{
		m_show_appearance = m_greate_soldier_appearance;
		m_show_appearance_param = m_greate_soldier_appearance_param;
	}
}

bool SpecialAppearance::InAppearnceCountdown()
{
	if (m_fb_appearance_end_time > 0 || m_skill_appearance_end_time > 0 || m_item_appearance_end_time > 0 )
	{
		return true;
	}

	return false;
}