
#pragma once

#ifndef GAME_H
#define GAME_H

#include "iinterfacemgr.h"
#include "imodule.h"
#include <list>
#include <map>
#include "common/estring.h"

struct GameConfig
{
	GameConfig()
		:loop_interval(0)
	{

	}
	unsigned long loop_interval;
	estring work_path;
};


class Game : public IInterfaceMgr
{
	enum GameState
	{
		GS_STOP = 0,
		GS_RUNNING,
		GS_STOPPING,
	};

	typedef std::map<std::string, IModule*> ModuleMap;

	typedef std::list<IModule*> ModuleList;
	typedef int (IModule::*InterfaceFunc)();
public:
	Game();
	~Game();

	void Run(const GameConfig& config);
	void Stop();

	bool RegisterModule(const char* name, IModule* module);
	IModule* QueryModule(const char* name);

	void GetWorkPath(estring* path)const;
protected:
	void Loop(const ModuleList& module_list, InterfaceFunc func, ModuleList *succeed_list, bool igrone_exit, int succeed_state, int pending_state);

	bool m_exit;

	ModuleList m_module_list;
	ModuleList m_inited_list;
	ModuleList m_started_list;

	ModuleMap m_module_map;

	GameConfig m_game_config;
private:
	Game(const Game&);
	Game& operator=(const Game&);
};

#endif
