#ifndef __ZHUANZHI_EQUIP_CONFIG_HPP__
#define __ZHUANZHI_EQUIP_CONFIG_HPP__

#include "gamedef.h"
#include "servercommon/basedef.h"
#include "servercommon/zhuanzhiequipdef.hpp"
#include "servercommon/struct/itemlistparam.h"

#include "equipment/equipment.h"
#include "item/itembase.h"
#include "item/itempool.h"

#include "obj/character/attribute.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include <map>
#include <vector>

// 其它配置
struct ZhuanzhiOtherConfig
{
	ZhuanzhiOtherConfig(): refine_min_order(0), zhizun_add_max_level(0), wake_min_order(0), wake_min_color(0) {}

	unsigned int refine_min_order;      // 玉石精炼装备的最小阶数
	int zhizun_add_max_level;
	int wake_min_order;
	int wake_min_color;
};

// 转职装备配置
struct ZhuanzhiEquipCfg
{
	ZhuanzhiEquipCfg() : equip_part(0), equip_order(0), role_need_min_level(0), role_need_min_prof_level(0),
		special_attr_color(0), add_cap(0), pvp_zengshang_per(0), pve_zengshang_per(0), pvp_jianshang_per(0), pve_jianshang_per(0), sys_baoji_per(0), sys_kangbao_per(0),
		effect_type(0) {}

	int equip_part;
	int equip_order;
	int role_need_min_level;
	int role_need_min_prof_level;
	int special_attr_color;
	int add_cap;
	int pvp_zengshang_per;
	int pve_zengshang_per;
	int pvp_jianshang_per;
	int pve_jianshang_per;
	int sys_baoji_per;
	int sys_kangbao_per;
	int effect_type;
};

// 装备特殊效果
struct EquipSpecialEffectCfg
{
	EquipSpecialEffectCfg() : effect_type(0), cd_time(0), rate(0), param_1(0), param_2(0) {}

	int effect_type;
	int cd_time;
	int rate;
	int param_1;
	int param_2;
};

//升星相关-------------------------------------------------------------------
struct ZhuanzhiUpStarConfig
{
	ZhuanzhiUpStarConfig() : equip_index(0), star_level(0), equip_level(0), need_mojing(0), add_maxhp(0), add_gongji(0), add_fangyu(0), is_broadcast(0)
	{
	}

	int equip_index;
	int star_level;
	int equip_level;
	int need_mojing;
	Attribute add_maxhp;
	Attribute add_gongji;
	Attribute add_fangyu;
	int is_broadcast;
};

// 全身升星
struct ZhuanzhiUpStarTotalConfig
{
	ZhuanzhiUpStarTotalConfig() : seq(0), total_star(0), add_maxhp(0), add_gongji(0), add_fangyu(0), is_broadcast(0)
	{
	}

	int seq;
	int total_star;
	Attribute add_maxhp;
	Attribute add_gongji;
	Attribute add_fangyu;
	int is_broadcast;
};

//附灵相关-------------------------------------------------------------------
struct ZhuanzhiEquipFulingCfg
{
	ZhuanzhiEquipFulingCfg() : equip_order(0), fuling_max_count(0)
	{
	}

	int equip_order;
	int fuling_max_count;
};

// 附灵材料
struct ZhuanzhiFulingStuffCfg
{
	ZhuanzhiFulingStuffCfg() : seq(0), stuff_id(0), add_attr_type(0), add_attr_val(0)
	{
	}

	int seq;
	int stuff_id;
	short add_attr_type;
	int add_attr_val;
};

// 玉石相关-------------------------------------------------------------------
struct ZhuanzhiStoneCfg
{
	ZhuanzhiStoneCfg() : item_id(0), stone_type(0), attr_type1(0), attr_val1(0), attr_type2(0), attr_val2(0), role_level_limit(0), level(0)
	{
	}

	int item_id;
	int stone_type;
	short attr_type1;
	Attribute attr_val1;
	short attr_type2;
	Attribute attr_val2;
	int role_level_limit;
	int level;
};

//全身玉石
struct ZhuanzhiStoneTotalCfg
{
	ZhuanzhiStoneTotalCfg() : total_stone_level(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), is_broadcast(0) {}

	int total_stone_level;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	int is_broadcast;
};

// 玉石升级配置
struct ZhuanzhiStoneLevelCfg
{
	ZhuanzhiStoneLevelCfg(): old_stone_item_id(0), new_stone_item_id(0), need_num(0) {}

	int old_stone_item_id;
	int new_stone_item_id;
	int need_num;
};

// 玉石孔配置
struct ZhuanzhiStoneSlotCfg
{
	ZhuanzhiStoneSlotCfg() : part_index(0), slot_index(0), stone_type_limit(0), order_limit(0), vip_level_limit(0){}

	int part_index;
	int slot_index;
	int stone_type_limit;
	int order_limit;
	int vip_level_limit;
};

// 玉石精炼配置
struct ZhuanzhiStoneRefineCfg
{
	ZhuanzhiStoneRefineCfg() : refine_level(0), consume_refine_val(0), per_attr_add(0) {}

	int refine_level;
	int consume_refine_val;
	int per_attr_add;
};

// 精炼材料配置
struct RefineStuffCfg
{
	RefineStuffCfg() : part_index(0), seq(0), stuff_id(0), add_refine(0) {}

	int part_index;
	int seq;
	int stuff_id;
	int add_refine;
};

// 玉石分解配置
struct StoneResolveCfg
{
	StoneResolveCfg() : stone_id(0), resolve_get_score(0) {}

	int stone_id;
	int resolve_get_score;
};

// 玉石兑换配置
struct StoneConvertCfg
{
	StoneConvertCfg() : seq(0), convert_need_score(0), convert_stone_id(0) {}

	int seq;
	int convert_need_score;
	int convert_stone_id;
};

//套装相关-------------------------------------------------------------------

// 套装组合类型
struct SuitGroupTypeCfg
{
	SuitGroupTypeCfg() : equip_index(0), group_type(0) {}

	int equip_index;
	int group_type;
};

// 套装锻造限制
struct SuitForgeLimitCfg
{
	int suit_index = 0;
	int suit_min_color = 0;
	int suit_min_order = 0;
};

//套装部位激活
struct SuitOneEquipActiveCfg
{
	SuitOneEquipActiveCfg() : role_prof(0), suit_index(0), equip_part(0), equip_order(0), stuff_1_id(0), stuff_1_num(0), stuff_2_id(0),
		stuff_2_num(0), stuff_3_id(0), stuff_3_num(0) {}

	int role_prof;
	int suit_index;
	int equip_part;
	int equip_order;

	ItemID stuff_1_id;
	int stuff_1_num;
	ItemID stuff_2_id;
	int stuff_2_num;
	ItemID stuff_3_id;
	int stuff_3_num;
};

// 防具组合套装属性
struct FangGroupAttrCfg
{
	FangGroupAttrCfg() : role_prof(0), suit_index(0), equip_order(0), same_order_num(0), maxhp(0), per_maxhp(0), gongji(0),
		per_gongji(0), fangyu(0), per_fangyu(0) {}

	int role_prof;
	int suit_index;
	int equip_order;
	int same_order_num;
	int maxhp;
	int per_maxhp;
	int	gongji;
	int per_gongji;
	int fangyu;
	int per_fangyu;
};

// 攻击组合套装属性
struct GongGroupAttrCfg
{
	GongGroupAttrCfg() : suit_index(0), same_order_num(0), maxhp(0), fangyu(0), gongji(0),
		per_maxhp(0), per_fangyu(0), per_gongji(0), skill_zengshang_per(0), per_mianshang(0), per_pofang(0) {}

	int suit_index;
	int same_order_num;
	int maxhp;
	int fangyu;
	int gongji;
	int per_maxhp;
	int per_fangyu;
	int per_gongji;
	int skill_zengshang_per;
	int per_mianshang;
	int per_pofang;
};

// 至尊组合套装属性
struct ZhizunGroupAttrCfg
{

	ZhizunGroupAttrCfg() : equip_order(0), pvp_zengshang_per(0), pve_zengshang_per(0) {}

	int equip_order;
	int pvp_zengshang_per;
	int pve_zengshang_per;
};

//至尊装备相关-------------------------------------------------------------------

// 至尊装备配置
struct ZhiZunEquipCfg		
{

	ZhiZunEquipCfg() : equip_id(0), equip_order(0), param1(0), param2(0), role_need_min_level(0), role_need_min_prof_level(0), add_cap(0) {}

	ItemID equip_id;
	int equip_order;

	int param1;
	int param2;
	int role_need_min_level;
	int role_need_min_prof_level;
	int add_cap;
};

// 至尊装备合成
struct EquipZhiZunComposeCfg
{
	EquipZhiZunComposeCfg() : compose_equip_id(0), compose_equip_best_attr_num(0), stuff1_id(0), stuff1_num(0), equip1_id(0), equip1_num(0), equip2_id(0), equip2_num(0) {}

	ItemID compose_equip_id;
	int compose_equip_best_attr_num;
	ItemID stuff1_id;
	int stuff1_num;
	ItemID equip1_id;
	int equip1_num;
	ItemID equip2_id;
	int equip2_num;
};

// 特殊装备配置-------------------------------------------------------------------
//无穿戴限制
struct SpecialEquipCfg
{

	SpecialEquipCfg(){}

	ItemID equip_id = 0;
	int role_need_min_level = 0;
	int role_need_min_prof_level = 0;
};

//觉醒属性配置
struct AwakeningAttributeCfg
{
	AwakeningAttributeCfg() : attribute_type(0), attribute_value(0), max_gongji_per(0), max_fangyu_per(0), max_hp_per(0) {}

	int attribute_type;
	Attribute attribute_value;
	int max_gongji_per;
	int max_fangyu_per;
	int max_hp_per;
};

struct AwakeningTypeWeightCfg
{
	AwakeningTypeWeightCfg() : total_weight(0) {}

	wake_weight_t total_weight;
	struct TypeCfg
	{
		TypeCfg() : weight(0), is_precious(0) {}
		wake_weight_t weight;
		int is_precious;
	};
	std::map<wake_type_t, TypeCfg> type_cfg_map;
};

struct AwakeningLevelWeightCfg
{
	AwakeningLevelWeightCfg() {}

	struct LevelRangeWeightCfg
	{
		LevelRangeWeightCfg() : level_left(0), level_right(0), total_weight(0) {}
		short level_left;
		short level_right;
		wake_weight_t total_weight;
		std::vector<wake_weight_t> weight;
	};
	std::vector<LevelRangeWeightCfg> level_weight_cfg_vec;
};

struct AwakeningLockConsumeCfg
{
	AwakeningLockConsumeCfg() : consume_stuff_id(0), lock_stuff_id(0), consume_stuff_num(0), consume_stuff_num_1(0), lock_stuff_num(0), gold_num(0) {}

	ItemID consume_stuff_id;
	ItemID lock_stuff_id;
	int consume_stuff_num;
	int consume_stuff_num_1;
	int lock_stuff_num;
	Int64 gold_num;
};

struct ZhuanzhiEquipComposeCfg
{
	ZhuanzhiEquipComposeCfg() : best_attr_count(0), color(0), is_broadcast(0), need_equip_num_min(0), need_equip_num_max(0) {}

	short best_attr_count;
	short color;
	short is_broadcast;
	short need_equip_num_min;
	short need_equip_num_max;
	std::vector<short> success_rate;
};

struct ZhuanzhiEquipConditionAttrCfg
{
	ZhuanzhiEquipConditionAttrCfg() : xianpin_num(0) {}

	int xianpin_num;
	std::vector<int> attribute_type;
	std::vector<Attribute> attribute_value;
};

// 转职装备配置
class ZhuanzhiEquipConfig : public ConfigBase
{
public:
	static const int MAX_STAR_LEVEL = 1000;													// 装备星星最大等级
	static const int TOTAL_STAR_CFG_MAX_COUNT = 60;											// 全身星星配置最大数量

	ZhuanzhiEquipConfig();
	~ZhuanzhiEquipConfig();

	bool Init(const std::string &configname, std::string *err);

	const ZhuanzhiOtherConfig &GetOtherCfg() { return m_other_cfg; }

	// 转职装备相关-----------------------------------------------------------------------------
	const ZhuanzhiEquipCfg * GetZhuanzhiEquipCfg(int equip_part, int equip_order);
	const EquipSpecialEffectCfg * GetEquipEffectCfg(int effect_type);

	// 升星相关-----------------------------------------------------------------------------
	const ZhuanzhiUpStarConfig * GetUpStarCfg(int equip_index, int star_level);
	const ZhuanzhiUpStarTotalConfig * GetUpStarTotalCfg(int star_level, bool is_system_msg);

	//附灵相关-----------------------------------------------------------------------------
	const ZhuanzhiEquipFulingCfg * GetZhuanzhiFulingCfg(int equip_order);
	const ZhuanzhiFulingStuffCfg * GetFulingStuffCfg(int seq);

	//玉石相关-----------------------------------------------------------------------------
	const ZhuanzhiStoneCfg * GetZhuanzhiStoneCfg(int stone_id);
	const ZhuanzhiStoneTotalCfg * GetZhuanzhiStoneTotalCfg(int total_stone_level);
	const ZhuanzhiStoneLevelCfg * GetZhuanzhiStoneLevelCfg(int old_stone_id);
	const ZhuanzhiStoneLevelCfg * GetZhuanzhiStoneLevelReverseCfg(int new_stone_id);
	const ZhuanzhiStoneSlotCfg * GetZhuanzhiStoneSlotCfg(int part_index, int slot_index);
	const ZhuanzhiStoneRefineCfg * GetZhuanzhiStoneRefineCfg(int refine_level);
	const RefineStuffCfg * GetRefineStuffCfg(int part_index, int seq);
	const StoneConvertCfg * GetStoneConvertCfg(int seq);

	int GetStoneResolveScore(int stone_id);
	int GetZhuanzhiStoneType(int stone_id);
	bool IsVipLimitSlot(int part_index, int slot_index);

	//套装相关-----------------------------------------------------------------------------
	const SuitOneEquipActiveCfg * GetSuitOneEquipActCfg(int role_prof, int suit_index, int equip_part, int equip_order);
	const SuitForgeLimitCfg * GetSuitForgeLimitCfg(int suit_index);
	const FangGroupAttrCfg * GetFangGroupAttrCfg(int role_prof, int suit_index, int equip_order, int same_order_num);
	const GongGroupAttrCfg * GetGongGroupAttrCfg(int suit_index, int same_order_num);
	const ZhizunGroupAttrCfg * GetZhizunGroupAttrCfg(int equip_order);

	int GetGroupType(int equip_index);
	bool IsActiveFangSuit(int role_prof, int suit_index, int equip_order, int same_order_num);

	//至尊装备相关-----------------------------------------------------------------------------
	const ZhiZunEquipCfg * GetZhiZunEquipCfg(ItemID equip_id);
	const EquipZhiZunComposeCfg * GetEquipZhiZunComposeCfg(ItemID equip_id, int best_attr_num);

	bool IsZhiZunEquip(ItemID equip_id);
	int IsZhizunEquipSuit(ItemID equip_id1, ItemID equip_id2);

	//特殊装备相关-----------------------------------------------------------------------------
	const SpecialEquipCfg * GetSpecialEquipCfg(ItemID equip_id);

	//觉醒相关---------------------------------------------------------------------------------
	wake_level_t GetWakeLevelLimitCfg(size_t equip_grade);
	wake_key_t MakeWakeTypeLevelKey(wake_type_t type, wake_level_t level);
	const AwakeningAttributeCfg *GetWakeAttributeCfg(wake_type_t type, wake_level_t level);
	AwakeningTypeWeightCfg GetWakeTypeWeightCfg(size_t part_index);
	int WakeTypeIsPrecious(size_t part_index, wake_type_t type);
	const AwakeningLevelWeightCfg::LevelRangeWeightCfg *GetWakeLevelWeightCfg(wake_level_t level);
	const AwakeningLockConsumeCfg *GetWakeLockConsumeCfg(size_t lock_num);

	int MakeComposeKey(ItemID item_id, short xianpin_num);
	const ZhuanzhiEquipComposeCfg * GetEquipComposeCfg(ItemID item_id, short xianpin_num);

	int MakeConditionAttrKey(int part_index, int order);
	const ZhuanzhiEquipConditionAttrCfg *GetConditionAttrCfg(int part_index, int order);

private:

	// 其他-----------------------------------------------------------------------------
	int InitOtherConfig(PugiXmlNode RootElement);         
	ZhuanzhiOtherConfig m_other_cfg;            

	// 转职装备相关-----------------------------------------------------------------------------
	int InitZhuanzhiEquipCfg(PugiXmlNode RootElement);
	int InitEquipSpecialEffectCfg(PugiXmlNode RootElement);

	typedef std::map<long long, ZhuanzhiEquipCfg> EquipPutOnLimitConfigMap;
	typedef std::map<long long, ZhuanzhiEquipCfg>::const_iterator EquipPutOnLimitConfigMapIt;
	EquipPutOnLimitConfigMap m_equip_puton_limit_cfg_map;

	std::vector<EquipSpecialEffectCfg> m_special_effect_vec;

	//升星相关-----------------------------------------------------------------------------
	int InitUpStarCfg(PugiXmlNode RootElement);
	int InitUpStarTotalCfg(PugiXmlNode RootElement);

	ZhuanzhiUpStarConfig m_up_star_list[Equipment::E_INDEX_MAX][MAX_STAR_LEVEL + 1];

	int m_up_star_total_cfg_count;
	ZhuanzhiUpStarTotalConfig m_up_star_total_cfg_list[TOTAL_STAR_CFG_MAX_COUNT];

	//附灵相关-----------------------------------------------------------------------------
	int InitFulingCfg(PugiXmlNode RootElement);
	int InitFulingStuffCfg(PugiXmlNode RootElement);

	ZhuanzhiEquipFulingCfg m_zhuanzhi_fuling_cfg_list[ItemPool::MAX_EQUIPMENT_ORDER];
	ZhuanzhiFulingStuffCfg m_fuling_stuff_cfg_list[MAX_ZHUANZHI_FULING_ATTR_COUNT];

	//玉石相关-----------------------------------------------------------------------------
	int InitZhuanzhiStoneCfg(PugiXmlNode RootElement);
	int InitZhuanzhiStoneTotalCfg(PugiXmlNode RootElement);
	int InitZhuanzhiStoneLevelCfg(PugiXmlNode RootElement);
	int InitZhuanzhiStoneSlotCfg(PugiXmlNode RootElement);
	int InitZhuanzhiStoneRefineCfg(PugiXmlNode RootElement);
	int InitRefineStuffCfg(PugiXmlNode RootElement);
	int InitStoneResolveCfg(PugiXmlNode RootElement);
	int InitStoneConvertCfg(PugiXmlNode RootElement);

	std::map<int, ZhuanzhiStoneCfg> m_zhuanzhi_stone_map;
	std::vector<ZhuanzhiStoneTotalCfg> m_zhuanzhi_stone_total_vec;
	std::vector<ZhuanzhiStoneLevelCfg> m_zhuanzhi_stone_level_vec;
	std::vector<ZhuanzhiStoneSlotCfg> m_zhuanzhi_stone_slot_vec;
	std::vector<ZhuanzhiStoneRefineCfg> m_zhuanzhi_stone_refine_vec;
	std::vector<RefineStuffCfg> m_refine_stuff_vec;
	std::map<int, StoneResolveCfg> m_stone_resolve_map;
	std::map<int, StoneConvertCfg> m_stone_convert_map;

	//套装相关-----------------------------------------------------------------------------
	int InitSuitGroupTypeCfg(PugiXmlNode RootElement);
	int InitSuitForgeLimitCfg(PugiXmlNode RootElement);
	int InitOneEquipActConfig(PugiXmlNode RootElement);
	int InitFangGroupAttrCfg(PugiXmlNode RootElement);
	int InitGongGroupAttrCfg(PugiXmlNode RootElement);
	int InitZhizunGroupAttrCfg(PugiXmlNode RootElement);

	SuitGroupTypeCfg m_suit_group_cfg_list[E_INDEX_MAX];
	SuitForgeLimitCfg m_suit_forge_limit_list[ZHUANZHI_SUIT_TYPE_MAX];
	std::vector<SuitOneEquipActiveCfg> m_suit_active_cfg_vec;
	std::vector<FangGroupAttrCfg> m_suit_fang_group_vec;
	std::vector<GongGroupAttrCfg> m_suit_gong_group_vec;
	std::vector<ZhizunGroupAttrCfg> m_suit_zhizun_group_vec;

	//至尊装备相关-----------------------------------------------------------------------------
	int InitZhiZunEquipCfg(PugiXmlNode RootElement);
	int InitEquipZhiZunComposeCfg(PugiXmlNode RootElement);

	std::map<ItemID, ZhiZunEquipCfg> m_zhizun_cfg_map;
	std::vector<EquipZhiZunComposeCfg> m_zhizun_compose_vec;

	//特殊装备相关-----------------------------------------------------------------------------
	int InitSpecialEquipCfg(PugiXmlNode RootElement);

	std::map<ItemID, SpecialEquipCfg> m_special_equip_cfg_map;

	//觉醒相关-----------------------------------------------------------------------------
	int InitWakeLevelLimitCfg(PugiXmlNode RootElement);
	int InitWakeAttributeCfg(PugiXmlNode RootElement);
	int InitWakeTypeWeightCfg(PugiXmlNode RootElement);
	int InitWakeLevelWeightCfg(PugiXmlNode RootElement);
	int InitWakeLockConsumeCfg(PugiXmlNode RootElement);
	std::vector<wake_level_t> m_awakening_level_limit_vec;				//觉醒等级限制	
	std::map<wake_key_t, AwakeningAttributeCfg> m_awakening_attribute_map;		//觉醒属性
	std::vector<AwakeningTypeWeightCfg> m_awakening_type_weight_vec;	//觉醒类型权重
	AwakeningLevelWeightCfg m_awakening_level_weight_cfg;				//觉醒等级权重
	std::vector<AwakeningLockConsumeCfg> m_awakening_lock_consume_vec;	//觉醒消耗

	//装备合成
	int InitEquipComposeCfg(PugiXmlNode RootElement);
	std::map<int, ZhuanzhiEquipComposeCfg> m_zhuanzhi_equip_compose_map;

	//满足多少仙品，多少阶的属性加成配置
	
	int InitConditionAttrCfg(PugiXmlNode RootElement);
	std::map<int, ZhuanzhiEquipConditionAttrCfg> m_condition_attr_cfg_map;
};

#endif




