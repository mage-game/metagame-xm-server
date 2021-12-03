/******************************************************************************************
FileName: gameserver.hpp
Author: dreamwang
Description: ������Ϸ�����������Ϣ ��Ҫ�ǳ�����Ϣ 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/19/ 09:04:05     1.0           ����
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

