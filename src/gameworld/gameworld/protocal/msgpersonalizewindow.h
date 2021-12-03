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
		PERSONALIZE_WINDOW_BUBBLE_INFO = 0,					// 请求气泡框信息
		PERSONALIZE_WINDOW_BUBBLE_UP_LEVEL = 1,				// 升级气泡框请求	
		PERSONALIZE_WINDOW_BUBBLE_USE = 2,					// 使用气泡框请求		

		PERSONALIZE_WINDOW_AVATAR_INFO = 3,					// 请求头像框信息
		PERSONALIZE_WINDOW_AVATAR_UP_LEVEL = 4,				// 升级头像框请求	
		PERSONALIZE_WINDOW_AVATAR_USE = 5,					// 使用头像框请求			
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

		short cur_use_avatar_type;					//当前使用头像框类型
		short reserve;
		short avatar_level[AVATAR_WINDOW_MAX_TYPE]; //头像框等级
	};
}

#pragma pack(pop)

#endif 


