#include "hotkeylistparam.h"
#include "servercommon/struct/structcommon.h"

bool HotkeyListParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(count);
	if (!ret)
	{
		return false;
	}

	for (int i = 0; i < count && i < MAX_HOTKEY_NUM; ++i)
	{
		ret = outstream.Push(hotkey_list[i].change_state)
			&& outstream.Push(hotkey_list[i].index);
		if (!ret)
		{
			return false;
		}
		if (hotkey_list[i].change_state == structcommon::CS_DELETE)
		{
			continue;
		}
		ret = outstream.Push(hotkey_list[i].item_id) 
			&& outstream.Push(hotkey_list[i].item_type);
		if (!ret)
		{
			return false;
		}
	}

	return ret;
}

bool HotkeyListParam::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&count);
	if (!ret)
	{
		return false;
	}

	for (int i = 0; i < count && i < MAX_HOTKEY_NUM; ++i)
	{
		ret = instream.Pop(&hotkey_list[i].change_state)
			&& instream.Pop(&hotkey_list[i].index) ;
		if (!ret)
		{
			return false;
		}
		if (hotkey_list[i].change_state == structcommon::CS_DELETE)
		{
			continue;
		}
		ret = instream.Pop(&hotkey_list[i].item_id) 
			&& instream.Pop(&hotkey_list[i].item_type);
		if (!ret)
		{
			return false;
		}
	}
	return ret;
}

