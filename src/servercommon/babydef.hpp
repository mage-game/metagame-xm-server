#ifndef __BABYDEF_H__
#define __BABYDEF_H__

#include "servercommon.h"
#include "gameworld/gameworld/item/itembase.h"
#include "chatserverdef.h"

#pragma pack(push, 4)

static const int BABY_MAX_COUNT = 10;											// ����ӵ�еı�����������
static const int BABY_MAX_LEVEL_LIMIT = 100;										// ��󱦱��ȼ�����
static const int BABY_MAX_GRADE_LIMIT = 15;										// ��󱦱���������
static const int BABY_MAX_QIFU_TREE_CFG_COUNT = 3;								// ���������������
static const int BABY_BORN_MEMBER_COUNT = 2;									// �����Ӷ�����������
static const int BABY_MAX_CHAOSHENG_COUNT = 5;									// �������
static const int BABY_SPIRIT_COUNT = 4;											// �ػ�������Ŀ
static const int BABY_MAX_SPIRIT_LEVEL_LIMIT = 100;								// �ػ��������ȼ�

static const int INVALID_BABY_ID = -1;											// ��Ч�ı���ID
static const int TOP_TALENT_BABY_ID = 2;										// ��������ID

struct BabySpiritInfo
{
	BabySpiritInfo() { this->Reset(); }

	void Reset()
	{
		spirit_level = 0;
		reserve_ch = 0;
		spirit_train = 0;
	}

	char spirit_level;			// ����ȼ�
	char reserve_ch;
	short spirit_train;			// ��������ֵ
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

	long long unique_id;								// ȫƽ̨ΨһID
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

	int chaosheng_count;								// ��������
};

typedef char BabyParamHex[sizeof(BabyParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(BabyParamHex) < 2048);

#pragma pack(pop)

#endif

