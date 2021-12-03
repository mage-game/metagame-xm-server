
#ifndef MSGHOTKEY_H
#define MSGHOTKEY_H

#include "servercommon/userprotocal/msgheader.h"


#pragma pack(push) 
#pragma pack(4)



namespace Protocol
{
	/*
		快捷键相关操作
	*/

	class CSHotkeyInfoReq			// 1950
	{
	public:
		CSHotkeyInfoReq();
		MessageHeader		header;
	};

	#define MAX_HOTKEY_INFO_NUM	36

	struct HotkeyItem  
	{
		char	index;
		char	type;
		UInt16	item_id;
	};

	class SCHotkeyInfoAck		// 1900
	{
	public:
		SCHotkeyInfoAck();
		MessageHeader		header;
		int					count;			
		HotkeyItem		item_list[MAX_HOTKEY_INFO_NUM];		// 36
	};

	class CSChangeHotkey		// 1951
	{
	public:
		CSChangeHotkey();
		MessageHeader		header;
		char	index;
		char	type;
		UInt16	item_id;
	};
}


#pragma pack(pop)




#endif
