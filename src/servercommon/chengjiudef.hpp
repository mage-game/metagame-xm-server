#ifndef __CHENGJIU_DEF_HPP__
#define __CHENGJIU_DEF_HPP__

#include "servercommon.h"

#pragma pack(push, 4)

enum CHENGJIU_CONDITION
{
	CJ_CONDITION_INVALID = 0,

	CJ_CONDITION_KILL_MONSTER,					// �ɹ���ɱnֻ����(��ͨ�֣�
	CJ_CONDITION_ADD_FRIEND,					// �ɹ����n������
	CJ_CONDITION_JOIN_TEAM,						// ��n�μ������
	CJ_CONDITION_COMPOSE,						// ��n�κϳ�
	CJ_CONDITION_JOIN_GUILD,					// ��n�μ��빫��
	CJ_CONDITION_EQUIP_STRENGTH,				// ��n��װ��ǿ��
	CJ_CONDITION_EQUIP_UPSTAR,					// ��n��װ������
	CJ_CONDITION_EQUIP_SHENZHU,					// ��n��װ������(����)
	CJ_CONDITION_EQUIP_UPLEVEL,					// ��n��װ������
	CJ_CONDITION_ROLE_LEVEL,					// ����ȼ���n��
	CJ_CONDITION_LOGIN_LAST_DAY,				// �ۼƵ�½��n��
	CJ_CONDITION_MONEY_GOLD_BIND,				// ����ʯ������
	CJ_CONDITION_KILL_BOSS,						// �ɹ���ɱnֻboss
	CJ_CONDITION_PUTON_EQUIP_STRENGTH,			// ����һ��ǿ��+x��װ��
	CJ_CONDITION_PUTON_EQUIP_STAR,				// ����һ��xxx�ǵ�װ��
	CJ_CONDITION_CHENGJIU_TITLE,				// �ɾʹﵽ��xxx��
	CJ_CONDITION_MOUNT_JIE,						// ����ﵽxx��
	CJ_CONDITION_SKILL_TOTAL_LEVEL,				// �����ܵȼ�
	CJ_CONDITION_XIANJIE_LEVEL,					// �ɽ׵ȼ�(����)
	CJ_CONDITION_WING_JIE_CHANGE,				// ����ﵽn��
	CJ_CONDITION_FABAO_LEVEL,					// �����ﵽxx��
	CJ_CONDITION_EQU_1_STAR_AND_COLOR,			// ������ɫһ�ǻ��ߺ�ɫһ��
	CJ_CONDITION_EQU_2_STAR_AND_COLOR,			// ������ɫ���ǻ��ߺ�ɫ����
	CJ_CONDITION_EQU_3_STAR_AND_COLOR,			// ������ɫ����
	CJ_CONDITION_EQU_JIEZHI,					// �״δ�����ָ
	CJ_CONDITION_YSZC_KILL_ROLE,				// Ԫ��ս����ɱn�����
	CJ_CONDITION_XLT_KILL_ROLE,					// �����������ɱn�����
	CJ_CONDITION_GCZ_WIN_TIMES,					// ����ս���n��ʤ��
	CJ_CONDITION_1V1_WIN_TIMES,					// ���1v1���n��ʤ��
	CJ_CONDITION_HUSONG_TIMES,					// ������n��
	CJ_CONDITION_EXP_FB_PASS_LEVEL,				// ���鸱���������
	CJ_CONDITION_PATA_PASS_LEVEL,				// ͨ������֮��xx��
	CJ_CONDITION_GUILD_FINISH_TASK,				// ���n����������
	CJ_CONDITION_GUILD_DONATION_EQU,			// �������n��װ��
	CJ_CONDITION_LTZ_SKILL_TA,					// ����ս�ݻ�xx��������
	CJ_CONDITION_SKILL_ROLE,					// �����ⳡ����ɱn�����
	CJ_CONDITION_SKILL_RED_NAME_ROLE,			// �����ⳡ����ɱn���������
	CJ_CONDITION_GUANG_HUAN_LEVEL,				// �⻷�ﵽxx��
	CJ_CONDITION_MO_QI_LEVEL,					// ħ��ﵽxx��
	CJ_CONDITION_ACTIVITE_MOJIE,				// ����ħ��xx
	CJ_CONDITION_RESERVE1,						// Ԥ��1
	CJ_CONDITION_RESERVE2,						// Ԥ��2
	CJ_CONDITION_RESERVE3,						// Ԥ��3
	CJ_CONDITION_ACTIVITE_NVSHEN,				// ����Ů��xx
	CJ_CONDITION_SHEN_GONG_LEVEL,				// �񹭴ﵽxx��
	CJ_CONDITION_SHEN_WING_LEVEL,				// ����ﵽxx��
	CJ_CONDITION_XUN_ZHANG_TOTLE_LEVEL,			// ѫ���ܵȼ���xxx��
	CJ_CONDITION_SJHJ_SHUI_JIN_COUNT,			// ˮ���þ��ɼ�xx��ˮ��
	CJ_CONDITION_SKILL_GUILD_BOSS,				// �����ɱnֻ����boss
	CJ_CONDITION_LIEQU_JINGLING_TOTAL,			// ��ȡxxֻ����
	CJ_CONDITION_ZHUANGBEI_JINGLING_LEVEL,		// װ��һֻxx���ľ���
	CJ_CONDITION_LIEQU_JINGLING,				// �ɹ���ȡ����xx
	CJ_CONDITION_NVSHEN_LEVEL,					// һλŮ�����ʵȼ�����xx��
	CJ_CONDITION_FOOTPRINT_LEVEL,				// �㼣�ﵽxx��

	CJ_CONDITION_MAX,
};

enum CHENGJIU_REWARD_STATUS
{
	CJ_REWARD_STATUS_CAN_NOT_FETCH = 0,			// ������ȡ
	CJ_REWARD_STATUS_CAN_FETCH,					// ����ȡ
	CJ_RWARD_STATUS_BE_FETCHED,					// ����ȡ
};

static const int CHENGJIU_REWARD_MAX_NUM = 400;					//�ɾͽ��������
static const int CHENGJIU_REWARD_MAX_CHANGE_NUM_INBATCH = 80;	//�ɾͽ��������ı������
UNSTD_STATIC_CHECK(CJ_CONDITION_MAX < CHENGJIU_REWARD_MAX_NUM);
UNSTD_STATIC_CHECK(CJ_CONDITION_MAX < CHENGJIU_REWARD_MAX_CHANGE_NUM_INBATCH);

struct ChengJiuRewardItem
{
	ChengJiuRewardItem()
	{
		this->Reset();
	}
	
	void Reset()
	{
		flag = 0;
		reserve_ch = 0;
		reserve_sh = 0;
		process = 0;
	}

	char  flag;
	char  reserve_ch;

	short reserve_sh;
	unsigned int process;
};

struct ChengJiuParam
{
	static const int CHENGJIU_SHUXINGDAN_MAX_TYPE = 3;

	ChengJiuParam()
	{
		this->Reset();
	}

	void Reset()
	{
		title_level = 0;
		reserve_sh1 = 0;

		fuwen_level = 0;
		process_times = 0;
		day_times = 0;

		for (int i = 0; i < CHENGJIU_REWARD_MAX_NUM; ++ i)
		{
			reward_list[i].Reset();
		}
		memset(shuxingdan_list, 0, sizeof(shuxingdan_list));
	}

	short title_level;
	short reserve_sh1;

	short fuwen_level;
	short process_times;
	int day_times;

	ChengJiuRewardItem reward_list[CHENGJIU_REWARD_MAX_NUM];

	int shuxingdan_list[CHENGJIU_SHUXINGDAN_MAX_TYPE];						//���Ե��б�
};

typedef char ChengJiuParamHex[sizeof(ChengJiuParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ChengJiuParamHex) < 8192);

#pragma pack(pop)
#endif
