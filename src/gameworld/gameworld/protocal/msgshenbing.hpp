#ifndef __MSG_SHENBING_H__
#define __MSG_SHENBING_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/shenbingparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class CSShenBingUpLevel								// 1764 �����������
	{
	public:
		CSShenBingUpLevel();
		MessageHeader header;

		short stuff_index;
		short resevre;
	};

	class SCAllShenBingInfo								// ���������Ϣ 1766
	{
	public:
		SCAllShenBingInfo();
		MessageHeader header;

		short level;
		short use_image;
		int exp;

		unsigned short shuxingdan_list[ShenBingParam::SHUXINGDAN_MAX_TYPE];			// ���Ե�����
		
	};
}

#pragma pack(pop)

#endif