#ifndef __RANDACTIVITYCONFIG_HPP__
#define __RANDACTIVITYCONFIG_HPP__

#include "servercommon/activitydef.hpp"
#include "gamedef.h"
#include "servercommon/maildef.hpp"
#include <map>
#include "servercommon/rankdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include <vector>

static const int MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM = 32;

static const int RAND_ACTIVITY_LUCKROOL_REWARD_CONFIG_MAX_COUNT = 8;						// 幸运摇奖奖励最大配置个数
static const int RAND_ACTIVITY_STONE_UPLEVEL_REWARD_CONFIG_MAX_COUNT = 16;					// 随机活动宝石升级奖励最大配置个数
static const int RAND_ACTIVITY_CHONGZHI_RANK_REWARD_CONFIG_MAX_COUNT = 100;					// 随机活动累计充值排行榜奖励最大配置个数
static const int RAND_ACTIVITY_CONSUME_GOLD_RANK_REWARD_CONFIG_MAX_COUNT = 100;				// 随机活动累计消费排行榜奖励最大配置个数
static const int RAND_ACTIVITY_EQUIP_STRENGTHEN_REWARD_CONFIG_MAX_COUNT = 12;				// 随机活动装备强化奖励最大配置个数
static const int RAND_ACTIVITY_EQUIP_STRENGTHEN_REWARD_ITEM_MAX_COUNT = 2;					// 随机活动装备强化奖励物品最大配置个数
static const int RAND_ACTIVITY_DAY_CHONGZHI_FANLI_REWARD_CONFIG_MAX_COUNT = 12;				// 每日充值奖励最大配置个数
static const int RAND_ACTIVITY_DAY_CONSUME_GOLD_REWARD_CONFIG_MAX_COUNT = 8;				// 每日消费奖励最大配置个数
static const int RAND_ACTIVITY_XIANNV_CHANMIAN_UPLEVEL_REWARD_CONFIG_MAX_COUNT = 16;		// 仙女缠绵升级奖励最大配置个数
static const int RAND_ACTIVITY_MOUNT_UPGRADE_REWARD_CONFIG_MAX_COUNT = 10;					// 坐骑进阶奖励最大配置个数
static const int RAND_ACTIVITY_QIBING_UPGRADE_REWARD_CONFIG_MAX_COUNT = 10;					// 骑兵进阶奖励最大配置个数
static const int RAND_ACTIVITY_MENTALITY_TOTAL_LEVEL_REWARD_CONFIG_MAX_COUNT = 10;			// 根骨全身等级奖励最大配置个数
static const int RAND_ACTIVITY_WING_UPGRADE_REWARD_CONFIG_MAX_COUNT = 20;					// 羽翼升级奖励最大配置个数
static const int RAND_ACTIVITY_CHESTSHOP_REWARD_CONFIG_MAX_COUNT = 16;						// 寻宝次数奖励最大配置个数
static const int RAND_ACTIVITY_VIP_QIFU_REWARD_CONFIG_MAX_COUNT = 16;						// 全民祈福奖励最大配置个数
static const int RAND_ACTIVITY_TOTAL_CONSUME_GOLD_REWARD_CONFIG_MAX_COUNT = 10;				// 累计消费奖励最大配置个数
static const int RAND_ACTIVITY_DAY_ACTIVE_DEGREE_REWARD_CONFIG_MAX_COUNT = 8;				// 活跃奖励奖励最大配置个数
static const int RAND_ACTIVITY_XIANMENG_BIPIN_REWARD_CONFIG_MAX_COUNT = 8;					// 仙盟比拼奖励最大配置个数
static const int RAND_ACTIVITY_DAY_ONLINE_GIFT_CONFIG_MAX_COUNT = 8;						// 每日在线奖励最大配置个数
static const int RAND_ACTIVITY_XIANMENG_JUEQI_REWARD_CONFIG_MAX_COUNT = 8;					// 仙盟崛起奖励最大配置个数
static const int RAND_ACTIVITY_SHOUYOU_YUXIANG_REWARD_CONFIG_MAX_COUNT = 8;					// 手有余香奖励最大配置个数
static const int RAND_ACTIVITY_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT = 32;						// 登录送礼奖励最大配置个数
static const int RAND_ACTIVITY_BIPIN_ACTIVITY_MAX_COUNT = 16;								// 比拼活动最大数
static const int RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT = 32;						// 比拼全民奖励最大数
static const int RAND_ACTIVITY_CHARGE_REPAYMENT_CONFIG_MAX_COUNT = 10;						// 充值回馈配置最大数
static const int RAND_ACTIVITY_CORNUCOPIA_CONFIG_MAX_COUNT = 10;							// 聚宝盆配置最大数
static const int RAND_ACTIVITY_CORNUCOPIA_MAX_LUN = 8;										// 聚宝盆最大轮数
static const int RAND_ACTIVITY_DANBI_CHONGZHI_DAY_CFG_MAX_COUNT = 16;						// 单笔充值日配置最大数
static const int RAND_ACTIVITY_DANBI_CHONGZHI_REWARD_CFG_MAX_COUNT_PER_DAY = 16;			// 单笔充值每日奖励配置最大数
static const int RAND_ACTIVITY_TOTAL_CHARGE_DAY_CFG_MAX_SEQ = 10;							// 累计充值（天数）活动最大奖励索引
static const int RAND_ACTIVITY_TOMORROW_REWARD_CFG_MAX_COUNT = 10;							// 次日福利奖励最大个数
static const int RAND_ACTIVITY_DAY_CHONGZHI_RANK_REWARD_MAX_DAY = 16;						// 每日充值排行最多奖励天数
static const int RAND_ACTIVITY_DAY_CONSUME_RANK_REWARD_MAX_DAY = 16;						// 每日消费排行最多奖励天数
static const int RAND_ACTIVITY_ZHENBAOGE_MAX_SEQ = 900;										// 珍宝阁刷新物品配置数量
static const int RAND_ACTIVITY_ZHENBAOGE_SERVER_MAX_SEQ = 50;								// 珍宝阁全服礼包配置数量
static const int RAND_ACTIVITY_ZHENBAOGE_AUTO_ADD_WEIGHT_CFG_COUNT = 5;						// 珍宝阁自增长权重配置数量
static const int RAND_ACTIVITY_ZHENBAOGE_MAX_RETURN_REWARD_COUNT = 50;						// 珍宝阁最大返利配置数
static const int RAND_ACTIVITY_MIJINGXUNBAO_AUTO_ADD_WEIGHT_CFG_COUNT = 5;			    	// 秘境寻宝自增长权重配置数量
static const int RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ = 250;									// 秘境寻宝淘宝物品配置数量
static const int RAND_ACTIVITY_MIJINGXUNBAO_REWARD_MAX_COUNT = 10;							// 秘境寻宝返利每段最大配置数
static const int RAND_ACTIVITY_JISUCHONGZHAN_CFG_NUM = 10;									// 急速冲战配置数
static const int RAND_ACTIVITY_TOTAL_CHARGE_3_CFG_COUNT = 100;								// 累计充值3，最大时间段数
static const int RAND_ACTIVITY_ANY_MONSTER_DROP_CFG_COUNT = 10;								// 全场景怪物掉落活动，配置最大数
static const int RAND_ACTIVITY_TOTAL_CHARGE_4_CFG_COUNT = 10;								// 累计充值4，最大时间段数
static const int RAND_ACTIVITY_TIANMING_DIVINATION_REWARD_MAX_SEQ = 480;					// 天命卜卦奖励最大奖池下标
static const int RAND_ACTIVITY_TIANMING_DIVINATION_TOTAL_MAX_ADD_LOT_TIMES = 54;			// 天命卜卦最大加注次数
static const int RAND_ACTIVITY_TIANMING_DIVINATION_EVERY_LOT_MAX_ADD_TIMES = 9;				// 天命卜卦每个签筒最大加注次数
static const int RAND_ACTIVITY_TIANMING_DIVINATION_OPENGAME_DAY_INTERVAL = 24;				// 天命卜卦每个分段最大配置数
static const int RAND_ACTIVITY_OPEN_SERVER_REWARD_COUNT_MAX = 20;							// 开服活动最大奖励物品数量
static const int RAND_ACTIVITY_NEW_CORNUCOPIA_CFG_COUNT_MAX = 7;							// 新聚宝盆最大配置数量
static const int RAND_ACTIVITY_NEW_CORNUCOPIA_TOTAL_CFG_COUNT_MAX = 5;						// 新聚宝盆全服奖励最大配置数量
static const int RAND_ACTIVITY_EXP_REFINE_REWARD_MAX_COUNT = 20;							// 经验炼制配置最大个数
static const int RAND_ACTIVITY_OPEN_SERVER_INVEST_MAX_COUNT = 7;							// 开服投资配置最大个数
static const int RAND_ACTIVITY_SINGLE_CHONGZHI_MAX_CFG = 5;									// 单笔返固定元宝最大配置数
static const int RAND_ACTIVITY_XIANYUAN_TREAS_CFG_COUNT = 10;								// 聚划算配置数量
static const int RAND_ACTIVITY_ZHENBAOGE2_MAX_SEQ = 900;									// 珍宝阁2最大配置数
static const int RAND_ACTIVITY_ZHENBAOGE2_MAX_RETURN_REWARD_COUNT = 50;						// 珍宝阁2最大返利配置数

static const int RAND_ACTIVITY_JINJIE_THEME_ACT_MAX_OPENGAME_DAY = 1000;                   //进阶主题活动以1000天为一个循环

static const int RAND_ACTIVITY_CONSUM_GIFT_MAX_SEQ = 6;                                    // 消费领奖每个主题最大配置数
static const int RAND_ACTIVITY_CONSUM_GIFT_MAX_REWARD_COUNT = 4;						   // 消费领奖奖励个数

static const int  RAND_ACTIVITY_COLLECT_TREASURE_MAX_CFG = 8;                               // 聚宝盆（新）

static const int RAND_ACTIVITY_HAPPY_CUMUL_CHONGZHI_MAX_REWARD_COUNT = 5;					// 欢乐累充奖励个数
static const int RAND_ACTIVITY_HAPPY_CUMUL_CHONGZHI_MAX_CFG = 9;						    // 欢乐累充配置数

static const int RAND_ACTIVITY_JINJIE_RETURN_MAX_CFG = 14;						           // 进阶返还配置数

static const int RAND_ACTIVITY_EVERYDAY_NICE_GIFT_MAX_REWARD_COUNT = 7;						// 每日好礼  最大配置奖励数量

static const int RA_LIMIT_TIME_REBATE_MAX_CFG_COUNT = 32;                                   //限时反馈

static const int RAND_ACTIVITY_LIMIT_TIME_GIFT_MAX_SEQ = 14;								// 限时礼包最大配置数

static const int RAND_ACITIVTY_EXPENSE_NICE_GIFT_NUM_TYPE_CONFIG_MAX_COUNT = 10;			// 消费好礼奖励数量类型最大配置数
static const int RAND_ACITIVTY_EXPENSE_NICE_GIFT_GRAND_TOTAL_REWARD_MAX_COUNT = 4;			// 消费好礼累计消费奖励最大配置数

static const int RAND_ACTIVITY_BUY_ONE_GET_ONE_FREE_ITEM_MAX_COUNT = 20;					// 买一送一最大物品个数
UNSTD_STATIC_CHECK(RAND_ACTIVITY_BUY_ONE_GET_ONE_FREE_ITEM_MAX_COUNT <= 63);

static const int RAND_ACTIVITY_EXTREME_CHALLENGE_MAX_TASK_COUNT = 40;						// 极限挑战最大任务数量

static const int RAND_ACTIVITY_HOLIDAYGUARD_PERSON_RANK_MAX = 10;							// 排行榜最大人数

static const int RAND_ACTIVITY_TOTAL_CHARGE_5_CFG_COUNT = 10;								// 累计充值5，最大时间段数

static const int RAND_ACTIVITY_LUCKY_WISH_LOTTERY_CFG_COUNT = 40;							// 幸运许愿开服天数配置数量

static const int RAND_ACTIVITY_IMAGE_COMPETITION_REWARD_CFG_COUNT = 4;                      // 形象比拼排行奖励配置数  

static const int RAND_ACTIVITY_SINGLE_CHARGE2_CFG_COUNT = 100;								// 急速冲战2开服天数配置数
static const int RAND_ACTIVITY_SINGLE_CHARGE3_CFG_COUNT = 100;								// 急速冲战3开服天数配置数
static const int RAND_ACTIVITY_SINGLE_CHARGE4_CFG_COUNT = 100;								// 急速冲战4开服天数配置数
static const int RAND_ACTIVITY_SINGLE_CHARGE5_CFG_COUNT = 100;								// 急速冲战5开服天数配置数
static const int RAND_ACTIVITY_DANBI_CHONGZHI_DAY_CFG_COUNT = 100;									// 每日单笔配置数
static const int RAND_ACTIVITY_SINGLE_CHAR_CFG_COUNT = 100;											// 单笔充值配置数
static const int RAND_ACTIVITY_CRACY_BUY_MAX_NUM_TO_OPENDAY = 21;							// 狂欢大乐购开服同一天数最多承载配置数量
static const int RAND_ACTIVITY_CRACY_BUY_MAX_OF_OPENGAMEDAY = 32;							// 狂欢大乐购最多承载10个不同的开服天数
static const int RAND_ACTIVITY_DOUBLE_GET_CFG_COUNT = 16;									// 双倍元宝开发天数配置数

struct RandActivityLuckyRollRewardCfg
{
	RandActivityLuckyRollRewardCfg() 
		: seq(0), reward_type(0), is_broadcast(false), weight(0), gold_percent(0) 
	{

	}

	int seq;
	int reward_type;
	bool is_broadcast;
	int weight;
	int gold_percent;													
	ItemConfigData reward_item;
};

struct RandActivityLuckyRollCfg
{
	RandActivityLuckyRollCfg() : cfg_count(0), opengame_day(0)
	{

	}

	struct RALuckyRollItem
	{
		RALuckyRollItem() : get_gold_min_gold_poll(0), replace_gold_reward_seq(0), extra_reward_need_times(0),
			unit_roll_consume_gold(0), unit_enter_gold_pool_num(0) 
		{
			
		}

		int get_gold_min_gold_poll;
		short replace_gold_reward_seq;
		short extra_reward_need_times;
		int unit_roll_consume_gold;
		int unit_enter_gold_pool_num;

		ItemConfigData extra_reward_item;
		RandActivityLuckyRollRewardCfg reward_cfg_list[RAND_ACTIVITY_LUCKROOL_REWARD_CONFIG_MAX_COUNT];
	};

	int cfg_count;
	int opengame_day;
	RALuckyRollItem cfg;
};

struct RandActivityEquipStrengthenCfg
{
	struct ConfigItem
	{
		ConfigItem() : level(0), item_count(0) {}

		int level;
		int item_count;
		ItemConfigData item_list[RAND_ACTIVITY_EQUIP_STRENGTHEN_REWARD_ITEM_MAX_COUNT];
	};

	RandActivityEquipStrengthenCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[RAND_ACTIVITY_EQUIP_STRENGTHEN_REWARD_CONFIG_MAX_COUNT];
};

struct RandActivityChongzhiRankCfg
{
	struct ConfigItem
	{
		ConfigItem() : rank(0), limit_chongzhi(0) {}

		int rank;
		int limit_chongzhi;
		ItemConfigData item;
	};

	RandActivityChongzhiRankCfg() : cfg_count(0), opengame_day(0) {}

	int cfg_count;
	int opengame_day;
	ConfigItem cfg_list[RAND_ACTIVITY_CHONGZHI_RANK_REWARD_CONFIG_MAX_COUNT];
};

struct RandActivityConsumeGoldRankCfg
{
	struct ConfigItem
	{
		ConfigItem() : rank(0), limit_comsume(0) {}

		int rank;
		int limit_comsume;
		ItemConfigData item;
	};

	RandActivityConsumeGoldRankCfg() : cfg_count(0), opengame_day(0) {}

	int cfg_count;
	int opengame_day;
	ConfigItem cfg_list[RAND_ACTIVITY_CONSUME_GOLD_RANK_REWARD_CONFIG_MAX_COUNT];
};

struct RandActivityServerPanicBuyCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), gold_price(0), server_limit_buy_count(0), personal_limit_buy_count(0) {}

		int seq;
		int gold_price;														
		int server_limit_buy_count;
		int personal_limit_buy_count;
		ItemConfigData reward_item;
	};

	RandActivityServerPanicBuyCfg() : cfg_count(0), opengame_day(0) {}

	int cfg_count;
	int opengame_day;
	ConfigItem cfg_list[RAND_ACTIVITY_SERVER_PANIC_BUY_ITEM_MAX_COUNT];
};

struct RandActivityPersonalPanicBuyCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), gold_price(0), limit_buy_count(0) {}

		int seq;
		int gold_price;
		int limit_buy_count;
		ItemConfigData reward_item;
	};

	RandActivityPersonalPanicBuyCfg() : cfg_count(0), opengame_day(0) {}

	int cfg_count;
	int opengame_day;
	ConfigItem cfg_list[RAND_ACTIVITY_PERSONAL_PANIC_BUY_ITEM_MAX_COUNT];
};

struct RandActivityDayChongZhiFanLiCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), need_gold(0), reward_gold(0), reward_bind_gold(0) {}

		int seq;															// 奖励索引
		int need_gold;														// 需要元宝
		int reward_gold;													// 奖励元宝
		int reward_bind_gold;												// 奖励绑定元宝
		ItemConfigData reward_item;											// 奖励物品
	};

	RandActivityDayChongZhiFanLiCfg() : cfg_count(0), opengame_day(0) {}

	int cfg_count;
	int opengame_day;
	ConfigItem cfg_list[RAND_ACTIVITY_DAY_CHONGZHI_FANLI_REWARD_CONFIG_MAX_COUNT];
};

struct RandActivityDayConsumeGoldCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), need_consume_gold(0) {}

		int seq;															// 奖励索引
		int need_consume_gold;												// 需要消费元宝	
		ItemConfigData reward_item;											// 奖励物品
	};

	RandActivityDayConsumeGoldCfg() : cfg_count(0), opengame_day(0) {}

	int cfg_count;
	int opengame_day;
	ConfigItem cfg_list[RAND_ACTIVITY_DAY_CONSUME_GOLD_REWARD_CONFIG_MAX_COUNT];
};

struct RandActivityChestShopCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), need_chestshop_times(0) {}

		int seq;
		int need_chestshop_times;
		ItemConfigData reward_item;
	};

	RandActivityChestShopCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[RAND_ACTIVITY_CHESTSHOP_REWARD_CONFIG_MAX_COUNT];
};

struct RandActivityStoneUplevelCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), need_total_stone_level(0) {}

		int seq;
		int need_total_stone_level;
		ItemConfigData reward_item;
	};

	RandActivityStoneUplevelCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[RAND_ACTIVITY_STONE_UPLEVEL_REWARD_CONFIG_MAX_COUNT];
};

struct RandActivityTotalConsumeGoldCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), need_consume_gold(0) {}

		int seq;															// 奖励索引
		int need_consume_gold;												// 需要累计消费元宝	
		ItemConfigData reward_item;											// 奖励物品
	};

	RandActivityTotalConsumeGoldCfg() : opengame_day(0), cfg_count(0) {}

	int opengame_day;
	int cfg_count;
	ConfigItem cfg_list[RAND_ACTIVITY_TOTAL_CONSUME_GOLD_REWARD_CONFIG_MAX_COUNT];
};

struct RandActivityDayActiveDegreeCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), need_active(0) {}

		int seq;															// 奖励索引
		int need_active;													// 需要活跃度	
		ItemConfigData reward_item;											// 奖励物品
	};

	RandActivityDayActiveDegreeCfg() : cfg_count(0), opengame_day(0) {}

	int cfg_count;
	int opengame_day;
	ConfigItem cfg_list[RAND_ACTIVITY_DAY_ACTIVE_DEGREE_REWARD_CONFIG_MAX_COUNT];
};

struct RandActivityXiannvChanmianUplevelCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), need_chanmian_level(0) {}

		int seq;
		int need_chanmian_level;
		ItemConfigData reward_item;
	};

	RandActivityXiannvChanmianUplevelCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[RAND_ACTIVITY_XIANNV_CHANMIAN_UPLEVEL_REWARD_CONFIG_MAX_COUNT];
};

struct RandActivityMountUpgradeCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), need_mount_grade(0) {}

		int seq;
		int need_mount_grade;
		ItemConfigData reward_item;
	};

	RandActivityMountUpgradeCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[RAND_ACTIVITY_MOUNT_UPGRADE_REWARD_CONFIG_MAX_COUNT];
};

struct RandActivityQibingUpgradeCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), need_qibing_grade(0) {}

		int seq;
		int need_qibing_grade;
		ItemConfigData reward_item;
	};

	RandActivityQibingUpgradeCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[RAND_ACTIVITY_QIBING_UPGRADE_REWARD_CONFIG_MAX_COUNT];
};

struct RandActivityMentalityTotalLevelCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), need_mentality_total_level(0) {}

		int seq;
		int need_mentality_total_level;
		ItemConfigData reward_item;
	};

	RandActivityMentalityTotalLevelCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[RAND_ACTIVITY_MENTALITY_TOTAL_LEVEL_REWARD_CONFIG_MAX_COUNT];
};

struct RandActivityWingUpgradeCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), need_wing_grade(0) {}

		int seq;
		int need_wing_grade;
		ItemConfigData reward_item;
	};

	RandActivityWingUpgradeCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[RAND_ACTIVITY_WING_UPGRADE_REWARD_CONFIG_MAX_COUNT];
};

struct RandActivityKillBossCfg
{
	RandActivityKillBossCfg() : need_kill_boss_count(0) {}

	int need_kill_boss_count;
	ItemConfigData reward_item;
};

struct RandActivityDayOnlineGiftCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), send_reward_hour(0) {}

		int seq;
		int send_reward_hour;
	};

	RandActivityDayOnlineGiftCfg() : cfg_count(0), opengame_day(0) {}

	ItemConfigData reward_item;

	int cfg_count;
	int opengame_day;
	ConfigItem cfg_list[RAND_ACTIVITY_DAY_ONLINE_GIFT_CONFIG_MAX_COUNT];
};

struct RandActivityQuanminQifuCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), need_qifu_times(0) {}

		int seq;
		int need_qifu_times;
		ItemConfigData reward_item;
	};

	RandActivityQuanminQifuCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[RAND_ACTIVITY_VIP_QIFU_REWARD_CONFIG_MAX_COUNT];
};

struct RandActivityXianmengBipinCfg
{
	struct ConfigItem
	{
		ConfigItem() : need_rank(0) {}

		int need_rank;
		ItemConfigData reward_item;
	};

	RandActivityXianmengBipinCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[RAND_ACTIVITY_XIANMENG_BIPIN_REWARD_CONFIG_MAX_COUNT];
};

struct RandActivityXianmengJueQiCfg
{
	struct ConfigItem
	{
		ConfigItem() : need_rank(0) {}

		int need_rank;
		ItemConfigData reward_item;
	};

	RandActivityXianmengJueQiCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[RAND_ACTIVITY_XIANMENG_JUEQI_REWARD_CONFIG_MAX_COUNT];
};

struct RandActivityLoginGiftCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), need_login_days(0) {}

		int seq;
		int need_login_days;
		ItemConfigData common_reward_item;
		ItemConfigData vip_reward_item;
	};

	RandActivityLoginGiftCfg() : need_accumulate_days(0), cfg_count(0), opengame_day(0) {}

	int need_accumulate_days;
	ItemConfigData accumulate_reward;

	int cfg_count;
	int opengame_day;
	ConfigItem cfg_list[RAND_ACTIVITY_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT];
};

struct RandActivityShouYouYuXiangCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), need_flower(0) {}

		int seq;															// 奖励索引
		int need_flower;													// 需要鲜花数	
		ItemConfigData reward_item;											// 奖励物品
	};

	RandActivityShouYouYuXiangCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[RAND_ACTIVITY_SHOUYOU_YUXIANG_REWARD_CONFIG_MAX_COUNT];
};

struct RandActivityRankBipinCfg
{
	RandActivityRankBipinCfg() : activity_type(0), give_reward_day(0), need_capability(0) {}

	int activity_type;
	int give_reward_day;
	int need_capability;
	ItemConfigData reward_item;
};

struct RandActivityAllBipinCfg
{
	struct ConfigItem
	{
		ConfigItem() : activity_type(0), seq(0), need_capability(0), reward_gold_bind(0) {}

		int activity_type;
		int seq;
		int need_capability;
		int reward_gold_bind;
		ItemConfigData reward_item;
	};

	RandActivityAllBipinCfg() : activity_type(0), cfg_count(0) {}

	int activity_type;
	int cfg_count;
	ConfigItem cfg_list[RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT];
};

struct RandActivityChargeRepaymentCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), charge_value(0), bind_gold_repayment(0) {}

		int seq;														// 索引
		int charge_value;												// 单笔充值额度
		int bind_gold_repayment;										// 返利绑定元宝数
	};

	RandActivityChargeRepaymentCfg() : cfg_count(0), opengame_day(0) {}

	int cfg_count;
	int opengame_day;
	ConfigItem cfg_list[RAND_ACTIVITY_CHARGE_REPAYMENT_CONFIG_MAX_COUNT];
};

struct RandActivitySingleChargeCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), charge_value(0) {}

		int seq;														// 索引
		int charge_value;												// 单笔充值额度
		ItemConfigData reward_item;										// 奖励物品
	};

	RandActivitySingleChargeCfg() : cfg_count(0), opengame_day(0) {}

	int cfg_count;
	int opengame_day;
	ConfigItem cfg_list[RAND_ACTIVITY_SINGLE_CHARGE_CONFIG_MAX_COUNT];
};

struct RandActivityCornucopiaCfg
{
	RandActivityCornucopiaCfg() : lun(0), need_total_charge(0), need_put_gold(0), min_reward_gold(0), max_reward_gold(0), is_broadcast(false) {}

	int lun;															// 第几轮
	int need_total_charge;												// 需要累计充值数
	int need_put_gold;													// 需要投入资金
	int min_reward_gold;												// 最低可拿钻石
	int max_reward_gold;												// 最高可拿钻石
	bool is_broadcast;													// 是否传闻
};

struct RandActivityDanbiChongzhiRewardCfg
{
	RandActivityDanbiChongzhiRewardCfg() : activity_day(0), reward_seq(0), need_chongzhi_num(0) {}

	int activity_day;
	int reward_seq;
	int need_chongzhi_num;
	ItemConfigData reward_item;
};

struct RandActivityDanbiChongzhiDayCfg
{
	RandActivityDanbiChongzhiDayCfg() : opengame_day(0), activity_day(0), reward_count(0) {}

	int opengame_day;
	int activity_day;
	int reward_count;
	RandActivityDanbiChongzhiRewardCfg reward_list[RAND_ACTIVITY_DANBI_CHONGZHI_REWARD_CFG_MAX_COUNT_PER_DAY];
};

struct RandActivityDanbiChongzhiDayDiffCfg
{
	RandActivityDanbiChongzhiDayDiffCfg() : opengame_day(0), reward_max_day(0) {}

	int opengame_day;
	int reward_max_day;
	RandActivityDanbiChongzhiDayCfg day_cfglist[RAND_ACTIVITY_DANBI_CHONGZHI_DAY_CFG_MAX_COUNT];
};

struct RandActivityTotalChargeDayCfg
{
	RandActivityTotalChargeDayCfg() : cfg_count(0), opengame_day(0) {}

	struct TotalChargeDayItem
	{
		static const int REWARD_ITEM_MAX_COUT = 3;

		TotalChargeDayItem() : need_recharge_day(0), seq(-1), reward_count(0) {}

		int need_recharge_day;
		int seq;
		int reward_count;
		ItemConfigData reward_item_list[REWARD_ITEM_MAX_COUT];
	};

	int cfg_count;
	int opengame_day;
	TotalChargeDayItem cfg_list[RAND_ACTIVITY_TOTAL_CHARGE_DAY_CFG_MAX_SEQ + 1];
};

struct RandActivityTomorrowRewardCfg
{
	RandActivityTomorrowRewardCfg() : index(0), weight(0), is_broadcast(false) {}

	int index;
	int weight;
	bool is_broadcast;
	ItemConfigData reward_item;
};

struct RandActivityDayChongzhiRankRewardConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 32;

	struct ConfigItem
	{
		ConfigItem() : day(0), limit_rank(0), limit_gold(0), item_count(0) {}

		int day;
		int limit_rank;
		int limit_gold;
		int item_count;
		ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
	};

	RandActivityDayChongzhiRankRewardConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct RandActivityDayChongzhiRankRewardDiffConfig
{
	RandActivityDayChongzhiRankRewardDiffConfig() : opengame_day(0), reward_max_day(0) {}

	int opengame_day;
	int reward_max_day;
	RandActivityDayChongzhiRankRewardConfig rank_reward_cfglist[RAND_ACTIVITY_DAY_CHONGZHI_RANK_REWARD_MAX_DAY + 1];
};

struct RandActivityDayConsumeRankRewardConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 32;

	struct ConfigItem
	{
		ConfigItem() : day(0), limit_rank(0), limit_gold(0), item_count(0), fanli_rate(0) {}

		int day;
		int limit_rank;
		int limit_gold;
		int item_count;
		int fanli_rate;
		ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
	};

	RandActivityDayConsumeRankRewardConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct RandActivityDayConsumeRankRewardDiffConfig
{
	RandActivityDayConsumeRankRewardDiffConfig() : opengame_day(0), reward_max_day(0) {}

	int opengame_day;
	int reward_max_day;
	RandActivityDayConsumeRankRewardConfig rank_reward_cfglist[RAND_ACTIVITY_DAY_CONSUME_RANK_REWARD_MAX_DAY + 1];
};

struct RandActivityTotalChargeRewardConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 32;
	UNSTD_STATIC_CHECK(CONFIG_ITEM_MAX_COUNT <= 32);

	const static int REWARD_ITEM_MAX_COUNT = 4;

	struct ConfigItem
	{
		ConfigItem() : seq(0), need_chongzhi(0), item_count(0) {}

		int seq;
		int need_chongzhi;

		int item_count;
		ItemConfigData item_list[REWARD_ITEM_MAX_COUNT];
	};

	RandActivityTotalChargeRewardConfig() : opengame_day(0), cfg_count(0) {}

	int opengame_day;
	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct RandActivityTimeLimitExchangeConfig
{
	RandActivityTimeLimitExchangeConfig() : seq(0), need_score(0), double_count(0) {}

	int seq;
	int need_score;
	int double_count;

	ItemConfigData item;
};

struct RandActivityLevelLotteryOtherCfg
{
	RandActivityLevelLotteryOtherCfg() :free_times(0), free_interval(0)
	{}

	int free_times;
	int free_interval;
};

struct RandActivityLevelLotteryTotalTimesCfg
{
	RandActivityLevelLotteryTotalTimesCfg(): reward_index(0), total_times(0), vip_level_limit(0)
	{}

	int reward_index;
	int total_times;
	int vip_level_limit;
	ItemConfigData reward[MAX_ATTACHMENT_ITEM_NUM];
};

struct RandActivityAdditionLotteryConfig
{
	RandActivityAdditionLotteryConfig() : cfg_count(0), opengame_day(0) {}

	struct ConfigItem
	{
		ConfigItem() : reward_index(0), level(0), weight(0), to_level(0), is_broadcast(false), is_record_history(false){ }

		int reward_index;														// 奖励索引
		ItemConfigData reward_item;												// 奖励物品
		int level;																// 当前层数
		int weight;																// 抽到该奖励的权重
		int to_level;															// 获取当前奖励后跳到层数
		bool is_broadcast;														// 抽到奖励后是否传闻
		bool is_record_history;													// 抽到奖励后是记录到抽奖历史里		
	};

	int cfg_count;
	int opengame_day;

	std::map<int, int> baodi_map;												// 保底seq对应保底次数 map
	ConfigItem cfg_list[RA_LEVEL_LOTTERY_MAX_REWARD_ID_LIMIT + 1];
};

struct RandActivityNiuEggChouRewardConfig
{
	RandActivityNiuEggChouRewardConfig() : cfg_count(0), opengame_day(0) {}

	struct RANiuEggConfigItem 
	{
		RANiuEggConfigItem() : req(0), weight(0), is_broadcast(false) {}

		int req;
		ItemConfigData reward_item;
		int weight;
		bool is_broadcast;
	};
	
	int cfg_count;
	int opengame_day;

	std::map<int, int> baodi_map;
	RANiuEggConfigItem cfg_list[RA_NIU_EGG_CHOU_REWARD_CFG_COUNT_LIMIT];
};

struct RandActivityNiuEggServerRewardConfig
{
	RandActivityNiuEggServerRewardConfig() : cfg_count(0), opengame_day(0) {}
	
	struct RANiuEggConfigItem
	{
		RANiuEggConfigItem () : req(0), server_niu_times(0), vip_limit(0) {}

		int req;
		ItemConfigData reward_item;
		int server_niu_times;
		int vip_limit;
	};

	int cfg_count;
	int opengame_day;
	RANiuEggConfigItem cfg_list[RA_NIU_EGG_SERVER_REWARD_CFG_COUNT_LIMIT];
};

struct RandActivityZhenBaoGeAddWeightConfig
{
	RandActivityZhenBaoGeAddWeightConfig() : seq(0), add_weight(0) {}

	int seq;
	int add_weight;
};

struct RandActivityZhenBaoGeAddWeightDayCfg
{
	RandActivityZhenBaoGeAddWeightDayCfg() : cfg_count(0), opengame_day(0) {}

	int cfg_count;
	int opengame_day;

	RandActivityZhenBaoGeAddWeightConfig cfg_list[RAND_ACTIVITY_ZHENBAOGE_AUTO_ADD_WEIGHT_CFG_COUNT];
};

struct RandActivityZhenBaoGeConfig
{
	RandActivityZhenBaoGeConfig() : seq(0), is_rare(0), weight(0), auto_add_weight(0), buy_consume_gold(0), opengame_day(0){}

	int seq;
	ItemConfigData reward_item;
	int is_rare;		
	int weight;
	int auto_add_weight;								// 自增权重
	int buy_consume_gold;
	int opengame_day;
};

struct RandActivityZhenBaoGeDayCfg
{
	RandActivityZhenBaoGeDayCfg() : cfg_count(0), opengame_day(0) {}

	int cfg_count;
	int opengame_day;
	std::map<int, int> baodi_map;

	RandActivityZhenBaoGeConfig cfg_list[RAND_ACTIVITY_ZHENBAOGE2_MAX_SEQ + 1];
};

struct RandActivityZhenBaoGeServerConfig
{
	RandActivityZhenBaoGeServerConfig() : seq(0), can_fetch_times(0), vip_limit(0) {}

	int seq;
	ItemConfigData reward_item;
	int can_fetch_times;
	int vip_limit;
};

struct RandActivityZhenBaoGeServerDayCfg
{
	RandActivityZhenBaoGeServerDayCfg() : cfg_count(0), opengame_day(0) {}

	int cfg_count;
	int opengame_day;

	RandActivityZhenBaoGeServerConfig cfg_list[RAND_ACTIVITY_ZHENBAOGE2_MAX_RETURN_REWARD_COUNT + 1];
};

struct RandActivityChargeRewardGoldConfig
{
	RandActivityChargeRewardGoldConfig() : type(0), gold_low_limit(0), gold_high_limit(0), reward_precent(0)
	{
		
	}

	int type;
	int gold_low_limit;
	int gold_high_limit;
	int reward_precent;
};

struct RandActivityMiJingXunBaoConfig
{
	RandActivityMiJingXunBaoConfig() : seq(0), is_rare(false), weight(0), auto_add_weight(0) {}

	int seq;
	ItemConfigData reward_item;
	bool is_rare;
	int weight;
	int auto_add_weight;
};

struct RandActivityMiJingXunBaoCfgList
{
	RandActivityMiJingXunBaoCfgList() : opengame_day(0) {}

	int opengame_day;
	RandActivityMiJingXunBaoConfig cfg_list[RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ + 1];			// 秘境寻宝物品列表

	std::map<int, int> baodi_map;																// 保底seq对应保底次数 map
};

struct RandActivityMiJingXunBaoAddWeightConfig
{
	RandActivityMiJingXunBaoAddWeightConfig() : seq(0), add_weight(0) {}

	int seq;
	int add_weight;
};

struct RandActivityMiJingXunBaoAddWeightCfgList
{
	RandActivityMiJingXunBaoAddWeightCfgList() : opengame_day(0) {}

	int opengame_day;
	RandActivityMiJingXunBaoAddWeightConfig add_weight_cfg_list[RAND_ACTIVITY_MIJINGXUNBAO_AUTO_ADD_WEIGHT_CFG_COUNT];
};

struct RandActivityMiJingXunBaoRewardConfig
{
	RandActivityMiJingXunBaoRewardConfig() : index(0), choujiang_times(0) {}

	int index;
	int choujiang_times;
	ItemConfigData reward_item;
};

struct RandActivityMiJingXunBaoRewardDayCfg 
{
	RandActivityMiJingXunBaoRewardDayCfg() : opengame_day(0), cfg_count(0)
	{}

	int opengame_day;
	int cfg_count;
	RandActivityMiJingXunBaoRewardConfig cfg_list[RAND_ACTIVITY_MIJINGXUNBAO_REWARD_MAX_COUNT];
};

struct RandActivityJiSuChongZhanConfig
{
	RandActivityJiSuChongZhanConfig() : seq(0), charge_value(0)
	{

	}

	int seq;
	int charge_value;
	ItemConfigData reward_item;
};

struct RandActivityJiSuChongZhanCfg
{
	RandActivityJiSuChongZhanCfg() : opengame_day(0)
	{

	}

	RandActivityJiSuChongZhanConfig cfg_list[RAND_ACTIVITY_JISUCHONGZHAN_CFG_NUM];
	int opengame_day;
};

struct RandActivityMoneyTreeServerRewardConfig
{
	RandActivityMoneyTreeServerRewardConfig() : seq(0), server_rock_times(0), vip_limit(0) {}

	int seq;
	ItemConfigData reward_item;
	int server_rock_times;
	int vip_limit;
};

struct RandActivityServerBaodiConfig
{
	RandActivityServerBaodiConfig():seq(0),weight(0),is_rare(0){}

	int seq;
	int weight;
	ItemConfigData reward_item;
	bool is_rare;
};

struct RandActivityServerBaodiCfgList
{
	RandActivityServerBaodiCfgList():opengame_day(0), baodi(0){}

	int opengame_day;
	int baodi;
	std::vector<RandActivityServerBaodiConfig> server_baodi_vec;
};

struct RandActivityMoneyTreeServerRewardCfgList
{
	RandActivityMoneyTreeServerRewardCfgList() : opengame_day(0) {}

	int opengame_day;
	RandActivityMoneyTreeServerRewardConfig server_reward_cfg_list[RA_MONEY_TREE_SERVER_REWARD_CFG_COUNT];  // 摇钱树全服物品列表
};

struct RandActivityMoneyTreeRewardConfig
{
	RandActivityMoneyTreeRewardConfig() : seq(0), weight(0), prize_pool_percent(0), is_broadcast(false) {}

	int seq;
	ItemConfigData reward_item;
	int weight;
	int prize_pool_percent;
	bool is_broadcast;
};

struct RandActivityMoneyTreeRewardCfgList
{
	RandActivityMoneyTreeRewardCfgList() : opengame_day(0) {}

	int opengame_day;
	std::map<int, int> baodi_map;
	RandActivityMoneyTreeRewardConfig reward_cfg_list[RA_MONEY_TREE_REWARD_CFG_COUNT];		// 摇钱树抽奖物品列表
};

// 陛下请翻牌-奖励配置
struct RandActivityKingDrawRewardConfig										// 奖励配置
{
	RandActivityKingDrawRewardConfig() : seq(0), weight(0), auto_add_weight(0), is_rare(false) {}

	ItemConfigData reward_item;												// 奖励物品
	short seq;																// 奖励索引
	short weight;															// 奖励权重
	short auto_add_weight;													// 自增权重
	bool is_rare;															// 是否珍稀
};

// 陛下请翻牌-自增权重配置
struct RandActivitySelfWeightConfig
{
	RandActivitySelfWeightConfig() : seq(0), auto_add_weight(0) {}

	short seq;																// 自增项对应的索引
	short auto_add_weight;													// 自增权重
};

struct RandActivityKingDrawLevelStageConfig
{
	static const int KING_DRAW_REWARD_COUNT_PER_STAGE = 60;						// 一阶段最多的奖励数（配置的）

	RandActivityKingDrawLevelStageConfig() : m_reward_count(0), m_kingdraw_self_weigth_count(0) {}

	int m_reward_count;
	RandActivityKingDrawRewardConfig reward_list[KING_DRAW_REWARD_COUNT_PER_STAGE];

	int m_kingdraw_self_weigth_count;									// 陛下请翻牌，权重自增项的配置数
	RandActivitySelfWeightConfig m_kingdraw_self_weight_cfg_list[RA_KING_DRAW_MAX_SELF_WEIGTH_COUNT];			// 陛下请翻牌，权重自增项的配置信息
};

struct RandActivityKingDrawLevelConfig
{

	RandActivityKingDrawLevelConfig() : opengame_day(0) {}

	int opengame_day;
	std::map<int, int> baodi_map;

	RandActivityKingDrawLevelStageConfig level_cfg;
};

struct RandActivityKingDrawReturnRewardStageCfg
{
	RandActivityKingDrawReturnRewardStageCfg() : draw_times(0){}

	int draw_times;
	ItemConfigData reward_item;
};

struct RandActivityKingDrawReturnRewardCfg
{
	static const int KING_DRAW_RETURN_REWARD_COUNT_PER_STAGE = 10;				// 一阶段最多的奖励数（配置的）

	RandActivityKingDrawReturnRewardCfg() : opengame_day(0), cfg_count(0) {}

	int opengame_day;
	int cfg_count;

	RandActivityKingDrawReturnRewardStageCfg cfg_list[KING_DRAW_RETURN_REWARD_COUNT_PER_STAGE];
};

struct RandActivityTotalCharge3Cfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), need_chongzhi_num(0) {}

		int seq;															// 充值档次，自动生成，不是读配置

		int need_chongzhi_num;
		ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];				// 邮件发放奖励
	};

	static const int MAX_CHONGZHI_CFG_COUNT = 5;				// 同一个分段最大奖励配置数

	RandActivityTotalCharge3Cfg() : start_day(0), end_day(0), chongzhi_list_count(0) {}
	
	int start_day;
	int end_day;

	int chongzhi_list_count;
	ConfigItem chongzhi_list[MAX_CHONGZHI_CFG_COUNT];
};

struct RandActivityTotalCharge3Config
{
	RandActivityTotalCharge3Config() : opengame_day(0), total_cfg_count(0) {}
	
	int opengame_day;
	int total_cfg_count;

	RandActivityTotalCharge3Cfg total_charge_3_cfglist[RAND_ACTIVITY_TOTAL_CHARGE_3_CFG_COUNT];
};

struct RandActivityAnyMonsterDropConfig
{
	RandActivityAnyMonsterDropConfig () : add_attr_item_id(0), add_attr_rate(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0) {}

	ItemID add_attr_item_id;												// 加属性的物品ID		
	int add_attr_rate;														// 使用物品增加的基础属性倍数（以下是基础属性）

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
};

// 累计充值4
struct RandActivityTotalCharge4Cfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), need_chongzhi_num(0) {}

		int seq;															// 充值档次，自动生成，不是读配置

		int need_chongzhi_num;
		ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];				// 邮件发放奖励
	};

	static const int MAX_CHONGZHI_CFG_COUNT = 5;				// 同一个分段最大奖励配置数

	RandActivityTotalCharge4Cfg() : start_day(0), end_day(0), chongzhi_list_count(0) {}

	int start_day;
	int end_day;

	int chongzhi_list_count;
	ConfigItem chongzhi_list[MAX_CHONGZHI_CFG_COUNT];
};

struct RandActivityTotalCharge4Config
{
	RandActivityTotalCharge4Config() : opengame_day(0), total_charge_4_cfglist_count(0) {}

	int opengame_day;
	int total_charge_4_cfglist_count;
	
	RandActivityTotalCharge4Cfg total_charge_4_cfglist[RAND_ACTIVITY_TOTAL_CHARGE_4_CFG_COUNT];
};

// 挖矿-矿石信息配置
struct RandActivityMineInfoStageConfig
{
	RandActivityMineInfoStageConfig() : seq(0), need_gold(0), exchange_need_num(0), show_weight(0), show_need_refresh_times(0),
		show_certain_refresh_beishu(0), opengame_day(0), is_broadcast(false) {}

	int seq;

	int need_gold;							// 挖矿所需元宝
	int exchange_need_num;					// 兑换所需矿石数
	ItemConfigData exchange_item;			// 兑换的奖励
	int show_weight;						// 刷出的权重
	int show_need_refresh_times;			// 至少所需刷矿次数
	int show_certain_refresh_beishu;		// 必出刷矿倍数
	int opengame_day;
	bool is_broadcast;
};

// 挖矿-矿石信息配置
struct RandActivityMineInfoConfig
{
	static const int RA_MINE_INFO_STAGE_LIST_MAX_LEN = 12;
	RandActivityMineInfoConfig() : opengame_day(0), stage_list_count(0) {}

	int opengame_day;
	int stage_list_count;
	std::map<int, int> baodi_map;

	RandActivityMineInfoStageConfig stage_list[RA_MINE_INFO_STAGE_LIST_MAX_LEN];
};


// 挖矿-全服礼包配置
struct RandActivityMineServerRewardStageConfig
{
	RandActivityMineServerRewardStageConfig() : seq(0), total_refresh_times(0), vip_limit(0) {}

	int seq;

	int total_refresh_times;				// 所需全服刷矿次数
	ItemConfigData reward_item;
	int vip_limit;
};

// 挖矿-全服礼包配置
struct RandActivityMineServerRewardConfig
{
	static const int RA_MINE_REWARD_STAGE_LIST_MAX_LEN = 10;

	RandActivityMineServerRewardConfig() : opengame_day(0), stage_list_count(0) {}

	int opengame_day;
	int stage_list_count;
	RandActivityMineServerRewardStageConfig stage_list[RA_MINE_REWARD_STAGE_LIST_MAX_LEN];
};

// 顶刮刮-奖励配置
struct RandActivityGuaGuaConfig	
{
	RandActivityGuaGuaConfig() : opengame_day(0), seq(0), weight(0), baodi(0) {}

	bool operator<(const RandActivityGuaGuaConfig &rhs) const
	{
		return opengame_day < rhs.opengame_day;
	}

	int opengame_day;
	int seq;						// 奖励索引
	int weight;
	int is_special;					// 是否特别奖								
	ItemConfigData reward_item;		// 奖励物品列表

	int baodi;
};

// 顶刮刮 - 累计配置
struct RandActivityGuaGuaAccConfig
{
	RandActivityGuaGuaAccConfig() : index(0), acc_count(0) {}

	int opengame_day;															// 开服天数	
	int index;																	// 奖励索引
	int acc_count;																// 累加次数
	ItemConfigData reward_item;													// 奖励物品
};

// 天命卜卦-奖池配置
struct RandActivityTianMingDivinationRewardConfig
{
	RandActivityTianMingDivinationRewardConfig() : seq(0), weight(0), opengame_day(0), can_add_lot(false), is_broadcast(false){}

	int seq;
	ItemConfigData reward_item;
	int weight;
	int opengame_day;
	bool can_add_lot;
	bool is_broadcast;

};

struct RandActivityTianMingDivinationBaodiConfig
{
	RandActivityTianMingDivinationBaodiConfig() : opengame_day(0) {}

	int opengame_day;
	std::map<int, int> baodi_map;
};

// 天命卜卦-元宝消耗
struct RandActivityTianMingDivinationGoldConsumeConfig
{
	RandActivityTianMingDivinationGoldConsumeConfig() : lot_add_times(-1), add_consume_gold(-1), chou_consume_gold(0) {}

	int lot_add_times;
	int add_consume_gold;
	int chou_consume_gold;

};

// 翻翻转-物品信息
struct RandActivityFanFanItemInfo
{
	RandActivityFanFanItemInfo() : seq(0), weight(0), is_broadcast(false) {}

	int seq;
	ItemConfigData reward_item;
	int weight;
	bool is_broadcast;
};

struct RandActivityFanFanItemDayCfg
{
	RandActivityFanFanItemDayCfg() : opengame_day(0), cfg_count(0)
	{}

	int opengame_day;
	int cfg_count;

	RandActivityFanFanItemInfo cfg_list[RA_FANFAN_MAX_ITEM_COUNT];
};

// 翻翻转-字组信息
struct RandActivityFanFanWordInfo
{
	RandActivityFanFanWordInfo() : seq(0), weight(0), floor_refresh_times(0) {}

	int seq;
	int weight;
	int floor_refresh_times;			// 保底刷新次数
	ItemConfigData exchange_item;
};

struct RandActivityFanFanWordDayCfg
{
	RandActivityFanFanWordDayCfg() : opengame_day(0), cfg_count(0)
	{}

	int opengame_day;
	int cfg_count;
	std::map<int, int> baodi_map;

	RandActivityFanFanWordInfo cfg_list[RA_FANFAN_MAX_WORD_COUNT];
};

//翻翻转累抽奖励
struct RandActivityFanFanBaodiConfig
{
	RandActivityFanFanBaodiConfig() : index(-1), choujiang_times(0) {}

	int index;
	int choujiang_times;
	ItemConfigData reward_item;
};

struct RandActivityFanFanBaodiDayConfig
{
	RandActivityFanFanBaodiDayConfig() : opengame_day(0), cfg_count(0)
	{}

	int opengame_day;
	int cfg_count;

	RandActivityFanFanBaodiConfig cfg_list[RA_FANFAN_BAODI_REWARD_CFG_COUNT];
};

// 连续充值、连充特惠初、连充特惠高-奖励配置
struct RandActivityContinueChongzhiRewardConfig
{
	RandActivityContinueChongzhiRewardConfig() : seq(0), open_server_day(0), need_chongzhi(0), lei_chong_day(0), fetch_extra_reward_need_days(0){}

	int seq;
	int open_server_day;
	int need_chongzhi;
	int lei_chong_day;
	int fetch_extra_reward_need_days;
	ItemConfigData reward_item;
	ItemConfigData extra_reward;
};

// 连续消费-其他配置
struct RandActivityContinueConsumeOtherDayCfg
{
	RandActivityContinueConsumeOtherDayCfg() : opengame_day(0), cfg_count(0), continue_consume_fetch_extra_reward_need_days(0){}

	int opengame_day;
	int cfg_count;

	int continue_consume_fetch_extra_reward_need_days;
	ItemConfigData continue_consume_extra_reward;
};

// 连续消费-奖励配置
struct RandActivityContinueConsumeRewardConfig
{
	RandActivityContinueConsumeRewardConfig() : need_consume_gold(0) {}

	unsigned int need_consume_gold;
	ItemConfigData reward_item;
};

struct RandActivityContinueConsumeRewardDayCfg
{
	RandActivityContinueConsumeRewardDayCfg() : opengame_day(0), cfg_count(0) {}

	int opengame_day;
	int cfg_count;

	RandActivityContinueConsumeRewardConfig cfg_list[RA_CONTINUE_CONSUME_MAX_DAY_INDEX + 1];
};

// 军歌嘹亮--活跃度兑换配置
struct RandActivityArmyDayExchangeFlagConfig 
{
	RandActivityArmyDayExchangeFlagConfig() : seq(0), need_active_degree(0), reward_flag_num(0)
	{

	}

	int seq;
	int need_active_degree;
	int reward_flag_num;
};

//军歌嘹亮--旗帜兑换配置
struct RandActivityArmyDayExchangeItemConfig 
{
	RandActivityArmyDayExchangeItemConfig() : side_type(INVALID_ARMY_SIDE), need_flags(0)
	{

	}

	int side_type;
	int need_flags;
	ItemConfigData reward_item;
};

// 循环充值-奖励配置
struct RandActivityCirculationChongzhiStageRewardConfig
{
	RandActivityCirculationChongzhiStageRewardConfig() : seq(0), need_chongzhi_gold(0) {}

	int seq;
	unsigned int need_chongzhi_gold;
	ItemConfigData reward_item;
};

// 循环充值-奖励配置
struct RandActivityCirculationChongzhiRewardConfig
{
	RandActivityCirculationChongzhiRewardConfig() : cfg_count(0), opengame_day(0) {}

	int cfg_count;
	int opengame_day;

	RandActivityCirculationChongzhiStageRewardConfig cfg_list[RA_CIRCULATION_CHONGZHI_MAX_REWARD_COUNT];
};

// 至尊幸运抽奖消耗
struct RandActivityExtremeLuckyDrawConsumeConfig
{
	RandActivityExtremeLuckyDrawConsumeConfig() : draw_times(0), consume_gold(0)
	{
	
	}

	int draw_times;
	int consume_gold;
};

// 至尊幸运抽奖—奖励配置
struct RandActivityExtremeLuckyDrawRewardConfig
{
	RandActivityExtremeLuckyDrawRewardConfig() : seq(0), min_flush_times(0), weight(0), is_rare(false)
	{
		
	}

	int seq;
	int min_flush_times;
	int weight;
	bool is_rare;
	ItemConfigData reward_item;
};

struct RandActivityExtremeLuckyRewardCfg
{
	RandActivityExtremeLuckyRewardCfg() : opengame_day(0)
	{
		
	}

	int opengame_day;
	std::map<int, int> baodi_map;
	RandActivityExtremeLuckyDrawRewardConfig reward_list[RA_EXTREME_LUCKY_DRAW_REWARD_CFG_COUNT];
};

struct RandActivityExtremeLuckyReturnRewardStageCfg
{
	RandActivityExtremeLuckyReturnRewardStageCfg() : draw_times(0)
	{}

	int draw_times;
	ItemConfigData reward_item;
};

struct RandActivityExtremeLuckyReturnRewardCfg
{
	RandActivityExtremeLuckyReturnRewardCfg() : opengame_day(0), cfg_count(0)
	{}

	int opengame_day;
	int cfg_count;

	RandActivityExtremeLuckyReturnRewardStageCfg stage_cfg_list[RA_EXTREME_LUCKY_RETURN_REWARD_ONCE_COUNT];
};

struct RandActivityNationalDayConfig
{
	RandActivityNationalDayConfig() : seq(0), consume_count(0)
	{
		
	}

	int seq;
	int consume_count;

	ItemConfigData consume_item_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	ItemConfigData reward_item;
};

// 步步高升配置
struct RandActivityPromotingPositionConfig
{
	RandActivityPromotingPositionConfig() : seq(0), reward_type(-1), reward_gold_rate(0), free_play_weight(0), gold_play_weight(0), extra_time_play_weight(0), item_play_weight(0),
		opengame_day(0), is_rare(false), is_broadcast(false) {}

	int seq;
	int reward_type;
	ItemConfigData reward_item;
	int reward_gold_rate;
	int free_play_weight;
	int gold_play_weight;
	int extra_time_play_weight;
	int item_play_weight;
	int opengame_day;
	bool is_rare;
	bool is_broadcast;
};

struct RandActivityPromotingPositionRewardCfg
{
	RandActivityPromotingPositionRewardCfg() : play_times(0), vip_limit(0), opengame_day(0)
	{}

	int play_times;
	int vip_limit;
	int opengame_day;

	ItemConfigData reward_item;
};

struct RandActivityPromotingPositionBaodiCfg
{
	int opengame_day;
	std::map<int, int> baodi_map;
};

// 黑市拍卖配置
struct RandActivityBlackMarketItemCfg
{
	struct ItemConfig
	{
		ItemConfig() : seq(0), init_gold(0), min_add_gold(0) {}

		int seq;
		ItemConfigData item;
		int init_gold;
		int min_add_gold;
	};

	RandActivityBlackMarketItemCfg() : day(0), max_item_count(0) {}

	int day;

	int max_item_count;
	ItemConfig item_cfg_list[RA_BLACK_MARKET_MAX_ITEM_COUNT];
};

struct RandActivityBlackMarketConfig
{
	RandActivityBlackMarketConfig () : opengame_day(0), black_market_max_day_count(0)
	{
		
	}

	int opengame_day;
	int black_market_max_day_count;

	RandActivityBlackMarketItemCfg  black_market_cfg_list[RA_BLACK_MARKET_MAX_DAY_COUNT];	
};

struct RandActivityTreasuresMallBuyConfig
{
	RandActivityTreasuresMallBuyConfig() : opengame_day(0), item_cfg_count(0)
	{

	}

	struct BuyItemConfig
	{
		
		BuyItemConfig() : index(0), cost_gold(0), get_score(0)
		{
			
		}

		int index;
		int cost_gold;
		int get_score;
		ItemConfigData reward_item;
	};
	
	int opengame_day;
	int item_cfg_count;

	BuyItemConfig item_list[RA_TREASURES_MALL_MAX_BUY_ITEM_COUNT];
};

struct RandActivityTreasuresMallExchangeConfig
{
	RandActivityTreasuresMallExchangeConfig() : opengame_day(0), exchange_cfg_count(0)
	{
		
	}

	struct ExchangeItemConfig
	{
		ExchangeItemConfig() : index(0), need_score(0)
		{
			
		}

		int index;
		int need_score;
		ItemConfigData reward_item;
	};

	int opengame_day;
	int exchange_cfg_count;

	ExchangeItemConfig exchange_item_cfg_list[RA_TREASURES_MALL_MAX_EXCHANGE_ITEM_COUNT];
};

struct RandActivityOpenServerRewardItemConfig
{
	RandActivityOpenServerRewardItemConfig() : seq(0), cond1(0), cond2(0), cond3(0),reward_item_count(0) {}
	int seq;
	int cond1;
	int cond2;
	int cond3;
	int reward_item_count;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct RandActivityOpenServerRewardConfig
{
	RandActivityOpenServerRewardConfig() : rand_activity_type(0) {}

	struct RewardDayConfig
	{
		RewardDayConfig() : opengame_day(0) {}

		int opengame_day;
		RandActivityOpenServerRewardItemConfig item_list[RAND_ACTIVITY_OPEN_SERVER_REWARD_COUNT_MAX];
	};

	int rand_activity_type;
	RewardDayConfig reward_day_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
};

struct RandActivityNewCornucopiaConfig
{
	RandActivityNewCornucopiaConfig() {}

	struct ConfigItem
	{
		ConfigItem() : task_value(0), add_percent(0) {}
		short task_value;
		short add_percent;
	};

	ConfigItem item_list[RA_NEW_CORNUCOPIA_TASK_TYPE_MAX];
};

struct RandActivityNewCornucopiaTotalConfig
{
	RandActivityNewCornucopiaTotalConfig() : seq(0), cornucopia_value(0) {}

	int seq;
	int cornucopia_value;
	ItemConfigData reward_item;
};

// 集字活动
struct RandActivityItemCollectionRewardCfg
{
	static const int MAX_COLLECTION_STUFF_COUNT = 4;

	struct ConfigItem
	{
		ConfigItem() : seq(0), stuff_count(0), exchange_times_limit(0) {}

		int seq;

		int stuff_count;
		ItemConfigData stuff_id_list[MAX_COLLECTION_STUFF_COUNT];

		ItemConfigData reward_item;

		int exchange_times_limit;
	};

	RandActivityItemCollectionRewardCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[RAND_ACTIVITY_ITEM_COLLECTION_REWARD_MAX_COUNT];
};

// 经验炼制
struct RandActivityExpRefineRewardCfg
{
	RandActivityExpRefineRewardCfg() : seq(0), consume_gold(0), reward_exp(0) {}

	int seq;
	int consume_gold;
	Attribute reward_exp;
};

struct RandActivityExpRefineRewardCfg_2
{
	RandActivityExpRefineRewardCfg_2() : opengameday(0) {}

	int opengameday;
	RandActivityExpRefineRewardCfg m_exp_refine_cfg_list[RAND_ACTIVITY_EXP_REFINE_REWARD_MAX_COUNT];
};

// 红包好礼
struct RandActivityRedEnvelopeGiftCfg
{
	RandActivityRedEnvelopeGiftCfg() : percent(0){}
	int percent;
};

// 我们结婚吧
struct RandActivityMarryMeCfg
{
	RandActivityMarryMeCfg() : reward_couple_count(0) {}

	int reward_couple_count;
	ItemConfigData reward_item;
};


// 金猪召唤
struct RandActivityGoldenPigOtherCfg
{
	RandActivityGoldenPigOtherCfg() : gold_consume(0), scene_id(0), per_hp_reward(0)
	{}

	enum GOLDEN_PIG_SUMMON_TYPE
	{
		GOLDEN_PIG_SUMMON_TYPE_JUNIOR = 0,
		GOLDEN_PIG_SUMMON_TYPE_MEDIUM,
		GOLDEN_PIG_SUMMON_TYPE_SENIOR,

		GOLDEN_PIG_SUMMON_TYPE_MAX_COUNT,
	};

	struct RandActivityGoldenPigSummonCfg
	{
		RandActivityGoldenPigSummonCfg() : summon_consume(0), summon_pos(0, 0)
		{}

		int summon_consume;
		Posi summon_pos;
	};

	int gold_consume;
	int scene_id;
	int per_hp_reward;

	RandActivityGoldenPigSummonCfg summon_cfg_list[GOLDEN_PIG_SUMMON_TYPE_MAX_COUNT];
};

//开服投资
struct RandActivityOpenServerInvestOtherCfg
{
	RandActivityOpenServerInvestOtherCfg() : consume(0), invest_limit_time(0), active_reward_limit(0)
	{}

	enum INVEST_TYPE
	{
		INVEST_TYPE_BOSS = 0,
		INVEST_TYPE_ACTIVE,
		INVEST_TPYE_BIPIN,

		MAX_INVEST_TYPE,
	};

	int consume;
	int invest_limit_time;
	int active_reward_limit;

	ItemConfigData reward_item;
};

struct RandActivityOpenServerInvestRewardCfg
{
	RandActivityOpenServerInvestRewardCfg() : param(0)
	{}

	int param;

	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct RandActivityGoldenPigRewardCfg
{
	RandActivityGoldenPigRewardCfg() : opengame_day(0), monster_id(0), joiner_reward_flag(0)
	{}

	enum GOLDEN_PIG_REWARD_TYPE
	{
		GOLDEN_PIG_REWARD_TYPE_SUMMON = 0,
		GOLDEN_PIG_REWARD_TYPE_JOINER,
		GOLDEN_PIG_REWARD_TYPE_KILLER,

		GOLDEN_PIG_REWARD_TYPE_MAX_COUNT,
	};

	int opengame_day;
	int	monster_id;
	int joiner_reward_flag;
		
	ItemConfigData reward_list[GOLDEN_PIG_REWARD_TYPE_MAX_COUNT];
};

// 单返豪礼
struct RandActivitySingleChongZhiRewardCfg
{
	struct ItemConfig
	{
		ItemConfig() : seq(0), need_gold(0), reward_gold(0) {}

		int seq;
		int need_gold;
		int reward_gold;
		ItemConfigData reward_item;
	};

	RandActivitySingleChongZhiRewardCfg() : seq(0)
	{
	}

	int seq;
	ItemConfig item_cfg_list[RAND_ACTIVITY_SINGLE_CHONGZHI_MAX_CFG];
};

// 聚划算
struct RandActivityXianyuanTreasConfig
{
	RandActivityXianyuanTreasConfig() : seq(0), max_reward_day(0), consume_gold(0) {}

	int seq;
	ItemConfigData reward_item;
	int max_reward_day;
	int consume_gold;
};

// 限时秒杀-抢购阶段配置
struct RandActivityRushBuyingPhaseConfig
{
	struct SaleItemConfig
	{
		SaleItemConfig() : index(0), sale_price(0), original_price(0), server_buy_times_limit(0), role_buy_times_limit(0) {}

		int index;
		ItemConfigData sale_item;
		int sale_price;
		int original_price;
		int server_buy_times_limit;
		int role_buy_times_limit;
	};

	RandActivityRushBuyingPhaseConfig() : seq(0), buying_begin_time(0), sale_items_count(0) {}

	int seq;
	int buying_begin_time;		// 抢购开始时间（小时）

	int sale_items_count;		// 此阶段可抢购物品数
	SaleItemConfig sale_item_cfg_list[RA_RUSH_BUYING_SALES_ITEMS_COUNT];
};

struct RandActivityRushBuyingPhaseDayConfig
{
	RandActivityRushBuyingPhaseDayConfig() : opengame_day(0), cfg_count(0) {}

	int opengame_day;
	int cfg_count;
	RandActivityRushBuyingPhaseConfig cfg_list[RA_RUSH_BUYING_PHASES_COUNT];
};

// 地图寻宝
struct RandActivityMapHuntCityConfig
{
	RandActivityMapHuntCityConfig() : opengame_day(0), city_count(0)
	{

	}

	struct CityCfg
	{
		CityCfg() : city_id(0), weight(0), is_broadcast(false) {}

		int city_id;
		int weight;
		ItemConfigData base_reward_item;
		bool is_broadcast;
	};


	int opengame_day;
	int city_count;
	std::map<int, int> baodi_map;

	CityCfg city_cfg_list[MAX_RA_MAP_HUNT_CITY_COUNT];
};

struct RandActivityMapHuntRouteConfig
{
	RandActivityMapHuntRouteConfig() : opengame_day(0), route_count(0)
	{

	}

	struct RouteCfg
	{
		RouteCfg() : route_index(0)
		{
			memset(city_list, 0, sizeof(city_list));
		}

		int route_index;
		int city_list[MAX_RA_MAP_HUNT_CITY_COUNT_OF_ROUTE];
		ItemConfigData reward_item;
	};

	int opengame_day;
	int route_count;

	RouteCfg route_cfg_list[MAX_RA_MAP_HUNT_ROUTE_COUNT];
};

struct RandActivityMapHuntServerRewardConfig
{
	RandActivityMapHuntServerRewardConfig() : opengame_day(0), server_reward_cout(0)
	{

	}

	struct RewardCfg
	{
		RewardCfg() : index(0), need_flush_count(0) {}

		int index;
		int need_flush_count;
		ItemConfigData reward_item;
	};

	int opengame_day;
	int server_reward_cout;

	RewardCfg server_reward_cfg_list[MAX_RA_MAP_HUNT_SERVER_REWARD_COUNT];
};
struct RandActivityLightTowerExploreCfg
{
	RandActivityLightTowerExploreCfg() : layer(0), layer_total_weight(0)
	{

	}

	struct LayerConfig
	{
		LayerConfig() : index(0), reward_type(LIGHT_TOWER_EXPLORE_REWAED_TYPE_INVALID), goto_layer(0), is_broadcast(false)
		{

		}

		int index;
		int reward_type;
		int weight;
		ItemConfigData reward_item;
		int goto_layer;
		bool is_broadcast;
	};

	int layer;
	int layer_total_weight;

	LayerConfig reward_list[RA_LIGHT_TOWER_EXPLORE_MAX_PER_LAYER_REWAED_COUNT];
};

struct RandActivityLightTowerExploreConfig
{
	RandActivityLightTowerExploreConfig() : opengame_day(0)
	{

	}

	int opengame_day;

	RandActivityLightTowerExploreCfg layer_reward_list[RA_LIGHT_TOWER_EXPLORE_MAX_LAYER];
};

struct RandActivityLightTowerExploreExternCfg
{
	RandActivityLightTowerExploreExternCfg() : layer(0)
	{

	}

	int layer;
	ItemConfigData reward_item;
};

//灯塔寻宝
struct RandActivityLightTowerExploreExternConfig
{
	RandActivityLightTowerExploreExternConfig() : opengame_day(0)
	{

	}

	int opengame_day;

	RandActivityLightTowerExploreExternCfg layer_extern_reward_list[RA_LIGHT_TOWER_EXPLORE_MAX_LAYER];
};

struct RandActivityLightTowerExploreReturnCfg
{
	RandActivityLightTowerExploreReturnCfg() : index(0), need_count(0)
	{

	}

	int index;
	int need_count;
	ItemConfigData reward_item;
};

struct RandActivityLightTowerExploreServerConfig
{
	RandActivityLightTowerExploreServerConfig() : opengame_day(0)
	{

	}

	int opengame_day;

	RandActivityLightTowerExploreReturnCfg server_reward_list[RA_LIGHT_TOWER_EXPLORE_MAX_SERVER_REWARD_COUNT];
};

struct RandActivityNewTotalChargeRewardConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 32;
	UNSTD_STATIC_CHECK(CONFIG_ITEM_MAX_COUNT <= 32);

	const static int REWARD_ITEM_MAX_COUNT = 4;

	struct ConfigItem
	{
		ConfigItem() : seq(0), need_chongzhi(0), item_count(0) {}

		int seq;
		int need_chongzhi;

		int item_count;
		ItemConfigData item_list[REWARD_ITEM_MAX_COUNT];
	};

	RandActivityNewTotalChargeRewardConfig() : opengame_day(0), cfg_count(0) {}

	int opengame_day;
	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct RandActivityMagicShopConfig
{
	enum RA_MAGIC_SHOP_BUY_TYPE
	{
		RA_MAGIC_SHOP_BUY_TYPE_BUY,
		RA_MAGIC_SHOP_BUY_TYPE_BUY3,	// 一键购买3个

		RA_MAGIC_SHOP_BUY_TYPE_MAX,
	};

	RandActivityMagicShopConfig() : opengame_day(0), activity_day(0), index(0), need_gold(0) {}

	int opengame_day;
	int activity_day;
	int index;
	int need_gold;
	ItemConfigData reward_item;
};

struct RandActivityPlantingTreeRankRewardConfig
{
	RandActivityPlantingTreeRankRewardConfig() : rank(0) {}

	int rank;
	ItemConfigData planting_rank_reward[MAX_ATTACHMENT_ITEM_NUM];
	ItemConfigData watering_rank_reward[MAX_ATTACHMENT_ITEM_NUM];
};

struct RandActivityEverydayNiceGiftCfg
{
	RandActivityEverydayNiceGiftCfg() : opengame_day(0) {}

	struct ConfigItem
	{
		ConfigItem() : index(0), recharge_gold(0), reward_count(0) {}

		int index;
		int recharge_gold;
		int reward_count;
		ItemConfigData reward_item_list[RAND_ACTIVITY_EVERYDAY_NICE_GIFT_MAX_REWARD_COUNT];
	};

	int opengame_day;
	ConfigItem everyday_reward;		// 只有一个档位  
};

struct RandActivityFishingInfoCfg
{
	RandActivityFishingInfoCfg() : fish_type(0), fish_score(0), be_steal_fish_score(0), fish_num(0), is_broadcast(0), fish_weight(0)
	{

	}

	short fish_type;
	int fish_score;
	int be_steal_fish_score;
	short fish_num;
	short is_broadcast;
	ItemConfigData reward_item;
	short fish_weight;
};

struct RandActivityFishingRankCfg
{
	RandActivityFishingRankCfg() : rank_num(0)
	{

	}

	short rank_num;
	ItemConfigData reward_item;
	ItemConfigData be_stealed_reward_item;
};

struct RandActivityStealCrtstalDalaranRewardCfg
{
	RandActivityStealCrtstalDalaranRewardCfg() : rank_num(0)
	{

	}

	short rank_num;
	ItemConfigData reward_item;
	ItemConfigData be_stealed_reward_item;
};

struct RandActivityChongzhiCrazyRebateConfig
{
	RandActivityChongzhiCrazyRebateConfig() : gold_low_limit(0), gold_high_limit(0), reward_precent(0)
	{

	}

	int gold_low_limit;
	int gold_high_limit;
	int reward_precent;
};

struct JinjieThemeActConfig
{
	int act_id;
	int opengame_day;
	int act_theme;
};

struct ConsumGiftRewardConfig
{
	ConsumGiftRewardConfig() : opengame_day(0), act_theme(0), need_gold(0), item_count(0){}

	int opengame_day;
	int act_theme;
	int seq;
	int need_gold;
	int item_count;
	ItemConfigData reward_item_list[RAND_ACTIVITY_CONSUM_GIFT_MAX_REWARD_COUNT];
};

struct ConsumGiftRollRewardPool
{
	ConsumGiftRollRewardPool() : opengame_day(0), act_theme(0), seq(0), num_rate(0), num_type(0) {}

	int opengame_day;
	int act_theme;
	int seq;
	int num_rate;
	int num_type;
	ItemConfigData roll_reward_item;
};

struct ConsumGiftRollRewardConfig
{
	ConsumGiftRollRewardConfig() : num_type(0), decade_num(0), decade_rate(0), 
		units_digit_num(0), units_digit_rate(0){}

	int num_type;
	int decade_num;
	int decade_rate;
	int units_digit_num;
	int units_digit_rate;
};

struct DailyLimitBuyConfig
{
	DailyLimitBuyConfig() : act_type(0), seq(0), gold_price(0), limit_buy_count(0) {}

	int act_type;
	int seq;
	int gold_price;
	int limit_buy_count;
	ItemConfigData reward_item;
};

struct CollectTreasureConfig
{
	CollectTreasureConfig() : seq(0), units_digit(0), units_rate(0) {}

	int seq;
	int units_digit;
	int units_rate;
};

struct HappyCumulChongzhiConfig
{
	HappyCumulChongzhiConfig() : act_type(0), seq(0), need_chongzhi(0), reward_count(0) {}

	int act_type;
	int seq;
	int need_chongzhi;
	int reward_count;
	ItemConfigData reward_item_list[RAND_ACTIVITY_HAPPY_CUMUL_CHONGZHI_MAX_REWARD_COUNT];
};

struct CriticalStrikeDayConfig
{
	

	CriticalStrikeDayConfig() : act_type(0), baoji_rate(0), baoji_mul(0) {}
	int act_type;
	int baoji_rate;
	int baoji_mul;
};

struct JinJieReturnConfig
{
	JinJieReturnConfig() : act_type(0), seq(0), need_grade(0) {}

	int act_type;
	int seq;
	int need_grade;
	ItemConfigData reward_item;
};

// 消费换大礼-兑换物品配置
struct RandActivityConsumeForGiftItemConfig
{
	RandActivityConsumeForGiftItemConfig() : opengame_day(0), point_per_gold(0), seq(0), need_points(0), double_points_need_ex_times(0) {}

	int opengame_day;
	int point_per_gold;
	int seq;
	ItemConfigData exchange_item;
	int need_points;
	int double_points_need_ex_times;
};

struct LimitTimeRebateItem
{
	LimitTimeRebateItem() : chongzhi_cout(0), need_day(0) {}

	int chongzhi_cout;
	int need_day;
	ItemConfigData reward_item;
};

struct RandActivityLimitTimeRebateCfg
{
	RandActivityLimitTimeRebateCfg() : opengame_day(0), cfg_count(0) { }

	int opengame_day;
	int cfg_count;
	LimitTimeRebateItem cfg_list[RA_LIMIT_TIME_REBATE_MAX_CFG_COUNT];
};

struct RandActivityLimitTimeGiftCfg
{
	struct SubItemCfg
	{
		SubItemCfg() : seq(0), limit_time(0), limit_vip_level(0), charge_value1(0), charge_value2(0), charge_value3(0), gift_value1(0), gift_value2(0), gift_value3(0), item_count1(0), item_count2(0),
			item_count3(0),limit_charge_min(0), limit_charge_max(0) {}

		int seq;
		int limit_time;
		int limit_vip_level;
		int charge_value1;
		int charge_value2;
		int charge_value3;
		int gift_value1;
		int gift_value2;
		int gift_value3;
		int item_count1;
		int item_count2;
		int item_count3;
		int limit_charge_min;
		int limit_charge_max;

		ItemConfigData reward_item_list1[MAX_ATTACHMENT_ITEM_NUM];
		ItemConfigData reward_item_list2[MAX_ATTACHMENT_ITEM_NUM];
		ItemConfigData reward_item_list3[MAX_ATTACHMENT_ITEM_NUM];
	};

	RandActivityLimitTimeGiftCfg() : opengame_day(0), sub_item_cfg_count(0) {}

	int opengame_day;
	int sub_item_cfg_count;
	SubItemCfg sub_item_cfg_list[RAND_ACTIVITY_LIMIT_TIME_GIFT_MAX_SEQ + 1];
};

struct RandActivityCirculationChongzhiSecondRewardCfg
{
	RandActivityCirculationChongzhiSecondRewardCfg() : level(0)
	{}

	int level;

	ItemConfigData reward_item;
};

// 随机活动 - 疯狂摇钱树
struct RandActivityShakeMoneyConfig
{
	RandActivityShakeMoneyConfig() : seq(0), section_start(0), section_end(0), chongzhi_return(0), return_max(0)
	{
	}

	int seq;
	int section_start;			// 开启时间
	int section_end;			// 结束时间
	int chongzhi_return;		//充值返比
	int return_max;				//返比上限
};

struct RandActivityLimitTimeLuxuryGiftBagCfg
{
	struct SubItemCfg
	{
		SubItemCfg() : seq(0), limit_time(0), limit_vip_level(0), gift_value(0), item_count(0), need_gold(0) {}

		int seq;
		int limit_time;
		int limit_vip_level;
		int gift_value;
		int item_count;
		int need_gold;
		ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
	};

	RandActivityLimitTimeLuxuryGiftBagCfg() : opengame_day(0), sub_item_cfg_count(0) {}

	int opengame_day;
	int sub_item_cfg_count;
	SubItemCfg sub_item_cfg_list[RAND_ACTIVITY_LIMIT_TIME_GIFT_MAX_SEQ + 1];
};

struct RandActivityConsumeRewardStageCfg
{
	RandActivityConsumeRewardStageCfg() : activity_day(0), limit_vip_level(0), consume_gold(0)
	{}

	bool operator<(const RandActivityConsumeRewardStageCfg &rhs) const
	{
		return activity_day < rhs.activity_day;
	}

	int activity_day;
	int limit_vip_level;
	int consume_gold;

	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct RandActivityRmbBuyChestShopOpenDayCfg
{
	struct RmbRewardCfg
	{
		RmbRewardCfg() : index(0), count_limit(0), rmb_num(0)
		{
		}

		short index;
		short count_limit;
		int rmb_num;
		ItemConfigData reward_item;
	};

	RandActivityRmbBuyChestShopOpenDayCfg() : opengame_day(0){}

	int opengame_day;
	std::vector<RmbRewardCfg> reward_list;
};

struct RandActivityConsumeRewardCfg
{
	RandActivityConsumeRewardCfg() : openserver_day(0)
	{}

	int openserver_day;
	std::vector<RandActivityConsumeRewardStageCfg> stage_cfg_vec;
};

// 集月饼活动
struct RandActivityItemCollectionSecondRewardCfg
{
	static const int MAX_COLLECTION_STUFF_COUNT = 4;

	struct ConfigItem
	{
		ConfigItem() : seq(0), stuff_count(0), exchange_times_limit(0) {}

		int seq;

		int stuff_count;
		ItemConfigData stuff_id_list[MAX_COLLECTION_STUFF_COUNT];

		ItemConfigData reward_item;

		int exchange_times_limit;
	};

	RandActivityItemCollectionSecondRewardCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[RAND_ACTIVITY_ITEM_COLLECTION_SECOND_REWARD_MAX_COUNT];
};

// 线下单笔充值
struct RandActivityOfflineSingleChargeCfg
{
	enum REWARD_TYPE
	{
		REWARD_TYPE_ONCE_GIFT,						// 充值一次就有一个奖励
		REWARD_TYPE_MAX_GIFT,						// 只拿最高充值档位的奖励
		REWARD_TYPE_MAX,
	};

	struct ConfigItem
	{
		ConfigItem() : reward_type(0), seq(0), charge_value(0), reward_limit(0) {}

		int reward_type;
		int seq;														// 索引
		int charge_value;												// 单笔充值额度
		int reward_limit;												// 奖励次数
		ItemConfigData reward_item;										// 奖励物品
	};

	RandActivityOfflineSingleChargeCfg() : cfg_count(0), opengame_day(0) {}

	int cfg_count;
	int opengame_day;
	ConfigItem cfg_list[RAND_ACTIVITY_SINGLE_CHARGE_CONFIG_MAX_COUNT];
};

// 买一送一
struct BuyOneGetOneFreeItemCfg
{
	BuyOneGetOneFreeItemCfg() : price_gold(0) { }

	int price_gold;
	ItemConfigData buy_item;
	ItemConfigData free_reward_item;
};

struct RandActivityBuyOneGetOneFreeConfig
{
	RandActivityBuyOneGetOneFreeConfig() : opengame_day(0), item_count(0) { }

	int opengame_day;
	int item_count;
	BuyOneGetOneFreeItemCfg item_list[RAND_ACTIVITY_BUY_ONE_GET_ONE_FREE_ITEM_MAX_COUNT];
};

// 节日守护
struct HolidayGuardKillRankReward
{
	HolidayGuardKillRankReward() : open_server_day(0) {}

	struct ConfigItem
	{
		ItemConfigData reward_list[MAX_ATTACHMENT_ITEM_NUM];
	};

	int open_server_day;
	ConfigItem kill_rank_reward_list_cfg[RAND_ACTIVITY_HOLIDAYGUARD_PERSON_RANK_MAX];
};

// 累计充值5 (版本吉祥三宝)
struct RandActivityTotalCharge5Cfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), need_chongzhi_num(0) {}

		int seq;															// 充值档次，自动生成，不是读配置

		int need_chongzhi_num;
		ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];				// 邮件发放奖励
	};

	static const int MAX_CHONGZHI_CFG_COUNT = 5;				// 同一个分段最大奖励配置数

	RandActivityTotalCharge5Cfg() : start_day(0), end_day(0), chongzhi_list_count(0) {}

	int start_day;
	int end_day;

	int chongzhi_list_count;
	ConfigItem chongzhi_list[MAX_CHONGZHI_CFG_COUNT];
};

struct RandActivityTotalCharge5Config
{
	RandActivityTotalCharge5Config() : opengame_day(0), total_charge_5_cfglist_count(0) {}

	int opengame_day;
	int total_charge_5_cfglist_count;

	RandActivityTotalCharge5Cfg total_charge_5_cfglist[RAND_ACTIVITY_TOTAL_CHARGE_5_CFG_COUNT];
};

struct RandActivityExtremeChallengeCfg
{
	RandActivityExtremeChallengeCfg() : task_id(0), task_type(0), param1(0), param2(0) {}

	int task_id;
	int task_type;
	int param1;
	int param2;
	ItemConfigData reward_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct RandActivityChongZhiGiftConfig
{

	RandActivityChongZhiGiftConfig() : opengame_day(0), activity_day(0), index(0), need_gold(0) {}

	int opengame_day;
	int activity_day;
	int index;
	int need_gold;
	ItemConfigData reward_item;
};

// 表白排行榜
struct RandActivityProfessRankCfg
{
	struct ConfigItem
	{
		ConfigItem() : rank(0), limit_score(0) {}

		int rank;
		int limit_score;
		ItemConfigData item;
		ItemConfigData female_item;
	};

	RandActivityProfessRankCfg() : cfg_count(0), opengame_day(0) {}

	int cfg_count;
	int opengame_day;
	ConfigItem cfg_list[RAND_ACTIVITY_CONSUME_GOLD_RANK_REWARD_CONFIG_MAX_COUNT];
};

// 组合购买
struct RandActivityCombineBuyDiscountCfg
{
	RandActivityCombineBuyDiscountCfg() : opengame_day(0){}

	int opengame_day;
	std::map<int, int> item_count_to_discount_cfg_map;
};

struct RandActivityCombineBuyItemCfg
{
	struct ItemCfg
	{
		ItemCfg() : price(0), buy_limit(0){}

		int price;
		int buy_limit;
		ItemConfigData item;
	};

	RandActivityCombineBuyItemCfg() : opengame_day(0) {}

	int opengame_day;
	ItemCfg item_cfg_list[RA_COMBINE_BUY_MAX_ITEM_COUNT];
};

struct RandActivityLuckyWishLotteryCfg
{
	RandActivityLuckyWishLotteryCfg() : opengame_day(0), seq(0), weight(0), is_rare(0),is_baodi(false) {}

	int opengame_day;			// 开服天数
	int seq;					// 序列
	int weight;					// 权重
	bool is_rare;				// 是否珍稀
	bool is_baodi;				// 是否是保底
	ItemConfigData item;
};

struct RandActivityLuckyWishLotteryAndTotalWeightCfg
{
	RandActivityLuckyWishLotteryAndTotalWeightCfg() : total_weight(0) {}

	std::vector<RandActivityLuckyWishLotteryCfg> lottery_vec;
	int total_weight;			// 总权重
	std::map<int, int> baodi_map;
};

// 周末狂欢-------------------------------------------

struct RALottery1OtherCfg
{
	int lottery1_consume_gold = 0;
	int lottery1_free_interval_s = 0;
	ItemID lottery1_one_consume_item = 0;
	int lottery1_ten_consume_gold = 0;
	ItemID lottery1_ten_consume_item = 0;
};

struct RALottery1RewardWeight
{
	int seq = 0;
	int weight = 0;
};

struct RALottery1RewardItem
{
	int seq = 0;
	int is_record_history = 0;
	int is_broadcast = 0;
	ItemConfigData reward_item;
};

struct RALottery1RewardCfg
{
	RALottery1RewardCfg() {}

	std::vector<RALottery1RewardItem> reward_item;
	std::vector<RALottery1RewardWeight> one_lottery;
	std::vector<RALottery1RewardWeight> ten_lottery;
};

struct RALottery1PersonRewardCfg
{
	RALottery1PersonRewardCfg() : opengame_day(0), seq(0), person_chou_times(0), vip_limit(0), reward_count(0){}

	int opengame_day;
	int seq;
	int person_chou_times;
	int vip_limit;

	int reward_count;
	ItemConfigData reward_item_list[MULTI_MAX_ATTACHMENT_ITEM_NUM];
};

struct RALottery1ServerRewardWeight
{
	int seq = 0;
	int weight = 0;
};

struct RALottery1ServerRewardItem
{
	int seq = 0;
	int is_record_history = 0;
	int is_broadcast = 0;

	ItemConfigData reward_item;
};

struct RALottery1ServerRewardCfg
{
	RALottery1ServerRewardCfg() : opengame_day(0), server_chou_times(0) {}

	int opengame_day;
	int server_chou_times;

	std::vector<RALottery1ServerRewardWeight> lottery_weight;

	std::vector<RALottery1ServerRewardItem> reward_item;
};

//-------------------------------------------------------------

struct RandActivityLuckyWishLuckyCfg
{
	RandActivityLuckyWishLuckyCfg() : opengame_day(0), consume_gold_1(0), consume_gold_30(0), add_lucky_value(0), lucky_min(0), lucky_max(0) {}
	
	int opengame_day;		//开服天数
	int consume_gold_1;		//消耗元宝
	int consume_gold_30;		//消耗元宝
	int add_lucky_value;	//每次许愿增加的幸运值
	int lucky_min;			//中奖所需最小幸运值
	int lucky_max;			//中奖所需最大幸运值
	ItemConfigData item;
};

struct RADailyLoveOpengameDayCfg
{
	RADailyLoveOpengameDayCfg() : opengame_day(0), gold_percent(0) {}

	int opengame_day;
	int gold_percent;
};

enum IMAGE_COMPETITION_TYPE    // 形象比拼类型
{
	IMAGE_COMPETITION_TYPE_INVALID = 0,
	IMAGE_COMPETITION_TYPE_MOUNT = 1,        // 坐骑
	IMAGE_COMPETITION_TYPE_WING = 2,         // 羽翼
	IMAGE_COMPETITION_TYPE_FIGHT_MOUNT = 3,  // 战骑
	IMAGE_COMPETITION_TYPE_SHIZHUANG = 4,    // 时装
	IMAGE_COMPETITION_TYPE_SHENBING = 5,     // 神兵
	IMAGE_COMPETITION_TYPE_FABAO = 6,        // 法宝
	IMAGE_COMPETITION_TYPE_XIANNV_HALO = 7,  // 仙女光环
	IMAGE_COMPETITION_TYPE_QILINBI = 8,      // 麒麟臂
	IMAGE_COMPETITION_TYPE_TOUSHI = 9,       // 头饰
	IMAGE_COMPETITION_TYPE_YAOSHI = 10,      // 腰饰
	IMAGE_COMPETITION_TYPE_LINGTONG = 11,    // 灵童
	IMAGE_COMPETITION_TYPE_LINGGONG = 12,    // 灵弓
	IMAGE_COMPETITION_TYPE_LINGMOUNT = 13,   // 灵骑
	IMAGE_COMPETITION_TYPE_HALO = 14,        // 光环
	IMAGE_COMPETITION_TYPE_FEICHONG = 15,	 // 飞宠
	IMAGE_COMPETITION_TYPE_WEIYAN = 16,		 // 尾焰
	IMAGE_COMPETITION_TYPE_FOOTPRINT = 17,	 // 足迹
	IMAGE_COMPETITION_TYPE_SHENYI = 18,		 // 仙女法阵 
	IMAGE_COMPETITION_TYPE_MASK = 19,		 // 面饰
	IMAGE_COMPETITION_TYPE_WEIBA = 20,		 // 尾巴
	IMAGE_COMPETITION_TYPE_SHOUHUAN = 21,	 // 手环
	IMAGE_COMPETITION_TYPE_XIANNV = 22,		 // 仙女
	IMAGE_COMPETITION_TYPE_MAX,
};

struct RandActivityImageCompetition
{
	RandActivityImageCompetition() : opengame_day(0), rank_system_type(0), rank_system_id(0), item_id(0) {}

	int opengame_day;
	int rank_system_type;
	int rank_system_id;
	ItemID item_id;
};

struct RandActivityImageCompetitionReward
{
	struct RewardCfg
	{
		RewardCfg() : reward_index(0), rank_pos(0), reward_count(0), limit_grade(0){}

		int reward_index;
		int rank_pos;
		int reward_count;
		int limit_grade;
		ItemConfigData reward_list[MAX_ATTACHMENT_ITEM_NUM];
	};

	RandActivityImageCompetitionReward() : opengame_day(0){}

	int opengame_day;
	RewardCfg reward_cfg[RAND_ACTIVITY_IMAGE_COMPETITION_REWARD_CFG_COUNT];
};

// 零元购-------------------------------------------
struct RAZeroBuyReturnConfig
{
	RAZeroBuyReturnConfig() : need_gold(0)
	{
		memset(daily_reward_gold, 0, sizeof(daily_reward_gold));
	}

	int need_gold;
	ItemConfigData buy_reward;
	int daily_reward_gold[RAND_ACTICITY_ZERO_BUY_RETURN_MAX_DAY];
};

// 狂嗨庆典-------------------------------------------
struct RandActivityKuangHaiCfg
{
	struct ItemCfg
	{
		int index = 0;			// 索引
		int task_type = 0;		// 任务类型
		int task_arg = 0;		// 条件参数
		int per_score = 0;		// 每次可得嗨点
		int max_score = 0;		// 嗨点上限
		int open_level = 0;		// 开启等级
	};

	int item_num = 0;
	ItemCfg item_cfg_list[RA_KUANG_HAI_TASK_ITEM_MAX_NUM];
	std::map<int, std::vector<const RandActivityKuangHaiCfg::ItemCfg*> > id_task_map;// <task_id, task_vec>
};

struct RandActivityKuangHaiRewardCfg
{
	struct ItemCfg
	{
		int reward_seq = 0;			// 索引
		int need_score = 0;			// 任务类型
		int is_broadcast = 0;		// 是否传闻
		int reward_num = 0;			// 奖励数量
		ItemConfigData reward_list[RA_KUANG_HAI_REWARD_MAX_ITEM_NUM];
	};

	int item_num = 0;
	ItemCfg item_cfg_list[RA_KUANG_HAI_REWARD_LIST_MAX_NUM];
};

// 礼物收割-------------------------
struct GiftHarvestRankRewardCfg
{
	struct ConfigItem
	{
		ConfigItem() : rank(0), num(0) {}

		int rank;
		int num;
		ItemConfigData reward_list[MAX_ATTACHMENT_ITEM_NUM];
	};

	int item_num = 0;
	ConfigItem rank_list[GIFT_HARVEST_PERSON_RANK_MAX];
};

struct RandActivityDailyLoveConfig
{
	RandActivityDailyLoveConfig() : opengame_day(0), reward_gold_per(0)
	{
	}

	int opengame_day;
	int reward_gold_per;
};

// 幸运云购
static const int LUCKY_CLOUD_BUY_MIN_REWARD_MAX_NUM = 6;	// 幸运云购 保底奖励 最大物品数量
static const int LUCKY_CLOUD_BUY_ROUND_MAX_NUM = 16;			// 幸运云购 每天最大轮数

struct RandLuckyCloudBuyCfg
{
	RandLuckyCloudBuyCfg() :round_num_tody(0), opengame_day(0) {}

	struct RoundItem
	{
		int seq = 0;																	// 本轮 索引
		int sale_count = 0;																// 本轮 发售量
		int need_gold = 0;																// 本轮 单价
		int begin_time_hour = 0;														// 本轮 开始时间（小时）
		int begin_time_min = 0;															// 本轮 开始时间（分钟）
		int end_time_hour = 0;															// 本轮 结束时间（小时）
		int end_time_min = 0;															// 本轮 结束时间（分钟）
		int min_reward_item_count = 0;													// 本轮 保底奖数量
		int total_reward_weight = 0;													// 本轮 保底奖总权重
		ItemConfigData big_reward_item;													// 本轮 大奖物品
		ItemConfigData min_reward_item_list[LUCKY_CLOUD_BUY_MIN_REWARD_MAX_NUM];		// 本轮 保底奖励物品列表
		unsigned int min_reward_weight_list[LUCKY_CLOUD_BUY_MIN_REWARD_MAX_NUM] = { 0 };// 本轮 保底奖励权重列表

		const ItemConfigData* GetMinRewardItem() const
		{

			int rand_select = RandomNum(0, total_reward_weight);
			int rand_sum = 0;

			for (int index = 0; index < min_reward_item_count && index < LUCKY_CLOUD_BUY_MIN_REWARD_MAX_NUM; index++)
			{
				rand_sum += min_reward_weight_list[index];
				if (rand_sum >= rand_select)
				{
					return &min_reward_item_list[index];
				}
			}

			return nullptr;
		}
	};

	int round_num_tody;									// 本日的轮数
	int opengame_day;									// 开服天数
	RoundItem round_list[LUCKY_CLOUD_BUY_ROUND_MAX_NUM];
};

struct RandLuckyCloudBuyOtherCfg
{
	RandLuckyCloudBuyOtherCfg() :buy_limit(0), nolimit_time_hour(0), nolimit_time_minute(0), is_broadcast(0), today_time_nolimit(0), huge_reward_need_percent(0){}

	int buy_limit;				// 每轮的购买限制
	int nolimit_time_hour;		// 无限制购买的时间-小时
	int nolimit_time_minute;	// 无限制购买的时间-分钟
	int is_broadcast;			// 是否大奖广播
	int today_time_nolimit;		// 无限制日时间戳
	int huge_reward_need_percent;	//购买数量大于等于%x直接给大奖
};

struct FirstChargeTuanCfg
{
	FirstChargeTuanCfg() :refresh_time(0), fix_charge_num(0){}
	int refresh_time;
	int fix_charge_num;
};

struct RandActivityCrazyBuy
{
	RandActivityCrazyBuy() : opengameday(0) {}

	struct ContentCfg
	{
		ContentCfg() :seq(0), level(0), gold_buy(0), gold_level(0), server_buy_times_limit(0), role_buy_times_limit(0), is_broadcast(0) {}

		int seq;
		int level;
		int gold_buy;
		int gold_level;
		int server_buy_times_limit;
		int role_buy_times_limit;
		ItemConfigData reward_item;

		int is_broadcast;
	};

	int opengameday;
	ContentCfg content[RAND_ACTIVITY_CRACY_BUY_MAX_NUM_TO_OPENDAY];
};

struct RandActivityDoubleGet
{
	RandActivityDoubleGet(): count(0),opengame_day(0){}
	struct ConfigItem
	{
		ConfigItem():seq(0),chongzhi_value(0),reward_gold(0){}
		int seq;
		int chongzhi_value;
		int reward_gold;
	};

	int count;
	int opengame_day;
	ConfigItem item_list[RAND_ACTIVITY_DOUBLE_GET_CFG_COUNT];
};

class RandActivityConfig : public ConfigBase
{
public:
	RandActivityConfig();
	~RandActivityConfig();
	
	bool Init(const std::string &path, std::string *err);
	
	const RandActivityLuckyRollCfg * GetLuckyRollCfg();
	const RandActivityLuckyRollRewardCfg * RandLuckyRollYaojiangRewardCfg(long long gold_poll, int *result_index);
	const RandActivityDayChongZhiFanLiCfg::ConfigItem * GetDayChongZhiFanLiRewardCfg(int reward_seq);
	const RandActivityDayConsumeGoldCfg::ConfigItem * GetDayConsumeGoldRewardCfg(int reward_seq);
	const RandActivityServerPanicBuyCfg::ConfigItem * GetServerPanicBuyItemCfg(int item_idx);
	const RandActivityPersonalPanicBuyCfg::ConfigItem * GetPersonalPanicBuyItemCfg(int item_idx);
	const RandActivityChestShopCfg::ConfigItem * GetChestshopCfgBySeq(int seq);
	const RandActivityStoneUplevelCfg::ConfigItem * GetStoneUplevelCfgByLevel(int total_stone_level);
	const RandActivityStoneUplevelCfg::ConfigItem * GetStoneUplevelCfgBySeq(int seq);
	const RandActivityChongzhiRankCfg::ConfigItem * GetChongzhiRankRewardCfg(int user_rank, int chongzhi);
	const RandActivityChongzhiRankCfg::ConfigItem * GetChongzhiRank2RewardCfg(int user_rank, int chongzhi);

	const RandActivityConsumeGoldRankCfg::ConfigItem * GetConsumeGoldRankRewardCfg(int user_rank, int consume);
	const RandActivityEquipStrengthenCfg::ConfigItem * GetEquipStrengthRewardCfg(int strengthen_level);
	const int GetConsumeGoldRankNeedChonghzi(int user_rank);
	
	const RandActivityTotalConsumeGoldCfg::ConfigItem * GetTotalConsumeGoldRewardCfg(int reward_seq);
	const RandActivityTotalConsumeGoldCfg * GetDiffTotalConsumeGoldRewardCfg();

	const RandActivityDayActiveDegreeCfg::ConfigItem * GetDayActiveRewardRewardCfg(int reward_seq);
	const RandActivityXiannvChanmianUplevelCfg::ConfigItem * GetXiannvChanmianUplevelCfgByLevel(int level);
	const RandActivityXiannvChanmianUplevelCfg::ConfigItem * GetXiannvChanmianUplevelCfgBySeq(int seq);
	const RandActivityMountUpgradeCfg::ConfigItem * GetMountUpgradeCfgBySeq(int seq);
	const RandActivityMountUpgradeCfg::ConfigItem * GetMountUpgradeCfgByGrade(int grade);
	const RandActivityQibingUpgradeCfg::ConfigItem * GetQibingUpgradeCfgBySeq(int seq);
	const RandActivityQibingUpgradeCfg::ConfigItem * GetQibingUpgradeCfgByGrade(int grade);
	const RandActivityMentalityTotalLevelCfg::ConfigItem * GetMentalityTotalLevelCfgBySeq(int seq);
	const RandActivityMentalityTotalLevelCfg::ConfigItem * GetMentalityTotalLevelCfgByLevel(int level);
	const RandActivityWingUpgradeCfg::ConfigItem * GetWingUpgradeCfgBySeq(int seq);
	const RandActivityWingUpgradeCfg::ConfigItem * GetWingUpgradeCfgByGrade(int grade);
	const RandActivityDayOnlineGiftCfg::ConfigItem * GetNextDayOnlineGiftCfg(int now_hour);
	const ItemConfigData * GetDayOnlineGiftCfgRewardItem();
	const RandActivityQuanminQifuCfg::ConfigItem * GetQuanminQifuCfgBySeq(int seq);
	const RandActivityXianmengBipinCfg::ConfigItem * GetXianmengBipinCfg(int rank);
	const RandActivityKillBossCfg & GetKillBossRewardCfg() { return m_kill_boss_reward_cfg;}
	const RandActivityXianmengJueQiCfg::ConfigItem * GetXianmengJueQiCfg(int rank);
	const RandActivityLoginGiftCfg * GetLoginGiftCfg();
	const RandActivityLoginGiftCfg * GetLoginGiftCfg_0();
	const RandActivityLoginGiftCfg::ConfigItem * GetLoginGiftRewardCfg(int seq);
	const RandActivityLoginGiftCfg::ConfigItem * GetLoginGiftRewardCfg_0(int seq);
	const RandActivityShouYouYuXiangCfg::ConfigItem * GetShouYouYuXiangRewardCfg(int reward_seq);
	const RandActivityRankBipinCfg * GetRankBipinCfg(int activity_type);
	const RandActivityAllBipinCfg::ConfigItem * GetAllBipinCfg(int activity_type, int seq);
	const RandActivityChargeRepaymentCfg::ConfigItem * GetChargeRepaymentCfgWithSeq(int seq);
	const RandActivityChargeRepaymentCfg::ConfigItem * GetChargeRepaymentCfgWithCharge(long long charge_value);
	const RandActivitySingleChargeCfg::ConfigItem * GetSingleChargeCfg(long long charge_value);
	const RandActivitySingleChargeCfg::ConfigItem * GetSingleCharge1Cfg(long long charge_value,int flag);
	const RandActivitySingleChargeCfg::ConfigItem * GetSingleCharge2Cfg(long long charge_value,int flag);
	const RandActivitySingleChargeCfg::ConfigItem * GetSingleCharge3Cfg(long long charge_value,int flag);
	const RandActivityCornucopiaCfg *GetCornucopiaCfg(int lun);

	const RandActivityDanbiChongzhiRewardCfg * GetDanbiChongzhiRewardBySeq(int activity_day, int seq);
	const RandActivityDanbiChongzhiDayDiffCfg * GetDanbiChongzhiRewardDiffCfg();
	const RandActivityDanbiChongzhiRewardCfg * GetDanbiChongzhiRewardByChongzhi(int activity_day, int chongzhi);

	const RandActivityTotalChargeDayCfg::TotalChargeDayItem *GetTotalChargeDayCfgWithSeq(int seq);
	const RandActivityTotalChargeDayCfg::TotalChargeDayItem *GetTotalChargeDayCfgWithDayCount(int charge_day_count);
	const RandActivityTomorrowRewardCfg *GetTomorrowRandomRewardCfg();

	const RandActivityTotalChargeRewardConfig::ConfigItem * GetTotalChargeRewardWithSeq(int seq);
	const RandActivityTotalChargeRewardConfig * GetTotalChargeRewardCfg();
	const RandActivityTimeLimitExchangeConfig * GetTimeLimitExchangeEquiCfg(int seq);
	const RandActivityTimeLimitExchangeConfig * GetTimeLimitExchangeJLCfg(int seq);

	const RandActivityAdditionLotteryConfig::ConfigItem * GetLevelLotteryRandomRewardCfg(int level);
	const RandActivityAdditionLotteryConfig::ConfigItem * GetLevelLotteryBaodiRewardCfg(int server_chou_times);
	const RandActivityLevelLotteryOtherCfg & GetLevelLotteryOhterCfg() { return m_level_lottery_other_cfg; }
	const RandActivityLevelLotteryTotalTimesCfg * GetLevelLotteryTotalTimesRewardCfg(int total_times);
	const std::map<int, RandActivityLevelLotteryTotalTimesCfg > & GetLevelLotteryOpengameDayRewardCfg(int join_day);

	const RandActivityNiuEggChouRewardConfig::RANiuEggConfigItem * GetRandomNiuEggRewardCfg();
	const RandActivityNiuEggChouRewardConfig::RANiuEggConfigItem * GetNiuEggBaodiRewardCfg(const int server_chou_times);
	const RandActivityNiuEggServerRewardConfig::RANiuEggConfigItem * GetNiuServerCfgWithServerNiuTimes(int server_total_niu_times);
	const RandActivityNiuEggChouRewardConfig::RANiuEggConfigItem * GetNiuRewardCfgWithReq(int req);
	const RandActivityNiuEggServerRewardConfig::RANiuEggConfigItem * GetNiuServerCfgWithReq(int req);

	const RandActivityZhenBaoGeConfig * GetZhenbaogeCfg(int seq);
	const RandActivityZhenBaoGeAddWeightConfig * GetZhenbaogeAddWeight(int index);
	const RandActivityZhenBaoGeServerConfig * GetZhenbaogeServerCfg(int seq);
	const ItemConfigData & GetZhenbaogeAllBuyReward() { return m_zhenbaoge_all_buy_reward; }
	bool GetZhenbaogeRandomResult(const ZhenbaogeAddWeightInfo *self_weight, int length, int is_free, RandActivityZhenBaoGeConfig *result_list, int result_list_length,int server_chou_times);

	const RandActivityDayChongzhiRankRewardConfig::ConfigItem * GetDayChongzhiRankRewardCfg(int day, int rank, int gold_num);
	const RandActivityDayChongzhiRankRewardDiffConfig * GetDayChongzhiRankRewardDayCfg();

	const RandActivityDayConsumeRankRewardConfig::ConfigItem * GetDayConsumeRankRewardCfg(int day, int rank, int gold_num);
	const RandActivityDayConsumeRankRewardDiffConfig * GetDayConsumeRankRewardDayCfg();

	const RandActivityJiSuChongZhanConfig * GetJiSuChongZhanRewardCfg(const int charge_value);

	const RandActivityMoneyTreeRewardConfig * GetRandomMoneyTreeRewardCfg();
	const RandActivityMoneyTreeRewardConfig * GetRandomMoneyTreeItemRewardCfg();
	const RandActivityMoneyTreeRewardConfig * GetMoneyTreeBaodiRewardCfg(int server_chou_times);
	const RandActivityServerBaodiConfig* GetMoneyTreeBaodiRandReward(int server_chou_times);
	const RandActivityMoneyTreeServerRewardConfig * GetTreeServerCfgWithServerMoneyTreeTimes(int server_total_money_tree_times, int join_day = 0);
	const RandActivityMoneyTreeServerRewardConfig * GetMoneyTreeServerCfgWithReq(int req, int join_day = 0);

	const RandActivityKingDrawRewardConfig * GetKingDrawRandomRewardCfg(int level, const RAKingDrawSelfWeight self_weight_list[]);
	const RandActivityKingDrawRewardConfig * GetKingDrawBaodiRewardCfg(int level, const int server_chou_times);
	const RandActivitySelfWeightConfig * GetKingDrawSelfWeightCfg(int level, int index);
	int GetKingDrwaRewardCount(int level);
	int GetKingDrawOpenGameDay(int level, int index);

	const RandActivityTotalCharge3Cfg::ConfigItem * GetTotalCharge3Cfg(int day_idx, int seq);

	const ItemConfigData * GetAppearaceRankReward(int rank);				// 获取变身排行活动的排行奖励
	const ItemConfigData & GetAppearaceRankJoinReward() { return m_appearance_rank_join_reward; }// 获取变身排行活动参与奖励
	const ItemConfigData * GetAppearacePassiveRankReward(int rank);				// 获取被动变身排行活动的排行奖励
	const ItemConfigData & GetAppearacePassiveRankJoinReward() { return m_appearance_passive_rank_join_reward; }// 获取被动变身排行活动参与奖励

	const RandActivityAnyMonsterDropConfig * GetAndyMonsterDropCfg(UInt16 item_id = 0);

	const RandActivityTotalCharge4Cfg::ConfigItem * GetTotalCharge4Cfg(int day_idx, int seq);

	const RandActivityMineServerRewardStageConfig * GetMineServerRewardCfg(int seq);
	const RandActivityMineInfoStageConfig * GetMineInfoCfg(int seq);

	const RandActivityTianMingDivinationRewardConfig * GetTianMingDivinationRewardCfg(int seq);
	const RandActivityTianMingDivinationRewardConfig * GetTianMingDivinationBaodiRewardCfg(const int server_chou_times);
	const RandActivityTianMingDivinationGoldConsumeConfig * GetTianMingDivinationGoldConsumeCfg(int times);

	const RandActivityArmyDayExchangeFlagConfig * GetArmyDayExhangeFlagCfg(const int seq);
	const RandActivityArmyDayExchangeItemConfig * GetArmyDayExhangeItemCfg(const int side_type);
	const ItemConfigData &GetArmyDayWinReward() { return m_army_day_win_side_reward; }

	const RandActivityCirculationChongzhiStageRewardConfig * GetCirculationChongzhiRewardCfg(int reward_seq);

	bool GetExtremLuckyFlushRewardList(int flush_tiems, RAExtremeLuckyRewardInfo *reward_list, const int list_length, const int server_chou_times);
	const RandActivityExtremeLuckyDrawRewardConfig * GetExtremeLuckyRewardCfg(const int seq);
	const RandActivityExtremeLuckyReturnRewardStageCfg * GetExtremeLuckyReturnRewardCfg(const int seq);

	const RandActivityNationalDayConfig * GetNationalDayRewardCfg(const int seq);

	const RandActivityNewTotalChargeRewardConfig::ConfigItem * GetNewTotalChargeRewardWithSeq(int seq);
	const RandActivityNewTotalChargeRewardConfig * GetNewTotalChargeRewardCfg();
	const RandActivityNewTotalChargeRewardConfig::ConfigItem * GetNewTotalCharge1RewardWithSeq(int seq);
	const RandActivityNewTotalChargeRewardConfig * GetNewTotalCharge1RewardCfg();
	const RandActivityNewTotalChargeRewardConfig::ConfigItem * GetNewTotalCharge2RewardWithSeq(int seq);
	const RandActivityNewTotalChargeRewardConfig * GetNewTotalCharge2RewardCfg();
	const RandActivityNewTotalChargeRewardConfig::ConfigItem * GetNewTotalCharge3RewardWithSeq(int seq);
	const RandActivityNewTotalChargeRewardConfig * GetNewTotalCharge3RewardCfg();

	const RandActivityMagicShopConfig * GetMagicShopCfg(int index);

	RandActivityEverydayNiceGiftCfg::ConfigItem *  GetEveydayNiceGiftRewardCfg();			// 每日好礼

	int GetDayChongZhiFanLiRewardCfgCount();
	int GetDayConsumeGoldRewardCfgCount();
	int GetConsumeGoldFanliRate() { return m_consume_gold_fanli_rate; }
	int GetChestshopRewardCfgCount() { return m_chestshop_reward_cfg.cfg_count; }
	int GetDayActiveDegreeCfgRewardCfgCount();
	int GetStoneTotalLevelRewardCfgCount() { return m_stone_uplevel_cfg.cfg_count; }
	int GetXiannvChanmianUplevelRewardCfgCount() { return m_xiannv_chanmian_uplevel_cfg.cfg_count; }
	int GetMountUpgradeRewardCfgCount() { return m_mount_upgrade_cfg.cfg_count; }
	int GetQibingUpgradeRewardCfgCount() { return m_qibing_upgrade_cfg.cfg_count; }
	int GetMentalityTotalLevelRewardCfgCount() { return m_mentality_total_level_cfg.cfg_count; }
	int GetWingUpgradeRewardCfgCount() { return m_wing_upgrade_cfg.cfg_count; }
	int GetDayOnlineGiftCfgCount();
	int GetVipQifuRewardCfgCount() { return m_quanmin_qifu_cfg.cfg_count; }
	int GetChestshopDiscount() { return m_chestshop_discount; }
	int GetQuanminQifuDiscount() { return m_quanmin_qifu_discount; }
	int GetShouYouYuXiangCfgRewardCfgCount() { return m_shouyou_yuxiang_cfg.cfg_count;}
	int GetBiPinActivityTypeByOpenDayIndex(int openday_idx);
	int GetTomorrowRewardNeedActiveDegree() { return m_tomorrow_reward_need_active_degree; }
	int GetRelateBipinActivity(int person_rank_type);
	int GetTotalChargeSeqWithCharge(int total_charge);
	int GetNewTotalChargeSeqWithCharge(int total_charge);
	int GetNewTotalCharge1SeqWithCharge(int total_charge);
	int GetNewTotalCharge2SeqWithCharge(int total_charge);
	int GetNewTotalCharge3SeqWithCharge(int total_charge);
	int GetIntervalOpenGameDay(int activity_type);
	int GetLevelLotteryNeedGold(int level);
	ItemID GetLevelLotteryConsumeItem() { return m_level_lottery_consume_item; }
	int GetNiuEggNeedCharge() { return m_niu_egg_need_charge; }
	int GetZhenbaogeAddWeightSeq(int index);
	int GetZhenbaogeFlushConsumeGold() { return m_zhenbaoge_reflush_gold; }
	int GetZhenbaogeFlushConsumeItem() { return m_zhenbaoge_flush_item; }
	int GetZhenbaogeAutoFlushTimes() { return m_zhenbaoge_auto_flush_times; }
	int GetChargeRewardPrecent(const int type, const int chongzhi_num);
	
	int GetMiJingXunBaoAddWeightSeq(int index);
	int GetMiJingXunBaoCfgAddWeight(int seq);
	int GetMiJingXunBaoInterval() { return m_mijingxunbao_interval; }
	int GetMiJingXunBaoTaoGold(int type);
	int GetMiJingXunBaoTaoItem(){ return m_mijingxunbao_fifthtimes_item_id; }
	int GetMiJingXunBaoAutoWeightAddCount();
	int GetMiJingXunBaoAddWeightTaoTimes() { return m_add_weight_tao_times;}
	bool GetMiJingXunBaoRandomResult(MiJingXunBaoAddWeightInfo *self_weight, int length, int is_free, RandActivityMiJingXunBaoConfig *result_list, int result_list_length);
	const RandActivityMiJingXunBaoRewardConfig * GetRandActivityMiJingXunBaoPersonalRewardCfg(int seq, int chou_times);

	int GetMoneyTreeInitGold() { return m_money_tree_init_pool_gold; }
	int GetMoneyTreeNeedGold() { return m_rock_money_tree_need_gold; }
	int GetMoneyTreeIntoGold() { return m_money_tree_into_pool; }
	int GetMoneyTreeFreeTimes() { return m_money_tree_free_times; }
	int GetMoneyTreeFreeInterval() { return m_money_tree_free_interval; }
	ItemID GetMoneyTreeConsumeItem() { return m_money_tree_consume_item; }
	int GetDailyLoveRewardPrecent(int opengame_day);
	int GetKingDrawPlayOnceGold(int level);
	//int GetKingDrawPlayOnceScore(int level);
	int GetKingDrawSelfWeigthCfgCount(int level);
	int GetKingDrawReturnRewardCfgCount(int level);
	const RandActivityKingDrawReturnRewardStageCfg * GetKingDrawReturnRewardCfg(int level, int seq, int join_day = 0);
	ItemID GetKingDrawConsumeItem() { return m_king_draw_gaoji_consume_item; }
	int GetTotalCharge3CfgCount(int day_idx);
	bool CheckIsResetTotalCharge3Day(int day_idx);
	int GetAnyMonsterDropLimitDropTime() { return m_any_monster_drop_limit_drop_time; }
	int GetAnyMonsterDropID() { return m_any_monster_drop_id; }
	int GetAnyMonsterDropDayLimitRate() { return m_any_monster_drop_day_limit_rate; }
	int GetChongzhiRankCfgCount();
	int GetChongzhiRank2CfgCount();
	int GetConsumeRankCfgCount();
	int GetTotalCharge4CfgCount(int day_idx);
	bool CheckIsResetTotalCharge4Day(int day_idx);
	int GetMineServerRewardCfgCount() { return m_mine_server_reward_cfg_count; }
	int GetMineFetchServerRewardLevel() { return m_mine_server_reward_level_limit; }
	int GetMineInfoCfgCount() { return m_mine_info_cfg_count; }
	int GetMineRefreshGold() { return m_mine_refresh_gold; }
	int GetMineFreeGatherTimes() { return m_mine_free_times; }
	int GetMineRefreshCount() { return m_mine_refresh_count; }
	bool GetMineRefreshResult(char result_list[], int result_list_len, int refresh_max_times, int server_chou_times);
	int GetGuaGuaPlayTimesGold(int type);
	int GetGuaGuaFreeTimeInterval();
	bool GetGuaGuaRandomRewardCfg(RandActivityGuaGuaConfig *guagua_cfg, int *is_win, int server_roll_times);
	const RandActivityGuaGuaAccConfig *GetGuaGuaAccRewardByIndex(int index);
	const RandActivityServerBaodiConfig *GetGuaGuaBaodiRewardCfg(int server_chou_times);
	ItemID GetGuaGuaRollItemID() { return m_guagua_roll_item_id; }
	int GetTianMingDivinationRewardMaxSeq() { return m_tianming_reward_max_seq; }
	int GetTianMingDivinationGoldConsumeMaxAddLotTimes() { return m_tianming_gold_consume_max_add_lot_times; }
	int GetTianMingDivinationSeqWithLotIndex(int lot_index);
	int GetTianMingDivinationLotIndexWithSeq(int seq);
	int GetTianMingDivinationFreeChouTimes() { return m_tianming_free_chou_times; }
	int GetTianMingDivinationTotalWeight() { return m_tianming_reward_total_weight; }

	const RandActivityFanFanItemInfo * GetFanFanItemInfoCfg(int seq);
	const RandActivityFanFanWordInfo * GetFanFanWordInfoCfg(int seq);
	int GetFanFanFreeTimesPerDay() { return m_fanfan_free_fan_times_per_day; }
	int GetFanFanOnceNeedGold() { return m_fanfan_once_need_gold; }
	int GetFanFanRefreshNeedGold() { return m_fanfan_refresh_need_gold; }
	int GetFanFanAutoRefreshInterval() { return m_fanfan_auto_refresh_interval; }
	int GetFanFanWordIncreaseRate() { return m_fanfan_word_increase_rate; }
	int GetFanFanRandomRewardSeq(int letter_occur_rate, int *is_reward_word);
	RAFanFanHiddenWordInfo GetFanFanRandomHiddenWord(int role_refresh_times , int server_chou_times);
	const RandActivityFanFanBaodiConfig * GetFanFanBaodi(int index);
	int GetFanFanBaodiTimes() { return m_fanfan_baodi_reward_cfg_count; }

	int GetContinueChongzhiMaxDayIndax() { return m_continue_chongzhi_max_day_index; }
	int GetContinueChongzhiFetchExtraRewardNeedDays() { return m_continue_chongzhi_fetch_extra_reward_need_days; }
	const ItemConfigData & GetContinueChonghziExtraReward() { return m_continue_chongzhi_extra_reward; }

//  int GetContinueChongzhi2FetchExtraRewardNeedDays() { return m_continue_chongzhi_fetch_extra_reward_need_days_2; }
//  const ItemConfigData & GetContinueChonghzi2ExtraReward() { return m_continue_chongzhi_extra_reward_2; }

	int GetContinueChongzhiChuFetchExtraRewardNeedDays() { return m_continue_chongzhi_fetch_extra_reward_need_days_chu; }
	const ItemConfigData & GetContinueChonghziChuExtraReward() { return m_continue_chongzhi_extra_reward_chu; }

	int GetContinueChongzhiGaoFetchExtraRewardNeedDays() { return m_continue_chongzhi_fetch_extra_reward_need_days_gao; }
	const ItemConfigData & GetContinueChonghziGaoExtraReward() { return m_continue_chongzhi_extra_reward_gao; }

	const RandActivityContinueChongzhiRewardConfig * GetContinueChongzhiRewardInfoWithdayindex(int day_index);
	const RandActivityContinueChongzhiRewardConfig * GetContinueChongzhiRewardInfoWithdayindexChu(int day_index, bool is_chongzhi, bool is_bu_fa_reward);
	const RandActivityContinueChongzhiRewardConfig * GetContinueChongzhiRewardInfoWithdayindexGao(int day_index, bool is_chongzhi, bool is_bu_fa_reward);
	const RandActivityContinueChongzhiRewardConfig * GetContinueChongzhiRewardInfoWithdayindex2(int day_index, bool is_chongzhi, bool is_bu_fa_reward);

	int GetContinueConsumeMaxDayIndax() { return m_continue_consume_max_reward_day_cfg_count; }
	int GetContinueConsumeFetchExtraRewardNeedDays();
	const RandActivityContinueConsumeRewardConfig * GetContinueConsumeRewardInfoWithdayindex(int day_index);
	const ItemConfigData * GetContinueConsumeExtraReward();

	int GetExtremeLuckyFlushNeedGold() { return m_extreme_lucky_flush_consume_glod; }
	int GetExtremeLuckyFreeDrawTimes() { return m_extreme_lucky_free_draw_times; }
	int GetExtremeLuckyReturnRewardCount() { return m_extreme_lucky_return_reward_cfg_count; }
	int GetExtremeLuckyDrawConsumeByDrawTimes(int draw_times) const;

	int GetMiJingXunBao2AddWeightSeq(int index);
	int GetMiJingXunBao2CfgAddWeight(int seq);
	int GetMiJingXunBao2Interval() { return m_mijingxunbao2_interval; }
	int GetMiJingXunBao2TaoGold(int type);
	int GetMiJingXunBao2TaoItem(){ return m_mijingxunbao2_fifthtimes_item_id; }
	int GetMiJingXunBao2AutoWeightAddCount();
	int GetMiJingXunBao2AddWeightTaoTimes() { return m_add_weight_tao2_times;}
	bool GetMiJingXunBao2RandomResult(MiJingXunBaoAddWeightInfo *self_weight, int length, int is_free, RandActivityMiJingXunBaoConfig *result_list, int result_list_length);

	int GetMoneyTree2InitGold() { return m_money_tree2_init_pool_gold; }
	int GetMoneyTree2NeedGold() { return m_rock_money_tree2_need_gold; }
	int GetMoneyTree2IntoGold() { return m_money_tree2_into_pool; }
	const RandActivityMoneyTreeRewardConfig * GetRandomMoneyTree2RewardCfg();
	const RandActivityMoneyTreeRewardConfig * GetRandomMoneyTree2ItemRewardCfg();
	const RandActivityMoneyTreeServerRewardConfig * GetTreeServerCfgWithServerMoneyTree2Times(int server_total_money_tree_times);
	const RandActivityMoneyTreeServerRewardConfig * GetMoneyTree2ServerCfgWithReq(int req);

	int GetZhenbaoge2AddWeightSeq(int index);
	int GetZhenbaoge2FlushConsumeGold() { return m_zhenbaoge2_reflush_gold; }
	int GetZhenbaoge2AutoFlushTimes() { return m_zhenbaoge2_auto_flush_times; }
	const RandActivityZhenBaoGeConfig * GetZhenbaoge2Cfg(int seq);
	const RandActivityZhenBaoGeAddWeightConfig * GetZhenbaoge2AddWeight(int index);
	const RandActivityZhenBaoGeServerConfig * GetZhenbaoge2ServerCfg(int seq);
	const ItemConfigData & GetZhenbaoge2AllBuyReward() { return m_zhenbaoge2_all_buy_reward; }
	bool GetZhenbaoge2RandomResult(const ZhenbaogeAddWeightInfo *self_weight, int length, int is_free, RandActivityZhenBaoGeConfig *result_list, int result_list_length, int server_chou_times);

	int GetPromotingPositionFreePlayInterval() { return m_promoting_position_free_play_interval; }
	ItemID GetPromotingPosition10TimesUseItem() { return m_promoting_position_10_times_use_item; }
	int GetPromotingPositionExtraTimePerChongzhi() { return m_promoting_position_extra_time_per_chongzhi_gold; }
	int GetPromotingPositionPlayOnceGold() { return m_promoting_position_play_once_gold; }
	int GetPromotingPositionPlay10TimesGold() { return m_promoting_position_10_times_gold; }
	int GetPromotingPositionOutsideCfgCount() { return m_promoting_position_outside_max_seq; }
	int GetPromotingPositionInsideCfgCount() { return m_promoting_position_inside_max_seq; }
	int GetPromotingPositionRewardCfgCount() { return m_promoting_position_reward_cfg_count; }
	const RandActivityPromotingPositionConfig * GetPromotingPositionOutsideCfg(int seq);
	const RandActivityPromotingPositionConfig * GetPromotingPositionInsideCfg(int seq);
	const RandActivityPromotingPositionConfig * GetRandomPromotingPositionReward(int begin_index, int end_index, int play_type, int circle_type, bool is_split_mode = false);
	const RandActivityPromotingPositionRewardCfg * GetPromotingPositionRewardCfg(int seq);
	const RandActivityPromotingPositionConfig * GetPromotingPositionBaodiReward(const int server_chou_times);

	const RandActivityBlackMarketItemCfg::ItemConfig * GetBlackMarketItemCfg(int day, int seq);

	const RandActivityTreasuresMallBuyConfig::BuyItemConfig * GetTreasuresMallBuyRewardCfg(int buy_index);
	const RandActivityTreasuresMallExchangeConfig::ExchangeItemConfig * GetTreasuresMallExchangeRewardCfg(int exchange_index);

	const RandActivityOpenServerRewardItemConfig * GetRandActivityOpenServerRewardConfig(int activity_type, int seq, bool is_reward_yesterday = false);
	const int GetRandActivityOpenServerMaxCon3(int activity_type, bool is_reward_yesterday = false);

	const RandActivityOpenServerRewardConfig::RewardDayConfig * GetOpenServerRewardConfigByTypeAndOGDay(int activity_type);

	int GetNewCornucopiaPercent() { return m_new_cornucopia_percent; }
	const RandActivityNewCornucopiaConfig * GetRandActivityNewCornucopiaConfig(int opengame_day);
	const RandActivityNewCornucopiaTotalConfig * GetRandActivityNewCornucopiaTotalConfig(int seq);

	//集字
	const RandActivityItemCollectionRewardCfg::ConfigItem * GetItemCollectionRewardCfg(int seq);
	
	//红包好礼
	const RandActivityRedEnvelopeGiftCfg &GetRedEnvelopeGiftCfg(){ return m_red_envelope_gift_cfg; };

	// 经验炼制
	const RandActivityExpRefineRewardCfg * GetRandActivityExpRefineRewardCfg(int seq);

	// 我们结婚吧
	bool IsMarryMeRewardCountLimit(int count) { return count >= m_marryme_cfg.reward_couple_count; }
	const ItemConfigData &GetMarryMeRewardItem() { return m_marryme_cfg.reward_item; }

	// 金猪召唤
	const RandActivityGoldenPigOtherCfg * GetRAGoldenPigOtherCfg() { return &m_golden_pig_other_cfg; }
	ItemConfigData * GetRAGoldenPigRewardCfg(int summon_type, int reward_type, int &boss_id);
	int IsGoldenPigBoss(int boss_id);
	int GetGoldenPigJoinerRewardFlag(int boss_id);
	void SetGoldenPigJoinerRewardFlag(int flag, int boss_id);

	// 开服投资
	const RandActivityOpenServerInvestOtherCfg * GetRAServerInvestOtherCfg(int invest_type);
	const RandActivityOpenServerInvestRewardCfg * GetRAServerInvestRewardCfg(int invest_type, int seq);

	// 单返豪礼
	const RandActivitySingleChongZhiRewardCfg::ItemConfig *GetSingleChongZhiCfgByGoldNum(int chongzhi);
	const RandActivitySingleChongZhiRewardCfg::ItemConfig *GetSingleChongZhiCfgBySeq(int seq);

	// 聚划算
	const RandActivityXianyuanTreasConfig * GetXianyuanTreasCfg(int seq);
	int GetXianyuanTreasCount() { return m_xianyuan_treas_cfg_count; }
	int GetXianyuanTreasBuyLimitDay() { return m_xianyuan_treas_buy_limit_day; }
	const ItemConfigData* GetXianyuanTreasAllBuyReward();
	int GetActivityLastedDays(int activity_type);

	// 限时秒杀
	int GetRushBuyingDuration() { return m_rush_buying_duration; }
	int GetRushBuyingPhase(time_t timestamp);
	int GetRushBuyingSaleItemsCount(int phase_seq);
	unsigned int GetRushBuyingPhaseBeginTimestamp(int phase_seq);
	const RandActivityRushBuyingPhaseConfig::SaleItemConfig * GetRushBuyingSaleItemCfg(int phase_seq, int item_index);

	const RandActivitySingleChargeCfg::ConfigItem * GetSingleChargeCfg2(long long charge_value);
	const RandActivitySingleChargeCfg::ConfigItem * GetSingleChargeCfg3(long long charge_value);
	const RandActivitySingleChargeCfg::ConfigItem * GetSingleChargeCfg4(long long charge_value);
	const RandActivitySingleChargeCfg::ConfigItem * GetSingleChargeCfg5(long long charge_value);

	//地图寻宝
	const int GetMapHuntFreeXunbaoCount() { return m_map_hunt_free_xunbao_count; }
	const int GetMapHuntFlushInterval() { return m_map_hunt_free_flush_interval; }
	const int GetMapHuntXunbaoGold() { return m_map_hunt_xunbao_glod; }
	const int GetMapHuntFlushNeedGold() { return m_map_hunt_flush_gold; }
	bool GetMapHuntRandCityCfg(int rand_count, RandActivityMapHuntCityConfig::CityCfg *rand_list[MAX_RA_MAP_HUNT_CITY_COUNT_OF_ROUTE], int server_chou_times);
	const ItemConfigData * GetMapHuntBaseReward(const int city_id);
	const RandActivityMapHuntRouteConfig::RouteCfg * GetMapHuntRouteCfg(const int route_index);
	const int GetMapHuntRouteIndex(const int first_city_id);
	const RandActivityMapHuntServerRewardConfig::RewardCfg * GetMapHuntServerRewardCfg(const int index);

	//灯塔寻宝
	int GetLightTowerExploreDrawOneGold() { return m_light_tower_explore_one_gold; }
	int GetLightTowerExploreDrawTenGold() { return m_light_tower_explore_ten_gold; }
	const RandActivityLightTowerExploreCfg::LayerConfig * GetLightTowerExploreReward(const int cur_layer, bool is_fixed = false);  //fixed时返回下标为0值
	const RandActivityLightTowerExploreCfg * GetLightTowerExploreLayerCfg(const int layer);
	const RandActivityLightTowerExploreExternCfg * GetLightTowerExploreExternReward(const int layer);
	const RandActivityLightTowerExploreReturnCfg * GetLightTowerExploreReturnReward(const int index);

	// 趣味植树
	ItemConfigData GetPlantingTreeCanyuReward() { return m_planting_tree_canyu_reward; }
	int GetPlantingTreeGatherID() { return m_planting_tree_gather_id; }
	int GetPlantingTreeWaterTime() { return m_planting_tree_watering_time; }
	int GetPlantingTreeSceneid() { return m_planting_tree_scene_id; }
	int GetPlantingTreeNorexItemID() { return m_planting_tree_norexitem_id; }
	
	const RandActivityPlantingTreeRankRewardConfig * GetPlantingTreeRankRewardCfg(int rank);
	int GetPlantingTreeTreeLivingTime() { return m_planting_tree_tree_living_time; }
	int GetPlantingTreeMaxWateringTimes() { return m_planting_tree_max_watering_times; }

	bool IsFishingTimeLimit(short fish_times) { return fish_times >= m_fish_time_limit; }
	bool IsStealFishTimeLimit(short steal_fish_times) { return steal_fish_times >= m_steal_fish_time_limit; }
	bool IsBeStealFishTimeLimit(short be_steal_fish_times) { return be_steal_fish_times >= m_be_stealed_fish_time_limit; }
	RandActivityFishingInfoCfg *GetFishinfo();
	RandActivityFishingInfoCfg *GetFishinfobyType(short fish_type);
	RandActivityFishingRankCfg *GetFishingRewardInfo(short rank_num);
	const short GetStealFishPersent() { return m_steal_fish_percent; }

	bool IsBeStealTimesLimit(short be_steal_times) { return be_steal_times >= m_crystal_dalaran_be_stealed_time_limit; }
	bool IsOnePeopleBeStealTimesLimit(short be_steal_times) { return be_steal_times >= m_steal_crystal_dalaran_one_people_time_limit; }
	bool IsStealTimesLimit(short steal_times) { return steal_times >= m_steal_crystal_dalaran_time_limit; }
	const short GetStealCrystalDalaranPercent() { return m_steal_crystal_dalaran_percent; }
	const int GetStealCrystalDalaranCountLimit() { return m_steal_crystal_dalaran_limit; }
	const short GetStealCrystalDalaranBroadcastpercent() { return m_broadcast_percent; }
	const RandActivityStealCrtstalDalaranRewardCfg *GetStealCrystalDalaranRewardCfg(int rank_num);

	int GetChongzhiCrazyRebateRewardGold(int total_chongzhi_count, int had_rabate_count);

	/////////////////////////////////////// 进阶主题活动 //////////////////////////////////////////////////////
	const int GetJinJieThemeActTheme(int act_id);

	// 消费领奖
	const int GetConsumGiftRollGoldNum() { return m_roll_reward_gold; }
	const int GetConsumGiftRollTimesLimit() { return m_roll_times_limit; }
	const ConsumGiftRewardConfig *GetConsumGiftRewardCfg(int opengame_day,int theme, int seq);
	const ConsumGiftRollRewardPool *GetConsumGiftRollNumType(int theme);
	const ConsumGiftRollRewardPool *GetConsumGiftRollReward(int theme, int seq);
	const bool GetConsumGiftRollRewardNumByType(int type, int &decade, int &units_digit);

	// 每日限购
	const DailyLimitBuyConfig *GetLimitBuyCfg(int theme, int seq);

	// 聚宝盆（新）
	const int GetCollectTreasureCanJoinTimes() { return m_collect_treasure_join_times; }
	const int GetCollectTreasureGold() { return m_collect_treasure_gold; }
	const CollectTreasureConfig *GetCollectTreasureCfg();
	const CollectTreasureConfig *GetCollectTreasureCfgBySeq(int seq);

	// 欢乐累充
	const HappyCumulChongzhiConfig *GetHappyCumulChongzhiCfg(int theme, int seq);

	// 进阶返还
	const JinJieReturnConfig *GetJinJieReturnCfg(int theme, int seq);

	// 暴击日
	const int GetCriticalStrikeDayMul(int type);
	const int GetCriticalStrikeDayRate(int type);

	// 进阶返还2
	const JinJieReturnConfig *GetJinJieReturn2Cfg(int theme, int seq);

	//暴击日2
	const int GetCriticalStrikeDay2Mul(int type);
	const int GetCriticalStrikeDay2Rate(int type);
	/////////////////////////////////////// 进阶主题活动 //////////////////////////////////////////////////////

	// 消费送礼
	const int GetConsumeForGiftItemCount();
	int GetConsumeForGiftPointsPerGold();
	const RandActivityConsumeForGiftItemConfig * GetConsumeForGiftItemCfg(int seq);

	// 秘境寻宝3
	int GetMiJingXunBao3AutoWeightAddCount();
	int GetMiJingXunBao3CfgAddWeight(int seq);
	int GetMiJingXunBao3AddWeightSeq(int index);
	int GetMiJingXunBao3TaoGold(int type);
	int GetMiJingXunBao3Interval() { return m_mijingxunbao3_interval; }
	int GetMiJingXunBao3TaoItem() { return m_mijingxunbao3_thirtytimes_item_id; }
	int GetMiJingXunBao3AddWeightTaoTimes() { return m_add_weight_tao3_times; }
	bool GetMiJingXunBao3RandomResult(MiJingXunBaoAddWeightInfo *self_weight, int length, int is_free, RandActivityMiJingXunBaoConfig *result_list, int result_list_length);
	const RandActivityMiJingXunBaoRewardConfig * GetRandActivityMiJingXunBao3PersonalRewardCfg(int seq, int chou_times);
	const RandActivityMiJingXunBaoConfig * GetMiJingXUnBao3BaodiRewardCfg(int server_chou_times);
	const RandActivityServerBaodiConfig * GetMiJingXunBao3BaodiRandreward(int server_chou_times);
	int GetMiJingXunBao3FreeTimes() { return m_mijingxunbao3_free_times; }
	int GetMiJingXunBao3TotalRewardNum() { return m_mijingxunbao3_reward_cfg_count; }

	// 欢乐砸蛋
	int GetHuanLeZaDanAutoWeightAddCount();
	int GetHuanLeZaDanAddWeightSeq(int index);
	int GetHuanLeZaDanCfgAddWeight(int seq);
	int GetHuanLeZaDanTaoGold(int type);
	int GetHuanLeZaDanInterval() { return m_huanlezadan_interval; }
	int GetHuanLeZaDanTaoItem() { return m_huanlezadan_thirtytimes_item_id; }
	int GetHuanLeZaDanAddWeightTaoTimes() { return m_huanlezadan_add_weight_tao_times; }
	bool GetHuanLeZaDanRandomResult(MiJingXunBaoAddWeightInfo *self_weight, int length, int is_free, RandActivityMiJingXunBaoConfig *result_list, int result_list_length);
	const RandActivityMiJingXunBaoRewardConfig * GetRandActivityHuanLeZaDanPersonalRewardCfg(int seq, int chou_times);
	const RandActivityMiJingXunBaoConfig * GetHuanLeZaDanBaodiRewardCfg(int server_chou_times);
	int GetHuanLeZaDanFreeTimes() { return m_huanlezadan_free_times; }
	int GetHuanLeZaDanTotalRewardNum() { return m_huanlezadan_reward_cfg_count; }

	// 欢乐摇奖
	int GetHuanLeYaoJiangAutoWeightAddCount();
	int GetHuanLeYaoJiangAddWeightSeq(int index);
	int GetHuanLeYaoJiangCfgAddWeight(int seq);
	int GetHuanLeYaoJiangTaoGold(int type);
	int GetHuanLeYaoJiangInterval() { return m_huanleyaojiang_interval; }
	int GetHuanLeYaoJiangTaoItem() { return m_huanleyaojaing_thirtytimes_item_id; }
	int GetHuanLeYaoJiangAddWeightTaoTimes() { return m_huanleyaojiang_add_weight_tao_times; }
	bool GetHuanLeYaoJiangRandomResult(MiJingXunBaoAddWeightInfo *self_weight, int length, int is_free, RandActivityMiJingXunBaoConfig *result_list, int result_list_length);
	const RandActivityMiJingXunBaoRewardConfig * GetRandActivityHuanLeYaoJiangPersonalRewardCfg(int seq, int chou_times);
	const RandActivityMiJingXunBaoConfig * GetHuanLeYaoJiangBaodiRewardCfg(int server_chou_times);
	const RandActivityServerBaodiConfig * GetHuanLeYaoJiangBaodiRandReward(int server_chou_times);
	int GetHuanLeYaoJiangFreeTimes() { return m_huanleyaojiang_free_times; }
	int GetHuanLeYaoJiangTotalRewardNum() { return m_huanleyaojiang_reward_cfg_count; }
	
	// 买一送一
	const BuyOneGetOneFreeItemCfg* GetBuyOneGetOneFreeCfg(int opengame_day, int item_index);


	// 限时回馈
	const RandActivityLimitTimeRebateCfg *GetLimitTimeRebateCfg();
	const LimitTimeRebateItem *GetLimitTimeRebateItemCfg(int seq);

	// 限时礼包
	const RandActivityLimitTimeGiftCfg::SubItemCfg * GetLimitTimeGiftRewardCfg(int opengame_day, const int vip_level, long long recharge_gold);
	const RandActivityLimitTimeGiftCfg::SubItemCfg * GetLimitTimeGiftRewardCfgBySeq(int opengame_day, const int seq);

	//循环充值2
	int GetCirculationChongzhiSecondNeedChongzhi() { return m_circulation_chongzhi_2_need_chongzhi; }
	const ItemConfigData * GetCirculationChongzhiSecondRewardItem(int level);

	// 疯狂摇钱树
	const RandActivityShakeMoneyConfig * GetShakeMoneyCfg(int opengame_day);

	// 限时豪礼
	const RandActivityLimitTimeLuxuryGiftBagCfg::SubItemCfg * GetLimitTimeLuxuryGiftBagRewardCfg(int opengame_day, const int vip_level);
	const RandActivityLimitTimeLuxuryGiftBagCfg::SubItemCfg * GetLimitTimeLuxuryGiftBagRewardCfgBySeq(int opengame_day, const int seq);

	// RMB购买
	const RandActivityRmbBuyChestShopOpenDayCfg::RmbRewardCfg * GetRmbBuyChestShopCfg(int rmb_num);

	// 消费返礼
	const RandActivityConsumeRewardStageCfg * GetConsumeRewardCfg(int vip_level, int activity_day = 0);

	// 集月饼活动
	const RandActivityItemCollectionSecondRewardCfg::ConfigItem * GetItemCollectionSecondRewardCfg(int seq);

	// 版本累计充值
	const RandActivityTotalChargeRewardConfig * GetVersionGrandotalChargeRewardCfg();
	int GetVersionTotalChargeSeqWithCharge(int total_charge);
	const RandActivityTotalChargeRewardConfig::ConfigItem * GetVersionTotalChargeRewardWithSeq(int seq);

	// 版本连续充值
	const RandActivityContinueChongzhiRewardConfig * GetVersionContinueChargeRewardInfoWithdayindex(int day_index, bool is_chongzhi, bool is_bu_fa_reward);

	// 欢乐摇奖2
	int GetHuanLeYaoJiang2AutoWeightAddCount();
	int GetHuanLeYaoJiang2AddWeightSeq(int index);
	int GetHuanLeYaoJiang2CfgAddWeight(int seq);
	int GetHuanLeYaoJiang2TaoGold(int type);
	int GetHuanLeYaoJiang2Interval() { return m_huanleyaojiang_2_interval; }
	int GetHuanLeYaoJiang2TaoItem() { return m_huanleyaojaing_2_thirtytimes_item_id; }
	int GetHuanLeYaoJiang2AddWeightTaoTimes() { return m_huanleyaojiang_2_add_weight_tao_times; }
	bool GetHuanLeYaoJiang2RandomResult(MiJingXunBaoAddWeightInfo *self_weight, int length, int is_free, RandActivityMiJingXunBaoConfig *result_list, int result_list_length);
	const RandActivityMiJingXunBaoRewardConfig * GetRandActivityHuanLeYaoJiang2PersonalRewardCfg(int seq, int chou_times);
	const RandActivityMiJingXunBaoConfig * GetHuanLeYaoJiang2BaodiRewardCfg(int server_chou_times);
	int GetHuanLeYaoJiang2FreeTimes() { return m_huanleyaojiang_2_free_times; }
	int GetHuanLeYaoJiang2TotalRewardNum() { return m_huanleyaojiang_2_reward_cfg_count; }

	// 线下单笔充值0
	const RandActivityOfflineSingleChargeCfg::ConfigItem * GetOfflineSingleChargeCfg0(long long charge_value, unsigned int join_timestamp = 0);

	// 节日守护
	HolidayGuardKillRankReward::ConfigItem *GetHolidayGuardKillRankRewardCfg(int role_rank);
	ItemConfigData &GetHolidayGuardParticipationRewardCfg() { return m_holiday_guard_participation_reward; }

	// 累计充值5
	const RandActivityTotalCharge5Cfg::ConfigItem * GetTotalCharge5Cfg(int day_idx, int seq);
	int GetTotalCharge5CfgCount(int day_idx);
	bool CheckIsResetTotalCharge5Day(int day_idx);

	// 极限挑战
	int GetExtremeChallengeRefreshNeedGold() { return m_extreme_challenge_refresh_task_need_gold; }
	int GetExtremeChallengeMaxTaskNum() { return m_extreme_challenge_rand_give_role_task_num; }
	int GetExtremeChallengeRandTaskByType(char type);
	const RandActivityExtremeChallengeCfg *GetExtremeChallengePerformTaskInfoCfg(int task_id);
	ItemConfigData *GetExtremeChallengeFinishAllTaskRewardCfg() { return m_extreme_challenge_finish_all_task_reward_cfg; }

	// 你充我送
	RandActivityChongZhiGiftConfig *GetChongZhiGiftByIndex(int index);
	const RandActivityChongZhiGiftConfig *GetChongZhiGiftCfg(int act_day, int index);

	// 表白排行榜
	int GetProfessRankCfgCount();
	const RandActivityProfessRankCfg::ConfigItem * GetProfessRankRewardCfg(int user_rank);

	// 组合购买
	int GetCombineBuyDiscount(int item_count);
	const RandActivityCombineBuyItemCfg::ItemCfg * GetCombineBuyItemCfg(int seq);

	const std::vector<RandActivityLuckyWishLotteryCfg> *GetLuckyWishLotteryCfgVec();
	int GetLuckyWishLotteryTotalWeight();
	const RandActivityLuckyWishLuckyCfg *GetLuckyWishLuckyCfg();
	const RandActivityLuckyWishLotteryCfg * GetLuckyWishLotteryBaodiCfg(int server_chou_times) const;
	void GetImageCompetitionRankInfo(int *rank_type, int *rank_id);
	const RandActivityImageCompetition * GetImageCompetitionStruct(int move_day);
	int GetImageCompetitionOpengameDay();
	const RandActivityImageCompetitionReward::RewardCfg *GetCompetitionRankReward(int rank_pos, long long rank_value);

	// 周末狂欢-----------------------------------------------
	const RALottery1OtherCfg &GetLottery1OtherCfg() { return m_lottery1_other_cfg; }
	const RALottery1RewardCfg *GetLottery1RewardWeightCfg(int opengame_day);
	const RALottery1RewardItem *GetLottery1RewardCfg(int opengame_day, int seq);
	const RALottery1PersonRewardCfg *GetLottery1PersonRewardCfg(int opengame_day, int seq);
	const RALottery1ServerRewardItem *GetLottery1ServerRewardCfg(int opengame_day, int total_server_times);

	// 零元购-----------------------------------------------
	const RAZeroBuyReturnConfig *GetRAZeroBuyReturnConfig(int buy_type);
	
	const RandActivityDailyLoveConfig * GetDailyLove2RewardPrecent();

	// 狂嗨庆典-----------------------------------------------
	int GetKuangHaiTaskNum();
	const RandActivityKuangHaiCfg::ItemCfg* GetKuangHaiTaskItem(int index);
	const std::vector<const RandActivityKuangHaiCfg::ItemCfg*>* GetKuangHaiTaskAllItems(int task_id);
	int GetKuangHaiRewardListNum();
	int GetKuangHaiRewardNum(int seq);
	const RandActivityKuangHaiRewardCfg::ItemCfg* GetKuangHaiRewardItem(int seq);

	// 礼物收割-----------------------------------------------
	const GiftHarvestRankRewardCfg::ConfigItem * GetGiftHarvestRankItem(int rank);
	ItemConfigData *GetGiftHarvestJoinRewardCfg() { return m_gift_harvest_join_reward_cfg; }

	int GetLuckyWisk30TimesConsumeItem() { return m_lucky_wish_30_times_use_item; }

	// 幸运云购
	const RandLuckyCloudBuyCfg::RoundItem* GetLuckyCloudBuyCfg(int rounds_index, unsigned int join_timestamp = 0);
	int GetLuckyBuyRoundsNum(unsigned int join_timestamp = 0);// 获取本日的云购轮数
	bool IsLuckyBuyUnlimitTime();// 是否在无限制购买的时间
	bool IsLuckyBuyBroadcast() { return m_lucky_buy_other.is_broadcast != 0; }
	int GetLuckyBuylimit() { return m_lucky_buy_other.buy_limit; }
	int GetLuckyBuyLuckyRewardBuyPercent() { return m_lucky_buy_other.huge_reward_need_percent; }

	int GetFirstChargeTuanRefreshTime();
	int GetFirstChargeTuanFixChargeTotalNum(int time);
	int GetFirstChargeTuanFixNumOnInit();

	const RandActivityCrazyBuy::ContentCfg * GetCracyBuyCfg(int seq);
	int GetCracyBuyLevelCfg(int chongzhi);
	
	const RandActivityDoubleGet::ConfigItem * GetDoubleGetCfg(int gold_num, int flag);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitLuckyRollCfg(PugiXmlNode RootElement);
	int InitDayChongZhiFanLiCfg(PugiXmlNode RootElement);
	int InitDayConsumeGoldCfg(PugiXmlNode RootElement);
	int InitServerPanicBuyCfg(PugiXmlNode RootElement);
	int InitPersonalPanicBuyCfg(PugiXmlNode RootElement);
	int InitChongzhiRankRewardCfg(PugiXmlNode RootElement);
	int InitChongzhiRank2RewardCfg(PugiXmlNode RootElement);
	int InitConsumeGoldRankRewardCfg(PugiXmlNode RootElement);
	int InitEquipStrengthenRewardCfg(PugiXmlNode RootElement);
	int InitChestshopRewardCfg(PugiXmlNode RootElement);
	int InitTotalConsumeGoldRewardCfg(PugiXmlNode RootElement);
	int InitDayActiveDegreeRewardCfg(PugiXmlNode RootElement);
	int InitStoneUplevelRewardCfg(PugiXmlNode RootElement);
	int InitXiannvChanmianUplevelRewardCfg(PugiXmlNode RootElement);
	int InitMountUpgradeRewardCfg(PugiXmlNode RootElement);
	int InitQibingUpgradeRewardCfg(PugiXmlNode RootElement);
	int InitMentalityTotalLevelRewardCfg(PugiXmlNode RootElement);
	int InitWingUpgradeRewardCfg(PugiXmlNode RootElement);
	int InitDayOnlineGiftCfg(PugiXmlNode RootElement);
	int InitQuanminQifuRewardCfg(PugiXmlNode RootElement);
	int InitXianmengBipinRewardCfg(PugiXmlNode RootElement);
	int InitXianmengJueQiRewardCfg(PugiXmlNode RootElement);
	int InitLoginGiftRewardCfg(PugiXmlNode RootElement);
	int InitLoginGiftRewardCfg_0(PugiXmlNode RootElement);
	int InitEverydayNiceGiftCfg(PugiXmlNode RootElement);
	int InitShouYouYuXiangRewardCfg(PugiXmlNode RootElement);
	int InitRankBipinRewordCfg(PugiXmlNode RootElement);
	int InitAllBipinRewordCfg(PugiXmlNode RootElement);
	int InitChargeRepaymentCfg(PugiXmlNode RootElement);
	int InitSingleChargeCfg(PugiXmlNode RootElement);
	int InitSingleCharge1Cfg(PugiXmlNode RootElement);
	int InitSingleCharge2Cfg(PugiXmlNode RootElement);
	int InitSingleCharge3Cfg(PugiXmlNode RootElement);
	int InitCornucopiaCfg(PugiXmlNode RootElement);
	int InitDanbiChongzhiCfg(PugiXmlNode RootElement);
	int InitTotalChargeDayCfg(PugiXmlNode RootElement);
	int InitTomorrowRewardCfg(PugiXmlNode RootElement);
	int InitDayChongzhiRankCfg(PugiXmlNode RootElement);
	int InitDayConsumeRankCfg(PugiXmlNode RootElement);
	int InitTotalChongzhiCfg(PugiXmlNode RootElement);
	int InitTimeLimitExchangeEquiCfg(PugiXmlNode RootElement);
	int InitTimeLimitExchangeJLCfg(PugiXmlNode RootElement);
	int InitLevelLotteryOtherCfg(PugiXmlNode RootElement);
	int InitLevelLotteryConsumeCfg(PugiXmlNode RootElement);
	int InitLevelLotteryRewardCfg(PugiXmlNode RootElement);
	int InitLevelLotteryTotalTimesCfg(PugiXmlNode RootElement);
	int InitNiuEggCfg(PugiXmlNode RootElement);
	int InitZhenbaogeCfg(PugiXmlNode RootElement);
	int InitChargeRewardGold(PugiXmlNode RootElement);
	int InitMiJingXunBaoCfg(PugiXmlNode RootElement);
	int InitMiJingXunBaoRewardCfg(PugiXmlNode RootElement);
	int InitJiSuChongZhanCfg(PugiXmlNode RootElement);
	int InitMoneyTreeCfg(PugiXmlNode RootElement);
	int InitMoneyTreeBaodiCfg(PugiXmlNode RootElement);
	int InitKingDrawLevelCfg(PugiXmlNode RootElement);
	int InitKingDrawReturnRewardCfg(PugiXmlNode RootElement);
	int InitTotalChongzhi3Cfg(PugiXmlNode RootElement);
	int InitAppearacePassiveRankCfg(PugiXmlNode RootElement);
	int InitAppearaceRankCfg(PugiXmlNode RootElement);
	int InitAnyMonsterDropCfg(PugiXmlNode RootElement);
	int InitTotalChongzhi4Cfg(PugiXmlNode RootElement);
	int InitMineInfoCfg(PugiXmlNode RootElement);
	int InitMineServerRewardCfg(PugiXmlNode RootElement);
	int InitGuaGuaCfg(PugiXmlNode RootElement);
	int InitGuaGuaAccRewardCfg(PugiXmlNode RootElement);
	int InitGuaGuaBaodiRewardCfg(PugiXmlNode RootElement);
	int InitTianMingDivinationRewardCfg(PugiXmlNode RootElement);
	int InitTianMingDivinationGoldConsumeCfg(PugiXmlNode RootElement);
	int InitFanFanItemInfoCfg(PugiXmlNode RootElement);
	int InitFanFanWordInfoCfg(PugiXmlNode RootElement);
	int InitFanFanBaodiCfg(PugiXmlNode RootRlement);
	int InitContinueChongzhiRewardCfg(PugiXmlNode RootElement);
	int InitContinueChongzhiRewardChuCfg(PugiXmlNode RootElement);
	int InitContinueChongzhiRewardGaoCfg(PugiXmlNode RootElement);
	int InitContinueChongzhiReward2Cfg(PugiXmlNode RootElement);
	int InitContinueConsumeRewardCfg(PugiXmlNode RootElement);
	int InitArmyDayExchangeFlagCfg(PugiXmlNode RootElement);
	int InitArmyDayExchangeItemCfg(PugiXmlNode RootElement);
	int InitCirculationChongzhiRewardCfg(PugiXmlNode RootElement);
	int InitExtremeLuckyDrawConsumeCfg(PugiXmlNode RootElement);
	int InitExtremeLuckyDrawRewardCfg(PugiXmlNode RootElement);
	int InitExtremeLuckyDrawReturnRewardCfg(PugiXmlNode RootElement);
	int InitMiJingXunBao2Cfg(PugiXmlNode RootElement);
	int InitMoneyTree2Cfg(PugiXmlNode RootElement);
	int InitNationalDayCfg(PugiXmlNode RootElement);
	int InitZhenbaoge2Cfg(PugiXmlNode RootElement);
	int InitPromotingPositionOutsideCfg(PugiXmlNode RootElement);
	int InitPromotingPositionInsideCfg(PugiXmlNode RootElement);
	int InitPromotingReturnRewardCfg(PugiXmlNode RootElement);
	int InitBlackMarketCfg(PugiXmlNode RootElement);
	int InitTreasuresMallCfg(PugiXmlNode RootElement);
	int InitTreasuresMallExchangeCfg(PugiXmlNode RootElement);
	int InitOpenServerRewardCfg(PugiXmlNode RootElement);
	int InitNewCornucopiaCfg(PugiXmlNode RootElement);
	int InitNewCornucopiaTotalCfg(PugiXmlNode RootElement);
	int InitItemCollectionRewardCfg(PugiXmlNode RootElement);
	int InitExpRefineRewardCfg(PugiXmlNode RootElement);
	int InitRedEnvelopeGift(PugiXmlNode RootElement);
	int InitMarryMeCfg(PugiXmlNode RootElement);
	int InitGoldenPigOtherCfg(PugiXmlNode RootElement);
	int InitGoldenPigRewardCfg(PugiXmlNode RootElement);
	int InitOpenServerInvestOtherCfg(PugiXmlNode RootElement);
	int InitOpenServerInvestRewardCfg(PugiXmlNode RootElement);
	int InitSingleChongZhiRewardCfg(PugiXmlNode RootElement);
	int InitXianyuanTreasCfg(PugiXmlNode RootElement);
	int InitRushBuyingPhaseCfg(PugiXmlNode RootElement);
	int InitSingleChargeCfg2(PugiXmlNode RootElement);
	int InitSingleChargeCfg3(PugiXmlNode RootElement);
	int InitSingleChargeCfg4(PugiXmlNode RootElement);
	int InitSingleChargeCfg5(PugiXmlNode RootElement);
	int InitMapHuntCityCfg(PugiXmlNode RootElement);
	int InitMapHuntRouteCfg(PugiXmlNode RootElement);
	int InitMapHuntServerRewardCfg(PugiXmlNode RootElement);
	int InitLightTowerExploreCfg(PugiXmlNode RootElement);
	int InitLightTowerExploreExternCfg(PugiXmlNode RootElement);
	int InitLightTowerExploreServerCfg(PugiXmlNode RootElement);
	int InitNewTotalChongzhiCfg(PugiXmlNode RootElement);
	int InitNewTotalChongzhi1Cfg(PugiXmlNode RootElement);
	int InitNewTotalChongzhi2Cfg(PugiXmlNode RootElement);
	int InitNewTotalChongzhi3Cfg(PugiXmlNode RootElement);
	int InitMagicShopCfg(PugiXmlNode RootElement);
	int InitPlantingTreeRankRewardCfg(PugiXmlNode RootElement);
	int InitFishInfoCfg(PugiXmlNode RootElement);
	int InitFishRankRewardCfg(PugiXmlNode RootElement);
	int InitCrtstalDalaranRewardCfg(PugiXmlNode RootElement);
	int InitChongzhiCrazyRabateCfg(PugiXmlNode RootElement);
	int InitJinJieThemeAct(PugiXmlNode RootElement);
	int InitConsumGiftCfg(PugiXmlNode RootElement);
	int InitConsumGiftRollPoolCfg(PugiXmlNode RootElement);
	int InitConsumGiftRollCfg(PugiXmlNode RootElement);
	int InitCriticalStrikeDayConfig(PugiXmlNode RootElement);
	int InitDailyLimitBuyConfig(PugiXmlNode RootElement);
	int InitCollectTreasureConfig(PugiXmlNode RootElement);
	int InitHappyCumulChongzhiConfig(PugiXmlNode RootElement);
	int InitJinJieReturnConfig(PugiXmlNode RootElement);
	int InitConsumeForGiftCfg(PugiXmlNode RootElement);
	int InitMiJingXunBao3Cfg(PugiXmlNode RootElement);
	int InitMiJingXunBao3RewardCfg(PugiXmlNode RootElement);
	int InitMiJingXunBao3BaodiCfg(PugiXmlNode RootElement);
	int InitHuanLeZaDanCfg(PugiXmlNode RootElement);
	int InitHuanLeZaDanRewardCfg(PugiXmlNode RootElement);
	int InitHuanLeYaoJiangCfg(PugiXmlNode RootElement);
	int InitHuanLeYaoJiangRewardCfg(PugiXmlNode RootElement);
	int InitHuanLeYaoJiangBaodiCfg(PugiXmlNode RootElement);
	int InitLimitTimeRebateCfg(PugiXmlNode RootElement);
	int InitLimitTimeGiftCfg(PugiXmlNode RootElement);
	int InitCirculationChongzhiSecondRewardCfg(PugiXmlNode RootElement);
	int InitShakeMoneyCfg(PugiXmlNode RootElement);
	int InitTimeLimitLuxuryGiftBag(PugiXmlNode RootElement);
	int InitConsumeRewardCfg(PugiXmlNode RootElement);
	int InitRmbBuyChestShopCfg(PugiXmlNode RootElement);
	int InitItemCollectionSecondRewardCfg(PugiXmlNode RootElement);
	int InitGrandTotalChargeCfg(PugiXmlNode RootElement);
	int InitContinueChargeRewardChuCfg(PugiXmlNode RootElement);
	int InitHuanLeYaoJiang2Cfg(PugiXmlNode RootElement);
	int InitHuanLeYaoJiang2RewardCfg(PugiXmlNode RootElement);
	int InitOfflineSingleChargeCfg0(PugiXmlNode RootElement);
	int InitBuyOneGetOneFreeCfg(PugiXmlNode RootElement);
	int InitHolidayGuardKillRankRewardCfg(PugiXmlNode RootElement);
	int InitTotalChongzhi5Cfg(PugiXmlNode RootElement);
	int InitExtremeChallengeCfg(PugiXmlNode RootElement);
	int InitChongZhiGiftCfg(PugiXmlNode RootElement);
	int InitProfessRankRewardCfg(PugiXmlNode RootElement);
	int InitJinJieReturn2Config(PugiXmlNode RootElement);
	int InitCriticalStrikeDay2Config(PugiXmlNode RootElement);
	int InitCombineBuyDiscountCfg(PugiXmlNode RootElement);
	int InitCombineBuyItemCfg(PugiXmlNode RootElement);
	int InitLuckyWishLotteryCfg(PugiXmlNode RootElement);
	int InitLuckyWishLuckyCfg(PugiXmlNode RootElement);
	int InitImageCompetitionCfg(PugiXmlNode RootElement);
	int InitImageCompetitionRewardCfg(PugiXmlNode RootElement);
	int InitDailyLoveRewardPercentCfg(PugiXmlNode RootElement);
	int InitZeroBuyReturnCfg(PugiXmlNode RootElement);
	int InitDailyLove2Cfg(PugiXmlNode RootElement);

	int InitKuangHaiQingDianCfg(PugiXmlNode RootElement);
	int InitKuangHaiQingDianRewardCfg(PugiXmlNode RootElement);

	int InitGiftHarvestRankRwardCfg(PugiXmlNode RootElement);

	int InitLuckyCloudBuyCfg(PugiXmlNode RootElement);
	int InitLuckyCloudBuyOtherCfg(PugiXmlNode RootElement);
	int InitFirstChargeTuanCfg(PugiXmlNode RootElement);
	int InitCracyBuyCfg(PugiXmlNode  RootElement);
	int InitDoubleGetCfg(PugiXmlNode RootElement);

	RandActivityLuckyRollCfg m_luckyroll_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityDayChongZhiFanLiCfg m_day_chongzhi_fanli_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityServerPanicBuyCfg m_server_panic_buy_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityPersonalPanicBuyCfg m_personal_panic_buy_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityDayConsumeGoldCfg m_day_consume_gold_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityChestShopCfg m_chestshop_reward_cfg;
	RandActivityStoneUplevelCfg m_stone_uplevel_cfg;
	RandActivityChongzhiRankCfg m_chongzhi_rank_reward_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityChongzhiRankCfg m_chongzhi_rank2_reward_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityConsumeGoldRankCfg m_consume_gold_rand_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityEquipStrengthenCfg m_strengthen_reward_cfg;
	RandActivityTotalConsumeGoldCfg m_diff_total_consume_gold_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	
	RandActivityDayActiveDegreeCfg m_day_active_degree_reward_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityXiannvChanmianUplevelCfg m_xiannv_chanmian_uplevel_cfg;
	RandActivityMountUpgradeCfg m_mount_upgrade_cfg;
	RandActivityQibingUpgradeCfg m_qibing_upgrade_cfg;
	RandActivityMentalityTotalLevelCfg m_mentality_total_level_cfg;
	RandActivityWingUpgradeCfg m_wing_upgrade_cfg;
	RandActivityQuanminQifuCfg m_quanmin_qifu_cfg;
	RandActivityXianmengBipinCfg m_xianmeng_bipin_cfg;
	RandActivityXianmengJueQiCfg m_xianmeng_jueqi_cfg;
	RandActivityLoginGiftCfg m_login_gift_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityShouYouYuXiangCfg m_shouyou_yuxiang_cfg;
	
	int m_consume_gold_fanli_rate;
	RandActivityKillBossCfg m_kill_boss_reward_cfg;
	RandActivityDayOnlineGiftCfg m_day_online_gift_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	int m_chestshop_discount;
	int m_quanmin_qifu_discount;

	int m_rank_bipin_cfg_count;
	RandActivityRankBipinCfg m_rank_bipin_cfglist[RAND_ACTIVITY_BIPIN_ACTIVITY_MAX_COUNT];

	int m_all_bipin_cfg_count;
	RandActivityAllBipinCfg m_all_bipin_cfglist[RAND_ACTIVITY_BIPIN_ACTIVITY_MAX_COUNT];

	RandActivityChargeRepaymentCfg m_charge_repayment_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivitySingleChargeCfg m_single_charge_cfg[RAND_ACTIVITY_SINGLE_CHAR_CFG_COUNT];

	RandActivitySingleChargeCfg m_single_charge1_cfg[RAND_ACTIVITY_SINGLE_CHAR_CFG_COUNT];
	RandActivitySingleChargeCfg m_single_charge2_cfg[RAND_ACTIVITY_SINGLE_CHAR_CFG_COUNT];
	RandActivitySingleChargeCfg m_single_charge3_cfg[RAND_ACTIVITY_SINGLE_CHAR_CFG_COUNT];

	int m_cornucopia_max_lun;
	RandActivityCornucopiaCfg m_cornucopia_cfglist[RAND_ACTIVITY_CORNUCOPIA_MAX_LUN + 1];
	
	//单笔充值奖励
	RandActivityDanbiChongzhiDayDiffCfg m_diff_danbi_chongzhi_day_cfglist[RAND_ACTIVITY_DANBI_CHONGZHI_DAY_CFG_COUNT];

	UNSTD_STATIC_CHECK(RAND_ACTIVITY_TOTAL_CHARGE_DAY_CFG_MAX_SEQ < 16);

	int m_total_charge_day_max_seq;
	RandActivityTotalChargeDayCfg m_total_charge_day_cfglist[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	int m_tomorrow_reward_need_active_degree;
	int m_tomorrow_reward_cfg_total_weight;
	int m_tomorrow_reward_cfg_count;
	RandActivityTomorrowRewardCfg m_tomorrow_reward_cfglist[RAND_ACTIVITY_TOMORROW_REWARD_CFG_MAX_COUNT];

	int m_person_rank_bipin_activity_map[PERSON_RANK_TYPE_MAX];

	// 每日充值排行
	RandActivityDayChongzhiRankRewardDiffConfig m_diff_daychongzhirank_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	// 每日消费排行
	RandActivityDayConsumeRankRewardDiffConfig m_diff_dayconsume_rank_reward_cfglist[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	
	//累计充值
	RandActivityTotalChargeRewardConfig m_diff_totalcharge_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	//限时--积分兑换稀有装备
	RandActivityTimeLimitExchangeConfig m_time_limit_exchange_equi_list[TIME_LIMIT_EXCHANGE_ITEM_COUNT];
	RandActivityTimeLimitExchangeConfig m_time_limit_exchange_jl_list[TIME_LIMIT_EXCHANGE_ITEM_COUNT];

	//金银塔抽奖活动
	int m_level_lottery_max_level;											// 金银塔抽奖活动，最大层数
	int m_level_lottery_consume_gold_level_list[RA_LEVEL_LOTTERY_MAX_LEVEL_LIMIT + 1];

	ItemID m_level_lottery_consume_item;									// 金银塔10抽代替道具
	int m_level_lottery_cfg_list_count;										// 金银塔抽奖活动，配置数量
	RandActivityAdditionLotteryConfig m_level_lottery_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	RandActivityLevelLotteryOtherCfg m_level_lottery_other_cfg;				// 金银塔其他配置

	std::map<int, std::map<int, RandActivityLevelLotteryTotalTimesCfg> > m_level_lottery_total_times_map; // 金银塔累计次数奖励
	 
	//充值扭蛋活动
	int m_niu_egg_need_charge;												// 扭蛋一次所需充值额度

	int m_niu_egg_chou_total_weight;
	int m_niu_egg_chou_reward_cfg_count;
	RandActivityNiuEggChouRewardConfig m_niu_egg_chou_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		// 扭蛋抽奖物品列表

	int m_niu_egg_server_reward_cfg_count;
	RandActivityNiuEggServerRewardConfig m_niu_egg_server_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];// 扭蛋全服奖励物品列表

	//珍宝阁
	int m_zhenbaoge_reflush_gold;																				// 珍宝阁刷新一次消耗元宝
	int m_zhenbaoge_flush_item;																					// 珍宝阁刷新1次钥匙
	int m_zhenbaoge_auto_flush_times;																			// 珍宝阁一键刷新的次数
	ItemConfigData m_zhenbaoge_all_buy_reward;																	// 珍宝阁全购买赠送礼包

	int m_zhenbaoge_auto_add_cfg_count;																			// 珍宝阁自增权重配置数量
	RandActivityZhenBaoGeAddWeightDayCfg m_zhenbaoge_add_weight_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	int m_zhenbaoge_max_seq;
	RandActivityZhenBaoGeDayCfg m_zhenbaoge_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM + 1];				// 珍宝阁刷新物品列表

	int m_zhenbaoge_reward_max_seq;
	RandActivityZhenBaoGeServerDayCfg m_zhenbaoge_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM + 1];   // 珍宝阁全服礼包列表

	//充值返元宝
	typedef std::vector<RandActivityChargeRewardGoldConfig> ChargeRewardGoldVec;
	ChargeRewardGoldVec m_charge_reward_gold_vec;

	//秘境寻宝
	int m_mijingxunbao_once_gold;																			//秘境寻宝淘宝一次消耗非绑定元宝      
	int m_mijingxunbao_tentimes_gold;																		//秘境寻宝淘宝十次消耗非绑定元宝  
	int m_mijingxunbao_fifthtimes_gold;																		//秘境寻宝淘宝五十次消耗非绑定元宝  
	int m_mijingxunbao_interval;																			//秘境寻宝淘宝免费时间间隔(分钟)
	int m_add_weight_tao_times;																				//秘境寻宝增加权重需要淘不到珍稀物品次数
	int m_mijingxunbao_fifthtimes_item_id;																	//秘境寻宝淘宝五十次消耗物品 

	int m_mijingxunbao_auto_add_cfg_count_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];																	// 秘境寻宝自增权重配置数量
	RandActivityMiJingXunBaoAddWeightCfgList m_mijingxunbao_add_weight_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	
	int m_mijingxunbao_reward_cfg_count;																	
	RandActivityMiJingXunBaoRewardDayCfg m_mijingxunbao_reward_list_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		//秘境寻宝累抽奖励

	int m_mijingxunbao_max_seq_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityMiJingXunBaoCfgList m_mijingxunbao_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// 秘境寻宝物品列表

	//急速冲战
	RandActivityJiSuChongZhanCfg m_jisuchongzhan_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// 急速冲战奖励配置表

	//摇钱树
	int m_rock_money_tree_need_gold;												// 摇钱树一次需要的元宝
	int m_money_tree_init_pool_gold;												// 摇钱树奖池初始金额
	int m_money_tree_into_pool;														// 摇钱树一次进入奖池的元宝
	int m_money_tree_free_times;													// 摇钱树 免费次数
	int m_money_tree_free_interval;													// 摇钱树 免费间隔
	ItemID m_money_tree_consume_item;												// 摇钱树10次顶替道具

	int m_money_tree_max_stage;														// 摇钱树 最大段数

	int m_money_tree_total_weight_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	int m_money_tree_item_reward_total_weight_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	int m_money_tree_reward_cfg_count_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityMoneyTreeRewardCfgList m_money_tree_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		// 摇钱树抽奖物品列表

	RandActivityServerBaodiCfgList m_money_tree_baodi_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	int m_money_tree_server_reward_cfg_count_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityMoneyTreeServerRewardCfgList m_money_tree_server_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];  // 摇钱树全服物品列表

	// 陛下请翻牌
	int m_kingdraw_level_chuji_consume;								// 陛下请翻牌，初级消耗配置信息
	int m_kingdraw_level_zhongji_consume;							// 陛下请翻牌，中级消耗配置信息
	int m_kingdraw_level_gaoji_consume;								// 陛下请翻牌，高级消耗配置信息
	ItemID m_king_draw_gaoji_consume_item;							// 陛下请翻牌，高级顶替道具
	RandActivityKingDrawLevelConfig m_kingdraw_level_cfg_list[RA_KING_DRAW_LEVEL_COUNT][MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];	// 陛下请翻牌，奖励配置信息
	int m_kingdraw_return_reward_cfg_count[RA_KING_DRAW_LEVEL_COUNT];
	RandActivityKingDrawReturnRewardCfg m_kingdraw_return_reward_cfg_list[RA_KING_DRAW_LEVEL_COUNT][MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];	// 陛下请翻牌，累抽奖励配置信息

	// 累计充值3
	RandActivityTotalCharge3Config m_total_charge_3_cfglist[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	//特殊形象变身榜
	ItemConfigData m_appearance_rank_join_reward;											// 变身排行榜活动，参与奖励
	ItemConfigData m_appearance_rank_reward_list[RA_SPECIAL_APPEARANCE_RANK_MAX_COUNT + 1]; // 变身排行榜活动排行奖励列表

	//被动变身榜
	ItemConfigData m_appearance_passive_rank_join_reward;							// 被动变身排行榜活动，参与奖励
	ItemConfigData m_appearance_passive_rank_reward_list[RA_SPECIAL_APPEARANCE_PASSIVE_RANK_MAX_COUNT + 1];// 被动变身排行榜活动排行奖励列表

	//全场怪物掉落
	unsigned int m_any_monster_drop_limit_drop_time;						// 全场景怪物掉落活动,杀死任意怪物掉落物品的最小时间间隔
	UInt16 m_any_monster_drop_id;											// 全场景怪物掉落活动,掉落ID
	int m_any_monster_drop_day_limit_rate;									// 全场景怪物掉落活动,每日限制使用物品数量
	int m_any_monster_drop_cfg_list_count;									// 配置数量
	RandActivityAnyMonsterDropConfig m_any_monster_drop_cfg_list[RAND_ACTIVITY_ANY_MONSTER_DROP_CFG_COUNT];

	//累计充值4(新三件套)
	RandActivityTotalCharge4Config m_total_charge_4_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	//趣味挖矿3333333333
	int m_mine_refresh_gold;										// 挖矿，刷新矿石所需元宝
	int m_mine_refresh_count;										// 挖矿，一次刷新出的矿石数
	int m_mine_free_times;											// 挖矿，每天免费挖矿次数
	int m_mine_server_reward_level_limit;							// 挖矿，领取全服礼包等级限制
	int m_mine_info_cfg_count;										// 挖矿，矿石信息配置数
	RandActivityMineInfoConfig m_mine_info_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];						// 挖矿，矿石类型配置
	int m_mine_server_reward_cfg_count;								// 挖矿，全服礼包配置数
	RandActivityMineServerRewardConfig m_mine_server_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];	// 挖矿，个人礼包配置

	//	顶刮刮
	int m_guagua_free_time_interval;								// 顶刮刮，免费刮一次的间隔时间
	int m_guagua_need_gold_list[RA_GUAGUA_TYPE_MAX];				// 顶刮刮, 元宝花费列表
	std::map<int, int> m_guagua_total_weight;						// 顶刮刮, 配置总权重
	ItemID m_guagua_roll_item_id;										// 顶刮刮，30抽替代物
	std::vector<RandActivityGuaGuaConfig> m_guagua_cfg_list;		// 顶刮刮，所有奖励配置信息
	std::vector<RandActivityGuaGuaAccConfig> m_guagua_acc_cfg_list;

	RandActivityServerBaodiCfgList m_guagua_baodi_reward_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		//顶刮刮全服保底

	// 天命卜卦
	int m_tianming_free_chou_times;									// 天命卜卦, 每天免费卜卦次数
	int m_tianming_reward_total_weight;								// 天命卜卦，配置总权重
	int m_tianming_reward_max_seq;									// 天命卜卦, 奖池最大下标
	RandActivityTianMingDivinationRewardConfig m_tianming_reward_cfg_list[RAND_ACTIVITY_TIANMING_DIVINATION_REWARD_MAX_SEQ]; // 天命卜卦奖励配置信息
	RandActivityTianMingDivinationBaodiConfig m_tianming_baodi_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];				// 天命卜卦保底次数奖励

	int m_tianming_reward_add_lot_seq_count;						// 天命卜卦，竹签数量
	int m_tianming_reward_add_lot_seq_list[RA_TIANMING_LOT_COUNT];	// 天命卜卦, 可加注奖品下标数组
	int m_tianming_gold_consume_max_add_lot_times;					// 天命卜卦, 最大加注次数
	RandActivityTianMingDivinationGoldConsumeConfig m_tianming_gold_consume_cfg_list[RAND_ACTIVITY_TIANMING_DIVINATION_TOTAL_MAX_ADD_LOT_TIMES + 1];	// 天命卜卦元宝消耗信息

	// 翻翻转
	int m_fanfan_free_fan_times_per_day;							// 翻翻转，每天免费翻次数
	int m_fanfan_once_need_gold;									// 翻翻转，一次所需元宝
	int m_fanfan_refresh_need_gold;									// 翻翻转，重置所需元宝
	int m_fanfan_auto_refresh_interval;                             // 翻翻转，自动重置间隔（小时）
	int m_fanfan_word_increase_rate;								// 翻翻转，字组增长概率

	int m_fanfan_item_info_cfg_count;
	RandActivityFanFanItemDayCfg m_fanfan_item_info_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		// 翻翻转，物品信息配置

	int m_fanfan_word_info_cfg_count;
	RandActivityFanFanWordDayCfg m_fanfan_word_info_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		// 翻翻转，字组信息配置
	
	int m_fanfan_baodi_reward_cfg_count;
	RandActivityFanFanBaodiDayConfig m_fanfan_baodi_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];	// 翻翻转累抽奖励配置

	//连续充值
	ItemConfigData	m_continue_chongzhi_extra_reward;				// 连续充值，额外奖励
	int m_continue_chongzhi_fetch_extra_reward_need_days;			// 连续充值, 获取额外奖励所需的连续充值达标天数
	int m_continue_chongzhi_max_day_index;							// 连续充值，最大天数下标
	RandActivityContinueChongzhiRewardConfig m_continue_chongzhi_reward_cfg_list[RA_CONTINUE_CHONGZHI_MAX_DAY_INDEX + 1];

	//连充特惠初
	ItemConfigData	m_continue_chongzhi_extra_reward_chu;				// 连续特惠初，额外奖励
	int m_continue_chongzhi_fetch_extra_reward_need_days_chu;			// 连续特惠初, 获取额外奖励所需的连续充值达标天数
	std::vector<RandActivityContinueChongzhiRewardConfig> m_continue_chongzhi_reward_cfg_vec_chu;

	//连充特惠高
	ItemConfigData	m_continue_chongzhi_extra_reward_gao;				// 连续特惠高，额外奖励
	int m_continue_chongzhi_fetch_extra_reward_need_days_gao;			// 连续特惠高, 获取额外奖励所需的连续充值达标天数
	std::vector<RandActivityContinueChongzhiRewardConfig> m_continue_chongzhi_reward_cfg_vec_gao;

	//限时连充
// 	ItemConfigData	m_continue_chongzhi_extra_reward_2;				// 限时连充，额外奖励
// 	int m_continue_chongzhi_fetch_extra_reward_need_days_2;			// 限时连充, 获取额外奖励所需的连续充值达标天数
	std::vector<RandActivityContinueChongzhiRewardConfig> m_continue_chongzhi_reward_cfg_vec_2;

	//连续消费
	int m_continue_consume_other_day_cfg_count;
	RandActivityContinueConsumeOtherDayCfg m_continue_consume_other_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];	// 连续消费，其他配置
	int m_continue_consume_max_reward_day_cfg_count;																	// 连续消费，最大天数下标
	RandActivityContinueConsumeRewardDayCfg m_continue_consume_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	//军歌嘹亮
	RandActivityArmyDayExchangeFlagConfig m_army_day_exchange_flag_cfg_list[RA_ARMY_DAY_ACTIVE_DEGREE_EXCHANGE_NUM];			// 军歌嘹亮 活跃度兑换旗帜
	RandActivityArmyDayExchangeItemConfig m_army_day_exchange_item_cfg_list[RA_ARMY_DAY_ARMY_SIDE_NUM];							// 军歌嘹亮 旗帜兑换物品
	ItemConfigData m_army_day_win_side_reward;										// 军歌嘹亮 胜出方奖励

	//循环充值
	int m_circulation_chongzhi_max_reward_count;						// 循环充值，可领取奖励数
	RandActivityCirculationChongzhiRewardConfig m_circulation_chongzhi_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	//至尊幸运
	int m_extreme_lucky_flush_consume_glod;													// 刷新所需元宝
	int m_extreme_lucky_free_draw_times;
	RandActivityExtremeLuckyDrawConsumeConfig  m_extreme_lucky_draw_consume_gold_list[RA_EXTREME_LUCKY_DRAW_CONSUME_GOLD_CFG_COUNT];		//抽奖次数对应消耗元宝

	int m_extreme_lucky_reward_cfg_count_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	int m_extreme_lucky_total_weight_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];													// 抽奖物品总权重
	RandActivityExtremeLuckyRewardCfg m_extreme_lucky_draw_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];						//抽奖物品
	int m_extreme_lucky_return_reward_cfg_count;																					//返利配置數
	RandActivityExtremeLuckyReturnRewardCfg m_extreme_lucky_return_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			//返利配置

	//秘境寻宝2
	int m_mijingxunbao2_once_gold;																			//秘境寻宝淘宝一次消耗非绑定元宝      
	int m_mijingxunbao2_tentimes_gold;																		//秘境寻宝淘宝十次消耗非绑定元宝  
	int m_mijingxunbao2_fifthtimes_gold;																	//秘境寻宝淘宝五十次消耗非绑定元宝  
	int m_mijingxunbao2_interval;																			//秘境寻宝淘宝免费时间间隔(分钟)
	int m_add_weight_tao2_times;																			//秘境寻宝增加权重需要淘不到珍稀物品次数
	int m_mijingxunbao2_fifthtimes_item_id;																	//秘境寻宝淘宝五十次消耗物品 

	int m_mijingxunbao2_auto_add_cfg_count_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];																	// 秘境寻宝自增权重配置数量
	RandActivityMiJingXunBaoAddWeightCfgList m_mijingxunbao2_add_weight_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];


	int m_mijingxunbao2_max_seq_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityMiJingXunBaoCfgList m_mijingxunbao2_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// 秘境寻宝物品列表

	//摇钱树2
	int m_rock_money_tree2_need_gold;												// 摇钱树一次需要的元宝
	int m_money_tree2_init_pool_gold;												// 摇钱树奖池初始金额
	int m_money_tree2_into_pool;													// 摇钱树一次进入奖池的元宝

	int m_money_tree2_max_stage;														// 最大段数

	int m_money_tree2_total_weight_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	int m_money_tree2_item_reward_total_weight_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	int m_money_tree2_reward_cfg_count_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityMoneyTreeRewardCfgList m_money_tree2_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		// 摇钱树抽奖物品列表

	int m_money_tree2_server_reward_cfg_count_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityMoneyTreeServerRewardCfgList m_money_tree2_server_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];  // 摇钱树全服物品列表

	//欢度国庆
	int m_national_day_cfg_count;																			//兑换配置个数
	RandActivityNationalDayConfig m_national_day_exchange_cfg_list[RA_NATIONAL_DAY_EXCHANGE_CFG_COUNT];		//欢度国庆兑换配置列表

	//珍宝阁2
	int m_zhenbaoge2_reflush_gold;																				// 珍宝阁刷新一次消耗元宝
	int m_zhenbaoge2_auto_flush_times;																			// 珍宝阁一键刷新的次数
	ItemConfigData m_zhenbaoge2_all_buy_reward;																	// 珍宝阁全购买赠送礼包

	int m_zhenbaoge2_auto_add_cfg_count;																			// 珍宝阁自增权重配置数量
	RandActivityZhenBaoGeAddWeightDayCfg m_zhenbaoge2_add_weight_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	int m_zhenbaoge2_max_seq;
	RandActivityZhenBaoGeDayCfg m_zhenbaoge2_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM + 1];						// 珍宝阁刷新物品列表

	int m_zhenbaoge2_reward_max_seq;
	RandActivityZhenBaoGeServerDayCfg m_zhenbaoge2_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM + 1];   // 珍宝阁全服礼包列表

	// 步步高升
	int m_promoting_position_free_play_interval;						// 步步高升，免费抽时间间隔（小时）
	ItemID m_promoting_position_10_times_use_item;						// 步步高升，10抽替代物品
	int m_promoting_position_extra_time_per_chongzhi_gold;				// 步步高升，获取一次额外抽奖次数的抽奖金额
	int m_promoting_position_play_once_gold;							// 步步高升，单抽所需元宝
	int m_promoting_position_10_times_gold;								// 步步高升，10抽所需元宝

	int m_promoting_position_outside_max_seq;
	RandActivityPromotingPositionConfig m_promoting_position_outside_cfg_list[RA_PROMOTING_POSITION_OUTSIDE_REWARD_MAX_SEQ + 1];		// 步步高升，外圈奖励配置
	RandActivityPromotingPositionBaodiCfg m_promoting_position_outside_baodi_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// 步步高升，保底奖励配置

	int m_promoting_position_inside_max_seq;
	RandActivityPromotingPositionConfig m_promoting_position_inside_cfg_list[RA_PROMOTING_POSITION_INSIDE_REWARD_MAX_SEQ + 1];			// 步步高升，内圈奖励配置
	RandActivityPromotingPositionBaodiCfg m_promoting_position_inside_baodi_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// 步步高升，保底奖励配置

	int m_promoting_position_reward_cfg_count;
	RandActivityPromotingPositionRewardCfg m_promoting_position_reward_cfg_list[RA_PROMOTION_POSITION_REWARD_CFG_MAX_COUNT];			// 步步高升，抽奖次数返利配置
	

	// 黑市竞拍
	RandActivityBlackMarketConfig m_black_market_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		// 黑市拍卖配置

	// 珍宝商城
	RandActivityTreasuresMallBuyConfig m_treasures_mall_buy_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityTreasuresMallExchangeConfig m_treasures_mall_exchange_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	// 开服活动
	std::map<int, RandActivityOpenServerRewardConfig> m_openserver_reward_map;

	// 新聚宝盆
	int m_new_cornucopia_percent;
	RandActivityNewCornucopiaConfig m_new_cornucopia_cfg_list[RAND_ACTIVITY_NEW_CORNUCOPIA_CFG_COUNT_MAX];
	RandActivityNewCornucopiaTotalConfig m_new_cornucopia_total_cfg_list[RAND_ACTIVITY_NEW_CORNUCOPIA_TOTAL_CFG_COUNT_MAX];
	
	// 集字活动
	RandActivityItemCollectionRewardCfg m_item_collection_reward_cfg;

	// 经验炼制
	//RandActivityExpRefineRewardCfg m_exp_refine_cfg_list[RAND_ACTIVITY_EXP_REFINE_REWARD_MAX_COUNT];

	std::map<int, RandActivityExpRefineRewardCfg_2> m_exp_refine_cfg_map;

	// 红包好礼
	RandActivityRedEnvelopeGiftCfg m_red_envelope_gift_cfg;

	// 我们结婚吧
	RandActivityMarryMeCfg m_marryme_cfg;

	// 金猪召唤
	RandActivityGoldenPigOtherCfg m_golden_pig_other_cfg;
	std::vector<RandActivityGoldenPigRewardCfg> m_golden_pig_reward_vec_list[RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_MAX_COUNT];

	// 开服投资
	RandActivityOpenServerInvestOtherCfg m_openserver_invest_other_cfg[RandActivityOpenServerInvestOtherCfg::MAX_INVEST_TYPE];
	RandActivityOpenServerInvestRewardCfg m_openserver_invest_reward_cfg[RandActivityOpenServerInvestOtherCfg::MAX_INVEST_TYPE][RAND_ACTIVITY_OPEN_SERVER_INVEST_MAX_COUNT];

	// 单返豪礼
	RandActivitySingleChongZhiRewardCfg m_single_chongzhi_cfg;

	// 聚划算
	int m_xianyuan_treas_buy_limit_day;										// 仙缘宝库购买天数限制
	std::map<int,ItemConfigData> m_xianyuan_treas_all_buy_reward;			// 仙缘宝库全购买赠送礼包 
	int m_xianyuan_treas_cfg_count;
	RandActivityXianyuanTreasConfig m_xianyuan_treas_cfg_list[RAND_ACTIVITY_XIANYUAN_TREAS_CFG_COUNT];	// 仙缘宝库

	// 限时秒杀
	int m_rush_buying_duration;												// 限时秒杀-抢购持续时间（分钟）
	RandActivityRushBuyingPhaseDayConfig m_rush_buying_phase_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	// 极速冲战2
	RandActivitySingleChargeCfg m_single_charge_cfg2[RAND_ACTIVITY_SINGLE_CHARGE2_CFG_COUNT];

	// 极速冲战3
	RandActivitySingleChargeCfg m_single_charge_cfg3[RAND_ACTIVITY_SINGLE_CHARGE3_CFG_COUNT];

	// 极速冲战4
	RandActivitySingleChargeCfg m_single_charge_cfg4[RAND_ACTIVITY_SINGLE_CHARGE4_CFG_COUNT];

	// 极速冲战5
	RandActivitySingleChargeCfg m_single_charge_cfg5[RAND_ACTIVITY_SINGLE_CHARGE5_CFG_COUNT];

	//地图寻宝
	int m_map_hunt_free_xunbao_count;																		//每天免费次数
	int m_map_hunt_free_flush_interval;																		//自动刷新间隔（分钟）
	int m_map_hunt_xunbao_glod;																				//寻宝所需元宝
	int m_map_hunt_flush_gold;																				//刷新所需元宝
	int m_map_hunt_total_city_weight_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];							//城市总权重

	RandActivityMapHuntCityConfig m_map_hunt_city_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];						//寻宝地图城市配置
	RandActivityMapHuntRouteConfig m_map_hunt_route_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];					//地图寻宝路线配置
	RandActivityMapHuntServerRewardConfig m_map_hunt_server_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			//全服奖励配置

	//灯塔寻宝
	int m_light_tower_explore_one_gold;					//寻宝一次所需元宝
	int m_light_tower_explore_ten_gold;					//寻宝十次所需元宝

	RandActivityLightTowerExploreConfig m_light_tower_explore_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];								//灯塔寻宝
	RandActivityLightTowerExploreExternConfig m_light_tower_explore_extern_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			//每层额外奖励
	RandActivityLightTowerExploreServerConfig m_light_tower_explore_server_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			//灯塔寻宝全服奖励

	//新累计充值
	RandActivityNewTotalChargeRewardConfig m_diff_new_totalcharge_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	//新累计充值1
	RandActivityNewTotalChargeRewardConfig m_diff_new_totalcharge1_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	//新累计充值2
	RandActivityNewTotalChargeRewardConfig m_diff_new_totalcharge2_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	//新累计充值3
	RandActivityNewTotalChargeRewardConfig m_diff_new_totalcharge3_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	//幻装商店
	std::vector<RandActivityMagicShopConfig> m_magic_shop_cfg_list;

	//植树
	int m_planting_tree_tree_living_time;								// 趣味植树，树存活时间
	int m_planting_tree_max_watering_times;								// 趣味植树，一棵树最多浇水次数
	int m_planting_tree_gather_id;										// 趣味植树，采集物id
	int m_planting_tree_watering_time;									// 趣味植树，浇水(采集)花费时间
	int m_planting_tree_scene_id;										// 趣味植树，场景id
	ItemID m_planting_tree_norexitem_id;								// 趣味植树，主动使用类id
	ItemConfigData m_planting_tree_canyu_reward;						// 趣味植树，参与奖

	int m_planting_tree_rank_reward_count;								// 趣味植树，排行榜配置数
	RandActivityPlantingTreeRankRewardConfig m_planting_tree_rank_reward_cfg_list[RA_TREE_PLANTING_RANK_MAX_COUNT + 1];

	// 钓鱼
	short m_fish_time_limit;
	short m_steal_fish_percent;
	short m_steal_fish_time_limit;
	short m_be_stealed_fish_time_limit;
	RandActivityFishingInfoCfg m_fish_info_list[RA_FISHING_FISH_TYPE_COUNT + 1];
	RandActivityFishingRankCfg m_fish_rank_reward_list[RA_FISHING_RANK_MAX_COUNT + 1];

	//达拉然水晶
	int m_crystal_dalaran_initial_count;
	short m_steal_crystal_dalaran_time_limit;
	short m_crystal_dalaran_be_stealed_time_limit;
	short m_steal_crystal_dalaran_one_people_time_limit;
	short m_steal_crystal_dalaran_percent;
	short m_broadcast_percent;
	unsigned int m_steal_crystal_dalaran_limit;
	RandActivityStealCrtstalDalaranRewardCfg m_steal_crystal_dalaran_reward[RA_CRYSTAL_DALARAN_RANK_REWARD_COUNT + 1];

	// 狂返元宝配置
	typedef std::vector<RandActivityChongzhiCrazyRebateConfig> ChongzhiCrazyRebateVec;
	ChongzhiCrazyRebateVec m_chongzhi_crazy_rebate_vec;

	//////////////////////////////// 进阶主题日活动 ///////////////////////////////////////////
	std::map<long long, JinjieThemeActConfig> m_jinjie_theme_act_map;

	// 消费领奖配置
	int m_roll_reward_gold;          // 可摇奖消费数
	int m_roll_times_limit;
	typedef std::vector<ConsumGiftRewardConfig>::iterator ConsumGiftRewardVecIt;
	std::vector<ConsumGiftRewardConfig> m_consum_gift_vec;

	std::vector<ConsumGiftRollRewardPool> m_consum_gift_roll_reward_pool;
	typedef std::vector<ConsumGiftRollRewardConfig> ConsumGiftRollRewardCfgVec;
	ConsumGiftRollRewardCfgVec m_consum_gift_roll_reward_vec;

	// 每日限购
	DailyLimitBuyConfig m_daily_limit_buy_cfg[JINJIE_ACT_THEME_TYPE_MAX][RAND_ACTIVITY_DAILY_LIMIT_BUY_MAX_SEQ];

	// 聚宝盆（新）
	int m_collect_treasure_gold;
	int m_collect_treasure_total_rate;
	int m_collect_treasure_join_times;
	CollectTreasureConfig m_collect_treasure_cfg[RAND_ACTIVITY_COLLECT_TREASURE_MAX_CFG];

	// 欢乐累充
	HappyCumulChongzhiConfig m_happy_cumul_chongzhi_cfg[JINJIE_ACT_THEME_TYPE_MAX][RAND_ACTIVITY_HAPPY_CUMUL_CHONGZHI_MAX_CFG];

    //暴击日
	CriticalStrikeDayConfig m_critical_strike_day_cfg[JINJIE_ACT_THEME_TYPE_MAX];

	// 进阶返还
	JinJieReturnConfig m_jinjie_return_cfg[JINJIE_ACT_THEME_TYPE_MAX][RAND_ACTIVITY_JINJIE_RETURN_MAX_CFG];
	
	//陛下请翻牌（新增）
	//int m_kingdraw_level_primary_score;
	//int m_kingdraw_level_middle_score;
	//int m_kingdraw_level_high_score;

	// 消费换大礼
	int m_consume_for_gift_points_per_gold;								// 消费换大礼，每个元宝换几个积分
	std::map<int, int> m_consume_for_gift_item_count_map;
	std::vector<RandActivityConsumeForGiftItemConfig> m_consume_for_gift_item_cfg_list;

	//秘境寻宝3
	int m_mijingxunbao3_once_gold;																			//秘境寻宝淘宝一次消耗非绑定元宝      
	int m_mijingxunbao3_tentimes_gold;																		//秘境寻宝淘宝十次消耗非绑定元宝  
	int m_mijingxunbao3_thirtytimes_gold;																	//秘境寻宝淘宝三十次消耗非绑定元宝  
	int m_mijingxunbao3_interval;																			//秘境寻宝淘宝免费时间间隔(分钟)
	int m_add_weight_tao3_times;																			//秘境寻宝增加权重需要淘不到珍稀物品次数
	int m_mijingxunbao3_thirtytimes_item_id;																	//秘境寻宝淘宝三十次消耗物品 
	int m_mijingxunbao3_free_times;																			//秘境寻宝3免费抽奖次数

	int m_mijingxunbao3_auto_add_cfg_count_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];						// 秘境寻宝自增权重配置数量
	RandActivityMiJingXunBaoAddWeightCfgList m_mijingxunbao3_add_weight_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	int m_mijingxunbao3_reward_cfg_count;
	RandActivityMiJingXunBaoRewardDayCfg m_mijingxunbao3_reward_list_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		//秘境寻宝累抽奖励

	int m_mijingxunbao3_daily_cfg_max_count;
	int m_mijingxunbao3_max_seq_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityMiJingXunBaoCfgList m_mijingxunbao3_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// 秘境寻宝物品列表

	RandActivityServerBaodiCfgList m_mijingxunbao3_baodi_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// 秘境寻宝3保底

	//欢乐砸蛋
	int m_huanlezadan_once_gold;																			//欢乐砸蛋淘宝一次消耗非绑定元宝      
	int m_huanlezadan_tentimes_gold;																		//欢乐砸蛋淘宝十次消耗非绑定元宝  
	int m_huanlezadan_thirtytimes_gold;																		//欢乐砸蛋淘宝三十次消耗非绑定元宝  
	int m_huanlezadan_interval;																				//欢乐砸蛋淘宝免费时间间隔(分钟)
	int m_huanlezadan_add_weight_tao_times;																	//欢乐砸蛋增加权重需要淘不到珍稀物品次数
	int m_huanlezadan_thirtytimes_item_id;																	//欢乐砸蛋淘宝三十次消耗物品 
	int m_huanlezadan_free_times;																			//欢乐砸蛋免费抽奖次数

	int m_huanlezadan_auto_add_cfg_count_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];						// 欢乐砸蛋自增权重配置数量
	RandActivityMiJingXunBaoAddWeightCfgList m_huanlezadan_add_weight_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	int m_huanlezadan_reward_cfg_count;
	RandActivityMiJingXunBaoRewardDayCfg m_huanlezadan_reward_list_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		//欢乐砸蛋累抽奖励

	int m_huanlezadan_daily_cfg_max_count;
	int m_huanlezadan_max_seq_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityMiJingXunBaoCfgList m_huanlezadan_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// 欢乐砸蛋物品列表

	//欢乐摇奖
	int m_huanleyaojiang_once_gold;																			//欢乐摇奖淘宝一次消耗非绑定元宝      
	int m_huanleyaojiang_tentimes_gold;																		//欢乐摇奖淘宝十次消耗非绑定元宝  
	int m_huanleyaojiang_thirtytimes_gold;																	//欢乐摇奖淘宝三十次消耗非绑定元宝  
	int m_huanleyaojiang_interval;																			//欢乐摇奖淘宝免费时间间隔(分钟)
	int m_huanleyaojiang_add_weight_tao_times;																//欢乐摇奖增加权重需要淘不到珍稀物品次数
	int m_huanleyaojaing_thirtytimes_item_id;																//欢乐摇奖淘宝三十次消耗物品 
	int m_huanleyaojiang_free_times;																		//欢乐摇奖免费抽奖次数

	int m_huanleyaojiang_auto_add_cfg_count_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];					// 欢乐摇奖自增权重配置数量
	RandActivityMiJingXunBaoAddWeightCfgList m_huanleyaojiang_add_weight_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	int m_huanleyaojiang_reward_cfg_count;																				//欢乐摇奖累抽奖励数量
	RandActivityMiJingXunBaoRewardDayCfg m_huanleyaojiang_reward_list_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		//欢乐摇奖累抽奖励

	int m_huanleyaojiang_stage_cfg_max_count;
	int m_huanleyaojiang_max_seq_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityMiJingXunBaoCfgList m_huanleyaojiang_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// 欢乐摇奖物品列表

	RandActivityServerBaodiCfgList m_huanleyaojiang_baodi_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// 欢乐摇奖全服奖励

	//限时反馈
	RandActivityLimitTimeRebateCfg m_limit_time_rebate_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	// 限时礼包
	RandActivityLimitTimeGiftCfg m_limit_time_gift_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	//循环充值2
	int m_circulation_chongzhi_2_need_chongzhi;																			//循环充值2所需充值数
	std::vector<RandActivityCirculationChongzhiSecondRewardCfg> m_circulation_chongzhi_2_reward_cfg_vec;				//循环充值2配置

	// 疯狂摇钱树
	RandActivityShakeMoneyConfig m_shakemoney_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];	

	// 限时豪礼
	RandActivityLimitTimeLuxuryGiftBagCfg m_limit_time_luxury_gift_bag_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	// RMB购买
	RandActivityRmbBuyChestShopOpenDayCfg m_rmb_buy_ches_shop_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	// 消费返礼
	int m_consume_reward_stage_cfg_count;
	RandActivityConsumeRewardCfg m_consume_reward_stage_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	// 集月饼活动
	RandActivityItemCollectionSecondRewardCfg m_item_collection_2_reward_cfg;

	// 版本累计充值
	RandActivityTotalChargeRewardConfig m_grand_total_charge_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	// 版本连续充值
	std::vector<RandActivityContinueChongzhiRewardConfig> m_continue_charge_reward_cfg_vec;

	//欢乐摇奖2
	int m_huanleyaojiang_2_once_gold;																			//欢乐摇奖2淘宝一次消耗非绑定元宝      
	int m_huanleyaojiang_2_tentimes_gold;																		//欢乐摇奖2淘宝十次消耗非绑定元宝  
	int m_huanleyaojiang_2_thirtytimes_gold;																	//欢乐摇奖2淘宝三十次消耗非绑定元宝  
	int m_huanleyaojiang_2_interval;																			//欢乐摇奖2淘宝免费时间间隔(分钟)
	int m_huanleyaojiang_2_add_weight_tao_times;																//欢乐摇奖2增加权重需要淘不到珍稀物品次数
	int m_huanleyaojaing_2_thirtytimes_item_id;																	//欢乐摇奖2淘宝三十次消耗物品 
	int m_huanleyaojiang_2_free_times;																			//欢乐摇奖2免费抽奖次数

	int m_huanleyaojiang_2_auto_add_cfg_count_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];						// 欢乐摇奖2自增权重配置数量
	RandActivityMiJingXunBaoAddWeightCfgList m_huanleyaojiang_2_add_weight_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	int m_huanleyaojiang_2_reward_cfg_count;																	//欢乐摇奖2累抽奖励数量
	RandActivityMiJingXunBaoRewardDayCfg m_huanleyaojiang_2_reward_list_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		//欢乐摇奖2累抽奖励

	int m_huanleyaojiang_2_stage_cfg_max_count;
	int m_huanleyaojiang_2_max_seq_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityMiJingXunBaoCfgList m_huanleyaojiang_2_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// 欢乐摇奖2物品列表

	RandActivityOfflineSingleChargeCfg m_offline_single_charge_cfg_0[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];	// 线下单笔充值0

	// 买一送一配置
	RandActivityBuyOneGetOneFreeConfig m_buy_one_get_one_free_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	
	int m_everyday_nice_gift_cfg_count;																			// 配置数量
	RandActivityEverydayNiceGiftCfg m_everyday_nice_gift_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// 每日好礼
	RandActivityLoginGiftCfg m_login_gift_cfg_0;																// 登陆好礼

	// 节日守护
	int m_holiday_guard_kill_rank_num;																			// 奖励配置数量
	HolidayGuardKillRankReward	m_holiday_guard_kill_rank_reward_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];	// 击杀排行奖励
	ItemConfigData m_holiday_guard_participation_reward;														// 参与奖励

	// 累计充值5
	RandActivityTotalCharge5Config m_total_charge_5_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	//极限挑战
	int m_extreme_challenge_rand_give_role_task_num;															// 随机给玩家的任务数量
	int m_extreme_challenge_refresh_task_need_gold;																// 刷新已有任务所需元宝
	int m_extreme_challenge_reward_count;																		// 配置数量
	RandActivityExtremeChallengeCfg m_extreme_challenge_cfg[RAND_ACTIVITY_EXTREME_CHALLENGE_MAX_TASK_COUNT];	// 极限挑战
	ItemConfigData m_extreme_challenge_finish_all_task_reward_cfg[MAX_ATTACHMENT_ITEM_NUM];						// 完成所有任务奖励

	//你充我送
	std::vector<RandActivityChongZhiGiftConfig> m_chongzhi_gift_cfg_list;
	
	// 表白排行榜
	RandActivityProfessRankCfg m_profess_rand_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	// 进阶返还2
	JinJieReturnConfig m_jinjie_return2_cfg[JINJIE_ACT_THEME_TYPE_MAX][RAND_ACTIVITY_JINJIE_RETURN_MAX_CFG];

	//暴击日2
	CriticalStrikeDayConfig m_critical_strike_day2_cfg[JINJIE_ACT_THEME_TYPE_MAX];

	// 组合购买
	int m_combine_buy_discount_cfg_list_count;
	RandActivityCombineBuyDiscountCfg m_combine_buy_discount_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	int m_combine_buy_item_cfg_list_count;
	RandActivityCombineBuyItemCfg m_combine_buy_Item_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	RandActivityLuckyWishLotteryAndTotalWeightCfg m_lucky_wish_lottery_and_total_weight_cfg_list[RAND_ACTIVITY_LUCKY_WISH_LOTTERY_CFG_COUNT];
	std::vector<RandActivityLuckyWishLuckyCfg> m_lucky_wish_lucky_cfg_vec;

	int m_image_competition_max_opengame_day;
	std::vector<RandActivityImageCompetition> m_image_competition_cfg_vec;
	RandActivityImageCompetitionReward m_image_competition_reward_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	// 周末狂欢-----------------------------------------------
	int InitLottery1RewardCfg(PugiXmlNode RootElement);
	int InitLottery1PersonRewardCfg(PugiXmlNode RootElement);
	int InitLottery1ServerRewardCfg(PugiXmlNode RootElement);

	RALottery1OtherCfg m_lottery1_other_cfg;
	std::map<int, RALottery1RewardCfg> m_lottery_reward_map;
	std::map<int, std::vector<RALottery1PersonRewardCfg>> m_lottery_person_reward_map;
	std::map<int, std::map<int, RALottery1ServerRewardCfg>> m_lottery_server_reward_map;

	std::vector<RADailyLoveOpengameDayCfg> m_daily_love_reward_precent_vec;

	// 零元购-----------------------------------------------
	std::map<int, RAZeroBuyReturnConfig> m_zero_buy_return_map;  //零元购 map<buy_type, cfg>

	// 每日一爱2
	RandActivityDailyLoveConfig m_daily_love_2_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	// 狂嗨庆典-----------------------------------------------
	RandActivityKuangHaiCfg m_kuang_hai_cfg;
	RandActivityKuangHaiRewardCfg m_kuang_hai_reward_cfg;

	// 礼物收割-----------------------------------------------
	GiftHarvestRankRewardCfg m_gift_harvest_rank_reward_cfg;
	ItemConfigData m_gift_harvest_join_reward_cfg[MAX_ATTACHMENT_ITEM_NUM];						// 活动结束时 派发的参与奖

	int m_lucky_wish_30_times_use_item;								//幸运许愿30抽代替钥匙

	RandLuckyCloudBuyCfg m_lucky_buy_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];	// 幸运云购配置
	RandLuckyCloudBuyOtherCfg m_lucky_buy_other;	// 幸运云购其他配置

	std::map<int, std::vector<FirstChargeTuanCfg>>	m_first_charge_tuan_refresh_timp_map;

	// 狂欢大乐购
	RandActivityCrazyBuy m_cracy_buy[RAND_ACTIVITY_CRACY_BUY_MAX_OF_OPENGAMEDAY];

	//双倍元宝
	RandActivityDoubleGet m_double_get_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
};
#endif