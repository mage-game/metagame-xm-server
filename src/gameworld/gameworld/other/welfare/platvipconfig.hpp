#ifndef __PLAT_VIP_CONFIG__
#define __PLAT_VIP_CONFIG__

#include "servercommon/welfaredef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"

class PlatVipConfig : public ConfigBase
{
public :
	PlatVipConfig();
	~PlatVipConfig();

	bool Init(const std::string &path, std::string *err);

	const ItemConfigData * GetReward(int fetch_type, int plat_id, int vip_level, int vip_type, int *reward_type);

private:
	ItemConfigData m_reward_item_list[PLAT_WELFARE_MAX][PLAT_VIP_REWARD_MAX][2];
};

#endif // __PLAT_VIP_CONFIG__
