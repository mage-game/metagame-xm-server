
#ifndef NPCMANAGER_H
#define NPCMANAGER_H

#include <map>
#include <vector>
#include "servercommon/basedef.h"

#include "npc.h"

struct ConfigSceneNpc;
typedef std::vector<ConfigSceneNpc> ConfigSceneNpcList;

class NPCManager
{
public:
	NPCManager();
	~NPCManager();

	void Init(const ConfigSceneNpcList &npc_list);
	void Release();

	NPC * GetNpc(UInt16 npc_id);

protected:

	typedef std::map<UInt16, NPC*> NPCList;
	NPCList	m_npc_list;
	
};


#endif

