#include "common/md5/md5cpp.h"
#include "internalcomm.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "servercommon/internalprotocal/crossglobalprotocal.h"
#include "servercommon/servercommon.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "global/cross/crossmanager.h"

void InternalComm::Init(INetworkModule *internal_network)
{
	m_internal_network = internal_network;
}

void InternalComm::SetSesstionKeyBase(const std::string &session_key_base)
{
	MD5 md5;
	MD5_buffer(&md5, session_key_base.c_str(), (unsigned long)session_key_base.size());
	MD52String(&m_session_key_base, md5);
}


void InternalComm::SetDBSession(const rmi::Session &accounter, rmi::Session *role_db, unsigned int role_db_num, const rmi::Session &global_session, 
								const rmi::Session &systm_session, const rmi::Session &name_session)
{
	m_db_handler.Init(accounter, role_db, role_db_num, global_session, systm_session, name_session);
}

bool InternalComm::SendToCross(const char *data, int length, int cross_func_type)
{
	if (NULL == data || length <= 0 || length > crossgameprotocal::MAX_CROSSGAMEPROTOCAL_LENGTH) return false;
	return CrossManager::Instance().SendToCrossWithFuncType(cross_func_type, data, length);
}

static char s_tmp_buffer[MAX_MSG_SIZE] = {0};
bool InternalComm::SendToCrossTransferToGame(int transfer_type, long long param, const char *data, int length)
{
	static int base_length = (int)sizeof(crossgameprotocal::GameCrossRransferToGame);
	if (length <= 0 || base_length + length > MAX_MSG_SIZE) return false;

	crossgameprotocal::GameCrossRransferToGame *gcrtg = (crossgameprotocal::GameCrossRransferToGame *)s_tmp_buffer;
	*gcrtg = crossgameprotocal::GameCrossRransferToGame();
	gcrtg->length = length;
	gcrtg->transfer_type = transfer_type;
	gcrtg->param = param;

	memcpy(s_tmp_buffer + base_length, data, length);

	return InternalComm::Instance().SendToCross((const char *)s_tmp_buffer, base_length + length);
}

bool InternalComm::UserMsgTransmitToCross(const UserID &user_id, int msg_type, const char *data, int length)
{
	static int base_length = (int)sizeof(crossgameprotocal::GameCrossUserMsg);
	if (length <= 0 || base_length + length > MAX_MSG_SIZE) return false;

	static const int PLAT_TYPE = LocalConfig::Instance().GetPlatType();

	crossgameprotocal::GameCrossUserMsg *global_cross_user_msg = (crossgameprotocal::GameCrossUserMsg *)s_tmp_buffer;
	*global_cross_user_msg = crossgameprotocal::GameCrossUserMsg();
	global_cross_user_msg->uuid = UniqueUserIDToLongLong(UniqueUserID(PLAT_TYPE, user_id));
	global_cross_user_msg->merge_server_id = LocalConfig::Instance().GetMergeServerId();

	memcpy(s_tmp_buffer + base_length, data, length);

	return this->SendToCross((const char *)s_tmp_buffer, base_length + length);
}

bool InternalComm::UserProtocalTransferToGame(long long uuid, const char *data, int length)
{
	static int base_length = (int)sizeof(crossgameprotocal::GameCrossUserProtocalTransferToGame);
	if (length <= 0 || base_length + length > MAX_MSG_SIZE) return false;

	crossgameprotocal::GameCrossUserProtocalTransferToGame *gcupttg = (crossgameprotocal::GameCrossUserProtocalTransferToGame *)s_tmp_buffer;
	*gcupttg = crossgameprotocal::GameCrossUserProtocalTransferToGame();
	gcupttg->uuid = uuid;
	gcupttg->protocal_len = length;

	memcpy(s_tmp_buffer + base_length, data, length);

	return this->SendToCross((const char *)s_tmp_buffer, base_length + length);
}

InternalComm::InternalComm(): m_internal_network(NULL), m_login_server_id(-1), m_golbal_server_id(-1),
	m_reconnet_login_handle(-1)
{

}

InternalComm::~InternalComm()
{

}

