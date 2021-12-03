#ifndef _JINGLING_CFG_HPP_
#define _JINGLING_CFG_HPP_

#include <string>
#include <map>
#include <vector>
#include "servercommon/serverdef.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/jinglingdef.hpp"
#include "servercommon/maildef.hpp"
#include "item/itembase.h"
#include "item/itemgriddata.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct JingLingOtherCfg
{
	JingLingOtherCfg() : halo_upgrade_item_id(0), halo_normal_exp(0), halo_baoji_rate(0), halo_baoji_exp(0), rename_cost(0),
		xianzhen_stuff_id(0), xianzhen_add_exp(0), xianzhen_small_rate(0), xianzhen_small_exp(0), xianzhen_big_rate(0),
		xianzhen_big_exp(0), xianzhen_force_big_num(0), skill_free_refresh_count(0), get_skill_item(0),
		home_get_reward_interval(0), home_reward_interval(0), home_reward_times_limit(0), home_reward_hunli_level(0),
		home_rob_times_limit(0), home_rob_repeat_limit(0), home_rob_lingjing_per(0), home_rob_lingjing_max(0), home_rob_hunli_per(0),
		home_rob_hunli_max(0), home_rob_item_min(0), home_rob_item_max(0), home_quick_gold(0), home_quick_time(0), explore_times(0),
		explore_other_buy_gold(0), explore_buff_add_per(0), explore_buff_buy_gold(0), explore_buff_max_count(0), explore_name_count(0),
		skill_solt_free_count (0), skill_refresh_consume_id(0), refresh_one_consume_count(0), refresh_ten_consume_count(0){}

	int halo_upgrade_item_id;
	int halo_normal_exp;
	int halo_baoji_rate;
	int halo_baoji_exp;
	int rename_cost;
	ItemID xianzhen_stuff_id;
	int xianzhen_add_exp;
	int xianzhen_small_rate;
	int xianzhen_small_exp;
	int xianzhen_big_rate;
	int xianzhen_big_exp;
	int xianzhen_force_big_num;
	int skill_free_refresh_count;
	ItemID get_skill_item;

	int home_get_reward_interval;
	int home_reward_interval;
	int home_reward_times_limit;
	int home_reward_hunli_level;
	int home_rob_times_limit;
	int home_rob_repeat_limit;
	int home_rob_lingjing_per;
	int home_rob_lingjing_max;
	int home_rob_hunli_per;
	int home_rob_hunli_max;
	int home_rob_item_min;
	int home_rob_item_max;
	int home_quick_gold;
	int home_quick_time;

	int explore_times;
	int explore_hp_percent;
	int explore_other_buy;
	int explore_other_buy_gold;
	int explore_buff_add_per;
	int explore_buff_buy_gold;
	int explore_buff_max_count;
	int explore_name_count;
	int skill_solt_free_count;
	int skill_refresh_consume_id;
	int refresh_one_consume_count;
	int refresh_ten_consume_count;
};

struct JingLingItemCfg
{
	JingLingItemCfg() : item_id(0), level(0), cost_lingjing(0), 
		gongji(0), fangyu(0), maxhp(0),mingzhong(0),shanbi(0),baoji(0),jianren(0),
		baoji_per(0), jingzhun_per(0), pofang_per(0), mianshang_per(0), is_broadcast(0), skill_num(0) {}

	ItemID item_id;
	short level;
	int cost_lingjing;

	Attribute gongji;
	Attribute fangyu;
	Attribute maxhp;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	Attribute baoji_per;
	Attribute jingzhun_per;
	Attribute pofang_per;
	Attribute mianshang_per;
	short is_broadcast;
	int skill_num;
};

struct JingLingLevelItemList
{
	JingLingLevelItemList() :item_id(0) {}

	ItemID item_id;

	JingLingItemCfg item_list[JINGLING_MAX_LEVEL + 1];
};

struct JingLingTalentAttrItemCfg
{
	struct TalentAttr
	{
		TalentAttr() : talent_type(0), param1(0) {}

		int	talent_type;
		int param1;

		int	get_rate;
	};

	JingLingTalentAttrItemCfg() : item_id(0), maxhp_zizhi(0), gongji_zizhi(0), fangyu_zizhi(0), skill_num(0), max_num(0),
		home_reward_lingjing(0), home_reward_hunli(0), home_other_rate(0), home_other_type(0) {}

	ItemID item_id;
	int maxhp_zizhi;
	int gongji_zizhi;
	int fangyu_zizhi;
	int skill_num;
	short max_num;

	int home_reward_lingjing;
	int home_reward_hunli;
	int home_other_rate;
	int home_other_type;

	std::map<int, TalentAttr> random_talent_map;
};

struct JingLingGroupCfg
{
	JingLingGroupCfg() : id(0), itemid1(0), itemid2(0), itemid3(0), itemid4(0), itemid5(0),
		gongji(0), fangyu(0), maxhp(0), gongji_per(0), mianshang_per(0), maxhp_per(0), skill_id(0)
	{

	}

	bool IsAllMatch(ItemID id1, ItemID id2, ItemID id3, ItemID id4, ItemID id5)  //配置里配置都具有则认为匹配
	{
		return IsMatchOne(itemid1, id1, id2, id3, id4, id5) && 
				IsMatchOne(itemid2, id1, id2, id3, id4, id5) &&
				IsMatchOne(itemid3, id1, id2, id3, id4, id5) &&
				IsMatchOne(itemid4, id1, id2, id3, id4, id5) &&
				IsMatchOne(itemid5, id1, id2, id3, id4, id5);
	}

	bool IsMatchOne(ItemID check_id, ItemID id1, ItemID id2, ItemID id3, ItemID id4 ,ItemID id5)
	{
		return (0 == check_id || id1 == check_id || id2 == check_id || id3 == check_id || id4 == check_id || id5 == check_id);
	}

	int id;

	ItemID itemid1;
	ItemID itemid2;
	ItemID itemid3;
	ItemID itemid4;
	ItemID itemid5;

	int gongji;
	int fangyu;
	int maxhp;
	int gongji_per;
	int mianshang_per;
	int maxhp_per;
	int jingzhun_per;
	int baoji_per;

	int skill_id;
};

struct JingLingGetCfg
{
	JingLingGetCfg() : itemid(0), getway(0), param1(0), param2(0)
	{
		memset(talent_list, 0, sizeof(talent_list));
	}

	ItemID itemid;
	int getway;
	int param1;
	int param2;

	int talent_list[ItemNamespace::MAX_XIANPIN_NUM];
};

struct JingLingCardCfg
{
	JingLingCardCfg() : type(0), level(0), stuff_id(0), exp(0) {}

	int type;
	int level;
	
	int stuff_id;
	int exp;

	struct AttrItem
	{
		AttrItem() : attr_type(0), value(0) {}
		short attr_type;
		int value;
	};

	JingLingCardCfg::AttrItem attr_list[JINGLING_CARD_ATTR_COUNT];
};

struct JingLingUpGradeCfg
{
	JingLingUpGradeCfg() : grade(0), bless_val_limit(0), stuff_id(0), stuff_count(0), broadcast(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0),
		active_image_id(0) {}

	int grade;
	int bless_val_limit;
	ItemID stuff_id;
	int stuff_count;
	int broadcast;

	int maxhp;
	int gongji;
	int fangyu;
	int mingzhong;
	int shanbi;
	
	int active_image_id;
};

struct JingLingUpGradeProbCfg
{
	JingLingUpGradeProbCfg() : grade(0), low_bless(0), high_bless(0), succ_rate(0), fail_add_1_rate(0), fail_add_2_rate(0), fail_add_3_rate(0) {}

	int grade;
	int low_bless;
	int high_bless;
	int succ_rate;
	int fail_add_1_rate;
	int fail_add_2_rate;
	int fail_add_3_rate;

	short RandAddBlessVal() const
	{		
		int rand_val = RandomNum(100);
		if (rand_val < fail_add_1_rate) return 8;
		else if (rand_val < fail_add_2_rate) return 10;
		else return 12;		
	}
};

struct  JingLingEquipStrengthCfg
{
	JingLingEquipStrengthCfg() : index(0), strength_level(0), stuff_id(0), stuff_count(0), 
											maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), broadcast(0) {}

	int index;
	int strength_level;
	int stuff_id;
	int stuff_count;
	int maxhp;
	int gongji;
	int fangyu;
	int mingzhong;
	int shanbi;
	int  broadcast;
};

struct JingLingPhantomCfg
{
	JingLingPhantomCfg() : type(-1), stuff_id(0), stuff_num(0), level(0), gongji(0), fangyu(0), 
		maxhp(0), mingzhong(0), shanbi(0), is_broadcast(false), imageid(0) {}

	int type;
	int stuff_id;
	int stuff_num;
	int level;
	int gongji;
	int fangyu;
	int maxhp;
	int mingzhong;
	int shanbi;
	bool is_broadcast;
	int imageid;
};

struct JingLingSoulCfg
{
	JingLingSoulCfg() : type(0), level(0), stuff1_id(0), stuff1_num(0), stuff2_id(0),
		stuff2_num(0), fail_down_level(false), gongji(0), fangyu(0), maxhp(0), mingzhong(0),
		shanbi(0), succ_rate(0), is_broadcast(false) 
	{

	}
	
	int type;
	int level;
	int stuff1_id;
	int stuff1_num;
	int stuff2_id;
	int stuff2_num;
	bool fail_down_level;
	int gongji;
	int fangyu;
	int maxhp;
	int mingzhong;
	int shanbi;
	int succ_rate;
	bool is_broadcast;
};

struct JingLingSoulTaoZhuangCfg
{
	JingLingSoulTaoZhuangCfg() : suit_id(0), gongji(0), fangyu(0), maxhp(0), mingzhong(0), shanbi(0),
		need_min_level(0), is_broadcast(false) 
	{
		
	}

	int suit_id;
	int gongji;
	int fangyu;
	int maxhp;
	int mingzhong;
	int shanbi;
	int need_min_level;
	bool is_broadcast;
};

struct JingLingHaloActiveCfg
{
	JingLingHaloActiveCfg() : halo_id(0), active_need_level(0) {}
	~JingLingHaloActiveCfg(){}

	bool IsValid(){ return (0 == halo_id || 0 == active_need_level);}

	short halo_id;
	short active_need_level;
};

struct JingLingHaloUplevelCfg
{
	JingLingHaloUplevelCfg() : level(0), big_grade(0), uplevel_need_stuff_count(0), bless_val_limit(0), is_broadcast(false),
		gongji(0), fangyu(0), maxhp(0), mingzhong(0), shanbi(0) {}
	~JingLingHaloUplevelCfg(){}

	short level;
	short big_grade;
	short uplevel_need_stuff_count;
	int bless_val_limit;
	bool is_broadcast;

	int gongji;													
	int fangyu;													
	int maxhp;													
	int mingzhong;												
	int shanbi;																								
};

struct JingLingWuXingCfg
{
	JingLingWuXingCfg() : wuxing_level(-1), stuff_id(0), stuff_num(0), safe_id(0), safe_num(0), succ_rate(0), is_down(false), broadcast_type(0),
		maxhp_zizhi(0), gongji_zizhi(0), fangyu_zizhi(0), maxhp(0), gongji(0), fangyu(0), skill_num(0), recycle_stuff_num(0) {}

	int wuxing_level;
	ItemID stuff_id;
	short stuff_num;
	ItemID safe_id;
	short safe_num;
	int succ_rate;
	bool is_down;
	short broadcast_type;
	int maxhp_zizhi;
	int gongji_zizhi;
	int fangyu_zizhi;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	int skill_num;
	short recycle_stuff_num;
};

struct JingLingXianZhenCfg
{
	JingLingXianZhenCfg() : xianzhen_level(-1), need_exp(0), stuff_num(0), maxhp(0), gongji(0), fangyu(0), convert_rate(0) {}

	int xianzhen_level;
	int need_exp;
	int stuff_num;
	int maxhp;
	int gongji;
	int fangyu;
	int convert_rate;
};

struct JingLingXianZhenHunYuCfg
{
	JingLingXianZhenHunYuCfg() : hunyu_type(0), hunyu_level(0), stuff_id(0), stuff_num(0), convert_rate(0) {}

	int hunyu_type;
	int hunyu_level;
	ItemID stuff_id;
	short stuff_num;
	int convert_rate;
};

struct JingLingSkillCfg
{
	JingLingSkillCfg() : skill_id(0), pre_skill(0), book_id(0), skill_level(0), skill_type(0), maxhp(0), gongji(0), fangyu(0),
		mingzhong(0), shanbi(0), baoji(0), kangbao(0),
		maxhp_per(0), gongji_per(0), fangyu_per(0), constant_zengshang(0), constant_mianshang(0),
		capability(0), remove_stuff_id(0), move_stuff_id(0), cooldown(0), param_1(0), param_2(0), param_3(0), param_4(0), rand_weight(0) {}

	int skill_id;
	int pre_skill;
	ItemID book_id;
	int skill_level;
	int skill_type;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute kangbao;
	int maxhp_per;
	int gongji_per;
	int fangyu_per;
	Attribute constant_zengshang;
	Attribute constant_mianshang;
	int capability;
	ItemID remove_stuff_id;
	ItemID move_stuff_id;
	int cooldown;				// 冷却 秒
	int param_1;
	int param_2;
	int param_3;
	int param_4;
	int rand_weight;
};

static const int JING_LING_SKILL_REFRESH_STATE_MAX = 20;
static const int JING_LING_SKILL_REFRESH_RATE_MAX = 50;
struct JingLingSkillRefreshRateCfg
{
	JingLingSkillRefreshRateCfg() : skill_id(0), weight(0) {}

	int skill_id;
	int weight;
};

struct JingLingSkillRefreshCfg
{
	JingLingSkillRefreshCfg() : stage(0), min_count(0), max_count(0), once_gold(0), ten_gold(0), total_weight(0), cfg_count(0) {}

	int stage;
	int min_count;
	int max_count;
	int once_gold;
	int ten_gold;

	int total_weight;
	int cfg_count;
	JingLingSkillRefreshRateCfg cfg_list[JING_LING_SKILL_REFRESH_RATE_MAX];
};

static const int JING_LING_HOME_REWARD_TYPE_MAX = 20;
static const int JING_LING_HOME_REWARD_ITEM_CFG_MAX = 20;
struct JingLingHomeOtherRewardCfg
{
	JingLingHomeOtherRewardCfg() : total_weight(0), reward_count(0)
	{
		memset(weight_list, 0, sizeof(weight_list));
	}

	int total_weight;
	int reward_count;
	int weight_list[JING_LING_HOME_REWARD_ITEM_CFG_MAX];
	ItemConfigData reward_item_list[JING_LING_HOME_REWARD_ITEM_CFG_MAX];
};

static const int JING_LING_EXPLORE_GOLE_FETCH_COUNT = 2;
static const int JING_LING_EXPLORE_REWARD_COUNT_MAX = 20;
struct JingLingExploreCfg
{
	JingLingExploreCfg() : mode(0), level(0), min_capability(0), max_capability(0), free_times(0), reward_lingjing(0), reward_count(0)
	{
		memset(fetch_gold_list, 0, sizeof(fetch_gold_list));
	}

	struct RewardCfg
	{
		RewardCfg() : item_count(0), role_level(0) {}

		int item_count;
		int role_level;
		ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
	};

	const RewardCfg *GetReward(int role_level) const
	{
		for (int i = 0; i < reward_count && i < JING_LING_EXPLORE_REWARD_COUNT_MAX; i++)
		{
			if (role_level <= reward_cfg_list[i].role_level)
			{
				return &reward_cfg_list[i];
			}
		}

		return NULL;
	}

	int mode;
	int level;
	int min_capability;
	int max_capability;
	int free_times;
	int fetch_gold_list[JING_LING_EXPLORE_GOLE_FETCH_COUNT];
	int reward_lingjing;
	int reward_count;
	RewardCfg reward_cfg_list[JING_LING_EXPLORE_REWARD_COUNT_MAX];
};

struct JingLingHuanHuaSpecialCapAddCfgItem
{
	JingLingHuanHuaSpecialCapAddCfgItem() : huanhua_id(-1), huanhua_level(0), maxhp(0), gongji(0), fangyu(0) {}

	int huanhua_id;
	int huanhua_level;
	int maxhp;
	int gongji;
	int fangyu;
};

struct WuXingStuffCfg
{
	WuXingStuffCfg() :quality(0), stuff_id(0) {}

	int quality;
	ItemID stuff_id;
};

struct SkillSlotOpenConsume
{
	SkillSlotOpenConsume() : slot_index(0), gold(0) {}
	int slot_index;
	int gold;
};

class JingLingConfig : public ConfigBase
{
public:
	JingLingConfig();
	~JingLingConfig();

	bool Init(const std::string &configname, std::string *err);
	int InitOther(PugiXmlNode RootElement);
	int InitUpLevelConfig(PugiXmlNode RootElement);
	int InitTalentConfig(PugiXmlNode RootElement);
	int InitGroupConfig(PugiXmlNode RootElement);
	int InitGetConfig(PugiXmlNode RootElement);
	int InitCardConfig(PugiXmlNode RootElement);
	int InitUpGradeConfig(PugiXmlNode RootElement);
	int InitUpGradeProbConfig(PugiXmlNode RootElement);
	int InitEquipConfig(PugiXmlNode RootElement);
	int InitPhantomConfig(PugiXmlNode RootElement);
	int InitSoulConfig(PugiXmlNode RootElement);
	int InitSoulTaoZhuangConfig(PugiXmlNode RootElement);
	int InitHaloActiveCfg(PugiXmlNode RootElement);
	int InitHaloUplevelCfg(PugiXmlNode RootElement);
	int InitWuXingCfg(PugiXmlNode RootElement);
	int InitXianZhenCfg(PugiXmlNode RootElement);
	int InitXianZhenHunYuCfg(PugiXmlNode RootElement);
	int InitSkillCfg(PugiXmlNode RootElement);
	int InitSkillRefreshCfg(PugiXmlNode RootElement);
	int InitSkillRefreshRateCfg(PugiXmlNode RootElement);
	int InitHomeOtherRewardCfg(PugiXmlNode RootElement);
	int InitExploreCfg(PugiXmlNode RootElement);
	int InitExploreRewardCfg(PugiXmlNode RootElement);
	int InitExploreNameCfg(PugiXmlNode RootElement);
	int InitHuanHuaSpecialCapAddCfg(PugiXmlNode RootElement);
	int InitSkillSlotOpenConsumeCfg(PugiXmlNode RootElement);

	const JingLingOtherCfg * GetOtherCfg() { return &m_other_cfg; }
	const JingLingTalentAttrItemCfg *GetTalentAttrCfg(ItemID itemid);
	const JingLingItemCfg *GetJingLingItemCfg(ItemID itemid, int level);
	
	const JingLingGroupCfg *GetJingLingGroupCfg(int group_id);
	void GetJingLingGroupCfg(ItemID id1, ItemID id2, ItemID id3, ItemID id4, ItemID id5, int groupid_list[JINGLING_MAX_GROUP_IN_ONECE], int *group_num);

	const JingLingCardCfg *GetJingLingCardCfg(int card_type, int level);

	const JingLingUpGradeCfg *GetJingLingUpGradeCfg(int level);
	const JingLingUpGradeProbCfg *GetJingLingUpGradeProbCfg(int old_grade, int old_bless_val);
	const JingLingEquipStrengthCfg *GetJingLingEquipStrengthCfg(int part_index, int level);
	const JingLingPhantomCfg *GetJingLingPhantomCfg(int type, int level);
	const JingLingSoulCfg *GetJingLingSoulcfg(int type, int level);
	const JingLingSoulTaoZhuangCfg *GetSoulTaoZhuangCfg(int level);
	const JingLingHaloUplevelCfg * GetHaloUplevelCfg(int halo_level);
	
	bool HasJingLingPhantomCfg(int phantom_img_id);

	const bool GetRandomTalent(ItemGridData *item_grid_data);   		// 为精灵获得随机天赋属性
	const bool GetFixTalent(ItemGridData *item_grid_data, int getway, int getway_param1 = 0, int getway_param2 = 0);	// 为精灵获得固定属性
	const bool SetJingLingInitLevel(ItemGridData *item_grid_data);		// 设置精灵的初始等级
	int GetTalentValue(ItemID item_id, int talent_type);

	int GetRecycleLingjing(ItemID item_id, short level);
	int GetRecycleWuXingDan(short level);

	int GetHaloMaxLevel() { return m_halo_max_level; }
	int GetHaloActiveIdWithLevel(int halo_level);

	const JingLingWuXingCfg *GetJingLingWuXingCfg(int color, int wuxing_level);
	const JingLingXianZhenCfg *GetJingLingXianZhenCfg(int xianzhen_level);
	const JingLingXianZhenHunYuCfg *GetJingLingXianZhenHunYuCfg(int hunyu_type, int hunyu_level);
	const JingLingSkillCfg * GetSkillCfg(int skill_id);
	const JingLingSkillCfg * GetSkillCfgByItem(ItemID item_id);
	const JingLingSkillRefreshCfg * GetJingLingSkillRefreshCfg(int refresh_count);
	const ItemConfigData *RandHomeOtherRewardItem(int other_type);
	const JingLingExploreCfg *GetJingLingExploreCfg(int mode, int level);
	const JingLingHuanHuaSpecialCapAddCfgItem* GetHuanHuaSpecialCapAddCfg(int huanhua_id)const;
	bool IsAddSpecialCap();
	ItemID RandJingLingId();

	const WuXingStuffCfg *GetWuXingStuffCfg(int quality) const;

	const int GetSkillSlotOpenConsume(int slot_index);

private:

	JingLingOtherCfg m_other_cfg;

	std::map<ItemID, JingLingLevelItemList> m_jingling_map;
	std::map<ItemID, JingLingTalentAttrItemCfg> m_talent_attr_map;
	std::map<int, JingLingGroupCfg> m_jingling_group_map;
	std::vector<JingLingGetCfg> m_jingling_get;

	typedef std::vector<JingLingCardCfg> JingLingCardLevelVec;
	typedef std::map<int, JingLingCardLevelVec> JingLingCardMap;
	
	JingLingCardMap m_jingling_card_map;

	typedef std::vector<JingLingUpGradeCfg> JingLingUpGradeVec;
	JingLingUpGradeVec m_jingling_upgrade_vec;

	typedef std::vector<JingLingUpGradeProbCfg> JingLingUpGradeProbVec;
	JingLingUpGradeProbVec m_jingling_upgradeprob_vec;

	typedef std::vector<JingLingEquipStrengthCfg> JingLingEquipStrengthVec;
	typedef std::map<int, JingLingEquipStrengthVec> JingLingEquipStrengthMap;

	JingLingEquipStrengthMap m_jingling_equip_strength_map;

	typedef std::vector<JingLingPhantomCfg> JingLingPhantomVec;
	typedef std::map<int, JingLingPhantomVec> JingLingPhantomMap;
	JingLingPhantomMap m_jingling_phantom_map;

	typedef std::vector<JingLingSoulCfg> JingLingSoulVec;
	typedef std::map<int, JingLingSoulVec> JingLingSoulMap;
	JingLingSoulMap m_jingling_soul_map;

	typedef std::vector<JingLingSoulTaoZhuangCfg> JingLingSoulTaoZhuangVec;
	JingLingSoulTaoZhuangVec m_soul_taozhuang_vec;

	int m_halo_max_img_id;
	JingLingHaloActiveCfg m_halo_active_cfg_list[HALO_MAX_IMAGEID + 1];

	int m_halo_max_level;
	JingLingHaloUplevelCfg m_halo_uplevel_cfg_list[JINGLING_HALO_MAX_LEVEL + 1];

	//JingLingWuXingCfg m_wuxing_cfg_list[JING_LING_WU_XING_LEVEL_MAX + 1];
	JingLingWuXingCfg m_diff_wuxing_attr_cfg_list[JING_LING_COLOR_COUNT][JING_LING_WU_XING_LEVEL_MAX + 1];
	JingLingXianZhenCfg m_xianzhen_cfg_list[JING_LING_XIAN_ZHEN_LEVEL_MAX + 1];
	JingLingXianZhenHunYuCfg m_xianzhen_hunyu_cfg_list[XIAN_ZHEN_HUN_YU_TYPE_MAX][JING_LING_XIAN_ZHEN_HUN_YU_LEVEL_MAX + 1];
	JingLingSkillCfg m_skill_cfg_list[JING_LING_SKILL_ID_MAX + 1];
	JingLingSkillRefreshCfg m_skill_refresh_list[JING_LING_SKILL_REFRESH_STATE_MAX];

	JingLingHomeOtherRewardCfg m_home_reward_cfg_list[JING_LING_HOME_REWARD_TYPE_MAX];

	JingLingExploreCfg m_explore_cfg_list[JING_LING_EXPLORE_MODE_COUNT][JING_LING_EXPLORE_LEVEL_COUNT];

	bool m_is_open_special_cap_add;
	typedef std::map<int, JingLingHuanHuaSpecialCapAddCfgItem> JingLingHuanHuaSpecialCapAddCfg;
	JingLingHuanHuaSpecialCapAddCfg m_huanhua_special_cap_cfg;

	SkillSlotOpenConsume m_skill_slot_open_consume_list[JING_LING_SKILL_COUNT_MAX];
};

#define CHECK_ITEM_IS_JINGLING(item_id)\
{\
	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_id);\
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType() || Equipment::E_TYPE_SPECIAL_JINGLING != equip->GetEquipType())\
	{\
		return -100;\
	}\
}

#define JINGLINGCONFIG_GETTALENT(t_type, param1_str, get_rate_str)\
{\
	JingLingTalentAttrItemCfg::TalentAttr talent_attr;\
	talent_attr.talent_type = t_type;\
	if (!PugiGetSubNodeValue(data_element, param1_str, talent_attr.param1) ||  talent_attr.param1 < 0)\
	{\
		return -2;\
	}\
	if (!PugiGetSubNodeValue(data_element, get_rate_str, talent_attr.get_rate) ||  talent_attr.get_rate < 0)\
	{\
		return -3;\
	}\
	talent_cfg.random_talent_map[talent_attr.talent_type] = talent_attr;\
}

#define JINGLINGCONFIG_GET_FIX_TALENT(type, talent)\
if (!PugiGetSubNodeValue(data_element, type, is_has) || (is_has != 0 && is_has != 1))\
{\
	return -11;\
}\
if (1 == is_has)\
{\
	item_cfg.talent_list[count] = talent;\
	++ count;\
}\

#endif


