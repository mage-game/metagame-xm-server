#include "hotkeyrecorder.h"
#include "servercommon/struct/structcommon.h"
#include "protocal/msghotkey.h"
#include "obj/character/role.h"
#include "engineadapter.h"
#include "gamelog.h"

HotkeyRecorder::HotkeyRecorder()
	: m_role(NULL), m_hotkey_is_change(false)
{
	memset(m_hotkey_list,0, sizeof(m_hotkey_list));

	memset(m_dirty_mark,0,sizeof(m_dirty_mark));
    memset(m_old_state,0,sizeof(m_old_state));
}

HotkeyRecorder::~HotkeyRecorder()
{
}

void HotkeyRecorder::Init(Role *role, const HotkeyListParam& hotkeyitemlist)
{
	m_role = role;

	int count =  0;

	if (hotkeyitemlist.count < HotkeyListParam::MAX_HOTKEY_NUM)
	{
		count = hotkeyitemlist.count;
	} 
	else
	{
		count = HotkeyListParam::MAX_HOTKEY_NUM;
	}

	for (int i = count - 1; i >= 0; --i)
	{
		int index = hotkeyitemlist.hotkey_list[i].index;
		if (index < 0 || index >= MAX_HOT_KET_NUM) 
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[HotkeyRecorder:Init][[%s %d] index is invalid:%d]",
				role->GetName(), UidToInt(role->GetUserId()), index);
			continue;
		}

		if (m_hotkey_list[index].item_type != INVALID_HOTKEY_TYPE) 
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[HotkeyRecorder:Init][[%s %d] index duplicate:%d]",
				role->GetName(), UidToInt(role->GetUserId()), index);
			continue;
		}

		m_hotkey_list[index].item_id = hotkeyitemlist.hotkey_list[i].item_id;
		m_hotkey_list[index].item_type = hotkeyitemlist.hotkey_list[i].item_type;
		m_old_state[index] = true;
	}
}

bool HotkeyRecorder::ChangeHotkey(char index,unsigned short item_id,char item_type)
{
	if (index >= 0 && index < MAX_HOT_KET_NUM)
	{
		m_hotkey_list[(int)index].item_id = item_id;
		m_hotkey_list[(int)index].item_type = item_type;
		m_dirty_mark[(int)index] = true;
		m_hotkey_is_change = true;

		return true;
	}
	
	return false;
}

void HotkeyRecorder::SendHotKeyListToRole()
{
	static Protocol::SCHotkeyInfoAck hia;

	int hotkeycount = 0;

	for (int i = 0; i < HotkeyRecorder::MAX_HOT_KET_NUM; ++i)
	{
		if (m_hotkey_list[i].item_type == INVALID_HOTKEY_TYPE) continue;
		
		hia.item_list[hotkeycount].index = i;
		hia.item_list[hotkeycount].item_id = m_hotkey_list[i].item_id;
		hia.item_list[hotkeycount].type = m_hotkey_list[i].item_type;

		++ hotkeycount;

		if (hotkeycount >= MAX_HOTKEY_INFO_NUM) break;
	}

	hia.count = hotkeycount;

	unsigned int sendlen = sizeof(Protocol::SCHotkeyInfoAck) - sizeof(Protocol::HotkeyItem) * (MAX_HOTKEY_INFO_NUM - hia.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&hia, sendlen);
}

void HotkeyRecorder::GetHotkeyList(HotkeyListParam *p)const
{
	int count = 0;

	for (int i = 0; i < HotkeyRecorder::MAX_HOT_KET_NUM && count < HotkeyListParam::MAX_HOTKEY_NUM; ++i)
	{
		if (m_hotkey_list[i].item_type == INVALID_HOTKEY_TYPE) continue;
		
		p->hotkey_list[count].change_state = structcommon::CS_NONE;
		p->hotkey_list[count].index = i ;
		p->hotkey_list[count].item_id = m_hotkey_list[i].item_id;
		p->hotkey_list[count].item_type = m_hotkey_list[i].item_type;
		++count;
	}

	p->count = count; 
}

void HotkeyRecorder::GetChangeHotKeyList(HotkeyListParam *p)const
{
	if (!m_hotkey_is_change)
	{
		p->count = 0;
		return;
	}

	int count = 0;

	for (int i = 0; i < HotkeyRecorder::MAX_HOT_KET_NUM && count < HotkeyListParam::MAX_HOTKEY_NUM; ++i)
	{
		if(!m_dirty_mark[i]) continue;

		if (m_hotkey_list[i].item_type != INVALID_HOTKEY_TYPE)
		{
			p->hotkey_list[count].change_state = (m_old_state[i] ? structcommon::CS_UPDATE : structcommon::CS_INSERT);
			p->hotkey_list[count].index = i;
			p->hotkey_list[count].item_type = m_hotkey_list[i].item_type;
			p->hotkey_list[count].item_id = m_hotkey_list[i].item_id;
		}
		else
		{
			if (!m_old_state[i]) continue;

			p->hotkey_list[count].change_state = structcommon::CS_DELETE;
			p->hotkey_list[count].index = i;
			p->hotkey_list[count].item_type = INVALID_HOTKEY_TYPE;
			p->hotkey_list[count].item_id = -1;
		}

		++count;
	}

	p->count = count;
}

void HotkeyRecorder::ClearDirtyMark()
{
	if (!m_hotkey_is_change) return;

	for (int i = 0; i < HotkeyRecorder::MAX_HOT_KET_NUM; ++i)
	{
		m_old_state[i] = (m_hotkey_list[i].item_type != HotkeyRecorder::INVALID_HOTKEY_TYPE);
	}

	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));

	m_hotkey_is_change = false;
}

