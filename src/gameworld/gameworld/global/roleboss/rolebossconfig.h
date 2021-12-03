#ifndef __ROLE_BOSS_CONFIG_H__
#define __ROLE_BOSS_CONFIG_H__

#include "servercommon/configcommon.h"
#include "servercommon/serverdef.h"
#include "servercommon/basedef.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>

const static int ROLE_BOSS_REFRESH_TIME_CFG_MAX_COUNT = 16;
const static int ROLE_BOSS_MAX_COUNT = 3;

struct RoleBossRefreshCfg
{
	RoleBossRefreshCfg() : scene_id(0), pos(0, 0), attr_level(0), exp_level(0) {}

	int scene_id;
	Posi pos;
	int attr_level;
	int exp_level;

	std::vector<UInt16> top_hurt_dropid_set;
	std::vector<UInt16> join_dropid_set;
	std::vector<UInt16> kill_dropid_set;
};

class RoleBossConfig : public ConfigBase
{
public:
	RoleBossConfig();
	~RoleBossConfig();

	bool Init(std::string path, std::string *err);
	
	const RoleBossRefreshCfg * GetRefreshCfg(int index);
	time_t CalNextRefreshTimestamp();
	double GetExpFactor(int rank);

private:
	int InitRefreshTimeCfg(PugiXmlNode RootElement);
	int InitRefreshCfg(PugiXmlNode RootElement);

	int m_refresh_time_count;
	int m_refresh_time_list[ROLE_BOSS_REFRESH_TIME_CFG_MAX_COUNT];

	RoleBossRefreshCfg m_refresh_cfg_list[ROLE_BOSS_MAX_COUNT];

	struct ExpFactor
	{
		ExpFactor() : rank(-1), factor(0) {}
		ExpFactor(int _rank, double _factor) : rank(_rank), factor(_factor) {}

		bool IsValid() { return rank >= 0; }

		int rank;
		double factor;
	};

	const static int EXP_FACTOR_MAX_COUNT = 16;

	int m_exp_factor_count;
	ExpFactor m_exp_factor_list[EXP_FACTOR_MAX_COUNT];
};

#endif

