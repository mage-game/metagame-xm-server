#ifndef SHENYICONFIG_HPP
#define SHENYICONFIG_HPP

#include "gamedef.h"
#include "servercommon/basedef.h"
#include <map>

#include "servercommon/struct/itemlistparam.h"

#include "item/itembase.h"

#include "obj/character/attribute.hpp"
#include "servercommon/shenyidef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
// struct ShenyiOtherConfig
// {
// 	ShenyiOtherConfig() : extra_attrs_per_grade(0), extra_attrs_per(0) {} 
// 	
// 	int extra_attrs_per_grade;
// 	int extra_attrs_per;
// };

// 神翼等级
struct ShenyiLevelConfig
{
	ShenyiLevelConfig() : shenyi_level(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0)
	{
	}

	int shenyi_level;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
};

// 神翼进阶
struct ShenyiGradeConfig
{
	ShenyiGradeConfig() : grade(0), client_grade(0), upgrade_stuff_id(0), upgrade_stuff2_id(0), upgrade_stuff_count(0),
		coin(0), active_image_id(0), bless_val_limit(0), is_clear_bless(false), bless_addition(0), shuxingdan_limit(0), chengzhangdan_limit(0),equip_level_limit(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), is_broadcast(false), pack_num(0), equip_level_toplimit(0), 
		per_jingzhun(0), per_baoji(0), jianshang_per(0), zengshang_per(0)
	{
	}

	short grade;											// 阶数
	short client_grade;										// 客户端阶数
	ItemID upgrade_stuff_id;								// 进阶材料id
	ItemID upgrade_stuff2_id;								// 进阶材料2id
	short upgrade_stuff_count;								// 进阶材料数量
	int coin;												// 铜钱
	short active_image_id;									// 激活形象ID
	int bless_val_limit;							// 祝福值上限
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

	Attribute per_jingzhun;										//精准（破甲率）
	Attribute per_baoji;											//暴击（暴击伤害率）
	Attribute jianshang_per;										//减伤万分比
	Attribute zengshang_per;										//增伤万分比
};

// 神翼进阶成功率
struct ShenyiUpGradeProbConfig
{
	ShenyiUpGradeProbConfig() : low_bless_val(0), high_bless_val(0), succ_rate(0),
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
struct ShenyiImageConfig								 
{
	ShenyiImageConfig() : image_id(0)
	{
		memset(image_name, 0, sizeof(image_name));
	}

	int image_id;
	GameName image_name;
};

// 特殊形象
struct ShenyiSpecialImgCfg
{
	ShenyiSpecialImgCfg() : image_id(0){}

	bool IsValid() { return 0 != image_id; }

	int image_id;
};

//特殊形象进阶
struct ShenyiSpecialImgUpgradeCfg
{
	ShenyiSpecialImgUpgradeCfg() : image_id(0), grade(0), stuff_id(0), stuff_num(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0),
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

	int shuxingdan_count;
	int chengzhangdan_count;
	int equip_level;
};

// 神翼装备
struct ShenyiEquipConifg
{
	struct ShenyiEquipUpLevelConfig
	{
		ShenyiEquipUpLevelConfig(): equip_level(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), add_percent(0) {}

		int equip_level;
		ItemConfigData item;

		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
		Attribute baoji;
		Attribute jianren;
		int add_percent;
	};

	ShenyiEquipConifg() : equip_idx(0), max_level(0) {}

	int equip_idx;
	int max_level;

	ShenyiEquipUpLevelConfig equip_level_cfg_list[ShenyiParam::EQUIP_MAX_LEVEL];
};

struct ShenyiSkillConifg
{
	ShenyiSkillConifg() : index(0), skill_type(0), skill_level(0), shenyi_grade(0),
		uplevel_stuff_id(0), uplevel_stuff_num(0), param_a(0), param_b(0), param_c(0), param_d(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), skill_cap_per(0){}

	int index;
	int skill_type;
	int skill_level;
	int shenyi_grade;
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

struct ShenyiUpStarExpCfg
{
	ShenyiUpStarExpCfg(): up_star_level_exp(0), up_start_total_exp(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0){}

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

struct ShenyiUpStarStuffCfg
{
	ShenyiUpStarStuffCfg(): up_star_item_index(0), up_star_item_id(0), star_exp(0), baoji_per(0), baoji_mul(0){}

	short up_star_item_index;
	ItemID up_star_item_id;
	int star_exp;
	int baoji_per;
	int baoji_mul;
};

struct ShenyiHuanHuaSpecialCapAddCfgItem
{
	ShenyiHuanHuaSpecialCapAddCfgItem() : huanhua_id(-1), huanhua_level(0), maxhp(0), gongji(0), fangyu(0) {}

	int huanhua_id;
	int huanhua_level;
	int maxhp;
	int gongji;
	int fangyu;
};

class ShenyiConfig : public ConfigBase
{
public:
	ShenyiConfig();
	~ShenyiConfig();

	bool Init(const std::string &configname, std::string *err);

//	const ShenyiOtherConfig & GetOtherCfg() { return m_other_cfg; }

	const ShenyiLevelConfig * GetLevelConfig(int shenyi_level) const;

	const ShenyiGradeConfig * GetGradeConfig(int grade) const;
	const ShenyiUpGradeProbConfig * GetUpGradeProbConfig(int grade, int bless_val) const;
	const ShenyiImageConfig * GetShenyiImage(int shenyi_attr_id) const;

	int GetUseJinjieCardRewardItemID() { return m_use_jinjie_card_reward_itemid; }
	int GetChengZhangDanPercent() { return m_chengzhangdan_percent; }
	int GetActiveEquipGrade() { return m_active_equip_grade; }
	int GetActiveEquipSkillLevel() { return m_active_equip_skill_level; }
	int GetEquipSkillLevel(int min_equip_level);

	const ShenyiSpecialImgCfg * GetSpecialImgCfg(int special_img_id);
	const ShenyiSpecialImgUpgradeCfg * GetSpecialImgUpgradeCfg(int special_img_id, int grade);

	const ShenyiEquipConifg * GetOldEquipCfg(int equip_idx);
	const ShenyiEquipConifg * GetEquipCfg(int equip_idx);

	const ShenyiSkillConifg * GetSkillCfg(int skill_idx, int level);

	int GetMaxStarLevel() { return m_max_star_level; }
	const ShenyiUpStarExpCfg * GetUpStarExpCfg(int star_level);
	const ShenyiUpStarStuffCfg * GetUpStarStuffCfg(int stuff_index);
	const ShenyiHuanHuaSpecialCapAddCfgItem* GetHuanHuaSpecialCapAddCfg(int huanhua_id)const;
	bool IsAddSpecialCap();

private:
	int InitOther(PugiXmlNode RootElement);
	int InitLevelCfg(PugiXmlNode RootElement);
	int InitShenyiGradeCfg(PugiXmlNode RootElement);
	int InitUpGradeProbCfg(PugiXmlNode RootElement);
	int InitImageCfg(PugiXmlNode RootElement);
	int InitSpecialImageCfg(PugiXmlNode RootElement);
	int InitSpecialImageUpgradeCfg(PugiXmlNode RootElement);
	int InitOldEquipCfg(PugiXmlNode RootElement);
	int InitEquipCfg(PugiXmlNode RootElement);
	int InitSkillCfg(PugiXmlNode RootElement);
	int InitUpStarExpCfg(PugiXmlNode RootElement);
	int InitUpStarStuffCfg(PugiXmlNode RootElement);
	int InitEquipSkillLevel(PugiXmlNode RootElement);
	int InitHuanHuaSpecialCapAddCfg(PugiXmlNode RootElement);
	//ShenyiOtherConfig m_other_cfg;

	int m_use_jinjie_card_reward_itemid;
	int m_chengzhangdan_percent;
	int m_active_equip_grade;
	int m_active_equip_skill_level;

	//////////////////////////////////////////////////////////////////////////
	ShenyiLevelConfig m_level_list[ShenyiParam::MAX_LEVEL + 1];

	//////////////////////////////////////////////////////////////////////////
	ShenyiGradeConfig m_grade_cfg_list[ShenyiParam::MAX_GRADE];

	struct ShenyiUpGradeProbList
	{
		static const int MAX_PROB_COUNT_PER_GRADE = 600;

		ShenyiUpGradeProbList() : grade(0), prob_count(0) {}

		bool Add(ShenyiUpGradeProbConfig &cfg)
		{
			if (prob_count >= MAX_PROB_COUNT_PER_GRADE) return false;

			prob_list[prob_count ++] = cfg;
			return true;
		}

		short grade;
		short prob_count;
		ShenyiUpGradeProbConfig prob_list[MAX_PROB_COUNT_PER_GRADE];
	};

	std::map<int, ShenyiUpGradeProbList> m_upgrade_prob_map;

	ShenyiImageConfig m_image_list[ShenyiParam::MAX_IMAGE_COUNT];

	////////////////////////////////////////////////////////////////////////////
	ShenyiSpecialImgCfg m_special_img_cfglist[ShenyiParam::MAX_SPECIAL_IMAGE_COUNT];

	struct ShenyiSpecialImgUpgradeCfgList
	{
		ShenyiSpecialImgUpgradeCfgList() : max_grade(0) {}

		int max_grade;
		ShenyiSpecialImgUpgradeCfg upgrade_list[ShenyiParam::MAX_UPGRADE_LIMIT + 1];
	};

	typedef std::map<int, ShenyiSpecialImgUpgradeCfgList> ShenyiSpecialImgUpgradeCfgMap;
	ShenyiSpecialImgUpgradeCfgMap m_special_img_upgrade_map;

	////////////////////////////////////////////////////////////////////////////
	ShenyiEquipConifg m_old_equip_cfg_list[ShenyiParam::EQUIP_COUNT];
	ShenyiEquipConifg m_equip_cfg_list[ShenyiParam::EQUIP_COUNT];
	int m_equip_skill_level_cfg[ShenyiParam::EQUIP_MAX_LEVEL+1];

	//////////////////////////////////////////////////////////////////////////
	ShenyiSkillConifg m_skill_cfg_list[ShenyiParam::SKILL_COUNT][ShenyiParam::SKILL_MAX_LEVEL + 1];

	int m_max_star_level;
	ShenyiUpStarExpCfg m_upstar_exp_list[ShenyiParam::MAX_STAR_LEVEL];
	ShenyiUpStarStuffCfg m_upstar_item_list[ShenyiParam::MAX_UP_STAR_ITEM_COUNT];

	bool m_is_open_special_cap_add;
	typedef std::map<int, ShenyiHuanHuaSpecialCapAddCfgItem> ShenyiHuanHuaSpecialCapAddCfg;
	ShenyiHuanHuaSpecialCapAddCfg m_huanhua_special_cap_cfg;
	
};

#endif
