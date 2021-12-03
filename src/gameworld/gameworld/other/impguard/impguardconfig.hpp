#ifndef __IMP_GUARD_CONFIG_HPP__
#define __IMP_GUARD_CONFIG_HPP__

#include "servercommon/serverdef.h"
#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

static const int IMP_GUARD_MAX_IMP_TYPE = 4;
static const int TWELVE_HOURS = 12;		// 12Сʱ

struct ImpGuardOtherCfg
{
	ImpGuardOtherCfg() : free_imp_type(-1), free_imp_item_id(0), range(0)
	{
	}

	int free_imp_type;
	int free_imp_item_id;
	Axis range;
};

struct ImpConfig
{
	ImpConfig() : imp_type(0), item_id(0), add_per_exp(0), per_mianshang(0), fangyu(0), max_hp(0), fujia_shanghai(0), per_kangbao(0),
		pvp_reduce_hurt_per(0), gold_price(0), bind_gold_price(0), appe_image_id(0), level_limit(0), use_day(0) {}

	int imp_type;
	ItemID item_id;
	int add_per_exp;
	Attribute per_mianshang;
	Attribute fangyu;
	Attribute max_hp;
	Attribute fujia_shanghai;
	Attribute per_kangbao;
	Attribute pvp_reduce_hurt_per;
	int gold_price;
	int bind_gold_price;
	int appe_image_id;
	int level_limit;
	int use_day;
};

class ImpGuardConfig : public ConfigBase
{
public:
	ImpGuardConfig();
	~ImpGuardConfig();

	bool Init(const std::string &configname, std::string *err);

	ImpGuardOtherCfg &GetOtherCfg() { return m_other_cfg; }
	const ImpConfig *GetImpCfgByImpType(int imp_type) const;
	const ImpConfig *GetImpCfgByItemId(ItemID item_id) const;
private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitImpCfg(PugiXmlNode RootElement);

	ImpGuardOtherCfg m_other_cfg;
	ImpConfig m_imp_cfg_list[IMP_GUARD_MAX_IMP_TYPE + 1];
};
#endif