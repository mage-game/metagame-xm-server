#ifndef __SHENGQI_CONFIG_H__
#define __SHENGQI_CONFIG_H__

#include "servercommon/configcommon.h"
#include <map>
#include "servercommon/struct/shengqiparam.hpp"
#include "servercommon/configbase.h"
#include "servercommon/struct/itemlistparam.h"

struct ShengqiOtherCfg
{
	ShengqiOtherCfg() : open_level(0), open_gold(0), spirit_max(0), strength_max(0), per_one_spirit_strength(0), per_two_spirit_strength(0), per_three_spirit_strength(0), per_spirit_default(0)
	{
	}

	int open_level;
	int open_gold;
	int spirit_max;
	int strength_max;
	int per_one_spirit_strength;
	int per_two_spirit_strength;
	int per_three_spirit_strength;
	int per_spirit_default;
};

struct ShengqiCfg
{
	ShengqiCfg() : index(0), id(0), series(0)
	{
	}

	int index;
	ItemID id;
	int series;
};

struct ShengqiOpenConditionCfg
{
	ShengqiOpenConditionCfg() : index(0), open_level(0), stuff_id(0), stuff_num(0)
	{
	}

	int index;
	int open_level;

	ItemID stuff_id;
	int stuff_num;
};

struct ShengqiSpiritCfg
{
	ShengqiSpiritCfg() : index(0), spirit_stuff_id(0), spirit_stuff_num(0), per_strength_min(0), per_strength_max(0)
	{
		memset(attr_type, 0, sizeof(attr_type));
		memset(attr_total_value, 0, sizeof(attr_total_value));
	}

	int index;
	ItemID spirit_stuff_id;
	int spirit_stuff_num;
	int per_strength_min;
	int per_strength_max;

	int attr_type[SHENGQI_SPIRIT_MAX_NUM];
	int attr_total_value[SHENGQI_SPIRIT_MAX_NUM];
};

struct ShengqiStrengthCfg
{
	ShengqiStrengthCfg() : index(0), level(0), strength_stuff_id(0), strength_stuff_num(0), maxhp(0),
		gongji(0), fangyu(0), per_baoji(0), per_jingzhuan(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), per_kangbao(0), skill_jianshang_per(0), shanbi_per(0), 
		per_mianshang(0), mingzhong_per(0), gedang_per(0), per_pofang(0)
	{
	}

	int index;
	int level;
	ItemID strength_stuff_id;
	int strength_stuff_num;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	//int fa_fangyu;
	//int pojia;
	Attribute per_baoji;
	Attribute per_jingzhuan;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	Attribute per_kangbao;
	Attribute skill_jianshang_per;
	Attribute shanbi_per;
	Attribute per_mianshang;
	Attribute mingzhong_per;
	Attribute gedang_per;
	Attribute per_pofang;
};

struct ShengqiDecomposeCfg
{
	ShengqiDecomposeCfg() :stuff_id(0) {}
	ItemID stuff_id;
	ItemConfigData return_item;
};

class ShengqiConfig : public ConfigBase
{
public:
	ShengqiConfig();
	~ShengqiConfig();

	bool Init(const std::string &configname, std::string *err);

	const ShengqiOtherCfg & GetOtherCfg() const { return m_other_cfg; }
	const ShengqiCfg *GetShengqiCfg(int shengqi_index) const;
	const ShengqiOpenConditionCfg *GetOpenConditionCfg(int shengqi_index) const;
	const ShengqiSpiritCfg *GetSpiritCfg(int shengqi_index) const;
	const ShengqiStrengthCfg *GetStrengthCfg(int shengqi_index, int level) const;
	
	int GetShengqiIndexByItem(ItemID item_id);
	ItemID GetItemByShengqiIndex(int shengqi_index);
	const ShengqiDecomposeCfg *GetDecomposeCfg(ItemID stuff_id) const;

private:
	int InitOtherCfg(TiXmlElement *RootElement);
	int InitShengqiCfg(TiXmlElement *RootElement);
	int InitOpenConditionCfg(TiXmlElement *RootElement);
	int InitSpiritCfg(TiXmlElement *RootElement);
	int InitStrengthCfg(TiXmlElement *RootElement);
	int InitDecomposeCfg(TiXmlElement *RootElement);

	ShengqiOtherCfg m_other_cfg;

	ShengqiCfg m_shengqi_cfg_list[SHENGQI_MAX_NUM];

	typedef std::map<int, ShengqiOpenConditionCfg> ShengqiOpenConditionCfgMap;
	typedef std::map<int, ShengqiOpenConditionCfg>::const_iterator ShengqiOpenConditionCfgMapIt;
	ShengqiOpenConditionCfgMap m_open_condition_cfg_map;

	typedef std::map<int, ShengqiSpiritCfg> ShengqiSpiritCfgMap;
	typedef std::map<int, ShengqiSpiritCfg>::const_iterator ShengqiSpiritCfgMapIt;
	ShengqiSpiritCfgMap m_spirit_cfg_map;

	typedef std::map<long long, ShengqiStrengthCfg> ShengqiStrengthCfgMap;
	typedef std::map<long long, ShengqiStrengthCfg>::const_iterator ShengqiStrengthCfgMapIt;
	ShengqiStrengthCfgMap m_strength_cfg_map;

	std::map<ItemID, int> m_item_to_shengqi_type_map;								// 物品id对应圣器类型
	std::map<ItemID, ShengqiDecomposeCfg> m_decompose_map;
};

#endif
