#include "specialguildquestion.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "obj/character/role.h"
#include "obj/otherobj/gatherobj.h"
#include "servercommon/activitydef.hpp"
#include "config/logicconfigmanager.hpp"
#include "servercommon/string/gameworldstr.h"
#include "scene/scenemanager.h"
#include "other/event/eventhandler.hpp"
#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"
#include "protocal/msgchatmsg.h"
#include "config/activityconfig/guildquestionconfig.hpp"


SpecialGuildQuestion::SpecialGuildQuestion(Scene *scene)
	: SpecialLogic(scene), m_is_init(false),  m_is_open(true), m_is_first(false)
{
}

SpecialGuildQuestion::~SpecialGuildQuestion()
{
}

void SpecialGuildQuestion::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (!m_is_open)
	{
		this->DelayKickOutAllRole();
	}
}

void SpecialGuildQuestion::OnRoleEnterScene(Role *role)
{
	if (NULL == role || !m_is_open)
	{
		return;
	}

	if (!m_is_first && ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILD_QUESTION))
	{
		m_is_first = true;

		this->RefreshGatherItem();
	}

	ActivityShadow::Instance().OnJoinGuildQuestionActivity(role);
}

void SpecialGuildQuestion::OnRoleLeaveScene(Role *role, bool is_logout)
{

}

void SpecialGuildQuestion::NotifySystemMsg(char *str_buff, int str_len)
{

}

void SpecialGuildQuestion::OnActivityStart()
{
	if (!m_is_first)
	{
		m_is_first = true;

		this->RefreshGatherItem();
	}
}

void SpecialGuildQuestion::OnActivityClose()
{
	m_is_open = false;
}

void SpecialGuildQuestion::RefreshGatherItem()
{
	const GuildQuestionOtherConfig &other_cfg = LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg();
	int pos_count = LOGIC_CONFIG->GetGuildQuestionConfig().GetGatherPosCount();

	for (int i = 0; i < pos_count; i ++)
	{
		Posi gather_pos(0, 0);
		if (!LOGIC_CONFIG->GetGuildQuestionConfig().GetGatherPos(i, &gather_pos))
		{
			continue;
		}

		if (m_scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, gather_pos.x, gather_pos.y))
		{
			GatherObj *gather_obj = new GatherObj();
			gather_obj->SetPos(gather_pos);
			gather_obj->Init(NULL, other_cfg.gather_id, other_cfg.gather_time_s * 1000, 0, false);

			m_scene->AddObj(gather_obj);
		}
	}
}

bool SpecialGuildQuestion::SpecialCanGather(Role *role, GatherObj *gather)
{
	if (NULL == role || NULL == gather)
	{
		return false;
	}

	if (gather->GatherId() != LOGIC_CONFIG->GetGuildQuestionConfig().GetOtherCfg().gather_id)
	{
		return false;
	}

	if (!ActivityShadow::Instance().IsCanGatherGuildQuestion(role))
	{
		return false;
	}

	return true;
}
