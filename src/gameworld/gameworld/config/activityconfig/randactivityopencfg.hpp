#ifndef __RAND_ACTIVITY_OPEN_CFG_HPP__
#define __RAND_ACTIVITY_OPEN_CFG_HPP__

#include <string.h>
#include <map>
#include "servercommon/configcommon.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"



struct RandActivityOpenDetail
{
	RandActivityOpenDetail() : activity_type(0), begin_day_idx(0), end_day_idx(0), open_type(0), send_mail_need_level(0), begin_timestamp(0), end_timestamp(0) {}

	bool IsValid() { return 0 != activity_type; }

	int activity_type;
	int begin_day_idx;
	int end_day_idx;
	int open_type;
	int send_mail_need_level;

	unsigned int begin_timestamp;
	unsigned int end_timestamp;
};

class RandActivityOpenCfg : public ConfigBase
{
public:
	RandActivityOpenCfg();
	~RandActivityOpenCfg();

	bool Init(const std::string &path, std::string *err);

	int GetAllowSetTimeDayIdx() { return m_allow_set_time_dayidx; }
	time_t GetRandActivitySwitchTimeStamp();
	const RandActivityOpenDetail * GetOpenCfg(int activity_type);
	int GetUpgradeActivityEndDay()const { return m_upgrade_activity_end_day; }
	bool isUpgradeRankActivity(int ra_type) const;
private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitOpenCfg(PugiXmlNode RootElement);
	int InitVersionOpenCfg(PugiXmlNode RootElement);

	int m_begin_day_idx;
	int m_end_day_idx; 
	RandActivityOpenDetail m_open_detail_list[RAND_ACTIVITY_TYPE_MAX];

	time_t m_rand_activity_cfg_switch_timestamp;

	int m_allow_set_time_dayidx;
	
	int m_upgrade_activity_end_day;
};

#endif

