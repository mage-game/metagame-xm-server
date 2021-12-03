#ifndef __MSG_HUASHEN_HPP__
#define __MSG_HUASHEN_HPP__

#include "servercommon/huashendef.hpp"
#include "servercommon/userprotocal/msgheader.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum HUASHEN_REQ_TYPE
	{
		HUASHEN_REQ_TYPE_ALL_INFO = 0,
		HUASHEN_REQ_TYPE_CHANGE_IMAGE = 1,		// ÇÐ»»ÐÎÏó
		HUASHEN_REQ_TYPE_UP_LEVEL = 2,
		HUASHEN_REQ_TYPE_SPIRIT_INFO = 3,
		HUASHEN_REQ_TYPE_UPGRADE_SPIRIT = 4,
		HUASHEN_REQ_TYPE_UP_GRADE = 5,

		HUASHEN_REQ_TYPE_MAX,
	};

	class SCHuaShenAllInfo						// 6115
	{
	public:
		SCHuaShenAllInfo();
		MessageHeader header;

		char active_flag;
		char cur_huashen_id;
		short dirty_sh;
		HuaShenLevelInfo level_info_list[HUASHEN_MAX_ID_LIMIT + 1];
		short grade_list[HUASHEN_MAX_ID_LIMIT + 1];
	};

	class SCHuaShenSpiritInfo					// 6129
	{
	public:
		SCHuaShenSpiritInfo();
		MessageHeader header;

		int huashen_id;
		HuaShenSpiritInfo::SingleSpirit spirit_list[HUASHEN_SPIRIT_MAX_ID_LIMIT + 1];
	};

	class CSHuaShenOperaReq						// 6116
	{
	public:
		CSHuaShenOperaReq();
		MessageHeader header;

		unsigned short req_type;
		unsigned short param_1;
		unsigned short param_2;
		unsigned short param_3;
	};
}

#pragma pack(pop)

#endif
