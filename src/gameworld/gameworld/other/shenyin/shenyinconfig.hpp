#ifndef __SHEN_YIN_CONFIG_HPP__
#define __SHEN_YIN_CONFIG_HPP__

#include <servercommon/configcommon.h>
#include "servercommon/shenyindef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <servercommon/configcommon.h>
#include <vector>

static const int SHEN_YIN_MAX_COMBINE_CFG = 30;			// 组合最大数量
static const int SHEN_YIN_COMBINE_MAX_BEADS = 15;			// 单个组合最大的珠子数
static const int SHEN_YIN_LEVEL_LIMIT_MAX_CFG = 10;		// 等级限制配置个数
static const int PASTURE_SPIRIT_ITEM_LIST_MAX_CFG_COUNT = 100;
//static const int PASTURE_SPIRIT_IMPRINT_BASE_ATTR_MAX_CFG_COUNT = 7;		//改成用SHEN_YIN_LIEMING_CHOUHUN_COLOR_MAX代替
static const int PASTURE_SPIRIT_IMPRINT_UP_START_MAX_CFG_COUNT = 15;
static const int PASTURE_SPIRIT_IMPRINT_EXTRA_ATTR_COUNT_MAX_CFG_COUNT = 5;
static const int PASTURE_SPIRIT_IMPRINT_RECYCLE_MAX_CFG_COUNT = 20;
static const int PASTURE_SPIRIT_IMPRINT_SUIT_MAX_CFG_COUNT = 11;
static const int SHEN_YIN_LIEMING_HUNSHOU_PER_COLOR_MAX_COUNT = 64;
static const int SHEN_YIN_SUIT_CFG_MAX_COUNT = 10;			//每种品质套装最大配置数量

struct ShenYinOtherCfg
{
	ShenYinOtherCfg(): limit_level(0), change_bead_type_need_gold(0), flush_attr_type_need_v_item_id(0), flush_attr_type_need_v_item_num(0),
		flush_attr_value_need_v_item_id(0),flush_attr_value_need_v_item_num(0), add_attr_type_need_v_item_id(0), super_chouhun_price(0), lock_consume(0)
	{
	}

	int limit_level;					//限制等级
	int change_bead_type_need_gold;		//改变珠子的颜色需要的元宝
	short flush_attr_type_need_v_item_id;
	short flush_attr_type_need_v_item_num;
	short flush_attr_value_need_v_item_id;
	short flush_attr_value_need_v_item_num;
	short add_attr_type_need_v_item_id;
	int super_chouhun_price;
	int lock_consume;
};

struct ShenYinBeadCfg
{
	ShenYinBeadCfg(): type(0)
	{
	}

	int type;							// 珠子颜色
	AttrConfig attr;
};

struct ShenYinCombineAttrCfg
{
	ShenYinCombineAttrCfg() : seq(0)
	{
	}

	int seq;							// 组合索引
	AttrConfig attr;
};

struct ShenYinCombineItem
{
	ShenYinCombineItem() : type(0), x(0), y(0)
	{
	}

	int type;							// 珠子颜色
	short x;
	short y;
};

struct ShenYinCombineSeqCfg
{
	ShenYinCombineSeqCfg(): seq(0)
	{
	}

	int seq;							// 组合索引
	ShenYinCombineItem bead_list[SHEN_YIN_COMBINE_MAX_BEADS];
};

struct ShenYinLevelLimitCfg
{
	ShenYinLevelLimitCfg(): limit_level(0), bead_num(0)
	{
	}

	int limit_level;
	int bead_num;
};

struct ShenYinPastureSpiritItemCfg
{
	enum ITEM_TYPE
	{
		ITEM,
		EQUIPMENT,
		ADD_SCORE,
	};

	ShenYinPastureSpiritItemCfg() : v_item_id(0), item_id(0), item_type(0), stack_num(0), quanlity(0), imprint_slot(0), suit_id(0), return_item_id(0), return_item_num(0)
	{
	}

	int v_item_id;
	int item_id;
	int item_type;
	int stack_num;
	int quanlity;
	int imprint_slot;
	int suit_id;
	int return_item_id;
	int return_item_num;
};

struct ShenYinPastureSpiritImprintBaseAttrCfg
{
	ShenYinPastureSpiritImprintBaseAttrCfg() : quality(0) {}
	int quality;

	struct SlotParam
	{
		SlotParam() : slot_type(0)
		{
		}

		int slot_type;
		AttrConfig attr;
	};

	SlotParam slot_list[SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT];
};

struct ShenYinPastureSpiritImprintUpStartCfg
{
	static const int MAX_LEVEL = 10;
	ShenYinPastureSpiritImprintUpStartCfg() : slot_type(0), grade(0), level(0), consume_v_item_id(0), need_num(0), rate(0), 
		protect_v_item_id(0), need_protect_num(0), basics_addition(0)
	{
	}

	int slot_type;
	int grade;
	int level;
	short consume_v_item_id;
	short need_num;
	int rate;
	short protect_v_item_id;
	short need_protect_num;
	int basics_addition;
	AttrConfig attr_cfg;
};

struct ShenYinPastureSpiritImprintExtraAttrCountCfg
{
	ShenYinPastureSpiritImprintExtraAttrCountCfg() : attr_count(0), weight(0) {}

	int attr_count;
	int weight;
};

struct ShenYinPastureSpiritImprintExtraAttrCfg
{
	struct BaodiCfg
	{
		BaodiCfg() : baodi_count(0), criti_per(0) {}

		int baodi_count;
		int criti_per;
	};

	struct Result
	{
		Result() : type(0), value(0)
		{
		}

		int type;
		int value;
	};
	ShenYinPastureSpiritImprintExtraAttrCfg() : slot_type(0), attr_type(0), min_value(0), max_value(0) {}

	int slot_type;
	int attr_type;
	int min_value;
	int max_value;
	BaodiCfg baodi_count_cfg[SHEN_YIN_XILIAN_BAODI_MAX_COUNT];
};

struct ShenYinPastureSpiritImprintRecycleCfg
{
	ShenYinPastureSpiritImprintRecycleCfg() : quanlity(0), add_imprint_score(0), is_suit(false)
	{
	}

	int quanlity;
	int add_imprint_score;
	bool is_suit;
};

struct ShenYinPastureSpiritImprintExchangeCfg
{
	ShenYinPastureSpiritImprintExchangeCfg() : index(0), v_item_id(0), item_num(0), is_bind(false), is_virtual_item(false), imprint_score(0), buy_count(0)
	{
	}

	int index;
	int v_item_id;
	int item_num;
	bool is_bind;
	bool is_virtual_item;
	int imprint_score;
	int buy_count;
	int is_week_refresh;
	int day;
};

struct ShenYinPastureSpiritImprintSuitCfg
{
	struct SuitAttr
	{
		SuitAttr() : count(0)
		{
		}

		int count;
		AttrConfig attr;
	};
	ShenYinPastureSpiritImprintSuitCfg() : suit_id(0), cfg_count(0)
	{
	}

	short suit_id;
	short cfg_count;
	SuitAttr suit_list[SHEN_YIN_PASTURE_SPIRIT_MAX_IMPRINT_SLOT];
};

struct ShenYinLieMingItemConfig
{
	ShenYinLieMingItemConfig() : item_index(0), slot_type(0), shenyin_color(0), chouhun_weight(0), virtual_item_id(0) {}

	int item_index;
	int slot_type;
	int shenyin_color;
	int chouhun_weight;
	int virtual_item_id;
	int is_broadcast;
};

struct ShenYinLieMingChouhunItemWeightCfg
{
	struct ConfigItem
	{
		ConfigItem() : index(0), weight(0) {}

		int index;
		int weight;
	};

	ShenYinLieMingChouhunItemWeightCfg() : total_weight(0), cfg_count(0) {}

	int total_weight;

	int cfg_count;
	ConfigItem cfg_list[SHEN_YIN_LIEMING_HUNSHOU_PER_COLOR_MAX_COUNT];

};

struct ShenYinLieMingChouhunConfig
{
	ShenYinLieMingChouhunConfig() : chouhun_color(0), cost_gold(0), advance_rate(0), chouhun_score(0)
	{
		memset(rate_list, 0, sizeof(rate_list));
	}

	int chouhun_color;
	int cost_gold;
	int advance_rate;

	int rate_list[SHEN_YIN_LIEMING_CHOUHUN_COLOR_MAX];
	int chouhun_score;
};

struct ShenYinWashAttr
{
	ShenYinWashAttr() : attr_id(0), weight(0) {}

	int attr_id;
	int weight;
};

struct ShenYinWashAddCount
{
	ShenYinWashAddCount() : add_count(0), consume_num(0), rate(0) {}

	int add_count;
	int consume_num;
	int rate;
};

struct ShenYinCombineAttrAddCfg
{
	ShenYinCombineAttrAddCfg() : combine_seq_1(0), combine_seq_2(0), attr_add(0) {}

	int combine_seq_1;
	int combine_seq_2;
	int attr_add;
};

struct ShenYinNewSuitAttrCfg
{
	ShenYinNewSuitAttrCfg() : need_count(0), maxhp(0), fangyu(0), mingzhong(0), shanbi(0),
		baoji(0), jianren(0), per_gongji(0), per_maxhp(0), per_fangyu(0), skill_jianshang_per(0)
	{}

	int need_count;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	int per_gongji;
	int per_maxhp;
	int per_fangyu;
	Attribute skill_jianshang_per;
};

struct ShenYinNewSuitCfg
{
	ShenYinNewSuitCfg() : suit_color(0), suit_count(0){}

	int suit_color;
	int suit_count;
	ShenYinNewSuitAttrCfg attr[SHEN_YIN_SUIT_CFG_MAX_COUNT];
};

//struct ShenYinLockConsumeCfg
//{
//	ShenYinLockConsumeCfg() :xilian_type(0), lock_num(0){}
//
//	int xilian_type;
//	int lock_num;
//	ItemConfigData lock_comsume_item;
//};

class ShenYinConfig : public ConfigBase
{
public:
	ShenYinConfig();
	~ShenYinConfig();

	bool Init(const std::string &configname, std::string *err);

	const ShenYinOtherCfg & GetOtherCfg() const { return m_other_cfg; }
	const ShenYinBeadCfg * GetBeadCfg(int type) const;
	int GetCombineSeqCfgCount() const { return m_bead_combine_cfg_count; }
	const ShenYinCombineSeqCfg * GetCombineSeqCfg(int seq) const;
	const ShenYinCombineAttrCfg * GetCombineAttrCfg(int seq) const;
	const ShenYinLevelLimitCfg * GetLevelLimitCfg(int level) const;

	const ShenYinPastureSpiritItemCfg * GetItemByVid(int v_item_id) const;
	const ShenYinPastureSpiritItemCfg * GetItemByid(ItemID item_id) const;
	const ItemID GetItemIdByVid(int v_item_id) const;
	const ShenYinPastureSpiritImprintBaseAttrCfg::SlotParam * GetImprintBaseAttr(int quality, int slot_type) const;
	const ShenYinPastureSpiritImprintUpStartCfg * GetImprintUpLevel(int slot_type, int grade, int level) const;
	int GetImprintExtraAttrCount() const;
	AttrConfig::ATTR_TYPE GetRandImprintExtraAttrType() const;
	const ShenYinPastureSpiritImprintExtraAttrCfg * GetRandImprintExtraAttrByType(int slot_type, int attr_tyoe) const;
	const ShenYinPastureSpiritImprintRecycleCfg * GetImprintRecycle(int quality, bool is_suit) const;
	const ShenYinPastureSpiritImprintExchangeCfg * GetImprintExchange(int index) const;
	const ShenYinPastureSpiritImprintSuitCfg::SuitAttr * GetSuitAttr(int suit_id, int part_count) const;

	const ShenYinLieMingItemConfig * GetHunshouCfg(int hunshou_id);
	const ShenYinLieMingChouhunConfig * GetChouhunCfg(int chouhun_color);
	const ShenYinLieMingChouhunItemWeightCfg * GetChouhunItemWeightCfg(int hunshou_color);

	const int GetWashAttrTotalWeight() const { return m_shenyin_wash_attr_total_weight; }
	const ShenYinWashAttr * GetShenYinWashAttrCfg(int type) const;
	const ShenYinWashAddCount * GetShenYinWashAddCountCfg(int attr_num) const;

	const int GetShenYinCombineAttrAddCount() { return static_cast<int>(m_combine_attr_add_list.size()); }
	const ShenYinCombineAttrAddCfg *GetShenYinCombineAttrAddCfg(int index) const;
	const ShenYinNewSuitAttrCfg * GetSuitCfg(int color, int count) const;
	//const ShenYinLockConsumeCfg *GetXilianLockCfg(int xilian_type, int lock_num);

protected:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitBeadCfg(PugiXmlNode RootElement);
	int InitCombineCfg(PugiXmlNode RootElement);
	int InitCombineAttrCfg(PugiXmlNode RootElement);
	int InitLevelLimitCfg(PugiXmlNode RootElement);
	int InitItemListCfg(PugiXmlNode RootElement);
	int InitImprintBaseAttrCfg(PugiXmlNode RootElement);
	int InitImprintUpStartCfg(PugiXmlNode RootElement);
	int InitImprintExtraAttrCountCfg(PugiXmlNode RootElement);
	int InitImprintExtraAttrCfg(PugiXmlNode RootElement);
	int InitImprintRecycleCfg(PugiXmlNode RootElement);
	int InitImprintExchangeCfg(PugiXmlNode RootElement);
	int InitImprintSuitCfg(PugiXmlNode RootElement);
	int InitLieHunItemCfg(PugiXmlNode RootElement);
	int InitChouhunCfg(PugiXmlNode RootElement);
	int InitWashAttrCfg(PugiXmlNode RootElement);
	int InitWashAddCountCfg(PugiXmlNode RootElement);
	int InitCombineAttrAddCfg(PugiXmlNode RootElement);
	//int InitLockConsumeCfg(PugiXmlNode RootElement);
	int InitNewSuitCfg(PugiXmlNode RootElement);

private:
	ShenYinOtherCfg m_other_cfg;

	int m_bead_cfg_count;
	ShenYinBeadCfg m_bead_cfg[SHEN_YIN_BEAD_TYPE_MAX];

	int m_bead_combine_cfg_count;
	ShenYinCombineSeqCfg m_bead_combine_cfg_list[SHEN_YIN_MAX_COMBINE_CFG];

	int m_bead_combine_attr_cfg_count;
	ShenYinCombineAttrCfg m_bead_combine_attr_cfg_list[SHEN_YIN_MAX_COMBINE_CFG];

	int m_level_limit_cfg_count;
	ShenYinLevelLimitCfg m_level_limit_cfg_list[SHEN_YIN_LEVEL_LIMIT_MAX_CFG];

	int m_item_list_cfg_count;
	ShenYinPastureSpiritItemCfg m_item_list_cfg_list[PASTURE_SPIRIT_ITEM_LIST_MAX_CFG_COUNT];

	int m_imprint_base_attr_cfg_count;
	ShenYinPastureSpiritImprintBaseAttrCfg m_imprint_base_attr_cfg_list[SHEN_YIN_LIEMING_CHOUHUN_COLOR_MAX];

	int m_imprint_up_start_cfg_count;
	std::vector<ShenYinPastureSpiritImprintUpStartCfg> m_imprint_up_start_cfg_list;

	int m_imprint_extra_attr_count_cfg_count;
	int m_imprint_extra_attr_count_cfg_total_weight;
	ShenYinPastureSpiritImprintExtraAttrCountCfg m_imprint_extra_attr_count_cfg_list[PASTURE_SPIRIT_IMPRINT_EXTRA_ATTR_COUNT_MAX_CFG_COUNT];

	int m_imprint_extra_attr_cfg_count;
	std::vector<ShenYinPastureSpiritImprintExtraAttrCfg> m_imprint_extra_attr_cfg_list;

	int m_imprint_recycle_cfg_count;
	ShenYinPastureSpiritImprintRecycleCfg m_imprint_recycle_cfg_list[PASTURE_SPIRIT_IMPRINT_RECYCLE_MAX_CFG_COUNT];

	int m_imprint_exchange_cfg_count;
	ShenYinPastureSpiritImprintExchangeCfg m_imprint_exchange_cfg_list[SHEN_YIN_PASTURE_SPIRIT_MAX_SHOP_ITEM_COUNT];

	int m_imprint_suit_cfg_count;
	ShenYinPastureSpiritImprintSuitCfg m_imprint_suit_cfg_list[PASTURE_SPIRIT_IMPRINT_SUIT_MAX_CFG_COUNT];

	int m_max_liehun_item_id;																				// 最大物品数量
	ShenYinLieMingItemConfig m_hunshou_cfglist[SHEN_YIN_MAX_NUM];											// 猎魂物品配置
	ShenYinLieMingChouhunItemWeightCfg m_chouhun_weight_cfglist[SHEN_YIN_LIEMING_CHOUHUN_COLOR_MAX];// 抽魂权重
	ShenYinLieMingChouhunConfig m_chouhun_cfglist[SHEN_YIN_LIEMING_CHOUHUN_COLOR_MAX];						// 抽魂概率

	int m_shenyin_wash_attr_total_weight;
	std::vector<ShenYinWashAttr> m_shenyin_wash_attr_list;
	std::vector<ShenYinWashAddCount> m_shenyin_wash_add_count_list;

	std::vector<ShenYinCombineAttrAddCfg> m_combine_attr_add_list;
	
	ShenYinNewSuitCfg m_suit_list[SHEN_YIN_LIEMING_CHOUHUN_COLOR_PINK + 1];

	//锁定洗练消耗配置
	//std::vector<ShenYinLockConsumeCfg> m_lock_consume_vct;
};
#endif
