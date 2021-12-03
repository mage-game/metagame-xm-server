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

static const int RAND_ACTIVITY_LUCKROOL_REWARD_CONFIG_MAX_COUNT = 8;						// ����ҡ������������ø���
static const int RAND_ACTIVITY_STONE_UPLEVEL_REWARD_CONFIG_MAX_COUNT = 16;					// ������ʯ��������������ø���
static const int RAND_ACTIVITY_CHONGZHI_RANK_REWARD_CONFIG_MAX_COUNT = 100;					// �����ۼƳ�ֵ���а���������ø���
static const int RAND_ACTIVITY_CONSUME_GOLD_RANK_REWARD_CONFIG_MAX_COUNT = 100;				// �����ۼ��������а���������ø���
static const int RAND_ACTIVITY_EQUIP_STRENGTHEN_REWARD_CONFIG_MAX_COUNT = 12;				// ����װ��ǿ������������ø���
static const int RAND_ACTIVITY_EQUIP_STRENGTHEN_REWARD_ITEM_MAX_COUNT = 2;					// ����װ��ǿ��������Ʒ������ø���
static const int RAND_ACTIVITY_DAY_CHONGZHI_FANLI_REWARD_CONFIG_MAX_COUNT = 12;				// ÿ�ճ�ֵ����������ø���
static const int RAND_ACTIVITY_DAY_CONSUME_GOLD_REWARD_CONFIG_MAX_COUNT = 8;				// ÿ�����ѽ���������ø���
static const int RAND_ACTIVITY_XIANNV_CHANMIAN_UPLEVEL_REWARD_CONFIG_MAX_COUNT = 16;		// ��Ů������������������ø���
static const int RAND_ACTIVITY_MOUNT_UPGRADE_REWARD_CONFIG_MAX_COUNT = 10;					// ������׽���������ø���
static const int RAND_ACTIVITY_QIBING_UPGRADE_REWARD_CONFIG_MAX_COUNT = 10;					// ������׽���������ø���
static const int RAND_ACTIVITY_MENTALITY_TOTAL_LEVEL_REWARD_CONFIG_MAX_COUNT = 10;			// ����ȫ��ȼ�����������ø���
static const int RAND_ACTIVITY_WING_UPGRADE_REWARD_CONFIG_MAX_COUNT = 20;					// ������������������ø���
static const int RAND_ACTIVITY_CHESTSHOP_REWARD_CONFIG_MAX_COUNT = 16;						// Ѱ����������������ø���
static const int RAND_ACTIVITY_VIP_QIFU_REWARD_CONFIG_MAX_COUNT = 16;						// ȫ��������������ø���
static const int RAND_ACTIVITY_TOTAL_CONSUME_GOLD_REWARD_CONFIG_MAX_COUNT = 10;				// �ۼ����ѽ���������ø���
static const int RAND_ACTIVITY_DAY_ACTIVE_DEGREE_REWARD_CONFIG_MAX_COUNT = 8;				// ��Ծ��������������ø���
static const int RAND_ACTIVITY_XIANMENG_BIPIN_REWARD_CONFIG_MAX_COUNT = 8;					// ���˱�ƴ����������ø���
static const int RAND_ACTIVITY_DAY_ONLINE_GIFT_CONFIG_MAX_COUNT = 8;						// ÿ�����߽���������ø���
static const int RAND_ACTIVITY_XIANMENG_JUEQI_REWARD_CONFIG_MAX_COUNT = 8;					// ����������������ø���
static const int RAND_ACTIVITY_SHOUYOU_YUXIANG_REWARD_CONFIG_MAX_COUNT = 8;					// �������㽱��������ø���
static const int RAND_ACTIVITY_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT = 32;						// ��¼������������ø���
static const int RAND_ACTIVITY_BIPIN_ACTIVITY_MAX_COUNT = 16;								// ��ƴ������
static const int RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT = 32;						// ��ƴȫ���������
static const int RAND_ACTIVITY_CHARGE_REPAYMENT_CONFIG_MAX_COUNT = 10;						// ��ֵ�������������
static const int RAND_ACTIVITY_CORNUCOPIA_CONFIG_MAX_COUNT = 10;							// �۱������������
static const int RAND_ACTIVITY_CORNUCOPIA_MAX_LUN = 8;										// �۱����������
static const int RAND_ACTIVITY_DANBI_CHONGZHI_DAY_CFG_MAX_COUNT = 16;						// ���ʳ�ֵ�����������
static const int RAND_ACTIVITY_DANBI_CHONGZHI_REWARD_CFG_MAX_COUNT_PER_DAY = 16;			// ���ʳ�ֵÿ�ս������������
static const int RAND_ACTIVITY_TOTAL_CHARGE_DAY_CFG_MAX_SEQ = 10;							// �ۼƳ�ֵ������������������
static const int RAND_ACTIVITY_TOMORROW_REWARD_CFG_MAX_COUNT = 10;							// ���ո�������������
static const int RAND_ACTIVITY_DAY_CHONGZHI_RANK_REWARD_MAX_DAY = 16;						// ÿ�ճ�ֵ������ཱ������
static const int RAND_ACTIVITY_DAY_CONSUME_RANK_REWARD_MAX_DAY = 16;						// ÿ������������ཱ������
static const int RAND_ACTIVITY_ZHENBAOGE_MAX_SEQ = 900;										// �䱦��ˢ����Ʒ��������
static const int RAND_ACTIVITY_ZHENBAOGE_SERVER_MAX_SEQ = 50;								// �䱦��ȫ�������������
static const int RAND_ACTIVITY_ZHENBAOGE_AUTO_ADD_WEIGHT_CFG_COUNT = 5;						// �䱦��������Ȩ����������
static const int RAND_ACTIVITY_ZHENBAOGE_MAX_RETURN_REWARD_COUNT = 50;						// �䱦�������������
static const int RAND_ACTIVITY_MIJINGXUNBAO_AUTO_ADD_WEIGHT_CFG_COUNT = 5;			    	// �ؾ�Ѱ��������Ȩ����������
static const int RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ = 250;									// �ؾ�Ѱ���Ա���Ʒ��������
static const int RAND_ACTIVITY_MIJINGXUNBAO_REWARD_MAX_COUNT = 10;							// �ؾ�Ѱ������ÿ�����������
static const int RAND_ACTIVITY_JISUCHONGZHAN_CFG_NUM = 10;									// ���ٳ�ս������
static const int RAND_ACTIVITY_TOTAL_CHARGE_3_CFG_COUNT = 100;								// �ۼƳ�ֵ3�����ʱ�����
static const int RAND_ACTIVITY_ANY_MONSTER_DROP_CFG_COUNT = 10;								// ȫ����������������������
static const int RAND_ACTIVITY_TOTAL_CHARGE_4_CFG_COUNT = 10;								// �ۼƳ�ֵ4�����ʱ�����
static const int RAND_ACTIVITY_TIANMING_DIVINATION_REWARD_MAX_SEQ = 480;					// �������Խ�����󽱳��±�
static const int RAND_ACTIVITY_TIANMING_DIVINATION_TOTAL_MAX_ADD_LOT_TIMES = 54;			// ������������ע����
static const int RAND_ACTIVITY_TIANMING_DIVINATION_EVERY_LOT_MAX_ADD_TIMES = 9;				// ��������ÿ��ǩͲ����ע����
static const int RAND_ACTIVITY_TIANMING_DIVINATION_OPENGAME_DAY_INTERVAL = 24;				// ��������ÿ���ֶ����������
static const int RAND_ACTIVITY_OPEN_SERVER_REWARD_COUNT_MAX = 20;							// ������������Ʒ����
static const int RAND_ACTIVITY_NEW_CORNUCOPIA_CFG_COUNT_MAX = 7;							// �¾۱��������������
static const int RAND_ACTIVITY_NEW_CORNUCOPIA_TOTAL_CFG_COUNT_MAX = 5;						// �¾۱���ȫ�����������������
static const int RAND_ACTIVITY_EXP_REFINE_REWARD_MAX_COUNT = 20;							// ������������������
static const int RAND_ACTIVITY_OPEN_SERVER_INVEST_MAX_COUNT = 7;							// ����Ͷ������������
static const int RAND_ACTIVITY_SINGLE_CHONGZHI_MAX_CFG = 5;									// ���ʷ��̶�Ԫ�����������
static const int RAND_ACTIVITY_XIANYUAN_TREAS_CFG_COUNT = 10;								// �ۻ�����������
static const int RAND_ACTIVITY_ZHENBAOGE2_MAX_SEQ = 900;									// �䱦��2���������
static const int RAND_ACTIVITY_ZHENBAOGE2_MAX_RETURN_REWARD_COUNT = 50;						// �䱦��2�����������

static const int RAND_ACTIVITY_JINJIE_THEME_ACT_MAX_OPENGAME_DAY = 1000;                   //����������1000��Ϊһ��ѭ��

static const int RAND_ACTIVITY_CONSUM_GIFT_MAX_SEQ = 6;                                    // �����콱ÿ���������������
static const int RAND_ACTIVITY_CONSUM_GIFT_MAX_REWARD_COUNT = 4;						   // �����콱��������

static const int  RAND_ACTIVITY_COLLECT_TREASURE_MAX_CFG = 8;                               // �۱��裨�£�

static const int RAND_ACTIVITY_HAPPY_CUMUL_CHONGZHI_MAX_REWARD_COUNT = 5;					// �����۳佱������
static const int RAND_ACTIVITY_HAPPY_CUMUL_CHONGZHI_MAX_CFG = 9;						    // �����۳�������

static const int RAND_ACTIVITY_JINJIE_RETURN_MAX_CFG = 14;						           // ���׷���������

static const int RAND_ACTIVITY_EVERYDAY_NICE_GIFT_MAX_REWARD_COUNT = 7;						// ÿ�պ���  ������ý�������

static const int RA_LIMIT_TIME_REBATE_MAX_CFG_COUNT = 32;                                   //��ʱ����

static const int RAND_ACTIVITY_LIMIT_TIME_GIFT_MAX_SEQ = 14;								// ��ʱ������������

static const int RAND_ACITIVTY_EXPENSE_NICE_GIFT_NUM_TYPE_CONFIG_MAX_COUNT = 10;			// ���Ѻ����������������������
static const int RAND_ACITIVTY_EXPENSE_NICE_GIFT_GRAND_TOTAL_REWARD_MAX_COUNT = 4;			// ���Ѻ����ۼ����ѽ������������

static const int RAND_ACTIVITY_BUY_ONE_GET_ONE_FREE_ITEM_MAX_COUNT = 20;					// ��һ��һ�����Ʒ����
UNSTD_STATIC_CHECK(RAND_ACTIVITY_BUY_ONE_GET_ONE_FREE_ITEM_MAX_COUNT <= 63);

static const int RAND_ACTIVITY_EXTREME_CHALLENGE_MAX_TASK_COUNT = 40;						// ������ս�����������

static const int RAND_ACTIVITY_HOLIDAYGUARD_PERSON_RANK_MAX = 10;							// ���а��������

static const int RAND_ACTIVITY_TOTAL_CHARGE_5_CFG_COUNT = 10;								// �ۼƳ�ֵ5�����ʱ�����

static const int RAND_ACTIVITY_LUCKY_WISH_LOTTERY_CFG_COUNT = 40;							// ������Ը����������������

static const int RAND_ACTIVITY_IMAGE_COMPETITION_REWARD_CFG_COUNT = 4;                      // �����ƴ���н���������  

static const int RAND_ACTIVITY_SINGLE_CHARGE2_CFG_COUNT = 100;								// ���ٳ�ս2��������������
static const int RAND_ACTIVITY_SINGLE_CHARGE3_CFG_COUNT = 100;								// ���ٳ�ս3��������������
static const int RAND_ACTIVITY_SINGLE_CHARGE4_CFG_COUNT = 100;								// ���ٳ�ս4��������������
static const int RAND_ACTIVITY_SINGLE_CHARGE5_CFG_COUNT = 100;								// ���ٳ�ս5��������������
static const int RAND_ACTIVITY_DANBI_CHONGZHI_DAY_CFG_COUNT = 100;									// ÿ�յ���������
static const int RAND_ACTIVITY_SINGLE_CHAR_CFG_COUNT = 100;											// ���ʳ�ֵ������
static const int RAND_ACTIVITY_CRACY_BUY_MAX_NUM_TO_OPENDAY = 21;							// �񻶴��ֹ�����ͬһ������������������
static const int RAND_ACTIVITY_CRACY_BUY_MAX_OF_OPENGAMEDAY = 32;							// �񻶴��ֹ�������10����ͬ�Ŀ�������
static const int RAND_ACTIVITY_DOUBLE_GET_CFG_COUNT = 16;									// ˫��Ԫ����������������

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

		int seq;															// ��������
		int need_gold;														// ��ҪԪ��
		int reward_gold;													// ����Ԫ��
		int reward_bind_gold;												// ������Ԫ��
		ItemConfigData reward_item;											// ������Ʒ
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

		int seq;															// ��������
		int need_consume_gold;												// ��Ҫ����Ԫ��	
		ItemConfigData reward_item;											// ������Ʒ
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

		int seq;															// ��������
		int need_consume_gold;												// ��Ҫ�ۼ�����Ԫ��	
		ItemConfigData reward_item;											// ������Ʒ
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

		int seq;															// ��������
		int need_active;													// ��Ҫ��Ծ��	
		ItemConfigData reward_item;											// ������Ʒ
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

		int seq;															// ��������
		int need_flower;													// ��Ҫ�ʻ���	
		ItemConfigData reward_item;											// ������Ʒ
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

		int seq;														// ����
		int charge_value;												// ���ʳ�ֵ���
		int bind_gold_repayment;										// ������Ԫ����
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

		int seq;														// ����
		int charge_value;												// ���ʳ�ֵ���
		ItemConfigData reward_item;										// ������Ʒ
	};

	RandActivitySingleChargeCfg() : cfg_count(0), opengame_day(0) {}

	int cfg_count;
	int opengame_day;
	ConfigItem cfg_list[RAND_ACTIVITY_SINGLE_CHARGE_CONFIG_MAX_COUNT];
};

struct RandActivityCornucopiaCfg
{
	RandActivityCornucopiaCfg() : lun(0), need_total_charge(0), need_put_gold(0), min_reward_gold(0), max_reward_gold(0), is_broadcast(false) {}

	int lun;															// �ڼ���
	int need_total_charge;												// ��Ҫ�ۼƳ�ֵ��
	int need_put_gold;													// ��ҪͶ���ʽ�
	int min_reward_gold;												// ��Ϳ�����ʯ
	int max_reward_gold;												// ��߿�����ʯ
	bool is_broadcast;													// �Ƿ���
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

		int reward_index;														// ��������
		ItemConfigData reward_item;												// ������Ʒ
		int level;																// ��ǰ����
		int weight;																// �鵽�ý�����Ȩ��
		int to_level;															// ��ȡ��ǰ��������������
		bool is_broadcast;														// �鵽�������Ƿ���
		bool is_record_history;													// �鵽�������Ǽ�¼���齱��ʷ��		
	};

	int cfg_count;
	int opengame_day;

	std::map<int, int> baodi_map;												// ����seq��Ӧ���״��� map
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
	int auto_add_weight;								// ����Ȩ��
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
	RandActivityMiJingXunBaoConfig cfg_list[RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ + 1];			// �ؾ�Ѱ����Ʒ�б�

	std::map<int, int> baodi_map;																// ����seq��Ӧ���״��� map
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
	RandActivityMoneyTreeServerRewardConfig server_reward_cfg_list[RA_MONEY_TREE_SERVER_REWARD_CFG_COUNT];  // ҡǮ��ȫ����Ʒ�б�
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
	RandActivityMoneyTreeRewardConfig reward_cfg_list[RA_MONEY_TREE_REWARD_CFG_COUNT];		// ҡǮ���齱��Ʒ�б�
};

// �����뷭��-��������
struct RandActivityKingDrawRewardConfig										// ��������
{
	RandActivityKingDrawRewardConfig() : seq(0), weight(0), auto_add_weight(0), is_rare(false) {}

	ItemConfigData reward_item;												// ������Ʒ
	short seq;																// ��������
	short weight;															// ����Ȩ��
	short auto_add_weight;													// ����Ȩ��
	bool is_rare;															// �Ƿ���ϡ
};

// �����뷭��-����Ȩ������
struct RandActivitySelfWeightConfig
{
	RandActivitySelfWeightConfig() : seq(0), auto_add_weight(0) {}

	short seq;																// �������Ӧ������
	short auto_add_weight;													// ����Ȩ��
};

struct RandActivityKingDrawLevelStageConfig
{
	static const int KING_DRAW_REWARD_COUNT_PER_STAGE = 60;						// һ�׶����Ľ����������õģ�

	RandActivityKingDrawLevelStageConfig() : m_reward_count(0), m_kingdraw_self_weigth_count(0) {}

	int m_reward_count;
	RandActivityKingDrawRewardConfig reward_list[KING_DRAW_REWARD_COUNT_PER_STAGE];

	int m_kingdraw_self_weigth_count;									// �����뷭�ƣ�Ȩ���������������
	RandActivitySelfWeightConfig m_kingdraw_self_weight_cfg_list[RA_KING_DRAW_MAX_SELF_WEIGTH_COUNT];			// �����뷭�ƣ�Ȩ���������������Ϣ
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
	static const int KING_DRAW_RETURN_REWARD_COUNT_PER_STAGE = 10;				// һ�׶����Ľ����������õģ�

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

		int seq;															// ��ֵ���Σ��Զ����ɣ����Ƕ�����

		int need_chongzhi_num;
		ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];				// �ʼ����Ž���
	};

	static const int MAX_CHONGZHI_CFG_COUNT = 5;				// ͬһ���ֶ������������

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

	ItemID add_attr_item_id;												// �����Ե���ƷID		
	int add_attr_rate;														// ʹ����Ʒ���ӵĻ������Ա����������ǻ������ԣ�

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
};

// �ۼƳ�ֵ4
struct RandActivityTotalCharge4Cfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), need_chongzhi_num(0) {}

		int seq;															// ��ֵ���Σ��Զ����ɣ����Ƕ�����

		int need_chongzhi_num;
		ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];				// �ʼ����Ž���
	};

	static const int MAX_CHONGZHI_CFG_COUNT = 5;				// ͬһ���ֶ������������

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

// �ڿ�-��ʯ��Ϣ����
struct RandActivityMineInfoStageConfig
{
	RandActivityMineInfoStageConfig() : seq(0), need_gold(0), exchange_need_num(0), show_weight(0), show_need_refresh_times(0),
		show_certain_refresh_beishu(0), opengame_day(0), is_broadcast(false) {}

	int seq;

	int need_gold;							// �ڿ�����Ԫ��
	int exchange_need_num;					// �һ������ʯ��
	ItemConfigData exchange_item;			// �һ��Ľ���
	int show_weight;						// ˢ����Ȩ��
	int show_need_refresh_times;			// ��������ˢ�����
	int show_certain_refresh_beishu;		// �س�ˢ����
	int opengame_day;
	bool is_broadcast;
};

// �ڿ�-��ʯ��Ϣ����
struct RandActivityMineInfoConfig
{
	static const int RA_MINE_INFO_STAGE_LIST_MAX_LEN = 12;
	RandActivityMineInfoConfig() : opengame_day(0), stage_list_count(0) {}

	int opengame_day;
	int stage_list_count;
	std::map<int, int> baodi_map;

	RandActivityMineInfoStageConfig stage_list[RA_MINE_INFO_STAGE_LIST_MAX_LEN];
};


// �ڿ�-ȫ���������
struct RandActivityMineServerRewardStageConfig
{
	RandActivityMineServerRewardStageConfig() : seq(0), total_refresh_times(0), vip_limit(0) {}

	int seq;

	int total_refresh_times;				// ����ȫ��ˢ�����
	ItemConfigData reward_item;
	int vip_limit;
};

// �ڿ�-ȫ���������
struct RandActivityMineServerRewardConfig
{
	static const int RA_MINE_REWARD_STAGE_LIST_MAX_LEN = 10;

	RandActivityMineServerRewardConfig() : opengame_day(0), stage_list_count(0) {}

	int opengame_day;
	int stage_list_count;
	RandActivityMineServerRewardStageConfig stage_list[RA_MINE_REWARD_STAGE_LIST_MAX_LEN];
};

// ���ι�-��������
struct RandActivityGuaGuaConfig	
{
	RandActivityGuaGuaConfig() : opengame_day(0), seq(0), weight(0), baodi(0) {}

	bool operator<(const RandActivityGuaGuaConfig &rhs) const
	{
		return opengame_day < rhs.opengame_day;
	}

	int opengame_day;
	int seq;						// ��������
	int weight;
	int is_special;					// �Ƿ��ر�								
	ItemConfigData reward_item;		// ������Ʒ�б�

	int baodi;
};

// ���ι� - �ۼ�����
struct RandActivityGuaGuaAccConfig
{
	RandActivityGuaGuaAccConfig() : index(0), acc_count(0) {}

	int opengame_day;															// ��������	
	int index;																	// ��������
	int acc_count;																// �ۼӴ���
	ItemConfigData reward_item;													// ������Ʒ
};

// ��������-��������
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

// ��������-Ԫ������
struct RandActivityTianMingDivinationGoldConsumeConfig
{
	RandActivityTianMingDivinationGoldConsumeConfig() : lot_add_times(-1), add_consume_gold(-1), chou_consume_gold(0) {}

	int lot_add_times;
	int add_consume_gold;
	int chou_consume_gold;

};

// ����ת-��Ʒ��Ϣ
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

// ����ת-������Ϣ
struct RandActivityFanFanWordInfo
{
	RandActivityFanFanWordInfo() : seq(0), weight(0), floor_refresh_times(0) {}

	int seq;
	int weight;
	int floor_refresh_times;			// ����ˢ�´���
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

//����ת�۳齱��
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

// ������ֵ�������ػݳ��������ػݸ�-��������
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

// ��������-��������
struct RandActivityContinueConsumeOtherDayCfg
{
	RandActivityContinueConsumeOtherDayCfg() : opengame_day(0), cfg_count(0), continue_consume_fetch_extra_reward_need_days(0){}

	int opengame_day;
	int cfg_count;

	int continue_consume_fetch_extra_reward_need_days;
	ItemConfigData continue_consume_extra_reward;
};

// ��������-��������
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

// ��������--��Ծ�ȶһ�����
struct RandActivityArmyDayExchangeFlagConfig 
{
	RandActivityArmyDayExchangeFlagConfig() : seq(0), need_active_degree(0), reward_flag_num(0)
	{

	}

	int seq;
	int need_active_degree;
	int reward_flag_num;
};

//��������--���Ķһ�����
struct RandActivityArmyDayExchangeItemConfig 
{
	RandActivityArmyDayExchangeItemConfig() : side_type(INVALID_ARMY_SIDE), need_flags(0)
	{

	}

	int side_type;
	int need_flags;
	ItemConfigData reward_item;
};

// ѭ����ֵ-��������
struct RandActivityCirculationChongzhiStageRewardConfig
{
	RandActivityCirculationChongzhiStageRewardConfig() : seq(0), need_chongzhi_gold(0) {}

	int seq;
	unsigned int need_chongzhi_gold;
	ItemConfigData reward_item;
};

// ѭ����ֵ-��������
struct RandActivityCirculationChongzhiRewardConfig
{
	RandActivityCirculationChongzhiRewardConfig() : cfg_count(0), opengame_day(0) {}

	int cfg_count;
	int opengame_day;

	RandActivityCirculationChongzhiStageRewardConfig cfg_list[RA_CIRCULATION_CHONGZHI_MAX_REWARD_COUNT];
};

// �������˳齱����
struct RandActivityExtremeLuckyDrawConsumeConfig
{
	RandActivityExtremeLuckyDrawConsumeConfig() : draw_times(0), consume_gold(0)
	{
	
	}

	int draw_times;
	int consume_gold;
};

// �������˳齱����������
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

// ������������
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

// ������������
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

// ���ֻ
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

// ��������
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

// �������
struct RandActivityRedEnvelopeGiftCfg
{
	RandActivityRedEnvelopeGiftCfg() : percent(0){}
	int percent;
};

// ���ǽ���
struct RandActivityMarryMeCfg
{
	RandActivityMarryMeCfg() : reward_couple_count(0) {}

	int reward_couple_count;
	ItemConfigData reward_item;
};


// �����ٻ�
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

//����Ͷ��
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

// ��������
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

// �ۻ���
struct RandActivityXianyuanTreasConfig
{
	RandActivityXianyuanTreasConfig() : seq(0), max_reward_day(0), consume_gold(0) {}

	int seq;
	ItemConfigData reward_item;
	int max_reward_day;
	int consume_gold;
};

// ��ʱ��ɱ-�����׶�����
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
	int buying_begin_time;		// ������ʼʱ�䣨Сʱ��

	int sale_items_count;		// �˽׶ο�������Ʒ��
	SaleItemConfig sale_item_cfg_list[RA_RUSH_BUYING_SALES_ITEMS_COUNT];
};

struct RandActivityRushBuyingPhaseDayConfig
{
	RandActivityRushBuyingPhaseDayConfig() : opengame_day(0), cfg_count(0) {}

	int opengame_day;
	int cfg_count;
	RandActivityRushBuyingPhaseConfig cfg_list[RA_RUSH_BUYING_PHASES_COUNT];
};

// ��ͼѰ��
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

//����Ѱ��
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
		RA_MAGIC_SHOP_BUY_TYPE_BUY3,	// һ������3��

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
	ConfigItem everyday_reward;		// ֻ��һ����λ  
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

// ���ѻ�����-�һ���Ʒ����
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

// ���� - ���ҡǮ��
struct RandActivityShakeMoneyConfig
{
	RandActivityShakeMoneyConfig() : seq(0), section_start(0), section_end(0), chongzhi_return(0), return_max(0)
	{
	}

	int seq;
	int section_start;			// ����ʱ��
	int section_end;			// ����ʱ��
	int chongzhi_return;		//��ֵ����
	int return_max;				//��������
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

// ���±��
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

// ���µ��ʳ�ֵ
struct RandActivityOfflineSingleChargeCfg
{
	enum REWARD_TYPE
	{
		REWARD_TYPE_ONCE_GIFT,						// ��ֵһ�ξ���һ������
		REWARD_TYPE_MAX_GIFT,						// ֻ����߳�ֵ��λ�Ľ���
		REWARD_TYPE_MAX,
	};

	struct ConfigItem
	{
		ConfigItem() : reward_type(0), seq(0), charge_value(0), reward_limit(0) {}

		int reward_type;
		int seq;														// ����
		int charge_value;												// ���ʳ�ֵ���
		int reward_limit;												// ��������
		ItemConfigData reward_item;										// ������Ʒ
	};

	RandActivityOfflineSingleChargeCfg() : cfg_count(0), opengame_day(0) {}

	int cfg_count;
	int opengame_day;
	ConfigItem cfg_list[RAND_ACTIVITY_SINGLE_CHARGE_CONFIG_MAX_COUNT];
};

// ��һ��һ
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

// �����ػ�
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

// �ۼƳ�ֵ5 (�汾��������)
struct RandActivityTotalCharge5Cfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), need_chongzhi_num(0) {}

		int seq;															// ��ֵ���Σ��Զ����ɣ����Ƕ�����

		int need_chongzhi_num;
		ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];				// �ʼ����Ž���
	};

	static const int MAX_CHONGZHI_CFG_COUNT = 5;				// ͬһ���ֶ������������

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

// ������а�
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

// ��Ϲ���
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

	int opengame_day;			// ��������
	int seq;					// ����
	int weight;					// Ȩ��
	bool is_rare;				// �Ƿ���ϡ
	bool is_baodi;				// �Ƿ��Ǳ���
	ItemConfigData item;
};

struct RandActivityLuckyWishLotteryAndTotalWeightCfg
{
	RandActivityLuckyWishLotteryAndTotalWeightCfg() : total_weight(0) {}

	std::vector<RandActivityLuckyWishLotteryCfg> lottery_vec;
	int total_weight;			// ��Ȩ��
	std::map<int, int> baodi_map;
};

// ��ĩ��-------------------------------------------

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
	
	int opengame_day;		//��������
	int consume_gold_1;		//����Ԫ��
	int consume_gold_30;		//����Ԫ��
	int add_lucky_value;	//ÿ����Ը���ӵ�����ֵ
	int lucky_min;			//�н�������С����ֵ
	int lucky_max;			//�н������������ֵ
	ItemConfigData item;
};

struct RADailyLoveOpengameDayCfg
{
	RADailyLoveOpengameDayCfg() : opengame_day(0), gold_percent(0) {}

	int opengame_day;
	int gold_percent;
};

enum IMAGE_COMPETITION_TYPE    // �����ƴ����
{
	IMAGE_COMPETITION_TYPE_INVALID = 0,
	IMAGE_COMPETITION_TYPE_MOUNT = 1,        // ����
	IMAGE_COMPETITION_TYPE_WING = 2,         // ����
	IMAGE_COMPETITION_TYPE_FIGHT_MOUNT = 3,  // ս��
	IMAGE_COMPETITION_TYPE_SHIZHUANG = 4,    // ʱװ
	IMAGE_COMPETITION_TYPE_SHENBING = 5,     // ���
	IMAGE_COMPETITION_TYPE_FABAO = 6,        // ����
	IMAGE_COMPETITION_TYPE_XIANNV_HALO = 7,  // ��Ů�⻷
	IMAGE_COMPETITION_TYPE_QILINBI = 8,      // �����
	IMAGE_COMPETITION_TYPE_TOUSHI = 9,       // ͷ��
	IMAGE_COMPETITION_TYPE_YAOSHI = 10,      // ����
	IMAGE_COMPETITION_TYPE_LINGTONG = 11,    // ��ͯ
	IMAGE_COMPETITION_TYPE_LINGGONG = 12,    // �鹭
	IMAGE_COMPETITION_TYPE_LINGMOUNT = 13,   // ����
	IMAGE_COMPETITION_TYPE_HALO = 14,        // �⻷
	IMAGE_COMPETITION_TYPE_FEICHONG = 15,	 // �ɳ�
	IMAGE_COMPETITION_TYPE_WEIYAN = 16,		 // β��
	IMAGE_COMPETITION_TYPE_FOOTPRINT = 17,	 // �㼣
	IMAGE_COMPETITION_TYPE_SHENYI = 18,		 // ��Ů���� 
	IMAGE_COMPETITION_TYPE_MASK = 19,		 // ����
	IMAGE_COMPETITION_TYPE_WEIBA = 20,		 // β��
	IMAGE_COMPETITION_TYPE_SHOUHUAN = 21,	 // �ֻ�
	IMAGE_COMPETITION_TYPE_XIANNV = 22,		 // ��Ů
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

// ��Ԫ��-------------------------------------------
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

// �������-------------------------------------------
struct RandActivityKuangHaiCfg
{
	struct ItemCfg
	{
		int index = 0;			// ����
		int task_type = 0;		// ��������
		int task_arg = 0;		// ��������
		int per_score = 0;		// ÿ�οɵ��˵�
		int max_score = 0;		// �˵�����
		int open_level = 0;		// �����ȼ�
	};

	int item_num = 0;
	ItemCfg item_cfg_list[RA_KUANG_HAI_TASK_ITEM_MAX_NUM];
	std::map<int, std::vector<const RandActivityKuangHaiCfg::ItemCfg*> > id_task_map;// <task_id, task_vec>
};

struct RandActivityKuangHaiRewardCfg
{
	struct ItemCfg
	{
		int reward_seq = 0;			// ����
		int need_score = 0;			// ��������
		int is_broadcast = 0;		// �Ƿ���
		int reward_num = 0;			// ��������
		ItemConfigData reward_list[RA_KUANG_HAI_REWARD_MAX_ITEM_NUM];
	};

	int item_num = 0;
	ItemCfg item_cfg_list[RA_KUANG_HAI_REWARD_LIST_MAX_NUM];
};

// �����ո�-------------------------
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

// �����ƹ�
static const int LUCKY_CLOUD_BUY_MIN_REWARD_MAX_NUM = 6;	// �����ƹ� ���׽��� �����Ʒ����
static const int LUCKY_CLOUD_BUY_ROUND_MAX_NUM = 16;			// �����ƹ� ÿ���������

struct RandLuckyCloudBuyCfg
{
	RandLuckyCloudBuyCfg() :round_num_tody(0), opengame_day(0) {}

	struct RoundItem
	{
		int seq = 0;																	// ���� ����
		int sale_count = 0;																// ���� ������
		int need_gold = 0;																// ���� ����
		int begin_time_hour = 0;														// ���� ��ʼʱ�䣨Сʱ��
		int begin_time_min = 0;															// ���� ��ʼʱ�䣨���ӣ�
		int end_time_hour = 0;															// ���� ����ʱ�䣨Сʱ��
		int end_time_min = 0;															// ���� ����ʱ�䣨���ӣ�
		int min_reward_item_count = 0;													// ���� ���׽�����
		int total_reward_weight = 0;													// ���� ���׽���Ȩ��
		ItemConfigData big_reward_item;													// ���� ����Ʒ
		ItemConfigData min_reward_item_list[LUCKY_CLOUD_BUY_MIN_REWARD_MAX_NUM];		// ���� ���׽�����Ʒ�б�
		unsigned int min_reward_weight_list[LUCKY_CLOUD_BUY_MIN_REWARD_MAX_NUM] = { 0 };// ���� ���׽���Ȩ���б�

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

	int round_num_tody;									// ���յ�����
	int opengame_day;									// ��������
	RoundItem round_list[LUCKY_CLOUD_BUY_ROUND_MAX_NUM];
};

struct RandLuckyCloudBuyOtherCfg
{
	RandLuckyCloudBuyOtherCfg() :buy_limit(0), nolimit_time_hour(0), nolimit_time_minute(0), is_broadcast(0), today_time_nolimit(0), huge_reward_need_percent(0){}

	int buy_limit;				// ÿ�ֵĹ�������
	int nolimit_time_hour;		// �����ƹ����ʱ��-Сʱ
	int nolimit_time_minute;	// �����ƹ����ʱ��-����
	int is_broadcast;			// �Ƿ�󽱹㲥
	int today_time_nolimit;		// ��������ʱ���
	int huge_reward_need_percent;	//�����������ڵ���%xֱ�Ӹ���
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

	const ItemConfigData * GetAppearaceRankReward(int rank);				// ��ȡ�������л�����н���
	const ItemConfigData & GetAppearaceRankJoinReward() { return m_appearance_rank_join_reward; }// ��ȡ�������л���뽱��
	const ItemConfigData * GetAppearacePassiveRankReward(int rank);				// ��ȡ�����������л�����н���
	const ItemConfigData & GetAppearacePassiveRankJoinReward() { return m_appearance_passive_rank_join_reward; }// ��ȡ�����������л���뽱��

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

	RandActivityEverydayNiceGiftCfg::ConfigItem *  GetEveydayNiceGiftRewardCfg();			// ÿ�պ���

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

	//����
	const RandActivityItemCollectionRewardCfg::ConfigItem * GetItemCollectionRewardCfg(int seq);
	
	//�������
	const RandActivityRedEnvelopeGiftCfg &GetRedEnvelopeGiftCfg(){ return m_red_envelope_gift_cfg; };

	// ��������
	const RandActivityExpRefineRewardCfg * GetRandActivityExpRefineRewardCfg(int seq);

	// ���ǽ���
	bool IsMarryMeRewardCountLimit(int count) { return count >= m_marryme_cfg.reward_couple_count; }
	const ItemConfigData &GetMarryMeRewardItem() { return m_marryme_cfg.reward_item; }

	// �����ٻ�
	const RandActivityGoldenPigOtherCfg * GetRAGoldenPigOtherCfg() { return &m_golden_pig_other_cfg; }
	ItemConfigData * GetRAGoldenPigRewardCfg(int summon_type, int reward_type, int &boss_id);
	int IsGoldenPigBoss(int boss_id);
	int GetGoldenPigJoinerRewardFlag(int boss_id);
	void SetGoldenPigJoinerRewardFlag(int flag, int boss_id);

	// ����Ͷ��
	const RandActivityOpenServerInvestOtherCfg * GetRAServerInvestOtherCfg(int invest_type);
	const RandActivityOpenServerInvestRewardCfg * GetRAServerInvestRewardCfg(int invest_type, int seq);

	// ��������
	const RandActivitySingleChongZhiRewardCfg::ItemConfig *GetSingleChongZhiCfgByGoldNum(int chongzhi);
	const RandActivitySingleChongZhiRewardCfg::ItemConfig *GetSingleChongZhiCfgBySeq(int seq);

	// �ۻ���
	const RandActivityXianyuanTreasConfig * GetXianyuanTreasCfg(int seq);
	int GetXianyuanTreasCount() { return m_xianyuan_treas_cfg_count; }
	int GetXianyuanTreasBuyLimitDay() { return m_xianyuan_treas_buy_limit_day; }
	const ItemConfigData* GetXianyuanTreasAllBuyReward();
	int GetActivityLastedDays(int activity_type);

	// ��ʱ��ɱ
	int GetRushBuyingDuration() { return m_rush_buying_duration; }
	int GetRushBuyingPhase(time_t timestamp);
	int GetRushBuyingSaleItemsCount(int phase_seq);
	unsigned int GetRushBuyingPhaseBeginTimestamp(int phase_seq);
	const RandActivityRushBuyingPhaseConfig::SaleItemConfig * GetRushBuyingSaleItemCfg(int phase_seq, int item_index);

	const RandActivitySingleChargeCfg::ConfigItem * GetSingleChargeCfg2(long long charge_value);
	const RandActivitySingleChargeCfg::ConfigItem * GetSingleChargeCfg3(long long charge_value);
	const RandActivitySingleChargeCfg::ConfigItem * GetSingleChargeCfg4(long long charge_value);
	const RandActivitySingleChargeCfg::ConfigItem * GetSingleChargeCfg5(long long charge_value);

	//��ͼѰ��
	const int GetMapHuntFreeXunbaoCount() { return m_map_hunt_free_xunbao_count; }
	const int GetMapHuntFlushInterval() { return m_map_hunt_free_flush_interval; }
	const int GetMapHuntXunbaoGold() { return m_map_hunt_xunbao_glod; }
	const int GetMapHuntFlushNeedGold() { return m_map_hunt_flush_gold; }
	bool GetMapHuntRandCityCfg(int rand_count, RandActivityMapHuntCityConfig::CityCfg *rand_list[MAX_RA_MAP_HUNT_CITY_COUNT_OF_ROUTE], int server_chou_times);
	const ItemConfigData * GetMapHuntBaseReward(const int city_id);
	const RandActivityMapHuntRouteConfig::RouteCfg * GetMapHuntRouteCfg(const int route_index);
	const int GetMapHuntRouteIndex(const int first_city_id);
	const RandActivityMapHuntServerRewardConfig::RewardCfg * GetMapHuntServerRewardCfg(const int index);

	//����Ѱ��
	int GetLightTowerExploreDrawOneGold() { return m_light_tower_explore_one_gold; }
	int GetLightTowerExploreDrawTenGold() { return m_light_tower_explore_ten_gold; }
	const RandActivityLightTowerExploreCfg::LayerConfig * GetLightTowerExploreReward(const int cur_layer, bool is_fixed = false);  //fixedʱ�����±�Ϊ0ֵ
	const RandActivityLightTowerExploreCfg * GetLightTowerExploreLayerCfg(const int layer);
	const RandActivityLightTowerExploreExternCfg * GetLightTowerExploreExternReward(const int layer);
	const RandActivityLightTowerExploreReturnCfg * GetLightTowerExploreReturnReward(const int index);

	// Ȥζֲ��
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

	/////////////////////////////////////// �������� //////////////////////////////////////////////////////
	const int GetJinJieThemeActTheme(int act_id);

	// �����콱
	const int GetConsumGiftRollGoldNum() { return m_roll_reward_gold; }
	const int GetConsumGiftRollTimesLimit() { return m_roll_times_limit; }
	const ConsumGiftRewardConfig *GetConsumGiftRewardCfg(int opengame_day,int theme, int seq);
	const ConsumGiftRollRewardPool *GetConsumGiftRollNumType(int theme);
	const ConsumGiftRollRewardPool *GetConsumGiftRollReward(int theme, int seq);
	const bool GetConsumGiftRollRewardNumByType(int type, int &decade, int &units_digit);

	// ÿ���޹�
	const DailyLimitBuyConfig *GetLimitBuyCfg(int theme, int seq);

	// �۱��裨�£�
	const int GetCollectTreasureCanJoinTimes() { return m_collect_treasure_join_times; }
	const int GetCollectTreasureGold() { return m_collect_treasure_gold; }
	const CollectTreasureConfig *GetCollectTreasureCfg();
	const CollectTreasureConfig *GetCollectTreasureCfgBySeq(int seq);

	// �����۳�
	const HappyCumulChongzhiConfig *GetHappyCumulChongzhiCfg(int theme, int seq);

	// ���׷���
	const JinJieReturnConfig *GetJinJieReturnCfg(int theme, int seq);

	// ������
	const int GetCriticalStrikeDayMul(int type);
	const int GetCriticalStrikeDayRate(int type);

	// ���׷���2
	const JinJieReturnConfig *GetJinJieReturn2Cfg(int theme, int seq);

	//������2
	const int GetCriticalStrikeDay2Mul(int type);
	const int GetCriticalStrikeDay2Rate(int type);
	/////////////////////////////////////// �������� //////////////////////////////////////////////////////

	// ��������
	const int GetConsumeForGiftItemCount();
	int GetConsumeForGiftPointsPerGold();
	const RandActivityConsumeForGiftItemConfig * GetConsumeForGiftItemCfg(int seq);

	// �ؾ�Ѱ��3
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

	// �����ҵ�
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

	// ����ҡ��
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
	
	// ��һ��һ
	const BuyOneGetOneFreeItemCfg* GetBuyOneGetOneFreeCfg(int opengame_day, int item_index);


	// ��ʱ����
	const RandActivityLimitTimeRebateCfg *GetLimitTimeRebateCfg();
	const LimitTimeRebateItem *GetLimitTimeRebateItemCfg(int seq);

	// ��ʱ���
	const RandActivityLimitTimeGiftCfg::SubItemCfg * GetLimitTimeGiftRewardCfg(int opengame_day, const int vip_level, long long recharge_gold);
	const RandActivityLimitTimeGiftCfg::SubItemCfg * GetLimitTimeGiftRewardCfgBySeq(int opengame_day, const int seq);

	//ѭ����ֵ2
	int GetCirculationChongzhiSecondNeedChongzhi() { return m_circulation_chongzhi_2_need_chongzhi; }
	const ItemConfigData * GetCirculationChongzhiSecondRewardItem(int level);

	// ���ҡǮ��
	const RandActivityShakeMoneyConfig * GetShakeMoneyCfg(int opengame_day);

	// ��ʱ����
	const RandActivityLimitTimeLuxuryGiftBagCfg::SubItemCfg * GetLimitTimeLuxuryGiftBagRewardCfg(int opengame_day, const int vip_level);
	const RandActivityLimitTimeLuxuryGiftBagCfg::SubItemCfg * GetLimitTimeLuxuryGiftBagRewardCfgBySeq(int opengame_day, const int seq);

	// RMB����
	const RandActivityRmbBuyChestShopOpenDayCfg::RmbRewardCfg * GetRmbBuyChestShopCfg(int rmb_num);

	// ���ѷ���
	const RandActivityConsumeRewardStageCfg * GetConsumeRewardCfg(int vip_level, int activity_day = 0);

	// ���±��
	const RandActivityItemCollectionSecondRewardCfg::ConfigItem * GetItemCollectionSecondRewardCfg(int seq);

	// �汾�ۼƳ�ֵ
	const RandActivityTotalChargeRewardConfig * GetVersionGrandotalChargeRewardCfg();
	int GetVersionTotalChargeSeqWithCharge(int total_charge);
	const RandActivityTotalChargeRewardConfig::ConfigItem * GetVersionTotalChargeRewardWithSeq(int seq);

	// �汾������ֵ
	const RandActivityContinueChongzhiRewardConfig * GetVersionContinueChargeRewardInfoWithdayindex(int day_index, bool is_chongzhi, bool is_bu_fa_reward);

	// ����ҡ��2
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

	// ���µ��ʳ�ֵ0
	const RandActivityOfflineSingleChargeCfg::ConfigItem * GetOfflineSingleChargeCfg0(long long charge_value, unsigned int join_timestamp = 0);

	// �����ػ�
	HolidayGuardKillRankReward::ConfigItem *GetHolidayGuardKillRankRewardCfg(int role_rank);
	ItemConfigData &GetHolidayGuardParticipationRewardCfg() { return m_holiday_guard_participation_reward; }

	// �ۼƳ�ֵ5
	const RandActivityTotalCharge5Cfg::ConfigItem * GetTotalCharge5Cfg(int day_idx, int seq);
	int GetTotalCharge5CfgCount(int day_idx);
	bool CheckIsResetTotalCharge5Day(int day_idx);

	// ������ս
	int GetExtremeChallengeRefreshNeedGold() { return m_extreme_challenge_refresh_task_need_gold; }
	int GetExtremeChallengeMaxTaskNum() { return m_extreme_challenge_rand_give_role_task_num; }
	int GetExtremeChallengeRandTaskByType(char type);
	const RandActivityExtremeChallengeCfg *GetExtremeChallengePerformTaskInfoCfg(int task_id);
	ItemConfigData *GetExtremeChallengeFinishAllTaskRewardCfg() { return m_extreme_challenge_finish_all_task_reward_cfg; }

	// �������
	RandActivityChongZhiGiftConfig *GetChongZhiGiftByIndex(int index);
	const RandActivityChongZhiGiftConfig *GetChongZhiGiftCfg(int act_day, int index);

	// ������а�
	int GetProfessRankCfgCount();
	const RandActivityProfessRankCfg::ConfigItem * GetProfessRankRewardCfg(int user_rank);

	// ��Ϲ���
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

	// ��ĩ��-----------------------------------------------
	const RALottery1OtherCfg &GetLottery1OtherCfg() { return m_lottery1_other_cfg; }
	const RALottery1RewardCfg *GetLottery1RewardWeightCfg(int opengame_day);
	const RALottery1RewardItem *GetLottery1RewardCfg(int opengame_day, int seq);
	const RALottery1PersonRewardCfg *GetLottery1PersonRewardCfg(int opengame_day, int seq);
	const RALottery1ServerRewardItem *GetLottery1ServerRewardCfg(int opengame_day, int total_server_times);

	// ��Ԫ��-----------------------------------------------
	const RAZeroBuyReturnConfig *GetRAZeroBuyReturnConfig(int buy_type);
	
	const RandActivityDailyLoveConfig * GetDailyLove2RewardPrecent();

	// �������-----------------------------------------------
	int GetKuangHaiTaskNum();
	const RandActivityKuangHaiCfg::ItemCfg* GetKuangHaiTaskItem(int index);
	const std::vector<const RandActivityKuangHaiCfg::ItemCfg*>* GetKuangHaiTaskAllItems(int task_id);
	int GetKuangHaiRewardListNum();
	int GetKuangHaiRewardNum(int seq);
	const RandActivityKuangHaiRewardCfg::ItemCfg* GetKuangHaiRewardItem(int seq);

	// �����ո�-----------------------------------------------
	const GiftHarvestRankRewardCfg::ConfigItem * GetGiftHarvestRankItem(int rank);
	ItemConfigData *GetGiftHarvestJoinRewardCfg() { return m_gift_harvest_join_reward_cfg; }

	int GetLuckyWisk30TimesConsumeItem() { return m_lucky_wish_30_times_use_item; }

	// �����ƹ�
	const RandLuckyCloudBuyCfg::RoundItem* GetLuckyCloudBuyCfg(int rounds_index, unsigned int join_timestamp = 0);
	int GetLuckyBuyRoundsNum(unsigned int join_timestamp = 0);// ��ȡ���յ��ƹ�����
	bool IsLuckyBuyUnlimitTime();// �Ƿ��������ƹ����ʱ��
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
	
	//���ʳ�ֵ����
	RandActivityDanbiChongzhiDayDiffCfg m_diff_danbi_chongzhi_day_cfglist[RAND_ACTIVITY_DANBI_CHONGZHI_DAY_CFG_COUNT];

	UNSTD_STATIC_CHECK(RAND_ACTIVITY_TOTAL_CHARGE_DAY_CFG_MAX_SEQ < 16);

	int m_total_charge_day_max_seq;
	RandActivityTotalChargeDayCfg m_total_charge_day_cfglist[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	int m_tomorrow_reward_need_active_degree;
	int m_tomorrow_reward_cfg_total_weight;
	int m_tomorrow_reward_cfg_count;
	RandActivityTomorrowRewardCfg m_tomorrow_reward_cfglist[RAND_ACTIVITY_TOMORROW_REWARD_CFG_MAX_COUNT];

	int m_person_rank_bipin_activity_map[PERSON_RANK_TYPE_MAX];

	// ÿ�ճ�ֵ����
	RandActivityDayChongzhiRankRewardDiffConfig m_diff_daychongzhirank_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	// ÿ����������
	RandActivityDayConsumeRankRewardDiffConfig m_diff_dayconsume_rank_reward_cfglist[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	
	//�ۼƳ�ֵ
	RandActivityTotalChargeRewardConfig m_diff_totalcharge_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	//��ʱ--���ֶһ�ϡ��װ��
	RandActivityTimeLimitExchangeConfig m_time_limit_exchange_equi_list[TIME_LIMIT_EXCHANGE_ITEM_COUNT];
	RandActivityTimeLimitExchangeConfig m_time_limit_exchange_jl_list[TIME_LIMIT_EXCHANGE_ITEM_COUNT];

	//�������齱�
	int m_level_lottery_max_level;											// �������齱���������
	int m_level_lottery_consume_gold_level_list[RA_LEVEL_LOTTERY_MAX_LEVEL_LIMIT + 1];

	ItemID m_level_lottery_consume_item;									// ������10��������
	int m_level_lottery_cfg_list_count;										// �������齱�����������
	RandActivityAdditionLotteryConfig m_level_lottery_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	RandActivityLevelLotteryOtherCfg m_level_lottery_other_cfg;				// ��������������

	std::map<int, std::map<int, RandActivityLevelLotteryTotalTimesCfg> > m_level_lottery_total_times_map; // �������ۼƴ�������
	 
	//��ֵŤ���
	int m_niu_egg_need_charge;												// Ť��һ�������ֵ���

	int m_niu_egg_chou_total_weight;
	int m_niu_egg_chou_reward_cfg_count;
	RandActivityNiuEggChouRewardConfig m_niu_egg_chou_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		// Ť���齱��Ʒ�б�

	int m_niu_egg_server_reward_cfg_count;
	RandActivityNiuEggServerRewardConfig m_niu_egg_server_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];// Ť��ȫ��������Ʒ�б�

	//�䱦��
	int m_zhenbaoge_reflush_gold;																				// �䱦��ˢ��һ������Ԫ��
	int m_zhenbaoge_flush_item;																					// �䱦��ˢ��1��Կ��
	int m_zhenbaoge_auto_flush_times;																			// �䱦��һ��ˢ�µĴ���
	ItemConfigData m_zhenbaoge_all_buy_reward;																	// �䱦��ȫ�����������

	int m_zhenbaoge_auto_add_cfg_count;																			// �䱦������Ȩ����������
	RandActivityZhenBaoGeAddWeightDayCfg m_zhenbaoge_add_weight_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	int m_zhenbaoge_max_seq;
	RandActivityZhenBaoGeDayCfg m_zhenbaoge_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM + 1];				// �䱦��ˢ����Ʒ�б�

	int m_zhenbaoge_reward_max_seq;
	RandActivityZhenBaoGeServerDayCfg m_zhenbaoge_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM + 1];   // �䱦��ȫ������б�

	//��ֵ��Ԫ��
	typedef std::vector<RandActivityChargeRewardGoldConfig> ChargeRewardGoldVec;
	ChargeRewardGoldVec m_charge_reward_gold_vec;

	//�ؾ�Ѱ��
	int m_mijingxunbao_once_gold;																			//�ؾ�Ѱ���Ա�һ�����ķǰ�Ԫ��      
	int m_mijingxunbao_tentimes_gold;																		//�ؾ�Ѱ���Ա�ʮ�����ķǰ�Ԫ��  
	int m_mijingxunbao_fifthtimes_gold;																		//�ؾ�Ѱ���Ա���ʮ�����ķǰ�Ԫ��  
	int m_mijingxunbao_interval;																			//�ؾ�Ѱ���Ա����ʱ����(����)
	int m_add_weight_tao_times;																				//�ؾ�Ѱ������Ȩ����Ҫ�Բ�����ϡ��Ʒ����
	int m_mijingxunbao_fifthtimes_item_id;																	//�ؾ�Ѱ���Ա���ʮ��������Ʒ 

	int m_mijingxunbao_auto_add_cfg_count_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];																	// �ؾ�Ѱ������Ȩ����������
	RandActivityMiJingXunBaoAddWeightCfgList m_mijingxunbao_add_weight_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	
	int m_mijingxunbao_reward_cfg_count;																	
	RandActivityMiJingXunBaoRewardDayCfg m_mijingxunbao_reward_list_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		//�ؾ�Ѱ���۳齱��

	int m_mijingxunbao_max_seq_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityMiJingXunBaoCfgList m_mijingxunbao_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// �ؾ�Ѱ����Ʒ�б�

	//���ٳ�ս
	RandActivityJiSuChongZhanCfg m_jisuchongzhan_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// ���ٳ�ս�������ñ�

	//ҡǮ��
	int m_rock_money_tree_need_gold;												// ҡǮ��һ����Ҫ��Ԫ��
	int m_money_tree_init_pool_gold;												// ҡǮ�����س�ʼ���
	int m_money_tree_into_pool;														// ҡǮ��һ�ν��뽱�ص�Ԫ��
	int m_money_tree_free_times;													// ҡǮ�� ��Ѵ���
	int m_money_tree_free_interval;													// ҡǮ�� ��Ѽ��
	ItemID m_money_tree_consume_item;												// ҡǮ��10�ζ������

	int m_money_tree_max_stage;														// ҡǮ�� ������

	int m_money_tree_total_weight_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	int m_money_tree_item_reward_total_weight_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	int m_money_tree_reward_cfg_count_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityMoneyTreeRewardCfgList m_money_tree_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		// ҡǮ���齱��Ʒ�б�

	RandActivityServerBaodiCfgList m_money_tree_baodi_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	int m_money_tree_server_reward_cfg_count_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityMoneyTreeServerRewardCfgList m_money_tree_server_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];  // ҡǮ��ȫ����Ʒ�б�

	// �����뷭��
	int m_kingdraw_level_chuji_consume;								// �����뷭�ƣ���������������Ϣ
	int m_kingdraw_level_zhongji_consume;							// �����뷭�ƣ��м�����������Ϣ
	int m_kingdraw_level_gaoji_consume;								// �����뷭�ƣ��߼�����������Ϣ
	ItemID m_king_draw_gaoji_consume_item;							// �����뷭�ƣ��߼��������
	RandActivityKingDrawLevelConfig m_kingdraw_level_cfg_list[RA_KING_DRAW_LEVEL_COUNT][MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];	// �����뷭�ƣ�����������Ϣ
	int m_kingdraw_return_reward_cfg_count[RA_KING_DRAW_LEVEL_COUNT];
	RandActivityKingDrawReturnRewardCfg m_kingdraw_return_reward_cfg_list[RA_KING_DRAW_LEVEL_COUNT][MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];	// �����뷭�ƣ��۳齱��������Ϣ

	// �ۼƳ�ֵ3
	RandActivityTotalCharge3Config m_total_charge_3_cfglist[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	//������������
	ItemConfigData m_appearance_rank_join_reward;											// �������а������뽱��
	ItemConfigData m_appearance_rank_reward_list[RA_SPECIAL_APPEARANCE_RANK_MAX_COUNT + 1]; // �������а����н����б�

	//���������
	ItemConfigData m_appearance_passive_rank_join_reward;							// �����������а������뽱��
	ItemConfigData m_appearance_passive_rank_reward_list[RA_SPECIAL_APPEARANCE_PASSIVE_RANK_MAX_COUNT + 1];// �����������а����н����б�

	//ȫ���������
	unsigned int m_any_monster_drop_limit_drop_time;						// ȫ�����������,ɱ��������������Ʒ����Сʱ����
	UInt16 m_any_monster_drop_id;											// ȫ�����������,����ID
	int m_any_monster_drop_day_limit_rate;									// ȫ�����������,ÿ������ʹ����Ʒ����
	int m_any_monster_drop_cfg_list_count;									// ��������
	RandActivityAnyMonsterDropConfig m_any_monster_drop_cfg_list[RAND_ACTIVITY_ANY_MONSTER_DROP_CFG_COUNT];

	//�ۼƳ�ֵ4(��������)
	RandActivityTotalCharge4Config m_total_charge_4_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	//Ȥζ�ڿ�3333333333
	int m_mine_refresh_gold;										// �ڿ�ˢ�¿�ʯ����Ԫ��
	int m_mine_refresh_count;										// �ڿ�һ��ˢ�³��Ŀ�ʯ��
	int m_mine_free_times;											// �ڿ�ÿ������ڿ����
	int m_mine_server_reward_level_limit;							// �ڿ���ȡȫ������ȼ�����
	int m_mine_info_cfg_count;										// �ڿ󣬿�ʯ��Ϣ������
	RandActivityMineInfoConfig m_mine_info_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];						// �ڿ󣬿�ʯ��������
	int m_mine_server_reward_cfg_count;								// �ڿ�ȫ�����������
	RandActivityMineServerRewardConfig m_mine_server_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];	// �ڿ󣬸����������

	//	���ι�
	int m_guagua_free_time_interval;								// ���ιΣ���ѹ�һ�εļ��ʱ��
	int m_guagua_need_gold_list[RA_GUAGUA_TYPE_MAX];				// ���ι�, Ԫ�������б�
	std::map<int, int> m_guagua_total_weight;						// ���ι�, ������Ȩ��
	ItemID m_guagua_roll_item_id;										// ���ιΣ�30�������
	std::vector<RandActivityGuaGuaConfig> m_guagua_cfg_list;		// ���ιΣ����н���������Ϣ
	std::vector<RandActivityGuaGuaAccConfig> m_guagua_acc_cfg_list;

	RandActivityServerBaodiCfgList m_guagua_baodi_reward_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		//���ι�ȫ������

	// ��������
	int m_tianming_free_chou_times;									// ��������, ÿ����Ѳ��Դ���
	int m_tianming_reward_total_weight;								// �������ԣ�������Ȩ��
	int m_tianming_reward_max_seq;									// ��������, ��������±�
	RandActivityTianMingDivinationRewardConfig m_tianming_reward_cfg_list[RAND_ACTIVITY_TIANMING_DIVINATION_REWARD_MAX_SEQ]; // �������Խ���������Ϣ
	RandActivityTianMingDivinationBaodiConfig m_tianming_baodi_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];				// �������Ա��״�������

	int m_tianming_reward_add_lot_seq_count;						// �������ԣ���ǩ����
	int m_tianming_reward_add_lot_seq_list[RA_TIANMING_LOT_COUNT];	// ��������, �ɼ�ע��Ʒ�±�����
	int m_tianming_gold_consume_max_add_lot_times;					// ��������, ����ע����
	RandActivityTianMingDivinationGoldConsumeConfig m_tianming_gold_consume_cfg_list[RAND_ACTIVITY_TIANMING_DIVINATION_TOTAL_MAX_ADD_LOT_TIMES + 1];	// ��������Ԫ��������Ϣ

	// ����ת
	int m_fanfan_free_fan_times_per_day;							// ����ת��ÿ����ѷ�����
	int m_fanfan_once_need_gold;									// ����ת��һ������Ԫ��
	int m_fanfan_refresh_need_gold;									// ����ת����������Ԫ��
	int m_fanfan_auto_refresh_interval;                             // ����ת���Զ����ü����Сʱ��
	int m_fanfan_word_increase_rate;								// ����ת��������������

	int m_fanfan_item_info_cfg_count;
	RandActivityFanFanItemDayCfg m_fanfan_item_info_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		// ����ת����Ʒ��Ϣ����

	int m_fanfan_word_info_cfg_count;
	RandActivityFanFanWordDayCfg m_fanfan_word_info_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		// ����ת��������Ϣ����
	
	int m_fanfan_baodi_reward_cfg_count;
	RandActivityFanFanBaodiDayConfig m_fanfan_baodi_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];	// ����ת�۳齱������

	//������ֵ
	ItemConfigData	m_continue_chongzhi_extra_reward;				// ������ֵ�����⽱��
	int m_continue_chongzhi_fetch_extra_reward_need_days;			// ������ֵ, ��ȡ���⽱�������������ֵ�������
	int m_continue_chongzhi_max_day_index;							// ������ֵ����������±�
	RandActivityContinueChongzhiRewardConfig m_continue_chongzhi_reward_cfg_list[RA_CONTINUE_CHONGZHI_MAX_DAY_INDEX + 1];

	//�����ػݳ�
	ItemConfigData	m_continue_chongzhi_extra_reward_chu;				// �����ػݳ������⽱��
	int m_continue_chongzhi_fetch_extra_reward_need_days_chu;			// �����ػݳ�, ��ȡ���⽱�������������ֵ�������
	std::vector<RandActivityContinueChongzhiRewardConfig> m_continue_chongzhi_reward_cfg_vec_chu;

	//�����ػݸ�
	ItemConfigData	m_continue_chongzhi_extra_reward_gao;				// �����ػݸߣ����⽱��
	int m_continue_chongzhi_fetch_extra_reward_need_days_gao;			// �����ػݸ�, ��ȡ���⽱�������������ֵ�������
	std::vector<RandActivityContinueChongzhiRewardConfig> m_continue_chongzhi_reward_cfg_vec_gao;

	//��ʱ����
// 	ItemConfigData	m_continue_chongzhi_extra_reward_2;				// ��ʱ���䣬���⽱��
// 	int m_continue_chongzhi_fetch_extra_reward_need_days_2;			// ��ʱ����, ��ȡ���⽱�������������ֵ�������
	std::vector<RandActivityContinueChongzhiRewardConfig> m_continue_chongzhi_reward_cfg_vec_2;

	//��������
	int m_continue_consume_other_day_cfg_count;
	RandActivityContinueConsumeOtherDayCfg m_continue_consume_other_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];	// �������ѣ���������
	int m_continue_consume_max_reward_day_cfg_count;																	// �������ѣ���������±�
	RandActivityContinueConsumeRewardDayCfg m_continue_consume_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	//��������
	RandActivityArmyDayExchangeFlagConfig m_army_day_exchange_flag_cfg_list[RA_ARMY_DAY_ACTIVE_DEGREE_EXCHANGE_NUM];			// �������� ��Ծ�ȶһ�����
	RandActivityArmyDayExchangeItemConfig m_army_day_exchange_item_cfg_list[RA_ARMY_DAY_ARMY_SIDE_NUM];							// �������� ���Ķһ���Ʒ
	ItemConfigData m_army_day_win_side_reward;										// �������� ʤ��������

	//ѭ����ֵ
	int m_circulation_chongzhi_max_reward_count;						// ѭ����ֵ������ȡ������
	RandActivityCirculationChongzhiRewardConfig m_circulation_chongzhi_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	//��������
	int m_extreme_lucky_flush_consume_glod;													// ˢ������Ԫ��
	int m_extreme_lucky_free_draw_times;
	RandActivityExtremeLuckyDrawConsumeConfig  m_extreme_lucky_draw_consume_gold_list[RA_EXTREME_LUCKY_DRAW_CONSUME_GOLD_CFG_COUNT];		//�齱������Ӧ����Ԫ��

	int m_extreme_lucky_reward_cfg_count_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	int m_extreme_lucky_total_weight_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];													// �齱��Ʒ��Ȩ��
	RandActivityExtremeLuckyRewardCfg m_extreme_lucky_draw_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];						//�齱��Ʒ
	int m_extreme_lucky_return_reward_cfg_count;																					//�������Ô�
	RandActivityExtremeLuckyReturnRewardCfg m_extreme_lucky_return_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			//��������

	//�ؾ�Ѱ��2
	int m_mijingxunbao2_once_gold;																			//�ؾ�Ѱ���Ա�һ�����ķǰ�Ԫ��      
	int m_mijingxunbao2_tentimes_gold;																		//�ؾ�Ѱ���Ա�ʮ�����ķǰ�Ԫ��  
	int m_mijingxunbao2_fifthtimes_gold;																	//�ؾ�Ѱ���Ա���ʮ�����ķǰ�Ԫ��  
	int m_mijingxunbao2_interval;																			//�ؾ�Ѱ���Ա����ʱ����(����)
	int m_add_weight_tao2_times;																			//�ؾ�Ѱ������Ȩ����Ҫ�Բ�����ϡ��Ʒ����
	int m_mijingxunbao2_fifthtimes_item_id;																	//�ؾ�Ѱ���Ա���ʮ��������Ʒ 

	int m_mijingxunbao2_auto_add_cfg_count_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];																	// �ؾ�Ѱ������Ȩ����������
	RandActivityMiJingXunBaoAddWeightCfgList m_mijingxunbao2_add_weight_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];


	int m_mijingxunbao2_max_seq_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityMiJingXunBaoCfgList m_mijingxunbao2_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// �ؾ�Ѱ����Ʒ�б�

	//ҡǮ��2
	int m_rock_money_tree2_need_gold;												// ҡǮ��һ����Ҫ��Ԫ��
	int m_money_tree2_init_pool_gold;												// ҡǮ�����س�ʼ���
	int m_money_tree2_into_pool;													// ҡǮ��һ�ν��뽱�ص�Ԫ��

	int m_money_tree2_max_stage;														// ������

	int m_money_tree2_total_weight_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	int m_money_tree2_item_reward_total_weight_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	int m_money_tree2_reward_cfg_count_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityMoneyTreeRewardCfgList m_money_tree2_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		// ҡǮ���齱��Ʒ�б�

	int m_money_tree2_server_reward_cfg_count_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityMoneyTreeServerRewardCfgList m_money_tree2_server_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];  // ҡǮ��ȫ����Ʒ�б�

	//���ȹ���
	int m_national_day_cfg_count;																			//�һ����ø���
	RandActivityNationalDayConfig m_national_day_exchange_cfg_list[RA_NATIONAL_DAY_EXCHANGE_CFG_COUNT];		//���ȹ���һ������б�

	//�䱦��2
	int m_zhenbaoge2_reflush_gold;																				// �䱦��ˢ��һ������Ԫ��
	int m_zhenbaoge2_auto_flush_times;																			// �䱦��һ��ˢ�µĴ���
	ItemConfigData m_zhenbaoge2_all_buy_reward;																	// �䱦��ȫ�����������

	int m_zhenbaoge2_auto_add_cfg_count;																			// �䱦������Ȩ����������
	RandActivityZhenBaoGeAddWeightDayCfg m_zhenbaoge2_add_weight_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	int m_zhenbaoge2_max_seq;
	RandActivityZhenBaoGeDayCfg m_zhenbaoge2_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM + 1];						// �䱦��ˢ����Ʒ�б�

	int m_zhenbaoge2_reward_max_seq;
	RandActivityZhenBaoGeServerDayCfg m_zhenbaoge2_reward_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM + 1];   // �䱦��ȫ������б�

	// ��������
	int m_promoting_position_free_play_interval;						// ������������ѳ�ʱ������Сʱ��
	ItemID m_promoting_position_10_times_use_item;						// ����������10�������Ʒ
	int m_promoting_position_extra_time_per_chongzhi_gold;				// ������������ȡһ�ζ���齱�����ĳ齱���
	int m_promoting_position_play_once_gold;							// ������������������Ԫ��
	int m_promoting_position_10_times_gold;								// ����������10������Ԫ��

	int m_promoting_position_outside_max_seq;
	RandActivityPromotingPositionConfig m_promoting_position_outside_cfg_list[RA_PROMOTING_POSITION_OUTSIDE_REWARD_MAX_SEQ + 1];		// ������������Ȧ��������
	RandActivityPromotingPositionBaodiCfg m_promoting_position_outside_baodi_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// �������������׽�������

	int m_promoting_position_inside_max_seq;
	RandActivityPromotingPositionConfig m_promoting_position_inside_cfg_list[RA_PROMOTING_POSITION_INSIDE_REWARD_MAX_SEQ + 1];			// ������������Ȧ��������
	RandActivityPromotingPositionBaodiCfg m_promoting_position_inside_baodi_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// �������������׽�������

	int m_promoting_position_reward_cfg_count;
	RandActivityPromotingPositionRewardCfg m_promoting_position_reward_cfg_list[RA_PROMOTION_POSITION_REWARD_CFG_MAX_COUNT];			// �����������齱������������
	

	// ���о���
	RandActivityBlackMarketConfig m_black_market_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		// ������������

	// �䱦�̳�
	RandActivityTreasuresMallBuyConfig m_treasures_mall_buy_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityTreasuresMallExchangeConfig m_treasures_mall_exchange_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	// �����
	std::map<int, RandActivityOpenServerRewardConfig> m_openserver_reward_map;

	// �¾۱���
	int m_new_cornucopia_percent;
	RandActivityNewCornucopiaConfig m_new_cornucopia_cfg_list[RAND_ACTIVITY_NEW_CORNUCOPIA_CFG_COUNT_MAX];
	RandActivityNewCornucopiaTotalConfig m_new_cornucopia_total_cfg_list[RAND_ACTIVITY_NEW_CORNUCOPIA_TOTAL_CFG_COUNT_MAX];
	
	// ���ֻ
	RandActivityItemCollectionRewardCfg m_item_collection_reward_cfg;

	// ��������
	//RandActivityExpRefineRewardCfg m_exp_refine_cfg_list[RAND_ACTIVITY_EXP_REFINE_REWARD_MAX_COUNT];

	std::map<int, RandActivityExpRefineRewardCfg_2> m_exp_refine_cfg_map;

	// �������
	RandActivityRedEnvelopeGiftCfg m_red_envelope_gift_cfg;

	// ���ǽ���
	RandActivityMarryMeCfg m_marryme_cfg;

	// �����ٻ�
	RandActivityGoldenPigOtherCfg m_golden_pig_other_cfg;
	std::vector<RandActivityGoldenPigRewardCfg> m_golden_pig_reward_vec_list[RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_MAX_COUNT];

	// ����Ͷ��
	RandActivityOpenServerInvestOtherCfg m_openserver_invest_other_cfg[RandActivityOpenServerInvestOtherCfg::MAX_INVEST_TYPE];
	RandActivityOpenServerInvestRewardCfg m_openserver_invest_reward_cfg[RandActivityOpenServerInvestOtherCfg::MAX_INVEST_TYPE][RAND_ACTIVITY_OPEN_SERVER_INVEST_MAX_COUNT];

	// ��������
	RandActivitySingleChongZhiRewardCfg m_single_chongzhi_cfg;

	// �ۻ���
	int m_xianyuan_treas_buy_limit_day;										// ��Ե���⹺����������
	std::map<int,ItemConfigData> m_xianyuan_treas_all_buy_reward;			// ��Ե����ȫ����������� 
	int m_xianyuan_treas_cfg_count;
	RandActivityXianyuanTreasConfig m_xianyuan_treas_cfg_list[RAND_ACTIVITY_XIANYUAN_TREAS_CFG_COUNT];	// ��Ե����

	// ��ʱ��ɱ
	int m_rush_buying_duration;												// ��ʱ��ɱ-��������ʱ�䣨���ӣ�
	RandActivityRushBuyingPhaseDayConfig m_rush_buying_phase_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	// ���ٳ�ս2
	RandActivitySingleChargeCfg m_single_charge_cfg2[RAND_ACTIVITY_SINGLE_CHARGE2_CFG_COUNT];

	// ���ٳ�ս3
	RandActivitySingleChargeCfg m_single_charge_cfg3[RAND_ACTIVITY_SINGLE_CHARGE3_CFG_COUNT];

	// ���ٳ�ս4
	RandActivitySingleChargeCfg m_single_charge_cfg4[RAND_ACTIVITY_SINGLE_CHARGE4_CFG_COUNT];

	// ���ٳ�ս5
	RandActivitySingleChargeCfg m_single_charge_cfg5[RAND_ACTIVITY_SINGLE_CHARGE5_CFG_COUNT];

	//��ͼѰ��
	int m_map_hunt_free_xunbao_count;																		//ÿ����Ѵ���
	int m_map_hunt_free_flush_interval;																		//�Զ�ˢ�¼�������ӣ�
	int m_map_hunt_xunbao_glod;																				//Ѱ������Ԫ��
	int m_map_hunt_flush_gold;																				//ˢ������Ԫ��
	int m_map_hunt_total_city_weight_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];							//������Ȩ��

	RandActivityMapHuntCityConfig m_map_hunt_city_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];						//Ѱ����ͼ��������
	RandActivityMapHuntRouteConfig m_map_hunt_route_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];					//��ͼѰ��·������
	RandActivityMapHuntServerRewardConfig m_map_hunt_server_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			//ȫ����������

	//����Ѱ��
	int m_light_tower_explore_one_gold;					//Ѱ��һ������Ԫ��
	int m_light_tower_explore_ten_gold;					//Ѱ��ʮ������Ԫ��

	RandActivityLightTowerExploreConfig m_light_tower_explore_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];								//����Ѱ��
	RandActivityLightTowerExploreExternConfig m_light_tower_explore_extern_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			//ÿ����⽱��
	RandActivityLightTowerExploreServerConfig m_light_tower_explore_server_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			//����Ѱ��ȫ������

	//���ۼƳ�ֵ
	RandActivityNewTotalChargeRewardConfig m_diff_new_totalcharge_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	//���ۼƳ�ֵ1
	RandActivityNewTotalChargeRewardConfig m_diff_new_totalcharge1_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	//���ۼƳ�ֵ2
	RandActivityNewTotalChargeRewardConfig m_diff_new_totalcharge2_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	//���ۼƳ�ֵ3
	RandActivityNewTotalChargeRewardConfig m_diff_new_totalcharge3_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	//��װ�̵�
	std::vector<RandActivityMagicShopConfig> m_magic_shop_cfg_list;

	//ֲ��
	int m_planting_tree_tree_living_time;								// Ȥζֲ���������ʱ��
	int m_planting_tree_max_watering_times;								// Ȥζֲ����һ������ཽˮ����
	int m_planting_tree_gather_id;										// Ȥζֲ�����ɼ���id
	int m_planting_tree_watering_time;									// Ȥζֲ������ˮ(�ɼ�)����ʱ��
	int m_planting_tree_scene_id;										// Ȥζֲ��������id
	ItemID m_planting_tree_norexitem_id;								// Ȥζֲ��������ʹ����id
	ItemConfigData m_planting_tree_canyu_reward;						// Ȥζֲ�������뽱

	int m_planting_tree_rank_reward_count;								// Ȥζֲ�������а�������
	RandActivityPlantingTreeRankRewardConfig m_planting_tree_rank_reward_cfg_list[RA_TREE_PLANTING_RANK_MAX_COUNT + 1];

	// ����
	short m_fish_time_limit;
	short m_steal_fish_percent;
	short m_steal_fish_time_limit;
	short m_be_stealed_fish_time_limit;
	RandActivityFishingInfoCfg m_fish_info_list[RA_FISHING_FISH_TYPE_COUNT + 1];
	RandActivityFishingRankCfg m_fish_rank_reward_list[RA_FISHING_RANK_MAX_COUNT + 1];

	//����Ȼˮ��
	int m_crystal_dalaran_initial_count;
	short m_steal_crystal_dalaran_time_limit;
	short m_crystal_dalaran_be_stealed_time_limit;
	short m_steal_crystal_dalaran_one_people_time_limit;
	short m_steal_crystal_dalaran_percent;
	short m_broadcast_percent;
	unsigned int m_steal_crystal_dalaran_limit;
	RandActivityStealCrtstalDalaranRewardCfg m_steal_crystal_dalaran_reward[RA_CRYSTAL_DALARAN_RANK_REWARD_COUNT + 1];

	// ��Ԫ������
	typedef std::vector<RandActivityChongzhiCrazyRebateConfig> ChongzhiCrazyRebateVec;
	ChongzhiCrazyRebateVec m_chongzhi_crazy_rebate_vec;

	//////////////////////////////// ���������ջ ///////////////////////////////////////////
	std::map<long long, JinjieThemeActConfig> m_jinjie_theme_act_map;

	// �����콱����
	int m_roll_reward_gold;          // ��ҡ��������
	int m_roll_times_limit;
	typedef std::vector<ConsumGiftRewardConfig>::iterator ConsumGiftRewardVecIt;
	std::vector<ConsumGiftRewardConfig> m_consum_gift_vec;

	std::vector<ConsumGiftRollRewardPool> m_consum_gift_roll_reward_pool;
	typedef std::vector<ConsumGiftRollRewardConfig> ConsumGiftRollRewardCfgVec;
	ConsumGiftRollRewardCfgVec m_consum_gift_roll_reward_vec;

	// ÿ���޹�
	DailyLimitBuyConfig m_daily_limit_buy_cfg[JINJIE_ACT_THEME_TYPE_MAX][RAND_ACTIVITY_DAILY_LIMIT_BUY_MAX_SEQ];

	// �۱��裨�£�
	int m_collect_treasure_gold;
	int m_collect_treasure_total_rate;
	int m_collect_treasure_join_times;
	CollectTreasureConfig m_collect_treasure_cfg[RAND_ACTIVITY_COLLECT_TREASURE_MAX_CFG];

	// �����۳�
	HappyCumulChongzhiConfig m_happy_cumul_chongzhi_cfg[JINJIE_ACT_THEME_TYPE_MAX][RAND_ACTIVITY_HAPPY_CUMUL_CHONGZHI_MAX_CFG];

    //������
	CriticalStrikeDayConfig m_critical_strike_day_cfg[JINJIE_ACT_THEME_TYPE_MAX];

	// ���׷���
	JinJieReturnConfig m_jinjie_return_cfg[JINJIE_ACT_THEME_TYPE_MAX][RAND_ACTIVITY_JINJIE_RETURN_MAX_CFG];
	
	//�����뷭�ƣ�������
	//int m_kingdraw_level_primary_score;
	//int m_kingdraw_level_middle_score;
	//int m_kingdraw_level_high_score;

	// ���ѻ�����
	int m_consume_for_gift_points_per_gold;								// ���ѻ�����ÿ��Ԫ������������
	std::map<int, int> m_consume_for_gift_item_count_map;
	std::vector<RandActivityConsumeForGiftItemConfig> m_consume_for_gift_item_cfg_list;

	//�ؾ�Ѱ��3
	int m_mijingxunbao3_once_gold;																			//�ؾ�Ѱ���Ա�һ�����ķǰ�Ԫ��      
	int m_mijingxunbao3_tentimes_gold;																		//�ؾ�Ѱ���Ա�ʮ�����ķǰ�Ԫ��  
	int m_mijingxunbao3_thirtytimes_gold;																	//�ؾ�Ѱ���Ա���ʮ�����ķǰ�Ԫ��  
	int m_mijingxunbao3_interval;																			//�ؾ�Ѱ���Ա����ʱ����(����)
	int m_add_weight_tao3_times;																			//�ؾ�Ѱ������Ȩ����Ҫ�Բ�����ϡ��Ʒ����
	int m_mijingxunbao3_thirtytimes_item_id;																	//�ؾ�Ѱ���Ա���ʮ��������Ʒ 
	int m_mijingxunbao3_free_times;																			//�ؾ�Ѱ��3��ѳ齱����

	int m_mijingxunbao3_auto_add_cfg_count_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];						// �ؾ�Ѱ������Ȩ����������
	RandActivityMiJingXunBaoAddWeightCfgList m_mijingxunbao3_add_weight_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	int m_mijingxunbao3_reward_cfg_count;
	RandActivityMiJingXunBaoRewardDayCfg m_mijingxunbao3_reward_list_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		//�ؾ�Ѱ���۳齱��

	int m_mijingxunbao3_daily_cfg_max_count;
	int m_mijingxunbao3_max_seq_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityMiJingXunBaoCfgList m_mijingxunbao3_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// �ؾ�Ѱ����Ʒ�б�

	RandActivityServerBaodiCfgList m_mijingxunbao3_baodi_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// �ؾ�Ѱ��3����

	//�����ҵ�
	int m_huanlezadan_once_gold;																			//�����ҵ��Ա�һ�����ķǰ�Ԫ��      
	int m_huanlezadan_tentimes_gold;																		//�����ҵ��Ա�ʮ�����ķǰ�Ԫ��  
	int m_huanlezadan_thirtytimes_gold;																		//�����ҵ��Ա���ʮ�����ķǰ�Ԫ��  
	int m_huanlezadan_interval;																				//�����ҵ��Ա����ʱ����(����)
	int m_huanlezadan_add_weight_tao_times;																	//�����ҵ�����Ȩ����Ҫ�Բ�����ϡ��Ʒ����
	int m_huanlezadan_thirtytimes_item_id;																	//�����ҵ��Ա���ʮ��������Ʒ 
	int m_huanlezadan_free_times;																			//�����ҵ���ѳ齱����

	int m_huanlezadan_auto_add_cfg_count_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];						// �����ҵ�����Ȩ����������
	RandActivityMiJingXunBaoAddWeightCfgList m_huanlezadan_add_weight_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	int m_huanlezadan_reward_cfg_count;
	RandActivityMiJingXunBaoRewardDayCfg m_huanlezadan_reward_list_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		//�����ҵ��۳齱��

	int m_huanlezadan_daily_cfg_max_count;
	int m_huanlezadan_max_seq_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityMiJingXunBaoCfgList m_huanlezadan_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// �����ҵ���Ʒ�б�

	//����ҡ��
	int m_huanleyaojiang_once_gold;																			//����ҡ���Ա�һ�����ķǰ�Ԫ��      
	int m_huanleyaojiang_tentimes_gold;																		//����ҡ���Ա�ʮ�����ķǰ�Ԫ��  
	int m_huanleyaojiang_thirtytimes_gold;																	//����ҡ���Ա���ʮ�����ķǰ�Ԫ��  
	int m_huanleyaojiang_interval;																			//����ҡ���Ա����ʱ����(����)
	int m_huanleyaojiang_add_weight_tao_times;																//����ҡ������Ȩ����Ҫ�Բ�����ϡ��Ʒ����
	int m_huanleyaojaing_thirtytimes_item_id;																//����ҡ���Ա���ʮ��������Ʒ 
	int m_huanleyaojiang_free_times;																		//����ҡ����ѳ齱����

	int m_huanleyaojiang_auto_add_cfg_count_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];					// ����ҡ������Ȩ����������
	RandActivityMiJingXunBaoAddWeightCfgList m_huanleyaojiang_add_weight_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	int m_huanleyaojiang_reward_cfg_count;																				//����ҡ���۳齱������
	RandActivityMiJingXunBaoRewardDayCfg m_huanleyaojiang_reward_list_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		//����ҡ���۳齱��

	int m_huanleyaojiang_stage_cfg_max_count;
	int m_huanleyaojiang_max_seq_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityMiJingXunBaoCfgList m_huanleyaojiang_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// ����ҡ����Ʒ�б�

	RandActivityServerBaodiCfgList m_huanleyaojiang_baodi_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// ����ҡ��ȫ������

	//��ʱ����
	RandActivityLimitTimeRebateCfg m_limit_time_rebate_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	// ��ʱ���
	RandActivityLimitTimeGiftCfg m_limit_time_gift_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	//ѭ����ֵ2
	int m_circulation_chongzhi_2_need_chongzhi;																			//ѭ����ֵ2�����ֵ��
	std::vector<RandActivityCirculationChongzhiSecondRewardCfg> m_circulation_chongzhi_2_reward_cfg_vec;				//ѭ����ֵ2����

	// ���ҡǮ��
	RandActivityShakeMoneyConfig m_shakemoney_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];	

	// ��ʱ����
	RandActivityLimitTimeLuxuryGiftBagCfg m_limit_time_luxury_gift_bag_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	// RMB����
	RandActivityRmbBuyChestShopOpenDayCfg m_rmb_buy_ches_shop_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	// ���ѷ���
	int m_consume_reward_stage_cfg_count;
	RandActivityConsumeRewardCfg m_consume_reward_stage_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	// ���±��
	RandActivityItemCollectionSecondRewardCfg m_item_collection_2_reward_cfg;

	// �汾�ۼƳ�ֵ
	RandActivityTotalChargeRewardConfig m_grand_total_charge_reward_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	// �汾������ֵ
	std::vector<RandActivityContinueChongzhiRewardConfig> m_continue_charge_reward_cfg_vec;

	//����ҡ��2
	int m_huanleyaojiang_2_once_gold;																			//����ҡ��2�Ա�һ�����ķǰ�Ԫ��      
	int m_huanleyaojiang_2_tentimes_gold;																		//����ҡ��2�Ա�ʮ�����ķǰ�Ԫ��  
	int m_huanleyaojiang_2_thirtytimes_gold;																	//����ҡ��2�Ա���ʮ�����ķǰ�Ԫ��  
	int m_huanleyaojiang_2_interval;																			//����ҡ��2�Ա����ʱ����(����)
	int m_huanleyaojiang_2_add_weight_tao_times;																//����ҡ��2����Ȩ����Ҫ�Բ�����ϡ��Ʒ����
	int m_huanleyaojaing_2_thirtytimes_item_id;																	//����ҡ��2�Ա���ʮ��������Ʒ 
	int m_huanleyaojiang_2_free_times;																			//����ҡ��2��ѳ齱����

	int m_huanleyaojiang_2_auto_add_cfg_count_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];						// ����ҡ��2����Ȩ����������
	RandActivityMiJingXunBaoAddWeightCfgList m_huanleyaojiang_2_add_weight_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	int m_huanleyaojiang_2_reward_cfg_count;																	//����ҡ��2�۳齱������
	RandActivityMiJingXunBaoRewardDayCfg m_huanleyaojiang_2_reward_list_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];		//����ҡ��2�۳齱��

	int m_huanleyaojiang_2_stage_cfg_max_count;
	int m_huanleyaojiang_2_max_seq_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	RandActivityMiJingXunBaoCfgList m_huanleyaojiang_2_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// ����ҡ��2��Ʒ�б�

	RandActivityOfflineSingleChargeCfg m_offline_single_charge_cfg_0[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];	// ���µ��ʳ�ֵ0

	// ��һ��һ����
	RandActivityBuyOneGetOneFreeConfig m_buy_one_get_one_free_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
	
	int m_everyday_nice_gift_cfg_count;																			// ��������
	RandActivityEverydayNiceGiftCfg m_everyday_nice_gift_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];			// ÿ�պ���
	RandActivityLoginGiftCfg m_login_gift_cfg_0;																// ��½����

	// �����ػ�
	int m_holiday_guard_kill_rank_num;																			// ������������
	HolidayGuardKillRankReward	m_holiday_guard_kill_rank_reward_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];	// ��ɱ���н���
	ItemConfigData m_holiday_guard_participation_reward;														// ���뽱��

	// �ۼƳ�ֵ5
	RandActivityTotalCharge5Config m_total_charge_5_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	//������ս
	int m_extreme_challenge_rand_give_role_task_num;															// �������ҵ���������
	int m_extreme_challenge_refresh_task_need_gold;																// ˢ��������������Ԫ��
	int m_extreme_challenge_reward_count;																		// ��������
	RandActivityExtremeChallengeCfg m_extreme_challenge_cfg[RAND_ACTIVITY_EXTREME_CHALLENGE_MAX_TASK_COUNT];	// ������ս
	ItemConfigData m_extreme_challenge_finish_all_task_reward_cfg[MAX_ATTACHMENT_ITEM_NUM];						// �������������

	//�������
	std::vector<RandActivityChongZhiGiftConfig> m_chongzhi_gift_cfg_list;
	
	// ������а�
	RandActivityProfessRankCfg m_profess_rand_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	// ���׷���2
	JinJieReturnConfig m_jinjie_return2_cfg[JINJIE_ACT_THEME_TYPE_MAX][RAND_ACTIVITY_JINJIE_RETURN_MAX_CFG];

	//������2
	CriticalStrikeDayConfig m_critical_strike_day2_cfg[JINJIE_ACT_THEME_TYPE_MAX];

	// ��Ϲ���
	int m_combine_buy_discount_cfg_list_count;
	RandActivityCombineBuyDiscountCfg m_combine_buy_discount_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	int m_combine_buy_item_cfg_list_count;
	RandActivityCombineBuyItemCfg m_combine_buy_Item_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	RandActivityLuckyWishLotteryAndTotalWeightCfg m_lucky_wish_lottery_and_total_weight_cfg_list[RAND_ACTIVITY_LUCKY_WISH_LOTTERY_CFG_COUNT];
	std::vector<RandActivityLuckyWishLuckyCfg> m_lucky_wish_lucky_cfg_vec;

	int m_image_competition_max_opengame_day;
	std::vector<RandActivityImageCompetition> m_image_competition_cfg_vec;
	RandActivityImageCompetitionReward m_image_competition_reward_cfg[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	// ��ĩ��-----------------------------------------------
	int InitLottery1RewardCfg(PugiXmlNode RootElement);
	int InitLottery1PersonRewardCfg(PugiXmlNode RootElement);
	int InitLottery1ServerRewardCfg(PugiXmlNode RootElement);

	RALottery1OtherCfg m_lottery1_other_cfg;
	std::map<int, RALottery1RewardCfg> m_lottery_reward_map;
	std::map<int, std::vector<RALottery1PersonRewardCfg>> m_lottery_person_reward_map;
	std::map<int, std::map<int, RALottery1ServerRewardCfg>> m_lottery_server_reward_map;

	std::vector<RADailyLoveOpengameDayCfg> m_daily_love_reward_precent_vec;

	// ��Ԫ��-----------------------------------------------
	std::map<int, RAZeroBuyReturnConfig> m_zero_buy_return_map;  //��Ԫ�� map<buy_type, cfg>

	// ÿ��һ��2
	RandActivityDailyLoveConfig m_daily_love_2_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];

	// �������-----------------------------------------------
	RandActivityKuangHaiCfg m_kuang_hai_cfg;
	RandActivityKuangHaiRewardCfg m_kuang_hai_reward_cfg;

	// �����ո�-----------------------------------------------
	GiftHarvestRankRewardCfg m_gift_harvest_rank_reward_cfg;
	ItemConfigData m_gift_harvest_join_reward_cfg[MAX_ATTACHMENT_ITEM_NUM];						// �����ʱ �ɷ��Ĳ��뽱

	int m_lucky_wish_30_times_use_item;								//������Ը30�����Կ��

	RandLuckyCloudBuyCfg m_lucky_buy_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];	// �����ƹ�����
	RandLuckyCloudBuyOtherCfg m_lucky_buy_other;	// �����ƹ���������

	std::map<int, std::vector<FirstChargeTuanCfg>>	m_first_charge_tuan_refresh_timp_map;

	// �񻶴��ֹ�
	RandActivityCrazyBuy m_cracy_buy[RAND_ACTIVITY_CRACY_BUY_MAX_OF_OPENGAMEDAY];

	//˫��Ԫ��
	RandActivityDoubleGet m_double_get_cfg_list[MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM];
};
#endif