#include <memory.h>

#include "platform.h"
#include "platform91wan.h"
#include "platform4399.h"
#include "platformyujun.h"
#include "platform360.h"
#include "platform336.h"
#include "engineadapter.h"
#include "servercommon/servercommon.h"
#include "servercommon/serverconfig/serverconfigpool.h"
#include "servercommon/serverconfig/localconfig.hpp"

#include "server/protocal/msglogin.h"

#include "server/loginlog.h"

OLD_USE_INTERNAL_NETWORK();

Platform * Platform::Create()
{
	switch(ServerConfigPool::Instance().common_config.login_server_account_check_type)
	{
	case CommonConfig::PLAT_TYPE_YOUKE:
		return new Platform();	// 不验证，则是游客模式
		break;

	default:
		return new Platform();
	};
}

bool Platform::CheckAccount(NetID netid, PlatName pname, unsigned int login_time, LoginStr str, short server)
{
	if (pname[0] == 0) return false;

	if (!LocalConfig::Instance().IsServerIdAllowed(server))
	{
		loginlog::g_log_login.printf(LL_INFO, "[Platform::CheckAccount server_id Not Allowed] pname:%s server_id:%d", pname, server);

		return false;
	}

	for (int i = 0; i< (int)sizeof(LoginStr); ++i)
	{
		if (str[i] >= 'a' && str[i] <= 'z')
		{
			str[i] -= 32;
		}
	}
	
	if (Check(netid, pname, login_time, str, server))
	{
		PlatNameCatPostfix(pname, server);
		return true;
	}

	Protocol::SCAccountKeyError ake;
	EngineAdapter::Instance().NetSend(netid, (const char *)&ake, sizeof(ake));

	// EngineAdapter::Instance().NetDisconnect(netid);

	return false;
}

bool Platform::Check(NetID netid, PlatName pname, unsigned int login_time, LoginStr str, short server)
{
	return true;
}

void Platform::CreateLoginStr(LoginStr login_str, PlatName pname, unsigned int login_time, char cm, short server)
{
	memset(login_str, 0, sizeof(LoginStr));
}

