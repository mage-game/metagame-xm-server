#ifndef HALOCONFIG_HPP
#define HALOCONFIG_HPP

#include "gamedef.h"
#include "servercommon/basedef.h"
#include <map>

#include "servercommon/struct/itemlistparam.h"

#include "item/itembase.h"

#include "obj/character/attribute.hpp"
#include "servercommon/halodef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

// �⻷�ȼ�
struct HaloLevelConfig
{
	HaloLevelConfig() : halo_level(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0)
	{
	}

	int halo_level;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
};

// �⻷����
struct HaloGradeConfig
{
	HaloGradeConfig() : grade(0), client_grade(0), upgrade_stuff_id(0), upgrade_stuff2_id(0), upgrade_stuff_count(0),
		coin(0), active_image_id(0), bless_val_limit(0), is_clear_bless(false), bless_addition(0), shuxingdan_limit(0), chengzhangdan_limit(0),equip_level_limit(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), assist_count(0), is_broadcast(false), pack_num(0), equip_level_toplimit(0), 
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
	int assist_count;

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

// �⻷���׳ɹ���
struct HaloUpGradeProbConfig
{
	HaloUpGradeProbConfig() : low_bless_val(0), high_bless_val(0), succ_rate(0),
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
struct HaloImageConfig								 
{
	HaloImageConfig() : image_id(0)
	{
		memset(image_name, 0, sizeof(image_name));
	}

	int image_id;
	GameName image_name;
};

// ��������
struct HaloSpecialImgCfg
{
	HaloSpecialImgCfg() : image_id(0){}

	bool IsValid() { return 0 != image_id; }

	int image_id;
};

//�����������
struct HaloSpecialImgUpgradeCfg
{
	HaloSpecialImgUpgradeCfg() : image_id(0), grade(0), stuff_id(0), stuff_num(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0),
		shuxingdan_count(0), chengzhangdan_count(0), equip_level(0),
		extra_zengshang(0), extra_mianshang(0),
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

// �⻷װ��
struct HaloEquipConifg
{
	HaloEquipConifg() : max_level(0) {}

	int max_level;

	struct HaloEquipLevelInfo
	{
		HaloEquipLevelInfo(): maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), add_percent(0) {}

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

	HaloEquipLevelInfo level_cfg[HaloParam::EQUIP_MAX_LEVEL];
};

// �⻷����
struct HaloSkillConifg
{
	HaloSkillConifg() : index(0), skill_type(0), skill_level(0), halo_grade(0),
		uplevel_stuff_id(0), end_day_of_up_skill(0), uplevel_stuff_num(0), param_a(0), param_b(0), param_c(0), param_d(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), skill_cap_per(0), base_attr_add_per(0) {}

	int index;
	int skill_type;
	int skill_level;
	int halo_grade;
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

// �⻷���Ǿ���ֵ
struct HaloUpStarExpCfg
{
	HaloUpStarExpCfg() : up_star_level_exp(0), up_star_total_exp(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0){}
	int up_star_level_exp;
	int up_star_total_exp;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
};

// �⻷������Ʒ
struct HaloUpStarStuffCfg
{
	HaloUpStarStuffCfg(): up_star_item_index(0), up_star_item_id(0), star_exp(0), baoji_per(0), baoji_mul(0){}

	short up_star_item_index;
	ItemID up_star_item_id;
	int star_exp;
	int baoji_per;
	int baoji_mul;
};

struct HaloHuanHuaSpecialCapAddCfgItem
{
	HaloHuanHuaSpecialCapAddCfgItem() : huanhua_id(-1), huanhua_level(0), maxhp(0), gongji(0), fangyu(0) {}

	int huanhua_id;
	int huanhua_level;
	int maxhp;
	int gongji;
	int fangyu;
};

class HaloConfig : public ConfigBase
{
public:
	HaloConfig();
	~HaloConfig();

	bool Init(const std::string &configname, std::string *err);

	const HaloLevelConfig * GetLevelConfig(int halo_level) const;

	const HaloGradeConfig * GetGradeConfig(int grade) const;
	const HaloUpGradeProbConfig * GetUpGradeProbConfig(int grade, int bless_val) const;
	const HaloImageConfig * GetHaloImage(int halo_attr_id) const;

	int GetUseJinjieCardRewardItemID() { return m_use_jinjie_card_reward_itemid; }
	int GetChengZhangDanPercent() { return m_chengzhangdan_percent; }
	int GetActiveEquipGrade() { return m_active_equip_grade; }
	int GetActiveEquipSkillLevel() { return m_active_equip_skill_level; }
	int GetEquipSkillLevel(int halo_equip_level);
	short GetMedalSkillIndex() { return m_medal_skill_index; }

	const HaloSpecialImgCfg * GetSpecialImgCfg(int special_img_id);
	const HaloSpecialImgUpgradeCfg * GetSpecialImgUpgradeCfg(int special_img_id, int grade);

	const HaloEquipConifg * GetEquipCfg(int equip_idx);

	const HaloSkillConifg * GetSkillCfg(int skill_idx, int level);
	const HaloUpStarExpCfg * GetUpStarExpCfg(int star_level);
	const HaloUpStarStuffCfg * GetUpStarStuffCfg(int stuff_index);
	const HaloHuanHuaSpecialCapAddCfgItem* GetHuanHuaSpecialCapAddCfg(int huanhua_id)const;

	bool IsAddSpecialCap();

private:
	int InitOther(PugiXmlNode RootElement);
	int InitLevelCfg(PugiXmlNode RootElement);
	int InitHaloGradeCfg(PugiXmlNode RootElement);
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
	short m_active_equip_grade;
	short m_active_equip_skill_level;
	bool m_is_open_special_cap_add;
	short m_medal_skill_index;

	//////////////////////////////////////////////////////////////////////////
	HaloLevelConfig m_level_list[HaloParam::MAX_LEVEL + 1];

	//////////////////////////////////////////////////////////////////////////
	HaloGradeConfig m_grade_cfg_list[HaloParam::MAX_GRADE];

	struct HaloUpGradeProbList
	{
		static const int MAX_PROB_COUNT_PER_GRADE = 600;

		HaloUpGradeProbList() : grade(0), prob_count(0) {}

		bool Add(HaloUpGradeProbConfig &cfg)
		{
			if (prob_count >= MAX_PROB_COUNT_PER_GRADE) return false;

			prob_list[prob_count ++] = cfg;
			return true;
		}

		short grade;
		short prob_count;
		HaloUpGradeProbConfig prob_list[MAX_PROB_COUNT_PER_GRADE];
	};

	std::map<int, HaloUpGradeProbList> m_upgrade_prob_map;

	HaloImageConfig m_image_list[HaloParam::MAX_IMAGE_COUNT];

	////////////////////////////////////////////////////////////////////////////
	HaloSpecialImgCfg m_special_img_cfglist[HaloParam::MAX_SPECIAL_IMAGE_COUNT];

	struct HaloSpecialImgUpgradeCfgList
	{
		HaloSpecialImgUpgradeCfgList() : max_grade(0) {}

		int max_grade;
		HaloSpecialImgUpgradeCfg upgrade_list[HaloParam::MAX_UPGRADE_LIMIT + 1];
	};

	typedef std::map<int, HaloSpecialImgUpgradeCfgList> HaloSpecialImgUpgradeCfgMap;
	HaloSpecialImgUpgradeCfgMap m_special_img_upgrade_map;

	////////////////////////////////////////////////////////////////////////////
	HaloEquipConifg m_equip_cfg_list[HaloParam::EQUIP_COUNT];
	int m_equip_skill_level_cfg[HaloParam::EQUIP_MAX_LEVEL];

	//////////////////////////////////////////////////////////////////////////
	HaloSkillConifg m_skill_cfg_list[HaloParam::SKILL_COUNT][HaloParam::SKILL_MAX_LEVEL + 1];
	HaloUpStarExpCfg m_upstar_exp_list[HaloParam::MAX_STAR_LEVEL];
	HaloUpStarStuffCfg m_upstar_item_list[HaloParam::MAX_UP_STAR_ITEM_COUNT];

	typedef std::map<int, HaloHuanHuaSpecialCapAddCfgItem> HaloHuanHuaSpecialCapAddCfg;
	HaloHuanHuaSpecialCapAddCfg m_huanhua_special_cap_cfg;
};

#endif
