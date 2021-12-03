#ifndef __LOGINLOG_H__
#define __LOGINLOG_H__

#include "ilogmodule.h"
#include "utility/logagent.h"

class ILogModule;

namespace loginlog
{
	extern LogAgent g_log_login;
	extern LogAgent g_log_create;
	extern LogAgent g_log_destory;
	extern LogAgent	g_log_global_user_state;
	extern LogAgent g_log_cmd;
	extern LogAgent g_log_login_trace;
	extern LogAgent g_log_role_forbid;
	extern LogAgent g_log_quick;

	void LogInit(ILogModule *log, const std::string& logdir);
}

#endif

