
#ifndef MODULE_H
#define MODULE_H

class IInterfaceMgr;
class Game;

namespace gsclock
{
	void Register(IInterfaceMgr* interface_mgr);
}

namespace gsgame
{
	void Register(IInterfaceMgr* interface_mgr, Game *game);
}

namespace gsstdcommand
{
	void Register(IInterfaceMgr* interface_mgr);
}

namespace gstimer
{
	void Register(IInterfaceMgr* interface_mgr);
}

namespace gsfilewrite
{
	void Register(IInterfaceMgr *interface_mgr);
}

#endif

