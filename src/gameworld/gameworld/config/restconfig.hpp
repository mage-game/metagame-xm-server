#ifndef __RESTCONFIG_HPP__
#define __RESTCONFIG_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "servercommon/struct/itemlistparam.h"

class RestConfig : public ConfigBase
{
public:
	struct RewardItem
	{
		RewardItem() : exp(0), xianhun(0), yuanli(0), mojing(0),
			monster_id(0), minute_drop_count(0){}

		bool Invalid() { return (0 == exp && 0 == xianhun && 0 == yuanli && 0 == mojing);}

		long long exp;
		int xianhun;
		int yuanli;
		int mojing;

		int monster_id;
		int minute_drop_count;
	};

public:
	RestConfig();
	~RestConfig();

	bool Init(const std::string &path, std::string *err);

	bool GetRestReward(int level, long long *exp, int *xianhun = NULL, int *yuanli = NULL, int *mojing = NULL);

	const RewardItem* GetRestConfig(int level) const;


private:

	RewardItem m_reward[MAX_ROLE_LEVEL + 1];
};

#endif // __RESTCONFIG_H__

