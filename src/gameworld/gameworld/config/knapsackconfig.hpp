#ifndef __KNAPSACK_CONFIG_HPP__
#define __KNAPSACK_CONFIG_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

class KnapsackConfig : public ConfigBase
{
public:
	KnapsackConfig();
	~KnapsackConfig();

	bool Init(const std::string &path, std::string *err);

	int GetExtendBeginLevel() { return m_extend_begin_level; }
	int GetExtendOnlineTime(int grid_index);

private:
	int m_extend_begin_level;													// 在线时间开格子起始等级
	int m_extend_online_time_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];		// 开格子需要的在线时间
};

#endif // __KNAPSACK_CONFIG_HPP__

