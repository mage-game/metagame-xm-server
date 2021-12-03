#include "guildmembermanager.hpp"
#include "guildmember.hpp"
#include <algorithm>
#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"
#include "global/guild/impl/guildauthority.hpp"

#include "servercommon/struct/structcommon.h"
#include "servercommon/errornum.h"
#include "servercommon/string/globalstr.h"
#include "servercommon/commondata.hpp"
#include "servercommon/chatserverdef.h"

#include "engineadapter.h"
#include "protocal/msgchatmsg.h"
#include "servercommon/userprotocal/chatmsgcode.h"
#include "servercommon/string/gameworldstr.h"

#include "protocal/msgscguild.hpp"
#include "world.h"
#include "gstr.h"
#include "gamelog.h"
#include "obj/character/role.h"
#include "global/mail/mailmanager.hpp"
#include "global/usercache/usercache.hpp"
#include "global/guild/roleguild.hpp"
#include "scene/scene.h"

#include "config/guildconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "chat/chatmanager.h"
#include "other/route/mailroute.hpp"
#include "other/shop/roleshop.hpp"
#include "servercommon/serverconfig/localconfig.hpp"
#include "internalcomm.h"
#include "servercommon/serverconfig/openserverconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "config/agentadapterconfig.hpp"

GuildMemberManager::GuildMemberManager()
	: m_guild(NULL), m_change_flag(false), m_day_invite_count(0), m_total_member_signin_count_today(0)
{
	memset(m_member_list, 0, sizeof(m_member_list));

	memset(m_dirty_mark, 0 ,sizeof(m_dirty_mark));
	memset(m_old_state, 0, sizeof(m_old_state));

	m_guild_saizi_rank_vec.clear();
}

GuildMemberManager::~GuildMemberManager()
{
	for (int i = 0; i < MAX_MEMBER_COUNT; i++)
	{
		if (NULL != m_member_list[i])
		{
			delete m_member_list[i];
		}
	}
}

void GuildMemberManager::Init(const GuildMemberParam & member_param)
{
	int count = 0;

	if (member_param.count < MAX_MEMBER_COUNT)
	{
		count = member_param.count;
	}
	else 
	{
		count = MAX_MEMBER_COUNT;
	}

	this->GetGuild()->GetGuildRegister().OnGuildMemberCountChanged(count);

	m_total_member_signin_count_today = 0;
	unsigned int zero_timestamp = (unsigned int)GetZeroTime(EngineAdapter::Instance().Time());
	for (int i = count - 1; i >= 0; i--)
	{
		int index = (int)member_param.member_list[i].index;
		if (index < 0 || index >= MAX_MEMBER_COUNT) continue;
		if (NULL != m_member_list[index]) continue;

		GuildMember *member_item = new GuildMember(this);
		member_item->Init(member_param.member_list[i]);

		m_member_list[index] = member_item;
		m_old_state[index] = true;

		m_userid_index_map[member_item->GetUserID()] = index;
		if (member_item->GetCommonData().last_signin_timestamp >= zero_timestamp)
		{
			++m_total_member_signin_count_today;
		}
	}

	this->CalcSaiziRank();
}

void GuildMemberManager::GetChangeMemberList(GuildMemberParam *member_param)
{
	if (!m_change_flag)
	{
		member_param->count = 0;
		return;
	}

	int count = 0;
	for (int i = 0; i < MAX_MEMBER_COUNT; ++i)
	{
		if (m_dirty_mark[i])
		{
			if (NULL == m_member_list[i])
			{
				if (!m_old_state[i]) continue;

				member_param->member_list[count].change_state = structcommon::CS_DELETE;
				member_param->member_list[count].index = i;
			}
			else
			{
				member_param->member_list[count].index = i;
				member_param->member_list[count].change_state = m_old_state[i] ? structcommon::CS_UPDATE : structcommon::CS_INSERT;
				m_member_list[i]->GetAttr(&member_param->member_list[count]);
			}

			++count;
		}
	}

	member_param->count = count;
}

void GuildMemberManager::ClearDirtyMark()
{
	if (!m_change_flag) return;

	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));

	for (int i = 0; i < MAX_MEMBER_COUNT; ++i)
	{
		m_old_state[i] = (NULL != m_member_list[i]);
	}

	m_change_flag = false;
}

void GuildMemberManager::OnDayChange()
{
	GuildOtherConfig & other_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig();
	int open_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	if (open_day < other_cfg.kill_cross_boss_open_day)
	{
		static char buff[1024]; memset(buff, 0, sizeof(buff));

		Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)buff;
		cc->channel_type = chatdef::CHANNEL_TYPE_GUILD;
		
		int rank_size = (int)(m_guild_saizi_rank_vec.size());

		if (rank_size == 1)
		{
			UserCacheNode *user_cache_node0 = UserCacheManager::Instance().GetUserNode(m_guild_saizi_rank_vec[0]->GetUserID());
			if (NULL == user_cache_node0) return;

			cc->msg_length = SNPRINTF(buff + sizeof(Protocol::CSChannelChat), sizeof(buff) - sizeof(Protocol::CSChannelChat), gamestring::g_guild_pao_saizi_rank1, 
				user_cache_node0->user_name, m_guild_saizi_rank_vec[0]->GetTodayGuildSaiziScore());
		}
		else if (rank_size == 2)
		{
			UserCacheNode *user_cache_node0 = UserCacheManager::Instance().GetUserNode(m_guild_saizi_rank_vec[0]->GetUserID());
			if (NULL == user_cache_node0) return;

			UserCacheNode *user_cache_node1 = UserCacheManager::Instance().GetUserNode(m_guild_saizi_rank_vec[1]->GetUserID());
			if (NULL == user_cache_node1) return;

			cc->msg_length = SNPRINTF(buff + sizeof(Protocol::CSChannelChat), sizeof(buff) - sizeof(Protocol::CSChannelChat), gamestring::g_guild_pao_saizi_rank2, 
				user_cache_node0->user_name, m_guild_saizi_rank_vec[0]->GetTodayGuildSaiziScore(), 
				user_cache_node1->user_name, m_guild_saizi_rank_vec[1]->GetTodayGuildSaiziScore());
		}
		else if (rank_size >= 3)
		{
			UserCacheNode *user_cache_node0 = UserCacheManager::Instance().GetUserNode(m_guild_saizi_rank_vec[0]->GetUserID());
			if (NULL == user_cache_node0) return;

			UserCacheNode *user_cache_node1 = UserCacheManager::Instance().GetUserNode(m_guild_saizi_rank_vec[1]->GetUserID());
			if (NULL == user_cache_node1) return;

			UserCacheNode *user_cache_node2 = UserCacheManager::Instance().GetUserNode(m_guild_saizi_rank_vec[2]->GetUserID());
			if (NULL == user_cache_node2) return;

			cc->msg_length = SNPRINTF(buff + sizeof(Protocol::CSChannelChat), sizeof(buff) - sizeof(Protocol::CSChannelChat), gamestring::g_guild_pao_saizi_rank3, 
				user_cache_node0->user_name, m_guild_saizi_rank_vec[0]->GetTodayGuildSaiziScore(), 
				user_cache_node1->user_name, m_guild_saizi_rank_vec[1]->GetTodayGuildSaiziScore(),
				user_cache_node2->user_name, m_guild_saizi_rank_vec[2]->GetTodayGuildSaiziScore());
		}

		this->GuildleaderGuildChat(cc, cc->msg_length + sizeof(Protocol::CSChannelChat));
		this->SendSaiziReward();
	}

	
	
	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end(); it != end; ++it)
	{
		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;

		GuildMember *member = m_member_list[it->second];
		if (NULL == member) continue;

		member->OnDayChange();
	}
	
	this->CalcSaiziRank();

	// 骰子信息通知放在roleguild
// 	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end(); it != end; ++it)
// 	{
// 		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;
// 
// 		GuildMember *member = m_member_list[it->second];
// 		if (NULL == member) continue;
// 
// 		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(member->GetUserID());
// 		if (NULL != role)
// 		{
// 			this->SyncSCGulidSaiziInfo(role);
// 		}
// 	}

	m_day_invite_count = 0;
	m_total_member_signin_count_today = 0;
}

void GuildMemberManager::OnMonthChange()
{
	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end(); it != end; ++it)
	{
		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;

		GuildMember *member = m_member_list[it->second];
		if (NULL == member) continue;

		member->OnMonthChange();
	}
}

GuildMember * GuildMemberManager::AddMember(const UserID &user_id, int post)
{
	if (NULL != this->GetMember(user_id)) return NULL;
	if (m_guild->GetGuildRegister().GetGuildMemberCount() >= m_guild->GetGuildMaxMemberCount()) return NULL;

	int i = 0;
	for (i = 0; i < MAX_MEMBER_COUNT; ++i)
	{
		if (NULL == m_member_list[i])
		{
			break;
		}
	}
	if (i >= MAX_MEMBER_COUNT) return NULL;

	time_t now = EngineAdapter::Instance().Time();

	static GuildMemberParam::Member member_param;
	member_param.index = i;
	member_param.uid = UidToInt(user_id);
	member_param.post = post;

	member_param.gongxian = 0;
	member_param.total_gongxian = 0;

	member_param.join_time = now;
	member_param.last_login_time = now;

	member_param.lucky_color = GUILD_MEMBER_LUCKY_COLOR_WHITE;

	member_param.today_guild_pao_saizi_times = 0;
	member_param.today_last_guild_pao_saizi_time = 0;
	member_param.today_guild_saizi_score = 0;

	member_param.maze_layer = 0;
	member_param.maze_complete_time = 0;
	member_param.common_data.Reset();

	GuildMember * member_item = new GuildMember(this);
	member_item->Init(member_param);
	m_member_list[i] = member_item;

	member_item->SetLastLoginTime(now);
	member_item->SetOnline(NULL != World::GetInstWorld()->GetSceneManager()->GetRole(user_id));

	m_change_flag = true;
	m_dirty_mark[i] = true;

	m_userid_index_map[user_id] = i;

	GuildManager::Instance().SyncToRole(user_id, m_guild->GetGuildID(), m_guild->GetGuildName(), post);
	m_guild->GetGuildRegister().OnGuildAddMember();
	m_guild->GetGuildManager()->OnUserJoinGuildRemoveApplyFor(user_id, m_guild->GetGuildID());
	
	m_guild->OnBaseInfoChange();

	Role *be_add_role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
	if (NULL != be_add_role)
	{
		CommonDataParam *common_data = be_add_role->GetCommonDataParam();
		if (NULL != common_data)
		{
			common_data->guild_question_right_num = 0;
		}

		m_guild->BrocastGuildQuestion(be_add_role);
		m_guild->SendGuildQuestionRankInfo(be_add_role);
		be_add_role->SendQuestionRightAnswerNum();
		m_guild->RemoveGuildEnemy(be_add_role->GetUniqueUserID());  // 尝试从仇人列表删除，不然就永远删不掉了
	}

	UserCacheNode *uc = UserCacheManager::Instance().GetUserNode(user_id);
	if (NULL != uc)
	{
		LOG_QUICK8(gamelog::g_log_quick, LOG_TYPE_GUILD_JOIN, uc->uid, uc->user_name, NULL, uc->level, m_guild->GetGuildID(), m_guild->GetGuildName(), "Join");
	}

	{
		if (GUILD_POST_TUANGZHANG != post)
		{
			// 策划需求去掉欢迎邮件
// 			static MailContentParam contentparam; contentparam.Reset();
// 			int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_join_guild_mail_subject);
// 			int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_join_guild_mail_content, m_guild->GetGuildName());
// 			if (length1 > 0 && length2 > 0)
// 			{
// 				UserMailManager::Instance().MailToUser(MAIL_TYPE_SYSTEM, user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);
// 			}

			if (NULL != uc)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_msg_join_guild,
					UidToInt(user_id), uc->user_name, (int)m_guild->GetCamp());
				if (length > 0)
				{
					m_guild->GuildSystemMsg(length, gamestring::GAMESTRING_BUF, 1, &user_id);
				}

				// 策划需求新人入会，随机挑选两个在线的公会成员发欢迎词
				if (this->GetMemberCount() >= 2)
				{
					int member_uid_list[MAX_MEMBER_COUNT];
					memset(member_uid_list, 0, sizeof(member_uid_list));
					int member_total_count = this->GetOnlineMemberList(member_uid_list);
					if (member_total_count == 2)
					{
						for (int i = 0; i < 2; ++i)
						{
							if (member_uid_list[i] != 0 && member_uid_list[i] != UidToInt(user_id))
							{
								this->SendWelcomChat(IntToUid(member_uid_list[i]));
							}
						}
					}
					else if (2 < member_total_count && member_total_count < MAX_MEMBER_COUNT)
					{
						int member_1 = 0;
						int member_2 = 0;

						for(int rand_1 = 0; rand_1 < 1000; rand_1++)
						{
							member_1 = RandomNum(member_total_count);
							if (member_uid_list[member_1] != UidToInt(user_id))
							{
								break;
							}
						}

						for (int rank_2 = 0; rank_2 < 1000; rank_2++)
						{
							member_2 = RandomNum(member_total_count);
							if (member_uid_list[member_1] != member_uid_list[member_2] && UidToInt(user_id) != member_uid_list[member_2])
							{
								break;
							}
						}

						if (member_uid_list[member_1] != 0)
						{
							this->SendWelcomChat(IntToUid(member_uid_list[member_1]));
							
						}

						if (member_uid_list[member_2] != 0)
						{
							this->SendWelcomChat(IntToUid(member_uid_list[member_2]));
						}
					}
				}
			}
		}
	}

	Role * role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
	if (NULL != role)
	{
		this->SyncSCGulidSaiziInfo(role);
	}

	++ m_day_invite_count;
	return member_item;
}

void GuildMemberManager::SendWelcomChat(const UserID &user_id)
{
	if (INVALID_USER_ID == user_id) return;

	int msg_length = 0;
	int i = RandomNum(8);
	switch (i)
	{
	case 0:
		msg_length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_msg_guild_welcome1);
		break;
	case 1:
		msg_length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_msg_guild_welcome2);
		break;
	case 2:
		msg_length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_msg_guild_welcome3);
		break;
	case 3:
		msg_length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_msg_guild_welcome4);
		break;
	case 4:
		msg_length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_msg_guild_welcome5);
		break;
	case 5:
		msg_length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_msg_guild_welcome6);
		break;
	case 6:
		msg_length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_msg_guild_welcome7);
		break;
	case 7:
		msg_length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_msg_guild_welcome8);
		break;
	}

	if (0 >= msg_length)
	{
		return;
	}

	static char buff[sizeof(Protocol::SCChannelChat) + MAX_CHAT_LENGTH + 1] = {0};
	Protocol::SCChannelChat *channel_chat = (Protocol::SCChannelChat*)buff;
	UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(user_id);
	if (NULL == user_node)
	{
		return;
	}

	int plat_type = LocalConfig::Instance().GetPlatType();
	UniqueUserID unique_userid(plat_type, user_id);
	long long uuid = UniqueUserIDToLongLong(unique_userid);
	
	channel_chat->header.msg_type = Protocol::MT_CHANNEL_CHAT_SC;
	channel_chat->from_uid = user_node->uid;
	channel_chat->uuid = uuid;
	channel_chat->channel_type = (char)OPENLEVEL_LIMIT_TYPE_GUILD;
	channel_chat->msg_length = msg_length;
	user_node->GetName(channel_chat->username);
	channel_chat->sex = user_node->sex;
	channel_chat->camp = user_node->camp;
	channel_chat->vip_level = user_node->vip_level;
	channel_chat->prof = user_node->prof;
	channel_chat->authority_type = 0;
	channel_chat->level = user_node->level;
	channel_chat->avatar_timestamp = user_node->avatar_timestamp;
	channel_chat->content_type = 0;
	channel_chat->tuhaojin_color = 0;
	channel_chat->bigchatface_status = 0;
	channel_chat->personalize_window_bubble_type = (char)user_node->cur_use_bubble_type;
	channel_chat->personalize_window_avatar_type = (char)user_node->cur_use_avatar_type;
	channel_chat->origin_type = chatdef::ORIGIN_TYPE_NORMAL_CHAT;
	channel_chat->has_xianzunka_flag = user_node->has_xianzunka_flag;
	channel_chat->msg_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	memcpy(((char*)buff) + sizeof(Protocol::SCChannelChat), gamestring::GAMESTRING_BUF, msg_length);

	int total_len = sizeof(Protocol::SCChannelChat) + msg_length;
	this->OnChat(buff, total_len);
	return;
}

bool GuildMemberManager::RemoveMember(const UserID &user_id)
{
	UserIDIndexMapIt it = m_userid_index_map.find(user_id);
	if (it == m_userid_index_map.end()) return false;

	if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) return false;
	if (NULL == m_member_list[it->second]) return false;

	GuildManager::Instance().SyncToRole(user_id, INVALID_GUILD_ID, "", 0);
	m_guild->GetGuildRegister().OnGuildRemoveMember();
	m_guild->RemoveMemberSyncPaperInfo(UidToInt(user_id));
	m_guild->RemoveMemberSyncQuestionRankInfo(UidToInt(user_id));

	// 减少相应的工资
	int member_gongzi = m_member_list[it->second]->GetGongzi();
	m_guild->AddGuildGongzi(-member_gongzi);
	
	delete m_member_list[it->second];
	m_member_list[it->second] = NULL;

	m_change_flag = true;
	m_dirty_mark[it->second] = true;

	m_userid_index_map.erase(it);

	m_guild->OnBaseInfoChange();

	this->CalcSaiziRank();
	{
		UserCacheNode *uc = UserCacheManager::Instance().GetUserNode(UidToInt(user_id));
		if (NULL != uc)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_msg_leave_guild,
				UidToInt(user_id), uc->user_name, (int)m_guild->GetCamp());
			if (length > 0)
			{
				m_guild->GuildSystemMsg(length, gamestring::GAMESTRING_BUF, 1, &user_id);
			}

			LOG_QUICK8(gamelog::g_log_quick, LOG_TYPE_GUILD_JOIN, uc->uid, uc->user_name, NULL, uc->level, m_guild->GetGuildID(), m_guild->GetGuildName(), "Leave");
		}
	}

	return true;
}

GuildMember * GuildMemberManager::GetMember(const UserID &user_id, int *member_index)
{
	UserIDIndexMapIt it = m_userid_index_map.find(user_id);
	if (it == m_userid_index_map.end() || it->second < 0 || it->second >= MAX_MEMBER_COUNT) return NULL;

	if (NULL != member_index) *member_index = it->second;

	return m_member_list[it->second];
}

static GuildMember * CmpPriority(GuildMember *member1, GuildMember *member2)
{
	if (NULL == member1 || NULL == member2) return NULL;

	if (member1->GetPost() == member2->GetPost())
	{
		if (member1->GetTotalGongxian() == member2->GetTotalGongxian())
		{
			if (member1->GetJoinTime() == member2->GetJoinTime())
			{
				int cap2 = 0, cap1 = 0;
				UserCacheNode *uc = UserCacheManager::Instance().GetUserNode(UidToInt(member1->GetUserID()));
				if (NULL != uc)
				{
					cap1 = uc->capability;
				}

				uc = UserCacheManager::Instance().GetUserNode(UidToInt(member2->GetUserID()));
				if (NULL != uc)
				{
					cap2 = uc->capability;
				}

				if (cap2 > cap1) return member2;
				return member1;
			}
			else 
			{
				if (member2->GetJoinTime() < member1->GetJoinTime()) return member2;
				return member1;
			}
		}
		else 
		{
			if (member2->GetTotalGongxian() > member1->GetTotalGongxian()) return member2;
			return member1;
		}
	}
	else 
	{
		if (member2->GetPost() > member1->GetPost()) return member2;
		return member1;
	}
}

GuildMember * GuildMemberManager::GetReplaceTuanZhangMember()
{
	GuildMember *replace_member = NULL;

	for (int i = 0; i < MAX_MEMBER_COUNT; i++)
	{
		if (NULL != m_member_list[i] && GUILD_POST_TUANGZHANG != m_member_list[i]->GetPost())
		{
			if (NULL == replace_member)
			{
				replace_member = m_member_list[i];
			}
			else 
			{
				replace_member = CmpPriority(replace_member, m_member_list[i]);
			}
		}
	}

	return replace_member;
}

GuildMember * GuildMemberManager::GetTuanZhangMember()
{
	for (int i = 0; i < MAX_MEMBER_COUNT; i++)
	{
		if (NULL != m_member_list[i] && GUILD_POST_TUANGZHANG == m_member_list[i]->GetPost())
		{
			return m_member_list[i];
		}
	}
	
	return NULL;
}

GuildMember * GuildMemberManager::GetFuTuanZhangMember()
{
	for (int i = 0; i < MAX_MEMBER_COUNT; i++)
	{
		if (NULL != m_member_list[i] && GUILD_POST_FU_TUANGZHANG == m_member_list[i]->GetPost())
		{
			return m_member_list[i];
		}
	}

	return NULL;
}

int GuildMemberManager::GetTuanZhangUid()
{
	GuildMember *guild_menber = this->GetTuanZhangMember();
	if (NULL == guild_menber) return 0;

	return UidToInt(guild_menber->GetUserID());
}

int GuildMemberManager::GetFuTuanZhangUid()
{
	GuildMember *guild_menber = this->GetFuTuanZhangMember();
	if (NULL == guild_menber) return 0;

	return UidToInt(guild_menber->GetUserID());
}

void GuildMemberManager::GetMemberList(Protocol::SCGuildMemberList *ml)
{
	int count = 0;

	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end(); it != end && count < MAX_MEMBER_COUNT; ++it)
	{
		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;

		GuildMember *member = m_member_list[it->second];
		if (NULL == member) continue;

		int uid = UidToInt(member->GetUserID());
		UserCacheNode *uc = UserCacheManager::Instance().GetUserNode(uid);
		if (NULL == uc)
		{
			continue;
		}

		ml->member_list[count].uid = uid;
		F_STRNCPY(ml->member_list[count].role_name, uc->user_name, sizeof(ml->member_list[count].role_name));
		ml->member_list[count].level = uc->level;
		ml->member_list[count].sex = uc->sex;
		ml->member_list[count].prof = uc->prof;
		ml->member_list[count].post = member->GetPost();

		ml->member_list[count].gongxian = member->GetGongxian();
		ml->member_list[count].total_gongxian = member->GetTotalGongxian();

		ml->member_list[count].capability = uc->capability;
		ml->member_list[count].vip_level = uc->vip_level;
		ml->member_list[count].is_online = member->IsOnline() ? 1 : 0;
		ml->member_list[count].join_time = static_cast<unsigned int>(member->GetJoinTime());
		ml->member_list[count].last_login_time = static_cast<unsigned int>(member->GetLastLoginTime());
		ml->member_list[count].avatar_timestamp = uc->avatar_timestamp;
		ml->member_list[count].guild_sign_count = member->GetCommonData().signin_count_day;
		ml->member_list[count].avatar_window = uc->cur_use_avatar_type;		
	
		++count;
	}

	ml->count = count;
}

void GuildMemberManager::GetMemberLuckyInfo(Protocol::SCGuildLuckyInfo *gli)
{
	int count = 0;

	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end(); it != end && count < MAX_MEMBER_COUNT; ++it)
	{
		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;

		GuildMember *member = m_member_list[it->second];
		if (NULL == member) continue;

		gli->member_luckyinfo_list[count].uid = UidToInt(member->GetUserID());
		gli->member_luckyinfo_list[count].lucky_color = member->GetLuckyColor();
		memset(gli->member_luckyinfo_list[count].user_name, 0, sizeof(gli->member_luckyinfo_list[count].user_name));

		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(member->GetUserID());
		if (NULL != user_node)
		{
			user_node->GetName(gli->member_luckyinfo_list[count].user_name);
		}

		++ count;
	}

	gli->member_count = count;
}

bool GuildMemberManager::HasOnlineMember()
{
	for (int i = 0; i < MAX_MEMBER_COUNT; i++)
	{
		if (NULL != m_member_list[i] && m_member_list[i]->IsOnline())
		{
			return true;
		}
	}

	return false;
}

int GuildMemberManager::GetMemberList(int member_uid_list[MAX_MEMBER_COUNT])
{
	if (NULL == member_uid_list)
	{
		return 0;
	}

	int member_count = 0;

	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end(); it != end && member_count < MAX_MEMBER_COUNT; ++it)
	{
		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;

		GuildMember *member = m_member_list[it->second];
		if (NULL == member) continue;

		int uid = UidToInt(member->GetUserID());

		member_uid_list[member_count] = uid;
		++member_count;
	}

	return member_count;
}

int GuildMemberManager::GetOnlineMemberList(int member_uid_list[MAX_MEMBER_COUNT])
{
	if (NULL == member_uid_list)
	{
		return 0;
	}

	int member_count = 0;

	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end(); it != end && member_count < MAX_MEMBER_COUNT; ++it)
	{
		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;

		GuildMember *member = m_member_list[it->second];
		if (NULL == member) continue;
		if (!member->IsOnline()) continue;

		int uid = UidToInt(member->GetUserID());

		member_uid_list[member_count] = uid;
		++member_count;
	}

	return member_count;
}

void GuildMemberManager::SetDirty(int member_index)
{
	if (member_index < 0 || member_index >= MAX_MEMBER_COUNT) return;

	m_change_flag = true;
	m_dirty_mark[member_index] = true;
}

int GuildMemberManager::GetCapability(int level_limit, int login_time_limit)
{
	int capability = 0;
	time_t now_second = EngineAdapter::Instance().Time();

	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end(); it != end; ++it)
	{
		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;

		GuildMember *member = m_member_list[it->second];
		if (NULL == member) continue;

		UserCacheNode *uc = UserCacheManager::Instance().GetUserNode(UidToInt(member->GetUserID()));
		if (NULL == uc)
		{
			continue;
		}
		
		if (uc->level < level_limit || (login_time_limit > 0 && now_second - member->GetLastLoginTime() > login_time_limit)) continue;

		capability += uc->capability;
	}

	return capability;
}

int GuildMemberManager::Appoint(GuildMember *appoint_member, GuildMember *beappoint_member, int post)
{
	int error = 0, old_post = beappoint_member->GetPost();
	
	if (old_post == post) // 1：职位必须不一样 
	{
		error = errornum::EN_GUILD_POST_IS_SAME;
	}
	else 
	{
		if (post == GUILD_POST_TUANGZHANG) // 2：转让军团长  
		{
			appoint_member->BeAppoint(GUILD_POST_CHENG_YUAN);  // 顺序有依赖
			beappoint_member->BeAppoint(post);
		}
		else 
		{
			if (!this->IsPostFull(post))  // 3：该职位人数未满
			{
				beappoint_member->BeAppoint(post);
			}
			else
			{
				error = errornum::EN_GUILD_POST_FULL;
			}
		}
	}

	return error;
}

void GuildMemberManager::LeavePost(GuildMember *leavepost_member)
{
	if (NULL == leavepost_member)
	{
		return;
	}

	leavepost_member->BeAppoint(GUILD_POST_CHENG_YUAN);
}

void GuildMemberManager::Dismiss()
{
	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end(); it != end; ++it)
	{
		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;

		GuildMember *member = m_member_list[it->second];
		if (NULL == member) continue;

		UserCacheNode *uc = UserCacheManager::Instance().GetUserNode(member->GetUserID());
		if (NULL != uc)
		{
			LOG_QUICK8(gamelog::g_log_quick, LOG_TYPE_GUILD_JOIN, uc->uid, uc->user_name, NULL, uc->level, m_guild->GetGuildID(), m_guild->GetGuildName(), "Leave");
		}

		GuildManager::Instance().SyncToRole(member->GetUserID(), INVALID_GUILD_ID, "", 0);
	}
}

void GuildMemberManager::CheckAutoKickout()
{
	// 踢掉僵尸玩家 (不足x级且VIPx的玩家，离线超过x时间(秒)就自动踢出)
	if (NULL == m_guild) return;

	GuildMember *tuanzhang = this->GetTuanZhangMember();
	if (NULL == tuanzhang) return;

	const AutoClearGuildMemberCfg *auto_clear_cfg = LOGIC_CONFIG->GetAgentAdapterConfig().GetAutoClearGuildMemberCfg(LocalConfig::Instance().GetPlatName());
	//即使没找到配置，也是默认自动清理
	if (NULL != auto_clear_cfg && !auto_clear_cfg->is_auto_clear)
	{
		return;
	}

	time_t now_sec = EngineAdapter::Instance().Time();
	GuildOtherConfig &other_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig();

	const int level_limit = other_cfg.auto_kickout_level_limit;
	const int vip_limit = other_cfg.auto_kickout_vip_limit;
	const int time_s_limit = other_cfg.auto_kickout_time_s_limit;
	int critical_number = (int)ceil(m_guild->GetGuildMaxMemberCount() * other_cfg.auto_kickout_member_critical_per * 0.01);

	if (m_guild->GetGuildRegister().GetGuildMemberCount() < critical_number)  // 成员人数没有达到临界值
	{
		return;
	}

	for (int i = 0; i < MAX_MEMBER_COUNT; i++)
	{
		GuildMember *member = m_member_list[i];
		if (NULL == member) continue;

		if (GUILD_POST_CHENG_YUAN != member->GetPost())
		{
			continue;
		}

		UserCacheNode *uc = UserCacheManager::Instance().GetUserNode(UidToInt(member->GetUserID()));
		if (NULL == uc)
		{
			continue;
		}

		if (World::GetInstWorld()->GetSceneManager()->UserExist(member->GetUserID()))   // 当前在线
		{
			continue;
		}

		if (World::GetInstWorld()->GetSceneManager()->UserCrosserExist(member->GetUserID()))  // 当前处于跨服
		{
			continue;
		}

		if ( uc->level <= level_limit && 
			(uc->vip_level <= vip_limit || (uc->limit_vip_time > 0 && static_cast<unsigned int>(now_sec) > uc->limit_vip_time)) &&
			 static_cast<long long>(now_sec) > static_cast<long long>(member->GetLastLoginTime() + time_s_limit))
		{
			m_guild->GetGuildEventManager().OnMemberKick(member, tuanzhang);
			m_guild->GetGuildMemberManager().RemoveMember(member->GetUserID());
		}
	}
}

void GuildMemberManager::SystemMailToAll(int mail_reason)
{
	static UserID mail_user_list[MAX_MEMBER_COUNT];
	int mail_user_num = 0;

	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end(); it != end && mail_user_num < MAX_MEMBER_COUNT; ++it)
	{
		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;

		GuildMember *member = m_member_list[it->second];
		if (NULL == member) continue;

		mail_user_list[mail_user_num ++] = member->GetUserID();
	}

	if (mail_user_num > 0)
	{
		static MailContentParam contentparam; contentparam.Reset();
		UserMailManager::Instance().MailToMultUser(MAIL_TYPE_GUILD, mail_user_list, mail_user_num, mail_reason, contentparam);
	}
}

void GuildMemberManager::SystemMailToAll(MailSubject subject, MailContentTxt content)
{
	static UserID mail_user_list[MAX_MEMBER_COUNT];
	int mail_user_num = 0;

	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end(); it != end && mail_user_num < MAX_MEMBER_COUNT; ++it)
	{
		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;

		GuildMember *member = m_member_list[it->second];
		if (NULL == member) continue;

		mail_user_list[mail_user_num ++] = member->GetUserID();
	}

	if (mail_user_num > 0)
	{
		static MailContentParam contentparam; contentparam.Reset();
		F_STRNCPY(contentparam.subject, subject, sizeof(contentparam.subject));
		F_STRNCPY(contentparam.contenttxt, content, sizeof(contentparam.contenttxt));
		UserMailManager::Instance().MailToMultUser(MAIL_TYPE_SYSTEM, mail_user_list, mail_user_num, 0, contentparam);
	}
}

void GuildMemberManager::SystemMailToAll(const MailContentParam &mail_content_param, bool is_send_to_tuanzhang)
{
	static UserID mail_user_list[MAX_MEMBER_COUNT];
	int mail_user_num = 0;

	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end(); it != end && mail_user_num < MAX_MEMBER_COUNT; ++it)
	{
		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;

		GuildMember *member = m_member_list[it->second];
		if (NULL == member) continue;

		if (!is_send_to_tuanzhang && GUILD_POST_TUANGZHANG == member->GetPost())
		{
			continue;
		}

		mail_user_list[mail_user_num ++] = member->GetUserID();
	}

	if (mail_user_num > 0)
	{
		UserMailManager::Instance().MailToMultUser(MAIL_TYPE_SYSTEM, mail_user_list, mail_user_num, 0, mail_content_param);
	}
}

void GuildMemberManager::SystemMailToPrincipal(MailSubject subject, MailContentTxt contenttxt, int notify_type)
{
	static UserID mail_user_list[MAX_MEMBER_COUNT];
	int mail_user_num = this->GetPrincipalList(notify_type, mail_user_list);

	if (mail_user_num > 0)
	{
		static MailContentParam contentparam; contentparam.Reset();
		F_STRNCPY(contentparam.subject, subject, sizeof(contentparam.subject));
		F_STRNCPY(contentparam.contenttxt, contenttxt, sizeof(contentparam.contenttxt));
		UserMailManager::Instance().MailToMultUser(MAIL_TYPE_SYSTEM, mail_user_list, mail_user_num, 0, contentparam);
	}
}

void GuildMemberManager::SystemMailToPrincipal(const MailContentParam &mail_content_param, int notify_type)
{
	static UserID mail_user_list[MAX_MEMBER_COUNT];
	int mail_user_num = this->GetPrincipalList(notify_type, mail_user_list);

	if (mail_user_num > 0)
	{
		UserMailManager::Instance().MailToMultUser(MAIL_TYPE_SYSTEM, mail_user_list, mail_user_num, 0, mail_content_param);
	}
}

void GuildMemberManager::SendMailToAll(Role *role, gglobalprotocal::GuildMailAll *mail_all_req)
{
	if (NULL == role)
	{
		return;
	}

	static UserID mail_user_list[MAX_MEMBER_COUNT];
	int mail_user_num = 0;

	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end();
		it != end && mail_user_num < MAX_MEMBER_COUNT; ++it)
	{
		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;

		GuildMember *member = m_member_list[it->second];
		if (NULL == member) continue;

		mail_user_list[mail_user_num ++] = member->GetUserID();
	}

	if (mail_user_num > 0)
	{
		static MailContentParam contentparam; contentparam.Reset();
		F_STRNCPY(contentparam.subject, mail_all_req->subject, sizeof(contentparam.subject));
		F_STRNCPY(contentparam.contenttxt, mail_all_req->contenttxt, sizeof(contentparam.contenttxt));

		UserMailManager::Instance().MailToMultUser(MAIL_TYPE_GUILD, role->GetUserId(),
			(char *)role->GetName(), mail_user_list, mail_user_num, 0, contentparam);
	}
}

void GuildMemberManager::NotifyMsgToPrincipal(int notify_type, GuildMember *member, int notify_param, int nofity_param1)
{
	Protocol::SCNotifyGuildSuper ngs;
	ngs.notify_type = notify_type;
	ngs.notify_param = notify_param;
	ngs.notify_param1 = nofity_param1;
	
	this->SendMessageToPrincipal(notify_type, (const char *)&ngs, sizeof(ngs), member);
}

void GuildMemberManager::SendMessageToAll(int notify_type, const char *data, int len, GuildMember *member )
{
	if (NULL == data || len <= 0) return;

	if (NULL != member)
	{
		if (member->CheckNotifyPrincipal(notify_type))
		{
			World::GetInstWorld()->GetSceneManager()->SendToUser(member->GetUserID(), data, len);
		}
	}
	else
	{
		UserID notify_user_list[MAX_MEMBER_COUNT];
		int notify_user_count = this->GetPrincipalList(notify_type, notify_user_list, false);

		for (int i = 0; i < notify_user_count && i < MAX_MEMBER_COUNT; ++i)
		{
			if (!World::GetInstWorld()->GetSceneManager()->SendToUser(notify_user_list[i], data, len))
			{
				UniqueUserID uuid(LocalConfig::Instance().GetPlatType(), notify_user_list[i]);
				InternalComm::Instance().UserProtocalTransferToGame(UniqueUserIDToLongLong(uuid), data, len);
			}
		}
	}
}

void GuildMemberManager::SendMessageToPrincipal(int notify_type, const char *data, int len, GuildMember *member)
{
	if (NULL == data || len <= 0) return;

	if (NULL != member)
	{
		if (member->CheckNotifyPrincipal(notify_type)) 
		{
			World::GetInstWorld()->GetSceneManager()->SendToUser(member->GetUserID(), data, len);
		}
	}
	else 
	{
		static UserID notify_user_list[MAX_MEMBER_COUNT];
		int notify_user_count = this->GetPrincipalList(notify_type, notify_user_list, true);

		for (int i = 0; i < notify_user_count && i < MAX_MEMBER_COUNT; ++i)
		{
			World::GetInstWorld()->GetSceneManager()->SendToUser(notify_user_list[i], data, len);
		}
	}
}

bool GuildMemberManager::IsPostFull(int post)
{
	int post_count = 0;

	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end(); it != end; ++it)
	{
		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;

		GuildMember *member = m_member_list[it->second];
		if (NULL != member && member->GetPost() == post)
		{
			++ post_count;
		}
	}

	if (GUILD_POST_TUANGZHANG == post) return true;
	if (GUILD_POST_FU_TUANGZHANG == post && post_count >= 2) return true;
	if (GUILD_POST_ZHANG_LAO == post && post_count >= 4) return true;
	if (GUILD_POST_HUFA == post && post_count >= 6) return true;

	return false;
}

int GuildMemberManager::GetPrincipalList(int notify_type, UserID user_list[MAX_MEMBER_COUNT], bool must_online)
{
	int count = 0;

	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end();
		it != end && count < MAX_MEMBER_COUNT; ++it)
	{
		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;

		GuildMember *member = m_member_list[it->second];
		if (NULL == member) continue;

		if ((!must_online || member->IsOnline()) && member->CheckNotifyPrincipal(notify_type))
		{
			user_list[count ++] = member->GetUserID();
		}
	}

	return count;
}

void GuildMemberManager::OnSyncGuildInfoToAllMember()
{
	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end(); it != end; ++it)
	{
		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;

		GuildMember *member = m_member_list[it->second];
		if (NULL == member || !member->IsOnline()) continue;

		GuildManager::Instance().SyncToRole(member->GetUserID(), m_guild->GetGuildID(), m_guild->GetGuildName(), member->GetPost());
	}
}

void GuildMemberManager::OnSyncGuildSorgeChangeToAllMember(Protocol::SCGuildStorgeChange &cmd)
{
	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end(); it != end; ++it)
	{
		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;

		GuildMember *member = m_member_list[it->second];
		if (NULL == member || !member->IsOnline()) continue;

		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(member->GetUserID());
		if (NULL != role)
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&cmd, sizeof(cmd));
		}
	}
}

void GuildMemberManager::OnRefreshGuildAppearanceInfo()
{
	if (NULL == m_guild)
	{
		return;
	}

	static Protocol::SCGuildSendAppearance appearance;
	appearance.reserve_sh = 0;
	m_guild->GetGuildName(appearance.guild_name);
	appearance.avatar_timestamp = m_guild->GetGuildAvatar();

	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end(); it != end; ++it)
	{
		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;

		GuildMember *member = m_member_list[it->second];
		if (NULL == member || !member->IsOnline()) continue;

		Role *member_role = World::GetInstWorld()->GetSceneManager()->GetRole(member->GetUserID());
		if (NULL != member_role)
		{
			appearance.obj_id = member_role->GetId();
			EngineAdapter::Instance().NetSend(member_role->GetNetId(), (const char *)&appearance, sizeof(appearance));
			member_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(member_role->GetObserHandle(), (void*)&appearance, sizeof(appearance));
		}
	}
}

int GuildMemberManager::GetMemberCount()
{
	int count = (int)(m_userid_index_map.size());
	return count;
}

void GuildMemberManager::OnChat(const char* buff, int total_len)
{
	if (NULL == buff || total_len <= 0) return;

	int net_num = 0;
	static GSNetID gsnetidlist[512];

	CrossUserMap cross_user_map = World::GetInstWorld()->GetSceneManager()->GetCrossUserMap();
	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end(); it != end; ++it)
	{
		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;

		GuildMember *member = m_member_list[it->second];
		if (NULL == member) continue;

		if (!member->IsOnline())
		{
			static const int PLAT_TYPE = LocalConfig::Instance().GetPlatType();

			CrossUserMapIt it = cross_user_map.find(UniqueUserIDToLongLong(UniqueUserID(PLAT_TYPE, member->GetUserID())));
			if (it != cross_user_map.end())
			{
				InternalComm::Instance().UserProtocalTransferToGame(it->first, buff, total_len);
			}

			continue;
		}

		Role *tmp_role = World::GetInstWorld()->GetSceneManager()->GetRole(member->GetUserID());
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

bool GuildMemberManager::OnSystemMsg(const char* buff, int total_len, int filter_member_count, const UserID *filter_member_list)
{
	if (NULL == buff || total_len <= 0) 
	{
		return false;
	}

	std::set<UserID> filter_member_set;
	if (filter_member_count > 0 && NULL != filter_member_list)
	{
		for (int i = 0; i < filter_member_count; ++ i)
		{
			filter_member_set.insert(filter_member_list[i]);
		}
	}

	int net_num = 0;
	static GSNetID gsnetidlist[512];

	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end(); it != end; ++it)
	{
		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;

		GuildMember *member = m_member_list[it->second];
		if (NULL == member) continue;

		if (!member->IsOnline())
		{
			continue;
		}

		if (filter_member_set.end() != filter_member_set.find(member->GetUserID()))
		{
			continue;
		}

		Role *tmp_role = World::GetInstWorld()->GetSceneManager()->GetRole(member->GetUserID());
		if (NULL != tmp_role)
		{
			gsnetidlist[net_num ++] = tmp_role->GetNetId();
		}
	}

	if (net_num > 0)
	{
		EngineAdapter::Instance().NetMulSend(gsnetidlist, net_num, buff, total_len);
	}

	return true;
}

bool SortSaiziScore(GuildMember * mem1, GuildMember * mem2)
{
	if (NULL == mem1 || NULL == mem2) return true;

	if (mem1->GetTodayGuildSaiziScore() != mem2->GetTodayGuildSaiziScore())
	{
		return mem1->GetTodayGuildSaiziScore() > mem2->GetTodayGuildSaiziScore();
	}

	return mem1->GetTodayLastGuildPaoSaiziTime() < mem2->GetTodayLastGuildPaoSaiziTime();
}

// 按骰子积分排名
void GuildMemberManager::CalcSaiziRank()
{
	m_guild_saizi_rank_vec.clear();
	int member_count = 0;

	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end(); it != end && member_count < MAX_MEMBER_COUNT; ++it)
	{
		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;

		GuildMember *member = m_member_list[it->second];
		if (NULL == member) continue;
		if (0 == member->GetTodayGuildSaiziScore() || 0 == member->GetTodayLastGuildPaoSaiziTime()) continue;

		m_guild_saizi_rank_vec.push_back(member);
		member_count++;
	}

	std::sort(m_guild_saizi_rank_vec.begin(), m_guild_saizi_rank_vec.end(), &SortSaiziScore);
}

int GuildMemberManager::GetSaiziRankByUid(int uid)
{
	for (int i = 0; i < (int)(m_guild_saizi_rank_vec.size()) && i < MAX_MEMBER_COUNT; i++)
	{
		GuildMember * rank_member = m_guild_saizi_rank_vec[i];
		if (NULL == rank_member) continue;

		if (UidToInt(rank_member->GetUserID()) == uid)
		{
			return i + 1;
		}
	}
	return -1;
}

void GuildMemberManager::SyncSCGulidSaiziInfo(Role *role, int pao_saizi_num)
{
	if (NULL == role) return;

	GuildMember *member = this->GetMember(role->GetUserId());
	if (NULL == member)
	{
		return;
	}

	static Protocol::SCGulidSaiziInfo pro;
//	pro.today_guild_pao_saizi_times = member->GetTodayGuildPaoSaiziTimes();
	pro.today_guild_pao_saizi_times = role->GetRoleGuild()->GetTodayGuildPaoSaiziTimes();
	pro.today_last_guild_pao_saizi_time = member->GetTodayLastGuildPaoSaiziTime();
	pro.today_guild_saizi_score = member->GetTodayGuildSaiziScore();
	pro.pao_saizi_num = pao_saizi_num;
	for (int i = 0; i < MAX_MEMBER_COUNT; i++)
	{
		pro.guild_saizi_rank_list[i].uid = 0;
		pro.guild_saizi_rank_list[i].score = 0;
		memset(pro.guild_saizi_rank_list[i].name, 0, sizeof(GameName));
	}

	for (int i = 0; i < (int)(m_guild_saizi_rank_vec.size()) && i < MAX_MEMBER_COUNT; i++)
	{	
		pro.guild_saizi_rank_list[i].uid = 0;
		pro.guild_saizi_rank_list[i].score = 0;
		memset(pro.guild_saizi_rank_list[i].name, 0, sizeof(GameName));
	
		GuildMember * rank_member = m_guild_saizi_rank_vec[i];
		if (NULL == rank_member) continue;
		
		UserCacheNode *user_cache_node = UserCacheManager::Instance().GetUserNode(rank_member->GetUserID());
		if (NULL == user_cache_node) continue;

		user_cache_node->GetName(pro.guild_saizi_rank_list[i].name);

		pro.guild_saizi_rank_list[i].uid = UidToInt(rank_member->GetUserID());
		pro.guild_saizi_rank_list[i].score = rank_member->GetTodayGuildSaiziScore();
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&pro, sizeof(pro));
}

void GuildMemberManager::ReqGuildPaoSaizi(Role *role)
{
	if (NULL == role) return;

	GuildMember *member = this->GetMember(role->GetUserId());
	if (NULL == member)
	{
		return;
	}

	GuildOtherConfig &guild_other_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig();
	if (member->GetTodayGuildPaoSaiziTimes() >= guild_other_cfg.today_saizi_count || EngineAdapter::Instance().Time() < member->GetTodayLastGuildPaoSaiziTime() + guild_other_cfg.siai_cold_down)
	{
		return;
	}

	if (role->GetRoleGuild()->GetTodayGuildPaoSaiziTimes() >= guild_other_cfg.today_saizi_count)
	{
		return;
	}

	int rand_saizi = RandomNum(6);
	member->AddTodayGuildSaiziScore(rand_saizi + 1);
	member->AddTodayGuildPaoSaiziTimes();
	role->GetRoleGuild()->AddTodayGuildPaoSaiziTimes();
	member->SetTodayLastGuildPaoSaiziTime(EngineAdapter::Instance().Time());

	this->CalcSaiziRank();
	this->SyncSCGulidSaiziInfo(role, rand_saizi + 1);

	role->GetRoleShop()->AddChestShopMojing(guild_other_cfg.siai_mojing, "ReqGuildPaoSaizi");

	int rank = this->GetSaiziRankByUid(role->GetUID());
	if (rank > 0)
	{
// 		static char buff[1024]; memset(buff, 0, sizeof(buff));
// 
// 		Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)buff;
// 		cc->channel_type = chatdef::CHANNEL_TYPE_GUILD;
// 		cc->msg_length = SNPRINTF(buff + sizeof(Protocol::CSChannelChat), sizeof(buff) - sizeof(Protocol::CSChannelChat), gamestring::g_guild_pao_saizi, role->GetName(), rand_saizi + 1, member->GetTodayGuildSaiziScore(), rank);
// 
// 		ChatManager::Instance().OnChannelChat(role, cc, cc->msg_length + sizeof(Protocol::CSChannelChat));

		int length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "guildpaosaizi",
			role->GetUID(), role->GetName(), role->GetCamp(), rand_saizi + 1, member->GetTodayGuildSaiziScore(), rank);

		if (length > 0)
		{
			m_guild->GuildSystemMsg(length, gamestring::GAMESTRING_BUF);
		}
	}
}

// 以盟主身份发送仙盟聊天
void GuildMemberManager::GuildleaderGuildChat(void* msg, int length)
{
	if (NULL == msg || length <= 0)
	{
		return;
	}

	UserCacheNode *user_cache_node = UserCacheManager::Instance().GetUserNode(this->GetTuanZhangUid());
	if (NULL == user_cache_node) return;

	Protocol::CSChannelChat *cc = (Protocol::CSChannelChat*)msg;
	if (cc->msg_length + (int)sizeof(Protocol::CSChannelChat) > length) // 检查格式
	{
		return;
	}

	if (cc->msg_length <= 0 || cc->msg_length > MAX_CHAT_LENGTH)
	{
		return;
	}

	static char buff[sizeof(Protocol::SCChannelChat) + MAX_CHAT_LENGTH + 1] = {0};

	Protocol::SCChannelChat *channel_chat = (Protocol::SCChannelChat*)buff;

	UserID userid = IntToUid(user_cache_node->uid);
	int plat_type = LocalConfig::Instance().GetPlatType();
	UniqueUserID unique_userid(plat_type, userid);
	long long uuid = UniqueUserIDToLongLong(unique_userid);

	channel_chat->header.msg_type = Protocol::MT_CHANNEL_CHAT_SC;
	channel_chat->from_uid = user_cache_node->uid;
	channel_chat->uuid = uuid;
	channel_chat->channel_type = (char)cc->channel_type;
	channel_chat->msg_length = cc->msg_length;
	user_cache_node->GetName(channel_chat->username);
	channel_chat->sex = user_cache_node->sex;
	channel_chat->camp = user_cache_node->camp;
	channel_chat->vip_level = user_cache_node->vip_level;
	channel_chat->prof = user_cache_node->prof;
	channel_chat->authority_type = 0;
	channel_chat->level = (short)user_cache_node->level;
	channel_chat->avatar_timestamp = user_cache_node->avatar_timestamp;
	channel_chat->personalize_window_bubble_type = (char)user_cache_node->cur_use_bubble_type;
	channel_chat->personalize_window_avatar_type = (char)user_cache_node->cur_use_avatar_type;
	channel_chat->content_type = cc->content_type;
	channel_chat->origin_type = chatdef::ORIGIN_TYPE_NORMAL_CHAT;
	channel_chat->has_xianzunka_flag = user_cache_node->has_xianzunka_flag;
	channel_chat->msg_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	memcpy(((char*)buff) + sizeof(Protocol::SCChannelChat), (char *)msg + sizeof(Protocol::CSChannelChat), cc->msg_length);

	int total_len = sizeof(Protocol::SCChannelChat) + cc->msg_length;

	char *chat_content = ((char*)buff) + sizeof(Protocol::SCChannelChat);
	chat_content[cc->msg_length] = 0;

	this->OnChat(buff, total_len);
	ChatManager::Instance().AddGuildChatBoard(m_guild->GetGuildID(), buff, total_len);
}

void GuildMemberManager::SendSaiziReward()
{
	for (int i = 0; i < (int)(m_guild_saizi_rank_vec.size()) && i < MAX_MEMBER_COUNT; i++)
	{
		GuildMember * rank_member = m_guild_saizi_rank_vec[i];
		if (NULL == rank_member) continue;

		GuildSaiziCfg * saizi_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildSaiziCfg(i + 1);
		if (NULL == saizi_cfg) continue;

		static MailContentParam contentparam; contentparam.Reset();

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_guild_saizi_reward_mail, i + 1);

		for (int i = 0; i < saizi_cfg->reward_item_count && i < MAX_ATTACHMENT_ITEM_NUM; i++)
		{
			if (0 == saizi_cfg->reward_item_list[i].item_id) return;

			contentparam.item_list[i].item_id = saizi_cfg->reward_item_list[i].item_id;
			contentparam.item_list[i].num = saizi_cfg->reward_item_list[i].num;
			contentparam.item_list[i].is_bind = saizi_cfg->reward_item_list[i].is_bind;
		}

		if (length > 0)
		{
			MailRoute::MailToUser(rank_member->GetUserID(), SYSTEM_MAIL_REASON_INVALID, contentparam);
		}
	}
}

void GuildMemberManager::SendGuildBattleDailyRewardInfo(int rank)
{
	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end(); it != end; ++it)
	{
		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;

		GuildMember *member = m_member_list[it->second];
		if (NULL == member) continue;

		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(member->GetUserID());
		if (nullptr != role)
		{
			member->SendDailyRewardTimes(role, rank);
		}
	}
}

void GuildMemberManager::AutoClearMember(unsigned int now_sec)
{
	if (NULL == m_guild) return;

	GuildMember *tuanzhan= this->GetTuanZhangMember();
	if (NULL == tuanzhan) return;

	for (int i = 0; i < MAX_MEMBER_COUNT; i++)
	{
		GuildMember *member = m_member_list[i];
		if (NULL == member) continue;

		if (GUILD_POST_CHENG_YUAN != member->GetPost())
		{
			continue;
		}

		if (World::GetInstWorld()->GetSceneManager()->UserExist(member->GetUserID()))   // 当前在线
		{
			continue;
		}

		if (World::GetInstWorld()->GetSceneManager()->UserCrosserExist(member->GetUserID()))  // 当前处于跨服
		{
			continue;
		}
		
		if (static_cast<long long>(now_sec) > static_cast<long long>(member->GetLastLoginTime() + SECOND_PER_DAY * 3))
		{
			m_guild->GetGuildEventManager().OnMemberKick(member, tuanzhan);
			m_guild->GetGuildMemberManager().RemoveMember(member->GetUserID());
		}
	}
}

bool GuildMemberManager::IsSigninSucc(Role *role)
{
	if (NULL == role)
	{
		return false;
	}

	GuildMember *member = this->GetMember(role->GetUserId());
	if (NULL == member)
	{
		return  false;
	}

	if (member->IsSigninSucc(role))
	{
		++m_total_member_signin_count_today;
		return true;
	}

	return  false;
}

int GuildMemberManager::GetMemberGongziRate(int uid)
{
	int user_post_rate = 0;
	auto it = m_userid_index_map.find(IntToUid(uid));
	if (it == m_userid_index_map.end())
	{
		return 0;
	}
	
	GuildMember *member = m_member_list[it->second];
	if (NULL == member) return 0;

	user_post_rate = LOGIC_CONFIG->GetGuildConfig().GetGongziRate(member->GetPost());

	// 计算工资总权重
	int total_gongzi_rate = this->GetRealGongziTotalRate();

	float gongzi_rate = total_gongzi_rate > 0 ? user_post_rate * 1.0f / total_gongzi_rate * 10000.0f : 0;

	return static_cast<int>(gongzi_rate);
}

int GuildMemberManager::GetRealGongziTotalRate()
{
	int total_gongzi_rate = 0;
	for (int post = GUILD_POST_CHENG_YUAN; post < GUILD_POST_MAX; post++)
	{
		int post_rate = LOGIC_CONFIG->GetGuildConfig().GetGongziRate(post);
		int post_count = this->GetPostCount(post);

		total_gongzi_rate += post_rate * post_count;
	}

	// 成员数量不足，补齐工资权重
	int min_give_gongzi_member_num = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig().min_give_gongzi_member_num;
	int delta_member_num = min_give_gongzi_member_num - this->GetMemberCount();
	int post_rate = LOGIC_CONFIG->GetGuildConfig().GetGongziRate(GUILD_POST_CHENG_YUAN);
	if (delta_member_num > 0)
	{
		total_gongzi_rate += post_rate * delta_member_num;
	}

	return total_gongzi_rate;
}

int GuildMemberManager::GetPostCount(int post)
{
	int post_count = 0;

	for (UserIDIndexMapIt it = m_userid_index_map.begin(), end = m_userid_index_map.end(); it != end; ++it)
	{
		if (it->second < 0 || it->second >= MAX_MEMBER_COUNT) continue;

		GuildMember *member = m_member_list[it->second];
		if (NULL != member && member->GetPost() == post)
		{
			++post_count;
		}
	}

	return post_count;
}

void GuildMemberManager::SendSaiziRankMsg()
{
	int sendlen = 0;
	int rank_size = (int)(m_guild_saizi_rank_vec.size());
	if (rank_size == 1)
	{
		UserCacheNode *user_cache_node0 = UserCacheManager::Instance().GetUserNode(m_guild_saizi_rank_vec[0]->GetUserID());
		if (NULL == user_cache_node0) return;

		sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_saizi_rank_msg1,
			user_cache_node0->user_name, m_guild_saizi_rank_vec[0]->GetTodayGuildSaiziScore());
	}
	else if (rank_size >= 2)
	{
		UserCacheNode *user_cache_node0 = UserCacheManager::Instance().GetUserNode(m_guild_saizi_rank_vec[0]->GetUserID());
		if (NULL == user_cache_node0) return;

		UserCacheNode *user_cache_node1 = UserCacheManager::Instance().GetUserNode(m_guild_saizi_rank_vec[1]->GetUserID());
		if (NULL == user_cache_node1) return;

		sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_saizi_rank_msg2, 
			user_cache_node0->user_name, m_guild_saizi_rank_vec[0]->GetTodayGuildSaiziScore(), user_cache_node1->user_name);
	}

	if (sendlen > 0)
	{
		m_guild->GuildSystemMsg(sendlen, gamestring::GAMESTRING_BUF);
	}
}

void GuildMemberManager::GuildSystemChat(void* msg, int length, int origin_type)
{
	if (NULL == msg || length <= 0)
	{
		return;
	}

	UserCacheNode *user_cache_node = UserCacheManager::Instance().GetUserNode(this->GetTuanZhangUid());
	if (NULL == user_cache_node) return;

	Protocol::CSChannelChat *cc = (Protocol::CSChannelChat*)msg;
	if (cc->msg_length + (int)sizeof(Protocol::CSChannelChat) > length) // 检查格式
	{
		return;
	}

	if (cc->msg_length <= 0 || cc->msg_length > MAX_CHAT_LENGTH)
	{
		return;
	}

	static char buff[sizeof(Protocol::SCChannelChat) + MAX_CHAT_LENGTH + 1] = { 0 };

	Protocol::SCChannelChat *channel_chat = (Protocol::SCChannelChat*)buff;

	UserID userid = IntToUid(user_cache_node->uid);
	int plat_type = LocalConfig::Instance().GetPlatType();
	UniqueUserID unique_userid(plat_type, userid);
	long long uuid = UniqueUserIDToLongLong(unique_userid);

	channel_chat->header.msg_type = Protocol::MT_CHANNEL_CHAT_SC;
	channel_chat->from_uid = user_cache_node->uid;
	channel_chat->uuid = uuid;
	channel_chat->channel_type = (char)cc->channel_type;
	channel_chat->msg_length = cc->msg_length;
	user_cache_node->GetName(channel_chat->username);
	channel_chat->sex = user_cache_node->sex;
	channel_chat->camp = user_cache_node->camp;
	channel_chat->vip_level = user_cache_node->vip_level;
	channel_chat->prof = user_cache_node->prof;
	channel_chat->authority_type = 0;
	channel_chat->level = (short)user_cache_node->level;
	channel_chat->avatar_timestamp = user_cache_node->avatar_timestamp;
	channel_chat->personalize_window_bubble_type = (char)user_cache_node->cur_use_bubble_type;
	channel_chat->personalize_window_avatar_type = (char)user_cache_node->cur_use_avatar_type;
	channel_chat->content_type = cc->content_type;
	channel_chat->origin_type = origin_type;
	channel_chat->has_xianzunka_flag = user_cache_node->has_xianzunka_flag;
	channel_chat->msg_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	memcpy(((char*)buff) + sizeof(Protocol::SCChannelChat), (char *)msg + sizeof(Protocol::CSChannelChat), cc->msg_length);

	int total_len = sizeof(Protocol::SCChannelChat) + cc->msg_length;

	char *chat_content = ((char*)buff) + sizeof(Protocol::SCChannelChat);
	chat_content[cc->msg_length] = 0;

	this->OnChat(buff, total_len);
	ChatManager::Instance().AddGuildChatBoard(m_guild->GetGuildID(), buff, total_len);
}