#ifndef WINGCONFIG_HPP
#define WINGCONFIG_HPP


#include <map>
#include "servercommon/struct/itemlistparam.h"
#include "item/itembase.h"
#include "servercommon/wingdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

// 羽翼等级
struct WingLevelConfig
{
	WingLevelConfig() : wing_level(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0)
	{
	}

	int wing_level;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
};

// 羽翼进阶
struct WingGradeConfig
{
	WingGradeConfig() : grade(0), client_grade(0), upgrade_stuff_id(0), upgrade_stuff2_id(0), upgrade_stuff_count(0),
		coin(0), active_image_id(0), bless_val_limit(0), is_clear_bless(false), bless_addition(0), shuxingdan_limit(0), chengzhangdan_limit(0),equip_level_limit(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), is_broadcast(false), pack_num(0), equip_level_toplimit(0),
		extra_zengshang(0), extra_mianshang(0), per_jingzhun(0), per_baoji(0), per_mianshang(0), per_pofang(0), pve_zengshang_per(0), pvp_jianshang_per(0), pve_jianshang_per(0)
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

// 羽翼进阶成功率
struct WingUpGradeProbConfig
{
	WingUpGradeProbConfig() : low_bless_val(0), high_bless_val(0), succ_rate(0),
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

// 形象配置
struct WingImageConfig								 
{
	WingImageConfig() : image_id(0)
	{
		memset(image_name, 0, sizeof(image_name));
	}

	int image_id;
	GameName image_name;
};

// 特殊形象
struct WingSpecialImgCfg
{
	WingSpecialImgCfg() : image_id(0), active_item_id(0), zhuanzhi_prof(0) {}

	bool IsValid() { return 0 != image_id; }

	int image_id;
	ItemID active_item_id;
	unsigned short zhuanzhi_prof;
};

//特殊形象进阶
struct WingSpecialImgUpgradeCfg
{
	WingSpecialImgUpgradeCfg() : image_id(0), grade(0), stuff_id(0), stuff_num(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), extra_zengshang(0), extra_mianshang(0),
		per_jingzhun(0), per_baoji(0), per_mianshang(0), per_pofang(0), pvp_zengshang_per(0), pve_zengshang_per(0), pvp_jianshang_per(0), pve_jianshang_per(0), 
		shuxingdan_count(0), chengzhangdan_count(0), equip_level(0), per_maxhp(0)  {}

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
	int per_maxhp;
};

// 羽翼装备
struct WingEquipConifg
{
	WingEquipConifg() : max_level(0) {}

	int max_level;

	struct WingEquipLevelInfo
	{
		WingEquipLevelInfo(): maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), add_percent(0) {}

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

	WingEquipLevelInfo level_cfg[WingParam::EQUIP_MAX_LEVEL];
};

// 羽翼技能
struct WingSkillConifg
{
	WingSkillConifg() : index(0), skill_type(0), skill_level(0), wing_grade(0),
		uplevel_stuff_id(0), end_day_of_up_skill(0), uplevel_stuff_num(0), param_a(0), param_b(0), param_c(0), param_d(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), skill_cap_per(0), base_attr_add_per(0) {}

	int index;
	int skill_type;
	int skill_level;
	int wing_grade;
	ItemID uplevel_stuff_id;
	short end_day_of_up_skill;
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
	int base_attr_add_per;
};

struct WingUpStarExpCfg
{
	WingUpStarExpCfg(): up_star_level_exp(0), up_start_total_exp(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0){}

	int up_star_level_exp;
	int up_start_total_exp;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
};

struct WingUpStarStuffCfg
{
	WingUpStarStuffCfg(): up_star_item_index(0), up_star_item_id(0), star_exp(0), baoji_per(0), baoji_mul(0){}

	short up_star_item_index;
	ItemID up_star_item_id;
	int star_exp;
	int baoji_per;
	int baoji_mul;
};

struct WingHuanHuaSpecialCapAddCfgItem
{
	WingHuanHuaSpecialCapAddCfgItem() : huanhua_id(-1), huanhua_level(0), maxhp(0), gongji(0), fangyu(0) {}

	int huanhua_id;
	int huanhua_level;
	int maxhp;
	int gongji;
	int fangyu;
};

class WingConfig : public ConfigBase
{
public:
	WingConfig();
	~WingConfig();

	bool Init(const std::string &configname, std::string *err);

	const WingLevelConfig * GetLevelConfig(int wing_level) const;

	const WingGradeConfig * GetGradeConfig(int grade) const;
	const WingUpGradeProbConfig * GetUpGradeProbConfig(int grade, int bless_val) const;
	const WingImageConfig * GetWingImage(int wing_attr_id) const;

	int GetUseJinjieCardRewardItemID() { return m_use_jinjie_card_reward_itemid; }
	int GetChengZhangDanPercent() { return m_chengzhangdan_percent; }
	int GetTemporaryImageValidTime(){return m_temporary_image_time;}
	int GetActiveEquipGrade() {return m_active_equip_grade;}
	int GetActiveEquipSkillLevel() {return m_active_equip_skill_level;}
	int GetEquipSkillLevel(int min_equip_level);
	short GetMedalSkillIndex() { return m_medal_skill_index; }
	bool IsAddSpecialCap();

	const WingSpecialImgCfg * GetSpecialImgCfg(int special_img_id);
	const WingSpecialImgCfg * GetSpecialImgCfgByItemID(ItemID item_id);
	const WingSpecialImgUpgradeCfg * GetSpecialImgUpgradeCfg(int special_img_id, int grade);

	const WingEquipConifg * GetEquipCfg(int equip_idx);

	const WingSkillConifg * GetSkillCfg(int skill_idx, int level);

	const WingUpStarExpCfg * GetUpStarExpCfg(int star_level);
	const WingUpStarStuffCfg * GetUpStarStuffCfg(int stuff_index);

	const WingHuanHuaSpecialCapAddCfgItem* GetHuanHuaSpecialCapAddCfg(int huanhua_id)const;

private:
	int InitOther(PugiXmlNode RootElement);
	int InitLevelCfg(PugiXmlNode RootElement);
	int InitWingGradeCfg(PugiXmlNode RootElement);
	int InitUpGradeProbCfg(PugiXmlNode RootElement);
	int InitImageCfg(PugiXmlNode RootElement);
	int InitSpecialImageCfg(PugiXmlNode RootElement);
	int InitSpecialImageUpgradeCfg(PugiXmlNode RootElement);
	int InitEquipCfg(PugiXmlNode RootElement);
	int InitSkillCfg(PugiXmlNode RootElement);
	int InitUpStarExpCfg(PugiXmlNode RootElement);
	int InitUpStarStuffCfg(PugiXmlNode RootElement);
	int InitEquipSkillLevel(PugiXmlNode RootElement);
	int InitHuanHuaSpecialCapAddCfg(PugiXmlNode RootElement);

	int m_use_jinjie_card_reward_itemid;
	int m_chengzhangdan_percent;
	int m_temporary_image_time;
	short m_active_equip_grade;
	short m_active_equip_skill_level;
	bool m_is_open_special_cap_add;
	short m_medal_skill_index;

	//////////////////////////////////////////////////////////////////////////
	WingLevelConfig m_level_list[WingParam::MAX_LEVEL + 1];
	int m_equip_skill_level_cfg[WingParam::EQUIP_MAX_LEVEL];

	//////////////////////////////////////////////////////////////////////////
	WingGradeConfig m_grade_cfg_list[WingParam::MAX_GRADE];

	struct WingUpGradeProbList
	{
		static const int MAX_PROB_COUNT_PER_GRADE = 600;

		WingUpGradeProbList() : grade(0), prob_count(0) {}

		bool Add(WingUpGradeProbConfig &cfg)
		{
			if (prob_count >= MAX_PROB_COUNT_PER_GRADE) return false;

			prob_list[prob_count ++] = cfg;
			return true;
		}

		short grade;
		short prob_count;
		WingUpGradeProbConfig prob_list[MAX_PROB_COUNT_PER_GRADE];
	};

	std::map<int, WingUpGradeProbList> m_upgrade_prob_map;

	WingImageConfig m_image_list[WingParam::MAX_IMAGE_COUNT];

	////////////////////////////////////////////////////////////////////////////
	WingSpecialImgCfg m_special_img_cfglist[WingParam::MAX_SPECIAL_IMAGE_COUNT];

	struct WingSpecialImgUpgradeCfgList
	{
		WingSpecialImgUpgradeCfgList() : max_grade(0) {}

		int max_grade;
		WingSpecialImgUpgradeCfg upgrade_list[WingParam::MAX_UPGRADE_LIMIT + 1];
	};

	typedef std::map<int, WingSpecialImgUpgradeCfgList> WingSpecialImgUpgradeCfgMap;
	WingSpecialImgUpgradeCfgMap m_special_img_upgrade_map;

	////////////////////////////////////////////////////////////////////////////
	WingEquipConifg m_equip_cfg_list[WingParam::EQUIP_COUNT];

	//////////////////////////////////////////////////////////////////////////
	WingSkillConifg m_skill_cfg_list[WingParam::SKILL_COUNT][WingParam::SKILL_MAX_LEVEL + 1];

	WingUpStarExpCfg m_upstar_exp_list[WingParam::MAX_STAR_LEVEL];
	WingUpStarStuffCfg m_upstar_item_list[WingParam::MAX_UP_STAR_ITEM_COUNT];

	typedef std::map<int, WingHuanHuaSpecialCapAddCfgItem> WingHuanHuaSpecialCapAddCfg;
	WingHuanHuaSpecialCapAddCfg m_huanhua_special_cap_cfg;
};

#endif
