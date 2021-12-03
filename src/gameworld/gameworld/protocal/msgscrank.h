/***************************************************************************
FileName: msgscrank.h
Author: fyh
Description: ���а� ������-�ͻ��� ���Э�� 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
fyh		   2011/09/06/ 17:48:05     1.0           ����
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
			int male_uid;			// ��uid
			int female_uid;			// Ůuid
			GameName male_name;		// ������
			GameName female_name;	// Ů����
			char male_prof;			// ��ְҵ
			char female_prof;		// Ůְҵ
			short reserve_sh;		// ����
			long long male_avatar;	// ��ͷ��
			long long female_avatar;// Ůͷ��
			int male_rank_value;	// ������ֵ
			int female_rank_value;	// Ů����ֵ
		};

		int rank_type;
		int item_count;
		RankItem rank_item_list[COUPLE_RANK_MAX_SIZE];
	};
}

#pragma pack(pop)

#endif // __MSG_SC_RANK_HPP__
