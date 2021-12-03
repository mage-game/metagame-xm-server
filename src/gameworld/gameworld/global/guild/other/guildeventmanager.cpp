#include "servercommon/struct/structcommon.h"
#include "engineadapter.h"

#include "global/guild/impl/guildmember.hpp"
#include "global/guild/guild.hpp"

#include "guildeventmanager.hpp"

#include "global/usercache/usercache.hpp"

GuildEventManager::GuildEventManager()
	: m_guild(NULL), m_change_flag(false)
{
	memset(m_old_state, 0 , sizeof(m_old_state));
	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));
}

GuildEventManager::~GuildEventManager()
{

}

void GuildEventManager::Init(const GuildEventParam &event_param)
{
	int count = 0;

	if (event_param.count < MAX_ALL_GUILD_EVENT_COUNT)
	{
		count = event_param.count;
	}
	else 
	{
		count = MAX_ALL_GUILD_EVENT_COUNT;
	}

	for (int i = count - 1; i >= 0; i--)
	{
		int index = (int)event_param.event_list[i].index;

		if (index < 0 || index >= MAX_ALL_GUILD_EVENT_COUNT) continue;
		if (!m_event_list[index].Invalid()) continue;

		m_event_list[index].event_type = event_param.event_list[i].event_type;
		F_STRNCPY(m_event_list[index].event_owner, event_param.event_list[i].event_owner, sizeof(m_event_list[0].event_owner));
		m_event_list[index].event_owner_post = event_param.event_list[i].event_owner_post;
		m_event_list[index].event_time = event_param.event_list[i].event_time;
		m_event_list[index].big_event = event_param.event_list[i].big_event;
		m_event_list[index].cannot_remove = event_param.event_list[i].cannot_remove;

		m_event_list[index].param0 = event_param.event_list[i].param0;
		m_event_list[index].param1 = event_param.event_list[i].param1;
		m_event_list[index].param2 = event_param.event_list[i].param2;
		m_event_list[index].param3 = event_param.event_list[i].param3;
		F_STRNCPY(m_event_list[index].sparam0, event_param.event_list[i].sparam0, sizeof(m_event_list[0].sparam0));

		m_old_state[index] = true;
	}	
}

void GuildEventManager::GetChangeEventList(GuildEventParam *event_param)
{
	if (NULL == event_param) return;

	if (!m_change_flag)
	{
		event_param->count = 0;
		return;
	}

	int count = 0;

	for (int i = 0; i < MAX_ALL_GUILD_EVENT_COUNT; ++i)
	{
		if (m_dirty_mark[i])
		{
			if (m_event_list[i].Invalid())
			{
				if (!m_old_state[i]) continue;

				event_param->event_list[count].change_state = structcommon::CS_DELETE;
				event_param->event_list[count].index = i;
			}
			else
			{
				event_param->event_list[count].index = i;
				event_param->event_list[count].change_state = m_old_state[i] ? structcommon::CS_UPDATE : structcommon::CS_INSERT;
				
				event_param->event_list[count].event_type = m_event_list[i].event_type;
				F_STRNCPY(event_param->event_list[count].event_owner, m_event_list[i].event_owner, sizeof(event_param->event_list[0].event_owner));
				event_param->event_list[count].event_owner_post = m_event_list[i].event_owner_post;
				event_param->event_list[count].event_time = m_event_list[i].event_time;
				event_param->event_list[count].big_event = m_event_list[i].big_event;
				event_param->event_list[count].cannot_remove = m_event_list[i].cannot_remove;

				event_param->event_list[count].param0 = m_event_list[i].param0;
				event_param->event_list[count].param1 = m_event_list[i].param1;
				event_param->event_list[count].param2 = m_event_list[i].param2;
				event_param->event_list[count].param3 = m_event_list[i].param3;
				F_STRNCPY(event_param->event_list[count].sparam0, m_event_list[i].sparam0, sizeof(event_param->event_list[0].sparam0));
			}

			++count;
		}
	}

	event_param->count = count;
}

void GuildEventManager::ClearDirtyMark()
{
	if (!m_change_flag) return;

	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));

	for (int i = 0; i < MAX_ALL_GUILD_EVENT_COUNT; ++i)
	{
		m_old_state[i] = !m_event_list[i].Invalid();
	}

	m_change_flag = false;
}

void GuildEventManager::GetEventList(Protocol::SCGuildEventList *event_list)
{
	if (NULL == event_list) return;

	int count = 0;

	for (int i = 0; i < MAX_COMMON_EVENT_COUNT + MAX_BIG_EVENT_COUNT && count < MAX_COMMON_EVENT_COUNT + MAX_BIG_EVENT_COUNT; ++i)
	{
		if (!m_event_list[i].Invalid())
		{
			if (GUILD_EVENT_TYPE_STORAGE_OPERATE == m_event_list[i].event_type)
			{
				continue;  // 理论上应该不会在这个范围内出现仓库日志类型
			}

			event_list->event_for_list[count].event_type = static_cast<short>(m_event_list[i].event_type);
			F_STRNCPY(event_list->event_for_list[count].event_owner, m_event_list[i].event_owner, sizeof(event_list->event_for_list[0].event_owner));
			event_list->event_for_list[count].event_owner_post = static_cast<short>(m_event_list[i].event_owner_post);
			event_list->event_for_list[count].event_time = m_event_list[i].event_time;
			event_list->event_for_list[count].big_event = m_event_list[i].big_event;
			event_list->event_for_list[count].cannot_remove = m_event_list[i].cannot_remove;

			event_list->event_for_list[count].param0 = m_event_list[i].param0;
			event_list->event_for_list[count].param1 = m_event_list[i].param1;
			event_list->event_for_list[count].param2 = m_event_list[i].param2;
			event_list->event_for_list[count].param3 = m_event_list[i].param3;
			F_STRNCPY(event_list->event_for_list[count].sparam0, m_event_list[i].sparam0, sizeof(event_list->event_for_list[0].sparam0));

			++count;
		}
	}

	event_list->count = count;
}

void GuildEventManager::GetStorageLogList(Protocol::SCGuildStorageLogList *log_list)
{
	if (NULL == log_list) return;

	int count = 0;

	for (int i = MAX_COMMON_EVENT_COUNT + MAX_BIG_EVENT_COUNT; i < MAX_ALL_GUILD_EVENT_COUNT && count < MAX_STORAGE_EVENT_COUNT; ++i)
	{
		if (!m_event_list[i].Invalid())
		{
			if (GUILD_EVENT_TYPE_STORAGE_OPERATE != m_event_list[i].event_type)
			{
				continue;  // 理论上应该不会在这个范围内出现普通事件类型
			}

			F_STRNCPY(log_list->log_list[count].log_owner_name, m_event_list[i].event_owner, sizeof(GameName));
			log_list->log_list[count].log_time = m_event_list[i].event_time;

			log_list->log_list[count].opt_type = m_event_list[i].param0;
			log_list->log_list[count].item_id = m_event_list[i].param1;
			
			memset(log_list->log_list[count].xianpin_type_list, 0, sizeof(log_list->log_list[count].xianpin_type_list));
			UNSTD_STATIC_CHECK(sizeof(log_list->log_list[count].xianpin_type_list[0]) * ItemNamespace::MAX_XIANPIN_NUM <= sizeof(GuildEventStrParam));
			F_STRNCPY((char *)log_list->log_list[count].xianpin_type_list, m_event_list[i].sparam0, sizeof(log_list->log_list[count].xianpin_type_list[0]) * ItemNamespace::MAX_XIANPIN_NUM);


			++count;
		}
	}

	log_list->count = count;
}

void GuildEventManager::OnCreate(GameName createor)
{
	int free_pos = -1;
	if (this->GetFreeEventSlot(false, false, &free_pos, true)) 
	{
		m_event_list[free_pos].event_type = GUILD_EVENT_TYPE_CREATE;
		F_STRNCPY(m_event_list[free_pos].event_owner, createor, sizeof(m_event_list[0].event_owner));
		m_event_list[free_pos].event_owner_post = GUILD_POST_TUANGZHANG;
		m_event_list[free_pos].event_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

		m_dirty_mark[free_pos] = true;
		m_change_flag = true;
	}
}

void GuildEventManager::OnLevelChange(int to_level, bool is_upgrade)
{
	int free_pos = -1;
	if (this->GetFreeEventSlot(false, false, &free_pos, true))
	{
		m_event_list[free_pos].event_type = (is_upgrade ? GUILD_EVENT_TYPE_UPGRADE_LEVEL : GUILD_EVENT_TYPE_DOWNGRADE_LEVEL);
		m_event_list[free_pos].event_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		m_event_list[free_pos].param0 = to_level;

		m_dirty_mark[free_pos] = true;
		m_change_flag = true;
	}
}

void GuildEventManager::OnMemberAdd(GuildMember *new_member, GuildMember *verify_member)
{
	if (NULL == new_member || NULL == verify_member)
	{
		return;
	}

	UserCacheNode *new_uc = UserCacheManager::Instance().GetUserNode(UidToInt(new_member->GetUserID()));
	UserCacheNode *verify_uc = UserCacheManager::Instance().GetUserNode(UidToInt(verify_member->GetUserID()));
	if (NULL == new_uc || NULL == verify_uc)
	{
		return;
	}

	int free_pos = -1;
	if (this->GetFreeEventSlot(false, false, &free_pos, true))
	{
		m_event_list[free_pos].event_type = GUILD_EVENT_TYPE_MEMBER_ADD;
		F_STRNCPY(m_event_list[free_pos].event_owner, new_uc->user_name, sizeof(GameName));
		m_event_list[free_pos].event_owner_post = new_member->GetPost();
		m_event_list[free_pos].event_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		m_event_list[free_pos].param0 = verify_member->GetPost();
		STRNCPY(m_event_list[free_pos].sparam0, verify_uc->user_name, sizeof(m_event_list[free_pos].sparam0));

		m_dirty_mark[free_pos] = true;
		m_change_flag = true;
	}
}

void GuildEventManager::OnMemberQuit(GuildMember *quit_member)
{
	if (NULL == quit_member)
	{
		return;
	}

	UserCacheNode *quit_uc = UserCacheManager::Instance().GetUserNode(UidToInt(quit_member->GetUserID()));
	if (NULL == quit_uc)
	{
		return;
	}

	int free_pos = -1;
	if (this->GetFreeEventSlot(false, false, &free_pos, true))
	{
		m_event_list[free_pos].event_type = GUILD_EVENT_TYPE_MEMBER_QUIT;
		F_STRNCPY(m_event_list[free_pos].event_owner, quit_uc->user_name, sizeof(GameName));
		m_event_list[free_pos].event_owner_post = quit_member->GetPost();
		m_event_list[free_pos].event_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		
		m_dirty_mark[free_pos] = true;
		m_change_flag = true;
	}
}

void GuildEventManager::OnMemberKick(GuildMember *bekicker_member, GuildMember *kicker_member)
{
	if (NULL == bekicker_member || NULL == kicker_member)
	{
		return;
	}

	UserCacheNode *bekick_uc = UserCacheManager::Instance().GetUserNode(UidToInt(bekicker_member->GetUserID()));
	UserCacheNode *kick_uc = UserCacheManager::Instance().GetUserNode(UidToInt(kicker_member->GetUserID()));
	if (NULL == bekick_uc || NULL == kick_uc)
	{
		return;
	}

	int free_pos = -1;
	if (this->GetFreeEventSlot(false, false, &free_pos, true))
	{
		m_event_list[free_pos].event_type = GUILD_EVENT_TYPE_MEMBER_KICK;
		F_STRNCPY(m_event_list[free_pos].event_owner, bekick_uc->user_name, sizeof(GameName));
		m_event_list[free_pos].event_owner_post = bekicker_member->GetPost();
		m_event_list[free_pos].event_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		m_event_list[free_pos].param0 = kicker_member->GetPost();
		STRNCPY(m_event_list[free_pos].sparam0, kick_uc->user_name, sizeof(m_event_list[free_pos].sparam0));

		m_dirty_mark[free_pos] = true;
		m_change_flag = true;
	}
}

void GuildEventManager::OnMemberAppoint(GuildMember *beappoint_member, GuildMember *appoint_member)
{
	if (NULL == beappoint_member || NULL == appoint_member)
	{
		return;
	}

	UserCacheNode *beappoint_uc = UserCacheManager::Instance().GetUserNode(UidToInt(beappoint_member->GetUserID()));
	UserCacheNode *appoint_uc = UserCacheManager::Instance().GetUserNode(UidToInt(appoint_member->GetUserID()));
	if (NULL == beappoint_uc || NULL == appoint_uc)
	{
		return;
	}

	int free_pos = -1;
	if (this->GetFreeEventSlot(false, false, &free_pos, true))
	{
		m_event_list[free_pos].event_type = GUILD_EVENT_TYPE_MEMBER_APPOINT;
		F_STRNCPY(m_event_list[free_pos].event_owner, beappoint_uc->user_name, sizeof(GameName));
		m_event_list[free_pos].event_owner_post = beappoint_member->GetPost();
		m_event_list[free_pos].event_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		m_event_list[free_pos].param0 = appoint_member->GetPost();
		STRNCPY(m_event_list[free_pos].sparam0, appoint_uc->user_name, sizeof(m_event_list[free_pos].sparam0));

		m_dirty_mark[free_pos] = true;
		m_change_flag = true;
	}
}

void GuildEventManager::OnMemberLeavePost(GuildMember *leavepost_member)
{
	if (NULL == leavepost_member)
	{
		return;
	}

	UserCacheNode *lp_uc = UserCacheManager::Instance().GetUserNode(UidToInt(leavepost_member->GetUserID()));
	if (NULL == lp_uc)
	{
		return;
	}

	int free_pos = -1;
	if (this->GetFreeEventSlot(false, false, &free_pos, true))
	{
		m_event_list[free_pos].event_type = GUILD_EVENT_TYPE_MEMBER_LEAVE_POST;
		F_STRNCPY(m_event_list[free_pos].event_owner, lp_uc->user_name, sizeof(GameName));
		m_event_list[free_pos].event_owner_post = leavepost_member->GetPost();
		m_event_list[free_pos].event_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

		m_dirty_mark[free_pos] = true;
		m_change_flag = true;
	}
}

void GuildEventManager::OnTransferTuanzhang(GuildMember *old_tuanzhang_member, GuildMember *new_tuanzhang_member)
{
	if (NULL == old_tuanzhang_member || NULL == new_tuanzhang_member)
	{
		return;
	}

	UserCacheNode *old_uc = UserCacheManager::Instance().GetUserNode(UidToInt(old_tuanzhang_member->GetUserID()));
	UserCacheNode *new_uc = UserCacheManager::Instance().GetUserNode(UidToInt(new_tuanzhang_member->GetUserID()));
	if (NULL == old_uc || NULL == new_uc)
	{
		return;
	}

	int free_pos = -1;
	if (this->GetFreeEventSlot(false, false, &free_pos, true))
	{
		m_event_list[free_pos].event_type = GUILD_EVENT_TYPE_TRANSFER_TUANZHANG;
		F_STRNCPY(m_event_list[free_pos].event_owner, old_uc->user_name, sizeof(GameName));
		m_event_list[free_pos].event_owner_post = GUILD_POST_TUANGZHANG;
		m_event_list[free_pos].event_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		STRNCPY(m_event_list[free_pos].sparam0, new_uc->user_name, sizeof(m_event_list[free_pos].sparam0));

		m_dirty_mark[free_pos] = true;
		m_change_flag = true;
	}
}

void GuildEventManager::OnDelateTuanzhang(GuildMember *delate_member)
{
	if (NULL == delate_member)
	{
		return;
	}

	UserCacheNode *del_uc = UserCacheManager::Instance().GetUserNode(UidToInt(delate_member->GetUserID()));
	if (NULL == del_uc)
	{
		return;
	}

	int free_pos = -1;
	if (this->GetFreeEventSlot(false, false, &free_pos, true))
	{
		m_event_list[free_pos].event_type = GUILD_EVENT_TYPE_DELATE_TUANZHANG;
		F_STRNCPY(m_event_list[free_pos].event_owner, del_uc->user_name, sizeof(GameName));
		m_event_list[free_pos].event_owner_post = delate_member->GetPost();
		m_event_list[free_pos].event_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

		m_dirty_mark[free_pos] = true;
		m_change_flag = true;
	}
}

void GuildEventManager::OnMemberAddExp(GuildMember *oper_member, int add_type, int add_value)
{
	if (NULL == oper_member)
	{
		return;
	}

	UserCacheNode *uc = UserCacheManager::Instance().GetUserNode(UidToInt(oper_member->GetUserID()));
	if (NULL == uc)
	{
		return;
	}

	int free_pos = -1;
	if (this->GetFreeEventSlot(false, false, &free_pos, true))
	{
		m_event_list[free_pos].event_type = GUILD_EVENT_TYPE_ADD_EXP;
		F_STRNCPY(m_event_list[free_pos].event_owner, uc->user_name, sizeof(GameName));
		m_event_list[free_pos].event_owner_post = oper_member->GetPost();
		m_event_list[free_pos].event_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		m_event_list[free_pos].param0 = add_type;
		m_event_list[free_pos].param1 = add_value;

		m_dirty_mark[free_pos] = true;
		m_change_flag = true;
	}
}

//void GuildEventManager::OnStorageOperate(GuildMember *oper_member, int op_type, int item_id, int item_num, unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM])
void GuildEventManager::OnStorageOperate(GuildMember *oper_member, int op_type, const ItemDataWrapper &wrapper)
{
	if (NULL == oper_member)
	{
		return;
	}

	UserCacheNode *uc = UserCacheManager::Instance().GetUserNode(UidToInt(oper_member->GetUserID()));
	if (NULL == uc)
	{
		return;
	}

	int free_pos = -1;
	if (this->GetFreeEventSlot(false, true, &free_pos, true))
	{
		m_event_list[free_pos].event_type = GUILD_EVENT_TYPE_STORAGE_OPERATE;
		F_STRNCPY(m_event_list[free_pos].event_owner, uc->user_name, sizeof(GameName));
		m_event_list[free_pos].event_owner_post = oper_member->GetPost();
		m_event_list[free_pos].event_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		m_event_list[free_pos].param0 = op_type;
		m_event_list[free_pos].param1 = wrapper.item_id;
		m_event_list[free_pos].param2 = wrapper.num;

		memset(m_event_list[free_pos].sparam0, 0, sizeof(GuildEventStrParam));
		UNSTD_STATIC_CHECK(sizeof(wrapper.param_data.xianpin_type_list[0]) * ItemNamespace::MAX_XIANPIN_NUM <= sizeof(GuildEventStrParam));
		F_STRNCPY(m_event_list[free_pos].sparam0, (char *)wrapper.param_data.xianpin_type_list, sizeof(wrapper.param_data.xianpin_type_list[0]) * ItemNamespace::MAX_XIANPIN_NUM);

		m_dirty_mark[free_pos] = true;
		m_change_flag = true;
	}
}

bool GuildEventManager::GetFreeEventSlot(bool bigevent, bool storageevent, int *pos, bool is_clear)
{
	int begin_idx = 0, end_idx = MAX_COMMON_EVENT_COUNT; 
	if (bigevent) { begin_idx = MAX_COMMON_EVENT_COUNT; end_idx = MAX_COMMON_EVENT_COUNT + MAX_BIG_EVENT_COUNT; }
	if (storageevent) { begin_idx = MAX_COMMON_EVENT_COUNT + MAX_BIG_EVENT_COUNT; end_idx = MAX_COMMON_EVENT_COUNT + MAX_BIG_EVENT_COUNT + MAX_STORAGE_EVENT_COUNT; }

	int free_pos = -1;
	unsigned int oldest_event_time = 0xffffffff;
	int i = begin_idx;

	for (i = begin_idx; i < end_idx; ++i)
	{
		if (m_event_list[i].Invalid()) break;

		if (!m_event_list[i].cannot_remove && m_event_list[i].event_time < oldest_event_time) 
		{
			oldest_event_time = m_event_list[i].event_time;
			free_pos = i;
		}
	}

	if (i >= end_idx)
	{
		if (-1 == free_pos) return false;
	}
	else 
	{
		free_pos = i;
	}

	if (NULL != pos) *pos = free_pos;

	if (is_clear && !m_event_list[free_pos].Invalid()) m_event_list[free_pos].Reset();

	return true;
}

void GuildEventManager::OnRumorEvent(const char *name_1, const char *name_2, int event_type, int param0, int param1, int param2, int param3)
{
	int free_pos = -1;
	if (this->GetFreeEventSlot(false, false, &free_pos, true))
	{
		m_event_list[free_pos].event_type = event_type;
		m_event_list[free_pos].event_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		m_event_list[free_pos].param0 = param0;
		m_event_list[free_pos].param1 = param1;
		m_event_list[free_pos].param2 = param2;
		m_event_list[free_pos].param3 = param3;

		if (name_1 != NULL)
		{
			F_STRNCPY(m_event_list[free_pos].event_owner, name_1, sizeof(GameName));
		}

		if (name_2 != NULL)
		{
			F_STRNCPY(m_event_list[free_pos].sparam0, name_2, sizeof(GameName));
		}

		m_dirty_mark[free_pos] = true;
		m_change_flag = true;


		int member_uid_list[MAX_MEMBER_COUNT];
		memset(member_uid_list, 0, sizeof(member_uid_list));
		int member_total_count = m_guild->GetGuildMemberManager().GetOnlineMemberList(member_uid_list);

		for (int i = 0; i < member_total_count; i++)
		{
			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid_list[i]));

			if (role != NULL)
			{
				Protocol::SCGuildEventList gel;
				m_guild->GetGuildEventManager().GetEventList(&gel);

				int sendlen = sizeof(gel) - (MAX_COMMON_EVENT_COUNT + MAX_BIG_EVENT_COUNT - gel.count) * sizeof(gel.event_for_list[0]);
				EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gel, sendlen);
			}
		}
	}
}

