#include "loginlog.h"

namespace loginlog
{	
	LogAgent g_log_login;
	LogAgent g_log_create;
	LogAgent g_log_destory;
	LogAgent g_log_global_user_state;
	LogAgent g_log_role_num;
	LogAgent g_log_cmd;
	LogAgent g_log_login_trace;
	LogAgent g_log_role_forbid;
	LogAgent g_log_quick;

	void LogInit(ILogModule *log, const std::string& logdir)
	{
		g_log_login.SetLogModule(log);
		g_log_login.SetCatagory("LOGIN", (logdir+"loginlog/login.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_create.SetLogModule(log);
		g_log_create.SetCatagory("CREATE_ROLE", (logdir+"loginlog/createrole.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_destory.SetLogModule(log);
		g_log_destory.SetCatagory("DESTORY_ROLE", (logdir+"loginlog/destoryrole.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_global_user_state.SetLogModule(log);
		g_log_global_user_state.SetCatagory("GLOBAL_USER", (logdir+"loginlog/userstate.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_role_num.SetLogModule(log);
		g_log_role_num.SetCatagory("RoleNum", (logdir+"loginlog/rolenum.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_cmd.SetLogModule(log);
		g_log_cmd.SetCatagory("Cmd", (logdir+"loginlog/cmd.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_login_trace.SetLogModule(log);
		g_log_login_trace.SetCatagory("LoginTrace", (logdir+"loginlog/logintrace.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_role_forbid.SetLogModule(log);
		g_log_role_forbid.SetCatagory("RoleForbid", (logdir+"loginlog/roleforbid.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_quick.SetLogModule(log);
		g_log_quick.SetCatagory("quick", (logdir+"loginlog/quick/quick.txt").c_str(), ILogModule::NFIS_HALF_HOUR);
	}
}
