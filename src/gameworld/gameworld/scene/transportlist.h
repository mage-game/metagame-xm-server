#ifndef __TRANSPORTLIST_H__
#define __TRANSPORTLIST_H__

#include <map>
#include <vector>

#include "gamedef.h"

struct ConfigSceneDoor;
typedef std::vector<ConfigSceneDoor> ConfigSceneDoorList;

struct Transport
{
	Transport() : tid(0), target_scene(0) {}
	Transport(int i, const Posi &l, int scene, int _target_door_id)
		: tid(i), location(l), target_scene(scene), target_door_id(_target_door_id) {}
	int			tid;
	Posi		location;
	int			target_scene;
	int			target_door_id;
};

class TransportList
{
public:
	TransportList();
	~TransportList();

	void Init(const ConfigSceneDoorList &door_list);
	void Add(int tid, const Posi &location, int target_scene, int target_door_id);

	Transport * GetTransport(int tid);
	Transport * GetOneTransport();

protected:
	typedef std::map<int, Transport> TransportMap;
	typedef std::map<int, Transport>::iterator TransportMapIt;

	TransportMap m_transport_list;
};

#endif
