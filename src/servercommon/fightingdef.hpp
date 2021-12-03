#ifndef __FIGHTING_DEF_HPP__
#define __FIGHTING_DEF_HPP__

#include "servercommon/servercommon.h"
#include "gameworld/gameworld/gamedef.h"

static const int FIGHTING_MAX_ROB_HISTORY_COUNT = 20;					// ����洢�����������ʷ��¼
static const int FIGHTING_CHALLENGE_OPPONENT_COUNT = 4;					// ���� - ���ָ���
static const int FIGHTING_CHALLENGE_DEFAULT_TIMES = 6;					// ���� - ÿ��Ĭ�ϸ���ҵ���ս����
static const int FIGHTING_FAKE_MINING_SAILING_DATA_COUNT = 8;			// ������ - �ڿ󺽺���Ļ���������������˵������ҳ���Ǿ�8����
//static const int FIGHTING_FAKE_MINING_SAILING_BEEN_ROB_COUNT = 5;		// ������ - �ڿ󺽺���Ļ�������౻��ٴ����������ظ���ͬһ����Ҵ�٣�

#pragma pack(push, 4)

struct FightingRobHistoryItem
{
	FightingRobHistoryItem() { this->Reset(); }
	
	void Reset()
	{
		rober_uid = 0;
		rob_time = 0;
		has_revenge = 0;
		quality = 0;
		reserve_sh = 0;
	}

	int rober_uid;														// ������uid
	unsigned int rob_time;												// ����ʱ��
	char has_revenge;													// �Ƿ��ѱ���
	char quality;														// �����ٵĿ�Ʒ��
	short reserve_sh;
};

struct FightingChellangeOpponentInfo
{
	FightingChellangeOpponentInfo() { this->Reset(); }

	void Reset()
	{
		random_uid = 0;
		random_name_index = -1;
		has_win = 0;
		reserve_ch = 0;
		capability = 0;
		memset(reserve_list, 0, sizeof(reserve_list));
	}

	int random_uid;														// ������ı������ID
	short random_name_index;											// ���ɵĶ���������������ͻ��˸������ȥ��������б��Ӧ�����֣�
	char has_win;														// �Ƿ��Ѿ�սʤ���������
	char reserve_ch;
	int capability;
	char reserve_list[28];
};

// �������ڿ�/��������
struct FightingFakeMiningSailingData
{
	FightingFakeMiningSailingData() { this->Reset(); }

	void Reset()
	{
		random_index = -1;
		reserve_sh = 0;
		cur_quality = 0;
		cur_been_rob_times = 0;
		end_time = 0;
		capability = 0;
		memset(reserve_list, 0, sizeof(reserve_list));
	}

	int random_index;													// �����������ȷ��������֡�ְҵ���Ա����Ϣ��
	short reserve_sh;
	char cur_quality;													// ��ǰ��Ʒ��
	char cur_been_rob_times;											// ��ǰ�󱻣����ǣ����ٴ���
	unsigned int end_time;												// ��ǰ�ڿ������ʱ���
	int capability;
	char reserve_list[28];
};

struct FightingParam
{
	FightingParam() { this->Reset(); }

	void Reset()
	{
		mining_today_times = 0;
		mining_today_buy_times = 0;
		mining_today_rob_times = 0;
		mining_been_rob_last_index = 0;
		mining_been_rob_need_notify = 0;
		reserve_ch3 = 0;
		mining_cur_quality = 0;
		mining_cur_been_rob_times = 0;
		mining_end_time = 0;
		for (int i = 0; i < FIGHTING_MAX_ROB_HISTORY_COUNT; ++i)
		{
			mining_been_rob_list[i].Reset();
			sailing_been_rob_list[i].Reset();
		}

		sailing_today_times = 0;
		sailing_today_buy_times = 0;
		sailing_today_rob_times = 0;
		sailing_been_rob_last_index = 0;
		sailing_been_rob_need_notify = 0;
		reserve_ch4 = 0;
		sailing_cur_quality = 0;
		sailing_cur_been_rob_times = 0;
		sailing_end_time = 0;
		for (int i = 0; i < FIGHTING_CHALLENGE_OPPONENT_COUNT; ++i)
		{
			challenge_opponent_lsit[i].Reset();
		}

		for (int i = 0; i < FIGHTING_FAKE_MINING_SAILING_DATA_COUNT; ++i)
		{
			mining_fake_list[i].Reset();
			sailing_fake_list[i].Reset();
		}

		challenge_score = 0;
		challenge_next_auto_reflush_time = 0;
		challenge_left_times = FIGHTING_CHALLENGE_DEFAULT_TIMES;
		challenge_vip_buy_times = 0;
		reserve_ch1 = 0;
		reserve_ch2 = 0;
		for (int i = 0; i < FIGHTING_CHALLENGE_OPPONENT_COUNT; ++i)
		{
			challenge_opponent_lsit[i].Reset();
		}

		memset(reserve_list, 0, sizeof(0));
	}

	char mining_today_times;											// �������ڿ����
	char mining_today_buy_times;										// �����ѹ������
	char mining_today_rob_times;										// ���������ٿ����
	char mining_been_rob_last_index;									// �ڿ�������ʷ����������¼���±�
	char mining_been_rob_need_notify;									// �Ƿ��б����ټ�¼��Ҫ֪ͨ��ң��������һ�������б��ԭ��
	char reserve_ch3;
	char mining_cur_quality;											// ��ǰ��Ʒ��
	char mining_cur_been_rob_times;										// ��ǰ�����ٴ���
	unsigned int mining_end_time;										// ��ǰ�ڿ������ʱ���
	FightingRobHistoryItem mining_been_rob_list[FIGHTING_MAX_ROB_HISTORY_COUNT];	// �ڿ�������ʷ�б�ѭ���滻��¼��
	FightingFakeMiningSailingData mining_fake_list[FIGHTING_FAKE_MINING_SAILING_DATA_COUNT]; // ������ - �������ڿ�����

	char sailing_today_times;											// �����Ѻ�������
	char sailing_today_buy_times;										// �����ѹ������
	char sailing_today_rob_times;										// ���������ٴ���
	char sailing_been_rob_last_index;									// ������������ʷ����������¼���±�
	char sailing_been_rob_need_notify;									// �Ƿ��б����ټ�¼��Ҫ֪ͨ��ң��������һ�������б��ԭ��
	char reserve_ch4;
	char sailing_cur_quality;											// ��ǰ����Ʒ��
	char sailing_cur_been_rob_times;									// ��ǰ�α����ٴ���
	unsigned int sailing_end_time;										// ��ǰ����������ʱ���
	FightingRobHistoryItem sailing_been_rob_list[FIGHTING_MAX_ROB_HISTORY_COUNT];	// ������������ʷ�б�ѭ���滻��¼��
	FightingFakeMiningSailingData sailing_fake_list[FIGHTING_FAKE_MINING_SAILING_DATA_COUNT]; // ������ - �����˺�������

	int challenge_score;												// ���� - �ܻ���
	unsigned int challenge_next_auto_reflush_time;						// ���� - ��һ���Զ�ˢ��ʱ�䣨��ʱ�Զ�ˢ�������б�
	char challenge_left_times;											// ���� - ��ǰʣ����ս����
	char challenge_vip_buy_times;										// ���� - VIP�ѹ������ս����
	char reserve_ch1;
	char reserve_ch2;
	FightingChellangeOpponentInfo challenge_opponent_lsit[FIGHTING_CHALLENGE_OPPONENT_COUNT];// ���� - ƥ�䵽�Ķ����б�

	char reserve_list[152];
};

typedef char FightingParamHex[sizeof(FightingParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(FightingParamHex) < 4096);

#pragma pack(pop)

#endif

