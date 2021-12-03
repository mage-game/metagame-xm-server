#pragma once
#ifndef __XIANZUNKA_CONFIG_HPP__
#define __XIANZUNKA_CONFIG_HPP__
#include <string>
#include "servercommon/configbase.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/xianzunkadef.hpp"
#include "servercommon/pugixml/pugixml_adapter.hpp"

// œ…◊ø®∆‰À˚≈‰÷√
struct XianZunKaOtherCfg
{
	XianZunKaOtherCfg() : active_attribute_type(0), active_attribute_value(0){}

	int active_attribute_type;
	Attribute active_attribute_value;
};

// œ…◊ø®ª˘¥°≈‰÷√
struct XianZunKaBaseConfig
{
	XianZunKaBaseConfig() : card_type(0), valid_day(0), free_time_limit(0), gift_day(0), need_gold(0), daily_reward_bind_gold(0), active_item_id(0), active_chong_zhi(0){}

	int card_type;
	int valid_day;
	time_t free_time_limit;
	int gift_day;
	int need_gold;
	ItemConfigData first_reward_item;
	int daily_reward_bind_gold;
	int active_item_id;
	int active_chong_zhi;
};

// œ…◊ø®∏Ωº”≈‰÷√
struct XianZunKaAdditionConfig
{
	XianZunKaAdditionConfig() : card_type(0), add_exp_per(0), add_equip_strength_succ_rate(0), add_mount_grade_attrs_per(0), 
		add_wing_grade_attrs_per(0), add_shenbing_grade_attrs_per(0), add_shizhuang_grade_attrs_per(0) {}

	int card_type;
	int add_exp_per;
	int add_equip_strength_succ_rate;
	int add_mount_grade_attrs_per;
	int add_wing_grade_attrs_per;
	int add_shenbing_grade_attrs_per;
	int add_shizhuang_grade_attrs_per;
};

class XianZunKaConfig : public ConfigBase
{
public:
	XianZunKaConfig();
	virtual ~XianZunKaConfig();

	bool Init(const std::string &path, std::string *err);

	const XianZunKaOtherCfg & GetOtherCfg();
	const XianZunKaBaseConfig * GetXianZunKaBaseCfg(int card_type);
	const XianZunKaAdditionConfig * GetXianZunKaAdditionCfg(int card_type);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitXianZunKaCfg(PugiXmlNode RootElement);
	int InitXianZunKaAdditionCfg(PugiXmlNode RootElement);

	XianZunKaOtherCfg m_other_cfg;
	XianZunKaBaseConfig m_xianzunka_cfg_list[XIANZUNKA_TYPE_MAX];
	XianZunKaAdditionConfig m_xianzunka_addition_cfg_list[XIANZUNKA_TYPE_MAX];
};

#endif