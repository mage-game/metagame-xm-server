#ifndef __MSG_PASTURESPIRIT_HPP__
#define __MSG_PASTURESPIRIT_HPP__

#include "servercommon/pasturespiritdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum PASTURESPIRIT_REQ_TYPE
	{
		PASTURE_SPIRIT_REQ_TYPE_ALL_INFO,							// ����������Ϣ
		PASTURE_SPIRIT_REQ_TYPE_UPGRADE,							// ��������
		PASTURE_SPIRIT_REQ_TYPE_PROMOTE_QUALITY,					// ������ʾƷ��
		PASTURE_SPIRIT_REQ_TYPE_AUTO_PROMOTE_QUALITY,				// ����һ����ʾƷ��
		PASTURE_SPIRIT_REQ_TYPE_FREE_DRAW_ONCE,						// ������ѳ�һ��
		PASTURE_SPIRIT_REQ_TYPE_LUCKY_DRAW_ONCE,					// ����齱һ��
		PASTURE_SPIRIT_REQ_TYPE_LUCKY_DRAW_TIMES,					// ����齱���

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

		int point;				// ���û���
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

		int point;				// ���û���
		unsigned int last_free_draw_timestamp;
		short reward_count;
		char reward_seq_list[SC_PASTURE_SPIRIT_LUCKY_DRAW_RESULT_MAX_COUNT];
	};
}

#pragma pack(pop)

#endif