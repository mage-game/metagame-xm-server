#include "redpapermanager.h"
#include <set>
#include "global/offlineregister/offlineregistermanager.hpp"
#include "obj/character/role.h"
#include "world.h"
#include "scene/scene.h"

#include "servercommon/errornum.h"
#include "servercommon/commondata.hpp"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/userprotocal/msgcode.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "servercommon/userprotocal/msgsystem.h"
#include "servercommon/serverconfig/crossconfig.hpp"

#include "config/logicconfigmanager.hpp"
#include "other/commandspeaker/commandspeakerconfig.hpp"
#include "globalconfig/globalconfig.h"
#include "config/sharedconfig/sharedconfig.h"

#include "protocal/msgscguild.hpp"
#include "chat/chatmanager.h"
#include "global/usercache/usercache.hpp"
#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"
#include "global/guild/guild.hpp"
#include "global/guild/roleguild.hpp"
#include "other/route/mailroute.hpp"
#include "other/vip/vip.hpp"
#include "other/tuhaojin/tuhaojin.hpp"
#include "other/bigchatface/bigchatface.hpp"
#include "other/personalizewindow/personalizewindow.hpp"

RedPaperManager::RedPaperManager() 
	: m_red_paper_id(0), m_last_check_invalid_red_paper_timestamp(0)
{

}

RedPaperManager::~RedPaperManager()
{

}

RedPaperManager & RedPaperManager::Instance()
{
	static RedPaperManager _instance; 
	return _instance;
}

int RedPaperManager::OnCreateRedPaper(Role *role, int red_paper_type, int gold_num, int can_fetch_times, bool is_system_provide, int is_bind_gold, int boss_id)
{
	if (NULL == role || red_paper_type <= RED_PAPER_TYPE_INVALID || red_paper_type >= RED_PAPER_TYPE_MAX)
	{
		return 0;
	}

	if (gold_num <= 0 || can_fetch_times <= 0 || gold_num < can_fetch_times)
	{
		return 0;
	}

	if (RED_PAPER_TYPE_COMMON == red_paper_type && 0 != (gold_num % can_fetch_times))
	{
		return 0;
	}

	//单笔金额限制200--手气红包平均每笔不超过200
	if (RED_PAPER_TYPE_GUILD_PAPER != red_paper_type && RED_PAPER_TYPE_GLOBAL != red_paper_type && (gold_num / can_fetch_times) > RED_PAPER_ONCE_MAX_GOLD_NUM)
	{
		role->NoticeNum(errornum::EN_RED_PAPER_ONCE_GOLD_NUM_LIMIT);
		return 0;
	}
	//用户发红包消耗的元宝总数限制1000 
	if (RED_PAPER_TYPE_GUILD_PAPER != red_paper_type && RED_PAPER_TYPE_GLOBAL != red_paper_type && role->GetCommonDataParam()->day_guild_red_paper_consume_gold_num + gold_num > RED_PAPER_MAX_GOLD_NUM)
	{
		return 0;
	}

	if (RED_PAPER_TYPE_GUILD_PAPER != red_paper_type && RED_PAPER_TYPE_GLOBAL != red_paper_type && can_fetch_times > RED_PAPER_MAX_CAN_FETCH_TIMES)
	{
		role->NoticeNum(errornum::EN_RED_PAPER_CAN_FETCH_TIMES_LIMIT);
		return 0;
	}

	if (RED_PAPER_TYPE_GLOBAL != red_paper_type)
	{
		Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
		if (NULL == guild)
		{
			role->NoticeNum(errornum::EN_GUILD_NOT_JOIN_GUILD);
			return 0;
		}

		if (role->GetKnapsack()->IsLock())
		{
			role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
			return 0;
		}

		if (!is_system_provide && !role->GetKnapsack()->GetMoney()->GoldMoreThan(gold_num))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return 0;
		}
	}

	int red_paper_id = this->CreateRedPaperID();
	if (m_red_paper_map.end() != m_red_paper_map.find(red_paper_id))
	{
		return 0;
	}

	if (RED_PAPER_TYPE_GLOBAL != red_paper_type)
	{
		if (!is_system_provide && !role->GetKnapsack()->GetMoney()->UseGold(gold_num, "CreateRedPaper", false))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return 0;
		}
	}

	RedPaper red_paper;
	red_paper.id = red_paper_id;
	red_paper.type = red_paper_type;
	red_paper.is_bind_gold = is_bind_gold;
	red_paper.total_gold_num = gold_num;
	red_paper.can_fetch_times = can_fetch_times;
	role->GetName(red_paper.creater_name);
	red_paper.creater_guild_id = role->GetGuildID();
	red_paper.creater_avator_timestamp = role->GetAvatarTimeStamp();
	red_paper.creater_sex = role->GetSex();
	red_paper.creater_prof = role->GetProf();
	red_paper.reserve = 0;
	red_paper.boss_id = boss_id;

	if (RED_PAPER_TYPE_GUILD_PAPER == red_paper_type)
	{
		red_paper.timeount_timestamp = EngineAdapter::Instance().Time() + SECOND_PER_DAY;
	}
	else
	{
		red_paper.timeount_timestamp = EngineAdapter::Instance().Time() + RED_PAPER_MAX_VALID_TIME_S;
	}

	if (is_system_provide)
	{
		red_paper.creater_uid = 0;
	}
	else
	{
		red_paper.creater_uid = role->GetUID();
	}

	m_red_paper_map[red_paper_id] = red_paper;

	if (RED_PAPER_TYPE_GLOBAL != red_paper_type)
	{
		this->SendRedPaperDetailInfo(NULL, role->GetGuildID(), red_paper_id, Protocol::SCRedPaperDetailInfo::NOTIFY_REASON_CREATE);

		{
			int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_red_paper_create_notice, 
				role->GetUID(), role->GetName(), (int)role->GetCamp(), red_paper.type, red_paper.total_gold_num, red_paper.can_fetch_times);

			if (len > 0) 
			{
				Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
				if (NULL != guild)
				{
					guild->GuildSystemMsg(len, gamestring::GAMESTRING_BUF);
				}
			}
		}
	}
	else
	{
		this->SendRedPaperDetailInfo(NULL, INVALID_GUILD_ID, red_paper_id, Protocol::SCRedPaperDetailInfo::NOTIFY_REASON_CREATE);

		{
			int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_red_paper_create_notice2, 
				role->GetUID(), role->GetName(), (int)role->GetCamp(), red_paper.type, red_paper.total_gold_num, red_paper.can_fetch_times);

			if (len > 0) 
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_ROLL);
			}
		}
	}
	
	if (!is_system_provide && RED_PAPER_TYPE_GLOBAL != red_paper_type)
	{
		//subowu--每天红包金额增加
		role->GetCommonDataParam()->day_guild_red_paper_consume_gold_num += gold_num;
		this->SendRedPaperRoleInfo(role);
	}

	return red_paper_id;
}

int RedPaperManager::OnCreateSceneRedPaper(Role *role, int red_paper_type, int gold_num, int can_fetch_times, bool is_system_provide, int is_bind_gold, int boss_id)
{
	if (NULL == role) return 0;

	int red_paper_id = this->CreateRedPaperID();
	if (m_red_paper_map.end() != m_red_paper_map.find(red_paper_id))
	{
		return 0;
	}

	RedPaper red_paper;
	red_paper.type = red_paper_type;
	red_paper.is_bind_gold = is_bind_gold;
	red_paper.total_gold_num = gold_num;
	red_paper.can_fetch_times = can_fetch_times;
	red_paper.timeount_timestamp = EngineAdapter::Instance().Time() + RED_PAPER_MAX_VALID_TIME_S;
	red_paper.id = red_paper_id;
	red_paper.creater_guild_id = role->GetGuildID();
	red_paper.creater_avator_timestamp = role->GetAvatarTimeStamp();
	red_paper.creater_sex = role->GetSex();
	red_paper.creater_prof = role->GetProf();
	red_paper.reserve = 0;
	red_paper.boss_id = boss_id;

	if (is_system_provide)
	{
		red_paper.creater_uid = 0;
		F_STRNCPY(red_paper.creater_name, gamestring::g_worldboss_red_paper_reward, sizeof(GameName));
	}
	else
	{
		if (!role->GetKnapsack()->GetMoney()->UseGold(gold_num, "CreateRedPaper", false))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return false;
		}

		red_paper.creater_uid = role->GetUID();
		role->GetName(red_paper.creater_name);
		role->GetCommonDataParam()->day_guild_red_paper_consume_gold_num += gold_num;
		this->SendRedPaperRoleInfo(role);
	}

	m_red_paper_map[red_paper_id] = red_paper;

	if (INVALID_GUILD_ID != role->GetGuildID())
	{
		this->SendRedPaperDetailInfo(role, role->GetGuildID(), red_paper_id, Protocol::SCRedPaperDetailInfo::NOTIFY_REASON_CREATE, true);
	}
	else
	{
		this->SendRedPaperDetailInfo(role, INVALID_GUILD_ID, red_paper_id, Protocol::SCRedPaperDetailInfo::NOTIFY_REASON_CREATE, true);
	}

	return red_paper_id;
}

int RedPaperManager::OnCreateCommandRedPaper(Role *role, void* msg, int length)
{
	if (NULL == role)
	{
		return 0;
	}

	const CommandSpeakerOtherCfg *other_cfg = LOGIC_CONFIG->GetCommandSpeakerConfig().GetCommandSpeakerOtherCfg();
	if (NULL == other_cfg)
	{
		return 0;
	}

	if (role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return 0;
	}

	if (length >= RED_PAPER_COMMAND_CONTEXT_MAX_LENGTH)
	{
		gamelog::g_log_debug.printf(LL_DEBUG, "OnCommandSpeaker msg_length:%d is too long.", length);
		return 0;
	}

	int red_paper_id = this->CreateRedPaperID();
	if (m_red_paper_map.end() != m_red_paper_map.find(red_paper_id))
	{
		return 0;
	}

	if (!role->GetKnapsack()->GetMoney()->UseGold(other_cfg->consume, "CreateCommandRedPaper", false))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return 0;
	}

	//创建红包
	RedPaper red_paper;
	red_paper.id = red_paper_id;
	red_paper.type = RED_PAPER_TYPE_COMMAND_SPEAKER;
	red_paper.total_gold_num = other_cfg->bind_gold_num * other_cfg->reward_role_limit;
	red_paper.is_bind_gold = 1; 
	red_paper.timeount_timestamp = EngineAdapter::Instance().Time() + RED_PAPER_MAX_VALID_TIME_S;
	red_paper.can_fetch_times = other_cfg->reward_role_limit;
	role->GetName(red_paper.creater_name);
	red_paper.creater_guild_id = role->GetGuildID();
	red_paper.creater_avator_timestamp = role->GetAvatarTimeStamp();
	red_paper.creater_sex = role->GetSex();
	red_paper.creater_prof = role->GetProf();
	red_paper.reserve = 0;
	red_paper.creater_uid = role->GetUID();

	length++;
	F_STRNCPY(red_paper.command_context, (char *)msg + sizeof(Protocol::CSCreateCommandRedPaper), length);

	m_red_paper_map[red_paper_id] = red_paper;

	//发喇叭
	static Protocol::SCSpeaker speaker_ack;
	speaker_ack.from_uid = UidToInt(role->GetUserId());
	role->GetName(speaker_ack.username);
	speaker_ack.sex = role->GetSex();
	speaker_ack.camp = role->GetCamp();
	speaker_ack.camp_post = 0;
	speaker_ack.guild_post = role->GetGuildPost();
	speaker_ack.prof = role->GetProf();
	speaker_ack.authourity_type = role->GetAuthority();
	speaker_ack.vip_level = role->GetVip()->GetVipLevel();
	speaker_ack.content_type = Protocol::CHAT_CONTENT_TYPE_TEXT;
	speaker_ack.avatar_timestamp = role->GetAvatarTimeStamp();
	speaker_ack.speaker_type = Protocol::CSSpeaker::SPEAKER_TYPE_LOCAL;
	speaker_ack.tuhaojin_color = role->GetTuHaoJin()->GetTuhaojinColor();
	speaker_ack.bigchatface_status = role->GetBigChatFace()->GetBigChatFaceStatus();
	speaker_ack.personalize_window_type = role->GetPersonalizeWindow()->GetCurUseBubbleWindow();
	speaker_ack.personalize_window_bubble_type = role->GetPersonalizeWindow()->GetCurUseBubbleWindow();
	speaker_ack.personalize_window_avatar_type = role->GetPersonalizeWindow()->GetCurUseAvatarWindowForClient();
	LocalConfig::Instance().GetPlatName(speaker_ack.plat_name);
	speaker_ack.server_id = role->GetUserId().db_index;
	speaker_ack.speaker_msg_length = length;
	F_STRNCPY(speaker_ack.speaker_msg, red_paper.command_context, sizeof(speaker_ack.speaker_msg));

	int sendlen = int(sizeof(speaker_ack) - MAX_SPEAKER_MSG_LENGTH + speaker_ack.speaker_msg_length);
	World::GetInstWorld()->SendToAllGateway((const char*)&speaker_ack, sendlen);

	{
		//传闻
		int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_command_speak_create_command,
			role->GetUID(), role->GetName(), (int)role->GetCamp(), red_paper.command_context);

		if (len > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_ROLL);
		}
	}


	ChatManager::Instance().AddWorldChatBoard((const char *)&speaker_ack, sendlen, true);

	this->SendRedPaperDetailInfo(NULL, INVALID_GUILD_ID, red_paper_id, Protocol::SCRedPaperDetailInfo::NOTIFY_REASON_CREATE);

	return red_paper_id;
}

void RedPaperManager::OnFecthCommandRedPaper(Role *role, int red_paper_id)
{
	if (role->GetLevel() < SHAREDCONFIG->GetOpenServerConfig().GetChannelOpenLevelWorld() && !role->IsIgnoreChatOrMailLevelLimit())
	{
		role->NoticeNum(errornum::EN_ROLE_LEVEL_NOT_ENOUGH);
		return;
	}

	RedPaperMapIt it = m_red_paper_map.find(red_paper_id);
	if (m_red_paper_map.end() != it)
	{
		//在世界频道发口令
		Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)gamestring::GAMESTRING_BUF;
		F_STRNCPY(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSChannelChat), it->second.command_context, strlen(it->second.command_context) + 1);
		cc->msg_length = strlen(it->second.command_context) + 1;

		if (cc->msg_length > 0)
		{
			cc->channel_type = chatdef::CHANNEL_TYPE_WORLD;
			ChatManager::Instance().OnChannelChat(role, cc, sizeof(Protocol::CSChannelChat) + cc->msg_length, chatdef::ORIGIN_TYPE_NORMAL_CHAT);
		}

		this->OnFetchRedPaper(role, red_paper_id);
	}
}

void RedPaperManager::OnCheckCommandRedPaper(Role *role, int red_paper_id)
{
	if (role == nullptr)
	{
		return;
	}

	if (role->GetLevel() < SHAREDCONFIG->GetOpenServerConfig().GetChannelOpenLevelWorld() && !role->IsIgnoreChatOrMailLevelLimit())
	{
		role->NoticeNum(errornum::EN_ROLE_LEVEL_NOT_ENOUGH);
		return;
	}

	RedPaperMapIt it = m_red_paper_map.find(red_paper_id);
	if (m_red_paper_map.end() == it)
	{
		role->NoticeNum(errornum::EN_COMMAND_SPEAKER_RED_PAPER_TIME_LIMIT);
		return;
	}

	Protocol::SCCommandRedPaperSendInfo *crpsi = (Protocol::SCCommandRedPaperSendInfo *)gamestring::GAMESTRING_BUF;
	crpsi->header.msg_type = Protocol::MT_RED_PAPER_COMMAND_SEND_INFO_SC;

	crpsi->creater_uid = it->second.creater_uid;
	F_STRNCPY(crpsi->creater_name, it->second.creater_name, sizeof(crpsi->creater_name));
	crpsi->creater_avator_timestamp = it->second.creater_avator_timestamp;
	crpsi->msg_len = strlen(it->second.command_context) + 1;
	crpsi->creater_sex = it->second.creater_sex;
	crpsi->creater_prof = it->second.creater_prof;
	crpsi->red_paper_id = red_paper_id;
	F_STRNCPY(gamestring::GAMESTRING_BUF + sizeof(Protocol::SCCommandRedPaperSendInfo), it->second.command_context, crpsi->msg_len);

	EngineAdapter::Instance().NetSend(role->GetNetId(), gamestring::GAMESTRING_BUF, sizeof(Protocol::SCCommandRedPaperSendInfo) + crpsi->msg_len);
}

void RedPaperManager::OnFetchRedPaper(Role *role, int red_paper_id)
{
	if (NULL == role)
	{
		return;
	}

	int notify_reason = Protocol::SCRedPaperFetchResult::NOTIFY_REASON_FAIL;
	int fetch_gold_num = 0;
	int red_paper_type = RED_PAPER_TYPE_INVALID;
	int red_paper_id_client = red_paper_id;
	GameName creater_name; memset(creater_name, 0, sizeof(creater_name));

	RedPaperMapIt it = m_red_paper_map.find(red_paper_id);
	if (m_red_paper_map.end() != it)
	{
		RedPaper &red_paper = it->second;

		red_paper_type = red_paper.type;
		red_paper_id_client = red_paper.id;

		if (RED_PAPER_TYPE_GLOBAL != red_paper.type && RED_PAPER_TYPE_COMMAND_SPEAKER != red_paper.type)
		{
			if (red_paper.creater_guild_id != role->GetGuildID())
			{
				return;
			}
		}

		F_STRNCPY(creater_name, red_paper.creater_name, sizeof(creater_name));

		if (red_paper.fetch_user_map.end() == red_paper.fetch_user_map.find(role->GetUID()))
		{
			if (RED_PAPER_TYPE_GUILD_PAPER == red_paper.type || red_paper.creater_uid != role->GetUID())
			{
				int has_fetch_times = static_cast<int>(red_paper.fetch_user_map.size());
				int left_gold = red_paper.total_gold_num - red_paper.fetch_gold_num;
				int left_fetch_times = red_paper.can_fetch_times - has_fetch_times;

				if (left_fetch_times <= 0)
				{
					notify_reason = Protocol::SCRedPaperFetchResult::NOTIFY_REASON_NO_FETCH_TIMES;
				}
				else
				{
					if (left_gold > 0 && left_fetch_times > 0 && left_gold >= left_fetch_times && 
						EngineAdapter::Instance().Time() < red_paper.timeount_timestamp)
					{
						int can_fetch_gold_num = 0;
						{
							if (RED_PAPER_TYPE_COMMON == red_paper.type || RED_PAPER_TYPE_COMMAND_SPEAKER == red_paper.type)
							{
								can_fetch_gold_num = left_gold / left_fetch_times;
							}
							else if (RED_PAPER_TYPE_RAND == red_paper.type || RED_PAPER_TYPE_GUILD_PAPER == red_paper.type)
							{
								if (1 == left_fetch_times)
								{
									can_fetch_gold_num = left_gold;
								}
								else
								{
									can_fetch_gold_num = 1;

									int rand_base = 0;
									if (left_fetch_times > 1)
									{
										rand_base = (left_gold - left_fetch_times + 1) / left_fetch_times;
									}

									if (rand_base > 0)
									{
										can_fetch_gold_num = static_cast<unsigned int>(RandomNum(50, 150) * 0.01f * rand_base + 1);
									}		
								}
							}
							else if (RED_PAPER_TYPE_GLOBAL == red_paper.type)
							{
								if (left_fetch_times > 0)
								{
									can_fetch_gold_num = RandomNum(2, 6);
								}
							}
						}
						if (can_fetch_gold_num > 0)
						{
							if (RED_PAPER_TYPE_GLOBAL != red_paper.type)
							{
								if (red_paper.is_bind_gold > 0)
								{
									role->GetKnapsack()->GetMoney()->AddGoldBind(can_fetch_gold_num, "FetchRedPaper");
								}
								else
								{
									role->GetKnapsack()->GetMoney()->AddGold(can_fetch_gold_num, "FetchRedPaper");
								}
							}
							else
							{
								role->GetKnapsack()->GetMoney()->AddGoldBind(can_fetch_gold_num, "FetchRedPaper");
							}

							notify_reason = Protocol::SCRedPaperFetchResult::NOTIFY_REASON_FETCH_SUCC;
							fetch_gold_num = can_fetch_gold_num;

							red_paper.fetch_user_map[role->GetUID()].gold_num = fetch_gold_num;
							red_paper.fetch_user_map[role->GetUID()].avator_timestamp = role->GetAvatarTimeStamp();
							role->GetName(red_paper.fetch_user_map[role->GetUID()].user_name);
							red_paper.fetch_gold_num += fetch_gold_num;
						}
					}
				}
			}
		}
		else
		{
			notify_reason = Protocol::SCRedPaperFetchResult::NOTIFY_REASON_HAS_FETCH;
		}

		if (Protocol::SCRedPaperFetchResult::NOTIFY_REASON_FETCH_SUCC == notify_reason && fetch_gold_num > 0)
		{
			UserCacheNode *creater_user_node = UserCacheManager::Instance().GetUserNode(red_paper.creater_uid);
			if (NULL != creater_user_node)
			{
				int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_red_paper_fetch_succ_notice, 
					role->GetUID(), role->GetName(), (int)role->GetCamp(), 
					creater_user_node->uid, creater_user_node->user_name, creater_user_node->camp, fetch_gold_num);

				if (len > 0) 
				{
					if (RED_PAPER_TYPE_GLOBAL != red_paper.type)
					{
						Guild *guild = GuildManager::Instance().GetGuild(red_paper.creater_guild_id);
						if (NULL != guild)
						{
							guild->GuildSystemMsg(len, gamestring::GAMESTRING_BUF);
						}
					}
					else
					{
						const static int GLOBAL_RED_PAPER_BROCAST_GOLD_NUM = 10;

						if (fetch_gold_num >= GLOBAL_RED_PAPER_BROCAST_GOLD_NUM)
						{
							World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
						}
					}
				}
			}
		}
	}

	Protocol::SCRedPaperFetchResult rpfr;
	rpfr.notify_reason = notify_reason;
	rpfr.fetch_gold = fetch_gold_num;
	F_STRNCPY(rpfr.creater_name, creater_name, sizeof(rpfr.creater_name));
	rpfr.red_paper_type = red_paper_type;
	rpfr.red_paper_id = red_paper_id_client;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&rpfr, sizeof(rpfr));

	if (RED_PAPER_TYPE_GUILD_PAPER == red_paper_type)
	{
		role->GetRoleGuild()->NoticeGuildRedPaperInfo(Protocol::SCNoticeGuildPaperInfo::HAS_FETCH_RED_PAPER);
	}
}

void RedPaperManager::OnQueryRedPaperDetailInfo(Role *role, int red_paper_id)
{
	this->SendRedPaperDetailInfo(role, INVALID_GUILD_ID, red_paper_id);
}

void RedPaperManager::Update(unsigned long interval, time_t now_second)
{
	this->CheckInvalidRedPaper(now_second);
}

void RedPaperManager::OnStopServer()
{
	this->CheckInvalidRedPaper(EngineAdapter::Instance().Time() + 2 * RED_PAPER_MAX_VALID_TIME_S);
}

void RedPaperManager::SendRedPaperRoleInfo(Role *role)
{
	if (NULL == role) return;

	static Protocol::SCRedPaperRoleInfo rpdi;

	rpdi.day_guild_red_paper_consume_gold_num = role->GetCommonDataParam()->day_guild_red_paper_consume_gold_num;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&rpdi, sizeof(rpdi));
}

void RedPaperManager::SendRedPaperDetailInfo(Role *role, int guild_id, int red_paper_id, int notify_reason, bool is_scene)
{
	RedPaperMapIt it = m_red_paper_map.find(red_paper_id);
	if (m_red_paper_map.end() == it)
	{
		return;
	}

	const RedPaper &red_paper = it->second;

	static Protocol::SCRedPaperDetailInfo rpdi;
	rpdi.notify_reason = notify_reason;
	rpdi.id = red_paper.id;
	rpdi.type = red_paper.type;
	rpdi.total_gold_num = red_paper.total_gold_num;
	rpdi.fetch_gold_num = red_paper.fetch_gold_num;
	rpdi.can_fetch_times = red_paper.can_fetch_times;
	rpdi.timeount_timestamp = static_cast<unsigned int>(red_paper.timeount_timestamp);
	rpdi.creater_guild_id = red_paper.creater_guild_id;
	rpdi.creater_avator_timestamp = red_paper.creater_avator_timestamp;
	rpdi.creater_sex= red_paper.creater_sex;
	rpdi.creater_prof = red_paper.creater_prof;
	rpdi.reserve = red_paper.reserve;
	rpdi.boss_id = red_paper.boss_id;
	rpdi.creater_uid = red_paper.creater_uid;
	F_STRNCPY(rpdi.creater_name, red_paper.creater_name, sizeof(rpdi.creater_name)); 
	rpdi.fetch_user_count = 0;

	for (std::map<int, RedPaper::UserInfo>::const_iterator it = red_paper.fetch_user_map.begin(); it != red_paper.fetch_user_map.end() && rpdi.fetch_user_count < RED_PAPER_MAX_CAN_FETCH_TIMES; ++ it)
	{
		rpdi.fetch_user_info_list[rpdi.fetch_user_count].uid= it->first;
		rpdi.fetch_user_info_list[rpdi.fetch_user_count].gold_num = it->second.gold_num;
		rpdi.fetch_user_info_list[rpdi.fetch_user_count].avator_timestamp = it->second.avator_timestamp;
		F_STRNCPY(rpdi.fetch_user_info_list[rpdi.fetch_user_count].fetch_name, it->second.user_name, sizeof(rpdi.fetch_user_info_list[rpdi.fetch_user_count].fetch_name));
		++ rpdi.fetch_user_count;
	}

	int send_len = sizeof(rpdi) - sizeof(rpdi.fetch_user_info_list) + sizeof(rpdi.fetch_user_info_list[0]) * rpdi.fetch_user_count;

	if (is_scene)
	{
		if (INVALID_GUILD_ID != guild_id && NULL != role)
		{
			Scene * scene = role->GetScene();
			if (scene != NULL)
			{
				for(unsigned int i = 0; i < scene->RoleNum(); i++)
				{
					Role *temp_role = scene->GetRoleByIndex(i);
					if (NULL !=  temp_role)
					{
						if (temp_role->GetGuildID() == role->GetGuildID())
						{
							EngineAdapter::Instance().NetSend(temp_role->GetNetId(), (const char *)&rpdi, send_len);
						}
					}
				}
			}
		}
		else if (INVALID_GUILD_ID == guild_id && NULL != role)
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&rpdi, send_len);
		}

		return;
	}

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&rpdi, send_len);
	}

	if (INVALID_GUILD_ID != guild_id)
	{
		Guild *guild = GuildManager::Instance().GetGuild(guild_id);
		if (NULL != guild)
		{
			guild->SendMessageToGuild(Protocol::GUILD_NOTIFY_TYPE_REP_PAPER, (const char *)&rpdi, send_len);
		}
	}
	
	if (INVALID_GUILD_ID == guild_id && NULL == role)
	{
		World::GetInstWorld()->SendToAllGateway((const char *)&rpdi, send_len);
	}
}

void RedPaperManager::CheckInvalidRedPaper(time_t now_second)
{
	UNSTD_STATIC_CHECK(CHECK_INVALID_RED_PAPER_INTERVAL < RED_PAPER_MAX_VALID_TIME_S);

	if (now_second >= m_last_check_invalid_red_paper_timestamp + CHECK_INVALID_RED_PAPER_INTERVAL)
	{
		m_last_check_invalid_red_paper_timestamp = now_second;

		std::set<int> erase_set;

		for (RedPaperMapIt it = m_red_paper_map.begin(); it != m_red_paper_map.end(); ++ it)
		{
			RedPaper &red_paper = it->second;

			if (now_second >= red_paper.timeount_timestamp || ((red_paper.fetch_gold_num >= red_paper.total_gold_num ||
				static_cast<int>(red_paper.fetch_user_map.size()) >= red_paper.can_fetch_times) && 
				RED_PAPER_TYPE_GUILD_PAPER != red_paper.type && RED_PAPER_TYPE_COMMAND_SPEAKER != red_paper.type))
			{
				erase_set.insert(red_paper.id);

				int left_money = red_paper.total_gold_num - red_paper.fetch_gold_num;
				if (left_money > 0 && RED_PAPER_TYPE_GLOBAL != red_paper.type && 0 != red_paper.creater_uid && RED_PAPER_TYPE_COMMAND_SPEAKER != red_paper.type)
				{
					static MailContentParam contentparam; contentparam.Reset();
					
					contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_GOLD, left_money);

					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_red_paper_give_left_money_content, left_money);
					if (length > 0)
					{
						MailRoute::MailToUser(IntToUid(red_paper.creater_uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
					}
				}

				if (red_paper.fetch_gold_num > 0)
				{
					UserCacheNode *creater_user_node = UserCacheManager::Instance().GetUserNode(red_paper.creater_uid);
					if (NULL != creater_user_node)
					{
						Role *creater_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(red_paper.creater_uid));
						if (NULL != creater_role)
						{
							creater_role->AddRedPaperConsumeGold(red_paper.fetch_gold_num, "RoleOnline");
						}
						else
						{
							OfflineRegisterManager::Instance().OnRedPaperConsumeGold(IntToUid(red_paper.creater_uid), red_paper.fetch_gold_num);
						}
					}

					if (RED_PAPER_TYPE_COMMON == red_paper.type)
					{
						if (NULL != creater_user_node)
						{
							int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_red_paper_fetch_over_notice, 
								creater_user_node->uid, creater_user_node->user_name, creater_user_node->camp);

							if (len > 0) 
							{
								Guild *guild = GuildManager::Instance().GetGuild(red_paper.creater_guild_id);
								if (NULL != guild)
								{
									guild->GuildSystemMsg(len, gamestring::GAMESTRING_BUF);
								}
							}
						}
					}
					else if (RED_PAPER_TYPE_RAND == red_paper.type || RED_PAPER_TYPE_GLOBAL == red_paper.type)
					{
						int lucky_uid = 0; int lucky_user_fetch_money = 0;
						{
							// 最幸运角色
							for (std::map<int, RedPaper::UserInfo>::iterator it = red_paper.fetch_user_map.begin(); it != red_paper.fetch_user_map.end(); ++ it)
							{
								if (it->second.gold_num > lucky_user_fetch_money)
								{
									lucky_uid = it->first;
									lucky_user_fetch_money = it->second.gold_num;
								}

								UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(it->first);
								if (NULL != user_node)
								{
									Role *fetch_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(it->first));
									if (NULL != fetch_role)
									{
										fetch_role->AddRedPaperLuckyValue(it->second.gold_num);
									}
									else
									{
										OfflineRegisterManager::Instance().OnAddRedPaperLuckyValue(IntToUid(it->first), it->second.gold_num);
									}
								}
							}
						}
						
						UserCacheNode *lucky_user_node = UserCacheManager::Instance().GetUserNode(lucky_uid);

						if (NULL != lucky_user_node && NULL != creater_user_node)
						{
							int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_red_paper_lucky_user_notice, 
								creater_user_node->uid, creater_user_node->user_name, creater_user_node->camp, 
								lucky_user_node->uid, lucky_user_node->user_name, lucky_user_node->camp, lucky_user_fetch_money);

							if (len > 0) 
							{
								if (RED_PAPER_TYPE_GLOBAL != red_paper.type)
								{
									Guild *guild = GuildManager::Instance().GetGuild(red_paper.creater_guild_id);
									if (NULL != guild)
									{
										guild->GuildSystemMsg(len, gamestring::GAMESTRING_BUF);
									}
								}
								else
								{
									World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_ROLL);
								}
							}
						}
					}
				}
			}
		}

		for (std::set<int>::iterator it = erase_set.begin(); it != erase_set.end(); ++ it)
		{
			m_red_paper_map.erase(*it);
		}
	}
}



