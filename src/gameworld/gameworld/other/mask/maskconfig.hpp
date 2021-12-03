#ifndef MASK_CONFIG_HPP
#define MASK_CONFIG_HPP

#include <map>
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/struct/maskparam.hpp"
#include "servercommon/configbase.h"
#include "item/itembase.h"

#define MASK_CFG() LOGIC_CONFIG->GetMaskConfig()

class TiXmlElement;

struct MaskOtherConfig
{
	MaskOtherConfig() : extra_attrs_per_grade(0), extra_attrs_per(0) {}

	int extra_attrs_per_grade;
	int extra_attrs_per;
};

// ��ߵȼ�
struct MaskLevelConfig
{
	MaskLevelConfig() : mask_level(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0)
	{
	}

	int mask_level;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
};

// ��߽���
struct MaskGradeConfig
{
	MaskGradeConfig() : grade(0), client_grade(0), upgrade_stuff_id(0), upgrade_stuff2_id(0), upgrade_stuff_count(0),
		coin(0), active_image_id(0), bless_val_limit(0), is_clear_bless(false),bless_addition(0), shuxingdan_limit(0), chengzhangdan_limit(0),equip_level_limit(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), movespeed(0), is_broadcast(false), pack_num(0), extra_zengshang(0), extra_mianshang(0),
		per_jingzhun(0), per_baoji(0), per_mianshang(0), per_pofang(0), pvp_zengshang_per(0), pve_zengshang_per(0), pvp_jianshang_per(0), pve_jianshang_per(0)
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

// ��߽��׳ɹ���
struct MaskUpGradeProbConfig
{
	MaskUpGradeProbConfig() : low_bless_val(0), high_bless_val(0), succ_rate(0),
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
struct MaskImageConfig								 
{
	MaskImageConfig() : image_id(0)
	{
		memset(image_name, 0, sizeof(image_name));
	}

	int image_id;
	GameName image_name;
};

// ��������
struct MaskSpecialImgCfg
{
	MaskSpecialImgCfg() : image_id(0){}

	bool IsValid() { return 0 != image_id; }

	int image_id;
};

struct MaskSpecialImgUpgradeCfg
{
	MaskSpecialImgUpgradeCfg() : image_id(0), grade(0), stuff_id(0), stuff_num(0),
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

// ���װ��
struct MaskEquipConifg
{
	MaskEquipConifg() : max_level(0) {}

	int max_level;

	struct MaskEquipLevelInfo
	{
		MaskEquipLevelInfo(): maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), add_percent(0) {}

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

	MaskEquipLevelInfo level_cfg[MaskParam::EQUIP_MAX_LEVEL + 1];
};

// ��߼���
struct MaskSkillConifg
{
	MaskSkillConifg() : index(0), skill_type(0), skill_level(0), mask_grade(0),
		uplevel_stuff_id(0), uplevel_stuff_num(0), param_a(0), param_b(0), param_c(0), param_d(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0) , capability(0), skill_cap_per(0){}

	int index;
	int skill_type;
	int skill_level;
	int mask_grade;
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

struct MaskUpStarExpCfg
{
	MaskUpStarExpCfg(): up_star_level_exp(0), up_start_total_exp(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), movespeed(0) {}

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

struct MaskUpStarStuffCfg
{
	MaskUpStarStuffCfg(): up_star_item_index(0), up_star_item_id(0), star_exp(0), baoji_per(0), baoji_mul(0){}

	short up_star_item_index;
	ItemID up_star_item_id;
	int star_exp;
	int baoji_per;
	int baoji_mul;
};

struct MaskHuanHuaSpecialCapAddCfgItem
{
	MaskHuanHuaSpecialCapAddCfgItem() : huanhua_id(-1), huanhua_level(0), maxhp(0), gongji(0), fangyu(0) {}

	int huanhua_id;
	int huanhua_level;
	int maxhp;
	int gongji;
	int fangyu;
};

class MaskConfig : public ConfigBase
{
public:
	MaskConfig();
	~MaskConfig();

	bool Init(const std::string &configname, std::string *err);

	const MaskOtherConfig & GetOtherCfg() { return m_other_cfg; }

	const MaskLevelConfig * GetLevelConfig(int mask_level);
	const MaskGradeConfig * GetGradeConfig(int grade);
	const MaskUpGradeProbConfig * GetUpGradeProbConfig(int grade, int bless_val);
	const MaskImageConfig * GetMaskImage(int mask_image_id);

	int GetUseJinjieCardRewardItemID() { return m_use_jinjie_card_reward_itemid; }
	int GetChengZhangDanPercent() { return m_chengzhangdan_percent; }
	int GetTemporaryImageValidTime(){return m_temporary_image_time;}
	int GetActiveEquipGrade() {return m_active_equip_grade;}
	int GetActiveEquipSkillLevel() {return m_active_equip_skill_level;}
	int GetEquipSkillLevel(int min_equip_level);

	const MaskSpecialImgCfg * GetSpecialImgCfg(int special_img_id);
	const MaskSpecialImgUpgradeCfg * GetSpecialImgUpgradeCfg(int special_img_id, int grade);
	const MaskEquipConifg * GetEquipCfg(int equip_idx);
	const MaskSkillConifg * GetSkillCfg(int skill_idx, int level);
	const MaskUpStarExpCfg * GetUpStarExpCfg(int star_level);
	int GetMaxStarLevel() { return m_max_star_level; }

	const MaskUpStarStuffCfg * GetUpStarStuffCfg(int stuff_index);
	const MaskHuanHuaSpecialCapAddCfgItem* GetHuanHuaSpecialCapAddCfg(int huanhua_id)const;
	bool IsAddSpecialCap();


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


	MaskOtherConfig m_other_cfg;

	int m_use_jinjie_card_reward_itemid;
	int m_chengzhangdan_percent;
	int m_temporary_image_time;
	short m_active_equip_grade;
	short m_active_equip_skill_level;
	bool m_is_open_special_cap_add;
	int m_max_star_level;

	//////////////////////////////////////////////////////////////////////////
	MaskLevelConfig m_level_list[MaskParam::MAX_LEVEL + 1];

	//////////////////////////////////////////////////////////////////////////
	MaskGradeConfig m_grade_cfg_list[MaskParam::MAX_GRADE];

	struct MaskUpGradeProbList
	{
		static const int MAX_PROB_COUNT_PER_GRADE = 600;

		MaskUpGradeProbList() : grade(0), prob_count(0) {}

		bool Add(MaskUpGradeProbConfig &cfg)
		{
			if (prob_count >= MAX_PROB_COUNT_PER_GRADE) return false;
			
			prob_list[prob_count ++] = cfg;
			return true;
		}

		short grade;
		short prob_count;
		MaskUpGradeProbConfig prob_list[MAX_PROB_COUNT_PER_GRADE];
	};

	std::map<int, MaskUpGradeProbList> m_upgrade_prob_map;

	MaskImageConfig m_image_list[MaskParam::MAX_IMAGE_COUNT];

	////////////////////////////////////////////////////////////////////////////
	MaskSpecialImgCfg m_special_img_cfglist[MaskParam::MAX_SPECIAL_IMAGE_COUNT];

	struct MaskSpecialImgUpgradeCfgList
	{
		MaskSpecialImgUpgradeCfgList() : max_grade(0) {}

		int max_grade;
		MaskSpecialImgUpgradeCfg upgrade_list[MaskParam::MAX_UPGRADE_LIMIT + 1];
	};

	typedef std::map<int, MaskSpecialImgUpgradeCfgList> MaskSpecialImgUpgradeCfgMap;
	MaskSpecialImgUpgradeCfgMap m_special_img_upgrade_map;

	////////////////////////////////////////////////////////////////////////////
	MaskEquipConifg m_old_equip_cfg_list[MaskParam::EQUIP_COUNT];
	MaskEquipConifg m_equip_cfg_list[MaskParam::EQUIP_COUNT];
	int m_equip_skill_level_cfg[MaskParam::EQUIP_MAX_LEVEL + 1];

	//////////////////////////////////////////////////////////////////////////
	MaskSkillConifg m_skill_cfg_list[MaskParam::SKILL_COUNT][MaskParam::SKILL_MAX_LEVEL + 1];

	MaskUpStarExpCfg m_upstar_exp_list[MaskParam::MAX_STAR_LEVEL];
	MaskUpStarStuffCfg m_upstar_item_list[MaskParam::MAX_UP_STAR_ITEM_COUNT];
	typedef std::map<int, MaskHuanHuaSpecialCapAddCfgItem> MaskHuanHuaSpecialCapAddCfg;
	MaskHuanHuaSpecialCapAddCfg m_huanhua_special_cap_cfg;

};

#endif

