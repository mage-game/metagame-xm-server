#ifndef __MONSTER_SPECIAL_LOGIC_CONFIG_HPP__
#define __MONSTER_SPECIAL_LOGIC_CONFIG_HPP__

#include "servercommon/basedef.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gamedef.h"

#include <string>
#include <map>

enum MONSTER_SPECIAL_LOGIC_TYPE
{
	MONSTER_SPECIAL_LOGIC_TYPE_INVALID = 0,
	
	MONSTER_SPECIAL_LOGIC_TYPE_TWINS,					// 双生怪 (key，relive_time_ms)
	MONSTER_SPECIAL_LOGIC_TYPE_RELATIVE,				// 关联怪 (key, percent, fix)

	MONSTER_SPECIAL_LOGIC_TYPE_MAX,
};

struct MonsterSpecialLogicItem
{
	MonsterSpecialLogicItem() : special_type(0), param1(0), param2(0), param3(0), param4(0) {}

	short special_type;
	int param1;
	int param2;
	int param3;
	int param4;
};

static const int SPECIAL_LOGIC_ITEM_COUNT = 1;			// 最多附带1种特殊逻辑

struct MonsterSpecialParam 
{
	MonsterSpecialParam() : count(0) {}

	int count;
	MonsterSpecialLogicItem special_logic_item_list[SPECIAL_LOGIC_ITEM_COUNT];
};

class MonsterSpecialLogicConfig : public ConfigBase
{
public:
	MonsterSpecialLogicConfig();
	~MonsterSpecialLogicConfig();

	bool Init(const std::string &configname, std::string *err);

	MonsterSpecialParam *GetMonsterSpecialParam(unsigned short monster_id);

private:
	bool CheckSpecialLogicItemConfig(MonsterSpecialLogicItem &logic_item, std::string &err);

	typedef std::map<unsigned short, MonsterSpecialParam> MonsterSpecialParamMap;
	typedef std::map<unsigned short, MonsterSpecialParam>::iterator MonsterSpecialParamMapIt;
	MonsterSpecialParamMap m_monster_special_param_map;
};

#endif
