#ifndef __MSG_PASTURESPIRIT_HPP__
#define __MSG_PASTURESPIRIT_HPP__

#include "servercommon/pasturespiritdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum PASTURESPIRIT_REQ_TYPE
	{
		PASTURE_SPIRIT_REQ_TYPE_ALL_INFO,							// 请求所有信息
		PASTURE_SPIRIT_REQ_TYPE_UPGRADE,							// 请求升级
		PASTURE_SPIRIT_REQ_TYPE_PROMOTE_QUALITY,					// 请求提示品质
		PASTURE_SPIRIT_REQ_TYPE_AUTO_PROMOTE_QUALITY,				// 请求一键提示品质
		PASTURE_SPIRIT_REQ_TYPE_FREE_DRAW_ONCE,						// 请求免费抽一次
		PASTURE_SPIRIT_REQ_TYPE_LUCKY_DRAW_ONCE,					// 请求抽奖一次
		PASTURE_SPIRIT_REQ_TYPE_LUCKY_DRAW_TIMES,					// 请求抽奖多次

		PASTURESPIRIT_REQ_TYPE_MAX,
	};

	class CSPastureSpiritOperaReq		// 5685
	{
	public:
		CSPastureSpiritOperaReq();
		MessageHeader header;

		unsigned short req_type;
		unsigned short param_1;
		unsigned short param_2;
		unsigned short param_3;
	};

	class SCPastureSpiritAllInfo			// 6120
	{
	public:
		SCPastureSpiritAllInfo();
		MessageHeader header;

		int point;				// 可用积分
		unsigned int last_free_draw_timestamp;
		PastureSpiritItem pasture_spirit_item_list[PASTURE_SPIRIT_MAX_COUNT];
	};

	class SCPastureSpiritSinglelInfo	// 6121
	{
	public:
		SCPastureSpiritSinglelInfo();
		MessageHeader header;

		int id;
		PastureSpiritItem pasture_spirit_item;
	};

	static const int SC_PASTURE_SPIRIT_LUCKY_DRAW_RESULT_MAX_COUNT = 50;
	class SCPastureSpiritLuckyDrawResult	// 6122
	{
	public:
		SCPastureSpiritLuckyDrawResult();
		MessageHeader header;

		int point;				// 可用积分
		unsigned int last_free_draw_timestamp;
		short reward_count;
		char reward_seq_list[SC_PASTURE_SPIRIT_LUCKY_DRAW_RESULT_MAX_COUNT];
	};
}

#pragma pack(pop)

#endif