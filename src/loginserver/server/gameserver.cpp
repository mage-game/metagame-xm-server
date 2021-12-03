
#include "gameserver.h"

GameServer::GameServer() 
	: index(-1), port(-1), netid(-1)
{
	ip.clear();
	scene_list.clear();
}

GameServer::~GameServer()
{
	
}

