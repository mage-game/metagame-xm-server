#ifndef JINGLINGGUANGHUANCONFIG_HPP
#define JINGLINGGUANGHUANCONFIG_HPP

#include "gamedef.h"
#include "servercommon/basedef.h"
#include <map>

#include "servercommon/struct/itemlistparam.h"

#include "item/itembase.h"

#include "obj/character/attribute.hpp"
#include "servercommon/jinglingguanghuandef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

// ����⻷����
struct JinglingGuanghuanGradeConfig
{
	JinglingGuanghuanGradeConfig() : grade(0), client_grade(0), upgrade_stuff_id(0), upgrade_stuff_count(0),
		coin(0), active_image_id(0), bless_val_limit(0), is_clear_bless(false),maxhp(0), gongji(0),fangyu(0),
		mingzhong(0), shanbi(0), baoji(0), jianren(0), movespeed(0), is_broadcast(false)
	{
	}

	short grade;											// ����
	short client_grade;										// �ͻ��˽���
	ItemID upgrade_stuff_id;								// ���ײ���id
	short upgrade_stuff_count;								// ���ײ�������
	int coin;												// ͭǮ
	short active_image_id;									// ��������ID
	short bless_val_limit;									// ף��ֵ����
	int is_clear_bless;										// �Ƿ����ף��ֵ

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	Attribute movespeed;

	bool is_broadcast;										// �Ƿ���
};

// ����⻷���׳ɹ���
struct JinglingGuanghuanUpGradeProbConfig
{
	JinglingGuanghuanUpGradeProbConfig() : low_bless_val(0), high_bless_val(0), succ_rate(0),
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
struct JinglingGuanghuanImageConfig								 
{
	JinglingGuanghuanImageConfig() : image_id(0)
	{
		memset(image_name, 0, sizeof(image_name));
	}

	int image_id;
	GameName image_name;
};

// ��������
struct JinglingGuanghuanSpecialImgCfg
{
	JinglingGuanghuanSpecialImgCfg() : image_id(0){}

	bool IsValid() { return 0 != image_id; }

	int image_id;
};

// �����������
struct JinglingGuanghuanSpecialImgUpgradeCfg
{
	JinglingGuanghuanSpecialImgUpgradeCfg() : image_id(0), grade(0), stuff_id(0), stuff_num(0),
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

class JinglingGuanghuanConfig : public ConfigBase
{
public:
	JinglingGuanghuanConfig();
	~JinglingGuanghuanConfig();

	bool Init(const std::string &configname, std::string *err);
	const JinglingGuanghuanGradeConfig * GetGradeConfig(int grade) const;
	const JinglingGuanghuanUpGradeProbConfig * GetUpGradeProbConfig(int grade, int bless_val) const;
	const JinglingGuanghuanImageConfig * GetImageCfg(int attr_id) const;

	const JinglingGuanghuanSpecialImgCfg * GetSpecialImgCfg(int special_img_id);
	const JinglingGuanghuanSpecialImgUpgradeCfg * GetSpecialImgUpgradeCfg(int special_img_id, int grade);

private:
	int InitOther(PugiXmlNode RootElement);
	int InitGradeCfg(PugiXmlNode RootElement);
	int InitUpGradeProbCfg(PugiXmlNode RootElement);
	int InitImageCfg(PugiXmlNode RootElement);
	int InitUpStarExpCfg(PugiXmlNode RootElement);
	int InitUpStarStuffCfg(PugiXmlNode RootElement);
	int InitSpecialImageCfg(PugiXmlNode RootElement);
	int InitSpecialImageUpgradeCfg(PugiXmlNode RootElement);

	JinglingGuanghuanGradeConfig m_grade_cfg_list[JinglingGuanghuanParam::MAX_GRADE];

	struct UpGradeProbList
	{
		static const int MAX_PROB_COUNT_PER_GRADE = 600;

		UpGradeProbList() : grade(0), prob_count(0) {}

		bool Add(JinglingGuanghuanUpGradeProbConfig &cfg)
		{
			if (prob_count >= MAX_PROB_COUNT_PER_GRADE) return false;

			prob_list[prob_count ++] = cfg;
			return true;
		}

		short grade;
		short prob_count;
		JinglingGuanghuanUpGradeProbConfig prob_list[MAX_PROB_COUNT_PER_GRADE];
	};

	std::map<int, UpGradeProbList> m_upgrade_prob_map;

	JinglingGuanghuanImageConfig m_image_list[JinglingGuanghuanParam::MAX_IMAGE_ID + 1];

	////////////////////////////////////////////////////////////////////////////
	JinglingGuanghuanSpecialImgCfg m_special_img_cfglist[JinglingGuanghuanParam::MAX_SPECIAL_IMAGE_ID + 1];

	struct JinglingGuanghuanSpecialImgUpgradeCfgList
	{
		JinglingGuanghuanSpecialImgUpgradeCfgList() : max_grade(0) {}

		int max_grade;
		JinglingGuanghuanSpecialImgUpgradeCfg upgrade_list[JinglingGuanghuanParam::MAX_UPGRADE_LIMIT + 1];
	};

	typedef std::map<int, JinglingGuanghuanSpecialImgUpgradeCfgList> JinglingGuanghuanSpecialImgUpgradeCfgMap;
	JinglingGuanghuanSpecialImgUpgradeCfgMap m_special_img_upgrade_map;
};

#endif
