#ifndef __LIFESKILL_CONFIG_HPP__
#define __LIFESKILL_CONFIG_HPP__

#include "gamedef.h"
#include "servercommon/lifeskilldef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>
#include <map>

struct LifeSkillItemCfg
{
	LifeSkillItemCfg() : type(0)
	{
		memset(need_huoli_list, 0, sizeof(need_huoli_list));
	}
	
	int type;
	int need_huoli_list[LIFE_SKILL_LV_MAX + 1];
};

struct LifeSkillShulianduItemCfg
{
	LifeSkillShulianduItemCfg() : type(0)
	{
		memset(need_shuliandu, 0, sizeof(need_shuliandu));
	}

	int type;
	int need_shuliandu[LIFE_SKILL_SHULIANDU_LV_MAX + 1];
};

struct LifeSkillMakeItemCfg
{
	LifeSkillMakeItemCfg() : index(0), type(0), stuff1(0), count1(0), stuff2(0), count2(0),
							need_skill_level(0), need_huoli(0), add_shuliandu(0), shuliandu_xisu(0),
							green_weight(0), green_item_id(0), blue_weight(0), blue_item_id(0), purple_weight(0), purple_item_id(0), orange_weight(0), orange_item_id(0),
							lucky_rate(0), self_prof_rate(0) {}

	int index;
	int type;
	ItemID stuff1;
	int count1;
	ItemID stuff2;
	int count2;
	int need_skill_level;
	int need_huoli;
	int add_shuliandu;
	int shuliandu_xisu;
	int green_weight;
	ItemID green_item_id;
	int blue_weight;
	ItemID blue_item_id;
	int purple_weight;
	ItemID purple_item_id;
	int orange_weight;
	ItemID orange_item_id;
	int lucky_rate;
	int self_prof_rate;
};

struct LifeSkillProfReplaceCfg
{
	LifeSkillProfReplaceCfg() : make_item_id(0)
	{
		memset(prof_replace_list, 0, sizeof(0));
	}

	ItemID make_item_id;

	static const int PROF_NUM = 4;
	ItemID prof_replace_list[PROF_NUM];
};

class LifeSkillConfig : public ConfigBase
{
public:
	LifeSkillConfig();
	~LifeSkillConfig();

	bool Init(const std::string &path, std::string *err);

	const LifeSkillItemCfg *GetSkillItemCfg(int type);
	const LifeSkillShulianduItemCfg *GetShulianduCfg(int type);
	const LifeSkillMakeItemCfg *GetMakeCfg(int index);
	const LifeSkillProfReplaceCfg *GetProfReplaceCfg(ItemID item_id);

	int GetTotalShuliandu(int type, int level);
	void GetMakeItemId(const LifeSkillMakeItemCfg* make_item_cfg, ItemID *item_id, int self_prof, int cur_total_sld, int total_sld, int *is_make_precious = NULL);

private:
	int InitSkill(PugiXmlNode RootElement);
	int InitShuliandu(PugiXmlNode RootElement);
	int InitMake(PugiXmlNode RootElement);
	int InitProfReplace(PugiXmlNode RootElement);

	ItemID GetProfReplaceItemId(const LifeSkillMakeItemCfg* make_item_cfg, ItemID item_id, int self_prof);
	
	typedef std::map<int, LifeSkillItemCfg> LifeSkillMap;
	typedef std::map<int, LifeSkillShulianduItemCfg> ShulianduMap;
	typedef std::map<int, LifeSkillMakeItemCfg> MakeMap;
	typedef std::map<ItemID, LifeSkillProfReplaceCfg> ProfReplaceMap;

	LifeSkillMap m_skill_map;
	ShulianduMap m_shuliandu_map;
	MakeMap m_make_map;
	ProfReplaceMap m_prof_replace_map;
};

#endif // __LIFESKILL_CONFIG_HPP__

