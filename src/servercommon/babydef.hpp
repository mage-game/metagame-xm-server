#ifndef __BABYDEF_H__
#define __BABYDEF_H__

#include "servercommon.h"
#include "gameworld/gameworld/item/itembase.h"
#include "chatserverdef.h"

#pragma pack(push, 4)

static const int BABY_MAX_COUNT = 10;											// 最大可拥有的宝宝数量限制
static const int BABY_MAX_LEVEL_LIMIT = 100;										// 最大宝宝等级限制
static const int BABY_MAX_GRADE_LIMIT = 15;										// 最大宝宝阶数限制
static const int BABY_MAX_QIFU_TREE_CFG_COUNT = 3;								// 祈福树配置最大数量
static const int BABY_BORN_MEMBER_COUNT = 2;									// 生猴子队伍人数限制
static const int BABY_MAX_CHAOSHENG_COUNT = 5;									// 最大超生数
static const int BABY_SPIRIT_COUNT = 4;											// 守护精灵数目
static const int BABY_MAX_SPIRIT_LEVEL_LIMIT = 100;								// 守护精灵最大等级

static const int INVALID_BABY_ID = -1;											// 无效的宝宝ID
static const int TOP_TALENT_BABY_ID = 2;										// 顶级宝宝ID

struct BabySpiritInfo
{
	BabySpiritInfo() { this->Reset(); }

	void Reset()
	{
		spirit_level = 0;
		reserve_ch = 0;
		spirit_train = 0;
	}

	char spirit_level;			// 精灵等级
	char reserve_ch;
	short spirit_train;			// 精灵培养值
};

struct BabyInfo
{
	BabyInfo() { this->Reset(); }

	void Reset()
	{
		unique_id = 0;
		last_modity_time = 0;
		lover_uid = 0;
		baby_id = INVALID_BABY_ID;
		rename_times = 0;

		grade = 0;
		level = 0;
		bless = 0;
		reserve_sh = 0;

		memset(baby_name, 0, sizeof(baby_name));

		for (int spirit_id = 0; spirit_id < BABY_SPIRIT_COUNT; ++ spirit_id)
		{
			baby_spirit_list[spirit_id].Reset();
		}
	}

	long long unique_id;								// 全平台唯一ID
	unsigned int last_modity_time;

	UID  lover_uid;
	short baby_id;
	short rename_times;

	short grade;
	short level;
	short bless;
	short reserve_sh;

	GameName baby_name;

	BabySpiritInfo baby_spirit_list[BABY_SPIRIT_COUNT];
};


struct BabyParam
{
	BabyParam() { this->Reset(); }

	void Reset()
	{
		for (int i = 0; i < BABY_MAX_COUNT; ++ i)
		{
			baby_list[i].Reset();
		}

		chaosheng_count = 0;
	}

	BabyInfo baby_list[BABY_MAX_COUNT];

	int chaosheng_count;								// 超生名额
};

typedef char BabyParamHex[sizeof(BabyParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(BabyParamHex) < 2048);

#pragma pack(pop)

#endif

