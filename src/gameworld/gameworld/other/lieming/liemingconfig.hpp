#ifndef _LIEMING_CFG_HPP_
#define _LIEMING_CFG_HPP_

#include <string>
#include <map>
#include "servercommon/serverdef.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/liemingdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "obj/character/role.h"

struct LieMingOtherConfig
{
	LieMingOtherConfig() : hunshou_exp_discount_rate(0), exp_hunshou_exp_val(0), super_chouhun_price(0) {}

	int hunshou_exp_discount_rate;
	int exp_hunshou_exp_val;
	int super_chouhun_price;
};


struct LieMingHunshouConfig
{
	LieMingHunshouConfig() : item_id(0), hunshou_id(0), hunshou_type(0), hunshou_color(0), chouhun_weight(0) {}

	ItemID item_id;
	int hunshou_id;
	int hunshou_type;
	int hunshou_color;
	int chouhun_weight;
};

struct LieMingChouhunConfig
{
	LieMingChouhunConfig() : chouhun_color(0), cost_hunli(0), advance_rate(0)
	{
		memset(rate_list, 0, sizeof(rate_list));
	}

	int chouhun_color;
	int cost_hunli;
	int advance_rate;

	int rate_list[LIEMING_HUNSHOU_COLOR_MAX];
};

struct LieMingHunshouLevelConfigItem
{
	LieMingHunshouLevelConfigItem() : color(0), level(0), need_exp(0), total_exp(0)
	{
		memset(attr_val_list, 0, sizeof(attr_val_list));
	}

	int color;
	int level;
	int need_exp;
	int total_exp;

	Attribute attr_val_list[LIEMING_HUNSHOU_TYPE_MAX_COUNT];
};

struct LieMingHunshouLevelConfig
{
	LieMingHunshouLevelConfig() : color(0), max_level(-1) {}

	int color;

	int max_level;
	LieMingHunshouLevelConfigItem level_cfglist[LIEMING_HUNSHOU_MAX_LEVEL + 1];
};

struct LieMingChouhunHunshouWeightCfg
{
	struct ConfigItem
	{
		ConfigItem() : hunshou_id(0), weight(0) {}

		int hunshou_id;
		int weight;
	};

	LieMingChouhunHunshouWeightCfg() : total_weight(0), cfg_count(0) {}

	int total_weight;

	int cfg_count;
	ConfigItem cfg_list[LIEMING_HUNSHOU_PER_COLOR_MAX_COUNT];

};

struct LieMingJihuoCfg
{
	LieMingJihuoCfg():role_level(0),active_mingge_count(0){}

	short role_level;
	short active_mingge_count;

};

class LieMingConfig : public ConfigBase
{
public:
	LieMingConfig();
	~LieMingConfig();

	bool Init(const std::string &configname, std::string *err);

	const LieMingOtherConfig & GetOtherCfg() { return m_other_cfg; }
	//const LieMingHunshouConfig * GetHunshouCfg(int hunshou_id);
	const LieMingHunshouConfig * GetHunshouCfg(ItemID item_id);
	const LieMingHunshouLevelConfigItem * GetHunshouLevelCfg(int color, int level);
	const LieMingChouhunConfig * GetChouhunCfg(int chouhun_color);
	long long CalSellExp(int hunshou_id, int level, long long curr_exp);
	const LieMingChouhunHunshouWeightCfg * GetChouhunHunshouWeightCfg(int hunshou_color);
	int GetHunshouMaxLevel(int hunshou_color);
	//bool GetHunshouAddAttr(int hunshou_id, int hunshou_level, int *attr_type, int *attr_val);
	bool GetHunshouAddAttr(ItemID item_id, int hunshou_level, int *attr_type, Attribute *attr_val);
	void ActiveSoltByRoleLevel(Role *role, int level);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitHunshouCfg(PugiXmlNode RootElement);
	int InitHunshouExpCfg(PugiXmlNode RootElement);
	int InitChouhunCfg(PugiXmlNode RootElement);
	int InitLieMingJihuoCfg(PugiXmlNode RootElement);

	LieMingOtherConfig m_other_cfg;

	int m_hunshou_type_attr_map[LIEMING_HUNSHOU_TYPE_MAX_COUNT];										// ªÍ ﬁ”ÎÀ˘º” Ù–‘”≥…‰
	int m_max_hunshou_id;																				// ◊Ó¥ÛªÍ ﬁID
	LieMingHunshouConfig m_hunshou_cfglist[LIEMING_HUNSHOU_MAX_ID + 1];									// ªÍ ﬁ≈‰÷√
	LieMingHunshouLevelConfig m_hunshou_level_cfglist[LIEMING_HUNSHOU_COLOR_MAX];						// ªÍ ﬁµ»º∂≈‰÷√
	LieMingChouhunConfig m_chouhun_cfglist[LIEMING_CHOUHUN_COLOR_MAX];									// ≥ÈªÍ∏≈¬ 
	LieMingChouhunHunshouWeightCfg m_chouhun_hunshou_weight_cfglist[LIEMING_HUNSHOU_COLOR_MAX];			// ªÍ ﬁ≥ÈªÍ»®÷ÿ
	LieMingJihuoCfg m_jihuo_cfg[LIEMING_FUHUN_SLOT_COUNT];												// ªÍ∏Òº§ªÓÃıº˛≈‰÷√

};

#endif


