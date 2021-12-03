#include "channel.h"
#include "chatmanager.h"
#include "global/team/team.hpp"
#include "protocal/msgchatmsg.h"
#include "gamelog.h"
#include "world.h"
#include "gstr.h"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "servercommon/userprotocal/chatmsgcode.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "servercommon/errornum.h"
#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "servercommon/serverconfig/localconfig.hpp"
#include "chat/chatuser.h"
#include "global/usercache/usercache.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "globalconfig/globalconfig.h"
#include "servercommon/commondata.hpp"
#include "internalcomm.h"
#include "other/event/eventhandler.hpp"
#include "friend/teaminfo.hpp"
#include "other/vip/vip.hpp"
#include "other/cross/rolecross.h"
#include "other/tuhaojin/tuhaojin.hpp"
#include "other/bigchatface/bigchatface.hpp"
#include "other/personalizewindow/personalizewindow.hpp"
#include "gameworld/config/logicconfigmanager.hpp"
#include "gameworld/config/agentadapterconfig.hpp"
#include "gameworld/gameworld/friend/friendmanager.h"
#include "servercommon/userprotocal/systemmsgcode.h"
#include "other/xianzunka/xianzunka.hpp"

static const int MSX_GSNETID_NUM = 1024 * 16;
static GSNetID gsnetidlist[MSX_GSNETID_NUM];

ChatManager::ChatManager()
{

}

ChatManager::~ChatManager()
{
	for (ChannelListIt it = m_channel_list.begin(); it != m_channel_list.end(); ++it)
	{
		delete it->second;
	}

	m_channel_list.clear();

	for (ChatUserListIt it = m_chat_user_list.begin(); it != m_chat_user_list.end(); ++it)
	{
		delete it->second;
	}

	m_chat_user_list.clear();
}

ChatManager & ChatManager::Instance()
{
	static ChatManager umm;
	return umm;
}

void ChatManager::OnUserLogin(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	this->CreateChatUser(role->GetUID(), role->GetNetId());

	if (CAMP_TYPE_INVALID != role->GetCamp())
	{
		int cid = chatdef::CHANNEL_ID_CAMP_BEGIN + role->GetCamp();
		this->AddToChannel(cid, role->GetUID());
	}

	long long unique_userid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	this->CheckChangeRoleOnline(unique_userid);
	
	// 这个时候发客端准备工作没做好，改变由客户端上线请求信息下时再发
	//this->CheckSingleChatBoard(role);
	//this->CheckGuildChatBoard(role);
	Protocol::SCForbidUserInfo fui;
	fui.forbid_talk_end_timestamp = static_cast<unsigned int>(role->GetForbidTalkTime());
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)& fui, sizeof(fui));
}

void ChatManager::OnUserLogout(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	long long unique_userid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());

	ChatUserListIt it = m_chat_user_list.find(role->GetUID());
	if (it == m_chat_user_list.end())
	{
		return;
	}

	delete it->second;
	m_chat_user_list.erase(it);

	this->CheckChangeRoleOnline(unique_userid);
}

void ChatManager::OnChannelAddUser(int channel_id, int uid)
{
	ChatUser *chat_user = this->GetChatUser(uid);
	if (NULL != chat_user)
	{
		chat_user->AddChannel(channel_id);
	}
}

void ChatManager::OnChannelRemoveUser(int channel_id, int uid)
{
	ChatUser *chat_user = this->GetChatUser(uid);
	if (NULL != chat_user)
	{
		chat_user->DeleteChannel(channel_id);
	}
}

ChatUser * ChatManager::GetChatUser(int uid)
{
	ChatUserListIt it = m_chat_user_list.find(uid);
	if (it == m_chat_user_list.end()) return NULL; 
	return it->second;
}

void ChatManager::OnChatForbidRole(int forbid_uid)
{
	Role *forbid_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(forbid_uid));
	if (forbid_role != nullptr)
	{
		Protocol::SCForbidUserInfo fui;

		fui.forbid_talk_end_timestamp = static_cast<unsigned int>(forbid_role->GetForbidTalkTime());

		EngineAdapter::Instance().NetSend(forbid_role->GetNetId(), (const char *)&fui, sizeof(fui));
	}


	// 删除私聊记录
	{
		for (MsgBoardMapIt it = m_single_chat_board_map.begin(); it != m_single_chat_board_map.end(); ++it)
		{
			for (MsgBoardItemQueueIt msg_it = it->second.begin(); msg_it != it->second.end();)
			{
				Protocol::SCSingleChat *sc = (Protocol::SCSingleChat *)&(*msg_it);

				if (sc->from_uid == forbid_uid)
				{
					msg_it = it->second.erase(msg_it);
				}
				else
				{
					++msg_it;
				}
			}
		}
	}

	// 删除仙盟聊天记录
	{
		for (MsgBoardMapIt it = m_guild_chat_board_map.begin(); it != m_guild_chat_board_map.end(); ++it)
		{
			for (MsgBoardItemQueueIt msg_it = it->second.begin(); msg_it != it->second.end();)
			{
				Protocol::SCChannelChat *cc = (Protocol::SCChannelChat *)&(*msg_it);

				if (cc->from_uid == forbid_uid)
				{
					msg_it = it->second.erase(msg_it);
				}
				else
				{
					++msg_it;
				}
			}
		}
	}

	// 删除世界聊天记录
	{
		for (MsgBoardItemQueueIt msg_it = m_world_board_list.begin(); msg_it != m_world_board_list.end();)
		{
			Protocol::MessageHeader *header = (Protocol::MessageHeader *)&(*msg_it);
			if (header->msg_type == Protocol::MT_CHANNEL_CHAT_SC)
			{
				Protocol::SCChannelChat *cc = (Protocol::SCChannelChat *)&(*msg_it);

				if (cc->from_uid == forbid_uid)
				{
					msg_it = m_world_board_list.erase(msg_it);
				}
				else
				{
					++msg_it;
				}
			}
			else if (header->msg_type == Protocol::MT_SYSTEM_SPEAKER_SC)
			{
				Protocol::SCSpeaker *speaker = (Protocol::SCSpeaker *)&(*msg_it);

				if (speaker->from_uid == forbid_uid)
				{
					msg_it = m_world_board_list.erase(msg_it);
				}
				else
				{
					++msg_it;
				}
			}
			else
			{
				++msg_it;
			}
		}
	}

	// 通知客户端删除记录
	{
		Protocol::SCForbidChatInfo fci;

		fci.forbid_uid_count = 0;
		fci.forbid_uid_list[fci.forbid_uid_count++] = forbid_uid;

		int send_len = UNSTD_OFFSETOFF(Protocol::SCForbidChatInfo, forbid_uid_list) + fci.forbid_uid_count * sizeof(fci.forbid_uid_list[0]);

		World::GetInstWorld()->GetSceneManager()->SendToAllUser((const char *)&fci, send_len);
	}

	// 记录封禁者uid
	m_forbid_uid_set.insert(forbid_uid);
}

void ChatManager::OnRequestForbidInfo(Role *role)
{
	if (nullptr == role) return;

	Protocol::SCForbidChatInfo fci;

	fci.forbid_uid_count = 0;

	for (auto &it : m_forbid_uid_set)
	{
		if (fci.forbid_uid_count >= Protocol::SCForbidChatInfo::MAX_FORBID_UID_COUNT) break;

		fci.forbid_uid_list[fci.forbid_uid_count++] = it;
	}

	int send_len = UNSTD_OFFSETOFF(Protocol::SCForbidChatInfo, forbid_uid_list) + fci.forbid_uid_count * sizeof(fci.forbid_uid_list[0]);

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&fci, send_len);
}

ChatUser * ChatManager::CreateChatUser(int uid, GSNetID netid)
{
	ChatUserListIt it = m_chat_user_list.find(uid);
	if (it != m_chat_user_list.end()) 
	{
		return NULL; 
	}

	ChatUser *chat_user = new ChatUser(this, uid, netid);
	m_chat_user_list[uid] = chat_user;

	return chat_user;
}

Channel * ChatManager::CreateChatChannel(ChannelID cid, unsigned int cooldown, ChannelType type, const ChannelName  name)
{
	ChannelListIt it = m_channel_list.find(cid);
	if (it != m_channel_list.end()) 
	{
		return NULL; 
	}

	Channel *channel = new Channel(this, cid, cooldown, type, name);
	m_channel_list[cid] = channel;

	return channel;
}

bool ChatManager::DestroyChatChannel(ChannelID cid)
{
	ChannelListIt it = m_channel_list.find(cid);
	if (it == m_channel_list.end())
	{
		return false;
	}

	delete it->second;
	m_channel_list.erase(it);

	return true;
}

Channel * ChatManager::GetChatChannel(ChannelID cid)
{
	ChannelListIt it = m_channel_list.find(cid);
	if (it == m_channel_list.end()) return NULL; 
	return it->second;
}

void ChatManager::OnChannelChat(Role *role, void* msg, int length, short origin_type)
{
	if (nullptr == role) return;

	if (CrossConfig::Instance().IsHiddenServer())
	{
		// 大跨服中不允许频道聊天
		const int PLATFROM_BIG_CROSS_TYPE = 2;
		if (PLATFROM_BIG_CROSS_TYPE == CrossConfig::Instance().GetHiddenServerCrossType())
		{
			gstr::SendError(role, "en_channel_chat_scene_limit");
			return;
		}
		else
		{
			if (LOGIC_CONFIG->GetAgentAdapterConfig().IsForbidCrossChat(role->GetRoleCross()->GetCrossOriginalUniqueUserID().plat_type))
			{
				return;
			}
		}
	}

	Protocol::CSChannelChat *cc = (Protocol::CSChannelChat*)msg;

	if (cc->msg_length + (int)sizeof(Protocol::CSChannelChat) > length) // 检查格式
	{
		gamelog::g_log_debug.printf(LL_DEBUG, "OnChannelChat msg_length:%d is too long.", cc->msg_length);
		return;
	}

	if (cc->msg_length <= 0 || cc->msg_length > MAX_CHAT_LENGTH)
	{
		return;
	}

	// 语音屏蔽
	if (WorldStatus::Instance().GetAudioChatForbidFlag() > 0 && (Protocol::CHAT_CONTENT_TYPE_AUDIO == cc->content_type || Protocol::CHAT_CONTENT_TYPE_AUDIO_2 == cc->content_type))
	{
		return;
	}

	if (this->IsInForbidChannelChatTime(cc->channel_type))
	{
		return;
	}

	ChatUser *user = this->GetChatUser(role->GetUID());
	if (NULL == user)
	{
		return;
	}
	if (!user->ChannelCanChat(role, cc->channel_type))
	{
		return;
	}

	{
		// 检查敏感字
		//bool NAMEFILTER_OPEN_FLAG = ServerConfigPool::Instance().common_config.namefilter_open_flag;
		//if (NAMEFILTER_OPEN_FLAG)
		//{
		//	char *msg_content = (char *)msg + sizeof(Protocol::CSChannelChat);
		//	int msg_length = cc->msg_length;

		//	if (NameFilter::Instance().HasSensitiveWord(msg_length, msg_content))
		//	{
		//		// printf("ChanelChat Has SensitiveWorld !\n");
		//		return;
		//	}
		//}
	}

	static char buff[sizeof(Protocol::SCChannelChat) + MAX_CHAT_LENGTH + 1] = {0};

	Protocol::SCChannelChat *channel_chat = (Protocol::SCChannelChat*)buff;

	channel_chat->header.msg_type = Protocol::MT_CHANNEL_CHAT_SC;
	channel_chat->from_uid = role->GetUID();
	channel_chat->channel_type = (char)cc->channel_type;
	channel_chat->msg_length = cc->msg_length;
	role->GetName(channel_chat->username);
	channel_chat->sex = role->GetSex();
	channel_chat->camp = role->GetCamp();
	channel_chat->vip_level = role->GetVip()->GetVipLevel();
	channel_chat->prof = role->GetProf();
	channel_chat->authority_type = role->GetAuthority();
	channel_chat->level = (short)role->GetLevel();
	channel_chat->avatar_timestamp = role->GetAvatarTimeStamp();
	channel_chat->uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	channel_chat->content_type = cc->content_type;
	channel_chat->tuhaojin_color = role->GetTuHaoJin()->GetTuhaojinColor();
	channel_chat->bigchatface_status = (char)role->GetBigChatFace()->GetBigChatFaceStatus();
	channel_chat->personalize_window_bubble_type = role->GetPersonalizeWindow()->GetCurUseBubbleWindow();
	channel_chat->personalize_window_avatar_type = role->GetPersonalizeWindow()->GetCurUseAvatarWindowForClient();
	channel_chat->origin_type = origin_type;
	channel_chat->has_xianzunka_flag = role->GetHasXianzunkaFlag();

	channel_chat->guild_signin_count = 0;
	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (NULL != guild)
	{
		GuildMember *member = guild->GetGuildMemberManager().GetMember(role->GetUserId());
		if (NULL != member)
		{
			channel_chat->guild_signin_count = member->GetCommonData().signin_count_day;
		}
	}

	channel_chat->is_answer_true = 0;

	channel_chat->msg_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	memcpy(((char*)buff) + sizeof(Protocol::SCChannelChat), (char *)msg + sizeof(Protocol::CSChannelChat), cc->msg_length);

	int total_len = sizeof(Protocol::SCChannelChat) + cc->msg_length;

	char *chat_content = ((char*)buff) + sizeof(Protocol::SCChannelChat);
	chat_content[cc->msg_length] = 0;

	if (cc->channel_type == chatdef::CHANNEL_TYPE_WORLD)	// 世界聊天
	{
		if (!user->IgnoreWorldTalk())
		{
			this->OnWorldChat(role, buff, total_len);
		}
		
		user->OnWorldChat(cc);
		ROLE_LOG_QUICK6(LOG_TYPE_CHAT_RECORD, role, LOG_CHAT_TYPE_WORLD, 0, chat_content, NULL);
		
		// 检测是否需要禁言
		CommonDataParam *common_data = role->GetCommonDataParam();

		if (role->GetLevel() < GLOBALCONFIG->GetRoleCfg().GetRoleForbidChannel().role_level_limit)
		{
			unsigned int now_time = (unsigned int)(EngineAdapter::Instance().Time());

			// 1小时内
			{
				if (now_time - common_data->world_chat_record_time1 < static_cast<unsigned int>(SECOND_PER_HOUR))
				{
					++ common_data->world_chat_count1;
					if (common_data->world_chat_count1 >= GLOBALCONFIG->GetRoleCfg().GetRoleForbidChannel().max_world_chat_count1)
					{
						role->ForbidTalk((time_t)SECOND_PER_DAY, "toolmuchworldchat", "toolmuchworldchat");
					}
				}
				else // 超过1个小时
				{
					common_data->world_chat_count1 = 1;
					common_data->world_chat_record_time1 = now_time;
				}
			}

			// 2小时内
			{
				if (static_cast<int>(now_time - common_data->world_chat_record_time2) < 2 * SECOND_PER_HOUR)
				{
					++ common_data->world_chat_count2;
					if (common_data->world_chat_count2 >= GLOBALCONFIG->GetRoleCfg().GetRoleForbidChannel().max_world_chat_count2)
					{
						role->ForbidTalk((time_t)SECOND_PER_DAY, "toolmuchworldchat", "toolmuchworldchat");
					}
				}
				else // 超过2个小时
				{
					common_data->world_chat_count2 = 1;
					common_data->world_chat_record_time2 = now_time;
				}
			}
		}

		return;
	}

	if (cc->channel_type == chatdef::CHANNEL_TYPE_SCENE)
	{
		this->OnSceneChat(role, buff, total_len);
		ROLE_LOG_QUICK6(LOG_TYPE_CHAT_RECORD, role, LOG_CHAT_TYPE_SCENE, role->GetScene()->GetSceneID(), chat_content, NULL);
		return;
	}

	if (cc->channel_type == chatdef::CHANNEL_TYPE_TEAM)
	{
		this->OnTeamChat(role, buff, total_len);
		ROLE_LOG_QUICK6(LOG_TYPE_CHAT_RECORD, role, LOG_CHAT_TYPE_TEAM, 0, chat_content, NULL);
		return;
	}

	if (cc->channel_type == chatdef::CHANNEL_TYPE_CAMP)
	{
		this->OnCampChat(role, buff, total_len);
		ROLE_LOG_QUICK6(LOG_TYPE_CHAT_RECORD, role, LOG_CHAT_TYPE_GUILD, 0, chat_content, NULL);
		return;
	}

	if (cc->channel_type == chatdef::CHANNEL_TYPE_GUILD)
	{
		this->OnGuildChat(role, buff, total_len);
		user->OnGuildChat(cc);
		ROLE_LOG_QUICK6(LOG_TYPE_CHAT_RECORD, role, LOG_CHAT_TYPE_CAMP, role->GetGuildID(), chat_content, NULL);
		return;
	}

	if (cc->channel_type == chatdef::CHANNEL_TYPE_WORLD_QUESTION)
	{
		this->OnWorldQuestionChat(role, buff, total_len);
		ROLE_LOG_QUICK6(LOG_TYPE_CHAT_RECORD, role, LOG_CHAT_TYPE_WORLD_QUESTION, 0, chat_content, NULL);
		return;
	}

	if (cc->channel_type == chatdef::CHANNEL_TYPE_GUILD_QUESTION)
	{
		this->OnGuildQuestionChat(role, buff, total_len);
		ROLE_LOG_QUICK6(LOG_TYPE_CHAT_RECORD, role, LOG_CHAT_TYPE_GUILD_QUESTION, role->GetGuildID(), chat_content, NULL);
		return;
	}
}

void ChatManager::OnSingleChat(Role *role, void* msg, int length, bool is_sys)
{
	if (NULL == role || NULL == msg || length <= 0)
	{
		return;
	}

	Protocol::CSSingleChat *sc = (Protocol::CSSingleChat*)msg;

	if (sc->msg_length + (int)sizeof(Protocol::CSChannelChat) > length) // 检查格式
	{
		gamelog::g_log_debug.printf(LL_DEBUG, "OnSingleChat msg_length:%d is too long.", sc->msg_length);
		role->NoticeNum(errornum::EN_SINGLE_CHAT_FAIL_TYPE_FORMAT);
		return;
	}

	if (sc->msg_length <= 0 || sc->msg_length > MAX_CHAT_LENGTH)
	{
		role->NoticeNum(errornum::EN_SINGLE_CHAT_FAIL_TYPE_LENGTH);
		return;
	}

	// 语音屏蔽
	if (WorldStatus::Instance().GetAudioChatForbidFlag() > 0 && (Protocol::CHAT_CONTENT_TYPE_AUDIO == sc->content_type || Protocol::CHAT_CONTENT_TYPE_AUDIO_2 == sc->content_type))
	{
		role->NoticeNum(errornum::EN_SINGLE_CHAT_FAIL_TYPE_AUDIO);
		return;
	}

	ChatUser *chat_user = this->GetChatUser(role->GetUID());
	if (NULL == chat_user)
	{
		role->NoticeNum(errornum::EN_SINGLE_CHAT_FAIL_TYPE_CHAT_USER);
		return;
	}

	// 玩家自己发的私聊检查各种限制
	if(!is_sys)
	{
		// 跨服限制
		if (CrossConfig::Instance().IsHiddenServer())
		{
			if (LOGIC_CONFIG->GetAgentAdapterConfig().IsForbidCrossChat(role->GetRoleCross()->GetCrossOriginalUniqueUserID().plat_type))
			{
				role->NoticeNum(errornum::EN_SINGLE_CHAT_FAIL_TYPE_FORBID_CROSS_CHAT);
				return;
			}
		}

		if (chat_user->IsForbidChannel(role))  // 被禁言
		{
			role->NoticeNum(errornum::EN_SINGLE_CHAT_FAIL_TYPE_FORBID_CHANNEL);
			return;
		}

		// 禁止聊天时间内
		if (this->IsInForbidChannelChatTime(OPENLEVEL_LIMIT_TYPE_SINGLE))
		{
			role->NoticeNum(errornum::EN_SINGLE_CHAT_FAIL_TYPE_FORBID_CHANNEL_TIME);
			return;
		}

		// 充值额度限制检查
// 		if (!role->IsIgnoreChatOrMailLevelLimit())
// 		{
// 			return;
// 		}

		UniqueUserID uuid = LongLongToUniqueUserID(sc->to_uuid);

		// 渠道聊天要求必须好友检查
		if (LOGIC_CONFIG->GetAgentAdapterConfig().IsSingleChatMustFirend() && !role->GetFriendManager()->IsFriend(uuid.user_id))
		{
			role->NoticeNum(errornum::EN_SINGLE_CHAT_FAIL_TYPE_NOT_FRIEND);
			return;
		}

		// 等级限制、vip等级限制
		{
			//bool can_chat = false;
			if (!WorldStatus::Instance().CheckSatisfyLevel(role, OPENLEVEL_LIMIT_TYPE_SINGLE) &&
				!IsCanAckSingleChat(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()), sc->to_uuid))
			{
				const int chat_limit_condition_type = WorldStatus::Instance().GetChatLimitConditionType(OPENLEVEL_LIMIT_TYPE_SINGLE);
				//const int vip_level = role->GetVip()->IsTimeLimitVip() ? 0 : role->GetVip()->GetVipLevel();
				//const int role_level = role->GetLevel();
				const int vip_level_limit = chat_user->GetSingleChatOpenVipLevel();
				const int role_level_limit = chat_user->GetChannelOpenLevel(OPENLEVEL_LIMIT_TYPE_SINGLE);

				if (vip_level_limit == 0)
				{
					gstr::SendError(role, "single_chat_level_limit_tips", role_level_limit);
				}
				else
				{
					if (CHAT_LIMIT_CONDITION_TYPE_AND == chat_limit_condition_type)
					{
						gstr::SendError(role, "single_chat_limit_tips_and", vip_level_limit, role_level_limit);
					}
					else if (CHAT_LIMIT_CONDITION_TYPE_OR == chat_limit_condition_type)
					{
						gstr::SendError(role, "single_chat_limit_tips_or", vip_level_limit, role_level_limit);
					}
				}
				
				return;
			}
		}
	}

	{
		// 检查敏感字
		//bool NAMEFILTER_OPEN_FLAG = ServerConfigPool::Instance().common_config.namefilter_open_flag;
		//if (NAMEFILTER_OPEN_FLAG)
		//{
		//	char *msg_content = (char *)msg + sizeof(Protocol::CSSingleChat);
		//	int msg_length = sc->msg_length;

		//	if (NameFilter::Instance().HasSensitiveWord(msg_length, msg_content))
		//	{
		//		// printf("SingleChat Has SensitiveWorld !\n");
		//		return;
		//	}
		//}
	}

	static char buff[sizeof(Protocol::SCChannelChat) + MAX_CHAT_LENGTH + 1];

	Protocol::SCSingleChat *single_chat = (Protocol::SCSingleChat*)buff;

	single_chat->header.msg_type = Protocol::MT_SINGLE_CHAT_SC;
	single_chat->from_uid = role->GetUID();
	single_chat->from_uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	single_chat->msg_length = sc->msg_length;
	role->GetName(single_chat->username);
	single_chat->sex = role->GetSex();
	single_chat->camp = role->GetCamp();
	single_chat->vip_level = role->GetVip()->GetVipLevel();
	single_chat->prof = role->GetProf();
	single_chat->authority_type = role->GetAuthority();
	single_chat->level = (short)role->GetLevel(); 
	single_chat->tuhaojin_color = role->GetTuHaoJin()->GetTuhaojinColor();
	single_chat->bigchatface_status = (char)role->GetBigChatFace()->GetBigChatFaceStatus();
	single_chat->personalize_window_bubble_type = role->GetPersonalizeWindow()->GetCurUseBubbleWindow();
	single_chat->personalize_window_avatar_type = role->GetPersonalizeWindow()->GetCurUseAvatarWindowForClient();
	single_chat->is_echo = sc->is_echo;
	single_chat->special_param = sc->special_param;
	single_chat->has_xianzunka_flag = role->GetHasXianzunkaFlag();
	single_chat->content_type = sc->content_type;
	single_chat->avatar_timestamp = role->GetAvatarTimeStamp();
	single_chat->msg_timestamp = 0;
	memcpy(((char*)buff) + sizeof(Protocol::SCSingleChat), (char *)msg + sizeof(Protocol::CSSingleChat), sc->msg_length);

	int total_len = sizeof(Protocol::SCSingleChat) + sc->msg_length;


	UniqueUserID uuid = LongLongToUniqueUserID(sc->to_uuid);
	UniqueServerID target_server_id(uuid.plat_type, uuid.user_id.db_index);
	UniqueServerID local_server_id(LocalConfig::Instance().GetPlatType(), LocalConfig::Instance().GetMergeServerId());


	Role * to_role = World::GetInstWorld()->GetSceneManager()->GetRoleByUUID(sc->to_uuid);
	if (to_role)
	{
		//if (!is_sys /*&& !to_role->IsIgnoreChatOrMailLevelLimit() */&& !WorldStatus::Instance().CheckSatisfyLevel(to_role, OPENLEVEL_LIMIT_TYPE_SINGLE) && 
		//	IsCanAckSingleChat(role->GetUID(), UidToInt(uuid.user_id)))  // 对方私聊等级不足
		//{
		//	role->NoticeNum(errornum::EN_SINGLE_CHAT_OTHER_LEVEL_LIMIT);
		//	return;
		//}

		EngineAdapter::Instance().NetSend(to_role->GetNetId(), (const char*)buff, total_len);

		chat_user->OnSingleChat(sc);
		role->GetFriendManager()->AddFavorableImpression(to_role->GetUniqueUserID().user_id);	//增加好感度
		to_role->GetFriendManager()->AddFavorableImpression(role->GetUniqueUserID().user_id);	//增加好感度

		{
			char *chat_content = ((char*)buff) + sizeof(Protocol::SCSingleChat);
			chat_content[sc->msg_length] = 0;
			ROLE_LOG_QUICK6(LOG_TYPE_CHAT_RECORD, role, 0, sc->to_uuid, chat_content, NULL);
		}
	}
	else 
	{
		if (!CrossConfig::Instance().IsHiddenServer())
		{
		//	//InternalComm::Instance().UserProtocalTransferToGame(sc->to_uuid, (const char *)buff, total_len);
		//	this->CheckCrossSingleChat(role, sc->to_uuid, buff, total_len);
		//}
		//else
		//{
			std::map<long long, CrossUserInfo> &cross_user_map = World::GetInstWorld()->GetSceneManager()->GetCrossUserMap();
			if (cross_user_map.find(sc->to_uuid) == cross_user_map.end())
			{
			//	//InternalComm::Instance().UserProtocalTransferToGame(sc->to_uuid, (const char *)buff, total_len);
			//	this->CheckCrossSingleChat(role, sc->to_uuid, buff, total_len);
			//}
			//else
			//{
				gamelog::g_log_debug.printf(LL_DEBUG, "OnSignleChat Can NOT find to_user:%lld.", sc->to_uuid);

				Protocol::SCSingleChatUserNotExist scune;
				scune.to_uid = UidToInt(uuid.user_id);
				EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&scune, sizeof(Protocol::SCSingleChatUserNotExist));
				single_chat->msg_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				if (this->AddSingleChatBoard(scune.to_uid, buff, total_len))
				{
					role->GetFriendManager()->AddFavorableImpression(uuid.user_id);
				}
			}
		}
	}
	//无论怎样都转发到另一个服务器.添加反向聊天状态
	this->CheckCrossSingleChat(role, sc->to_uuid, buff, total_len);
	AddCanAckSingleChat(single_chat->from_uuid, sc->to_uuid);
}

void ChatManager::OnCrossChannelChat(short channel_type, short camp, int guild_id, const char* buff, int total_len)
{
	if (NULL == buff || total_len <= 0) return;

	if (chatdef::CHANNEL_TYPE_WORLD == channel_type)
	{
		World::GetInstWorld()->SendToAllGateway((const char*)buff, total_len);
		ChatManager::Instance().AddWorldChatBoard(buff, total_len, false);
	}
	else if (chatdef::CHANNEL_TYPE_CAMP == channel_type)
	{
		int cid = chatdef::CHANNEL_ID_CAMP_BEGIN + camp;

		Channel *channel = this->GetChatChannel(cid); // 频道是否存在
		if (NULL == channel)
		{
			gamelog::g_log_debug.printf(LL_INFO, "OnCrossChannelChat Can NOT find channel:%d.", cid);
			return;
		}

		channel->OnChannelMsg(0, buff, total_len);

	}
	else if (chatdef::CHANNEL_TYPE_GUILD == channel_type)
	{
		Guild *guild = GuildManager::Instance().GetGuild(guild_id);
		if (NULL == guild)
		{
			return;
		}

		guild->GetGuildMemberManager().OnChat(buff, total_len);	
		this->AddGuildChatBoard(guild->GetGuildID(), buff, total_len);
	}
}

void ChatManager::OnCrossSingleChat(long long to_uuid, const char *buff, int total_len)
{
	UniqueUserID uuid = LongLongToUniqueUserID(to_uuid);

	//UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(uuid.user_id);
	//if (nullptr == user_node)
	//{
	//	return;
	//}
	long long from_uuid = 0;
	GetFromUUIDBySingleChatBuff(buff, total_len, &from_uuid);

	Role *to_role = World::GetInstWorld()->GetSceneManager()->GetRoleByUUID(to_uuid);
	if (nullptr != to_role)
	{
		if (from_uuid != 0)
		{
			to_role->GetFriendManager()->AddFavorableImpression(LongLongToUniqueUserID(from_uuid).user_id);
		}
		EngineAdapter::Instance().NetSend(to_role->GetNetId(), buff, total_len);
	}
	else
	{
		this->AddSingleChatBoard(UidToInt(uuid.user_id), buff, total_len);
	}

	if (from_uuid != 0)
	{
		AddCanAckSingleChat(from_uuid, to_uuid);
	}
}

void ChatManager::OnWorldQuestionChat(Role *role, const char* buff, int total_len)
{
	this->OnWorldChat(role, buff, total_len);
}

void ChatManager::OnWorldChat(Role *role, const char* buff, int total_len)
{
	if (NULL == role || NULL == buff || total_len <= 0) return;

	int role_orginal_plat_type = role->GetRoleCross()->GetCrossOriginalUniqueUserID().plat_type;
	if (!LOGIC_CONFIG->GetAgentAdapterConfig().IsCanbeViewOtherServer(role_orginal_plat_type) &&
		CrossConfig::Instance().IsHiddenServer())
	{
		World::GetInstWorld()->GetSceneManager()->SendToAllUser((const char*)buff, total_len, [role](Role *tmp_role) -> bool
		{
			return role->GetRoleCross()->IsFromSameGameServer(tmp_role);
		});
	}
	else
	{
		World::GetInstWorld()->SendToAllGateway((const char*)buff, total_len);
	}

	CrossUserMap cross_user_map = World::GetInstWorld()->GetSceneManager()->GetCrossUserMap();
	for (CrossUserMapIt it = cross_user_map.begin(); it != cross_user_map.end(); it++)
	{
		InternalComm::Instance().UserProtocalTransferToGame(it->first, buff, total_len);
	}

	Protocol::SCChannelChat *cc = (Protocol::SCChannelChat *)buff;

	if (chatdef::CHANNEL_TYPE_WORLD == cc->channel_type)
	{
		this->AddWorldChatBoard(buff, total_len);
		this->CheckCrossChannelChat(role, chatdef::CHANNEL_TYPE_WORLD, buff, total_len);
	}

	EventHandler::Instance().OnChatOnWorldChannel(role);
}

void ChatManager::OnSceneChat(Role *role, const char* buff, int total_len)
{
	if (NULL == role || NULL == buff || total_len <= 0) return;

	Scene *scene = role->GetScene();
	if (NULL == scene)
	{
		return;
	}

	int net_num = 0;

	int role_num = scene->RoleNum();
	for (int i = 0; i < role_num; ++i)
	{
		if (net_num >= MSX_GSNETID_NUM)
		{
			break;
		}

		Role *tmp_role = scene->GetRoleByIndex(i);
		if (NULL != tmp_role)
		{
			gsnetidlist[net_num ++] = tmp_role->GetNetId();
		}
	}

	if (net_num > 0)
	{
		EngineAdapter::Instance().NetMulSend(gsnetidlist, net_num, buff, total_len);
	}
}

void ChatManager::OnTeamChat(Role *role, const char* buff, int total_len)
{
	if (NULL == role || NULL == buff || total_len <= 0) return;

	if (!role->GetTeamInfo()->InTeam())
	{
		return;
	}

	Team *team = role->GetTeamInfo()->GetMyTeam();
	if (NULL == team)
	{
		return;
	}

	int net_num = 0;

	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
	{
		Team::TeamMember *tmm = team->GetTeamMember(i);
		if (NULL == tmm)
		{
			continue;
		}

		Role *tmp_role = World::GetInstWorld()->GetSceneManager()->GetRole(tmm->user_id);
		if (NULL != tmp_role)
		{
			gsnetidlist[net_num ++] = tmp_role->GetNetId();
		}
	}

	if (net_num > 0)
	{
		EngineAdapter::Instance().NetMulSend(gsnetidlist, net_num, buff, total_len);
	}
}

void ChatManager::OnGuildQuestionChat(Role *role, const char* buff, int total_len)
{
	this->OnGuildChat(role, buff, total_len);
}

void ChatManager::OnGuildChat(Role *role, const char* buff, int total_len)
{
	if (NULL == role || NULL == buff || total_len <= 0) return;

	if (INVALID_GUILD_ID == role->GetGuildID())
	{
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (NULL != guild)
	{
		if (ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILD_QUESTION))
		{
			//标记该条聊天信息
			if (ActivityShadow::Instance().OnGuildQuestionAnswerQuestion(role, buff, total_len))
			{
				Protocol::SCChannelChat *channel_chat = (Protocol::SCChannelChat*)buff;
				channel_chat->is_answer_true = 1;
			}
		}

		guild->GetGuildMemberManager().OnChat(buff, total_len);
		this->AddGuildChatBoard(guild->GetGuildID(), buff, total_len);
	}
	else
	{
		// 拿不到guild，就应该是在跨服
		this->CheckCrossChannelChat(role, chatdef::CHANNEL_TYPE_GUILD, buff, total_len);
	}
}

void ChatManager::OnCampChat(Role *role, const char* buff, int total_len)
{
	if (NULL == role || NULL == buff || total_len <= 0) return;

	char camp = role->GetCamp();
	int cid = chatdef::CHANNEL_ID_CAMP_BEGIN + camp;

	Channel *channel = this->GetChatChannel(cid); // 频道是否存在
	if (NULL == channel)
	{
		gamelog::g_log_debug.printf(LL_INFO, "OnCampChat Can NOT find channel:%d.", cid);
		return;
	}

	channel->OnChannelMsg(role->GetUID(), buff, total_len);

	CrossUserMap cross_user_map = World::GetInstWorld()->GetSceneManager()->GetCrossUserMap();
	for (CrossUserMapIt it = cross_user_map.begin(); it != cross_user_map.end(); it++)
	{
		if (it->second.camp == camp)
		{
			InternalComm::Instance().UserProtocalTransferToGame(it->first, buff, total_len);
		}
	}

	this->CheckCrossChannelChat(role, chatdef::CHANNEL_TYPE_CAMP, buff, total_len);
}

void ChatManager::CheckCrossChannelChat(Role *role, short channel_type, const char* buff, int total_len)
{
	if (NULL == role || NULL == buff || total_len <= 0 || total_len + sizeof(crossgameprotocal::CrossGameChannelChat) > MAX_MSG_SIZE) return;

	if (CrossConfig::Instance().IsHiddenServer())
	{
		static char tmp_buffer[MAX_MSG_SIZE];

		int send_len = 0;

		crossgameprotocal::CrossGameChannelChat *cgcc = (crossgameprotocal::CrossGameChannelChat *)(tmp_buffer + send_len);
		*cgcc = crossgameprotocal::CrossGameChannelChat();
		cgcc->uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
		cgcc->channel_type = channel_type;
		cgcc->role_camp = role->GetCamp();
		cgcc->guild_id = role->GetGuildID();
		cgcc->protocal_len = total_len;

		send_len += sizeof(crossgameprotocal::CrossGameChannelChat);

		memcpy(tmp_buffer + send_len, buff, total_len);
		send_len += total_len;
		
		InternalComm::Instance().SendToCross((const char *)tmp_buffer, send_len);
	}
}

void ChatManager::CheckCrossSingleChat(Role *role, long long to_uuid, const char *buff, int total_len)
{
	if (nullptr == role || nullptr == buff || total_len <= 0 || total_len + sizeof(crossgameprotocal::CrossGameSingleChat) > MAX_MSG_SIZE) return;

	int send_len = 0;
	static  char tmp_buffer[MAX_MSG_SIZE];

	crossgameprotocal::CrossGameSingleChat *cgsc = (crossgameprotocal::CrossGameSingleChat *)tmp_buffer;
	*cgsc = crossgameprotocal::CrossGameSingleChat();
	cgsc->to_uuid = to_uuid;
	memcpy(tmp_buffer + sizeof(crossgameprotocal::CrossGameSingleChat), buff, total_len);

	send_len = total_len + sizeof(crossgameprotocal::CrossGameSingleChat);

	UniqueUserID unique_user_id = LongLongToUniqueUserID(to_uuid);
	role->GetFriendManager()->AddFavorableImpression(unique_user_id.user_id);
	if (CrossConfig::Instance().IsHiddenServer())
	{
		InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_BY_UUID, UniqueUserIDToLongLong(role->GetUniqueUserID()), (const char*)cgsc, send_len);
	}
	else
	{
		InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_HIDDEN, UniqueUserIDToLongLong(role->GetUniqueUserID()), (const char*)cgsc, send_len);
	}
}

void ChatManager::OnGuildChatInCross(UniqueUserID guild_id, int msg_origin_type, const char* buff, int length)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	if (nullptr == buff || length <= 0 || length + sizeof(crossgameprotocal::CrossGameRequestGuildChat) > MAX_MSG_SIZE)
	{
		return;
	}

	long long guild_uuid = UniqueUserIDToLongLong(guild_id);

	static crossgameprotocal::CrossGameRequestGuildChat cgrgc;

	int send_len = length + sizeof(cgrgc) - sizeof(cgrgc.msg_buff);

	if (length < (int)sizeof(cgrgc.msg_buff) && send_len > 0)
	{
		cgrgc.guild_id = guild_uuid;
		cgrgc.msg_origin_type = msg_origin_type;
		cgrgc.msg_length = length;
		F_STRNCPY(cgrgc.msg_buff, buff, length);

		InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_BY_UUID, guild_uuid, (const char*)&cgrgc, send_len);
	}
}

bool ChatManager::AddToChannel(ChannelID cid, int uid)
{
	Channel *channel = this->GetChatChannel(cid); // 频道是否存在
	if (NULL == channel)
	{
		gamelog::g_log_debug.printf(LL_INFO, "AddToChannel Can NOT find channel:%d uid:%d.", cid, uid);
		return false;
	}

	ChatUserListIt it = m_chat_user_list.find(uid);
	if (m_chat_user_list.end() != it)
	{
		channel->AddUser(it->second->GetUID(), it->second->GetNetId());
	}

	return true;
}

void ChatManager::RemoveFromChannel(ChannelID cid, int uid)
{
	Channel *channel = this->GetChatChannel(cid); // 频道是否存在
	if (NULL == channel)
	{
		gamelog::g_log_debug.printf(LL_INFO, "RemoveFromChannel Can NOT find channel:%d uid:%d.", cid, uid);
		return;
	}

	channel->DeleteUser(uid);
}

bool ChatManager::AddSingleChatBoard(int uid, const char *msg, int msg_len)
{
	const ChannelChatLimitCfg *channel_chat_limit_cfg = LOGIC_CONFIG->GetAgentAdapterConfig().GetChannelChatLimitCfg(OPENLEVEL_LIMIT_TYPE_SINGLE);
	if (nullptr == channel_chat_limit_cfg || channel_chat_limit_cfg->not_save_chat || !LOGIC_CONFIG->GetAgentAdapterConfig().IsSingleChatCanOffline())
	{
		return false;
	}

	if (NULL == msg || msg_len <= 0) 
	{
		return false;
	}

	UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(uid);
	if (NULL == user_node)
	{
		return false;
	}

	MsgBoardMapIt board_it = m_single_chat_board_map.find(uid);
	if (m_single_chat_board_map.end() == board_it)
	{
		MsgBoardItemQueue tmp_queue;
		m_single_chat_board_map[uid] = tmp_queue;

		board_it = m_single_chat_board_map.find(uid); 
		if (m_single_chat_board_map.end() == board_it)
		{
			return false;
		}
	}

	const static unsigned int MAX_MSG_BOARD_ITEM_COUNT = 100;

	MsgBoardItemQueue &msg_item_queue = board_it->second;
	if (msg_item_queue.size() >= MAX_MSG_BOARD_ITEM_COUNT)
	{
		msg_item_queue.pop_front();
	}

	MsgBoardItem msg_board_item;
	msg_board_item.SetMsg(msg, msg_len);
	msg_item_queue.push_back(msg_board_item);
	return true;
}

// void ChatManager::CheckSingleChatBoard(Role *role)
// {
// 	if (NULL == role)
// 	{
// 		return;
// 	}
// 
// 	MsgBoardMapIt board_it = m_single_chat_board_map.find(role->GetUID());
// 	if (m_single_chat_board_map.end() == board_it)
// 	{
// 		return;
// 	}
// 
// 	MsgBoardItemQueue &msg_item_queue = board_it->second;
// 
// 	for (MsgBoardItemQueueIt it = msg_item_queue.begin(); msg_item_queue.end() != it; ++it)
// 	{
// 		int send_len = 0;
// 		const char *send_buffer = it->GetSendBuffer(&send_len);
// 		if (send_len > 0 && NULL != send_buffer)
// 		{
// 			EngineAdapter::Instance().NetSend(role->GetNetId(), send_buffer, send_len);
// 		}
// 	}
// 
// 	m_single_chat_board_map.erase(board_it);
// }

void ChatManager::CheckSingleChatBoard(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	MsgBoardMapIt board_it = m_single_chat_board_map.find(role->GetUID());
	if (m_single_chat_board_map.end() == board_it)
	{
		return;
	}

	MsgBoardItemQueue &msg_item_queue = board_it->second;

	int send_msg_len = 0;			// 消息总长度
	static char buff[MAX_MSG_SIZE]; // 消息内容

	Protocol::SCChatBoardListInfo *bli = (Protocol::SCChatBoardListInfo*)buff;
	bli->header.msg_type = Protocol::MT_CHAT_BOARD_LIST_SC;
	bli->channel_type = chatdef::CHANNEL_TYPE_SINGLE;
	bli->list_count = 0;

	send_msg_len += sizeof(Protocol::SCChatBoardListInfo);

	for (MsgBoardItemQueueIt it = msg_item_queue.begin(); msg_item_queue.end() != it; ++it)
	{
		int msg_len = 0;
		const char *send_buffer = it->GetSendBuffer(&msg_len);
		if (msg_len > 0 && NULL != send_buffer)
		{
			if (send_msg_len + msg_len >= MAX_MSG_SIZE) break;

			*(int *)(buff + send_msg_len) = msg_len;
			send_msg_len += sizeof(int);
			memcpy(buff + send_msg_len, send_buffer, msg_len);   // 拷贝协议内容
			send_msg_len += msg_len;

			bli->list_count++;

			long long from_uuid = 0;
			if (this->GetFromUUIDBySingleChatBuff(send_buffer, msg_len, &from_uuid))
			{
				role->GetFriendManager()->AddFavorableImpression(LongLongToUniqueUserID(from_uuid).user_id);
			}
		}
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)buff, send_msg_len);

	m_single_chat_board_map.erase(board_it);
}

void ChatManager::AddGuildChatBoard(int guild_id, const char *msg, int msg_len)
{
	const ChannelChatLimitCfg *channel_chat_limit_cfg = LOGIC_CONFIG->GetAgentAdapterConfig().GetChannelChatLimitCfg(OPENLEVEL_LIMIT_TYPE_GUILD);
	if (nullptr == channel_chat_limit_cfg || channel_chat_limit_cfg->not_save_chat)
	{
		return;
	}

	if (NULL == msg || msg_len <= 0) 
	{
		return;
	}

	MsgBoardMapIt board_it = m_guild_chat_board_map.find(guild_id);
	if (m_guild_chat_board_map.end() == board_it)
	{
		MsgBoardItemQueue tmp_queue;
		m_guild_chat_board_map[guild_id] = tmp_queue;

		board_it = m_guild_chat_board_map.find(guild_id); 
		if (m_guild_chat_board_map.end() == board_it)
		{
			return;
		}
	}

	const static unsigned int MAX_MSG_BOARD_ITEM_COUNT = 100;

	MsgBoardItemQueue &msg_item_queue = board_it->second;
	if (msg_item_queue.size() >= MAX_MSG_BOARD_ITEM_COUNT)
	{
		msg_item_queue.pop_front();
	}

	MsgBoardItem msg_board_item;
	msg_board_item.SetMsg(msg, msg_len);
	msg_item_queue.push_back(msg_board_item);
}

void ChatManager::SetChannelChatForbid(int channel_type, unsigned int start_forbid_timestamp, unsigned int end_forbid_timestamp)
{
	if (channel_type >= OPENLEVEL_LIMIT_TYPE_WORLD && channel_type < OPENLEVEL_LIMIT_TYPE_MAX)
	{
		m_channel_chat_forbid_info_list[channel_type].start_forbid_timestamp = start_forbid_timestamp;
		m_channel_chat_forbid_info_list[channel_type].end_forbid_timestamp = end_forbid_timestamp;
	}
	else
	{
		for (int i = OPENLEVEL_LIMIT_TYPE_WORLD; i < OPENLEVEL_LIMIT_TYPE_MAX; ++i)
		{
			m_channel_chat_forbid_info_list[i].start_forbid_timestamp = start_forbid_timestamp;
			m_channel_chat_forbid_info_list[i].end_forbid_timestamp = end_forbid_timestamp;
		}
	}
}

bool ChatManager::IsInForbidChannelChatTime(int channel_type)
{
	if (channel_type >= OPENLEVEL_LIMIT_TYPE_WORLD && channel_type < OPENLEVEL_LIMIT_TYPE_MAX)
	{
		unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

		if (now_sec >= m_channel_chat_forbid_info_list[channel_type].start_forbid_timestamp &&
			now_sec <= m_channel_chat_forbid_info_list[channel_type].end_forbid_timestamp)
		{
			return true;
		}
	}

	const ChannelChatLimitCfg *channel_chat_limit_cfg = LOGIC_CONFIG->GetAgentAdapterConfig().GetChannelChatLimitCfg(channel_type);
	if (nullptr != channel_chat_limit_cfg)
	{
		const tm *local_time = EngineAdapter::Instance().LocalTime();
		if (local_time->tm_hour >= channel_chat_limit_cfg->forbid_chat_begin_hour && local_time->tm_hour < channel_chat_limit_cfg->forbid_chat_end_hour)
		{
			return true;
		}
	}

	return false;
}

// void ChatManager::CheckWorldChatBoard(Role *role)
// {
// 	if (NULL == role)
// 	{
// 		return;
// 	}
// 
// 	// 新角色不拉取聊天记录
// 	{
// 		const unsigned int create_role_timestamp = static_cast<unsigned int>(role->GetCreateRoleTimestamp());
// 		const unsigned int now_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
// 
// 		if (now_timestamp < create_role_timestamp + 10)
// 		{
// 			return;
// 		}
// 	}
// 
// 	for (MsgBoardItemQueueIt it = m_world_board_list.begin(); m_world_board_list.end() != it; ++it)
// 	{
// 		int send_len = 0;
// 		const char *send_buffer = it->GetSendBuffer(&send_len);
// 		if (send_len > 0 && NULL != send_buffer)
// 		{
// 			EngineAdapter::Instance().NetSend(role->GetNetId(), send_buffer, send_len);
// 		}
// 	}
// }

void ChatManager::CheckWorldChatBoard(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	// 新角色不拉取聊天记录
	{
		const unsigned int create_role_timestamp = static_cast<unsigned int>(role->GetCreateRoleTimestamp());
		const unsigned int now_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

		if (now_timestamp < create_role_timestamp + 10)
		{
			return;
		}
	}

	int send_msg_len = 0;			// 消息总长度
	static char buff[MAX_MSG_SIZE]; // 消息内容

	Protocol::SCChatBoardListInfo *bli = (Protocol::SCChatBoardListInfo*)buff;
	bli->header.msg_type = Protocol::MT_CHAT_BOARD_LIST_SC;
	bli->channel_type = chatdef::CHANNEL_TYPE_WORLD;
	bli->list_count = 0;

	send_msg_len += sizeof(Protocol::SCChatBoardListInfo);
	
	for (MsgBoardItemQueueIt it = m_world_board_list.begin(); m_world_board_list.end() != it; ++it)
	{
		int msg_len = 0;
		const char *send_buffer = it->GetSendBuffer(&msg_len);
		if (msg_len > 0 && NULL != send_buffer)
		{
			if (send_msg_len + msg_len >= MAX_MSG_SIZE) break;

			*(int *)(buff + send_msg_len) = msg_len;
			send_msg_len += sizeof(int);
			memcpy(buff + send_msg_len, send_buffer, msg_len);   // 拷贝协议内容
			send_msg_len += msg_len;

			bli->list_count++;
		}
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)buff, send_msg_len);
}

void ChatManager::AddWorldChatBoard(const char *msg, int msg_len, bool is_speaker)
{
	const ChannelChatLimitCfg *channel_chat_limit_cfg = LOGIC_CONFIG->GetAgentAdapterConfig().GetChannelChatLimitCfg(OPENLEVEL_LIMIT_TYPE_WORLD);
	if (nullptr == channel_chat_limit_cfg || channel_chat_limit_cfg->not_save_chat)
	{
		return;
	}

	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	if (NULL == msg || msg_len <= 0)
	{
		return;
	}

	if (is_speaker)
	{
		Protocol::SCSpeaker *sc_speaker = (Protocol::SCSpeaker *)msg;
		sc_speaker->is_record_msg = 1;
	}
	else
	{
		Protocol::SCChannelChat *cc = (Protocol::SCChannelChat *)msg;
		cc->is_msg_record = 1;
	}

	const static unsigned int MAX_MSG_BOARD_ITEM_COUNT = 100;

	if (m_world_board_list.size() >= MAX_MSG_BOARD_ITEM_COUNT)
	{
		m_world_board_list.pop_front();
	}

	MsgBoardItem msg_board_item;
	msg_board_item.SetMsg(msg, msg_len);
	m_world_board_list.push_back(msg_board_item);
}

void ChatManager::AddCanAckSingleChat(long long  role_uuid, long long target_uuid)
{
	bool is_can_ack = LOGIC_CONFIG->GetAgentAdapterConfig().IsSingleChatCanAckChat();
	if (!is_can_ack)
	{
		return;
	}

	m_recent_single_chat_role_map[target_uuid].insert(role_uuid);
}

bool ChatManager::IsCanAckSingleChat(long long role_uuid, long long target_uuid)
{	
	bool is_can_ack = LOGIC_CONFIG->GetAgentAdapterConfig().IsSingleChatCanAckChat();
	if (!is_can_ack)
	{
		return false;
	}

	auto it = m_recent_single_chat_role_map.find(role_uuid);
	if (it != m_recent_single_chat_role_map.end())
	{
		return it->second.find(target_uuid) != it->second.end();
	}

	return false;
}

void ChatManager::AddListenRoleInfo(Role *role, long long target_uuid)
{
	if (nullptr == role || target_uuid <= 0)
	{
		return;
	}

	long long unique_userid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	UniqueUserID target_uniqueid = LongLongToUniqueUserID(target_uuid);

	// 在隐藏服
	if (CrossConfig::Instance().IsHiddenServer())
	{
		crossgameprotocal::GameCrossWorldCommonDataChange cross_msg;

		cross_msg.uuid = unique_userid;
		cross_msg.change_type = crossgameprotocal::WORLD_COMMONDATA_CHANGE_TYPE_SINGLE_CHAT_LISTEN;
		cross_msg.sub_type = 0;
		cross_msg.param_2 = target_uniqueid.plat_type;
		cross_msg.param_3 = UidToInt(target_uniqueid.user_id);

		InternalComm::Instance().SendToCross((const char*)&cross_msg, sizeof(cross_msg));
	}
	// 游戏服
	else
	{
		m_single_chat_listen_role_map[unique_userid].insert(target_uuid);
		m_single_chat_be_listen_role_map[target_uuid].insert(unique_userid);

		int online_type = World::GetInstWorld()->GetSceneManager()->GetOnLineType(target_uuid);

		this->SendOnlineInfo(unique_userid, target_uuid, online_type);
	}
}

void ChatManager::OnCrossAddListenRoleInfo(long long uuid, long long target_uuid)
{
	m_single_chat_listen_role_map[uuid].insert(target_uuid);
	m_single_chat_be_listen_role_map[target_uuid].insert(uuid);

	int online_type = World::GetInstWorld()->GetSceneManager()->GetOnLineType(target_uuid);

	// 发信息回隐藏服
	this->SendOnlineInfo(uuid, target_uuid, online_type);
}

void ChatManager::DeleteListenRoleInfo(Role *role, long long target_uuid)
{
	if (nullptr == role)
	{
		return;
	}

	long long unique_userid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());

	// 自己监听列表
	auto listen_it = m_single_chat_listen_role_map.find(unique_userid);
	if (listen_it != m_single_chat_listen_role_map.end())
	{
		auto target_it = listen_it->second.find(target_uuid);
		if (target_it != listen_it->second.end())
		{
			listen_it->second.erase(target_it);
		}
	}

	// 对方被监听列表
	auto be_listen_it = m_single_chat_be_listen_role_map.find(target_uuid);
	if (be_listen_it != m_single_chat_be_listen_role_map.end())
	{
		auto be_target_it = be_listen_it->second.find(unique_userid);
		if (be_target_it != be_listen_it->second.end())
		{
			be_listen_it->second.erase(be_target_it);
		}
	}
}

void ChatManager::DeleteListenAllRoleInfo(Role *role)
{
	if (nullptr == role)
	{
		return;
	}

	long long unique_userid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());

	// 自己监听列表
	auto listen_it = m_single_chat_listen_role_map.find(unique_userid);
	if (listen_it != m_single_chat_listen_role_map.end())
	{
		for (auto be_listen_it : listen_it->second)
		{
			// 对方被监听列表
			auto self_it = m_single_chat_be_listen_role_map.find(be_listen_it);
			if (self_it != m_single_chat_be_listen_role_map.end())
			{
				auto be_target_it = self_it->second.find(unique_userid);
				if (be_target_it != self_it->second.end())
				{
					self_it->second.erase(be_target_it);
				}
			}
		}

		m_single_chat_listen_role_map.erase(listen_it);
	}
}

void ChatManager::CheckChangeRoleOnline(long long uuid)
{
	int online_type = World::GetInstWorld()->GetSceneManager()->GetOnLineType(uuid);

	auto it = m_single_chat_be_listen_role_map.find(uuid);
	if (it != m_single_chat_be_listen_role_map.end())
	{
		for (auto target_it : it->second)
		{
			this->SendOnlineInfo(target_it, uuid, online_type);
		}
	}
}

void ChatManager::SendOnlineInfo(long long send_to_uuid, long long uuid, int online_type)
{
	Protocol::SCSingleChatOnlineStatus msg;
	msg.online_type = online_type;
	msg.uuid = uuid;

	UniqueUserID unique_userid  = LongLongToUniqueUserID(send_to_uuid);
	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(unique_userid.user_id);
	if (NULL != target_role)
	{
		EngineAdapter::Instance().NetSend(target_role->GetNetId(), (const char *)&msg, sizeof(Protocol::SCSingleChatOnlineStatus));
	}
	else
	{
		if (!CrossConfig::Instance().IsHiddenServer())
		{
			std::map<long long, CrossUserInfo> &cross_user_map = World::GetInstWorld()->GetSceneManager()->GetCrossUserMap();
			if (cross_user_map.find(send_to_uuid) != cross_user_map.end())
			{
				InternalComm::Instance().UserProtocalTransferToGame(send_to_uuid, (const char *)&msg, sizeof(Protocol::SCSingleChatOnlineStatus));
			}
		}
	}
}

bool ChatManager::GetFromUUIDByCrossSingleChatBuff(const char * buff, size_t len, long long *from_uuid)
{
	if (NULL == buff || NULL == from_uuid)
	{
		return false;
	}
	size_t len0 = sizeof(crossgameprotocal::CrossGameSingleChat);
	size_t len1 = sizeof(Protocol::SCSingleChat);
	if (len <= len0 + len1)
	{
		return false;
	}
	Protocol::SCSingleChat sc_single_chat;
	memcpy(&sc_single_chat, buff + len0, len1);
	*from_uuid = sc_single_chat.from_uuid;
	return true;
}

bool ChatManager::GetFromUUIDBySingleChatBuff(const char * buff, size_t len, long long * from_uuid)
{
	if (NULL == buff || NULL == from_uuid)
	{
		return false;
	}
	size_t len0 = sizeof(Protocol::SCSingleChat);
	if (len <= len0)
	{
		return false;
	}
	Protocol::SCSingleChat sc_single_chat;
	memcpy(&sc_single_chat, buff, len0);
	*from_uuid = sc_single_chat.from_uuid;
	return true;
}

// void ChatManager::CheckGuildChatBoard(Role *role)
// {
// 	if (NULL == role)
// 	{
// 		return;
// 	}
// 
// 	MsgBoardMapIt board_it = m_guild_chat_board_map.find(role->GetGuildID());
// 	if (m_guild_chat_board_map.end() == board_it)
// 	{
// 		return;
// 	}
// 
// 	MsgBoardItemQueue &msg_item_queue = board_it->second;
// 
// 	for (MsgBoardItemQueueIt it = msg_item_queue.begin(); msg_item_queue.end() != it; ++ it)
// 	{
// 		int send_len = 0;
// 		const char *send_buffer = it->GetSendBuffer(&send_len);
// 		if (send_len > 0 && NULL != send_buffer)
// 		{
// 			EngineAdapter::Instance().NetSend(role->GetNetId(), send_buffer, send_len);
// 		}
// 	}
// }

void ChatManager::CheckGuildChatBoard(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	MsgBoardMapIt board_it = m_guild_chat_board_map.find(role->GetGuildID());
	if (m_guild_chat_board_map.end() == board_it)
	{
		return;
	}


	int send_msg_len = 0;			// 消息总长度
	static char buff[MAX_MSG_SIZE]; // 消息内容

	Protocol::SCChatBoardListInfo *bli = (Protocol::SCChatBoardListInfo*)buff;
	bli->header.msg_type = Protocol::MT_CHAT_BOARD_LIST_SC;
	bli->channel_type = chatdef::CHANNEL_TYPE_GUILD;
	bli->list_count = 0;

	send_msg_len += sizeof(Protocol::SCChatBoardListInfo);

	MsgBoardItemQueue &msg_item_queue = board_it->second;

	for (MsgBoardItemQueueIt it = msg_item_queue.begin(); msg_item_queue.end() != it; ++it)
	{
		int msg_len = 0;
		const char *send_buffer = it->GetSendBuffer(&msg_len);
		if (msg_len > 0 && NULL != send_buffer)
		{
			if (send_msg_len + msg_len >= MAX_MSG_SIZE) break;

			*(int *)(buff + send_msg_len) = msg_len;
			send_msg_len += sizeof(int);
			memcpy(buff + send_msg_len, send_buffer, msg_len);   // 拷贝协议内容
			send_msg_len += msg_len;

			bli->list_count++;
		}
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)buff, send_msg_len);
}

void ChatManager::SyncChatOpenLevelToClient(Role *role)
{
	static Protocol::SCOpenLevelLimit cmd;
	cmd.ignoreLevelLimit = GLOBALCONFIG->GetIgnoreLevelLimitChongzhi();

	cmd.is_forbid_audio_chat = WorldStatus::Instance().GetAudioChatForbidFlag();
	cmd.is_forbid_change_avatar = LOGIC_CONFIG->GetAgentAdapterConfig().IsForbidChagneAvatar() ? 1 : 0;
	cmd.chat_limit_condition_type = WorldStatus::Instance().GetChatLimitConditionTypeFlag();
	memset(cmd.open_level_list, 0, sizeof(cmd.open_level_list));
	memset(cmd.vip_level_list, 0, sizeof(cmd.vip_level_list));
	memset(cmd.forbid_time_info_list, 0, sizeof(cmd.forbid_time_info_list));

	for (int i = OPENLEVEL_LIMIT_TYPE_WORLD; i < OPENLEVEL_LIMIT_TYPE_MAX; ++i)
	{
		const ChannelChatLimitCfg *channel_chat_limit_cfg = LOGIC_CONFIG->GetAgentAdapterConfig().GetChannelChatLimitCfg(i);
		if (nullptr != channel_chat_limit_cfg)
		{
			cmd.open_level_list[i] = channel_chat_limit_cfg->level_limit;
			cmd.vip_level_list[i] = channel_chat_limit_cfg->vip_level_limit;
			cmd.forbid_time_info_list[i].begin_hour = channel_chat_limit_cfg->forbid_chat_begin_hour;
			cmd.forbid_time_info_list[i].end_hour = channel_chat_limit_cfg->forbid_chat_end_hour;
		}
	}

	WorldStatus::Instance().GetChannelLevelLimit(cmd.open_level_list, sizeof(cmd.open_level_list) / sizeof(cmd.open_level_list[0]));
	WorldStatus::Instance().GetChannelVipLevelLimit(cmd.vip_level_list, sizeof(cmd.vip_level_list) / sizeof(cmd.vip_level_list[0]));

	cmd.is_forbid_cross_speaker = LOGIC_CONFIG->GetAgentAdapterConfig().IsForBidCrossSpeaker() ? 1 : 0;
	cmd.reserve_sh = 0;

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&cmd, sizeof(cmd));
	}
	else
	{
		World::GetInstWorld()->SendToAllGateway((const char*)&cmd, sizeof(cmd));
	}
}

