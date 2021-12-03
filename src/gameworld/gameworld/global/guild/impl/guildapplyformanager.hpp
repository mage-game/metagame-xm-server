#ifndef __GUILD_APPLYFOR_MANAGER_HPP__
#define __GUILD_APPLYFOR_MANAGER_HPP__

#include "servercommon/guilddef.hpp"
#include "servercommon/serverdef.h"
#include "servercommon/vipdef.hpp"

#include "servercommon/struct/global/guildparam.hpp"

#include "protocal/msgscguild.hpp"

class Guild;

class GuildApplyForManager
{
public:
	struct ApplyFor
	{
		ApplyFor()
			: apply_user_id(INVALID_USER_ID), applyfor_time(0)
		{
		}

		bool Invalid() { return INVALID_USER_ID == apply_user_id; }
		void Reset() { apply_user_id = INVALID_USER_ID; applyfor_time = 0; }

		UserID apply_user_id;
		unsigned int applyfor_time;
	};

	GuildApplyForManager();
	~GuildApplyForManager();

	void SetGuild(Guild *guild) { m_guild = guild; }
	Guild * GetGuild() { return m_guild; }

	void Init(const GuildApplyForParam & apply_for_param);
	void GetChangeApplyForList(GuildApplyForParam *apply_for_param);
	void ClearDirtyMark();

	void Update(unsigned long interval, time_t now_second);
	
	bool AddApplyFor(const UserID &user_id);
	ApplyFor * GetApplayFor(const UserID &user_id);
	bool RemoveApplyFor(const UserID &user_id);
	bool ApplyForListIsEmpty();
	void GetApplyForList(Protocol::SCGuildGetApplyForList *guild_apply_for_list);
	int GetApplyForList(UserID apply_for_userid_list[MAX_APPLY_FOR_COUNT]);

	bool AddInvite(const UserID &user_id);
	bool RemoveInvite(const UserID &user_id);
	bool IsbeInvite(const UserID &user_id);

private:
	Guild *m_guild;

	ApplyFor m_apply_for_list[MAX_APPLY_FOR_COUNT];

	bool m_dirty_mark[MAX_APPLY_FOR_COUNT];
	bool m_old_state[MAX_APPLY_FOR_COUNT];
	bool m_change_flag;

	struct InviteRecord
	{
		InviteRecord() : beinvite_user_id(INVALID_USER_ID), invite_time(0) {}

		bool Invalid() { return INVALID_USER_ID == beinvite_user_id; }
		void Reset() { beinvite_user_id = INVALID_USER_ID; invite_time = 0; }

		UserID beinvite_user_id;
		unsigned int invite_time;
	};
	InviteRecord m_invite_list[MAX_INVITE_COUNT];
};

#endif // __GUILD_APPLYFOR_MANAGER_HPP__
