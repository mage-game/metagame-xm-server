#ifndef __RUNE_SYSTEM_CONFIG_HPP__
#define __RUNE_SYSTEM_CONFIG_HPP__

#include "common/tinyxml/tinyxml.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include <vector>
#include <map>
#include <set>
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "servercommon/runesystemdef.hpp"

const static int RUNESYSTEM_AWAKEN_LIMIT_MAX_COUNT = 20; 
const static int RUNESYSTEM_AWAKEN_COST_MAX_COUNT = 20;
static const int RUNESYSTEM_ZHULING_MAX_SLOT_CFG = 10;

struct RuneSystemOtherCfg
{
	RuneSystemOtherCfg() : free_cd(0), xunbao_consume_itemid(0), xunbao_one_consume_num(0), xunbao_ten_consume_num(0),
		rune_suipian_num_low(0), rune_suipian_num_high(0), rune_compose_need_layer(0), xunbao_one_magic_crystal(0),
		jinghua_box_min(0), jinghua_box_max(0), rune_box_min(0), rune_box_max(0),
		jinghua_box_magic_crystal(0), fuwen_box_magic_crystal(0), first_xubao_rune_id(0),
		awaken_baodi(0), zhuling_slot_max_value(0), zhuling_cost(0), rune_awake_need_layer(0), rune_lianhun_need_layer(0),
		total_x_times(0), total_x1_times(0)
	{}

	int free_cd;
	ItemID xunbao_consume_itemid;
	int xunbao_one_consume_num;
	int xunbao_ten_consume_num;
	int rune_suipian_num_low;
	int rune_suipian_num_high;
	int rune_compose_need_layer;
	int xunbao_one_magic_crystal;
	int jinghua_box_min;
	int jinghua_box_max;
	int rune_box_min;
	int rune_box_max;
	int jinghua_box_magic_crystal;
	int fuwen_box_magic_crystal;
	ItemID first_xubao_rune_id;
	
	int awaken_baodi;

	int zhuling_slot_max_value;
	int zhuling_cost;
	int rune_awake_need_layer;
	int rune_lianhun_need_layer;

	int total_x_times;
	int total_x1_times;
};

struct RuneSystemSlotOpenCfg
{
	RuneSystemSlotOpenCfg() : open_rune_slot(0), need_pass_layer(0) {}

	int open_rune_slot;
	int need_pass_layer;
};

struct RuneSystemRuneFetchCfg
{
	RuneSystemRuneFetchCfg() : rune_id(0), seq(0), in_layer_open(0), one_weight(0), ten_weight(0), is_red_rune(0), red_rune_weight(0),
		total_x_times_weight(0), total_x1_times_weight(0), convert_consume_rune_suipian(0) {}

	ItemID rune_id;
	int seq;
	int in_layer_open;
	int one_weight;
	int ten_weight;
	int is_red_rune;
	int red_rune_weight;
	int total_x_times_weight;
	int total_x1_times_weight;
	int convert_consume_rune_suipian;
};

struct RuneSystemAttrCfg
{
	RuneSystemAttrCfg() : quality(0), types(0), level(0), uplevel_need_jinghua(0), attr_type_0(0), add_attributes_0(0), 
		attr_type_1(0), add_attributes_1(0), dispose_fetch_jinghua(0), other_capability(0), broadcast(false) {}

	int quality;
	int types;
	int level;
	int uplevel_need_jinghua;
	int attr_type_0;
	Attribute add_attributes_0;
	int attr_type_1;
	Attribute add_attributes_1;
	int dispose_fetch_jinghua;
	int other_capability;

	bool broadcast;
};

struct RuneSystemRealIdCfg
{
	RuneSystemRealIdCfg() : rune_id(0), quality(0), type(0), is_notice(false) {}

	ItemID rune_id;
	int quality;
	int type;
	bool is_notice;
};

struct RuneSystemComposeCfg
{
	RuneSystemComposeCfg() : rune1_id(0), rune2_id(0), magic_crystal_num(0), get_rune_id(0) {}

	ItemID rune1_id;
	ItemID rune2_id;
	int magic_crystal_num;
	ItemID get_rune_id;
};

struct RuneSystemOpenBoxCfg
{
	RuneSystemOpenBoxCfg() : rune_id(0), in_layer_open(0), weight_rune_box(0), weight_jinghua_box(0) {}

	ItemID rune_id;
	int in_layer_open;
	int weight_rune_box;
	int weight_jinghua_box;
};

enum RUNE_SYSTEM_AWAKEN_REWARD_TYPE
{
	RUEN_AWAKEN_REWARD_TYPE_INVALID = 0,
	
	RUEN_AWAKEN_REWARD_TYPE_GONGJI,
	RUEN_AWAKEN_REWARD_TYPE_FANGYU,
	RUEN_AWAKEN_REWARD_TYPE_MAXHP,
	RUEN_AWAKEN_REWARD_TYPE_ADD_PER,
	RUEN_AWAKEN_REWARD_TYPE_JINGHUA,
	RUEN_AWAKEN_REWARD_TYPE_MOJING,
	RUEN_AWAKEN_REWARD_TYPE_EXP,
	
	RUEN_AWAKEN_REWARD_TYPE_MAX,
};

struct RuneSystemAwakenCfg
{
	RuneSystemAwakenCfg(): awaken_seq(0), awaken_type(0), add_value(0), common_pro(0), diamond_pro(0)
	{}

	int awaken_seq;
	int awaken_type;
	int add_value;
	int common_pro;
	int diamond_pro;
};

struct RuneSystemAwakenLimitCfg
{
	RuneSystemAwakenLimitCfg(): min_level(0), max_level(0), gongji_limit(0), fangyu_limit(0), maxhp_limit(0), addper_limit(0)
	{}

	int min_level;
	int max_level;
	int gongji_limit;
	int fangyu_limit;
	int maxhp_limit;
	int addper_limit;
};

struct RuneAwakenCost
{
	RuneAwakenCost(): min_times(0), max_times(0), gold_cost(0)
	{}

	int min_times;
	int max_times;
	int gold_cost;
	ItemConfigData common_awaken_item;
};

struct RuneZhulingGradeConfig
{
	RuneZhulingGradeConfig() : fuwen_index(0), grade(0), up_grade_need_bless(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), 
		special_add(0), is_broadcast(0)
	{
	}

	int fuwen_index;
	int grade;
	int up_grade_need_bless;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	int special_add;
	int is_broadcast;
};

struct RuneZhulingSlotConfig
{
	enum TYPE
	{
		TYPE_FIX,
		TYPE_PERCENT,
		TYPE_UPSTAR,
	};

	RuneZhulingSlotConfig() : weight(0), type(0), param1(0)
	{
	}

	short weight;
	short type;
	int param1;
};

struct RuneLevelOpenCfg
{
	RuneLevelOpenCfg() : rune_level(0), need_rune_tower_layer(0) {}

	int rune_level;
	int need_rune_tower_layer;
};

class RuneSystemCfg : public ConfigBase
{
public:
	RuneSystemCfg();
	~RuneSystemCfg();

	bool Init(const std::string &configname, std::string *err);
	const RuneSystemOtherCfg & GetOterCfg() const { return m_other_cfg; }
	const RuneSystemSlotOpenCfg * GetSlotOpenCfg(int pass_layer);
	const RuneSystemRuneFetchCfg * GetRuneIdCfg(ItemID rune_id);
	const RuneSystemAttrCfg * GetAttrCfg(int quality, int type, int level);
	const RuneSystemRealIdCfg * GetRealIdCfg(ItemID rune_id);
	const RuneSystemComposeCfg * GetComposeCfg(ItemID rune1_id, ItemID rune2_id);

	int GetLevelConsumeTotalJinghua(int quality, int type, int level);
	int GetRuneIdByQualityAndTypeCfg(int quality, int type);
	int GetRuneSlotNum() { return (int)m_slot_open_cfg_vec.size(); }
	void GetOpenRuneId(int pass_layer, std::set<ItemID> &rune_id_set);
	void GetRuneFetchWeightCfg(int pass_layer, std::vector<RuneSystemRuneFetchCfg> &rune_id_weight_vec);
	ItemID GetOpenBoxGetItemId(int box_type, int level_limit);
	ItemID GetFirstXunbaoRuneId(){return m_other_cfg.first_xubao_rune_id;}

	std::map<int ,RuneSystemAwakenCfg> & GetAwakenCfgMap() {return m_rune_awaken_map;};
	RuneSystemAwakenLimitCfg * GetAwakenLimitByLevel(int level);
	RuneAwakenCost * GetAwakenCostByTimes(int times);

	const RuneZhulingGradeConfig * GetZhulingGradeCfg(int fuwen_index, int grade) const;
	int GetZhulingSlotCfgTotalWeight() const { return m_zhuling_slot_cfg_total_weight; }
	int GetZhulingSlotCfgCount() const { return m_zhuling_slot_cfg_count; }
	const RuneZhulingSlotConfig *	GetZhulingSlotCfg(int index) const;
	const RuneLevelOpenCfg * GetRuneLevelOpenCfg(int rune_level) const;

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitSlotOpenCfg(PugiXmlNode RootElement);
	int InitFetchCfg(PugiXmlNode RootElement);
	int InitAttrCfg(PugiXmlNode RootElement);
	int InitIdListCfg(PugiXmlNode RootElement);
	int InitComposeCfg(PugiXmlNode RootElement);
	int InitOpenBoxCfg(PugiXmlNode RootElement);
	int InitAwakenCfg(PugiXmlNode RootElement);
	int InitAwakenLimitCfg(PugiXmlNode RootElement);
	int InitAwakenCostCfg(PugiXmlNode RootElement);
	int InitZhulingGradeCfg(PugiXmlNode RootElement);
	int InitZhulingSlotCfg(PugiXmlNode RootElement);
	int InitRuneLevelOpenCfg(PugiXmlNode RootElement);

	RuneSystemOtherCfg m_other_cfg;

	std::vector<RuneSystemSlotOpenCfg> m_slot_open_cfg_vec;

	typedef std::map<ItemID, RuneSystemRuneFetchCfg> RuneSystemRuneFetchCfgMap;
	typedef std::map<ItemID, RuneSystemRuneFetchCfg>::const_iterator RuneSystemRuneFetchCfgMapIt;
	RuneSystemRuneFetchCfgMap m_rune_id_cfg_map;

	std::vector<RuneSystemRuneFetchCfg> m_rune_fetch_weight_vec;					//符文获取权重配置

	typedef std::map<long long, RuneSystemAttrCfg> RuneSystemAttrCfgMap;
	typedef std::map<long long, RuneSystemAttrCfg>::const_iterator RuneSystemAttrCfgMapIt;
	RuneSystemAttrCfgMap m_attr_cfg_map;

	typedef std::map<ItemID, RuneSystemRealIdCfg> RuneSystemRealIdCfgMap;
	typedef std::map<ItemID, RuneSystemRealIdCfg>::const_iterator RuneSystemRealIdCfgMapIt;
	RuneSystemRealIdCfgMap m_real_id_cfg_map;

	typedef std::map<long long, RuneSystemComposeCfg> RuneSystemComposeCfgMap;
	typedef std::map<long long, RuneSystemComposeCfg>::const_iterator RuneSystemComposeCfgMapIt;
	RuneSystemComposeCfgMap m_compose_cfg_map;

	typedef std::map<ItemID, RuneSystemOpenBoxCfg> RuneSystemOpenBoxCfgMap;
	typedef std::map<ItemID, RuneSystemOpenBoxCfg>::const_iterator RuneSystemOpenBoxCfgMapIt;
	RuneSystemOpenBoxCfgMap m_open_box_cfg_map;

	std::map<int ,RuneSystemAwakenCfg> m_rune_awaken_map;

	int m_rune_awaken_count;
	RuneSystemAwakenLimitCfg m_rune_awaken_limit_list[RUNESYSTEM_AWAKEN_LIMIT_MAX_COUNT];

	int m_rune_awaken_cost_count;
	RuneAwakenCost m_rune_awaken_cost_list[RUNESYSTEM_AWAKEN_COST_MAX_COUNT];

	std::vector<RuneZhulingGradeConfig> m_zhuling_grade_cfg_list;// 注灵配置

	int m_zhuling_slot_cfg_total_weight;
	int m_zhuling_slot_cfg_count;
	RuneZhulingSlotConfig m_zhuling_slot_cfg_list[RUNESYSTEM_ZHULING_MAX_SLOT_CFG];

	int m_rune_level_open_cfg_count;
	RuneLevelOpenCfg rune_level_open_cfg_list[RUNE_SYSTEM_RUNE_MAX_LEVEL];
};

#endif