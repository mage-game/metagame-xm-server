#ifndef __MSG_DRESSING_ROOM_HPP__
#define __MSG_DRESSING_ROOM_HPP__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/dressingroomdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum DRESSING_ROOM_OPEAR_TYPE
	{
		DRESSING_ROOM_OPEAR_TYPE_INFO = 0,				// 请求信息
	};

	class CSDressingRoomOpera							// 5245
	{
	public:
		CSDressingRoomOpera();
		MessageHeader header;

		int opera_type;
	};

	class SCDressingRoomInfo							// 5246
	{
	public:
		SCDressingRoomInfo();
		MessageHeader header;

		int suit_count = 0;										//套装数量
		int info_list[DressingRoomParam::MAX_SUIT_COUNT];		//套装部件激活情况
	};

	class SCDressingRoomSingleInfo						// 5247
	{
	public:
		SCDressingRoomSingleInfo();
		MessageHeader header;

		struct DressingRoomInfo
		{
			short suit_index = 0;
			short img_index = 0;
		};

		int is_active;				// 1激活0取消激活
		DressingRoomInfo info;		// 单个信息变化
	};

	class CSDressingRoomExchange							// 5248
	{
	public:
		CSDressingRoomExchange();
		MessageHeader header;

		int suit_index;										// 套装ID
		int sub_index;										// 部位ID
	};
}

#pragma pack(pop)

#endif // __MSG_DRESSING_ROOM_HPP__

