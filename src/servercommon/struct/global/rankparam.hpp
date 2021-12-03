/******************************************************************************************
FileName: rankparam.hpp
Author: dreamwang
Description: 排行榜在数据中的定义  
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/26/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __RANK_PARAM_HPP__
#define __RANK_PARAM_HPP__

#include "servercommon/serverdef.h"
#include "servercommon/rankdef.hpp"
#include "common/tlvprotocol.h"

class PersonRankParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct DBPersonRankItem
	{
		char change_state;
		int index;
		
		int uid;
		GameName user_name;
		GameName flexible_name;
		char reserve;
		char sex;
		char prof;
		char camp;
		int vip_level;

		long long exp;
		int level;
		int rank_type;
		long long rank_value;
		int record_index;
		int flexible_int;
		long long flexible_ll;
	};

	int count;
	DBPersonRankItem person_rank_list[PERSON_RANK_TYPE_MAX * PERSON_RANK_MAX_SIZE];
};

class TeamRankParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct DBTeamRankItem
	{
		char change_state;
		int index;
		
		TeamRankMemberInfo member_list[MAX_TEAM_MEMBER_NUM];
		int rank_type;
		long long rank_value;
		int flexible_int;
		long long flexible_ll;
	};

	int count;
	DBTeamRankItem team_rank_list[TEAM_RANK_TYPE_MAX * TEAM_RANK_MAX_SIZE];
};

class CrossPersonRankParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct DBCrossPersonRankItem
	{
		char change_state;
		int index;

		long long uuid;
		GameName user_name;
		int level;
		char prof;
		char sex;
		char camp;
		long long exp;
		int rank_type;
		long long rank_value;
		int record_index;
		long long flexible_ll;
		GameName flexible_name;
		int flexible_int;
		int vip_level;
	};

	int count;
	DBCrossPersonRankItem cross_person_rank_list[CROSS_PERSON_RANK_TYPE_MAX * CROSS_PERSON_RANK_MAX_SIZE];
};

#endif  // __RANK_PARAM_HPP__

