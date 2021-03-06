#ifndef JINGLINGFAZHENCONFIG_HPP
#define JINGLINGFAZHENCONFIG_HPP

#include "gamedef.h"
#include "servercommon/basedef.h"
#include <map>

#include "servercommon/struct/itemlistparam.h"

#include "item/itembase.h"

#include "obj/character/attribute.hpp"
#include "servercommon/jinglingfazhendef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

// 精灵法阵进阶
struct JinglingFazhenGradeConfig
{
	JinglingFazhenGradeConfig() : grade(0), client_grade(0), upgrade_stuff_id(0), upgrade_stuff_count(0),
		coin(0), active_image_id(0), bless_val_limit(0), is_clear_bless(false),maxhp(0), gongji(0), fangyu(0), 
		mingzhong(0), shanbi(0), baoji(0), jianren(0), movespeed(0), is_broadcast(false)
	{
	}

	short grade;											// 阶数
	short client_grade;										// 客户端阶数
	ItemID upgrade_stuff_id;								// 进阶材料id
	short upgrade_stuff_count;								// 进阶材料数量
	int coin;												// 铜钱
	short active_image_id;									// 激活形象ID
	short bless_val_limit;									// 祝福值上限
	int is_clear_bless;										// 是否清空祝福值

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	Attribute movespeed;

	bool is_broadcast;										// 是否传闻
};

// 精灵法阵进阶成功率
struct JinglingFazhenUpGradeProbConfig
{
	JinglingFazhenUpGradeProbConfig() : low_bless_val(0), high_bless_val(0), succ_rate(0),
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
struct JinglingFazhenImageConfig								 
{
	JinglingFazhenImageConfig() : image_id(0)
	{
		memset(image_name, 0, sizeof(image_name));
	}

	int image_id;
	GameName image_name;
};

// 特殊形象
struct JinglingFazhenSpecialImgCfg
{
	JinglingFazhenSpecialImgCfg() : image_id(0){}

	bool IsValid() { return 0 != image_id; }

	int image_id;
};

// 特殊形象进阶
struct JinglingFazhenSpecialImgUpgradeCfg
{
	JinglingFazhenSpecialImgUpgradeCfg() : image_id(0), grade(0), stuff_id(0), stuff_num(0),
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

class JinglingFazhenConfig : public ConfigBase
{
public:
	JinglingFazhenConfig();
	~JinglingFazhenConfig();

	bool Init(const std::string &configname, std::string *err);
	const JinglingFazhenGradeConfig * GetGradeConfig(int grade) const;
	const JinglingFazhenUpGradeProbConfig * GetUpGradeProbConfig(int grade, int bless_val) const;
	const JinglingFazhenImageConfig * GetImageCfg(int attr_id) const;

	const JinglingFazhenSpecialImgCfg * GetSpecialImgCfg(int special_img_id);
	const JinglingFazhenSpecialImgUpgradeCfg * GetSpecialImgUpgradeCfg(int special_img_id, int grade);

private:
	int InitOther(PugiXmlNode RootElement);
	int InitGradeCfg(PugiXmlNode RootElement);
	int InitUpGradeProbCfg(PugiXmlNode RootElement);
	int InitImageCfg(PugiXmlNode RootElement);
	int InitSpecialImageCfg(PugiXmlNode RootElement);
	int InitSpecialImageUpgradeCfg(PugiXmlNode RootElement);

	JinglingFazhenGradeConfig m_grade_cfg_list[JinglingFazhenParam::MAX_GRADE];

	struct UpGradeProbList
	{
		static const int MAX_PROB_COUNT_PER_GRADE = 600;

		UpGradeProbList() : grade(0), prob_count(0) {}

		bool Add(JinglingFazhenUpGradeProbConfig &cfg)
		{
			if (prob_count >= MAX_PROB_COUNT_PER_GRADE) return false;

			prob_list[prob_count ++] = cfg;
			return true;
		}

		short grade;
		short prob_count;
		JinglingFazhenUpGradeProbConfig prob_list[MAX_PROB_COUNT_PER_GRADE];
	};

	std::map<int, UpGradeProbList> m_upgrade_prob_map;

	JinglingFazhenImageConfig m_image_list[JinglingFazhenParam::MAX_IMAGE_ID + 1];

	////////////////////////////////////////////////////////////////////////////
	JinglingFazhenSpecialImgCfg m_special_img_cfglist[JinglingFazhenParam::MAX_SPECIAL_IMAGE_ID + 1];

	struct JinglingFazhenSpecialImgUpgradeCfgList
	{
		JinglingFazhenSpecialImgUpgradeCfgList() : max_grade(0) {}

		int max_grade;
		JinglingFazhenSpecialImgUpgradeCfg upgrade_list[JinglingFazhenParam::MAX_UPGRADE_LIMIT + 1];
	};

	typedef std::map<int, JinglingFazhenSpecialImgUpgradeCfgList> JinglingFazhenSpecialImgUpgradeCfgMap;
	JinglingFazhenSpecialImgUpgradeCfgMap m_special_img_upgrade_map;
};

#endif
