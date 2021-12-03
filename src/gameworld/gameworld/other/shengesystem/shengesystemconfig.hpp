#ifndef __SHENGE_SYSTEM_CONFIG_HPP__
#define __SHENGE_SYSTEM_CONFIG_HPP__

#include "servercommon/shengesystemdef.hpp"
#include "common/tinyxml/tinyxml.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "config/activityconfig/opengameactivityconfig.hpp"

static const int SHENGE_SYSTEM_CHOUJIANG_MAX_CFG_COUNTS = 100;							// 最大抽奖配置
static const int SHENGE_SYSTEM_FIRST_TEN_CHOUJIANG_MAX_CFG_COUNTS = 30;					// 十抽抽奖配置
static const int SHENGE_SYSTEM_COMBINATION_MAX_CFG_COUNTS = 5;							// 最大组合配置
static const int SHENGE_SYSTEM_COMBINATION_PERCEN_CFG = 3;								// 组合百分比属性加成 数量
static const int SHENGE_SYSTEM_COMBINATION_SPECIAL_EFFECT_CFG = 2;						// 组合额外属性数量
static const int SHENGE_SYSTEM_ITEM_ID_SHENGE_MAX_CFG = 150;							// 实物换虚拟神格配置数量
static const int SHENGE_SYSTEM_DROP_ID_MAX_CFG = 15;									// 掉落物品id配置数量
static const int SHENGE_SYSTEM_TEN_CHOU_MUST_GET_QUALITY = 2;							// 十抽必得品质
static const int SHENGE_SYSTEM_SUIT_CFG_MAX_COUNT = 10;									// 每种品质套装最大配置数量

enum SHENGE_SYSTEM_SHENGE_KIND
{
	SHENGE_SYSTEM_SHENGE_KIND_NORMAL = 0,												// 普通神格
	SHENGE_SYSTEM_SHENGE_KIND_UNLIMIT = 1,												// 终极神格

	SHENGE_SYSTEM_MAX_KINDS																// 最大种类
};

struct ShengeSystemOtherCfg
{
	ShengeSystemOtherCfg() : one_chou_need_gold(0), ten_chou_need_gold(0), free_choujiang_times(0), left_free_chou_cd_s(0),
		normal_shenge_type_max_count(0), ultimate_shenge_type_max_count(0), unactive_attr_add_percen(0), active_attr_add_percen(0), drop_item_cd_s(0), uplevel_zhangkong_itemid(0), uplevel_zhangkong_gold(0),
		replacement_id(0), consume_replacement(0), ten_chou_use_itemid(0), ten_chou_use_item_num(0), can_use_item(true)
	{}
	int one_chou_need_gold;
	int ten_chou_need_gold;
	int free_choujiang_times;
	int left_free_chou_cd_s;															// 免费抽奖间隔
	int normal_shenge_type_max_count;													// 同种普通神格最大装备数量
	int ultimate_shenge_type_max_count;													// 同种终极神格最大装备数量
	int unactive_attr_add_percen;														// 非活跃状态神格页的属性加成
	int active_attr_add_percen;															// 活跃状态神格页的属性加成
	int drop_item_cd_s;																	// 神格额外掉落冷却cd秒
	int uplevel_zhangkong_itemid;
	int uplevel_zhangkong_gold;

	int replacement_id;
	int consume_replacement;
	int ten_chou_use_itemid;
	int ten_chou_use_item_num;
	bool can_use_item;
};

struct ShengeSystemAttrSingelCfg
{
	ShengeSystemAttrSingelCfg() : attr_type(0), attr_value(0)
	{}

	int attr_type;
	Attribute attr_value;
};

struct ShengeSystemAttrLevelCfg
{
	static const int MAX_ATTR_COUNTS = 2;

	ShengeSystemAttrLevelCfg() : level(0), kind(0), next_level_need_marrow_score(0), return_score(0), capbility(0), quyu(0)
	{}

	int level;																			// 神格等级
	int kind;																			// 神格种类
	int next_level_need_marrow_score;
	int return_score;
	int capbility;
	int quyu;
	ShengeSystemAttrSingelCfg base_attr[MAX_ATTR_COUNTS];
};

struct ShengeSystemAttrtypeCfg
{

	ShengeSystemAttrtypeCfg() : type(0)
	{}

	int type;																			// 神格类型
	ShengeSystemAttrLevelCfg leve_cfg[SHENGE_SYSTEM_MAX_SHENGE_LEVEL];

};

struct ShengeSystemAttrCfg
{
	ShengeSystemAttrCfg() : quality(0)
	{}

	int quality;																		// 神格阶级
	ShengeSystemAttrtypeCfg type_cfg_list[SHENGE_TYPE_COUNT];
};

struct ShengeSystemDecomposeQualityCfg
{
	ShengeSystemDecomposeQualityCfg() : quality(0), composite_prob(0)
	{}

	int quality;																		// 神格阶级
	int composite_prob;
};

struct ShengeSystemComposeCfg
{
	ShengeSystemComposeCfg() : kind(0)
	{}

	int kind;																			// 神格种类
	ShengeSystemDecomposeQualityCfg quality_cfg_list[SHENGE_QUALITY_COUNT];
};

struct ShengeSystemChoujiagnCfg
{
	ShengeSystemChoujiagnCfg() : seq(0), weight(0), ten_weight(0), is_msg(0)
	{
	}

	int seq;
	int weight;
	int ten_weight;
	int is_msg;
	struct reward_item
	{
		reward_item() : quality(0), type(0)
		{}

		short quality;
		short type;
	};

	reward_item reward_shenge;
};

struct ShengeSystemCombinationCfg
{
	struct AddPerCenCfg
	{
		AddPerCenCfg() : attr_tyep(0), attr_value(0)
		{}

		short attr_tyep;
		short attr_value;
	};

	struct SpecialEffectCfg
	{
		SpecialEffectCfg() : ecffect_type(0), percen_value(0)
		{}

		short ecffect_type;
		short percen_value;
	};
	ShengeSystemCombinationCfg() : seq(0)
	{}

	int seq;
	int type[SHENGE_TYPE_COUNT];
	int quality[SHENGE_TYPE_COUNT];
	AddPerCenCfg  add_percen_cfg_list[SHENGE_SYSTEM_COMBINATION_PERCEN_CFG];
	SpecialEffectCfg special_cfg_list[SHENGE_SYSTEM_COMBINATION_SPECIAL_EFFECT_CFG];
};

struct ShengeSystemItemIdShengeCfg
{
	ShengeSystemItemIdShengeCfg() : item_id(0), quality(0), type(0)
	{}
	ItemID item_id;
	short quality;
	short type;
};

struct ShengeSystemDropIdCfg
{
	ShengeSystemDropIdCfg() : seq(0), weight(0), drop_id(0)
	{}

	int seq;
	int weight;
	int drop_id;
};

struct ShengeSystemPageCfg
{
	ShengeSystemPageCfg() : page(0), is_broadcast(0)
	{}

	short page;
	bool is_broadcast;
};

struct ZhangkongLevelCfg
{
	struct ConfigItem
	{
		ConfigItem() : grid(0), level(0), grade(0), star(0), exp(0), is_broadcast(0), gongji_pro(0), fangyu_pro(0), maxhp_pro(0), shanbi_pro(0), baoji_pro(0), kangbao_pro(0), mingzhong_pro(0), shenge_pro(0)
		{
		}

		int grid;
		int level;
		int grade;
		int star;
		int exp;
		int is_broadcast;

		Attribute gongji_pro;
		Attribute fangyu_pro;
		Attribute maxhp_pro;
		Attribute shanbi_pro;
		Attribute baoji_pro;
		Attribute kangbao_pro;
		Attribute mingzhong_pro;
		int shenge_pro;
	};

	ZhangkongLevelCfg() {}

	ConfigItem cfg_list[SHENGE_SYSTEM_ZHANGKONG_MAX_LEVEL + 1];
};

struct ZhangkongRandExpCfg
{
	ZhangkongRandExpCfg() : weight(0), exp(0){}

	int weight;
	int exp;
};

struct ZhangkongRandGradeCfg
{
	ZhangkongRandGradeCfg() : weight(0), grid(0){}

	int weight;
	int grid;
};

struct ShengeSystemShenquCfg
{
	struct StuffNumCfg
	{
		StuffNumCfg() : stuff_num(0) {}

		int stuff_num;
	};
	ShengeSystemShenquCfg() : shenqu_id(0), role_level(0), stuff_id(0), value_percent(0), fighting_capacity(0) {}

	int shenqu_id;
	int role_level;
	int stuff_id;
	int value_percent;
	int fighting_capacity;

	StuffNumCfg stuff_num_list[SHENGE_SYSTEM_SHENGESHENQU_XILIAN_SLOT_MAX_NUM];
};

struct ShengeSystemXilianAttrCfg
{
	struct AttrInfo
	{
		AttrInfo() : value_type(0), weight(0), value_max(0), value_min(0), criti_value(0) {}

		int value_type;
		int weight;
		int value_max;
		int value_min;
		int criti_value;
	};

	struct BaodiCfg
	{
		BaodiCfg() : baodi_count(0), criti_per(0) {}

		int baodi_count;
		int criti_per;
	};

	ShengeSystemXilianAttrCfg() : shenqu_id(0), point_type(0), total_weight(0) {}

	int shenqu_id;
	int point_type;
	int total_weight;
	BaodiCfg baodi_count_cfg[SHENGE_SYSTEM_XILIAN_BAODI_MAX_COUNT];
	AttrInfo attr_list[SHENGE_SYSTEM_SHENGESHENQU_ATTR_MAX_NUM];
};

struct UplevelGridCfg
{
	UplevelGridCfg() :index(0), stuff_id(0), quality_range(0), quality_range_2(0){}
	int index;
	ItemID stuff_id;
	int quality_range;	//聚星格子加成范围
	int quality_range_2;	//聚星格子加成范围
};

struct ShengeSystemNewSuitAttrCfg
{
	ShengeSystemNewSuitAttrCfg() : need_count(0), maxhp(0), fangyu(0), mingzhong(0), shanbi(0),
		baoji(0), jianren(0), per_hunshi(0), per_gongji(0), per_maxhp(0), per_fangyu(0), per_mianshang(0)
	{}

	int need_count;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	int per_hunshi;
	int per_gongji;
	int per_maxhp;
	int per_fangyu;
	Attribute per_mianshang;
};

struct ShengeSystemNewSuitCfg
{
	ShengeSystemNewSuitCfg() : suit_color(0), suit_count(0) {}

	int suit_color;
	int suit_count;
	ShengeSystemNewSuitAttrCfg attr[SHENGE_SYSTEM_SUIT_CFG_MAX_COUNT];
};

class ShengeSystemCfg : public ConfigBase
{
public:
	ShengeSystemCfg();
	~ShengeSystemCfg();

	bool Init(std::string configname, std::string *err);
	bool CheckSeq(int seq);
	const ShengeSystemOtherCfg & GetOterCfg() const { return m_other_cfg; }
	const ShengeSystemAttrLevelCfg * GetShengeSystemCfgByQualityTypeLevel(int quality, int type, int level) const;
	int GetShengeKind(int quality, int type, int level);
	const ShengeSystemDecomposeQualityCfg * GetcomposeCfgByKindAndQuality(int kind, int quality);
	int GetChoujiangTotalWeight() const { return m_choujiang_cfg_total_weight; }
	int GetChoujiangTotalTenWeight() const { return m_choujiang_cfg_total_ten_weight; }
	int GetChoujiangTotalTenSpecialWeight() const { return m_choujiang_cfg_total_ten_special_weight; }
	int GetChoujiangCfgCount() const { return m_choujiang_cfg_count; }
	const ShengeSystemChoujiagnCfg * GetChoujiangCfgBySeq(int seq) const;
	const ShengeSystemChoujiagnCfg * GetChoujiangCfgByQualityAndType(short quality, short type) const;
	int GetFirstTenChouJiangTotalWeight() const { return m_first_ten_chou_total_weight; }
	int GetFirstTenChouJiangCfgCount() const { return m_first_ten_chou_cfg_count; }
	const ShengeSystemChoujiagnCfg *  GetFirstTenChouJiangCfgBySeq(int seq) const;
	int GetCombinationCfgCount() const { return m_combination_cfg_count; }
	const ShengeSystemCombinationCfg * GetCombinationCfgBySeq(int seq) const;
	const ShengeSystemItemIdShengeCfg * GetItemIdShengeCfgByItemId(ItemID item_id) const;
	int GetDropIdCfgCount() const { return m_drop_id_list_count; }
	int GetDropIdCfgTotalWeight() const { return m_drop_id_total_weight; }
	const ShengeSystemDropIdCfg * GetDropIdCfgBySeq(int seq) const;
	int IsBroadcast(int level) { return static_cast<int>(m_is_broadcast_map.count(level)); }
	const ShengeSystemPageCfg GetShengePageCfg(int level);
	int InitZhangkongLevelCfg(PugiXmlNode RootElement);
	ZhangkongLevelCfg::ConfigItem * GetZhangkongCfgByGridAndLevel(int grid, int level);
	int GetZhangkongRandExp();
	int GetZhangkongRandGrid();
	int GetZhangkongGridRandWeight(int grid_index);

	const ShengeSystemShenquCfg *GetShenquCfg(int shenqu_id);
	const ShengeSystemXilianAttrCfg *GetXilianCfg(int shenqu_id, int point_type);
	const UplevelGridCfg * GetUplevelGridCfg(int index) const;

	const ShengeSystemNewSuitAttrCfg * GetSuitCfg(int color, int count) const;

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitShengeCfg(PugiXmlNode RootElement);
	int InitDecomposeCfg(PugiXmlNode RootElement);
	int InitChouJiangCfg(PugiXmlNode RootElement);
	int InitFirstTenChouJiangCfg(PugiXmlNode RootElement);
	int InitCombinationCfg(PugiXmlNode RootElement);
	int InitItemIdShengeCfg(PugiXmlNode RootElement);
	int InitDropIdCfg(PugiXmlNode RootElement);
	int InitBroadcastCfg(PugiXmlNode RootElement);
	int InitShengePageCfg(PugiXmlNode RootElement);
	int InitZhangkongRandExpCfg(PugiXmlNode RootElement);
	int InitZhangkongRandGridCfg(PugiXmlNode RootElement);
	int InitShengeShenquCfg(PugiXmlNode RootElement);
	int InitShengeShenquXilianCfg(PugiXmlNode RootElement);
	int InitUplevelGridCfg(PugiXmlNode RootElement);
	int InitNewSuitCfg(PugiXmlNode RootElement);

	ShengeSystemOtherCfg m_other_cfg;

	int m_attr_cfg_count;
	ShengeSystemAttrCfg m_attr_cfg_list[SHENGE_QUALITY_COUNT];												// 神格等级配置

	int m_compose_cfg_count;
	ShengeSystemComposeCfg m_compose_cfg_list[SHENGE_SYSTEM_MAX_KINDS];										// 神格合成配置 

	int m_choujiang_cfg_count;
	int m_choujiang_cfg_total_weight;
	int m_choujiang_cfg_total_ten_weight;
	int m_choujiang_cfg_total_ten_special_weight;
	ShengeSystemChoujiagnCfg m_choujiang_cfg_list[SHENGE_SYSTEM_CHOUJIANG_MAX_CFG_COUNTS];					// 抽奖配置

	int m_first_ten_chou_cfg_count;
	int m_first_ten_chou_total_weight;
	ShengeSystemChoujiagnCfg m_first_ten_chou_cfg_list[SHENGE_SYSTEM_FIRST_TEN_CHOUJIANG_MAX_CFG_COUNTS];	// 第一次抽奖配置

	int m_combination_cfg_count;
	ShengeSystemCombinationCfg m_combination_cfg_list[SHENGE_SYSTEM_COMBINATION_MAX_CFG_COUNTS];			// 组合配置

	int m_item_id_Shenge_cfg_count;
	ShengeSystemItemIdShengeCfg m_item_id_Shenge_cfg_list[SHENGE_SYSTEM_ITEM_ID_SHENGE_MAX_CFG];			// 实物id 虚拟神格对照配置

	int m_drop_id_list_count;
	int m_drop_id_total_weight;
	ShengeSystemDropIdCfg m_drop_id_list[SHENGE_SYSTEM_DROP_ID_MAX_CFG];

	ShengeSystemPageCfg m_page_broadcast_list[SHENGE_SYSTEM_MAX_SHENGE_PAGE];
	std::map<int, bool> m_is_broadcast_map;
	std::map<int, ShengeSystemPageCfg> m_page_map;
	
	ZhangkongLevelCfg m_zhangkong_level_cfg[SHENGE_SYSTEM_QUALITY_MAX_VALUE];

	int m_zhangkong_rand_exp_all_weight;
	ZhangkongRandExpCfg m_zhangkong_rand_exp_cfg_list[SHENGE_SYSTEM_ZHANGKONG_RAND_EXP_COUNT];

	int m_zhangkong_rand_grade_all_weight;
	ZhangkongRandGradeCfg m_zhangkong_rand_grade_cfg_list[SHENGE_SYSTEM_QUALITY_MAX_VALUE];

	std::vector<ShengeSystemShenquCfg> m_shenge_shenqu_cfg;
	std::vector<ShengeSystemXilianAttrCfg> m_shenge_shenqu_xilian_cfg;

	std::vector<UplevelGridCfg> m_shenge_uplevel_grid_cfg;

	ShengeSystemNewSuitCfg m_suit_list[SHENGE_QUALITY_COUNT];
};

#endif