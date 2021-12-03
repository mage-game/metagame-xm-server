
#include <stdlib.h>
#include "common/syspath.h"

#include "gameserver/game.h"

#include "log/logmain.h"
#include "network/networkmain.h"
#include "config/configmain.h"

#include "gameserver/modulemain.h"
#include "gatewaymodule.h"

#include "servercommon/servercommon.h"
#include "servercommon/version.hpp"

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
	#ifdef __unix
	assert(CheckResource());
	#endif

	GameConfig config;
	config.loop_interval = 8;
	config.work_path = argv[0];
	SysPath::Up(&config.work_path);

	const char *config_file = "serverconfig.xml";   

	if (argc < 3) 
	{
		printf("please add gateway index from 1 to 3 \n");
		exit(0);
	}

	if(!strcmp(argv[1], "-v"))
	{
		printf("not version \n");
		exit(0);
	}
	config_file = argv[1];

	int gateway_index = atoi(argv[2]);
	if (gateway_index < 1 || gateway_index > 6)
	{
		printf("please add gateway index from 1 to 6 \n");
		exit(0);
	}

	Game game;

	// 静态链接模块
	gsclock::Register(&game);
	gsgame::Register(&game, &game);
	gstimer::Register(&game);

	game.RegisterModule(GATEWAY_MODULE, new GatewayModule(gateway_index));

	// 动态链接模块
	ConfigDLL::Register(&game, config_file, true);
	LogDLL::Register(&game);
	NetworkDLL::Register(&game);
	NetworkDLL::Register(&game, INTERNAL_NETWORK);

	game.Run(config);

	system("pause");

	return 0;
}

