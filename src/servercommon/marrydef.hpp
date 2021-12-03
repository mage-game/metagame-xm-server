#ifndef __MARRY_DEF_HPP__
#define __MARRY_DEF_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/serverdef.h"

#pragma pack(push, 4)

enum MARRY_LEVEL_TYPE
{
	MARRY_LEVEL_TYPE_INVALID = 0,
	MARRY_LEVEL_TYPE_POTONG,									// 档次 普通（绑钻）
	MARRY_LEVEL_TYPE_HAOHUA,									// 档次 豪华（钻石）
	MARRY_LEVEL_TYPE_MAX,
};

enum MARRY_STATUS
{
	MARRY_STATUS_INVALID = 0,
	MARRY_STATUS_ZHUN_BEI,										// 婚礼准备
	MARRY_STATUS_BAI_TIAN_DI_ZHUN_BEI,							// 拜天地准备
	MARRY_STATUS_BAI_TIAN_DI,									// 拜天地
	MARRY_STATUS_XUN_YOU,										// 巡游
	MARRY_STATUS_HUN_YAN_ZHUN_BEI,								// 婚宴准备				
	MARRY_STATUS_HUN_YAN,										// 婚宴
	MARRY_STATUS_FINISH,										// 婚礼结束
};

enum MARRY_XUNYOU_OPERA_TYPE
{
	MARRY_XUNYOU_OPERA_TYPE_INVALID = 0,
	MARRY_XUNYOU_OPERA_TYPE_SA_HONGBAO,							// 巡游撒红包
	MARRY_XUNYOU_OPERA_TYPE_RANFANG_YANHUA,						// 巡游燃放烟花
	MARRY_XUNYOU_OPERA_TYPE_MEIGUI_BIAOBAI,						// 巡游玫瑰表白
	MARRY_XUNYOU_OPERA_TYPE_MAX,
};

static const int MARRY_MAX_DAILY_MERRY_NUM = 24;				// 每天举行的最大婚礼数目
static const int MARRY_COUPLE_USER_LIMIT_NUM = 2;				// 一场婚礼只允许两个人
static const int MARRY_ROLE_LEVEL_LIMIT = 50;					// 结婚所需等级
static const int MARRY_REWARD_ITEM_MAX_NUM = 2;					// 结婚奖励物品最大数量
static const int MARRY_XUNYOU_POINT_MAX_NUM = 30;				// 结婚巡游最大点数量
static const int MARRY_HUNYAN_INVITE_USER_MAX_NUM = 50;			// 结婚婚宴喜帖最大邀请人数
static const int MARRY_HUNYAN_BLESS_GIFT_MAX_NUM = 6;			// 结婚婚宴祝福贺礼最大数量
static const int MARRY_MARRYOBJ_MAX_NUM = 7;					// 结婚对象最大数量
static const int MARRY_HUNYAN_SCENE_KEY = 1;					// 结婚场景key
static const int MARRY_LEVEL_REWARD_SHIZHUANG_NUM = 8;			// 结婚奖励时装配置数
static const int MARRY_HUNYAN_OPERA_LANGMAN_WUSHUANG_DIS = 20;	// 结婚婚宴甜蜜无双要求两个角色的距离
static const int PER_DAY_JOIN_YANHUI_MAX_COUNT = 3;				// 每天最多参加宴会次数

static const int MARRY_MIN_INTERVAL_FROM_DIVORCE = 5 * 24 * 3600;// 再次结婚最小时间间隔
static const int DIVORCE_MIN_INTERVAL_FROM_MARRY = 24 * 3600;	// 离婚距结婚最小时间间隔

struct SyncMarryInfo
{
	SyncMarryInfo() { this->Reset(); }

	void Reset()
	{
		memset(uid_list, 0, sizeof(uid_list));
		memset(user_name_list, 0, sizeof(user_name_list));

		marry_level = 0;
		marry_status = 0;

		memset(camp_list, 0, sizeof(camp_list));

		total_marry_count = 0;
		marryobjpos_x = 0;
		marryobjpos_y = 0;

		marry_start_time = 0;
	}

	int uid_list[MARRY_COUPLE_USER_LIMIT_NUM];					// 结婚双方ID
	GameName user_name_list[MARRY_COUPLE_USER_LIMIT_NUM];		// 结婚双方名字
	char marry_level;											// 结婚的档次
	char marry_status;											// 结婚当前状态
	char camp_list[MARRY_COUPLE_USER_LIMIT_NUM];				// 双方阵营
	int total_marry_count;										// 全服结婚总场数
	short marryobjpos_x;										// 结婚对象坐标X
	short marryobjpos_y;										// 结婚对象坐标Y
	unsigned int marry_start_time;								// 婚礼开始时间
};

struct MarryReserve
{
	MarryReserve() { this->Reset(); }

	void Reset()
	{
		memset(uid_list, 0, sizeof(uid_list));
		memset(user_name_list, 0, sizeof(user_name_list));

		marry_level = 0;
		is_already_start = 0;
		
		memset(camp_list, 0, sizeof(camp_list));
		reserve_ll = 0;
	}

	bool Invalid() { return MARRY_LEVEL_TYPE_INVALID == marry_level; }
	bool IsAlreadyStart() { return 0 != is_already_start; }

	int uid_list[MARRY_COUPLE_USER_LIMIT_NUM];					// 结婚双方ID
	GameName user_name_list[MARRY_COUPLE_USER_LIMIT_NUM];		// 结婚双方名字
	char marry_level;											// 结婚的档次
	char is_already_start;										// 是否已进行
	char camp_list[MARRY_COUPLE_USER_LIMIT_NUM];				// 结婚双方阵营
	long long reserve_ll;										// 保留3
};

struct MarryReserveParam
{
	MarryReserveParam() : total_marry_count(0) { this->Reset(); }

	void Reset()
	{
		for (int i = 0; i < MARRY_MAX_DAILY_MERRY_NUM; i++)
		{
			reserve_list[i].Reset();
		}
	}

	int total_marry_count;										// 全服结婚的总场数
	MarryReserve reserve_list[MARRY_MAX_DAILY_MERRY_NUM];		// 每天预定的婚礼
};

struct MarryHunyanInviteList
{
	MarryHunyanInviteList() : uid(0), count(0)
	{
		memset(be_invite_uid_list, 0, sizeof(be_invite_uid_list));
	}

	int uid;
	int count;
	int be_invite_uid_list[MARRY_HUNYAN_INVITE_USER_MAX_NUM];
};

struct MarryHunyanBlessNewsItem
{
	MarryHunyanBlessNewsItem() : news_id(0), uid(0), user_camp(0), bless_gift_index(0), be_bless_uid(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	int news_id;
	int uid;
	GameName user_name;
	short user_camp;
	short bless_gift_index; 
	int be_bless_uid;
};

#pragma pack(pop)

#endif // __MARRY_DEF_HPP__

