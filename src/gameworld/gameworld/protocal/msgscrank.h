/***************************************************************************
FileName: msgscrank.h
Author: fyh
Description: 排行榜 服务器-客户端 相关协议 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
fyh		   2011/09/06/ 17:48:05     1.0           创建
**************************************************************************/

#ifndef __MSG_SC_RANK_HPP__
#define __MSG_SC_RANK_HPP__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/serverdef.h"
#include "servercommon/struct/global/publicsaleparam.hpp"
#include "servercommon/rankdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCGetPersonRankListAck	// 10000
	{
	public:
		SCGetPersonRankListAck();

		MessageHeader header;

		struct PersonRankListItem
		{
			int user_id;
			GameName user_name;
			GameName flexible_name;
			char sex;
			char prof;
			char camp;
			char vip_level;
			short jingjie_level;
			short record_index;
			long long exp;
			int level;
			long long rank_value;
			int flexible_int;
			long long flexible_ll;
			long long avatar_timestamp;
		};

		unsigned int last_snapshot_time;
		int rank_type;
		int count;
		PersonRankListItem  rank_list[PERSON_RANK_MAX_SIZE];
	};

	class SCGetPersonRankTopUser
	{
	public:
		SCGetPersonRankTopUser();

		MessageHeader header;

		int rank_type;

		int user_id;
		GameName user_name;
		char sex;
		char prof;
		char camp;
		char reserved;
		int level;
		long long rank_value;
	};

	class SCGetWorldLevelAck
	{
	public:
		SCGetWorldLevelAck();

		MessageHeader header;

		int world_level;
		int top_user_level;
	};

	class SCGetGuildRankListAck
	{
	public:
		SCGetGuildRankListAck();

		MessageHeader header;

		struct GuildRankListItem
		{
			GuildID guild_id;
			GuildName guild_name;
			int tuan_zhang_uid;
			GameName tuan_zhang_name;
			short guild_level;
			short max_member_count;
			short camp;
			short member_count;
			long long rank_value;
		};

		int rank_type;
		int rank_count;
		GuildRankListItem rank_list[GUILD_RANK_MAX_SIZE];
	};

	class SCGetTeamRankListAck
	{
	public:
		SCGetTeamRankListAck();

		MessageHeader header;

		struct TeamRankListItem
		{
			TeamRankMemberInfo member_list[MAX_TEAM_MEMBER_NUM];
			long long rank_value;
			int flexible_int;
			long long flexible_ll;
		};

		int rank_type;
		int rank_count;
		TeamRankListItem rank_list[TEAM_RANK_MAX_SIZE];
	};

	class SCGetCoupleRankListAck			// 10005
	{
	public:
		SCGetCoupleRankListAck();
		MessageHeader header;

		struct RankItem
		{
			int male_uid;			// 男uid
			int female_uid;			// 女uid
			GameName male_name;		// 男名字
			GameName female_name;	// 女名字
			char male_prof;			// 男职业
			char female_prof;		// 女职业
			short reserve_sh;		// 保留
			long long male_avatar;	// 男头像
			long long female_avatar;// 女头像
			int male_rank_value;	// 男排行值
			int female_rank_value;	// 女排行值
		};

		int rank_type;
		int item_count;
		RankItem rank_item_list[COUPLE_RANK_MAX_SIZE];
	};
}

#pragma pack(pop)

#endif // __MSG_SC_RANK_HPP__
