#ifndef __CROSS_RANK_WEEK_REWARD_CONFIG__
#define __CROSS_RANK_WEEK_REWARD_CONFIG__

#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/rankdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct CrossRankWeekReward
{
	CrossRankWeekReward() : rank_type(0), rank(0) {}

	bool IsValid() { return 0 != reward_item.item_id; }

	int rank_type;
	int rank;
	ItemConfigData reward_item;
};

class CrossRankWeekRewardConfig : public ConfigBase
{
public:

	CrossRankWeekRewardConfig();
	~CrossRankWeekRewardConfig();

	bool Init(const std::string &path, std::string *err);

	const CrossRankWeekReward * GetReward(int rank_type, int rank);

private:
	int InitRewardCfg(PugiXmlNode RootElement);

	CrossRankWeekReward m_reward_list[CROSS_PERSON_RANK_TYPE_MAX][CROSS_RANK_WEEK_REWARD_MAX_ROLE_COUNT];
};

#endif // __TOUZIJIHUA_CONFIG_HPP__

