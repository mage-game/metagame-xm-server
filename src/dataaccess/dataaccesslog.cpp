#include "dataaccesslog.hpp"

#include "servercommon/servercommon.h"

namespace dataaccesslog
{
	LogAgent g_log_role;
	LogAgent g_log_global;
	LogAgent g_log_pet;
	LogAgent g_log_favour;
	LogAgent g_log_familiar;
	LogAgent g_log_science;
	LogAgent g_log_roleskill;
	LogAgent g_log_achive;
	LogAgent g_log_blacklist;
	LogAgent g_log_business;
	LogAgent g_log_enemy;
	LogAgent g_log_friend;
	LogAgent g_log_hotkey;
	LogAgent g_log_taskrecord;
	LogAgent g_log_accountgold;
	LogAgent g_log_system;

	void LogInit(ILogModule *log, const std::string &logdir)
	{
		g_log_role.SetLogModule(log);
		g_log_role.SetCatagory("Role", (logdir+"dblog/role_fatal.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_global.SetLogModule(log);
		g_log_global.SetCatagory("Global", (logdir+"dblog/global_fatal.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_pet.SetLogModule(log);
		g_log_pet.SetCatagory("Pet", (logdir+"dblog/pet/role_pet.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_favour.SetLogModule(log);
		g_log_favour.SetCatagory("Favour", (logdir+"dblog/favour/role_favour.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_familiar.SetLogModule(log);
		g_log_familiar.SetCatagory("Familiar", (logdir+"dblog/familiar/role_familiar.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_science.SetLogModule(log);
		g_log_science.SetCatagory("Science", (logdir+"dblog/science/role_science.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_roleskill.SetLogModule(log);
		g_log_roleskill.SetCatagory("Roleskill", (logdir+"dblog/rolesill/role_roleskill.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_achive.SetLogModule(log);
		g_log_achive.SetCatagory("Achive", (logdir+"dblog/achive/role_achive.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_blacklist.SetLogModule(log);
		g_log_blacklist.SetCatagory("Blacklist", (logdir+"dblog/balcklist/role_blacklist.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_business.SetLogModule(log);
		g_log_business.SetCatagory("Business", (logdir+"dblog/business/role_business.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_enemy.SetLogModule(log);
		g_log_enemy.SetCatagory("Enemy", (logdir+"dblog/enemy/role_enemy.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_friend.SetLogModule(log);
		g_log_friend.SetCatagory("Friend", (logdir+"dblog/friend/role_frient.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_hotkey.SetLogModule(log);
		g_log_hotkey.SetCatagory("Hotkey", (logdir+"dblog/hotkey/role_hotkey.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_taskrecord.SetLogModule(log);
		g_log_taskrecord.SetCatagory("Taskrecord", (logdir+"dblog/taskrecord/role_taskrecord.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_accountgold.SetLogModule(log);
		g_log_accountgold.SetCatagory("AccountGold", (logdir+"dblog/accountgold/accountgold.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_system.SetLogModule(log);
		g_log_system.SetCatagory("System", (logdir+"dblog/system/system.txt").c_str(), ILogModule::NFIS_DAY);

	}
}

