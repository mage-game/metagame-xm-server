// F-副本-传世名剑.xls
#ifndef __MOJIE_SKILL_CONFIG_HPP__
#define __MOJIE_SKILL_CONFIG_HPP__

#include <string>
#include "servercommon/configbase.h"
#include "servercommon/servercommon.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct MojieSkillActiveCfg
{
	MojieSkillActiveCfg() : skill_id(0), pata_layer(0)
	{}

	int skill_id;
	int pata_layer;
};

struct MojieSkillCfg 
{
	MojieSkillCfg() : skill_id(0), skill_type(0)
	{
		memset(param_list, 0, sizeof(param_list));
	}

	int skill_id;
	int skill_type;

	enum SKILL_PARAM
	{
		SKILL_PARAM_0 = 0,
		SKILL_PARAM_1 = 1,
		SKILL_PARAM_2 = 2,
		SKILL_PARAM_3 = 3,

		SKILL_PARAM_COUNT,
	};
	static const int MAX_PARAM_COUNT = 4;
	UNSTD_STATIC_CHECK(SKILL_PARAM_COUNT == MAX_PARAM_COUNT)

	int param_list[MAX_PARAM_COUNT];

//	static const int INVALID_PARAM = -1;
};

enum SKILL_TYPE
{
	SKILL_TYPE_0 = 0,							//攻击时有概率召唤天雷，对主目标造成X%攻击的伤害。
	SKILL_TYPE_1 = 1,							//移动速度+10%
	SKILL_TYPE_2 = 2,							//每10秒恢复X%最大生命值。
	SKILL_TYPE_3 = 3,							//经验获取+20%
	SKILL_TYPE_4 = 4,							//PVE伤害加成+X%
	SKILL_TYPE_5 = 5,							//暴击伤害+X%，会心伤害+Y%
	SKILL_TYPE_6 = 6,							//减伤X%，PVP减伤Y%
	SKILL_TYPE_7 = 7,							//会心一击率提高X%，会心伤害提高Y%
	SKILL_TYPE_8 = 8,							//抗暴几率+X%，暴击抵抗+Y%
	SKILL_TYPE_9 = 9,							//受到控制效果影响的5秒内，其他控制效果持续时间缩短50%。
	SKILL_TYPE_10 = 10,							//攻击时有概率使目标移动速度降低下X%，持续2秒

	SKILL_TYPE_MAX,
};

class MojieSkillConfig : public ConfigBase
{
public:
	MojieSkillConfig();
	~MojieSkillConfig();

	static const int MOJIE_SKILL_COUNT = 40;

	bool Init(const std::string &configname, std::string *err);

	void SetActiveSkillFlag(unsigned long long &flag, int pata_layer);
	const MojieSkillCfg * GetSkillCfg(int skill_id);
	const int GetSkillTypeByLayer(int layer);

private:
	int InitActiveCfg(PugiXmlNode RootElement);
	int InitSkillCfg(PugiXmlNode RootElement);

	bool CheckSkillParam();

	MojieSkillActiveCfg m_active_cfg_list[MOJIE_SKILL_COUNT];
	MojieSkillCfg m_skill_cfg_list[MOJIE_SKILL_COUNT];
};

#endif // __MOJIE_SKILL_CONFIG_HPP__
