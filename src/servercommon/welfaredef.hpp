#ifndef __WELFARE_DEF_HPP__
#define __WELFARE_DEF_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"

static const int DURLOGIN_LOTTORY_MAX = 3;						// 连续登陆最多抽奖次数配置
static const int DURLOGIN_LOTTORY_ITEM_MAX = 9;					// 连续登陆抽奖最多物品数目

static const int MAX_GROWTH_VALUE_GET_TYPE = 3;					// 欢乐果树长成值获得方式
	
static const int DAYS_PER_WEEK = 7;								// 一周天数
static const int WEEKLY_ACCOUNT_LOGIN_MAX = 3;					// 周累计礼包最大数 

static const int COUNT_LOGIN_MAX = 60;							// 累计登陆 最大计数

static const int DAILY_ONLINE_FETCH_GIFT_MAX = 8;				// 在线礼包最大领取个数
static const int DAILY_ONLINE_LEVEL_REWARD_INTERVAL_MAX = 20;	// 在线奖励等级最大分段数
static const int DAILY_ONLINE_REWRARD_ITEM_MAX = 3;				// 在线礼包奖励最大物品个数
static const int DAILY_ONLINE_RAND_REWRARD_ITEM_MAX = 8;		// 在线礼包随机奖励配置最大物品个数

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const int OFFLINE_EXP_GET_MAX_TIME = 12 * 3600;			// 离线经验最多找回时间

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const int SEVEN_DAY_LOGIN_MAX_REWARD_DAY = 7;			// 七天登录奖励 最大奖励天数

// enum OFFLINE_EXP_FIND_TYPE
// {
// 	OFFLINE_EXP_FIND_FREE = 0,									// 离线经验找回 免费
// 	OFFLINE_EXP_FIND_VIP,										// 离线经验找回 vip
// 	OFFLINE_EXP_FIND_VIP4,										// 离线经验找回 vip4
// 	OFFLINE_EXP_FIND_MAX
// };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const int WELFARE_DAILY_FIND_MAXDAYS = 1;				// 日常找回 最多天数
static const int WELFARE_SECONDS_ONE_DAY = 24 * 3600;			// 一天秒数

enum DAILYFIND_GET_TYPE
{
	DAILYFIND_GET_FREE = 0,										// 免费领取
	DAILYFIND_GET_GOLD,											// 元宝领取
	DAILYFIND_GET_MAX,
};

enum DAILYFIND_TYPE
{
	DAILYFIND_TYPE_FB_EXP = 0,									// 经验本
	DAILYFIND_TYPE_PATA = 1,									// 爬塔
	DAILYFIND_TYPE_STORY_FB = 2,								// 剧情副本
	DAILYFIND_TYPE_UPGRADE_MOUNT_FB = 3 ,						// 坐骑进阶本
	DAILYFIND_TYPE_UPGRADE_SHIZHUANG_BODY_FB = 4,				// 时装进阶本
	DAILYFIND_TYPE_UPGRADE_SHIZHUANG_WUQI_FB = 5,				// 神兵进阶本
	DAILYFIND_TYPE_UPGRADE_LINGTONG_FB = 6,						// 灵童进阶本
	DAILYFIND_TYPE_UPGRADE_FIGHT_MOUNT_FB =7,					// 战骑进阶本
	DAILYFIND_TYPE_UPGRADE_LINGGONG_FB =8,						// 灵弓进阶本
	DAILYFIND_TYPE_UPGRADE_LINGQI_FB =9,						// 灵骑进阶本
	DAILYFIND_TYPE_TUMO =10,									// 屠魔(日常任务)
	DAILYFIND_TYPE_GUILD_TASK =11,								// 公会任务
	DAILYFIND_TYPE_QUESTION =12,								// 答题-旧版
	DAILYFIND_TYPE_HUSONG =13,									// 护送
	DAILYFIND_TYPE_PAOHUAN = 14,								// 跑环任务
	DAILYFIND_TYPE_EQUIP_FB = 15,								// 武器
	DAILYFIND_TYPE_TOWER_DEFEND_FB = 16,						// 防具
	DAILYFIND_TYPE_CHALLENGE_FB = 17,							// 品质
	DAILYFIND_TYPE_ARMOR_FB = 18,								// 守护
	DAILYFIND_TYPE_TEAM_EQUIP_FB = 19,							// 组队爬塔
	DAILYFIND_TYPE_TEAM_TOWER_DEFEND_FB = 20,					// 组队守护

	DAILYFIND_TYPE_MAX,
};

enum PLAT_WELFARE_TYPE
{
	PLAT_WELFARE_YY,											// YY平台
	PLAT_WELFARE_360,											// 360平台
	PLAT_WELFARE_XUNLEI,										// 迅雷平台
	PLAT_WELFARE_MAX,
};

enum PLAT_VIP_REWARD_TYPE
{
	PLAT_VIP_REWARD_LOGIN = 0,									// 登录礼包
	PLAT_VIP_REWARD_LEVEL = 10,									// VIP等级礼包
	PLAT_VIP_REWARD_360_BOOST_BALL = 15,						// 360加速球礼包
	PLAT_VIP_REWARD_MAX = 31,
};

const static int MAX_HAPPY_TREE_LEVEL = 20;						// 欢乐果树最高等级

enum GET_HAPPYTREE_VALUE_TYPE									// 欢乐果树长成值获得类型
{
	GET_HAPPYTREE_VALUE_TYPE_ONLINE = 0,
	GET_HAPPYTREE_VALUE_TYPE_ZHIBAO_EXP = 1,
	GET_HAPPYTREE_VALUE_TYPE_CHONGZHI = 2,

	GET_HAPPYTREE_VALUE_TYPE_MAX
};

UNSTD_STATIC_CHECK(PLAT_VIP_REWARD_MAX < 32);

// 经验药水类型
enum EXP_BUFF_TYPE
{
	EXP_BUFF_TYPE_INVALID = -1,
	EXP_BUFF_TYPE_1,
	EXP_BUFF_TYPE_2,
	EXP_BUFF_TYPE_3,
	EXP_BUFF_TYPE_4,

	EXP_BUFF_TYPE_MAX,
};

UNSTD_STATIC_CHECK(EXP_BUFF_TYPE_MAX <= 4);



struct ExpBuffInfo
{
	struct ExpBuffItem
	{
		void Reset()
		{
			exp_buff_left_time_s = 0;
			exp_buff_rate = 0;
			exp_buff_level = 0;
		}

		int exp_buff_left_time_s = 0;												// 经验药水BUFF剩余时间
		short exp_buff_rate = 0;													// 经验药水buff倍数
		char exp_buff_level = 0;													// 经验药水buff等级
		char reserve_ch = 0;
	};

	void Reset()
	{
		memset(exp_buff_list, 0, sizeof(exp_buff_list));
	}

	int GetCurMaxExpBuffType()
	{
		int buff_type = EXP_BUFF_TYPE_INVALID;
		for (int i = 0; i < EXP_BUFF_TYPE_MAX; i ++)
		{
			if (exp_buff_list[i].exp_buff_left_time_s > 0)
			{
				buff_type = i;
			}
		}

		return buff_type;
	}

	ExpBuffItem exp_buff_list[EXP_BUFF_TYPE_MAX];
};

struct DailyFindItem
{
	DailyFindItem() { this->Reset(); }

	bool Invalid() const { return 0 == find_time; }

	void Reset()
	{
		find_time = 0;
		role_level = 0;

		find_count = 0;
		param = 0;
		param1 = 0;
	}

	unsigned int find_time;
	short role_level;

	int find_count;
	int param;
	long long param1;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push, 4)

struct DailyFindRewardItem
{
	const static int ITEM_MAX_COUNT = 8;

	DailyFindRewardItem() { this->Reset(); }

	void Reset()
	{
		find_type = -1;
		role_level = 0;
		find_times = 0;
		reserve_sh = 0;

		exp = 0;
		bind_coin = 0;
		honor = 0;
		banggong = 0;
		gold_need = 0;
		coin_need = 0;

		item_count = 0;

		memset(item_list, 0, sizeof(item_list));
	}

	short find_type;
	short role_level;
	short find_times;
	short reserve_sh;

	long long exp;
	int bind_coin;
	int honor;
	int banggong; //  仙盟贡献
	int gold_need;
	int coin_need;

	int item_count;
	ItemConfigData item_list[ITEM_MAX_COUNT];
};

struct WelfareParam
{
	WelfareParam() { this->Reset(); }

	void Reset()
	{
		offline_second = 0;
		old_offline_exp = 0;

		chongzhi_count = 0;
		account_total_login_daycount = 1;
		sign_in_days = 0;
		login_level = 0;
		is_chongzhi_today = 0;
		continuity_signin_days = 0;
		sign_in_reward_mark = 0;
		activity_find_flag = 0;
		activity_join_flag = 0;
		auto_activity_flag = 0;
		last_signin_time = 0;
		online_reward_mark = 0;
		happy_tree_level = 0;
		sign_in_today_times = 0;
		is_not_first_send = 0;
		happy_tree_reward = 0;
		chongjihaoli_reward_mark = 0;
		memset(happy_tree_grouth_val, 0, sizeof(happy_tree_grouth_val));
		total_happy_tree_growth_val = 0;
		seven_day_login_fetch_reward_mark = 0;
		offline_mojing = 0;
		offline_exp = 0;
		
		collect_left_time_s = 0;

		happy_tree_online_times = 0;
		today_chongzhi_gold_num = 0;

		exp_buff_info.Reset();
	}

	int offline_second;														// 离线时间
	int old_offline_exp;														// 离线经验(不用了)
	int chongzhi_count;														// 充值次数
	int account_total_login_daycount;										// 一生累计登录天数
	int sign_in_days;														// 签到天数
	short login_level;														// 登录等级
	char is_chongzhi_today;													// 今天是否充值
	char continuity_signin_days;											// 连续签到天数
	int sign_in_reward_mark;												// 签到奖励标记
	int activity_find_flag;													// 活动找回标记
	int activity_join_flag;													// 活动参与标记 每日清零
	int auto_activity_flag;													// 活动委托标记 每日清零
	int last_signin_time;													// 最后签到时间
	int online_reward_mark;													// 今日在线奖励领取标记
	short happy_tree_level;													// 欢乐果树等级
	char sign_in_today_times;												// 今日签到次数
	char is_not_first_send;													// 是否第一次领取离线经验
	short happy_tree_reward;												// 在线果树奖励领取标记
	short chongjihaoli_reward_mark;											// 冲级豪礼领取标记
	short happy_tree_grouth_val[MAX_GROWTH_VALUE_GET_TYPE + 1];				// 欢乐果树长成值
	int total_happy_tree_growth_val;										// 总欢乐果树长成值
	unsigned int happy_tree_online_times;									// 在线时长
	long long today_chongzhi_gold_num;										// 今天充值钻石可领取欢乐果树长成值
	int seven_day_login_fetch_reward_mark;									// 七天累计登录奖励领取标记
	int offline_mojing;														// 离线魔晶
	long long offline_exp;													// 离线经验(新)
	
	int collect_left_time_s;												// 集字活动剩余时间
	ExpBuffInfo exp_buff_info;												// 经验药水buff信息
};

typedef char WelfareParamHex[sizeof(WelfareParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(WelfareParamHex) < 512);

#pragma pack(pop)

#endif // __WELFARE_DEF_HPP__

