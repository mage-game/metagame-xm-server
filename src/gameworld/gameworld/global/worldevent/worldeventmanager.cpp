#include "worldeventmanager.hpp"
#include "global/worldevent/worldeventimpl/findnpc.hpp"
#include "global/worldevent/worldeventimpl/jiqingpk.hpp"
#include "global/worldevent/worldeventimpl/killrandomboss.hpp"
#include "global/worldevent/worldeventimpl/quanmintafang.hpp"
#include "global/worldevent/worldeventimpl/shenmozhixi.hpp"
#include "global/worldevent/worldeventimpl/tiannvsanhua.hpp"
#include "global/worldevent/worldeventimpl/xukongzhimen.hpp"
#include <time.h>
#include "engineadapter.h"
#include "config/logicconfigmanager.hpp"
#include "global/worldevent/worldeventconfig.hpp"
#include "protocal/msgworldevent.h"
#include "obj/character/role.h"
#include "world.h"

WorldEventManager & WorldEventManager::Instance()
{
	static WorldEventManager _instance;
	return _instance;
}

WorldEventManager::WorldEventManager() 
	: m_curr_event_type(0), m_logic_state(WORLD_EVENT_LOGIC_TYPE_INVALID), m_next_logic_timestamp(0)
{
	memset(m_world_event_list, 0, sizeof(m_world_event_list));

	UNSTD_STATIC_CHECK(8 == WORLD_EVENT_TYPE_MAX);

// 	m_world_event_list[WORLD_EVENT_TYPE_FIND_NPC] = new WorldEventFindNpc(this);
// 	m_world_event_list[WORLD_EVENT_TYPE_KILL_RANDOM_BOSS] = new WorldEventKillRandBoss(this);
// 	m_world_event_list[WORLD_EVENT_TYPE_SHENMOZHIXI] = new WorldEventShenmozhiji(this);
// 	m_world_event_list[WORLD_EVENT_TYPE_XUKONGZHIMEN] = new WorldEventXukongzhimen(this);
// 	m_world_event_list[WORLD_EVENT_TYPE_QUANMINTAFANG] = new WorldEventQuanmintafang(this);
// 	m_world_event_list[WORLD_EVENT_TYPE_JIQINGPK] = new WorldEventJiqingPk(this);
// 	m_world_event_list[WORLD_EVENT_TYPE_TIANNVSANHUA] = new WorldEventTiannvshanhua(this);
}

WorldEvent * WorldEventManager::GetCurrWorldEvent()
{
	if (m_curr_event_type <= WORLD_EVENT_TYPE_INVALID || m_curr_event_type >= WORLD_EVENT_TYPE_MAX)
	{
		return NULL;
	}

	return m_world_event_list[m_curr_event_type];
}

WorldEventManager::~WorldEventManager()
{

}

bool WorldEventManager::IsEvFinish(int ev_type, int score)
{
// 	const WorldEventTypeConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetWorldEventCfg().GetEventTypeCfg(ev_type);
// 	if (NULL != cfg_item && score >= cfg_item->complete_event_score)
// 	{
// 		return true;
// 	}

	return false;
}

time_t WorldEventManager::CalcNextLogicTime(int ev_type, int curr_logic_type, time_t curr_logic_timestamp)
{	
	time_t zero_timestamp = GetZeroTime(EngineAdapter::Instance().Time());
	time_t invalid_timestamp = zero_timestamp + SECOND_PER_DAY + ROLE_RESET_MODULE_DATA_TM_HOUR * SECOND_PER_HOUR + 1;

// 	const WorldEventLogicOpenConfig *open_cfg = LOGIC_CONFIG->GetWorldEventCfg().GetLogicOpenCfg(ev_type);
// 	if (NULL == open_cfg) 
// 	{
// 		return invalid_timestamp;
// 	}
// 
// 	if (curr_logic_type < WORLD_EVENT_LOGIC_TYPE_INVALID || curr_logic_type >= WORLD_EVENT_LOGIC_TYPE_MAX)
// 	{
// 		return invalid_timestamp;
// 	}
// 
// 	if (WORLD_EVENT_LOGIC_TYPE_INVALID == curr_logic_type)
// 	{
// 		curr_logic_timestamp = EngineAdapter::Instance().Time();
// 	}
// 	if (curr_logic_timestamp <= 0)
// 	{
// 		return invalid_timestamp;
// 	}
// 
// 	struct tm *curr_logic_tm = ::localtime(&curr_logic_timestamp);
// 	if (NULL == curr_logic_tm)
// 	{
// 		return invalid_timestamp;
// 	}
// 
// 	for (int i = 0; i < open_cfg->cfg_count; i++)
// 	{
// 		const WorldEventLogicOpenConfig::LogicStateTime &curr_state_cfg = open_cfg->cfg_list[i].logic_state_time[curr_logic_type] ;
// 
// 		if ((curr_state_cfg.hour == curr_logic_tm->tm_hour && curr_state_cfg.minute >= curr_logic_tm->tm_min) ||
// 			curr_state_cfg.hour > curr_logic_tm->tm_hour)
// 		{
// 			int next_open_cfg_index = i;
// 			int next_logic_state = curr_logic_type + 1;
// 
// 			if (curr_logic_type >= WORLD_EVENT_LOGIC_TYPE_MAX)
// 			{
// 				next_logic_state = WORLD_EVENT_LOGIC_TYPE_INVALID;
// 				next_open_cfg_index = (next_open_cfg_index + 1);
// 
// 				if (next_open_cfg_index >= open_cfg->cfg_count)
// 				{
// 					return invalid_timestamp;
// 				}
// 			}
// 
// 			const WorldEventLogicOpenConfig::LogicStateTime &next_state_cfg = open_cfg->cfg_list[next_open_cfg_index].logic_state_time[next_logic_state] ;
// 
// 			time_t next_time = zero_timestamp + next_state_cfg.hour * SECOND_PER_HOUR + next_state_cfg.minute * SECOND_PER_MIN;
// 			return next_time;
// 		}
// 	}

	return invalid_timestamp;
}

void WorldEventManager::UpdateEventLogic(unsigned long interval, time_t now_second)
{
// 	WorldEvent *world_event = this->GetCurrWorldEvent(); 
// 	if (NULL != world_event)
// 	{
// 		if (0 == m_next_logic_timestamp)
// 		{
// 			m_logic_state = WORLD_EVENT_LOGIC_TYPE_INVALID;
// 			m_next_logic_timestamp = this->CalcNextLogicTime(world_event->GetEvType(), m_logic_state, m_next_logic_timestamp);
// 		}
// 		else if (now_second >= m_next_logic_timestamp)
// 		{
// 			if (WORLD_EVENT_LOGIC_TYPE_INVALID == m_logic_state)
// 			{
// 				m_logic_state = WORLD_EVENT_LOGIC_TYPE_STAND;
// 				world_event->OnLogicStand();
// 			}
// 			else if (WORLD_EVENT_LOGIC_TYPE_STAND == m_logic_state)
// 			{
// 				m_logic_state = WORLD_EVENT_LOGIC_TYPE_START;
// 				world_event->OnLogicStart();
// 			}
// 			else if (WORLD_EVENT_LOGIC_TYPE_START == m_logic_state)
// 			{
// 				m_logic_state = WORLD_EVENT_LOGIC_TYPE_STOP;
// 				world_event->OnLogicStop();
// 			}
// 			else if (WORLD_EVENT_LOGIC_TYPE_STOP == m_logic_state)
// 			{
// 				m_logic_state = WORLD_EVENT_LOGIC_TYPE_INVALID;
// 			}
// 
// 			m_next_logic_timestamp = this->CalcNextLogicTime(world_event->GetEvType(), m_logic_state, m_next_logic_timestamp);
// 			this->SendCommonInfo(NULL);
// 		}
// 	}
}

void WorldEventManager::Init(const WorldEventParam &ev_param)
{
// 	for (int i = WORLD_EVENT_TYPE_INVALID + 1; i < WORLD_EVENT_TYPE_MAX; ++ i)
// 	{
// 		if (NULL != m_world_event_list[i])
// 		{
// 			m_world_event_list[i]->Init(ev_param);
// 		}
// 	}
// 	
// 	m_curr_event_type = ev_param.curr_event_type;
}

void WorldEventManager::GetInitParam(WorldEventParam *ev_param)
{
// 	for (int i = WORLD_EVENT_TYPE_INVALID + 1; i < WORLD_EVENT_TYPE_MAX; ++ i)
// 	{
// 		if (NULL != m_world_event_list[i])
// 		{
// 			m_world_event_list[i]->GetInitParam(ev_param);
// 		}
// 	}

	ev_param->curr_event_type = m_curr_event_type;
}

void WorldEventManager::Update(unsigned long interval, time_t now_second)
{
// 	if (WORLD_EVENT_TYPE_INVALID == m_curr_event_type) // 初始化第一个世界事件
// 	{
// 		m_curr_event_type = WORLD_EVENT_TYPE_INVALID + 1;
// 
// 		WorldEvent *world_event = this->GetCurrWorldEvent();
// 		if (NULL != world_event)
// 		{
// 			world_event->OnEventStart();
// 		}
// 	}
// 
// 	WorldEvent *world_event = this->GetCurrWorldEvent();
// 	if (NULL != world_event)
// 	{
// 		this->UpdateEventLogic(interval, now_second);
// 		world_event->Update(interval, now_second);
// 	}
}

void WorldEventManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
// 	WorldEvent *world_event = this->GetCurrWorldEvent();
// 	if (NULL != world_event)
// 	{
// 		if (this->IsEvFinish(world_event->GetEvType(), world_event->GetScore()))	// 如果已经结束
// 		{
// 			world_event->OnEventStop();												// 触发结束事件
// 
// 			++ m_curr_event_type;
// 
// 			WorldEvent * next_world_event = this->GetCurrWorldEvent();
// 			if (NULL != next_world_event)
// 			{
// 				next_world_event->OnEventStart();									// 触发新的事件开始
// 			}
// 		}
// 
// 		m_logic_state = WORLD_EVENT_LOGIC_TYPE_INVALID;								// 每日重置的时候，活动阶段时间都必须重新算
// 		m_next_logic_timestamp = 0;
// 	}
}

void WorldEventManager::OnUserLogin(Role *role)
{
	//this->SendCommonInfo(role);
}

void WorldEventManager::OnMonsterDisapper(Monster *monster)
{
	if (NULL == monster)
	{
		return;
	}
// 
// 	WorldEvent *world_event = this->GetCurrWorldEvent();
// 	if (NULL != world_event)
// 	{
// 		world_event->OnMonsterDisapper(monster);
// 	}
}

void WorldEventManager::OnKillMonster(Role *role, Monster *monster)
{
// 	if (NULL == role || NULL == monster)
// 	{
// 		return;
// 	}
// 
// 	WorldEvent *world_event = this->GetCurrWorldEvent();
// 	if (NULL != world_event)
// 	{
// 		world_event->OnKillMonster(role, monster);
// 	}
}

void WorldEventManager::OnHurtMonster(Role *role, Monster *monster, long long hurt)
{
// 	if (NULL == role || NULL == monster || hurt <= 0)
// 	{
// 		return;
// 	}
// 
// 	WorldEvent *world_event = this->GetCurrWorldEvent();
// 	if (NULL != world_event)
// 	{
// 		world_event->OnHurtMonster(role, monster, hurt);
// 	}
}

void WorldEventManager::OnHurtRoleShadow(Role *role, RoleShadow *role_shadow, long long hurt)
{
// 	if (NULL == role || NULL == role_shadow || hurt <= 0)
// 	{
// 		return;
// 	}
// 
// 	WorldEvent *world_event = this->GetCurrWorldEvent();
// 	if (NULL != world_event)
// 	{
// 		world_event->OnHurtRoleShadow(role, role_shadow, hurt);
// 	}
}

void WorldEventManager::OnTouchEventObj(Role *role, int scene_id, ObjID obj_id)
{
// 	WorldEvent *world_event = this->GetCurrWorldEvent();
// 	if (NULL != world_event)
// 	{
// 		world_event->OnTouchEventObj(role, scene_id, obj_id);
// 	}
}

void WorldEventManager::OnSetSpecialOtherAppearance(Role *role, int special_appearacne_type)
{
// 	if (NULL == role)
// 	{
// 		return;
// 	}
// 
// 	WorldEvent *world_event = this->GetCurrWorldEvent();
// 	if (NULL != world_event)
// 	{
// 		world_event->OnSetOtherSpecialAppearance(role, special_appearacne_type);
// 	}
}

void WorldEventManager::SendCommonInfo(Role *role)
{
// 	Protocol::SCWorldEventCommonInfo weci;
// 	weci.curr_event_type = m_curr_event_type;
// 	weci.logic_state = m_logic_state;
// 	weci.next_logic_state_timestamp = static_cast<unsigned int>(m_next_logic_timestamp);
// 	weci.event_score = 0;
// 
// 	WorldEvent *world_event = this->GetCurrWorldEvent();
// 	if (NULL != world_event)
// 	{
// 		weci.event_score = world_event->GetScore();
// 	}
// 
// 	if (NULL != role)
// 	{
// 		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&weci, sizeof(weci));
// 	}
// 	else
// 	{
// 		World::GetInstWorld()->SendToAllGateway((const char *)&weci, sizeof(weci));
// 	}
}

void WorldEventManager::ForceChangeLogicState(int event_type, int from_state, int to_state)
{
// 	if (event_type <= WORLD_EVENT_LOGIC_TYPE_INVALID || event_type >= WORLD_EVENT_TYPE_MAX) return;
// 	if (m_curr_event_type != event_type || from_state != m_logic_state || ((from_state + 1) % WORLD_EVENT_LOGIC_TYPE_MAX) != to_state) return;
// 
// 	m_next_logic_timestamp = EngineAdapter::Instance().Time();
}

void WorldEventManager::GmNextEventType(Role *role)
{
// 	WorldEvent *world_event = this->GetCurrWorldEvent();
// 	if (NULL != world_event)
// 	{
// 		world_event->OnEventStop();				// 触发结束事件
// 	}
// 
// 	++ m_curr_event_type;
// 	if (m_curr_event_type > WORLD_EVENT_TYPE_MAX)
// 	{
// 		m_curr_event_type = WORLD_EVENT_TYPE_INVALID + 1;
// 	}
// 
// 	m_logic_state = WORLD_EVENT_LOGIC_TYPE_INVALID;
// 	m_next_logic_timestamp = 0;
// 
// 	WorldEvent * next_world_event = this->GetCurrWorldEvent();
// 	if (NULL != next_world_event)
// 	{
// 		next_world_event->OnEventStart();		// 触发新的事件开始
// 	}
}

void WorldEventManager::GmNextLogicState(Role *role)
{
// 	m_next_logic_timestamp = EngineAdapter::Instance().Time();
// 	this->SendCommonInfo(role);
}

void WorldEventManager::AddScore(int event_type, int add_score)
{
// 	if (event_type > WORLD_EVENT_TYPE_INVALID && event_type < WORLD_EVENT_TYPE_MAX && add_score > 0)
// 	{
// 		if (NULL != m_world_event_list[event_type])
// 		{
// 			m_world_event_list[event_type]->AddScore(add_score);
// 		}
// 	}
}

void WorldEventManager::GmAddScore(Role *role, int add_score)
{
// 	WorldEvent *world_event = this->GetCurrWorldEvent();
// 	if (NULL != world_event)
// 	{
// 		world_event->AddScore(add_score);
// 		this->SendCommonInfo(role);
// 	}
}

int WorldEventManager::GetCurrEventScore()
{
// 	WorldEvent * world_event = this->GetCurrWorldEvent();
// 	if (NULL != world_event)
// 	{
// 		return world_event->GetScore();
// 	}

	return 0;
}

void WorldEventManager::GetNpcLocateInfo(Role *role, int npc_id)
{
// 	if (NULL == role)
// 	{
// 		return;
// 	}
// 
// 	if (NULL != m_world_event_list[WORLD_EVENT_TYPE_FIND_NPC])
// 	{
// 		if (WORLD_EVENT_TYPE_FIND_NPC == m_world_event_list[WORLD_EVENT_TYPE_FIND_NPC]->GetEvType())
// 		{
// 			WorldEventFindNpc *find_npc = (WorldEventFindNpc *)m_world_event_list[WORLD_EVENT_TYPE_FIND_NPC];
// 
// 			find_npc->GetNpcLocateInfo(role, npc_id);
// 		}
// 	}
}

int WorldEventManager::RandSpecialAppearanceParam()
{
 	int monster_appearance = LOGIC_CONFIG->GetWorldEventCfg().RandSpecialMonsterAppearance();
 
 	return monster_appearance;
}





