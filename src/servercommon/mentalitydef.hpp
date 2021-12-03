#ifndef __MENTALITY_DEF_HPP__
#define __MENTALITY_DEF_HPP__

#include <string.h>
#include "servercommon/servercommon.h"

enum MENTALITY_TYPE
{
	MENTALITY_TYPE_MAXHP = 0,										// ��Ѫ
	MENTALITY_TYPE_GONGJI,											// ����
	MENTALITY_TYPE_FANGYU,											// ����
	MENTALITY_TYPE_MINGZHONG,										// ����
	MENTALITY_TYPE_SHANBI,											// ����
	MENTALITY_TYPE_FUJIA_SHANGHAI,									// �����˺�
	MENTALITY_TYPE_DIKANG_SHANGHAI,									// �ֿ��˺�

	MENTALITY_TYPE_MAX,												
};

static const int MAX_MENTALITY_BASE_LEVEL = 100;					// Ԫ�����ȼ� 
static const int MAX_MENTALITY_GENGU_LEVEL = 50;					// �������ȼ�

static const int MAX_MENTATITY_TITLE_COUNT_MAX = 10;				// Ԫ��ƺ��������
static const int MAX_GENGU_TITLE_COUNT_MAX = 6;						// ���ǳƺ��������

static const int MAX_MENTALITY_UPGRADE_ONCE = MENTALITY_TYPE_MAX;	// Ԫ��һ�����������������
static const int CLEAR_CD_INTERVAL_PER_GOLD = 600;					// ��λԪ������ Ԫ��������ȴʱ��(s)


#pragma pack(push, 4)

const static int MENTALITY_WUXING_MAX_COUNT = 35;
static const int MENTALITY_SHUXINGDAN_MAX_TYPE = 3;

struct MentalityItem
{
	MentalityItem() : mentality_level(0), gengu_level(0), gengu_max_level(0), reserve_1(0) {}

	short mentality_level;											// Ԫ��ȼ�
	short gengu_level;												// ���ǵȼ�
	short gengu_max_level;											// ��ʷ��� ���ǵȼ� 
	short reserve_1;											

	bool Invalid() { return 0 == mentality_level; }
	bool GenguInvalid() { return 0 == gengu_level; }
};

struct MentalityParam
{
	MentalityParam() { this->Reset(); }

	void Reset()
	{
		train_cd_next_time = 0;
		curr_trained_mentality_type = 0;
		flag = 0;
		reserved1 = 0;
		reserved2 = 0;

		memset(mentality_list, 0, sizeof(mentality_list));
		memset(reserve_sh_list, 0, sizeof(reserve_sh_list));
		memset(wuxing_level_list, 0, sizeof(wuxing_level_list));
		memset(shuxingdan_list, 0, sizeof(shuxingdan_list));
	}

	enum
	{
		FLAG_CLEAR_CD = 0X1,
	};

	unsigned int train_cd_next_time;								// Ԫ��ѵ�����ʱ���	
	short curr_trained_mentality_type;								// ��ǰѵ��Ԫ������
	short flag;												
	long long reserved1;
	long long reserved2;

	MentalityItem mentality_list[MENTALITY_TYPE_MAX];				// Ԫ����ǵȼ�

	UNSTD_STATIC_CHECK(35 == MENTALITY_WUXING_MAX_COUNT);

	short reserve_sh_list[5];
	short wuxing_level_list[MENTALITY_WUXING_MAX_COUNT];			// ���еȼ�

	int shuxingdan_list[MENTALITY_SHUXINGDAN_MAX_TYPE];				// ���Ե��б�
};

typedef char MentalityParamHex[sizeof(MentalityParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MentalityParamHex) < 512);

#pragma pack(pop)

#endif  // __MENTALITY_DEF_HPP__

