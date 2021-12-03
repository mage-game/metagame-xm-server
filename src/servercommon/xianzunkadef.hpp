#pragma once
#ifndef __XIANZUNKA_DEF_HPP__
#define __XIANZUNKA_DEF_HPP__
#include "servercommon/servercommon.h"

#pragma pack(push, 4)

enum XIANZUNKA_TYPE
{
	XIANZUNKA_TYPE_SILVER,			//��������
	XIANZUNKA_TYPE_GOLD,			//�ƽ�����
	XIANZUNKA_TYPE_DIAMOND,			//��ʯ����

	XIANZUNKA_TYPE_MAX
};

struct XianZunKaParam
{
	XianZunKaParam() { this->Reset(); }

	void Reset()
	{
		forever_active_flag = 0;
		first_active_reward_flag = 0;
		daily_reward_fetch_flag = 0;

		sent_emails_flag = 0;
		is_first_recharge = 0;
		reserve_sh = 0;

		memset(temporary_valid_end_timestamp_list, 0, sizeof(temporary_valid_end_timestamp_list));
		capability = 0;
	}

	short forever_active_flag;						// ���ü�����
	char first_active_reward_flag;					// �״μ�������
	char daily_reward_fetch_flag;					// ÿ�ս�����ȡ���

	char sent_emails_flag;        // �ʼ����ѱ��
	char is_first_recharge;							// �Ƿ��״γ�ֵ
	short reserve_sh;

	UNSTD_STATIC_CHECK(3 == XIANZUNKA_TYPE_MAX);
	unsigned int temporary_valid_end_timestamp_list[XIANZUNKA_TYPE_MAX];		// ��ʱ��Ч ����ʱ���
	int capability;						// ս��
};

#pragma pack(pop)

typedef char XianZunKaParamHex[sizeof(XianZunKaParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(XianZunKaParamHex) < 128);

#endif
