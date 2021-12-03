#include "world.h"
#include "engineadapter.h"
#include "gamecommon.h"
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "scene/activityshadow/activityshadow.hpp"

#include "other/event/eventhandler.hpp"
#include "other/route/mailroute.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/achieve/title/titlemanager.hpp"

#include "servercommon/noticenum.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/string/globalstr.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "globalconfig/globalconfig.h"
#include "config/logicconfigmanager.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "servercommon/taskdef.hpp"
#include "config/activityconfig/fairybuddhacardconfig.hpp"

#include "item/itemextern.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"

void  RoleActivity::OnFairyBuddhaCardUpdate(unsigned int now_second)
{
	bool is_time_out = false;
	if(0 != m_activity_info.fairy_buddha_card.bronze_timestamp && now_second >= m_activity_info.fairy_buddha_card.bronze_timestamp)
	{
		m_activity_info.fairy_buddha_card.bronze_timestamp = 0;
		if(0 == (m_activity_info.fairy_buddha_card.is_forever_open & (1 << FAIRY_BUDDHA_CARD_BRONZE)))
		{
			//如果没有永久开启，将其标志位清零
			m_activity_info.fairy_buddha_card.fairy_buddha_card_is_activate &= static_cast<unsigned char>(~(1 << FAIRY_BUDDHA_CARD_BRONZE));
			auto config = LOGIC_CONFIG->GetFairyBuddhaCardConfig().GetFairyBuddhaCardCfg(FAIRY_BUDDHA_CARD_BRONZE);
			if (config)
			{
				m_role->GetTitleManager()->RemoveTitle(config->title_id, true, true, true);
			}
			this->SendFairyBuddhaCardInfo();
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_RAND_ACTIVITY);
			is_time_out = true;
		}
	}

	if(0 != m_activity_info.fairy_buddha_card.silver_timestamp && now_second >= m_activity_info.fairy_buddha_card.silver_timestamp)
	{
		m_activity_info.fairy_buddha_card.silver_timestamp = 0;
		if(0 == (m_activity_info.fairy_buddha_card.is_forever_open & (1 << FAIRY_BUDDHA_CARD_SILVER)))
		{
			m_activity_info.fairy_buddha_card.fairy_buddha_card_is_activate &= static_cast<unsigned char>(~(1 << FAIRY_BUDDHA_CARD_SILVER));
			auto config = LOGIC_CONFIG->GetFairyBuddhaCardConfig().GetFairyBuddhaCardCfg(FAIRY_BUDDHA_CARD_SILVER);
			if (config)
			{
				m_role->GetTitleManager()->RemoveTitle(config->title_id, true, true, true);
			}
			this->SendFairyBuddhaCardInfo();
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_RAND_ACTIVITY);

			is_time_out = true;
		}
	}

	if(0 != m_activity_info.fairy_buddha_card.jewel_timestamp && now_second >= m_activity_info.fairy_buddha_card.jewel_timestamp)
	{
		m_activity_info.fairy_buddha_card.jewel_timestamp = 0; 
		if(0 == (m_activity_info.fairy_buddha_card.is_forever_open & (1 << FAIRY_BUDDHA_CARD_JEWEL)))
		{
			m_activity_info.fairy_buddha_card.fairy_buddha_card_is_activate &= static_cast<unsigned char>(~(1 << FAIRY_BUDDHA_CARD_JEWEL));
			auto config = LOGIC_CONFIG->GetFairyBuddhaCardConfig().GetFairyBuddhaCardCfg(FAIRY_BUDDHA_CARD_JEWEL);
			if (config)
			{
				m_role->GetTitleManager()->RemoveTitle(config->title_id, true, true, true);
			}
			this->SendFairyBuddhaCardInfo();
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_RAND_ACTIVITY);

			is_time_out = true;
		}
	}

	if (is_time_out)
	{
		//CToLuaFun::OnFairyBuddhaChange(UidToInt(m_role->GetUserId()));
	}
}


void RoleActivity::OnFairyBuddhaCardActivateReq(int card_type)
{
	if(card_type <= FAIRY_BUDDHA_CARD_INVALID || card_type >= FAIRY_BUDDHA_CARD_MAX) return;

	if (card_type == FAIRY_BUDDHA_CARD_ALL) return; // 屏蔽一键激活

	if(m_role->GetLevel() < FAIRY_BUDDHA_CARD_START_LEVEL) return;

	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), EngineAdapter::Instance().Time()) + 1;
	if(opengame_day < FAIRY_BUDDHA_CARD_START_DAY) return;

	if(card_type == FAIRY_BUDDHA_CARD_ALL)
	{
		for(int i = FAIRY_BUDDHA_CARD_BRONZE; i < FAIRY_BUDDHA_CARD_ALL; i ++)
		{
			if(0 != (m_activity_info.fairy_buddha_card.is_forever_open & (1 << i)))
			{
				//m_role->NoticeNum(errornum::EN_FAIRY_BUDDHA_CARD_HAVE_FORVER_ACTIVE);
				return;
			}
		}
	}
	else
	{
		if(0 != (m_activity_info.fairy_buddha_card.is_forever_open & (1 << card_type)))
		{
			//m_role->NoticeNum(errornum::EN_FAIRY_BUDDHA_CARD_HAVE_FORVER_ACTIVE);
			return;
		}
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	auto config = LOGIC_CONFIG->GetFairyBuddhaCardConfig().GetFairyBuddhaCardCfg(card_type);
	if (!config)
	{
		return;
	}

	unsigned int* useful_time = nullptr;
	switch (card_type)
	{
	case FAIRY_BUDDHA_CARD_BRONZE:
		useful_time = &m_activity_info.fairy_buddha_card.bronze_timestamp;
		break;
	case FAIRY_BUDDHA_CARD_SILVER:
		useful_time = &m_activity_info.fairy_buddha_card.silver_timestamp;
		break;
	case FAIRY_BUDDHA_CARD_JEWEL:
		useful_time = &m_activity_info.fairy_buddha_card.jewel_timestamp;
		break;
	default:
		return;
	}


	if (*useful_time < now)
	{//激活时间
		if (!m_role->GetKnapsack()->GetMoney()->UseGold(config->use_gold, "fairybuddhacardactivate"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
		*useful_time = now + config->useful_time;
	}
	else
	{//激活永久
		ItemExtern::ItemConsumeConfigList stuff_list;
		stuff_list.add_item(ItemExtern::ItemConsumeConfig(config->stuff_item.item_id, false, config->stuff_item.num));
		if (!stuff_list.done(*m_role, "RoleActivity::OnFairyBuddhaCardActivateReq"))
		{
			return;
		}

		m_activity_info.fairy_buddha_card.is_forever_open |= 1 << card_type;
	}

	m_role->GetTitleManager()->AddTitle(config->title_id, true, true, true, true);

	m_activity_info.fairy_buddha_card.fairy_buddha_card_is_activate |= (1 << card_type);

	this->SendFairyBuddhaCardInfo();
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_RAND_ACTIVITY);
}

void RoleActivity::OnFairyBuddhaCardGetBindGoldReq(int card_type)
{
	if(card_type <= FAIRY_BUDDHA_CARD_INVALID || card_type >= FAIRY_BUDDHA_CARD_ALL) return;

	if(m_role->GetLevel() < FAIRY_BUDDHA_CARD_START_LEVEL) return;

	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), EngineAdapter::Instance().Time()) + 1;
	if (opengame_day < FAIRY_BUDDHA_CARD_START_DAY) return;

	if(!IsFairyBuddhaActive(card_type))
	{
		if (FAIRY_BUDDHA_CARD_BRONZE == card_type)
		{
			//m_role->NoticeNum(errornum::EN_FAIRY_BUDDHA_CARD_NOT_ACTIVE);
		}
		else if (FAIRY_BUDDHA_CARD_SILVER == card_type)
		{
			//m_role->NoticeNum(errornum::EN_FAIRY_BUDDHA_CARD_NOT_ACTIVE_2);
		}
		else if (FAIRY_BUDDHA_CARD_JEWEL == card_type)
		{
			//m_role->NoticeNum(errornum::EN_FAIRY_BUDDHA_CARD_NOT_ACTIVE_3);
		}
		
		return;
	}

	if(0 != (m_activity_info.fairy_buddha_card.gold_bind_is_get_flage & (1 << card_type)))
	{
		//m_role->NoticeNum(errornum::EN_FAIRY_BUDDHA_CARD_HAS_FETCH_REWARD);
		return;
	}

	const FairyBuddhaCardCfg  *card_cfg = LOGIC_CONFIG->GetFairyBuddhaCardConfig().GetFairyBuddhaCardCfg(card_type);
	if(card_cfg == NULL) return;

	switch(card_type)
	{
	case FAIRY_BUDDHA_CARD_BRONZE: m_role->GetKnapsack()->GetMoney()->AddGoldBind(card_cfg->day_gold_bind, "fairybuddhacardreward"); break;
	case FAIRY_BUDDHA_CARD_SILVER: m_role->GetKnapsack()->GetMoney()->AddGoldBind(card_cfg->day_gold_bind, "fairybuddhacardreward"); break;
	case FAIRY_BUDDHA_CARD_JEWEL:  m_role->GetKnapsack()->GetMoney()->AddGoldBind(card_cfg->day_gold_bind, "fairybuddhacardreward"); break;
	}	

	m_activity_info.fairy_buddha_card.gold_bind_is_get_flage |= (1 << card_type);

	this->SendFairyBuddhaCardInfo();
}

void RoleActivity::FairyBuddhaCardOnDayChange()
{
	m_activity_info.fairy_buddha_card.gold_bind_is_get_flage = 0;
	this->SendFairyBuddhaCardInfo();
}

bool RoleActivity::IsFairyBuddhaActive(int card_type)
{
	if(card_type <= FAIRY_BUDDHA_CARD_INVALID || card_type >= FAIRY_BUDDHA_CARD_ALL) return false;

	if(0 != (m_activity_info.fairy_buddha_card.is_forever_open & (1 << card_type)))
	{
		return true;
	}
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	switch (card_type)
	{
	case FAIRY_BUDDHA_CARD_BRONZE:
		return now < m_activity_info.fairy_buddha_card.bronze_timestamp;
	case FAIRY_BUDDHA_CARD_SILVER:
		return now < m_activity_info.fairy_buddha_card.silver_timestamp;
	case FAIRY_BUDDHA_CARD_JEWEL:
		return now < m_activity_info.fairy_buddha_card.jewel_timestamp;
	default:
		break;
	}
	return false;
}

void RoleActivity::SendFairyBuddhaCardInfo()
{
	Protocol::SCFairyBuddhaCardActivateInfo caif;

	caif.bronze_timestamp = m_activity_info.fairy_buddha_card.bronze_timestamp;      
	caif.silver_timestamp = m_activity_info.fairy_buddha_card.silver_timestamp;       
	caif.jewel_timestamp = m_activity_info.fairy_buddha_card.jewel_timestamp;               
	caif.is_forever_open = m_activity_info.fairy_buddha_card.is_forever_open;               
	caif.gold_bind_is_get_flage = m_activity_info.fairy_buddha_card.gold_bind_is_get_flage;
	caif.fairy_buddha_card_is_activate = 0;//废弃此字段
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&caif, sizeof(caif));
}

int RoleActivity::GetFairyBuddhaCardExtraExpPer()
{
	//加经验
	int add_per = 0;
	for(int j = FAIRY_BUDDHA_CARD_BRONZE; j < FAIRY_BUDDHA_CARD_ALL; j ++)
	{
		if(IsFairyBuddhaActive(j))
		{
			const FairyBuddhaCardCfg  *card_cfg = LOGIC_CONFIG->GetFairyBuddhaCardConfig().GetFairyBuddhaCardCfg(j);
			if(NULL != card_cfg)
			{
				add_per += card_cfg->monster_exp;
			}
		}
	}
	return add_per;
}

int RoleActivity::GetFairyBuddhaCardExtraMoJingPer()
{
	//分解魔晶
	int add_per = 0;
	for (int j = FAIRY_BUDDHA_CARD_BRONZE; j < FAIRY_BUDDHA_CARD_ALL; j++)
	{
		if (IsFairyBuddhaActive(j))
		{
			const FairyBuddhaCardCfg  *card_cfg = LOGIC_CONFIG->GetFairyBuddhaCardConfig().GetFairyBuddhaCardCfg(j);
			if (NULL != card_cfg)
			{
				add_per += card_cfg->add_mojing_per;
			}
		}
	}
	return add_per;
}

