#ifndef __MSG_MOJIE_H__
#define __MSG_MOJIE_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/mojieparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCMojieInfo				// 5050 魔戒信息
	{
	public:
		SCMojieInfo();
		MessageHeader header;

		MojieParam::MojieInfo mojie_list[MojieParam::MOJIE_MAX_TYPE];
	};

	class CSMojieGetInfo			// 5075 请求魔戒信息
	{
	public:
		CSMojieGetInfo();	
		MessageHeader header;
	};

	class CSMojieUplevelReq
	{
	public:
		CSMojieUplevelReq();		// 5076 魔戒升级请求
		MessageHeader header;

		int mojie_type;
		char is_auto_buy;
		char reserve_1;
		short resrver_2;
	};

	class CSMojieChangeSkillReq		// 5077 请求改变魔戒技能
	{
	public:
		CSMojieChangeSkillReq();
		MessageHeader header;

		unsigned short mojie_skill_id;
		unsigned short mojie_skill_type;
		int mojie_skill_level;
	};
}

#pragma pack(pop)

#endif // 

