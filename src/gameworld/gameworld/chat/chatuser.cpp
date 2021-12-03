#include <stdlib.h>
#include "chatuser.h"

#include "engineadapter.h"
#include "protocal/msgchatctrl.h"

#include "servercommon/servercommon.h"
#include "servercommon/guilddef.hpp"
#include "obj/character/role.h"

#include "config/sharedconfig/sharedconfig.h"
#include "chat/chatmanager.h"
#include "global/worldstatus/worldstatus.hpp"
#include "globalconfig/globalconfig.h"

#include "gameworld/config/logicconfigmanager.hpp"
#include "gameworld/config/agentadapterconfig.hpp"

ChatUser::ChatUser(ChatManager *chat_manager, int uid, GSNetID netid)
	: m_chat_manager(chat_manager), m_uid(uid), m_netid(netid), m_world_msg_last_length(0), m_world_msg_same_length_accumulate_count(0), 
	m_world_msg_first_same_length_timestamp(0), m_ignore_world_talk(false)
{
	memset(next_active_list, 0, sizeof(next_active_list));
}

ChatUser::~ChatUser()
{
	std::vector<UserChannelRef> temp_chanel_vec(m_channel_vec);

	for (std::vector<UserChannelRef>::iterator iter = temp_chanel_vec.begin(); iter != temp_chanel_vec.end(); ++iter)
	{
		m_chat_manager->RemoveFromChannel(iter->channel_id, m_uid);
	}
}

void ChatUser::AddChannel(ChannelID change_id)
{
	UserChannelRef ucr;
	ucr.channel_id = change_id;
	ucr.next_active_time = 0;

	m_channel_vec.push_back(ucr);
}

bool ChatUser::DeleteChannel(ChannelID cid)
{
	for (std::vector<UserChannelRef>::iterator iter = m_channel_vec.begin(); iter != m_channel_vec.end(); ++iter)
	{
		if (iter->channel_id == cid)
		{
			m_channel_vec.erase(iter);
			return true;
		}
	}

	return false;
}

bool ChatUser::IgnoreWorldTalk()
{
	if (GLOBALCONFIG->GetWorldTalkIsLimit())
	{
		return m_ignore_world_talk;
	}

	return false;
}

bool ChatUser::IsForbidChannel(Role *role)
{
	if (NULL == role)
	{
		return true;
	}

	return role->IsForbidTalk();
}

bool ChatUser::ChannelCanChat(Role *role, int channel_type)
{
	if (NULL == role)
	{
		return false;
	}

	if (channel_type < chatdef::CHANNEL_TYPE_WORLD || channel_type >= chatdef::CHANNEL_TYPE_MAX)
	{
		return false;
	}

	if (this->IsForbidChannel(role)) 
	{
		return false;
	}

// 	int channel_open_level = this->GetChannelOpenLevel(channel_type);
// 	if (!role->IsIgnoreChatOrMailLevelLimit() && role->GetLevel() < channel_open_level)
// 	{
// 		return false;
// 	}

	// 等级、vip等级检查
	if (!WorldStatus::Instance().CheckSatisfyLevel(role, channel_type))
	{
		return false;
	}

	unsigned int now_sec = (unsigned int)(EngineAdapter::Instance().Time());

	if (now_sec < next_active_list[channel_type])
	{
		return false;
	}

	int cooldown = this->GetCooldown(channel_type);
	next_active_list[channel_type] = now_sec + cooldown;

	return true;
}

int ChatUser::GetChannelOpenLevel(int channel_type)
{
	int channel_open_level = 0;

	const ChannelChatLimitCfg *channel_chat_limit_cfg = LOGIC_CONFIG->GetAgentAdapterConfig().GetChannelChatLimitCfg(channel_type);
	if (nullptr != channel_chat_limit_cfg)
	{
		channel_open_level = channel_chat_limit_cfg->level_limit;
	}

	// 如果手动设置过则用设置的值
	if (WorldStatus::Instance().GetChannelLevelLimit(channel_type) > 0)
	{
		channel_open_level = WorldStatus::Instance().GetChannelLevelLimit(channel_type);
	}

	return channel_open_level;
}

int ChatUser::GetChannelOpenVipLevel(int channel_type)
{
	int channel_open_vip_level = 0;

	switch (channel_type)
	{
	case chatdef::CHANNEL_TYPE_WORLD:
		channel_open_vip_level = WorldStatus::Instance().GetChannelVipLevelLimit(OPENLEVEL_LIMIT_TYPE_WORLD);
		break;

	case chatdef::CHANNEL_TYPE_CAMP:
		channel_open_vip_level = WorldStatus::Instance().GetChannelVipLevelLimit(OPENLEVEL_LIMIT_TYPE_CAMP);
		break;

	case chatdef::CHANNEL_TYPE_SCENE:
		channel_open_vip_level = WorldStatus::Instance().GetChannelVipLevelLimit(OPENLEVEL_LIMIT_TYPE_SCENE);
		break;

	case chatdef::CHANNEL_TYPE_TEAM:
		channel_open_vip_level = WorldStatus::Instance().GetChannelVipLevelLimit(OPENLEVEL_LIMIT_TYPE_TEAM);
		break;

	case chatdef::CHANNEL_TYPE_GUILD:
		channel_open_vip_level = WorldStatus::Instance().GetChannelVipLevelLimit(OPENLEVEL_LIMIT_TYPE_GUILD);
		break;
	}

	// 如果没有设置过就取默认配置
	const ChannelChatLimitCfg *channel_chat_limit_cfg = LOGIC_CONFIG->GetAgentAdapterConfig().GetChannelChatLimitCfg(channel_type);
	if (channel_open_vip_level <= 0 && nullptr != channel_chat_limit_cfg)
	{
		channel_open_vip_level = channel_chat_limit_cfg->vip_level_limit;
	}

	return channel_open_vip_level;
}

int ChatUser::GetSingleChatOpenVipLevel()
{
	int vip_level_limit = 0;

	const ChannelChatLimitCfg *channel_chat_limit_cfg = LOGIC_CONFIG->GetAgentAdapterConfig().GetChannelChatLimitCfg(OPENLEVEL_LIMIT_TYPE_SINGLE);
	if (nullptr != channel_chat_limit_cfg)
	{
		vip_level_limit = channel_chat_limit_cfg->vip_level_limit;
	}

	// 如果手动设置过则用设置的值
	if (WorldStatus::Instance().GetChannelVipLevelLimit(OPENLEVEL_LIMIT_TYPE_SINGLE) > 0)
	{
		vip_level_limit = WorldStatus::Instance().GetChannelVipLevelLimit(OPENLEVEL_LIMIT_TYPE_SINGLE);
	}

	return vip_level_limit;
}

int ChatUser::FindSingleChatMonitorMinTimeIndex()
{
	int min_index = 0;
	for (int i = 1; i < MAX_SINGLECHAT_MONITOR_COUNT; i++)
	{
		if (m_single_chat_monitor[i].last_single_chat_time < m_single_chat_monitor[min_index].last_single_chat_time)
		{
			min_index = i;
		}
	}

	return min_index;
}

void ChatUser::OnSingleChat(Protocol::CSSingleChat *sc_req)
{
}

void ChatUser::OnGuildChat(Protocol::CSChannelChat *cc_req)
{
}

void ChatUser::OnWorldChat(Protocol::CSChannelChat *cc_req)
{
	if (NULL == cc_req) return;

	if (GLOBALCONFIG->GetWorldTalkIsLimit() && !m_ignore_world_talk)
	{
		if (cc_req->msg_length >= GLOBALCONFIG->GetWorldTalkEffectMsgMinLen())
		{
			time_t now = EngineAdapter::Instance().Time();

			if ((now >= m_world_msg_first_same_length_timestamp + GLOBALCONFIG->GetWorldTalkAccumulateCountResetInterval()) ||
				cc_req->msg_length != m_world_msg_last_length)
			{
				m_world_msg_first_same_length_timestamp = now;
				m_world_msg_last_length = cc_req->msg_length;
				m_world_msg_same_length_accumulate_count = 1;

			}
			else
			{
				++ m_world_msg_same_length_accumulate_count;

				int torelate_count = GLOBALCONFIG->GetWorldTalkTorelateSameMaxCount();
				if (m_world_msg_same_length_accumulate_count >= torelate_count)
				{
					m_ignore_world_talk = true;
				}
			}
		}
	}
}

int ChatUser::GetCooldown(int channal_type)
{
	int cd_s = 0;	//CHANNEL_CD_TIME_WORLD;

	switch (channal_type)
	{
	case chatdef::CHANNEL_TYPE_WORLD:
		cd_s = CHANNEL_CD_TIME_WORLD;
		break;

	case chatdef::CHANNEL_TYPE_CAMP:
		cd_s = CHANNEL_CD_TIME_CAMP;
		break;

	case chatdef::CHANNEL_TYPE_SCENE:
		cd_s = CHANNEL_CD_TIME_SCENE;
		break;

	case chatdef::CHANNEL_TYPE_TEAM:
		cd_s = 0;
		break;

	case chatdef::CHANNEL_TYPE_GUILD:
		cd_s = 0;
		break;

	case chatdef::CHANNEL_TYPE_WORLD_QUESTION:
		cd_s = 0;
		break;

	case chatdef::CHANNEL_TYPE_GUILD_QUESTION:
		cd_s = 0;
		break;
	}

	return cd_s;
}

