#ifndef __MSG_SC_GUILD_UNION_HPP_
#define __MSG_SC_GUILD_UNION_HPP_

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/servercommon.h"
#include "servercommon/guilddef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	static const int SC_GUILD_UNION_MAX_NUM = 128;

	class SCGuildUnionList
	{
	public:
		SCGuildUnionList();
		MessageHeader		header;

		struct GuildUnionInfo
		{
			GuildID guild_list[GUILD_UNION_MAX_GUILD_NUM];
		};

		int is_first;
		int get_type;
		int count;
		GuildUnionInfo union_list[SC_GUILD_UNION_MAX_NUM];
	};

	class SCGuildUnionApplyForList
	{
	public:
		SCGuildUnionApplyForList();
		MessageHeader		header;

		struct UnionApplyForInfo
		{
			GuildID guild_id;
			GuildName guild_name;
			unsigned int applyfor_time;
		};

		int count;
		UnionApplyForInfo applyfor_list[GUILD_UNION_APPLYFOR_MAX];
	};

	class SCGuildApplyForUnionGuildList
	{
	public:
		SCGuildApplyForUnionGuildList();
		MessageHeader		header;

		static const int MAX_APPLY_FOR_UNION_GUILD_COUNT = 1024;

		int					count;
		GuildID				applyfor_union_guild_list[MAX_APPLY_FOR_UNION_GUILD_COUNT];
	};

	class CSGetGuildUnionList
	{
	public:
		CSGetGuildUnionList();
		MessageHeader		header;

		enum
		{
			GET_GUILD_UNION_LIST_TYPE_MYGUILD = 0,	// 自己军团的联盟
			GET_GUILD_UNION_LIST_TYPE_ALL,			// 全部联盟
		};

		int					get_type;
	};

	class CSGetGuildUnionApplyForList
	{
	public:
		CSGetGuildUnionApplyForList();
		MessageHeader		header;

		GuildID				guild_id;
	};

	class CSGetGuildApplyForUnionGuildList
	{
	public:
		CSGetGuildApplyForUnionGuildList();
		MessageHeader		header;

		GuildID				guild_id;
	};

	class CSGuildUnionApplyFor
	{
	public:
		CSGuildUnionApplyFor();
		MessageHeader		header;

		GuildID guild_id;
		GuildID target_guild_id;
	};

	class CSGuildUnionApplyForAck
	{
	public:
		CSGuildUnionApplyForAck();
		MessageHeader		header;

		int result;							// 0为同意 其他不同意
		GuildID guild_id;
		GuildID target_guild_id;
	};

	class CSGuildUnionQuit
	{
	public:
		CSGuildUnionQuit();
		MessageHeader		header;

		GuildID guild_id;
	};
}

#pragma pack(pop)

#endif  // __MSG_SC_GUILD_UNION_HPP_

