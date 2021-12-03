#ifndef __MSG_CROSS_COMMON_H__
#define __MSG_CROSS_COMMON_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/userprotocal/crossmsgcode.h"

#include "servercommon/crossdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	struct SCReturnOriginalServer    // 14000 ֪ͨ��һ�ԭ��(�ͻ��˽��մ���Ϣ��ʼ�������߲��ص�ԭ������)
	{
		SCReturnOriginalServer() : header(MT_RETURN_ORIGINAL_SERVER_SC) {}
		MessageHeader	header;
	};
}

#pragma pack(pop)

#endif // __MSG_CROSS_COMMON_H__
