#ifndef _ZHUANSHENGEQUIP_CONFIG_HPP_
#define _ZHUANSHENGEQUIP_CONFIG_HPP_

#include "common/tinyxml/tinyxml.h"
#include "servercommon/serverdef.h"
#include "item/itembase.h"
#include "servercommon/zhuanshengequipdef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include <map>

struct ZhuanshengOtherConfig
{
	ZhuanshengOtherConfig() : common_roll_cost(0), super_roll_cost(0), super_roll_cost_item_id(0), free_cold_down_time(0), grade_3_attr_add_percent_8(0), grade_5_attr_add_percent_8(0){}

	int common_roll_cost;						// һ���黨�ѽ��
	int super_roll_cost;						// ʮ���黨�ѽ��
	int super_roll_cost_item_id;				// ʮ���黨����Ʒ

	int free_cold_down_time;					// ��ѵ�����ȴʱ�� ��
	int grade_3_attr_add_percent_8;				// 8��������װ���Լӳɰٷֱ�
	int grade_5_attr_add_percent_8;				// 8��������װ���Լӳɰٷֱ�
};

struct ZhuanshengLevelConfig
{
	ZhuanshengLevelConfig(): level(0), consume_xiuwei(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0),extra_exp_add(0) {}

	int level;							// ת���ȼ�
	int consume_xiuwei;					// ������Ϊ

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	int extra_exp_add;

};

struct ZhuanshengEquipInfoConfig
{
	ZhuanshengEquipInfoConfig() : equip_id(0), type(0), part_index(0), item_id(ItemBase::INVALID_ITEM_ID), grade(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0) {}

	int equip_id;						// װ��id
	int type;
	int part_index;						// ��λ�±�
	ItemID item_id;						// ת������ƷID
	int grade;							// ����

	int maxhp;	
	int gongji;
	int fangyu;
	int mingzhong;
	int shanbi;
	int baoji;
	int jianren;
};

struct ZhuanshengXiuweiConfig
{
	ZhuanshengXiuweiConfig() : seq(0), consume_gold(0), consume_coin(0), reward_xiuwei(0) {}
	
	int seq;							// ����
	int consume_gold;					// ����Ԫ��
	int consume_coin;					// ���Ľ��
	int reward_xiuwei;					// �õ���Ϊ

};

struct ZhuanshengChouConfig
{
	ZhuanshengChouConfig() : seq(0), reward_equip_id(0), weight(0) {}

	int seq;
	int reward_equip_id;
	int weight;
};

struct ZhuanshengChouSectionConfig
{
	ZhuanshengChouSectionConfig() : section_start(0), section_end(0), total_weight(0), chou_cfg_count(0) {}

	int section_start;
	int section_end;

	int total_weight;
	int chou_cfg_count;
	ZhuanshengChouConfig chou_cfg_list[ZHUANSHENG_CHOU_CFG_LIMIT + 1];
};

enum ZhuanShengChouType
{
	ZHUANSHENG_CHOU_TYPE_MIN = 0,
	ZHUANSHENG_CHOU_TYPE_COMMON_ROLL = 1,		// һ��
	ZHUANSHENG_CHOU_TYPE_SUPER_ROLL = 2,		// ʮ��
	ZHUANSHENG_CHOU_TYPE_MAX,
};

enum ZhuanShengEquipType
{
	ZHUANSHENG_EQUIP_INFO_TYPE_MIN = 0,
	ZHUANSHENG_EQUIP_INFO_TYPE_EQUIP = 1,			// װ������
	ZHUANSHENG_EQUIP_INFO_TYPE_XIUWEI_DAN = 2,		// ��Ϊ������
	ZHUANSHENG_EQUIP_INFO_TYPE_MAX,
};

enum ZHUANSHENG_EQUIP_COLOR_TYPE
{
	ZHUANSHENG_EQUIP_COLOR_TYPE_MIN = 0,
	ZHUANSHENG_EQUIP_COLOR_TYPE_GREEN,
	ZHUANSHENG_EQUIP_COLOR_TYPE_BLUE,
	ZHUANSHENG_EQUIP_COLOR_TYPE_PURPLE,
	ZHUANSHENG_EQUIP_COLOR_TYPE_ORANGE,
	ZHUANSHENG_EQUIP_COLOR_TYPE_RED,
	ZHUANSHENG_EQUIP_COLOR_TYPE_MAX,
};

struct ZhuanShengEquipRandAttrCountCfg
{
	ZhuanShengEquipRandAttrCountCfg() : rand_attr_0_rate(0), rand_attr_1_rate(0), rand_attr_2_rate(0), rand_attr_3_rate(0) {}

	int rand_attr_0_rate;
	int rand_attr_1_rate;
	int rand_attr_2_rate;
	int rand_attr_3_rate;
};

struct ZhuanShengEquipRandAttrRate
{
	ZhuanShengEquipRandAttrRate() : attr_type(0), rate(0) {}

	int attr_type;
	int rate;
};

static const int MAX_ZHUANSHENG_EQUIP_RAND_ATTR_MAX = 15;

struct ZhuanShengEquipRandAttrVal
{
	ZhuanShengEquipRandAttrVal() : attr_type(0), attr_val_max(0), attr_val_min(0) {}

	int attr_type;
	int attr_val_max;
	int attr_val_min;
};

struct ZhuanShengEquipColorAttrValCfg
{
	ZhuanShengEquipColorAttrValCfg() : attr_count(0), equip_color(0) {}

	int attr_count;
	int equip_color;
	ZhuanShengEquipRandAttrVal attr_val_list[MAX_ZHUANSHENG_EQUIP_RAND_ATTR_MAX];
};

struct ZhuanShengEquipRandAttrValCfg
{
	ZhuanShengEquipColorAttrValCfg color_attr_list[ZHUANSHENG_EQUIP_COLOR_TYPE_MAX - 1];
};

struct ZhuanShengEquipSuitAddAttr
{
	ZhuanShengEquipSuitAddAttr() : grade(0), max_hp(0), gongji(0), fangyu(0), min_gongji(0), baoji(0), per_pofang(0), per_mianshang(0),
									per_to_role_shang(0), per_to_role_mianshang(0) {}

	int grade;
	Attribute max_hp;
	Attribute gongji;
	Attribute fangyu;
	Attribute min_gongji;
	Attribute baoji;
	Attribute per_pofang;
	Attribute per_mianshang;
	int per_to_role_shang;
	int per_to_role_mianshang;
};

class ZhuanShengEquipConfig : public ConfigBase
{
public:
	ZhuanShengEquipConfig();
	~ZhuanShengEquipConfig();

	bool Init(const std::string &configname, std::string *err);
	
	const int GetEquipIDByItemID(ItemID item_id);
	const int GetXiuweiChangeTimes() { return m_xiuwei_cfg_count; }

	const ZhuanshengOtherConfig & GetOtherCfg() { return m_other_cfg; }
	const ZhuanshengLevelConfig * GetLevelCfg(int level);
	const ZhuanshengXiuweiConfig * GetXiuweiCfg(int times);
	const ZhuanshengEquipInfoConfig * GetRandomCombineEquip(int grade);
	const ZhuanshengChouConfig * GetRandomChouResult();
	const ZhuanShengEquipSuitAddAttr * GetEquipSuitAddAttr(int grade);
	
	int GetZhuanshengRandAttrCount(int type);
	void GetZhuanshengRandAttrType(int attr_count, char rand_attr_type_list[ItemParamDataStruct::RAND_ATTR_NUM]);
	int GetZhuanshengRandAttrValue(int equip_level, int equip_color, int attr_type);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitZhuanShenglevelCfg(PugiXmlNode RootElement);
	int InitChangeXiuweiCfg(PugiXmlNode RootElement);
	int InitChouCfg(PugiXmlNode RootElement);
	int InitRandAttrCountCfg(PugiXmlNode RootElement);
	int InitRandAttrRateCfg(PugiXmlNode RootElement);
	int InitRandAttrValCfg(PugiXmlNode RootElement);
	int InitEquipSuitAddAttrCfg(PugiXmlNode RootElement);

	ZhuanshengOtherConfig m_other_cfg;

	int m_level_cfg_count;
	ZhuanshengLevelConfig m_level_cfg_list[ZHUANSHENG_ATTR_LEVEL_LIMIT];

	int m_xiuwei_cfg_count;
	ZhuanshengXiuweiConfig m_xiuwei_cfg_list[ZHUANSHENG_XIUWEI_CFG_LIMIT];

	int m_chou_section_cfg_max_count;
	ZhuanshengChouSectionConfig m_chou_section_cfg_list[ZHUANSHENG_EQUIP_SECTION_MAX_COUNT];

	ZhuanShengEquipRandAttrCountCfg m_rand_attr_count_cfg_list[ZHUANSHENG_EQUIP_COLOR_TYPE_MAX - 1];

	int m_rand_attr_rate_count;
	ZhuanShengEquipRandAttrRate m_rand_attr_rate_list[MAX_ZHUANSHENG_EQUIP_RAND_ATTR_MAX];
	int m_rand_attr_total_rate;

	std::map<int, ZhuanShengEquipRandAttrValCfg> m_rand_attr_val_cfg_map;

	ZhuanShengEquipSuitAddAttr m_equip_suit_add_attr_list[ZHUANSHENG_EQUIP_GRADE_LIMIT + 1];
};

#endif

