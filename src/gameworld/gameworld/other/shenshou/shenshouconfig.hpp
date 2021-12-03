#ifndef __SHENSHOU_CONFIG_HPP__
#define __SHENSHOU_CONFIG_HPP__

#include "servercommon/configbase.h"

#include "servercommon/shenshoudef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include <set>
#include <map>
#include <vector>

static const int SHENSHOU_MAX_QUALITY = 5;											// 神兽装备最大品质
static const int SHENSHOU_MAX_EQUIP_ID = 40;										// 神兽装备最大ID
static const int SHENSHOU_MAX_EQUIP_STRENGTH_LEVEL = 500;							// 神兽装备最大强化等级
static const int SHENSHOU_MAX_EXTEND_NUM = 5;										// 最大允许额外增加的助战数量
static const int SHENSHOU_MAX_SKILL_LEVEL = 5;										// 最大技能等级
static const int SHENSHOU_MAX_COMPOSE_CFG = 20;										// 神兽装备最大合成配置

struct ShenshouAttrCfg
{
	ShenshouAttrCfg() : attr_type(0), attr_value(0) {}

	int attr_type;																	// 属性类型（SHENSHOU_ATTR_TYPE）
	int attr_value;																	// 属性值
};

struct ShenshouOtherCfg
{
	ShenshouOtherCfg() : need_role_level(0), equip_double_shulian_per_gold(0), default_zhuzhan_count(0), 
						huanling_draw_limit(0), huanling_refresh_consume(0), start_limit_level(0), opengame_day(0)
	{
	}

	int need_role_level;
	int equip_double_shulian_per_gold;
	int default_zhuzhan_count;
	int huanling_draw_limit;
	int huanling_refresh_consume;
	int start_limit_level;
	int opengame_day;
};

struct ShenshouEquipQualityCfg
{
	static const int MAX_NORMAL_ATTR_CFG_COUNT = 10;								// 普通属性最大个数
	static const int MAX_STAR_ATTR_CFG_COUNT = 10;									// 星级属性最大个数
	
	struct LevelShulianduCfg
	{
		LevelShulianduCfg() : slot_index(0), level(0), maxhp(0), gongji(0), fangyu(0), shanbi(0), mingzhong(0), upgrade_need_shulian(0), contain_shulian(0),
			baoji(0), jianren(0), constant_zengshang(0), constant_mianshang(0)
		{
		}

		int slot_index;																// 装备部位
		int level;																	// 强化等级

		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute shanbi;
		Attribute mingzhong;

		int upgrade_need_shulian;													// 提升等级所需熟练度
		int contain_shulian;														// 当强化材料时提升熟练度值

		Attribute baoji;
		Attribute jianren;
		Attribute constant_zengshang;
		Attribute constant_mianshang;
	};

	struct BaseAttrduCfg
	{
		BaseAttrduCfg() : slot_index(0), maxhp(0), gongji(0), fangyu(0), shanbi(0), mingzhong(0), baoji(0), jianren(0), constant_zengshang(0), constant_mianshang(0)
		{
		}

		int slot_index;																// 装备部位

		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute shanbi;
		Attribute mingzhong;
		Attribute baoji;
		Attribute jianren;
		Attribute constant_zengshang;
		Attribute constant_mianshang;
	};

	ShenshouEquipQualityCfg() : quality(0), rand_attr_num(0), star_attr_max_num(0), total_weight(0), normal_attr_num(0), star_attr_num(0), max_equip_strength_level(0){}

	const ShenshouAttrCfg *GetRandomNormalAttr(const std::set<int> &attr_set) const;
	const ShenshouAttrCfg *GetRandomStarAttr(const std::set<int> &attr_set) const;
	int GetRandomStartNum() const;

	int quality;
	int rand_attr_num;
	int star_attr_max_num;
	int total_weight;
	std::map<int, int> weight_list_map;

	int normal_attr_num;
	ShenshouAttrCfg normal_attr_list[MAX_NORMAL_ATTR_CFG_COUNT];					// 普通属性列表
	int star_attr_num;
	ShenshouAttrCfg star_attr_list[MAX_STAR_ATTR_CFG_COUNT];						// 星级属性列表

	int max_equip_strength_level;													// 最大强化等级
	LevelShulianduCfg level_cfg_list[SHENSHOU_MAX_EQUIP_SLOT_INDEX + 1][SHENSHOU_MAX_EQUIP_STRENGTH_LEVEL + 1]; // 槽+强化等级对应熟练度信息

	BaseAttrduCfg base_attr_cfg_list[SHENSHOU_MAX_EQUIP_SLOT_INDEX + 1];			// 基础属性列表（根据不同槽对应唯一配置）
};

struct ShenshouItemCfg
{
	ShenshouItemCfg() : real_id(0),item_id(0), is_equip(false), quality(0), slot_index(0), contain_shulian(0)
	{
	}

	ItemID real_id;																	// 真实物品ID
	int item_id;																	// 物品ID
	bool is_equip;																	// 是否装备
	int quality;																	// 装备品质
	int slot_index;																	// 所在装备部位
	int contain_shulian;															// 当强化材料时提升熟练度值(针对非装备，因为装备的熟练度在等级配置里)
};

struct ShenshouZhuzhanCfg
{
	ShenshouZhuzhanCfg(): extra_num(0), stuff_id(0), stuff_num(0)
	{
	}

	int extra_num;												// 额外助战位
	ItemID stuff_id;											// 材料id
	int stuff_num;												// 消耗数量
};

struct ShenshouSkillCfg
{
	const static int ATTR_COUNT = 4;							// 属性个数
	const static int PARAM_COUNT = 7;							// 技能参数个数

	struct LevelCfg
	{

		LevelCfg() : level(0) {}

		int level;												// 等级
		
		int param_list[PARAM_COUNT];							// 如果代表是百分比，参数为万分比的分子，记得做对应出来
	};

	ShenshouSkillCfg() : skill_type(0), max_level(0) {}

	int skill_type;												// 技能ID

	int max_level;
	LevelCfg level_cfg_list[SHENSHOU_MAX_SKILL_LEVEL + 1];
};

struct ShenshouInfoCfg
{
	struct SkillItem
	{
		SkillItem() : skill_type(0), skill_level(0) {}

		int skill_type;
		int skill_level;
	};

	const static int ATTR_COUNT = 4;							// 属性个数
	const static int MAX_SKILL_COUNT = 4;						// 技能最大个数

	ShenshouInfoCfg() : shou_id(0), maxhp(0), gongji(0), fangyu(0) {}

	int shou_id;												// 神兽ID

	int slot_need_quality[SHENSHOU_MAX_EQUIP_SLOT_INDEX + 1];	// 每个部位所需装备最低品质

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;

	SkillItem skill_list[MAX_SKILL_COUNT];
};

struct ShenshouComposeCfg
{
	ShenshouComposeCfg(): v_item_id(0), give_start_num(0), give_quality(0), need_start_num(0), need_qualit(0), 
							is_need_item(false), is_broatcast(false), item_id(0), item_num(0)
	{}

	ItemID v_item_id;
	int give_start_num;
	int give_quality;
	int need_start_num;
	int need_qualit;

	bool is_need_item;
	bool is_broatcast;
	ItemID item_id;
	int item_num;
};

struct HuanlingRefreshItemCfg
{
	HuanlingRefreshItemCfg() : refresh_weight(0), draw_weight(0), seq(0), is_broadcast(false)
	{}

	int refresh_weight;
	int draw_weight;
	int seq;
	bool is_broadcast;

	ItemConfigData item;
};

struct HuanlingPoolCfg
{
	HuanlingPoolCfg() : refresh_weight(0), baodi_limit_times(0), dupilcate(false), item_total_refresh_weight(0), item_total_draw_weight(0)
	{}

	int refresh_weight;								//该档的刷新权重
	int baodi_limit_times;
	bool dupilcate;

	int item_total_refresh_weight;					//该档物品的刷新总权重
	int item_total_draw_weight;						//该档物品的抽奖总权重

	std::map<int, HuanlingRefreshItemCfg> item_map;
};

struct HuanlingDrawCfg
{
	HuanlingDrawCfg() : score(0), total_weight(0)
	{}

	int score;
	int total_weight;

	std::vector<int> draw_pool_weight_vec;
};

class ShenshouConfig : public ConfigBase
{
public:
	ShenshouConfig();
	~ShenshouConfig();

	bool Init(const std::string &configname, std::string *err);
	const ShenshouOtherCfg & GetOtherCfg() const { return m_other_cfg; }
	const ShenshouItemCfg * GetItemInfo(int item_id) const;	//用虚拟id取配置
	const ShenshouItemCfg * GetItemInfo(ItemID real_id,int item_id) const; //用物品ID取配置 item_id传0就可以了
	const ShenshouInfoCfg * GetShenshouInfo(int shou_id) const;
	const ShenshouEquipQualityCfg * GetEquipQualityCfg(int quality) const;
	const ShenshouEquipQualityCfg::LevelShulianduCfg * GetEquipShulianduCfg(int equip_slot_index, int strength_level) const;
	int GetEquipShulianduMax(int equip_slot_index);
	const ShenshouEquipQualityCfg::BaseAttrduCfg * GetEquipBaseAttrCfg(int quality, int equip_slot_index) const;
	const ShenshouSkillCfg::LevelCfg * GetSkillLevelCfg(int skill_type, int skill_level) const;
	const ShenshouZhuzhanCfg * GetExtraZhuzhanCfg(int extra_zhuzhan_count) const;
	const ShenshouComposeCfg * GetComposeCfg(int v_item_id) const;

	int GetHuanlingDrawConsumeScore(int draw_times);
	bool GetHuanlingRefreshItem(int &refresh_times, HuanlingListInfo item_list[SHENSHOU_MAX_RERFESH_ITEM_COUNT], int item_count = SHENSHOU_MAX_RERFESH_ITEM_COUNT);
	const ItemConfigData * GetHuanlingDrawItem(short &draw_times, HuanlingListInfo cur_list[SHENSHOU_MAX_RERFESH_ITEM_COUNT], int &item_seq, bool &is_broadcast, int list_len = SHENSHOU_MAX_RERFESH_ITEM_COUNT);

	int GetMaxEquipStrengthLevel() { return m_equip_quality_cfg_list[0].max_equip_strength_level; }

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitQualityCfg(PugiXmlNode RootElement);
	int InitEquipAttrCfg(PugiXmlNode RootElement);
	int InitEquipCfg(PugiXmlNode RootElement);
	int InitEquipBaseAttrCfg(PugiXmlNode RootElement);
	int InitEquipLevelCfg(PugiXmlNode RootElement);
	int InitZhuzhanCfg(PugiXmlNode RootElement);
	int InitSkillCfg(PugiXmlNode RootElement);
	int InitShenshouCfg(PugiXmlNode RootElement);
	int InitSlotNeedQualityCfg(PugiXmlNode RootElement);
	int InitComposeCfg(PugiXmlNode RootElement);
	int InitHuanlingPoolCfg(PugiXmlNode RootElement);
	int InitHuanlingRefreshCfg(PugiXmlNode RootElement);
	int InitHuanlingDrawCfg(PugiXmlNode RootElement);

	int HuanlingRefreshPoolHelper(int refresh_times, int &dupilcate_flag);

	ShenshouOtherCfg m_other_cfg;

	ShenshouEquipQualityCfg m_equip_quality_cfg_list[SHENSHOU_MAX_QUALITY + 1];

	int m_max_equip_id;
	ShenshouItemCfg m_equip_cfg_list[SHENSHOU_MAX_EQUIP_ID + 1];
	
	int m_max_extend_num;
	ShenshouZhuzhanCfg m_zhuzhan_consume_cfg_list[SHENSHOU_MAX_EXTEND_NUM + 1];

	ShenshouSkillCfg m_skill_cfg_list[SHENSHOU_SKILL_TYPE_COUNT];

	int m_shenshou_max_id;
	ShenshouInfoCfg m_shenshou_cfg_list[SHENSHOU_MAX_ID + 1];

	int m_compose_cfg_count;
	ShenshouComposeCfg m_shenshou_compose_cfg_list[SHENSHOU_MAX_COMPOSE_CFG];

	int m_huanling_pool_total_refresh_weight;
	std::vector<HuanlingPoolCfg> m_huanling_pool_cfg_vec;
	std::vector<HuanlingDrawCfg> m_huanling_draw_cfg_vec;
};

#endif