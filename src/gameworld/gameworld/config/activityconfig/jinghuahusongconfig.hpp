#ifndef _JINGHUASONG_CONFIG_HPP_
#define _JINGHUASONG_CONFIG_HPP_

#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gamedef.h"

#include <vector>

struct JHHusongOtherCfg
{
	JHHusongOtherCfg() : open_hour(0), end_hour(0), gather_id(0), gather_scene_id(0), gather_pos(0,0), 
					gather_time(0), gather_change_times(0),
					gather_min_times(0), gather_max_times(0), commit_npcid(0), gather_day_count(0), gather_get_jinghua(0),
					lost_jinghua_per(0), husong_timeout(0) {}

	int open_hour;
	int end_hour;
	int gather_id;
	int gather_scene_id;
	Posi gather_pos;
	int gather_time;
	int gather_change_times;
	int gather_min_times;
	int gather_max_times;
	int commit_npcid;
	int gather_day_count;
	int gather_get_jinghua;
	int lost_jinghua_per;
	int husong_timeout;
};

struct JHHusongRewardCfg
{
	JHHusongRewardCfg() : commit_times(0), reward_per(0) {}

	int commit_times;
	int reward_per;
};

class JingHuaHusongConfig : public ConfigBase
{
public:
	JingHuaHusongConfig();
	~JingHuaHusongConfig();

	bool Init(const std::string &configname, std::string *err);

	const JHHusongOtherCfg & GetOtherCfg() { return m_other_cfg; }

	int GetJingHuaRewardPer(int commit_times);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitRewardCfg(PugiXmlNode RootElement);
	
	JHHusongOtherCfg m_other_cfg;

	typedef std::vector<JHHusongRewardCfg> JhhusongRewardVec;
	JhhusongRewardVec m_reward_vec;
};

#endif



