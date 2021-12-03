#pragma once

#include <functional>
#include <map>
#include <set>

#define EVENT_USING_FUNCTION 1

enum class EventType;

struct EventData
{
	const static int PERSONAL_DATA_LEN = 1024;

	EventType event_type;
	int param1;
	int param2;
	int param3;
	char param4[PERSONAL_DATA_LEN];
};

#ifdef EVENT_USING_FUNCTION

class EventRegister
{
public:
	EventRegister();
	~EventRegister();

	void Register(EventType event_type, std::function<void(const EventData&)> &&func);
	void UnRegister(EventType event_type);
	void Dispatch(const EventData & e_data);

private:
	std::map<EventType, std::function<void(const EventData&)>> m_func_list;
};

class EventManager
{
public:
	~EventManager()	{}

	static EventManager & GetInstance();

	bool Register(EventType event_type, EventRegister *reg);
	bool UnRegister(EventType event_type, EventRegister *reg);
	bool Dispatch(EventType event_type, int param1 = 0, int param2 = 0, int param3 = 0, char *param4 = nullptr);

private:
	EventManager() {}

	std::map<EventType, std::set<EventRegister*>> m_func_list_map;
};

#define EventMgr EventManager::GetInstance()
#define DEFINE_EVENT_REGISTER EventRegister m_event_register;
#define EVENT_MGR_REGISTER(EVENT_TYPE, CLASS_FUNC) m_event_register.Register(EVENT_TYPE, std::bind(&CLASS_FUNC, this, std::placeholders::_1))
#define EVENT_MGR_UNREGISTER(EVENT_TYPE) m_event_register.UnRegister(EVENT_TYPE)

#else

class EventManager;
class IEventHandler
{
public:
	IEventHandler();
	virtual ~IEventHandler();

	friend class EventManager;

protected:
	virtual void OnHandleEvent(const EventData & e_data) = 0;	// 处理事件接口

	bool ListeningEvent(bool value);

private:
	bool m_listening;
};

class EventManager
{
public:
	~EventManager() {}

	static EventManager & GetInstance();

	bool Register(IEventHandler *handler);
	bool UnRegister(IEventHandler *handler);
	bool Dispatch(EventType event_type, int param1 = 0, int param2 = 0, int param3 = 0, char *param4 = nullptr);

private:
	EventManager() {}

	std::set<IEventHandler *> m_func_list_map;
};


#define EventMgr EventManager::GetInstance()

#endif