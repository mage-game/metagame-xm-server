#ifndef __CROSS_RAND_ACTIVITY_OPEN_CONFIG_HPP__
#define __CROSS_RAND_ACTIVITY_OPEN_CONFIG_HPP__

#include "servercommon/crossdef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/activitydef.hpp"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/crossrandactivitydef.hpp"

static const int ROSS_RAND_ACTIVITY_TIME_CFG_COUNT_MAX = 5;

struct CrossRandActivityTimeCfg
{
	CrossRandActivityTimeCfg() : activity_type(0), time_cfg_count(0) {}

	struct TimeItem
	{
		TimeItem() : begin_timestamp(0), end_timestamp(0)
		{
		}

		unsigned int begin_timestamp;
		unsigned int end_timestamp;
	};

	int activity_type;

	int time_cfg_count;
	TimeItem time_cfg_list[ROSS_RAND_ACTIVITY_TIME_CFG_COUNT_MAX];
};

class CrossRandActivityOpenConfig
{
public:
	static CrossRandActivityOpenConfig & Instance();

	bool Init(const std::string &path, std::string *err);

	const CrossRandActivityTimeCfg * GetOpenCfg(int activity_type);
	
private:
	CrossRandActivityOpenConfig();
	~CrossRandActivityOpenConfig();

	int InitOtherCfg(PugiXmlNode RootElement);
	int InitOpenTimeCfg(PugiXmlNode RootElement);

	int m_activity_open_time_cfg_count;
	CrossRandActivityTimeCfg m_activity_open_time_cfg_list[CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT];
};

#endif // __CROSS_1V1_COMMON_CONFIG_HPP__

