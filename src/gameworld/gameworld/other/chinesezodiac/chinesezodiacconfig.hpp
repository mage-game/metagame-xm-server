#ifndef __CHINESE_SINGLE_CONFIG_H__
#define __CHINESE_SINGLE_CONFIG_H__

#include "servercommon/chinesezodiacdef.hpp"
#include "gameworld/config/gameconfigcommon/gameconfigcommon.h"
#include "common/tinyxml/tinyxml.h"
#include <map>
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct ChineseZodiacOther
{
	ChineseZodiacOther() : limit_level(0), change_bead_type_need_gold(0), ggl_day_bind_gold_time(0), ggl_consume_gold(0), miji_compound_consume_gold1(0), miji_compound_consume_gold2(0), miji_compound_consume_gold3(0), ggl_free_times(0), ggl_free_times_cd(0), ggl_ten_consume_gold(0), equiment_recycle(0),
		replacement_id(0), consume_replacement(0), ten_chou_use_itemid(0), ten_chou_use_item_num(0), can_use_item(true)
	{}

	int limit_level;
	int change_bead_type_need_gold;
	int ggl_day_bind_gold_time;
	ItemConfigData ggl_comfort_reward;
	int ggl_consume_gold;
	int miji_compound_consume_gold1;
	int miji_compound_consume_gold2;
	int miji_compound_consume_gold3;
	int ggl_free_times;
	int ggl_free_times_cd;
	int ggl_ten_consume_gold;
	int equiment_recycle;

	int replacement_id;
	int consume_replacement;
	int ten_chou_use_itemid;
	int ten_chou_use_item_num;
	bool can_use_item;
};

enum GUNGUNLE_TYPE
{
	GUNGUNLE_TYPE_INVALID = 0,
	GUNGUNLE_TYPE_777 =1,
	GUNGUNLE_TYPE_THIRD_FUIT =2,
	GUNGUNLE_TYPE_2_ADD_1 = 3,
	GUNGUNLE_TYPE_THIRD_DIFF = 4,
	GUNGUNLE_TYPE_MAX,
};
struct GglCombineType
{	
	GglCombineType() :type_index(0),combine_same_num(0), type_pro(0){}

	int type_index;
	int combine_same_num;
	std::vector<int> combine_type_vec;
	int type_pro;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct ChineseZodiacSingleAttr
{
	ChineseZodiacSingleAttr(){}

	struct SingleType
	{
		SingleType() : soul_type(0), level(0), color(0), level_limit(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), item_id(0), expend(0)
		{
			memset(name, 0, sizeof(name));
		}

		int soul_type;
		int level;
		int color;
		int level_limit;
		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
		Attribute baoji;
		Attribute jianren;
		int item_id;
		int expend;
		GameName name;
	};

	SingleType single_type_vec[CHINESE_ZODIAC_LEVEL_MAX_LIMIT + 1];
};

struct ChineseZodiacSuitAttr
{
	ChineseZodiacSuitAttr() : level(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0) {}

	int level;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
};

struct ChineseZodiacEquipAttr
{
	ChineseZodiacEquipAttr() : equip_type(0), quality(0), zodiac_level(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0) {}

	int equip_type;
	int quality;
	int zodiac_level;

	int maxhp;
	int gongji;
	int fangyu;
	int mingzhong;
	int shanbi;
	int baoji;
	int jianren;
};

struct ChineseZodiacEquipSuitAttr
{
	ChineseZodiacEquipSuitAttr() :suit_id(0), suit_color(0), need_count(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), per_kangbao(0),
		per_baoji_hurt(0), per_baoji(0){}

	int suit_id;
	int suit_color;
	int need_count;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	Attribute per_kangbao;
	Attribute per_baoji_hurt;
	Attribute per_baoji;
};

struct ChineseZodiacXingHunCfg
{
	ChineseZodiacXingHunCfg() : seq(0), level(0), consume_stuff_id(0), consume_stuff_num(0), succ_percent(0), is_protect_level(false), backwards_highest_level(0),
		max_baoji_value(0), faild_add_baoji_value(0), baoji_value_change_value(0),
		protect_item_id(0), protect_item_num(0),
		mount_add(0), wing_add(0), halo_add(0),shengong_add(0), shenyi_add(0), fightmount_add(0),footprint_add(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0) {}

	int seq;
	int level;

	ItemID consume_stuff_id;
	int consume_stuff_num;
	int succ_percent;
	bool  is_protect_level;
	int backwards_highest_level;

	int max_baoji_value;
	int faild_add_baoji_value;
	int baoji_value_change_value;

	ItemID protect_item_id;
	int protect_item_num;

	int mount_add;
	int wing_add;
	int halo_add;
	int shengong_add;
	int shenyi_add;
	int fightmount_add;
	int footprint_add;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
};

static const int TIAN_XIANG_LEVEL_LIMIT_MAX_CFG = 10;		// 等级限制配置个数


struct ChapterAttributesConfig
{
	ChapterAttributesConfig() : chapter(0), per_attr(0),per_jingzhun(0)
	{
		memset(name, 0, sizeof(name));
	}
	
	int chapter;
	int per_attr;
	Attribute per_jingzhun;
	GameName name;
	AttributesConfig attr;
};

struct TianXiangBeadCfg
{
	TianXiangBeadCfg(): type(0), item_id(0)
	{
	}

	short type;							// 珠子颜色
	int item_id;
	AttributesConfig attr;
};

struct TianXiangCombineAttrCfg
{
	TianXiangCombineAttrCfg() : seq(0)
	{
		memset(name, 0, sizeof(name));
	}

	int seq;							// 组合索引
	GameName name;
	AttributesConfig attr;
};

struct TianXiangCombineItem
{
	TianXiangCombineItem() : type(0), x(0), y(0)
	{
	}

	int type;;							// 珠子颜色
	short x;
	short y;
};

struct TianXiangCombineSeqCfg
{
	TianXiangCombineSeqCfg(): seq(0)
	{
	}

	int seq;							// 组合索引
	TianXiangCombineItem bead_list[TIAN_XIANG_COMBINE_MEX_BEAD_NUM];
};

struct TianXiangLevelLimitCfg
{
	TianXiangLevelLimitCfg(): limit_level(0), bead_num(0)
	{
	}

	int limit_level;
	int bead_num;
};

struct ChineseZodiacXingHunExtraAttrCfg
{
	ChineseZodiacXingHunExtraAttrCfg() : level(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0) {}

	int level;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
};

// 秘籍配置
struct ChineseZodiacMiji
{
	ChineseZodiacMiji() : index(-1), item_id(0), type(-1), level(-1), value(0), capacity(0) {}

	int index;
	int item_id;
	int type;
	int level;
	int value;
	int capacity;
};

// 秘籍开放限制
struct ChineseZodiacMijiOpenLimit
{
	ChineseZodiacMijiOpenLimit() : last_zodiac_miji_num_limit(0) {}

	int last_zodiac_miji_num_limit;
};

// 秘籍孔开放
struct ChineseZodiacMijiKongOpenLimit
{
	ChineseZodiacMijiKongOpenLimit() : zodiac_level_limit(0) {}

	int zodiac_level_limit;
};

struct XingLingCfg
{
	struct ConfigItem
	{
		ConfigItem() : id(0), level(0), uplevel_stuff_id(0), uplevel_stuff_num(0), bless_val_limit(0), add_prob(0), init_prob(0), add_bless(0), gongji(0), fangyu(0), maxhp(0), shanbi(0), baoji(0), kangbao(0), mingzhong(0), xingtu_add_prob(0)
		{
		}

		int id;
		int level;
		ItemID uplevel_stuff_id;
		int uplevel_stuff_num;
		int bless_val_limit;
		int add_prob;
		int init_prob;
		int add_bless;

		Attribute gongji;
		Attribute fangyu;
		Attribute maxhp;
		Attribute shanbi;
		Attribute baoji;
		Attribute kangbao;
		Attribute mingzhong;
		int xingtu_add_prob;
	};

	XingLingCfg() {}

	ConfigItem cfg_list[TIAN_XIANG_CURR_CHAPTER_NUM][XING_LING_MAX_LEVEL];
};

struct ChineseZodiacNewEquipSuitCfg
{
	ChineseZodiacNewEquipSuitCfg() : cfg_count(0)
	{
		memset(attr_cfg, 0, sizeof(attr_cfg));
	}

	int cfg_count;
	ChineseZodiacEquipSuitAttr attr_cfg[CHINESE_ZODIAC_MAX_EQUIP_SUIT];
};

struct ChineseZodiacColorCfg
{
	ChineseZodiacColorCfg() :index(0), chinesezodiac_color(0){}
	int index;
	int chinesezodiac_color;
};

class ChineseZodiacConfig : public ConfigBase
{
public:
	ChineseZodiacConfig();
	~ChineseZodiacConfig();

	bool Init(const std::string &configname, std::string *err);
	const ChineseZodiacSingleAttr::SingleType * GetSingleAttr(int soul_type, int level);
	const ChapterAttributesConfig * GetChapterAttr(int chapter);
	const ChineseZodiacSuitAttr * GetSuitAttr(int level);
	const ChineseZodiacEquipSuitAttr * GetEquipSuitAttr(int zodiac_type, int color) const;

	const ChineseZodiacXingHunCfg * GetChineseZodiacXingHunCfg(int zodiac_type, int xinghun_level);
	const ChineseZodiacXingHunExtraAttrCfg * GetChineseZodiacXingHunAttrCfg(int xinghun_level);
	
	const ChineseZodiacOther & GetOtherCfg() const { return m_other_cfg; }
	const TianXiangBeadCfg * GetBeadCfg(short type) const;
	int GetCombineSeqCfgCount() const { return m_bead_combine_cfg_count; }
	const TianXiangCombineSeqCfg * GetCombineSeqCfg(int seq) const;
	const TianXiangCombineAttrCfg * GetCombineAttrCfg(int seq) const;
	const TianXiangLevelLimitCfg * GetLevelLimitCfg(int level) const;

//	int GetTotalGglItemPro(){ return m_total_pro; }
	void GetGglResult(int &index,int &res_num);
// 	void GetGglTenResult(int &index, int &res_num);
// 	int GetGglResultCombineType(int same_num, int combine_type);
 	const GglCombineType * GetGglRewardCfg(int combine_type);

// 	const ChineseZodiacMiji * GetMijiCfgByIndex(int index);
// 	const ChineseZodiacMiji * GetMijiCfgByTypeAndLevel(int type, int level);
// 	const ChineseZodiacMiji * GetMijiCfgByItemid(int item_id);
// 	const ChineseZodiacMijiOpenLimit * GetMijiOpenLimit(int type);
// 	int GetMijiKongOpenNumByZodiacLevel(int zodiac_level);
	int GetMijiCount(){ return m_miji_count; };
	int GetMijiTypeCount(){ return m_miji_type_count; };
	int IsBroadcast(int level) { return static_cast<int>(m_is_broadcast_map.count(level)); }
	const XingLingCfg::ConfigItem * GetXinglingCfg(int id, int level);
	int GetColorCfg(int zodiac_type);
	const ChineseZodiacEquipSuitAttr * GetNewSuitCfg(int zodiac_type, int color, int count);
	bool CheckTypeIndex(int type_index);

//	const ChineseZodiacEquipAttr * GetEquigpAttrByColor(int color,int equip_type, int level);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitSingleAttrCfg(PugiXmlNode RootElement);
	int InitSuitAttrCfg(PugiXmlNode RootElement);
	int InitEquipAttrCfg(PugiXmlNode RootElement);
	int InitEquipSuitAttrCfg(PugiXmlNode RootElement);
	int InitChineseZodiacXingHunCfg(PugiXmlNode RootElement);
	int InitChineseZodiacXingHunExtraAttrCfg(PugiXmlNode RootElement);

	int InitBeadCfg(PugiXmlNode RootElement);
	int InitCombineCfg(PugiXmlNode RootElement);
	int InitCombineAttrCfg(PugiXmlNode RootElement);
	int InitChapterAttrCfg(PugiXmlNode RootElement);
	int InitLevelLimitCfg(PugiXmlNode RootElement);

	int InitGglCombineTypeCfg(PugiXmlNode RootElement);
//	int InitGglCombineItemCfg(PugiXmlNode RootElement);

// 	int InitMijiCfg(PugiXmlNode RootElement);
// 	int InitMijiOpenLimitCfg(PugiXmlNode RootElement);
// 	int InitMijiKongOpenLimitCfg(PugiXmlNode RootElement);

	int InitBroadcastCfg(PugiXmlNode RootElement);
	int InitXinglingCfg(PugiXmlNode RootElement);
	int InitChineseZodiacColorCfg(PugiXmlNode RootElement);
	int InitNewEquipSuitSuitCfg(PugiXmlNode RootElement);

	int m_single_attr_count;
	ChineseZodiacSingleAttr m_single_attr_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];

	int m_suit_attr_count;
	ChineseZodiacSuitAttr m_suit_attr_list[CHINESE_ZODIAC_LEVEL_MAX_LIMIT + 1];

	ChineseZodiacOther m_other_cfg;

	//int m_equip_attr_count;
	//ChineseZodiacEquipAttr m_equip_attr_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT][CHINESE_ZODIAC_CUR_EQUIP_SLOT_MAX_LIMIT][CHINESE_ZODIAC_MAX_EQUIP_LEVEL + 1];

	ChineseZodiacEquipSuitAttr m_equip_suit_attr_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT][CHINESE_ZODIAC_MAX_EQUIP_SUIT];

	std::map<long long, ChineseZodiacXingHunCfg> m_xinghun_level_map; 

	int m_xinghun_extra_count;
	ChineseZodiacXingHunExtraAttrCfg m_xinghun_attr_list[CHINESE_ZODIAC_XINGHUN_TITLE_COUNT_MAX];	

	int m_bead_cfg_count;
	TianXiangBeadCfg m_bead_cfg[TIAN_XIANG_BEAD_TYPE_MAX];

	int m_bead_combine_cfg_count;
	TianXiangCombineSeqCfg m_bead_combine_cfg_list[TIAN_XIANG_ALL_CHAPTER_COMBINE * TIAN_XIANG_CURR_CHAPTER_NUM];

	int m_bead_combine_attr_cfg_count;
	TianXiangCombineAttrCfg m_bead_combine_attr_cfg_list[TIAN_XIANG_ALL_CHAPTER_COMBINE * TIAN_XIANG_CURR_CHAPTER_NUM];

	int m_level_limit_cfg_count;
	TianXiangLevelLimitCfg m_level_limit_cfg_list[TIAN_XIANG_LEVEL_LIMIT_MAX_CFG];
	
	ChapterAttributesConfig m_bead_chapter_attr_list[TIAN_XIANG_CURR_CHAPTER_NUM];

	// 滚滚乐
	int m_total_pro;
	std::map<int,GglCombineType> m_ggl_combine_type;
 	
// 	std::vector<int> m_ggl_item_pro;
// 	int m_total_ten_pro;
// 	std::vector<int> m_ggl_item_ten_pro;

	// 秘籍
	int m_miji_count;
	int m_miji_type_count;
	ChineseZodiacMiji m_miji_list[MIJI_KONG_KILL_MAX_COUNT];
	ChineseZodiacMijiOpenLimit m_miji_open_limit_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];
	ChineseZodiacMijiKongOpenLimit  m_miji_kong_open_limit_list[MIJI_KONG_NUM];

	std::map<int, bool> m_is_broadcast_map;
	XingLingCfg m_xingling_cfg;

	ChineseZodiacColorCfg m_chinesezodiac_color_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];

	ChineseZodiacNewEquipSuitCfg m_new_equip_suit_cfg[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT][ItemBase::I_COLOR_MAX];
};

#endif

