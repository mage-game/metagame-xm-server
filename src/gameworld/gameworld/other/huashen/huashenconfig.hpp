#ifndef __HUASHEN_CONFIG_H__
#define __HUASHEN_CONFIG_H__

#include "common/tinyxml/tinyxml.h"
#include "servercommon/huashendef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "item/itembase.h"

struct HuaShenOtherConfig
{
	HuaShenOtherConfig() : level_once_val(0) {}
	
	int level_once_val;		// 升级一次得到的进化值
};

struct HuaShenLevelConfig
{
	HuaShenLevelConfig() : level(0), jinhua_val(0), stuff_id(ItemBase::INVALID_ITEM_ID), stuff_num(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0) {}

	int level;				// 等级
	int jinhua_val;			// 进化值
	ItemID stuff_id;		// 消化物品ID
	int stuff_num;			// 消耗物品数量

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
};

struct HuaShenGradeConfig
{
	HuaShenGradeConfig() : grade(0), stuff_id(ItemBase::INVALID_ITEM_ID), stuff_num(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0) {}

	int grade;				// 等级
	ItemID stuff_id;		// 消化物品ID
	int stuff_num;			// 消耗物品数量

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
};

struct HuaShenInfoConfig
{
	HuaShenInfoConfig() : id(-1), monster_id(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), max_level(0), max_grade(0) {}
	
	int id;						// 化神ID
	unsigned short monster_id;	// 变身成为的怪物ID

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;

	short max_level;	// 最大等级
	HuaShenLevelConfig level_list[HUASHEN_MAX_LEVEL_LIMIT + 1];

	short max_grade;	// 最大阶
	HuaShenGradeConfig grade_list[HUASHEN_MAX_GRADE_LIMIT + 1];
};

struct HuaShenSpiritConfig
{
	struct SpiritLevelConfig
	{
		SpiritLevelConfig() : level(0), consume_item_id(ItemBase::INVALID_ITEM_ID), consume_item_count(0), add_exp_val(0), need_exp_val(0), 
			maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0) {}

		int level;
		ItemID consume_item_id;
		int consume_item_count;
		int add_exp_val;
		int need_exp_val;

		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
	};

	HuaShenSpiritConfig() : id(0), max_level(-1) {}

	int id;

	int max_level;
	SpiritLevelConfig level_cfg_list[HUASHEN_SPIRIT_MAX_LEVEL + 1];
};

class HuaShenConfig : public ConfigBase
{
public:
	HuaShenConfig();
	~HuaShenConfig();

	bool Init(const std::string &configname, std::string *err);
	const HuaShenOtherConfig & GetOtherCfg() { return m_other_cfg; }
	const HuaShenInfoConfig * GetHuaShenInfoCfg(int id);
	const HuaShenLevelConfig * GetHuaShenLevelCfg(int id, int level);
	const HuaShenSpiritConfig::SpiritLevelConfig * GetHuaShenSpiritLevelCfg(int spirit_id, int level);
	const HuaShenGradeConfig * GetHuaShenGradeCfg(int id, int grade);

	int GetHuaShenSpiritMaxLevel(int spirit_id);
	int GetHuaShenMaxID() { return m_huashen_max_id; }
 
private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitHuaShenInfoCfg(PugiXmlNode RootElement);
	int InitHuaShenLevelCfg(PugiXmlNode RootElement);
	int InitHuaShenSpiritCfg(PugiXmlNode RootElement);
	int InitHuaShenGradeCfg(PugiXmlNode RootElement);
	
	HuaShenOtherConfig m_other_cfg;

	int m_huashen_max_id;
	HuaShenInfoConfig m_huashen_info_list[HUASHEN_MAX_ID_LIMIT + 1];

	int m_huashen_spirit_max_id;
	HuaShenSpiritConfig m_huashen_spirit_cfg_list[HUASHEN_SPIRIT_MAX_ID_LIMIT + 1];

};

#endif