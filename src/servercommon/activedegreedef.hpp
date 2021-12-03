#ifndef __ACTIVE_DEGREE_DEF_HPP__
#define __ACTIVE_DEGREE_DEF_HPP__

#include "servercommon/servercommon.h"

enum ACTIVEDEGREE_TYPE
{
	ACTIVEDEGREE_TYPE_ONLIME_TIME,								// ����Сʱ��
	ACTIVEDEGREE_TYPE_PHASE_FB,									// ���׸���
	ACTIVEDEGREE_TYPE_EXP_FB,									// ���鸱��
	ACTIVEDEGREE_TYPE_DAILY_TASK,								// ����ճ��������
	ACTIVEDEGREE_TYPE_GUILD_TASK,								// ��������
	ACTIVEDEGREE_TYPE_GUILD_BOX,								// ���ᱦ��
	ACTIVEDEGREE_TYPE_HUSONG,									// ����
	ACTIVEDEGREE_TYPE_FULI_BOSS,								// ����BOSS
	ACTIVEDEGREE_TYPE_PATA_FB,									// ��������
	ACTIVEDEGREE_TYPE_TEAM_FB,									// ��Ӹ���
	ACTIVEDEGREE_TYPE_GCZ,										// ����ս
	ACTIVEDEGREE_TYPE_YSZC,										// Ԫ��ս��
	ACTIVEDEGREE_TYPE_GUILDBATTLE,								// ��������
	ACTIVEDEGREE_TYPE_LINGTUZHAN,								// ����ս
	ACTIVEDEGREE_TYPE_WLTX,										// ����̽��
	ACTIVEDEGREE_TYPE_TJCB,										// �콵�Ʊ�
	ACTIVEDEGREE_TYPE_DAFUHAO,									// �󸻺�
	ACTIVEDEGREE_TYPE_WORLD_BOSS,								// ����BOSS
	ACTIVEDEGREE_TYPE_RUNE_TOWER,								// ������
	ACTIVEDEGREE_TYPE_WABAO,									// �ڱ�
	ACTIVEDEGREE_TYPE_ACTIVE_BOSS,								// ɱ��Ծboss��
	ACTIVEDEGREE_TYPE_SONG_HUA,									// �ͻ���
	ACTIVEDEGREE_TYPE_VIP_BOSS,									// ɱvipboss��
	ACTIVEDEGREE_TYPE_XINGZUOYIJI_GATHER,						// �����ż��ɼ���
	ACTIVEDEGREE_TYPE_ANCIENT_YIJI_GATHER,						// Զ���ż��ɼ���
	ACTIVEDEGREE_TYPE_MIKU_BOSS,							    // ��Ӣ���ؿߣ�boss������
	ACTIVEDEGREE_TYPE_QINGYUAN_SHENGDI,							// ��Եʥ��������
	ACTIVEDEGREE_TYPE_CHALLENGEFIELD_TIMES,						// ����1v1����������
	ACTIVEDEGREE_TYPE_EQUIP_FB,									// װ������ ��������
	ACTIVEDEGREE_TYPE_JOIN_FISHING,								// �������
	ACTIVEDEGREE_TYPE_PAOHUAN_TASK,								// �ܻ�����

	ACTIVEDEGREE_TYPE_BUILD_TOWER_FB,							// ��������
	ACTIVEDEGREE_TYPE_TOWERDEFEND_FB,							// ���߲���
	ACTIVEDEGREE_TYPE_CHALLENGE_FB,								// ��ս����
	ACTIVEDEGREE_TYPE_ARMORDEFEND_FB,							// �ػ�����
	ACTIVEDEGREE_TYPE_TOWERDEFEND_TEAM,							// ����ػ�
	ACTIVEDEGREE_TYPE_EQUIP_TEAM_FB,							// �������
	ACTIVEDEGREE_TYPE_JINGLING_GATHER,							// ��������

	ACTIVEDEGREE_TYPE_MIZANG_BOSS,								// �ܲ�BOSS
	ACTIVEDEGREE_TYPE_CROSS_BOSS,								// ���BOSS(Զ��)
	ACTIVEDEGREE_TYPE_NUM,

	ACTIVEDEGREE_TYPE_MAX = 64,
};

UNSTD_STATIC_CHECK(ACTIVEDEGREE_TYPE_NUM <= ACTIVEDEGREE_TYPE_MAX);

static const int ACTIVEDEGREE_REWARD_ITEM_MAX_NUM = 8;			// ��Ծ������Ʒ������
static const int ACTIVEDEGREE_REWARD_LEVEL_MAX_NUM = 10;		// ��Ծ���齱���ȼ���Χ��󻮷ָ���
static const int ACTIVEDEGREE_REWARD_MAX_DAY = 31;				// ��Ծ���콱���������

UNSTD_STATIC_CHECK(0 == ACTIVEDEGREE_REWARD_ITEM_MAX_NUM % 4);

enum ACTIVEDEGREE_REWARD_STATUS
{
	ACT_DEGREE_REWARD_STATUS_CAN_NOT_FETCH = 0,			// ������ȡ
	ACT_DEGREE_REWARD_STATUS_CAN_FETCH,					// ����ȡ
	ACT_DEGREE_RWARD_STATUS_BE_FETCHED,					// ����ȡ
};

#pragma pack(push, 4)

struct ActiveDegreeParam
{
	ActiveDegreeParam() { this->Reset(); }

	void Reset()
	{
		day_online_time = 0;
		total_degree = 0;
		memset(activedegree_list, 0, sizeof(activedegree_list));
		memset(reward_flag_list, 0, sizeof(reward_flag_list));
		activedegree_fetch_flag = 0;
		memset(reward_on_day_flag_list, 0, sizeof(reward_on_day_flag_list));
	}

	int day_online_time;
	int total_degree;
	char activedegree_list[ACTIVEDEGREE_TYPE_MAX];
	char reward_flag_list[ACTIVEDEGREE_REWARD_ITEM_MAX_NUM];

	unsigned long long activedegree_fetch_flag;					// ��Ծ����ȡ��־
	char reward_on_day_flag_list[ACTIVEDEGREE_REWARD_ITEM_MAX_NUM]; // ����ÿ���µڼ����ȡ�Ľ�����ʶ
};

typedef char ActiveDegreeHex[sizeof(ActiveDegreeParam) * 2 + 1];

UNSTD_STATIC_CHECK(sizeof(ActiveDegreeHex) < 256);

#pragma pack(pop)

#endif // __ACTIVE_DEGREE_DEF_HPP__

