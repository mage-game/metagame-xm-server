#ifndef __RANKCONFIGCHECK_HPP__
#define __RANKCONFIGCHECK_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

class RankConfigCheck
{
public:
	static const int MAX_HONOUR_RANK_REWARD_COUNT = 50;

	RankConfigCheck();
	~RankConfigCheck();

	bool Init(const std::string &configname, std::string *err);

private:
	int m_honour_rank_reward_count;
	ItemConfigData m_honour_rank_reward_list[MAX_HONOUR_RANK_REWARD_COUNT];
};

#endif // __RANKCONFIGCHECK_HPP__

