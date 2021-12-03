#ifndef __ROLE_BIG_SMALL_CONFIG_HPP__
#define __ROLE_BIG_SMALL_CONFIG_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/maildef.hpp"

#include <vector>
#include "servercommon/struct/rolebigsmallgoalparam.hpp"

struct RoleBSGoalItemConfig
{
	RoleBSGoalItemConfig() : reward_type(0), system_type(0), openserver_day(0), open_level(0), free_time_since_open_s(0),
		condition_type(0), param_1(0), param_2(0), cost_gold(0), reward_count(0), is_broadcast(false){}

	int reward_type;
	int system_type;
	int openserver_day;
	int open_level;
	unsigned int free_time_since_open_s;
	int condition_type;
	int param_1;
	int param_2;
	int cost_gold;
	int reward_count;
	bool is_broadcast;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct RoleBSGoalAttrConfig
{
	RoleBSGoalAttrConfig() : system_type(0), active_attr_param(0), add_attr_value(0),
		max_hp(0), gongji(0), fangyu(0) {}

	int system_type;
	int active_attr_param;
	int add_attr_value;
	Attribute max_hp;
	Attribute gongji;
	Attribute fangyu;
};

class RoleBigSmallGoalConfig : public ConfigBase
{
public:
	RoleBigSmallGoalConfig();
	~RoleBigSmallGoalConfig();

	bool Init(const std::string &path, std::string *err);
	RoleBSGoalItemConfig *GetBSGoalItemConfig(int system_type, int goal_type);
	int GetBSGoalAttrAddValue(int system_type);
	const RoleBSGoalAttrConfig *GetBSGoalAttrCfg(int system_type);
private:
	int InitGoalItemConfig(PugiXmlNode RootElement);
	int InitGoalAttrConfig(PugiXmlNode RootElement);

	typedef std::vector<RoleBSGoalItemConfig> RoleBSGoalItemVec;
	typedef std::vector<RoleBSGoalItemConfig>::iterator RoleBSGoalItemVecIt;

	RoleBSGoalItemVec m_role_bs_goal_reward_vec;
	RoleBSGoalAttrConfig m_role_bs_goal_attr_list[ROLE_BIG_SMALL_GOAL_SYSTEM_MAX];
};

#endif // __ROLE_BIG_SMALL_CONFIG_HPP__

