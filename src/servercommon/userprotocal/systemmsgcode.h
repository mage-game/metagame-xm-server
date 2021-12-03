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
		MT_SYSTEM_NOTICE_CODE_SC = 700,								// 系统通知
		MT_SYSTEM_MSG_SC,											// 系统消息
		MT_SYSTEM_ZERO_HOUR_SC,										// 0点通知
		MT_SYSTEM_SPEAKER_SC,										// 小喇叭
	};
}

#endif

