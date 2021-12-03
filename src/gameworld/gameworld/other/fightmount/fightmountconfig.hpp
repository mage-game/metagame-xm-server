#ifndef FIGHT_MOUNTOPCONFIG_HPP
#define FIGHT_MOUNTOPCONFIG_HPP

#include <map>
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/struct/fightmountparam.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "item/itembase.h"


// ս������ȼ�
struct FightMountLevelConfig
{
	FightMountLevelConfig() : mount_level(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0)
	{
	}

	int mount_level;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
};

// ս���������
struct FightMountGradeConfig
{
	FightMountGradeConfig() : grade(0), client_grade(0), upgrade_stuff_id(0), upgrade_stuff2_id(0), upgrade_stuff_count(0),
		coin(0), active_image_id(0), bless_val_limit(0), is_clear_bless(false),bless_addition(0), shuxingdan_limit(0), chengzhangdan_limit(0),equip_level_limit(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), movespeed(0), is_broadcast(false), pack_num(0), equip_level_toplimit(0),
		extra_zengshang(0), extra_mianshang(0),	per_jingzhun(0), per_baoji(0), per_mianshang(0), per_pofang(0), pvp_zengshang_per(0), pve_zengshang_per(0), 
		pvp_jianshang_per(0), pve_jianshang_per(0)
	{
	}

	short grade;											// ����
	short client_grade;										// �ͻ��˽���
	ItemID upgrade_stuff_id;								// ���ײ���id
	ItemID upgrade_stuff2_id;								// ���ײ���2id
	short upgrade_stuff_count;								// ���ײ�������
	int coin;												// ͭǮ
	short active_image_id;									// ��������ID
	int bless_val_limit;									// ף��ֵ����
	int is_clear_bless;										// �Ƿ����ף��ֵ
	int bless_addition;										// ף��ֵ�������ӳ���ֱ�
	int shuxingdan_limit;									// ���Ե�����
	int chengzhangdan_limit;								// �ɳ�������
	int equip_level_limit;									// װ���ȼ�����

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	Attribute movespeed;

	bool is_broadcast;										// �Ƿ���

	int pack_num;

	int equip_level_toplimit;								//װ���ȼ�����

	Attribute extra_zengshang;									//�̶�����
	Attribute extra_mianshang;									//�̶�����

	Attribute per_jingzhun;										//��׼���Ƽ��ʣ�
	Attribute per_baoji;											//�����������˺��ʣ�
	Attribute per_mianshang;										//������ֱ�
	Attribute per_pofang;											//������ֱ�

	Attribute pvp_zengshang_per;									//pvp������ֱ�
	Attribute pve_zengshang_per;									//pve������ֱ�
	Attribute pvp_jianshang_per;									//pvp������ֱ�
	Attribute pve_jianshang_per;									//pve������ֱ�
};

// ս��������׳ɹ���
struct FightMountUpGradeProbConfig
{
	FightMountUpGradeProbConfig() : low_bless_val(0), high_bless_val(0), succ_rate(0),
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

	int low_bless_val;										// ף��ֵ����
	int high_bless_val;										// ף��ֵ����
	int succ_rate;											// �ɹ���
	int fail_add_1_bless_rate;								// ʧ�ܼ�1ף��ֵ����
	int fail_add_2_bless_rate;								// ʧ�ܼ�2ף��ֵ����
	int fail_add_3_bless_rate;								// ʧ�ܼ�3ף��ֵ����
};

// ��������
struct FightMountImageConfig								 
{
	FightMountImageConfig() : image_id(0)
	{
		memset(image_name, 0, sizeof(image_name));
	}

	int image_id;
	GameName image_name;
};

// ��������
struct FightMountSpecialImgCfg
{
	FightMountSpecialImgCfg() : image_id(0), zhuanzhi_prof(0) {}

	bool IsValid() { return 0 != image_id; }

	int image_id;
	int zhuanzhi_prof;
};

struct FightMountSpecialImgUpgradeCfg
{
	FightMountSpecialImgUpgradeCfg() : image_id(0), grade(0), stuff_id(0), stuff_num(0),
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
	Attribute extra_zengshang;									//�̶�����
	Attribute extra_mianshang;									//�̶�����

	Attribute per_jingzhun;										//��׼���Ƽ��ʣ�
	Attribute per_baoji;											//�����������˺��ʣ�
	Attribute per_mianshang;										//������ֱ�
	Attribute per_pofang;											//������ֱ�

	Attribute pvp_zengshang_per;									//pvp������ֱ�
	Attribute pve_zengshang_per;									//pve������ֱ�
	Attribute pvp_jianshang_per;									//pvp������ֱ�
	Attribute pve_jianshang_per;									//pve������ֱ�
};

// ս������װ��
struct FightMountEquipConifg
{
	struct FightMountEquipUpLevelConfig
	{
		FightMountEquipUpLevelConfig(): equip_level(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), add_percent(0) {}

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

	FightMountEquipConifg() : equip_idx(0), max_level(0) {}

	int equip_idx;
	int max_level;

	FightMountEquipUpLevelConfig equip_level_cfg_list[FightMountParam::EQUIP_MAX_LEVEL];
};


// ս�����＼��
struct FightMountSkillConifg
{
	FightMountSkillConifg() : index(0), skill_type(0), skill_level(0), mount_grade(0),
		uplevel_stuff_id(0), end_day_of_up_skill(0), uplevel_stuff_num(0), param_a(0), param_b(0), param_c(0), param_d(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), skill_cap_per(0), base_attr_add_per(0) {}

	int index;
	int skill_type;
	int skill_level;
	int mount_grade;
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

// ս���������Ǿ���ֵ
struct FightMountUpStarExpCfg
{
	FightMountUpStarExpCfg() : up_star_level_exp(0), up_star_total_exp(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), movespeed(0) {}
	int up_star_level_exp;
	int up_star_total_exp;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	Attribute movespeed;
};

// ս������������Ʒ
struct FightMountUpStarStuffCfg
{
	FightMountUpStarStuffCfg(): up_star_item_index(0), up_star_item_id(0), star_exp(0), baoji_per(0), baoji_mul(0){}

	short up_star_item_index;
	ItemID up_star_item_id;
	int star_exp;
	int baoji_per;
	int baoji_mul;
};

struct FightMountHuanHuaSpecialCapAddCfgItem
{
	FightMountHuanHuaSpecialCapAddCfgItem() : huanhua_id(-1), huanhua_level(0), maxhp(0), gongji(0), fangyu(0) {}

	int huanhua_id;
	int huanhua_level;
	int maxhp;
	int gongji;
	int fangyu;
};

class FightMountOpConfig : public ConfigBase
{
public:
	FightMountOpConfig();
	~FightMountOpConfig();

	bool Init(const std::string &configname, std::string *err);

	const FightMountLevelConfig * GetLevelConfig(int mount_level) const;

	const FightMountGradeConfig * GetGradeConfig(int grade) const;
	const FightMountUpGradeProbConfig * GetUpGradeProbConfig(int grade, int bless_val) const;
	const FightMountImageConfig * GetFightMountImage(int mount_attr_id) const;

	int GetUseJinjieCardRewardItemID() { return m_use_jinjie_card_reward_itemid; }
	int GetChengZhangDanPercent() { return m_chengzhangdan_percent; }
	int GetActiveEquipGrade() { return m_active_equip_grade; }
	int GetActiveEquipSkillLevel() {return m_active_equip_skill_level; }
	int GetEquipSkillLevel(int min_equip_level);
	short GetMedalSkillIndex() { return m_medal_skill_index; }

	const FightMountSpecialImgCfg * GetSpecialImgCfg(int special_img_id);
	const FightMountSpecialImgUpgradeCfg * GetSpecialImgUpgradeCfg(int special_img_id, int grade);

	const FightMountEquipConifg * GetEquipCfg(int equip_idx);

	const FightMountSkillConifg * GetSkillCfg(int skill_idx, int level);
	const FightMountUpStarExpCfg * GetUpStarExpCfg(int star_level);
	const FightMountUpStarStuffCfg * GetUpStarStuffCfg(int stuff_index);

	const FightMountHuanHuaSpecialCapAddCfgItem* GetHuanHuaSpecialCapAddCfg(int huanhua_id)const;
	bool IsAddSpecialCap();

private:
	int InitOther(PugiXmlNode RootElement);
	int InitLevelCfg(PugiXmlNode RootElement);
	int InitFightMountGradeCfg(PugiXmlNode RootElement);
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
	int m_active_equip_grade;
	int m_active_equip_skill_level;
	bool m_is_open_special_cap_add;
	short m_medal_skill_index;

	//////////////////////////////////////////////////////////////////////////
	FightMountLevelConfig m_level_list[FightMountParam::MAX_LEVEL + 1];

	//////////////////////////////////////////////////////////////////////////
	FightMountGradeConfig m_grade_cfg_list[FightMountParam::MAX_GRADE];

	struct FightMountUpGradeProbList
	{
		static const int MAX_PROB_COUNT_PER_GRADE = 600;

		FightMountUpGradeProbList() : grade(0), prob_count(0) {}

		bool Add(FightMountUpGradeProbConfig &cfg)
		{
			if (prob_count >= MAX_PROB_COUNT_PER_GRADE) return false;
			
			prob_list[prob_count ++] = cfg;
			return true;
		}

		short grade;
		short prob_count;
		FightMountUpGradeProbConfig prob_list[MAX_PROB_COUNT_PER_GRADE];
	};

	std::map<int, FightMountUpGradeProbList> m_upgrade_prob_map;

	FightMountImageConfig m_image_list[FightMountParam::MAX_IMAGE_COUNT];

	////////////////////////////////////////////////////////////////////////////
	FightMountSpecialImgCfg m_special_img_cfglist[FightMountParam::MAX_SPECIAL_IMAGE_COUNT];

	struct FightMountSpecialImgUpgradeCfgList
	{
		FightMountSpecialImgUpgradeCfgList() : max_grade(0) {}

		int max_grade;
		FightMountSpecialImgUpgradeCfg upgrade_list[FightMountParam::MAX_UPGRADE_LIMIT + 1];
	};

	typedef std::map<int, FightMountSpecialImgUpgradeCfgList> FightMountSpecialImgUpgradeCfgMap;
	FightMountSpecialImgUpgradeCfgMap m_special_img_upgrade_map;

	////////////////////////////////////////////////////////////////////////////
	FightMountEquipConifg m_equip_cfg_list[FightMountParam::EQUIP_COUNT];
	int m_equip_skill_level_cfg[FightMountParam::EQUIP_MAX_LEVEL];

	//////////////////////////////////////////////////////////////////////////
	FightMountSkillConifg m_skill_cfg_list[FightMountParam::SKILL_COUNT][FightMountParam::SKILL_MAX_LEVEL + 1];

	FightMountUpStarExpCfg m_upstar_exp_list[FightMountParam::MAX_STAR_LEVEL];
	FightMountUpStarStuffCfg m_upstar_item_list[FightMountParam::MAX_UP_STAR_ITEM_COUNT];

	typedef std::map<int, FightMountHuanHuaSpecialCapAddCfgItem> FightMountHuanHuaSpecialCapAddCfg;
	FightMountHuanHuaSpecialCapAddCfg m_huanhua_special_cap_cfg;
};

#endif

