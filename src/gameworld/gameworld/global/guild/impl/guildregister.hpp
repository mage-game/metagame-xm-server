#ifndef __GUILD_REGISTER_HPP__
#define __GUILD_REGISTER_HPP__

#include <string>
#include "servercommon/serverdef.h"

class Guild;

class GuildRegister
{
public:
	GuildRegister();
	~GuildRegister();

	void SetGuild(Guild *guild) { m_guild = guild;}
	Guild * GetGuild() { return m_guild; }

	void OnGuildMemberCountChanged(int member_count) { m_member_count = member_count; }
	void OnGuildAddMember() { ++ m_member_count; }
	void OnGuildRemoveMember() { -- m_member_count; }
	int GetGuildMemberCount() { return m_member_count; }

	void OnGuildTuanZhangChanged(const UserID & user_id);
	UserID GetGuildTuanZhangInfo(GameName role_name, char *vip_level = NULL);
	char GetGuildTuanzhangVipLevel();
	bool IsTuanZhang(const UserID &user_id) { return user_id == m_tuanzhang_user_id; }
	char GetGuildTuanzhangProf();
	char GetGuildTuanzhangSex();
	long long GetGuildTuanzhangUuid();

private:
	Guild *m_guild;

	int m_member_count;							// 成员个数

	UserID m_tuanzhang_user_id;					// 团长ID
};

#endif // __GUILD_REGISTER_HPP__

