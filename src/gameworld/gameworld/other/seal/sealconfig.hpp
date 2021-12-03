#ifndef __SEAL_CONFIG_H__
#define __SEAL_CONFIG_H__

#include "servercommon/configcommon.h"
#include <map>
#include <vector>
#include "servercommon/struct/sealparam.hpp"
#include "servercommon/configbase.h"

struct SealSlotOpenLevelCfg
{
	SealSlotOpenLevelCfg() :role_level(0), slot_index(0) {}

	int role_level;
	int slot_index;
};

struct SealRealIdCfg
{
	SealRealIdCfg() : seal_id(0), slot_index(0), order(0), color(0) {}

	ItemID seal_id;
	int slot_index;
	int order;
	int color;
};

struct SealInitialAttrCfg
{
	SealInitialAttrCfg() : slot_index(0), order(0), color(0), gongji(0), pojia(0), yaolichuantou(0), yaolidikang(0), molichuantou(0), molidikang(0), shenlichuantou(0), shenlidikang(0), maxhp(0),
		fangyu(0), per_baoji(0), baoji(0), per_baoji_jiacheng(0), per_maxhp(0), per_fangyu(0), per_gongji(0), per_pojia(0), per_shanghaijiacheng(0), per_shanghaijianshao(0), per_base_attr_jiacheng(0),
		mingzhong(0), shanbi(0), jianren(0){}

	int slot_index;
	int order;
	int color;
	Attribute gongji;
	Attribute pojia;
	int yaolichuantou;
	int yaolidikang;
	int molichuantou;
	int molidikang;
	int shenlichuantou;
	int shenlidikang;
	Attribute maxhp;
	Attribute fangyu;
	Attribute per_baoji;
	Attribute baoji;
	int per_baoji_jiacheng;
	int per_maxhp;
	int per_fangyu;
	int per_gongji;
	Attribute per_pojia;
	Attribute per_shanghaijiacheng;
	Attribute per_shanghaijianshao;
	int per_base_attr_jiacheng;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute jianren;
};

struct SealItemCfg
{
	SealItemCfg() : slot_index(0), gongji(0), pojia(0), yaolichuantou(0), yaolidikang(0), molichuantou(0), molidikang(0), shenlichuantou(0), shenlidikang(0), maxhp(0),
		fangyu(0), per_baoji(0), baoji(0), per_baoji_jiacheng(0), per_maxhp(0), per_fangyu(0), per_gongji(0), per_pojia(0), per_shanghaijiacheng(0), per_shanghaijianshao(0) {}

	int slot_index;
	Attribute gongji;
	Attribute pojia;
	int yaolichuantou;
	int yaolidikang;
	int molichuantou;
	int molidikang;
	int shenlichuantou;
	int shenlidikang;
	Attribute maxhp;
	Attribute fangyu;
	Attribute per_baoji;
	Attribute baoji;
	int per_baoji_jiacheng;
	int per_maxhp;
	int per_fangyu;
	int per_gongji;
	Attribute per_pojia;
	Attribute per_shanghaijiacheng;
	Attribute per_shanghaijianshao;
};

struct SealInitialScoreCfg
{
	SealInitialScoreCfg() : color(0), hun_score(0), jinghua_hun_score(0) {}

	int color;
	int hun_score;
	int jinghua_hun_score;
};

struct SealScoreCfg
{
	SealScoreCfg() : level(0), hun_score(0) {}

	int level;
	int hun_score;
};

struct SealStrengthCfg
{
	SealStrengthCfg() : level(0), gongji(0), pojia(0), yaolichuantou(0), yaolidikang(0), molichuantou(0), molidikang(0), shenlichuantou(0), shenlidikang(0), maxhp(0),
		fangyu(0), per_baoji(0), baoji(0), per_baoji_jiacheng(0), per_maxhp(0), per_fangyu(0), per_gongji(0), per_pojia(0), per_shanghaijiacheng(0), per_shanghaijianshao(0) {}

	int level;
	Attribute gongji;
	Attribute pojia;
	int yaolichuantou;
	int yaolidikang;
	int molichuantou;
	int molidikang;
	int shenlichuantou;
	int shenlidikang;
	Attribute maxhp;
	Attribute fangyu;
	Attribute per_baoji;
	Attribute baoji;
	int per_baoji_jiacheng;
	int per_maxhp;
	int per_fangyu;
	int per_gongji;
	Attribute per_pojia;
	Attribute per_shanghaijiacheng;
	Attribute per_shanghaijianshao;
};

struct SealSuitCfg
{
	SealSuitCfg() : suit_type(0), same_order_num(0), gongji(0), pojia(0), yaolichuantou(0), yaolidikang(0), molichuantou(0), molidikang(0), shenlichuantou(0), shenlidikang(0), maxhp(0), fangyu(0), per_baoji(0),
		baoji(0), per_baoji_jiacheng(0), per_maxhp(0), per_fangyu(0), per_gongji(0), per_jingzhun(0), per_pofang(0), per_mianshang(0), per_base_attr_jiacheng(0), per_strength_attr_jiacheng(0),
		mingzhong(0), shanbi(0), jianren(0), skill_zengshang_per(0){}

	int suit_type;
	int same_order_num;
	Attribute gongji;
	Attribute pojia;
	int yaolichuantou;
	int yaolidikang;
	int molichuantou;
	int molidikang;
	int shenlichuantou;
	int shenlidikang;
	Attribute maxhp;
	Attribute fangyu;
	Attribute per_baoji;
	Attribute baoji;
	int per_baoji_jiacheng;
	int per_maxhp;
	int per_fangyu;
	int per_gongji;
	Attribute per_jingzhun;
	Attribute per_pofang;
	Attribute per_mianshang;
	int per_base_attr_jiacheng;
	int per_strength_attr_jiacheng;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute jianren;
	Attribute skill_zengshang_per;
};

struct SealSuitPartCfg
{
	SealSuitPartCfg() : suit_type(0), equip_order(0), equip_part(0) {}

	int suit_type;
	int equip_order;
	int equip_part;
};

struct SealSoulCfg
{
	SealSoulCfg() : soul_type(0), soul_id(0), gongji(0), pojia(0), yaolichuantou(0), yaolidikang(0), molichuantou(0), molidikang(0), shenlichuantou(0), shenlidikang(0), maxhp(0), fangyu(0), per_baoji(0),
		baoji(0), per_baoji_jiacheng(0), per_maxhp(0), per_fangyu(0), per_gongji(0), per_pojia(0), per_shanghaijiacheng(0), per_shanghaijianshao(0), per_base_attr_jiacheng(0), per_strength_attr_jiacheng(0) {}

	int soul_type;
	int soul_id;
	Attribute gongji;
	Attribute pojia;
	int yaolichuantou;
	int yaolidikang;
	int molichuantou;
	int molidikang;
	int shenlichuantou;
	int shenlidikang;
	Attribute maxhp;
	Attribute fangyu;
	Attribute per_baoji;
	Attribute baoji;
	int per_baoji_jiacheng;
	int per_maxhp;
	int per_fangyu;
	int per_gongji;
	Attribute per_pojia;
	Attribute per_shanghaijiacheng;
	Attribute per_shanghaijianshao;
	int per_base_attr_jiacheng;
	int per_strength_attr_jiacheng;
};

struct SealSoulUseLimitCfg
{
	SealSoulUseLimitCfg() : role_level(0), use_limit_num(0) {}

	int role_level;
	int use_limit_num;
};

class SealConfig : public ConfigBase
{
public:
	SealConfig();
	~SealConfig();

	bool Init(const std::string &configname, std::string *err);

	int GetOpenSlotIndex(int role_level);
	const SealRealIdCfg *GetRealIdCfg(ItemID seal_id) const;
	const SealInitialAttrCfg *GetInitialAttrCfg(int slot_index, int order, int color);
	const SealItemCfg *GetSealItemCfg(int slot_index) const;
	const int GetInitialHunScore(int color) const;
	const int GetInitialJinghuaScore(int color) const;
	const int GetSealScore(int level) const;
	const SealStrengthCfg *GetStrengthCfg(int level) const;
	const SealSuitCfg *GetSuitCfg(int suit_type, int same_order_num) const;
	const SealSuitPartCfg *GetSuitPartCfg(int equip_order, int equip_part) const;
	const SealSoulCfg *GetSoulCfg(int soul_type) const;
	const int GetSoulUseLimit(int role_level) const;

private:
	int InitSlotOpenCfg(TiXmlElement *RootElement);
	int InitIdListCfg(TiXmlElement *RootElement);
	int InitInitialAttrCfg(TiXmlElement *RootElement);
	int InitSealItemCfg(TiXmlElement *RootElement);
	int InitInitialScoreCfg(TiXmlElement *RootElement);
	int InitScoreCfg(TiXmlElement *RootElement);
	int InitStrengthCfg(TiXmlElement *RootElement);
	int InitSuitCfg(TiXmlElement *RootElement);
	int InitPartListCfg(TiXmlElement *RootElement);
	int InitSoulCfg(TiXmlElement *RootElement);
	int InitSoulUseLimitCfg(TiXmlElement *RootElement);

	std::vector<SealSlotOpenLevelCfg> m_slot_open_cfg_list;

	typedef std::map<ItemID, SealRealIdCfg> SealRealIdCfgMap;
	typedef std::map<ItemID, SealRealIdCfg>::const_iterator SealRealIdCfgMapIt;
	SealRealIdCfgMap m_seal_id_cfg_map;

	std::vector<SealInitialAttrCfg> m_initial_attr_cfg_list;

	typedef std::map<int, SealItemCfg> SealItemCfgMap;
	typedef std::map<int, SealItemCfg>::const_iterator SealItemCfgMapIt;
	SealItemCfgMap m_seal_item_cfg_map;

	SealInitialScoreCfg m_initial_score_list[SEAL_COLOR_MAX_TYPE];

	std::vector<SealScoreCfg> m_score_cfg_list;

	std::vector<SealStrengthCfg> m_strength_cfg_list;

	typedef std::map<long long, SealSuitCfg> SealSuitCfgMap;
	typedef std::map<long long, SealSuitCfg>::const_iterator SealSuitCfgMapIt;
	SealSuitCfgMap m_suit_cfg_map;

	typedef std::map<long long, SealSuitPartCfg> SealSuitPartCfgMap;
	typedef std::map<long long, SealSuitPartCfg>::const_iterator SealSuitPartCfgMapIt;
	SealSuitPartCfgMap m_suit_part_cfg_map;

	SealSoulCfg m_soul_cfg_list[SEAL_SOUL_MAX_TYPE + 1];

	std::vector<SealSoulUseLimitCfg> m_soul_use_limit_cfg_list;
};

#endif