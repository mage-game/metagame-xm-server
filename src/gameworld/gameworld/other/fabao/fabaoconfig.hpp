#ifndef FABAO_CONFIG_HPP
#define FABAO_CONFIG_HPP

#include <map>
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/struct/fabaoparam.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "item/itembase.h"


static const int FABAO_SPECIAL_IMG_MAX_GRADE = 100;		// ���������������������

struct FabaoOtherCfg
{
	FabaoOtherCfg() : m_reward_itemid(0) {}

	int m_reward_itemid;
};

//������������
struct FabaoGradeConfig
{
	FabaoGradeConfig() : grade(0), client_grade(0), upgrade_stuff_id(0), upgrade_stuff2_id(0), upgrade_stuff_count(0),
		coin(0), active_image_id(0), bless_val_limit(0), is_clear_bless(false), bless_addition(0), shuxingdan_limit(0), chengzhangdan_limit(0), equip_level_limit(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), movespeed(0), is_broadcast(false), pack_num(0), equip_level_toplimit(0),
		extra_zengshang(0), extra_mianshang(0),	per_jingzhun(0), per_baoji(0), per_mianshang(0), per_pofang(0), pvp_zengshang_per(0), pve_zengshang_per(0), 
		pvp_jianshang_per(0), pve_jianshang_per(0)
	{
	}

	short grade;                                   // ����
	short client_grade;                            // �ͻ��˽���
	ItemID upgrade_stuff_id;                       // ���ײ���id
	ItemID upgrade_stuff2_id;					   // ���ײ���2id
	short upgrade_stuff_count;                     // ���ײ�������
	int coin;                                      // ͭǮ
	short active_image_id;                         // ��������ID
	int bless_val_limit;                           // ף��ֵ����
	int is_clear_bless;                            // �Ƿ����ף��ֵ
	int bless_addition;                            // ף��ֵ�������ӳ���ֱ�
	int shuxingdan_limit;                          // ���Ե�����
	int chengzhangdan_limit;                       // �ɳ�������
	int equip_level_limit;                         // װ���ȼ�����

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	Attribute movespeed;

	bool is_broadcast;                             // �Ƿ���

	int pack_num;
	int equip_level_toplimit;					   //װ���ȼ�����

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

//�������׳ɹ�������
struct FabaoUpGradeProbConfig
{
	FabaoUpGradeProbConfig() : low_bless_val(0), high_bless_val(0), succ_rate(0),
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

	int low_bless_val;                      // ף��ֵ����
	int high_bless_val;                     // ף��ֵ����
	int succ_rate;                          // �ɹ���
	int fail_add_1_bless_rate;              // ʧ�ܼ�1ף��ֵ����
	int fail_add_2_bless_rate;              // ʧ�ܼ�2ף��ֵ����
	int fail_add_3_bless_rate;              // ʧ�ܼ�3ף��ֵ����
};

struct FabaoUpGradeProbConfigList
{
	static const int MAX_PROB_COUNT_PER_GRADE = 600;

	FabaoUpGradeProbConfigList() : grade(0), prob_count(0)
	{
	}

	bool Add(FabaoUpGradeProbConfig& cfg)
	{
		if (prob_count >= MAX_PROB_COUNT_PER_GRADE)
		{
			return false;
		}

		prob_list[prob_count++] = cfg;
		return true;
	}

	short grade;
	short prob_count;
	FabaoUpGradeProbConfig prob_list[MAX_PROB_COUNT_PER_GRADE];
};

// ������������
struct FabaoImageConfig
{
	FabaoImageConfig() : image_id(0)
	{
	}

	int image_id; //����id
};

//����������������
struct FabaoSpecialImgCfg
{
	FabaoSpecialImgCfg() : special_img_id(0), valid_time_s(0) {}

	bool IsValid() { return 0 != special_img_id; }

	int special_img_id;
	int valid_time_s;				// ��Чʱ�䣨�Ӽ���ʱ����
};

//�������������������
struct FabaoSpecialImgUpGradeCfg
{
	FabaoSpecialImgUpGradeCfg():special_img_id(0), grade(0), stuff_id(0), stuff_num(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), extra_zengshang(0), extra_mianshang(0),
		per_jingzhun(0), per_baoji(0), per_mianshang(0), per_pofang(0), pvp_zengshang_per(0), pve_zengshang_per(0), pvp_jianshang_per(0), pve_jianshang_per(0), 
		shuxingdan_count(0), chengzhangdan_count(0), equip_level(0) {}

	int special_img_id;                                //��������id
	int grade;                                         //�׼�
	int stuff_id;                                      //���ײ���id
	int stuff_num;                                     //���ײ�������
	Attribute maxhp;                                         
	Attribute gongji;                                  
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
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
	int shuxingdan_count;                              //���Ե���������
	int chengzhangdan_count;                           //�ɳ�����������
	int equip_level;                                   //װ���ȼ�����
};

struct FabaoSpecialImgUpGradeCfgList
{
	FabaoSpecialImgUpGradeCfgList() : max_grade(0){}

	int max_grade;
	FabaoSpecialImgUpGradeCfg upgrade_list[FABAO_SPECIAL_IMG_MAX_GRADE + 1];
};

// װ��
struct FabaoEquipConifg
{
	FabaoEquipConifg() : max_level(0) {}

	short max_level;

	struct FabaoEquipLevelInfo
	{
		FabaoEquipLevelInfo() : maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), add_percent(0) {}

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

	FabaoEquipLevelInfo level_cfg[FabaoParam::EQUIP_MAX_LEVEL];
};

// ����
struct FabaoSkillConifg
{
	FabaoSkillConifg() : index(0), skill_type(0), skill_level(0), fabao_grade(0),
		uplevel_stuff_id(0), end_day_of_up_skill(0), uplevel_stuff_num(0), param_a(0), param_b(0), param_c(0), param_d(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), capability(0), skill_cap_per(0), base_attr_add_per(0) {}

	int index;
	int skill_type;
	int skill_level;
	int fabao_grade;
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

struct FabaoHuanHuaSpecialCapAddCfgItem
{
	FabaoHuanHuaSpecialCapAddCfgItem() : huanhua_id(-1), huanhua_level(0), maxhp(0), gongji(0), fangyu(0) {}

	int huanhua_id;
	int huanhua_level;
	int maxhp;
	int gongji;
	int fangyu;
};

class FabaoConfig : public ConfigBase
{
public:
	FabaoConfig();
	~FabaoConfig();

	bool Init(const std::string &configname, std::string *err);

	const FabaoOtherCfg & GetOtherCfg() const { return m_other_cfg; }
	const FabaoGradeConfig* GetGradeConfig(int grade) const;
	const FabaoUpGradeProbConfig* GetUpGradeProbConfig(int grade,int bless_val) const;
	const FabaoImageConfig* GetImageConfig(int image_index_id) const;
	const FabaoSpecialImgCfg* GetSpecialImgCfg(int special_img_id);
	const FabaoSpecialImgUpGradeCfg* GetSpecialImgUpGradeCfg(int special_img_id,int grade);
	const FabaoSkillConifg* GetSkillCfg(int skill_idx, int level);
	const FabaoEquipConifg * GetEquipCfg(int equip_idx);
	int GetActiveEquipGrade() { return m_active_equip_grade; }
	int GetActiveEquipSkillLevel() { return m_active_equip_skill_level; }
	int GetEquipSkillLevel(int min_equip_level);
	short GetMedalSkillIndex() { return m_medal_skill_index; }

	const FabaoHuanHuaSpecialCapAddCfgItem* GetHuanHuaSpecialCapAddCfg(int huanhua_id)const;
	bool IsAddSpecialCap();

private:
	int InitOther(PugiXmlNode RootElement);
	int InitFabaoGradeCfg(PugiXmlNode RootElement);
	int InitFabaoUpGradeProbCfg(PugiXmlNode RootElement);
	int InitFabaoImageCfg(PugiXmlNode RootElement);
	int InitFabaoSpecialImageCfg(PugiXmlNode RootElement);
	int InitFabaoSpecialImageUpgradeCfg(PugiXmlNode RootElement);
	int InitFabaoSkillCfg(PugiXmlNode RootElement);
	int InitEquipCfg(PugiXmlNode RootElement);
	int InitEquipSkillLevel(PugiXmlNode RootElement);
	int InitHuanHuaSpecialCapAddCfg(PugiXmlNode RootElement);

	FabaoOtherCfg m_other_cfg;                                                                     // ��������
	short m_active_equip_grade;                                                                    // ����װ������׼�
	short m_active_equip_skill_level;                                                              // ����װ��������Ҫװ���ﵽ�ĵȼ�
	bool m_is_open_special_cap_add;
	short m_medal_skill_index;

	typedef std::map<int, FabaoSpecialImgUpGradeCfgList> FabaoSpecialImgUpgradeCfgMap;              
	FabaoSpecialImgUpgradeCfgMap m_special_img_upgrade_map;                                        // keyΪ��������id��valueΪ��idʱ������׶εȼ��б�
	FabaoGradeConfig m_fabao_grade_cfg_list[FabaoParam::MAX_GRADE];                                // �����ȼ������б��±�Ϊ�ȼ���Ԫ��Ϊ��Ӧ�ȼ���������
	FabaoImageConfig m_image_config_list[FabaoParam::MAX_IMAGE_COUNT];                                // ���������б��±�Ϊid��Ԫ��Ϊ��Ӧid����������
	FabaoSpecialImgCfg m_special_img_list[FabaoParam::MAX_SPECIAL_IMAGE_COUNT];                       // �������������б��±�Ϊid��Ԫ��Ϊ��Ӧid����������

	std::map<int, FabaoUpGradeProbConfigList> m_upgrade_prob_map;                                  // keyΪ�׼��ȼ���valueΪ�õȼ�ʱ�Ľ��׳ɹ����������� 
	FabaoSkillConifg m_skill_cfg_list[FabaoParam::SKILL_COUNT][FabaoParam::SKILL_MAX_LEVEL + 1];   // ���ܵȼ��б���һά�±��ǵȼ��������ڶ�ά�±��ǵȼ���Ԫ���Ƕ�Ӧ�����͵ȼ��ĵȼ�����
	FabaoEquipConifg m_equip_cfg_list[FabaoParam::EQUIP_COUNT];                                    // װ���б��±�Ϊװ��������ֵΪ��Ӧ����װ����������
	int m_equip_skill_level_cfg[FabaoParam::EQUIP_MAX_LEVEL];

	typedef std::map<int, FabaoHuanHuaSpecialCapAddCfgItem> FabaoHuanHuaSpecialCapAddCfg;
	FabaoHuanHuaSpecialCapAddCfg m_huanhua_special_cap_cfg;
};
#endif
