#ifndef __FRIEND_EXP_BOTTLE_CONFIG_HPP__
#define __FRIEND_EXP_BOTTLE_CONFIG_HPP__

#include "gamedef.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include <vector>

class Role;

struct FriendExpBottleOtherCfg
{
	FriendExpBottleOtherCfg() : active_level(0), per_minute_exp(0), friend_extra_add_exp(0), exp_limit(0), broadcast_cd(0), auto_add_friend_num_time(0)
	{}

	int active_level;
	int per_minute_exp;
	int friend_extra_add_exp;
	unsigned long long exp_limit;
	unsigned int broadcast_cd;
	unsigned int get_exp_interval;
	unsigned int auto_add_friend_num_time;
};

struct ExpBottleLimitCfg
{
	ExpBottleLimitCfg() : min_friend_max_count(0), get_exp_times(0)
	{}

	int min_friend_max_count;
	int get_exp_times;
	int fetch_level_limit;
	unsigned long long exp_limit;
};

class FriendExpBottleConfig : public ConfigBase
{
public:
	FriendExpBottleConfig();
	~FriendExpBottleConfig();

	bool Init(const std::string &configname, std::string *err);

	const FriendExpBottleOtherCfg * GetFriendExpBottleOtherCfg() { return &m_other_cfg; }
	int GetExpBottleLimit(int friend_num);
	ExpBottleLimitCfg * GetExpBottleLimitCfg(int get_exp_count);

private:
	int InitOtherConfig(PugiXmlNode RootElement);
	int InitExpBottleLimitConfig(PugiXmlNode RootElement);

	FriendExpBottleOtherCfg m_other_cfg;
	std::vector<ExpBottleLimitCfg> m_exp_bottle_limit_cfg_vec;
};

#endif // __FRIEND_EXP_BOTTLE_CONFIG_HPP__