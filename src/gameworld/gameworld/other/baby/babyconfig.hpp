#ifndef __BABY_CONFIG_H__
#define __BABY_CONFIG_H__

#include "obj/character/attribute.hpp"
#include "common/tinyxml/tinyxml.h"
#include "servercommon/babydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "item/itembase.h"
#include <vector>

struct BabyOtherCfg
{
	BabyOtherCfg() : free_rename_times(0), rename_card_id(ItemBase::INVALID_ITEM_ID), born_need_grade(0) {}

	int free_rename_times;						// 免费改名次数
	ItemID rename_card_id;						// 宝宝改名卡ID
	int born_need_grade;						// 生下一个孩子的时候需要其它孩子的阶数

};

struct BabyLevelCfg
{
	BabyLevelCfg() : level(0), stuff_id_1(ItemBase::INVALID_ITEM_ID), stuff_num_1(0), stuff_id_2(ItemBase::INVALID_ITEM_ID), stuff_num_2(0),
		stuff_id_3(ItemBase::INVALID_ITEM_ID), stuff_num_3(0), stuff_id_4(ItemBase::INVALID_ITEM_ID), stuff_num_4(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0) {}

	int level;

	ItemID stuff_id_1;
	int stuff_num_1;

	ItemID stuff_id_2;
	int stuff_num_2;

	ItemID stuff_id_3;
	int stuff_num_3;

	ItemID stuff_id_4;
	int stuff_num_4;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;

};

struct BabyGradeConfig
{
	BabyGradeConfig() : grade(0), consume_stuff_id(ItemBase::INVALID_ITEM_ID), consume_stuff_num(0), max_bless_value(0), is_broadcast(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0) {}

	int grade;															// 阶数
	int consume_stuff_id;												// 进阶消耗物品ID
	int consume_stuff_num;												// 进阶消耗物品数量
	int max_bless_value;												// 最大祝福值

	int is_broadcast;													// 是否传闻

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
};

struct BabyInfoCfg
{
	BabyInfoCfg() : baby_id(INVALID_BABY_ID), max_level(-1), max_grade(-1), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0)
	{
		memset(baby_name, 0, sizeof(baby_name));
	}

	int baby_id;
	GameName baby_name;

	int max_level;
	BabyLevelCfg level_cfg_list[BABY_MAX_LEVEL_LIMIT + 1];

	int max_grade;
	BabyGradeConfig grade_cfg_list[BABY_MAX_GRADE_LIMIT + 1];

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;

};

struct BabyUpGradeRateConfig
{
	BabyUpGradeRateConfig() : grade(0), low_bless(0), high_bless(0), succ_rate(0), fail_add_1_rate(0), fail_add_2_rate(0), fail_add_3_rate(0) {}

	int grade;															// 当前阶数
	int low_bless;														// 祝福值下限
	int high_bless;														// 祝福值上限
	int succ_rate;														// 成功率万分比
	int fail_add_1_rate;												// 失败加8祝福值概率
	int fail_add_2_rate;												// 失败加10祝福值概率
	int fail_add_3_rate;												// 失败加12祝福值概率

	int GetRandomZhufuOnFail() const;
};

struct BabyUpgradeRateConfigArr
{
	std::vector<BabyUpGradeRateConfig> vect;
};

struct BabyQifuTreeConfig
{
	BabyQifuTreeConfig() : qifu_type(0), qifu_consume_bind_gold(0), qifu_consume_gold(0), activity_qifu_consume_bind_gold(0), activity_qifu_consume_gold(0), qifu_reward_baby_id(INVALID_BABY_ID){}

	int qifu_type;
	int qifu_consume_bind_gold;
	int qifu_consume_gold;
	int activity_qifu_consume_bind_gold;
	int activity_qifu_consume_gold;

	int qifu_reward_baby_id;

};

struct BabyChaoshengConfig
{
	BabyChaoshengConfig() : need_gold(0) {}

	int need_gold;
};

struct BabySpiritConfig
{
	BabySpiritConfig() : consume_item(ItemBase::INVALID_ITEM_ID), train_val(0), 
						maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0) {}

	ItemID	consume_item;
	int train_val;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
};

// 特殊宝宝配置
struct SpecialBabyLvCfg
{
	int speical_baby_quality = 0;
	int level = 0;
	int active_item_id = 0;

	AttrConfig attr_cfg;
};

enum QIFU_TREE_TYPE
{
	QIFU_TREE_MIN = 0,

    QIFU_TREE_NORMAL = 1,				// 普通祈福
	QIFU_TREE_HIGH = 2,					// 高级祈福
	QIFU_TREE_BEST = 3,					// 至尊祈福

	QIFU_TREE_MAX
};

class BabyConfig : public ConfigBase
{
public:
	BabyConfig();
	~BabyConfig();

	bool Init(const std::string &configname, std::string *err);
	const BabyOtherCfg & GetOtherCfg() { return m_other_cfg; }
	const BabyInfoCfg * GetBabyInfo(int baby_id);
	const BabyUpGradeRateConfig * GetSuccRate(int grade, short bless);
	const BabyQifuTreeConfig * GetQifuCfg(int type);
	const BabyGradeConfig * GetBabyGradeCfg(int id, int grade);
	const BabyLevelCfg * GetBabyLevelCfg(int id, int level);
	const int GetBabyMaxGrade() { return m_grade_cfg_count; }
	int GetBabyChaoshengGold(int num);
	const BabySpiritConfig * GetBabySpiritCfg(int spirit_id, int level);

	const SpecialBabyLvCfg * GetSpecialBabyLvCfg(int speical_baby_type, int quality, int lv);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitBabyInfoCfg(PugiXmlNode RootElement);
	int InitBabyLevelCfg(PugiXmlNode RootElement);
	int InitBabyGradeCfg(PugiXmlNode RootElement);
	int InitBabyUpgradeRateCfg(PugiXmlNode RootElement);
	int InitBabyQifuTreeCfg(PugiXmlNode RootElement);
	int InitBabyChaoshengCfg(PugiXmlNode RootElement);
	int InitBabySpiritCfg(PugiXmlNode RootElement);
	int InitSpecialBabyCfg(PugiXmlNode RootElement);

	BabyOtherCfg m_other_cfg;
	
	int m_all_baby_count;
	BabyInfoCfg m_all_baby_list[BABY_MAX_COUNT];

	int m_grade_cfg_count;
	BabyUpgradeRateConfigArr m_grade_cfg_list[BABY_MAX_GRADE_LIMIT + 1];

	int m_qifu_tree_cfg_count;
	BabyQifuTreeConfig m_qifu_tree_cfg_list[BABY_MAX_QIFU_TREE_CFG_COUNT + 1];

	int m_chaosheng_cfg_count;
	BabyChaoshengConfig m_chaosheng_cfg_list[BABY_MAX_CHAOSHENG_COUNT + 1];

	int m_spirit_max_level_list[BABY_SPIRIT_COUNT];
	BabySpiritConfig m_spirit_cfg_list[BABY_SPIRIT_COUNT][BABY_MAX_SPIRIT_LEVEL_LIMIT + 1];

	std::vector<SpecialBabyLvCfg> m_special_baby_list[SPECIAL_BABY_TYPE_MAX];
};




#endif