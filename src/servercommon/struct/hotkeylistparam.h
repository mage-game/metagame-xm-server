#ifndef HOTKEYLISTPARAM_H
#define HOTKEYLISTPARAM_H

#include "common/tlvprotocol.h"

class HotkeyListParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct __changehotkey 
	{
		char change_state;                   //用于从gameserver到dataaccess时记录更新的状态
		char index;
		unsigned short item_id;
		char item_type;
	};

	static const int MAX_HOTKEY_NUM = 32;

	int count;
	__changehotkey hotkey_list[MAX_HOTKEY_NUM];
};

#endif

