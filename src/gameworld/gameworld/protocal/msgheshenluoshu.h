#ifndef MSG_HESHENLUOSHUO_H
#define MSG_HESHENLUOSHUO_H

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/heshenluoshudef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum HESHENLUOSHU_REQ_TYPE
	{
		HESHENLUOSHU_REQ_TYPE_ACTIVATION = 0,			// �������鼤�� param1 item_id
		HESHENLUOSHU_REQ_TYPE_UPGRADELEVEL,				// ������������ param1 item_id
		HESHENLUOSHU_REQ_TYPE_DECOMPOSE,				// ��������ֽ� param1 param2 param3 �ֽ���Ʒ����index
	};

	class CSHeShenLuoShuReq
	{
	public:
		CSHeShenLuoShuReq();				//8808
		MessageHeader header;

		short opera_type;					//��������
		short param1;
	};

	class SCHeShenLuoShuAllInfo				//8809
	{
	public:
		SCHeShenLuoShuAllInfo();
		MessageHeader header;

		HeShenLuoShuParam all_info;
	};

	class SCHeShenLuoShuChangeInfo		// 8810   ��������ȼ��ı�		
	{
	public:
		SCHeShenLuoShuChangeInfo();
		MessageHeader header;
		short param1;					//type
		short param2;					//seq
		short param3;					//index
		short param4;					//level
		short param5;					//shenghua_level
		short param6;					//shenghua_exp
	};
}

#pragma pack(pop)

#endif

