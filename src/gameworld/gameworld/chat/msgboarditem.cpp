#include "msgboarditem.h"
#include <string.h>
#include "servercommon/servercommon.h"

MsgBoardItem::MsgBoardItem() : m_msg_len(0)
{
	memset(m_msg, 0, sizeof(m_msg));
}

MsgBoardItem::~MsgBoardItem() 
{

}

void MsgBoardItem::SetMsg(const char *msg_content, int len)
{
	m_msg_len = 0;

	if (NULL != msg_content && len > 0 && len <= (int)sizeof(m_msg))
	{
		memcpy(m_msg, msg_content, len);
		m_msg_len = len;
	}
}

const char * MsgBoardItem::GetSendBuffer(int *send_len)
{
	if (NULL != send_len) *send_len = m_msg_len;
	return m_msg;
}
