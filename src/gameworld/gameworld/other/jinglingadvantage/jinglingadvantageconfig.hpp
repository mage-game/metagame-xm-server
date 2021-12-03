#ifndef __JINGLING_ADVANTAGE_CONFIG_HPP__
#define __JINGLING_ADVANTAGE_CONFIG_HPP__

#include "servercommon/configbase.h"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/serverdef.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include <vector>
#include <map>

static const int JINGLING_ADVANTAGE_MAX_EGG_PER_SCENE = 8;

struct JinglingAdvantageOtherCfg 
{
	JinglingAdvantageOtherCfg() : times(0), max_num(0), min_num(0), check_time_interval(0), rate(0), gather_time(0), gather_level_limit(0),
		skip_gather_consume(0), skip_level_limit(0), boss_max_drop_times(0)
	{}

	int times;
	int max_num;
	int min_num;
	int check_time_interval;
	int rate;
	int gather_time;
	int gather_level_limit;

	int skip_gather_consume;
	int skip_level_limit;
	int boss_max_drop_times;
};

struct JinglingAdvantageGatherCfg
{
	JinglingAdvantageGatherCfg() : seq(0), gather_type(-1), gather_id(0), weight(0)
	{}

	enum JINGLING_ADVANTAGE_GATHER_TYPE
	{
		JINGLING_ADVANTAGE_GATHER_TYPE_BLUE = 0,
		JINGLING_ADVANTAGE_GATHER_TYPE_PUEPLE,

		JINGLING_ADVANTAGE_GATHER_TYPE_MAX,
	};

	int seq;
	int gather_type;
	int gather_id;
	int weight;
	
	ItemConfigData reward_item;
};

struct JinglingAdvantageGatherPosCfg 
{
	JinglingAdvantageGatherPosCfg() : scene_id(0), pos(0,0)
	{}

	int scene_id;
	Posi pos;
};

struct JinglingAdvantageBossCfg
{
	JinglingAdvantageBossCfg() : vip_level_min(0), vip_level_max(0), rate(0), monster_id(0)
	{}

	int vip_level_min;
	int vip_level_max;
	int rate;
	unsigned short monster_id;
};

class JinglingAdvantageConfig : public ConfigBase
{
public:
	JinglingAdvantageConfig();
	~JinglingAdvantageConfig();

	bool Init(const std::string &configname, std::string *err);

	const JinglingAdvantageOtherCfg * GetJinglingAdvantageOtherCfg() { return &m_other_cfg; }
	const JinglingAdvantageGatherCfg * GetJinglingAdvantageRandGatherCfg(int type);
	const JinglingAdvantageGatherCfg * GetJinglingAdvantageGatherCfg(int gather_id);
	std::vector<JinglingAdvantageGatherPosCfg> & GetJinglingAdvantageGatherRandPosCfg();

	const JinglingAdvantageBossCfg * GetJinglingAdvantageBossCfg(int vip_level);
	bool IsJingLingAdvantageBoss(unsigned short boss_id);

private:
	int InitJinglingAdvantageOtherCfg(PugiXmlNode RootElement);
	int InitJinglingAdvantageGatherCfg(PugiXmlNode RootElement);
	int InitJinglingAdvantageGatherPosCfg(PugiXmlNode RootElement);
	int InitJinglingAdvantageBossCfg(PugiXmlNode RootElement);

	int m_blue_gather_total_weight;
	int m_purple_gather_total_weight;

	JinglingAdvantageOtherCfg m_other_cfg;
	std::map<int, JinglingAdvantageGatherCfg> m_jingling_gather_cfg_map;
	std::vector<JinglingAdvantageGatherPosCfg> m_jingling_gather_pos_cfg_vec;
	std::vector<JinglingAdvantageBossCfg> m_boss_cfg_vec;
};

#endif // __JINGLING_ADVANTAGE_CONFIG_HPP__
