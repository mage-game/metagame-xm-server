#ifndef YAOSHI_CONFIG_HPP
#define YAOSHI_CONFIG_HPP

#include <map>
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/struct/yaoshiparam.hpp"
#include "servercommon/configbase.h"
#include "item/itembase.h"

#define YAOSHI_CFG() LOGIC_CONFIG->GetYaoShiConfig()

class TiXmlElement;

struct YaoShiOtherConfig
{
	YaoShiOtherConfig() : extra_attrs_per_grade(0), extra_attrs_per(0) {}

	int extra_attrs_per_grade;
	int extra_attrs_per;
};

// 腰饰等级
struct YaoShiLevelConfig
{
	YaoShiLevelConfig() : yaoshi_level(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0)
	{
	}

	int yaoshi_level;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
};

// 腰饰进阶
struct YaoShiGradeConfig
{
	YaoShiGradeConfig() : grade(0), client_grade(0), upgrade_stuff_id(0), upgrade_stuff2_id(0), upgrade_stuff_count(0),
		coin(0), active_image_id(0), bless_val_limit(0), is_clear_bless(false),bless_addition(0), shuxingdan_limit(0), chengzhangdan_limit(0),equip_level_limit(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), movespeed(0), is_broadcast(false), pack_num(0), extra_zengshang(0), extra_mianshang(0),
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

// 腰饰进阶成功率
struct YaoShiUpGradeProbConfig
{
	YaoShiUpGradeProbConfig() : low_bless_val(0), high_bless_val(0), succ_rate(0),
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
struct YaoShiImageConfig								 
{
	YaoShiImageConfig() : image_id(0)
	{
		memset(image_name, 0, sizeof(image_name));
	}

	int image_id;
	GameName image_name;
};

// 特殊形象
struct YaoShiSpecialImgCfg
{
	YaoShiSpecialImgCfg() : image_id(0){}

	bool IsValid() { return 0 != image_id; }

	int image_id;
};

struct YaoShiSpecialImgUpgradeCfg
{
	YaoShiSpecialImgUpgradeCfg() : image_id(0), grade(0), stuff_id(0), stuff_num(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0),
		shuxingdan_count(0), chengzhangdan_count(0), equip_level(0), extra_zengshang(0), extra_mianshang(0),
		per_jingzhun(0), per_baoji(0), per_mianshang(0), per_pofang(0), pvp_zengshang_per(0), pve_zengshang_per(0), pvp_jianshang_per(0), pve_jianshang_per(0) {}

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

// 腰饰装备
struct YaoShiEquipConifg
{
	YaoShiEquipConifg() : max_level(0) {}

	int max_level;

	struct YaoShiEquipLevelInfo
	{
		YaoShiEquipLevelInfo(): maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), add_percent(0) {}

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

	YaoShiEquipLevelInfo level_cfg[YaoShiParam::EQUIP_MAX_LEVEL + 1];
};

// 腰饰技能
struct YaoShiSkillConifg
{
	YaoShiSkillConifg() : index(0), skill_type(0), skill_level(0), yaoshi_grade(0),
		uplevel_stuff_id(0), uplevel_stuff_num(0), param_a(0), param_b(0), param_c(0), param_d(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0) , capability(0), skill_cap_per(0){}

	int index;
	int skill_type;
	int skill_level;
	int yaoshi_grade;
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

struct YaoShiUpStarExpCfg
{
	YaoShiUpStarExpCfg(): up_star_level_exp(0), up_start_total_exp(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), movespeed(0) {}

	int up_star_level_exp;
	int up_start_total_exp;

	int maxhp;
	int gongji;
	int fangyu;
	int mingzhong;
	int shanbi;
	int baoji;
	int jianren;
	int movespeed;
};

struct YaoShiUpStarStuffCfg
{
	YaoShiUpStarStuffCfg(): up_star_item_index(0), up_star_item_id(0), star_exp(0), baoji_per(0), baoji_mul(0){}

	short up_star_item_index;
	ItemID up_star_item_id;
	int star_exp;
	int baoji_per;
	int baoji_mul;
};

struct YaoShiHuanHuaSpecialCapAddCfgItem
{
	YaoShiHuanHuaSpecialCapAddCfgItem() : huanhua_id(-1), huanhua_level(0), maxhp(0), gongji(0), fangyu(0) {}

	int huanhua_id;
	int huanhua_level;
	int maxhp;
	int gongji;
	int fangyu;
};

class YaoShiConfig : public ConfigBase
{
public:
	YaoShiConfig();
	~YaoShiConfig();

	bool Init(const std::string &configname, std::string *err);

	const YaoShiOtherConfig & GetOtherCfg() { return m_other_cfg; }

	const YaoShiLevelConfig * GetLevelConfig(int yaoshi_level);
	const YaoShiGradeConfig * GetGradeConfig(int grade);
	const YaoShiUpGradeProbConfig * GetUpGradeProbConfig(int grade, int bless_val);
	const YaoShiImageConfig * GetYaoShiImage(int yaoshi_image_id);

	int GetUseJinjieCardRewardItemID() { return m_use_jinjie_card_reward_itemid; }
	int GetChengZhangDanPercent() { return m_chengzhangdan_percent; }
	int GetTemporaryImageValidTime(){return m_temporary_image_time;}
	int GetActiveEquipGrade() {return m_active_equip_grade;}
	int GetActiveEquipSkillLevel() {return m_active_equip_skill_level;}
	int GetEquipSkillLevel(int min_equip_level);

	const YaoShiSpecialImgCfg * GetSpecialImgCfg(int special_img_id);
	const YaoShiSpecialImgUpgradeCfg * GetSpecialImgUpgradeCfg(int special_img_id, int grade);
	const YaoShiEquipConifg * GetEquipCfg(int equip_idx);
	const YaoShiSkillConifg * GetSkillCfg(int skill_idx, int level);
	const YaoShiUpStarExpCfg * GetUpStarExpCfg(int star_level);
	int GetMaxStarLevel() { return m_max_star_level; }
	const YaoShiHuanHuaSpecialCapAddCfgItem* GetHuanHuaSpecialCapAddCfg(int huanhua_id)const;
	bool IsAddSpecialCap();

	const YaoShiUpStarStuffCfg * GetUpStarStuffCfg(int stuff_index);

	bool IsValidGrade(int grade);
	bool IsValidClientGrade(int client_grade);
	bool IsValidLevel(int level);
	bool IsValidImageID(int image_id);
	bool IsValidSpecialImageID(int image_id);
	bool IsValidSpecialImageGrade(int grade);
	bool IsValidEquipIdx(int equip_idx);
	bool IsValidEquipLevel(int level);
	bool IsValidSkillIdx(int skill_idx);
	bool IsValidSkillLevel(int level);
	bool IsValidStartLevel(int level);
	bool IsValidStuffIdx(int stuff_idx);


private:
	int InitOtherCfg(TiXmlElement *RootElement);
	int InitLevelCfg(TiXmlElement *RootElement);
	int InitGradeCfg(TiXmlElement *RootElement);
	int InitUpGradeProbCfg(TiXmlElement *RootElement);
	int InitImageCfg(TiXmlElement *RootElement);
	int InitSpecialImageCfg(TiXmlElement *RootElement);
	int InitSpecialImageUpgradeCfg(TiXmlElement *RootElement);
	int InitEquipCfg(TiXmlElement *RootElement);
	int InitSkillCfg(TiXmlElement *RootElement);
	int InitUpStarExpCfg(TiXmlElement *RootElement);
	int InitUpStarStuffCfg(TiXmlElement *RootElement);
	int InitEquipSkillLevel(TiXmlElement *RootElement);
	int InitHuanHuaSpecialCapAddCfg(TiXmlElement *RootElement);

	YaoShiOtherConfig m_other_cfg;

	int m_use_jinjie_card_reward_itemid;
	int m_chengzhangdan_percent;
	int m_temporary_image_time;
	short m_active_equip_grade;
	short m_active_equip_skill_level;
	bool m_is_open_special_cap_add;
	int m_max_star_level;

	//////////////////////////////////////////////////////////////////////////
	YaoShiLevelConfig m_level_list[YaoShiParam::MAX_LEVEL + 1];

	//////////////////////////////////////////////////////////////////////////
	YaoShiGradeConfig m_grade_cfg_list[YaoShiParam::MAX_GRADE];

	struct YaoShiUpGradeProbList
	{
		static const int MAX_PROB_COUNT_PER_GRADE = 600;

		YaoShiUpGradeProbList() : grade(0), prob_count(0) {}

		bool Add(YaoShiUpGradeProbConfig &cfg)
		{
			if (prob_count >= MAX_PROB_COUNT_PER_GRADE) return false;
			
			prob_list[prob_count ++] = cfg;
			return true;
		}

		short grade;
		short prob_count;
		YaoShiUpGradeProbConfig prob_list[MAX_PROB_COUNT_PER_GRADE];
	};

	std::map<int, YaoShiUpGradeProbList> m_upgrade_prob_map;

	YaoShiImageConfig m_image_list[YaoShiParam::MAX_IMAGE_COUNT];

	////////////////////////////////////////////////////////////////////////////
	YaoShiSpecialImgCfg m_special_img_cfglist[YaoShiParam::MAX_SPECIAL_IMAGE_COUNT];

	struct YaoShiSpecialImgUpgradeCfgList
	{
		YaoShiSpecialImgUpgradeCfgList() : max_grade(0) {}

		int max_grade;
		YaoShiSpecialImgUpgradeCfg upgrade_list[YaoShiParam::MAX_UPGRADE_LIMIT + 1];
	};

	typedef std::map<int, YaoShiSpecialImgUpgradeCfgList> YaoShiSpecialImgUpgradeCfgMap;
	YaoShiSpecialImgUpgradeCfgMap m_special_img_upgrade_map;

	////////////////////////////////////////////////////////////////////////////
	YaoShiEquipConifg m_old_equip_cfg_list[YaoShiParam::EQUIP_COUNT];
	YaoShiEquipConifg m_equip_cfg_list[YaoShiParam::EQUIP_COUNT];
	int m_equip_skill_level_cfg[YaoShiParam::EQUIP_MAX_LEVEL + 1];

	//////////////////////////////////////////////////////////////////////////
	YaoShiSkillConifg m_skill_cfg_list[YaoShiParam::SKILL_COUNT][YaoShiParam::SKILL_MAX_LEVEL + 1];

	YaoShiUpStarExpCfg m_upstar_exp_list[YaoShiParam::MAX_STAR_LEVEL];
	YaoShiUpStarStuffCfg m_upstar_item_list[YaoShiParam::MAX_UP_STAR_ITEM_COUNT];
	typedef std::map<int, YaoShiHuanHuaSpecialCapAddCfgItem> YaoShiHuanHuaSpecialCapAddCfg;
	YaoShiHuanHuaSpecialCapAddCfg m_huanhua_special_cap_cfg;
};

#endif

