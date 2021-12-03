#ifndef __MYTH_CONFIG_HPP__
#define __MYTH_CONFIG_HPP__

#include "servercommon/configbase.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configcommon.h"
#include "servercommon/mythdef.hpp"
#include <vector>
#include <map>
#include <set>

class TiXmlElement;

struct MythChapterLevelConfig
{
	MythChapterLevelConfig() : level(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), pofang_per(0), mianshang_per(0), is_broadcast(false) {}

	int level;
	ItemConfigData stuff_id1;
	//ItemConfigData stuff_id2;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	Attribute pofang_per;
	Attribute mianshang_per;

	bool is_broadcast;
};

struct MythChapterConfig
{
	MythChapterConfig() : chapter_id(0), max_level(0) {}

	int chapter_id;
	int max_level;

	MythChapterLevelConfig level_list[MAX_MYTH_CHPATER_LEVEL + 1];
};

struct MythResonanceLevelConfig
{
	MythResonanceLevelConfig(): level(0), weight_total(0)
	{
		memset(position_soul_list, 0, sizeof(position_soul_list));
		memset(soul_weight_list, 0, sizeof(soul_weight_list));
	}

	struct ResonanceItem
	{
		ResonanceItem() : attr_type(0), attr_val(0) {}

		int attr_type;
		Attribute attr_val;
	};

	int level;
	ItemConfigData stuff;
	int position_soul_list[MAX_MYTH_SOUL_SLOT];

	int weight_total;
	int soul_weight_list[MAX_MYTH_SOUL_TYPE + 1];
	ResonanceItem resonance_attr_list[MAX_MYTH_RESONANCE_ATTR_NUM];
};

struct MythResonanceConfig
{
	MythResonanceConfig() : max_level(0), chpater_id(0) {}

	int max_level;
	int chpater_id;
	MythResonanceLevelConfig level_list[MAX_MYTH_RESONANCE_LEVEL + 1];
};

struct MytyDecomposeConfig
{
	MytyDecomposeConfig() : item_id(0), gain_soul_essence(0) {}

	int item_id;
	int gain_soul_essence;
};

struct DigestionLevelConfig
{
	DigestionLevelConfig() : single_essence(0), need_essence(0){}

	int single_essence;
	int need_essence;
	AttrConfig attr;
};

struct MythDigestionConfig
{
	MythDigestionConfig() : max_level(0), chpater_id(0) {}

	int max_level;
	int chpater_id;

	DigestionLevelConfig level_list[MAX_MYTH_DIGESTION_LEVEL + 1];
};

struct MythBaoJIConfig
{
	MythBaoJIConfig() : baoji_multiple(0), baoji_weight(0) {}

	int baoji_multiple;
	int baoji_weight;
};

struct MythSoulGodConfig
{
	MythSoulGodConfig() : soul_type(0), quality(0), item_id(0) {}

	int soul_type;
	int quality;
	ItemID item_id;
	AttrConfig attr;
};

struct RandStarConfig
{
	RandStarConfig() : rand_attr_num(0), star_attr_max_num(0), weight_total(0)
	{
		memset(star_weight_list, 0, sizeof(star_weight_list));
	}

	int rand_attr_num;
	int star_attr_max_num;
	int weight_total;
	int star_weight_list[MAX_MYTH_STAR_SOUL + 1];
};

struct SoulRandAttrConfig
{
	SoulRandAttrConfig() : attr_type(0), attr_val(0) {}

	int attr_type;
	Attribute attr_val;
};

struct SoulAttrConfig
{
	SoulAttrConfig() : quality(0), normal_count(0), rare_count(0)
	{

	}

	int quality;
	int normal_count;
	int rare_count;
	SoulRandAttrConfig normal_list[MAX_MYTH_RAND_ATTR_NORMAL_COUNT];
	SoulRandAttrConfig rare_list[MAX_MYTH_RAND_ATTR_RARE_COUNT];
};

struct MythComposeCfg
{
	MythComposeCfg() :  before_item_id(0), after_item_id(0), give_start_num(0), need_start_num(0),
		is_need_item(false), is_broatcast(false), consumeitem_id(0), item_num(0), is_broadcast(0)
	{}

	ItemID before_item_id;
	ItemID after_item_id;
	int give_start_num;
	int need_start_num;

	bool is_need_item;
	bool is_broatcast;
	ItemID consumeitem_id;
	int item_num;
	bool is_broadcast;
};

struct MythResonanceOpenLimitItem
{
	MythResonanceOpenLimitItem():chpater_level(0), resonance_level(0)
	{}

	int chpater_level;
	int resonance_level;
};

struct MythResonanceOpenLimitCfg
{
	MythResonanceOpenLimitCfg() : chpater_id(-1)
	{}

	int chpater_id;
	MythResonanceOpenLimitItem level_open_limit[MAX_MYTH_RESONANCE_LEVEL + 1];
};

class MythConfig : public ConfigBase
{
public:
	MythConfig();
	~MythConfig();

	bool Init(const std::string &configname, std::string *err);
	
	const MythChapterLevelConfig * GetChpaterLevelCfg(int chpater_id, int level);
	const MythResonanceLevelConfig * GetResonanceCfg(int chpater_id, int level);
	const DigestionLevelConfig * GetDigestionCfg(int chpater_id, int level);
	const MythSoulGodConfig *GetSoulGodCfgByItem(ItemID item_id);
	const SoulAttrConfig *GetSoulAttrCfg(int quality);
	const RandStarConfig *GetStarCfg(int quality);

	const MythComposeCfg *GetComposeCfg(int seq);
	int GetDecomposeSoulEssence(ItemID item_id);
	int GetBaoJiMultiple();

	void GetRandStarCount(int quality, int &attr_count, int &star_count);

	const SoulRandAttrConfig* GetRandomStarAttr(int quality, std::set<int> &attr_list);
	const SoulRandAttrConfig* GetRandomNormalAttr(int quality, std::set<int> &attr_list);

	bool CheckAttrStarCount(const MythSoulGodItem &tmp_item, int star_count, int quality);

	int GetAttrStarCount(const MythSoulGodItem &tmp_item, int quality);
	const MythResonanceOpenLimitItem * GetResonanceOpenLimitCfg(int chpater_id, int level);

private:
	int InitChpaterLevelConfig(TiXmlElement *RootElement);
	int InitResonanceConfig(TiXmlElement *RootElement);
	int InitDecomposeConfig(TiXmlElement *RootElement);
	int InitDigestionConfig(TiXmlElement *RootElement);
	int InitBaojiConfig(TiXmlElement *RootElement);
	int InitSoulConfig(TiXmlElement *RootElement);
	int InitRandStarConfig(TiXmlElement *RootElement);
	int InitRandAttrConfig(TiXmlElement *RootElement);
	int InitComposeConfig(TiXmlElement *RootElement);
	int InitResonanceOpenLimitConfig(TiXmlElement *RootElement);

	MythChapterConfig m_chpater_list[MAX_MYTH_CHAPTER_ID + 1];
	MythResonanceConfig m_resonance_list[MAX_MYTH_CHAPTER_ID + 1];
	MythDigestionConfig m_digestion_list[MAX_MYTH_CHAPTER_ID + 1];
	RandStarConfig m_soul_star_cfg_list[MAX_MYTH_SOUL_QUALITY + 1];

	SoulAttrConfig m_soul_attr_list[MAX_MYTH_SOUL_QUALITY + 1];

	int baoji_total_weight;
	std::vector<MythBaoJIConfig> m_baoji_vec;
	std::map<ItemID, int> m_decompose_map;
	std::map<int, MythComposeCfg> m_compose_map;
	std::map<ItemID, MythSoulGodConfig> m_soul_god_map;
	
	MythResonanceOpenLimitCfg m_resonance_open_limit_list[MAX_MYTH_CHAPTER_ID + 1];
};

#endif //__MYTH_CONFIG_HPP__

