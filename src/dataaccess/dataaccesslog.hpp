#ifndef __DATAACCESS_LOG_HPP__
#define __DATAACCESS_LOG_HPP__

#include "ilogmodule.h"
#include "utility/logagent.h"

class ILogModule;
struct ItemParamDataStruct;

namespace dataaccesslog
{
	extern LogAgent g_log_role;
	extern LogAgent g_log_global;
	extern LogAgent g_log_pet;
	extern LogAgent g_log_favour;
	extern LogAgent g_log_familiar;
	extern LogAgent g_log_science;
	extern LogAgent g_log_roleskill;
	extern LogAgent g_log_achive;
	extern LogAgent g_log_blacklist;
	extern LogAgent g_log_business;
	extern LogAgent g_log_enemy;
	extern LogAgent g_log_friend;
	extern LogAgent g_log_hotkey;
	extern LogAgent g_log_taskrecord;
	extern LogAgent g_log_accountgold;
	extern LogAgent g_log_system;

	void LogInit(ILogModule *log, const std::string &logdir);
}

#endif // __DATAACCESS_LOG_HPP__

