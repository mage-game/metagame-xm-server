#ifndef __LUCKY_LOG_HPP__
#define __LUCKY_LOG_HPP__

#include "servercommon/activitydef.hpp"
#include "gameworld/engineadapter.h"

static const int LOG_NUM_MAX = 15;

struct LuckyLogItem
{
	LuckyLogItem() : uid(0), item_id(0), item_num(0), timestamp(0)
	{}

	LuckyLogItem(int log_uid, const GameName log_name, ItemID log_item_id, int log_item_num) :
		uid(log_uid), item_id(log_item_id), item_num(log_item_num)
	{
		strcpy(role_name, log_name);
		timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	};

	int uid = 0;
	GameName role_name = { 0 };
	ItemID item_id = 0;
	short item_num = 0;
	unsigned int timestamp = 0;
};

class LuckyLogManager
{
	using LogItemDeque = std::deque<LuckyLogItem>;
	using LogItemMap = std::map<int, LogItemDeque>;

public:
	LuckyLogManager();
	~LuckyLogManager();

	static LuckyLogManager & Instance();

	void AddLog(int activity_type, int uid, const GameName role_name, ItemID item_id, int item_num);
	LogItemDeque * GetLog(int activity_type);
	void SendLog(Role *role, int activity_type);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

protected:

	LogItemMap m_log_map;			// 日志列表

};

#endif // __LUCKY_LOG_HPP__

