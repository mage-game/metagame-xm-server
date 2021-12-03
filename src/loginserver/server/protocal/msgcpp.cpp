
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/userprotocal/loginmsgcode.h"
#include "msglogin.h"
#include "msgregister.h"

namespace Protocol
{
	CSLogin::CSLogin():header(MT_NEW_LOGIN_REQ_CS){}
	SCRoleList::SCRoleList():header(MT_ROLE_LIST_SC){}
	CSRoleReq::CSRoleReq():header(MT_NEW_ROLE_REQ_CS){}
	SCLoginAck::SCLoginAck():header(MT_LOGIN_ACK_SC){}
	SCGetThreadAck::SCGetThreadAck():header(MT_GET_THREAD_ACK_SC){}
	CSPassAntiWallow::CSPassAntiWallow():header(MT_ANTI_WALLOW_CS){}
	CSPassAntiWallowEx::CSPassAntiWallowEx():header(MT_ANTI_WALLOW_EX_CS){}
	SCAntiWallowRet::SCAntiWallowRet():header(MT_ANTI_WALLOW_EX_RET_SC){}
	SCAccountKeyError::SCAccountKeyError():header(MT_ACCOUNT_KEY_ERROR_SC){}
	SCMergeRoleList::SCMergeRoleList():header(MT_MERGE_ROLE_LIST_SC){}
	SCProfNumInfo::SCProfNumInfo() : header(MT_PROF_NUM_INFO_SC) {}
	SCLHeartBeat::SCLHeartBeat() : header(MT_L_HEARTBEAT_SC) {}

	CSCreateRole::CSCreateRole():header(MT_CREATE_ROLE_REQ_CS){}
	SCCreateRoleAck::SCCreateRoleAck():header(MT_CREATE_ROLE_ACK_SC){}
	CSDestroyRole::CSDestroyRole():header(MT_DESTROY_ROLE_REQ_CS){}
	SCDestroyRoleAck::SCDestroyRoleAck():header(MT_DESTROY_ROLE_ACK_SC){}
	CSGetThreadInfo::CSGetThreadInfo():header(MT_GET_TREAD_CS){}

	CSChangePlatName::CSChangePlatName():header(MT_CHANGE_PLATNAME_REQ_CS) {}
	SCChangePlatNameAck::SCChangePlatNameAck():header(MT_CHANGE_PLATNAME_ACK_SC) {}

	CSLHeartBeat::CSLHeartBeat():header(MT_L_HEARTBEAT_CS){};
}

