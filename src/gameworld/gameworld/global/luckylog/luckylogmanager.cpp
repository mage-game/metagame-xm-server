#include "luckylogmanager.hpp"
#include "luckylogconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "gameworld/protocal/msgother.h"
#include "obj/character/role.h"

LuckyLogManager::LuckyLogManager()
{
}

LuckyLogManager::~LuckyLogManager()
{
}

LuckyLogManager & LuckyLogManager::Instance()
{
	static LuckyLogManager manager;
	return manager;
}

void LuckyLogManager::AddLog(int activity_type, int uid, const GameName role_name, ItemID item_id, int item_num)
{
	if (LUCKY_LOG_CFG().CheckNeedLog(activity_type, item_id))
	{
		auto it_log_map = m_log_map.find(activity_type);
		if (m_log_map.end() != it_log_map)
		{
			if (it_log_map->second.size() >= LOG_NUM_MAX)
			{
				it_log_map->second.pop_front();
				it_log_map->second.emplace_back(uid, role_name, item_id, item_num);
			}
			else
			{
				it_log_map->second.emplace_back(uid, role_name, item_id, item_num);
			}
		}
		else
		{
			m_log_map[activity_type].emplace_back(uid, role_name, item_id, item_num);
		}
	}
}

LuckyLogManager::LogItemDeque * LuckyLogManager::GetLog(int activity_type)
{
	auto it_idx = m_log_map.find(activity_type);
	if (m_log_map.end() != it_idx)
	{
		return &m_log_map[activity_type];
	}

	return nullptr;
}

void LuckyLogManager::SendLog(Role *role, int activity_type)
{
	if (nullptr == role)
	{
		return;
	}

	LogItemDeque *log_deque = this->GetLog(activity_type);

	static Protocol::SCLuckyLogRet msg;
	msg.count = 0;
	msg.activity_type = activity_type;

	if (nullptr != log_deque)
	{
		for (auto &log_item : *log_deque)
		{
			if (msg.count >= LOG_NUM_MAX)
			{
				break;
			}
			msg.log_item[msg.count] = log_item;
			msg.count++;
		}
	}

	//SEND_TO_ROLE(role, msg);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&msg, sizeof(Protocol::SCLuckyLogRet));

}

void LuckyLogManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	// Çå¿ÕÊý¾Ý
	m_log_map.clear();
}