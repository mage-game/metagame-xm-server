#include "guildxianshu.hpp"
#include "gameworld/protocal/msgguild.hpp"
#include "guildxianshuconfig.hpp"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "config/logicconfigmanager.hpp"
#include "scene/guildshadow/guildshadow.hpp"
#include "other/event/eventhandler.hpp"
#include "gameworld/global/guild/guild.hpp"
#include "gameworld/global/guild/guildmanager.hpp"
#include "servercommon/string/gameworldstr.h"
#include "gameworld/gameworld/world.h"

GuildXianshu::GuildXianshu() : m_role(NULL)
{

}


GuildXianshu::~GuildXianshu() 
{

}

void GuildXianshu::Init(Role *role, GuildXianshuParam xianshu_param)
{
	m_role = role;
	m_xianshu_param = xianshu_param;
}

void GuildXianshu::GetInitParam(GuildXianshuParam *xianshu_param)
{
	*xianshu_param = m_xianshu_param;
}

void GuildXianshu::UplevelXianshu()
{
// 	if (INVALID_GUILD_ID == m_role->GetGuildID())
// 	{
// 		return;
// 	}
// 
// 	const GuildXianshuConfig::UplevelCfg *next_level_cfg = LOGIC_CONFIG->GetGuildXianshuConfig().GetXianshuUplevelCfg(m_xianshu_param.xianshu_level + 1);
// 	if (NULL == next_level_cfg)
// 	{
// 		m_role->NoticeNum(errornum::EN_GUILD_XIANSHU_MAX_LEVEL);
// 		return;
// 	}
// 
// 	const GuildXianshuConfig::UplevelCfg *cur_level_cfg = LOGIC_CONFIG->GetGuildXianshuConfig().GetXianshuUplevelCfg(m_xianshu_param.xianshu_level);
// 	if (NULL == cur_level_cfg || cur_level_cfg->need_gongxian <= 0)
// 	{
// 		return;
// 	}
// 
// 	if (m_role->GetGuildGongxian() < cur_level_cfg->need_gongxian)
// 	{
// 		return;
// 	}
// 
// 	// 仙盟等级限制
// 	if (GuildShadow::Instance().GetGuildLevel(m_role->GetGuildID()) < cur_level_cfg->need_guild_level)
// 	{
// 		return;
// 	}
// 
// 	m_role->AddGuildGongxian(-cur_level_cfg->need_gongxian, "UplevelXianshu");
// 
// 	m_xianshu_param.xianshu_level += 1;
// 
// 	EventHandler::Instance().OnGuildUpleveXianshu(m_role, m_xianshu_param.xianshu_level);
// 
// 	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_GUILD_XIANSHU);
// 
// 	this->SendXianshuInfo();
// 
// 	if (next_level_cfg->longhui_change > cur_level_cfg->longhui_change)
// 	{
// 		Guild *guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
// 		if (NULL == guild)
// 		{
// 			return;
// 		}
// 
// 		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_xianshu_upgrade,
// 			guild->GetGuildName(), m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), m_xianshu_param.xianshu_level - 1, m_xianshu_param.xianshu_level);
// 
// 		if (length > 0)
// 		{
// 			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER);
// 		}
// 	}
}

void GuildXianshu::SendXianshuInfo()
{
// 	Protocol::SCGuildXianshuInfo xsi;
// 	xsi.xianshu_level = m_xianshu_param.xianshu_level;
// 	xsi.reserve = 0;
// 	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&xsi, sizeof(xsi));
}

void GuildXianshu::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
// 	if (is_recalc)
// 	{
// 		m_attr_temp.Reset();
// 
// 		LOGIC_CONFIG->GetGuildXianshuConfig().GetXianshuAttr(m_xianshu_param.xianshu_level, m_attr_temp);
// 
// 		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_XIANSHU, m_attr_temp);
// 	}
// 
// 	base_add.Add(m_attr_temp.m_attrs);
}

