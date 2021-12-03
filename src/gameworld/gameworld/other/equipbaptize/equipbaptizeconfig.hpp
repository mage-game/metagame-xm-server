#ifndef __EQUIP_BAPTIZE_CONFIG_HPP__
#define __EQUIP_BAPTIZE_CONFIG_HPP__

#include <map>
#include <vector>

#include "servercommon/struct/itemlistparam.h"
#include "obj/character/attribute.hpp"
#include "servercommon/basedef.h"
#include "servercommon/configcommon.h"
#include "servercommon/equipbaptizedef.hpp"
#include "servercommon/xml_adapter.hpp"
#include "servercommon/configbase.h"

enum EQUIP_XIANPIN_BIG_TYPE
{
	EQUIP_XIANPIN_BIG_TYPE_GONGJI = 0,	// 攻击类
	EQUIP_XIANPIN_BIG_TYPE_FANGYU,		// 防御类
	EQUIP_XIANPIN_BIG_TYPE_SPECIAL,		// 特殊类

	EQUIP_XIANPIN_BIG_TYPE_MAX,
};

struct EquipBaptizeOtherConfig
{
	EquipBaptizeOtherConfig() : everyday_free_times(0) {}
	
	int everyday_free_times;
};

struct EquipBaptizeOpenConfig
{
	EquipBaptizeOpenConfig() : equip_part(0), role_level(0) {}

	int equip_part;
	int role_level;
};

struct EquipBaptizeConsumeConfig
{
	EquipBaptizeConsumeConfig() : index(0), consume_gold(0) {}
	
	int index;
	int consume_gold;
};

struct EquipBaptizeLockConfig
{
	EquipBaptizeLockConfig() : lock_num(0), consume_stuff_id(0), consume_stuff_num(0), purple_stuff_id(0), purple_stuff_num(0), orange_stuff_id(0),
							  orange_stuff_num(0), red_stuff_id(0), red_stuff_num(0) {}

	int lock_num;
	int consume_stuff_id;
	int consume_stuff_num;

	int purple_stuff_id;
	int purple_stuff_num;

	int orange_stuff_id;
	int orange_stuff_num;

	int red_stuff_id;
	int red_stuff_num;
};

struct SignColorConfig
{
	SignColorConfig() : color_weight(0), attr_val_low(0), attr_val_high(0) {}

	int color_weight;
	int attr_val_low;
	int attr_val_high;
};

struct EquipBigTypeConfig
{
	EquipBigTypeConfig() : equip_index(0), equip_big_type(0) {}
	int equip_index;
	int equip_big_type;
};

struct EquipBaptizeAttrConfig
{
	EquipBaptizeAttrConfig() : special_type(0), seq(0), big_type(0), weight(0), attr_type(0) {}

	int special_type;
	int seq;
	int big_type;
	int weight;
	short attr_type;

	SignColorConfig color_list[EQUIP_BAPTIZE_ATTR_COLOR_MAX_NUM];
};

struct EquipBaptizeSuitAttrConfig
{
	EquipBaptizeSuitAttrConfig() : equip_part(0) {}

	struct EquipPartAttrCfg
	{
		EquipPartAttrCfg() : baptize_color(-1), baptize_count(0), attr_type(0), attr_value(0){}
		int baptize_color;
		int baptize_count;
		short attr_type;
		Attribute attr_value;
	};
	
	int equip_part;
	std::vector<EquipPartAttrCfg> equip_attr_cfg;
};

struct EquipBaptizeBroadcastConfig
{
	EquipBaptizeBroadcastConfig() : seq(0), orange_num(0), red_num(0), is_broadcast(false) {}
	
	int seq;
	int orange_num;
	int red_num;
	bool is_broadcast;
};

class EquipBaptizeConfig : public ConfigBase
{
public:
	EquipBaptizeConfig();
	~EquipBaptizeConfig();

	enum BAPTIZE_ATTR_COLOR
	{
		WHITE = 0,
		BLUE,
		PURPLE,
		ORANGE,
		RED,

		MAX,
	};
	UNSTD_STATIC_CHECK(MAX == EQUIP_BAPTIZE_ATTR_COLOR_MAX_NUM);

	bool Init(const std::string &configname, std::string *err);

	const EquipBaptizeOtherConfig & GetOtherCfg() { return m_other_cfg; }
	const EquipBaptizeOpenConfig * GetOpenCfg(int equip_part);
	const EquipBaptizeConsumeConfig * GetConsumeCfg(int index);
	const EquipBaptizeLockConfig * GetLockCfg(int lock_num);
	const EquipBaptizeAttrConfig * GetAttrCfg(int seq, int big_type);
	const EquipBaptizeAttrConfig * GetReCalcAttrCfg(int special_type, int seq);
	const EquipBaptizeBroadcastConfig * GetBroadcastCfg(int orange_num, int red_num);

	int GetAttrColorBySeqAndValue(int seq, int attr_value);
	int GetBaptizeCount() { return m_baptize_count; }

	const EquipBigTypeConfig * GetEquipBigType(int equip_index);
	Attribute GetBaptizeSuitAttrValue(int equip_index, int baptize_color_num[EQUIP_BAPTIZE_ATTR_COLOR_MAX_NUM], int &baptize_color);
	int GetBaptizeSuitAttrType(int equip_index, int baptize_color);

private:
	int InitOtherCfg(YY_XML_FUNCTION_NODE_TYPE RootElement);
	int InitOpenCfg(YY_XML_FUNCTION_NODE_TYPE RootElement);
	int InitConsumeCfg(YY_XML_FUNCTION_NODE_TYPE RootElement);
	int InitLockCfg(YY_XML_FUNCTION_NODE_TYPE RootElement);
	int InitEquipBigTypeCfg(YY_XML_FUNCTION_NODE_TYPE RootElement);
	int InitAttrCfg(YY_XML_FUNCTION_NODE_TYPE RootElement);
	int InitSuitCfg(YY_XML_FUNCTION_NODE_TYPE RootElement);
	int InitBroadcastCfg(YY_XML_FUNCTION_NODE_TYPE RootElement);
	
	EquipBaptizeOtherConfig m_other_cfg;

	EquipBaptizeOpenConfig m_open_cfg_list[EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM];

	int m_baptize_count;
	EquipBaptizeConsumeConfig m_consume_cfg_list[EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM];

	EquipBaptizeLockConfig m_lock_cfg_list[EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM];

	EquipBigTypeConfig m_equip_big_type_list[EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM];

	typedef std::map<long long, EquipBaptizeAttrConfig> EquipBaptizeAttrConfigMap;
	typedef std::map<long long, EquipBaptizeAttrConfig>::const_iterator EquipBaptizeAttrConfigMapIt;
	EquipBaptizeAttrConfigMap m_attr_cfg_map[EQUIP_BAPTIZE_SPECIAL_TYPE_MAX];
	EquipBaptizeAttrConfigMap m_recalc_attr_map[EQUIP_BAPTIZE_SPECIAL_TYPE_MAX]; // 根据seq用来计算战力

	std::vector<EquipBaptizeBroadcastConfig> m_broadcast_cfg_vec;

	EquipBaptizeSuitAttrConfig m_baptize_suit_attr_list[EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM];
};

#endif