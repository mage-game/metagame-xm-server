#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/userprotocal/systemmsgcode.h"
#include "servercommon/userprotocal/crossmsgcode.h"

#include "servercommon/userprotocal/msgsystem.h"

#include "msgcross.h"

namespace Protocol
{
	SCNoticeNum::SCNoticeNum():header(MT_SYSTEM_NOTICE_CODE_SC){}
	SCSystemMsg::SCSystemMsg():header(MT_SYSTEM_MSG_SC){}
}

