#ifndef __SYSTEMMSGCODE_H__
#define __SYSTEMMSGCODE_H__

namespace Protocol
{
	enum
	{
		SYSTEM_CODE_SC_BEGIN = 700,
	};

	enum _MsgTypeSystem
	{
		MT_SYSTEM_NOTICE_CODE_SC = 700,								// ϵͳ֪ͨ
		MT_SYSTEM_MSG_SC,											// ϵͳ��Ϣ
		MT_SYSTEM_ZERO_HOUR_SC,										// 0��֪ͨ
		MT_SYSTEM_SPEAKER_SC,										// С����
	};
}

#endif

