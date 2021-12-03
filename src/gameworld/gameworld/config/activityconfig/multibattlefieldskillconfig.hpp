#ifndef _MUTLIBATTLEFIELD_SKILL_CONFIG_HPP_
#define _MUTLIBATTLEFIELD_SKILL_CONFIG_HPP_

#include "servercommon/activitydef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gamedef.h"

struct MultiBattleFieldSkillCfg
{
	MultiBattleFieldSkillCfg() : skill_id(0), cd_s(0), distance(0), zhanyi_cost(0),
								 param_a(0), param_b(0), param_c(0) {}

	bool Invalid() const { return 0 == skill_id; }

	int skill_id;
	int cd_s;
	int distance;
	int zhanyi_cost;
	int param_a;
	int param_b;
	int param_c;
};

class MultiBattleFieldSkillConfig : public ConfigBase
{
public:
	MultiBattleFieldSkillConfig();
	~MultiBattleFieldSkillConfig();

	bool Init(const std::string &configname, std::string *err);

	const MultiBattleFieldSkillCfg * GetQunxianSkillCfg(int skill_index) const;
	const MultiBattleFieldSkillCfg * GetXianmoSkillCfg(int skill_index) const;

private:
	int InitQunxianSkillCfg(PugiXmlNode RootElement);
	int InitXianmoSkillCfg(PugiXmlNode RootElement);

	MultiBattleFieldSkillCfg m_qunxian_skill_list[MAX_MULTI_BATTLEFILED_SKILL_COUNT];
	MultiBattleFieldSkillCfg m_xianmo_skill_list[MAX_MULTI_BATTLEFILED_SKILL_COUNT];
};

#endif
