#ifndef __VIP_DEF_HPP__
#define __VIP_DEF_HPP__

#include <string.h>
#include "servercommon/servercommon.h"

enum VIP_TYPE
{
	VIP_NONE = 0,															// ��VIP

	VIP_EXPERIENCE,															// ���鿨
	VIP_WEEK,																// �ܿ�
	VIP_MON,																// �¿�
	VIP_HALF_YEAR,															// ���꿨

	VIP_COUNT,
};

enum VIP_EXP_ADDON_TYPE
{
	VIP_EXP_ADDON_TYPE_INVALID = 0,


	VIP_EXP_ADDON_TYPE_LOGIN,												// ÿ���¼	 ��VIP���� 
	VIP_EXP_ADDON_TYPE_ONLINE,												// ÿСʱ���� ��VIP����
	VIP_EXP_ADDON_TYPE_CONSUME,												// ����1Ԫ��  ��VIP����
	VIP_EXP_ADDON_TYPE_FIRST_BE_VIP,										// ��һ�γ�ΪVIP ��VIP����

	VIP_EXP_ADDON_TYPE_MAX,
};

enum VIP_WELFARE_TYPE
{
	VIP_WELFARE_NONE = 0,

	VIP_WELFARE_DAY,														// ��ȡ�ո���
	VIP_WELFARE_WEEK,														// ��ȡ�ܸ���
	VIP_WELFARE_BUFF,														// ��ȡvipף��

	VIP_WELFARE_COUNT,
};

enum VIP_AUTH_TYPE
{
	VAT_TRANSFER = 0,											// ����
	VAT_FINISH_ALL_DAILY_TASK = 1,								// һ������ճ�
	VAT_BUY_HUSONG_TIMES = 2,									// �����ʹ���
	VAT_RA_LOGIN_GIFT_VIP_REWARD = 3,							// ������¼����vip���
	VAT_STRENGTH_SUC_ADD_RATE = 4,								// ǿ���ɹ���+X%
	VAT_VIP_EXTRA_EXP = 5,										// VIP���⾭��ӳ�
	//VAT_ENTER_BOSS_HOME = 6,									// �ܷ����BOSS֮��
	VAT_BUY_SYT_TIMES = 7,										// �ɹ�������������
	VAT_BUY_YSGC_TIMES = 8,										// �ɹ������޹㳡����
	VAT_BUY_FREE_REVIVAL_TIMES = 9,								// ��Ѹ������
	VAT_FOLLOW_STORE = 10,										// ����ֿ�
	VAT_FOLLOW_SHOP = 11,										// ����ҩ��
	VAT_OFFLINE_EXP_RATE_2 = 12,								// ���߾���2����ȡ
	VAT_FB_EXP_BUY_TIMES = 13,									// ���鱾�������
	VAT_FB_COIN_BUY_TIMES = 14,									// ͭ�ұ�������� 
	VAT_FB_1V1_BUY_TIMES = 15,									// 1v1�������
	VAT_WABAO_BUY_JOIN_TIMES = 16,								// �ڱ��������
	VAT_FB_DAOJU_BUY_TIMES = 17,								// ���߱��������
	VAT_FB_MENTALITY_CLEAN_CD = 18,								// ������CD
	VAT_OFFLINE_EXP_RATE_3 = 19,								// ���߾���3����ȡ
	VAT_GUILD_BOX_COUNT = 20,									// ���˱�������
	VAT_FB_STROY_COUNT = 21,									// ���鸱���������
	VAT_FB_PAHSE_COUNT = 22,									// �׶θ����������
	VAT_FB_HOTSPRINT_EXTRA_REWARD = 23,							// ��Ȫ��������ӳ�(�ٷֱȼӳ�)
	VAT_FB_TEAM_EQUIP_COUNT= 24,								// ���װ����������������
	VAT_FB_MIKU_BOSS_BUY_WEARY = 25,							// �ؿ�boss����ƣ��ֵ
	VAT_TOWERDEFEND_FB_FREE_AUTO_TIMES = 26,					// �������ɨ������
	VAT_TOWERDEFEND_FB_BUY_TIMES = 27,							// ����������������ߣ�

	VAT_XIANNV_SHENGWU_AUTO_FETCH = 28,							// Ů����ʥ��Զ�ѡ����Ƭ
	VAT_XIANNV_SHENGWU_MULTI_CHOU = 29,							// Ů����ʥ�ʮ�γ�
	VAT_FIGHTING_DAY_BUY_CHALLENGE_TIMES = 30,					// ������ - ���ƹ������
	VAT_TUITU_FB_NORMAL_BUY_TIMES = 31,							// ��ͼ��ͨ�����������
	VAT_TUITU_FB_HARD_BUY_TIMES = 32,							// ��ͼ��Ӣ�����������
	VAT_FIGHTING_MINING_BUY_TIMES = 33,							// ������ - �ڿ������
	VAT_FIGHTING_SAILING_BUY_TIMES = 34,						// ������ - �����������
	VAT_NEQ_FB_BUY_TIMES = 35,									// ������ͼ��(��װ����)�������
	VAT_BUILD_TOWER_FB_BUY_TIMES = 36,							// �������������
	VAT_BABY_BOSS_BUY_ENTER_TIMES = 37,							// ����boss�������
	VAT_ARMOR_DEFEND_FB_BUY_TIMES = 38,							// ���߲��ϱ��������(����)
	VAT_SHANGGU_BOSS_BUY_ENTER_TIMES = 39,						// �Ϲ�boss�������
	VAT_SHANGGU_BOSS_EXTRA_TIRE_VALUE = 40,                     // �Ϲ�boss����ƣ��ֵ
	VAT_PERSON_BOSS_ENTER_TIMES = 41,                           // ����boss�������
	VAT_DABAO_BOSS_ENTER_TIMES = 43,                            // ��װboss�������
	VAT_VIP_OPEN_KNAPSACK_GRID = 44,							// VIP������������
	VAT_PERSON_BOSS_BUY_TIMES = 45,                             // ����boss�������
	VAT_CHALLENGE_FB_BUY_TIMES = 46,							// Ʒ�ʱ��������
	VAT_GUILD_TASK_ONE_KEY = 47,								// һ����������Ȩ��
	VAT_PAOHUAN_TASK_ONE_KEY = 48,								// һ���ܻ�����Ȩ��
	VAT_MAX = 49,
};

enum QIFU_TYPE												// ������
{
	QIFU_TYPE_INVALID = 0,

	QIFU_TYPE_BUYCOIN,										// ��ģ���е�ҡǮ
	QIFU_TYPE_BUYYUANLI,									// ��ģ���е�ҡԪ��
	QIFU_TYPE_BUYXIANHUN,									// ��ģ���е�ҡ����

	QIFU_TYPE_MAX,
};

static const int MAX_VIP_LEVEL = 20;						// VIP��ߵȼ�Ϊ10��
static const int VIP_TIMEOUT_REMIDE_DAYS = 3;				// VIP��������Ϊ3��

#pragma pack(push, 4)

struct VipParam
{
	VipParam() { this->Reset(); }

	void Reset()
	{
		vip_level = 0;
		fetch_qifu_buycoin_reward_flag = 0;
		gold_buycoin_times = 0;
		gold_buyyuanli_times = 0;
		gold_buyxianhun_times = 0;
		vip_exp = 0;
		fetch_level_reward_flag = 0;
		fetch_qifu_buyyuanli_reward_flag = 0;
		fetch_qifu_buyxianhun_reward_flag = 0;
		free_buycoin_times = 0;
		free_buyyuanli_times = 0;
		free_buyxianhun_times = 0;

		qifu_last_free_buycoin_timestamp = 0;
		qifu_last_free_buyyuanli_timestamp = 0;
		qifu_last_free_buyxianhun_timestamp = 0;

		vip_week_gift_times = 0;
		time_limit_vip_time = 0;

		reward_vip_exp_flag = 0;
		reserve_sh = 0;
	}

	char vip_level;
	char fetch_qifu_buycoin_reward_flag;
	short gold_buycoin_times;

	short gold_buyyuanli_times;
	short gold_buyxianhun_times;
	int vip_exp;
	int fetch_level_reward_flag;

	char fetch_qifu_buyyuanli_reward_flag;
	char fetch_qifu_buyxianhun_reward_flag;
	short free_buycoin_times;
	short free_buyyuanli_times;
	short free_buyxianhun_times;

	unsigned int qifu_last_free_buycoin_timestamp;
	unsigned int qifu_last_free_buyyuanli_timestamp;
	unsigned int qifu_last_free_buyxianhun_timestamp;

	int vip_week_gift_times;
	
	unsigned int time_limit_vip_time;

	short reward_vip_exp_flag;
	short reserve_sh;

};

typedef char VipParamHex[sizeof(VipParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(VipParamHex) < 128);

#pragma pack(pop)

#endif  // __MENTALITY_DEF_HPP__

