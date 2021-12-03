#ifndef __UPGRADE_SKILL_CONFIG_HPP__
#define __UPGRADE_SKILL_CONFIG_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <map>

#include "common/tinyxml/tinyxml.h"

struct UpGradeSkillLevelCfg
{
	UpGradeSkillLevelCfg():skill_type(0), skill_level(0), injure_per(0){}

	int skill_type;
	int skill_level;
	int injure_per;
};

class UpGradeSkillConfig : public ConfigBase
{
public:

	enum UPGRADE_SKILL_TYPE
	{
		SKILL_TYPE_MOUNT = 0,
		SKILL_TYPE_WING,
		SKILL_TYPE_HALO,
		SKILL_TYPE_SHENGONG,
		SKILL_TYPE_SHENYI,
		SKILL_TYPE_FIGHT_MOUNT,
		SKILL_TYPE_FOOTPRINT,
		SKILL_TYPE_FABAO,
		SKILL_TYPE_SHIZHUANG,

		SKILL_TYPE_MAX,
	};

	UpGradeSkillConfig();
	~UpGradeSkillConfig();

	bool Init(const std::string &path, std::string *err);
	
	int GetGaugeBySkillCount(int skill_count);
	UpGradeSkillLevelCfg * GetSkillLevelCfg(int skill_type, int skill_level);

private:

	int InitGaugeCfg(PugiXmlNode RootElement);
	int InitSkillLevelCfg(PugiXmlNode RootElement);

	std::map<int, int> m_gauge_map;
	std::map<int, UpGradeSkillLevelCfg> m_upgrade_skill_map[SKILL_TYPE_MAX];
};

#endif // __UPGRADE_SKILL_CONFIG_HPP__

