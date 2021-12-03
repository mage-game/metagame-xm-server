#ifndef __MITAMA_DEF_HPP__
#define __MITAMA_DEF_HPP__

#include "servercommon.h"

static const int MITAMA_MAX_MITAMA_COUNT = 15;			// ���������
static const int MITAMA_MAX_SPIRIT_COUNT = 5;			// һ������ӵ�е�������
static const int MITAMA_MAX_LEVEL = 6;					// �������ȼ�
static const int MITAMA_MAX_TASK_COUNT = 6;				// �������������
static const int MITAMA_MAX_EXCHANGE_ITEM_COUNT = 30;	// �ɶһ���Ʒ�����

#pragma pack(push, 4)

enum MITAMA_TASK_STATUS
{
	MITAMA_TASK_STATUS_IDLE = 0,	// ����
	MITAMA_TASK_STATUS_PROCEEDING,	// ��ս
	MITAMA_TASK_STATUS_AWARD,		// �콱

	MITAMA_TASK_STATUS_MAX,
};

struct MitamaInfo
{
	MitamaInfo() : level(0), task_status(MITAMA_TASK_STATUS_IDLE), task_seq(-1), task_begin_timestamp(0), reserve_int(0)
	{
		memset(spirit_level_list, 0, sizeof(spirit_level_list));
	}

	char level;											// ����ȼ�
	char spirit_level_list[MITAMA_MAX_SPIRIT_COUNT];	// ����Ļ�ĵȼ�
	UNSTD_STATIC_CHECK(5 == MITAMA_MAX_SPIRIT_COUNT);
	char task_status;		// ����״̬
	char task_seq;			// ִ�е�����

	unsigned int task_begin_timestamp;

	int reserve_int;
};

struct MitamaParam
{
	MitamaParam() { this->Reset(); }

	void Reset()
	{
		memset(info_list, 0, sizeof(info_list));
		hotspring_score = 0;
	}

	MitamaInfo info_list[MITAMA_MAX_MITAMA_COUNT];

	int hotspring_score;			// ��Ȫ����
};

typedef char MitamaParamHex[sizeof(MitamaParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MitamaParamHex) < 512);

#pragma pack(pop)

#endif
