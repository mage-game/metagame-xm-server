#ifndef __WORSHIP_CONFIG_H__
#define __WORSHIP_CONFIG_H__

#include <map>
#include "servercommon/configcommon.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

// B-拜谒.xls

// 活动膜拜配置
struct WorshipOtherConfig
{
	WorshipOtherConfig(): activityde_type(0), worship_time(0), 
		worship_scene_id(0), worship_pos(0,0), worship_range(0),
		worship_reward_interval(0), worship_click_times(0), worship_click_cd(0), worship_gather_time(0)
	{};

	int activityde_type;
	int worship_time;				// 膜拜总时间
	
	int worship_scene_id;
	Posi worship_pos;
	int worship_range;
	int worship_reward_interval;	// 
	int worship_click_times;		// 攻城战膜拜点击次数限制
	int worship_click_cd;			// 攻城战膜拜点击cd
	int worship_gather_time;		// 膜拜采集时长
};

class WorshipConfig : public ConfigBase
{
public:
	WorshipConfig() {};
	~WorshipConfig() {};

	bool Init(const std::string &configname, std::string *err);

	const WorshipOtherConfig * GetOtherConfig(int activity_type);
	int GetReardGoldBind();	// 获取奖励绑元

private:
	int InitOtherCfg(PugiXmlNode root_element);
	int InitRewardCfg(PugiXmlNode root_element);

	std::map<int, WorshipOtherConfig> m_other_cfg;
	std::map<int, int> m_reward_cfg;

};

#endif // __WORSHIP_CONFIG_H__