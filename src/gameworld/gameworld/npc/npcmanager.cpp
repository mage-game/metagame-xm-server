
#include "npcmanager.h"
#include "config/sceneconfig.h"
#include "protocal/msgnpc.h"
#include "engineadapter.h"
#include "obj/character/role.h"


NPCManager::NPCManager()
{

}

NPCManager::~NPCManager()
{
	this->Release();
}

void NPCManager::Init(const ConfigSceneNpcList &npc_list)
{
	this->Release();

	for (ConfigSceneNpcList::const_iterator iter = npc_list.begin(); iter != npc_list.end(); ++iter)
	{
		NPC *npc = new NPC();
		npc->Init(*iter);
		m_npc_list[iter->id] = npc;
	}
}

void NPCManager::Release()
{
	for (NPCList::iterator i = m_npc_list.begin(); i != m_npc_list.end(); ++i)
	{
		delete i->second;
	}

	m_npc_list.clear();
}

NPC * NPCManager::GetNpc(UInt16 npc_id)
{
	NPCList::iterator iter = m_npc_list.find(npc_id);
	if (iter != m_npc_list.end())
	{
		return iter->second;
	}

	return 0;
}
