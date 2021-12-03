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

//	UPGRADE_MODULE_TYPE_LEVEL,		// �ȼ�ģ��
	UPGRADE_MODULE_TYPE_UPGRADE,	// ����ģ��
	UPGRADE_MODULE_TYPE_IMAGE,		// ����ģ�� 
	UPGRADE_MODULE_TYPE_EQUIP,		// װ��ģ��
	UPGRADE_MODULE_TYPE_SKILL,		// ����ģ��
	UPGRADE_MODULE_TYPE_BIG_GOAL,	// ��Ŀ��ģ��

	UPGRADE_MODULE_TYPE_MAX,
};

//�������---------------------------------------------------------------
struct UpgradeOtherConfig
{
	UpgradeOtherConfig() : extra_attrs_per_grade(0), extra_attrs_per(0) {}

	int extra_attrs_per_grade;
	int extra_attrs_per;
};

//�������---------------------------------------------------------------
// �ȼ�
struct LevelConfig
{
	LevelConfig() : level(0)
	{
	}

	int level;
	AttrConfig attr_cfg;
};

// ����
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

	short grade;											// ����
	short client_grade;										// �ͻ��˽���
	ItemID upgrade_stuff_id;								// ���ײ���id
	ItemID upgrade_stuff2_id;								// ���ײ���2id
	short upgrade_stuff_count;								// ���ײ�������
	short active_image_id;									// ��������ID
	int bless_val_limit;									// ף��ֵ����
	int is_clear_bless;										// �Ƿ����ף��ֵ
	int bless_addition;										// ף��ֵ�������ӳ���ֱ�
	int shuxingdan_limit;									// ���Ե�����
	int chengzhangdan_limit;								// �ɳ�������
	int equip_level_limit;									// װ���ȼ�����

	AttrConfig attr_cfg;
	int movespeed;
	int extra_zengshang;									//�̶�����
	int extra_mianshang;									//�̶�����
	int per_jingzhun;										//��׼���Ƽ��ʣ�
	int per_baoji;											//�����������˺��ʣ�
	int jianshang_per;										//������ֱ�
	int zengshang_per;										//������ֱ�
	int pvp_zengshang_per;									//pvp������ֱ�
	int pve_zengshang_per;									//pve������ֱ�
	int pvp_jianshang_per;									//pvp������ֱ�
	int pve_jianshang_per;									//pve������ֱ�

	bool is_broadcast;										// �Ƿ���
	int equip_level_toplimit;								//װ���ȼ�����
};

// ���׳ɹ���
struct UpGradeProbConfig
{
	UpGradeProbConfig() : low_bless_val(0), high_bless_val(0), succ_rate(0)
	{
	}

	int low_bless_val;										// ף��ֵ����
	int high_bless_val;										// ף��ֵ����
	int succ_rate;											// �ɹ���

	LuckyConfig upgrade_lucky_cfg;							// ʧ�ܼ�ף��ֵ����
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

//�������---------------------------------------------------------------

// �����б�����
struct ImageConfig								 
{
	ImageConfig() : image_id(0)
	{
		
	}

	bool IsValid() { return 0 != image_id; }

	int image_id;
};

// �������
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
	Attribute extra_zengshang;									//�̶�����
	Attribute extra_mianshang;									//�̶�����
	Attribute per_jingzhun;										//��׼���Ƽ��ʣ�
	Attribute per_baoji;											//�����������˺��ʣ�
	Attribute jianshang_per;										//������ֱ�
	Attribute zengshang_per;										//������ֱ�
	Attribute pvp_zengshang_per;									//pvp������ֱ�
	Attribute pve_zengshang_per;									//pve������ֱ�
	Attribute pvp_jianshang_per;									//pvp������ֱ�
	Attribute pve_jianshang_per;									//pve������ֱ�
};

struct ImgUpgradeCfgList
{
	ImgUpgradeCfgList() : max_grade(0) {}

	int max_grade;
	ImgUpgradeCfg upgrade_list[UPGRADE_MAX_IMAGE_GRADE + 1];
};

//װ�����---------------------------------------------------------------
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

//�������---------------------------------------------------------------

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

//��ǿս���ӳ�����
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

	//�������---------------------------------------------------------------
	const UpgradeOtherConfig & GetOtherCfg() { return m_other_cfg; }

	int GetUpOneGradeRewardItem() { return m_up_one_grade_reward_id; }
	int GetTemporaryImageValidTime() { return m_temporary_image_time; }
	int GetActiveEquipGrade() { return m_active_equip_grade; }
	int GetActiveEquipSkillLevel() { return m_active_equip_skill_level; }
	short GetMedalSkillIndex() { return m_medal_skill_index; }

	//�ȼ����---------------------------------------------------------------
	const LevelConfig * GetLevelConfig(int level) const;

	//�������---------------------------------------------------------------
	const GradeConfig * GetGradeConfig(int grade) const;
	const UpGradeProbConfig * GetUpGradeProbConfig(int grade, int bless_val) const;

	//�������---------------------------------------------------------------
	const ImageConfig * GetImageCfg(int image_id) const;
	const ImgUpgradeCfg * GetImgUpgradeCfg(int special_img_id, int grade);

	//װ�����---------------------------------------------------------------
	const EquipConifg * GetEquipCfg(int equip_idx);

	//�������---------------------------------------------------------------
	const SkillConifg * GetSkillCfg(int skill_idx, int level);

	//��Ŀ�����
	const SuperSpecialCapAddCfgItem* GetSuperSpecialCapAddCfg(int image_id) const;
	bool IsAddSpecialCap();

protected:
	UpgradeconfigManager *m_upgradeconfig_manager;

	int m_upgrade_type;

	//�������---------------------------------------------------------------
	int InitOtherCfg(PugiXmlNode RootElement);

	UpgradeOtherConfig m_other_cfg;
	int m_up_one_grade_reward_id;
	int m_temporary_image_time;
	short m_active_equip_grade;
	short m_active_equip_skill_level;
	bool m_is_open_special_cap_add;
	short m_medal_skill_index;

	//�������---------------------------------------------------------------
	int InitLevelCfg(PugiXmlNode RootElement);
	int InitGradeCfg(PugiXmlNode RootElement);
	int InitUpGradeProbCfg(PugiXmlNode RootElement);

	LevelConfig m_level_list[UPGRADE_MAX_LEVEL + 1];
	GradeConfig m_grade_cfg_list[UPGRADE_MAX_GRADE + 1];

	std::map<int, UpGradeProbList> m_upgrade_prob_map;

	//�������---------------------------------------------------------------
	int InitImageCfg(PugiXmlNode RootElement);
	int InitImageUpgradeCfg(PugiXmlNode RootElement);

	ImageConfig m_image_list[UPGRADE_IMAGE_MAX_COUNT];

	typedef std::map<int, ImgUpgradeCfgList> ImgUpgradeCfgMap;
	ImgUpgradeCfgMap m_special_img_upgrade_map;

	//װ�����---------------------------------------------------------------
	int InitEquipCfg(PugiXmlNode RootElement);

	EquipConifg m_equip_cfg_list[UPGRADE_EQUIP_COUNT];
	int m_equip_skill_level_cfg[UPGRADE_EQUIP_MAX_LEVEL + 1];

	//�������---------------------------------------------------------------
	int InitSkillCfg(PugiXmlNode RootElement);

	SkillConifg m_skill_cfg_list[UPGRADE_SKILL_COUNT][UPGRADE_SKILL_MAX_LEVEL + 1];

	//��Ŀ�����
	int InitSuperSpecialCapAddCfg(PugiXmlNode RootElement);
	typedef std::map<int, SuperSpecialCapAddCfgItem> SuperSpecialCapAddCfg;
	SuperSpecialCapAddCfg m_super_special_cap_cfg;
};

#endif

