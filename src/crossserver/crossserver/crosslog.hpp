#ifndef __CROSS_LOG_HPP__
#define __CROSS_LOG_HPP__

#include "ilogmodule.h"
#include "utility/logagent.h"

class ILogModule;

namespace crosslog
{
	extern LogAgent g_log_serious_error;	// 严重错误, 必须马上处理
	extern LogAgent g_log_debug;			// 调试日志

	extern LogAgent g_log_cross_activity;
	extern LogAgent g_log_cross_rank;

	extern LogAgent g_log_cross_rand_activity;

	void LogInit(ILogModule *log, const std::string &logdir);
}

#endif // __CROSS_LOG_HPP__

