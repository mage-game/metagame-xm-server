#ifndef __HESHENLUOSHUCONFIG_HPP__
#define __HESHENLUOSHUCONFIG_HPP__

#include "servercommon/heshenluoshudef.hpp"
#include <map>
//#include "other/juhun/juhunconfig.hpp"
#include "item/itembase.h"
#include "servercommon/configbase.h"
#include "servercommon/xml_adapter.hpp"
//#include "common/tinyxml/tinyxml.h"


// ∆‰À˚≈‰÷√
struct HeShenLuoShuOtherConfig
{
	HeShenLuoShuOtherConfig() : open_level(0), max_star_level(0) {}

	int open_level;
	int max_star_level;
};

// Ã◊◊∞≈‰÷√
struct HeShenLuoShuSuiteAttr
{
	struct SuiteAttr
	{
		SuiteAttr() : suit_num(0), maxhp(0), gongji(0), fangyu(0), fa_fangyu(0), per_baoji(0), baoji(0), per_kangbao(0), fujiashanghai(0), dikangshanghai(0), pojia(0),
			skill_jianshang_per(0), per_pofang(0), per_mianshang(0), skill_zengshang_per(0)
		{
		}

		int suit_num;
		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute fa_fangyu;
		Attribute per_baoji;
		Attribute baoji;
		Attribute per_kangbao;
		Attribute fujiashanghai;
		Attribute dikangshanghai;
		Attribute pojia;
		Attribute skill_jianshang_per;
		Attribute per_pofang;
		Attribute per_mianshang;
		Attribute skill_zengshang_per;
	};

	SuiteAttr suit_attr[HESHENLUOSHU_SUIT_MAX_NUM];
};

// Õºº¯–«–«≈‰÷√
struct HeShenLuoShuItemStar
{
	struct ItemStarCfg
	{
		ItemStarCfg() :stuff_1(0), stuff_2(0), stuff_3(0), stuff_4(0), consume_jinghua(0), star_level(0), color(0), maxhp(0), gongji(0), fangyu(0), fa_fangyu(0), per_baoji(0), baoji(0), per_kangbao(0), fujiashanghai(0), dikangshanghai(0), pojia(0)
		{
		}
		int stuff_1;
		int stuff_2;
		int stuff_3;
		int stuff_4;
		int consume_jinghua;
		int star_level;
		int color;

		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute fa_fangyu;
		Attribute per_baoji;
		Attribute baoji;
		Attribute per_kangbao;
		Attribute fujiashanghai;
		Attribute dikangshanghai;
		Attribute pojia;
	};

	struct ItemPos
	{
		ItemPos() : type(-1), seq(-1), index(-1)
		{
		};
		int type;
		int seq;
		int index;
	};


	ItemPos item_pos;
	ItemStarCfg shu_item_cfg[HESHENLUOSHU_ITEM_MAX_LEVEL + 1];
};

struct HeShenLuoShuShenHuaAdd
{
	HeShenLuoShuShenHuaAdd() : per_add(0), exp(0)
	{
	}
	int per_add;
	int exp;
};


class HeShenLuoShuConfig : public ConfigBase
{
public:
	HeShenLuoShuConfig();
	~HeShenLuoShuConfig();
	bool Init(const std::string &configname, std::string *err);

	const HeShenLuoShuOtherConfig & GetHeShenLuoShuOther() const { return m_other_cfg; }
	const HeShenLuoShuItemStar::ItemPos * GetHeShenLuoShuItemPos(ItemID item_id) const;
	const HeShenLuoShuItemStar::ItemStarCfg * GetHeShenLuoShuItemStarCfg(ItemID item_id, int star_level) const;
	const HeShenLuoShuItemStar::ItemStarCfg * GetHeShenLuoShuItemStarCfg(int type, int seq, int index, int star_level) const;
	const HeShenLuoShuItemStar * GetHeShenLuoShuItemStarAllCfg(ItemID item_id) const;
	const HeShenLuoShuSuiteAttr::SuiteAttr * GetHeShenLuoShuSuiteAttr(int type, int seq, int suit_num) const;
	const HeShenLuoShuShenHuaAdd * GetHeShenLuoShuShenHuaAdd(int type, int seq, int level) const;

	int HeShenLuoShuOtherMaxStarLevel() const { return m_other_cfg.max_star_level; };

	int GetHeShenLuoShuItemDecompose(int item_color);
private:
	int InitOtherCfg(YY_XML_NODE RootElement);
	int InitHeShenLuoShuSuiteAttr(YY_XML_NODE RootElement);
	int InitHeShenLuoShuItemStar(YY_XML_NODE RootElement);
	int InitHeShenLuoShuItemDecompose(YY_XML_NODE RootElement);
	int InitHeShenLuoShuShenHuaAdd(YY_XML_NODE RootElement);

	HeShenLuoShuOtherConfig m_other_cfg;
	HeShenLuoShuSuiteAttr m_suite_attr_list[HESHENLUOSHU_MAX_TYPE][HESHENLUOSHU_MAX_SEQ];

	typedef std::map<ItemID, HeShenLuoShuItemStar> HeShenLuoShuItemStarMap;
	typedef std::map<ItemID, HeShenLuoShuItemStar>::const_iterator HeShenLuoShuItemStarMapIt;
	HeShenLuoShuItemStarMap m_item_star_cfg_map;

	ItemID m_item_attr_cfg[HESHENLUOSHU_MAX_TYPE][HESHENLUOSHU_MAX_SEQ][HESHENLUOSHU_MAX_INDEX];

	int m_item_decompose_cfg[ItemBase::I_COLOR_MAX];
	HeShenLuoShuShenHuaAdd m_shen_hua_add_cfg[HESHENLUOSHU_MAX_TYPE][HESHENLUOSHU_MAX_SEQ][HESHENLUOSHU_HUASHEN_MAX_LEVEL + 1];
};


#endif

