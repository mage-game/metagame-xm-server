#ifndef __HOTKEY_RECORDER_H__
#define __HOTKEY_RECORDER_H__

#include "gamedef.h"
#include "servercommon/struct/hotkeylistparam.h"

class Role;

class HotkeyRecorder
{
	static const int MAX_HOT_KET_NUM = 24;
	static const char INVALID_HOTKEY_TYPE = 0;

public:
	struct HotKeyItem
	{
		unsigned short item_id;
		char item_type;
	};

	HotkeyRecorder();
	~HotkeyRecorder();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const HotkeyListParam &hotkeyitemlist);
	
	bool ChangeHotkey(char index, unsigned short item_id, char item_type);
	void SendHotKeyListToRole();

	void GetHotkeyList(HotkeyListParam *p)const;
	void GetChangeHotKeyList(HotkeyListParam *p)const;
	void ClearDirtyMark();

private:
	Role			*m_role;

	HotKeyItem		m_hotkey_list[MAX_HOT_KET_NUM];

	bool			m_dirty_mark[MAX_HOT_KET_NUM];
	bool			m_old_state[MAX_HOT_KET_NUM];
	bool			m_hotkey_is_change;
};

#endif
