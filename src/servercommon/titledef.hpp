#ifndef _TITLEDEF_HPP_
#define _TITLEDEF_HPP_

#include "servercommon/servercommon.h"

static const int MAX_TITLE_COUNT_TO_SAVE = 256;				// ����Ľ��׳ƺ��������
static const int MAX_OTHER_TITLE_COUNT = 256;				// �ǳɾͳƺ��������

static const int MAX_OWN_TITLE_COUNT = 512;
static const int MAX_USE_TITLE_COUNT = 3;					// ʹ��title�����

static const int MAX_TITLE_GRADE = 200;						// ���title��������

UNSTD_STATIC_CHECK(MAX_OWN_TITLE_COUNT >= (MAX_OTHER_TITLE_COUNT + MAX_TITLE_COUNT_TO_SAVE));

enum OTHER_TITLE_TYPE
{
	OTHER_TITLE_TYPE_GOLD_MEMBER = 1,						// �ƽ��Ա
	OTHER_TITLE_TYPE_MARRY_ME,								// ���ǽ���
	OTHER_TITLE_TYPE_MAN_CHARM,								// �������ƺ�
	OTHER_TITLE_TYPE_GIRL_CHARM,							// Ů�����ƺ�
	OTHER_TITLE_TYPE_WORLD_QUESTION,						// �������ƺ�
	OTHER_TITLE_TYPE_CROSS_GUILDBATTLE_FIRST,				// �������ս��һ���ƺ�
	OTHER_TITLE_TYPE_CROSS_GUILDBATTLE_OTHER,				// �������ս�����ƺ�
	OTHER_TITLE_TYPE_GUILD_SIGN,							// ����ǩ��
	OTHER_TITLE_TYPE_QINGYUAN_CAP_TOP,						// ��Ես����һ��
	OTHER_TITLE_TYPE_BABY_CAP_TOP,							// ����ս����һ��
	OTHER_TITLE_TYPE_LITTLE_PET_TOP,						// С����ս����һ

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

	unsigned short title_id;				// �ƺ�ID
	char title_grade;						// �ƺŵȼ�
	char reserve_ch;
	unsigned int expired_time;				// ����ʱ���
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
