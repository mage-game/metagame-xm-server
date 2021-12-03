#ifndef __SHIZHUANG_CONFIG_H__
#define __SHIZHUANG_CONFIG_H__

#include "servercommon/serverdef.h"
#include "servercommon/configcommon.h"
#include "servercommon/configbase.h"
#include "servercommon/struct/shizhuangparam.hpp"
#include <map>
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

static const int SHIZHUANG_MAX_GRADE = 30;					// 时装进阶最大阶数
static const int SHIZHUANG_SPECIAL_IMG_MAX_GRADE = 100;		// 时装特殊形象进阶最大阶数
const static int MAX_SKILL_COUNT = 4;                       // 最大的技能数量
const static int SKILL_MAX_LEVEL = 100;                      // 最大的技能等级
const static int EQUIP_COUNT = 4;                           // 装备数量
const static int EQUIP_MAX_LEVEL = 151;                     // 装备最大等级

// 进阶
struct ShizhuangGradeConfig
{
	ShizhuangGradeConfig() : grade(0), client_grade(0), upgrade_stuff_id(0), upgrade_stuff2_id(0), upgrade_stuff_count(0),
		coin(0), active_image_id(0), bless_val_limit(0), is_clear_bless(false), bless_addition(0), shuxingdan_limit(0), chengzhangdan_limit(0), equip_level_limit(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), movespeed(0), is_broadcast(false), pack_num(0), equip_level_toplimit(0),	extra_zengshang(0), extra_mianshang(0),
		per_jingzhun(0), per_baoji(0), per_mianshang(0), per_pofang(0), pvp_zengshang_per(0), pve_zengshang_per(0), pvp_jianshang_per(0), pve_jianshang_per(0)
	{
	}

	short grade;											// 阶数
	short client_grade;										// 客户端阶数
	ItemID upgrade_stuff_id;								// 进阶材料id
	ItemID upgrade_stuff2_id;								// 进阶材料2id
	short upgrade_stuff_count;								// 进阶材料数量
	int coin;												// 铜钱
	short active_image_id;									// 激活形象ID
	int bless_val_limit;									// 祝福值上限
	int is_clear_bless;										// 是否清空祝福值
	int bless_addition;										// 祝福值进度条加成万分比
	int shuxingdan_limit;									// 属性丹限制
	int chengzhangdan_limit;								// 成长丹限制
	int equip_level_limit;									// 装备等级限制

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	Attribute movespeed;

	bool is_broadcast;										// 是否传闻

	int pack_num;
	int equip_level_toplimit;								//装备等级上限

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
};

// 进阶成功率
struct ShizhuangUpGradeProbConfig
{
	ShizhuangUpGradeProbConfig() : low_bless_val(0), high_bless_val(0), succ_rate(0),
		fail_add_1_bless_rate(0), fail_add_2_bless_rate(0), fail_add_3_bless_rate(0)
	{
	}

	short RandAddBlessVal() const
	{
		int rand_val = RandomNum(100);
		if (rand_val < fail_add_1_bless_rate) return 8;
		else if (rand_val < fail_add_2_bless_rate) return 10;
		else return 12;
	}

	int low_bless_val;										// 祝福值下限
	int high_bless_val;										// 祝福值上限
	int succ_rate;											// 成功率
	int fail_add_1_bless_rate;								// 失败加1祝福值概率
	int fail_add_2_bless_rate;								// 失败加2祝福值概率
	int fail_add_3_bless_rate;								// 失败加3祝福值概率
};

// 进阶成功率列表
struct ShizhuangUpGradeProbList
{
	static const int MAX_PROB_COUNT_PER_GRADE = 600;

	ShizhuangUpGradeProbList() : grade(0), prob_count(0) {}

	bool Add(ShizhuangUpGradeProbConfig &cfg)
	{
		if (prob_count >= MAX_PROB_COUNT_PER_GRADE) return false;

		prob_list[prob_count++] = cfg;
		return true;
	}

	short grade;
	short prob_count;
	ShizhuangUpGradeProbConfig prob_list[MAX_PROB_COUNT_PER_GRADE];
};

// 特殊形象
struct ShizhuangSpecialImgCfg
{
	ShizhuangSpecialImgCfg() : image_id(0), valid_time_s(0), active_item_id(0){}

	bool IsValid() { return 0 != image_id; }

	int image_id;
	int valid_time_s;				// 有效时间（从激活时算起）
	ItemID active_item_id;
};

struct ShizhuangSpecialImgUpgradeCfg
{
	ShizhuangSpecialImgUpgradeCfg() : image_id(0), grade(0), stuff_id(0), stuff_num(0),
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

struct ShizhuangSpecialImgUpgradeCfgList
{
	ShizhuangSpecialImgUpgradeCfgList() : max_grade(0) {}

	int max_grade;
	ShizhuangSpecialImgUpgradeCfg upgrade_list[SHIZHUANG_SPECIAL_IMG_MAX_GRADE + 1];
};

struct ShizhuangEquipInfoCfg
{
	ShizhuangEquipInfoCfg() : max_level(0) {}

	int max_level;

	struct ShizhuangEquipLevelInfo
	{
		ShizhuangEquipLevelInfo() : maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), add_percent(0) {}

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

	ShizhuangEquipLevelInfo level_cfg[EQUIP_MAX_LEVEL];
};

struct ShizhuangSkillCfg
{
	ShizhuangSkillCfg():index(0), skill_type(0), skill_level(0), shizhuang_grade(0),
		uplevel_stuff_id(0), uplevel_stuff_num(0), param_a(0), param_b(0), param_c(0), param_d(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0) ,capability(0), skill_cap_per(0){}
	
	int index;
	int skill_type;
	int skill_level;
	int shizhuang_grade;
	ItemID uplevel_stuff_id;
	int uplevel_stuff_num;

	int param_a;
	int param_b;
	int param_c;
	int param_d;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	int capability;
	Attribute skill_cap_per;
};

struct ShizhuangHuanHuaSpecialCapAddCfgItem
{
	ShizhuangHuanHuaSpecialCapAddCfgItem() : huanhua_id(-1), huanhua_level(0), maxhp(0), gongji(0), fangyu(0) {}

	int huanhua_id;
	int huanhua_level;
	int maxhp;
	int gongji;
	int fangyu;
};


class ShizhuangConfig : public ConfigBase
{
public:
	ShizhuangConfig();
	~ShizhuangConfig();

	bool Init(std::string path, std::string *err);

	const ShizhuangGradeConfig * GetShizhuangGradeConfig(int grade) const;
	const ShizhuangUpGradeProbConfig * GetShizhuangUpGradeProbConfig(int grade, int bless_val) const;
	const ShizhuangSpecialImgCfg * GetShizhuangSpecialImgCfg(int special_img_id);
	const ShizhuangSpecialImgUpgradeCfg * GetShizhuangSpecialImgUpgradeCfg(int special_img_id, int grade);
	const ShizhuangEquipInfoCfg * GetShizhuangEquipInfoCfg(int equip_idx);
	const ShizhuangSkillCfg * GetShizhuangSkillCfg(int index, int skill_level);
	

	const ShizhuangGradeConfig * GetWeaponGradeConfig(int grade) const;
	const ShizhuangUpGradeProbConfig * GetWeaponUpGradeProbConfig(int grade, int bless_val) const;
	const ShizhuangSpecialImgCfg * GetWeaponSpecialImgCfg(int special_img_id);
	const ShizhuangSpecialImgUpgradeCfg * GetWeaponSpecialImgUpgradeCfg(int special_img_id, int grade);
	const ShizhuangEquipInfoCfg * GetWeaponEquipInfoCfg(int equip_idx);
	const ShizhuangSkillCfg * GetWeaponSkillCfg(int index, int skill_level);

	const ShizhuangGradeConfig * GetGradeConfigByType(int grade, int type) const;

	int  GetEquipSkillLevelCfg(int min_equip_level);
	int GetActiveEquipSkillLevel() { return m_active_equip_skill_level; }
	int GetACtiveEquipGrade() { return m_active_equip_grade; }

	const ShizhuangHuanHuaSpecialCapAddCfgItem* GetShizhuangHuanHuaSpecialCapAddCfg(int huanhua_id)const;
	const ShizhuangHuanHuaSpecialCapAddCfgItem* GetShizhuangWuqiHuanHuaSpecialCapAddCfg(int huanhua_id)const;
	bool IsAddSpecialCap();

private:
	int InitOther(PugiXmlNode RootElement);
	int InitShizhuangUpGradeCfg(PugiXmlNode RootElement);
	int InitShizhuangUpGradeProbCfg(PugiXmlNode RootElement);
	int InitShizhuangSpecialImageCfg(PugiXmlNode RootElement);
	int InitShizhuangSpecialImageUpgradeCfg(PugiXmlNode RootElement);
	int InitShizhuangEquipInfoCfg(PugiXmlNode RootElement);
	int InitShizhuangSkillCfg(PugiXmlNode RootElement);

	int InitWeaponUpGradeCfg(PugiXmlNode RootElement);
	int InitWeaponUpGradeProbCfg(PugiXmlNode RootElement);
	int InitWeaponSpecialImageCfg(PugiXmlNode RootElement);
	int InitWeaponSpecialImageUpgradeCfg(PugiXmlNode RootElement);
	int InitWeaponEquipInfoCfg(PugiXmlNode RootElement);
	int InitWeaponSkillCfg(PugiXmlNode RootElement);

	int InitEquipSkillLevelCfg(PugiXmlNode RootElement);
	int InitShizhuangHuanHuaSpecialCapAddCfg(PugiXmlNode RootElement);
	int InitShizhuangWuqiHuanHuaSpecialCapAddCfg(PugiXmlNode RootElement);

	typedef std::map<int, ShizhuangSpecialImgUpgradeCfgList> ShizhuangSpecialImgUpgradeCfgMap;

	// 时装
	ShizhuangGradeConfig m_shizhuang_grade_cfg_list[SHIZHUANG_MAX_GRADE];
	std::map<int, ShizhuangUpGradeProbList> m_shizhuang_upgrade_prob_map;
	ShizhuangSpecialImgCfg m_shizhuang_special_img_cfglist[SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT];
	ShizhuangSpecialImgUpgradeCfgMap m_shizhuang_special_img_upgrade_map;
	ShizhuangEquipInfoCfg m_shizhuang_equip_info_list[EQUIP_COUNT];
	ShizhuangSkillCfg m_shizhuang_skill_cfg_list[MAX_SKILL_COUNT][SKILL_MAX_LEVEL + 1];

	
	// 由于武器的进阶跟进阶成功率，跟时装的一样，所以结构体就用同样的了，后续若要搞成不一样，再为武器重新复制一份配置结构体吧
	ShizhuangGradeConfig m_weapon_grade_cfg_list[SHIZHUANG_MAX_GRADE];
	std::map<int, ShizhuangUpGradeProbList> m_weapon_upgrade_prob_map;
	ShizhuangSpecialImgCfg m_weapon_special_img_cfglist[SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT];
	ShizhuangSpecialImgUpgradeCfgMap m_weapon_special_img_upgrade_map;
	ShizhuangEquipInfoCfg m_weapon_equip_info_list[EQUIP_COUNT];
	ShizhuangSkillCfg m_weapon_skill_cfg_list[MAX_SKILL_COUNT][SKILL_MAX_LEVEL + 1];

	int m_equip_skill_level_cfg[EQUIP_MAX_LEVEL];
	short m_active_equip_skill_level;                              // 激活装备技能需要装备达到的等级
	int m_active_equip_grade;
	bool m_is_open_special_cap_add;
	                                                              
	typedef std::map<int, ShizhuangHuanHuaSpecialCapAddCfgItem> ShizhuangHuanHuaSpecialCapAddCfg;
	ShizhuangHuanHuaSpecialCapAddCfg m_shizhuang_huanhua_special_cap_cfg;

	typedef std::map<int, ShizhuangHuanHuaSpecialCapAddCfgItem> ShizhuangWuqiHuanHuaSpecialCapAddCfg;
	ShizhuangWuqiHuanHuaSpecialCapAddCfg m_shizhuang_wuqi_huanhua_special_cap_cfg;
};

#endif

