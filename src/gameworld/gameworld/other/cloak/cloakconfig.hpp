#ifndef __CLOAK_CONFIG_HPP__
#define __CLOAK_CONFIG_HPP__

#include "gamedef.h"
#include "servercommon/basedef.h"
#include <map>

#include "servercommon/struct/itemlistparam.h"

#include "item/itembase.h"

#include "obj/character/attribute.hpp"
#include "servercommon/cloakdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct CloakOtherCfg
{
	CloakOtherCfg(): chengzhangdan_percent(0), active_equip_level(0), active_equip_skill_level(0)
	{}

	int chengzhangdan_percent;
	short active_equip_level;
	short active_equip_skill_level;
};

struct CloakUpLevelCfg
{
	CloakUpLevelCfg() : level(0), up_level_exp(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), extra_zengshang(0), extra_mianshang(0),
		per_jingzhun(0), per_baoji(0), per_mianshang(0), per_pofang(0), pvp_zengshang_per(0), pve_zengshang_per(0), pvp_jianshang_per(0), pve_jianshang_per(0),
		active_image(0), pack_num(0), is_broadcast(0), is_broadcast_image(0), shuxingdan_limit(0), chengzhangdan_limit(0)
	{}

	int level;
	int up_level_exp;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;

	Attribute extra_zengshang;									//固定增伤
	Attribute extra_mianshang;									//固定减伤

	Attribute per_jingzhun;										//精准（破甲率）
	Attribute per_baoji;											//暴击（暴击伤害率）
	Attribute per_mianshang;										//减伤万分比
	Attribute per_pofang;											//增伤万分比

	Attribute pvp_zengshang_per;									//pvp增伤万分比
	Attribute pve_zengshang_per;									//pve增伤万分比
	Attribute pvp_jianshang_per;									//pvp减伤万分比
	Attribute pve_jianshang_per;									//pve减伤万分比

	int active_image;
	int pack_num;

	int is_broadcast;
	int is_broadcast_image;

	int shuxingdan_limit;
	int chengzhangdan_limit;								// 成长丹限制

};

// 升级物品
struct CloakUpLevelStuffCfg
{
	CloakUpLevelStuffCfg() : up_level_item_index(0), up_level_item_id(0), add_exp(0), baoji_per(0), baoji_mul(0) {}

	short up_level_item_index;
	ItemID up_level_item_id;
	int add_exp;
	int baoji_per;
	int baoji_mul;
};

// 形象配置
struct CloakImageConfig
{
	CloakImageConfig() : image_id(0)
	{
	}

	int image_id;
};

// 特殊形象
struct CloakSpecialImgCfg
{
	CloakSpecialImgCfg() : image_id(0) {}

	bool IsValid() { return 0 != image_id; }

	int image_id;
};

//特殊形象进阶
struct CloakSpecialImgUpgradeCfg
{
	CloakSpecialImgUpgradeCfg() : image_id(0), grade(0), stuff_id(0), stuff_num(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), extra_zengshang(0), extra_mianshang(0), 
		per_jingzhun(0), per_baoji(0), per_mianshang(0), per_pofang(0), pvp_zengshang_per(0), pve_zengshang_per(0), pvp_jianshang_per(0), pve_jianshang_per(0),
		shuxingdan_count(0), chengzhangdan_count(0), equip_level(0) {}

	int image_id;
	int grade;
	int stuff_id;
	int stuff_num;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	Attribute extra_zengshang;									//固定增伤
	Attribute extra_mianshang;									//固定减伤
	Attribute per_jingzhun;										//精准（破甲率）
	Attribute per_baoji;											//暴击（暴击伤害率）
	Attribute per_mianshang;										//减伤万分比
	Attribute per_pofang;											//增伤万分比
	Attribute pvp_zengshang_per;									//pvp增伤万分比
	Attribute pve_zengshang_per;									//pve增伤万分比
	Attribute pvp_jianshang_per;									//pvp减伤万分比
	Attribute pve_jianshang_per;									//pve减伤万分比

	int shuxingdan_count;
	int chengzhangdan_count;
	int equip_level;
};

// 装备
struct CloakEquipConifg
{
	CloakEquipConifg() : max_level(0) {}

	int max_level;

	struct FootprintEquipLevelInfo
	{
		FootprintEquipLevelInfo() : maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), add_percent(0) {}

		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
		Attribute baoji;
		Attribute jianren;

		ItemConfigData item;
		int add_percent;
	};

	FootprintEquipLevelInfo level_cfg[CloakParam::EQUIP_MAX_LEVEL];
};

// 技能
struct CloakSkillConifg
{
	CloakSkillConifg() : index(0), skill_type(0), skill_level(0), cloak_level(0),
		uplevel_stuff_id(0), uplevel_stuff_num(0), param_a(0), param_b(0), param_c(0), param_d(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0) {}

	int index;
	int skill_type;
	int skill_level;
	int cloak_level;
	ItemID uplevel_stuff_id;
	int uplevel_stuff_num;

	int param_a;
	int param_b;
	int param_c;
	int param_d;

	int maxhp;
	int gongji;
	int fangyu;
	int mingzhong;
	int shanbi;
	int baoji;
	int jianren;
	int capability;
};

class CloakConfig : public ConfigBase
{
public:
	CloakConfig();
	~CloakConfig();

	bool Init(const std::string &configname, std::string *err);

	int GetChengZhangDanPercent() { return m_other_cfg.chengzhangdan_percent; }
	int GetActiveEquipLevel() { return m_other_cfg.active_equip_level; }
	int GetActiveEquipSkillLevel() { return m_other_cfg.active_equip_skill_level; }

	const CloakUpLevelCfg * GetUpLevelCfg(int star_level);
	const CloakUpLevelStuffCfg * GetUpLevelStuffCfg(int stuff_index);

	const CloakImageConfig * GetCommonImage(int attr_id) const;

	int GetEquipSkillLevel(int min_equip_level);

	const CloakSpecialImgCfg * GetSpecialImgCfg(int special_img_id);
	const CloakSpecialImgUpgradeCfg * GetSpecialImgUpgradeCfg(int special_img_id, int grade);

	const CloakEquipConifg * GetEquipCfg(int equip_idx);

	const CloakSkillConifg * GetSkillCfg(int skill_idx, int level);

private:
	int InitOther(PugiXmlNode RootElement);
	int InitUpLevelCfg(PugiXmlNode RootElement);
	int InitUpLevelStuffCfg(PugiXmlNode RootElement);

	int InitImageCfg(PugiXmlNode RootElement);
	int InitSpecialImageCfg(PugiXmlNode RootElement);
	int InitSpecialImageUpgradeCfg(PugiXmlNode RootElement);
	int InitEquipCfg(PugiXmlNode RootElement);
	int InitSkillCfg(PugiXmlNode RootElement);
	
	int InitEquipSkillLevel(PugiXmlNode RootElement);

	CloakOtherCfg m_other_cfg;

	CloakUpLevelCfg m_up_level_cfg_list[CloakParam::MAX_LEVEL + 1];
	CloakUpLevelStuffCfg m_upstar_item_list[CloakParam::MAX_UP_LEVEL_ITEM_COUNT];

	CloakImageConfig m_image_list[CloakParam::MAX_IMAGE_COUNT];

	////////////////////////////////////////////////////////////////////////////
	CloakSpecialImgCfg m_special_img_cfglist[CloakParam::MAX_SPECIAL_IMAGE_ID + 1];

	struct CloakSpecialImgUpgradeCfgList
	{
		CloakSpecialImgUpgradeCfgList() : max_grade(0) {}

		int max_grade;
		CloakSpecialImgUpgradeCfg upgrade_list[CloakParam::MAX_UPGRADE_LIMIT + 1];
	};

	typedef std::map<int, CloakSpecialImgUpgradeCfgList> CloakSpecialImgUpgradeCfgMap;
	CloakSpecialImgUpgradeCfgMap m_special_img_upgrade_map;

	////////////////////////////////////////////////////////////////////////////
	CloakEquipConifg m_equip_cfg_list[CloakParam::EQUIP_COUNT];
	int m_equip_skill_level_cfg[CloakParam::EQUIP_MAX_LEVEL];

	//////////////////////////////////////////////////////////////////////////
	CloakSkillConifg m_skill_cfg_list[CloakParam::SKILL_COUNT][CloakParam::SKILL_MAX_LEVEL + 1];
	
	
};

#endif

