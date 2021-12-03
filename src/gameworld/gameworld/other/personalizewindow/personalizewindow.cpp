#include "personalizewindow.hpp"
#include "config/logicconfigmanager.hpp"
#include "other/personalizewindow/personalizewindowconfig.hpp"
#include "obj/character/role.h"
#include "protocal/msgpersonalizewindow.h"
#include "gamelog.h"
#include "servercommon/errornum.h"
#include "world.h"
#include "gstr.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/noticenum.h"
#include "other/event/eventhandler.hpp"
#include "item/knapsack.h"
#include "other/capability/capability.hpp"
#include "servercommon/commondata.hpp"
#include "servercommon/crossdef.hpp"
#include "global/usercache/usercache.hpp"

PersonalizeWindow::PersonalizeWindow() : m_role(NULL)
{

}

PersonalizeWindow::~PersonalizeWindow()
{

}

void PersonalizeWindow::InitParam(Role *role, const PersonalizeWindowParam &param)
{
	m_role = role;
	m_param = param;
}

void PersonalizeWindow::GetInitParam(PersonalizeWindowParam *param)
{
	*param = m_param;

	m_role->SetUseAvatarType(m_param.cur_use_avatar_type);
	m_role->SetUseBubbleType(m_param.cur_use_bubble_type);
}

void PersonalizeWindow::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		// 气泡框属性
		for(int i = 0; i < BUBBLE_WINDOW_MAX_TYPE; i++)
		{
			if (m_param.bubble_level[i] <= 0)
			{
				continue;
			}

			const BubbleLevelConfig *level_cfg = LOGIC_CONFIG->GetPersonalizeWindowCfg().GetBubbleWindowLevelCfg(i, m_param.bubble_level[i]);
			if (NULL != level_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += level_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += level_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += level_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += level_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += level_cfg->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += level_cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += level_cfg->jianren;
			}
		}

		// 头像框属性
		for (int i = 0; i < AVATAR_WINDOW_MAX_TYPE; i++)
		{
			if (m_param.avatar_level[i] <= 0)
			{
				continue;
			}

			const AvatarLevelConfig *level_cfg = LOGIC_CONFIG->GetPersonalizeWindowCfg().GetAvatarWindowLevelCfg(i, m_param.avatar_level[i]);
			if (NULL != level_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += level_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += level_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += level_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += level_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += level_cfg->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += level_cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += level_cfg->jianren;
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_PERSONALIZE_WINDOW, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

void PersonalizeWindow::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid)
	{
		return;
	}
}

void PersonalizeWindow::SendBubbleWindowInfo()
{
	static Protocol::SCBubbleWindowInfo pwai;
	pwai.cur_use_bubble_type = m_param.cur_use_bubble_type;
	pwai.reserve = 0;
	memcpy(pwai.bubble_level, m_param.bubble_level, sizeof(pwai.bubble_level));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&pwai, sizeof(pwai));
}

bool PersonalizeWindow::OnBubbleWindowUpLevel(short bubble_type)
{
	if (bubble_type < 0 || bubble_type >= BUBBLE_WINDOW_MAX_TYPE)
	{
		return false;
	}

	int max_level_cfg = LOGIC_CONFIG->GetPersonalizeWindowCfg().GetTuhaojinMaxLevel(bubble_type);
	if (m_param.bubble_level[bubble_type] < 0 || m_param.bubble_level[bubble_type] >= max_level_cfg || m_param.bubble_level[bubble_type] >= BUBBLE_WINDOW_MAX_LEVEL)
	{
		m_role->NoticeNum(errornum::EN_PERSONALIZE_BUBBLE_WINDOW_LEVEL_LIMIT);
		return false;
	}

	const BubbleLevelConfig * cur_leve_cfg = LOGIC_CONFIG->GetPersonalizeWindowCfg().GetBubbleWindowLevelCfg(bubble_type, m_param.bubble_level[bubble_type]);
	if (NULL == cur_leve_cfg)
	{
		m_role->NoticeNum(errornum::EN_PERSONALIZE_BUBBLE_WINDOW_LEVEL_LIMIT);
		return false;
	}

	const BubbleLevelConfig * next_leve_cfg = LOGIC_CONFIG->GetPersonalizeWindowCfg().GetBubbleWindowLevelCfg(bubble_type, m_param.bubble_level[bubble_type] + 1);
	if (NULL == next_leve_cfg)
	{
		m_role->NoticeNum(errornum::EN_PERSONALIZE_BUBBLE_WINDOW_LEVEL_LIMIT);
		return false;
	}

	int old_level = m_param.bubble_level[bubble_type];

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	{
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		int item_count = 0;

		if (cur_leve_cfg->common_item.item_id > 0)
		{
			stuff_list[item_count].item_id = cur_leve_cfg->common_item.item_id;
			stuff_list[item_count].num = cur_leve_cfg->common_item.num;
			stuff_list[item_count].buyable = false;
			item_count++;
		}

		if (PROF_TYPE_PROF_1 == m_role->GetBaseProf())
		{
			if (cur_leve_cfg->prof_one_item.item_id > 0)
			{
				stuff_list[item_count].item_id = cur_leve_cfg->prof_one_item.item_id;
				stuff_list[item_count].num = cur_leve_cfg->prof_one_item.num;
				stuff_list[item_count].buyable = false;	
				item_count++;
			}
		}
		else if (PROF_TYPE_PROF_2 == m_role->GetBaseProf())
		{
			if (cur_leve_cfg->prof_two_item.item_id > 0)
			{
				stuff_list[item_count].item_id = cur_leve_cfg->prof_two_item.item_id;
				stuff_list[item_count].num = cur_leve_cfg->prof_two_item.num;
				stuff_list[item_count].buyable = false;
				item_count++;
			}
		}
		else if (PROF_TYPE_PROF_3 == m_role->GetBaseProf())
		{
			if (cur_leve_cfg->prof_three_item.item_id > 0)
			{
				stuff_list[item_count].item_id = cur_leve_cfg->prof_three_item.item_id;
				stuff_list[item_count].num = cur_leve_cfg->prof_three_item.num;
				stuff_list[item_count].buyable = false;
				item_count++;
			}
		}
		else if (PROF_TYPE_PROF_4 == m_role->GetBaseProf())
		{
			if (cur_leve_cfg->prof_four_item.item_id > 0)
			{
				stuff_list[item_count].item_id = cur_leve_cfg->prof_four_item.item_id;
				stuff_list[item_count].num = cur_leve_cfg->prof_four_item.num;
				stuff_list[item_count].buyable = false;
				item_count++;
			}
		}
		else
		{
			return false;
		}

		if (item_count <= 0)
		{
			return false;
		}

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, item_count, stuff_list, &consume_list, true))
		{
			return false;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "PersonalizeWindow::OnBubbleWindowUpLevel"))
	{
		return false;
	}

	// 升级
	++ m_param.bubble_level[bubble_type];

	if (0 == cur_leve_cfg->is_active && 0 != next_leve_cfg->is_active && m_param.cur_use_bubble_type != bubble_type)
	{
		this->SetBubbleTypeWindow(bubble_type);
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_PERSONALIZE_WINDOW);
	this->SendBubbleWindowInfo();

	if (0 != next_leve_cfg->is_notice)
	{
		int length = 0;

		length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_personalize_window_bubble_rim_active_content, 
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), bubble_type);

		if (length > 0) 
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
			m_param.reserve = 1;
		}
	}

	//日志
	gamelog::g_log_personalize_window.printf(LL_INFO, "PersonalizeWindow::BubbleWindow user[%d %s], bubble_type[%d], old_level[%d], cur_level[%d]",
		m_role->GetUID(), m_role->GetName(), bubble_type, old_level, m_param.bubble_level[bubble_type]);

	return true;
}

void PersonalizeWindow::OnUseBubbleWindwo(short bubble_type)
{
	if (bubble_type < -1 || bubble_type >= BUBBLE_WINDOW_MAX_TYPE)
	{
		return;
	}

	if (bubble_type != -1)
	{
		const BubbleLevelConfig * leve_cfg = LOGIC_CONFIG->GetPersonalizeWindowCfg().GetBubbleWindowLevelCfg(bubble_type, m_param.bubble_level[bubble_type]);
		if (NULL == leve_cfg)
		{
			return;
		}

		if (0 == leve_cfg->is_active)
		{
			m_role->NoticeNum(errornum::EN_PERSONALIZE_BUBBLE_WINDOW_NOT_ACTIVE);
			return;
		}

		this->SetBubbleTypeWindow(bubble_type);
		m_role->NoticeNum(noticenum::NT_CHAT_USE_BUBBLE_SUCCESS);
	}
	else
	{
		this->SetBubbleTypeWindow(bubble_type);
		m_role->NoticeNum(noticenum::NT_CHAT_UN_USE_BUBBLE_SUCCESS);
	}

	this->SendBubbleWindowInfo();
}

int PersonalizeWindow::GetCurUseBubbleWindow()
{
	return m_param.cur_use_bubble_type;
}

void PersonalizeWindow::SendAvatarWindowInfo()
{
	static Protocol::SCAvatarWindowInfo pro;
	pro.cur_use_avatar_type = m_param.cur_use_avatar_type;					
	pro.reserve = 0;
	memcpy(pro.avatar_level, m_param.avatar_level, sizeof(pro.avatar_level));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&pro, sizeof(pro));
}

bool PersonalizeWindow::OnAvatarWindowUpLevel(short avatar_type)
{
	if (avatar_type < 0 || avatar_type >= AVATAR_WINDOW_MAX_TYPE)
	{
		return false;
	}

	int max_level_cfg = LOGIC_CONFIG->GetPersonalizeWindowCfg().GetAvatarMaxLevel(avatar_type);
	if (m_param.avatar_level[avatar_type] < 0 || m_param.avatar_level[avatar_type] >= max_level_cfg || m_param.avatar_level[avatar_type] >= AVATAR_WINDOW_MAX_LEVEL)
	{
		gstr::SendError(m_role, "en_personalize_avatar_window_level_limit");
		return false;
	}

	const AvatarLevelConfig * cur_leve_cfg = LOGIC_CONFIG->GetPersonalizeWindowCfg().GetAvatarWindowLevelCfg(avatar_type, m_param.avatar_level[avatar_type]);
	if (NULL == cur_leve_cfg)
	{
		gstr::SendError(m_role, "en_personalize_avatar_window_level_limit");
		return false;
	}

	const AvatarLevelConfig * next_leve_cfg = LOGIC_CONFIG->GetPersonalizeWindowCfg().GetAvatarWindowLevelCfg(avatar_type, m_param.avatar_level[avatar_type] + 1);
	if (NULL == next_leve_cfg)
	{
		gstr::SendError(m_role, "en_personalize_avatar_window_level_limit");
		return false;
	}

	int old_level = m_param.avatar_level[avatar_type];

	static ItemExtern::ItemConsumeList consume_list;
	consume_list.Reset();

	{
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		int item_count = 0;

		if (cur_leve_cfg->common_item.item_id > 0)
		{
			stuff_list[item_count].item_id = cur_leve_cfg->common_item.item_id;
			stuff_list[item_count].num = cur_leve_cfg->common_item.num;
			stuff_list[item_count].buyable = false;
			item_count++;
		}

		if (item_count <= 0)
		{
			return false;
		}

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, item_count, stuff_list, &consume_list, true))
		{
			return false;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "PersonalizeWindow::OnAvatarWindowUpLevel"))
	{
		return false;
	}

	// 升级
	++m_param.avatar_level[avatar_type];

	if (0 == cur_leve_cfg->is_active && 0 != next_leve_cfg->is_active && m_param.cur_use_avatar_type != avatar_type)
	{
		this->SetAvatarTypeWindow(avatar_type);
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_PERSONALIZE_WINDOW);
	this->SendAvatarWindowInfo();

	if (0 != next_leve_cfg->is_notice)
	{
		int length = 0;

		length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "avatar_rim_active_notice",
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), avatar_type);

		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}

	//日志
	gamelog::g_log_personalize_window.printf(LL_INFO, "PersonalizeWindow::avatarWindow user[%d %s], avatar_type[%d], old_level[%d], cur_level[%d]",
		m_role->GetUID(), m_role->GetName(), avatar_type, old_level, m_param.avatar_level[avatar_type]);

	return true;
}

void PersonalizeWindow::OnUseAvatarWindow(short avatar_type)
{
	if (avatar_type < -1 || avatar_type >= AVATAR_WINDOW_MAX_TYPE)
	{
		return;
	}

	if (avatar_type != -1)
	{
		const AvatarLevelConfig * leve_cfg = LOGIC_CONFIG->GetPersonalizeWindowCfg().GetAvatarWindowLevelCfg(avatar_type, m_param.avatar_level[avatar_type]);
		if (NULL == leve_cfg)
		{
			return;
		}

		if (0 == leve_cfg->is_active)
		{
			gstr::SendError(m_role, "en_personalize_avatar_window_not_active");
			return;
		}

		this->SetAvatarTypeWindow(avatar_type);
		gstr::SendNotice(m_role, "nt_chat_use_avatar_success");
	}
	else
	{
		this->SetAvatarTypeWindow(avatar_type);
		gstr::SendNotice(m_role, "nt_chat_un_use_avatar_success");
	}

	this->SendAvatarWindowInfo();
}

void PersonalizeWindow::SetAvatarTypeWindow(short avatar_type)
{
	m_param.cur_use_avatar_type = avatar_type;
	m_role->SetUseAvatarType(m_param.cur_use_avatar_type);
	UserCacheManager::Instance().UpdateAvatarType(m_role->GetUID(), m_param.cur_use_avatar_type);
}

void PersonalizeWindow::SetBubbleTypeWindow(short bubble_type)
{
	m_param.cur_use_bubble_type = bubble_type;
	m_role->SetUseBubbleType(m_param.cur_use_bubble_type);
	UserCacheManager::Instance().UpdateBubbleType(m_role->GetUID(), m_param.cur_use_bubble_type);
}

