#include "transportlist.h"
#include "config/sceneconfig.h"

TransportList::TransportList()
{

}

TransportList::~TransportList()
{

}

void TransportList::Init(const ConfigSceneDoorList &door_list)
{
	m_transport_list.clear();

	for (ConfigSceneDoorList::const_iterator iter = door_list.begin(); iter != door_list.end(); ++iter)
	{
		this->Add(iter->id, iter->pos, iter->target_scene_id, iter->target_door_id);
	}
}

void TransportList::Add(int tid, const Posi &location, int target_scene, int target_door_id)
{
	m_transport_list[tid] = Transport(tid, location, target_scene, target_door_id);
}

Transport * TransportList::GetTransport(int tid)
{
	TransportMapIt it = m_transport_list.find(tid);
	if (it != m_transport_list.end())
	{
		return &it->second;
	}

	return NULL;
}

Transport * TransportList::GetOneTransport()
{
	TransportMapIt it = m_transport_list.begin();
	if (it != m_transport_list.end())
	{
		return &it->second;
	}

	return NULL;
}

