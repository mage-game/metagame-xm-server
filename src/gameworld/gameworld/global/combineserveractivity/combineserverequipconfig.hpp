#ifndef __COMBINE_SERVER_EQUIP_CONFIG_HPP__
#define __COMBINE_SERVER_EQUIP_CONFIG_HPP__

#include "servercommon/combineserverequipdef.hpp"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct CSACommonEquipUplevelItemCfg
{
	CSACommonEquipUplevelItemCfg() : index(0), level(0), stuff_id(0), stuff_num(0), gongji(0), fangyu(0),
		maxhp(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), extra_gongji(0), extra_fangyu(0), extra_maxhp(0),
		extra_mingzhong(0), extra_shanbi(0), extra_baoji(0), extra_jianren(0)
	{
	}

	int index;
	int level;
	int stuff_id;
	int stuff_num;

	Attribute gongji;
	Attribute fangyu;
	Attribute maxhp;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;

	Attribute extra_gongji;
	Attribute extra_fangyu;
	Attribute extra_maxhp;
	Attribute extra_mingzhong;
	Attribute extra_shanbi;
	Attribute extra_baoji;
	Attribute extra_jianren;
};

struct CSAGreatEquipUplevelItemCfg
{
	CSAGreatEquipUplevelItemCfg() : index(0), level(0), stuff_id(0), stuff_num(0), gongji(0), fangyu(0),
		maxhp(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), pvp_shanghai_jiacheng(0), pvp_shanghai_jianmian(0)
	{
	}

	int index;
	int level;
	int stuff_id;
	int stuff_num;

	Attribute gongji;
	Attribute fangyu;
	Attribute maxhp;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;

	Attribute pvp_shanghai_jiacheng;
	Attribute pvp_shanghai_jianmian;
};

struct CSAPartItemCfg
{
	CSAPartItemCfg() : part_index(0) {}

	int part_index;
	CSACommonEquipUplevelItemCfg common_equip_uplevel_list[CSA_EQUIP_MAX_LEVEL + 1];
	CSAGreatEquipUplevelItemCfg great_equip_uplevel_list[CSA_EQUIP_MAX_LEVEL + 1];
};

class CombineServerActivityEquipConfig : public ConfigBase
{
public:
	CombineServerActivityEquipConfig();
	~CombineServerActivityEquipConfig();

	const CSACommonEquipUplevelItemCfg *GetCommonlevelItemCfg(int part_index, int level);
	const CSAGreatEquipUplevelItemCfg *GetGreatlevelItemCfg(int part_index, int level);
	int GetCommonMaxLevel() { return m_common_max_level; }
	int GetGreatMaxLevel() { return m_great_max_level; }

	bool Init(const std::string &configname, std::string *err);

private:
	int InitCommonEquipLevelCfg(PugiXmlNode RootElement);
	int InitGreatEquipLevelCfg(PugiXmlNode RootElement);

	int m_common_max_level;
	int m_great_max_level;
	CSAPartItemCfg m_part_list[CSA_EQUIP_MAX_PART];
};

#endif // __COMBINE_SERVER_EQUIP_CONFIG_HPP__

