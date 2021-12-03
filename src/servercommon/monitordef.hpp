#ifndef __MONITORDEF_HPP__
#define __MONITORDEF_HPP__

#include "servercommon/servercommon.h"

enum MONITOR_TYPE
{
	MONITOR_TYPE_DAY_GOLD_GET,								// ÿ�ո����Ԫ������ (�ǳ�ֵ�ֶ�)
	MONITOR_TYPE_DAY_GOLD_CONSUME,							// ÿ�ո����Ԫ������
	MONITOR_TYPE_DAY_NOBIND_ITEM_NUM,						// ÿ�ո���ҷǰ���Ʒ������Ŀ

	MONITOR_TYPE_P2PTRADE_NUM,								// ÿ�ո���ҽ��״���
	MONITOR_TYPE_PUBLICSALE_NUM,							// ÿ�ո������������
	MONITOR_TYPE_SENDMAIL_NUM,								// ÿ�ո���ҷ��ż�����
	MONITOR_TYPE_FETCH_ATTACHMENT_NUM,						// ÿ�ո�����ո���
	MONITOR_TYPE_GUILD_STORE_OPER_NUM,						// ÿ�ո���Ҳ������Ųֿ����
	MONITOR_TYPE_SHOP_BUY_NUM,								// ÿ�ո�����̳ǹ������
	MONITOR_TYPE_CHEST_SHOP_BUY_NUM,						// ÿ�ո����������

	MONITOR_TYPE_DAY_NOBIND_COIN_GET,						// ÿ�ո���ҷǰ�ͭǮ����
	MONITOR_TYPE_DAY_NOBIND_COIN_CONSUME,					// ÿ�ո���ҷǰ�ͭǮ����

	MONITOR_TYPE_DAY_MYSTERYSHOP_FLUSH_NUM,					// ÿ�ո���������̵�ˢ�´���
	MONITOR_TYPE_DAY_COMPOSE_NUM,							// ÿ�ո���Һϳ���Ʒ����

	MONITOR_TYPE_MAX = 20,
};

static const int MONITOR_LOG_INTERVAL = 1800;				// �����־���

#pragma pack(push, 4)

struct MonitorParam
{
	MonitorParam() { this->Reset(); }

	void Reset()
	{
		next_monitor_time = 0;
		memset(monitor_num_list, 0, sizeof(monitor_num_list));
	}

	unsigned int next_monitor_time;
	int monitor_num_list[MONITOR_TYPE_MAX];
};

typedef char MonitorParamHex[sizeof(MonitorParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MonitorParamHex) < 256);

#pragma pack(pop)

#endif	// __MONITORDEF_HPP__
