#ifndef _HOT_SPRING_CONFIG_HPP_
#define _HOT_SPRING_CONFIG_HPP_

#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gamedef.h"

#include <vector>

struct HotspringOtherConfig
{
	HotspringOtherConfig() : open_level(0), sceneid(0),enter_pos(0,0), exp_interval_s(0), popularity_rank_interval_s(0), free_give_present_times(0),
		free_present_id(0),interaction_add(0), reward_item_interval(0),item_count(0),
		refresh_gather_interval(0), refresh_gather_start_time_s(0), max_gather_times(0), exp_factor(0)
	{
	}

	int open_level;
	int sceneid;
	Posi enter_pos;
	int exp_interval_s;
	int popularity_rank_interval_s;
	short free_give_present_times;
	short free_present_id;
	int interaction_add;
	unsigned int reward_item_interval;
	int item_count;
	ItemConfigData interval_reward_item[MAX_ATTACHMENT_ITEM_NUM];

	int refresh_gather_interval;
	int refresh_gather_start_time_s;
	int max_gather_times;
	float exp_factor;

};

struct HotspringExpRewardCfg
{
	HotspringExpRewardCfg():min_limit_level(0),max_limit_level(0), base_exp(0) {}

	int min_limit_level;
	int max_limit_level;
	int base_exp;

};

struct HotspringPresentCfg
{
	HotspringPresentCfg():present_id(0),present_price(0),add_popularity(0){}

	int present_id;
	int present_price;
	int add_popularity;
};

enum HOTSPRING_SKILL_TYPE
{
	HOTSPRING_SKILL_INVALID = 0,

	HOTSPRING_SKILL_MASSAGE,			// °´Ä¦
	HOTSPRING_SKILL_THROW_SNOWBALL,		// ÈÓÑ©Çò

	HOTSPRING_SKILL_MAX,
};

struct HotspringSkillCfg
{
	HotspringSkillCfg():skill_type(0),cold_down_time(0),buff_effect_time(0),
	use_times_limit(0), skill_distance(0), use_add_times(0), be_use_add_times(0), exp_factor_use(0), exp_factor_be_use(0)
	{}

	short skill_type;
	int cold_down_time;
	int buff_effect_time;
	short use_times_limit;
	int skill_distance;

	int use_add_times;
	int be_use_add_times;

	double exp_factor_use;
	double exp_factor_be_use;
};

struct HotspringGahterCfg
{
	HotspringGahterCfg() : gather_id(0), gather_time_ms(0), gather_disappear_time(0), gather_count(0), add_times(0), exp_factor_gather(0)
	{}

	int gather_id;
	int gather_time_ms;
	int gather_disappear_time;
	int gather_count;
	int add_times;
	double exp_factor_gather;

	ItemConfigData reward_item;
};

class HotspringConfig : public ConfigBase
{
public:
	HotspringConfig();
	~HotspringConfig();

	bool Init(const std::string &configname, std::string *err);
	
	const HotspringOtherConfig & GetOtherCfg() { return m_other_cfg; }
	const HotspringExpRewardCfg * GetExpCfg(int level);
	const HotspringPresentCfg * GetPresentCfg(int index);
	const HotspringPresentCfg * GetFreePresentCfg();
	const HotspringSkillCfg * GetSkillCfgBySkillType(int skill_type);

	const HotspringGahterCfg * GetGatherRewardCfg(int gather_id);
	const std::map<int, HotspringGahterCfg> & GetGatherCfg() { return m_gather_cfg_map; }
	const std::vector<Posi> & GetRandGatherPosVec();
	
private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitExpCfg(PugiXmlNode RootElement);
	int InitPresentCfg(PugiXmlNode RootElement);
	int InitSkillCfg(PugiXmlNode RootElement);
	int InitGatherCfg(PugiXmlNode RootElement);
	int InitGatherPosCfg(PugiXmlNode RootElement);

	HotspringOtherConfig m_other_cfg;

	std::map<int, HotspringExpRewardCfg> m_exp_reward_cfg;
	std::map<int, HotspringPresentCfg> m_present_cfg;
	std::map<int, HotspringSkillCfg> m_skill_cfg;

	std::map<int, HotspringGahterCfg> m_gather_cfg_map;

	std::vector<Posi> m_gather_pos_vec;
};

#endif // HotspringConfig



