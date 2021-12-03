#ifndef __CROSS_EQUIP_CONFIG_HPP__
#define __CROSS_EQUIP_CONFIG_HPP__

#include "servercommon/crossequipdef.hpp"
#include "servercommon/configbase.h"

#define CROSS_EQUIP_CFG() LOGIC_CONFIG->GetCrossEquipConfig()

class TiXmlElement;

enum CROSS_EQUIP_ROLL_TYPE
{
	CROSS_EQUIP_ROLL_TYPE_BEGIN,

	CROSS_EQUIP_ROLL_TYPE_1,		// 初级
	CROSS_EQUIP_ROLL_TYPE_2,		// 中级
	CROSS_EQUIP_ROLL_TYPE_3,		// 高级

	CROSS_EQUIP_ROLL_TYPE_MAX,
};

enum CROSS_EQUIP_ROLL_TIMES_TYPE
{
	CROSS_EQUIP_ROLL_COUNTS_TYPE_BEGIN,

	CROSS_EQUIP_ROLL_COUNTS_TYPE_ONE_TIMES ,				// 1抽
	CROSS_EQUIP_ROLL_COUNTS_TYPE_TEN_TIMES,					// 
	CROSS_EQUIP_ROLL_COUNTS_TYPE_THIRTY_TIMES,				//

	CROSS_EQUIP_ROLL_COUNTS_TYPE_MAX,
};

struct CrossEquipDouqiXiulianCfg
{
	CrossEquipDouqiXiulianCfg() :
		douqi_grade(0), xiulian_times(0),
		need_mojing(0), reward_exp(0)
	{}

	CrossEquipDouqiXiulianCfg(short __douqi_grade, short __xiulian_times, int __need_mojing, int __reward_exp) :
		douqi_grade(__douqi_grade), xiulian_times(__xiulian_times),
		need_mojing(__need_mojing), reward_exp(__reward_exp)
	{}

	short douqi_grade = 0;
	short xiulian_times = 0;

	int need_mojing = 0;
	int reward_exp = 0;
};

struct CrossEquipDouqiCfg
{
	short douqi_grade = 0;
	short equip_level = 0;

	int maxhp = 0;
	int gongji = 0;
	int fangyu = 0;
	int mingzhong = 0;
	int shanbi = 0;
	int baoji = 0;
	int jianren = 0;

	bool is_broadcast = false;
	int upgrade_need_exp = 0;

	std::vector<CrossEquipDouqiXiulianCfg> douqi_xiulian_vec;
};

struct CrossEquipDouqiDanCfg
{
	short douqidan_type = 0;
	short used_times_per_day = 0;

	int reward_exp = 0;
};

struct CrossEquipRollItemCfg
{
	CrossEquipRollItemCfg() : item_cfg(0, false, 0), weight(0)
	{}

	CrossEquipRollItemCfg(ItemConfigData &__item_cfg, int __weight) : item_cfg(__item_cfg), weight(__weight)
	{}

	ItemConfigData item_cfg;
	int weight = 0;
};

struct CrossEquipLianzhiCfg
{
	CrossEquipLianzhiCfg() :
		equip_id(0), need_fragment(0)
	{}

	CrossEquipLianzhiCfg(ItemID __equip_id, int __need_fragment) :
		equip_id(__equip_id), need_fragment(__need_fragment)
	{}

	ItemID equip_id = 0;
	int need_fragment = 0;
};

// 套装属性
struct CrossEquipSuitAttrCfg
{
	CrossEquipSuitAttrCfg() {}

	int order = 0;
	int color = 0;
	int need_count = 0;

	AttrConfig attr_cfg;
	Attribute per_mingzhong = 0;
	Attribute per_shanbi = 0;
	Attribute skill_zengshang_per = 0;
	Attribute skill_jianshang_per = 0;
};


struct CrossEquipRollCfg
{
	short roll_type = 0;
	short need_grade = 0;

	short roll_times[CROSS_EQUIP_ROLL_TYPE_MAX] = { 0 };
	short consume[CROSS_EQUIP_ROLL_TYPE_MAX] = { 0 };
	short reward_chuanshi_score[CROSS_EQUIP_ROLL_TYPE_MAX] = { 0 };

	std::vector<CrossEquipRollItemCfg> roll_item_vec;
};

class CrossEquipConfig : public ConfigBase
{
public:
	CrossEquipConfig();
	~CrossEquipConfig();

	bool Init(const std::string &configname, std::string *err);

	bool IsValidCrossEquipDouqiGrade(int douqi_grade) { return douqi_grade >= 0 && douqi_grade <= m_max_grade; };
	bool IsValidCrossEquipDouqidanType(short douqidan_type) 
	{ return douqidan_type >= CROSS_EQUIP_DOUQIDAN_TYPE_BEGIN && douqidan_type < CROSS_EQUIP_DOUQIDAN_TYPE_MAX; }

	const CrossEquipDouqiCfg * GetCrossEquipDouqi(short douqi_grade);
	const CrossEquipDouqiXiulianCfg * GetCrossEquipDouqiXiulianCfg(short douqi_grade, short today_xiulian_times);
	const CrossEquipDouqiDanCfg * GetCrossEquipDouqiDan(short douqidan_type);
	const CrossEquipRollCfg * GetCrossEquipRoll(short roll_type);
	bool GetCrossEquipRollReward(short roll_type, ItemConfigData *reward_cfg);
	short GetEquipGrade(ItemID equip_id);
	const CrossEquipLianzhiCfg * GetLianzhiCfg(short prof, short grade, short equip_index);

	//套装相关-----------------------------------------------------------------------------
	bool IsCalcSuit(int equip_order, int color);
	const CrossEquipSuitAttrCfg * GetSuitAttrCfg(int equip_order, int color, int same_order_num);

private:
	int InitCrossEquipDouqi(TiXmlElement *root_element);
	int InitCrossEquipDouqiXiulian(TiXmlElement *root_element);
	int InitCrossEquipDouqiDan(TiXmlElement *root_element);

	int InitCrossEquipRoll(TiXmlElement *root_element);
	int InitCrossEquipRollItem(TiXmlElement *root_element);

	int InitCrossEquipEquipInfo(TiXmlElement *root_element);

	short m_max_grade;

	std::map<short, CrossEquipDouqiCfg> m_cross_equip_douqi_map;
	std::map<short, CrossEquipDouqiDanCfg> m_cross_equip_douqidan_map;
	std::map<short, CrossEquipRollCfg> m_cross_equip_roll_map;

	std::map<ItemID, short> m_cross_equip_equip_need_grade_map;
	std::map<long long, CrossEquipLianzhiCfg> m_cross_equip_lianzhi_info_map;

	//套装相关-----------------------------------------------------------------------------
	int InitSuitAttrCfg(TiXmlElement *RootElement);
	std::vector<CrossEquipSuitAttrCfg> m_suit_attr_vec;
};

#endif // __CROSS_EQUIP_CONFIG_HPP__