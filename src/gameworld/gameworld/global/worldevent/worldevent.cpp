#include "worldevent.hpp"
#include "worldeventmanager.hpp"

WorldEvent::WorldEvent(WorldEventManager *world_event_manager, int event_type) : 
	m_world_event_manger(world_event_manager), m_event_type(event_type), m_score(0)
{

}

WorldEvent::~WorldEvent()
{

}

void WorldEvent::OnEventStop()
{
	this->OnLogicStop();
}

void WorldEvent::OnLogicStand()
{
	
}

void WorldEvent::AddScore(int add_score)
{
// 	long long tmp_score = m_score + add_score;
// 
// 	if (tmp_score < 0) tmp_score = 0;
// 	if (tmp_score > MAX_WORLD_EVENT_SCORE) tmp_score = MAX_WORLD_EVENT_SCORE;
// 
// 	m_score = static_cast<int>(tmp_score);
}

void WorldEvent::ForceEndLogicState()
{
// 	if (NULL != m_world_event_manger)
// 	{
// 		m_world_event_manger->ForceChangeLogicState(m_event_type, WORLD_EVENT_LOGIC_TYPE_START, WORLD_EVENT_LOGIC_TYPE_STOP);
// 	}
}





