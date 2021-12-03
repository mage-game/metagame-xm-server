#ifndef __ELEMENT_HEART_CONFIG_HPP__
#define __ELEMENT_HEART_CONFIG_HPP__

#include "common/tinyxml/tinyxml.h"	

#include "servercommon/elementheartdef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include "item/itembase.h"
#include <servercommon/configcommon.h>
#include <vector>
#include "servercommon/configbase.h"

static const int ELEMENT_HEART_LEVEL_CFG_COUNT = 300;
static const int ELEMENT_HEART_GRADE_CFG_COUNT = 11;
static const int ELEMENT_HEART_ITEM_CFG_COUNT = 50;
static const int ELEMENT_TEXTURE_GRADE_CFG_COUNT = 101;

enum  CHARM_GHOST_PRODUCT_NUM
{
	CHARM_GHOST_PRODUCT_NUM_1,
	CHARM_GHOST_PRODUCT_NUM_2,
	CHARM_GHOST_PRODUCT_NUM_3,
	CHARM_GHOST_PRODUCT_NUM_4,
	CHARM_GHOST_PRODUCT_NUM_5,
	CHARM_GHOST_PRODUCT_NUM_6,

	CHARM_GHOST_PRODUCT_NUM_MAX,
};

enum EH_XL_SHUXING_TYPE
{
	EH_XL_SHUXING_TYPE_INVALID = 0,

	EH_XL_SHUXING_TYPE_MAXHP,
	EH_XL_SHUXING_TYPE_GONGJI,
	EH_XL_SHUXING_TYPE_FANGYU,
	EH_XL_SHUXING_TYPE_MINGZHONG,
	EH_XL_SHUXING_TYPE_SHANBI,
	EH_XL_SHUXING_TYPE_BAOJI,
	EH_XL_SHUXING_TYPE_JIANREN,
	EH_XL_SHUXING_TYPE_BASE_CONSTANT_ZENGSHANG,
	EH_XL_SHUXING_TYPE_CONSTANT_MIANSHANG,

	EH_XL_SHUXING_TYPE_MAX,
};

struct ElementHeartOtherCfg
{
	ElementHeartOtherCfg() : food_percent_same(0), food_percent_promote(0), food_percent_restraint(0), food_percent_normal(0), change_wuxing_type_need_gold(0),ghost_product_up_seed_need_gold_per_min(0),
		one_chou_need_gold(0), ten_chou_need_gold(0), one_chou_need_item(0), ten_chou_need_item(0), one_chou_free_chou_times(0), inorder_interval(0), limit_level(0),
		ghost_product_up_seed_need_item_id(0), one_chou_need_score(0), ten_chou_need_score(0), shop_refresh_free_times(0), shop_refresh_need_gold(0), shop_refresh_need_gold_bind(0), shop_refresh_need_score(0), shop_need_gold_buy_count(0),
		shop_refresh_time_interval(0)
	{}

	int food_percent_same;							// 食物加成百分比相同
	int food_percent_promote;						// 食物加成百分比相生
	int food_percent_restraint;						// 食物加成百分比相克
	int food_percent_normal;						// 食物加成百分比普通
	int change_wuxing_type_need_gold;				// 五行转换消耗
	int ghost_product_up_seed_need_gold_per_min;	// 符鬼加速消耗
	int one_chou_need_gold;							// 一次抽奖消耗
	int ten_chou_need_gold;							// 十次抽奖消耗
	int one_chou_need_item;							// 一次抽奖道具
	int ten_chou_need_item;							// 十次抽奖道具
	int one_chou_free_chou_times;					// 一次抽奖免费次数
	int inorder_interval;							// 背包整理时间间隔
	int limit_level;
	int ghost_product_up_seed_need_item_id;			// 元素加速消耗

	int one_chou_need_score;
	int ten_chou_need_score;
	int shop_refresh_free_times;
	int shop_refresh_need_gold;
	int shop_refresh_need_gold_bind;
	int shop_refresh_need_score;
	int shop_need_gold_buy_count;

	int shop_refresh_time_interval;
};

enum ELEMENT_HEART_OPEN_CONDITION_TYPE
{
	ELEMENT_HEART_OPEN_CONDITION_TYPE_LAST_FEED_LEVEL = 0,
	ELEMENT_HEART_OPEN_CONDITION_TYPE_GOLD,
};

struct ElementHeartOpenCfg
{
	ElementHeartOpenCfg(): id(0), condition_type(0), param(0)
	{
	}

	int id;
	int condition_type;
	int param;
};

struct ElementHeartWuxingCfg
{
	ElementHeartWuxingCfg(): feed_level(0), wuxing_min(0), wuxing_max(0), product_num(0), total_weight(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0)
	{
		memset(weight_list, 0, sizeof(weight_list));
	}

	int feed_level;
	int wuxing_min;
	int wuxing_max;
	int product_num;
	int total_weight;
	int weight_list[CHARM_GHOST_PRODUCT_NUM_MAX];

	int maxhp;
	int gongji;
	int fangyu;
	int mingzhong;
	int shanbi;
	int baoji;
	int jianren;
};

struct ElementHeartGradeCfg
{
	ElementHeartGradeCfg(): grade(0), add_texture_percent_attr(0), product_time_s(0), item_id(0), need_item_num(0), bless_val_limit(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0)
	{
	}

	int grade;
	int add_texture_percent_attr;
	int product_time_s;
	int item_id;
	int need_item_num;
	int bless_val_limit;
	
	int maxhp;
	int gongji;
	int fangyu;
	int mingzhong;
	int shanbi;
	int baoji;
	int jianren;
};

struct ElementHeartItemCfg
{
	ElementHeartItemCfg(): item_id(0), type(0), param1(0), param2(0), param3(0)
	{
	}

	ItemID item_id;
	int type;
	int param1;
	int param2;
	int param3;
};

struct ElementHeartLevelCfg
{
	ElementHeartLevelCfg() : wuxing_type(0), grade(0), exp_limit(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0)
	
	{
	}

	int wuxing_type;
	int grade;
	int exp_limit;
	
	int maxhp;
	int gongji;
	int fangyu;
	int mingzhong;
	int shanbi;
	int baoji;
	int jianren;
};

struct ElementHeartChouCfg
{
	ElementHeartChouCfg(): seq(0),  weight(0)
	{
	}

	int seq;
	ItemConfigData reward_item;
	int weight;
};

struct ElementHeartEquipLevelCfg
{
	ElementHeartEquipLevelCfg() : real_level(0), attr_total_percent(0), comsume_item_id(0), comsume_item_num(0), upgrade_progress(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0)
	{
		memset(item_id_list, 0, sizeof(item_id_list));
	}

	int real_level;															// 等级
	int attr_total_percent;													// 全属性增加万份比
	int comsume_item_id;													// 升级消耗物品
	int comsume_item_num;													// 升级消耗物品数量
	int upgrade_progress;													// 升级所需进度值

	int item_id_list[ELEMENT_HEART_EQUIP_MAX_COUNT];						// 需消耗的虚拟物品ID列表

	int maxhp;
	int gongji;
	int fangyu;
	int mingzhong;
	int shanbi;
	int baoji;
	int jianren;
};

struct ElementHeartShopConfig
{
	ElementHeartShopConfig() : seq(-1), weight(0), cost_gold_bind(0), cost_gold(0)
	{
	}

	int seq;
	ItemConfigData reward_item;

	int weight;
	int cost_gold_bind;
	int cost_gold;
};

struct ElementHeartXilianLockCfg
{
	ElementHeartXilianLockCfg() :comsume_color(0), lock_num(0)
	{}

	int comsume_color;
	int lock_num;
	ItemConfigData lock_comsume_item;
};

enum EH_XL_COMSUME_COLOR
{
	EH_COUMME_COLOR_INVALID,

	EH_COUMME_COLOR_BLUE,
	EH_COUMME_COLOR_PURPLE,
	EH_COUMME_COLOR_ORANGE,
	EH_COUMME_COLOR_RED,

	EH_COUMME_COLOR_MAX,
};

struct  ElemenEquipAttrConfig
{
	ElemenEquipAttrConfig() : item_id(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0)
	{

	}

	int item_id;
	ItemConfigData reward_item;

	int maxhp;
	int gongji;
	int fangyu;
	int mingzhong;
	int shanbi;
	int baoji;
	int jianren;
};

struct ElementHeartXilianComsumeCfg
{
	ElementHeartXilianComsumeCfg() :comsume_color(EH_COUMME_COLOR_INVALID), max_star_number(0)
	{
		memset(star_weight, 0, sizeof(star_weight));
	}

	int comsume_color;
	ItemConfigData consume_item;
	int max_star_number;
	int star_weight[ELEMENT_HEART_XILIAN_MAX_STAR_COUNT];
};

struct EHXilianExtraNeedCfg
{
	EHXilianExtraNeedCfg() : need_count(0), add_percent(0)
	{

	}

	int need_count;
	int add_percent;
};

struct ElementHeartXilianExtraConfig
{
	ElementHeartXilianExtraConfig() : element_attr_type(0)
	{

	}

	int element_attr_type;
	std::vector<EHXilianExtraNeedCfg> need_count_vec;
};

struct EHXilianShuxingCfg
{
	EHXilianShuxingCfg() : attr_type(0)
	{
		memset(star_min, 0, sizeof(star_min));
		memset(star_max, 0, sizeof(star_max));
	}

	int attr_type;
	int star_min[ELEMENT_HEART_XILIAN_MAX_STAR_COUNT];
	int star_max[ELEMENT_HEART_XILIAN_MAX_STAR_COUNT];
};

struct AttrRateItem
{
	AttrRateItem() : attr_type(0), weight(0) {}

	int attr_type;
	int weight;
};

struct ElementAttrRateCfg
{
	ElementAttrRateCfg() : total_weight(0)
	{

	}

	int total_weight;
	std::vector<AttrRateItem> attr_list;
};

class ElementHeartConfig : public ConfigBase
{
public:
	ElementHeartConfig();
	~ElementHeartConfig();

	bool Init(const std::string &configname, std::string *err);

	const ElementHeartOtherCfg & GetOtherCfg() const{ return m_other_cfg; }
	const ElementHeartOpenCfg * GetElementIndexCfgById(int id) const;
	const ElementHeartWuxingCfg * GetElementWuxingCfgByWuxing(int wuxingzhi) const;
	int GetElementWuxingMax() const { return m_element_wuxing_max; }
	const ElementHeartGradeCfg * GetElementGradeCfgByGrade(int grade) const;
	const ElementHeartItemCfg * GetITemCfgById(int id) const;
	const ElementHeartItemCfg * GetITemCfgByItemId(int id) const;
	const ElementHeartChouCfg * GetChouCfgByIndex(int index) const;
	const int GetChouCfgChout() const { return m_chou_cfg_count; }
	int GetChouCfgTotalWeight() const { return m_chout_total_weight; }
	const ElementHeartItemCfg *GetItemCfg(int item_type, int wuxing_type, int quality) const;
	const int GetCharmWuxingType(int equipment_index);
	const ElementHeartLevelCfg * GetElementTextureLevelCfg(int wuxing_type, int grade) const;

	void GetRandomShopItem(ElementHeartShopItem shop_item_list[ELEMENT_SHOP_ITEM_COUNT]) const;
	const ElementHeartShopConfig * GetShopCfg(int shop_seq);
	const ElementHeartEquipLevelCfg *GetEquipLevelCfg(int level);
	const ElemenEquipAttrConfig * GetEquipAttrCfg(int v_id);
	const ElementHeartXilianLockCfg *GetXilianLockCfg(int consume_color, int lock_num);

	const int GetExtraAttrAddPercent(int attr_type, int count);
	const int GetXiLianSlotOpenLevel(int element_id, int slot_id);
	const EHXilianShuxingCfg* GetXiLianAttrCfg(int element_id, int slot_id);
	const ElementHeartXilianComsumeCfg * GetXilianConsumeCfg(int color);
	const int GetXilianConsuemeTotalWeight(int color);
	const ElementAttrRateCfg *GetElementAttrRateCfg(int element_id);
	const int GetRandElementAttr(int element_id);

	const std::map<int, ElementHeartItemCfg> &GetItemMap() { return m_item_map;  }
	const int GetElementUpgradeLimit(int element_id);

protected:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitElementHeartOpenCfg(PugiXmlNode RootElement);
	int InitElementHeartWuxingCfg(PugiXmlNode RootElement);
	int InitElementHeartGradeCfg(PugiXmlNode RootElement);
	int InitElementHeartUpgradeLimitCfg(PugiXmlNode RootElement);
	int InitItemCfg(PugiXmlNode RootElement);
	int InitChouCfg(PugiXmlNode RootElement);
	int InitCharmInlayCfg(PugiXmlNode RootElement);
	int InitElementTextureLevelCfg(PugiXmlNode RootElement);
	int InitShopCfg(PugiXmlNode RootElement);
	int InitEquipLevelCfg(PugiXmlNode RootElement);
	int InitEquipAttrCfg(PugiXmlNode RootElement);
	int InitXilianLockCfg(PugiXmlNode RootElement);
	int InitXilianConsumeCfg(PugiXmlNode RootElement);
	int InitXilianOpenCfg(PugiXmlNode RootElement);
	int InitXilianAttrCfg(PugiXmlNode RootElement);
	int InitXilianExtraAttrCfg(PugiXmlNode RootElement);
	int InitElementAttrRateCfg(PugiXmlNode RootElement);

private:
	ElementHeartOtherCfg m_other_cfg;
	int m_element_index_cfg_count;
	ElementHeartOpenCfg m_element_index_cfg_list[ELEMENT_HEART_MAX_COUNT];

	int m_element_wuxing_cfg_count;
	int m_element_wuxing_max;
	ElementHeartWuxingCfg m_element_wuxing_cfg_list[ELEMENT_HEART_LEVEL_CFG_COUNT];

	int m_element_grade_cfg_count;
	ElementHeartGradeCfg m_element_grade_cfg_list[ELEMENT_HEART_GRADE_CFG_COUNT + 1];
	int m_upgrade_limit_list[ELEMENT_HEART_MAX_COUNT];

	int m_item_cfg_count;
	ElementHeartItemCfg m_item_cfg_list[ELEMENT_HEART_ITEM_CFG_COUNT];

	std::map<int, ElementHeartItemCfg> m_item_map;

	int m_chout_total_weight;
	int m_chou_cfg_count;
	ElementHeartChouCfg m_chou_cfg_list[ELEMENT_HEART_ITEM_CFG_COUNT];

	int m_charm_inlay_cfg_list[ItemNamespace::MAX_EQUIPMENT_GRID_NUM];

	ElementHeartLevelCfg m_element_texture_level_cfg_list[ELEMENT_HEART_WUXING_TYPE_MAX][ELEMENT_TEXTURE_GRADE_CFG_COUNT + 1];

	int m_equip_max_level;
	ElementHeartEquipLevelCfg m_equip_level_cfg_list[ELEMENT_HEART_EQUIP_MAX_LEVEL + 1];
	std::vector<ElemenEquipAttrConfig> m_equip_attr_vec;

	int m_shop_total_weight;
	std::vector<ElementHeartShopConfig> m_shop_cfg_list;

	int m_xilian_open_level_cfg[ELEMENT_HEART_MAX_COUNT][ELEMENT_HEART_MAX_XILIAN_SLOT];
 	//ElementHeartXilianLockCfg m_xilian_lock_cfg[ELEMENT_HEART_MAX_XILIAN_SLOT];
 	int m_xilian_consuem_total_weight[EH_COUMME_COLOR_MAX];
 	ElementHeartXilianComsumeCfg m_xilian_consume_cfg[EH_COUMME_COLOR_MAX];

	std::vector<EHXilianExtraNeedCfg> m_extra_cfg_vec[ELEMENT_HEART_WUXING_TYPE_MAX];
	std::map<long long, EHXilianShuxingCfg> m_xilian_attr_cfg_map;
	ElementAttrRateCfg m_element_rate_list[ELEMENT_HEART_MAX_COUNT];

	//锁洗消耗配置
	std::vector<ElementHeartXilianLockCfg> m_lock_consume_vct;
};

#endif
