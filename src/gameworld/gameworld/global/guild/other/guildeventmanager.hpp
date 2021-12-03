/******************************************************************************************
FileName: guildauthority.hpp
Author: dreamwang
Description: 军团事件管理 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/09/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __GUILD_EVENT_MANAGER_HPP__
#define __GUILD_EVENT_MANAGER_HPP__

#include "servercommon/guilddef.hpp"
#include "servercommon/struct/global/guildparam.hpp"
//#include "globalserver/protocal/msgscguild.hpp"

class Guild;
class GuildMember;

class GuildEventManager
{
public:
	GuildEventManager();
	~GuildEventManager();

	void SetGuild(Guild *guild) { m_guild = guild; }
	Guild * GetGuild() { return m_guild; }

	void Init(const GuildEventParam &event_param);

	void GetChangeEventList(GuildEventParam *event_param);
	void ClearDirtyMark();

	void GetEventList(Protocol::SCGuildEventList *event_list);
	void GetStorageLogList(Protocol::SCGuildStorageLogList *log_list);
	
	void OnCreate(GameName createor);
	void OnLevelChange(int to_level, bool is_upgrade);
	void OnMemberAdd(GuildMember *new_member, GuildMember *verify_member);
	void OnMemberQuit(GuildMember *quit_member);
	void OnMemberKick(GuildMember *bekicker_member, GuildMember *kicker_member);
	void OnMemberAppoint(GuildMember *beappoint_member, GuildMember *appoint_member);
	void OnMemberLeavePost(GuildMember *leavepost_member);
	void OnTransferTuanzhang(GuildMember *old_tuanzhang_member, GuildMember *new_tuanzhang_member);
	void OnDelateTuanzhang(GuildMember *delate_member);
	void OnMemberAddExp(GuildMember *oper_member, int add_type, int add_value);
	//void OnStorageOperate(GuildMember *oper_member, int op_type, int item_id, int item_num);
	void OnStorageOperate(GuildMember *oper_member, int op_type, const ItemDataWrapper &wrapper);

	void OnRumorEvent(const char *name_1, const char *name_2, int event_type, int param0, int param1, int param2, int param3);

private:
	bool GetFreeEventSlot(bool bigevent, bool storageevent, int *pos, bool is_clear);

	Guild *m_guild;

	struct Event
	{
		Event() { this->Reset(); }

		void Reset()
		{
			event_type = GUILD_EVENT_TYPE_INVALID;
			memset(event_owner, 0, sizeof(event_owner));
			event_owner_post = GUILD_POST_INVALID;
			event_time = 0;

			big_event = 0;
			cannot_remove = 0;

			param0 = param1 = param2 = param3 = 0;
			memset(sparam0, 0, sizeof(sparam0));
		}

		bool Invalid() { return GUILD_EVENT_TYPE_INVALID == event_type; }

		int event_type;
		GameName event_owner;
		int event_owner_post;
		unsigned int event_time; 

		char big_event;									// 属于大事记
		char cannot_remove;								// 是否能被移除

		int param0;
		int param1;
		int param2;
		int param3;
		GuildEventStrParam sparam0;
	};

	Event m_event_list[MAX_ALL_GUILD_EVENT_COUNT];
	
	bool m_dirty_mark[MAX_ALL_GUILD_EVENT_COUNT];
	bool m_old_state[MAX_ALL_GUILD_EVENT_COUNT];
	bool m_change_flag;
};

#endif  // __GUILD_EVENT_MANAGER_HPP__

