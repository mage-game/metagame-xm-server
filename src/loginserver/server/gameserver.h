/******************************************************************************************
FileName: gameserver.hpp
Author: dreamwang
Description: 保存游戏服务器相关信息 主要是场景信息 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/19/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __GAMESERVER_H__
#define __GAMESERVER_H__

#include <string>
#include <vector>

#include "irmimodule.h"
#include "inetworkmodule.h"

class GameServer
{
public:
	GameServer();
	~GameServer();

	int					index;
	std::string			ip;
	unsigned short		port;
	NetID				netid;
	std::vector<int>	scene_list;
};

#endif

