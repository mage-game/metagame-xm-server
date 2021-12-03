#ifndef __TALENT_CONFIG_HPP__
#define __TALENT_CONFIG_HPP__

#include <map>
#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/talentdef.hpp"
#include "servercommon/configbase.h"

struct TalentOtherCfg
{
	TalentOtherCfg() : open_talent_level(0), base_talent_point(0), reset_consume_item(0), proficient_talent_limit(0), first_point_consume_gold(0){}

	int open_talent_level;					// 四转等级限制,巅峰等级
	int base_talent_point;					// 基础技能点
	ItemID reset_consume_item;				// 重置天赋点消耗物品
	short proficient_talent_limit;
	int first_point_consume_gold;			// 新天赋技能第一次加点的时候消耗（优先绑元）
};

static const int TALENT_LEVEL_MAX_CONFIG_COUNT = MAX_TELENT_INDEX_COUT * MAX_TELENT_TYPE_COUT;

struct TalentLevelMaxConifg
{
	TalentLevelMaxConifg() : talent_id(0), max_level(0){}

	int talent_id;
	int max_level;
};

struct TalentLevelConifg
{
	static const int TALENT_MAX_ATTR_TYPE = 3;

	TalentLevelConifg() : talent_id(0), talent_level(0), talent_type(0), pre_talent_type(0), pre_talent_type_level(0), pre_talent_id(0), pre_talent_level(0),
		effect_type(0), cool_down(0), add_fix_cap(0), skill_cap_per(0), param_a(0), param_b(0), param_c(0), param_d(0)
	{
		memset(talent_attr_type_list, 0, sizeof(talent_attr_type_list));
		memset(talent_attr_value_list, 0, sizeof(talent_attr_value_list));
	}

	int talent_id;
	int talent_level;
	int talent_type;
	int pre_talent_type;
	int pre_talent_type_level;
	int pre_talent_id;
	int pre_talent_level;

	int effect_type;

	int talent_attr_type_list[TALENT_MAX_ATTR_TYPE];
	int talent_attr_value_list[TALENT_MAX_ATTR_TYPE];

	int cool_down;
	int add_fix_cap;
	int skill_cap_per;
	int param_a;
	int param_b;
	int param_c;
	int param_d;
};

class TalentConfig : public ConfigBase
{
public:
	TalentConfig();
	~TalentConfig();

	bool Init(const std::string &path, std::string *err);

	const TalentOtherCfg &GetTalentOtherCfg() { return m_other_cfg; }

	const int GetTalentLevelMax(int talent_id) const;
	const TalentLevelConifg *GetTalentLevelCfg(int talent_id, int level) const;

	int GetLvAddTalentPoint(int lv);
	int GetZhuanzhiLvAddTalentPoint(int zhuanzhi_lv);

private:
	int InitTalentLevelMaxCfg(TiXmlElement *RootElement);
	int InitTalentLevelCfg(TiXmlElement *RootElement);
	int InitLevelAddTalentPointCfg(TiXmlElement *RootElement);
	int InitZhuanzhiLvAddTalentPointCfg(TiXmlElement *RootElement);
	int InitOtherCfg(TiXmlElement *RootElement);

	TalentOtherCfg m_other_cfg;

	TalentLevelMaxConifg m_talent_level_max_cfg[TALENT_LEVEL_MAX_CONFIG_COUNT];

	typedef std::map<long long, TalentLevelConifg> TalentLevelConfigMap;
	typedef TalentLevelConfigMap::const_iterator TalentLevelConfigMapIt;
	TalentLevelConfigMap m_talent_level_cfg_map;

	std::map<int, int> m_lv_add_talent_cfg;
	std::map<int, int> m_zhuanzhi_lv_add_talent_cfg;
};

#endif