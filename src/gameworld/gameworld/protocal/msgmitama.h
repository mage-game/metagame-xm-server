#ifndef __MSG_MITAMA_HPP__
#define __MSG_MITAMA_HPP__

#include "servercommon/mitamadef.hpp"
#include "servercommon/userprotocal/msgheader.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum MITAMA_REQ_TYPE
	{
		MITAMA_REQ_TYPE_ALL_INFO = 0,				// 请求所有信息
		MITAMA_REQ_TYPE_UPGRADE = 1,				// 升级御魂
		MITAMA_REQ_TYPE_TASK_FIGHTING = 2,			// 出征
		MITAMA_REQ_TYPE_TASK_AWARD = 3,				// 领取出征奖励
		MITAMA_REQ_TYPE_EXCHANGE_ITEM = 4,			// 兑换物品

		MITAMA_REQ_TYPE_MAX,
	};

	class CSMitamaOperaReq		// 6134
	{
	public:
		CSMitamaOperaReq();
		MessageHeader header;

		unsigned short req_type;
		unsigned short param_1;
		unsigned short param_2;
		unsigned short param_3;
	};

	class SCMitamaAllInfo		// 6135
	{
	public:
		SCMitamaAllInfo();
		MessageHeader header;

		int hotspring_score;
		MitamaInfo info_list[MITAMA_MAX_MITAMA_COUNT];
	};

	class SCMitamaSingleInfo		// 6136
	{
	public:
		SCMitamaSingleInfo();
		MessageHeader header;

		int seq;
		MitamaInfo info;
	};

	class SCMitamaHotSpringScore		// 6137
	{
	public:
		SCMitamaHotSpringScore();
		MessageHeader header;

		int hotspring_score;
	};
}

#pragma pack(pop)

#endif
