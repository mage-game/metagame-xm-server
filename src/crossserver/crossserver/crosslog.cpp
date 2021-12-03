#include "crosslog.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"

namespace crosslog
{
	LogAgent g_log_serious_error;
	LogAgent g_log_debug;

	LogAgent g_log_cross_activity;
	LogAgent g_log_cross_rank;

	LogAgent g_log_cross_rand_activity;

	void LogInit(ILogModule *log, const std::string &logdir)
	{
		g_log_serious_error.SetLogModule(log);
		g_log_serious_error.SetCatagory("SeriousError",(logdir+"crosslog/seriouserror/seriouserror.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_debug.SetLogModule(log);
		g_log_debug.SetCatagory("Debug", (logdir+"crosslog/debug/debug.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_cross_activity.SetLogModule(log);
		g_log_cross_activity.SetCatagory("CrossActivity", (logdir+"crosslog/cross_activity/cross_activity.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_cross_rank.SetLogModule(log);
		g_log_cross_rank.SetCatagory("CrossRank", (logdir+"crosslog/cross_rank/cross_rank.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_cross_rand_activity.SetLogModule(log);
		g_log_cross_rand_activity.SetCatagory("CrossRandActivity", (logdir + "crosslog/cross_randactivity/cross_randactivity.txt").c_str(), ILogModule::NFIS_DAY);
	}
}

