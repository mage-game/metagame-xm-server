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
		DRESSING_ROOM_OPEAR_TYPE_INFO = 0,				// ������Ϣ
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

		int suit_count = 0;										//��װ����
		int info_list[DressingRoomParam::MAX_SUIT_COUNT];		//��װ�����������
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

		int is_active;				// 1����0ȡ������
		DressingRoomInfo info;		// ������Ϣ�仯
	};

	class CSDressingRoomExchange							// 5248
	{
	public:
		CSDressingRoomExchange();
		MessageHeader header;

		int suit_index;										// ��װID
		int sub_index;										// ��λID
	};
}

#pragma pack(pop)

#endif // __MSG_DRESSING_ROOM_HPP__

