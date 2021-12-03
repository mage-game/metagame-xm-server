#include "event_manager.h"

#include <cstring>

EventManager & EventManager::GetInstance()
{
	static EventManager instance;
	return instance;
}

#ifdef EVENT_USING_FUNCTION

#include "event_type.h"

/*
* Demo
*
class EventManagerDemo
{
public:
	EventManagerDemo()
	{
		EVENT_MGR_REGISTER(EventType::EGG_BROKE, EventManagerDemo::OnTestMyEvent1);
		EVENT_MGR_REGISTER(EventType::MONSTER_DEAD, EventManagerDemo::OnTestMyEvent2);

		m_event_register.Register(EventType::EGG_BROKE, std::bind(&EventManagerDemo::OnTestMyEvent1, this, std::placeholders::_1));

		m_event_register.Register(EventType::MONSTER_DEAD, [this](const EventData & data)
		{
			
		});

		EVENT_MGR_UNREGISTER(EventType::MONSTER_DEAD);
	}
	
	~EventManagerDemo()	{}

	void OnTestMyEvent1(const EventData&) {}
	void OnTestMyEvent2(const EventData&) {}

private:
	DEFINE_EVENT_REGISTER;
};

int main()
{
	EventManagerDemo xx;
	EventMgr.Dispatch(EventType::EGG_BROKE, 22, 33, 44, "LoveU");
	EventMgr.Dispatch(EventType::MONSTER_DEAD);
	return 0;
}
//*/

EventRegister::EventRegister()
{
	
}

EventRegister::~EventRegister()
{
	for (auto & func_it : m_func_list)
	{
		EventMgr.UnRegister(func_it.first, this);
	}
}

void EventRegister::Register(EventType event_type, std::function<void(const EventData&)> &&func)
{
	m_func_list[event_type] = func;
	EventMgr.Register(event_type, this);
}

void EventRegister::UnRegister(EventType event_type)
{
	m_func_list.erase(event_type);
	EventMgr.UnRegister(event_type, this);
}

void EventRegister::Dispatch(const EventData & e_data)
{
	auto func_it = m_func_list.find(e_data.event_type);
	if (func_it != m_func_list.end())
	{
		func_it->second(e_data);
	}
}

bool EventManager::Register(EventType event_type, EventRegister *reg)
{
	if (m_func_list_map[event_type].find(reg) != m_func_list_map[event_type].end())
	{
		return false;
	}

	m_func_list_map[event_type].insert(reg);

	return true;
}

bool EventManager::UnRegister(EventType event_type, EventRegister *reg)
{
	m_func_list_map[event_type].erase(reg);
	if (m_func_list_map[event_type].empty())
	{
		m_func_list_map.erase(event_type);
	}

	return true;
}

bool EventManager::Dispatch(EventType event_type, int param1, int param2, int param3, char *param4)
{	
	static EventData e_data;
	e_data.event_type = event_type;
	e_data.param1 = param1;
	e_data.param2 = param2;
	e_data.param3 = param3;
	if (nullptr != param4)
	{
		memcpy(e_data.param4, param4, sizeof(e_data.param4));
	}

	if (!m_func_list_map[event_type].empty())
	{
		for (auto func_it : m_func_list_map[event_type])
		{
			func_it->Dispatch(e_data);
		}
	}

	return true;
}

#else
/*
* Demo
*
class EventManagerDemo : public IEventHandler
{
public:
	EventManagerDemo() { this->ListeningEvent(true); }

protected:
	virtual void OnHandleEvent(const EventData&) {}
};

int main()
{
	EventManagerDemo xx;
	EventMgr.Dispatch(EventType::EGG_BROKE, 22, 33, 44, "LoveU");
	EventMgr.Dispatch(EventType::MONSTER_DEAD);
	return 0;
}
*/

IEventHandler::IEventHandler() : m_listening(false)
{
}

IEventHandler::~IEventHandler()
{
	this->ListeningEvent(false);
}

bool IEventHandler::ListeningEvent(bool value)
{
	if (m_listening == value) return true;

	m_listening = value;
	if (m_listening)
	{
		return EventMgr.Register(this);
	}
	else
	{
		return EventMgr.UnRegister(this);
	}
}

bool EventManager::Register(IEventHandler *handler)
{
	if (m_func_list_map.find(handler) != m_func_list_map.end())
	{
		return false;
	}

	m_func_list_map.insert(handler);

	return true;
}

bool EventManager::UnRegister(IEventHandler *handler)
{
	if (m_func_list_map.find(handler) == m_func_list_map.end())
	{
		return false;
	}

	m_func_list_map.erase(handler);

	return true;
}

bool EventManager::Dispatch(EventType event_type, int param1, int param2, int param3, char *param4)
{
	static EventData e_data;
	e_data.event_type = event_type;
	e_data.param1 = param1;
	e_data.param2 = param2;
	e_data.param3 = param3;
	if (nullptr != param4)
	{
		memcpy(e_data.param4, param4, sizeof(e_data.param4));
	}

	if (!m_func_list_map.empty())
	{
		for (auto func_it : m_func_list_map)
		{
			func_it->OnHandleEvent(e_data);
		}
	}

	return true;
}


#endif