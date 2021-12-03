#ifndef __ZODIAC_CONFIG_H__
#define __ZODIAC_CONFIG_H__

#include "servercommon/configcommon.h"
#include <map>
#include "servercommon/struct/zodiacparam.hpp"

struct ZodiacOtherCfg
{
	ZodiacOtherCfg() : open_level(0), max_level(0) {}

	int open_level;
	int max_level;
};

struct ZodiacRealIdCfg
{
	ZodiacRealIdCfg() : suipian_id(0), zodiac_index(0), suipian_index(0) {}

	ItemID suipian_id;
	int zodiac_index;
	int suipian_index;
};

struct ZodiacActivateCfg
{
	ZodiacActivateCfg() : zodiac_index(0), activate_num(0), attr_type(0), attr_value(0) {}

	int zodiac_index;
	int activate_num;

	int attr_type;
	Attribute attr_value;
};

struct ZodiacLevelupCfg
{
	ZodiacLevelupCfg() : zodiac_index(0), level(0), jinghua_num(0)
	{
		memset(attr_type, 0, sizeof(attr_type));
		memset(attr_value, 0, sizeof(attr_value));
	}

	int zodiac_index;
	int level;
	int jinghua_num;

	int attr_type[ZODIAC_ATTR_MAX_NUM];
	Attribute attr_value[ZODIAC_ATTR_MAX_NUM];
};

struct ZodiacDecomposeCfg
{
	ZodiacDecomposeCfg() : zodiac_index(0), suipian_index(0), jinghua_num(0) {}

	int zodiac_index;
	int suipian_index;
	int jinghua_num;
};

class ZodiacConfig : public ConfigBase
{
public:
	ZodiacConfig();
	~ZodiacConfig();

	bool Init(const std::string &configname, std::string *err);

	const ZodiacOtherCfg & GetOtherCfg() const { return m_other_cfg; }
	const ZodiacRealIdCfg *GetRealIdCfg(ItemID item_id) const;
	const ZodiacActivateCfg *GetActivateCfg(int zodiac_index, int activate_num) const;
	const ZodiacLevelupCfg *GetLevelupCfg(int zodiac_index, int level) const;
	const ZodiacDecomposeCfg *GetDecomposeCfg(int zodiac_index, int suipian_index) const;

private:
	int InitOtherCfg(TiXmlElement *RootElement);
	int InitIdListCfg(TiXmlElement *RootElement);
	int InitActivateCfg(TiXmlElement *RootElement);
	int InitLevelupCfg(TiXmlElement *RootElement);
	int InitDecomposeCfg(TiXmlElement *RootElement);

	ZodiacOtherCfg m_other_cfg;

	typedef std::map<ItemID, ZodiacRealIdCfg> ZodiacRealIdCfgMap;
	typedef std::map<ItemID, ZodiacRealIdCfg>::const_iterator ZodiacRealIdCfgMapIt;
	ZodiacRealIdCfgMap m_zodiac_id_cfg_map;

	typedef std::map<long long, ZodiacActivateCfg> ZodiacActivateCfgMap;
	typedef std::map<long long, ZodiacActivateCfg>::const_iterator ZodiacActivateCfgMapIt;
	ZodiacActivateCfgMap m_activate_cfg_map;

	typedef std::map<long long, ZodiacLevelupCfg> ZodiacLevelupCfgMap;
	typedef std::map<long long, ZodiacLevelupCfg>::const_iterator ZodiacLevelupCfgMapIt;
	ZodiacLevelupCfgMap m_levelup_cfg_map;

	typedef std::map<long long, ZodiacDecomposeCfg> ZodiacDecomposeCfgMap;
	typedef std::map<long long, ZodiacDecomposeCfg>::const_iterator ZodiacDecomposeCfgMapIt;
	ZodiacDecomposeCfgMap m_decompose_cfg_map;
};

#endif