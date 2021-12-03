#ifndef __SEVEN_DAY_LOGIN_CONFIG__
#define __SEVEN_DAY_LOGIN_CONFIG__

#include "servercommon/welfaredef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

class SevenDayLoginConfig : public ConfigBase
{
public :
	SevenDayLoginConfig();
	~SevenDayLoginConfig();

	bool Init(const std::string &path, std::string *err);

	int GetMaxRewardDay() { return m_max_reward_day; }
	const ItemConfigData * getRewardItem(int day);

private:
	int m_max_reward_day;
	ItemConfigData m_reward_item_list[SEVEN_DAY_LOGIN_MAX_REWARD_DAY + 1];
};

#endif
