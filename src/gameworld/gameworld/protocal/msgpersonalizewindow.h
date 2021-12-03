#ifndef __MSG_PERSONALIZE_WINDOW_H__
#define __MSG_PERSONALIZE_WINDOW_H__

#include "servercommon/personalizewindowdef.hpp"
#include "servercommon/userprotocal/msgheader.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum PERSONALIZE_WINDOW_OPERA_TYPE
	{
		PERSONALIZE_WINDOW_BUBBLE_INFO = 0,					// �������ݿ���Ϣ
		PERSONALIZE_WINDOW_BUBBLE_UP_LEVEL = 1,				// �������ݿ�����	
		PERSONALIZE_WINDOW_BUBBLE_USE = 2,					// ʹ�����ݿ�����		

		PERSONALIZE_WINDOW_AVATAR_INFO = 3,					// ����ͷ�����Ϣ
		PERSONALIZE_WINDOW_AVATAR_UP_LEVEL = 4,				// ����ͷ�������	
		PERSONALIZE_WINDOW_AVATAR_USE = 5,					// ʹ��ͷ�������			
	};

	class CSPersonalizeWindowOperaReq			//5687			
	{
	public:
		CSPersonalizeWindowOperaReq();
		MessageHeader header;

		int req_type;
		int param_1;
		int param_2;
		int param_3;
	};

	class SCBubbleWindowInfo			// 6126
	{
	public:
		SCBubbleWindowInfo();
		MessageHeader header;

		short cur_use_bubble_type;
		short reserve;
		short bubble_level[BUBBLE_WINDOW_MAX_TYPE];
	};

	class SCAvatarWindowInfo			// 6127
	{
	public:
		SCAvatarWindowInfo();
		MessageHeader header;

		short cur_use_avatar_type;					//��ǰʹ��ͷ�������
		short reserve;
		short avatar_level[AVATAR_WINDOW_MAX_TYPE]; //ͷ���ȼ�
	};
}

#pragma pack(pop)

#endif 


