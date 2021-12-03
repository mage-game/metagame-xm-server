#ifndef __WELFARE_DEF_HPP__
#define __WELFARE_DEF_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"

static const int DURLOGIN_LOTTORY_MAX = 3;						// ������½���齱��������
static const int DURLOGIN_LOTTORY_ITEM_MAX = 9;					// ������½�齱�����Ʒ��Ŀ

static const int MAX_GROWTH_VALUE_GET_TYPE = 3;					// ���ֹ�������ֵ��÷�ʽ
	
static const int DAYS_PER_WEEK = 7;								// һ������
static const int WEEKLY_ACCOUNT_LOGIN_MAX = 3;					// ���ۼ��������� 

static const int COUNT_LOGIN_MAX = 60;							// �ۼƵ�½ ������

static const int DAILY_ONLINE_FETCH_GIFT_MAX = 8;				// ������������ȡ����
static const int DAILY_ONLINE_LEVEL_REWARD_INTERVAL_MAX = 20;	// ���߽����ȼ����ֶ���
static const int DAILY_ONLINE_REWRARD_ITEM_MAX = 3;				// ����������������Ʒ����
static const int DAILY_ONLINE_RAND_REWRARD_ITEM_MAX = 8;		// �����������������������Ʒ����

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const int OFFLINE_EXP_GET_MAX_TIME = 12 * 3600;			// ���߾�������һ�ʱ��

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const int SEVEN_DAY_LOGIN_MAX_REWARD_DAY = 7;			// �����¼���� ���������

// enum OFFLINE_EXP_FIND_TYPE
// {
// 	OFFLINE_EXP_FIND_FREE = 0,									// ���߾����һ� ���
// 	OFFLINE_EXP_FIND_VIP,										// ���߾����һ� vip
// 	OFFLINE_EXP_FIND_VIP4,										// ���߾����һ� vip4
// 	OFFLINE_EXP_FIND_MAX
// };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const int WELFARE_DAILY_FIND_MAXDAYS = 1;				// �ճ��һ� �������
static const int WELFARE_SECONDS_ONE_DAY = 24 * 3600;			// һ������

enum DAILYFIND_GET_TYPE
{
	DAILYFIND_GET_FREE = 0,										// �����ȡ
	DAILYFIND_GET_GOLD,											// Ԫ����ȡ
	DAILYFIND_GET_MAX,
};

enum DAILYFIND_TYPE
{
	DAILYFIND_TYPE_FB_EXP = 0,									// ���鱾
	DAILYFIND_TYPE_PATA = 1,									// ����
	DAILYFIND_TYPE_STORY_FB = 2,								// ���鸱��
	DAILYFIND_TYPE_UPGRADE_MOUNT_FB = 3 ,						// ������ױ�
	DAILYFIND_TYPE_UPGRADE_SHIZHUANG_BODY_FB = 4,				// ʱװ���ױ�
	DAILYFIND_TYPE_UPGRADE_SHIZHUANG_WUQI_FB = 5,				// ������ױ�
	DAILYFIND_TYPE_UPGRADE_LINGTONG_FB = 6,						// ��ͯ���ױ�
	DAILYFIND_TYPE_UPGRADE_FIGHT_MOUNT_FB =7,					// ս����ױ�
	DAILYFIND_TYPE_UPGRADE_LINGGONG_FB =8,						// �鹭���ױ�
	DAILYFIND_TYPE_UPGRADE_LINGQI_FB =9,						// ������ױ�
	DAILYFIND_TYPE_TUMO =10,									// ��ħ(�ճ�����)
	DAILYFIND_TYPE_GUILD_TASK =11,								// ��������
	DAILYFIND_TYPE_QUESTION =12,								// ����-�ɰ�
	DAILYFIND_TYPE_HUSONG =13,									// ����
	DAILYFIND_TYPE_PAOHUAN = 14,								// �ܻ�����
	DAILYFIND_TYPE_EQUIP_FB = 15,								// ����
	DAILYFIND_TYPE_TOWER_DEFEND_FB = 16,						// ����
	DAILYFIND_TYPE_CHALLENGE_FB = 17,							// Ʒ��
	DAILYFIND_TYPE_ARMOR_FB = 18,								// �ػ�
	DAILYFIND_TYPE_TEAM_EQUIP_FB = 19,							// �������
	DAILYFIND_TYPE_TEAM_TOWER_DEFEND_FB = 20,					// ����ػ�

	DAILYFIND_TYPE_MAX,
};

enum PLAT_WELFARE_TYPE
{
	PLAT_WELFARE_YY,											// YYƽ̨
	PLAT_WELFARE_360,											// 360ƽ̨
	PLAT_WELFARE_XUNLEI,										// Ѹ��ƽ̨
	PLAT_WELFARE_MAX,
};

enum PLAT_VIP_REWARD_TYPE
{
	PLAT_VIP_REWARD_LOGIN = 0,									// ��¼���
	PLAT_VIP_REWARD_LEVEL = 10,									// VIP�ȼ����
	PLAT_VIP_REWARD_360_BOOST_BALL = 15,						// 360���������
	PLAT_VIP_REWARD_MAX = 31,
};

const static int MAX_HAPPY_TREE_LEVEL = 20;						// ���ֹ�����ߵȼ�

enum GET_HAPPYTREE_VALUE_TYPE									// ���ֹ�������ֵ�������
{
	GET_HAPPYTREE_VALUE_TYPE_ONLINE = 0,
	GET_HAPPYTREE_VALUE_TYPE_ZHIBAO_EXP = 1,
	GET_HAPPYTREE_VALUE_TYPE_CHONGZHI = 2,

	GET_HAPPYTREE_VALUE_TYPE_MAX
};

UNSTD_STATIC_CHECK(PLAT_VIP_REWARD_MAX < 32);

// ����ҩˮ����
enum EXP_BUFF_TYPE
{
	EXP_BUFF_TYPE_INVALID = -1,
	EXP_BUFF_TYPE_1,
	EXP_BUFF_TYPE_2,
	EXP_BUFF_TYPE_3,
	EXP_BUFF_TYPE_4,

	EXP_BUFF_TYPE_MAX,
};

UNSTD_STATIC_CHECK(EXP_BUFF_TYPE_MAX <= 4);



struct ExpBuffInfo
{
	struct ExpBuffItem
	{
		void Reset()
		{
			exp_buff_left_time_s = 0;
			exp_buff_rate = 0;
			exp_buff_level = 0;
		}

		int exp_buff_left_time_s = 0;												// ����ҩˮBUFFʣ��ʱ��
		short exp_buff_rate = 0;													// ����ҩˮbuff����
		char exp_buff_level = 0;													// ����ҩˮbuff�ȼ�
		char reserve_ch = 0;
	};

	void Reset()
	{
		memset(exp_buff_list, 0, sizeof(exp_buff_list));
	}

	int GetCurMaxExpBuffType()
	{
		int buff_type = EXP_BUFF_TYPE_INVALID;
		for (int i = 0; i < EXP_BUFF_TYPE_MAX; i ++)
		{
			if (exp_buff_list[i].exp_buff_left_time_s > 0)
			{
				buff_type = i;
			}
		}

		return buff_type;
	}

	ExpBuffItem exp_buff_list[EXP_BUFF_TYPE_MAX];
};

struct DailyFindItem
{
	DailyFindItem() { this->Reset(); }

	bool Invalid() const { return 0 == find_time; }

	void Reset()
	{
		find_time = 0;
		role_level = 0;

		find_count = 0;
		param = 0;
		param1 = 0;
	}

	unsigned int find_time;
	short role_level;

	int find_count;
	int param;
	long long param1;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push, 4)

struct DailyFindRewardItem
{
	const static int ITEM_MAX_COUNT = 8;

	DailyFindRewardItem() { this->Reset(); }

	void Reset()
	{
		find_type = -1;
		role_level = 0;
		find_times = 0;
		reserve_sh = 0;

		exp = 0;
		bind_coin = 0;
		honor = 0;
		banggong = 0;
		gold_need = 0;
		coin_need = 0;

		item_count = 0;

		memset(item_list, 0, sizeof(item_list));
	}

	short find_type;
	short role_level;
	short find_times;
	short reserve_sh;

	long long exp;
	int bind_coin;
	int honor;
	int banggong; //  ���˹���
	int gold_need;
	int coin_need;

	int item_count;
	ItemConfigData item_list[ITEM_MAX_COUNT];
};

struct WelfareParam
{
	WelfareParam() { this->Reset(); }

	void Reset()
	{
		offline_second = 0;
		old_offline_exp = 0;

		chongzhi_count = 0;
		account_total_login_daycount = 1;
		sign_in_days = 0;
		login_level = 0;
		is_chongzhi_today = 0;
		continuity_signin_days = 0;
		sign_in_reward_mark = 0;
		activity_find_flag = 0;
		activity_join_flag = 0;
		auto_activity_flag = 0;
		last_signin_time = 0;
		online_reward_mark = 0;
		happy_tree_level = 0;
		sign_in_today_times = 0;
		is_not_first_send = 0;
		happy_tree_reward = 0;
		chongjihaoli_reward_mark = 0;
		memset(happy_tree_grouth_val, 0, sizeof(happy_tree_grouth_val));
		total_happy_tree_growth_val = 0;
		seven_day_login_fetch_reward_mark = 0;
		offline_mojing = 0;
		offline_exp = 0;
		
		collect_left_time_s = 0;

		happy_tree_online_times = 0;
		today_chongzhi_gold_num = 0;

		exp_buff_info.Reset();
	}

	int offline_second;														// ����ʱ��
	int old_offline_exp;														// ���߾���(������)
	int chongzhi_count;														// ��ֵ����
	int account_total_login_daycount;										// һ���ۼƵ�¼����
	int sign_in_days;														// ǩ������
	short login_level;														// ��¼�ȼ�
	char is_chongzhi_today;													// �����Ƿ��ֵ
	char continuity_signin_days;											// ����ǩ������
	int sign_in_reward_mark;												// ǩ���������
	int activity_find_flag;													// ��һر��
	int activity_join_flag;													// ������� ÿ������
	int auto_activity_flag;													// �ί�б�� ÿ������
	int last_signin_time;													// ���ǩ��ʱ��
	int online_reward_mark;													// �������߽�����ȡ���
	short happy_tree_level;													// ���ֹ����ȼ�
	char sign_in_today_times;												// ����ǩ������
	char is_not_first_send;													// �Ƿ��һ����ȡ���߾���
	short happy_tree_reward;												// ���߹���������ȡ���
	short chongjihaoli_reward_mark;											// �弶������ȡ���
	short happy_tree_grouth_val[MAX_GROWTH_VALUE_GET_TYPE + 1];				// ���ֹ�������ֵ
	int total_happy_tree_growth_val;										// �ܻ��ֹ�������ֵ
	unsigned int happy_tree_online_times;									// ����ʱ��
	long long today_chongzhi_gold_num;										// �����ֵ��ʯ����ȡ���ֹ�������ֵ
	int seven_day_login_fetch_reward_mark;									// �����ۼƵ�¼������ȡ���
	int offline_mojing;														// ����ħ��
	long long offline_exp;													// ���߾���(��)
	
	int collect_left_time_s;												// ���ֻʣ��ʱ��
	ExpBuffInfo exp_buff_info;												// ����ҩˮbuff��Ϣ
};

typedef char WelfareParamHex[sizeof(WelfareParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(WelfareParamHex) < 512);

#pragma pack(pop)

#endif // __WELFARE_DEF_HPP__

