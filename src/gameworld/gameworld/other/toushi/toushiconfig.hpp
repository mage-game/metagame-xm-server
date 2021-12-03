#ifndef TOUSHI_CONFIG_HPP
#define TOUSHI_CONFIG_HPP

#include <map>
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/struct/toushiparam.hpp"
#include "servercommon/configbase.h"
#include "item/itembase.h"

#define TOUSHI_CFG() LOGIC_CONFIG->GetTouShiConfig()

class TiXmlElement;

struct TouShiOtherConfig
{
	TouShiOtherConfig() : extra_attrs_per_grade(0), extra_attrs_per(0) {}
	
	int extra_attrs_per_grade;
	int extra_attrs_per;
};

// ͷ�εȼ�
struct TouShiLevelConfig
{
	TouShiLevelConfig() : toushi_level(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0)
	{
	}

	int toushi_level;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
};

// ͷ�ν���
struct TouShiGradeConfig
{
	TouShiGradeConfig() : grade(0), client_grade(0), upgrade_stuff_id(0), upgrade_stuff2_id(0), upgrade_stuff_count(0),
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

// ͷ�ν��׳ɹ���
struct TouShiUpGradeProbConfig
{
	TouShiUpGradeProbConfig() : low_bless_val(0), high_bless_val(0), succ_rate(0),
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
struct TouShiImageConfig								 
{
	TouShiImageConfig() : image_id(0)
	{
		memset(image_name, 0, sizeof(image_name));
	}

	int image_id;
	GameName image_name;
};

// ��������
struct TouShiSpecialImgCfg
{
	TouShiSpecialImgCfg() : image_id(0){}

	bool IsValid() { return 0 != image_id; }

	int image_id;
};

struct TouShiSpecialImgUpgradeCfg
{
	TouShiSpecialImgUpgradeCfg() : image_id(0), grade(0), stuff_id(0), stuff_num(0),
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

// ͷ��װ��
struct TouShiEquipConifg
{
	TouShiEquipConifg() : max_level(0) {}

	int max_level;

	struct TouShiEquipLevelInfo
	{
		TouShiEquipLevelInfo(): maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), add_percent(0) {}

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

	TouShiEquipLevelInfo level_cfg[TouShiParam::EQUIP_MAX_LEVEL + 1];
};

// ͷ�μ���
struct TouShiSkillConifg
{
	TouShiSkillConifg() : index(0), skill_type(0), skill_level(0), toushi_grade(0),
		uplevel_stuff_id(0), uplevel_stuff_num(0), param_a(0), param_b(0), param_c(0), param_d(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0) , capability(0), skill_cap_per(0){}

	int index;
	int skill_type;
	int skill_level;
	int toushi_grade;
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

struct TouShiUpStarExpCfg
{
	TouShiUpStarExpCfg(): up_star_level_exp(0), up_start_total_exp(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), movespeed(0) {}

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

struct TouShiUpStarStuffCfg
{
	TouShiUpStarStuffCfg(): up_star_item_index(0), up_star_item_id(0), star_exp(0), baoji_per(0), baoji_mul(0){}

	short up_star_item_index;
	ItemID up_star_item_id;
	int star_exp;
	int baoji_per;
	int baoji_mul;
};

struct TouShiHuanHuaSpecialCapAddCfgItem
{
	TouShiHuanHuaSpecialCapAddCfgItem() : huanhua_id(-1), huanhua_level(0), maxhp(0), gongji(0), fangyu(0) {}

	int huanhua_id;
	int huanhua_level;
	int maxhp;
	int gongji;
	int fangyu;
};

class TouShiConfig : public ConfigBase
{
public:
	TouShiConfig();
	~TouShiConfig();

	bool Init(const std::string &configname, std::string *err);

	const TouShiOtherConfig & GetOtherCfg() { return m_other_cfg; }

	const TouShiLevelConfig * GetLevelConfig(int toushi_level);
	const TouShiGradeConfig * GetGradeConfig(int grade);
	const TouShiUpGradeProbConfig * GetUpGradeProbConfig(int grade, int bless_val);
	const TouShiImageConfig * GetTouShiImage(int toushi_image_id);

	int GetUseJinjieCardRewardItemID() { return m_use_jinjie_card_reward_itemid; }
	int GetChengZhangDanPercent() { return m_chengzhangdan_percent; }
	int GetTemporaryImageValidTime(){return m_temporary_image_time;}
	int GetActiveEquipGrade() {return m_active_equip_grade;}
	int GetActiveEquipSkillLevel() {return m_active_equip_skill_level;}
	int GetEquipSkillLevel(int min_equip_level);

	const TouShiSpecialImgCfg * GetSpecialImgCfg(int special_img_id);
	const TouShiSpecialImgUpgradeCfg * GetSpecialImgUpgradeCfg(int special_img_id, int grade);
	const TouShiEquipConifg * GetEquipCfg(int equip_idx);
	const TouShiSkillConifg * GetSkillCfg(int skill_idx, int level);
	const TouShiUpStarExpCfg * GetUpStarExpCfg(int star_level);
	int GetMaxStarLevel() { return m_max_star_level; }

	const TouShiUpStarStuffCfg * GetUpStarStuffCfg(int stuff_index);
	const TouShiHuanHuaSpecialCapAddCfgItem* GetHuanHuaSpecialCapAddCfg(int huanhua_id)const;
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

	TouShiOtherConfig m_other_cfg;

	int m_use_jinjie_card_reward_itemid;
	int m_chengzhangdan_percent;
	int m_temporary_image_time;
	short m_active_equip_grade;
	short m_active_equip_skill_level;
	bool m_is_open_special_cap_add;

	//////////////////////////////////////////////////////////////////////////
	TouShiLevelConfig m_level_list[TouShiParam::MAX_LEVEL + 1];

	//////////////////////////////////////////////////////////////////////////
	TouShiGradeConfig m_grade_cfg_list[TouShiParam::MAX_GRADE];

	struct TouShiUpGradeProbList
	{
		static const int MAX_PROB_COUNT_PER_GRADE = 600;

		TouShiUpGradeProbList() : grade(0), prob_count(0) {}

		bool Add(TouShiUpGradeProbConfig &cfg)
		{
			if (prob_count >= MAX_PROB_COUNT_PER_GRADE) return false;
			
			prob_list[prob_count ++] = cfg;
			return true;
		}

		short grade;
		short prob_count;
		TouShiUpGradeProbConfig prob_list[MAX_PROB_COUNT_PER_GRADE];
	};

	std::map<int, TouShiUpGradeProbList> m_upgrade_prob_map;

	TouShiImageConfig m_image_list[TouShiParam::MAX_IMAGE_COUNT];

	////////////////////////////////////////////////////////////////////////////
	TouShiSpecialImgCfg m_special_img_cfglist[TouShiParam::MAX_SPECIAL_IMAGE_COUNT];

	struct TouShiSpecialImgUpgradeCfgList
	{
		TouShiSpecialImgUpgradeCfgList() : max_grade(0) {}

		int max_grade;
		TouShiSpecialImgUpgradeCfg upgrade_list[TouShiParam::MAX_UPGRADE_LIMIT + 1];
	};

	typedef std::map<int, TouShiSpecialImgUpgradeCfgList> TouShiSpecialImgUpgradeCfgMap;
	TouShiSpecialImgUpgradeCfgMap m_special_img_upgrade_map;

	////////////////////////////////////////////////////////////////////////////
	TouShiEquipConifg m_old_equip_cfg_list[TouShiParam::EQUIP_COUNT];
	TouShiEquipConifg m_equip_cfg_list[TouShiParam::EQUIP_COUNT];
	int m_equip_skill_level_cfg[TouShiParam::EQUIP_MAX_LEVEL + 1];

	//////////////////////////////////////////////////////////////////////////
	TouShiSkillConifg m_skill_cfg_list[TouShiParam::SKILL_COUNT][TouShiParam::SKILL_MAX_LEVEL + 1];

	int m_max_star_level;
	TouShiUpStarExpCfg m_upstar_exp_list[TouShiParam::MAX_STAR_LEVEL];
	TouShiUpStarStuffCfg m_upstar_item_list[TouShiParam::MAX_UP_STAR_ITEM_COUNT];
	typedef std::map<int, TouShiHuanHuaSpecialCapAddCfgItem> TouShiHuanHuaSpecialCapAddCfg;
	TouShiHuanHuaSpecialCapAddCfg m_huanhua_special_cap_cfg;
};

#endif

