#ifndef __MSG_BOARD_ITEM_H__
#define __MSG_BOARD_ITEM_H__

#include "protocal/msgchatmsg.h"
#include <stdlib.h>

class MsgBoardItem
{
public:
	MsgBoardItem();
	~MsgBoardItem();

	void SetMsg(const char *msg, int len);
	const char * GetSendBuffer(int *send_len);

	void * operator new(size_t c);
	void operator delete(void *m);

private:
	const static int PROTOCAL_MAX_LEN = 512;
	char m_msg[MAX_CHAT_LENGTH + PROTOCAL_MAX_LEN];
	int m_msg_len;
};

#endif

