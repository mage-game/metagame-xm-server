#ifndef _UPGRADE_CONFIG_HPP
#define _UPGRADE_CONFIG_HPP

#include <map>
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/struct/upgradeparam.hpp"
#include "servercommon/configcommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "item/itembase.h"

class UpgradeconfigManager;

enum UPGRADE_MODULE_TYPE
{
	UPGRADE_MODULE_TYPE_INVALID = -1,

//	UPGRADE_MODULE_TYPE_LEVEL,		// 等级模块
	UPGRADE_MODULE_TYPE_UPGRADE,	// 进阶模块
	UPGRADE_MODULE_TYPE_IMAGE,		// 形象模块 
	UPGRADE_MODULE_TYPE_EQUIP,		// 装备模块
	UPGRADE_MODULE_TYPE_SKILL,		// 技能模块
	UPGRADE_MODULE_TYPE_BIG_GOAL,	// 大目标模块

	UPGRADE_MODULE_TYPE_MAX,
};

//其他相关---------------------------------------------------------------
struct UpgradeOtherConfig
{
	UpgradeOtherConfig() : extra_attrs_per_grade(0), extra_attrs_per(0) {}

	int extra_attrs_per_grade;
	int extra_attrs_per;
};

//进阶相关---------------------------------------------------------------
// 等级
struct LevelConfig
{
	LevelConfig() : level(0)
	{
	}

	int level;
	AttrConfig attr_cfg;
};

// 进阶
struct GradeConfig
{
	GradeConfig() : grade(0), client_grade(0), upgrade_stuff_id(0), upgrade_stuff2_id(0), upgrade_stuff_count(0),
		active_image_id(0), bless_val_limit(0), is_clear_bless(false),bless_addition(0), shuxingdan_limit(0), chengzhangdan_limit(0),equip_level_limit(0),
		movespeed(0), extra_zengshang(0), extra_mianshang(0),
		per_jingzhun(0), per_baoji(0), jianshang_per(0), zengshang_per(0),
		pvp_zengshang_per(0), pve_zengshang_per(0), pvp_jianshang_per(0), pve_jianshang_per(0),
		is_broadcast(false), equip_level_toplimit(0)
	{
	}

	short grade;											// 阶数
	short client_grade;										// 客户端阶数
	ItemID upgrade_stuff_id;								// 进阶材料id
	ItemID upgrade_stuff2_id;								// 进阶材料2id
	short upgrade_stuff_count;								// 进阶材料数量
	short active_image_id;									// 激活形象ID
	int bless_val_limit;									// 祝福值上限
	int is_clear_bless;										// 是否清空祝福值
	int bless_addition;										// 祝福值进度条加成万分比
	int shuxingdan_limit;									// 属性丹限制
	int chengzhangdan_limit;								// 成长丹限制
	int equip_level_limit;									// 装备等级限制

	AttrConfig attr_cfg;
	int movespeed;
	int extra_zengshang;									//固定增伤
	int extra_mianshang;									//固定减伤
	int per_jingzhun;										//精准（破甲率）
	int per_baoji;											//暴击（暴击伤害率）
	int jianshang_per;										//减伤万分比
	int zengshang_per;										//增伤万分比
	int pvp_zengshang_per;									//pvp增伤万分比
	int pve_zengshang_per;									//pve增伤万分比
	int pvp_jianshang_per;									//pvp减伤万分比
	int pve_jianshang_per;									//pve减伤万分比

	bool is_broadcast;										// 是否传闻
	int equip_level_toplimit;								//装备等级上限
};

// 进阶成功率
struct UpGradeProbConfig
{
	UpGradeProbConfig() : low_bless_val(0), high_bless_val(0), succ_rate(0)
	{
	}

	int low_bless_val;										// 祝福值下限
	int high_bless_val;										// 祝福值上限
	int succ_rate;											// 成功率

	LuckyConfig upgrade_lucky_cfg;							// 失败加祝福值暴击
};

struct UpGradeProbList
{
	static const int MAX_PROB_COUNT_PER_GRADE = 200;

	UpGradeProbList() : grade(0), prob_count(0) {}

	bool Add(UpGradeProbConfig &cfg)
	{
		if (prob_count >= MAX_PROB_COUNT_PER_GRADE) return false;

		prob_list[prob_count++] = cfg;
		return true;
	}

	short grade;
	short prob_count;
	UpGradeProbConfig prob_list[MAX_PROB_COUNT_PER_GRADE];
};

//形象相关---------------------------------------------------------------

// 形象列表配置
struct ImageConfig								 
{
	ImageConfig() : image_id(0)
	{
		
	}

	bool IsValid() { return 0 != image_id; }

	int image_id;
};

// 形象进阶
struct ImgUpgradeCfg
{
	ImgUpgradeCfg() : image_id(0), grade(0), stuff_id(0), stuff_num(0),
		shuxingdan_count(0), chengzhangdan_count(0), equip_level(0), add_per(0), extra_zengshang(0), extra_mianshang(0),
		per_jingzhun(0), per_baoji(0), jianshang_per(0), zengshang_per(0),
		pvp_zengshang_per(0), pve_zengshang_per(0), pvp_jianshang_per(0), pve_jianshang_per(0) {}

	int image_id;
	int grade;
	int stuff_id;
	int stuff_num;

	int shuxingdan_count;
	int chengzhangdan_count;
	int equip_level;

	int add_per;
	AttrConfig attr_cfg;
	Attribute extra_zengshang;									//固定增伤
	Attribute extra_mianshang;									//固定减伤
	Attribute per_jingzhun;										//精准（破甲率）
	Attribute per_baoji;											//暴击（暴击伤害率）
	Attribute jianshang_per;										//减伤万分比
	Attribute zengshang_per;										//增伤万分比
	Attribute pvp_zengshang_per;									//pvp增伤万分比
	Attribute pve_zengshang_per;									//pve增伤万分比
	Attribute pvp_jianshang_per;									//pvp减伤万分比
	Attribute pve_jianshang_per;									//pve减伤万分比
};

struct ImgUpgradeCfgList
{
	ImgUpgradeCfgList() : max_grade(0) {}

	int max_grade;
	ImgUpgradeCfg upgrade_list[UPGRADE_MAX_IMAGE_GRADE + 1];
};

//装备相关---------------------------------------------------------------
struct EquipConifg
{
	EquipConifg() : max_level(0) {}

	int max_level;

	struct EquipLevelInfo
	{
		EquipLevelInfo(): add_percent(0) {}


		AttrConfig attr_cfg;
		ItemConfigData item;
		int add_percent;
	};

	EquipLevelInfo level_cfg[UPGRADE_EQUIP_MAX_LEVEL + 1];
};

//技能相关---------------------------------------------------------------

struct SkillConifg
{
	SkillConifg() : index(0), skill_type(0), skill_level(0), grade(0),
		uplevel_stuff_id(0), end_day_of_up_skill(0), uplevel_stuff_num(0), param_a(0), param_b(0), param_c(0), param_d(0), capability(0), skill_cap_per(0), base_attr_add_per(0) {}

	int index;
	int skill_type;
	int skill_level;
	int grade;
	ItemID uplevel_stuff_id;
	short end_day_of_up_skill;
	int uplevel_stuff_num;

	int param_a;
	int param_b;
	int param_c;
	int param_d;

	int capability;
	int skill_cap_per;
	int base_attr_add_per;
	AttrConfig attr_cfg;
};

//超强战力加成配置
struct SuperSpecialCapAddCfgItem
{
	SuperSpecialCapAddCfgItem() : image_id(-1), grade(0), maxhp(0), gongji(0), fangyu(0) {}

	int image_id;
	int grade;
	int maxhp;
	int gongji;
	int fangyu;
};

class UpgradeConfig
{
public:
	UpgradeConfig(UpgradeconfigManager *upgradeconfig_manager, int upgrade_type);
	virtual ~UpgradeConfig();

	virtual bool Init(const std::string &configname, std::string *err);

	bool IsModuleOpen(int module_type);

	//其他相关---------------------------------------------------------------
	const UpgradeOtherConfig & GetOtherCfg() { return m_other_cfg; }

	int GetUpOneGradeRewardItem() { return m_up_one_grade_reward_id; }
	int GetTemporaryImageValidTime() { return m_temporary_image_time; }
	int GetActiveEquipGrade() { return m_active_equip_grade; }
	int GetActiveEquipSkillLevel() { return m_active_equip_skill_level; }
	short GetMedalSkillIndex() { return m_medal_skill_index; }

	//等级相关---------------------------------------------------------------
	const LevelConfig * GetLevelConfig(int level) const;

	//进阶相关---------------------------------------------------------------
	const GradeConfig * GetGradeConfig(int grade) const;
	const UpGradeProbConfig * GetUpGradeProbConfig(int grade, int bless_val) const;

	//形象相关---------------------------------------------------------------
	const ImageConfig * GetImageCfg(int image_id) const;
	const ImgUpgradeCfg * GetImgUpgradeCfg(int special_img_id, int grade);

	//装备相关---------------------------------------------------------------
	const EquipConifg * GetEquipCfg(int equip_idx);

	//技能相关---------------------------------------------------------------
	const SkillConifg * GetSkillCfg(int skill_idx, int level);

	//大目标相关
	const SuperSpecialCapAddCfgItem* GetSuperSpecialCapAddCfg(int image_id) const;
	bool IsAddSpecialCap();

protected:
	UpgradeconfigManager *m_upgradeconfig_manager;

	int m_upgrade_type;

	//其他相关---------------------------------------------------------------
	int InitOtherCfg(PugiXmlNode RootElement);

	UpgradeOtherConfig m_other_cfg;
	int m_up_one_grade_reward_id;
	int m_temporary_image_time;
	short m_active_equip_grade;
	short m_active_equip_skill_level;
	bool m_is_open_special_cap_add;
	short m_medal_skill_index;

	//进阶相关---------------------------------------------------------------
	int InitLevelCfg(PugiXmlNode RootElement);
	int InitGradeCfg(PugiXmlNode RootElement);
	int InitUpGradeProbCfg(PugiXmlNode RootElement);

	LevelConfig m_level_list[UPGRADE_MAX_LEVEL + 1];
	GradeConfig m_grade_cfg_list[UPGRADE_MAX_GRADE + 1];

	std::map<int, UpGradeProbList> m_upgrade_prob_map;

	//形象相关---------------------------------------------------------------
	int InitImageCfg(PugiXmlNode RootElement);
	int InitImageUpgradeCfg(PugiXmlNode RootElement);

	ImageConfig m_image_list[UPGRADE_IMAGE_MAX_COUNT];

	typedef std::map<int, ImgUpgradeCfgList> ImgUpgradeCfgMap;
	ImgUpgradeCfgMap m_special_img_upgrade_map;

	//装备相关---------------------------------------------------------------
	int InitEquipCfg(PugiXmlNode RootElement);

	EquipConifg m_equip_cfg_list[UPGRADE_EQUIP_COUNT];
	int m_equip_skill_level_cfg[UPGRADE_EQUIP_MAX_LEVEL + 1];

	//技能相关---------------------------------------------------------------
	int InitSkillCfg(PugiXmlNode RootElement);

	SkillConifg m_skill_cfg_list[UPGRADE_SKILL_COUNT][UPGRADE_SKILL_MAX_LEVEL + 1];

	//大目标相关
	int InitSuperSpecialCapAddCfg(PugiXmlNode RootElement);
	typedef std::map<int, SuperSpecialCapAddCfgItem> SuperSpecialCapAddCfg;
	SuperSpecialCapAddCfg m_super_special_cap_cfg;
};

#endif

