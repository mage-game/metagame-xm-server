#ifndef _TITLEDEF_HPP_
#define _TITLEDEF_HPP_

#include "servercommon/servercommon.h"

static const int MAX_TITLE_COUNT_TO_SAVE = 256;				// 保存的进阶称号最大数量
static const int MAX_OTHER_TITLE_COUNT = 256;				// 非成就称号最大数量

static const int MAX_OWN_TITLE_COUNT = 512;
static const int MAX_USE_TITLE_COUNT = 3;					// 使用title最大数

static const int MAX_TITLE_GRADE = 200;						// 最大title进阶数量

UNSTD_STATIC_CHECK(MAX_OWN_TITLE_COUNT >= (MAX_OTHER_TITLE_COUNT + MAX_TITLE_COUNT_TO_SAVE));

enum OTHER_TITLE_TYPE
{
	OTHER_TITLE_TYPE_GOLD_MEMBER = 1,						// 黄金会员
	OTHER_TITLE_TYPE_MARRY_ME,								// 我们结婚吧
	OTHER_TITLE_TYPE_MAN_CHARM,								// 男魅力称号
	OTHER_TITLE_TYPE_GIRL_CHARM,							// 女魅力称号
	OTHER_TITLE_TYPE_WORLD_QUESTION,						// 世界答题称号
	OTHER_TITLE_TYPE_CROSS_GUILDBATTLE_FIRST,				// 跨服帮派战第一名称号
	OTHER_TITLE_TYPE_CROSS_GUILDBATTLE_OTHER,				// 跨服帮派战其他称号
	OTHER_TITLE_TYPE_GUILD_SIGN,							// 公会签到
	OTHER_TITLE_TYPE_QINGYUAN_CAP_TOP,						// 情缘战力第一名
	OTHER_TITLE_TYPE_BABY_CAP_TOP,							// 宝宝战力第一名
	OTHER_TITLE_TYPE_LITTLE_PET_TOP,						// 小宠物战力第一

	OTHER_TITLE_TYPE_MAX_COUNT
};

#pragma pack(push, 4)

struct Title
{
	Title() { this->Reset(); }

	bool Invalid() { return 0 == title_id; }
	
	void Reset() 
	{
		title_id = 0;
		title_grade = 0;
		reserve_ch = 0;
		expired_time = 0;
	}

	unsigned short title_id;				// 称号ID
	char title_grade;						// 称号等级
	char reserve_ch;
	unsigned int expired_time;				// 到期时间戳
};

struct TitleParam
{
	TitleParam() { this->Reset(); }

	void Reset() 
	{
		memset(used_title_list, 0, sizeof(used_title_list));

		saved_count = 0;
		memset(saved_title_list, 0, sizeof(saved_title_list));
	}

	unsigned short used_title_list[MAX_USE_TITLE_COUNT];

	short saved_count;
	Title saved_title_list[MAX_TITLE_COUNT_TO_SAVE];	
};

typedef char TitleParamHex[sizeof(TitleParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(TitleParamHex) < 5120);

#pragma pack(pop)

#endif
