#include <stdlib.h>
#include "common/syspath.h"

#include "gameserver/game.h"
#include "log/logmain.h"
#include "network/networkmain.h"
#include "config/configmain.h"
#include "gameserver/modulemain.h"
#include "rmi/rmimain.h"

#include "dataaccess.h"
#include "servercommon/servercommon.h"
#include "servercommon/version.hpp"

#include "rolecachemanager.hpp"

#ifdef _WIN32
#include "servercommon/exceptdump.h"
#endif

#ifdef __unix
#include <assert.h>
#include <sys/resource.h>

#define MAX_FILE_NO		(1024 * 32)

bool CheckResource()
{
	struct rlimit rl;
	if (getrlimit(RLIMIT_NOFILE, &rl) == -1)
	{
		return false;
	}
	rl.rlim_cur = (rl.rlim_cur > MAX_FILE_NO ? rl.rlim_cur : MAX_FILE_NO);
	rl.rlim_max = (rl.rlim_max > MAX_FILE_NO ? rl.rlim_max : MAX_FILE_NO);
	if (setrlimit(RLIMIT_NOFILE, &rl) == -1)
	{
		return false;
	}
	return true;
}
#endif

int main(int argc, char *argv[])
{
	#ifdef _WIN32
	start_except_catch();
	#endif

	#ifdef __unix
	assert(CheckResource());
	#endif

	GameConfig config;
	config.loop_interval = 8;
	config.work_path = argv[0];
	SysPath::Up(&config.work_path);

	const char *config_file = "serverconfig.xml";

	if (argc > 1)
	{
		if(!strcmp(argv[1], "-v"))
		{
			char version[128] = {0};
#ifdef _DEBUG
			SNPRINTF(&version[0], sizeof(version), "EXEdataaccess %s Ver:%s in %s %s\n", "debug", GetCurVersion(DATAACCESS_SVR_VERSION), __DATE__, __TIME__);
#else
			SNPRINTF(&version[0], sizeof(version), "EXEdataaccess %s Ver:%s in %s %s\n", "release", GetCurVersion(DATAACCESS_SVR_VERSION), __DATE__, __TIME__);
#endif
			printf("%s", version);

			exit(0);
		}

		config_file = argv[1];
	}

	int is_remove_role_cache = 0, remove_cache_time = 10 * 60;
	if (argc > 3)
	{
		is_remove_role_cache = atoi(argv[2]);
		remove_cache_time = atoi(argv[3]);
		RoleCacheManager::Instance().SetRemoveRoleCache(1 == is_remove_role_cache, remove_cache_time);
	}

	Game game;

	// 静态链接模块
	gsgame::Register(&game, &game);
	gsclock::Register(&game);
	// gsstdcommand::Register(&game);
	gstimer::Register(&game);

	game.RegisterModule(DATA_ACCESS_SERVER, new DataAccess());

	// 动态链接模块
	ConfigDLL::Register(&game, config_file, true);
	
	LogDLL::Register(&game);
	NetworkDLL::Register(&game, INTERNAL_NETWORK);
	RMIDLL::Register(&game, INTERNAL_NETWORK);

	game.Run(config);

	system("pause");

	#ifdef _WIN32
	stop_except_catch();
	#endif
	return 0;
}

